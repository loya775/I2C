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
#define ADDRESS 4
#define LONGBYTES 4
#define MESSAGE 64
#define DefineTime 6
#define FECHA 8
uint8 dias;
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
	Hours = convertValue(0, SetTime);
	Minute = convertValue(2, SetTime);
	Seconds = convertValue(4, SetTime) + 0x80;
	setTime_RTC(Hours, Minute, Seconds, FORMAT24, AM);
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
	Days = convertValue(0, SetFecha);
	Months = convertValue(2, SetFecha);
	YearsHigh = *(SetFecha + 4);
	YearsLow = *(SetFecha + 5);
	Years = convertValue(6, SetFecha);
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
	uint8 Flag1;
	uint8 Flag2;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) El Formato Actual es:\r");
	UART_putString(UART_0,"\033[2;0H");
	UART_putString(UART_0, "1) Desea cambiar el formato a:\r");
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "1) Si o no:\r");
	Flag1 = Uart_Writing_Address(MESSAGE);

	if(Flag1 != 0)
	{
		UART_putString(UART_0,"\033[3;0H");
		UART_putString(UART_0, "El formato ha sido cambiado\r");
	}

}

void Leer_Hora()
{
	uint8 Flag1 = 0;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) La hora actual es:\r");
	UART_putString(UART_0,"\033[2;0H");

	while(Flag1 != 13){
	UART_putString(UART_0,"\033[2;0H");
	horas = getHours_RTC();
	HorasHigh = horas/10;
	HorasLow = horas-(HorasHigh*10);
	UART_putChar (UART_0, HorasHigh+48);
	UART_putChar (UART_0, HorasLow+48);
	minutos = getMinutes_RTC();
	MinutosHigh = minutos/10;
	MinutosLow = minutos-(MinutosHigh*10);
	UART_putChar (UART_0, 58);
	UART_putChar (UART_0, MinutosHigh+48);
	UART_putChar (UART_0, MinutosLow+48);
	segundos = getSeconds_RTC();
	SegundosHigh = segundos/10;
	SegundosLow = segundos-(SegundosHigh*10);
	UART_putChar (UART_0, 58);
	UART_putChar (UART_0, SegundosHigh+48);
	UART_putChar (UART_0, SegundosLow+48);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "Presiona enter para salir\r");
	Flag1 = Uart_For_Enter();
	}
}

void Leer_Fecha()
{
	uint8 Flag1 = 0;
	UART_putString(UART_0,"\033[2J");
	UART_putString(UART_0,"\033[1;0H");
	UART_putString(UART_0, "1) La fecha actual es:\r");
	UART_putString(UART_0,"\033[2;0H");

	while(Flag1 != 13){
	UART_putString(UART_0,"\033[2;0H");
	dias = getDays_RTC();
	diasHigh = dias/10;
	diasLow = dias-(diasHigh*10);
	UART_putChar (UART_0, diasHigh+48);
	UART_putChar (UART_0, diasLow+48);
	meses = getMonths_RTC();
	mesesHigh = meses/10;
	mesesLow = meses-(mesesHigh*10);
	UART_putChar (UART_0, 92);
	UART_putChar (UART_0, mesesHigh+48);
	UART_putChar (UART_0, mesesLow+48);
	anos = getYears_RTC();
	anosHigh = anos/10;
	anosLow = anos-(anosHigh*10);
	UART_putChar (UART_0, 92);
	UART_putChar (UART_0, YearsHigh);
	UART_putChar (UART_0, YearsLow);
	UART_putChar (UART_0, anosHigh+48);
	UART_putChar (UART_0, anosLow+48);
	UART_putString(UART_0,"\033[3;0H");
	UART_putString(UART_0, "Presiona enter para salir\r");
	Flag1 = Uart_For_Enter();
	}
}

uint8 convertValue(uint8 Offset, uint32 *ToConvert)
{
	uint8 Contador = Offset;
	uint8 Max = 2 + Offset;
	uint8 Read;
	uint8 ValueToReturn = 0;
	uint8 Multiplier = 16;
	while(Contador < Max)
	{
		Read = *(ToConvert+Contador)-48;
		ValueToReturn += Read*Multiplier;
		Multiplier = Multiplier/16;
		Contador += 1;
	}
	return ValueToReturn;
}

void WriteHour()
{
	dias = getDays_RTC();
	dias = (dias > 4)*10;
	dias += (dias & 0xF);
	meses = getMonths_RTC();
	meses = (meses > 4)*10;
	meses += (meses & 0xF);
	anos = getYears_RTC();
	anos = (anos > 4)*10;
	anos += (anos & 0xF);
	horas = getHours_RTC();
	horas = (horas > 4)*10;
	horas += (horas & 0xF);
	minutos = getMinutes_RTC();
	minutos = (minutos > 4)*10;
	minutos += (minutos & 0xF);
	segundos = getSeconds_RTC();
	segundos = (segundos > 4)*10;
	segundos += (segundos & 0xF);
}
