#ifndef _DELAY_H
#define _DELAY_H
#include <stm32f4xx.h>

void Init_Delay(void);

void delay_us(u32 nus);

void delay_ms(u32 nms);

void delay_s(int n);


#endif
