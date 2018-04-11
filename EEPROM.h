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
uint8 EEPROM_Read(void);

void EEPROM_Write(uint8 Variable);

#endif /* EEPROM_H_ */
