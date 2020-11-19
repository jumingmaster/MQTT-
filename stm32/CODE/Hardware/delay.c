#include "delay.h"

const u32 my_us = 21;     //1微秒
const u32 my_ms = 21000;  //1毫秒

void Init_Delay(void)
{
    //systic定时器频率 168/8 = 21MHZ    
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}


//u32 nus范围:0~798915
void delay_us(u32 nus)
{
	u32 temp;
	
	//设置重装值
	SysTick->LOAD = my_us*nus - 1;
	//设置VAL0
	SysTick->VAL = 0x00;
	
	//开定时器
	SysTick->CTRL |= (0x01<<0);
	
	
	do
	{
		temp = SysTick->CTRL;  //temp 第十六位为1，则计数到0
	}while(  temp & (0x01<<0) && !(temp & (0x01<<16)) );
			//判断计时器是否开启，技术是否到0
	
	//关定时器
	SysTick->CTRL &= ~(0x01<<0);	

}

//nms范围：0~798
void delay_ms(u32 nms)
{

	u32 temp;
	
	//设置重装值
	SysTick->LOAD = my_ms*nms - 1;
	//设置VAL0
	SysTick->VAL = 0x00;
	
	
	//开定时器
	SysTick->CTRL |= (0x01<<0);
	
	
	do
	{
		temp = SysTick->CTRL;  //temp 第十六位为1，则计数到0
	}while(  temp & (0x01<<0) && !(temp & (0x01<<16)) );
			//判断计时器是否开启，技术是否到0
	
	//关定时器
	SysTick->CTRL &= ~(0x01<<0);

}


void delay_s(int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}
}
