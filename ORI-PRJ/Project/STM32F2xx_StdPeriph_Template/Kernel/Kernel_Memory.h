#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include "Hal_types.h"

void kernel_mem_set( uint8 * ptr, uint8 content, uint16 len );
void kernel_mem_init( void );
void * kernel_mem_alloc( uint16 size );
void kernel_mem_free( void * ptr );
void Dump_Heap_State(void);
void Show_Index(void * p);
void Dump_Heap_Info(void);

#endif

