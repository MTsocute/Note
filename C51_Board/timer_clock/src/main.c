#include "LCD_1064.h"

unsigned char Sec = 0, Min = 0, hour = 0;


void Timer0_Init(void)        //1毫秒@12.000MHz
{
    TMOD &= 0xF0;            //设置定时器模式
    TMOD |= 0x01;            //设置定时器模式
    TL0 = 0x18;                //设置定时初始值
    TH0 = 0xFC;                //设置定时初始值
    TF0 = 0;                //清除TF0标志
    TR0 = 1;                //定时器0开始计时

    // 下面的部分才可以保证中断
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

int main() {
    Timer0_Init();
    LCD_Init();
    LCD_ShowString(1, 1, "Clock:");   // 显示标题

    P2_1 = 0;

    while (1) {
        // 显示当前时间
        LCD_ShowNum(2, 2, hour, 2);   // 显示小时
        LCD_ShowString(2, 4, ":");
        LCD_ShowNum(2, 5, Min, 2);    // 显示分钟
        LCD_ShowString(2, 7, ":");
        LCD_ShowNum(2, 8, Sec, 2);    // 显示秒
    }
}

void time0() __interrupt(TF0_VECTOR) {
    static unsigned int T0_count = 0;

    TL0 = 0x18;                //设置定时初始值
    TH0 = 0xFC;                //设置定时初始值

    T0_count++;
    if (T0_count >= 1000) {
        T0_count = 0;

        // 每秒更新一次
        Sec++;
        if (Sec >= 60) {
            Sec = 0;
            Min++;
            if (Min >= 60) {
                Min = 0;
                hour++;
                if (hour == 24) hour = 0;
            }
        }
    }
}
