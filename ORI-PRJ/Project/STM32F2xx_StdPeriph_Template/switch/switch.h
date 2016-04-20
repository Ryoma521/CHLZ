/**
  ******************************************************************************
  * @file    switch.h
  * @author  daishengchen
  * @version V0.1
  * @date    15-oct-2013
  * @brief   This file provide the configuration switch.
  *通过stm32对ant switch控制。主要控制
  *antsw2----------------PC7
  *antsw1----------------PC6

  ******************************************************************************
  * 
  *
  ******************************************************************************
  */

#ifndef __SWITCH_H
#define __SWITCH_H

#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>


#define PORT_ANTSW2  GPIOC
#define PIN_ANTSW2  GPIO_Pin_6

#define PORT_ANTSW1 GPIOC
#define PIN_ANTSW1  GPIO_Pin_7





void switch_Config(void);

void switch_mid_ant(void);



#endif