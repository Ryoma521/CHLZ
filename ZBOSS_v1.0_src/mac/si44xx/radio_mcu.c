#include "stm32l1xx_gpio.h"
#include "stm32l1xx_exti.h"
//#include "stm322xg_eval.h"
#include "misc.h"
//#include "stm32f2xx_nvic.h"
//#include "stm32f2xx_lib.h"
#include "radio_mcu.h"

void Config_GPIO2_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  //GPIO_SetBits(GPIOA, GPIO_Pin_6);
  
  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}

void Config_GPIO3_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  //GPIO_SetBits(GPIOA, GPIO_Pin_5);
  
  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}
void Config_SW1_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_ANTSW1;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_ANTSW1, &GPIO_InitStruct);
  
  //GPIO_SetBits(PORT_ANTSW1, PIN_ANTSW1);
  
  GPIO_ResetBits(PORT_ANTSW1, PIN_ANTSW1);
}

void Config_SW2_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_ANTSW2;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_ANTSW2, &GPIO_InitStruct);
  
  //GPIO_SetBits(PORT_ANTSW2, PIN_ANTSW2);
  
  GPIO_ResetBits(PORT_ANTSW2, PIN_ANTSW2);
}

void Config_NSEL_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_NSEL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_NSEL, &GPIO_InitStruct);
  
  GPIO_SetBits(PORT_NSEL, PIN_NSEL);
  
  //GPIO_ResetBits(PORT_NSEL, PIN_NSEL);
}

void Config_CLK_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_SCLK;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//
  GPIO_Init(PORT_SCLK, &GPIO_InitStruct);
  
  //GPIO_SetBits(PORT_SCLK, PIN_SCLK);
  
  GPIO_ResetBits(PORT_SCLK, PIN_SCLK);
}

void Config_SDI_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_SDI;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_SDI, &GPIO_InitStruct);
  
  //GPIO_SetBits(PORT_SDI, PIN_SDI);
  
  GPIO_ResetBits(PORT_SDI, PIN_SDI);
}

void Config_NIRQ_AfterStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_NIRQ;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_NIRQ, &GPIO_InitStruct);
  
  //GPIO_SetBits(PORT_NIRQ, PIN_NIRQ);
  
  GPIO_ResetBits(PORT_NIRQ, PIN_NIRQ);
}

void Config_NIRQ_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_NIRQ;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_NIRQ, &GPIO_InitStruct);
  
  GPIO_SetBits(PORT_NIRQ, PIN_NIRQ);
  
  //GPIO_ResetBits(PORT_NIRQ, PIN_NIRQ);
}

void Config_SDO_BeforeStop(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PC5,SDO*/
  GPIO_InitStruct.GPIO_Pin = PIN_SDO;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//
  GPIO_Init(PORT_SDO, &GPIO_InitStruct);
  
  //GPIO_SetBits(PORT_SDO, PIN_SDO);
  
  GPIO_ResetBits(PORT_SDO, PIN_SDO);
}

void SI4463_EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = NIRQ_LINE;                 //外部中断线
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //中断触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //打开中断
    EXTI_Init(&EXTI_InitStructure);
    SYSCFG_EXTILineConfig(NIRQ_PortSource, NIRQ_PinSource);   //选择 GPIO管脚用作外部中断线路

   
    EXTI_InitStructure.EXTI_Line    = GDO2_LINE;                 //外部中断线
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //中断触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //打开中断
    EXTI_Init(&EXTI_InitStructure);
    SYSCFG_EXTILineConfig(GDO2_PortSource, GDO2_PinSource);   //选择 GPIO管脚用作外部中断线路
    
}

void Change_NIRQ_Falling_Int(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = NIRQ_LINE;                 //外部中断线
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //中断触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //打开中断
    EXTI_Init(&EXTI_InitStructure);
    SYSCFG_EXTILineConfig(NIRQ_PortSource, NIRQ_PinSource);   //选择 GPIO管脚用作外部中断线路
}

void Change_NIRQ_Rising_Int(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = NIRQ_LINE;                 //外部中断线
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;       //中断触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //打开中断
    EXTI_Init(&EXTI_InitStructure);
    SYSCFG_EXTILineConfig(NIRQ_PortSource, NIRQ_PinSource);   //选择 GPIO管脚用作外部中断线路
}


void SI4463_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               // 抢占式优先级别
    NVIC_InitStructure.NVIC_IRQChannel            = NIRQ_IRQChannel; //指定中断源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                        //未使用
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 指定响应优先级别1
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               // 抢占式优先级别
    NVIC_InitStructure.NVIC_IRQChannel            = GDO2_IRQChannel; //指定中断源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;                        //未使用
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;            // 指定响应优先级别1
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SI4463_Enable_NIRQ_Int(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               // 抢占式优先级别
    NVIC_InitStructure.NVIC_IRQChannel            = NIRQ_IRQChannel; //指定中断源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                            //每次接收后的NIRQ使能
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          // 指定响应优先级别1
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SI4463_Disable_NIRQ_Int(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               // 抢占式优先级别
    NVIC_InitStructure.NVIC_IRQChannel            = NIRQ_IRQChannel; //指定中断源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                   //进入接收中断时的禁止NIRQ
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;           // 指定响应优先级别1
    NVIC_InitStructure.NVIC_IRQChannelCmd         = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SI4463_Enable_NIRQ_TX()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_NIRQ;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_IN;//
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_NIRQ, &GPIO_InitStruct);
}

void SI4463_Enable_NIRQ_RX()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure PB1 pin as input floating */
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStruct.GPIO_Pin = PIN_NIRQ;
  GPIO_Init(PORT_NIRQ, &GPIO_InitStruct);
}



/*
  初始化与SI4463相关的引脚
*/
void Init_SI4463_Pin(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
     /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC, ENABLE);
  
  /*PA4,SDN*/
  GPIO_InitStruct.GPIO_Pin = PIN_SDN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_SDN, &GPIO_InitStruct);   
  
  /*PB0,SCLK*/
  GPIO_InitStruct.GPIO_Pin = PIN_SCLK;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_SCLK, &GPIO_InitStruct);
  
  /*PC5,SDO*/
  GPIO_InitStruct.GPIO_Pin = PIN_SDO;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PORT_SDO, &GPIO_InitStruct);
  
  /*PC4,SDI*/
  GPIO_InitStruct.GPIO_Pin = PIN_SDI;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_SDI, &GPIO_InitStruct);
  
  /*PA7,NSEL*/
  GPIO_InitStruct.GPIO_Pin = PIN_NSEL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_NSEL, &GPIO_InitStruct);
  
#if 0  
   /*PB1,NIRQ*/
  GPIO_InitStruct.GPIO_Pin = PIN_NIRQ;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PORT_NIRQ, &GPIO_InitStruct);
#endif
  
    /*PC7,ANT_SW1*/
  GPIO_InitStruct.GPIO_Pin = PIN_ANTSW1;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_ANTSW1, &GPIO_InitStruct);
  
    /*PC6,ANT_SW2*/
  GPIO_InitStruct.GPIO_Pin = PIN_ANTSW2;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_ANTSW2, &GPIO_InitStruct);
  
//  /*PA6,GDO2*/
//  GPIO_InitStruct.GPIO_Pin = PIN_GDO2;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(PORT_GDO2, &GPIO_InitStruct);
//  
//  /*PB11,GDO0*/
//  GPIO_InitStruct.GPIO_Pin = PIN_GDO0;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(PORT_GDO0, &GPIO_InitStruct);
  

  
 // SI4463_EXTI_Configuration();
  //SI4463_NVIC_Configuration();
  
  
  //SI4463_Enable_NIRQ_TX();
  
  EXTILineNIRQ_Config();
  
}

/*************************************************************************
  * @brief  Configures EXTI Line0 (connected to PA0 pin) in interrupt mode
  * @param  None
  * @retval None
*************************************************************************/
void EXTILineNIRQ_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
#ifdef KEEP_GARBAGGE  
  GPIO_InitTypeDef   GPIO_InitStructure;
#endif
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  
  
  SI4463_Enable_NIRQ_RX();
 
  
  /* Connect EXTI Line0 to PB1 pin */
  SYSCFG_EXTILineConfig(NIRQ_PortSource, NIRQ_PinSource);   //选择 GPIO管脚用作外部中断线路
  

  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line    = NIRQ_LINE;                 //外部中断线
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //中断触发方式
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //打开中断
  EXTI_Init(&EXTI_InitStructure);
  

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel            = NIRQ_IRQChannel; //指定中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             //NIRQ的首次初始化
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 指定响应优先级别1
  NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
}



void Disable_EXTILineNIRQ(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;

  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  
  
  //SI4463_Enable_NIRQ_RX();
 
  
  /* Connect EXTI Line0 to PB1 pin */
  SYSCFG_EXTILineConfig(NIRQ_PortSource, NIRQ_PinSource);   //选择 GPIO管脚用作外部中断线路
  

  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line    = NIRQ_LINE;                 //外部中断线
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //中断触发方式
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;                     //打开中断
  EXTI_Init(&EXTI_InitStructure);
  

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel            = NIRQ_IRQChannel; //指定中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             //NIRQ的首次初始化
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 指定响应优先级别1
  NVIC_InitStructure.NVIC_IRQChannelCmd         = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
}



void Enable_EXTILineNIRQ(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;

  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  
  
  SI4463_Enable_NIRQ_RX();
 
  
  /* Connect EXTI Line0 to PB1 pin */
  SYSCFG_EXTILineConfig(NIRQ_PortSource, NIRQ_PinSource);   //选择 GPIO管脚用作外部中断线路
  

  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line    = NIRQ_LINE;                 //外部中断线
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;        //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //中断触发方式
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //打开中断
  EXTI_Init(&EXTI_InitStructure);
  

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel            = NIRQ_IRQChannel; //指定中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;             //NIRQ的首次初始化
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 指定响应优先级别1
  NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
}




void RF_Switch_SW1()
{
  SI4463_SW1_DOWN();
  SI4463_SW2_UP();
}

void RF_Switch_SW2()
{
  SI4463_SW2_DOWN();
  SI4463_SW1_UP();
}

void SI4463_SDN_UP(void)
{
  GPIO_SetBits(PORT_SDN, PIN_SDN);
}

void SI4463_SDN_DOWN(void)
{
  GPIO_ResetBits(PORT_SDN, PIN_SDN);
}

void SI4463_SCLK_Up(void)
{
  GPIO_SetBits(PORT_SCLK, PIN_SCLK);
}

void SI4463_SCLK_Down(void)
{
  GPIO_ResetBits(PORT_SCLK, PIN_SCLK);
}

void SI4463_SDI_Up(void)
{
  GPIO_SetBits(PORT_SDI, PIN_SDI);
}

void SI4463_SDI_Down(void)
{
  GPIO_ResetBits(PORT_SDI, PIN_SDI);
}

uint8 SI4463_SDO_Bit(void)
{
  return (GPIO_ReadInputDataBit(PORT_SDO, PIN_SDO));
}

uint8 SI4463_NIRQ_Bit(void)
{
  return (GPIO_ReadInputDataBit(PORT_NIRQ, PIN_NIRQ));
}

uint8 SI4463_GDO0_Bit(void)
{
  return (GPIO_ReadInputDataBit(PORT_GDO0, PIN_GDO0));
}

void SI4463_NSEL_Up(void)
{
  GPIO_SetBits(PORT_NSEL, PIN_NSEL);
}

void SI4463_NSEL_Down(void)
{
  GPIO_ResetBits(PORT_NSEL, PIN_NSEL);
}

void SI4463_SW1_UP(void)
{
  GPIO_SetBits(PORT_ANTSW1, PIN_ANTSW1);
}

void SI4463_SW1_DOWN(void)
{
  GPIO_ResetBits(PORT_ANTSW1, PIN_ANTSW1);
}

void SI4463_SW2_UP(void)
{
  GPIO_SetBits(PORT_ANTSW2, PIN_ANTSW2);
}

void SI4463_SW2_DOWN(void)
{
  GPIO_ResetBits(PORT_ANTSW2, PIN_ANTSW2);
}

#if 1
void SI4463_LED1_ON(void)
{
  GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN);
}

void SI4463_LED1_OFF(void)
{
  GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
}

void SI4463_LED2_ON(void)
{
  GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN);
}

void SI4463_LED2_OFF(void)
{
  GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
}

void Toggle_LED1(void)
{
  static uint8 x = 0;
  
  if(x)
  {
    x = 0;
    SI4463_LED1_ON();
  }
  else
  {
    x = 1;
    SI4463_LED1_OFF();
  }
}

void Toggle_LED2(void)
{
  static uint8 x = 0;
  
  if(x)
  {
    x = 0;
    SI4463_LED2_ON();
  }
  else
  {
    x = 1;
    SI4463_LED2_OFF();
  }
}
#endif

void RF_Switch_RX(void)
{
  SI4463_SW1_DOWN();
  SI4463_SW2_UP();
}

void RF_Switch_TX()
{
  SI4463_SW2_DOWN();
  SI4463_SW1_UP();
}