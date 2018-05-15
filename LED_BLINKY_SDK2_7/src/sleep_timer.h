#ifndef SLEEP_TIMER_H_
#define SLEEP_TIMER_H_

typedef enum {
	EM0,
	EM1,
	EM2,
	EM3,
	EM4
}Sleepstate;

void unblockSleepMode(Sleepstate em);
void blockSleepMode(Sleepstate em);
void sleep(void);

#endif
