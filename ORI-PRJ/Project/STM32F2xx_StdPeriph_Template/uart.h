/**
  ******************************************************************************
  * @file    uart.h
  * @author  daishengchen
  * @version V0.1
  * @date    15-oct-2013
  * @brief   This file provide the configuration of your board.
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __UART_H
#define __UART_H

#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "node.h"


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



extern uint8_t NbrOfDataToTransfer;
extern uint8_t NbrOfDataToRead;
extern __IO uint8_t TxCounter; 
extern __IO uint16_t RxCounter;

enum UART_RX_FLAG
{
  UartNoData,
  UartRxData
};

void Uart_Config(void);
void UartSendByte(u8 *buf, u32 len);
void Uart_App(void);
void UartShowString(char str[]);



#endif