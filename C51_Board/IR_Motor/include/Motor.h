//
// Created by shuhe on 2024/11/17.
//

#ifndef IR_MOTOR_MOTOR_H
#define IR_MOTOR_MOTOR_H

#include "mcs51/8052.h"
#include <stdint.h>
#include "Timer1.h"

__sbit __at(0x90 ^ 0) Motor;

void Motor_Init(void);

void Motor_SetSpeed(unsigned char speed);

void Motor_PWM();


#endif //IR_MOTOR_MOTOR_H
