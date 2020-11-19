#include "exti.h"

void Exti_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource6);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource7);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource8);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource9);
    
    //PC6
    EXTI_InitStruct.EXTI_Line = EXTI_Line6;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line = EXTI_Line7;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line = EXTI_Line8;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line = EXTI_Line9;
    EXTI_Init(&EXTI_InitStruct);
    
    NVIC_InitStruct.NVIC_IRQChannel						= EXTI2_IRQn;  	//NVIC通道，在stm32f4xx.h可查看通道 （可变）
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   	= 0x01;			//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x01;			//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		//使能
	//配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);	
    NVIC_InitStruct.NVIC_IRQChannel						= EXTI3_IRQn;  	//NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_Init(&NVIC_InitStruct);	
}

