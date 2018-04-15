/*
 * RTC.h
 *
 *  Created on: Apr 8, 2018
 *      Author: d
 */

#ifndef RTC_H_
#define RTC_H_

#include "DataTypeDefinitions.h"

typedef enum{FORMAT24,FORMAT12}Format_Type;

typedef enum{AM,PM}AMPM_Type;

uint8 getSeconds_RTC(void);

uint8 getMinutes_RTC(void);

uint8 getHours_RTC(void);

uint8 getDays_RTC(void);

uint8 getMonths_RTC(void);

uint8 getYears_RTC(void);

uint16 getAMPM_RTC(void);

void RTC_initialize(uint8);

void setTime_RTC(uint8,uint8,uint8, Format_Type, AMPM_Type);

void setDate_RTC(uint8,uint8,uint8);

void setHourFormat_RTC(Format_Type);

void checkOSCRUN_RTC(void);

#endif /* RTC_H_ */
