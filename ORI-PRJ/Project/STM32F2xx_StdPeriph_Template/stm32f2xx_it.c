/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f2xx_it.h"
#include "main.h"


#include "hal_timer.h"
#include "kernel_timers.h"



#include "radio.h"
#include "macro.h"
#include "si4463.h"
#include "si4463_def.h"
#include "radio_config.h"
#include "radio_config_1st.h"
#include "radio_config_2nd.h"
#include "radio_comm.h"
#include "radio_hal.h"
#include "radio_mcu.h"
#include "uart.h"
#include "node.h"
#include "common.h"    

// 用于GDO0的测试。


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

 /* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx_IRQHANDLER   USART1_IRQHandler

#define TXBUFFERSIZE   (countof(TxBuffer) - 1)


/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";
uint8_t RxBuffer[RXBUFFERSIZE];
uint8_t NbrOfDataToTransfer = TXBUFFERSIZE;
uint8_t NbrOfDataToRead = RXBUFFERSIZE;
__IO uint8_t TxCounter = 0; 
__IO uint16_t RxCounter = 0; 
    
  uint8_t FrequencyConfig[2];  
    __IO uint16_t FeqRegCnt = 0; 
    
extern FrameInfo FrameBuffToUart,FrameBuffToRf;
extern AckInfo AckBuff;
extern enum UART_RX_FLAG UartRxFlag;
    
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
uint32 tmpGlobalTime = 0;
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
  tmpGlobalTime++;
}

/******************************************************************************/
/*            STM32F2xx Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTx global interrupt request.
  * @param  None
  * @retval None
  */
void USARTx_IRQHANDLER(void)
{
  if(USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
  {
    RxBuffer[RxCounter++] = (USART_ReceiveData(EVAL_COM1) & 0x7F);
    if(RxCounter==RxBuffer[0]+1 && DataCheck(RxBuffer))
    {
      RxBuff2FrameBuff(RxBuffer,&FrameBuffToRf);
      UartRxFlag=UartRxData;
      //Record the current FrameID
      AckBuff.CurrentFrameID=FrameBuffToRf.FrameID;
      RxCounter=0;
    }
    
    if(RxCounter>RxBuffer[0]+1)
    {
      RxCounter=0;
      UartRxFlag=UartNoData;
    }
    
   }
    //USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
}


//NIRQ
uint8 trx_state =1;//0:tx,1:rx
uint8 PKT_Sent_Flag=0;
extern SEGMENT_VARIABLE( Si446xCmd, union si446x_cmd_reply_union);

void EXTI1_IRQHandler(void)
{
  
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    SI4463_Disable_NIRQ_Int();
 
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearFlag(EXTI_Line1);
    EXTI_ClearITPendingBit(EXTI_Line1);
    
    if (trx_state == 0)//工作在发送模式
    {
     /* Read ITs, clear pending ones */
     si446x_get_int_status(0u, 0u, 0u);
    
     /* check the reason for the IT */
     if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)//未发送成功的话需要保护，但是目前还没写
     {
      PKT_Sent_Flag = 1;
      }
    }
    
    
    if (trx_state == 1)//工作在接收模式
    {
     DemoApp_Pollhandler_RX();
     }
    
    SI4463_Enable_NIRQ_Int();
  }
  //cont++;
  
}



//GDO0
void EXTI4_IRQHandler(void)
{
  
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
  {
    /* Toggle LED1 */
   STM_EVAL_LEDOn(LED1);
   STM_EVAL_LEDOff(LED1);
    
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearFlag(EXTI_Line4);
    EXTI_ClearITPendingBit(EXTI_Line4);
    
  }
  //cont++;
  
}

//GDO2
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line15) != RESET)
  {
    /* Toggle LED2 */
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOff(LED2);
    
        EXTI_ClearFlag(EXTI_Line15);
        EXTI_ClearITPendingBit(EXTI_Line15);                       //本函数与上一句功能相同
             
        //while(1);
    //   RXHandlerInGDO0();
        
  }
  //cont1++;
  
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
