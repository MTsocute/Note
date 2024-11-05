//
// Created by shuhe on 2024/11/3.
//

#ifndef MATRIX_KEYBOARD_MATRIXKEY_H
#define MATRIX_KEYBOARD_MATRIXKEY_H

#include "mcs51/8052.h"

void DelayXms(unsigned int xms);    //@11.0592MHz

void Timer0_Init(void);

unsigned char interrupt_key();

void crol(unsigned char *value, unsigned char bits);

void cror(unsigned char *value, unsigned char bits);

#endif //MATRIX_KEYBOARD_MATRIXKEY_H
