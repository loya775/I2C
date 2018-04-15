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

#define ONES_MASK 0x0F
#define TENS_MASK 0xF0
#define ONE_TEN_MASK 0x30
#define TWELVE_MASK 0x10
#define TENS_SHIFT 4
#define HOUR_SHIFT 6
#define AMPM_SHIFT 5
#define TENS 10
#define HOUR_FORMAT_MASK 0x40
#define PM_MASK 0x20
#define OSCRUN_MASK 0x20
#define HIGH_MASK 0x70

#define CTRL_BYTE 0x07
#define YEAR_BYTE 0x06
#define MTH_BYTE 0x05
#define DAY_BYTE 0x04
#define WK_BYTE 0x03
#define HOUR_BYTE 0x02
#define MIN_BYTE 0x01
#define SEC_BYTE 0x00
#define READRTC 0xDF
#define WRITERTC 0XDE

static volatile uint8 dataRTC;
static volatile uint8 ten;
static volatile uint8 one;
static volatile uint8 ampm;
static volatile uint8 setHourResult;
static volatile uint16 ampmChar;
static volatile uint8 lastHour;
static uint8 formatFlag;

void RTC_initialize(uint8 variable){
	I2C_TX_RX_Mode(TRUE, I2C_0);

	I2C_start(I2C_0);
	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(SEC_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(variable, I2C_0); //activar el oscilador 0x10000000
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));
}

uint8 getSeconds_RTC(void){
	delay(5000);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(SEC_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & HIGH_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getMinutes_RTC(void){
	delay(5000);
	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(MIN_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & HIGH_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getHours_RTC(void){
	delay(5100);
	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

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
	I2C_TX_RX_Mode(TRUE,I2C_0);
	delay(5000);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(DAY_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & ONE_TEN_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getMonths_RTC(void){
	I2C_TX_RX_Mode(TRUE, I2C_0);
	delay(5000);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(MTH_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & TWELVE_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint8 getYears_RTC(void){
	I2C_TX_RX_Mode(TRUE, I2C_0);
	delay(5000);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(YEAR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE,I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	one = dataRTC & ONES_MASK;
	ten = (dataRTC & TENS_MASK) >> TENS_SHIFT;

	return (ten*TENS)+one;
}

uint16 getAMPM_RTC(void){
	delay(5100);
	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(100);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(100);

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

	one = sec & ONES_MASK;
	ten = sec & TENS_MASK;

	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(SEC_BYTE,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(ten | one,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	one = minute & ONES_MASK;
	ten = minute & HIGH_MASK;

	I2C_TX_RX_Mode(TRUE,I2C_0);
	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(MIN_BYTE,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(ten | one,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

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
	delay(400);

	I2C_write_Byte(WRITERTC,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(ten | one,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

}

void setDate_RTC(uint8 year,uint8 month,uint8 day){

	//En esta función los valores tienen que llegar en formato hexadecimal

	one = day & ONES_MASK;
	ten = day & ONE_TEN_MASK;

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(DAY_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(ten | one, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	one = month & ONES_MASK;
	ten = month & TWELVE_MASK;

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(MTH_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(ten | one, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

	one = year & ONES_MASK;
	ten = year & TENS_MASK;

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(YEAR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(ten | one, I2C_0); //activar el oscilador 0x10000000
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

}

void setHourFormat_RTC(Format_Type format){

	I2C_TX_RX_Mode(TRUE, I2C_0);
	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(HOUR_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(400);

	I2C_stop(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);

	if(formatFlag == TRUE){
		dataRTC = lastHour;
		formatFlag = FALSE;
	}
	else if(formatFlag == FALSE){
		lastHour = dataRTC;
		formatFlag = TRUE;
	}
	if(FORMAT24 == ((dataRTC & HOUR_FORMAT_MASK) >> HOUR_SHIFT)){
		one = (dataRTC & ONES_MASK);
		ten = (dataRTC & ONE_TEN_MASK) >> TENS_SHIFT;

		if(ten*TENS+one > 12){
			setHourResult = (ten * TENS + one) % 12;
		}
		else{
			setHourResult = ten * TENS + one;
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
			setHourResult = (ten * TENS + one) + 12;
		}
	}

	ten = (setHourResult / 12) << TENS_SHIFT;
	one = (setHourResult % 12);

	I2C_start(I2C_0);
	delay(400);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(HOUR_BYTE,I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	if(FORMAT24 == format){
		I2C_write_Byte(ten | one, I2C_0);
	}
	else if (FORMAT12 == format){
		if(AM == ampm){
			I2C_write_Byte(ten | one, I2C_0);
		}
		else if(PM == ampm){
			I2C_write_Byte(HOUR_FORMAT_MASK | ten | one, I2C_0);
		}
	}
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	checkOSCRUN_RTC();

	while(TRUE == (dataRTC & OSCRUN_MASK));

}

void checkOSCRUN_RTC(void){
	I2C_repeted_Start(I2C_0);

	I2C_write_Byte(WRITERTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_write_Byte(WK_BYTE, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(READRTC, I2C_0);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	delay(400);

	I2C_TX_RX_Mode(FALSE, I2C_0);

	I2C_NACK(I2C_0);
	dataRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);
	delay(400);

	I2C_stop(I2C_0);
}

