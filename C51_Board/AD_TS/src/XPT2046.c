//
// Created by shuhe on 2024/11/16.
//

#include "XPT2046.h"

uint16_t XPT2046_ReadAD(uint16_t command) {
    uint16_t AD_Val = 0;
    XPT2046_DLCK = 0;
    XPT2046_CS = 0;

    for (int i = 0; i < 8; ++i) {
        XPT2046_DIN = command & (0x80 >> i);
        XPT2046_DLCK = 1;
        XPT2046_DLCK = 0;
    }

    for (int i = 0; i < 16; ++i) {
        XPT2046_DLCK = 1;
        XPT2046_DLCK = 0;
        if (XPT2046_DOUT == 0)  { AD_Val |= (0x8000 >> i); }
    }

    return AD_Val >> 8;
}
