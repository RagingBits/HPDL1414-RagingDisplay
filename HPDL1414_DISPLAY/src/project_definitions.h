/**
  ******************************************************************************
  * @file    project_definitions.h
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#ifndef PROJECT_DEFINITIONS_H_
#define PROJECT_DEFINITIONS_H_

#ifndef NULL
#define NULL    ((void*)0U)
#endif

#define UART_RX_PIN         GPIO_Pin_7
#define UART_RX_PORT        GPIOB
#define UART_TX_PIN         GPIO_Pin_6
#define UART_TX_PORT        GPIOB

#define LED_ERR_PIN         GPIO_Pin_0
#define LED_ERR_PORT        GPIOB
#define LED_WORK_PIN        GPIO_Pin_1
#define LED_WORK_PORT       GPIOB

#define HDPL1414_D0_PIN     GPIO_Pin_0
#define HDPL1414_D0_PORT    GPIOA
#define HDPL1414_D1_PIN     GPIO_Pin_1
#define HDPL1414_D1_PORT    GPIOA
#define HDPL1414_D2_PIN     GPIO_Pin_2
#define HDPL1414_D2_PORT    GPIOA
#define HDPL1414_D3_PIN     GPIO_Pin_3
#define HDPL1414_D3_PORT    GPIOA
#define HDPL1414_D4_PIN     GPIO_Pin_4
#define HDPL1414_D4_PORT    GPIOA
#define HDPL1414_D5_PIN     GPIO_Pin_5
#define HDPL1414_D5_PORT    GPIOA
#define HDPL1414_D6_PIN     GPIO_Pin_6
#define HDPL1414_D6_PORT    GPIOA
#define HDPL1414_A0_PIN     GPIO_Pin_7
#define HDPL1414_A0_PORT    GPIOA
#define HDPL1414_A1_PIN     GPIO_Pin_8
#define HDPL1414_A1_PORT    GPIOA
#define HDPL1414_0_WR_PIN   GPIO_Pin_9
#define HDPL1414_0_WR_PORT  GPIOA
#define HDPL1414_1_WR_PIN   GPIO_Pin_10
#define HDPL1414_1_WR_PORT  GPIOA


#define OFF                             ((uint8_t)0x00)
#define ON                              (!OFF)
#define LED_WORK_SET_STATE(NEW_STATE)   GPIO_WriteBit(LED_WORK_PORT, LED_WORK_PIN, ((NEW_STATE == OFF)?Bit_SET:Bit_RESET))
#define LED_ERROR_SET_STATE(NEW_STATE)  GPIO_WriteBit(LED_ERR_PORT, LED_ERR_PIN, ((NEW_STATE == OFF)?Bit_SET:Bit_RESET))

#define RTC_XTAL_FREQ 32767U
#define SYS_FREQ 8000000U

#define UART_BAUDRATE 115200
//#define UART_BAUDRATE 115200




#endif /* PROJECT_DEFINITIONS_H_ */
