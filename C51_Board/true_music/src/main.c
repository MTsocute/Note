#include "nixie.h"
#include "key.h"
#include "Buzzer.h"

uint8_t Key;

int main() {
    // 初始默认第一个为 0
    light_nixie(1, 0);

    while (1) {
        Key = key_num();

        if (Key) {
            Buzzer_Time(100);
            light_nixie(1, Key);
        }
    }
}

