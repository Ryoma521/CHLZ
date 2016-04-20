/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "debug.h"
   
#include "radio_mcu.h"
#include "radio.h"
#include "macro.h"
#include "si4463.h"
#include "si4463_def.h"
#include "radio_config.h"
#include "radio_comm.h"
#include "node.h" 
#include "common.h"



/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "     STM32F2xx      " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "   STM322xG-EVAL    " 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static __IO uint32_t TimingDelay;
static void SysTickConfig(void);
FrameInfo FrameBuffToUart,FrameBuffToRf;
AckInfo AckBuff={0x00,0x0000};
extern enum UART_RX_FLAG UartRxFlag;
extern enum RF_RX_FLAG RfRxFlag;
extern uint8_t RxBuffer[];

/* Private function prototypes -----------------------------------------------*/


extern SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8);

extern SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration);

extern SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration) ;

extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

extern SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], U8);
 
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
//int main(void)
//{
//  
//
//  /*!< At this stage the microcontroller clock setting is already configured, 
//       this is done through SystemInit() function which is called from startup
//       file (startup_stm32f2xx.s) before to branch to application main.
//       To reconfigure the default setting of SystemInit() function, refer to
//       system_stm32f2xx.c file
//     */  
//
//  /* SysTick end of count event each 1ms ：1/100=10ms,1/1000=1ms*/
//  RCC_ClocksTypeDef RCC_Clocks;
//  RCC_GetClocksFreq(&RCC_Clocks);
//  
//    /* Setup SysTick Timer for 1 msec interrupts.
//     ------------------------------------------
//    1. The SysTick_Config() function is a CMSIS function which configure:
//       - The SysTick Reload register with value passed as function parameter.
//       - Configure the SysTick IRQ priority to the lowest value (0x0F).
//       - Reset the SysTick Counter register.
//       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
//       - Enable the SysTick Interrupt.
//       - Start the SysTick Counter.
//    
//    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
//       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
//       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
//       inside the misc.c file.
//
//    3. You can change the SysTick IRQ priority by calling the
//       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
//       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.
//
//    4. To adjust the SysTick time base, use the following formula:
//                            
//         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
//    
//       - Reload Value is the parameter to be passed for SysTick_Config() function
//       - Reload Value should not exceed 0xFFFFFF
//  
//   if (SysTick_Config(SystemCoreClock / 1000))
//  { 
//  }
//   */
//   SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);//设置sysTick中断、systick定时器、优先权。我们用的是1ms
//  
//   NVIC_Config();
//  
//   /*uart config */
//   Uart_Config(); 
//   
//   //Show node info
//   UartShowNodeInfo();
//   
//   STM_EVAL_LEDInit(LED1);      
//   STM_EVAL_LEDOn(LED1);
//   STM_EVAL_LEDOff(LED1);
// 
//   STM_EVAL_LEDInit(LED2);
//   STM_EVAL_LEDOn(LED2);
//   STM_EVAL_LEDOff(LED2);
//   
//   Init_SI4463_Pin();
//  
//#ifdef SI4463TX 
//    RF_Switch_TX();
//#endif
//  
//#ifdef SI4463RX 
//    RF_Switch_RX();
//    while(1);
//#endif
//  
//   vRadio_Init();     
//   
//// //test
//// U8 packet_length_1st=63;
//// U8* data_1st=(U8*)malloc(sizeof(U8)*(packet_length_1st+1));
//// 
//// data_1st[0]=packet_length_1st;
//// for(int xi=1;xi<=packet_length_1st;xi++)
//// {
////  data_1st[xi]=xi;
//// }
//// 
//// while(1)
//// {
////  vRadio_StartTx_Variable_Packet(0u,data_1st,packet_length_1st+1);
////  Delay(0x000006FF); 
////  ArrayReset(RxBuffer,RXBUFFERSIZE);
////  RxBuffer[0]=10;
////  vRadio_StartTx_Variable_Packet(0u,RxBuffer,10);
////  Delay(0x000006FF);
////  si446x_fifo_info(0u);
//// }
//
//   
//   //Go into RX mode
//   vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber);     
//   
//   //main loop
//   while(1)
//   { 
//    if(AckBuff.AckStatus==0x00) //Ack is ok
//    {
//      if(UartRxFlag==UartNoData) //Uart no data
//      {
//        if(RfRxFlag==RfRxData) //Rf receive data
//        {
//         //RF into RDY mode
//         
//          
//         //Uart send the data from RF
//          //UartShowString("RF received data!\r\n"); 
//          UartSendByte(RxBuffer, (u32)FrameBuffToUart.FrameLen+1);
//          
//          //RxBuffer reset
//          ArrayReset(RxBuffer,RXBUFFERSIZE);
//          
//          //Clear rf rx flag
//          RfRxFlag=RfNoData;
//          
//          //Go into RX mode
//          vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber);    
//        }
//        else
//        {
//          //Do nothing
//        }
//        
//      }
//      else //Uart rx right data
//      {
//        //Send uart data with RF
//        vRadio_StartTx_Variable_Packet(0u,RxBuffer,FrameBuffToRf.FrameLen+1);
//        Delay(0x000006FF);
//        //RxBuffer reset
//        ArrayReset(RxBuffer,RXBUFFERSIZE);
//        
//        //Show process infomation        
//        UartShowString("UartData has been transmitted by RF!\r\n"); 
//        
//        //Clear uart rx flag
//        UartRxFlag=UartNoData;
//        
//        //Start ACK timer
//        
//        //Go into RX mode
//        vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber);    
//      }
//    }
//    else //Ack is timeout
//    {
//      //Uart show the timeout info
//      UartShowAckTimeout();
//      
//      //Clear the ACK timeout flag
//      AckBuff.AckStatus=0x00;
//      
//      
//    }
//    
//    }
//   
//   
//   
//   
//   
// #ifdef SI4463RX 
//  vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber); 
// #endif 
//  
//#ifdef SI4463TX  
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
//  Delay(0x00000FFF);
//  si446x_fifo_info(0u);
// }  
//#endif 
//
//   
//}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief  Configures the SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }
  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


void LEDApp(void)
{
  
  u8 i=0;
/* Initialize LEDs and LCD available on STM322xG-EVAL board *****************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOff(LED1);
 
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOff(LED2);

  /* Add your application code here
     */

  /* Infinite loop */
  while (i<10)
  {
    /* Toggle LD4 */
    STM_EVAL_LEDToggle(LED1);
    
   // UartSendByte(&a,1);
      
    /* Insert 50 ms delay */
    Delay(5);

    /* Toggle LD2 */
    STM_EVAL_LEDToggle(LED2);

    /* Insert 50 ms delay */
    Delay(5);
    i++;
  }
}
/**
  * @}
  */
void UartShowNodeInfo()
{  
  
  UartShowString("NodeNwkID=");
  char str[10];
  int strlenth=sprintf(str,"%04x",NODE_NWK_ID); 
  UartSendByte(str,strlenth);  
  UartShowString("\r\n");
  
  UartShowString("NodeNwkAddr=");
  strlenth=sprintf(str,"%04x",NODE_NWK_ADDR); 
  UartSendByte(str,strlenth);  
  UartShowString("\r\n");
  
  UartShowString("NodeGroupAddr=");
  strlenth=sprintf(str,"%02x",NODE_GROUP_ADDR); 
  UartSendByte(str,strlenth); 
  UartShowString("\r\n");  
}

void UartShowAckTimeout()
{
 UartShowString("ACK Timeout! FrameID=");
 char str[10];
 int strlenth=sprintf(str,"%04x",AckBuff.CurrentFrameID); 
 UartSendByte(str,strlenth);  
 UartShowString("\r\n"); 
 
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
