#include <stdint.h>
#include "Key.h"
#include "Nixie.h"
#include "Clock.h"
#include "Timer0.h"
#include "AT24C02.h"

uint8_t KeyNum;

int main() {
    Timer0_Init();
    while (1) {
        KeyNum = Key();

        if (KeyNum) {
            // 开始 | 停止 记时
            if (KeyNum == 1) { RunFlag = ! RunFlag; }
            // 重新开始
            if (KeyNum == 2) {
                MiniSec = 0;
                Sec = 0;
                Min = 0;
            }
            // 存储记忆的数据
            if (KeyNum == 3) {
                AT24_WriteByte(0, Min);
                Delay(5);
                AT24_WriteByte(1, Sec);
                Delay(5);
                AT24_WriteByte(2, MiniSec);
                Delay(5);
            }
            // 读出存储的数据
            if (KeyNum == 4) {
                Min = AT24_ReadByte(0);
                Sec = AT24_ReadByte(1);
                MiniSec = AT24_ReadByte(2);
            }
        }

        show_time();
    }
}

void timer0_itrp() __interrupt(1) {
    static uint16_t T0_Time, T1_Time, T2_Time;
    TL0 = 0x18;        //设置定时初值
    TH0 = 0xFC;        //设置定时初值
    T0_Time++;
    if (T0_Time >= 20) {
        T0_Time = 0;
        Key_Loop();
    }
    T1_Time++;
    if (T1_Time >= 2) {
        T1_Time = 0;
        Nixie_Loop();
    }
    T2_Time ++;
    if (T2_Time >= 1) {
        T2_Time = 0;
        Sec_Loop();
    }
}