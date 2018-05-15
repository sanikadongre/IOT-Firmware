/********************************************************************************************************
 * IP credit: The emlib .c and .h are being developed by SiliconLabs and the sleep routine functions
 * credit goes to SiliconLabs.This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 *
 ********************************************************************************************************/
#include <sleep_timer.h>
#include "em_emu.h"
#include "em_cmu.h"
#include <stdint.h>
#include <stdbool.h>
#include "em_core.h"
#include "em_letimer.h"
#define LED0_on 2000
#define LED0_off 80
uint8_t sleep_block[] = {0,0,0,0,0};



void blockSleepMode(Sleepstate em)
{
	CORE_ATOMIC_IRQ_DISABLE();
	sleep_block[em]++;
	CORE_ATOMIC_IRQ_ENABLE();
}

/*void peripheral_IRQHandler(){
	disable peripheral_call_interrupt;
	peripheral interrupt routine;
	unblockSleepMode(EMx);

}*/
void unblockSleepMode(Sleepstate em)
{
	CORE_ATOMIC_IRQ_DISABLE();
    if(sleep_block[em]> 0){
    	sleep_block[em]--;
    }
    CORE_ATOMIC_IRQ_ENABLE();
}

void sleep(void)
{
	if(sleep_block[0]>0){
		return;     //stay in EM0
	}
	else if(sleep_block[1]>0){
		return; //EM1 is blocked, so go into EM0
	}
	else if (sleep_block [2]>0){
		EMU_EnterEM1(); //EM2 is blocked, so go into EM1
	}
else if (sleep_block[3]>0){
	EMU_EnterEM2(true); //EM3 is blocked, so go into EM2
}
else {
		EMU_EnterEM3(true); //dont go in EM4
}
return;
}


