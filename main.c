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
 * @file    Practica_3_I2C.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "GPIO.h"
#include "GlobalFunctions.h"
#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "FunctionRotate.h"
#include "UART.h"/**UART device driver*/
#include "NVIC.h"/**NVIC device driver*/
#include "TeraTermWrite.h"
#include "I2C_Driver.h"
#include "RTC.h"
#define Ano 0x96
#define Mes 0X03
#define Dia 0x21
#define Hora 0x15
#define Min 0x20
#define seg 0x95
#define OFFSET_FOR_ASCII 49
#define Clock 21000000

int main(void) {
	uint8 Selector = 0;
	I2C_init(I2C_0,210000,250);
	GPIO_pinControlRegisterType MUXALT = PORT_PCR_MUX(3);
	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	/**Configures the pin coOntrol register of pin16 in PortB as UART RX*/
	GPIO_pinControlRegister(GPIO_B,BIT16, &MUXALT);
	/**Configures the pin control register of pin16 in PortB as UART TX*/
	GPIO_pinControlRegister(GPIO_B,BIT17, &MUXALT);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init (UART_0,  Clock, BD_115200);
	//printf("UART is configured");
	/**Enables the UART 0 interrupt*/
	UART0_interruptEnable(UART_0);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enableInterruptAndPriotity(UART0_IRQ, PRIORITY_10);
	FirstMenu();

	/**Enables interrupts*/
	EnableInterrupts;
	setHourFormat_RTC(FORMAT24);
	setTime_RTC(Hora, Min, seg, FORMAT24, PM);
	setDate_RTC(Ano, Mes, Dia);
	while(TRUE)
	{
		Selector = InitMenu();
		Selector -= OFFSET_FOR_ASCII;
		choose_function(Selector);
	}
}
