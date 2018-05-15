/*
 * letimer.c
 * Author: Sanika
 */

/***** IP Credit: The emlib functions are being developed by Silicon Labs*/
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include <stdint.h>
#include "em_letimer.h"
#include "em_emu.h"
#include "em_core.h"
#include "em_cmu.h"
#include "em_assert.h"
#include <stdbool.h>
#include "sleep_timer.h"
#include "sleep.h"
#include "i2c.h"
#include "native_gecko.h"
uint16_t g_external_event_status;
uint8_t BLE_CON;

void LETIMER_setup(void)
{
	const LETIMER_Init_TypeDef letimerInit=
	{
		.enable = true,         /**< Start counting when init completed. */
		.debugRun = false,       /**< Counter shall keep running during debug halt. */
		#if defined(LETIMER_CTRL_RTCC0TEN)
		.rtcComp0Enable = false, /**< Start counting on RTC COMP0 match. */
		.rtcComp1Enable = false, /**< Start counting on RTC COMP1 match. */
		#endif
		.comp0Top = true,       /**< Load COMP0 register into CNT when counter underflows. */
		.bufTop = false,         /**< Load COMP1 into COMP0 when REP0 reaches 0. */
		.out0Pol = false,        /**< Idle value for output 0. */
		.out1Pol = false,       /**< Idle value for output 1. */
		.ufoa0 = letimerUFOANone,          /**< Underflow output 0 action. */
		.ufoa1 = letimerUFOANone,          /**< Underflow output 1 action. */
		.repMode = letimerRepeatFree,        /** Repeat mode. */
	};
	LETIMER_CompareSet(LETIMER0,0,LED_Total);
	LETIMER_CompareSet(LETIMER0,1,LED_On);
	while (LETIMER0->SYNCBUSY);
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF|LETIMER_IFC_COMP1|LETIMER_IFC_COMP0);
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1|LETIMER_IEN_COMP0);
	NVIC_EnableIRQ(LETIMER0_IRQn);
	LETIMER_Init(LETIMER0,&letimerInit);
	LETIMER_Enable(LETIMER0, true);
}

void LETIMER0_IRQHandler(void)
{

	CORE_ATOMIC_IRQ_DISABLE();
	int int_flag;
	int Flags = LETIMER_IntGet(LETIMER0);
	int_flag = LETIMER0->IF;
	LETIMER0->IFC = int_flag;
	if (Flags & LETIMER_IF_COMP0)
		{
			LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0);
			//event_2 = 1;
		  // unblockSleepMode(EM3);
		   //unblockSleepMode(EM2);
		  // g_external_event_status |= LETIMER0_EXT_EVT_COMP;
           gecko_external_signal(1);
	    }
	    else if(Flags & LETIMER_IF_COMP1)
	    {
	    	LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP1);
	    	event_1 = 0x35;
	    	//event_1 = 1;
	    	gecko_external_signal(1);
	    }
	    else
	    {
	    	LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF|LETIMER_IFC_COMP1|LETIMER_IFC_COMP0);
	    }
	gecko_cmd_le_connection_get_rssi(BLE_CON);
    CORE_ATOMIC_IRQ_ENABLE();

}


