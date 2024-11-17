//
// Created by shuhe on 2024/11/16.
//

#ifndef REMOTE_CONTROL_IR_H
#define REMOTE_CONTROL_IR_H

#include "mcs51/8052.h"
#include "Timer0.h"
#include "Int0.h"
#include <stdint.h>
#include "LCD1602.h"

#define IR_POWER        0x45
#define IR_MODE            0x46
#define IR_MUTE            0x47
#define IR_START_STOP    0x44
#define IR_PREVIOUS        0x40
#define IR_NEXT            0x43
#define IR_EQ            0x07
#define IR_VOL_MINUS    0x15
#define IR_VOL_ADD        0x09
#define IR_0            0x16
#define IR_RPT            0x19
#define IR_USD            0x0D
#define IR_1            0x0C
#define IR_2            0x18
#define IR_3            0x5E
#define IR_4            0x08
#define IR_5            0x1C
#define IR_6            0x5A
#define IR_7            0x42
#define IR_8            0x52
#define IR_9            0x4A


/**
  * @brief  红外遥控初始化
  * @param  无
  * @retval 无
  */
void IR_Init(void);

/**
  * @brief  放在外部中断函数中，实现接受红外数据
  * @param  无
  * @retval 无
  */
void IR_Main(void);

/**
  * @brief  放在外部中断函数中，实现接受红外数据
  * @param  无
  * @retval 无
  */
uint8_t IR_GetAddress();

/**
  * @brief  放在外部中断函数中，实现接受红外数据
  * @param  无
  * @retval 无
  */
uint8_t IR_GetCommand();

uint8_t IR_Get_DataFlag();

uint8_t IR_Get_RepeatFlag();


#endif //REMOTE_CONTROL_IR_H
