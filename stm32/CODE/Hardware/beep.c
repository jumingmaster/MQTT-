#include "beep.h"

//初始化蜂鸣器
void Init_Beep(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOF,&GPIO_InitStruct);

    BeepCalm;

}
