/*! @file radio_config.h
 * @brief This file contains the automatically generated
 * configurations.
 *
 * @n WDS GUI Version: 3.2.5.0
 * @n Device: Si4463 Rev.: B1                                 
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2013 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_2nd_CONFIG_H_
#define RADIO_2nd_CONFIG_H_

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
// Crys_freq(Hz): 30000000    Crys_tol(ppm): 20    IF_mode: 2    High_perf_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 1    ANT_DIV: 0    PM_pattern: 0    
// MOD_type: 3    Rsymb(sps): 2500    Fdev(Hz): 3000    RXBW(Hz): 12000    Manchester: 0    AFC_en: 1    Rsymb_error: 0.0    Chip-Version: 2    
// RF_2nd Freq.(MHz): 490    API_TC: 29    fhst: 25000    inputBW: 1    BERT: 0    RAW_dout: 0    D_source: 0    Hi_pfm_div: 1    
// 
// # WB filter 2 (BW =  12.88 kHz);  NB-filter 5 (BW = 9.28 kHz)  // 
// Modulation index: 2
*/


// CONFIGURATION PARAMETERS
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_XO_FREQ                     {30000000L}
#define RADIO_2nd_CONFIGURATION_DATA_CHANNEL_NUMBER                    {0x00}
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PACKET_LENGTH               {0x4F}
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_STATE_AFTER_POWER_UP        {0x03}
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_DELAY_CNT_AFTER_RESET       {0xF000}

#if 1
const unsigned char RADIO_2nd_CONFIGURATION_DATA_ARRAY[198] = { 
        0x06, 0x11, 0x01, 0x02, 0x00, 0x01, 0x2A,/*\RF_INT_CTL_ENABLE_2, \*/
        0x0D, 0x11, 0x10, 0x09, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x00, 0x00, 0x00, 0x00,/*\RF_PREAMBLE_TX_LENGTH_9, \*/
        0x09, 0x11, 0x11, 0x05, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,/*\RF_SYNC_CONFIG_5, \*/
        0x10, 0x11, 0x12, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x4F, 0x06, 0x80, 0x00, 0x00, 0x00,/*\RF_PKT_LEN_12, \*/
        0x10, 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x01, 0x86, 0xA0, 0x05, 0xC9, 0xC3, 0x80, 0x00, 0x00,/*\RF_MODEM_MOD_TYPE_12, \*/
        0x05, 0x11, 0x20, 0x01, 0x0C, 0xD2,/*\RF_MODEM_FREQ_DEV_0_1, \*/
        0x0C, 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0xB0, 0x20,/*\RF_MODEM_TX_RAMP_DELAY_8, \*/
        0x0D, 0x11, 0x20, 0x09, 0x22, 0x00, 0xBC, 0x02, 0xBB, 0x0D, 0x01, 0x5D, 0x02, 0xC2,/*\RF_MODEM_BCR_OSR_1_9, \*/
        0x0B, 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0xC0, 0x07, 0x0A, 0x74, 0xC0,/*\RF_MODEM_AFC_GEAR_7, \*/
        0x0D, 0x11, 0x20, 0x09, 0x38, 0x11, 0x29, 0x29, 0x00, 0x1A, 0x99, 0x9A, 0x00, 0x29,/*\RF_MODEM_AGC_WINDOW_SIZE_9, \*/
        0x0F, 0x11, 0x20, 0x0B, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x00, 0xD0, 0x01, 0x80, 0xFF, 0x0C, 0x02,/*\RF_MODEM_OOK_CNT1_11, \*/
        0x05, 0x11, 0x20, 0x01, 0x4E, 0x40,/*\RF_MODEM_RSSI_COMP_1, \*/
        0x10, 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C,/*\RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \*/
        0x10, 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B,/*\RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \*/
        0x10, 0x11, 0x21, 0x0C, 0x18, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00, 0x03, 0x03, 0x15, 0xF0, 0x3F, 0x00,/*\RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \*/
        0x00 
 };

const unsigned char RADIO_2nd_CONFIGURATION_DATA_ARRAY_DIFF_1ST[144] = { 
        0x06, 0x11, 0x01, 0x02, 0x00, 0x01, 0x2A,/*\RF_INT_CTL_ENABLE_2, \*/
        0x09, 0x11, 0x10, 0x05, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x11, /*\RF_PREAMBLE_TX_LENGTH_9, \*/
        0x07, 0x11, 0x11, 0x03, 0x00, 0x80, 0x00, 0x00, /*\RF_SYNC_CONFIG_5, \*/
        0x07, 0x11, 0x12, 0x03, 0x0e, 0x4F, 0x06, 0x80, /*\RF_PKT_LEN_12, \*/
        0x07, 0x11, 0x20, 0x03, 0x03, 0x01, 0x86, 0xA0, /*\RF_MODEM_MOD_TYPE_12, \*/
        0x05, 0x11, 0x20, 0x01, 0x0C, 0xD2,/*\RF_MODEM_FREQ_DEV_0_1, \*/
        0x06, 0x11, 0x20, 0x02, 0x1E, 0xB0, 0x20,/*\RF_MODEM_TX_RAMP_DELAY_8, \*/
        0x0B, 0x11, 0x20, 0x07, 0x22, 0x00, 0xBC, 0x02, 0xBB, 0x0D, 0x01, 0x5D,/*\RF_MODEM_BCR_OSR_1_9, \*/
        0x07, 0x11, 0x20, 0x03, 0x2F, 0x07, 0x0A, 0x74, /*\RF_MODEM_AFC_GEAR_7, \*/
        0x0c, 0x11, 0x20, 0x08, 0x39, 0x29, 0x29, 0x00, 0x1A, 0x99, 0x9A, 0x00, 0x29,/*\RF_MODEM_AGC_WINDOW_SIZE_9, \*/
        0x05, 0x11, 0x20, 0x01, 0x4E, 0x40,/*\RF_MODEM_RSSI_COMP_1, \*/
        0x10, 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C,/*\RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \*/
        0x0F, 0x11, 0x21, 0x0B, 0x0D, 0x00, 0x15, 0xFF, 0x00, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B,/*\RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \*/
        0x10, 0x11, 0x21, 0x0C, 0x18, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00, 0x03, 0x03, 0x15, 0xF0, 0x3F, 0x00,/*\RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \*/
        0x00 
 };

#endif 

#if 0 
const unsigned char RADIO_2nd_CONFIGURATION_DATA_ARRAY[324] = { 
        0x07, 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80,/*\RF_POWER_UP, \*/
        0x08, 0x13, 0x21, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,/*\RF_GPIO_PIN_CFG, \*/
        0x05, 0x11, 0x00, 0x01, 0x00, GLOBAL_XO_TUNE/*0x46*/,/*\RF_GLOBAL_XO_TUNE_1, \*/
        0x05, 0x11, 0x00, 0x01, 0x03, 0x60,/*\RF_GLOBAL_CONFIG_1, \*/
        0x06, 0x11, 0x01, 0x02, 0x00, 0x01, 0x2A,/*\RF_INT_CTL_ENABLE_2, \*/
        0x08, 0x11, 0x02, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00,/*\RF_FRR_CTL_A_MODE_4, \*/
        0x0D, 0x11, 0x10, 0x09, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x00, 0x00, 0x00, 0x00,/*\RF_PREAMBLE_TX_LENGTH_9, \*/
        0x09, 0x11, 0x11, 0x05, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,/*\RF_SYNC_CONFIG_5, \*/
        0x05, 0x11, 0x12, 0x01, 0x00, 0x80,/*\RF_PKT_CRC_CONFIG_1, \*/
        0x08, 0x11, 0x12, 0x04, 0x03, 0xFF, 0xFF, 0x00, 0x02,/*\RF_PKT_WHT_SEED_15_8_4, \*/
        0x10, 0x11, 0x12, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x4F, 0x06, 0x80, 0x00, 0x00, 0x00,/*\RF_PKT_LEN_12, \*/
        0x10, 0x11, 0x12, 0x0C, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/*\RF_PKT_FIELD_2_CRC_CONFIG_12, \*/
        0x05, 0x11, 0x12, 0x01, 0x20, 0x00,/*\RF_PKT_FIELD_5_CRC_CONFIG_1, \*/
        0x10, 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x01, 0x86, 0xA0, 0x05, 0xC9, 0xC3, 0x80, 0x00, 0x00,/*\RF_MODEM_MOD_TYPE_12, \*/
        0x05, 0x11, 0x20, 0x01, 0x0C, 0xD2,/*\RF_MODEM_FREQ_DEV_0_1, \*/
        0x0C, 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0xB0, 0x20,/*\RF_MODEM_TX_RAMP_DELAY_8, \*/
        0x0D, 0x11, 0x20, 0x09, 0x22, 0x00, 0xBC, 0x02, 0xBB, 0x0D, 0x01, 0x5D, 0x02, 0xC2,/*\RF_MODEM_BCR_OSR_1_9, \*/
        0x0B, 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0xC0, 0x07, 0x0A, 0x74, 0xC0,/*\RF_MODEM_AFC_GEAR_7, \*/
        0x05, 0x11, 0x20, 0x01, 0x35, 0xE2,/*\RF_MODEM_AGC_CONTROL_1, \*/
        0x0D, 0x11, 0x20, 0x09, 0x38, 0x11, 0x29, 0x29, 0x00, 0x1A, 0x99, 0x9A, 0x00, 0x29,/*\RF_MODEM_AGC_WINDOW_SIZE_9, \*/
        0x0F, 0x11, 0x20, 0x0B, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x00, 0xD0, 0x01, 0x80, 0xFF, 0x0C, 0x02,/*\RF_MODEM_OOK_CNT1_11, \*/
        0x05, 0x11, 0x20, 0x01, 0x4E, 0x40,/*\RF_MODEM_RSSI_COMP_1, \*/
        0x05, 0x11, 0x20, 0x01, 0x51, 0x0A,/*\RF_MODEM_CLKGEN_BAND_1, \*/
        0x10, 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C,/*\RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \*/
        0x10, 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B,/*\RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \*/
        0x10, 0x11, 0x21, 0x0C, 0x18, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00, 0x03, 0x03, 0x15, 0xF0, 0x3F, 0x00,/*\RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \*/
	    0x08, 0x11, 0x22, 0x04, 0x00, 0x08, GLOBAL_PASET, 0x00, 0x3D,/*\RF_PA_MODE_4, \*/
        0x0B, 0x11, 0x23, 0x07, 0x00, 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03,/*\RF_SYNTH_PFDCP_CPFF_7, \*/
        0x10, 0x11, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/*\RF_MATCH_VALUE_1_12, \*/
        0x00 
 };
#endif

// DEFAULT VALUES FOR CONFIGURATION PARAMETERS
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_XO_FREQ_DEFAULT                     30000000L
#define RADIO_2nd_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT                    0x00
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PACKET_LENGTH_DEFAULT               0x10
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_STATE_AFTER_POWER_UP_DEFAULT        0x01
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_DELAY_CNT_AFTER_RESET_DEFAULT       0x1000

#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PATCH_INCLUDED                      0x00
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PATCH_SIZE                          0x00
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PATCH                               {  }

#ifndef RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_XO_FREQ
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_XO_FREQ         { RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_XO_FREQ_DEFAULT }
#endif

#ifndef RADIO_2nd_CONFIGURATION_DATA_CHANNEL_NUMBER
#define RADIO_2nd_CONFIGURATION_DATA_CHANNEL_NUMBER        { RADIO_2nd_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT }
#endif

#ifndef RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PACKET_LENGTH
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PACKET_LENGTH   { RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PACKET_LENGTH_DEFAULT }
#endif

#ifndef RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_STATE_AFTER_POWER_UP
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_STATE_AFTER_POWER_UP  { RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_STATE_AFTER_POWER_UP_DEFAULT }
#endif

#ifndef RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_DELAY_CNT_AFTER_RESET
#define RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_DELAY_CNT_AFTER_RESET { RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_DELAY_CNT_AFTER_RESET_DEFAULT }
#endif

#define RADIO_2nd_CONFIGURATION_DATA { \
                            RADIO_2nd_CONFIGURATION_DATA_CHANNEL_NUMBER,                   \
                            RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_PACKET_LENGTH,              \
                            RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_STATE_AFTER_POWER_UP,       \
                            RADIO_2nd_CONFIGURATION_DATA_RADIO_2nd_DELAY_CNT_AFTER_RESET       \
                            }

#endif /* RADIO_2nd_CONFIG_H_ */
