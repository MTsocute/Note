#include "Delay.h"


void Delay(uint16_t xms) {
    uint8_t i, j;
    while (xms--) {
        i = 2;
        j = 239;
        do {
            while (--j);
        } while (--i);
    }
}

