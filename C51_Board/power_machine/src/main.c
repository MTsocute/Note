#include "Timer0.h"
#include "Key.h"
#include "Nixie.h"
#include <stdint.h>

uint8_t COUNTER, COMPARE;
uint8_t Key_NUM, SPEED;

__sbit __at(0x90 ^ 0) MOTOR;

/**
 * @param Cmp_Val 这个值越大，那么亮度越高，因为对应的 0 的时间就越长
 * */
inline void set_compare_val(const uint8_t Cmp_Val) { COMPARE = Cmp_Val; }

/**
 * @brief 根据外部的速度来设置 Compare 值，以便于控制 PWM
 * @param speed 外部的速度等级
 * */
inline void set_compare_by_key(const uint8_t speed) {
    if (SPEED == 0) { set_compare_val(0); }
    if (SPEED == 1) { set_compare_val(50); }
    if (SPEED == 2) { set_compare_val(70); }
    if (SPEED == 3) { set_compare_val(100); }
}


int main() {
    Timer0_Init();
    set_compare_val(5);
    while (1) {
        Key_NUM = Key();

        if (Key_NUM == 1) {
            if (SPEED >= 0 && SPEED < 3)
                SPEED ++;
            set_compare_by_key(SPEED);
        }

        if (Key_NUM == 2) {
            if (SPEED > 0)
                SPEED --;
            set_compare_by_key(SPEED);
        }

        Nixie(1, SPEED);
    }
}

void Timer0_Routine() __interrupt(1) {
    static uint8_t Timer0_Counter = 0;
    TL0 = 0x9C;				//设置定时初始值
    TH0 = 0xFF;				//设置定时初始值
    COUNTER ++;
    COUNTER %= 100;

    Timer0_Counter ++;
    if (Timer0_Counter >= 20) {
        Key_Loop();
        Timer0_Counter = 0;
    }

    if (COUNTER <= COMPARE) MOTOR = 1;
    else MOTOR = 0;
}
