//
// Created by shuhe on 2024/11/2.
//

#ifndef LED_BOARD_MY_UTILITY_H
#define LED_BOARD_MY_UTILITY_H

#include "mcs51/8052.h"


// 0-9的七段码编码
const static unsigned char ALPHA_TO_BIN[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
};


void light_nixie(const unsigned char loc, const int number);

void delay_ms(unsigned int xms);

#endif //LED_BOARD_MY_UTILITY_H
