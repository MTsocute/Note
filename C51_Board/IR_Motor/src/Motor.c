//
// Created by shuhe on 2024/11/17.
//

#include "Motor.h"

extern unsigned char COMPARE;
extern unsigned char T0_Count;


void Motor_Init(void) {
    Timer1_Init();
}

void Motor_SetSpeed(unsigned char speed) {
    if (speed == 0) COMPARE = 0;
    if (speed == 1) COMPARE = 50;
    if (speed == 2) COMPARE = 75;
    if (speed == 3) COMPARE = 100;
}

void Motor_PWM() {
    TL1 = 0x66;                //设置定时初始值
    TH1 = 0xFC;                //设置定时初始值

    T0_Count++;
    T0_Count %= 100;

    if (COMPARE > T0_Count) Motor = 1;
    else Motor = 0;
}