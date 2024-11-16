//
// Created by shuhe on 2024/11/16.
//

#include "IR.h"

void IR_Init(void) {
    Timer0_Init();
    Int0_Init();
}

unsigned char IR_GetAddress(void) {
    return IR_Address;
}

unsigned char IR_GetCommand(void) {
    return IR_Command;
}

