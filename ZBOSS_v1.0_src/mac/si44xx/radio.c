#include "radio.h"
#include "macro.h"
#include "si4463.h"
#include "si4463_def.h"
#include "radio_config_1st.h"
#include "radio_config_2nd.h"
#include "radio_config_ack.h"
#include "radio_comm.h"
#include "radio_hal.h"
#include "TransmitReceive.h"
#include "radio_mcu.h"
#include "uart.h"
#include "Kernel.h"
#include "LowPowerSysClk.h"
#include "Client_Process.h"
#include "LowPowerTRConfig.h"
#include "LowPowerProcess.h"
//#include "Debug.h"
extern RTC_Sleep_Control_T RTCSleepControl;
extern uint32 kernel_systemClock;
extern uint8 interleaveEnable ;//2015-4-17 10:10:09
extern void tbcc_enc(unsigned char info[], unsigned char info_len, unsigned char code[]);
extern void tbcc_dec(unsigned char recv[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx);
extern void tbcc_dec_1(unsigned char recv[], unsigned char recv_1[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx);
extern void tbcc_dec_2(unsigned char recv[], unsigned char recv_1[], unsigned char recv_2[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx);
extern void tbcc_dec_3(unsigned char recv[], unsigned char recv_1[], unsigned char recv_2[], unsigned char recv_3[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx);
extern void tbcc_dec_4(unsigned char recv[], unsigned char recv_1[], unsigned char recv_2[], unsigned char recv_3[],unsigned char recv_4[], unsigned char info_len, unsigned char infoDec[], unsigned char numTx);
extern void StartDelayRun_InSystick(void);
extern void TIM3_Config(void);
extern void TIM3_Rx_Config(void);

uint16 marktime[11]={0};
uint16 beaconright=0;
uint16 beaconsum=0;
uint32 TxDelayCounter = 0;
uint8 BeforeTIslotID = 0;
uint8 BeforeTITemp_State = 0;
uint8 PHYcrc = 1;
uint8 min_d_shift_count = 0;
uint8 last_min_d_shift_count = 0;

uint8 nowRadioDataDecodeOutput[24] = {0};
uint8 nowRadiocrc = 1;
uint8 nowRadioredata1[128] = {0};
 uint8 PHYdataDecodeOutput[24] = {0};
 uint8 rxtotal2right = 0;
 unsigned int min_d_tmpArray[25] = {0};
 uint8 min_d_tmpArray_index[25] = {0};
  uint8 min_d_tmpArray_index_1[25] = {0};
   uint8 min_d_tmpArray_index_2[25] = {0};
    uint8 min_d_tmpArray_index_3[25] = {0};
 uint16 outmin_d = 0;
 uint8 wrong_min_d = 0;
uint32 tick1,tick2;
uint32 sys1,sys2;
double time1111,time2222;


uint8 TIM3_Active = 0;

void Reset_TxDelayCounter(void)
{
  TxDelayCounter = 0;
}

void Add_TxDelayCounter(void)
{
//  TxDelayCounter++;
//
//  if(TxDelayCounter == 20000)
  {
    
  
    TIM_Cmd(TIM4, DISABLE);
    
    si446x_start_tx(0u, 0x30,  0x00);
  }
}

uint8 In4463Rx = 0;

void SetInRx(void)
{
  In4463Rx = 1;
}

void SetOutRx(void)
{
  In4463Rx = 0;
}

uint8 CheckIfInRx(void)
{
  return In4463Rx;
}


//SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8) = \
              RADIO_CONFIGURATION_DATA_ARRAY;

//更改-2015年7月7日13:29:19
SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration) = \
                        RADIO_1st_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration) = \
                        &RadioConfiguration;


//只有sync无data的数据的配置,1st

//SEGMENT_VARIABLE(Radio_1st_Configuration_Data_Array[], U8) = \
              RADIO_1st_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration_1st, tRadioConfiguration) = \
                        RADIO_1st_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration_1st, tRadioConfiguration) = \
                        &RadioConfiguration_1st;






//无sync只有data的数据的配置,2nd

//SEGMENT_VARIABLE(Radio_2nd_Configuration_Data_Array[], U8) = \
              RADIO_2nd_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration_2nd, tRadioConfiguration) = \
                        RADIO_2nd_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration_2nd, tRadioConfiguration) = \
                        &RadioConfiguration_2nd;


//发送2字节的ack的配置,ack

//SEGMENT_VARIABLE(Radio_ack_Configuration_Data_Array[], U8) = \
              RADIO_ack_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration_ack, tRadioConfiguration) = \
                        RADIO_ack_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration_ack, tRadioConfiguration) = \
                        &RadioConfiguration_ack;


SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], U8);


extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);


extern ClientProcess_State Temp_State;

void vRadio_PowerUp(void)
{
  SEGMENT_VARIABLE(wDelay,  U16) = 0u;

  /* Hardware reset the chip */
  si446x_reset();

  /* Wait until reset timeout or Reset IT signal */
  for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}

/*!
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 */

void vRadio_config_1st(void)
{
  U16 wDelay;

  //芯片初始化的时候已经power up 过了
  /* Power Up the radio chip */
  //vRadio_PowerUp();

  

  
      /* Load radio configuration */
#if 1
    while (SI446X_SUCCESS != si446x_configuration_init(RADIO_base_CONFIGURATION_DATA_ARRAY))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
#endif 
  
  while (SI446X_SUCCESS != si446x_configuration_init(RADIO_1st_CONFIGURATION_DATA_ARRAY))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  
  
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
}

void vRadio_config_2nd_After_1st(void)
{
  U16 wDelay;

  //芯片初始化的时候已经power up 过了
  /* Power Up the radio chip */
 // vRadio_PowerUp();


  /* Load radio configuration */
  

  while (SI446X_SUCCESS != si446x_configuration_init(/*RADIO_2nd_CONFIGURATION_DATA_ARRAY*/RADIO_2nd_CONFIGURATION_DATA_ARRAY_DIFF_1ST))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
}


void vRadio_config_2nd(void)
{
  U16 wDelay;

  //芯片初始化的时候已经power up 过了
  /* Power Up the radio chip */
 // vRadio_PowerUp();


  /* Load radio configuration */
  
#if 1
      while (SI446X_SUCCESS != si446x_configuration_init(RADIO_base_CONFIGURATION_DATA_ARRAY))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
#endif
  while (SI446X_SUCCESS != si446x_configuration_init(RADIO_2nd_CONFIGURATION_DATA_ARRAY))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
}


void vRadio_config_ack(void)
{
  U16 wDelay;

  //芯片初始化的时候已经power up 过了
  /* Power Up the radio chip */
  //vRadio_PowerUp();


  
#if 1
   /* Load radio configuration */
  
   while (SI446X_SUCCESS != si446x_configuration_init(RADIO_base_CONFIGURATION_DATA_ARRAY))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
#endif 
  
  while (SI446X_SUCCESS != si446x_configuration_init(RADIO_ack_CONFIGURATION_DATA_ARRAY))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
}



/**
 *  Demo Application Poll-Handler-TX
 *
 *  @note This function must be called periodically.
 *
 */
extern Client_MAC_PIB MacPib;
void DemoApp_Pollhandler_RX()
{

  if(A_RX == Temp_State)
  {
    gRadio_CheckReceived_ForACK();
  }
  else
  {
    if(D_RX == Temp_State)
    {
      rxframetype = infodata;
    }
    else
    {
      rxframetype = beacon;
    }
  //  if ((D_RX == Temp_State)||(B_RX == Temp_State)||(Temp_State == RA_REP))
    {
      
    BeforeTIslotID = MacPib.SlotID;
    BeforeTITemp_State = Temp_State;
      
    gRadio_CheckReceived();//2015-02-06
    }
  }    
}

void Reset_Buf(uint8 * p, uint8 s)
{
  uint8 i;
  
  for(i=0;i<s;i++)
    p[i] = 0;
}

uint16 rxtotal1 = 0;
uint16 rxtotal2 = 0;
uint16 rxtotal3 = 0;
uint16 crcRightcont  = 0;

 extern uint32 tmpGlobalTime;
 
 uint32 delaystartMs=0;
 
 SEGMENT_VARIABLE(redata[128], U8);
  SEGMENT_VARIABLE(redata00[72], U8);
  SEGMENT_VARIABLE(redata11[72], U8);
  SEGMENT_VARIABLE(redata22[72], U8);
  SEGMENT_VARIABLE(redata33[72], U8);
 
 uint32 n_true=0;
  uint32 n_false=0;
    uint32 n_total=0;
      uint32 n_crc=0;
    
    uint32 n_total_true=0;

  
  //用于frequency scanning
  

  



uint8 left = 0;
uint8 right = 0;
uint8 compare(uint32 *input)
{
  uint8 i;
  uint32 max;
  max=input[0];
  uint8 maxre = 0;
  for (i=0;i<11;i++)
  {
    if(input[i]>max)
    {
      max = input[i];
      maxre = i;
      left = i;
    }
  }

/////
#if 0
  max=input[10];
  for (i=10;i>=0;i--)
  {
   
    if(input[i]>max)
    {
      max = input[i];
      right = i;
    }
    if (i==0)
      break;
  }
maxre = (left+right)/2;
#endif


return maxre;

}

 //end frequency scanning


  uint32 delaytimeMs=5;  
  uint32 delaytimeUs=238;
  
  //5,228:最初的，能够收到，但是在低snr条件下情况不好。
  
    extern uint32 TIMcont;
    
    
     
  uint8 info[8] = {0xFF,0xFF,0,0,0,0};   
    
extern uint8 USB_Fixed;    
#if 1   
uint8 gRadio_CheckReceived_ForACK(void)
{
  //
  //return 0;
#ifdef KEEP_GARBAGGE  
  unsigned char i;
  uint8 size,crc,InitLength;
  uint16 rssi;
  unsigned char BYTEOF1BITcount=0;
  unsigned char temp;
  unsigned char j,K=4;
  crc=1;
#endif
  ////////////////////////for tx
      uint8 BeaconInit[BeaconInitLength] = {0};
	uint8 DataInit[DataInitLength] = {0};
	uint8 ACKInit[ACKInitLength] = {0};
#ifdef KEEP_GARBAGGE
	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};


    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};

	uint8 BeaconEncodeOutput[BeaconLength] = {0};
	uint8 DataEncodeOutput[DataLength] = {0};
	uint8 ACKEncodeOutput[ACKLength] = {0};

	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
	uint8 DataInterleaveOutput[DataLength] = {0};
	uint8 ACKInterleaveOutput[ACKLength] = {0};
///////////////////////////////////////
   
        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
	uint8 rxDataInterleaveOutput[DataLength] = {0};
	uint8 rxACKInterleaveOutput[ACKLength] = {0};
	
     
  
	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
	uint8 DataDeinterleaveOutput[DataLength] = {0};
	uint8 ACKDeinterleaveOutput[ACKLength] = {0};


	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
	uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
#endif        
   switch(rxframetype)
  {
  case beacon:
   rxPayloadLength = BeaconLength;
#ifdef KEEP_GARBAGGE   
    InitLength = BeaconInitLength;
#endif
    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
    break;
  case infodata:
   rxPayloadLength = DataLength;
#ifdef KEEP_GARBAGGE   
    InitLength = DataInitLength;
#endif
    SourceGenerate(DataInit,DataInitLength,sourceSeed);
//	myCRC(DataInit,DataCRCOutput,infodata);
   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
    break;  
  case ACK:
   rxPayloadLength = ACKLength;
#ifdef KEEP_GARBAGGE   
    InitLength = ACKInitLength;
#endif
    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
    break;
  }
  
  
  //
  
  uint8 nirq_recv= SI4463_NIRQ_Bit();
  
  /* Read ITs, clear pending ones */
    //si446x_get_int_status(0u, 0u, 0u);
  //nirq_recv= SI4463_NIRQ_Bit();
  
#ifdef KEEP_GARBAGGE 
  uint8 info_beacon[3]={0xFF,0,0};

  U8 total = 0;
  U8  yihuo = 0;
  U8 num =0;

//barker code 0~5:101101110001011011100010110111000101101110001011:B716E2DC5B8B

//右移位1bit：      010110111000101101110001011011100010110111000101：5B8B716E2DC5：//移1bit相差27

//右移位2bit：      001011011100010110111000101101110001011011100010：2DC5B8B716E2：//移2bit相差26

//左移位1bit： 011011100010110111000101101110001011011100010110：6E2DC5B8B716:////移1bit相差27
  uint8 baker[6];
#endif
  
#if 1
#ifdef KEEP_GARBAGGE  
  baker[0]=0xB7;
  baker[1]=0x16;
  baker[2]=0xE2;
  baker[3]=0xDC;
  baker[4]=0x5B;
  baker[5]=0x8B;
#endif
#endif 
  
  

#ifdef KEEP_GARBAGGE
   uint32 time0 = 0;
   uint32 time1 = 0;
   uint32 time2 = 0;
   uint32 time3 = 0;
   
   uint8 rightCnt=0;
   
  uint8 stopFlag=0;
  
  uint8 fifo_cout=0;
#endif
  n_total++;
  
  if (nirq_recv==TRUE)
  {
  n_total_true++;
  }
  
  
 //if (nirq_recv == FALSE)
  //{
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
    
    
    
    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {
      
//        if(!Check_USBFiexd())
//        {
//          si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//
//
//          si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//          return FALSE;
//        }
      
      
      //有数据来，rxtotal++
   //  rxtotal1++;
       
      

      
      
      if(A_RX == Temp_State)
      {
         /* Read the length of RX_FIFO */
      si446x_fifo_info(0u);

      /* Packet RX */
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
      
     // vRadio_config_ack();
      vRadio_StartRX_ack(pRadioConfiguration_ack->Radio_ChannelNumber);
      
      UDP_Channel_Read_ForACK(fixRadioPacket);
#ifdef ENABLE_LED      
      Toggle_LED1();
#endif
      
      return TRUE;
      
      }
      /* Read the length of RX_FIFO */
      si446x_fifo_info(0u);

      /* Packet RX */
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
      
      
      
      
  //  si446x_read_rx_fifo(RadioConfiguration_1st.Radio_PacketLength, (U8 *) &fixRadioPacket[0u]);

  //  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);

   // si446x_read_rx_fifo(RadioConfiguration_1st.Radio_PacketLength, (U8 *) &fixRadioPacket[0u]);
    
  //if (fixRadioPacket[0]==0xB7)
  //{
  rxtotal1++;
  //}
  
  
    TIM_Cmd(TIM3, ENABLE);///???????????????????
     
    


     

       si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
       
       
       si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
    
       
     vRadio_config_2nd();   
      return TRUE;
    }

   
    
  else
  {
    asm("nop");
  }
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);


  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
  
  return FALSE;
  

}
#endif 


uint8 DebugWakeFlag;
uint8 DebugPrintWakeFlag  = 0;
//2014-10-30  dai

extern uint32 configtimesup;
uint8 rssihaveread = 1;
int rssidB= 0 ;
//SEGMENT_VARIABLE(redata1[128], U8);
//10.24
int bPosition = 0 ;
int bNumOfFreeBytes = 0;
const uint8 RADIO_RX_ALMOST_FULL_THRESHOLD=0x30;
int frame = 0;
extern uint8 fiforeadEnable;
extern uint8 RxBufOffsetdai;

uint32 error00 = 0;
extern uint32 TIMcont2;

int Get_RSSI_IndB(void)
{
  return rssidB;
}

uint32      rssicont = 0;
uint32      rssisum = 0;
  int AvgdBdB_shi=0;
  int AvgdBdB_ge=0;

uint8 gRadio_CheckReceived(void)
{
  
#ifdef KEEP_GARBAGGE    
  //
  SEGMENT_VARIABLE(redata1[128], U8);
   SEGMENT_VARIABLE(deinterleavebuffer[72], U8);
  unsigned char i;
  uint8 size,crc,InitLength;
#endif
  uint16 rssi;
#ifdef KEEP_GARBAGGE    
  unsigned char BYTEOF1BITcount=0;
  unsigned char temp;
  unsigned char j,K=4;
  crc=1;
#endif
  ////////////////////////for tx
      uint8 BeaconInit[BeaconInitLength] = {0};
	uint8 DataInit[DataInitLength] = {0};
	uint8 ACKInit[ACKInitLength] = {0};
#ifdef KEEP_GARBAGGE  
	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};


    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};

	uint8 BeaconEncodeOutput[BeaconLength] = {0};
	uint8 DataEncodeOutput[DataLength] = {0};
	uint8 ACKEncodeOutput[ACKLength] = {0};

	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
	uint8 DataInterleaveOutput[DataLength] = {0};
	uint8 ACKInterleaveOutput[ACKLength] = {0};
///////////////////////////////////////
   
        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
	uint8 rxDataInterleaveOutput[DataLength] = {0};
	uint8 rxACKInterleaveOutput[ACKLength] = {0};
	
     
  
	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
	uint8 DataDeinterleaveOutput[DataLength] = {0};
	uint8 ACKDeinterleaveOutput[ACKLength] = {0};


	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
	uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
#endif        
   switch(rxframetype)
  {
  case beacon:
   rxPayloadLength = BeaconLength;
#ifdef KEEP_GARBAGGE     
    InitLength = BeaconInitLength;
#endif
    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
    break;
  case infodata:
   rxPayloadLength = DataLength;
#ifdef KEEP_GARBAGGE     
    InitLength = DataInitLength;
#endif
    SourceGenerate(DataInit,DataInitLength,sourceSeed);
//	myCRC(DataInit,DataCRCOutput,infodata);
   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
    break;  
  case ACK:
   rxPayloadLength = ACKLength;
#ifdef KEEP_GARBAGGE     
    InitLength = ACKInitLength;
#endif
    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
    break;
  }
  
  
  //
  
  uint8 nirq_recv= SI4463_NIRQ_Bit();
  
  /* Read ITs, clear pending ones */
    //si446x_get_int_status(0u, 0u, 0u);
  //nirq_recv= SI4463_NIRQ_Bit();
  
#ifdef KEEP_GARBAGGE   
  uint8 info_beacon[3]={0xFF,0,0};

  U8 total = 0;
  U8  yihuo = 0;
  U8 num =0;

//barker code 0~5:101101110001011011100010110111000101101110001011:B716E2DC5B8B

//右移位1bit：      010110111000101101110001011011100010110111000101：5B8B716E2DC5：//移1bit相差27

//右移位2bit：      001011011100010110111000101101110001011011100010：2DC5B8B716E2：//移2bit相差26

//左移位1bit： 011011100010110111000101101110001011011100010110：6E2DC5B8B716:////移1bit相差27
  uint8 baker[6];

#if 1
  baker[0]=0xB7;
  baker[1]=0x16;
  baker[2]=0xE2;
  baker[3]=0xDC;
  baker[4]=0x5B;
  baker[5]=0x8B;
#endif 
#endif  
  

#ifdef KEEP_GARBAGGE  
   uint32 time0 = 0;
   uint32 time1 = 0;
   uint32 time2 = 0;
   uint32 time3 = 0;
   
   uint8 rightCnt=0;
   
  uint8 stopFlag=0;
  
  uint8 fifo_cout=0;
#endif
  n_total++;
  
  if (nirq_recv==TRUE)
  {
  n_total_true++;
  }
 // rxtotal1++;
  
 //if (nirq_recv == FALSE)
  //{
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);
  
  
  
  
  
 #if 1
  if (Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)
  {
    //Trigger1(); 
    beaconsum++;
      StartDelayRun_InSystick();
    
      if (rssihaveread==0)
     {
      rssi  = sRadioGetRSSI();
      
      if (rssi>0)
      {
#ifdef F_BOARD        //here here here
//#ifndef BOARD_F37
        frequency_compensation();
//#endif
#endif
        
#ifdef L_BOARD        
        frequency_compensation();
#endif        
        
#ifdef BOARD_3_030
        frequency_compensation();
#endif 
        
#ifdef BOARD_3_031
        frequency_compensation();
#endif        
        
        rssicont = 1;
        rssisum = rssi;
      
//      rssicont++;
//      rssisum = rssisum+rssi;
      
      
      AvgdBdB_shi = (13300 - 100*rssisum/rssicont/2)/100;
      AvgdBdB_ge = (13300 - 100*rssisum/rssicont/2)%100;
      }
      
      uint8 rssioffset = 133;
      
      rssidB = rssi/2-rssioffset;
#ifdef ENABLE_COMMON_COMMUNICATION_PROTOCAL      
      Set_RSSI_0x0008(rssi);
#endif      
      //rssidB = 0;
     rssi = 0;
     rssihaveread=1;
     }
     return TRUE;
   }
#endif

  
  
  
  
  
  
  
  
  
  

    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
    
    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {
      
#ifdef ENABLE_AUTO_SLEEP
      //Beacon_SyncRx_Handler_Process2();
      Beacon_SyncRx_Handler_AllInOne(kernel_GetSystemClock());
#endif      
      //有数据来，rxtotal++
   // rxtotal1++;
       
      rssihaveread = 0;
#ifdef ENABLE_SLOWRUN_DEBUG_MARKER        
      Save_TimeMarker();
#endif
      Save_RxSyncTime();

      
      /* Read the length of RX_FIFO */
      si446x_fifo_info(0u);

      /* Packet RX */
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
   DebugWakeFlag = fixRadioPacket[0];
   DebugPrintWakeFlag = 1;
#ifdef CO_BEACON_FLAG   

   uint8 tmpRet = Handle_Beacon_WakeFlag(fixRadioPacket[0]);
   if(tmpRet == 1)
  {
      rxtotal1++;
           
      configtimesup = 0;
      TIMcont = 0;
      fiforeadEnable = 0;

      TIM3_Config();//用于第一帧和第二帧直接的延时控制
      TIM_Cmd(TIM3, ENABLE); 
      TIM3_Active = 1;
       //vRadio_config_2nd();
      vRadio_config_2nd_After_1st();
      
      fixRadioPacket[0]=0; 
      
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
       return 0;  //Added Pei
   
  }
  else if(tmpRet == 2)
  {
    fixRadioPacket[0]=0; 
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
    vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);
    return 0;
  }

   
#else
#ifndef ENABLE_AUTO_SLEEP    
  if(fixRadioPacket[0]==0xB7)
#else
   if(Handle_Beacon_WakeFlag(fixRadioPacket[0]))
#endif
  {

  

      rxtotal1++;
           
      configtimesup = 0;
      TIMcont = 0;
      fiforeadEnable = 0;
      
      
      
      
      TIM3_Config();//用于第一帧和第二帧直接的延时控制
      TIM_Cmd(TIM3, ENABLE); 
      TIM3_Active = 1;
       //vRadio_config_2nd();
      vRadio_config_2nd_After_1st();
      
      
      
      fixRadioPacket[0]=0; 
      
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
       return 0;  //Added Pei
   
  }
#endif
    }
    


      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);


  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
  
  return TRUE;
  

}
 






/*!
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 */
uint8 gRadio_CheckReceived_VariablePacket(void)
{
  uint8 nirq_recv_var= SI4463_NIRQ_Bit();
  
  if (nirq_recv_var == FALSE)
  {
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

	if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_BIT)
    {
      /* State change to */
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP);
	
	  /* Reset FIFO */
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
      
	  /* State change to */
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
    }

    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)
    {
      /* Blink once LED2 to show Sync Word detected */
      //vHmi_ChangeLedState(eHmi_Led2_c, eHmi_LedBlinkOnce_c);
    }

    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {
      /* Blink once LED3 as CRC OK or not enabled */
     // vHmi_ChangeLedState(eHmi_Led3_c, eHmi_LedBlinkOnce_c);

      /* Read the length of RX_FIFO */
      si446x_fifo_info(0u);

      /* Packet RX */
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);

      /* Do not use UART in case of direct firmware load */


      return TRUE;
    }

    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_CRC_ERROR_BIT)
    {
      /* Reset FIFO */
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
    }
  }

  return FALSE;
}


/*!
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 */

void vRadio_StartRX_1st(U8 channel)
{

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
  si446x_start_rx(channel, 0u, RadioConfiguration_1st.Radio_PacketLength,
                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
                  
  /* Switch on LED1 to show RX state */
  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
  
  Start_Rx_ISR();
}

void vRadio_StartRX_2nd(U8 channel)
{

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
  si446x_start_rx(channel, 0u, RadioConfiguration_2nd.Radio_PacketLength,
                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
                  
  /* Switch on LED1 to show RX state */
  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
  
  Start_Rx_ISR();
}

void vRadio_StartRX_ack(U8 channel)
{

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
  si446x_start_rx(channel, 0u, RadioConfiguration_ack.Radio_PacketLength,
                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
                  
  /* Switch on LED1 to show RX state */
  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
  
  Start_Rx_ISR();
}

void vRadio_StartRX(U8 channel)
{

  
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
  si446x_start_rx(channel, 0u, RadioConfiguration.Radio_PacketLength,
                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
                  
  /* Switch on LED1 to show RX state */
  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
  

}


/*!
 *  Check if Packet sent IT flag is pending.
 *
 *  @return   TRUE / FALSE
 *
 *  @note
 *
 */
uint8 gRadio_CheckTransmitted(void)
{
    uint8 nirq_trans;
    
    
    nirq_trans = SI4463_NIRQ_Bit();
    
    /*
    while(nirq_trans!=0)
    {
       if(!SI4463_While_Handler())
       break;
       
       nirq_trans = SI4463_NIRQ_Bit();
    }
    SI4463_AfterWhile();
    */
    //DelayMs(200);
     
    
 // nirq_trans = SI4463_NIRQ_Bit();
    
  if (nirq_trans == 0)
  {
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
    {
      return TRUE;
    }
  }

  return FALSE;
}


/*!
 *  Set Radio to TX mode, fixed packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent
 *
 *  @note
 *
 */
uint8  vRadio_StartTx(U8 channel, U8 *pioFixRadioPacket,U8 len)
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
    return FALSE;
  }

  
  /* Fill the TX fifo with datas */
  si446x_write_tx_fifo(RadioConfiguration.Radio_PacketLength, pioFixRadioPacket);

  /* Start sending packet, channel 0, START immediately, Packet n bytes long, go READY when done */
  si446x_start_tx(channel, 0x30,  RadioConfiguration.Radio_PacketLength);
  
  return TRUE;
}

void RXHandlerInGDO0_SI4463(void)
{
   /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {
      /* Packet RX */
      si446x_read_rx_fifo(RadioConfiguration.Radio_PacketLength, (U8 *) &fixRadioPacket[0u]);

      /* Send it to UART */
//      for (lCnt = 0u; lCnt < RadioConfiguration.Radio_PacketLength; lCnt++)
//      {
//        Comm_IF_SendUART(*((U8 *) &fixRadioPacket[0u] + lCnt));
//      }
//      Comm_IF_SendUART('\n');
    }

    /* Reset FIFO */
    si446x_fifo_info(0x02);
}

/*!
 * This function is used to compare the content of the received packet to a string.
 *
 * @return  None.
 */
uint8 gSampleCode_StringCompare(U8* pbiPacketContent, U8* pbiString, U8 lenght)
{
  while ((*pbiPacketContent++ == *pbiString++) && (lenght > 0u))
  {
    if( (--lenght) == 0u )
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*!
 * This function is used to show the actual state of the push-buttons on the LEDs.
 *
 * @return  TRUE - Started a packet TX / FALSE - Not packet to be sent.
 */
uint8 vSampleCode_SendFixPacket(U8 * fixRadioPacket,U8 len)
{
 // SEGMENT_VARIABLE(boPbPushTrack, U8);
  //SEGMENT_VARIABLE(woPbPushTime, U16);

 // gHmi_PbIsPushed(&boPbPushTrack, &woPbPushTime);

//  fixRadioPacket[0]='B';
//  fixRadioPacket[1]='U';
//  fixRadioPacket[2]='T';
//  fixRadioPacket[3]='T';
//  fixRadioPacket[4]='O';
//  fixRadioPacket[5]='N';
//
//  fixRadioPacket[6]='1';
//  if(boPbPushTrack & eHmi_Pb1_c)
//  {
//    fixRadioPacket[6]='1';
//  }
//  else if(boPbPushTrack & eHmi_Pb2_c)
//  {
//    fixRadioPacket[6]='2';
//  }
//  else if(boPbPushTrack & eHmi_Pb3_c)
//  {
//    fixRadioPacket[6]='3';
//  }
//  else if(boPbPushTrack & eHmi_Pb4_c)
//  {
//    fixRadioPacket[6]='4';
//  }
//  else
//  {
//    return FALSE;
//  }

  uint8 TxFlag;
  TxFlag = vRadio_StartTx(pRadioConfiguration->Radio_ChannelNumber, (U8 *) fixRadioPacket,len);


  /* Packet sending initialized */
  return TxFlag;
}
extern uint32 tmpGlobalTime;
uint32 StatTime, StopTime;

extern uint8 trx_state;//0:tx,1:rx

extern  uint8 PKT_Sent_Flag;

uint8 bPositionInPayload;
uint8 pPositionInPayload[128];

uint8 length=0;








extern uint32 configtimesup; 
extern uint32 TIMcont; 
extern uint8 barkerSyncEnable;

#if 1 //添加barker的发送
//2015-5-18 13:48:38，添加barker的发送

#endif

uint8 ReFillTxBuf = 0;

uint8 Get_ReFillTxBuf(void)
{
  return ReFillTxBuf;
}

void Set_ReFillTxBuf(uint8 x)
{
  ReFillTxBuf = x;
}

uint8 InTx1stRate = 0;

uint8 SI4463_Config_Transmit_dualrate_WithPowerSaving(U8 * pSrc1,U8 len1,U8 * pSrc2,U8 len2)
{
  //添加13byte的barker码
//1111,1001,1010,1111,1100,1101,0111,1110,0110,1011,1111,0011,0101,1111,1001,1010,
  //1111,1100,1101,0111,1110,0110,1011,1111,0011,0101
  
 // F9 AF CD 7E 6B F3 5F 9A
  //FC D7 E6 BF 35
  
  uint8 ClkChanged = 0;
  uint8 ControlFlag = 0;
  marktime[0]=kernel_systemClock;
#ifdef ENABLE_LOWPOWER_CLK_SWITCH  
  LowPower_Config_HCLK_Fast();
#endif
  


 
#if 1 //new 2015-07-07
  unsigned char transmitArray[72+barkerLengthInByte]={0};
   uint8 barkerCode[7]={0};//最大是26个byte的barker，配置文件中两处要改动
  

  uint16 i,j;

  uint16 interleaveIndex;//新算法添加


  	// for lfsr
  //unsigned int start_state = 1;  /* Any nonzero start state will work. */
   // unsigned int lfsr = start_state;

    barkerCode[0] = 0xF9;
    barkerCode[1] = 0xAF;
    barkerCode[2] = 0xCD;
    barkerCode[3] = 0x7E;
    barkerCode[4] = 0x6B;
    barkerCode[5] = 0xF3;
    barkerCode[6] = 0x5F;
	/*
    barkerCode[7] = 0x9A;
    barkerCode[8] = 0xFC;
    barkerCode[9] = 0xD7;
    barkerCode[10] = 0xE6;
    barkerCode[11] = 0xBF;
    barkerCode[12] = 0x35; 
    barkerCode[13] = 0xF9;
    barkerCode[14] = 0xAF;
    barkerCode[15] = 0xCD;
    barkerCode[16] = 0x7E;
    barkerCode[17] = 0x6B;
    barkerCode[18] = 0xF3;
    barkerCode[19] = 0x5F;
    barkerCode[20] = 0x9A;
    barkerCode[21] = 0xFC;
    barkerCode[22] = 0xD7;
    barkerCode[23] = 0xE6;
    barkerCode[24] = 0xBF;
    barkerCode[25] = 0x35;
  */
  
	//新算法,交织和barker，interleaver、second sync word generator
	   if ((interleaveEnable==1)&&(barkerSyncEnable==1))//同时满足
        {


			for(i = 0; i < barkerLengthInByte; i++)
	       {

			 transmitArray[i] = barkerCode[i];//barker部分,不用barkerAddedArray。

			}

	      for(i = 0; i < 72; i++)
	       {
		    transmitArray[i+barkerLengthInByte] = 0;//必须要清0，累加的影响
		    for(j = 0; j < 8; j++)
		     {
				 
		//lsfr		 


		interleaveIndex = ((i*8+j)%24)*24 + ((i*8+j)/24);//块交织


			 transmitArray[i+barkerLengthInByte] = transmitArray[i+barkerLengthInByte] + (((pSrc2[interleaveIndex/8] >> (7-(interleaveIndex%8)))%2)<<(7-j));//累加，去掉了中间数组，直接用了input:pSrc2[]

              }
	        }
         }
  //  
          
#if 0
        uint8 jjj;
        
        for(jjj=7;jjj<79;jjj++)
        {
          transmitArray[jjj] = 0x00;
        
        }

#endif         
         
         
         
#endif  
  
  vRadio_config_1st();
     RF_Switch_TX();
  //可以由外面决定，但是为了调试方便。直接由配置中读取。
  len2 = RadioConfiguration_2nd.Radio_PacketLength;
  
  //发送第一个包
   trx_state = 0;//f发送模式
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
       PKT_Sent_Flag =0;
#ifdef DUAL_RATE_TX_BACK_TO_1STRX  
  vRadio_config_1st();
  
  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
  
#endif
  trx_state = 1;//第二个包发完之后默认为接收状态
   
  
       RF_Switch_RX();
    return FALSE;
  }

  /* Reset TX FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX FIFO with data */
  si446x_write_tx_fifo(len1, pSrc1);


  /* Start sending packet, channel 0, START immediately,
   * Packet length according to 'len', go READY when done */
  
  //TIM_Cmd(TIM4, ENABLE);
  
  si446x_start_tx(0u, 0x30,  len1); 
  Mark_SystemTime1();
  marktime[1]=kernel_systemClock;
  
  InTx1stRate = 1;
  
#ifdef ENABLE_LOWPOWER_CLK_SWITCH  
  LowPower_Config_HCLK_Slow(); //1st rate configed,tx buf loaded,start tx,back to Slot-Run
#endif
  
 
  uint32 counter = 0;
  while (PKT_Sent_Flag!=1)//发送等待
  {
  //等待保护
    asm("nop");
    counter++;
    if(counter >= 0xFFFFFFFF)
    {
     //第一个包未发送成功
        counter=0;
        PKT_Sent_Flag =0;
#ifdef DUAL_RATE_TX_BACK_TO_1STRX        
        vRadio_config_1st();
        
        vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
#endif
        trx_state = 1;//第二个包发完之后默认为接收状态
   
  
        RF_Switch_RX();
        InTx1stRate = 0;
        
        return FALSE;
    }  
    
    if((Compute_DiffMarkTime1() == 213)&&(ClkChanged == 0)) //before 1st tx done,to Fast-Run for SPI in ISR
    {
      marktime[2]=kernel_systemClock;
#ifdef ENABLE_LOWPOWER_CLK_SWITCH      
      LowPower_Config_HCLK_Fast();
#endif      
      ClkChanged = 1;
    }
  }
  
  ClkChanged = 0;
  
  InTx1stRate = 0;
  
   //开启TIM4
  configtimesup=0;
  TIM_Cmd(TIM4, ENABLE);
  /////
  tick1=SysTick->VAL;
    sys1=kernel_GetSystemClock();
    time1111=(double)((double)(32000-tick1)/32000);
    time1111+=sys1;
   PKT_Sent_Flag =0;
   //vRadio_config_2nd();
  vRadio_config_2nd_After_1st();
 
 
  trx_state = 0;//f发送模式
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
      PKT_Sent_Flag =0;
#ifdef DUAL_RATE_TX_BACK_TO_1STRX        
      vRadio_config_1st();
      
      vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
#endif
      trx_state = 1;//第二个包发完之后默认为接收状态
   
      RF_Switch_RX();
   
      //TIM4_Config_TXTXdelay();///
      TIM_Cmd(TIM4, DISABLE);//g关闭使能
      TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
       
      return FALSE;
  }
  
  for (uint8 i=0;i<(72+barkerLengthInByte);i++)
  {
  pPositionInPayload[i] = transmitArray[i];
 
  }
   bPositionInPayload = 0;
   

  /* Reset TX FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
  
  /* Fill the TX FIFO with data */
    /* Fill the TX FIFO with data */
  if (len2>50)
  {
    length =50;
    si446x_write_tx_fifo(length, &transmitArray[0]);
    
    
    
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);

    bPositionInPayload = length;
    // si446x_start_tx(0u, 0x30,  length);
  
    /////////////////////////////////////////////////////
    counter = 0;
    if (configtimesup == 1)
    {
      configtimesup = configtimesup+1;
      configtimesup = configtimesup-1;
    }
    
 
    while (configtimesup<1)//2980~2983
    {
    //等待保护
      //asm("nop");
      counter++;
      if(counter >= 0xFFFFFFFF)
      {
     //第一个包未发送成功
        counter=0;
        PKT_Sent_Flag =0;
#ifdef DUAL_RATE_TX_BACK_TO_1STRX        
        vRadio_config_1st();
  
        vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
#endif
        trx_state = 1;//第二个包发完之后默认为接收状态
   
  
        RF_Switch_RX();
       
       
        //TIM4_Config_TXTXdelay();///
        TIM_Cmd(TIM4, DISABLE);//g关闭使能
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
          return FALSE;
      }
    } 

  
       //TIM4_Config_TXTXdelay();///
#if 0
    TIM_Cmd(TIM4, DISABLE);//g关闭使能
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
#endif
 ////////////////////////////////////////////////////
    /////
    
    si446x_start_tx(0u, 0x30,  0x00);
    
    tick2=SysTick->VAL;
    sys2=kernel_GetSystemClock();
    time2222=(double)((double)(32000-tick2)/32000);
    time2222+=sys2;
    time2222=time2222-time1111;
    asm("nop");
  }
  else
  {
    si446x_write_tx_fifo(len2, transmitArray);
    si446x_start_tx(0u, 0x30,  0x00);
  
  }

  /* Start sending packet, channel 0, START immediately,
   * Packet length according to 'len', go READY when done */
 
  Mark_SystemTime1();
  marktime[4]=kernel_systemClock;
#ifdef ENABLE_LOWPOWER_CLK_SWITCH  
  LowPower_Config_HCLK_Slow(); 
#endif  
  counter = 0;
  
  while (PKT_Sent_Flag!=1)//发送等待
  {
    //等待保护
    asm("nop");
    counter++;
    if(counter >= 0xFFFFFFFF)
    {
       //第一个包未发送成功
      counter=0;
      PKT_Sent_Flag =0;
#ifdef DUAL_RATE_TX_BACK_TO_1STRX      
      vRadio_config_1st();
      
      vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
#endif
      trx_state = 1;//第二个包发完之后默认为接收状态
       
      
      RF_Switch_RX();
         
   ////////////////////////////////////////////////////
      return FALSE;
    }
    
    if((Compute_DiffMarkTime1() == 111)&&(ClkChanged == 0))
    {
      marktime[5]=kernel_systemClock;
#ifdef ENABLE_LOWPOWER_CLK_SWITCH      
      LowPower_Config_HCLK_Fast();  //before 2nd rate tx buf empty,to Fast-Run for SPI in ISR
#endif
      ClkChanged = 1;
    }
    
    if(Get_ReFillTxBuf() == 1)
    {
      Set_ReFillTxBuf(0);
    marktime[7]=kernel_systemClock;      
#ifdef ENABLE_LOWPOWER_CLK_SWITCH      
      LowPower_Config_HCLK_Slow(); //2nd rate tx buf reloaded,to Slow-Run
#endif
      ClkChanged = 0;
      Mark_SystemTime2();
      ControlFlag = 1;
    }
    
    if((Compute_DiffMarkTime2() == 139)&&(ClkChanged == 0)&&(ControlFlag == 1))
    {
      marktime[8]=kernel_systemClock;
#ifdef ENABLE_LOWPOWER_CLK_SWITCH      
      LowPower_Config_HCLK_Fast(); //befor 2nd tx finally done,to Fast-Run
#endif
      ClkChanged = 1;
    }
  
  }
#ifdef ENABLE_LED  
  Toggle_LED2(); 
#endif  
  
  PKT_Sent_Flag =0;
#ifdef DUAL_RATE_TX_BACK_TO_1STRX
  vRadio_config_1st();
  
  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
#endif
  marktime[10]=kernel_systemClock;
#ifdef ENABLE_LOWPOWER_CLK_SWITCH  
  LowPower_Config_HCLK_Slow(); //all done, to Slow-Run
#endif
  trx_state = 1;//第二个包发完之后默认为接收状态
     
  RF_Switch_RX();
  
              
  return TRUE;
}

uint8 New_SI4463_Transmit_WithPowerSaving(U8 * pSrc,U8 len)
{
   
  RF_Switch_TX();
  SI4463_Enable_NIRQ_TX();
  
 
  
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
        
        RF_Switch_RX();
    return FALSE;
  }

  //trx_state = 0;//f发送模式
  /* Reset TX FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX FIFO with data */
  si446x_write_tx_fifo(len, pSrc);
  
  


  /* Start sending packet, channel 0, START immediately,
   * Packet length according to 'len', go READY when done */
  si446x_start_tx(0u, 0x30,  len);
#ifdef ENABLE_LOWPOWER_CLK_SWITCH  
  LowPower_Config_HCLK_Slow();
#endif  
 uint32 counter = 0;
 StatTime = tmpGlobalTime;
 while(TRUE != gRadio_CheckTransmitted())
 {
   
  asm("nop");
  counter++;
  if(counter >= 0xFFFFFFFF)
  {
     
    SI4463_Enable_NIRQ_RX();
    EXTILineNIRQ_Config();
    RF_Switch_RX();
    return FALSE;
  }
    
 }  
  StopTime = tmpGlobalTime-StatTime;
 

 SI4463_Enable_NIRQ_RX();
  EXTILineNIRQ_Config();
  RF_Switch_RX();
#ifdef ENABLE_LED  
  Toggle_LED2();
#endif
  return TRUE;
}


uint8 New_SI4463_Transmit(U8 * pSrc,U8 len)
{
   
  RF_Switch_TX();
  SI4463_Enable_NIRQ_TX();
  
 
  
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
        
        RF_Switch_RX();
    return FALSE;
  }

  /* Reset TX FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX FIFO with data */
  si446x_write_tx_fifo(len, pSrc);


  /* Start sending packet, channel 0, START immediately,
   * Packet length according to 'len', go READY when done */
  si446x_start_tx(0u, 0x30,  len);
  
 uint32 counter = 0;
 StatTime = tmpGlobalTime;
 while(TRUE != gRadio_CheckTransmitted())
 {
   
  asm("nop");
  counter++;
  if(counter >= 0xFFFFFFFF)
  {
     
    SI4463_Enable_NIRQ_RX();
    EXTILineNIRQ_Config();
    RF_Switch_RX();
    return FALSE;
  }
    
 }  
  StopTime = tmpGlobalTime-StatTime;
 

 SI4463_Enable_NIRQ_RX();
  EXTILineNIRQ_Config();
  RF_Switch_RX();
#ifdef ENABLE_LED  
  Toggle_LED2();
#endif
  return TRUE;
}

/*!
 * This function is used to transmit data
 *
 *
 */
void SI4463_Transmit(U8 * Packet,U8 length)
{
  static SEGMENT_VARIABLE(lPktSending, U8) = 0u;
#ifdef KEEP_GARBAGGE    
  uint8 temp;
#endif  
 
  // Check if the radio packet sent successfully
  if (TRUE == gRadio_CheckTransmitted())
  {
    /* Clear Packet Sending flag */
#ifdef KEEP_GARBAGGE      
    temp = 2;
#endif
    lPktSending = 0u;
 }
 else
 {
#ifdef KEEP_GARBAGGE     
    temp = 3;
#endif
 
 }

  //lPktSending = 0;


  if ((0u == lPktSending))
  {
    uint8 flag=vSampleCode_SendFixPacket((U8 *)Packet,length);
    

    
    if (TRUE == flag)
    {
      //lPer_SecCntr = 0u;

      /* Set Packet Sending flag */
      lPktSending = 1u;
      
    }
  }
}


uint8 indexnum[2]={0};

#ifndef SOFT_MERGE
void Raw_Rx_Packet_Hanlder(void) //Added Pei
{
    unsigned char i;
  uint8 size,crc,InitLength;
  uint16 rssi;
  unsigned char BYTEOF1BITcount=0;
  unsigned char temp;
  unsigned char j,K=4;
  crc=1;
  

  
  uint8 BeaconInit[BeaconInitLength] = {0};
	uint8 DataInit[DataInitLength] = {0};
	uint8 ACKInit[ACKInitLength] = {0};

	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};


    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};

	uint8 BeaconEncodeOutput[BeaconLength] = {0};
	uint8 DataEncodeOutput[DataLength] = {0};
	uint8 ACKEncodeOutput[ACKLength] = {0};

	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
	uint8 DataInterleaveOutput[DataLength] = {0};
	uint8 ACKInterleaveOutput[ACKLength] = {0};
///////////////////////////////////////
   
        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
	uint8 rxDataInterleaveOutput[DataLength] = {0};
	uint8 rxACKInterleaveOutput[ACKLength] = {0};
	
     
  
	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
	uint8 DataDeinterleaveOutput[DataLength] = {0};
	uint8 ACKDeinterleaveOutput[ACKLength] = {0};


	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
	uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
        
   switch(rxframetype)
  {
  case beacon:
   rxPayloadLength = BeaconLength;
    InitLength = BeaconInitLength;
    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
    break;
  case infodata:
   rxPayloadLength = DataLength;
    InitLength = DataInitLength;
    SourceGenerate(DataInit,DataInitLength,sourceSeed);
//	myCRC(DataInit,DataCRCOutput,infodata);
   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
    break;  
  case ACK:
   rxPayloadLength = ACKLength;
    InitLength = ACKInitLength;
    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
    break;
  }
  
    SEGMENT_VARIABLE(redata1[128], U8);
    SEGMENT_VARIABLE(deinterleavebuffer[72], U8);
    
    if (rxframetype==beacon)
             {
          shift(redata,deinterleavebuffer,72,0);//input,output:  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
       //   ResetBeaconBuffer(deinterleavebuffer,redata1);

#if 1
          if ((BeaconDecodeOutput[4]!=0x99)&&(BeaconDecodeOutput[4]!=0x04))
          {
            crc = crc+1;
            crc = crc -1;
          }


          if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,1);//input,output:左移或者右移1bit或者2bit  
           deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);     
       //   ResetBeaconBuffer(deinterleavebuffer,redata1);

          //当前延时偏小
          }
          
           if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,2);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
       //  ResetBeaconBuffer(deinterleavebuffer,redata1);

          //当前延时偏大
          }
#if 1
           if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,3);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
        //  ResetBeaconBuffer(deinterleavebuffer,redata1);

          }
          
           if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,4);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
        //  ResetBeaconBuffer(deinterleavebuffer,redata1);

          }
          
            if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,5);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
        // ResetBeaconBuffer(deinterleavebuffer,redata1);

          }
          
            if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,6);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
         // ResetBeaconBuffer(deinterleavebuffer,redata1);

          }
          
              if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,7);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
        //  ResetBeaconBuffer(deinterleavebuffer,redata1);

          }
          
            if (crc!=0)
          {
          shift(redata,deinterleavebuffer,72,8);//input,output:左移或者右移1bit或者2bit  
          deinterleave(deinterleavebuffer,redata1,beacon);
          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
         //ResetBeaconBuffer(deinterleavebuffer,redata1);

          }
          
                          ///带协议的beacon发送的内容
      if (BeaconDecodeOutput[0]==0x00)
      {
         if (BeaconDecodeOutput[1]==0x00)
         {
            if (BeaconDecodeOutput[2]==0x00)
            {
              if (BeaconDecodeOutput[3]==0x00)
              {
                if (BeaconDecodeOutput[4]==0x00)
                {
                  error00++;
                  crc = 1;//crc错误
                }
              }
            }
         }
      }
          
#endif
#endif  
             }
          
             if (rxframetype==infodata)
             {
                    shift(redata,redata1,72,0);
           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
		   crc = CRCCheck(DataDecodeOutput,infodata);
    
      #if 1
          if (crc!=0)
          {
          shift(redata,redata1,72,1);//input,output:左移或者右移1bit或者2bit  
          tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
          crc = CRCCheck(DataDecodeOutput,infodata);   
   if (crc==0)
   {
   crc=crc;
   }
          }
          
           if (crc!=0)
          {
          shift(redata,redata1,72,2);//input,output:左移或者右移1bit或者2bit  
           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
          crc = CRCCheck(DataDecodeOutput,infodata);
      if (crc==0)
   {
   crc=crc;
   }
          }
          
       #if 0 
           if (crc!=0)
          {
          shift(redata,redata1,72,3);//input,output:左移或者右移1bit或者2bit  
           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
          crc = CRCCheck(DataDecodeOutput,infodata);
   if (crc==0)
   {
   crc=crc;
   }
          }
          
           if (crc!=0)
          {
          shift(redata,redata1,72,4);//input,output:左移或者右移1bit或者2bit  
           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
          crc = CRCCheck(DataDecodeOutput,infodata);
     if (crc==0)
   {
   crc=crc;
   }
          }
          

             if (crc!=0)
          {
          shift(redata,redata1,72,5);//input,output:左移或者右移1bit或者2bit  
           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
          crc = CRCCheck(DataDecodeOutput,infodata);
     if (crc==0)
   {
   crc=crc;
   }
          }
   if (crc!=0)
          {

          shift(redata,redata1,72,6);//input,output:左移或者右移1bit或者2bit  
           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
          crc = CRCCheck(DataDecodeOutput,infodata);
     if (crc==0)
   {
   crc=crc;
   }
          }
#endif
          
#endif               
                   
             }         
             
          
          
          if (crc==0)//crc=0时为解调正确
          {
            crc=1;
            rxtotal3++;
          
       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   

            if(rxframetype==beacon)
            {
           for ( i=0;i<5;i++)
              if (i!=BeaconDecodeOutput[i])
              {
                break;
              }
            if (i == 5)
            rxtotal2++;//全对   
            }
            
             if(rxframetype==infodata)
            {
           for ( i=0;i<22;i++)
              if (i!=DataDecodeOutput[i])
              {
                break;
              }
            if (i == 22)
            rxtotal2++;//全对   
            }


           
          }
      

      ///带协议的beacon发送的内容
      if (BeaconDecodeOutput[0]==0x01)
      {
         if (BeaconDecodeOutput[1]==0x6F)
         {
            if (BeaconDecodeOutput[2]==0xFF)
            {
              if (BeaconDecodeOutput[3]==0x10)
              {
                if (BeaconDecodeOutput[4]==0x99)
                {
               //   rxtotal2++;
                }
              }
            }
         }
      }
      
      ///
 info[2] = (rxtotal1>>8)&0xff;
      info[3] = rxtotal1&0xff;
      info[4] = (rxtotal2>>8)&0xff;
      info[5] = rxtotal2&0xff;

      
       if (rxtotal1!=rxtotal2)
      {
    //  rxtotal2 = rxtotal2 ; 
      }
      
      
      if (rxtotal1==1000)
      {
     // rxtotal2 = rxtotal2 ; 
      }
      
      if (BeaconDecodeOutput[1] == 0x00)
      {
      BeaconDecodeOutput[1] = 0x00;
      }
      
      
   
      //if(crc == 0)
      {
        crc=1;
        
        ///freq_autotest
        
      uint32 FrameIndex_temp;
      uint32 ZoneIndex_temp;
      uint32 ReservedBit;
      
       FrameIndex_temp = get_UINT16(BeaconDecodeOutput[1],BeaconDecodeOutput[2]);
       ZoneIndex_temp = (BeaconDecodeOutput[3]&0xC0)>>6;
       ReservedBit = BeaconDecodeOutput[4];
       
       
    //indexnum[0] = (((FrameIndex_temp-1)*3+ZoneIndex_temp)>>8)&0xff;
    //indexnum[1] = (((ZoneIndex_temp-1)*3+ZoneIndex_temp))&0xff;
      //  indexnum[0]  = ReservedBit;
       // indexnum[1]  = ReservedBit;
         indexnum[0] = (((ReservedBit)*3+ZoneIndex_temp)>>8)&0xff;
    indexnum[1] = (((ReservedBit)*3+ZoneIndex_temp))&0xff;
        
     
        UDP_Channel_Read(redata1);
        
      
        
        

        
      

        
        
        
      }
     
      
      for (i=0;i<128;i++)
      {
            redata[i]=0;
            redata1[i]=0;
            deinterleavebuffer[i] = 0;
      }
}

#endif
#ifdef SOFT_MERGE
#if 0//有barker的定于
uint8 redatabit1[576]={0};
uint8 first_redatabit1[576]={0};
uint8 second_redatabit1[576]={0};
uint8 third_redatabit1[576]={0};
uint8 fourth_redatabit1[576]={0};
uint8 merge_bit1[576]={0};
uint8 merge_bit2[576]={0};
uint8 merge_bit3[576]={0};
uint8 merge_bit4[576]={0};
#endif 

#if 1
uint8 first_redatabyte1[72]={0};
uint8 second_redatabyte1[72]={0};
uint8 third_redatabyte1[72]={0};
uint8 fourth_redatabyte1[72]={0};
#endif

#if 0
void byte2bit(unsigned char input[],unsigned char output[])
{
        uint16 i,j;
        //DEC2BIN
	for(i = 0; i < 72; i++)
	{
		for(j = 0; j < 8; j++)
		{
			output[7 - j + i*8] = (input[i] >> j)%2;
		}
	}

}

#endif

#if 0
void shift_bit(unsigned char input[],unsigned char output[], const unsigned int length, const unsigned char shiftbitlength)
{
        uint16 i,j;
       for(i = 0; i < length; i++)
       {
          output[i]=0;
        }
		
		
		 //shift,移位 .不偏移
        if (shiftbitlength==0)//左移0位
        {
        for(i = 0; i < length; i++)
	   {
          output[i]=input[i];
        }
        }
		

       //shift,移位 .左移1位
        if (shiftbitlength==1)//左移1位
        {
        for(i = 0; i < length-1; i++)
	{
          output[i]=input[i+1];
        }
        }
       
        
        //shift,移位。右移1位
         if (shiftbitlength==2)//右移1位
        {
        for(i = length-1; i >0; i--)
	{
          output[i]=input[i-1];
        }
        }
        
        
         //shift,移位 .左移2位
        if (shiftbitlength==3)//左移2位
        {
        for(i = 0; i < length-2; i++)
	{
          output[i]=input[i+2];
        }
        }
        
        
         //shift,移位。右移2位
         if (shiftbitlength==4)//右移2位
        {
        for(i = length-1; i >1; i--)
	{
          output[i]=input[i-2];
        }
        }
        
             //shift,移位 .左移3位
        if (shiftbitlength==5)//左移3位
        {
        for(i = 0; i < length-3; i++)
	    {
          output[i]=input[i+3];
        }
        }
        
             //shift,移位。右移3位
         if (shiftbitlength==6)//右移3位
        {
        for(i = length-1; i >2; i--)
	    {
          output[i]=input[i-3];
        }
        }
                
      if (interleaveEnable==1)
       {
       uint8 output1[576] = {0};
       for(i=0;i<length;i++)
       {
        output1[interleaveArray[i]] = output[i];//deinterleave
        }
       for(i=0;i<length;i++)
       {
        output[i] = output1[i];
        }
       }
}

#endif

#if 0
void merge(unsigned char data_bit[], unsigned char datastore[], unsigned char datamerge[])
{
  uint16 i,j;

	
	for(i = 0; i < 72*8; i++)
	{
	datamerge[i] = data_bit[i] + datastore[i];
	}
}

#endif



uint8 reTx_count=0;

void Set_reTx_count(uint8 x)
{
  reTx_count=x;
}
extern uint8 wrong_min_d;//2015-02-06
void ReSet_reTx_count(void)
{
  wrong_min_d = 0;
  reTx_count=0;
}


void reTx_count_plusplus(void)
{
  reTx_count++;
}

uint8 DataDecodeOutput[DataCRCOutputLength] = {0};//2015.02.09 



#if 1//添加barker的处理，2015-5-19 14:38:47
void Raw_Rx_Packet_Hanlder(void) //Added dai
{
unsigned char i;
#ifdef KEEP_GARBAGGE  
  uint8 size,crc,InitLength;
  unsigned char j,K=4;
#endif
  uint8 crc=1;
#ifdef KEEP_GARBAGGE    
  uint16 min_d_tmp=0;
  
	uint8 BeaconEncodeOutput[BeaconLength] = {0};
	uint8 DataEncodeOutput[DataLength] = {0};
#endif
	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0xFF};
        kernel_mem_set(BeaconDecodeOutput,0xFF,BeaconCRCOutputLength);
  
    SEGMENT_VARIABLE(redata1[128], U8);
    
  //85byte进，72byte出。包含找到准确的位置和deinterleave
   barkerCalculator(redata,redata1);//不管beacon和infodata都是这么处理
    //rxframetype=beacon;
    //beacon的处理
    if (rxframetype==beacon)
    {
       
      crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
      
      if(crc==0)
      {beaconright++;}
     }
    
    //infodata的处理
#ifdef KEEP_GARBAGGE      
   uint32 daiinfodatatimebegin = 0;  
#endif
    if (rxframetype==infodata)
    {
      
   // daiinfodatatimebegin =   kernel_GetSystemClock();

#if 1 //new barker 
    // byte2bit(redata1,redatabit1);
     
     crc = 1;

      if (reTx_count==0)
    {  
       //min_d_tmp=tbcc_dec_bit2byte_infodata(redatabit1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      //byte to byte:1
      tbcc_dec(redata1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      crc=CRCCheck(DataDecodeOutput,infodata);
    }
   
   if (reTx_count==1)
    {  
       //merge(first_redatabit1,redatabit1,merge_bit1);
       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      tbcc_dec_1(redata1,first_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      crc=CRCCheck(DataDecodeOutput,infodata);
    }
   
   if (reTx_count==2)
    {  
       //merge(first_redatabit1,second_redatabit1,merge_bit1);
       //merge(merge_bit1,redatabit1,merge_bit2);
       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit2,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      tbcc_dec_2(redata1,first_redatabyte1,second_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      crc=CRCCheck(DataDecodeOutput,infodata);
    }
   
   if (reTx_count==3)
    {  
       //merge(first_redatabit1,second_redatabit1,merge_bit1);
       //merge(merge_bit1,third_redatabit1,merge_bit2);
       //merge(merge_bit2,redatabit1,merge_bit3);
       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit3,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
     tbcc_dec_3(redata1,first_redatabyte1,second_redatabyte1,third_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count); 
     crc=CRCCheck(DataDecodeOutput,infodata);
    }
   
     if (reTx_count==4)
    {  
       //merge(first_redatabit1,second_redatabit1,merge_bit1);
       //merge(merge_bit1,third_redatabit1,merge_bit2);
       //merge(merge_bit2,fourth_redatabit1,merge_bit3);
       //merge(merge_bit3,redatabit1,merge_bit4);
       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit4,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
      tbcc_dec_4(redata1,first_redatabyte1,second_redatabyte1,third_redatabyte1,fourth_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);  
      crc=CRCCheck(DataDecodeOutput,infodata);
    }
   

   if (reTx_count==0)
   {
   //kernel_memcpy(first_redatabit1,redatabit1,576);//将原始数据保存
   kernel_memcpy(first_redatabyte1,redata1,72);//将原始数据保存
   }
   
    if (reTx_count==1)
   {
   //kernel_memcpy(second_redatabit1,redatabit1,576);//将原始数据保存
   kernel_memcpy(second_redatabyte1,redata1,72);//将原始数据保存
   }
   
   if (reTx_count==2)
   {
   //kernel_memcpy(third_redatabit1,redatabit1,576);//将原始数据保存
   kernel_memcpy(third_redatabyte1,redata1,72);//将原始数据保存
   }
   
   if (reTx_count==3)
   {
   //kernel_memcpy(fourth_redatabit1,redatabit1,576);//将原始数据保存
   kernel_memcpy(fourth_redatabyte1,redata1,72);//将原始数据保存
   }
      //必须添加
    if(crc==0)
    {
     reTx_count=0;
    }    
   
   

//重新生成redata1，用于上传
  tbcc_enc(DataDecodeOutput,DataCRCOutputLength,redata1);
    }
#endif   
     
    uint8 DecodeCRC = crc;
    if (crc==0)//crc=0时为解调正确
       {
        crc=1;
      if (rxframetype==infodata)
        {
          crcRightcont++;
      //  uint32 daiinfodatatimeend = kernel_GetSystemClock();
      //  uint16 daiinfodatatimechazhi = daiinfodatatimeend - daiinfodatatimebegin;
        //UartSendByte4_uint16dai(daiinfodatatimechazhi);
       // UartSendByte4_uint8dai(reTx_count);
        }
       
       }
             ///freq_autotest
       if(rxframetype==beacon)
        {
#ifdef KEEP_GARBAGGE            
          uint32 FrameIndex_temp;
          uint32 ZoneIndex_temp;
#endif
          uint32 ReservedBit;
#ifdef KEEP_GARBAGGE            
           FrameIndex_temp = get_UINT16(BeaconDecodeOutput[1],BeaconDecodeOutput[2]);
           ZoneIndex_temp = (BeaconDecodeOutput[3]&0xC0)>>6;
#endif
           ReservedBit = BeaconDecodeOutput[4];
                 
            indexnum[0]  = (ReservedBit>>8)&0xff;
            indexnum[1]  = ReservedBit&0xff;

        }
        ////
            for ( i=0;i<85;i++)
               {
              if (redata[i]!=0)
              {
                break;
              }
               }
            if (i == 85)
            {
#ifdef ENABLE_USART_DEBUG_INFO_DAI              
             UartSendByte4dai("Allredata0\n",12);
#endif
            }
        /////
             for ( i=0;i<72;i++)
               {
              if (redata1[i]!=0)
              {
                break;
              }
               }
            if (i == 72)
            {
#ifdef ENABLE_USART_DEBUG_INFO_DAI              
             UartSendByte4dai("Allredata1\n",12);
#endif
            }
    
                   rxtotal2right = 0;
            for ( i=2;i<22;i++)//2015-02-06
            {
             // if (((uint8)rand())==DataDecodeOutput[i])
                if ((i-2)==DataDecodeOutput[i])
              {
                rxtotal2right++;
              }

            }
    
        

        // UDP_Receive(redata1);//server
        //UDP_Channel_Read(redata1);//client
        UDP_Channel_Read(redata1,BeaconDecodeOutput,DataDecodeOutput,DecodeCRC);



          for (i=0;i<128;i++)
        {
            redata[i]=0;
            redata1[i]=0;
         }

      
}
#endif


#endif







/////////针对真实的情况，而不是测试性能用的test函数，即发送端发送的是真实的帧，而不是我们构造的帧


uint8 info_scan[6] = {0xff,0xff,0,0,0,0};
extern uint8 tmpCRC;

//发送的帧里面包含index信息
extern  uint8 frequencyEnable;
extern uint8 CenfreqfixFlag;  
extern uint32 rightbeaconcnt;

void freq_scan_for_realApp(void)
{
  if (frequencyEnable==0)//未开启了freq_scanning
   {
     CenfreqfixFlag = 1;
   }
  
  
   if (frequencyEnable==1)//开启了freq_scanning
   {
   if (CenfreqfixFlag==0)//频点未固定
     {
       
    rightbeaconcnt++;
    //rxtotal1 = 0;
    //rxtotal2 = 0;
    

     }
   }
   
 
}


////////////////////////////////////////
//发送的帧里面包含index信息
extern  uint8 frequencyEnable;
extern uint8 CenfreqfixFlag;  
extern uint32 rightbeaconcnt;


uint32 aa = 0;
uint32 bb = 0;
uint32 cc = 0;
uint32 dd = 0;
uint32 ee = 0;
uint32 ff = 0;
uint8 freqscanFlag =1;
uint8 beginre =1;
uint8 freq_method_cnt = 0;
extern uint8 freq_method;
uint8 wait_cntFlag = 0;

extern uint8 info_scan[6];//必须加，不然打印的东西不对
extern uint8 indexnum[2];

uint8 infofreq[2]={0xAB,0xAB};
extern uint8 info_scan[6];
extern uint16 rxtotal3;
extern uint16 rxtotal1;
extern uint32 count_beacon_ack_send;

void freq_scan_for_test(void)
{
  uint32 totaltemp = 0;
  
  totaltemp = rxtotal3;//crc对的数。
 // totaltemp = rxtotal1;//同步对的数。
  
    if (frequencyEnable==0)//未开启freqscanning
       {
         CenfreqfixFlag = 1;
         
          if (beginre == 1)//一开始为1
           {
#ifdef ENABLE_USART_DEBUG_INFO_DAI
             UartSendByte4dai("first Beacon\n",13);
#endif
           aa = indexnum[0]*256;
           aa = aa+indexnum[1];//初始的数
           bb = totaltemp;//初始的数
           beginre = 2;
           }
          else if (beginre ==2 )
          {
           cc = indexnum[0]*256;
           cc = cc+indexnum[1];
           dd = totaltemp;
           
           ee = cc - aa;//总共发送的包的个数
           //保护第一个包是正确的
           if(ee>100)
           {
           beginre = 1;//重新去读aa
           }
           else
           {
           beginre = 0;
           }
          }
          
          else if(beginre == 0)
          {
           cc = indexnum[0]*256;
           cc = cc+indexnum[1];
           dd = totaltemp;
           
           ee = cc - aa;//总共发送的包的个数
           ff = dd - bb;//实际接收到包的个数
           
           if (ff ==0)
           {
           ff = 0;
           }
           
           info_scan[2] = (ee>>8)&0xff;
           info_scan[3] = ee&0xff;
           info_scan[4]= (ff>>8)&0xff;
           info_scan[5] = ff&0xff;
          }
       }
      
      
      
      if (frequencyEnable==1)//开启了freqscan
       {
       if (CenfreqfixFlag==1)//频点固定
         {
           if (freqscanFlag == 1)//一开始为1
           {
#ifdef ENABLE_USART_DEBUG_INFO_DAI
             UartSendByte4dai("first Beacon\n",13);
#endif
           aa = indexnum[0]*256;
            aa   =   aa+indexnum[1];//初始的数
           bb = totaltemp;//初始的数
           freqscanFlag = 2;
           }
           
          else if (freqscanFlag ==2 )
          {
           cc = indexnum[0]*256;
           cc = cc+indexnum[1];
           dd = totaltemp;
           
           ee = cc - aa;//总共发送的包的个数
           //保护第一个包是正确的
           if(ee>100)
           {
           freqscanFlag = 1;//重新去读aa
           }
           else
           {
           freqscanFlag = 0;
           }
          }
    
          else if (freqscanFlag == 0)
          {                      
          if (freq_method==1)
           {
              freq_method_cnt++;
          
              if (freq_method_cnt<=5)//实际是3*4个包
                {
                freqscanFlag = 1;//重新来了一次，但是这次 wait_cntFlag==1;
                 }
              else
              {
              freqscanFlag = 0;
              freq_method_cnt = 100;
              
              
              cc = indexnum[0]*256;
              cc = cc+indexnum[1];
              dd = totaltemp;
           
              ee = cc - aa;//总共发送的包的个数
              ff = dd - bb;//实际接收到包的个数
        
              
              info_scan[2] = (ee>>8)&0xff;
              info_scan[3] = ee&0xff;
              info_scan[4]= (ff>>8)&0xff;
              info_scan[5] = ff&0xff;
              
              
              }
          }
          else//(freq_method!=1)
          {
            
            
            
           cc = indexnum[0]*256;
           cc = cc+indexnum[1];
           dd = totaltemp;
           
           ee = cc - aa;//总共发送的包的个数
           ff = dd - bb;//实际接收到包的个数
           
           
           info_scan[2] = (ee>>8)&0xff;
           info_scan[3] = ee&0xff;
           info_scan[4]= (ff>>8)&0xff;
           info_scan[5] = ff&0xff;
          }
          }
         }
       else//频点未固定
         {
         rightbeaconcnt++;
         rxtotal3=0;//频点未固定时，rxtotal3都复位0
         rxtotal1 = 0;
         }
      }
    
   if (count_beacon_ack_send==0)
    {

    }    
 // if ((count_beacon_ack_send%20==0)&&(count_beacon_ack_send>5))
    
    {
#ifdef ENABLE_USART_DEBUG_INFO_DAI
      UartSendByte4dai("B:",2);
       UartSendByte4_uint16dai(ee);
       UartSendByte4_uint16dai(ff);
       UartSendByte4dai("\n",2);
#endif
     }
}
///end

void paixu(unsigned int input[],unsigned char output[],unsigned char num)
{
  unsigned int temp =0 ;
  unsigned int backupinput[25]={0};
  uint8 i,j;
  uint8 temp_index = 0;
  for (i = 0;i<num;i++)
  {
  output[i] = i;
  backupinput[i]  = input[i];
  }
 
  for (i = 0;i<num;i++)
  {
    for (j = i+1;j<num;j++)
    {
      
      if (backupinput[j]<backupinput[i])
      {
        temp  = backupinput[j];
        backupinput[j] = backupinput[i];
        backupinput[i] = temp;
        
       temp_index  = output[i];
      output[i] = output[j];
      output[j] = temp_index;
     
      }
    }
  }
  
}
#ifdef ENABLE_SLOWRUN_DEBUG_MARKER
uint32 m1=0,m2=0,m3=0,m4=0,m5=0,m6=0,m7=0,m8=0,m9=0,m10=0,m11=0,m12=0,m13=0,m14=0,m15=0,m16=0,m17=0,m18=0,m19=0,m20=0;
uint32 d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19;
uint8 c1,c2,c3,c4,c5,c6,c7,c8,c9;

void Save_CRC(uint8 x)
{
   if(c1 == 0)
   {
    c1 = x;
   }
   else if(c2 == 0)
   {
    c2 = x;
   }
   else if(c3 == 0)
   {
    c3 = x;
   }
   else if(c4 == 0)
   {
    c4 = x;
   }
   else if(c5 == 0)
   {
    c5 = x;
   }
   else if(c6 == 0)
   {
    c6 = x;
   }
   else if(c7 == 0)
   {
    c7 = x;
   }
   else if(c8 == 0)
   {
    c8 = x;
   }
   else if(c9 == 0)
   {
    c9 = x;
   }
}

extern uint8 SLOWRUN_TEST;

void Save_TimeMarker(void)
{
     if(m1 == 0)
    {
      
      m1 = kernel_GetSystemClock();
    }
    else if(m2 == 0)
    {
#ifdef ENABLE_LOWPOWER_CLK_SWITCH      
      Time_Static_Start();
#endif
      m2 = kernel_GetSystemClock();
      d1 = m2 - m1;
    }
    else if(m3 == 0)
    {
      m3 = kernel_GetSystemClock();
      d2 = m3 - m2;
      asm("nop");
    }
    else if(m4 == 0)
    {
      
      m4 = kernel_GetSystemClock();
      d3 = m4 - m3;
    }
    else if(m5 == 0)
    {
      //SLOWRUN_TEST = 1;
      m5 = kernel_GetSystemClock();
      d4 = m5 - m4;
    }
    else if(m6 == 0)
    {
      m6 = kernel_GetSystemClock();
      d5 = m6 - m5;
    }
    else if(m7 == 0)
    {
      m7 = kernel_GetSystemClock();
      d6 = m7 - m6;
    }
    else if(m8 == 0)
    {
      m8 = kernel_GetSystemClock();
      d7 = m8 - m7;
    }
    else if(m9 == 0)
    {
      m9 = kernel_GetSystemClock();
      d8 = m9 - m8;
    }
    else if(m10 == 0)
    {
      m10 = kernel_GetSystemClock();
      d9 = m10 - m9;
    }
     else if(m11 == 0)
    {
      m11 = kernel_GetSystemClock();
      d10 = m11 - m10;
    }
     else if(m12 == 0)
    {
      m12 = kernel_GetSystemClock();
      d11 = m12 - m11;
    }
     else if(m13 == 0)
    {
      m13 = kernel_GetSystemClock();
      d12 = m13 - m12;
      asm("nop");
    }
     else if(m14 == 0)
    {
      m14 = kernel_GetSystemClock();
      d13 = m14 - m13;
    }
     else if(m15 == 0)
    {
      m15 = kernel_GetSystemClock();
      d14 = m15 - m14;
    }
     else if(m16 == 0)
    {
      m16 = kernel_GetSystemClock();
      d15 = m16 - m15;
    }
    else if(m17 == 0)
    {
      m17 = kernel_GetSystemClock();
      d16 = m17 - m16;
    }
    else if(m18 == 0)
    {
      m18 = kernel_GetSystemClock();
      d17 = m18 - m17;
    }
    else if(m19 == 0)
    {
      m19 = kernel_GetSystemClock();
      d18 = m18 - m17;
    }
    else if(m20 == 0)
    {
      m20 = kernel_GetSystemClock();
      d19 = m20 - m19;
    }
    else
    {
      asm("nop");
    }
}
#endif