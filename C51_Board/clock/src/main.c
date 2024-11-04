#include "LCD1602.h"
#include "matrixKey.h"

int main() {
    Timer0_Init();
    LCD_Init();

    while (1) {
        LCD_ShowString(1, 1, "Hello, Momo!");
    }
    return 0;
}

// 在中断函数中尽量只做简单的操作，不然内存和时间很容易不够0
void time0() __interrupt(TF0_VECTOR) {
    static unsigned int T0Count;
    // 否则写一个记时的外部变量，来控制终断的时间，给予更多的时间

//    TL0 = 0x18;		//设置定时初值
//    TH0 = 0xFC;		//设置定时初值值
    TL0 = 0x18;				//设置定时初始值
    TH0 = 0xFC;				//设置定时初始值

    // 1ms x 1000 = 1s
    T0Count++;      // 发生一次终端就会来到这个函数，然后记数 + 1；发生 1k 次就是 1s 了，然后执行我们要做的

    if (T0Count >= 1000) {
        T0Count = 0;
        P2_0 ^= 1;
    }
}
