/**
  ******************************************************************************
  * @file    io_setup.c
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#include "project_definitions.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void IoSetup(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Pin = LED_ERR_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(LED_ERR_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_ERR_PORT, LED_ERR_PIN, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = LED_WORK_PIN ;
    GPIO_Init(LED_WORK_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_WORK_PORT, LED_WORK_PIN, Bit_SET);


    GPIO_InitStructure.GPIO_Pin = HDPL1414_D0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D0_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D0_PORT, HDPL1414_D0_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_D1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D1_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D1_PORT, HDPL1414_D1_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_D2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D2_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D2_PORT, HDPL1414_D2_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_D3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D3_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D3_PORT, HDPL1414_D3_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_D4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D4_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D4_PORT, HDPL1414_D4_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_D5_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D5_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D5_PORT, HDPL1414_D5_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_D6_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_D6_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_D6_PORT, HDPL1414_D6_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_A0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_A0_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_A0_PORT, HDPL1414_A0_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_A1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_A1_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_A1_PORT, HDPL1414_A1_PIN, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_0_WR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_0_WR_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_0_WR_PORT, HDPL1414_0_WR_PIN, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = HDPL1414_1_WR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HDPL1414_1_WR_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(HDPL1414_1_WR_PORT, HDPL1414_1_WR_PIN, Bit_SET);

}
