/**
  ******************************************************************************
  * @file    switch.c
  * @author  daishengchen
  * @version V0.1
  * @date    15-oct-2013
  * @brief   This file provide the configuration switch.
  *通过stm32对ant switch控制。主要控制
  *antsw2----------------PC7-------v2
  *antsw1-----------------PC6------v1

  *****************************************************************************
*   V2V1 
*   0 0 Shutdown
*   1 0 RFC to J2
*   0 1 RFC to J1
*   1 1 RFC to J3
  ******************************************************************************
  */


#include "switch.h"

void switch_Config(void)
{
 GPIO_InitTypeDef GPIO_InitStruct;
  
  
   /* Enable GPIOC clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  ;
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 
  /*PC7,ANTSW2*/
  GPIO_InitStruct.GPIO_Pin = PIN_ANTSW2;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 // GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(PORT_ANTSW2, &GPIO_InitStruct);

    /*PC6,ANTSW1*/
  GPIO_InitStruct.GPIO_Pin = PIN_ANTSW1;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 // GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(PORT_ANTSW1, &GPIO_InitStruct);
}

void switch_mid_ant(void)//J3
{
switch_Config();

GPIO_ResetBits(PORT_ANTSW2, PIN_ANTSW2);
GPIO_ResetBits(PORT_ANTSW1, PIN_ANTSW1);

//1,1 to J3

GPIO_SetBits(PORT_ANTSW2, PIN_ANTSW2);
GPIO_SetBits(PORT_ANTSW1, PIN_ANTSW1);

}
