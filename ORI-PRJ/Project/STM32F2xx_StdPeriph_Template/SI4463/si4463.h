#ifndef SI4463_H
#define SI4463_H

#include "hal_types.h"

enum
{
    SI446X_SUCCESS,
    SI446X_NO_PATCH,
    SI446X_CTS_TIMEOUT,
    SI446X_PATCH_FAIL,
    SI446X_COMMAND_ERROR
};

//SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

void si446x_reset(void);
void si446x_frr_a_read(U8 respByteCount);

void si446x_start_rx(U8 CHANNEL, U8 CONDITION, U16 RX_LEN, U8 NEXT_STATE1, U8 NEXT_STATE2, U8 NEXT_STATE3);
void si446x_start_tx(U8 CHANNEL, U8 CONDITION, U16 TX_LEN);

void si446x_write_tx_fifo(U8 numBytes, U8* pTxData);
void si446x_read_rx_fifo(U8 numBytes, U8* pRxData);
void si446x_fifo_info(U8 FIFO);
void si446x_part_info(void);

void si446x_change_state(U8 NEXT_STATE1);

U8 si446x_configuration_init(const U8* pSetPropCmd);

void si446x_get_int_status(U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND);

void si446x_gpio_pin_cfg(U8 GPIO0, U8 GPIO1, U8 GPIO2, U8 GPIO3, U8 NIRQ, U8 SDO, U8 GEN_CONFIG);

void si446x_request_device_state(void);

#endif
