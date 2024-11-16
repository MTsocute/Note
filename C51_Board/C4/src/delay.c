//
// Created by shuhe on 2024/11/5.
//

#include "delay.h"


void delay_ms(unsigned int xms) {

    while (xms--) {
        unsigned char i, j;
        i = 2;
        j = 239;
        do {
            while (--j);
        } while (--i);
    }
}


void Delay500us(void)	//@12.000MHz
{
    unsigned char i;

    i = 247;
    while (--i);
}