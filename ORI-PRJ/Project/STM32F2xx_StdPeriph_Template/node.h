/**
  ******************************************************************************
  * @file    node.h
  * @author  ryoma
  * @version V0.1
  * @date    2-feb-2015
  * @brief   This file provide the description of node.
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __NODE_H
#define __NODE_H

#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "hal_types.h"

#define SI_3_33
    
#define NODE_NWK_ID 0x0001
#define NODE_NWK_ADDR 0x0005
#define NODE_GROUP_ADDR 0x01      



typedef struct
{
  U8 FrameLen;
  U16 NwkID;
  U16 DestNwkAddr;
  U16 SourceNwkAddr;
  U8 GroupAddr;
  U16 FrameID;
  U8 FrameCtrlWord;
  U8* PayLoad;
  U8 CheckByte;
}FrameInfo;

typedef struct
{
  U8 AckStatus; /*0:OK, 1:Timeout*/
  U16 CurrentFrameID;  
}AckInfo;

#endif