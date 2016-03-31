#ifndef RADIO_MCU_H
#define RADIO_MCU_H

#include "stm32l1xx_gpio.h"
#include "hal_types.h"
#include "stm32l1xx_syscfg.h"

#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_5
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOB  
  
#define LED2_PIN                         GPIO_Pin_6
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOB 

#define LED3_PIN                         GPIO_Pin_9
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOC  
  
#define LED4_PIN                         GPIO_Pin_7
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOC


#define PORT_SDN GPIOA
#define PIN_SDN  GPIO_Pin_4

#define PORT_SCLK GPIOB
#define PIN_SCLK  GPIO_Pin_0

#define PORT_SDO GPIOC
#define PIN_SDO  GPIO_Pin_5

#define PORT_SDI GPIOC
#define PIN_SDI  GPIO_Pin_4

#define PORT_NIRQ GPIOB
#define PIN_NIRQ  GPIO_Pin_1

#define PORT_GDO2 GPIOA
#define PIN_GDO2  GPIO_Pin_6

#define PORT_GDO0 GPIOB
#define PIN_GDO0  GPIO_Pin_11

#define PORT_NSEL  GPIOA
#define PIN_NSEL   GPIO_Pin_7

#define PORT_ANTSW1  GPIOC
#define PIN_ANTSW1   GPIO_Pin_7

#define PORT_ANTSW2  GPIOC
#define PIN_ANTSW2   GPIO_Pin_6


#define NIRQ_LINE       EXTI_Line1
#define NIRQ_PortSource EXTI_PortSourceGPIOB
#define NIRQ_PinSource  EXTI_PinSource1
#define NIRQ_IRQChannel EXTI1_IRQn

#define GDO2_LINE       EXTI_Line6
#define GDO2_PortSource EXTI_PortSourceGPIOA
#define GDO2_PinSource  EXTI_PinSource6
#define GDO2_IRQChannel EXTI9_5_IRQn


void SI4463_SDN_UP(void);
void SI4463_SDN_DOWN(void);
void SI4463_SCLK_Up(void);
void SI4463_SCLK_Down(void);
void SI4463_SDI_Up(void);
void SI4463_SDI_Down(void);
uint8 SI4463_SDO_Bit(void);
uint8 SI4463_NIRQ_Bit(void);
uint8 SI4463_GDO0_Bit(void);
void SI4463_NSEL_Up(void);
void SI4463_NSEL_Down(void);
void Init_SI4463_Pin(void);
void SI4463_Disable_NIRQ_Int(void);
void SI4463_SW1_UP(void);
void SI4463_SW1_DOWN(void);
void SI4463_SW2_UP(void);
void SI4463_SW2_DOWN(void);
void RF_Switch_TX();
void SI4463_LED1_ON(void);
void SI4463_LED1_OFF(void);
void SI4463_LED2_ON(void);
void SI4463_LED2_OFF(void);
void SI4463_Enable_NIRQ_Int(void);
void RF_Switch_SW1();
void RF_Switch_SW2();
void RF_Switch_RX(void);
void EXTILineNIRQ_Config(void);

void SI4463_Enable_NIRQ_TX();
void SI4463_Enable_NIRQ_RX();
void Toggle_LED1(void);
#endif
