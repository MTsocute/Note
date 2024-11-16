//
// Created by shuhe on 2024/11/5.
//

#include "key.h"


/**
 * @brief 捕获独立按键的输入，然后对应输出 1~4
 * @returnval 1 ~ 4
 * */
uint8_t key_num(void) {
    uint8_t Key_Num;

    if (P3_1 == 0) { delay_ms(20); while (P3_1 == 0) {} delay_ms(20); Key_Num = 1; }
    if (P3_0 == 0) { delay_ms(20); while (P3_0 == 0) {} delay_ms(20); Key_Num = 2; }
    if (P3_2 == 0) { delay_ms(20); while (P3_2 == 0) {} delay_ms(20); Key_Num = 3; }
    if (P3_3 == 0) { delay_ms(20); while (P3_3 == 0) {} delay_ms(20); Key_Num = 4; }

    return Key_Num;
}

uint8_t MatrixKey() {
    unsigned char KeyNumber = 0;

    P1 = 0xFF;
    P1_3 = 0;       // col 1
    // row 1
    if (P1_7 == 0) { delay_ms(20); while (P1_7 == 0); delay_ms(20); KeyNumber = 1; }
    if (P1_6 == 0) { delay_ms(20); while (P1_6 == 0); delay_ms(20); KeyNumber = 5; }
    if (P1_5 == 0) { delay_ms(20); while (P1_5 == 0); delay_ms(20); KeyNumber = 9; }
    if (P1_4 == 0) { delay_ms(20); while (P1_4 == 0); delay_ms(20); KeyNumber = 13; }

    P1 = 0xFF;
    P1_2 = 0;   // col 2
    if (P1_7 == 0) { delay_ms(20); while (P1_7 == 0); delay_ms(20); KeyNumber = 2; }
    if (P1_6 == 0) { delay_ms(20); while (P1_6 == 0); delay_ms(20); KeyNumber = 6; }
    if (P1_5 == 0) { delay_ms(20); while (P1_5 == 0); delay_ms(20); KeyNumber = 10; }
    if (P1_4 == 0) { delay_ms(20); while (P1_4 == 0); delay_ms(20); KeyNumber = 14; }

    P1 = 0xFF;
    P1_1 = 0;
    if (P1_7 == 0) { delay_ms(20); while (P1_7 == 0); delay_ms(20); KeyNumber = 3; }
    if (P1_6 == 0) { delay_ms(20); while (P1_6 == 0); delay_ms(20); KeyNumber = 7; }
    if (P1_5 == 0) { delay_ms(20); while (P1_5 == 0); delay_ms(20); KeyNumber = 11; }
    if (P1_4 == 0) { delay_ms(20); while (P1_4 == 0); delay_ms(20); KeyNumber = 15; }

    P1 = 0xFF;
    P1_0 = 0;
    if (P1_7 == 0) { delay_ms(20); while (P1_7 == 0); delay_ms(20); KeyNumber = 4; }
    if (P1_6 == 0) { delay_ms(20); while (P1_6 == 0); delay_ms(20); KeyNumber = 8; }
    if (P1_5 == 0) { delay_ms(20); while (P1_5 == 0); delay_ms(20); KeyNumber = 12; }
    if (P1_4 == 0) { delay_ms(20); while (P1_4 == 0); delay_ms(20); KeyNumber = 16; }

    return KeyNumber;
}
