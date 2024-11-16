//
// Created by shuhe on 2024/11/6.
//

#include "timer0.h"


void Timer0_Init(void) {
    TMOD &= 0xF0;			//设置定时器模式
    TMOD |= 0x01;			//设置定时器模式
    TL0 = 0x18;				//设置定时初始值
    TH0 = 0xFC;				//设置定时初始值
    TF0 = 0;				//清除TF0标志
    TR0 = 1;				//定时器0开始计时

    // 下面的部分才可以保证中断
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}