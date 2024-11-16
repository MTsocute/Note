//
// Created by shuhe on 2024/11/14.
//


#include "Delay.h"

void Delay480us(void)    //@11.0592MHz
{
    unsigned char i;
    i = 227;
    while (--i);
}

void Delay_X_ms(uint8_t xms) {
    unsigned char i, j;
    while (xms--) {
        i = 2;
        j = 199;
        do {
            while (--j);
        } while (--i);
    }
}
