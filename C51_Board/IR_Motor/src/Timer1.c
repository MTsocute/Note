//
// Created by shuhe on 2024/11/17.
//

#include "Timer1.h"

void Timer1_Init(void)		//1毫秒@11.0592MHz
{
    TMOD &= 0x0F;			//设置定时器模式
    TMOD |= 0x10;			//设置定时器模式
    TL1 = 0x66;				//设置定时初始值
    TH1 = 0xFC;				//设置定时初始值
    TF1 = 0;				//清除TF1标志
    TR1 = 1;				//定时器1开始计时

    ET1 = 1;
    EA = 1;
    PT1 = 0;
}

