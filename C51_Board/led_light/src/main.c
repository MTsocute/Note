#include "C:\Users\shuhe\.platformio\packages\toolchain-sdcc\include\mcs51\8052.h"

// generate a delay code by specific time
void delayXms(unsigned int xms) {
    for (; xms > 0; xms--) {
        unsigned char i, j;
        i = 11;
        j = 190;
        do {
            while (--j);
        } while (--i);
    }
}

unsigned char led = 0; // 初始化 LED 变量

void main() {

    P2 = ~0x01;

    while (1) {
        // 检测按下 P3_0 按钮，增加 led 值
        if (P3_0 == 0) {
            delayXms(20); // 消抖
            // 检测松手
            while (P3_0 == 0);
            delayXms(20);

            if (led > 7) // 如果超过 7，重置为 0
                led = 0;
            else led++; // 增加 LED 值

            P2 = ~(0x01 << led); // 更新 P2
        }
        // 检测按下 P3_1 按钮，减少 led 值
        else if (P3_1 == 0) {
            delayXms(20); // 消抖
            while (P3_1 == 0);
            delayXms(20);

            if (led == 0)  led = 7;
            else led --;

            P2 = ~(0x01 << led); // 更新 P2
        }
    }
}
