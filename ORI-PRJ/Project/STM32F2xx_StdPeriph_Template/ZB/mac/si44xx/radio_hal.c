#include "radio_hal.h"
#include "radio_mcu.h"
#include "si4463_spi.h"

void radio_hal_AssertShutdown(void)
{
  SI4463_SDN_UP();
}

void radio_hal_DeassertShutdown(void)
{
  SI4463_SDN_DOWN();
}

void radio_hal_ClearNsel(void)
{
  SI4463_NSEL_Down();
}

void radio_hal_SetNsel(void)
{
  SI4463_NSEL_Up();
}

void radio_hal_SpiWriteByte(U8 byteToWrite)
{
  bSpi_ReadWriteSpi1(byteToWrite);
}

U8 radio_hal_SpiReadByte(void)
{
  return bSpi_ReadWriteSpi1(0xFF);
}

void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
  vSpi_ReadDataSpi1(byteCount, pData);
}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
  vSpi_WriteDataSpi1(byteCount, pData);
}
