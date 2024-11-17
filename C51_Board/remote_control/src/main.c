#include "LCD1602.h"
#include "IR.h"

unsigned char Num;

uint8_t COMMAND, ADDRESS;

int main() {
    LCD_Init();
    LCD_ShowString(1, 1, "ADDR  CMD  NUM");
    LCD_ShowString(2, 1, "00    00   000");
    IR_Init();

    while (1) {
        if (IR_Get_DataFlag() || IR_Get_DataFlag()) {
            COMMAND = IR_GetCommand();
            ADDRESS = IR_GetAddress();

            LCD_ClearRegion(2,1,16);

            if (COMMAND == IR_VOL_ADD) Num ++;
            else if (COMMAND == IR_VOL_MINUS) Num --;

            LCD_ShowHexadecimal(2, 1, ADDRESS);
            LCD_ShowHexadecimal(2, 7, COMMAND);
            LCD_ShowNum(2, 12, Num, 3);
        }
    }
}

void Int0_Routine(void) __interrupt(0) {
    IR_Main();
}
