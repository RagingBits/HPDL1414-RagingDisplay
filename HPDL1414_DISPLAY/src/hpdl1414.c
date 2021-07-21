/**
  ******************************************************************************
  * @file    hpdl1414.c
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
#include "hpdl1414.h"
#include "project_definitions.h"



#define HDPL1414_0_WRITE_DISABLE (1<<9)
#define HDPL1414_1_WRITE_DISABLE (1<<10)


bool Hdpl1414Write(uint8_t *text, uint8_t text_len, uint8_t offset)
{
    bool success = true;
    uint8_t char_index = offset;
    uint8_t *char_p = text;

    if(NULL == char_p)
    {
        success = false;
    }
    else
    if(0U == text_len)
    {
        success = false;
    }
    else
    if(8U < (text_len+offset))
    {
        success = false;
    }
    else
    {
        uint8_t char_index_ = 0;
        uint8_t character = 0U;
        uint16_t value = 0;
        uint16_t hdpl_select = (2U-(char_index_>>2))<<9U;

        while(char_index<(text_len+offset))
        {
            char_index_ = 7U-(char_index++);

            hdpl_select = (char_index_&0x04)<<7U;
            hdpl_select += HDPL1414_0_WRITE_DISABLE;
            value = char_index_&0x03;

            value <<= 7U;
            value |= (HDPL1414_0_WRITE_DISABLE+HDPL1414_1_WRITE_DISABLE);

            character = *char_p++;
            value += (0x7F&character);

            GPIO_Write(GPIOA, value);

            value &= ~hdpl_select;
            GPIO_Write(GPIOA, value);
            value |= (HDPL1414_0_WRITE_DISABLE+HDPL1414_1_WRITE_DISABLE);
            GPIO_Write(GPIOA, value);
        }




    }

    return success;
}
