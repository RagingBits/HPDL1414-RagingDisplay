/**
  ******************************************************************************
  * @file    uart.h
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#ifndef UART_H_
#define UART_H_

#define UART1_TX_BUFF_LEN (128)

#define UART1_RX_BUFF_LEN (521)


void Uart1Init(uint32_t speed);

void Uart1Write(const uint8_t *data, uint32_t *data_len);

uint32_t Uart1InputFill(void);

void Uart1Read(uint8_t *data, uint32_t *data_len);

void Uart1Peek(uint8_t *data, uint32_t *data_len, uint32_t offset);

uint32_t Uart1OutputFill(void);

uint32_t Uart1OutputFree(void);

void Uart1ISR(void);

void Uart1TxClear(void);

void Uart1RxClear(void);

void Uart1Pause(bool val);


#endif /* UART_H_ */
