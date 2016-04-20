/**
  ******************************************************************************
  * @file    uart.c
  * @author  Tinux
  * @version V0.1
  * @date    12-Nov-2012
  * @brief   This file provide the initilization of board uarts.
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


#include "uart.h"
#include "node.h"

enum UART_RX_FLAG UartRxFlag=UartNoData;
extern U8 PayLoad[52];
extern U8 PayLoadLen;

/**
  * @brief  This function initilizes the leds on the board.
  * @param  None
  * @retval None
  */



// USART配置
void Uart_Config(void)
{
    
     USART_InitTypeDef my_USART_InitStructure; // USART初始化结构
    
    USART_StructInit(&my_USART_InitStructure);
    my_USART_InitStructure.USART_BaudRate = 115200;
    my_USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    my_USART_InitStructure.USART_StopBits = USART_StopBits_1;
    my_USART_InitStructure.USART_Parity = USART_Parity_No;
    my_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    my_USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    STM_EVAL_COMInit(COM1, &my_USART_InitStructure);//io cofig,GPIO
    
    /* Enable the EVAL_COM1 Transmit interrupt: this interrupt is generated when the 
     EVAL_COM1 transmit data register is empty */  
    
   USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);//中断使能。
   
}

// USART发送数据
void UartSendByte(u8 *buf, u32 len)
{
    u32 i2 = 0;
    u16 t2 = 0;
    u8* buff = buf;
    
    while(i2 < len)
    {
        USART_SendData(EVAL_COM1,buff[i2]);
    	/* Loop until the end of transmit */
    	while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
    	{
//            if(t2++>0xfff)
//            {
//                break;
//            }   
    	}
    //    Delay(10);
    	i2 ++;
    }
}

void Uart_App(void)
{
  
  u8 i=0;
  u8 aaa[2];
   aaa[0]=1;
   aaa[1]=2;
   
     /* Output a message on Hyperterminal using printf function */
   printf("\n\Uart_App uart printf\n\r");
   
   if(0)
 {
   while(i<10)
   {
   UartSendByte(aaa,2);
   i++;
   }
  
 /* Output a message on Hyperterminal using printf function */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");

 
    /* Wait until EVAL_COM1 send the TxBuffer */
  while(TxCounter < NbrOfDataToTransfer)
  {}
  
 /* The software must wait until TC=1. The TC flag remains cleared during all data
     transfers and it is set by hardware at the last frame抯 end of transmission*/
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}
 
  /* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the 
     EVAL_COM1 receive data register is not empty */
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
 
  /* Wait until EVAL_COM1 receive the RxBuffer */
  while(RxCounter < NbrOfDataToRead)
 {}
 }
}

void UartShowString(char str[])
{
  UartSendByte(str,strlen(str));
}


PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

