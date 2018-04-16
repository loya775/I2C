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

/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function call to ReadMemoryMenu.

 	 \return void
 */
void Menu_Function1(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function call to WriteMemoryMenu.

 	 \return void
 */
void Menu_Function2(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function call to Establecer_Hora.

 	 \return void
 */
void Menu_Function3(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \brief	 This function call to Establecer_Fecha.

 	 \return void
 */
void Menu_Function4(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function call to Formato_Hora.

 	 \return void
 */
void Menu_Function5(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief	 This function call to Leer_Hora.

 	 \return void
 */
void Menu_Function6(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function call to Leer_Fecha.

 	 \return void
 */
void Menu_Function7(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function doesnt do nothing.

 	 \return void
 	  \todo Implement Comunication with Terminal 2 Function.
 */
void Menu_Function8(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function doesnt do nothing.

 	 \return void
 	  \todo Implement LCD Echo Function.
 */
void Menu_Function9(void);
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function point to another function dependying of variable State.

 	 \param[in]  State With this value we use to call a function dependying of her value.
 	 \return void
 */
void choose_function(uint8 State);



#endif /* FUNCTIONROTATE_H_ */
