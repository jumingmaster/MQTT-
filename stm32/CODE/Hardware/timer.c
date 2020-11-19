#include "timer.h"
#include "led.h"
 	 
void TIM1_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_13 | GPIO_Pin_14; //GPIOF9 
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//配置IO口复用功能
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 		//速度 50MHz
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP; 		//推挽复用输出
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_13);
	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
	
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period= (5000 - 1);
	TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_Cmd(TIM1, ENABLE);

}


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Init(void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
 NVIC_InitTypeDef  NVIC_InitStruct;
 
 //能定时器时钟。
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
 TIM_TimeBaseInitStruct.TIM_Prescaler    = 8400-1;    //8400分频 84000 000/8400 = 10000HZ  1S数10000个数
 TIM_TimeBaseInitStruct.TIM_Period  = 2000-1;  //计1000个数 在10000HZ下，用时0.2s
 TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up; //向上计数
 TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //分频因子
 //初始化定时器，配置ARR,PSC。
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); 
 
 NVIC_InitStruct.NVIC_IRQChannel      = TIM3_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;   //抢占优先级
 NVIC_InitStruct.NVIC_IRQChannelSubPriority   = 0x01;   //响应优先级
 NVIC_InitStruct.NVIC_IRQChannelCmd     = ENABLE;  //使能
    //启定时器中断，配置NVIC。
 NVIC_Init(&NVIC_InitStruct);  
 
 
 //设置 TIM3_DIER  允许更新中断
 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
 
 //使能定时器。
 TIM_Cmd(TIM3, ENABLE);
 
}
void TIM4_LockControl_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_8; //GPIOF8
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//配置IO口复用功能
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 		//速度 50MHz
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP; 		//推挽复用输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP; 		//推挽复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period= (200-1);
	TIM_TimeBaseStructure.TIM_Prescaler= (8400 - 1);	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_Cmd(TIM4, ENABLE);

}
//TIM8 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM8_LED_Init(void)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<8; 	//TIM8时钟使能    
	RCC->AHB1ENR|=1<<5;   	//使能PORTF时钟	

	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM8);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_1;           //GPIOB1
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//配置IO口复用功能
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz; 		//速度 50MHz
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP; 			//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_13;           //GPIOC13
    GPIO_Init(GPIOC,&GPIO_InitStructure);
	PCout(13) = 0;
    GreenLED_OFF;
    
	TIM8->ARR = 4999;			//设定计数器自动重装值 
	TIM8->PSC = 83;			//预分频器不分频 
	TIM8->CCMR1|=6<<4;  	//CH1 PWM1模式		 
	TIM8->CCMR1|=1<<3; 	//CH1 预装载使能	   
	TIM8->CCER|=1<<0;   	//OC1 输出使能	
	TIM8->CCER|=1<<1;   	//OC1 低电平有效	   
	TIM8->CR1|=1<<7;   	//ARPE使能 
	TIM8->CR1|=1<<0;    	//使能定时器14 		

	
}  

//定时器2通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<3;   	//TIM5 时钟使能 
	RCC->AHB1ENR|=1<<0;   	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_TIM5);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;			//配置IO口复用功能
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz; 		//速度 50MHz
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN; 			//上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure); 
	
 	TIM5->ARR=arr;  		//设定计数器自动重装值   
	TIM5->PSC=psc;  		//预分频器 

	TIM5->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波
 	TIM5->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 

	TIM5->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM5->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

	TIM5->EGR=1<<0;			//软件控制产生更新事件,使写入PSC的值立即生效,否则将会要等到定时器溢出才会生效!
	TIM5->DIER|=1<<1;   	//允许捕获1中断				
	TIM5->DIER|=1<<0;   	//允许更新中断	
	TIM5->CR1|=0x01;    	//使能定时器2
	
	NVIC_InitTypeDef  NVIC_InitStruct;	
	NVIC_InitStruct.NVIC_IRQChannel						= TIM5_IRQn;  	//NVIC通道，在stm32f4xx.h可查看通道 （可变）
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority		= 0x02;			//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x00;			//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		//使能
	//配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);	
}
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//溢出
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			    TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//获取当前的捕获值.
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM5->CR1&=~(1<<0)		;    	//使能定时器2
	 			TIM5->CNT=0;					//计数器清空
	 			TIM5->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
				TIM5->CR1|=0x01;    			//使能定时器2
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;//清除中断标志位   
}

extern vu16 USART2_RX_STA;
//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	  		    
	if(TIM7->SR&0X01)//是更新中断
	{	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM7->SR&=~(1<<0);		//清除中断标志位		   
		TIM7->CR1&=~(1<<0);		//关闭定时器7	  
	}	      											 
} 
//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;	//TIM7时钟使能    
 	TIM7->ARR=arr;  	//设定计数器自动重装值 
	TIM7->PSC=psc;  	//预分频器	  
	TIM7->CNT=0;  		//计数器清零	  
	TIM7->DIER|=1<<0;   //允许更新中断	  
	TIM7->CR1|=0x01;    //使能定时器7
	
	NVIC_InitTypeDef  NVIC_InitStruct;	
	NVIC_InitStruct.NVIC_IRQChannel						= TIM7_IRQn;  	//NVIC通道，在stm32f4xx.h可查看通道 （可变）
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority		= 0x03;			//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x02;			//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		//使能
	//配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);	
    
} 












