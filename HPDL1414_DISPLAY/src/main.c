/**
  ******************************************************************************
  * @file    main.c
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rtc.h"
#include "project_definitions.h"
#include "io_setup.h"
#include "uart.h"
#include "at_cmd.h"
#include "hpdl1414.h"
#include "epoch.h"
#include "rtc.h"


int main(void)
{
    /* DO NOT REMOVE WHILE DEVELOPING!!! ************************************************/
    /* This delay ENSURES that if a blocking piece of code happens after this,
     * this delay will always grant about 5 seconds to reprograme the device before the
     * blocking bug kicks in after a reset. */
    volatile uint32_t generic_use_value = 0;//10000000;
    while(0U!=(generic_use_value--)){asm("NOP;");}
    /* DO NOT REMOVE WHILE DEVELOPING!!! *************************************************/



    uint8_t lcd_time_text[] = "00:00:0000/00/2000";
    uint8_t lcd_user_text[108] = "        ";
    uint8_t lcd_user_text_len = 0;
    uint8_t *text_to_disp_p = lcd_time_text;
    bool rotate_text = false;
    date_time_t timeData = {0};
    uint32_t last_time = 0;
    uint32_t uart_read_amount = 100;
    at_cmd_type_t cmd_received = AT_TOTAL_CMDS;
    uint8_t uart_data[100];

    IoSetup();

    Uart1Init(UART_BAUDRATE);

    RtcInit(RTC_XTAL_FREQ); /* 1 second resolution */

	for(;;)
	{

	    generic_use_value = 24000;
	    while(0U!=(generic_use_value--))
	    {


	        /* Collect data from uart. */
	        uart_read_amount = 100;
            Uart1Peek(&uart_data[0], &uart_read_amount,0);

            if(0U != uart_read_amount)
            {
                /* New data received. Interpretate it. */
                uint32_t data_len = uart_read_amount;
                bool new_cmd = AtCmdDataIn(&uart_data[0], &data_len);

                uart_read_amount -= data_len;

                Uart1Read(&uart_data[0], &uart_read_amount);


                if(new_cmd)
                {
                    /* New command has been collected. Get it and process it. */
                    uart_read_amount = 100;
                    cmd_received = AtCmdGetCmd(uart_data, &uart_read_amount, 0);

                    if(AT_TOTAL_CMDS > cmd_received)
                    {
                        switch(cmd_received)
                        {
                            case AT_SHOW_TEXT:
                            {
                                lcd_user_text_len = 8U;
                                while(lcd_user_text_len--)
                                {
                                   lcd_user_text[lcd_user_text_len] = ' ';
                                }

                                lcd_user_text_len = uart_read_amount+8U;
                                while(uart_read_amount--)
                                {
                                    lcd_user_text[uart_read_amount+8U] = uart_data[uart_read_amount];
                                }

                                text_to_disp_p = lcd_user_text;

                                rotate_text = true;

                                uart_read_amount = 3;
                                Uart1Write("OK\n", &uart_read_amount);
                            }
                            break;

                            case AT_SHOW_DATE:
                            {
                                if(uart_data[0] == 0x30)
                                {
                                    lcd_user_text_len = 10;
                                    uart_read_amount = 6;
                                    while(uart_read_amount--)
                                    {
                                        lcd_user_text[uart_read_amount] = lcd_time_text[8U+uart_read_amount];
                                    }

                                    uart_read_amount = 2;
                                    while(uart_read_amount--)
                                    {
                                        lcd_user_text[uart_read_amount+6] = lcd_time_text[16U+uart_read_amount];
                                    }
                                    rotate_text = false;
                                }
                                else
                                {
                                    lcd_user_text_len = 8U;
                                    while(lcd_user_text_len--)
                                    {
                                       lcd_user_text[lcd_user_text_len] = ' ';
                                    }

                                    lcd_user_text_len = 10+8U;
                                    uart_read_amount = 10;
                                    while(uart_read_amount--)
                                    {
                                        lcd_user_text[uart_read_amount+8U] = lcd_time_text[8U+uart_read_amount];
                                    }

                                    rotate_text = true;
                                }

                                text_to_disp_p = lcd_user_text;

                                uart_read_amount = 3;
                                Uart1Write("OK\n", &uart_read_amount);
                            }
                            break;

                            case AT_SHOW_TIME:
                            {
                                if(text_to_disp_p != lcd_time_text)
                                {

                                    text_to_disp_p = lcd_time_text;
                                    rotate_text = false;
                                }

                                uart_read_amount = 3;
                                Uart1Write("OK\n", &uart_read_amount);
                            }
                            break;

                            case AT_GET_TIME:
                            {
                                uint8_t temp_data1[6] = "Time: ";
                                uart_read_amount = 6;
                                Uart1Write(temp_data1, &uart_read_amount);

                                uart_read_amount = 8;
                                Uart1Write(lcd_time_text, &uart_read_amount);

                                uint8_t temp_data2[21] = " Date:   /  /20  \nOK\n";

                                temp_data2[7] = 0x30 + timeData.day/10;
                                temp_data2[8] = 0x30 + timeData.day%10;

                                temp_data2[10] = 0x30 + timeData.month/10;
                                temp_data2[11] = 0x30 + timeData.month%10;

                                temp_data2[15] = 0x30 + timeData.year/10;
                                temp_data2[16] = 0x30 + timeData.year%10;

                                uart_read_amount = 20;
                                Uart1Write(temp_data2, &uart_read_amount);
                            }
                            break;

                            case AT_SET_TIME:
                            {

                                timeData.second = uart_data[13] - 0x30;
                                timeData.second += (uart_data[12] - 0x30)*10;

                                timeData.minute = uart_data[10] - 0x30;
                                timeData.minute += (uart_data[9] - 0x30)*10;

                                timeData.hour = uart_data[7] - 0x30;
                                timeData.hour += (uart_data[6] - 0x30)*10;

                                timeData.day = uart_data[22] - 0x30;
                                timeData.day += (uart_data[21] - 0x30)*10;

                                timeData.month = uart_data[25] - 0x30;
                                timeData.month += (uart_data[24] - 0x30)*10;

                                timeData.year = uart_data[30] - 0x30;
                                timeData.year += (uart_data[29] - 0x30)*10;

                                uint32_t epoch = date_time_to_epoch(&timeData);
                                RtcSet(&epoch);
                            }
                            break;


                            case AT_SET_LED:
                            {
                                if(uart_data[0] == 0x31)
                                {
                                    if(uart_data[2] == 0x30)
                                    {
                                        LED_WORK_SET_STATE(OFF);
                                    }
                                    else
                                    {
                                        LED_WORK_SET_STATE(ON);
                                    }
                                }
                                else
                                if(uart_data[0] == 0x30)
                                {
                                    if(uart_data[2] == 0x30)
                                    {
                                        LED_ERROR_SET_STATE(OFF);
                                    }
                                    else
                                    {
                                        LED_ERROR_SET_STATE(ON);
                                    }
                                }
                                else
                                {
                                    uart_read_amount = 4;
                                    Uart1Write("NOK\n", &uart_read_amount);
                                }


                            }
                            break;
                        }
                    }
                    else
                    {
                        uart_read_amount = 4;
                        Uart1Write("NOK\n", &uart_read_amount);
                    }

                    AtCmdDumpCmd();

                }
            }
	    }


	    /* Get current RTC time. */
	    RtcGet(&generic_use_value);

	    if(last_time != generic_use_value)
	    {

	        /* New time available. Print it to the time date display string. */
	        last_time = generic_use_value;
	        epoch_to_date_time(&timeData,last_time);

	        lcd_time_text[0] = 0x30+(timeData.hour/10);
	        lcd_time_text[1] = 0x30+(timeData.hour%10);

	        lcd_time_text[3] = 0x30+(timeData.minute/10);
	        lcd_time_text[4] = 0x30+(timeData.minute%10);

	        lcd_time_text[6] = 0x30+(timeData.second/10);
	        lcd_time_text[7] = 0x30+(timeData.second%10);

            lcd_time_text[8] = 0x30+(timeData.day/10);
            lcd_time_text[9] = 0x30+(timeData.day%10);

            lcd_time_text[11] = 0x30+(timeData.month/10);
            lcd_time_text[12] = 0x30+(timeData.month%10);

            lcd_time_text[16] = 0x30+(timeData.year/10);
            lcd_time_text[17] = 0x30+(timeData.year%10);

	    }

	    /* Exercises display user text string rotation. */
	    if(rotate_text)
	    {
	        uint32_t text_len = 0;
	        uint8_t last_char = lcd_user_text[0];
	        while(text_len < (lcd_user_text_len-1))
	        {
	            lcd_user_text[text_len] = lcd_user_text[text_len+1];
	            text_len++;
	        }

	        lcd_user_text[text_len] = last_char;
	    }

	    Hdpl1414Write(text_to_disp_p, 8, 0);
	}
}
