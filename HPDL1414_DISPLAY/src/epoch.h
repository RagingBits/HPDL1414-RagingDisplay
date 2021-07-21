/**
  ******************************************************************************
  * @file    epoch.h
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#ifndef EPOCH_H_
#define EPOCH_H_

typedef struct
{
    uint8_t second; // 0-59
    uint8_t minute; // 0-59
    uint8_t hour;   // 0-23
    uint8_t day;    // 1-31
    uint8_t month;  // 1-12
    uint8_t year;   // 0-99
}
date_time_t;


void epoch_to_date_time(date_time_t* date_time,uint32_t epoch);

uint32_t date_time_to_epoch(date_time_t* date_time);

#endif /* EPOCH_H_ */
