/*
 * I2C_Driver.c
 *
 *  Created on: Apr 6, 2018
 *      Author: Jorge Loya
 */
#include "MK64F12.h"
#include "I2C_Driver.h"
#include "GPIO.h"
#define MUL 0x1
#define LOAD_I2Cx 0x0A
uint8 Flag;
uint8 Busy;
uint8 Data;
uint8 ReadData;

void I2C_init(I2C_ChannelType channel, uint32 systemClock, uint8 baudRate)
{
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	I2C0->F |= LOAD_I2Cx;
	//I2C0->F |= 1 << I2C_F_MULT_SHIFT;
	I2C0->C1 |= I2C_C1_IICEN_MASK;
	GPIO_pinControlRegisterType MUXAlt2 = GPIO_MUX2;
	GPIO_clockGating(GPIO_B);
	GPIO_pinControlRegister(GPIO_B,BIT2,&MUXAlt2);
	GPIO_pinControlRegister(GPIO_B,BIT3,&MUXAlt2);

	return;
}

uint8 I2C_busy()
{
	Busy = (I2C0->S & I2C_S_BUSY_MASK) << I2C_S_BUSY_SHIFT;
	return Busy;
}

void I2C_stop(void)
{
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
}

void I2C_MST_OrSLV_Mode(uint8 masterOrSlave)
{
	if (masterOrSlave)
	{
		I2C0->C1 |= I2C_C1_MST_MASK;
	}else
	{
		I2C0->C1 &= ~I2C_C1_MST_MASK;
	}
}

void I2C_TX_RX_Mode(uint8 TxOrRx)
{
	if(TxOrRx)
	{
		I2C0->C1 |= I2C_C1_TX_MASK;
	}else
	{
		I2C0->C1 &= ~I2C_C1_TX_MASK;
	}
}

void I2C_start(void)
{
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
}

void I2C_NACK(void)
{
	I2C0->C1 |= I2C_C1_TXAK_MASK;
}

void I2C_repeted_Start(void)
{
	I2C0->C1 |= I2C_C1_RSTA_MASK;
}

void I2C_write_Byte(uint8 Data){
	I2C0->D = Data;
}

uint8  I2C_read_Byte(void)
{
	ReadData = I2C0->D;
	return ReadData;
}

uint16 I2C_get_ACK(void)
{
	return (I2C0->S & I2C_S_RXAK_MASK);
}

void I2C_wait(void)
{
	while(FALSE == (I2C0->S & I2C_S_TCF_MASK));
}
