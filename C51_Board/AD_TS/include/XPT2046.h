//
// Created by shuhe on 2024/11/16.
//

#ifndef AD_TS_XPT2046_H
#define AD_TS_XPT2046_H

#include "mcs51/8052.h"
#include <stdint.h>

// 角针
__sbit __at(0xB0 ^ 4) XPT2046_DIN;
__sbit __at(0xB0 ^ 5) XPT2046_CS;
__sbit __at(0xB0 ^ 6) XPT2046_DLCK;
__sbit __at(0xB0 ^ 7) XPT2046_DOUT;




uint16_t XPT2046_ReadAD(uint16_t command);


#endif //AD_TS_XPT2046_H
