/***********************************************************
�ļ�����	Kernel.h
�޸�ʱ�䣺
���ܣ�		
************************************************************/

#ifndef KERNEL_H
#define KERNEL_H

#include "Hal_types.h"

/***********************************************************
                            ����
************************************************************/
typedef struct
{
  void   *next;	
  uint16 len;		
  uint8  dest_id;
} kernel_msg_hdr_t;

typedef struct
{
  uint8  event:6;
  uint8  status:2;
} kernel_event_hdr_t;

typedef void * kernel_msg_q_t;

/***********************************************************
                            ��
************************************************************/
#define KERNEL_MSG_NEXT(msg_ptr)      ((kernel_msg_hdr_t *) (msg_ptr) - 1)->next
#define KERNEL_MSG_Q_INIT(q_ptr)      *(q_ptr) = NULL
#define KERNEL_MSG_Q_EMPTY(q_ptr)     (*(q_ptr) == NULL)
#define KERNEL_MSG_Q_HEAD(q_ptr)      (*(q_ptr))
#define KERNEL_MSG_LEN(msg_ptr)      ((kernel_msg_hdr_t *) (msg_ptr) - 1)->len
#define KERNEL_MSG_ID(msg_ptr)      ((kernel_msg_hdr_t *) (msg_ptr) - 1)->dest_id

void * kernel_memcpy( void * dst, const void * src, uint16 len );
byte kernel_msg_send( byte destination_task, byte *msg_ptr );
byte * kernel_msg_allocate( uint16 len );
byte kernel_msg_deallocate( byte *msg_ptr );
byte *kernel_msg_receive( byte task_id );
void kernel_msg_enqueue( kernel_msg_q_t *q_ptr, void *msg_ptr );
void kernel_msg_extract( kernel_msg_q_t *q_ptr, void *msg_ptr, void *prev_ptr );
byte kernel_set_event( byte task_id, UINT16 event_flag );

byte kernel_init_system( void );
void kernel_start_system( void );

#endif


