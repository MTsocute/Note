//
// Created by shuhe on 2024/11/16.
//

#ifndef REMOTE_CONTROL_IR_H
#define REMOTE_CONTROL_IR_H

#include "mcs51/8052.h"
#include "Timer0.h"
#include "Int0.h"

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


static unsigned int IR_Time;
static unsigned char IR_State;

///* 这个数组是一个 unsigned char 所以是 32 bit，每一个位置存储一个地址 */
static unsigned char IR_Data[4];
static unsigned char IR_pData;

static unsigned char IR_DataFlag;       // 完成数据接受的标志
static unsigned char IR_RepeatFlag;     // 遇到的是 Repeat 段

static unsigned char IR_Address;        // 一帧红外中的地址
static unsigned char IR_Command;        // 一阵红外中的指令


/**
  * @brief  红外遥控初始化
  * @param  无
  * @retval 无
  */
void IR_Init(void);

/**
  * @brief  红外遥控获取收到数据帧标志位
  * @param  无
  * @retval 是否收到数据帧，1为收到，0为未收到
  */
unsigned char IR_GetDataFlag(void);

/**
  * @brief  红外遥控获取收到连发帧标志位
  * @param  无
  * @retval 是否收到连发帧，1为收到，0为未收到
  */
unsigned char IR_GetRepeatFlag(void);

/**
  * @brief  红外遥控获取收到的地址数据
  * @param  无
  * @retval 收到的地址数据
  */
unsigned char IR_GetAddress(void);

/**
  * @brief  红外遥控获取收到的命令数据
  * @param  无
  * @retval 收到的命令数据
  */
unsigned char IR_GetCommand(void);


void demo();

#endif //REMOTE_CONTROL_IR_H
