
#ifndef LETIMER_H_
#define LETIMER_H_
#define LED0_on 84 //time in ms
#define LED0_total 2040 //time in ms
/*static bool volatile event_0 true;
static bool  volatile event_1 true;*/
void LETIMER_setup(void);
void LETIMER0_IRQHandler(void);
#endif
