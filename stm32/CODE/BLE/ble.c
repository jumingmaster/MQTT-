#include "ble.h"
#include <string.h>




void Init_BLE(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
    
    //使能APB1时钟、时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    //GPIO 复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
    
    //初始化GPIO引脚
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    
    
    USART_InitStruct.USART_BaudRate 				= 9600;				
	USART_InitStruct.USART_WordLength 			= USART_WordLength_8b;	
	USART_InitStruct.USART_StopBits 				= USART_StopBits_1;		
	USART_InitStruct.USART_Parity 				= USART_Parity_No;		
	USART_InitStruct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	

	USART_Init(USART3, &USART_InitStruct); 

	NVIC_InitStruct.NVIC_IRQChannel						= USART3_IRQn;  	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority		= 0x01;		
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x01;			
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		

    NVIC_Init(&NVIC_InitStruct);	
	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
}

