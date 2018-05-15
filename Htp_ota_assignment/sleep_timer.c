/*
 * sleep_timer.c
 *  Author: Sanika
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
/**IP Credit:The sleep routines are being developed by Silicon Labs*/

#include "sleep_timer.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include <stdint.h>
#include "em_emu.h"
#include "em_core.h"
#include "em_cmu.h"
#include "em_assert.h"
#include <stdbool.h>
#include "sleep.h"
static uint8_t sleep_block[]={0,0,0,0,0};
void unblockSleepMode(Sleepstate em)
{
	CORE_ATOMIC_IRQ_DISABLE();
	sleep_block[em]--;
	CORE_ATOMIC_IRQ_ENABLE();
	return;
}

void blockSleepMode(Sleepstate em)
{
	CORE_ATOMIC_IRQ_DISABLE();
	sleep_block[em]++;
	CORE_ATOMIC_IRQ_ENABLE();
	return;
}

void sleep(void)
{
	if(sleep_block[0]>0)
	{
		return; //staying EM0
	}
	else if(sleep_block[1]>0)
	{
		return; //EM1 is blocked, so go into EM0
	}
	else if(sleep_block[2]>0)
	{
		EMU_EnterEM1(); //EM2 is blocked, so go into EM1
	}
	else if(sleep_block[3]>0)
	{
		EMU_EnterEM2(true); //EM3 is blocked, so go into EM2
	}
	else
	{
		EMU_EnterEM3(true); //Don't go into EM4
	}
	return;
}

