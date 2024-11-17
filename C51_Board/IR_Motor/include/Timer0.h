#ifndef __TIMER0_H__
#define __TIMER0_H__

#include "mcs51/8052.h"


/**
  * @brief  定时器0初始化
  * @param  无
  * @retval 无
  */
void Timer0_Init(void);

/**
  * @brief  定时器0设置计数器值
  * @param  Value，要设置的计数器值，范围：0~65535
  * @retval 无
  */
void Timer0_SetCounter(unsigned int Value);

/**
  * @brief  定时器0获取计数器值
  * @param  无
  * @retval 计数器值，范围：0~65535
  */
unsigned int Timer0_GetCounter(void);

/**
  * @brief  定时器0启动停止控制
  * @param  Flag 启动停止标志，1为启动，0为停止
  * @retval 无
  */
void Timer0_Run(unsigned char Flag);

#endif
