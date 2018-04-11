/**
 * \file
 *		SPI.c
 * \brief
 *		This is the source file for the GPIO device driver for Kinetis K64.
 *		It contains all the implementation for configuration functions and runtime functions.
 *		i.e., this is the application programming interface (API) for the GPIO peripheral.
 * \authors
 * 		Raymundo Garza
 * 		Jorge Loya Herrera
 * \date
 * 		03/12/2018
 */


#include "MK64F12.h"
#include "SPI.h"
#include "GPIO.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**In this function we activate the SPI of the respective channel*/
static void SPI_enable(SPI_ChannelType channel)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->MCR &= ~SPI_MCR_MDIS_MASK;
			break;
		case(SPI_1):
			SPI1->MCR &= ~SPI_MCR_MDIS_MASK;
			break;
		case(SPI_2):
			SPI2->MCR &= ~SPI_MCR_MDIS_MASK;
			break;
		default:
			break;
	}
}
/**In this function we activated the SPI Clock of the respective channel*/
static void SPI_clk(SPI_ChannelType channel)
{
	switch(channel)
	{
		case(SPI_0):
			SIM->SCGC6 |= SPI0_CLOCK_GATING;
			break;
		case(SPI_1):
			SIM->SCGC6 |= SPI1_CLOCK_GATING;
			break;
		case(SPI_2):
			SIM->SCGC3 |= SPI2_CLOCK_GATING;
			break;
		default:
			break;
	}
}
/**In this function we select if the SPI is master or slave*/
static void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrSlave)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->MCR |= (SPI_MCR_MSTR(masterOrSlave));
			break;
		case(SPI_1):
			SPI1->MCR |= (SPI_MCR_MSTR(masterOrSlave));
			break;
		case(SPI_2):
			SPI2->MCR |= (SPI_MCR_MSTR(masterOrSlave));
			break;
		default:
			break;
	}
}
/**In this function depending if is disable or enable we activate if is a RXF or TXF*/
static void SPI_fIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->MCR |= SPI_MCR_DIS_RXF(enableOrDisable);
			SPI0->MCR |= SPI_MCR_DIS_TXF(enableOrDisable);
			break;
		case(SPI_1):
			SPI1->MCR |= SPI_MCR_DIS_RXF(enableOrDisable);
			SPI1->MCR |= SPI_MCR_DIS_TXF(enableOrDisable);
			break;
		case(SPI_2):
			SPI2->MCR |= SPI_MCR_DIS_RXF(enableOrDisable);
			SPI2->MCR |= SPI_MCR_DIS_TXF(enableOrDisable);
			break;
		default:
			break;
	}
}
/**In this function using CTAR we choose the polarity of SPI depending of cpol*/
static void SPI_clockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->CTAR[0] |= SPI_CTAR_CPOL(cpol);
			break;
		case(SPI_1):
			SPI1->CTAR[0] |= SPI_CTAR_CPOL(cpol);
			break;
		case(SPI_2):
			SPI2->CTAR[0] |= SPI_CTAR_CPOL(cpol);
			break;
		default:
			break;
	}
}
/**In this function we change de frame size of the clk using CTAR and depending of
theframeSize value*/
static void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->CTAR[0] &= ~SPI_CTAR_FMSZ_MASK;
			SPI0->CTAR[0] |= frameSize;
			break;
		case(SPI_1):
			SPI1->CTAR[0] &= ~SPI_CTAR_FMSZ_MASK;
			SPI1->CTAR[0] |= frameSize;
			break;
		case(SPI_2):
			SPI2->CTAR[0] &= ~SPI_CTAR_FMSZ_MASK;
			SPI2->CTAR[0] |= frameSize;
			break;
		default:
			break;
	}
}
/**In this function we change the clock phase depending of the value of cpha*/
static void SPI_clockPhase(SPI_ChannelType channel, SPI_PhaseType cpha)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->CTAR[0] |= SPI_CTAR_CPHA(cpha);
			break;
		case(SPI_1):
			SPI1->CTAR[0] |= SPI_CTAR_CPHA(cpha);
			break;
		case(SPI_2):
			SPI2->CTAR[0] |= SPI_CTAR_CPHA(cpha);
			break;
		default:
			break;
	}
}
/**In this function we choose the baudrate depending of the value of baudRate*/
static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->CTAR[0] &= ~SPI_CTAR_BR_MASK;
			SPI0->CTAR[0] |= baudRate;
			break;
		case(SPI_1):
				SPI1->CTAR[0] &= ~SPI_CTAR_BR_MASK;
				SPI1->CTAR[0] |= baudRate;
				break;
		case(SPI_2):
			SPI0->CTAR[0] &= ~SPI_CTAR_BR_MASK;
			SPI0->CTAR[0] |= baudRate;
			break;
		default:
			break;
	}
}
/**In this function we specified who is the most significant bit*/
static void SPI_mSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->CTAR[0] |= SPI_CTAR_LSBFE(msb);
			break;
		case(SPI_1):
			SPI1->CTAR[0] |= SPI_CTAR_LSBFE(msb);
			break;
		case(SPI_2):
			SPI2->CTAR[0] |= SPI_CTAR_LSBFE(msb);
			break;
		default:
			break;
	}
}
/**In this function we start the transference, first we activated that we are sending
then we activated so we can use it in the debug and finally we clear the flag*/
void SPI_startTranference(SPI_ChannelType channel)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->MCR &= ~(SPI_MCR_HALT_MASK);
			SPI0->MCR &= ~SPI_MCR_FRZ_MASK;
			SPI0->SR &= ~SPI_SR_TCF_MASK;
			break;
		case(SPI_1):
			SPI1->MCR &= ~(SPI_MCR_HALT_MASK);
			SPI1->MCR &= ~SPI_MCR_FRZ_MASK;
			SPI1->SR &= ~SPI_SR_TCF_MASK;
			break;
		case(SPI_2):
			SPI2->MCR &= ~(SPI_MCR_HALT_MASK);
			SPI2->MCR &= ~SPI_MCR_FRZ_MASK;
			SPI2->SR &= ~SPI_SR_TCF_MASK;
			break;
		default:
			break;
	}
}
/**In this part we stop the tranference*/
void SPI_stopTranference(SPI_ChannelType channel)
{
	switch(channel)
	{
		case(SPI_0):
			SPI0->MCR |= (SPI_MCR_HALT_MASK);
			break;
		case(SPI_1):
			SPI1->MCR |= (SPI_MCR_HALT_MASK);
			break;
		case(SPI_2):
			SPI2->MCR |= (SPI_MCR_HALT_MASK);
			break;
		default:
			break;
	}
}
/**In this part we send one by one byte the info to the LCD*/
void SPI_sendOneByte(uint8 Data)
{
	SPI0->PUSHR = (Data);
	while(FALSE == (SPI0->SR & SPI_SR_TCF_MASK));
	SPI0->SR |= SPI_SR_TCF_MASK;
}
/**In this function we initialize the SPI using all the previous function we declared here*/
void SPI_init(const SPI_ConfigType* configuration)
{
	SPI_clk(configuration->SPI_Channel);
	GPIO_clockGating(configuration->GPIOForSPI.GPIO_portName);
	GPIO_pinControlRegister(configuration->GPIOForSPI.GPIO_portName,configuration->GPIOForSPI.SPI_clk, &(configuration->pinConttrolRegisterPORTD));
	GPIO_pinControlRegister(configuration->GPIOForSPI.GPIO_portName,configuration->GPIOForSPI.SPI_Sout,&(configuration->pinConttrolRegisterPORTD));
	SPI_setMaster(configuration->SPI_Channel, configuration->SPI_Master);
	SPI_fIFO(configuration->SPI_Channel,configuration->SPI_EnableFIFO);
	SPI_enable(configuration->SPI_Channel);
	SPI_clockPolarity(configuration->SPI_Channel,configuration->SPI_Polarity);
	SPI_frameSize(configuration->SPI_Channel, configuration->frameSize);
	SPI_clockPhase(configuration->SPI_Channel, configuration->SPI_Phase);
	SPI_baudRate(configuration->SPI_Channel,configuration->baudrate);
	SPI_mSBFirst(configuration->SPI_Channel,configuration->SPI_LSMorMSB);
}
