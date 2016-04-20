/**
  ******************************************************************************
  * @file    common.c
  * @author  Ryoma
  * @version V0.1
  * @date    4-Feb-2015
  * @brief   This file provide the description of node.
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


#include "common.h"

U8 PayLoad[52];
U8 PayLoadLen=0;   

extern uint8_t RxBuffer[];

int DataCheck(uint8_t* DataBuff)
{
   return 0x01;
}

void ArrayReset(int8_t* DataBuff,int8_t DataBuffLen)
{
  for(int i=0;i<DataBuffLen;i++)
  {
    *(DataBuff+i)=0;
  }
}

void RxBuff2FrameBuff(uint8_t Buf[],FrameInfo* FrameBuf)
{
  (*FrameBuf).FrameLen=Buf[0];
  (*FrameBuf).NwkID=((uint16_t)(Buf[1])|0xFF00)&(((uint16_t)(Buf[2])<<8)|0x00FF);
  (*FrameBuf).DestNwkAddr=((uint16_t)(Buf[3])|0xFF00)&(((uint16_t)(Buf[4])<<8)|0x00FF);
  (*FrameBuf).SourceNwkAddr=((uint16_t)(Buf[5])|0xFF00)&(((uint16_t)(Buf[6])<<8)|0x00FF);
  (*FrameBuf).GroupAddr=Buf[7];
  (*FrameBuf).FrameID=((uint16_t)(Buf[8])|0xFF00)&(((uint16_t)(Buf[9])<<8)|0x00FF);
  (*FrameBuf).FrameCtrlWord=Buf[10];
  PayLoadLen=(*FrameBuf).FrameLen-11;
  if(PayLoadLen>0)
  {
   for(int i=0;i<PayLoadLen;i++)
    {
    PayLoad[i]=Buf[11+i];
    }
   (*FrameBuf).PayLoad=PayLoad;
  }
 
  
  (*FrameBuf).CheckByte=Buf[(*FrameBuf).FrameLen];
}

