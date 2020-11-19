#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"
#include "sys.h"


#define Read_Line1	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)	//读取第一行的引脚状态
#define Read_Line2	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)	//读取第一行的引脚状态
#define Read_Line3	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)	//读取第一行的引脚状态
#define Read_Line4	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)	//读取第一行的引脚状态

#define Read_List1	GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_11)	//读取第一列的引脚状态
#define Read_List2	GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6)	//读取第二列的引脚状态
#define Read_List3	GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8)	//读取第三列的引脚状态
#define Read_List4	GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_6)	//读取第三列的引脚状态



//行列按键的初始化
void Key_Init(void);

//判断是否有按键按下
u8 KEY_work(void);

//按键扫描函数（用来确定哪一个按键被按下）
u8 KEY_Scan(void);


//按键功能选择函数
u8 KEY_Fun(u8 Keynumber);

//获取十进制值
u8 Get_Num(int size);

void Board_Key_Init(void);

#endif
