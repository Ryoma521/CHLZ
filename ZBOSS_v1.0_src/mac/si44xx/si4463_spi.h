#ifndef SI4463_SPI_H
#define SI4463_SPI_H

#include "hal_types.h"

U8 bSpi_ReadWriteSpi1(U8 biDataIn);
void vSpi_ReadDataSpi1(U8 biDataOutLength, U8 *paboDataOut);
void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn);

#endif
