//
// Created by shuhe on 2024/11/5.
//

#ifndef TRUE_MUSIC_NIXIE_H
#define TRUE_MUSIC_NIXIE_H

#include "mcs51/8052.h"
#include "delay.h"


typedef unsigned char uint8_t;

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


void light_nixie(uint8_t loc, uint8_t number);


#endif //TRUE_MUSIC_NIXIE_H
