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
#ifdef ENABLE_FAST_SPI
  ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
#endif
}

U8 radio_hal_SpiReadByte(void)
#ifndef ENABLE_FAST_SPI
{
    return bSpi_ReadWriteSpi1(0xFF);
}
#else
{
  U8 tmp;
  
  tmp = bSpi_ReadWriteSpi1(0xFF);
  ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
  
  return tmp;

}
#endif
void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
#ifndef ENABLE_FAST_SPI
  vSpi_ReadDataSpi1(byteCount, pData);
#else
  while (byteCount--) {
    *pData++ = bSpi_ReadWriteSpi1(0xFF);
  }  
  
  ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
#endif
}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
#ifndef ENABLE_FAST_SPI
  vSpi_WriteDataSpi1(byteCount, pData);
#else
  while (byteCount--) {
    bSpi_ReadWriteSpi1(*pData++);
  }  
  
  ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
#endif
}
