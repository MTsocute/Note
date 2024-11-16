#include "LED_SHOW.h"

int main() {
    LED_Init();  // 初始化LED控制

    int totalFrames = sizeof(kun_kun_Dance) / sizeof(kun_kun_Dance[0]); // 计算总帧数

    while (1) {
        unsigned int delayTime = 15; // 控制显示的延时

        for (int offset = 0; offset < totalFrames; offset += 8) {
            // 逐帧过渡
            for (unsigned char i = 0; i < 8; i++) {
                if (i + offset < totalFrames) {
//                    MatrixLED_ShowColumn(i, kun_kun_Dance[i + offset]);
                    MatrixLED_ShowColumn(i, spongebob[i]);
                }
                else { break; /* 超出有效范围时停止 */  }
            }
            // 在每一帧之间进行过渡
            DelayXms(delayTime); // 在每个帧之间增加延时
        }
    }
}
