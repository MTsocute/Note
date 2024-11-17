//
// Created by shuhe on 2024/11/16.
//

#include "Int0.h"


void Int0_Init(void) {
    IT0 = 1;  // 配置外部中断0为下降沿触发
    IE0 = 0;  // 清除外部中断0的中断请求标志位
    EX0 = 1;  // 使能外部中断0
    EA = 1;   // 总中断开关，使能所有中断
    PX0 = 1;  // 设置外部中断0的优先级为高优先级
}