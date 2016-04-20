#ifndef DEBUG_H
#define DEBUG_H

#include "hal_types.h"



//Flag
#define beacon	 1
#define infodata	 2
#define ACK	     3

//Length of initial array(byte)
#define BeaconInitLength        5
#define DataInitLength		   22 
#define ACKInitLength           1 //only 1 bit is used 

//Length of CRCOutput, CRC is appended at the end of information bits(byte)
#define BeaconCRCOutputLength   6
#define DataCRCOutputLength	   24 
#define ACKCRCOutputLength      1

//Length of payload(byte)
#define BeaconLength           24
#define DataLength		       24 
#define ACKLength               2

//Polynomials for CRC generation 
#define poly1		        0x19A
#define poly2		      0x11021


#define sourceSeed       100

#define deflautframetype beacon

static uint8 txPayloadLength ;//接收端知道帧类型之后，确定的负载的长度。由txframetype决定
static uint8 txframetype = deflautframetype; //接收端知道的接收的帧类型。

static uint8 rxPayloadLength;//接收端知道帧类型之后，确定的负载的长度。由rxframetype决定
static uint8 rxframetype = deflautframetype; //接收端知道的接收的帧类型。

static uint8 configframetype = deflautframetype;


static uint16 lengtherror = 0;

void SourceGenerate(uint8 source[],uint8 length,uint8 seed);
void myCRC(uint8 data_input[],uint8 data_output[], uint8 flag);
void encode(uint8 data_input[],uint8 data_output[], uint8 flag);
void interleave(uint8 data_input[],uint8 data_output[], uint8 flag);
void deinterleave(uint8 data_input[],uint8 data_output[], uint8 flag);
void decode(uint8 data_input[],uint8 data_output[], uint8 flag);
void decode1(uint8 data_input[],uint8 data_output[], uint8 flag);
uint8 decode2(uint8 data_input[],uint8 data_output[], uint8 flag);
uint8 CRCCheck(uint8 data_input[], uint8 flag);


void TestTask_Init(uint8 TaskID);
uint16 TestTask_ProcessEvent( uint8 task_id, uint16 events );
void TestCondif(void);
void InitTxBuffer(void);
void TestTx(void);
void DelayUs(uint16 x);
void DelayUUs(uint16 x);
void DelayMs(uint16 x);
uint8 GetRandom(void);
void TestRSSI(void);

#endif