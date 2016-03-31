#ifndef RADIO_HAL_H
#define RADIO_HAL_H

#include "hal_types.h"

void radio_hal_AssertShutdown(void);
void radio_hal_DeassertShutdown(void);
void radio_hal_ClearNsel(void);
void radio_hal_SetNsel(void);
void radio_hal_SpiWriteByte(U8 byteToWrite);
U8 radio_hal_SpiReadByte(void);
void radio_hal_SpiReadData(U8 byteCount, U8* pData);
void radio_hal_SpiWriteData(U8 byteCount, U8* pData);
#endif
