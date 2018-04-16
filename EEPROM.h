/*
 * EEPROM.h
 *
 *  Created on: Apr 8, 2018
 *      Author: Jorge Loya
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function Reads a number of bits depending in the direction you put.

 	 \param[in]  *Direccion Is a pointer to a direction that have the direction to read.
 	 \param[in]  *Bytes Is a pointer to the direction that has the number of bytes to read.
 	 \return void
 */
void EEPROM_Read(uint32 *Direccion, uint32 *Bytes);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function puts a value in our global variable call MAX.

 	 \param[in]  Value This variable is save in the global variable MAX.
 	 \return void
 */
void MaxValueNumber(uint8 Value);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function write data in a direction of memory.

 	 \param[in]  *AddressPointer Is a pointer from a direction that has the value of the direction you wanna write in the memory
 	 \param[in]  *ValuePointer Is a pointer to a direction that has the value to write in the memory.
 	 \return void
 */
void EEPROM_Write(uint32 *AddressPointer, uint32 *ValuePointer);
#endif /* EEPROM_H_ */
