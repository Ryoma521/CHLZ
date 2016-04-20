#ifndef HAL_DRIVERS_H
#define HAL_DRIVERS_H

#include "Hal_types.h"

void Hal_ProcessPoll ( void );
uint8 Hal_SendData(uint8 * Data, uint8 Length);

void HAL_IE_Enable(void);
void HAL_IE_Disable(void);

#endif
