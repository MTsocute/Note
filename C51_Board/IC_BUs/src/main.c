#include "LCD1602.h"
#include "AT24C02.h"
#include "Key.h"

unsigned int Num = 0;
unit8_t key_num;


int main() {
    LCD_Init();
    LCD_ShowString(1, 1, "Hello, World!");

    while (1) {
        key_num = Key();

        if (key_num == 1) {
            LCD_ClearRegion(2,0,16);
            Num ++;
            LCD_ShowNum(2,1, Num);
        }

        if (key_num == 2) {
            LCD_ClearRegion(2,0,16);
            Num --;
            LCD_ShowNum(2,1, Num);
        }

        if (key_num == 3) {
            AT24_WriteByte(0, Num % 256);   // 写入低字节
            Delay(5);                           // 延时，确保数据稳定
            AT24_WriteByte(1, Num / 256);   // 写入高字节
            Delay(5);                           // 延时，确保数据稳定
            LCD_ShowString(2, 1, "Write OK");  // 显示 "Write OK" 在 LCD 屏幕上
            Delay(1000);                        // 延时 1 秒
            LCD_ClearRegion(2,1, 16);
        }

        if (key_num == 4) {
            // Num 是 16 位数据，所以下面是把两个地址存入的数据合并成一个
            Num = AT24_ReadByte(0);           // 读取低字节
            Num |= AT24_ReadByte(1) << 8;     // 读取高字节并合并
            LCD_ShowString(2, 1, "Read OK ");    // 显示 "Read OK" 在 LCD 屏幕上
            Delay(1500);
            LCD_ClearRegion(2,1,16);
            LCD_ShowNum(2, 1, Num);           /* 在 LCD 上显示 Num 的值 */
            Delay(2000);
            LCD_ClearRegion(2,1,16);
        }
    }
}

