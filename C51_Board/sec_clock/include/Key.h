#ifndef __KEY_H__
#define __KEY_H__

#include "mcs51/8052.h"
#include "Delay.h"


unsigned char Key();

void Key_Loop(void);

uint8_t key_get_generate();

#endif
