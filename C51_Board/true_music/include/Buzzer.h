//
// Created by shuhe on 2024/11/5.
//

#ifndef TRUE_MUSIC_BUZZER_H
#define TRUE_MUSIC_BUZZER_H

#include "mcs51/8052.h"
#include "delay.h"

__sbit __at(0xA5) Buzzer;

void Buzzer_Time(unsigned int ms);


#endif //TRUE_MUSIC_BUZZER_H
