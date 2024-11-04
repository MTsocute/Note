//
// Created by shuhe on 2024/11/3.
//

#include "matrixKey.h"

void DelayXms(unsigned int xms)    //@11.0592MHz
{

    while (xms--) {
        unsigned char i, j;

        i = 11;
        j = 190;
        do {
            while (--j);
        } while (--i);
    }
}

unsigned char MatrixKey() {
    unsigned char KeyNumber = 0;

    P1 = 0xFF;
    P1_3 = 0;       // col 1
    // row 1
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 1; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 5; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 9; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 13; }

    P1 = 0xFF;
    P1_2 = 0;   // col 2
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 2; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 6; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 10; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 14; }

    P1 = 0xFF;
    P1_1 = 0;
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 3; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 7; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 11; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 15; }

    P1 = 0xFF;
    P1_0 = 0;
    if (P1_7 == 0) { DelayXms(20); while (P1_7 == 0); DelayXms(20); KeyNumber = 4; }
    if (P1_6 == 0) { DelayXms(20); while (P1_6 == 0); DelayXms(20); KeyNumber = 8; }
    if (P1_5 == 0) { DelayXms(20); while (P1_5 == 0); DelayXms(20); KeyNumber = 12; }
    if (P1_4 == 0) { DelayXms(20); while (P1_4 == 0); DelayXms(20); KeyNumber = 16; }

    return KeyNumber;
}