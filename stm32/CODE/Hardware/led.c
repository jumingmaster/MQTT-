#include "led.h"

void Init_LED(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init(GPIOE,&GPIO_InitStruct);

    
    AlarmLED_OFF;
    DataLED_OFF;
    FigurePrintLED_OFF;
    RuntimeLED_OFF;
    
}

void Bowl_ON(void)
{
    mqtt_packet_t msg;
    create_packet(DoorState, "open", &msg);
    send_packet(&msg);
    
}

void Bowl_OFF(void)
{
    mqtt_packet_t msg;
    create_packet(DoorState, "close", &msg);
    send_packet(&msg);
}
