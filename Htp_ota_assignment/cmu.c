/*
 * cmu.c
 *
 *  Created on: Feb 21, 2018
 *      Author: dongr
 */


//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"
/***** IP Credit: The emlib functions i.e. em_cmu.h are developed by Silicon Labs
*****/
//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void){
	// Peripheral clocks enabled
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
}

void cmu_letimer(CMU_ClkDiv_TypeDef PRESCALE)
{
	CMU_OscillatorEnable(cmuOsc_LFXO,true,true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
	CMU_ClockEnable(cmuClock_LETIMER0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockDivSet(cmuClock_LETIMER0,PRESCALE);
	}

void cmu_i2c(CMU_ClkDiv_TypeDef PRESCALE)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_I2C0, true);
	//CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_HFRCO);
	//CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_HFXO);
	//CMU_ClockDivSet(cmuClock_HFPER, PRESCALE);


}
