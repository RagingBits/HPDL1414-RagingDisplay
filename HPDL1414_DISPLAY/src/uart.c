/**
  ******************************************************************************
  * @file    uart.c
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stdbool.h"
#include "core_cm3.h"
#include "uart.h"


typedef void(*uart1_callback_t)(void);

//#define UART1_TX_BUFF_HALF_LEN (UART1_TX_BUFF_LEN/2)

#define UART1_DMA_TX_CHANNEL 	DMA1_Channel4
#define UART1_DMA_RX_CHANNEL 	DMA1_Channel5
#define UART1_DMA_TX_IRQ		DMA1_Channel4_IRQn
#define UART1_DMA_RX_IRQ		DMA1_Channel5_IRQn

#define UART1_DMA_TX_FLAG		DMA1_FLAG_TC4
#define UART1_DMA_RX_FLAG		DMA1_FLAG_HT5

uint8_t uart1_rx_buffer[UART1_RX_BUFF_LEN];
uint32_t uart1_rx_buffer_p = 0;
uint8_t uart1_tx_buffer[2][UART1_TX_BUFF_LEN*2];
uint32_t uart1_tx_buffer_len[2] = {0,0};
uint8_t uart1_tx_in_use_p = 0;
uint8_t uart1_tx_running = 0;
uart1_callback_t uart1_end_callback=0;
uint32_t uart1_nvic_val = 0;


void _Uart1Memcpy (uint8_t *dest, uint8_t *org, uint32_t len)
{
    while(len--)
    {
        *dest++ = *org++;
    }
}


static uint32_t uart1_criticalcounter = 0;

void _Uart1CriticalStart(void)
{
    if(0 == uart1_criticalcounter)
    {
        //__asm volatile ("cpsid i;");
    }
    else
    if(uart1_criticalcounter<0xffffffff)
    {
        uart1_criticalcounter++;
    }
}

void _Uart1CriticalEnd(void)
{
    if(1 == uart1_criticalcounter)
    {
        //__asm volatile ("cpsie i;");
    }

    if(0 != uart1_criticalcounter)
    {
        uart1_criticalcounter--;
    }
}

void _Uart1NvicConfig(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART1_DMA_TX_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the DMA Interrupt *//*
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
void _Uart1DmaConfig(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}


uint32_t Uart1InputFill(void)
{
    _Uart1CriticalStart ();
    uint32_t current_point = (UART1_RX_BUFF_LEN - DMA_GetCurrDataCounter(UART1_DMA_RX_CHANNEL));
    uint32_t len = 0;
    if(uart1_rx_buffer_p > current_point)
    {
        len = (UART1_RX_BUFF_LEN - (uart1_rx_buffer_p-current_point));
    }
    else
    {
        len = current_point - uart1_rx_buffer_p;
    }
    _Uart1CriticalEnd ();
    return len;
}


void Uart1Read(uint8_t *data, uint32_t *data_len)
{
    _Uart1CriticalStart ();
    uint32_t counter = Uart1InputFill();
    uint32_t count = counter;
    if((0 != counter)&&(0 != (*data_len)))
    {
        if(counter > (*data_len))
        {
            counter = (*data_len);
            count = counter;
        }

        while(counter)
        {
            counter--;
            *data++ = uart1_rx_buffer[uart1_rx_buffer_p];
            uart1_rx_buffer_p++;

            if(uart1_rx_buffer_p >= UART1_RX_BUFF_LEN)
            {
                uart1_rx_buffer_p = 0;
            }

        }
    }

    _Uart1CriticalEnd ();
    *data_len = count;
}

void Uart1Peek(uint8_t *data, uint32_t *data_len, uint32_t offset)
{
    _Uart1CriticalStart ();
    uint32_t counter = Uart1InputFill();
    uint32_t count = counter;
    uint32_t mock_p = uart1_rx_buffer_p;

    if((0 != counter)&&(0 != (*data_len))&&(counter>offset))
    {
        /* Advance to offset. */
        while(offset)
        {
            offset--;
            mock_p++;
            if(mock_p >= UART1_RX_BUFF_LEN)
            {
                mock_p = 0;
            }
            counter--;
        }

        if(counter > (*data_len))
        {
            counter = (*data_len);
            count = counter;
        }

        while(counter)
        {
            counter--;
            *data++ = uart1_rx_buffer[mock_p];
            mock_p++;

            if(mock_p >= UART1_RX_BUFF_LEN)
            {
                mock_p = 0;
            }

        }
    }

    _Uart1CriticalEnd ();
    *data_len = count;
}

void _Uart1StartRx(uint8_t *data, uint32_t data_len, uart1_callback_t callback)
{
  uart1_end_callback = callback;

  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(UART1_DMA_RX_CHANNEL);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)data;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_BufferSize = data_len;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(UART1_DMA_RX_CHANNEL, &DMA_InitStructure);
  DMA_Cmd(UART1_DMA_RX_CHANNEL, ENABLE);
  //DMA_ITConfig(UART1_DMA_RX_CHANNEL,DMA_IT_HT,ENABLE);
}


void _Uart1StartTx(uint8_t *data, uint32_t data_len, uart1_callback_t callback)
{
  uart1_end_callback = callback;

  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(UART1_DMA_TX_CHANNEL);

  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)data;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_BufferSize = data_len;

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(UART1_DMA_TX_CHANNEL, &DMA_InitStructure);

  DMA_Cmd(UART1_DMA_TX_CHANNEL, ENABLE);
  DMA_ITConfig(UART1_DMA_TX_CHANNEL,DMA_IT_TC,ENABLE);
}




void Uart1Write(const uint8_t *data, uint32_t *data_len)
{
    /* critical in */
    if(0U != *data_len)
    {
        _Uart1CriticalStart();

        if((*data_len) < (UART1_TX_BUFF_LEN-uart1_tx_buffer_len[uart1_tx_in_use_p]))
        {
            _Uart1Memcpy(&uart1_tx_buffer[uart1_tx_in_use_p][uart1_tx_buffer_len[uart1_tx_in_use_p]],(uint8_t*)data,*data_len);

            uart1_tx_buffer_len[uart1_tx_in_use_p] += (*data_len);


            if(0 == uart1_tx_running)
            {
                uart1_tx_in_use_p = (1-uart1_tx_in_use_p);
                uart1_tx_running = 1;
                _Uart1StartTx(uart1_tx_buffer[1-uart1_tx_in_use_p], uart1_tx_buffer_len[1-uart1_tx_in_use_p], 0);

            }
        }
        else
        {
            *data_len = 0;
        }

        _Uart1CriticalEnd();
    }

}

uint32_t Uart1OutputFill(void)
{
    uint32_t val = 0;
    _Uart1CriticalStart ();
    val = uart1_tx_buffer_len[uart1_tx_in_use_p];
    _Uart1CriticalEnd ();
    return val;
}

uint32_t Uart1OutputFree(void)
{
    uint32_t val = 0;
    _Uart1CriticalStart ();
    val = (UART1_TX_BUFF_LEN-uart1_tx_buffer_len[uart1_tx_in_use_p]);
    _Uart1CriticalEnd ();
    return val;
}



void Uart1ISR(void)
{
    if(DMA_GetITStatus(UART1_DMA_TX_FLAG))
    {
        DMA_ClearITPendingBit(UART1_DMA_TX_FLAG);

        uart1_tx_buffer_len[1-uart1_tx_in_use_p] = 0;

        if(uart1_tx_buffer_len[uart1_tx_in_use_p])
        {
            uart1_tx_in_use_p = (1-uart1_tx_in_use_p);
            _Uart1StartTx(uart1_tx_buffer[1-uart1_tx_in_use_p], uart1_tx_buffer_len[1-uart1_tx_in_use_p], 0);

        }
        else
        {
            if(0 != uart1_end_callback)
            {
                uart1_end_callback();
            }
            uart1_tx_running = 0;
            DMA_ITConfig(UART1_DMA_TX_CHANNEL,DMA_IT_TC,DISABLE);
        }
    }

    if(DMA_GetITStatus(UART1_DMA_RX_FLAG))
    {
        DMA_ClearITPendingBit(UART1_DMA_RX_FLAG);
    }


    //DMA_ClearITPendingBit(0xFFFFFFFF);
}


void _Uart1GpioConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

   /* Enable the USART1 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure USART1 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}


void _Uart1Config(uint32_t speed)
{

  USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitStructure.USART_BaudRate = speed;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_2;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USARTy */

  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
  //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  USART_Init(USART1, &USART_InitStructure);
  /* Enable USARTy */
  USART_Cmd(USART1, ENABLE);
}

void Uart1Pause(bool val)
{
    if(val)
    {
        USART_Cmd(USART1, DISABLE);
    }
    else
    {
        USART_Cmd(USART1, ENABLE);
    }
}

void Uart1Init(uint32_t speed)
{

    _Uart1GpioConfig();
    _Uart1DmaConfig();
    _Uart1Config(speed);
    _Uart1NvicConfig();
    _Uart1StartRx(uart1_rx_buffer, UART1_RX_BUFF_LEN, 0);
}



void Uart1TxClear(void)
{
  uart1_tx_buffer_len[0] = 0;
  uart1_tx_buffer_len[1] = 0;
  uart1_tx_in_use_p = 0;
  uart1_tx_running = 0;
  DMA_Cmd(UART1_DMA_TX_CHANNEL, DISABLE);
  DMA_ITConfig(UART1_DMA_TX_CHANNEL,DMA_IT_TC,DISABLE);
}

void Uart1RxClear(void)
{
    uart1_rx_buffer_p = (UART1_RX_BUFF_LEN-DMA_GetCurrDataCounter(UART1_DMA_RX_CHANNEL));
}



