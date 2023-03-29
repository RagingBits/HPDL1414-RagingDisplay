/**
  ******************************************************************************
  * @file    rtc.h
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#ifndef RTC_H_
#define RTC_H_

typedef enum
{
    RTC_TIMER_SUCCESS,
    RTC_TIMER_ALREADY_RUNNING,
    RTC_TIMER_NOT_RUNNING,
    RTC_TIMER_ERROR
}rtc_result_t;


rtc_result_t RtcStop(void);

rtc_result_t RtcGet(uint32_t *current_count);

rtc_result_t RtcSet(uint32_t *new_count);

rtc_result_t RtcInit(uint32_t res);


#endif /* RTC_H_ */
