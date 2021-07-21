/**
  ******************************************************************************
  * @file    rtc.c
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "project_definitions.h"
#include "rtc.h"





rtc_result_t RtcStop(void)
{
    rtc_result_t result = RTC_TIMER_SUCCESS;

    if(!RCC_GetFlagStatus(RCC_FLAG_LSERDY))
    {
        result = RTC_TIMER_NOT_RUNNING;
    }

    if(result == RTC_TIMER_SUCCESS)
    {
        /* Enable PWR and BKP clocks */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

        /* Allow access to BKP Domain */
        PWR_BackupAccessCmd(ENABLE);

        BKP_DeInit();

        /* Disable the alarm the interrupt. */
        RTC_ITConfig(RTC_IT_ALR,DISABLE);

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);
    }

    return result;
}

rtc_result_t RtcGet(uint32_t *current_count)
{
    rtc_result_t result = RTC_TIMER_SUCCESS;

    if(0 == current_count)
    {
        result = RTC_TIMER_ERROR;
    }
    else
    if(!RCC_GetFlagStatus(RCC_FLAG_LSERDY))
    {
        result = RTC_TIMER_NOT_RUNNING;
    }

    if(RTC_TIMER_SUCCESS == result)
    {
        RTC_WaitForLastTask();
        *current_count = RTC_GetCounter();
    }

    return result;
}


rtc_result_t RtcSet(uint32_t *new_count)
{
    rtc_result_t result = RTC_TIMER_SUCCESS;

    if(0 == new_count)
    {
        result = RTC_TIMER_ERROR;
    }
    else
    if(!RCC_GetFlagStatus(RCC_FLAG_LSERDY))
    {
        result = RTC_TIMER_NOT_RUNNING;
    }

    if(RTC_TIMER_SUCCESS == result)
    {
        RTC_WaitForLastTask();
        RTC_SetCounter(*new_count);
        RTC_WaitForLastTask();
    }

    return result;
}

rtc_result_t RtcInit(uint32_t res)
{
    rtc_result_t result = RTC_TIMER_SUCCESS;

    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* RTC clock source configuration ----------------------------------------*/
    /* Enable access to Backup Domain */
    PWR_BackupAccessCmd(ENABLE);

    if(RCC_GetFlagStatus(RCC_FLAG_LSERDY))
    {
        result = RTC_TIMER_ALREADY_RUNNING;

    }
    else
    {

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

        BKP_DeInit();
        RTC_ITConfig(RTC_IT_ALR,DISABLE);

        /* Allow access to BKP Domain */
        PWR_BackupAccessCmd(ENABLE);




        RCC_LSICmd(DISABLE);

        /* Enable LSE OSC */
        RCC_LSEConfig(RCC_LSE_ON);

        /* Wait till LSE is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
        }

        /* Select the RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        /* Enable the RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* RTC configuration -----------------------------------------------------*/
        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        /* Set the RTC time base to the given one. */
        RTC_WaitForLastTask();
        RTC_SetPrescaler(res);
        RTC_WaitForLastTask();
        RTC_SetCounter(0);
        RTC_WaitForLastTask();
    }

    return result;
}


