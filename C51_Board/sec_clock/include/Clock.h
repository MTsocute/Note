//
// Created by shuhe on 2024/11/10.
//

#ifndef SEC_CLOCK_CLOCK_H
#define SEC_CLOCK_CLOCK_H

#include <stdint.h>
#include "Nixie.h"

uint8_t Min, Sec, MiniSec;
uint8_t RunFlag;      // 记时器开关


/**
  * @brief  秒表驱动函数，在中断中调用
  */
void Sec_Loop(void) {
    if (RunFlag) {
        MiniSec++;
        if (MiniSec >= 100) {
            MiniSec = 0;
            Sec++;
            if (Sec >= 60) {
                Sec = 0;
                Min++;
                if (Min >= 60)  Min = 0;
            }
        }
    }
}

void show_time() {
    Nixie_SetBuf(1,Min/10);	//设置显示缓存，显示数据
    Nixie_SetBuf(2,Min%10);
    Nixie_SetBuf(3,11);
    Nixie_SetBuf(4,Sec/10);
    Nixie_SetBuf(5,Sec%10);
    Nixie_SetBuf(6,11);
    Nixie_SetBuf(7,MiniSec/10);
    Nixie_SetBuf(8,MiniSec%10);
}


#endif //SEC_CLOCK_CLOCK_H
