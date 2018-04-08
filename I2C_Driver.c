/*
 * I2C_Driver.c
 *
 *  Created on: Apr 6, 2018
 *      Author: Jorge Loya
 */
#include "MK64F12.h"
#include "I2C_Driver.h"
uint8 Flag;
uint8 Busy;
void I2C_init(I2C_ChannelType channel, uint32 systemClock, uint8 baudRate)
{
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	/*I2C0->A1 |= 0xDEU;
	I2C0->F |= 24;
	I2C0->C1 |= I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;*/
	return;
}

uint8 I2C_busy()
{
	Busy = I2C0->S & I2C_S_BUSY_MASK;
	return Busy;
}

void I2C_stop(void)
{
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
}

void I2C_start(void)
{
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
}

