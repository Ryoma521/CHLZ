#ifndef KERNEL_TIMERS_H
#define KERNEL_TIMERS_H

#include "Hal_types.h"
#include "Hal_timer.h"
#include "OnBoard.h"
#include "Kernel_Memory.h"
#include "Hal_mcu.h"
#include "ZComDef.h"
#include "Kernel.h"


typedef struct
{
  void *next;
  UINT16 timeout;
  UINT16 event_flag;
  byte task_id;
} kernelTimerRec_t;

/*********************************************************************
 * FUNCTIONS
 */

/*
* Initialization for the OSAL Timer System.
*/
extern void kernelTimerInit( void );
void kernel_update_timers( void );
byte kernel_start_timerEx( byte taskID, UINT16 event_id, UINT16 timeout_value );
byte kernel_stop_timerEx( byte task_id, UINT16 event_id );
uint32 kernel_GetSystemClock( void );
kernelTimerRec_t * kernelFindTimer( byte task_id, uint16 event_flag );

#endif
