#include "radio.h"
#include "macro.h"
#include "si4463.h"
#include "si4463_def.h"
#include "radio_config.h"
#include "radio_config_1st.h"
#include "radio_config_2nd.h"
#include "radio_config_ack.h"
#include "radio_comm.h"
#include "radio_hal.h"
#include "radio_mcu.h"
#include "uart.h"
//#include "Server_Process.h"
//#include "FrameSlot.h"

extern uint8 interleaveEnable ;//2015-4-17 10:10:09
//extern unsigned int interleaveArray[576];
uint8 StartTxInSysTickFlag = 0;

uint8 info[8] = {0xFF,0xFF,0,0,0,0};   //1016
uint16 rxtotal3 = 0;                   //1016

uint32 TxDelayCounter = 0;

//extern MAC_PIB_T MacPib;

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
 
   SEGMENT_VARIABLE(redata00[72], U8);
  SEGMENT_VARIABLE(redata11[72], U8);
  SEGMENT_VARIABLE(redata22[72], U8);
  SEGMENT_VARIABLE(redata33[72], U8);
  
  uint16 crcRightcont  = 0;
  uint8 indexnum[2]={0};

void Reset_TxDelayCounter(void)
{
  TxDelayCounter = 0;
}

void Add_TxDelayCounter(void)
{
  //TxDelayCounter++;

  //if(TxDelayCounter == 20000)
  {
    si446x_start_tx(0u, 0x30,  0x00);
  
    TIM_Cmd(TIM4, DISABLE);
  }
}

uint8 BA_Counter;

SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8) = \
              RADIO_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration) = \
                        RADIO_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration) = \
                        &RadioConfiguration;

//只有sync无data的数据的配置,1st

SEGMENT_VARIABLE(Radio_1st_Configuration_Data_Array[], U8) = \
              RADIO_1st_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration_1st, tRadioConfiguration) = \
                        RADIO_1st_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration_1st, tRadioConfiguration) = \
                        &RadioConfiguration_1st;

//无sync只有data的数据的配置,2nd

SEGMENT_VARIABLE(Radio_2nd_Configuration_Data_Array[], U8) = \
              RADIO_2nd_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration_2nd, tRadioConfiguration) = \
                        RADIO_2nd_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration_2nd, tRadioConfiguration) = \
                        &RadioConfiguration_2nd;

SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], U8);

//发送2字节的ack的配置,ack

SEGMENT_VARIABLE(Radio_ack_Configuration_Data_Array[], U8) = \
              RADIO_ack_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration_ack, tRadioConfiguration) = \
                        RADIO_ack_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration_ack, tRadioConfiguration) = \
                        &RadioConfiguration_ack;

extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

//extern STATE NowMacState;
void vRadio_PowerUp(void)
{
  SEGMENT_VARIABLE(wDelay,  U16) = 0u;

  /* Hardware reset the chip */
  si446x_reset();

  /* Wait until reset timeout or Reset IT signal */
  for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}

void vRadio_Init(void)
{
  U16 wDelay;

  /* Power Up the radio chip */
  vRadio_PowerUp();

  /* Load radio configuration */
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
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
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration_1st->Radio_ConfigurationArray))
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
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration_2nd->Radio_ConfigurationArray))
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

  /* Load radio configuration */
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration_ack->Radio_ConfigurationArray))
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
void DemoApp_Pollhandler_RX()
{
    //  Radio_Receive_FixedLength();
#if 0
  if((BA_RX == NowMacState) || (A_RX == NowMacState ))
  {
    Radio_Receive_FixedLength();
  }
  else
  {
    if(D_RX == NowMacState)
    {
      rxframetype = infodata;
    }
    else
    {
      rxframetype = beacon;
    }
    
    TRUE == gRadio_CheckReceived();
  }    
#endif
  

}

void Reset_Buf(uint8 * p, uint8 s)
{
  uint8 i;
  
  for(i=0;i<s;i++)
    p[i] = 0;
}

uint16 rxtotal1 = 0;
uint16 rxtotal2 = 0;

 extern uint32 tmpGlobalTime;
 
 uint32 delaystartMs=0;
 
 SEGMENT_VARIABLE(redata[128], U8);
 
 uint32 n_true=0;
  uint32 n_false=0;
    uint32 n_total=0;
      uint32 n_crc=0;
    
    uint32 n_total_true=0;

  
  //用于frequency scanning
  
   uint8 CenfreqfixFlag =0;  
uint32 scanCntRx[11]={0};

uint8 maxindex=0;
uint32 starttime=0;
uint32 nowtime=0;
uint32 freq_configurationCnt=0;
  



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
}

}
return maxre;

}
 //end frequency scanning


  uint32 delaytimeMs=5;  
  uint32 delaytimeUs=238;
  
  //5,228:最初的，能够收到，但是在低snr条件下情况不好。
  
  extern uint32 TIMcont;
  extern uint8 rssihaveread ;
extern int rssidB;
uint32      rssicont = 0;
uint32      rssisum = 0;
  int AvgdBdB_shi=0;
    int AvgdBdB_ge=0;

#if 0   
uint8 Radio_Receive_FixedLength(void)
{
  //
  unsigned char i;
  uint8 size,crc,InitLength;
  uint16 rssi;
  unsigned char BYTEOF1BITcount=0;
  unsigned char temp;
  unsigned char j,K=4;
  crc=1;
  ////////////////////////for tx
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
  
  uint8 nirq_recv= SI4463_NIRQ_Bit();
  
  /* Read ITs, clear pending ones */
    //si446x_get_int_status(0u, 0u, 0u);
  //nirq_recv= SI4463_NIRQ_Bit();
  
 
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
  
  


   uint32 time0 = 0;
   uint32 time1 = 0;
   uint32 time2 = 0;
   uint32 time3 = 0;
   
   uint8 rightCnt=0;
   
  uint8 stopFlag=0;
  
  uint8 fifo_cout=0;
  n_total++;
  
  if (nirq_recv==TRUE)
  {
  n_total_true++;
  }
  
  
 //if (nirq_recv == FALSE)
  //{
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);
  
  
   #if 0
if (Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)

  {
        if (rssihaveread==0)
        {
      rssi  = sRadioGetRSSI();
      uint8 rssioffset = 133;
      
       if (rssi>0)
      {
        //rssicont++;
        //rssisum = rssisum+rssi;
        
        rssicont = 1;
        rssisum = rssi;
      
        AvgdBdB_shi = (13300 - 100*rssisum/rssicont/2)/100;
        AvgdBdB_ge = (13300 - 100*rssisum/rssicont/2)%100;
        
        SaveTmpRSSI(AvgdBdB_shi, AvgdBdB_ge);
      }

      rssidB = rssi/2-rssioffset;
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
      //有数据来，rxtotal++
   //  rxtotal1++;
       
      
     rssihaveread=0;
      
      if((BA_RX == NowMacState) || (A_RX == NowMacState ))
      {
        /* Read the length of RX_FIFO */
          si446x_fifo_info(0u);

      /* Packet RX */
          si446x_read_rx_fifo(2, &fixRadioPacket[0]);
          
          vRadio_config_ack();          //接收ACK以后，准备再接收一次ACK
          vRadio_StartRX_ack(pRadioConfiguration_ack->Radio_ChannelNumber);
          
          UDP_Receive(fixRadioPacket);
          Toggle_LED1();
          
          return TRUE;
      }
       
      /* Read the length of RX_FIFO */
      si446x_fifo_info(0u);

      /* Packet RX */
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
      
      


  rxtotal1++;

  
  
   
     TIM_Cmd(TIM3, ENABLE);
      


#if 0
     DelayMs(delaytimeMs);
    
   DelayUs(delaytimeUs);
 vRadio_config_2nd();
 
      vRadio_StartRX_2nd(pRadioConfiguration_2nd->Radio_ChannelNumber);
    
      
      #if 1     
        while (Si446xCmd.FIFO_INFO.RX_FIFO_COUNT<60)
        {
          DelayUs(1);
        

         si446x_fifo_info(0u);
        }
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &redata[0]);
        si446x_fifo_info(0u);
        

        
       while (Si446xCmd.FIFO_INFO.RX_FIFO_COUNT<12)
        {
         DelayUs(1);
      
         
         si446x_fifo_info(0u);
        }

      
      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &redata[60]);
     
     #endif  
      
           
      ///
       if (rxframetype==beacon)
             {
             for(i=0; i<rxPayloadLength; i++)
          BeaconDeinterleaveOutput[i] = redata[i];         

          crc = decode_tbcc_beacon4repetition(BeaconDeinterleaveOutput,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算
          

             }
          
             if (rxframetype==infodata)
             {
             for(i=0; i<rxPayloadLength; i++)
          rxDataInterleaveOutput[i] = redata[i];         


           tbcc_dec(rxDataInterleaveOutput,DataCRCOutputLength,DataDecodeOutput,1);
		   crc = CRCCheck(DataDecodeOutput,infodata);
    
             }
             
                if (rxframetype==ACK)
             {
             for(i=0; i<rxPayloadLength; i++)
          rxACKInterleaveOutput[i] = redata[i];         
          deinterleave(rxACKInterleaveOutput,ACKDeinterleaveOutput,ACK);
          
         
           crc = decode2(ACKDeinterleaveOutput,ACKDecodeOutput,ACK);
             }
          
          if (crc==0)//crc=0时为解调正确
          {
            crc=1;
            rxtotal3++;
          
       
       
          
       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   

            if(rxframetype==beacon)
            {
           for ( i=0;i<InitLength;i++)
              if (BeaconInit[i]!=BeaconDecodeOutput[i])
              {
                break;
              }
            }
            
             if(rxframetype==infodata)
            {
           for ( i=0;i<InitLength;i++)
              if (DataInit[i]!=DataDecodeOutput[i])
              {
                break;
              }
            }
            
            
            if(rxframetype==ACK)
            {
           for ( i=0;i<InitLength;i++)
              if (ACKInit[i]!=ACKDecodeOutput[i])
              {
                break;
              }
            }
          if (i == InitLength)
            rxtotal2++;//全对           
           
          }
      vRadio_config_1st();
     vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);
     
   //  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
      ///
      
      info[2] = (rxtotal1>>8)&0xff;
      info[3] = rxtotal1&0xff;
      info[4] = (rxtotal2>>8)&0xff;
      info[5] = rxtotal2&0xff;
     // info[6] = (rxtotal3>>8)&0xff;
     // info[7] = rxtotal3&0xff;

          
          /* Send it to UART */
        //UartSendByte(fixRadioPacket,RadioConfiguration.Radio_PacketLength);
        UartSendByte(info,6);
#endif

      

       si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
       
       si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
    
       
      vRadio_config_2nd(); 
      return TRUE;
    }

   
    
  
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);


  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
  
  return FALSE;
  

}
#endif 









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
//uint8 gRadio_CheckReceived(void)
//{
//  //
//  SEGMENT_VARIABLE(redata1[128], U8);
//   SEGMENT_VARIABLE(deinterleavebuffer[72], U8);
//  unsigned char i;
//  uint8 size,crc,InitLength;
//  uint16 rssi;
//  unsigned char BYTEOF1BITcount=0;
//  unsigned char temp;
//  unsigned char j,K=4;
//  crc=1;
//  ////////////////////////for tx
//      uint8 BeaconInit[BeaconInitLength] = {0};
//	uint8 DataInit[DataInitLength] = {0};
//	uint8 ACKInit[ACKInitLength] = {0};
//
//	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
//	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
//	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};
//
//
//    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};
//
//	uint8 BeaconEncodeOutput[BeaconLength] = {0};
//	uint8 DataEncodeOutput[DataLength] = {0};
//	uint8 ACKEncodeOutput[ACKLength] = {0};
//
//	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 DataInterleaveOutput[DataLength] = {0};
//	uint8 ACKInterleaveOutput[ACKLength] = {0};
/////////////////////////////////////////
//   
//        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 rxDataInterleaveOutput[DataLength] = {0};
//	uint8 rxACKInterleaveOutput[ACKLength] = {0};
//	
//     
//  
//	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
//	uint8 DataDeinterleaveOutput[DataLength] = {0};
//	uint8 ACKDeinterleaveOutput[ACKLength] = {0};
//
//
//	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
//      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
//	uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
//	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
//        
//   switch(rxframetype)
//  {
//  case beacon:
//   rxPayloadLength = BeaconLength;
//    InitLength = BeaconInitLength;
//    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
//    break;
//  case infodata:
//   rxPayloadLength = DataLength;
//    InitLength = DataInitLength;
//    SourceGenerate(DataInit,DataInitLength,sourceSeed);
////	myCRC(DataInit,DataCRCOutput,infodata);
//   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
//    break;  
//  case ACK:
//   rxPayloadLength = ACKLength;
//    InitLength = ACKInitLength;
//    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
//    break;
//  }
//  
//  
//  //
//  
//  uint8 nirq_recv= SI4463_NIRQ_Bit();
//  
//  /* Read ITs, clear pending ones */
//    //si446x_get_int_status(0u, 0u, 0u);
//  //nirq_recv= SI4463_NIRQ_Bit();
//  
// 
//  uint8 info_beacon[3]={0xFF,0,0};
//
//  U8 total = 0;
//  U8  yihuo = 0;
//  U8 num =0;
//
////barker code 0~5:101101110001011011100010110111000101101110001011:B716E2DC5B8B
//
////右移位1bit：      010110111000101101110001011011100010110111000101：5B8B716E2DC5：//移1bit相差27
//
////右移位2bit：      001011011100010110111000101101110001011011100010：2DC5B8B716E2：//移2bit相差26
//
////左移位1bit： 011011100010110111000101101110001011011100010110：6E2DC5B8B716:////移1bit相差27
//  uint8 baker[6];
//
//#if 1
//  baker[0]=0xB7;
//  baker[1]=0x16;
//  baker[2]=0xE2;
//  baker[3]=0xDC;
//  baker[4]=0x5B;
//  baker[5]=0x8B;
//#endif 
//  
//  
//
//
//   uint32 time0 = 0;
//   uint32 time1 = 0;
//   uint32 time2 = 0;
//   uint32 time3 = 0;
//   
//   uint8 rightCnt=0;
//   
//  uint8 stopFlag=0;
//  
//  uint8 fifo_cout=0;
//  n_total++;
//  
//  if (nirq_recv==TRUE)
//  {
//  n_total_true++;
//  }
// // rxtotal1++;
//  
// //if (nirq_recv == FALSE)
//  //{
//    /* Read ITs, clear pending ones */
//    si446x_get_int_status(0u, 0u, 0u);
//  
//  
//  
//  
//  
// #if 1
//if (Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)
//
//  {
//        if (rssihaveread==0)
//        {
//      rssi  = sRadioGetRSSI();
//      uint8 rssioffset = 133;
//      
//           if (rssi>0)
//      {
//        //rssicont++;
//        rssicont = 1;
//        //rssisum = rssisum+rssi;
//        rssisum = rssi;
//      
//        AvgdBdB_shi = (13300 - 100*rssisum/rssicont/2)/100;
//        AvgdBdB_ge = (13300 - 100*rssisum/rssicont/2)%100;
//        
//        SaveTmpRSSI(AvgdBdB_shi, AvgdBdB_ge);
//      }
//
//      rssidB = rssi/2-rssioffset;
//      //rssidB = 0;
//     rssi = 0;
//     rssihaveread=1;
//        }
//        
//     return TRUE;
//   }
//#endif
//
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//
//    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
//    
//    /* check the reason for the IT */
//    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
//    {
//      //有数据来，rxtotal++
//   // rxtotal1++;
//       
//      rssihaveread = 0;
//
//      
//      
//      
//      /* Read the length of RX_FIFO */
//      si446x_fifo_info(0u);
//
//      /* Packet RX */
//      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
//   
//    
//  if (fixRadioPacket[0]==0xB7)
//  {
//  //rxtotal1++;
//  int frame=2;
//  fixRadioPacket[0]=0;
//
//  }
//  rxtotal1++;
//       
//    configtimesup = 0;
//  TIMcont = 0;
//  fiforeadEnable = 0;
//  TIM3_Config();//用于第一帧和第二帧直接的延时控制
//  TIM_Cmd(TIM3, ENABLE); 
//   vRadio_config_2nd();
//   //UartSendByte4("I.",2);
//   return 0;
//   
//   
//   while (TIMcont<=0 );//3590
//   
//#if 1
//      if (TIMcont>=1)//795
//      {        
//         TIMcont=0;
//     TIM_Cmd(TIM3, DISABLE);//g关闭使能
//     TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /* Clear TIM3 update interrupt */
//   
//        //  vRadio_StartRX_2nd(pRadioConfiguration_2nd->Radio_ChannelNumber);
//        //  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);//必须加
//        //  si446x_fifo_info(0u);//读取count的值
//    // vRadio_config_2nd();
//   //vRadio_StartRX_2nd(pRadioConfiguration_2nd->Radio_ChannelNumber);
//
//// si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
//// si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
////18945
//     
//      
//      #if 1     
//    
//     #if 1
//     //  DelayMs(2000);
//      
//        
//       //si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);//必须加
//       // si446x_fifo_info(0u);//读取count的值
//     //  fifocount_orign = Si446xCmd.FIFO_INFO.RX_FIFO_COUNT ;
//      
//#if 1
//        TIM3_Rx_Config();//用于接收fifo中的数
//       TIMcont = 0;
//       TIMcont2 = 0;
//       fiforeadEnable = 1;
//       
//     vRadio_StartRX_2nd(pRadioConfiguration_2nd->Radio_ChannelNumber);
//   
//       
//       TIM_Cmd(TIM3, ENABLE); 
//       
//       while(fiforeadEnable==1);
//       
//      TIM_Cmd(TIM3, DISABLE);//g关闭使能
//     TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /* Clear TIM3 update interrupt */
//     TIMcont =0;
//     fiforeadEnable = 0;
//#endif  
//#if 0
//     vRadio_StartRX_2nd(pRadioConfiguration_2nd->Radio_ChannelNumber);
//
//        while ((Si446xCmd.FIFO_INFO.RX_FIFO_COUNT )<60)
//        {
//          DelayUs(1);
//        
//         si446x_fifo_info(0u);
//         
//         
//        }
//      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &redata[0]);
//        si446x_fifo_info(0u);
//        
//        
//       // fifocount_orign  = 0;
//     //fifocount_orign = Si446xCmd.FIFO_INFO.RX_FIFO_COUNT ; 
//        while ((Si446xCmd.FIFO_INFO.RX_FIFO_COUNT)<12)
//        {
//          DelayUs(1);
//       
//         si446x_fifo_info(0u);
//        // fifocount_now = Si446xCmd.FIFO_INFO.RX_FIFO_COUNT ;
//        }
//
//      
//      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &redata[60]);
//
//#endif
//#endif
//    
//
//      
//       //DelayMs(4000);
//      
//          if (rxframetype==beacon)
//             {
//          shift(redata,deinterleavebuffer,72,0);//input,output:  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//#if 1
//          if ((BeaconDecodeOutput[4]!=0x99)&&(BeaconDecodeOutput[4]!=0x04))
//          {
//            crc = crc+1;
//            crc = crc -1;
//          }
//
//
//          if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,1);//input,output:左移或者右移1bit或者2bit  
//           deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);     
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏小
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,2);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//       //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏大
//          }
//#if 1
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,3);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,4);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,5);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,6);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//              if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,7);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,8);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         //ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//                          ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x00)
//      {
//         if (BeaconDecodeOutput[1]==0x00)
//         {
//            if (BeaconDecodeOutput[2]==0x00)
//            {
//              if (BeaconDecodeOutput[3]==0x00)
//              {
//                if (BeaconDecodeOutput[4]==0x00)
//                {
//                  error00++;
//                  crc = 1;//crc错误
//                }
//              }
//            }
//         }
//      }
//          
//#endif
//#endif  
//             }
//          
//             if (rxframetype==infodata)
//             {
//                    
//           tbcc_dec(redata,DataCRCOutputLength,DataDecodeOutput,1);
//		   crc = CRCCheck(DataDecodeOutput,infodata);
//    
//      #if 1
//          if (crc!=0)
//          {
//          shift(redata,redata1,72,1);//input,output:左移或者右移1bit或者2bit  
//          tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);   
//   if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,redata1,72,2);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//      if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//       #if 0 
//           if (crc!=0)
//          {
//          shift(redata,redata1,72,3);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//   if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,redata1,72,4);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//     if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//
//             if (crc!=0)
//          {
//          shift(redata,redata1,72,5);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//     if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//   if (crc!=0)
//          {
//
//          shift(redata,redata1,72,6);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//     if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//#endif
//          
//#endif               
//                   
//             }         
//             
//          
//          
//          if (crc==0)//crc=0时为解调正确
//          {
//            crc=1;
//            rxtotal3++;
//          
//       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   
//
//            if(rxframetype==beacon)
//            {
//           for ( i=0;i<5;i++)
//              if (i!=BeaconDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 5)
//            rxtotal2++;//全对   
//            }
//            
//             if(rxframetype==infodata)
//            {
//           for ( i=0;i<22;i++)
//              if (i!=DataDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 22)
//            rxtotal2++;//全对   
//            }
//
//
//           
//          }
//      
//
//      ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x01)
//      {
//         if (BeaconDecodeOutput[1]==0x6F)
//         {
//            if (BeaconDecodeOutput[2]==0xFF)
//            {
//              if (BeaconDecodeOutput[3]==0x10)
//              {
//                if (BeaconDecodeOutput[4]==0x99)
//                {
//                  rxtotal2++;
//                }
//              }
//            }
//         }
//      }
//      
//      ///
// info[2] = (rxtotal1>>8)&0xff;
//      info[3] = rxtotal1&0xff;
//      info[4] = (rxtotal2>>8)&0xff;
//      info[5] = rxtotal2&0xff;
//
//      
//       if (rxtotal1!=rxtotal2)
//      {
//      rxtotal2 = rxtotal2 ; 
//      }
//      
//      
//      if (rxtotal1==1000)
//      {
//      rxtotal2 = rxtotal2 ; 
//      }
//      
//      if (BeaconDecodeOutput[1] == 0x00)
//      {
//      BeaconDecodeOutput[1] = 0x00;
//      }
//      
//      
//    UartSendByte(info,6);
//      //if(crc == 0)
//      {
//        crc=1;
//      //  UartSendByte20140911("**\n",3);
//        UDP_Receive(redata1);
//        
//        uint8 Space[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//        
//        
//        
//        //UartSendByte20141021(Space, 6);
//        
//        uint8 ccccc = 0;
//        
////        for(ccccc = 0;ccccc<72; ccccc++)
////          UartSendByte20141021(redata1+ccccc, 1);
////        
//  //    UartSendByte20141021(Space, 6);
//      UartSendByte20141021(BeaconDecodeOutput, 5);
//        
//      
//
//        
//        
//        Toggle_LED1();
//      }
//      //else
//      {
//        //UartSendByte20140911("++\n",3);
//      }
//      
//      
//      for (i=0;i<128;i++)
//      {
//            redata[i]=0;
//            redata1[i]=0;
//            deinterleavebuffer[i] = 0;
//      }
//           vRadio_config_1st();
//     vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);
//     
//     si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//   si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//     #endif  
//     
//     
//             ///////////////////////////////////////////////
//   
//
//      
//        
//  
//      
//      }
//#endif
//   
// //vRadio_StartRX_2nd(pRadioConfiguration_2nd->Radio_ChannelNumber);
//   // TIM_Cmd(TIM3, DISABLE); 
//    //TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
//  
//     si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//   si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//    //si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//      return TRUE;
//    }
//    
//#if 0
//    	if((Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_RX_FIFO_ALMOST_FULL_BIT))
//          {
//                 
//              if (frame==2)
//              {
//               
//		  /* Calculate the number of free bytes in the array */
//		  bNumOfFreeBytes = RadioConfiguration_2nd.Radio_PacketLength - bPosition;
//               
//		  if (bNumOfFreeBytes >= RADIO_RX_ALMOST_FULL_THRESHOLD)
//                    
//		  { // free space in the array is more than the threshold
//	 si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY); 
//         
//			/* Read the RX FIFO with the number of THRESHOLD bytes */
//			si446x_read_rx_fifo(RADIO_RX_ALMOST_FULL_THRESHOLD, &redata[bPosition]);
//                        
//                     si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//	
//         
//                      
//			/* Calculate how many bytes are already stored in the array */
//			bPosition += RADIO_RX_ALMOST_FULL_THRESHOLD;
//                        
//                        
//	
//		  }
//		  else
//		  {
//           /* Read the RX FIFO with the number of THRESHOLD bytes */
//			si446x_read_rx_fifo(bNumOfFreeBytes, &redata[bPosition]);
//                        
//                  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//                  
//               
//	
//			/* Calculate how many bytes are already stored in the array */
//			bPosition =0;
//                        frame=1;
//                        
//                        	
//          if (rxframetype==beacon)
//             {
//            
//          crc = decode_tbcc_beacon4repetition(redata,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算
//  
//          
//          
//          
//             }
//          
//             if (rxframetype==infodata)
//             {
//                    
//
//
//           tbcc_dec(redata,DataCRCOutputLength,DataDecodeOutput,1);
//		   crc = CRCCheck(DataDecodeOutput,infodata);
//    
//             }
//          
//          
//          if (crc==0)//crc=0时为解调正确
//          {
//            crc=1;
//            rxtotal3++;
//          
//       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   
//
//            if(rxframetype==beacon)
//            {
//           for ( i=0;i<5;i++)
//              if (i!=BeaconDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 5)
//            rxtotal2++;//全对   
//            }
//            
//             if(rxframetype==infodata)
//            {
//           for ( i=0;i<22;i++)
//              if (i!=DataDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 22)
//            rxtotal2++;//全对   
//            }
//
//
//           
//          }
// 
//      info[2] = (rxtotal1>>8)&0xff;
//      info[3] = rxtotal1&0xff;
//      info[4] = (rxtotal2>>8)&0xff;
//      info[5] = rxtotal2&0xff;
//
//     // UartSendByte(redata,3);
//       UartSendByte(info,6);
//      
//      for (i=0;i<128;i++)
//            redata[i]=0;
//      
//           vRadio_config_1st();
//     vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);
//     
//                        
//		  }
//                  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//                  
//              }
//           }      
//#endif
//
//
//      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//
//
//  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//  
//  return TRUE;
//  
//
//}
// 
//
//
//
///*!
// *  Check if Packet received IT flag is pending.
// *
// *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
// *
// *  @note
// *
// */
//uint8 gRadio_CheckReceived_VariablePacket(void)
//{
//  uint8 nirq_recv_var= SI4463_NIRQ_Bit();
//  
//  if (nirq_recv_var == FALSE)
//  {
//    /* Read ITs, clear pending ones */
//    si446x_get_int_status(0u, 0u, 0u);
//
//	if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_BIT)
//    {
//      /* State change to */
//      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP);
//	
//	  /* Reset FIFO */
//      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//      
//	  /* State change to */
//      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//    }
//
//    /* check the reason for the IT */
//    if (Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)
//    {
//      /* Blink once LED2 to show Sync Word detected */
//      //vHmi_ChangeLedState(eHmi_Led2_c, eHmi_LedBlinkOnce_c);
//    }
//
//    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
//    {
//      /* Blink once LED3 as CRC OK or not enabled */
//     // vHmi_ChangeLedState(eHmi_Led3_c, eHmi_LedBlinkOnce_c);
//
//      /* Read the length of RX_FIFO */
//      si446x_fifo_info(0u);
//
//      /* Packet RX */
//      si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
//
//      /* Do not use UART in case of direct firmware load */
//#ifdef UART_LOGGING_SUPPORT
//      {
//          /* Send it to UART */
//        UartSendByte(fixRadioPacket,RadioConfiguration.Radio_PacketLength);
//      }
//#endif
//
//      return TRUE;
//    }
//
//    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_CRC_ERROR_BIT)
//    {
//      /* Reset FIFO */
//      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
//    }
//  }
//
//  return FALSE;
//}
//
///*!
// *  Set Radio to RX mode, fixed packet length.
// *
// *  @param channel Freq. Channel
// *
// *  @note
// *
// */
//
//void vRadio_StartRX_1st(U8 channel)
//{
//  // Read ITs, clear pending ones
//  si446x_get_int_status(0u, 0u, 0u);
//
//  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
//  si446x_start_rx(channel, 0u, RadioConfiguration_1st.Radio_PacketLength,
//                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
//                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
//                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
//                  
//  /* Switch on LED1 to show RX state */
//  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
//}
//
//void vRadio_StartRX_2nd(U8 channel)
//{
//  // Read ITs, clear pending ones
//  si446x_get_int_status(0u, 0u, 0u);
//
//  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
//  si446x_start_rx(channel, 0u, RadioConfiguration_2nd.Radio_PacketLength,
//                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
//                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
//                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
//                  
//  /* Switch on LED1 to show RX state */
//  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
//}
//
//void vRadio_StartRX_ack(U8 channel)
//{
//  // Read ITs, clear pending ones
//  si446x_get_int_status(0u, 0u, 0u);
//
//  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
//  si446x_start_rx(channel, 0u, RadioConfiguration_ack.Radio_PacketLength,
//                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
//                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
//                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
//                  
//  /* Switch on LED1 to show RX state */
//  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
//}
//
//
///*!
// *  Set Radio to RX mode, fixed packet length.
// *
// *  @param channel Freq. Channel
// *
// *  @note
// *
// */
//void vRadio_StartRX(U8 channel)
//{
//  // Read ITs, clear pending ones
//  si446x_get_int_status(0u, 0u, 0u);
//
//  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
//  si446x_start_rx(channel, 0u, RadioConfiguration.Radio_PacketLength,
//                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
//                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX,
//                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
//                  
//  /* Switch on LED1 to show RX state */
//  //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedOn_c);
//}
//
//
///*!
// *  Check if Packet sent IT flag is pending.
// *
// *  @return   TRUE / FALSE
// *
// *  @note
// *
// */
//uint8 gRadio_CheckTransmitted(void)
//{
//    uint8 nirq_trans;
//    
//    
//    nirq_trans = SI4463_NIRQ_Bit();
//    
//    /*
//    while(nirq_trans!=0)
//    {
//       if(!SI4463_While_Handler())
//       break;
//       
//       nirq_trans = SI4463_NIRQ_Bit();
//    }
//    SI4463_AfterWhile();
//    */
//    //DelayMs(200);
//     
//    
// // nirq_trans = SI4463_NIRQ_Bit();
//    
//  if (nirq_trans == 0)
//  {
//    /* Read ITs, clear pending ones */
//    si446x_get_int_status(0u, 0u, 0u);
//
//    /* check the reason for the IT */
//    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
//    {
//      return TRUE;
//    }
//  }
//
//  return FALSE;
//}
//
//
///*!
// *  Set Radio to TX mode, fixed packet length.
// *
// *  @param channel Freq. Channel, Packet to be sent
// *
// *  @note
// *
// */
//uint8  vRadio_StartTx(U8 channel, U8 *pioFixRadioPacket,U8 len)
//{
//  // Read ITs, clear pending ones
//  si446x_get_int_status(0u, 0u, 0u);
//
//  
//  /* Check if the radio is already in TX state */
//  si446x_request_device_state();
//  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
//      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
//    /* Still transmitting */
//    return FALSE;
//  }
//
//  
//  /* Fill the TX fifo with datas */
//  si446x_write_tx_fifo(RadioConfiguration.Radio_PacketLength, pioFixRadioPacket);
//
//  /* Start sending packet, channel 0, START immediately, Packet n bytes long, go READY when done */
//  si446x_start_tx(channel, 0x30,  RadioConfiguration.Radio_PacketLength);
//  
//  return TRUE;
//}
//
//void RXHandlerInGDO0_SI4463(void)
//{
//   /* Read ITs, clear pending ones */
//    si446x_get_int_status(0u, 0u, 0u);
//
//    /* check the reason for the IT */
//    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
//    {
//      /* Packet RX */
//      si446x_read_rx_fifo(RadioConfiguration.Radio_PacketLength, (U8 *) &fixRadioPacket[0u]);
//
//      /* Send it to UART */
////      for (lCnt = 0u; lCnt < RadioConfiguration.Radio_PacketLength; lCnt++)
////      {
////        Comm_IF_SendUART(*((U8 *) &fixRadioPacket[0u] + lCnt));
////      }
////      Comm_IF_SendUART('\n');
//    }
//
//    /* Reset FIFO */
//    si446x_fifo_info(0x02);
//}
//
///*!
// * This function is used to compare the content of the received packet to a string.
// *
// * @return  None.
// */
//uint8 gSampleCode_StringCompare(U8* pbiPacketContent, U8* pbiString, U8 lenght)
//{
//  while ((*pbiPacketContent++ == *pbiString++) && (lenght > 0u))
//  {
//    if( (--lenght) == 0u )
//    {
//      return TRUE;
//    }
//  }
//
//  return FALSE;
//}
//
///*!
// * This function is used to show the actual state of the push-buttons on the LEDs.
// *
// * @return  TRUE - Started a packet TX / FALSE - Not packet to be sent.
// */
//uint8 vSampleCode_SendFixPacket(U8 * fixRadioPacket,U8 len)
//{
// // SEGMENT_VARIABLE(boPbPushTrack, U8);
//  //SEGMENT_VARIABLE(woPbPushTime, U16);
//
// // gHmi_PbIsPushed(&boPbPushTrack, &woPbPushTime);
//
////  fixRadioPacket[0]='B';
////  fixRadioPacket[1]='U';
////  fixRadioPacket[2]='T';
////  fixRadioPacket[3]='T';
////  fixRadioPacket[4]='O';
////  fixRadioPacket[5]='N';
////
////  fixRadioPacket[6]='1';
////  if(boPbPushTrack & eHmi_Pb1_c)
////  {
////    fixRadioPacket[6]='1';
////  }
////  else if(boPbPushTrack & eHmi_Pb2_c)
////  {
////    fixRadioPacket[6]='2';
////  }
////  else if(boPbPushTrack & eHmi_Pb3_c)
////  {
////    fixRadioPacket[6]='3';
////  }
////  else if(boPbPushTrack & eHmi_Pb4_c)
////  {
////    fixRadioPacket[6]='4';
////  }
////  else
////  {
////    return FALSE;
////  }
//
//  uint8 TxFlag;
//  TxFlag = vRadio_StartTx(pRadioConfiguration->Radio_ChannelNumber, (U8 *) fixRadioPacket,len);
//
//
//  /* Packet sending initialized */
//  return TxFlag;
//}
//extern uint32 tmpGlobalTime;
//uint32 StatTime, StopTime;
//
//extern uint8 trx_state;//0:tx,1:rx
//
//extern  uint8 PKT_Sent_Flag;
//
//uint8 bPositionInPayload;
//uint8 pPositionInPayload[128];
//
//uint8 length=0;
//
//
//
//
//
////1016
//
//#if 1 //添加barker的发送
////2015-5-18 13:48:38，添加barker的发送
//extern uint32 configtimesup; 
//extern uint32 TIMcont; 
//extern uint8 barkerSyncEnable;
//uint8 SI4463_Config_Transmit_dualrate(U8 * pSrc1,U8 len1,U8 * pSrc2,U8 len2)
//{
// unsigned char transmitArray[79]={0};
//   uint8 barkerCode[7]={0};//最大是26个byte的barker，配置文件中两处要改动
//  
//
//  uint16 i,j;
//
//  uint16 interleaveIndex;//新算法添加
//
//
//  	// for lfsr
//  //unsigned int start_state = 1;  /* Any nonzero start state will work. */
//   // unsigned int lfsr = start_state;
//
//    barkerCode[0] = 0xF9;
//    barkerCode[1] = 0xAF;
//    barkerCode[2] = 0xCD;
//    barkerCode[3] = 0x7E;
//    barkerCode[4] = 0x6B;
//    barkerCode[5] = 0xF3;
//    barkerCode[6] = 0x5F;
//	/*
//    barkerCode[7] = 0x9A;
//    barkerCode[8] = 0xFC;
//    barkerCode[9] = 0xD7;
//    barkerCode[10] = 0xE6;
//    barkerCode[11] = 0xBF;
//    barkerCode[12] = 0x35; 
//    barkerCode[13] = 0xF9;
//    barkerCode[14] = 0xAF;
//    barkerCode[15] = 0xCD;
//    barkerCode[16] = 0x7E;
//    barkerCode[17] = 0x6B;
//    barkerCode[18] = 0xF3;
//    barkerCode[19] = 0x5F;
//    barkerCode[20] = 0x9A;
//    barkerCode[21] = 0xFC;
//    barkerCode[22] = 0xD7;
//    barkerCode[23] = 0xE6;
//    barkerCode[24] = 0xBF;
//    barkerCode[25] = 0x35;
//  */
//  
//	//新算法,交织和barker，interleaver、second sync word generator
//	   if ((interleaveEnable==1)&&(barkerSyncEnable==1))//同时满足
//        {
//
//
//			for(i = 0; i < barkerLengthInByte; i++)
//	       {
//
//			 transmitArray[i] = barkerCode[i];//barker部分,不用barkerAddedArray。
//
//			}
//
//	      for(i = 0; i < 72; i++)
//	       {
//		    transmitArray[i+barkerLengthInByte] = 0;//必须要清0，累加的影响
//		    for(j = 0; j < 8; j++)
//		     {
//				 
//		//lsfr		 
//
//
//		interleaveIndex = ((i*8+j)%24)*24 + ((i*8+j)/24);//块交织
//
//
//			 transmitArray[i+barkerLengthInByte] = transmitArray[i+barkerLengthInByte] + (((pSrc2[interleaveIndex/8] >> (7-(interleaveIndex%8)))%2)<<(7-j));//累加，去掉了中间数组，直接用了input:pSrc2[]
//
//              }
//	        }
//         }
//  //
//  vRadio_config_1st();
//     RF_Switch_TX();
//  //可以由外面决定，但是为了调试方便。直接由配置中读取。
//  len2 = RadioConfiguration_2nd.Radio_PacketLength;
//  
//  //发送第一个包
//   trx_state = 0;//f发送模式
//  
//  /* Check if the radio is already in TX state */
//  si446x_request_device_state();
//  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
//      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
//    /* Still transmitting */
//       PKT_Sent_Flag =0;
//  vRadio_config_1st();
//  
//  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
//  trx_state = 1;//第二个包发完之后默认为接收状态
//   
//  
//       RF_Switch_RX();
//    return FALSE;
//  }
//
//  /* Reset TX FIFO */
//  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
//
//  /* Fill the TX FIFO with data */
//  si446x_write_tx_fifo(len1, pSrc1);
//
//
//  /* Start sending packet, channel 0, START immediately,
//   * Packet length according to 'len', go READY when done */
//  
//  //TIM_Cmd(TIM4, ENABLE);
//  
////  if((NowMacState==B_TX)&&(1/*MacPib.SlotID==0*/))
////  {
////    if(((kernel_GetSystemClock()%SLOTTIME)<30))
////    {
////      StartTxInSysTickFlag = 1;
////    }
////    else
////    {
////      //counter=0;
////      PKT_Sent_Flag =0;
////      vRadio_config_1st();
////    
////      vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
////      trx_state = 1;//第二个包发完之后默认为接收状态
////     
////    
////      RF_Switch_RX();
////      return FALSE;      
////      //si446x_start_tx(0u, 0x30,  len1);
////    }
////  }
////  else
////  {
////    si446x_start_tx(0u, 0x30,  len1);
////  }
//  
// 
//  
// 
//  uint32 counter = 0;
//  while (PKT_Sent_Flag!=1)//发送等待
//  {
//  //等待保护
//  asm("nop");
//  counter++;
//  if(counter >= 0xFFFFFFFF)
//    {
//     //第一个包未发送成功
//    counter=0;
//    PKT_Sent_Flag =0;
//  vRadio_config_1st();
//  
//  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
//  trx_state = 1;//第二个包发完之后默认为接收状态
//   
//  
//       RF_Switch_RX();
//    return FALSE;
//    }  
//  }
//  
//   //开启TIM4
//  configtimesup=0;
//  TIM_Cmd(TIM4, ENABLE);
//  
//   PKT_Sent_Flag =0;
// vRadio_config_2nd();
// 
// 
// 
//  trx_state = 0;//f发送模式
//  
//  /* Check if the radio is already in TX state */
//  si446x_request_device_state();
//  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
//      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
//    /* Still transmitting */
//         PKT_Sent_Flag =0;
//  vRadio_config_1st();
//  
//  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
//  trx_state = 1;//第二个包发完之后默认为接收状态
//     
//       RF_Switch_RX();
//   
//  //TIM4_Config_TXTXdelay();///
//  TIM_Cmd(TIM4, DISABLE);//g关闭使能
//  TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
//       
//    return FALSE;
//  }
//  
//  for (uint8 i=0;i<(85+13);i++)
//  {
//  pPositionInPayload[i] = transmitArray[i];
// 
//  }
//   bPositionInPayload = 0;
//   
//
//  /* Reset TX FIFO */
//  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
//  
//  /* Fill the TX FIFO with data */
//    /* Fill the TX FIFO with data */
//  if (len2>50)
//  {
//    length =50;
//    si446x_write_tx_fifo(length, &transmitArray[0]);
//    bPositionInPayload = length;
//    // si446x_start_tx(0u, 0x30,  length);
//  
//    /////////////////////////////////////////////////////
//    counter = 0;
//    if (configtimesup == 1)
//    {
//      configtimesup = configtimesup+1;
//      configtimesup = configtimesup-1;
//    }
// 
//    while (configtimesup<1)//2980~2983
//    {
//    //等待保护
//      asm("nop");
//      counter++;
//      if(counter >= 0xFFFFFFFF)
//      {
//     //第一个包未发送成功
//        counter=0;
//        PKT_Sent_Flag =0;
//        vRadio_config_1st();
//  
//        vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
//        trx_state = 1;//第二个包发完之后默认为接收状态
//   
//  
//        RF_Switch_RX();
//       
//       
//        //TIM4_Config_TXTXdelay();///
//        TIM_Cmd(TIM4, DISABLE);//g关闭使能
//        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
//          return FALSE;
//      }
//    } 
//
//  
//       //TIM4_Config_TXTXdelay();///
//    TIM_Cmd(TIM4, DISABLE);//g关闭使能
//    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
// ////////////////////////////////////////////////////
// 
// 
//    si446x_start_tx(0u, 0x30,  0x00);
//  }
//  else
//  {
//  si446x_write_tx_fifo(len2, transmitArray);
// //   si446x_start_tx(0u, 0x30,  len2);
//   si446x_start_tx(0u, 0x30,  0x00);
//  
//  }
//
//  /* Start sending packet, channel 0, START immediately,
//   * Packet length according to 'len', go READY when done */
//
//  
//  
//  while (PKT_Sent_Flag!=1)//发送等待
//  {
//  //等待保护
//  asm("nop");
//  counter++;
//  if(counter >= 0xFFFFFFFF)
//  {
//     //第一个包未发送成功
//    counter=0;
//     PKT_Sent_Flag =0;
//  vRadio_config_1st();
//  
//  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
//  trx_state = 1;//第二个包发完之后默认为接收状态
//   
//  
//       RF_Switch_RX();
//       
//       //TIM4_Config_TXTXdelay();///
//  //TIM_Cmd(TIM4, DISABLE);//g关闭使能
//  //TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
// ////////////////////////////////////////////////////
//    return FALSE;
//  }
//    
//   
//  
//  }
//  
//  Toggle_LED2(); 
//// TIM_Cmd(TIM3, DISABLE);//使能
// //   TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /* Clear TIM3 update interrupt */
// //  TIMcont=0;
//  
//  
//  PKT_Sent_Flag =0;
//  vRadio_config_1st();
//  
//  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
//  trx_state = 1;//第二个包发完之后默认为接收状态
//     
//       RF_Switch_RX();
//  
//  //TIM4_Config_TXTXdelay();///
// // TIM_Cmd(TIM4, DISABLE);//g关闭使能
//  //TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM3 update interrupt */
// ////////////////////////////////////////////////////
//              
//  return TRUE;
//}
//
//#endif
//
//
//static uint8 InTx = 0;
//void SetInTx(uint8 x)
//{
//  InTx = x;
//}
//
//uint8 GetInTx(void)
//{
//  return InTx;
//}
//
//
//uint8 New_SI4463_Transmit(U8 * pSrc,U8 len)
//{
//  SetInTx(1);
//  DelayMs(1);
//  
//  RF_Switch_TX();
// 
//  
//  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
//  
//  SI4463_Enable_NIRQ_TX();
//  
//  
//  
//  /* Check if the radio is already in TX state */
//  si446x_request_device_state();
//  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
//      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
//    /* Still transmitting */
//        si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//        
//        SetInTx(0);
//        RF_Switch_RX();
//    return FALSE;
//  }
//
//  /* Reset TX FIFO */
//  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
//
//  /* Fill the TX FIFO with data */
//  si446x_write_tx_fifo(len, pSrc);
//
//
//  /* Start sending packet, channel 0, START immediately,
//   * Packet length according to 'len', go READY when done */
//  si446x_start_tx(0u, 0x30,  len);
//  
// uint32 counter = 0;
// StatTime = tmpGlobalTime;
// while(TRUE != gRadio_CheckTransmitted())
// {
//   
//  asm("nop");
//  counter++;
//  if(counter >= 0xFFFFFFFF)
//  {
//    SI4463_Enable_NIRQ_RX();
//    EXTILineNIRQ_Config();
//   
//    
//    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//    
//    SetInTx(0);
//    RF_Switch_RX();
//    return FALSE;
//  }   
// }  
//  StopTime = tmpGlobalTime-StatTime;
//  SI4463_Enable_NIRQ_RX();
//  EXTILineNIRQ_Config();
//  
//  
//  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
//  
//  SetInTx(0);
//  RF_Switch_RX();
//  
//  Toggle_LED1();
//  
//  return TRUE;
//}
//
///*!
// * This function is used to transmit data
// *
// *
// */
//void SI4463_Transmit(U8 * Packet,U8 length)
//{
//  static SEGMENT_VARIABLE(lPktSending, U8) = 0u;
//  uint8 temp;
//  
// 
//  // Check if the radio packet sent successfully
//  if (TRUE == gRadio_CheckTransmitted())
//  {
//    /* Clear Packet Sending flag */
//    temp = 2;
//    UartSendByte(&temp,1);
//    lPktSending = 0u;
// }
// else
// {
//    temp = 3;
//    UartSendByte(&temp,1);
// 
// }
//
//  //lPktSending = 0;
//
//
//  if ((0u == lPktSending))
//  {
//    uint8 flag=vSampleCode_SendFixPacket((U8 *)Packet,length);
//    
//    UartSendByte(&flag,1);
//    
//    if (TRUE == flag)
//    {
//      //lPer_SecCntr = 0u;
//
//      /* Set Packet Sending flag */
//      lPktSending = 1u;
//      
//    }
//  }
//}
//
//
//#ifndef SOFT_MERGE
//void Raw_Rx_Packet_Hanlder(void) //Added Pei
//{
//    unsigned char i;
//  uint8 size,crc,InitLength;
//  uint16 rssi;
//  unsigned char BYTEOF1BITcount=0;
//  unsigned char temp;
//  unsigned char j,K=4;
//  crc=1;
//  
//  
//  uint8 BeaconInit[BeaconInitLength] = {0};
//	uint8 DataInit[DataInitLength] = {0};
//	uint8 ACKInit[ACKInitLength] = {0};
//
//	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
//	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
//	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};
//
//
//    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};
//
//	uint8 BeaconEncodeOutput[BeaconLength] = {0};
//	uint8 DataEncodeOutput[DataLength] = {0};
//	uint8 ACKEncodeOutput[ACKLength] = {0};
//
//	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 DataInterleaveOutput[DataLength] = {0};
//	uint8 ACKInterleaveOutput[ACKLength] = {0};
/////////////////////////////////////////
//   
//        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 rxDataInterleaveOutput[DataLength] = {0};
//	uint8 rxACKInterleaveOutput[ACKLength] = {0};
//	
//     
//  
//	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
//	uint8 DataDeinterleaveOutput[DataLength] = {0};
//	uint8 ACKDeinterleaveOutput[ACKLength] = {0};
//
//
//	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
//      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
//	uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
//	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
//        
//   switch(rxframetype)
//  {
//  case beacon:
//   rxPayloadLength = BeaconLength;
//    InitLength = BeaconInitLength;
//    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
//    break;
//  case infodata:
//   rxPayloadLength = DataLength;
//    InitLength = DataInitLength;
//    SourceGenerate(DataInit,DataInitLength,sourceSeed);
////	myCRC(DataInit,DataCRCOutput,infodata);
//   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
//    break;  
//  case ACK:
//   rxPayloadLength = ACKLength;
//    InitLength = ACKInitLength;
//    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
//    break;
//  }
//  
//    SEGMENT_VARIABLE(redata1[128], U8);
//    SEGMENT_VARIABLE(deinterleavebuffer[72], U8);
//    
//    if (rxframetype==beacon)
//             {
//          shift(redata,deinterleavebuffer,72,0);//input,output:  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//#if 1
//          if ((BeaconDecodeOutput[4]!=0x99)&&(BeaconDecodeOutput[4]!=0x04))
//          {
//            crc = crc+1;
//            crc = crc -1;
//          }
//
//
//          if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,1);//input,output:左移或者右移1bit或者2bit  
//           deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);     
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏小
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,2);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//       //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏大
//          }
//#if 1
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,3);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,4);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,5);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,6);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//              if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,7);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,8);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         //ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//                          ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x00)
//      {
//         if (BeaconDecodeOutput[1]==0x00)
//         {
//            if (BeaconDecodeOutput[2]==0x00)
//            {
//              if (BeaconDecodeOutput[3]==0x00)
//              {
//                if (BeaconDecodeOutput[4]==0x00)
//                {
//                  error00++;
//                  crc = 1;//crc错误
//                }
//              }
//            }
//         }
//      }
//          
//#endif
//#endif  
//             }
//          
//             if (rxframetype==infodata)
//             {
//              shift(redata,redata1,72,0);//input,output:左移或者右移1bit或者2bit       
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//		   crc = CRCCheck(DataDecodeOutput,infodata);
//    
//      #if 1
//          if (crc!=0)
//          {
//          shift(redata,redata1,72,1);//input,output:左移或者右移1bit或者2bit  
//          tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);   
//   if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,redata1,72,2);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//      if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//       #if 1 
//           if (crc!=0)
//          {
//          shift(redata,redata1,72,3);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//   if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,redata1,72,4);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//     if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//          
//
//             if (crc!=0)
//          {
//          shift(redata,redata1,72,5);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//     if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//   if (crc!=0)
//          {
//
//          shift(redata,redata1,72,6);//input,output:左移或者右移1bit或者2bit  
//           tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//          crc = CRCCheck(DataDecodeOutput,infodata);
//     if (crc==0)
//   {
//   crc=crc;
//   }
//          }
//#endif
//          
//#endif               
//                   
//             }         
//             
//          
//          
//          if (crc==0)//crc=0时为解调正确
//          {
//            crc=1;
//            rxtotal3++;
//          
//       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   
//
//            if(rxframetype==beacon)
//            {
//           for ( i=0;i<5;i++)
//              if (i!=BeaconDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 5)
//            rxtotal2++;//全对   
//            }
//            
//             if(rxframetype==infodata)
//            {
//           for ( i=0;i<22;i++)
//              if (i!=DataDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 22)
//            rxtotal2++;//全对   
//            }
//
//
//           
//          }
//      
//
//      ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x01)
//      {
//         if (BeaconDecodeOutput[1]==0x6F)
//         {
//            if (BeaconDecodeOutput[2]==0xFF)
//            {
//              if (BeaconDecodeOutput[3]==0x10)
//              {
//                if (BeaconDecodeOutput[4]==0x99)
//                {
//                  rxtotal2++;
//                }
//              }
//            }
//         }
//      }
//      
//      ///
// info[2] = (rxtotal1>>8)&0xff;
//      info[3] = rxtotal1&0xff;
//      info[4] = (rxtotal2>>8)&0xff;
//      info[5] = rxtotal2&0xff;
//
//      
//       if (rxtotal1!=rxtotal2)
//      {
//      rxtotal2 = rxtotal2 ; 
//      }
//      
//      
//      if (rxtotal1==1000)
//      {
//      rxtotal2 = rxtotal2 ; 
//      }
//      
//      if (BeaconDecodeOutput[1] == 0x00)
//      {
//      BeaconDecodeOutput[1] = 0x00;
//      }
//      
//      
//   
//      //if(crc == 0)
//      {
//        crc=1;
//     
//        UDP_Receive(redata1);
//        
//      
//        
//        
//
//        
//      
//
//        
//        
//        
//      }
//     
//      
//      for (i=0;i<128;i++)
//      {
//            redata[i]=0;
//            redata1[i]=0;
//            deinterleavebuffer[i] = 0;
//      }
//}
//
//#endif
//
//#ifdef SOFT_MERGE_old
//uint8 redatabit[576]={0};
//uint8 redatabit_shift[576]={0};
//static unsigned char bittemp[72*8]={0};
//
//uint8 datastore[576]={0};
//uint8 datamerge[576]={0};  
//
//void byte2bit(unsigned char input[],unsigned char output[])
//{
//        uint16 i,j;
//        //DEC2BIN
//	for(i = 0; i < 72; i++)
//	{
//		for(j = 0; j < 8; j++)
//		{
//			output[7 - j + i*8] = (input[i] >> j)%2;
//		}
//	}
//
//}
//
//void shift_bit(unsigned char input[],unsigned char output[], const unsigned int length, const unsigned char shiftbitlength)
//{
//        uint16 i,j;
//       for(i = 0; i < length; i++)
//       {
//          output[i]=0;
//        }
//		
//		
//		 //shift,移位 .不偏移
//        if (shiftbitlength==0)//左移0位
//        {
//        for(i = 0; i < length; i++)
//	   {
//          output[i]=input[i];
//        }
//        }
//		
//
//       //shift,移位 .左移1位
//        if (shiftbitlength==1)//左移1位
//        {
//        for(i = 0; i < length-1; i++)
//	{
//          output[i]=input[i+1];
//        }
//        }
//       
//        
//        //shift,移位。右移1位
//         if (shiftbitlength==2)//右移1位
//        {
//        for(i = length-1; i >0; i--)
//	{
//          output[i]=input[i-1];
//        }
//        }
//        
//        
//         //shift,移位 .左移2位
//        if (shiftbitlength==3)//左移2位
//        {
//        for(i = 0; i < length-2; i++)
//	{
//          output[i]=input[i+2];
//        }
//        }
//        
//        
//         //shift,移位。右移2位
//         if (shiftbitlength==4)//右移2位
//        {
//        for(i = length-1; i >1; i--)
//	{
//          output[i]=input[i-2];
//        }
//        }
//        
//             //shift,移位 .左移3位
//        if (shiftbitlength==5)//左移3位
//        {
//        for(i = 0; i < length-3; i++)
//	    {
//          output[i]=input[i+3];
//        }
//        }
//        
//             //shift,移位。右移3位
//         if (shiftbitlength==6)//右移3位
//        {
//        for(i = length-1; i >2; i--)
//	    {
//          output[i]=input[i-3];
//        }
//        }
//                
//
//}
//
//
//void merge(unsigned char data_bit[], unsigned char datastore[], unsigned char datamerge[])
//{
//  uint16 i,j;
//
//	
//	for(i = 0; i < 72*8; i++)
//	{
//	datamerge[i] = data_bit[i] + datastore[i];
//	}
//}
//
//
//
//
//
//uint8 reTx_count=0;
//
//void Set_reTx_count(uint8 x)
//{
//  reTx_count=x;
//}
//
//void ReSet_reTx_count()
//{
//  reTx_count=0;
//}
//
//
//void reTx_count_plusplus()
//{
//  reTx_count++;
//}
//
//void Raw_Rx_Packet_Hanlder(void) //Added Pei
//{
//  unsigned char i;
//  uint8 size,crc,InitLength;
//  uint16 rssi;
//  unsigned char BYTEOF1BITcount=0;
//  unsigned char temp;
//  unsigned char j,K=4;
//  crc=1;
//  
//  uint8 shift_count=0;
//  uint16 min_d=0xFFFF;
//  uint16 min_d_tmp=0;
//  uint8 datastore_tmp[576]={0};
//  uint8 dataCRC_tmp[24]={0};
//  uint8 redata_tmp[72]={0};
//  
//  
//  uint8 BeaconInit[BeaconInitLength] = {0};
//	uint8 DataInit[DataInitLength] = {0};
//	uint8 ACKInit[ACKInitLength] = {0};
//
//	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
//	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
//	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};
//
//
//    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};
//
//	uint8 BeaconEncodeOutput[BeaconLength] = {0};
//	uint8 DataEncodeOutput[DataLength] = {0};
//	uint8 ACKEncodeOutput[ACKLength] = {0};
//
//	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 DataInterleaveOutput[DataLength] = {0};
//	uint8 ACKInterleaveOutput[ACKLength] = {0};
/////////////////////////////////////////
//   
//        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 rxDataInterleaveOutput[DataLength] = {0};
//	uint8 rxACKInterleaveOutput[ACKLength] = {0};
//	
//     
//  
//	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
//	uint8 DataDeinterleaveOutput[DataLength] = {0};
//	uint8 ACKDeinterleaveOutput[ACKLength] = {0};
//
//
//	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
//      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
//	uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
//	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
//        
//   switch(rxframetype)
//  {
//  case beacon:
//   rxPayloadLength = BeaconLength;
//    InitLength = BeaconInitLength;
//    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
//    break;
//  case infodata:
//   rxPayloadLength = DataLength;
//    InitLength = DataInitLength;
//    SourceGenerate(DataInit,DataInitLength,sourceSeed);
////	myCRC(DataInit,DataCRCOutput,infodata);
//   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
//    break;  
//  case ACK:
//   rxPayloadLength = ACKLength;
//    InitLength = ACKInitLength;
//    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
//    break;
//  }
//  
//    SEGMENT_VARIABLE(redata1[128], U8);
//    SEGMENT_VARIABLE(deinterleavebuffer[72], U8);
//    
//    if (rxframetype==beacon)
//    {
//          shift(redata,deinterleavebuffer,72,0);//input,output:  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//#if 1
//          if ((BeaconDecodeOutput[4]!=0x99)&&(BeaconDecodeOutput[4]!=0x04))
//          {
//            crc = crc+1;
//            crc = crc -1;
//          }
//
//
//          if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,1);//input,output:左移或者右移1bit或者2bit  
//           deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);     
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏小
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,2);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//       //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏大
//          }
//#if 1
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,3);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,4);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,5);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,6);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//              if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,7);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,8);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         //ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//                          ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x00)
//      {
//         if (BeaconDecodeOutput[1]==0x00)
//         {
//            if (BeaconDecodeOutput[2]==0x00)
//            {
//              if (BeaconDecodeOutput[3]==0x00)
//              {
//                if (BeaconDecodeOutput[4]==0x00)
//                {
//                  error00++;
//                  crc = 1;//crc错误
//                }
//              }
//            }
//         }
//      }
//          
//#endif
//#endif  
//             }
//          
//    if (rxframetype==infodata)
//    {
//
//     if(reTx_count==0)
//     {
//       uint16 ii;
//       for(ii=0;ii<576;ii++)
//       {
//         datastore[ii]=0;
//       }
//     }
//     
//     tbcc_dec(redata,DataCRCOutputLength, DataDecodeOutput,1);
//     byte2bit(redata,redatabit);
//     
//     while(shift_count<=4)
//     {
//       if(reTx_count==1)
//       {
//         asm("nop");
//       }
//       shift_bit(redatabit,redatabit_shift,576,shift_count);
//       merge(redatabit_shift,datastore,datamerge);
//       min_d_tmp=tbcc_dec_bit2byte_infodata(datamerge,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//       if(min_d_tmp<min_d)
//       {
//         min_d=min_d_tmp;
//         kernel_memcpy(datastore_tmp,redatabit_shift,576);
//       } 
//       crc=CRCCheck(DataDecodeOutput,infodata);   
//       if(crc==0)
//       {
//         reTx_count=0;
//         break;
//       }
//       else
//       {
//         shift_count++;
//       }
//     }
//     
//     if((shift_count>4) && (crc!=0))
//     {
//       kernel_memcpy(datastore,datastore_tmp,576);
//       
//       
//       myCRC(DataDecodeOutput,dataCRC_tmp,infodata);
//       tbcc_enc(dataCRC_tmp,DataCRCOutputLength,redata1);
//     }
//     
//     if(crc==0)
//     {
//       myCRC(DataDecodeOutput,dataCRC_tmp,infodata);
//       tbcc_enc(dataCRC_tmp,DataCRCOutputLength,redata1);
//     }
//     
//     
//     
//               
//#if 0       //Old Hard Merging Start        
//              shift(redata,redata1,72,0);//input,output:左移或者右移1bit或者2bit       
//              tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//              crc = CRCCheck(DataDecodeOutput,infodata);
//    
//      #if 1
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,1);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);  
//                
//                 if (crc==0)
//                 {
//                  crc=crc;
//                 }
//              }
//          
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,2);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);
//                if (crc==0)
//               {
//                crc=crc;
//               }
//              }
//          
//       #if 1 
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,3);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);
//               if (crc==0)
//               {
//                crc=crc;
//               }
//              }
//          
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,4);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);
//               if (crc==0)
//               {
//                crc=crc;
//               }
//              }
//          
//
//            if (crc!=0)
//            {
//              shift(redata,redata1,72,5);//input,output:左移或者右移1bit或者2bit  
//              tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//              crc = CRCCheck(DataDecodeOutput,infodata);
//             if (crc==0)
//             {
//              crc=crc;
//             }
//            }
//            
//            if (crc!=0)
//            {
//
//              shift(redata,redata1,72,6);//input,output:左移或者右移1bit或者2bit  
//              tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//              crc = CRCCheck(DataDecodeOutput,infodata);
//              if (crc==0)
//             {
//              crc=crc;
//             }
//            }
//#endif
//          
//#endif               
//
//#endif          //Old Hard Merging End
//   }         
//             
//          
//          
//          if (crc==0)//crc=0时为解调正确
//          {
//            crc=1;
//            rxtotal3++;
//          
//       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   
//
//            if(rxframetype==beacon)
//            {
//           for ( i=0;i<5;i++)
//              if (i!=BeaconDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 5)
//            rxtotal2++;//全对   
//            }
//            
//             if(rxframetype==infodata)
//            {
//           for ( i=0;i<22;i++)
//              if (i!=DataDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 22)
//            rxtotal2++;//全对   
//            }
//
//
//           
//          }
//      
//
//      ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x01)
//      {
//         if (BeaconDecodeOutput[1]==0x6F)
//         {
//            if (BeaconDecodeOutput[2]==0xFF)
//            {
//              if (BeaconDecodeOutput[3]==0x10)
//              {
//                if (BeaconDecodeOutput[4]==0x99)
//                {
//                  rxtotal2++;
//                }
//              }
//            }
//         }
//      }
//      
//      ///
//      info[2] = (rxtotal1>>8)&0xff;
//      info[3] = rxtotal1&0xff;
//      info[4] = (rxtotal2>>8)&0xff;
//      info[5] = rxtotal2&0xff;
//
//      
//       if (rxtotal1!=rxtotal2)
//      {
//        rxtotal2 = rxtotal2 ; 
//      }
//      
//      
//      if (rxtotal1==1000)
//      {
//        rxtotal2 = rxtotal2 ; 
//      }
//      
//      if (BeaconDecodeOutput[1] == 0x00)
//      {
//        BeaconDecodeOutput[1] = 0x00;
//      }
//      
//      
//   
//      //if(crc == 0)
//      {
//        crc=1;
//     
//
//        UDP_Receive(redata1);
//
//        
//
//        
//      
//        
//        
//
//        
//      
//
//        
//        
//        
//      }
//     
//      
//      for (i=0;i<128;i++)
//      {
//            redata[i]=0;
//            redata1[i]=0;
//            deinterleavebuffer[i] = 0;
//      }
//}
//
//
//#endif
//
//
//#ifdef SOFT_MERGE
//#if 1
//uint8 first_redatabyte1[72]={0};
//uint8 second_redatabyte1[72]={0};
//uint8 third_redatabyte1[72]={0};
//uint8 fourth_redatabyte1[72]={0};
//#endif
//
//#if 0
//void byte2bit(unsigned char input[],unsigned char output[])
//{
//        uint16 i,j;
//        //DEC2BIN
//	for(i = 0; i < 72; i++)
//	{
//		for(j = 0; j < 8; j++)
//		{
//			output[7 - j + i*8] = (input[i] >> j)%2;
//		}
//	}
//
//}
//#endif
//
//#if 0
//void shift_bit(unsigned char input[],unsigned char output[], const unsigned int length, const unsigned char shiftbitlength)
//{
//        uint16 i,j;
//       for(i = 0; i < length; i++)
//       {
//          output[i]=0;
//        }
//		
//		
//		 //shift,移位 .不偏移
//        if (shiftbitlength==0)//左移0位
//        {
//        for(i = 0; i < length; i++)
//	   {
//          output[i]=input[i];
//        }
//        }
//		
//
//       //shift,移位 .左移1位
//        if (shiftbitlength==1)//左移1位
//        {
//        for(i = 0; i < length-1; i++)
//	{
//          output[i]=input[i+1];
//        }
//        }
//       
//        
//        //shift,移位。右移1位
//         if (shiftbitlength==2)//右移1位
//        {
//        for(i = length-1; i >0; i--)
//	{
//          output[i]=input[i-1];
//        }
//        }
//        
//        
//         //shift,移位 .左移2位
//        if (shiftbitlength==3)//左移2位
//        {
//        for(i = 0; i < length-2; i++)
//	{
//          output[i]=input[i+2];
//        }
//        }
//        
//        
//         //shift,移位。右移2位
//         if (shiftbitlength==4)//右移2位
//        {
//        for(i = length-1; i >1; i--)
//	{
//          output[i]=input[i-2];
//        }
//        }
//        
//             //shift,移位 .左移3位
//        if (shiftbitlength==5)//左移3位
//        {
//        for(i = 0; i < length-3; i++)
//	    {
//          output[i]=input[i+3];
//        }
//        }
//        
//             //shift,移位。右移3位
//         if (shiftbitlength==6)//右移3位
//        {
//        for(i = length-1; i >2; i--)
//	    {
//          output[i]=input[i-3];
//        }
//        }
// 
//       
//        if (interleaveEnable==1)
//       {
//       uint8 output1[576] = {0};
//       for(i=0;i<length;i++)
//       {
//        output1[interleaveArray[i]] = output[i];//deinterleave
//        }
//       for(i=0;i<length;i++)
//       {
//        output[i] = output1[i];
//        }
//       }
//
//}
//
//#endif
//
//#if 0
//void merge(unsigned char data_bit[], unsigned char datastore[], unsigned char datamerge[])
//{
//  uint16 i,j;
//
//	
//	for(i = 0; i < 72*8; i++)
//	{
//	datamerge[i] = data_bit[i] + datastore[i];
//	}
//}
//#endif
//
//
//
//
//uint8 reTx_count=0;
//
//void Set_reTx_count(uint8 x)
//{
//  reTx_count=x;
//}
//extern uint8 wrong_min_d;//2015-02-06
//void ReSet_reTx_count()
//{
//  wrong_min_d = 0;
//  reTx_count=0;
//}
//
//
//void reTx_count_plusplus()
//{
//  reTx_count++;
//}
//
//uint8 DataDecodeOutput[DataCRCOutputLength] = {0};//2015.02.09 
//
//
//#if 0
//void Raw_Rx_Packet_Hanlder(void) //Added Pei
//{
//  unsigned char i;
//  uint8 size,crc,InitLength;
//  uint16 rssi;
//  unsigned char BYTEOF1BITcount=0;
//  unsigned char temp;
//  unsigned char j,K=4;
//  crc=1;
//  
//  uint8 shift_count=0;
//  uint8  shift_count_merge = 0;
//  uint16 min_d=0xFFFF;
//  uint16 last_min_d=0xFFFF;
//  uint16 min_d_tmp=0;
//  uint8 datastore_tmp[576]={0};
//  uint8 dataCRC_tmp[24]={0};
//  uint8 redata_tmp[72]={0};
//  
//  
//  uint8 BeaconInit[BeaconInitLength] = {0};
//	uint8 DataInit[DataInitLength] = {0};
//	uint8 ACKInit[ACKInitLength] = {0};
//
//	uint8 BeaconCRCOutput[BeaconCRCOutputLength] = {0};
//	uint8 DataCRCOutput[DataCRCOutputLength] = {0};
//	uint8 ACKCRCOutput[ACKCRCOutputLength] = {0};
//
//
//    uint8 BeaconTBCCOutput[BeaconTBCCOutputLength] = {0};
//
//	uint8 BeaconEncodeOutput[BeaconLength] = {0};
//	uint8 DataEncodeOutput[DataLength] = {0};
//	uint8 ACKEncodeOutput[ACKLength] = {0};
//
//	uint8 BeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 DataInterleaveOutput[DataLength] = {0};
//	uint8 ACKInterleaveOutput[ACKLength] = {0};
/////////////////////////////////////////
//   
//        uint8 rxBeaconInterleaveOutput[BeaconLength] = {0};
//	uint8 rxDataInterleaveOutput[DataLength] = {0};
//	uint8 rxACKInterleaveOutput[ACKLength] = {0};
//	
//     
//  
//	uint8 BeaconDeinterleaveOutput[BeaconLength] = {0};
//	uint8 DataDeinterleaveOutput[DataLength] = {0};
//	uint8 ACKDeinterleaveOutput[ACKLength] = {0};
//
//
//	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
//      //  uint8 BeaconDecodeOutput_k[16*BeaconCRCOutputLength] = {0};//最多16个不同的
//	//uint8 DataDecodeOutput[DataCRCOutputLength] = {0};
//        uint8 DataDecodeOutputNoMerge[DataCRCOutputLength] = {0};
//	uint8 ACKDecodeOutput[ACKCRCOutputLength] = {0};
//        
//   switch(rxframetype)
//  {
//  case beacon:
//   rxPayloadLength = BeaconLength;
//    InitLength = BeaconInitLength;
//    SourceGenerate(BeaconInit,BeaconInitLength,sourceSeed);
//    break;
//  case infodata:
//   rxPayloadLength = DataLength;
//    InitLength = DataInitLength;
//    SourceGenerate(DataInit,DataInitLength,sourceSeed);
////	myCRC(DataInit,DataCRCOutput,infodata);
//   //   tbcc_enc(DataCRCOutput,DataCRCOutputLength,DataEncodeOutput);
//    break;  
//  case ACK:
//   rxPayloadLength = ACKLength;
//    InitLength = ACKInitLength;
//    SourceGenerate(ACKInit,ACKInitLength,sourceSeed);
//    break;
//  }
//  
//    SEGMENT_VARIABLE(redata1[128], U8);
//    SEGMENT_VARIABLE(deinterleavebuffer[72], U8);
//    
//    if (rxframetype==beacon)
//    {
//          shift(redata,deinterleavebuffer,72,0);//input,output:  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//#if 1
//          if ((BeaconDecodeOutput[4]!=0x99)&&(BeaconDecodeOutput[4]!=0x04))
//          {
//            crc = crc+1;
//            crc = crc -1;
//          }
//
//
//          if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,1);//input,output:左移或者右移1bit或者2bit  
//           deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);     
//       //   ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏小
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,2);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//       //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          //当前延时偏大
//          }
//#if 1
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,3);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//           if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,4);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,5);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,6);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         // ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//              if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,7);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//        //  ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//            if (crc!=0)
//          {
//          shift(redata,deinterleavebuffer,72,8);//input,output:左移或者右移1bit或者2bit  
//          deinterleave(deinterleavebuffer,redata1,beacon);
//          crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);
//         //ResetBeaconBuffer(deinterleavebuffer,redata1);
//
//          }
//          
//                          ///带协议的beacon发送的内容
//      if (BeaconDecodeOutput[0]==0x00)
//      {
//         if (BeaconDecodeOutput[1]==0x00)
//         {
//            if (BeaconDecodeOutput[2]==0x00)
//            {
//              if (BeaconDecodeOutput[3]==0x00)
//              {
//                if (BeaconDecodeOutput[4]==0x00)
//                {
//                  error00++;
//                  crc = 1;//crc错误
//                }
//              }
//            }
//         }
//      }
//          
//#endif
//#endif  
//             }
//   uint32 daiinfodatatimebegin = 0;       
//    if (rxframetype==infodata)
//    {
//      
//    daiinfodatatimebegin =   kernel_GetSystemClock();
//     
//     if(reTx_count==0)
//     {
//       uint16 ii;
//       for(ii=0;ii<576;ii++)
//       {
//         datastore[ii]=0;
//       }
//     }
//     
//#if 1 //new soft merge
//     tbcc_dec(redata,DataCRCOutputLength, DataDecodeOutput,1);
//     byte2bit(redata,redatabit);
//     
//   //  for (i=0;i<11;i++)//初始化
//   //    min_d_tmpArray[i] = 0;
//     crc = 1;
//     uint8 max_shift_count = 4;
//     
//   if (reTx_count==0)
//    {
//     while(shift_count<=max_shift_count)//有两处地方,4
//     {
//       shift_bit(redatabit,redatabit_shift,576,shift_count);
//       merge(redatabit_shift,datastore,datamerge);
//       uint8 numTx = 0;
//       numTx  = 1+reTx_count-wrong_min_d;
//       //min_d_tmp=tbcc_dec_bit2byte_infodata(datamerge,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//       min_d_tmp=tbcc_dec_bit2byte_infodata(datamerge,DataCRCOutputLength, DataDecodeOutput,numTx);
//       min_d_tmpArray[shift_count] = min_d_tmp;
//       if(min_d_tmp<min_d)
//       {
//         last_min_d = min_d;//2015-02-09
//         last_min_d_shift_count = min_d_shift_count;
//         
//         outmin_d = min_d_tmp;
//         min_d=min_d_tmp;
//         min_d_shift_count = shift_count;
//         
//         //暂时先去掉
//         //kernel_memcpy(datastore_tmp,redatabit_shift,576);
//          
//         for ( i=0;i<24;i++)//2015.02.05
//          nowRadioDataDecodeOutput[i]  = DataDecodeOutput[i];//2015.02.05         
//       }
//       crc=CRCCheck(DataDecodeOutput,infodata);   
//       
//      if(crc==0)
//       {
//         reTx_count=0;
//         break;
//       }
//       else
//       {
//         shift_count++;
//       }
//     }//end  while(shift_count<=4)//有两处地方    
//    }//end if if (reTx_count==0)
//   kernel_memcpy(datastore_tmp,redatabit,576);//将原始数据保存//保存了最近一次
//   
//#if 1
//   if (reTx_count==0)
//   {
//   kernel_memcpy(first_redatabit,redatabit,576);//将原始数据保存
//   kernel_memcpy(redata00,redata,72);
//   }
//   
//    if (reTx_count==1)
//   {
//   kernel_memcpy(second_redatabit,redatabit,576);//将原始数据保存
//   kernel_memcpy(redata11,redata,72);
//   }
//   
//   if (reTx_count==2)
//   {
//   kernel_memcpy(redata22,redata,72);
//   }
//   
//  if (reTx_count==3)
//   {
//   kernel_memcpy(redata33,redata,72);
//   }
//#endif
//    
//    
//    
//#if 1 //2015-02-09
//   uint8 choose_shift_count = 0;
//   choose_shift_count = max_shift_count;//最大是等于，因为后面是+1
//   
//   uint8 max_paixu_3 = 0;
//   max_paixu_3 = (choose_shift_count+1)*(max_shift_count+1);
//   
//   uint8 max_paixu_2 = 0;
//   max_paixu_2 = (choose_shift_count+1)*(max_shift_count+1);
//   
//   uint8 max_paixu_1 = 0;
//   max_paixu_1 = (max_shift_count+1);
//   
//       if (reTx_count>0)
//       {
//         if (reTx_count == 1)
//         {
//        paixu(min_d_tmpArray, min_d_tmpArray_index_1,max_paixu_1);  //5,排序，必须是上面的shift次数
//         }
//         if (reTx_count == 2)
//         {
//        paixu(min_d_tmpArray, min_d_tmpArray_index_2,max_paixu_2);  //25,排序，必须是下面两个for的最大值的乘积
//         }
//         
//          if (reTx_count == 3)
//         {
//        paixu(min_d_tmpArray, min_d_tmpArray_index_3,max_paixu_3);  //25,排序，必须是下面两个for的最大值的乘积
//         }
//         
//         
//         uint8 dai_i = 0;
//        for (dai_i=0;dai_i<(choose_shift_count+1);dai_i++)//5
//         {
//           for (j=0;j<(max_shift_count+1);j++)//5,后面有好多5
//           {
//             if (reTx_count == 1)
//             {
//             shift_bit(datastore,datastore_shift,576,min_d_tmpArray_index_1[dai_i]);
//             }
//             
//             if (reTx_count == 2)
//             {
//             shift_bit(first_redatabit,redatabit_shift,576,min_d_tmpArray_index_2[dai_i]/5);
//             shift_bit(datastore,tempdatastore_shift,576,min_d_tmpArray_index_2[dai_i]%5);
//              merge(redatabit_shift,tempdatastore_shift,datastore_shift);
//             }
//             
//             if (reTx_count == 3)
//             {
//             uint8  shifttemp0  = min_d_tmpArray_index_2[min_d_tmpArray_index_3[dai_i]/5]/5;
//             shift_bit(first_redatabit,redatabit_shift_first,576,shifttemp0);
//             uint8  shifttemp1  = min_d_tmpArray_index_2[min_d_tmpArray_index_3[dai_i]/5]%5;
//             shift_bit(second_redatabit,redatabit_shift_second,576,shifttemp1);
//             shift_bit(datastore,tempdatastore_shift,576,min_d_tmpArray_index_3[dai_i]%5);
//             merge(redatabit_shift_first,redatabit_shift_second,redatabit_shift);
//             merge(redatabit_shift,tempdatastore_shift,datastore_shift);
//             }
//             
//          shift_bit(redatabit,redatabit_shift,576,j);
//          merge(redatabit_shift,datastore_shift,datamerge);
//          uint8 numTx = 0;
//          numTx  = 1+reTx_count;
//          min_d_tmp=tbcc_dec_bit2byte_infodata(datamerge,DataCRCOutputLength, DataDecodeOutput,numTx);
//          //min_d_tmp=tbcc_dec_bit2byte_infodata(datamerge,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//          min_d_tmpArray[dai_i*5+j] = min_d_tmp;
//         
//       if(min_d_tmp<min_d)          
//       {
//         last_min_d = min_d;//2015-02-09
//         last_min_d_shift_count = min_d_shift_count;
//         
//         outmin_d = min_d_tmp;
//         min_d=min_d_tmp;
//         min_d_shift_count = shift_count;
//         
//         //暂时先去掉
//         //kernel_memcpy(datastore_tmp,redatabit_shift,576);
//          
//         for ( i=0;i<24;i++)//2015.02.05,
//          nowRadioDataDecodeOutput[i]  = DataDecodeOutput[i];//2015.02.05         
//       }
//       crc=CRCCheck(DataDecodeOutput,infodata);   
//       
//       
//       
//       if(crc==0)
//       {
//         reTx_count=0;
//         break;
//       }
//         }//end j
//     if(crc==0)
//       {
//         reTx_count=0;
//         break;
//       }
//         
//       }//end i
//       }//end if  if (reTx_count>0)
//
//#endif
//     
//
//       
//       if ((crc!=0)&&(reTx_count == 3))
//       {
//       //downdata;
//         crc=crc+1;
//          crc=crc-1;
//       }
//     
//     
//#if 0
//     unsigned int min = 0xffff;
//      unsigned int min2nd = 0xffff;
//     uint8 index_min = 0;
//     //找最小值
//     min = min_d_tmpArray[0];
//     index_min = 0;
//     for (i=0;i<5;i++)
//     {
//     if (min_d_tmpArray[i]<min)
//     {
//     min = min_d_tmpArray[i];
//     index_min = i;
//     }
//     }
//     //找次小值
//     for (i=0;i<5;i++)
//     {
//     if ((min_d_tmpArray[i]<min2nd)&&(i!=index_min))
//     {
//     min2nd = min_d_tmpArray[i];
//    // index_min = i;
//     }
//     }
//     
//     
//     /////2015.02.09 
//     if (crc!=0)
//     {
//     //循环结束后//2015-02-06
//
//         if ((min2nd-min)<8)
//         {
//           wrong_min_d++;//在void ReSet_reTx_count()中清零
//           uint16 ii;
//           for(ii=0;ii<576;ii++)
//            {
//              datastore_tmp[ii]=0;
//            }
//          }
//     }
//#endif   
//     
//     
//#if 0
//     if (crc!=0)
//     {
//     //循环结束后//2015-02-06
//        if ((reTx_count == 0))
//         {
//         if (min_d>150)
//         {
//           wrong_min_d++;//在void ReSet_reTx_count()中清零
//           uint16 ii;
//           for(ii=0;ii<576;ii++)
//            {
//              datastore_tmp[ii]=0;
//            }
//          }
//         }
//        ///
//        if (((reTx_count == 1)&&(wrong_min_d == 1)))
//         {
//         if (min_d>150)
//         {
//           wrong_min_d++;//在void ReSet_reTx_count()中清零
//           uint16 ii;
//           for(ii=0;ii<576;ii++)
//            {
//              datastore_tmp[ii]=0;
//            }
//          }
//         }
//        
//        
//     //////
//        if ((reTx_count == 1)&&(wrong_min_d == 0))
//         {
//         if (min_d>500)
//          {
//           wrong_min_d++;//在void ReSet_reTx_count()中清零
//           uint16 ii;
//           for(ii=0;ii<576;ii++)
//            {
//              datastore_tmp[ii]=0;
//            }
//           }
//         }
//     }
//     
//#endif 
//   
//          if (crc!=0)//2015.02.05
//           {
//          for ( i=0;i<24;i++)
//          DataDecodeOutput[i]  = nowRadioDataDecodeOutput[i];//2015.02.05
//           }
//          nowRadiocrc = crc;      
//     
//     /////全零的处理
//             for ( i=0;i<24;i++)
//               {
//              if (DataDecodeOutput[i]!=0)
//              {
//                break;
//              }
//               }
//            if (i == 24)
//            {
//         //   DataDecodeOutput[22] = 0xff;
//         //   DataDecodeOutput[23] = 0xff;
//            }
//            ///
//
//            
//     /////end 全零的处理
//     
//   //  if((shift_count>max_shift_count) && (crc!=0))//上面还有一出
//     {
//       kernel_memcpy(datastore,datastore_tmp,576);
//       
//       
//      
//       //tbcc_enc(dataCRC_tmp,DataCRCOutputLength,redata1);
//       tbcc_enc(DataDecodeOutput,DataCRCOutputLength,redata1);
//     }
//     
//     if(crc==0)
//     {
//    
//      // tbcc_enc(dataCRC_tmp,DataCRCOutputLength,redata1);
//       
//      // tbcc_enc(DataDecodeOutput,DataCRCOutputLength,redata1);
//     }
//#endif   
//     
//     
//               
//#if 0       //Old Hard Merging Start        
//              shift(redata,redata1,72,0);//input,output:左移或者右移1bit或者2bit       
//              tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//              crc = CRCCheck(DataDecodeOutput,infodata);
//    
//      #if 1
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,1);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);  
//                
//                 if (crc==0)
//                 {
//                  crc=crc;
//                 }
//              }
//          
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,2);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);
//                if (crc==0)
//               {
//                crc=crc;
//               }
//              }
//          
//       #if 1 
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,3);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);
//               if (crc==0)
//               {
//                crc=crc;
//               }
//              }
//          
//              if (crc!=0)
//              {
//                shift(redata,redata1,72,4);//input,output:左移或者右移1bit或者2bit  
//                tbcc_dec(redata1,DataCRCOutputLength,DataDecodeOutput,1);
//                crc = CRCCheck(DataDecodeOutput,infodata);
//               if (crc==0)
//               {
//                crc=crc;
//               }
//              }
//          
//#endif
//          
//#endif               
//   //myCRC(DataDecodeOutput,dataCRC_tmp,infodata);//2015.02.04
//   //tbcc_enc(dataCRC_tmp,DataCRCOutputLength,redata1);
//              
//    tbcc_enc(DataDecodeOutput,DataCRCOutputLength,redata1);
//    
//#endif          //Old Hard Merging End
//   }         
//
//    
//          if (crc==0)//crc=0时为解调正确
//          {
//          //  crc=1;
//           
//          
//       ///////数据检测，判断发送的数据是否和输出的数据一样。。。   
//
//            if(rxframetype==beacon)
//            {
//               rxtotal3++;
//           for ( i=0;i<5;i++)
//              if (i!=BeaconDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 5)
//            {
//          //  rxtotal2++;//全对  
//            }
//            }
//            
//             if(rxframetype==infodata)
//            {
//              crcRightcont++;//crc正确
//           //   srand(1000);
//           for ( i=2;i<22;i++)
//             // if (((uint8)rand())!=DataDecodeOutput[i])
//             if ((i-2)!=DataDecodeOutput[i])
//              {
//                break;
//              }
//            if (i == 22)
//            rxtotal2++;//内容也全对  
//           
//
//           
//            }
//          }//end if(crc == 0)
//                srand(1000);
//               rxtotal2right = 0;
//            for ( i=2;i<22;i++)//2015-02-06
//            {
//             // if (((uint8)rand())==DataDecodeOutput[i])
//                if ((i-2)==DataDecodeOutput[i])
//              {
//                rxtotal2right++;
//              }
//              
//    
//            
//            }
//    
//      
//      ///
//      info[2] = (rxtotal1>>8)&0xff;
//      info[3] = rxtotal1&0xff;
//      info[4] = (rxtotal2>>8)&0xff;
//      info[5] = rxtotal2&0xff;
//
//      
//
//
//      //if(crc == 0)
//      {
//        crc=1;
//     
//
//             ///freq_autotest
//        if(rxframetype==beacon)
//        {
//      uint32 FrameIndex_temp;
//      uint32 ZoneIndex_temp;
//      uint32 ReservedBit;
//      
//       FrameIndex_temp = get_UINT16(BeaconDecodeOutput[1],BeaconDecodeOutput[2]);
//       ZoneIndex_temp = (BeaconDecodeOutput[3]&0xC0)>>6;
//       ReservedBit = BeaconDecodeOutput[4];
//       
//       
//    //indexnum[0] = (((FrameIndex_temp-1)*3+ZoneIndex_temp)>>8)&0xff;
//    //indexnum[1] = (((ZoneIndex_temp-1)*3+ZoneIndex_temp))&0xff;
//        indexnum[0]  = (ReservedBit>>8)&0xff;
//        indexnum[1]  = ReservedBit&0xff;
//        // indexnum[0] = (((ReservedBit)*3+ZoneIndex_temp)>>8)&0xff;
//   // indexnum[1] = (((ReservedBit)*3+ZoneIndex_temp))&0xff;
//        }
//        ////
//            for ( i=0;i<72;i++)
//               {
//              if (redata[i]!=0)
//              {
//                break;
//              }
//               }
//            if (i == 72)
//            {
//             UartSendByte4dai("Allredata0\n",12);
//            }
//        /////
//       for ( i=0;i<72;i++)
//               {
//              if (redata1[i]!=0)
//              {
//                break;
//              }
//               }
//            if (i == 72)
//            {
//             UartSendByte4dai("Allredata1\n",12);
//            }
//        if (rxframetype==infodata)
//        {
//        uint32 daiinfodatatimeend = kernel_GetSystemClock();
//        uint16 daiinfodatatimechazhi = daiinfodatatimeend - daiinfodatatimebegin;
//        //UartSendByte4_uint16dai(daiinfodatatimechazhi);
//       // UartSendByte4_uint8dai(reTx_count);
//        }
//        
//       // UDP_Channel_Read(redata1);
//       UDP_Receive(redata1);
//
//
//       if (rxframetype==infodata)
//       {
//        tbcc_enc(DataDecodeOutput,DataCRCOutputLength,nowRadioredata1);
//        tbcc_dec(redata1,DataCRCOutputLength,PHYdataDecodeOutput,1);
//        PHYcrc=CRCCheck(PHYdataDecodeOutput,infodata);           
//         }
//
//          for (i=0;i<128;i++)
//        {
//           // nowRadioredata1 [i] =  redata1[i];
//            redata[i]=0;
//            redata1[i]=0;
//            deinterleavebuffer[i] = 0;
//         }
//
//      }
//     
//      
//      
//}
//
//#endif
//
//#if 1//添加barker的处理，2015-5-19 14:38:47
//void Raw_Rx_Packet_Hanlder(void) //Added dai
//{
//unsigned char i;
//  uint8 size,crc,InitLength;
//  unsigned char j,K=4;
//  crc=1;
//  
//  uint16 min_d_tmp=0;
//  
//	uint8 BeaconEncodeOutput[BeaconLength] = {0};
//	uint8 DataEncodeOutput[DataLength] = {0};
//
//	uint8 BeaconDecodeOutput[BeaconCRCOutputLength] = {0};
//
//  
//    SEGMENT_VARIABLE(redata1[128], U8);
//    
//  //85byte进，72byte出。包含找到准确的位置和deinterleave
//   barkerCalculator(redata,redata1);//不管beacon和infodata都是这么处理
//    
//    //beacon的处理
//    if (rxframetype==beacon)
//    {
//
//      crc = decode_tbcc_beacon4repetition(redata1,BeaconDecodeOutput,beacon);//还是有问题，4倍相加，超出了值，所以只能bit级别的计算  
//     }
//    
//    //infodata的处理
//   uint32 daiinfodatatimebegin = 0;       
//    if (rxframetype==infodata)
//    {
//      
//   // daiinfodatatimebegin =   kernel_GetSystemClock();
//
//#if 1 //new barker 
//    // byte2bit(redata1,redatabit1);
//     
//     crc = 1;
//
//      if (reTx_count==0)
//    {  
//       //min_d_tmp=tbcc_dec_bit2byte_infodata(redatabit1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      //byte to byte:1
//      tbcc_dec(redata1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      crc=CRCCheck(DataDecodeOutput,infodata);
//    }
//   
//   if (reTx_count==1)
//    {  
//       //merge(first_redatabit1,redatabit1,merge_bit1);
//       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      tbcc_dec_1(redata1,first_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      crc=CRCCheck(DataDecodeOutput,infodata);
//    }
//   
//   if (reTx_count==2)
//    {  
//       //merge(first_redatabit1,second_redatabit1,merge_bit1);
//       //merge(merge_bit1,redatabit1,merge_bit2);
//       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit2,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      tbcc_dec_2(redata1,first_redatabyte1,second_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      crc=CRCCheck(DataDecodeOutput,infodata);
//    }
//   
//   if (reTx_count==3)
//    {  
//       //merge(first_redatabit1,second_redatabit1,merge_bit1);
//       //merge(merge_bit1,third_redatabit1,merge_bit2);
//       //merge(merge_bit2,redatabit1,merge_bit3);
//       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit3,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//     tbcc_dec_3(redata1,first_redatabyte1,second_redatabyte1,third_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count); 
//     crc=CRCCheck(DataDecodeOutput,infodata);
//    }
//   
//     if (reTx_count==4)
//    {  
//       //merge(first_redatabit1,second_redatabit1,merge_bit1);
//       //merge(merge_bit1,third_redatabit1,merge_bit2);
//       //merge(merge_bit2,fourth_redatabit1,merge_bit3);
//       //merge(merge_bit3,redatabit1,merge_bit4);
//       //min_d_tmp=tbcc_dec_bit2byte_infodata(merge_bit4,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);
//      tbcc_dec_4(redata1,first_redatabyte1,second_redatabyte1,third_redatabyte1,fourth_redatabyte1,DataCRCOutputLength, DataDecodeOutput,1+reTx_count);  
//      crc=CRCCheck(DataDecodeOutput,infodata);
//    }
//   
//
//   if (reTx_count==0)
//   {
//   //kernel_memcpy(first_redatabit1,redatabit1,576);//将原始数据保存
//   kernel_memcpy(first_redatabyte1,redata1,72);//将原始数据保存
//   }
//   
//    if (reTx_count==1)
//   {
//   //kernel_memcpy(second_redatabit1,redatabit1,576);//将原始数据保存
//   kernel_memcpy(second_redatabyte1,redata1,72);//将原始数据保存
//   }
//   
//   if (reTx_count==2)
//   {
//   //kernel_memcpy(third_redatabit1,redatabit1,576);//将原始数据保存
//   kernel_memcpy(third_redatabyte1,redata1,72);//将原始数据保存
//   }
//   
//   if (reTx_count==3)
//   {
//   //kernel_memcpy(fourth_redatabit1,redatabit1,576);//将原始数据保存
//   kernel_memcpy(fourth_redatabyte1,redata1,72);//将原始数据保存
//   }
//      //必须添加
//    if(crc==0)
//    {
//     reTx_count=0;
//    }    
//   
//
////重新生成redata1，用于上传
//  tbcc_enc(DataDecodeOutput,DataCRCOutputLength,redata1);
//    }
//#endif    
//     
//
//    if (crc==0)//crc=0时为解调正确
//       {
//        crc=1;
//      if (rxframetype==infodata)
//        {
//          crcRightcont++;
//      //  uint32 daiinfodatatimeend = kernel_GetSystemClock();
//      //  uint16 daiinfodatatimechazhi = daiinfodatatimeend - daiinfodatatimebegin;
//        //UartSendByte4_uint16dai(daiinfodatatimechazhi);
//       // UartSendByte4_uint8dai(reTx_count);
//        }
//       
//       }
//             ///freq_autotest
//       if(rxframetype==beacon)
//        {
//      uint32 FrameIndex_temp;
//      uint32 ZoneIndex_temp;
//      uint32 ReservedBit;
//      
//       FrameIndex_temp = get_UINT16(BeaconDecodeOutput[1],BeaconDecodeOutput[2]);
//       ZoneIndex_temp = (BeaconDecodeOutput[3]&0xC0)>>6;
//       ReservedBit = BeaconDecodeOutput[4];
//             
//    //indexnum[0] = (((FrameIndex_temp-1)*3+ZoneIndex_temp)>>8)&0xff;
//    //indexnum[1] = (((ZoneIndex_temp-1)*3+ZoneIndex_temp))&0xff;
//        indexnum[0]  = (ReservedBit>>8)&0xff;
//        indexnum[1]  = ReservedBit&0xff;
//        // indexnum[0] = (((ReservedBit)*3+ZoneIndex_temp)>>8)&0xff;
//   // indexnum[1] = (((ReservedBit)*3+ZoneIndex_temp))&0xff;
//        }
//        ////
//            for ( i=0;i<85;i++)
//               {
//              if (redata[i]!=0)
//              {
//                break;
//              }
//               }
//            if (i == 85)
//            {
//             UartSendByte4dai("Allredata0\n",12);
//            }
//        /////
//             for ( i=0;i<72;i++)
//               {
//              if (redata1[i]!=0)
//              {
//                break;
//              }
//               }
//            if (i == 72)
//            {
//             UartSendByte4dai("Allredata1\n",12);
//            }
//    
//                   rxtotal2right = 0;
//            for ( i=2;i<22;i++)//2015-02-06
//            {
//             // if (((uint8)rand())==DataDecodeOutput[i])
//                if ((i-2)==DataDecodeOutput[i])
//              {
//                rxtotal2right++;
//              }
//
//            }
//    
//        
//
//         UDP_Receive(redata1);//server
//        //UDP_Channel_Read(redata1);//client
//
//        if (rxframetype==infodata)
//         {
//      //tbcc_enc(DataDecodeOutput,DataCRCOutputLength,nowRadioredata1);
//     // tbcc_dec(redata1,DataCRCOutputLength,PHYdataDecodeOutput,1);
//     //  PHYcrc=CRCCheck(PHYdataDecodeOutput,infodata);  
//         
//         }
//
//          for (i=0;i<128;i++)
//        {
//            redata[i]=0;
//            redata1[i]=0;
//         }
//
//      
//}
//#endif
//
//
//#endif
//
//
//
//void paixu(unsigned int input[],unsigned char output[],unsigned char num)
//{
//  unsigned int temp =0 ;
//  unsigned int backupinput[25]={0};
//  uint8 i,j;
//  uint8 temp_index = 0;
//  for (i = 0;i<num;i++)
//  {
//  output[i] = i;
//  backupinput[i]  = input[i];
//  }
// 
//  for (i = 0;i<num;i++)
//  {
//    for (j = i+1;j<num;j++)
//    {
//      
//      if (backupinput[j]<backupinput[i])
//      {
//        temp  = backupinput[j];
//        backupinput[j] = backupinput[i];
//        backupinput[i] = temp;
//        
//       temp_index  = output[i];
//      output[i] = output[j];
//      output[j] = temp_index;
//     
//      }
//    }
//  }
//  
//}
//
//
//
//
//
