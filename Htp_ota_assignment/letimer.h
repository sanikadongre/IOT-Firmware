/*
 * letimer.h
 *Author: Sanika
 */
/*****IP Credit:The sleep routines are being dveloped by Silicon Labs***/
#include "native_gecko.h"
#ifndef Letimer
#define Letimer
#define LED_Total (4000) //mS
#define LED_On (80)	//mS
#define LETIMER0_EXT_EVT_COMP (0x01)
void LETIMER0_IRQHandler(void);
void LETIMER_setup(void);
volatile uint8_t event_1;
//volatile uint8_t event_2;

#endif
