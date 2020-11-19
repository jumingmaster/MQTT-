#include "keyboard.h"
#include "delay.h"
#include "Communication.h"
static u8 read_flag = 1;

void Board_Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
    
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;	//引脚6 7 8 9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 				//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line = EXTI_Line2;
    EXTI_Init(&EXTI_InitStruct);
    
    
    NVIC_InitStruct.NVIC_IRQChannel						= EXTI0_IRQn;  	//NVIC通道，在stm32f4xx.h可查看通道 （可变）
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   	= 0x01;			//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x01;			//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		//使能
	//配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);	    
    NVIC_InitStruct.NVIC_IRQChannel						= EXTI2_IRQn;  	//NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_Init(&NVIC_InitStruct);	
}
/*
引脚说明：
列：1->DCMI_D4->PC11 	2->DCMI_D5->PB6 	3->DCMI__XCLK->PA8 	4->DCMI_D7->PE6
行：5->DCMI_D0->PC6 	6->DCMI_D1->PC7 	7->DCMI_D2->PC8 	8->DCMI_D3->PC9

函数说明：对按键的列初始化为下拉输入，对按键的行初始化为上拉输出
*/

//行列按键的初始化
void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//使能时钟
	//GPIO A组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//GPIO B组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//GPIO C组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//GPIO E组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//设置各行的模式
	//第1、2、3、4行 PC6 PC7 PC8 PC9
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//引脚6 7 8 9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 				//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;			//速率
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//设置各列的模式
	//第1列 PC11
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_11;				//引脚11
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;			//推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; 		//快速
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_DOWN;     		//下拉
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//第2列 PB6
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;				//引脚6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;			//推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; 		//快速
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_DOWN;     		//下拉
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//第3列 PA8
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8;				//引脚6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;			//推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; 		//快速
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_DOWN;     		//下拉
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//第4列 PE6
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5|GPIO_Pin_6;	//引脚5 6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;			//推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; 		//快速
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_DOWN;     		//下拉
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
}

//判断矩阵键盘中是否有按键按下
u8 KEY_work(void)
{
	u8 key_work_flay = 0;
	if(Read_Line1==0 || Read_Line2==0 || Read_Line3==0 || Read_Line4==0)	//表示有按键按下
	{
		delay_ms(15);
		key_work_flay = 1;
	}
	else		//没有按键按下
		key_work_flay = 0;
	return key_work_flay;
}


//按键扫描函数（用来确定哪一个按键被按下）
u8 KEY_Scan(void)
{
	u8 Keynumber = 0;				//按键返回值
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);			//配置第一列为低电平输出
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);			//配置第二列为低电平输出
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);			//配置第三列为低电平输出
	GPIO_ResetBits(GPIOE, GPIO_Pin_6);			//配置第三列为低电平输出
	if(Read_Line1==0 || Read_Line2==0 || Read_Line3==0 || Read_Line4==0)	//判断是哪一行
	{
		delay_ms(15);
		if(Read_Line1==0 || Read_Line2==0 || Read_Line3==0 || Read_Line4==0)
		{
			if(Read_Line1==0)  
				Keynumber =0;
			if(Read_Line2==0)  
				Keynumber =4;
			if(Read_Line3==0)  
				Keynumber =8;
			if(Read_Line4==0)  
				Keynumber =12;
		
			//判断在哪一列
			GPIO_ResetBits(GPIOC, GPIO_Pin_11);			//配置第一列为低电平输出
			GPIO_SetBits(GPIOB, GPIO_Pin_6);			//配置第二列为高电平输出
			GPIO_SetBits(GPIOA, GPIO_Pin_8);			//配置第三列为高电平输出
			GPIO_SetBits(GPIOE, GPIO_Pin_6);			//配置第四列为高电平输出
			if(Read_Line1==0||Read_Line2==0||Read_Line3==0||Read_Line4==0) 	//确定是在第一列
				Keynumber = Keynumber+1;
		
			GPIO_SetBits(GPIOC, GPIO_Pin_11);			//配置第一列为高电平输出
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);			//配置第二列为低电平输出
			GPIO_SetBits(GPIOA, GPIO_Pin_8);			//配置第三列为高电平输出
			GPIO_SetBits(GPIOE, GPIO_Pin_6);			//配置第四列为高电平输出
			if(Read_Line1==0||Read_Line2==0||Read_Line3==0||Read_Line4==0) 	//确定是在第二列
				Keynumber = Keynumber+2;
		
			GPIO_SetBits(GPIOC, GPIO_Pin_11);			//配置第一列为高电平输出
			GPIO_SetBits(GPIOB, GPIO_Pin_6);			//配置第二列为高电平输出
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);			//配置第三列为低电平输出
			GPIO_SetBits(GPIOE, GPIO_Pin_6);			//配置第四列为高电平输出
			if(Read_Line1==0||Read_Line2==0||Read_Line3==0||Read_Line4==0) 	//确定是在第三列
				Keynumber = Keynumber+3;
		
			GPIO_SetBits(GPIOC, GPIO_Pin_11);			//配置第一列为高电平输出
			GPIO_SetBits(GPIOB, GPIO_Pin_6);			//配置第二列为高电平输出
			GPIO_SetBits(GPIOA, GPIO_Pin_8);			//配置第三列为高电平输出
			GPIO_ResetBits(GPIOE, GPIO_Pin_6);			//配置第四列为低电平输出
			if(Read_Line1==0||Read_Line2==0||Read_Line3==0||Read_Line4==0) 	//确定是在第四列
				Keynumber = Keynumber+4;
			
			//还原状态（原本列初始化为下拉输出）
			GPIO_ResetBits(GPIOC, GPIO_Pin_11);			//配置第一列为低电平输出
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);			//配置第二列为低电平输出
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);			//配置第三列为低电平输出
			GPIO_ResetBits(GPIOE, GPIO_Pin_6);			//配置第三列为低电平输出
			
			return Keynumber;
		}
	}
	
	return 0;
	
}

/*
按键功能选择函数
按键分布和对应的数字
1='1' 	2='2' 	3='3' 	4='A'
5='4' 	6='5' 	7='6' 	8='B'
9='7' 	10='8' 	11='9' 	12='C'
13='*' 	14='0' 	15='#' 	16='D'
*/
u8 KEY_Fun(u8 Keynumber)
{
	u8 temp = 0;
	
	switch(Keynumber)
	{
		case 1: temp = '1';
				break;
		case 2: temp = '2';
				break;
		case 3: temp = '3';
				break;
		case 4: temp = 'A';
				break;
		case 5: temp = '4';
				break;
		case 6: temp = '5';
				break;
		case 7: temp = '6';
				break;
		case 8: temp = 'B';
				break;
		case 9: temp = '7';
				break;
		case 10: temp = '8';
				break;
		case 11: temp = '9';
				break;
		case 12: temp = 'C';
				break;
		case 13: temp = '*';
				break;
		case 14: temp = '0';
				break;
		case 15: temp = '#';
				break;
		case 16: temp = 'D';
				break;
		default:
				break;
	}
	return temp;
}


u8 Get_Num(int size)
{
    u8 tmp[3] = {0}, val = 0;
    int i = 0;
    
    while(i < size)
    {
        
        if(KEY_work() && read_flag == 1)
        {
            read_flag = 0;
            tmp[i] = KEY_Scan();
            if(KEY_Fun(tmp[i]) == '#')
            {
                TIM_Cmd(TIM3, ENABLE);
                return val;
            }
            val *= i;
            val += tmp[i];
            i++; 
            TIM_Cmd(TIM3, ENABLE);
        }
        return val;
    }
}


void EXTI0_IRQHandler(void)
{
	//判断中断标志是否为1
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
        delay_ms(15);
        if(EXTI_GetITStatus(EXTI_Line0) == SET)
        {
            mqtt_packet_t packet;
            create_packet(DoorState, "close", &packet);
            send_packet(&packet);
        }
		//清空标志位
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
    
    if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
        delay_ms(15);
        if(EXTI_GetITStatus(EXTI_Line2) == SET)
        {
            mqtt_packet_t packet;
            create_packet(DoorState, "open", &packet);
            send_packet(&packet);
        }
		//清空标志位
		EXTI_ClearITPendingBit(EXTI_Line2);
	}

}

/*
void EXTI1_IRQHandler(void)
{
	//判断中断标志是否为1
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		
		//变更灯状态
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		//清空标志位
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}
void EXTI2_IRQHandler(void)
{
	//判断中断标志是否为1
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		
		//变更灯状态
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		//清空标志位
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}
*/
