/*
 * EEPROM.c
 *
 *  Created on: Apr 8, 2018
 *      Author: Jorge Loya
 */
#include "EEPROM.h"
#include "I2C_Driver.h"
#include "GlobalFunctions.h"
uint8 EEPROM_Read()
{
	uint8 dataFromEEPROM;

	I2C_start();

	I2C_write_Byte(0xA0);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_write_Byte(0x00);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_write_Byte(0x02);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_repeted_Start();
	I2C_write_Byte(0xA1);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_TX_RX_Mode(FALSE);

	I2C_NACK();
	delay(800);

	dataFromEEPROM = I2C_read_Byte();
	I2C_wait();
	delay(800);

	I2C_stop();
	dataFromEEPROM = I2C_read_Byte();
	return (dataFromEEPROM);
}

void EEPROM_Write(uint8 Variable)
{
	I2C_start();

	I2C_write_Byte(0xA0);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_write_Byte(0x00);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_write_Byte(0x01);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_write_Byte(Variable);
	I2C_wait();
	I2C_get_ACK();
	delay(800);

	I2C_stop();
	return ;
}
