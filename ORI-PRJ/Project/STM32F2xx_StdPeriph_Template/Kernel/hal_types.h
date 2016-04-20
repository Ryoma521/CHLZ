/***********************************************************
文件名：	Hal_types.h
修改时间：
功能：		Cygwin下运行协议栈时重定义的数据类型
************************************************************/

#ifndef HAL_TYPES_H
#define HAL_TYPES_H

/*----------------------------------------------------------
                            类型
-----------------------------------------------------------*/
typedef signed   char   int8;
typedef unsigned char   uint8;
typedef signed   short  int16;
typedef unsigned short  uint16;
typedef signed   long   int32;
typedef unsigned long   uint32;


typedef uint8   byte;
typedef uint8   BYTE;
typedef uint8		UINT8;	
typedef int8		INT8;
typedef uint16  UINT16;
typedef int16   INT16;
typedef uint32	UINT32;
typedef int32		INT32;
typedef int32   INT24;
typedef uint32  uint24;

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

typedef void (*ISR_FUNC_PTR)(void);
typedef void (*VFPTR)(void);

/*----------------------------------------------------------
                           标准定义
-----------------------------------------------------------*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#endif
