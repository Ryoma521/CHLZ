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

#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "hal_types.h"
#include "node.h"

#define RXBUFFERSIZE   0x40
    
int DataCheck(uint8_t* DataBuff);
void ArrayReset(int8_t* DataBuff,int8_t DataBuffLen);
void RxBuff2FrameBuff(uint8_t Buf[],FrameInfo* FrameBuf);

#endif