/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    RTC.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "I2C_Driver.h"
#include "DataTypeDefinitions.h"
#include "RTC.h"
#include "GlobalFunctions.h"
#include "MK64F12.h"

/*These macros are used to make shifts, to multiply variables and set delay values*/
#define TENS_SHIFT 4
#define HOUR_SHIFT 6
#define AMPM_SHIFT 5
#define TWELVEHOUR 12
#define CONVERSIONOFFSET 14
#define DELAYI2C 100
#define DELAYSTART 5000
#define TENS 10

/*These macros are used as masks to get individual or several bits from RTC*/
#define ONES_MASK 0x0F
#define TENS_MASK 0xF0
#define ONE_TEN_MASK 0x30
#define TWELVE_MASK 0x10
#define HOUR_FORMAT_MASK 0x40
#define PM_MASK 0x20
#define OSCRUN_MASK 0x20
#define HIGH_MASK 0x70

/*These macros are used to access specific bytes from the RTC*/
#define CTRL_BYTE 0x07
#define YEAR_BYTE 0x06
#define MTH_BYTE 0x05
#define DAY_BYTE 0x04
#define WK_BYTE 0x03
#define HOUR_BYTE 0x02
#define MIN_BYTE 0x01
#define SEC_BYTE 0x00

/*These macros are used to read and write in the RTC*/
#define READRTC 0xDF
#define WRITERTC 0XDE

/*dataRTC is the data read from the RTC*/
static volatile uint8 dataRTC;
/*ten contains the tens in a read value*/
static volatile uint8 ten;
/*one contains the ones in a read value*/
static volatile uint8 one;
/*ampm contains whether the 12 hour format is in AM or PM*/
static volatile uint8 ampm;
/*setHourResult contains the hour to be set in the RTC*/
static volatile uint8 setHourResult;
/*ampmChar contains the 'AM' or 'PM' chars in HEX format*/
static volatile uint16 ampmChar;


void RTC_initialize(uint8 variable){
	/*This function initiliazes the RTC by writing in the ST in the seconds byte
	 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
	 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	I2C_TX_RX_Mode(TRUE, I2C_0);

	I2C_start(I2C_0);
	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(SEC_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(variable, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));
}

uint8 getSeconds_RTC(void){
	/*This function reads the RTC seconds by writing the byte to be read and then using a repeated start and
	 * specifying you are reading the byte previously specified and finally converting that value into a DEC format
	 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
	 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	delay(DELAYSTART);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(SEC_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & HIGH_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getMinutes_RTC(void){
	/*This function reads the RTC minutes by writing the byte to be read and then using a repeated start and
	 * specifying you are reading the byte previously specified and finally converting that value into a DEC format
	 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
	 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	delay(DELAYSTART);
	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(MIN_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & HIGH_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getHours_RTC(void){
	/*This function reads the RTC hours by writing the byte to be read and then using a repeated start and
	 * specifying you are reading the byte previously specified.
	 * Then it uses a mask to check if the 12/24 format bit is on or off, and finally converts the value
	 * whether it's a 12 hour format or a 24 hour format
	 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
	 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	delay(DELAYSTART);
	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	if(FALSE == ((dataRTC & HOUR_FORMAT_MASK)) >> HOUR_SHIFT){
		one = dataRTC & ONES_MASK;
		ten = (dataRTC & ONE_TEN_MASK) >> TENS_SHIFT;
	}
	else if(TRUE == ((dataRTC & HOUR_FORMAT_MASK)) >> HOUR_SHIFT){
		one = dataRTC & ONES_MASK;
		ten = (dataRTC & TWELVE_MASK) >> TENS_SHIFT;
	}

	return (ten * TENS)+one;
}

uint8 getDays_RTC(void){
	/*This function reads the RTC days by writing the byte to be read and then using a repeated start and
		 * specifying you are reading the byte previously specified and finally converting that value into a DEC format
		 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
		 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	I2C_TX_RX_Mode(TRUE,I2C_0);
	delay(DELAYSTART);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(DAY_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & ONE_TEN_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getMonths_RTC(void){
	/*This function reads the RTC months by writing the byte to be read and then using a repeated start and
		 * specifying you are reading the byte previously specified and finally converting that value into a DEC format
		 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
		 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	I2C_TX_RX_Mode(TRUE, I2C_0);
	delay(DELAYSTART);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(MTH_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & TWELVE_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getYears_RTC(void){
	/*This function reads the RTC years by writing the byte to be read and then using a repeated start and
		 * specifying you are reading the byte previously specified and finally converting that value into a DEC format
		 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
		 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	I2C_TX_RX_Mode(TRUE, I2C_0);
	delay(DELAYSTART);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(YEAR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE,I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & TENS_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint16 getAMPM_RTC(void){
	/*This function reads the RTC hours by writing the byte to be read and then using a repeated start and
		 * specifying you are reading the byte previously specified and then applies a mask to verify if
		 * the hour read is in 24/12 format and then a applies another mask if it has a 12 hour format to see
		 * and finally it returns the chars for AM or PM.
		 * Before that it has to be in TX mode and it has to specify the I2C to be used as a slave.
		 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/
	delay(DELAYSTART);
	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	if(FALSE == ((dataRTC & HOUR_FORMAT_MASK)) >> HOUR_SHIFT){
			ampmChar = FALSE;
	}
	else if(TRUE == ((dataRTC & HOUR_FORMAT_MASK)) >> HOUR_SHIFT){
		if(AM == ((dataRTC & PM_MASK)) >> AMPM_SHIFT){
			ampmChar = 0x616D;
		}
		else if(PM == ((dataRTC & PM_MASK)) >> AMPM_SHIFT){
			ampmChar = 0x706D;
		}
	}

	return ampmChar;

}

void setTime_RTC(uint8 hour,uint8 minute,uint8 sec, Format_Type format, AMPM_Type ampm){
	/*This function writes the RTC time by accessing the hours, minutes and seconds byte as specified before
	 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/

	one = sec & ONES_MASK;
	ten = sec & TENS_MASK;

	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(SEC_BYTE,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(ten | one,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	one = minute & ONES_MASK;
	ten = minute & HIGH_MASK;

	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(MIN_BYTE,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(ten | one,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	if(FORMAT24 == format){
		one = hour & ONES_MASK;
		ten = hour & ONE_TEN_MASK;
	}
	else if(FORMAT12 == format){
		one = hour & ONES_MASK;
		if(PM == ampm){
			ten = HOUR_FORMAT_MASK | PM_MASK | (hour & TWELVE_MASK);
		}
		else if (AM == ampm){
			ten = HOUR_FORMAT_MASK | (hour & TWELVE_MASK);
		}
	}

	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(ten | one,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

}


void setDate_RTC(uint8 year,uint8 month,uint8 day){

	/*This function writes the RTC time by accessing the years, months and days byte as specified before
	 * Once it has done all its processing, it checks if the oscillator is running and wait until it's stopped*/

	one = day & ONES_MASK;
	ten = day & ONE_TEN_MASK;

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(DAY_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(ten | one, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	one = month & ONES_MASK;
	ten = month & TWELVE_MASK;

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(MTH_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(ten | one, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	one = year & ONES_MASK;
	ten = year & TENS_MASK;

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(YEAR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(ten | one, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

}

void checkOSCRUN_RTC(void){
	/*This function checks if the oscillator is still running by checking the OSCRUN bit
	 * in the RTC weekday byte, and it also stops the I2C*/

	I2C_repeted_Start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WK_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
}

void setHourFormat_RTC(Format_Type format){

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(DELAYI2C);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	if(FORMAT24 == ((dataRTC & HOUR_FORMAT_MASK) >> HOUR_SHIFT)){
		one = (dataRTC & ONES_MASK);
		ten = (dataRTC & ONE_TEN_MASK) >> TENS_SHIFT;

		if(ten*TENS+one > TWELVEHOUR){
			setHourResult = (ten * TENS + one) % TWELVEHOUR;
			ampm = PM;
		}
		else{
			setHourResult = ten * TENS + one;
			ampm = AM;
		}
	}
	else if(FORMAT12 == ((dataRTC & HOUR_FORMAT_MASK) >> HOUR_SHIFT)){
		ampm = (dataRTC & PM_MASK) >> AMPM_SHIFT;
		if(AM == ampm){
			one = (dataRTC & ONES_MASK);
			ten = (dataRTC & TWELVE_MASK) >> TENS_SHIFT;
			setHourResult = (ten * TENS + one);
		}
		else if (PM == ampm){
			one = (dataRTC & ONES_MASK);
			ten = (dataRTC & TWELVE_MASK) >> TENS_SHIFT;
			setHourResult = (ten * TENS + one) + CONVERSIONOFFSET;
		}
	}

	ten = (setHourResult / TWELVEHOUR) << TENS_SHIFT;
	one = (setHourResult % TWELVEHOUR);

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	if(FORMAT24 == format){
		I2C_write_Byte(ten | one, I2C_0);
	}
	else if (FORMAT12 == format){
		if(AM == ampm){
			I2C_write_Byte(HOUR_FORMAT_MASK | ten | one, I2C_0);
		}
		else if(PM == ampm){
			I2C_write_Byte(HOUR_FORMAT_MASK | PM_MASK | ten | one, I2C_0);
		}
	}
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(DELAYI2C);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

}
