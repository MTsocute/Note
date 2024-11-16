#include "Timer0.h"
#include "delay.h"
#include "sheet.h"

static unsigned char FreqSelect = 0, MusicSelect = 0;


int main() {
    Timer0Init();
    while (1) {
        // 不是休止符才播放
        if (sky_city[MusicSelect] != MUSIC_STOP) {
            FreqSelect = sky_city[MusicSelect++];
            delay_ms(SPEED / 4 * sky_city[MusicSelect++]);

            // 确保两个音符之间不会过的太快
            TR0 = 0;     // 定时器关闭
            delay_ms(5);
            TR0 = 1;
        } else {
            TR0 = 0;
            while (1);
            // 你可以手动设置 FreqS 和 MusicS 再放一边
        }
    }
}

// 间隔时间名叫的频率
void Timer0_inrp() __interrupt(1) {
    if(FreqTable[FreqSelect])	//如果不是休止符
    {
        TL0 = FreqTable[FreqSelect] % 256;        //设置定时初值
        TH0 = FreqTable[FreqSelect] / 256;        //设置定时初值
        Buzzer = !Buzzer;
    }
}
