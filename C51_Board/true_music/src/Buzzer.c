//
// Created by shuhe on 2024/11/5.
//

#include "Buzzer.h"



/**
 * @brief 让蜂鸣器发出生硬
 * @param ms
 * */
void Buzzer_Time(unsigned int ms) {
    // 因为一个周期是 执行两次相当于 2Pi，所以我们
    for (unsigned int i = 0; i < ms * 2; ++i) {
        Buzzer = !Buzzer;
        Delay500us();
    }
}