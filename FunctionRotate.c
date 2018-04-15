/*
 * FunctionRotate.c
 *
 *  Created on: Apr 14, 2018
 *      Author: Jorge Loya
 */
#include "FunctionRotate.h"
#include "TeraTermWrite.h"

typedef const struct Function
		{
			void(*fptr)(void);
			uint32 state;
		}Function;

const Function FSMMoore[9]=
{
		{Menu_Function1,1},
		{Menu_Function2,2},
		{Menu_Function3,3},
		{Menu_Function4,4},
		{Menu_Function5,5},
		{Menu_Function6,6},
		{Menu_Function7,7},
		{Menu_Function8,8},
		{Menu_Function9,9}
};



void Menu_Function1(void)
{
	ReadMemoryMenu();
}

void Menu_Function2()
{
	WriteMemoryMenu();
}
void Menu_Function3()
{
	Establecer_hora();
}
void Menu_Function4()
{
	Establecer_fecha();
}
void Menu_Function5()
{
	Formato_Hora();
}
void Menu_Function6()
{
	Leer_Hora();
}
void Menu_Function7(void)
{
	Leer_Fecha();
}
void Menu_Function8(void)
{

}
void Menu_Function9(void)
{

}

void choose_function(uint8 State)
{
 FSMMoore[State].fptr();
}
