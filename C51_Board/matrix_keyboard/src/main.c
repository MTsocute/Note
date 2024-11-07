#include "LCD1602.h"
#include "matrixKey.h"

unsigned char key_num;
unsigned int passwd = 0;
unsigned int count = 0;

// 清除密码函数
void ClearPasswd() {
    passwd = 0;
    count = 0;
    LCD_ClearRegion(2, 1, 16); // 清除显示区域
}

int main() {
    DelayXms(20);
    LCD_Init();

    LCD_ShowString(1, 1, "Password:");

    while (1) {
        key_num = MatrixKey();
        if (key_num != 0) {
            // 如果是数字键（1-9），且密码输入未满4位
            if (key_num <= 10 && count < 4) {
                passwd = passwd * 10 + (key_num % 10);  // 构建密码
                count++;
                LCD_ShowNum(2, 1, passwd); // 显示当前输入的密码
            }

            // 当按下确认键（key_num == 11 as confirm）且已输入4位密码
            if (key_num == 11) {
                if (count == 4) {
                    if (passwd == 1234) LCD_ShowString(1, 14, "OK"); // 显示成功
                    else LCD_ShowString(1, 13, "ERR"); // 显示错误
                    // 显示一段时间后清除
                    DelayXms(120);
                    LCD_ClearRegion(1,13,3);
                    ClearPasswd(); // 清除密码重新输入
                }
                else {
                    LCD_ShowString(2, 1, "Less than 4!");
                    DelayXms(120); // 提示后清除
                    LCD_ClearRegion(2, 1, 16);
                    LCD_ShowString(2, 1, "Try Again!");
                    // 清除密码重新输入
                    DelayXms(120);
                    ClearPasswd();
                }
            }
        }
    }
}
