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