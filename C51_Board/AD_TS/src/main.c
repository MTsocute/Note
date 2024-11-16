#include "Delay.h"
#include "XPT2046.h"
#include "LCD1602.h"


// 8 bit
#define XPT2046_VBAT	0xAC
#define XPT2046_AUX		0xEC
#define XPT2046_XP		0x9C	//0xBC
#define XPT2046_YP		0xDC

uint16_t AD_Val = 0;

int main() {

    LCD_Init();

    LCD_ShowString(1,1,"ADJ  NTC  GR");

    while (1) {
        //读取AIN0，可调电阻
        AD_Val=XPT2046_ReadAD(XPT2046_XP);
        LCD_ShowNum(2,1,AD_Val,3);			//显示AIN0

        //读取AIN1，热敏电阻
        AD_Val=XPT2046_ReadAD(XPT2046_YP);
        LCD_ShowNum(2,6,AD_Val,3);			//显示AIN1

        //读取AIN2，光敏电阻
        AD_Val=XPT2046_ReadAD(XPT2046_VBAT);
        LCD_ShowNum(2,11,AD_Val,3);			//显示AIN2
        Delay(100);
    }
}
