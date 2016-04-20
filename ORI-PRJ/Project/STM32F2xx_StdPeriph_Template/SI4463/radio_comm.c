#include "radio_comm.h"
#include "macro.h"
#include "radio_hal.h"

U8 ctsWentHigh = 0;

void radio_comm_ClearCTS(void)
{
  ctsWentHigh = 0;
}

U8 radio_comm_GetResp(U8 byteCount, U8* pData)
{
  SEGMENT_VARIABLE(ctsVal = 0u, U8);
  SEGMENT_VARIABLE(errCnt = RADIO_CTS_TIMEOUT, U16);

  while (errCnt != 0)      //wait until radio IC is ready with the data
  {
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(0x44);    //read CMD buffer
    ctsVal = radio_hal_SpiReadByte();
    if (ctsVal == 0xFF)
    {
      if (byteCount)
      {
        radio_hal_SpiReadData(byteCount, pData);
      }
      radio_hal_SetNsel();
      break;
    }
    radio_hal_SetNsel();
    errCnt--;
  }

  if (errCnt == 0)
  {
    while(1)
    {
      /* ERROR!!!!  CTS should never take this long. */
      #ifdef RADIO_COMM_ERROR_CALLBACK
        RADIO_COMM_ERROR_CALLBACK();
      #endif
    }
  }

  if (ctsVal == 0xFF)
  {
    ctsWentHigh = 1;
  }

  return ctsVal;
}

U8 radio_comm_PollCTS(void)
{
    return radio_comm_GetResp(0, 0);
}

void radio_comm_SendCmd(U8 byteCount, U8* pData)
{
    /* There was a bug in A1 hardware that will not handle 1 byte commands. 
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
    if (byteCount == 1)
        byteCount++;

    while (!ctsWentHigh)
    {
        radio_comm_PollCTS();
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData)
{
    radio_comm_SendCmd(cmdByteCount, pCmdData);
    return radio_comm_GetResp(respByteCount, pRespData);
}

void radio_comm_ReadData(U8 cmd, uint8 pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiReadData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 */
void radio_comm_WriteData(U8 cmd, uint8 pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}