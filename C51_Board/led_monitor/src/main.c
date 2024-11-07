#include "LCD1602.h"


int main() {
    LCD_Init();
    LCD_ShowDecimal(1, 1, -12);       // 在 LCD 的第一行第一列显示十进制数
    LCD_ShowBinary(2, 1, 255);        // 在 LCD 的第二行第一列显示二进制数
//    LCD_ShowHexadecimal(2, 1, 255);   // 在 LCD 的第三行第一列显示十六进制数

    return 0;
}
