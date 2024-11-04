//
// Created by shuhe on 2024/11/3.
//

#include "matrixKey.h"

void DelayXms(unsigned int xms)    //@11.0592MHz
{

    while (xms--) {
        unsigned char i, j;

        i = 11;
        j = 190;
        do {
            while (--j);
        } while (--i);
    }
}


void Timer0_Init(void)		//1毫秒@12.000MHz
{
    TMOD &= 0xF0;			//设置定时器模式
    TMOD |= 0x01;			//设置定时器模式
    TL0 = 0x18;				//设置定时初始值
    TH0 = 0xFC;				//设置定时初始值
    TF0 = 0;				//清除TF0标志
    TR0 = 1;				//定时器0开始计时
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}



//void Timer0_Init(void)        //1微秒@11.0592MHz
//{
//    TMOD &= 0xF0;            //设置定时器模式
//    TMOD |= 0x01;            //设置定时器模式
//    TL0 = 0xFF;                //设置定时初始值
//    TH0 = 0xFF;                //设置定时初始值
//    TF0 = 0;                //清除TF0标志
//    TR0 = 1;                //定时器0开始计时
//    ET0 = 1;                //使能定时器0中断
//    EA = 1;                 //允许总中断
//    PT0 = 1;                //中断的优先级
//}
