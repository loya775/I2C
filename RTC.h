/*
* RTC.h
 *
 *  Created on: Apr 8, 2018
 *      Author: d
 */

#ifndef RTC_H_
#define RTC_H_

#include "DataTypeDefinitions.h"

/**
 * \brief This enum defines the valid formats for the hour
 */
typedef enum{FORMAT24,FORMAT12}Format_Type;

/**
 * \brief This enum defines the valid subformats for the 12 hour format
 */
typedef enum{AM,PM}AMPM_Type;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the seconds byte in the RTC. It has a value range of 0 to 59.
  	\param[in] This function doesn't receive any arguments
  	\return This function returns the seconds from the RTC in HEX format
 */
uint8 getSeconds_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the minutes byte in the RTC. It has a value range of 0 to 59.
  	\param[in] This function doesn't receive any arguments
  	\return This function returns the minutes from the RTC in HEX format
 */
uint8 getMinutes_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the hours byte in the RTC, it can return values for 12 hour format or 24 format.
  	 	 It has a value range of 1 to 12.
  	\param[in] This function doesn't receive any arguments
  	\return This function returns the hours from the RTC in HEX format
 */
uint8 getHours_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the days byte in the RTC. It has value range of 1 to 31.
  	\param[in] This function doesn't receive any arguments
  	\return This function returns the days from the RTC in HEX format
 */
uint8 getDays_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the months byte in the RTC. It has a value range of 1 to 12.
  	\param[in] This function doesn't receive any arguments
  	\return This function returns the months from the RTC in HEX format
 */
uint8 getMonths_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the years byte in the RTC. It has a value range of 1 to 99.
  	\param[in] This function doesn't receive any arguments
  	\return This function returns the years from the RTC in HEX format
 */
uint8 getYears_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function reads the hours byte in the RTC and determines if the hours are in a 12 hour format (AM or PM) or 24 hour format
  	\param[in] This function doesn't receive any arguments
  	\return This function returns AM, PM or 0 in HEX format to specify whether the hour read is in that format
 */
uint16 getAMPM_RTC(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function initializes the RTC oscillator
  	\param[in] This function receives the value needed to initialize the oscillator
  	\return This function doesn't have a return value
 */
void RTC_initialize(uint8);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function writes the time received in the RTC hour, minute, and seconds byte
  	\param[in] This function receives the hours, minutes and seconds to be set. It also receives the format chosen and a AM or PM format.
  	\return This function doesn't have a return value
 */
void setTime_RTC(uint8,uint8,uint8, Format_Type, AMPM_Type);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function writes the date received in the RTC year, month, and day byte
  	\param[in] This function receives the years, months, and days to be set.
  	\return This function doesn't have a return value
 */
void setDate_RTC(uint8,uint8,uint8);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function converts the current time into the format specified.
  	\param[in] This function receives the format which you want the time to be converted to.
  	\return This function doesn't have a return value
 */
void setHourFormat_RTC(Format_Type);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  	\brief
  	 	 This function waits until the oscillator has stopped by checking the OSCRUN bit in the weekday byte of the RTC
  	\param[in] This function doesn't have a parameters.
  	\return This function doesn't have a return value
 */
void checkOSCRUN_RTC(void);

#endif /* RTC_H_ */

