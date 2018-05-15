
#include "letimer.h"
#include "cmu.h"
#include "em_core.h"
#include "sleep.h"
#include "sleep_timer.h"
#include "gpio.h"
#include "em_letimer.h"
#include "i2c.h"
#include "em_i2c.h"
uint8_t Frequency;
uint8_t period;
uint8_t timer_count;
uint8_t letimer_set;
#define SI7021_enable_port gpioPortD
#define SI7021_enable_pin 9
void LETIMER_setup(void)

{

  /* Enable necessary clocks */
  CMU_OscillatorEnable(cmuOsc_LFXO,true, true);
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
  CMU_ClockEnable(cmuClock_CORELE, true);
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  const LETIMER_Init_TypeDef letimerInit =
  {
  .enable         = true,                   /* Start counting when init completed. */
  .debugRun       = false,
#if defined(LETIMER_CTRL_RTCC0TEN)

 .rtcComp0Enable = false,                  /* Don't start counting on RTC COMP0 match. */
  .rtcComp1Enable = false,
  #endif/* Don't start counting on RTC COMP1 match. */
 .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
  .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
  .out0Pol        = 0,                      /* Idle value for output 0. */
  .out1Pol        = 0,                      /* Idle value for output 1. */
  .ufoa0          = letimerUFOANone,
  .ufoa1          = letimerUFOANone,
 .repMode        = letimerRepeatFree       /* Count until stopped */
 };

 Frequency = CMU_ClockFreqGet(cmuClock_LETIMER0);
  //LETIMER0_IntEnable(LETIMER_IFC_COMP0, LETIMER_IFC_COMP1);
 LETIMER_CompareSet(LETIMER0, 0, LED0_total);
 LETIMER_CompareSet(LETIMER0, 1, LED0_on);
 while(LETIMER0->SYNCBUSY);
 LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF|LETIMER_IFC_COMP0|LETIMER_IFC_COMP1);
 LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0);
 LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP1);
 //blockSleepMode(EM3);
 NVIC_EnableIRQ(LETIMER0_IRQn);
 LETIMER_Init(LETIMER0, &letimer_set);
 LETIMER_Enable(LETIMER0, true);

}


void LETIMER0_IRQHandler(void)
{
     uint16_t data=0;
      CORE_ATOMIC_IRQ_DISABLE();//Disabling interrupt for operation atomic
      int Flags = LETIMER_IntGet(LETIMER0);
      if(Flags & LETIMER_IF_COMP0) {
    	  LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0);
    	  temp_find_record(data);
    	  Temp_Sensor_off();


    }
    else if(Flags & LETIMER_IF_COMP1)
    {


    	LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1);
        Temp_Sensor_on();
    }
    else
    {
       LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1);
    }
     CORE_ATOMIC_IRQ_ENABLE(); //enabling interrupt
}
