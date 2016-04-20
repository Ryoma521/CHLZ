/***********************************************************
�ļ�����	Hal_timer.c
�޸�ʱ�䣺
���ܣ�		��Cygwin�з���ʱʹ�õļ�ʱ������ʱ�ļ�ʱ��û���ж�ģʽ
************************************************************/



#include "Hal_timer.h"
#include "Kernel_Timers.h"


cygwin_timer_t CygwinTimer;

/*********************************************************************
 * @fn      HalTimerInit
 *
 * @brief   Initialize Timer Service
 *
 * @param   None
 *
 * @return  None
 */
void HalTimerInit (void)
{
	CygwinTimer.start_timer = FALSE;
	CygwinTimer.timer_overflow = FALSE;
	CygwinTimer.timer_per_tick = 0xFFFFFFFF;
}

/***************************************************************************************************
 * @fn      HalTimerStart
 *
 * @brief   Start the Timer Service
 *
 * @param   ��������ģ��ʱ���жϵļ������λΪ΢�롣
 *          timerPerTick - number of micro sec per tick, (ticks x prescale) / clock = usec/tick
 *
 * @return  Status - OK or Not OK
 **************************************************************************************************/
uint8 HalTimerStart (uint32 timePerTick)
{
	
	return HAL_TIMER_OK;
}

/***************************************************************************************************
 * @fn      HalTimerStop
 *
 * @brief   Stop the Timer Service
 *
 * @param   
 *
 * @return  Status - OK or Not OK
 **************************************************************************************************/
uint8 HalTimerStop( void )
{

	
	return HAL_TIMER_OK;
}

/***************************************************************************************************
 * @fn      HalTimerTick
 *
 * @brief   Check the counter for expired counter.
 *
 * @param   None
 *
 * @return  None
 **************************************************************************************************/
void HalTimerTick (void)
{
    halProcessTimer();

}



/***************************************************************************************************
 * @fn      halProcessTimer
 *
 * @brief   Processes Timer Events.
 *
 * @param
 *
 * @return
 **************************************************************************************************/
void halProcessTimer(void)
{
  kernel_update_timers();	
}








