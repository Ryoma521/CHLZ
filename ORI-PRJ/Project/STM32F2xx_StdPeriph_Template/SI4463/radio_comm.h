#ifndef RADIO_COMM_H
#define RADIO_COMM_H

#include "hal_types.h"

#define RADIO_CTS_TIMEOUT 10000
                        
void radio_comm_ClearCTS(void);
U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData);
void radio_comm_ReadData(U8 cmd, uint8 pollCts, U8 byteCount, U8* pData);
void radio_comm_SendCmd(U8 byteCount, U8* pData);
U8 radio_comm_GetResp(U8 byteCount, U8* pData);
void radio_comm_WriteData(U8 cmd, uint8 pollCts, U8 byteCount, U8* pData);

#endif
