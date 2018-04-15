/*
 * EEPROM.c
 *
 *  Created on: Apr 8, 2018
 *      Author: Jorge Loya
 */
#include "EEPROM.h"
#include "I2C_Driver.h"
#include "GlobalFunctions.h"
#include "UART.h"
#include "TeraTermWrite.h"
#define MAX_OF_MEMORY_HEX 4
#define DELAY_AFTER_ACK 100
#define DELAY_BETWEEN_TRANSMISION 5000
#define INITIAL_VALUE 0
#define MOVE_ONE_STEP 1
#define A0 0xA0
#define A1 0xA1
#define Receptor_Mode 0
#define HIGH_VALUE_FOR_HEX 0xFF00
#define LOW_VALUE_FOR_HEX 0xFF
#define DIVIDER_FOR_HEX 16
#define VALUE_HEX_TO_DEC 4096
#define OFFSEET_FOR_NUMBER_ASCII 48
#define MIN_NUMBER_FOR_HEX 10
#define MAX_NUMBER_FOR_HEX 15
uint8 Max;
uint16 Preview;
uint16 HighDirection;
uint8 LowDirection;

void EEPROM_Read(uint32 *Direccion, uint32 *Bytes)
{
	uint8 dataFromEEPROM;
	uint8 Contador = INITIAL_VALUE;
	uint16 DirectionToSend = INITIAL_VALUE;
	uint32 Multiplier = VALUE_HEX_TO_DEC;
	uint16 Bite = INITIAL_VALUE;

	while(Contador < MAX_OF_MEMORY_HEX)
	{
		Preview = *(Direccion+Contador);
		if (Preview >= MIN_NUMBER_FOR_HEX && Preview <= MAX_NUMBER_FOR_HEX)
		{

		}else
		{
			Preview -= OFFSEET_FOR_NUMBER_ASCII;
		}
		DirectionToSend += Preview*Multiplier;
		Multiplier = Multiplier/DIVIDER_FOR_HEX;
		Contador += MOVE_ONE_STEP;
	}

	Contador = INITIAL_VALUE;
	Multiplier = VALUE_HEX_TO_DEC;

	while(Contador < MAX_OF_MEMORY_HEX)
	{
		Preview = *(Bytes+Contador);
		if (Preview >= MIN_NUMBER_FOR_HEX && Preview <= MAX_NUMBER_FOR_HEX)
		{

		}else
		{
			Preview -= OFFSEET_FOR_NUMBER_ASCII;
		}
		Bite += Preview*Multiplier;
		Multiplier = Multiplier/DIVIDER_FOR_HEX;
		Contador += MOVE_ONE_STEP;
	}

	Contador = INITIAL_VALUE;

	while(Contador<Bite)
	{
		DirectionToSend = DirectionToSend + Contador;
		HighDirection = DirectionToSend & HIGH_VALUE_FOR_HEX;
		LowDirection = DirectionToSend & LOW_VALUE_FOR_HEX;

		delay(DELAY_BETWEEN_TRANSMISION);
		I2C_start(I2C_0);

		I2C_write_Byte(A0, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_write_Byte(HighDirection, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_write_Byte(LowDirection, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_repeted_Start(I2C_0);
		I2C_write_Byte(A1, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_TX_RX_Mode(Receptor_Mode,I2C_0);

		I2C_NACK(I2C_0);
		dataFromEEPROM = I2C_read_Byte(I2C_0);
		I2C_wait(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_stop(I2C_0);
		dataFromEEPROM = I2C_read_Byte(I2C_0);
		UART_putChar (UART_0, dataFromEEPROM);
		Contador = Contador + MOVE_ONE_STEP;
	}
	return;
}

void EEPROM_Write(uint32 *AddressPointer, uint32 *ValuePointer)
{
	uint32 *Direccion;
	uint8 Contador = INITIAL_VALUE;
	uint16 Preview;
	uint32 DirectionToSend = INITIAL_VALUE;
	uint32 Multiplier = VALUE_HEX_TO_DEC;
	uint16 HighDirection;
	uint8 LowDirection;
	uint8 WriteValue;
	Direccion = AddressPointer;

	while(Contador < MAX_OF_MEMORY_HEX)
	{
		Preview = *(Direccion+Contador);
		if (Preview >= MIN_NUMBER_FOR_HEX && Preview <= MAX_NUMBER_FOR_HEX)
		{

		}else
		{
			Preview -= OFFSEET_FOR_NUMBER_ASCII;
		}
		DirectionToSend += Preview*Multiplier;
		Multiplier = Multiplier/DIVIDER_FOR_HEX;
		Contador += MOVE_ONE_STEP;
	}

	Contador = INITIAL_VALUE;

	while(Contador < Max)
	{
		DirectionToSend = DirectionToSend + Contador;
		HighDirection = DirectionToSend & HIGH_VALUE_FOR_HEX;
		LowDirection = DirectionToSend & LOW_VALUE_FOR_HEX;
		WriteValue = *(ValuePointer+Contador);

		delay(DELAY_BETWEEN_TRANSMISION);
		I2C_start(I2C_0);
		I2C_write_Byte(A0, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_write_Byte(HighDirection, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_write_Byte(LowDirection, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_write_Byte(WriteValue, I2C_0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		delay(DELAY_AFTER_ACK);

		I2C_stop(I2C_0);
		Contador += MOVE_ONE_STEP;
	}
	return ;
}

void MaxValueNumber(uint8 Value)
{
	Max = Value;
	return;
}
