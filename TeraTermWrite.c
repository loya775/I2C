/*
 * TeraTermWrite.c
 *
 *  Created on: Apr 11, 2018
 *      Author: Jorge Loya
 */
#include "DataTypeDefinitions.h"
#include "TeraTermWrite.h"
#include "MK64F12.h"
#include "NVIC.h"
#include "GPIO.h"
#include "EEPROM.h"
#include "RTC.h"
#define OFFSET0 0
#define OFFSET2 2
#define OFFSET4 4
#define VALUEAM 0x616D
#define VALUEPM 0x706D
#define OFFSET5 5
#define ENTERTERA 13
#define OFFSET6 6
#define ADDRESS 4
#define LONGBYTES 4
#define FLAGFORSECONDS 0x80
#define MESSAGE 64
#define ASCIIFORDIAGONAL 92
#define DefineTime 6
#define MAXTOMOVEINARRAY 2
#define MOVEONESTEP 1
#define FECHA 8
#define MAX_VALUE_IN_HEX 16
#define OFFSET_ASCII_NUM 48
#define INITVALUE 0
#define GETDECENAS 10
#define TWOPOINTS 58
uint8 dias;
uint8 Format = TRUE;
uint8 diasHigh;
uint8 diasLow;
uint8 meses;
uint8 mesesLow;
uint8 mesesHigh;
uint8 anos;
uint8 anosLow;
uint8 anosHigh;
uint8 horas;
uint8 HorasLow;
uint8 HorasHigh;
uint8 minutos;
uint8 MinutosLow;
uint8 MinutosHigh;
uint8 segundos;
uint8 SegundosLow;
uint8 SegundosHigh;
uint8 YearsHigh;
uint8 YearsLow;
void FirstMenu(){
	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in red and background in cyan*/
	UART_putString(UART_0,"\033[0;32;46m");
	/*VT100 command for clearing the screen*/
	UART_putString(UART_0,"\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_putString(UART_0,"\033[0;32;41m");
	/** VT100 command for positioning the cursor in x and y position*/
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) Leer Memoria I2C\r");
	UART_putString(UART_0,"\033[2;0H");
	UART_putString(UART_0, "2) Escribir Memoria I2C\r");
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "3) Establecer Hora\r");
	UART_putString(UART_0,"\033[4;0H");
	UART_putString(UART_0, "4) Establecer Fecha\r");
	UART_putString(UART_0,"\033[5;0H");
	UART_putString(UART_0, "5) Formato de hora\r");
	UART_putString(UART_0,"\033[6;0H");
	UART_putString(UART_0, "6) Leer Hora\r");
	UART_putString(UART_0,"\033[7;0H");
	UART_putString(UART_0, "7) Leer Fecha\r");
	UART_putString(UART_0,"\033[8;0H");
	UART_putString(UART_0, "8) Comunicacion con terminal 2\r");
	UART_putString(UART_0,"\033[9;0H");
	UART_putString(UART_0, "9) Eco en LCD\r");
	UART_putString(UART_0,"\033[10;0H");
	return;
}

void ReadMemoryMenu(void)
{
	uint32 *WriteAdress;
	uint32 *WriteNumberOfBytes;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) Direccion de lectura:\r");
	UART_putString(UART_0,"\033[2;0H");
	UART_putString(UART_0, "0x0000\r");
	UART_putString(UART_0,"\033[2;3H");
	WriteAdress = Uart_Writing_Address(ADDRESS);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "2) Longitud en Bytes:\r");
	UART_putString(UART_0,"\033[4;0H");
	UART_putString(UART_0, "0x0000\r");
	UART_putString(UART_0,"\033[4;3H");
	WriteNumberOfBytes =  Uart_Reading_Bytes_Address(LONGBYTES);
	UART_putString(UART_0,"\033[5;0H");
	UART_putString(UART_0, "3) Contenido: \r");
	UART_putString(UART_0,"\033[6;0H");
	EEPROM_Read(WriteAdress, WriteNumberOfBytes);
	UART_putString(UART_0,"\033[7;0H");
	UART_putString(UART_0, "Presiona Enter para Salir");
	Uart_Writing_Address(MESSAGE);
	return;
}

void WriteMemoryMenu(void)
{
	uint32 *Write;
	uint32 *WriteAddress;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) Direccion de escritura:\r");
	UART_putString(UART_0,"\033[2;0H");
	UART_putString(UART_0, "0x0000\r");
	UART_putString(UART_0,"\033[2;3H");
	WriteAddress = Uart_Writing_Address(ADDRESS);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "2) Texto a guardar:\r");
	UART_putString(UART_0,"\033[4;0H");
	Write = Uart_Text_To_Read(MESSAGE);
	UART_putString(UART_0,"\033[5;0H");
	UART_putString(UART_0, "Presiona Enter para Salir");
	EEPROM_Write(WriteAddress, Write);
	Uart_Writing_Address(MESSAGE);
	return;
}

void Establecer_hora(void)
{
	uint32 *SetTime;
	uint8 Hours;
	uint8 Minute;
	uint8 Seconds;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) Escribir hora en hh/mm/ss:\r");
	UART_putString(UART_0,"\033[2;0H");
	UART_putString(UART_0, "00:00:00\r");
	UART_putString(UART_0,"\033[2;0H");
	SetTime = Uart_setTime_Get(DefineTime);
	Hours = convertValue(OFFSET0, SetTime);
	Minute = convertValue(OFFSET2, SetTime);
	Seconds = convertValue(OFFSET4, SetTime) + FLAGFORSECONDS;
	setTime_RTC(Hours, Minute, Seconds, FORMAT24, PM);
	//WriteAddress = Uart_Writing_Address(ADDRESS);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "La hora ha sido cambiada:\r");
	UART_putString(UART_0,"\033[4;0H");
	UART_putString(UART_0, "Presiona enter para salir:\r");
	Uart_Writing_Address(MESSAGE);
	return;
}

void Establecer_fecha(void)
{
	uint32 *SetFecha;
	uint8 Days;
	uint8 Months;
	uint32 Years;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) Escribir hora en dd/mm/aa:\r");
	UART_putString(UART_0,"\033[2;0H");
	UART_putString(UART_0, "00/00/0000\r");
	UART_putString(UART_0,"\033[2;0H");
	SetFecha = Uart_setTime_Get(FECHA);
	Days = convertValue(OFFSET0, SetFecha);
	Months = convertValue(OFFSET2, SetFecha);
	YearsHigh = *(SetFecha + OFFSET4);
	YearsLow = *(SetFecha + OFFSET5);
	Years = convertValue(OFFSET6, SetFecha);
	setDate_RTC(Years, Months, Days);
	//WriteAddress = Uart_Writing_Address(ADDRESS);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "La hora ha sido cambiada:\r");
	UART_putString(UART_0,"\033[4;0H");
	UART_putString(UART_0, "Presiona enter para salir:\r");
	Uart_Writing_Address(MESSAGE);
	return;
}

void Formato_Hora()
{
	uint8 Change;
	uint8 Flag1;
	if (Format == TRUE)
	{
		UART_putString(UART_0,"\033[2J");
		UART_putString(UART_0,"\033[1;0H");
		UART_putString(UART_0, "El Formato Actual es de 24 horas:\r");
		UART_putString(UART_0,"\033[2;0H");
		UART_putString(UART_0, "Desea cambiar el formato a 12 horas:\r");
		UART_putString(UART_0,"\033[3;0H");
		UART_putString(UART_0, "Si o no:\r");
		UART_putString(UART_0,"\033[4;0H");
		Change = Uart_For_Yes_Or_No();
	}else
	{
		UART_putString(UART_0,"\033[2J");
		UART_putString(UART_0,"\033[1;0H");
		UART_putString(UART_0, "1) El Formato Actual es de 12 horas:\r");
		UART_putString(UART_0,"\033[2;0H");
		UART_putString(UART_0, "1) Desea cambiar el formato a 24 horasa:\r");
		UART_putString(UART_0,"\033[3;0H");
		UART_putString(UART_0, "1) Si o no:\r");
		Change = Uart_For_Yes_Or_No();
	}

	if(Format == TRUE && Change == TRUE)
	{
		setHourFormat_RTC(FORMAT12);
		Format = FALSE;
		UART_putString(UART_0,"\033[3;0H");
		UART_putString(UART_0, "El formato ha sido cambiado\r");
		while(Flag1 != ENTERTERA)
		{
			UART_putString(UART_0,"\033[4;0H");
			Flag1 = Uart_For_Enter();
		}
	}else if(Format == FALSE && Change == TRUE)
	{
		UART_putString(UART_0,"\033[3;0H");
		UART_putString(UART_0, "El formato ha sido cambiado\r");
		setHourFormat_RTC(FORMAT24);
		Format = TRUE;
		while(Flag1 != ENTERTERA)
		{
			Flag1 = Uart_For_Enter();
		}
	}
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "El formato no ha sido cambiado\r");
	while(Flag1 != ENTERTERA)
	{
		UART_putString(UART_0,"\033[4;0H");
		Flag1 = Uart_For_Enter();
	}

}

void Leer_Hora()
{
	uint8 Flag1 = 0;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) La hora actual es:\r");
	UART_putString(UART_0,"\033[2;0H");

	while(Flag1 != ENTERTERA){
	UART_putString(UART_0,"\033[2;0H");
	horas = getHours_RTC();
	HorasHigh = horas/GETDECENAS;
	HorasLow = horas-(HorasHigh*GETDECENAS);
	UART_putChar (UART_0, HorasHigh + OFFSET_ASCII_NUM);
	UART_putChar (UART_0, HorasLow + OFFSET_ASCII_NUM);
	minutos = getMinutes_RTC();
	MinutosHigh = minutos/GETDECENAS;
	MinutosLow = minutos-(MinutosHigh*GETDECENAS);
	UART_putChar (UART_0, TWOPOINTS);
	UART_putChar (UART_0, MinutosHigh + OFFSET_ASCII_NUM);
	UART_putChar (UART_0, MinutosLow + OFFSET_ASCII_NUM);
	segundos = getSeconds_RTC();
	SegundosHigh = segundos/GETDECENAS;
	SegundosLow = segundos-(SegundosHigh*GETDECENAS);
	UART_putChar (UART_0, TWOPOINTS);
	UART_putChar (UART_0, SegundosHigh + OFFSET_ASCII_NUM);
	UART_putChar (UART_0, SegundosLow + OFFSET_ASCII_NUM);
	if (Format == TRUE)
	{
	}else
	{
		if(getAMPM_RTC() == VALUEPM)
		{
		UART_putString(UART_0, "AM");
		}
		else if(getAMPM_RTC() == VALUEPM)
		{
			UART_putString(UART_0, "PM");
		}
	}
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "Presiona enter para salir\r");
	Flag1 = Uart_For_Enter();
	}
}

void Leer_Fecha()
{
	uint8 Flag1 = INITVALUE;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) La fecha actual es:\r");
	UART_putString(UART_0,"\033[2;0H");

	while(Flag1 != ENTERTERA){
	UART_putString(UART_0,"\033[2;0H");
	dias = getDays_RTC();
	diasHigh = dias/GETDECENAS;
	diasLow = dias-(diasHigh*GETDECENAS);
	UART_putChar (UART_0, diasHigh + OFFSET_ASCII_NUM);
	UART_putChar (UART_0, diasLow + OFFSET_ASCII_NUM);
	meses = getMonths_RTC();
	mesesHigh = meses/GETDECENAS;
	mesesLow = meses-(mesesHigh*GETDECENAS);
	UART_putChar (UART_0, ASCIIFORDIAGONAL);
	UART_putChar (UART_0, mesesHigh + OFFSET_ASCII_NUM);
	UART_putChar (UART_0, mesesLow + OFFSET_ASCII_NUM);
	anos = getYears_RTC();
	anosHigh = anos/GETDECENAS;
	anosLow = anos-(anosHigh*GETDECENAS);
	UART_putChar (UART_0, ASCIIFORDIAGONAL);
	UART_putChar (UART_0, YearsHigh);
	UART_putChar (UART_0, YearsLow);
	UART_putChar (UART_0, anosHigh + OFFSET_ASCII_NUM);
	UART_putChar (UART_0, anosLow + OFFSET_ASCII_NUM);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "Presiona enter para salir\r");
	Flag1 = Uart_For_Enter();
	}
}

uint8 convertValue(uint8 Offset, uint32 *ToConvert)
{
	uint8 Contador = Offset;
	uint8 Max = MAXTOMOVEINARRAY + Offset;
	uint8 Read;
	uint8 ValueToReturn = INITVALUE;
	uint8 Multiplier = MAX_VALUE_IN_HEX;
	/*In this part we do the conversion to HEX*/
	while(Contador < Max)
	{
		/*We move across the direction*/
		Read = *(ToConvert+Contador)-OFFSET_ASCII_NUM;
		/*We multiplied the value of read for a multiplier who has a value in Hex*/
		ValueToReturn += Read*Multiplier;
		Multiplier = Multiplier/MAX_VALUE_IN_HEX;
		Contador += MOVEONESTEP;
	}
	return ValueToReturn;
}


uint8 getFormat()
{
	return Format;
}
