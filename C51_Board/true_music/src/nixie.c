//
// Created by shuhe on 2024/11/5.
//

#include "nixie.h"

/**
 * @brief 在数字数码板显示数字
 * @param loc 哪一个位置显示 (1 ~ 8)
 * @param number 显示多少 (0 ~ 9)
 * */
void light_nixie(uint8_t loc, uint8_t number) {
    // 防止越界
    if (loc < 1 || loc > 9) return;
    if (number < 0 || number > 9) return;

    switch (loc) {
        case 1 : P2_4 = 1; P2_3 = 1;  P2_2 = 1; break;
        case 2 : P2_4 = 1; P2_3 = 1;  P2_2 = 0; break;
        case 3 : P2_4 = 1; P2_3 = 0;  P2_2 = 1; break;
        case 4 : P2_4 = 1; P2_3 = 0;  P2_2 = 0; break;
        case 5 : P2_4 = 0; P2_3 = 1;  P2_2 = 1; break;
        case 6 : P2_4 = 0; P2_3 = 1;  P2_2 = 0; break;
        case 7 : P2_4 = 0; P2_3 = 0;  P2_2 = 1; break;
        case 8 : P2_4 = 1; P2_3 = 1;  P2_2 = 1; break;
        default: break;
    }

    P0 = ALPHA_TO_BIN[number];
    delay_ms(1);
}


void clean_nixie(void) { P0 = 0x00; }