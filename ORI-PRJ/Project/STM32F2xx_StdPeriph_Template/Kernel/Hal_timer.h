#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include "Hal_types.h"

typedef struct
{
	uint8 start_timer:1;
	uint8 timer_overflow:1;
	uint32 timer_per_tick;	//µ¥Î»uSecond
	uint32 last_time;
} cygwin_timer_t;

/* Error Code */
#define HAL_TIMER_OK              0x00

void HalTimerTick(void);
uint8 HalTimerStart (uint32 timePerTick);
uint8 HalTimerStop( void );
void halProcessTimer(void);
uint32 CygwinGetTime( void );
uint32 HalGetTime( void );
void CygwinTimerHandler( void );

#define MACTIMER_MAX_VALUE 0xFFFFFFFF 
#define halTimeNowDelta(x) (((HalGetTime()-(x))& MACTIMER_MAX_VALUE))


#endif
