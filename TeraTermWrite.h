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

void FirstMenu(void);
void ReadMemoryMenu(void);
void WriteMemoryMenu(void);
void Establecer_hora(void);
uint8 convertValue(uint8 Offset, uint32 *ToConvert);
void Establecer_fecha(void);
void WriteHour(void);
void Formato_Hora(void);
void Leer_Fecha(void);
void Leer_Hora(void);
#endif /* TERATERMWRITE_H_ */
