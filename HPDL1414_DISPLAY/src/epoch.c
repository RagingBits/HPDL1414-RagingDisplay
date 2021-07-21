/**
  ******************************************************************************
  * @file    epoch.c
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include <stdbool.h>
#include "epoch.h"


const uint16_t days[4][12] =
{
    {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
    { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
    { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
    {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
};


uint32_t date_time_to_epoch(date_time_t* date_time)
{
    uint8_t second = date_time->second;  // 0-59
    uint8_t minute = date_time->minute;  // 0-59
    uint8_t hour   = date_time->hour;    // 0-23
    uint8_t day    = date_time->day-1;   // 0-30
    uint8_t month  = date_time->month-1; // 0-11
    uint8_t year   = date_time->year;    // 0-99

    uint32_t result = (((year/4*(365*4+1)+days[year%4][month]+day)*24+hour)*60+minute)*60+second;

    return result;
}


void epoch_to_date_time(date_time_t* date_time,uint32_t epoch)
{
    date_time->second = epoch%60; epoch /= 60;
    date_time->minute = epoch%60; epoch /= 60;
    date_time->hour   = epoch%24; epoch /= 24;

    uint32_t years = epoch/(365*4+1)*4; epoch %= 365*4+1;

    uint8_t year;
    for (year=3; year>0; year--)
    {
        if (epoch >= days[year][0])
            break;
    }

    uint8_t month;
    for (month=11; month>0; month--)
    {
        if (epoch >= days[year][month])
            break;
    }

    date_time->year  = years+year;
    date_time->month = month+1;
    date_time->day   = epoch-days[year][month]+1;
}



