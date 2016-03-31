#include "si4463_spi.h"
#include "macro.h"
#include "radio_mcu.h"

U8 bSpi_ReadWriteSpi1(U8 biDataIn)
{  
  uint8 x;
  uint8 status = 0;
  
  //HAL_IE_Disable();

  for(x=8;x>0;x--)
  {
    if(biDataIn & 0x80)   // If bit is high...
    {
#ifndef ENABLE_FAST_SPI
      SI4463_SDI_Up();   // Set SIMO high...
#else      
      ((GPIO_TypeDef*)PORT_SDI)->BSRRL = PIN_SDI;
#endif
    }                         
    else
    {
#ifndef ENABLE_FAST_SPI   
      SI4463_SDI_Down(); // Set SIMO low...
#else
    ((GPIO_TypeDef*)PORT_SDI)->BSRRH = PIN_SDI;
#endif
    }
    biDataIn = biDataIn << 1; // Rotate bits

    //DelayUs(10);
#ifndef ENABLE_FAST_SPI
    SI4463_SCLK_Down(); // Set clock low
#else
    ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
#endif
    //DelayUs(10);
#ifndef ENABLE_FAST_SPI    
    SI4463_SCLK_Up(); // Set clock high
#else
    ((GPIO_TypeDef*)PORT_SCLK)->BSRRL = PIN_SCLK;
#endif
    //DelayUs(10);
    
    status = status << 1;
#ifndef ENABLE_FAST_SPI    
    status |= SI4463_SDO_Bit();
#else    
    status |= ((PORT_SDO->IDR & PIN_SDO)>>5);
#endif
  }
#ifndef ENABLE_FAST_SPI
  SI4463_SCLK_Down();
#endif
  
  //HAL_IE_Enable();

  return status;
}

void vSpi_ReadDataSpi1(U8 biDataOutLength, U8 *paboDataOut)
{
  while (biDataOutLength--) {
    *paboDataOut++ = bSpi_ReadWriteSpi1(0xFF);
  }
#ifdef ENABLE_FAST_SPI
  ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
#endif
}

void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn)
{
  while (biDataInLength--) {
    bSpi_ReadWriteSpi1(*pabiDataIn++);
  }
#ifdef ENABLE_FAST_SPI
  ((GPIO_TypeDef*)PORT_SCLK)->BSRRH = PIN_SCLK;
#endif
}