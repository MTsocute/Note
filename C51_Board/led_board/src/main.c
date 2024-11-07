#include "my_utility.h"


int main() {
    while(1) {
        light_nixie(1, 1);
        delay_ms(1);

        light_nixie(2, 4);
        delay_ms(1);

        light_nixie(3, 5);
        delay_ms(1);
    }
    return 0;
}
