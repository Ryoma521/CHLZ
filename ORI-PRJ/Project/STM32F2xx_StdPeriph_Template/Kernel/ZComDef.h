#ifndef ZCOMDEF_H
#define ZCOMDEF_H

/*** Return Values ***/
/*需要保留，在Kernel部分使用*/
#define ZSUCCESS                  0
#define INVALID_TASK              1
#define MSG_BUFFER_NOT_AVAIL      2
#define INVALID_MSG_POINTER       3
#define INVALID_LEN               4
#define INVALID_SENDING_TASK      5
#define INVALID_DESTINATION_TASK  6
#define INVALID_EVENT_ID          7
#define INVALID_TIMEOUT_VALUE     8
#define INVALID_INTERRUPT_ID      9
#define INVALID_ISR_PTR           10
#define INVALID_TASK_TABLE        11
#define NO_TIMER_AVAIL            12
#define INVALID_MSG_LEN           13
#define NOT_READY                 14
#define NV_ITEM_UNINIT            15
#define NV_OPER_FAILED            16
#define INVALID_MEM_SIZE          17
#define NV_BAD_ITEM_LEN           18


/*********************************************************************
 * Global System Events
 */

#define SYS_EVENT_MSG               0x8000  // A message is waiting event
#define SYS_EVENT_MEMORY_PANIC			0x4000	//内存耗尽











#endif
