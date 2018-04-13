/*
 * GlobalFunctions.c
 *
 *  Created on: 16/08/2017
 *      Author: jlpe
 */


#include "GlobalFunctions.h"

void delay(uint16 delay)
{
	volatile int counter, counter2;

	for(counter2=16; counter2 > 0; counter2--)
	{
		for(counter=delay; counter > 0; counter--);

	}
}
