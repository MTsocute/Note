#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"
#include "Key.h"
#include "../../power_machine/include/Timer0.h"
#include "AT24C02.h"

float T, TShow;
int TLow, THigh;
uint8_t KeyNum;

void Temp_OP() {
    DS18B20_ConvertT();
    Delay_X_ms(1000);
    THigh = AT24_ReadByte(0);
    TLow = AT24_ReadByte(1);

    if (THigh > 125 || TLow < -55 || THigh <= TLow) {
        THigh = 20;
        TLow = 15;
    }

    LCD_Init();
    LCD_ShowString(1, 1, "T:");
    LCD_ShowString(2, 1, "TH:");
    LCD_ShowString(2, 9, "TL:");
    LCD_ShowSignedNum(2, 4, THigh, 3);
    LCD_ShowSignedNum(2, 12, TLow, 3);
}

void Temp_Show() {
    DS18B20_ConvertT();    //转换温度
    T = DS18B20_ReadT();    //读取温度
    if (T < 0)                //如果温度小于0
    {
        LCD_ShowChar(1, 3, '-');    //显示负号
        TShow = -T;        //将温度变为正数
    }
    //如果温度大于等于0
    else
    {
        LCD_ShowChar(1, 3, '+');    //显示正号
        TShow = T;
    }
    //显示温度整数部分
    LCD_ShowNum(1, 4, (uint8_t) TShow, 3);
    // 显示小数点
    LCD_ShowChar(1, 7, '.');
    //显示温度小数部分
    LCD_ShowNum(1, 8, (unsigned long) (TShow * 100) % 100, 3);
}

void Key_OP(uint8_t Key_num) {
    /*阈值判断及显示*/
    if (Key_num) {
        if (Key_num == 1)    //K1按键，THigh自增
        {
            THigh++;
            if (THigh > 125) { THigh = 125; }
        }
        if (Key_num == 2)    //K2按键，THigh自减
        {
            THigh--;
            if (THigh <= TLow) { THigh++; }
        }
        if (Key_num == 3)    //K3按键，TLow自增
        {
            TLow++;
            if (TLow >= THigh) { TLow--; }
        }
        if (Key_num == 4)    //K4按键，TLow自减
        {
            TLow--;
            if (TLow < -55) { TLow = -55; }
        }
        //显示阈值数据
        LCD_ShowSignedNum(2, 4, THigh, 3);
        LCD_ShowSignedNum(2, 12, TLow, 3);
        //写入到At24C02中保存
        AT24_WriteByte(0, THigh);
        Delay_X_ms(5);
        AT24_WriteByte(1, TLow);
        Delay_X_ms(5);
    }

    if (T > THigh)            //越界判断
    {
        LCD_ShowString(1, 13, "OV:H");
    } else if (T < TLow) {
        LCD_ShowString(1, 13, "OV:L");
    } else {
        LCD_ShowString(1, 13, "    ");
    }
}

int main() {
    Temp_OP();
    Timer0_Init();

    while (1) {
        /*温度读取及显示*/
        KeyNum = Key();
        Temp_Show();

        /*阈值判断及显示*/
        Key_OP(KeyNum);
    }
}

// 对于有中断的程序，是很容易影响温度传感器电流变换的延迟的
// 再有需要延迟的函数内容执行操作的时候，记得关闭定时器，然后出来的时候再开启
void Timer0_Routine() __interrupt(1) {
    static unsigned int T0Count;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0Count++;
    if (T0Count >= 20) {
        T0Count = 0;
        Key_Loop();
    }
}