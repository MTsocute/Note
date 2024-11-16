#include "Timer0.h"

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
    TMOD &= 0xF0;			//设置定时器模式
    TMOD |= 0x01;			//设置定时器模式
    TL0 = 0;				//设置定时初始值
    TH0 = 0;				//设置定时初始值
    TF0 = 0;				//清除TF0标志
    TR0 = 0;				//定时器0开始计时
}

void Timer0_SetCounter(unsigned int Value) {
    TH0 = Value / 256;
    TL0 = Value % 256;
}

unsigned int Timer0_GetCounter(void) {
    // 高位和低位置拼成一个，知道过去了多少时间
    return (TH0<<8) | TL0;
}

void Timer0_Run(unsigned char Flag) {
    TR0 = Flag;
}

