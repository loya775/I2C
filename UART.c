/*
 * UART.c
 *
 *  Created on: Mar 31, 2018
 *      Author: d
 */
#include "UART.h"
#include "MK64F12.h"
#include "EEPROM.h"
#include "TeraTermWrite.h"
#define BRFACONSTANT 32
#define UARTBRCONSTANT 16
#define BUSCLKCONSTANT 2
#define BDH_SBR_MASK 11111
#define UARTBR_SBR_MASK 1111100000000
#define UARTBR_SBR_SHIFT 7
#define DELETE_KEY_PRESS 8
#define MIN_NUM_ASCII 48
#define OFSSET_HEX_ASCII 55
#define MAX_NUM_ASCII 57
#define MAX_HEX_ASCII 70
#define MIN_HEX_ASCII 65
#define MOVE_ONE_STEP 1
#define TWOPOINTS1 1
#define TWOPOINTS2 2
#define TWOPOINTS3 3
#define TWOPOINTS4 4
#define FORCOUNTER2 2
#define INIT_VALUE 0
#define ENTER_KEY_PRESS 13
#define MEMORY 20
#define ASCIIN 110
#define ASCIIO 111
#define ASCIIS 115
#define ASCIII 105
uint8 SaveValue = 0;
uint8 Value = 0;
uint8 CMatrix = 0;
UART_MailBoxType UART0_MailBox;
static volatile uint8 brfd = 0;
static uint8 brfa;
static uint16 uartBaudRate;
uint8 flag;
void UART0_RX_TX_IRQHandler(void){
	while(FALSE == ((UART0->S1 & UART_S1_RDRF_MASK))){}
		UART0_MailBox.mailBox = UART0->D;
		UART0_MailBox.flag = TRUE;

}

void UART_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate){
	switch(uartChannel){
		case UART_0:
			UART0_MailBox.mailBox = 0;
			UART0_MailBox.flag = FALSE;
			uartBaudRate = systemClk / (UARTBRCONSTANT * (baudRate + brfd));
			brfd = brfa / BRFACONSTANT;
			SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
			UART0->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
			UART0->BDH |= UART_BDH_SBR_MASK & (uartBaudRate >> 0x08);
			UART0->BDL &= ~UART_BDL_SBR_MASK;
			UART0->BDL |=  (uint8)uartBaudRate;
			UART0->C4 |= brfa;
			UART0->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
			break;
		case UART_1:
			uartBaudRate = systemClk / (UARTBRCONSTANT * (baudRate + brfd));
			brfd = brfa / BRFACONSTANT;
			SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
			//UART1->C2 = ~(UART_C2_RE_MASK|UART_C2_TE_MASK);
			UART1->BDH |=  uartBaudRate|UARTBR_SBR_MASK;
			UART1->BDL &= ~UART_BDL_SBR_MASK;
			UART1->BDL |=  uartBaudRate ;
			UART1->C4 |= brfa;
			UART1->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
			break;
		case UART_2:
			uartBaudRate = (systemClk/BUSCLKCONSTANT) / (UARTBRCONSTANT * (baudRate + brfd));
			brfd = brfa / BRFACONSTANT;
			SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
			//UART2->C2 = ~(UART_C2_RE_MASK|UART_C2_TE_MASK);
			UART2->BDH |=  uartBaudRate|UARTBR_SBR_MASK;
			UART2->BDL &= ~UART_BDL_SBR_MASK;
			UART2->BDL |=  uartBaudRate;
			UART2->C4 |= brfa;
			UART2->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
			break;
		case UART_3:
			uartBaudRate = (systemClk/BUSCLKCONSTANT) / (UARTBRCONSTANT * (baudRate + brfd));
			brfd = brfa / BRFACONSTANT;
			SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
			//UART3->C2 = ~(UART_C2_RE_MASK|UART_C2_TE_MASK);
			UART3->BDH |=  uartBaudRate|UARTBR_SBR_MASK;
			UART3->BDL &= ~UART_BDL_SBR_MASK;
			UART3->BDL |=  uartBaudRate;
			UART3->C4 |= brfa;
			UART3->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
			break;
		case UART_4:
			uartBaudRate = (systemClk/BUSCLKCONSTANT) / (UARTBRCONSTANT * (baudRate + brfd));
			brfd = brfa / BRFACONSTANT;
			SIM->SCGC4 |= SIM_SCGC1_UART4_MASK;
			//UART4->C2 = ~(UART_C2_RE_MASK|UART_C2_TE_MASK);
			UART4->BDH |=  (uartBaudRate|UARTBR_SBR_MASK) >> UARTBR_SBR_SHIFT;
			UART4->BDL &= ~UART_BDL_SBR_MASK;
			UART4->BDL |=  uartBaudRate;
			UART4->C4 |= brfa;
			UART4->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
			break;
		case UART_5:
			uartBaudRate = (systemClk/BUSCLKCONSTANT) / (UARTBRCONSTANT * (baudRate + brfd));
			brfd = brfa / BRFACONSTANT;
			SIM->SCGC4 = SIM_SCGC1_UART5_MASK;
			UART5->C2 |= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);
			UART5->BDH |=  uartBaudRate|UARTBR_SBR_MASK;
			UART5->BDL &= ~UART_BDL_SBR_MASK;
			UART5->BDL |=  uartBaudRate;
			UART5->C4 |= brfa;
			UART5->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
			break;
		default:
			break;
	}
}

void UART0_interruptEnable(UART_ChannelType uartChannel){
	switch(uartChannel){
			case UART_0:
				UART0->C2 |= UART_C2_RIE_MASK;
				break;
			case UART_1:
				UART1->C2 |= UART_C2_RIE_MASK;
				break;
			case UART_2:
				UART2->C2 |= UART_C2_RIE_MASK;
				break;
			case UART_3:
				UART3->C2 |= UART_C2_RIE_MASK;
				break;
			case UART_4:
				UART4->C2 |= UART_C2_RIE_MASK;
				break;
			case UART_5:
				UART5->C2 |= UART_C2_RIE_MASK;
				break;
			default:
				break;
		}
}

void UART_putChar (UART_ChannelType uartChannel, uint8 character){
	switch(uartChannel){
			case UART_0:
				while(!(UART0->S1 & UART_S1_TC_MASK));
				UART0->D = character;
				break;
			case UART_1:
				while(!(UART1->S1 & UART_S1_TC_MASK));
				UART1->D = character;
				break;
			case UART_2:
				while(!(UART2->S1 & UART_S1_TC_MASK)){
					UART2->D = character;
				}
				break;
			case UART_3:
				while(!(UART3->S1 & UART_S1_TC_MASK)){
					UART3->D = character;
				}
				break;
			case UART_4:
				while(!(UART4->S1 & UART_S1_TC_MASK)){
					UART4->D = character;
				}
				break;
			case UART_5:
				while(!(UART5->S1 & UART_S1_TC_MASK)){
					UART5->D = character;
				}
				break;
			default:
				break;
		}
}

void UART_putString(UART_ChannelType uartChannel, sint8 *string){
	while(*string){
		UART_putChar(uartChannel, *string++);
	}
}

uint32 *Uart_Text_To_Read(uint8 Max){
	CMatrix = INIT_VALUE;
	uint8 Contador = Max - MOVE_ONE_STEP;
	uint32 MemoryToSearch[Max];
	uint32 *Apuntador;
	while(!(CMatrix == Max) && Value != ENTER_KEY_PRESS)
	{
		if(UART0_MailBox.flag)
			{
				/**Sends to the PCA the received data in the mailbox*/
				UART_putChar (UART_0, UART0_MailBox.mailBox);
				Value = UART0->D;

				if(CMatrix != Max && Value != DELETE_KEY_PRESS && Value != ENTER_KEY_PRESS)
				{
					MemoryToSearch[CMatrix] = Value;
					SaveValue |= MemoryToSearch[CMatrix] << Contador;
					Value = INIT_VALUE;
					CMatrix = CMatrix + MOVE_ONE_STEP;
					Contador = Contador - MOVE_ONE_STEP;
				}else if(Value == DELETE_KEY_PRESS)
				{
					CMatrix = CMatrix - MOVE_ONE_STEP;
					Contador = Contador + MOVE_ONE_STEP;
					Value = INIT_VALUE;
				}

				/**clear the reception flag*/
				UART0_MailBox.flag = INIT_VALUE;
			}
	}
	Apuntador = MemoryToSearch;
	MaxValueNumber(CMatrix);
	Value = INIT_VALUE;
	return Apuntador;
}

uint32 *Uart_Writing_Address(uint8 Max){
	uint32 *Apuntador;
	CMatrix = INIT_VALUE;
	uint8 Contador = Max - MOVE_ONE_STEP;
	uint32 MemoryToSearch[Max];
	while(!(CMatrix == Max) && Value != ENTER_KEY_PRESS)
	{
		if(UART0_MailBox.flag)
			{
				/**Sends to the PCA the received data in the mailbox*/
				UART_putChar (UART_0, UART0_MailBox.mailBox);
				Value = UART0->D;


				if(CMatrix != Max && Value >= MIN_NUM_ASCII && Value <= MAX_NUM_ASCII)
				{
					MemoryToSearch[CMatrix] = Value;
					SaveValue += MemoryToSearch[CMatrix]<<Contador;
					Value = INIT_VALUE;
					CMatrix = CMatrix + MOVE_ONE_STEP;
					Contador = Contador - MOVE_ONE_STEP;
				}else if (Value == DELETE_KEY_PRESS)
				{
					CMatrix = CMatrix - MOVE_ONE_STEP;
					Contador = Contador + MOVE_ONE_STEP;
					Value = INIT_VALUE;
				}else if (Value >= MIN_HEX_ASCII && Value <= MAX_HEX_ASCII)
				{
					Value -= OFSSET_HEX_ASCII;
					MemoryToSearch[CMatrix] = Value;
					SaveValue += MemoryToSearch[CMatrix]<<Contador;
					Value = INIT_VALUE;
					CMatrix = CMatrix + MOVE_ONE_STEP;
					Contador = Contador - MOVE_ONE_STEP;
				}
				else
				{
					UART_putString(UART_0,"\033[1D");
				}
					UART0_MailBox.flag = INIT_VALUE;
				}
			}
	Apuntador = MemoryToSearch;
	MaxValueNumber(CMatrix);
	Value = INIT_VALUE;
	return Apuntador;
}

uint32 *Uart_Reading_Bytes_Address(uint8 Max){
	uint32 MemoryToSearch[MEMORY];
	CMatrix = INIT_VALUE;
	uint8 Contador = Max - MOVE_ONE_STEP;
	uint32 *Apuntador;
	while(!(CMatrix == Max) && Value != ENTER_KEY_PRESS)
	{
		if(UART0_MailBox.flag)
			{
				/**Sends to the PCA the received data in the mailbox*/
				UART_putChar (UART_0, UART0_MailBox.mailBox);
				Value = UART0->D;


				if(CMatrix != Max && Value >= MIN_NUM_ASCII && Value <= MAX_NUM_ASCII)
				{
					MemoryToSearch[CMatrix] = Value;
					SaveValue += MemoryToSearch[CMatrix]<<Contador;
					Value = INIT_VALUE;
					CMatrix = CMatrix + MOVE_ONE_STEP;
					Contador = Contador - MOVE_ONE_STEP;
				}else if (Value == DELETE_KEY_PRESS)
				{
					CMatrix = CMatrix - MOVE_ONE_STEP;
					Contador = Contador + MOVE_ONE_STEP;
					Value = INIT_VALUE;
				}else if (Value >= MIN_HEX_ASCII && Value <= MAX_HEX_ASCII)
				{
					Value -= OFSSET_HEX_ASCII;
					MemoryToSearch[CMatrix] = Value;
					SaveValue += MemoryToSearch[CMatrix]<<Contador;
					Value = INIT_VALUE;
					CMatrix = CMatrix + MOVE_ONE_STEP;
					Contador = Contador - MOVE_ONE_STEP;
				}
				else
				{
					UART_putString(UART_0,"\033[1D");
				}
					UART0_MailBox.flag = INIT_VALUE;
				}
			}
	Apuntador = MemoryToSearch;
	MaxValueNumber(CMatrix);
	Value = INIT_VALUE;
	return Apuntador;
}

uint8 InitMenu()
{
	uint8 Flag = INIT_VALUE;
	FirstMenu();
	while(Flag == INIT_VALUE)
	{
		if(UART0_MailBox.flag)
		{
			UART_putChar (UART_0, UART0_MailBox.mailBox);
			Flag = UART0->D;
			if(Flag >= (MIN_NUM_ASCII - MOVE_ONE_STEP) && Flag <= MAX_NUM_ASCII)
			{
			}else
			{
				Flag = INIT_VALUE;
				FirstMenu();
			}
			}
			UART0_MailBox.flag = INIT_VALUE;
	}

	return Flag;
}

uint32 *Uart_setTime_Get(uint8 Max){
	uint32 MemoryToSearch[MEMORY];
	CMatrix = INIT_VALUE;
	uint8 Contador = Max - MOVE_ONE_STEP;
	uint32 *Apuntador;
	uint8 TwoPointsCounter = INIT_VALUE;
	while(CMatrix != Max )
	{
		if(UART0_MailBox.flag)
			{
				/**Sends to the PCA the received data in the mailbox*/
				UART_putChar (UART_0, UART0_MailBox.mailBox);
				Value = UART0->D;
				if(TwoPointsCounter == TWOPOINTS1 || TwoPointsCounter == TWOPOINTS3)
				{
					UART_putString(UART_0,"\033[1C");
				}
				if(Value >= MIN_NUM_ASCII && Value <= MAX_NUM_ASCII)
				{
					TwoPointsCounter += MOVE_ONE_STEP;
					MemoryToSearch[CMatrix] = Value;
					SaveValue += MemoryToSearch[CMatrix] << Contador;
					Value = INIT_VALUE;
					CMatrix = CMatrix + MOVE_ONE_STEP;
					Contador = Contador - MOVE_ONE_STEP;
				}else if (Value == DELETE_KEY_PRESS)
				{
					CMatrix = CMatrix - MOVE_ONE_STEP;
					Contador = Contador + MOVE_ONE_STEP;
					if(TwoPointsCounter == TWOPOINTS2 || TwoPointsCounter == TWOPOINTS4)
					{
						UART_putString(UART_0,"\033[1D");
					}else{
						UART_putString(UART_0,"\033[1D");
					}
					TwoPointsCounter -= MOVE_ONE_STEP;
					Value = INIT_VALUE;
				}else
				{
					UART_putString(UART_0,"\033[1D");
				}
					UART0_MailBox.flag = INIT_VALUE;
				}
			}
	Apuntador = MemoryToSearch;
	MaxValueNumber(CMatrix);
	Value = INIT_VALUE;
	return Apuntador;
}

uint8 Uart_For_Enter()
{
	uint8 Flag = INIT_VALUE;
	if(UART0_MailBox.flag)
	{
		UART_putChar (UART_0, UART0_MailBox.mailBox);
		Flag = UART0->D;
	if(Flag == ENTER_KEY_PRESS)
	{}
	UART0_MailBox.flag = INIT_VALUE;
	}
	return Flag;
}

uint8 Uart_For_Yes_Or_No()
{
	uint8 Counter = INIT_VALUE;
	uint8 YesOrNo[FORCOUNTER2] = {INIT_VALUE,INIT_VALUE};
	while(TRUE)
	{
	if(UART0_MailBox.flag)
	{
		UART_putChar (UART_0, UART0_MailBox.mailBox);
		YesOrNo[Counter] = UART0->D;
		Counter += MOVE_ONE_STEP;
		if (Counter == FORCOUNTER2)
		{
			if(YesOrNo[INIT_VALUE] == ASCIIS && YesOrNo[MOVE_ONE_STEP] == ASCIII)
			{
				UART0_MailBox.flag = INIT_VALUE;
				return TRUE;
			}else if(YesOrNo[INIT_VALUE] == ASCIIN && YesOrNo[MOVE_ONE_STEP] == ASCIIO)
			{
				UART0_MailBox.flag = INIT_VALUE;
				return FALSE;
			}else
			{
				Counter -= FORCOUNTER2;
				UART_putString(UART_0,"\033[4;0H");
				YesOrNo[INIT_VALUE] = FALSE;
				YesOrNo[MOVE_ONE_STEP] = FALSE;
			}
		}
	UART0_MailBox.flag = INIT_VALUE;
	}
	}
}
