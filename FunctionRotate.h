/*
 * FunctionRotate.h
 *
 *  Created on: Apr 14, 2018
 *      Author: Jorge Loya
 */

#ifndef FUNCTIONROTATE_H_
#define FUNCTIONROTATE_H_

#include "DataTypeDefinitions.h"
#include "GPIO.h"
#include "GlobalFunctions.h"

typedef void(*const fptroToFunctions[9])(void);
typedef float* (pftG)(const uint8*);


void Menu_Function1(void);
void Menu_Function2(void);
void Menu_Function3(void);
void Menu_Function4(void);
void Menu_Function5(void);
void Menu_Function6(void);
void Menu_Function7(void);
void Menu_Function8(void);
void Menu_Function9(void);
void choose_function(uint8 State);



#endif /* FUNCTIONROTATE_H_ */
