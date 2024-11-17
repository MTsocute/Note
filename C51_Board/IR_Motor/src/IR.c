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

void demo() {
    if (IR_State == 0)                   // 状态0，空闲状态
    {
        Timer0_SetCounter(0);    //定时计数器清0
        Timer0_Run(1);            //定时器启动
        IR_State = 1;                //置状态为1
    } else if (IR_State == 1)        //状态1，等待Start信号或Repeat信号
    {
        IR_Time = Timer0_GetCounter();    //获取上一次中断到此次中断的时间
        Timer0_SetCounter(0);    //定时计数器清0
//如果计时为13.5ms，则接收到了Start信号（判定值在12MHz晶振下为13500，在11.0592MHz晶振下为12442）
        if (IR_Time > 12442 - 500 && IR_Time < 12442 + 500) {
            P2 = ~P2;
            IR_State = 2;            //置状态为2
        }
//如果计时为11.25ms，则接收到了Repeat信号（判定值在12MHz晶振下为11250，在11.0592MHz晶振下为10368）
        else if (IR_Time > 10368 - 500 && IR_Time < 10368 + 500) {
            P2 = ~P2;
            IR_RepeatFlag = 1;    //置收到连发帧标志位为1
            Timer0_Run(0);        //定时器停止
            IR_State = 0;            //置状态为0
        } else                    //接收出错
        {
            IR_State = 1;            //置状态为1
        }
    } else if (IR_State == 2)        //状态2，接收数据
    {
        IR_Time = Timer0_GetCounter();    //获取上一次中断到此次中断的时间
        Timer0_SetCounter(0);    //定时计数器清0
//如果计时为1120us，则接收到了数据0（判定值在12MHz晶振下为1120，在11.0592MHz晶振下为1032）
        if (IR_Time > 1032 - 300 && IR_Time < 1032 + 300) {
            IR_Data[IR_pData / 8] &= ~(0x01 << (IR_pData % 8));    //数据对应位清0
            IR_pData++;            //数据位置指针自增
        }
//如果计时为2250us，则接收到了数据1（判定值在12MHz晶振下为2250，在11.0592MHz晶振下为2074）
        else if (IR_Time > 2074 - 300 && IR_Time < 2074 + 300) {
            IR_Data[IR_pData / 8] |= (0x01 << (IR_pData % 8));    //数据对应位置1
            IR_pData++;            //数据位置指针自增
        } else                    //接收出错
        {
            IR_pData = 0;            //数据位置指针清0
            IR_State = 1;            //置状态为1
        }
        if (IR_pData >= 32)        // 如果接收到了32位数据
        {
            IR_Address = IR_Data[0];    //转存数据
            IR_Command = IR_Data[2];
            IR_DataFlag = 1;    //置收到连发帧标志位为1
            IR_pData = 0;            //数据位置指针清0
            Timer0_Run(0);        //定时器停止
            IR_State = 0;            //置状态为0
        }
    }
}