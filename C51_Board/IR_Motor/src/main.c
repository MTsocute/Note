#include "Timer1.h"
#include <stdint.h>
#include "IR.h"
#include "Motor.h"
#include "LCD1602.h"

unsigned char SPEED = 0, T0_Count;
unsigned char COMPARE;

int main() {
    LCD_Init();
    LCD_ShowString(1, 1, "SPEED: ");

    Motor_Init();
    IR_Init();

    while (1) {
        unsigned char d1 = IR_DataFlag;
        IR_DataFlag = 0;

        Motor_SetSpeed(SPEED);

        if (d1) {
            LCD_ClearRegion(2, 1, 16);
            if (IR_Command == IR_POWER) { SPEED = 0; }
            else if (IR_Command == IR_VOL_ADD && SPEED < 3) SPEED++;
            else if (IR_Command == IR_VOL_MINUS && SPEED > 0) SPEED--;
        }

        Motor_SetSpeed(SPEED);
        LCD_ShowNum(2, 3, SPEED, 2);

    }
}

void Int0_Routine(void) __interrupt(0) {
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

void Timer1_Routine(void) __interrupt(3) {
    Motor_PWM();
}
