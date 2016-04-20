#include "si4463.h"
#include "macro.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si4463_def.h"
#include "debug.h"

#include "radio.h"

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

//frequency scanning
//左右2k，一个步长400hz


//489.9980
#define RF_FREQ_CONTROL_INTE_8_0 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0x1E, 0x44, 0x44, 0x20, 0xFE

//489.9984
#define RF_FREQ_CONTROL_INTE_8_1 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0x3A, 0x44, 0x44, 0x20, 0xFE


//489.9988
#define RF_FREQ_CONTROL_INTE_8_2 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0x56, 0x44, 0x44, 0x20, 0xFE

//489.9992
#define RF_FREQ_CONTROL_INTE_8_3 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0x72, 0x44, 0x44, 0x20, 0xFE


//489.9996
#define RF_FREQ_CONTROL_INTE_8_4 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0x8E, 0x44, 0x44, 0x20, 0xFE

//490
#define RF_FREQ_CONTROL_INTE_8_5 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0xAA, 0x44, 0x44, 0x20, 0xFE


//490.0004
#define RF_FREQ_CONTROL_INTE_8_6 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0xC6, 0x44, 0x44, 0x20, 0xFE

//490.0008
#define RF_FREQ_CONTROL_INTE_8_7 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0xE2, 0x44, 0x44, 0x20, 0xFE


//490.0012
#define RF_FREQ_CONTROL_INTE_8_8 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAA, 0xFE, 0x44, 0x44, 0x20, 0xFE


//490.0016
#define RF_FREQ_CONTROL_INTE_8_9 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAB, 0x1A, 0x44, 0x44, 0x20, 0xFE

//490.0020
#define RF_FREQ_CONTROL_INTE_8_10 0x11, 0x40, 0x08, 0x00, 0x40, 0x0A, 0xAB, 0x36, 0x44, 0x44, 0x20, 0xFE










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

//end function frequency scanning


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