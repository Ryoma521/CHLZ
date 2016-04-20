#include "si4463.h"
#include "macro.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si4463_def.h"
#include "debug.h"

SEGMENT_VARIABLE( Pro2Cmd[16], U8);
SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

void si446x_fifo_info(U8 FIFO)
{
    Pro2Cmd[0] = SI446X_CMD_ID_FIFO_INFO;
    Pro2Cmd[1] = FIFO;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_FIFO_INFO,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_FIFO_INFO,
                              Pro2Cmd );

    Si446xCmd.FIFO_INFO.RX_FIFO_COUNT   = Pro2Cmd[0];
    Si446xCmd.FIFO_INFO.TX_FIFO_SPACE   = Pro2Cmd[1];
}

void si446x_get_int_status(U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_INT_STATUS;
    Pro2Cmd[1] = PH_CLR_PEND;
    Pro2Cmd[2] = MODEM_CLR_PEND;
    Pro2Cmd[3] = CHIP_CLR_PEND;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_INT_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_INT_STATUS,
                              Pro2Cmd );

    Si446xCmd.GET_INT_STATUS.INT_PEND       = Pro2Cmd[0];
    Si446xCmd.GET_INT_STATUS.INT_STATUS     = Pro2Cmd[1];
    Si446xCmd.GET_INT_STATUS.PH_PEND        = Pro2Cmd[2];
    Si446xCmd.GET_INT_STATUS.PH_STATUS      = Pro2Cmd[3];
    Si446xCmd.GET_INT_STATUS.MODEM_PEND     = Pro2Cmd[4];
    Si446xCmd.GET_INT_STATUS.MODEM_STATUS   = Pro2Cmd[5];
    Si446xCmd.GET_INT_STATUS.CHIP_PEND      = Pro2Cmd[6];
    Si446xCmd.GET_INT_STATUS.CHIP_STATUS    = Pro2Cmd[7];
}

void si446x_reset(void)
{
    U8 loopCount;

    /* Put radio in shutdown, wait then release */
    radio_hal_AssertShutdown();
    //! @todo this needs to be a better delay function.
    for (loopCount = 255; loopCount != 0; loopCount--);
    DelayMs(10);
    radio_hal_DeassertShutdown();
    for (loopCount = 255; loopCount != 0; loopCount--);
    DelayMs(10);
    radio_comm_ClearCTS();
}

//ack len变化
#define RF_1st_PKT_LEN_12_1 0x11, 0x12, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00
#define RF_1st_PKT_LEN_12_2 0x11, 0x12, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00


#define RF_1st_PKT_LEN_ARRAY_1 { \
        0x10, RF_1st_PKT_LEN_12_1, \
        0x00 \
 }
#define RF_1st_PKT_LEN_ARRAY_2 { \
        0x10, RF_1st_PKT_LEN_12_2, \
        0x00 \
 }

SEGMENT_VARIABLE(PKT_LEN_Data_Array_1[], U8) = \
            RF_1st_PKT_LEN_ARRAY_1;
SEGMENT_VARIABLE(PKT_LEN_Data_Array_2[], U8) = \
            RF_1st_PKT_LEN_ARRAY_2;

U8 si446x_pkt_len_configuration(U8 cnt)
{

  U16 wDelay;

 if (cnt==1)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(PKT_LEN_Data_Array_1))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==2)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(PKT_LEN_Data_Array_2))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
}


#ifdef BOARD_3_002
//Client 3-002 480M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x00, 0x02, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_3_010
//Client 3-010 480M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x00, 0xA0, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_3_011
//Client 3-011 480M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x00, 0x35, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_3_012
//Client 3-012 480M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3E, 0xFF, 0xFF, 0xE0, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_3_030
//Client 3-030 470M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_3_031
//Client 3-030 470M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0x65, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_3_032
//Client 3-032 480M
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x00, 0x19, 0x44, 0x44, 0x20, 0xFE






/***********************************************************/

#elif defined BOARD_F03  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xE0, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F04  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0x28, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F05  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xEA, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F08  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xA4, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F11  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xB0, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F16  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0x9E, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F17  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0x8E, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F19  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xDE, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F22  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xB6, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F24  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0x8D, 0x44, 0x44, 0x20, 0xFE

#elif defined BOARD_F30  //470
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xA0, 0x44, 0x44, 0x20, 0xFE

#else
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x53, 0xEA, 0x44, 0x44, 0x20, 0xFE

#endif

//server 端没有freq_scan,用470M

#define RF_FREQ_CONTROL_INTE_8_0 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_1 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_2 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_3 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE

//si-0-30---defluat,server
//#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xC3, 0x44, 0x44, 0x20, 0xFE

//si-0-31,470    -18.83,3的时候最准，但是该为4
//#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0x83, 0x44, 0x44, 0x20, 0xFE



//////////////////////////////////////////////////
//si-3-030:470M:-18.56(PA:0x01),20150104
#define RF_FREQ_CONTROL_INTE_8_5 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD3, 0x44, 0x44, 0x20, 0xFE

//si-3-031:470M:-18.83(PA:0x01),20150104
//#define RF_FREQ_CONTROL_INTE_8_5 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0x83, 0x44, 0x44, 0x20, 0xFE


/////////////////////////////////////////////////

#define RF_FREQ_CONTROL_INTE_8_6 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_7 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_8 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_9 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE
#define RF_FREQ_CONTROL_INTE_8_10 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0D, 0x55, 0xD4, 0x44, 0x44, 0x20, 0xFE










////////////////////////////////////////////////////




//////////////////////////////////////////
#define FREQ_CONFIGURATION_DATA_ARRAY_0 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_0, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_1 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_1, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_2 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_2, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_3 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_3, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_4 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_4, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_5 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_5, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_6 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_6, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_7 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_7, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_8 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_8, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_9 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_9, \
        0x00 \
 }
#define FREQ_CONFIGURATION_DATA_ARRAY_10 { \
        0x0C, RF_FREQ_CONTROL_INTE_8_10, \
        0x00 \
 }

SEGMENT_VARIABLE(FreqConfiguration_Data_Array_0[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_0;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_1[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_1;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_2[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_2;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_3[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_3;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_4[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_4;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_5[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_5;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_6[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_6;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_7[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_7;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_8[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_8;
SEGMENT_VARIABLE(FreqConfiguration_Data_Array_9[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_9;

SEGMENT_VARIABLE(FreqConfiguration_Data_Array_10[], U8) = \
            FREQ_CONFIGURATION_DATA_ARRAY_10;


U8 si446x_freq_configuration(U8 cnt)
{

  U16 wDelay;

 if (cnt==0)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_0))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==1)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_1))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==2)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_2))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==3)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_3))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==4)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_4))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==5)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_5))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==6)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_6))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==7)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_7))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==8)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_8))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
   if (cnt==9)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_9))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
 
    if (cnt==10)
  {
  while (SI446X_SUCCESS != si446x_configuration_init(FreqConfiguration_Data_Array_10))
  {

    for (wDelay = 0x7FFF; wDelay--; ) ;

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }
  }
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);

}

U8 si446x_configuration_init(const U8* pSetPropCmd)
{
  SEGMENT_VARIABLE(col, U8);
  SEGMENT_VARIABLE(numOfBytes, U8);

  /* While cycle as far as the pointer points to a command */
  while (*pSetPropCmd != 0x00)
  {
    /* Commands structure in the array:
     * --------------------------------
     * LEN | <LEN length of data>
     */

    numOfBytes = *pSetPropCmd++;

    if (numOfBytes > 16u)   //CMD Frame Length Must Be Less Than 16
    {
      /* Number of command bytes exceeds maximal allowable length */
      return SI446X_COMMAND_ERROR;
    }

    for (col = 0u; col < numOfBytes; col++)			//Make A Copy Of CMD Frame
    {
      Pro2Cmd[col] = *pSetPropCmd;
      pSetPropCmd++;
    }

    if (radio_comm_SendCmdGetResp(numOfBytes, Pro2Cmd, 0, 0) != 0xFF)
    {
      /* Timeout occured */
      return SI446X_CTS_TIMEOUT;
    }

#if 0    
    if (radio_hal_NirqLevel() == 0)
    {
      /* Get and clear all interrupts.  An error has occured... */
      si446x_get_int_status(0, 0, 0);
      if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
      {
        return SI446X_COMMAND_ERROR;
      }
    }
#endif    
  }

  return SI446X_SUCCESS;
}

void si446x_frr_a_read(U8 respByteCount)
{
    radio_comm_ReadData(SI446X_CMD_ID_FRR_A_READ,
                            0,
                        respByteCount,
                        Pro2Cmd);

    Si446xCmd.FRR_A_READ.FRR_A_VALUE = Pro2Cmd[0];
    Si446xCmd.FRR_A_READ.FRR_B_VALUE = Pro2Cmd[1];
    Si446xCmd.FRR_A_READ.FRR_C_VALUE = Pro2Cmd[2];
    Si446xCmd.FRR_A_READ.FRR_D_VALUE = Pro2Cmd[3];
}

void si446x_part_info(void)
{
    Pro2Cmd[0] = SI446X_CMD_ID_PART_INFO;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PART_INFO,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_PART_INFO,
                              Pro2Cmd );

    Si446xCmd.PART_INFO.CHIPREV         = Pro2Cmd[0];

    Si446xCmd.PART_INFO.PBUILD          = Pro2Cmd[3];

    Si446xCmd.PART_INFO.CUSTOMER        = Pro2Cmd[6];
    Si446xCmd.PART_INFO.ROMID           = Pro2Cmd[7];
}

/*!
 * Issue a change state command to the radio.
 *
 * @param NEXT_STATE1 Next state.
 */
void si446x_change_state(U8 NEXT_STATE1)
{
    Pro2Cmd[0] = SI446X_CMD_ID_CHANGE_STATE;
    Pro2Cmd[1] = NEXT_STATE1;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_CHANGE_STATE, Pro2Cmd );
}


/*!
 * Sends START_RX command to the radio.
 *
 * @param CHANNEL     Channel number.
 * @param CONDITION   Start RX condition.
 * @param RX_LEN      Payload length (exclude the PH generated CRC).
 * @param NEXT_STATE1 Next state when Preamble Timeout occurs.
 * @param NEXT_STATE2 Next state when a valid packet received.
 * @param NEXT_STATE3 Next state when invalid packet received (e.g. CRC error).
 */
void si446x_start_rx(U8 CHANNEL, U8 CONDITION, U16 RX_LEN, U8 NEXT_STATE1, U8 NEXT_STATE2, U8 NEXT_STATE3)
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_RX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (U8)(RX_LEN >> 8);
    Pro2Cmd[4] = (U8)(RX_LEN);
    Pro2Cmd[5] = NEXT_STATE1;
    Pro2Cmd[6] = NEXT_STATE2;
    Pro2Cmd[7] = NEXT_STATE3;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_RX, Pro2Cmd );
}

/*! Sends START_TX command to the radio.
 *
 * @param CHANNEL   Channel number.
 * @param CONDITION Start TX condition.
 * @param TX_LEN    Payload length (exclude the PH generated CRC).
 */
void si446x_start_tx(U8 CHANNEL, U8 CONDITION, U16 TX_LEN)
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_TX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (U8)(TX_LEN >> 8);
    Pro2Cmd[4] = (U8)(TX_LEN);
    Pro2Cmd[5] = 0x00;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_TX, Pro2Cmd );
}

/*!
 * The function can be used to load data into TX FIFO.
 *
 * @param numBytes  Data length to be load.
 * @param pTxData   Pointer to the data (U8*).
 */
void si446x_write_tx_fifo(U8 numBytes, U8* pTxData)
{
  radio_comm_WriteData( SI446X_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
}

/*!
 * Reads the RX FIFO content from the radio.
 *
 * @param numBytes  Data length to be read.
 * @param pRxData   Pointer to the buffer location.
 */
void si446x_read_rx_fifo(U8 numBytes, U8* pRxData)
{
  radio_comm_ReadData( SI446X_CMD_ID_READ_RX_FIFO, 0, numBytes, pRxData );
}

/*!
 * Send GPIO pin config command to the radio and reads the answer into
 * @Si446xCmd union.
 *
 * @param GPIO0       GPIO0 configuration.
 * @param GPIO1       GPIO1 configuration.
 * @param GPIO2       GPIO2 configuration.
 * @param GPIO3       GPIO3 configuration.
 * @param NIRQ        NIRQ configuration.
 * @param SDO         SDO configuration.
 * @param GEN_CONFIG  General pin configuration.
 */
void si446x_gpio_pin_cfg(U8 GPIO0, U8 GPIO1, U8 GPIO2, U8 GPIO3, U8 NIRQ, U8 SDO, U8 GEN_CONFIG)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GPIO_PIN_CFG;
    Pro2Cmd[1] = GPIO0;
    Pro2Cmd[2] = GPIO1;
    Pro2Cmd[3] = GPIO2;
    Pro2Cmd[4] = GPIO3;
    Pro2Cmd[5] = NIRQ;
    Pro2Cmd[6] = SDO;
    Pro2Cmd[7] = GEN_CONFIG;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GPIO_PIN_CFG,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GPIO_PIN_CFG,
                              Pro2Cmd );

    Si446xCmd.GPIO_PIN_CFG.GPIO0        = Pro2Cmd[0];
    Si446xCmd.GPIO_PIN_CFG.GPIO1        = Pro2Cmd[1];
    Si446xCmd.GPIO_PIN_CFG.GPIO2        = Pro2Cmd[2];
    Si446xCmd.GPIO_PIN_CFG.GPIO3        = Pro2Cmd[3];
    Si446xCmd.GPIO_PIN_CFG.NIRQ         = Pro2Cmd[4];
    Si446xCmd.GPIO_PIN_CFG.SDO          = Pro2Cmd[5];
    Si446xCmd.GPIO_PIN_CFG.GEN_CONFIG   = Pro2Cmd[6];
}

/*!
 * Requests the current state of the device and lists pending TX and RX requests
 */
void si446x_request_device_state(void)
{
   
    
    Pro2Cmd[0] = SI446X_CMD_ID_REQUEST_DEVICE_STATE;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_REQUEST_DEVICE_STATE,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE,
                              Pro2Cmd );

    Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE       = Pro2Cmd[0];
    Si446xCmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL  = Pro2Cmd[1];
}
uint8  sRadioGetRSSI(void)
{
  /* FRR A holds the latched RSSI (if any). */
  si446x_frr_a_read(SI446X_CMD_REPLY_COUNT_FRR_A_READ);
  return Si446xCmd.FRR_A_READ.FRR_A_VALUE;
}

uint8 initial_freq_configuration_data[14] = {0};
void new_si446x_freq_configuration(uint8 initial_freq_count)
{
  initial_freq_configuration_data[0] = 0x0C;
  
  initial_freq_configuration_data[1] = 0x11;
  initial_freq_configuration_data[2] = 0x40;
  initial_freq_configuration_data[3] = 0x08;
  initial_freq_configuration_data[4] = 0x00;
  
  initial_freq_configuration_data[5] = 0x3D;
  initial_freq_configuration_data[6] = 0x0D;
  

  ///0
    if (initial_freq_count == 0)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;

  }
    if (initial_freq_count == 1)
  {

 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 2)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 3)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;

  }
    if (initial_freq_count == 4)
  {
#ifdef BOARD_F38
 initial_freq_configuration_data[7] = 0x53;
 initial_freq_configuration_data[8] = 0xE0;    
#else
 initial_freq_configuration_data[7] = 0x55;
#ifdef BOARD_3_030
 initial_freq_configuration_data[8] = 0xD4;//30,当前
#elif defined BOARD_3_031
 initial_freq_configuration_data[8] = 0x65;//031
#endif
#endif

  }
    if (initial_freq_count == 5)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 6)
  {
 
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
  
  }
  
    
  if (initial_freq_count == 7)
  {
 
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 8)
  {
 
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
  
  }
    if (initial_freq_count == 9)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
  
  }
  
  
  initial_freq_configuration_data[9] = 0x44;
  initial_freq_configuration_data[10] = 0x44;
  initial_freq_configuration_data[11] = 0x20;
  initial_freq_configuration_data[12] = 0xFE;
  
  initial_freq_configuration_data[13] = 0x00;
  
  while (SI446X_SUCCESS != si446x_configuration_init(initial_freq_configuration_data))
  {
  }
  
}

void new478MHz_si446x_freq_configuration(uint8 initial_freq_count)
{
  initial_freq_configuration_data[0] = 0x0C;
  
  initial_freq_configuration_data[1] = 0x11;
  initial_freq_configuration_data[2] = 0x40;
  initial_freq_configuration_data[3] = 0x08;
  initial_freq_configuration_data[4] = 0x00;
  
  initial_freq_configuration_data[5] = 0x3E;
  initial_freq_configuration_data[6] = 0x0D;
  

  ///0
    if (initial_freq_count == 0)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;

  }
    if (initial_freq_count == 1)
  {

 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 2)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 3)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;

  }
    if (initial_freq_count == 4)
  {
#ifdef BOARD_F38
 initial_freq_configuration_data[7] = 0x53;
 initial_freq_configuration_data[8] = 0xE0; 
#elif defined newBOARD_SD100039
 initial_freq_configuration_data[7] = 0xDE;
#elif defined newBOARD_SD100048
 initial_freq_configuration_data[7] = 0xDD;
#else
 initial_freq_configuration_data[7] = 0xDE;
#endif
 
#ifdef BOARD_3_030
 initial_freq_configuration_data[8] = 0xD4;//30,当前
#elif defined BOARD_3_031
 initial_freq_configuration_data[8] = 0x65;//031
#elif defined BOARD_3_011
 initial_freq_configuration_data[8] = 0xD8;//011
#elif defined BOARD_3_012
 initial_freq_configuration_data[8] = 0x62;//012
#elif defined newBOARD_SD100033
 initial_freq_configuration_data[8] = 0x20;//033
#elif defined newBOARD_SD100039
 initial_freq_configuration_data[8] = 0x30;//039
#elif defined newBOARD_SD100048
 initial_freq_configuration_data[8] = 0xD0;//048
#endif


  }
    if (initial_freq_count == 5)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 6)
  {
 
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
  
  }
  
    
  if (initial_freq_count == 7)
  {
 
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
 
  }
    if (initial_freq_count == 8)
  {
 
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
  
  }
    if (initial_freq_count == 9)
  {
  
 initial_freq_configuration_data[7] = 0x55;
  initial_freq_configuration_data[8] = 0x55;
  
  }
  
  
  initial_freq_configuration_data[9] = 0x44;
  initial_freq_configuration_data[10] = 0x44;
  initial_freq_configuration_data[11] = 0x20;
  initial_freq_configuration_data[12] = 0xFE;
  
  initial_freq_configuration_data[13] = 0x00;
  
  while (SI446X_SUCCESS != si446x_configuration_init(initial_freq_configuration_data))
  {
  }
  
}