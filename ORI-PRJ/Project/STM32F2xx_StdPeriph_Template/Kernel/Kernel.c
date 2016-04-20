/************************************************************
文件名：	Kernel.c
修改时间：
功能：
*************************************************************/

/************************************************************
                        头文件
*************************************************************/


#include "Hal_types.h"
#include "Kernel.h"
#include "Kernel_Memory.h"
#include "Kernel_Tasks.h"
#include "Kernel_Timers.h"
#include "ZComDef.h"
#include "Hal_mcu.h"
#include "Hal_drivers.h"




/************************************************************
                        外部变量
*************************************************************/
extern const uint8 tasksCnt;
extern uint16 *tasksEvents;
//extern uint16 *tasksEventsCopy;

/************************************************************
                        全局变量
*************************************************************/
//消息链表头
kernel_msg_q_t kernel_qHead;


/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * HELPER FUNCTIONS
 */
 
/*********************************************************************
 * @fn      kernel_memcpy
 *
 * @brief
 *
 *   Generic memory copy.
 *
 *   Note: This function differs from the standard memcpy(), since
 *         it returns the pointer to the next destination byte. The
 *         standard memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void * kernel_memcpy( void * dst, const void * src, uint16 len )
{
  uint8 *pDst;
  const uint8 *pSrc;

  pSrc = src;
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc++;

  return ( pDst );
}

/*********************************************************************
 * API FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_msg_allocate
 *
 * @brief
 *
 *    This function is called by a task to allocate a message buffer
 *    into which the task will encode the particular message it wishes
 *    to send.  This common buffer scheme is used to strictly limit the
 *    creation of message buffers within the system due to RAM size
 *    limitations on the microprocessor.   Note that all message buffers
 *    are a fixed size (at least initially).  The parameter len is kept
 *    in case a message pool with varying fixed message sizes is later
 *    created (for example, a pool of message buffers of size LARGE,
 *    MEDIUM and SMALL could be maintained and allocated based on request
 *    from the tasks).
 *
 *
 * @param   byte len  - wanted buffer length
 *
 *
 * @return  pointer to allocated buffer or NULL if allocation failed.
 */


byte * kernel_msg_allocate( uint16 len )//分配消息空间:包括消息头和消息内容
{
	kernel_msg_hdr_t * hdr;

  if ( len == 0 )
    return ( NULL );

  hdr = (kernel_msg_hdr_t *) kernel_mem_alloc( (short)(len + sizeof( kernel_msg_hdr_t )) );//内容空间和头空间
  if ( hdr )
  {//初始化
    hdr->next = NULL;//指向下一个消息
    hdr->len = len;//当前消息内容长度
    hdr->dest_id = TASK_NO_TASK;//应该处理当前消息的任务号
    
#if defined( OSAL_TOTAL_MEM )
    osal_msg_cnt++;
#endif
    return ( (byte *) (hdr + 1) );//跳过消息头，返回指向当前消息内容的指针
  }
  else
    return ( NULL );
}

/*********************************************************************
 * @fn      kernel_msg_deallocate
 *
 * @brief
 *
 *    This function is used to deallocate a message buffer. This function
 *    is called by a task (or processing element) after it has finished
 *    processing a received message.
 *
 *
 * @param   byte *msg_ptr - pointer to new message buffer
 *
 * @return  ZSUCCESS, INVALID_MSG_POINTER
 */
byte kernel_msg_deallocate( byte *msg_ptr )
{
  byte *x;

  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  // don't deallocate queued buffer
  if ( KERNEL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
    return ( MSG_BUFFER_NOT_AVAIL );

  x = (byte *)((byte *)msg_ptr - sizeof( kernel_msg_hdr_t ));//找到消息头指针

  kernel_mem_free( (void *)x );//将消息头和消息内容占据的空间一起释放


  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      kernel_msg_send
 *
 * @brief
 *
 *    This function is called by a task to send a command message to
 *    another task or processing element.  The sending_task field must
 *    refer to a valid task, since the task ID will be used
 *    for the response message.  This function will also set a message
 *    ready event in the destination tasks event list.
 *
 *
 * @param   byte destination task - Send msg to?  Task ID
 * @param   byte *msg_ptr - pointer to new message buffer
 * @param   byte len - length of data in message
 *
 * @return  ZSUCCESS, INVALID_SENDING_TASK, INVALID_DESTINATION_TASK,
 *          INVALID_MSG_POINTER, INVALID_LEN
 */
byte kernel_msg_send( byte destination_task, byte *msg_ptr )//发送消息之前必须调用kernel_msg_allocate为消息分配空间
{
  if ( msg_ptr == NULL )
  {
    return ( INVALID_MSG_POINTER );
  }

  if ( destination_task >= tasksCnt )
  {
    kernel_msg_deallocate( msg_ptr );
    return ( INVALID_TASK );
  }

  // Check the message header
  if ( KERNEL_MSG_NEXT( msg_ptr ) != NULL ||
       KERNEL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
  {
    kernel_msg_deallocate( msg_ptr );


    return ( INVALID_MSG_POINTER );

  }

  KERNEL_MSG_ID( msg_ptr ) = destination_task;//应该处理这个消息的任务号

  // queue message
  kernel_msg_enqueue( &kernel_qHead, msg_ptr );//添加进消息队列

  // Signal the task that a message is waiting
  kernel_set_event( destination_task, SYS_EVENT_MSG );//置事件标志，告知任务需要处理消息

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      kernel_msg_receive
 *
 * @brief
 *
 *    This function is called by a task to retrieve a received command
 *    message. The calling task must deallocate the message buffer after
 *    processing the message using the osal_msg_deallocate() call.
 *
 * @param   byte task_id - receiving tasks ID
 *
 * @return  *byte - message information or NULL if no message
 */
byte *kernel_msg_receive( byte task_id )//返回指针直接指向消息内容，不包括消息头域
{
  kernel_msg_hdr_t *listHdr;
  kernel_msg_hdr_t *prevHdr=0;
 
  halIntState_t   intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);
  HAL_IE_Disable();


  // Point to the top of the queue
  listHdr = kernel_qHead;

  // Look through the queue for a message that belongs to the asking task
  while ( listHdr != NULL )
  {
    if ( (listHdr - 1)->dest_id == task_id )
    {
      break;
    }
    prevHdr = listHdr;
    listHdr = KERNEL_MSG_NEXT( listHdr );
  }

  // Did we find a message?
  if ( listHdr == NULL )
  {
    // Release interrupts

    HAL_EXIT_CRITICAL_SECTION(intState);
    HAL_IE_Enable();

    return NULL;
  }

  // Take out of the link list 从队列中抽取处一个消息，但没有释放空间
  kernel_msg_extract( &kernel_qHead, listHdr, prevHdr );

  // Release interrupts

  HAL_EXIT_CRITICAL_SECTION(intState);
  HAL_IE_Enable();


  return ( (byte*) listHdr );//返回指针跳过了消息头，直接指向消息内容
}

/*********************************************************************
 * @fn      kernel_msg_enqueue
 *
 * @brief
 *
 *    This function enqueues an KERNEL message into an KERNEL queue.向队列中添加消息
 *
 * @param   osal_msg_q_t *q_ptr - KERNEL queue
 * @param   void *msg_ptr  - HERNEL message
 *
 * @return  none
 */
void kernel_msg_enqueue( kernel_msg_q_t *q_ptr, void *msg_ptr )
{
  void *list;


  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);
  HAL_IE_Disable();


  // If first message in queue 作为第一个消息插入队列
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
  }
  else
  {
    // Find end of queue
    for ( list = *q_ptr; KERNEL_MSG_NEXT( list ) != NULL; list = KERNEL_MSG_NEXT( list ) );

    // Add message to end of queue
    KERNEL_MSG_NEXT( list ) = msg_ptr;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
  HAL_IE_Enable();

}

/*********************************************************************
 * @fn      kernel_msg_dequeue
 *
 * @brief
 *
 *    This function dequeues an KERNEL message from an KERNEL queue.
 *
 * @param   osal_msg_q_t *q_ptr - KERNEL queue
 *
 * @return  void * - pointer to KERNEL message or NULL of queue is empty.
 */
void *kernel_msg_dequeue( kernel_msg_q_t *q_ptr )
{
  void *msg_ptr;
  

  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);
  HAL_IE_Disable();


  if ( *q_ptr == NULL )
  {
    HAL_EXIT_CRITICAL_SECTION(intState);
    HAL_IE_Enable();
    return NULL;
  }

  // Dequeue message
  msg_ptr = *q_ptr;
  *q_ptr = KERNEL_MSG_NEXT( msg_ptr );
  KERNEL_MSG_NEXT( msg_ptr ) = NULL;
  KERNEL_MSG_ID( msg_ptr ) = TASK_NO_TASK;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
  HAL_IE_Enable();

  return msg_ptr;
}

/*********************************************************************
 * @fn      kernel_msg_push
 *
 * @brief
 *
 *    This function pushes an KERNEL message to the head of an KERNEL
 *    queue.
 *
 * @param   kernel_msg_q_t *q_ptr - KERNEL queue
 * @param   void *msg_ptr  - KERNEL message
 *
 * @return  none
 */
void kernel_msg_push( kernel_msg_q_t *q_ptr, void *msg_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);
  HAL_IE_Disable();

  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
  }
  else
  {
    // Push message to head of queue
    KERNEL_MSG_NEXT( msg_ptr ) = *q_ptr;
    *q_ptr = msg_ptr;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
  HAL_IE_Enable();
}

/*********************************************************************
 * @fn      kernel_msg_extract
 *
 * @brief
 *
 *    This function extracts and removes an KERNEL message from the
 *    middle of an KERNEL queue.
 *
 * @param   kernel_msg_q_t *q_ptr - KERNEL queue
 * @param   void *msg_ptr  - KERNEL message to be extracted
 * @param   void *prev_ptr  - KERNEL message before msg_ptr in queue
 *
 * @return  none
 */
void kernel_msg_extract( kernel_msg_q_t *q_ptr, void *msg_ptr, void *prev_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);
  HAL_IE_Disable();

  if ( msg_ptr == *q_ptr )
  {
    // remove from first
    *q_ptr = KERNEL_MSG_NEXT( msg_ptr );
  }
  else
  {
    // remove from middle
    KERNEL_MSG_NEXT( prev_ptr ) = KERNEL_MSG_NEXT( msg_ptr );
  }
  KERNEL_MSG_NEXT( msg_ptr ) = NULL;
  KERNEL_MSG_ID( msg_ptr ) = TASK_NO_TASK;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
  HAL_IE_Enable();
}

/*********************************************************************
 * @fn      kernel_msg_enqueue_max
 *
 * @brief
 *
 *    This function enqueues an KERNEL message into an KERNEL queue if
 *    the length of the queue is less than max.
 *
 * @param   kernel_msg_q_t *q_ptr - KERNEL queue
 * @param   void *msg_ptr  - KERNEL message
 * @param   byte max - maximum length of queue
 *
 * @return  TRUE if message was enqueued, FALSE otherwise
 */
byte kernel_msg_enqueue_max( kernel_msg_q_t *q_ptr, void *msg_ptr, byte max )
{
  void *list;
  byte ret = FALSE;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);
  HAL_IE_Disable();

  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
    ret = TRUE;
  }
  else
  {
    // Find end of queue or max
    list = *q_ptr;
    max--;
    while ( (KERNEL_MSG_NEXT( list ) != NULL) && (max > 0) )
    {
      list = KERNEL_MSG_NEXT( list );
      max--;
    }

    // Add message to end of queue if max not reached
    if ( max != 0 )
    {
      KERNEL_MSG_NEXT( list ) = msg_ptr;
      ret = TRUE;
    }
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
  HAL_IE_Enable();

  return ret;
}

/*********************************************************************
 * @fn      kernel_set_event
 *
 * @brief
 *
 *    This function is called to set the event flags for a task.  The
 *    event passed in is OR'd into the task's event variable.
 *
 * @param   byte task_id - receiving tasks ID
 * @param   byte event_flag - what event to set
 *
 * @return  ZSUCCESS, INVALID_TASK
 */
byte kernel_set_event( byte task_id, UINT16 event_flag )//置事件标志位，告知任务处理某个事件
{
  if ( task_id < tasksCnt )
  {
  halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    HAL_IE_Disable();
    tasksEvents[task_id] |= event_flag;  // Stuff the event bit(s)
    //tasksEventsCopy[task_id] |= event_flag;  // Stuff the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    HAL_IE_Enable();
  }
   else
    return ( INVALID_TASK );

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      kernel_init_system
 *
 * @brief
 *
 *   This function initializes the "task" system by creating the
 *   tasks defined in the task table (OSAL_Tasks.h).
 *
 * @param   void
 *
 * @return  ZSUCCESS
 */
byte kernel_init_system( void )
{
  // Initialize the Memory Allocation System
  kernel_mem_init();

  // Initialize the message queue
  kernel_qHead = NULL;

  // Initialize the timers
  kernelTimerInit();

  // Initialize the Power Management System
  //osal_pwrmgr_init();

  // Initialize the system tasks.初始化系统任务
  kernelInitTasks();

  // Setup efficient search for the first free block of heap.
  //osal_mem_kick();

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      kernel_start_system
 *
 * @brief
 *
 *   This function is the main loop function of the task system.  It
 *   will look through all task events and call the task_event_processor()
 *   function for the task with the event.  If there are no events (for
 *   all tasks), this function puts the processor into Sleep.
 *   This Function doesn't return.
 *
 * @param   void
 *
 * @return  none
 */
void kernel_start_system( void )
{
	
  for(;;)  // Forever Loop
  {
    uint8 idx = 0;

    Hal_ProcessPoll();  // This replaces MT_SerialPoll() and osal_check_timer().

		//DEBUG("Can reach here 4.\n");
    do 
    {
    	
      if (tasksEvents[idx])  // Task is highest priority that is ready.
      {
      	
        break;
      }
    } while (++idx < tasksCnt);
		//DEBUG("Can reach here 5.\n");
    if (idx < tasksCnt)
    {
    	
      uint16 events;
      halIntState_t intState;

      HAL_ENTER_CRITICAL_SECTION(intState);
      HAL_IE_Disable();
      events = tasksEvents[idx];
      tasksEvents[idx] = 0;  // Clear the Events for this task.清空所有事件
      HAL_EXIT_CRITICAL_SECTION(intState);
      HAL_IE_Enable();

      events = (tasksArr[idx])( idx, events );
			
      HAL_ENTER_CRITICAL_SECTION(intState);
      HAL_IE_Disable();
     	tasksEvents[idx] |= events;  // Add back unprocessed events to the current task.添加回尚未处理的事件
     	//tasksEventsCopy[idx] |= events; 
     	
     	
     	
      HAL_EXIT_CRITICAL_SECTION(intState);
      HAL_IE_Enable();
      
      
    }
    //DEBUG("Can reach here 6.\n");
  }
  
  //usleep(100000);
}

