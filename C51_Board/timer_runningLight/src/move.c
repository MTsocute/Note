//
// Created by shuhe on 2024/11/4.
//

#include "matrixKey.h"


void crol(unsigned char *value, unsigned char bits) {
    *value = (*value << bits) | (*value >> (8 - bits));
}

void cror(unsigned char *value, unsigned char bits) {
    *value = (*value >> bits) | (*value << (8 - bits));
}
