//
// Created by shuhe on 2024/11/2.
//

#include "my_utility.h"
//#include "mcs51/8052.h"

// 将整数转换为二进制数组
void light_nixie(const unsigned char loc, const int number) {
    if (loc < 0 || loc > 9) return;  // 防止越界
    if (number > 9 || number < 0) return;

    switch (loc) {
        // 7
        case 1 : P2_4 = 1; P2_3 = 1;  P2_2 = 1; break;
            // 6
        case 2 : P2_4 = 1; P2_3 = 1;  P2_2 = 0; break;
            // 5
        case 3 : P2_4 = 1; P2_3 = 0;  P2_2 = 1; break;
            // 4
        case 4 : P2_4 = 1; P2_3 = 0;  P2_2 = 0 ; break;
            // 3
        case 5 : P2_4 = 0; P2_3 = 1;  P2_2 = 1; break;
            // 2
        case 6 : P2_4 = 0; P2_3 = 1;  P2_2 = 0; break;
            // 1
        case 7 : P2_4 = 0; P2_3 = 0;  P2_2 = 1; break;

        case 8 : P2_4 = 1; P2_3 = 1;  P2_2 = 1; break;

        default:
            break;
    }

    P0 = ALPHA_TO_BIN[number];
}

// 简单的毫秒级延时函数
void delay_ms(unsigned int xms) {
    while (xms--) {
        unsigned char i = 11, j = 190;
        do {
            while (--j);
        } while (--i);
    }
}