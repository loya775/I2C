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


void EEPROM_Read(uint32 *Direccion, uint32 *Bytes);
void MaxValueNumber(uint8 Value);
void EEPROM_Write(uint32 *AddressPointer, uint32 *ValuePointer);
#endif /* EEPROM_H_ */
