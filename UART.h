/**
	\file
	\brief
		This is the header file for the UART device driver.
		It contains the macros and function definition.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	27/07/2015
	\todo
		To implement all needed functions
 */
#ifndef UART_H_
#define UART_H_

#include "DataTypeDefinitions.h"

/**
 * \brief A mail box type definition for serial port
 */
typedef struct{
	uint8 flag; /** Flag to indicate that there is new data*/
	uint8 mailBox; /** it contains the received data*/
} UART_MailBoxType;


/**
 * \brief This enum define the UART port to be used.
 */
typedef enum {UART_0,UART_1,UART_2,UART_3,UART_4,UART_5}UART_ChannelType;

/**
 * \brief It defines some common transmission baud rates
 */
typedef enum {BD_4800=4800,BD_9600=9600,BD_5600=5600, BD_115200=115200}UART_BaudRateType;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is the interrupt services routing (ISR).
 	 \param[in]  void.
 	 \return void
 */
void UART0_Status_IRQHandler(void);

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It configures the UART to be used
 	 \param[in]  uartChannel indicates which UART will be used.
 	 \param[in]  systemClk indicates the MCU frequency.
 	 \param[in]  baudRate sets the baud rate to transmit.
 	 \return void
 */
void UART_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 enables the RX UART interrupt). This function should include the next sentence:
 	 while (!(UART0_S1 & UART_S1_RDRF_MASK)). It is to guaranty that the incoming data is complete
 	 when reception register is read. For more details see chapter 52 in the kinetis reference manual.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \return void
 */
void UART0_interruptEnable(UART_ChannelType uartChannel);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends one character through the serial port. This function should include the next sentence:
 	 while(!(UART0_S1 & UART_S1_TC_MASK)). It is to guaranty that before to try to transmit a byte, the previous
 	 one was transmitted. In other word, to avoid to transmit data while the UART is busy transmitting information.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \param[in]  character to be transmitted.
 	 \return void
 */

void UART_putChar (UART_ChannelType uartChannel, uint8 character);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends a string character through the serial port.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \param[in]  string pointer to the string to be transmitted.
 	 \return void
 */
void UART_putString(UART_ChannelType uartChannel, sint8* string);
uint32 *Uart_Writing_Address(uint8 Max);
uint32 *Uart_Text_To_Read(uint8 Max);
uint32 *Uart_Reading_Bytes_Address(uint8 Max);
uint8 InitMenu();
uint32 *Uart_setTime_Get(uint8 Max);
uint8 Uart_For_Enter();
#endif /* UART_H_ */
