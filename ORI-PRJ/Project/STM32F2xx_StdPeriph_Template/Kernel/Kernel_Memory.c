#include "Hal_types.h"
#include "OnBoard.h"
#include "Hal_mcu.h"
#include "Hal_drivers.h"
#include "Kernel_Memory.h"

typedef uint16 MEMHDR;

#define MEMHDR_FREE_MASK 0x80

#define MEMHDR_GET_FREE( x ) ( *( x+1 ) & MEMHDR_FREE_MASK )
#define MEMHDR_CLR_FREE( x )  *( x+1 ) = *( x+1 ) & ( ~MEMHDR_FREE_MASK )
#define MEMHDR_SET_FREE( x )  *( x+1 ) = *( x+ 1) | MEMHDR_FREE_MASK

#define MINSIZE 16+sizeof(MEMHDR)

uint8 KernelHeap[MAXMEMHEAP];


uint16 memhdr_get_size( uint8 * ptr )
{
	uint16 x;

	x = ( uint8 ) *ptr;
	x += ( ( uint16 ) *( ptr+1 ) << 8 );
	x = x & 0x7FFF;
	return x;
}


void memhdr_set_size( uint8 *ptr, uint16 size ) 
{

	*ptr = ( uint8 ) size;
	ptr++;
	*ptr = *ptr & 0x80;  //clear size field 空闲标志
	*(ptr) += (size >> 8);  //add in size.
}

void kernel_mem_set( uint8 * ptr, uint8 content, uint16 len )
{
	uint16 x = 0;
	
	for( x=0; x<len; x++ )
	{
		*( ptr + x ) = content;	
	}		
}

void kernel_mem_init( void )
{
	kernel_mem_set( KernelHeap, 0, MAXMEMHEAP );
	MEMHDR_SET_FREE( ( (uint8 *)&KernelHeap[0] ) );
	memhdr_set_size( ( (uint8 *)&KernelHeap[0] ), ( MAXMEMHEAP - sizeof( MEMHDR ) ) );
}

void * kernel_mem_alloc( uint16 size )
{
	uint8 * free_blk, * next_blk;
	uint16 offset;
	uint16 remainder;
	uint8 gie_status;

	if( !size )
		return( NULL );      //illegal size  分配空间为0，返回空指针
	if(size < MINSIZE) 
		size = MINSIZE;

	SAVE_AND_DISABLE_GLOBAL_INTERRUPT( gie_status );//保存并禁止全局中断
	HAL_IE_Disable();
	free_blk = KernelHeap;
	offset = 0;

	
	while( 1 )
	{
		if( MEMHDR_GET_FREE( ( ( uint8 * )free_blk ) ) && ( memhdr_get_size( ( uint8 * )free_blk ) >= size ) ) 
			break; //found block
		//advance to next block
		offset = offset + memhdr_get_size( ( uint8 * )free_blk ) + sizeof( MEMHDR );
		if( offset >= MAXMEMHEAP )
		{
			RESTORE_GLOBAL_INTERRUPT( gie_status );
			HAL_IE_Enable();
			return( NULL ); // no free blocks
		}
		free_blk = KernelHeap + offset;
	}
	remainder =  memhdr_get_size( ( uint8 * )free_blk ) - size;
	
	if(remainder < MINSIZE) 
	{
		//found block, mark as not-free
		MEMHDR_CLR_FREE((UINT8 *)free_blk);
    RESTORE_GLOBAL_INTERRUPT(gie_status);
    HAL_IE_Enable();
		return(free_blk + sizeof(MEMHDR));
	}

	//remainder is large enough to support a new block
	//adjust allocated block to requested size
	memhdr_set_size( ( ( uint8 * )free_blk ), size );
	//format next blk
	next_blk = free_blk + size+sizeof( MEMHDR );
	MEMHDR_SET_FREE( ( uint8 * )next_blk );
	memhdr_set_size( ( ( uint8 * )next_blk ), ( remainder - sizeof( MEMHDR ) ) );

	MEMHDR_CLR_FREE( ( uint8 * )free_blk ); //mark allocated block as non-free清空空闲标志位
	RESTORE_GLOBAL_INTERRUPT( gie_status );
	HAL_IE_Enable();
	
	
	//Dump_Heap_Info();
	
	return( free_blk + sizeof( MEMHDR ) );      //return new block
}

void kernel_mem_free( void * ptr )
{
	uint8 * hdr;
	uint16 offset, tmp;
	uint8 gie_status;

	if( ptr == NULL )
		return;

	SAVE_AND_DISABLE_GLOBAL_INTERRUPT( gie_status );
	HAL_IE_Disable();
	hdr = (uint8 *)ptr - sizeof( MEMHDR );
	//free this block
	MEMHDR_SET_FREE( ( uint8 * )hdr );
	//now merge
	offset = 0;
	hdr = KernelHeap;
	//loop until blocks that can be merged are merged
	while( 1 )
	{
		if( MEMHDR_GET_FREE( ( uint8 * )hdr ) )
		{
			//found a free block, see if we can merge with next block
			tmp = offset +  memhdr_get_size( ( uint8 * )hdr ) + sizeof( MEMHDR );
			if( tmp >= MAXMEMHEAP )
				break; //at end of heap, exit loop
			ptr = KernelHeap + tmp; //point at next block
			if( MEMHDR_GET_FREE( ( uint8 * )ptr ) )
			{
				//next block is free, do merge by adding size of next block
	            		memhdr_set_size( ( ( uint8 * )hdr ), ( memhdr_get_size( ( uint8 * )hdr ) + memhdr_get_size( ( uint8 * )ptr ) + sizeof( MEMHDR ) ) );
				// after merge, do not change offset, try to merge again
				//next time through loop
				continue; //back to top of loop
			}			
		}
		// next block
		offset = offset + memhdr_get_size( ( uint8 * )hdr ) + sizeof( MEMHDR );
		if( offset >= MAXMEMHEAP )
			break;  //at end of heap, exit loop
		hdr = KernelHeap + offset;
	}
	RESTORE_GLOBAL_INTERRUPT( gie_status );
	HAL_IE_Enable();
}





