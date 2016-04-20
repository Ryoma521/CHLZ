/***************************************************************************
*                      ZBOSS ZigBee Pro 2007 stack                         *
*                                                                          *
*          Copyright (c) 2012 DSR Corporation Denver CO, USA.              *
*                       http://www.dsr-wireless.com                        *
*                                                                          *
*                            All rights reserved.                          *
*          Copyright (c) 2011 ClarIDy Solutions, Inc., Taipei, Taiwan.     *
*                       http://www.claridy.com/                            *
*                                                                          *
*          Copyright (c) 2011 Uniband Electronic Corporation (UBEC),       *
*                             Hsinchu, Taiwan.                             *
*                       http://www.ubec.com.tw/                            *
*                                                                          *
*          Copyright (c) 2011 DSR Corporation Denver CO, USA.              *
*                       http://www.dsr-wireless.com                        *
*                                                                          *
*                            All rights reserved.                          *
*                                                                          *
*                                                                          *
* ZigBee Pro 2007 stack, also known as ZBOSS (R) ZB stack is available     *
* under either the terms of the Commercial License or the GNU General      *
* Public License version 2.0.  As a recipient of ZigBee Pro 2007 stack, you*
* may choose which license to receive this code under (except as noted in  *
* per-module LICENSE files).                                               *
*                                                                          *
* ZBOSS is a registered trademark of DSR Corporation AKA Data Storage      *
* Research LLC.                                                            *
*                                                                          *
* GNU General Public License Usage                                         *
* This file may be used under the terms of the GNU General Public License  *
* version 2.0 as published by the Free Software Foundation and appearing   *
* in the file LICENSE.GPL included in the packaging of this file.  Please  *
* review the following information to ensure the GNU General Public        *
* License version 2.0 requirements will be met:                            *
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html.                   *
*                                                                          *
* Commercial Usage                                                         *
* Licensees holding valid ClarIDy/UBEC/DSR Commercial licenses may use     *
* this file in accordance with the ClarIDy/UBEC/DSR Commercial License     *
* Agreement provided with the Software or, alternatively, in accordance    *
* with the terms contained in a written agreement between you and          *
* ClarIDy/UBEC/DSR.                                                        *
*                                                                          *
****************************************************************************
PURPOSE: Test for ZC application written using ZDO.
*/


#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"


#include "main.h" //Ryoma
#include "misc.h" //Ryoma
#include "stm32f2xx_rcc.h" //Ryoma

#include "uart.h" //Ryoma
#include "debug.h" //Ryoma
   
#include "radio_mcu.h" //Ryoma
#include "radio.h" //Ryoma
#include "macro.h" //Ryoma
#include "si4463.h" //Ryoma
#include "si4463_def.h" //Ryoma
#include "radio_config.h" //Ryoma
#include "radio_comm.h" //Ryoma
#include "node.h"  //Ryoma
#include "common.h" //Ryoma

/* Private function prototypes -----------------------------------------------Ryoma*/


extern SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8);

extern SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration);

extern SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration) ;

extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

extern SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], U8);
 
/* Private functions ---------------------------------------------------------Ryoma*/



#ifndef ZB_ED_ROLE
#error define ZB_ED_ROLE to compile ze tests
#endif
/*! \addtogroup ZB_TESTS */
/*! @{ */

#define ZB_TEST_DUMMY_DATA_SIZE 10

static void send_data(zb_uint8_t param) ZB_CALLBACK;
void data_indication(zb_uint8_t param) ZB_CALLBACK;



/*
  ZE joins to ZC(ZR), then sends APS packet.
*/


MAIN()
{
  
  
   /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */  

  /* SysTick end of count event each 1ms ：1/100=10ms,1/1000=1ms*/
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  
    /* Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
    
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
  
   if (SysTick_Config(SystemCoreClock / 1000))
  { 
  }
   */
   SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);//设置sysTick中断、systick定时器、优先权。我们用的是1ms
  
   NVIC_Config();
  
   /*uart config */
   Uart_Config(); 
   
   //Show node info
   UartShowNodeInfo();
   
   STM_EVAL_LEDInit(LED1);      
   STM_EVAL_LEDOn(LED1);
   STM_EVAL_LEDOff(LED1);
 
   STM_EVAL_LEDInit(LED2);
   STM_EVAL_LEDOn(LED2);
   STM_EVAL_LEDOff(LED2);
   
   Init_SI4463_Pin();
  
#ifdef SI4463TX 
    RF_Switch_TX();
#endif
  
#ifdef SI4463RX 
    RF_Switch_RX();
    while(1);
#endif
  
   vRadio_Init();     
   
// //test
// U8 packet_length_1st=63;
// U8* data_1st=(U8*)malloc(sizeof(U8)*(packet_length_1st+1));
// 
// data_1st[0]=packet_length_1st;
// for(int xi=1;xi<=packet_length_1st;xi++)
// {
//  data_1st[xi]=xi;
// }
// 
// while(1)
// {
//  vRadio_StartTx_Variable_Packet(0u,data_1st,packet_length_1st+1);
//  Delay(0x000006FF); 
//  ArrayReset(RxBuffer,RXBUFFERSIZE);
//  RxBuffer[0]=10;
//  vRadio_StartTx_Variable_Packet(0u,RxBuffer,10);
//  Delay(0x000006FF);
//  si446x_fifo_info(0u);
// }

   
   //Go into RX mode
   vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber); 
  
  ARGV_UNUSED;

#if !(defined KEIL || defined SDCC || defined ZB_IAR )
  if ( argc < 3 )
  {
    printf("%s <read pipe path> <write pipe path>\n", argv[0]);
    return 0;
  }
#endif

  /* Init device, load IB values from nvram or set it to default */
#ifndef ZB8051
  ZB_INIT("zdo_ze", argv[1], argv[2]);
#else
  ZB_INIT((char*)"zdo_ze", (char*)"3", (char*)"3");
#endif
#ifdef ZB_SECURITY
  ZG->nwk.nib.security_level = 0;
#endif
  
  ZB_PIB_RX_ON_WHEN_IDLE() = ZB_TRUE;

  if (zdo_dev_start() != RET_OK)
  {
    TRACE_MSG(TRACE_ERROR, "zdo_dev_start failed", (FMT__0));
  }
  else
  {
    zdo_main_loop();
  }

  TRACE_DEINIT();

  MAIN_RETURN(0);
}


void zb_zdo_startup_complete(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  if (buf->u.hdr.status == 0)
  {
    TRACE_MSG(TRACE_APS1, "Device STARTED OK", (FMT__0));
    zb_af_set_data_indication(data_indication);
    
    ZB_SCHEDULE_ALARM(send_data, param, 195);
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Device started FAILED status %d", (FMT__D, (int)buf->u.hdr.status));
    zb_free_buf(buf);
  }
}


void send_data(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_apsde_data_req_t *req = ZB_GET_BUF_TAIL(buf, sizeof(zb_apsde_data_req_t));
  zb_uint8_t *ptr = NULL;
  zb_short_t i;

  req->dst_addr.addr_short = 0; /* send to ZC */
  req->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
  req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
  req->radius = 1;
  req->profileid = 2;
  req->src_endpoint = 10;
  req->dst_endpoint = 10;

  buf->u.hdr.handle = 0x11;
  ZB_BUF_INITIAL_ALLOC(buf, ZB_TEST_DATA_SIZE, ptr);

  for (i = 0 ; i < ZB_TEST_DATA_SIZE ; ++i)
  {
    ptr[i] = i % 32 + '0';
  }
  TRACE_MSG(TRACE_APS2, "Sending apsde_data.request", (FMT__0));

  ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, ZB_REF_FROM_BUF(buf));
}


void data_indication(zb_uint8_t param)
{
  zb_uint8_t *ptr;
  zb_buf_t *asdu = (zb_buf_t *)ZB_BUF_FROM_REF(param);

  /* Remove APS header from the packet */
  ZB_APS_HDR_CUT_P(asdu, ptr);

  TRACE_MSG(TRACE_APS2, "data_indication: packet %p len %d handle 0x%x", (FMT__P_D_D,
                         asdu, (int)ZB_BUF_LEN(asdu), asdu->u.hdr.status));
  
  ZB_SCHEDULE_ALARM(send_data, param, 195);
}




/*! @} */
