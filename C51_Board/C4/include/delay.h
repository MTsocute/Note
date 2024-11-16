//
// Created by shuhe on 2024/11/5.
//

#ifndef TRUE_MUSIC_DELAY_H
#define TRUE_MUSIC_DELAY_H

void _nop_() {
    __asm nop __endasm;  // 执行一个空操作指令
}

void Delay500us(void);

void delay_ms(unsigned int xms);

#endif //TRUE_MUSIC_DELAY_H
