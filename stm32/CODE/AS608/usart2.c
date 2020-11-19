#include "sys.h"
#include "usart2.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "timer.h" 

//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART2_RX_STA=0;
void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 			 
		if((USART2_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)//还可以接收数据
			{
				TIM7->CNT=0;         				//计数器清空
				if(USART2_RX_STA==0) 				//使能定时器7的中断 
				{
					TIM7->CR1|=1<<0;     			//使能定时器7
				}
				USART2_RX_BUF[USART2_RX_STA++]=res;	//记录接收到的值 
			}else 
			{
				USART2_RX_STA|=1<<15;				//强制标记接收完成
			} 
		}
	}  				 											 
}
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)

void usart2_init(void)
{  	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	// 串口是挂载在 APB1 下面的外设，所以使能函数为
	//使能 USART2 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//使用的是串口 2，串口 2 对应着芯片引脚 PA2,PA3 需要使能PA的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 

	//设置引脚复用器映射
	//引脚复用器映射配置，需要配置PA2，PA3 的引脚，调用函数为：
	//PA2复用为 USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	//PA10 复用为 USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	//初始化 PA9，PA10
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;				
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 		
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP; 			
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP; 			
	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	
	USART_InitStruct.USART_BaudRate 				= 57600;				
	USART_InitStruct.USART_WordLength 			= USART_WordLength_8b;	
	USART_InitStruct.USART_StopBits 				= USART_StopBits_2;		
	USART_InitStruct.USART_Parity 				= USART_Parity_No;		
	USART_InitStruct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	

	USART_Init(USART2, &USART_InitStruct); 

	NVIC_InitStruct.NVIC_IRQChannel						= USART2_IRQn;  	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority		= 0x01;		
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x01;			
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		

    NVIC_Init(&NVIC_InitStruct);	
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);

	TIM7_Int_Init(100-1,8400-1);	//10ms中断一次
	TIM7->CR1&=~(1<<0);		    //关闭定时器
	
	USART2_RX_STA=0;			   //清零
}

//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART2->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART2->DR=USART2_TX_BUF[j];  
	} 
}





