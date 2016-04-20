#include "radio.h"
#include "macro.h"
#include "si4463.h"
#include "si4463_def.h"
#include "radio_config.h"
#include "radio_config_1st.h"
#include "radio_config_2nd.h"
#include "radio_comm.h"
#include "radio_hal.h"
#include "radio_mcu.h"
#include "uart.h"

SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8) = \
              RADIO_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration) = \
                        RADIO_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration) = \
                        &RadioConfiguration;

SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], U8);

extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

extern FrameInfo FrameBuffToUart,FrameBuffToRf;
enum RF_RX_FLAG RfRxFlag=RfNoData;
extern uint8_t RxBuffer[];

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

  
/**
 *  Demo Application Poll-Handler-TX
 *
 *  @note This function must be called periodically.
 *
 */
void DemoApp_Pollhandler_RX()
{
  // Check if radio packet received
  if (TRUE == gRadio_CheckReceived())
  {
      
  }    
}

uint16 rxtotal1 = 0;
uint16 rxtotal2 = 0;

 extern uint32 tmpGlobalTime;
 
 uint32 delaystartMs=0;
 
 SEGMENT_VARIABLE(redata[RADIO_MAX_PACKET_LENGTH], U8);
 
 uint32 n_true=0;
  uint32 n_false=0;
    uint32 n_total=0;
      uint32 n_crc=0;
    
    uint32 n_total_true=0;
  uint32 delaytimeMs=5;  
  uint32 delaytimeUs=228;
  
  //5,228
  
//用于frequency scanning
  
   uint8 CenfreqfixFlag =0;  
uint32 scanCntRx[11]={0};

uint8 maxindex=0;
uint32 starttime=0;
uint32 nowtime=0;
uint32 freq_configurationCnt=0;
  

//end frequency scanning

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

uint8 gRadio_CheckReceived(void)
{  
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
    
    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {    
      si446x_fifo_info(0u);     
            
      si446x_read_rx_fifo(1, &RxBuffer[0]);    
      
      si446x_read_rx_fifo(RxBuffer[0], &RxBuffer[1]);        
      
      RxBuff2FrameBuff(RxBuffer,&FrameBuffToUart);      
      
      RfRxFlag=RfRxData;      
      
      /* Reset FIFO */
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT); 
      si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
      return TRUE;  
    }
    /* Reset FIFO */
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT); 
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
    return FALSE;
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
#ifdef UART_LOGGING_SUPPORT
      {
          /* Send it to UART */
        UartSendByte(fixRadioPacket,RadioConfiguration.Radio_PacketLength);
      }
#endif

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
void vRadio_StartRX(U8 channel)
{
  //Switch to RX match network
  RF_Switch_RX();
  
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


uint8 SI4463_Config_Transmit_singlerate(U8 * pSrc1,U8 len1)
{
  
  //发送第一个包
   trx_state = 0;//f发送模式
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
    return FALSE;
  }

  /* Reset TX FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX FIFO with data */
  si446x_write_tx_fifo(len1, pSrc1);


  /* Start sending packet, channel 0, START immediately,
   * Packet length according to 'len', go READY when done */
  si446x_start_tx(0u, 0x30,  len1);
  
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
    return FALSE;
    }  
  }
  
   PKT_Sent_Flag =0;
  
  
#if 0
  //DelayMs(1);
//发送第二个包
  
  vRadio_config_2nd();
  
  trx_state = 0;//f发送模式
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
    return FALSE;
  }

  /* Reset TX FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX FIFO with data */
  si446x_write_tx_fifo(len2, pSrc2);


  /* Start sending packet, channel 0, START immediately,
   * Packet length according to 'len', go READY when done */
  si446x_start_tx(0u, 0x30,  len2);
  
  
  while (PKT_Sent_Flag!=1)//发送等待
  {
  //等待保护
  asm("nop");
  counter++;
  if(counter >= 0xFFFFFFFF)
  {
     //第一个包未发送成功
    counter=0;
    return FALSE;
  }
    
  }
  
  PKT_Sent_Flag =0;
  vRadio_config_1st();
  
  vRadio_StartRX_1st(pRadioConfiguration_1st->Radio_ChannelNumber);//回复第一个包的接收模式
  trx_state = 1;//第二个包发完之后默认为接收状态
   
#endif 
  
    vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber);//回复第一个包的接收模式
  trx_state = 1;//第二个包发完之后默认为接收状态
  
  return TRUE;
}

uint8 New_SI4463_Transmit(U8 * pSrc,U8 len)
{
   
  
  SI4463_Enable_NIRQ_TX();
  
 
  
  
  /* Check if the radio is already in TX state */
  si446x_request_device_state();
  if (SI446X_CMD_REQUEST_DEVICE_STATE_REP_MAIN_STATE_ENUM_TX == \
      (Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE & 0x0F)) {
    /* Still transmitting */
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
    return FALSE;
  }
    
 }  
  StopTime = tmpGlobalTime-StatTime;
 

 SI4463_Enable_NIRQ_RX();
  EXTILineNIRQ_Config();
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
  uint8 temp;
  
 
  // Check if the radio packet sent successfully
  if (TRUE == gRadio_CheckTransmitted())
  {
    /* Clear Packet Sending flag */
    temp = 2;
    UartSendByte(&temp,1);
    lPktSending = 0u;
  }
  else
  {
    temp = 3;
    UartSendByte(&temp,1);
 
  }

  //lPktSending = 0;


  if ((0u == lPktSending))
  {
    uint8 flag=vSampleCode_SendFixPacket((U8 *)Packet,length);
    
    UartSendByte(&flag,1);
    
    if (TRUE == flag)
    {
      //lPer_SecCntr = 0u;

      /* Set Packet Sending flag */
      lPktSending = 1u;
      
    }
  }
}

void vRadio_StartTx_Variable_Packet(U8 channel, U8 *pioRadioPacket, U8 length)
{
  /* Leave RX state */
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
  
  /*Switch to TX match network*/
  RF_Switch_TX();
  
  
  /* Read ITs, clear pending ones */
  si446x_get_int_status(0u, 0u, 0u);

  /* Reset the Tx Fifo */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  si446x_fifo_info(0u);
  /* Fill the TX fifo with datas */
  si446x_write_tx_fifo(length, pioRadioPacket);
  si446x_fifo_info(0u);
  /* Start sending packet, channel 0, START immediately */
   si446x_start_tx(channel, 0x30, length);   
}

