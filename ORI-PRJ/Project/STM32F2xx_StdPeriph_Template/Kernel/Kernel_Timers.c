/***********************************************************
文件名：	Kernel_Timer.c
修改时间：
功能：
************************************************************/

#include "Hal_types.h"
#include "Hal_timer.h"
#include "OnBoard.h"
#include "Kernel_Memory.h"
#include "Hal_mcu.h"
#include "ZComDef.h"
#include "Kernel.h"
#include "Kernel_Timers.h"
#include "Hal_drivers.h"

/*********************************************************************
 * TYPEDEFS
 */



/*********************************************************************
 * GLOBAL VARIABLES
 */

kernelTimerRec_t *timerHead;
uint32 tmr_count;          // Amount of time per tick - in micro-sec
uint16 tmr_decr_time;      // Decr_Time for system timer
byte timerActive;          // Flag if hw timer active

/*********************************************************************
 * LOCAL VARIABLES
 */
// Milliseconds since last reboot
static uint32 kernel_systemClock;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
kernelTimerRec_t  *kernelAddTimer( byte task_id, UINT16 event_flag, UINT16 timeout );
kernelTimerRec_t *kernelFindTimer( byte task_id, uint16 event_flag );
void kernelDeleteTimer( kernelTimerRec_t *rmTimer );
static void kernelTimerUpdate( uint16 time );

void kernel_timer_activate( byte turn_on );
void kernel_timer_hw_setup( byte turn_on );
void kernel_set_timer_interrupt( byte turn_on );
void kernel_retune_timers( void );


/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      kernelTimerInit
 *
 * @brief   Initialization for the KERNEL Timer System.
 *
 * @param   none
 *
 * @return
 */
void kernelTimerInit( void )
{
  // Initialize the rollover modulo
  tmr_count = TICK_TIME;
  tmr_decr_time = TIMER_DECR_TIME;

  // Initialize the system timer
  kernel_timer_activate( FALSE );
  timerActive = FALSE;

  kernel_systemClock = 0;
}

/*********************************************************************
 * @fn      kernelAddTimer
 *
 * @brief   Add a timer to the timer list.
 *          Ints must be disabled.
 *
 * @param   task_id
 * @param   event_flag
 * @param   timeout
 *
 * @return  kernelTimerRec_t * - pointer to newly created timer
 */
kernelTimerRec_t * kernelAddTimer( byte task_id, UINT16 event_flag, UINT16 timeout )
{
  kernelTimerRec_t *newTimer;
  kernelTimerRec_t *srchTimer;

  // Look for an existing timer first
  newTimer = kernelFindTimer( task_id, event_flag );
  if ( newTimer )
  {
    // Timer is found - update it.
    newTimer->timeout = timeout;

    return ( newTimer );
  }
  else
  {
    // New Timer
    newTimer = ( kernelTimerRec_t * ) kernel_mem_alloc( sizeof( kernelTimerRec_t ) );

    if ( newTimer )
    {
      // Fill in new timer
      newTimer->task_id = task_id;
      newTimer->event_flag = event_flag;
      newTimer->timeout = timeout;
      newTimer->next = (void *)NULL;

      // Does the timer list already exist
      if ( timerHead == NULL )
      {
        // Start task list
        timerHead = newTimer;
      }
      else
      {
        // Add it to the end of the timer list
        srchTimer = timerHead;

        // Stop at the last record
        while ( srchTimer->next )
          srchTimer = srchTimer->next;

        // Add to the list
        srchTimer->next = newTimer;
      }
      return ( newTimer );
    }
    else
      return ( (kernelTimerRec_t *)NULL );
  }
}

/*********************************************************************
 * @fn      kernelFindTimer
 *
 * @brief   Find a timer in a timer list.
 *          Ints must be disabled.
 *
 * @param   task_id
 * @param   event_flag
 *
 * @return  kernelTimerRec_t *
 */
kernelTimerRec_t *kernelFindTimer( byte task_id, uint16 event_flag )
{
  kernelTimerRec_t *srchTimer;

  // Head of the timer list
  srchTimer = timerHead;

  // Stop when found or at the end
  while ( srchTimer )
  {
    if ( srchTimer->event_flag == event_flag &&
         srchTimer->task_id == task_id )
      break;

    // Not this one, check another
    srchTimer = srchTimer->next;
  }

  return ( srchTimer );
}

/*********************************************************************
 * @fn      kernelDeleteTimer
 *
 * @brief   Delete a timer from a timer list.
 *          Ints must be disabled.
 *
 * @param   table
 * @param   rmTimer
 *
 * @return  none
 */
void kernelDeleteTimer( kernelTimerRec_t *rmTimer )
{
  kernelTimerRec_t *srchTimer;

  // Does the timer list really exist
  if ( (timerHead != NULL) && rmTimer )
  {
    // Add it to the end of the timer list
    srchTimer = timerHead;

    // First element?
    if ( srchTimer == rmTimer )
    {
      timerHead = rmTimer->next;
      kernel_mem_free( rmTimer );
    }
    else
    {
      // Stop when found or at the end
      while ( srchTimer->next && srchTimer->next != rmTimer )
        srchTimer = srchTimer->next;

      // Found?
      if ( srchTimer->next == rmTimer )
      {
        // Fix pointers
        srchTimer->next = rmTimer->next;

        // Deallocate the timer struct memory
        kernel_mem_free( rmTimer );
      }
    }
  }
}

/*********************************************************************
 * @fn      kernel_start_timerEx
 *
 * @brief
 *
 *   This function is called to start a timer to expire in n mSecs.
 *   When the timer expires, the calling task will get the specified event.
 *
 * @param   byte taskID - task id to set timer for
 * @param   UINT16 event_id - event to be notified with
 * @param   UNINT16 timeout_value - in milliseconds.
 *
 * @return  ZSUCCESS, or NO_TIMER_AVAIL.
 */
byte kernel_start_timerEx( byte taskID, UINT16 event_id, UINT16 timeout_value )
{
  halIntState_t intState;
  kernelTimerRec_t *newTimer;

  HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
  HAL_IE_Disable();

  // Add timer
  newTimer = kernelAddTimer( taskID, event_id, timeout_value );
  if ( newTimer )
  {
    // Does the timer need to be started?
    if ( timerActive == FALSE )//如果还没有启动硬件定时器，则立即启动之
    {
      kernel_timer_activate( TRUE );//启动硬件定时器
    }
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // Re-enable interrupts.
  HAL_IE_Enable();

  return ( (newTimer != NULL) ? ZSUCCESS : NO_TIMER_AVAIL );
}

/*********************************************************************
 * @fn      kernel_stop_timerEx
 *
 * @brief
 *
 *   This function is called to stop a timer that has already been started.
 *   If ZSUCCESS, the function will cancel the timer and prevent the event
 *   associated with the timer from being set for the calling task.
 *
 * @param   byte task_id - task id of timer to stop
 * @param   UINT16 event_id - identifier of the timer that is to be stopped
 *
 * @return  ZSUCCESS or INVALID_EVENT_ID
 */
byte kernel_stop_timerEx( byte task_id, UINT16 event_id )
{
  halIntState_t intState;
  kernelTimerRec_t *foundTimer;

  HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
  HAL_IE_Disable();

  // Find the timer to stop
  foundTimer = kernelFindTimer( task_id, event_id );
  if ( foundTimer )
  {
    kernelDeleteTimer( foundTimer );
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // Re-enable interrupts.
  HAL_IE_Enable();

  return ( (foundTimer != NULL) ? ZSUCCESS : INVALID_EVENT_ID );
}

/*********************************************************************
 * @fn      kernel_get_timeoutEx
 *
 * @brief
 *
 * @param   byte task_id - task id of timer to check
 * @param   UINT16 event_id - identifier of timer to be checked
 *
 * @return  Return the timer's tick count if found, zero otherwise.
 */
UINT16 kernel_get_timeoutEx( byte task_id, UINT16 event_id )
{
  halIntState_t intState;
  uint16 rtrn = 0;
  kernelTimerRec_t *tmr;

  HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
  HAL_IE_Disable();

  tmr = kernelFindTimer( task_id, event_id );

  if ( tmr )
  {
    rtrn = tmr->timeout;
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // Re-enable interrupts.
  HAL_IE_Enable();

  return rtrn;
}

/*********************************************************************
 * @fn      kernel_timer_activate
 *
 * @brief
 *
 *   Turns the hardware timer on or off
 *
 * @param  byte turn_on - false - turn off, true - turn on
 *
 * @return  none
 */
void kernel_timer_activate( byte turn_on )
{
  kernel_timer_hw_setup( turn_on );
  timerActive = turn_on;
}

/*********************************************************************
 * @fn      kernel_timer_num_active
 *
 * @brief
 *
 *   This function counts the number of active timers.
 *
 * @return  byte - number of timers
 */
byte kernel_timer_num_active( void )
{
  halIntState_t intState;
  byte num_timers = 0;
  kernelTimerRec_t *srchTimer;

  HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
  HAL_IE_Disable();

  // Head of the timer list
  srchTimer = timerHead;

  // Count timers in the list
  while ( srchTimer != NULL )
  {
    num_timers++;
    srchTimer = srchTimer->next;
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // Re-enable interrupts.
  HAL_IE_Enable();

  return num_timers;
}

/*********************************************************************
 * @fn      kernel_timer_hw_setup
 *
 * @brief
 *
 *   Setup the timer hardware.
 *
 * @param  byte turn_on
 *
 * @return  void
 */
void kernel_timer_hw_setup( byte turn_on )
{
  if (turn_on)
  {
    HalTimerStart ( tmr_count );//启动定时器
  }
  else
  {
    HalTimerStop ( );//停止硬件定时器功能
  }
}

/*********************************************************************
 * @fn      kernelTimerUpdate
 *
 * @brief   Update the timer structures for a timer tick.
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
static void kernelTimerUpdate( uint16 updateTime )
{
  halIntState_t intState;
  kernelTimerRec_t *srchTimer;
  kernelTimerRec_t *prevTimer;
  kernelTimerRec_t *saveTimer;

  HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
  HAL_IE_Disable();

  // Update the system time
  kernel_systemClock += updateTime;

  // Look for open timer slot
  if ( timerHead != NULL )
  {
    // Add it to the end of the timer list
    srchTimer = timerHead;
    prevTimer = (void *)NULL;

    // Look for open timer slot
    while ( srchTimer )
    {
      // Decrease the correct amount of time
      if (srchTimer->timeout <= updateTime)
        srchTimer->timeout = 0;
      else
        srchTimer->timeout = srchTimer->timeout - updateTime;

      // When timeout, execute the task 到期，执行任务
      if ( srchTimer->timeout == 0 )
      {
      	//printf("time over\n");
        kernel_set_event( srchTimer->task_id, srchTimer->event_flag );//向任务发送事件

        // Take out of list
        if ( prevTimer == NULL )
          timerHead = srchTimer->next;
        else
          prevTimer->next = srchTimer->next;

        // Next
        saveTimer = srchTimer->next;

        // Free memory
        kernel_mem_free( srchTimer );//释放计时器结构体占据的空间

        srchTimer = saveTimer;
      }
      else
      {
        // Get next
        prevTimer = srchTimer;
        srchTimer = srchTimer->next;
      }
    }
  }

  HAL_EXIT_CRITICAL_SECTION( intState );   // Re-enable interrupts.
  HAL_IE_Enable();
}

/*********************************************************************
 * @fn      kernel_update_timers
 *
 * @brief   Update the timer structures for timer ticks.
 *
 * @param   none
 *
 * @return  none
 */
void kernel_update_timers( void )
{
  kernelTimerUpdate( tmr_decr_time );
}

/*********************************************************************
 * @fn      kernel_GetSystemClock()
 *
 * @brief   Read the local system clock.
 *
 * @param   none
 *
 * @return  local clock in milliseconds
 */
uint32 kernel_GetSystemClock( void )
{
  return ( kernel_systemClock );
}
