#include <stdio.h>
#include <stdint.h>
int main() {
    uint8_t x = 0;

    for (int i=0;i<255;i++) {
        x++;
    }
    printf("%d", x);
    return 0;
}
