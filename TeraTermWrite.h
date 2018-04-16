/*
 * TeraTermWrite.h
 *
 *  Created on: Apr 11, 2018
 *      Author: Jorge Loya
 */

#include "UART.h"
#include "DataTypeDefinitions.h"

#ifndef TERATERMWRITE_H_
#define TERATERMWRITE_H_

typedef enum {A_Value = 65,/*!< Definition to configure a pin as input */
			  B_Value = 66, /*!< Definition to configure a pin as output */
			  C_Value = 67,
			  D_Value = 68,
			  E_Value = 69,
			  F_Value = 70
			 }AsciToHex;
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	\brief	 This function Prints the first Menu .
	\return  Nothing

*/

void FirstMenu(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function prints the Read Memory Menu.
 	 \return  Nothing

 */
void ReadMemoryMenu(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function prints the Write Memory Menu.
 	 \return  Nothing.

 */
void WriteMemoryMenu(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function prints the establish hour menu.
 	 \return  Nothing.

 */
void Establecer_hora(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads all the GPIO port.
 	 \param[in] portName Port to be read.
 	 \return  It is the value read from a GPIO. It is a 32-bit value.

 */
uint8 convertValue(uint8 Offset, uint32 *ToConvert);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function Convert a decimal value to an hexadecimal value.
 	 \param[in] Offset This variable has the point of an array that we gonna read.
 	 \param[in] ToConvert Is a pointer to a direction for the values to convert.
 	 \return  One of the three values convert to Hex.

 */
void Establecer_fecha(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function Prints the establish date menu.
 	 \return  Nothing.

 */
void Formato_Hora(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads  the actual date and print it.
 	 \param[in] portName Port to be read.
 	 \return  It is the value read from a GPIO. It is a 32-bit value.

 */
void Leer_Fecha(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the acutal hour and print it.
 	 \param[in] portName Port to be read.
 	 \return  It is the value read from a GPIO. It is a 32-bit value.

 */
void Leer_Hora(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the actual format of hour.
 	 \return  The Actual format of hour.

 */
uint8 getFormat(void);
#endif /* TERATERMWRITE_H_ */
