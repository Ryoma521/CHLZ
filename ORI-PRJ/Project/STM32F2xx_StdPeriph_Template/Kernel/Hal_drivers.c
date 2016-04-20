/***********************************************************
�ļ����ƣ�	Hal_drivers.c
�޸�ʱ�䣺
���ܣ�
************************************************************/


#include "misc.h"      //nvic
#include "Hal_timer.h"
#include "core_cm3.h" //systick

extern void ReConnect(void);

/**************************************************************************************************
 * @fn      Hal_ProcessPoll
 *
 * @brief   This routine will be called by KERNEL to poll UART, TIMER...
 *
 * @param   task_id - Hal TaskId
 *
 * @return  None
 **************************************************************************************************/
void Hal_ProcessPoll ( void )
{

  /* Timer Poll */
  
  //HalTimerTick();
  //ReConnect();
  //SysTick_CounterCmd(SysTick_CTRL_ENABLE_Msk);
}




void HAL_IE_Enable(void)		//ע��������������Ҫ����Ӳ��ʵ��
{
  //NVIC_RESETPRIMASK();
}

void HAL_IE_Disable(void)
{
  //NVIC_SETPRIMASK();
}
