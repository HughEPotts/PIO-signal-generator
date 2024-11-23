#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "square_PIO.h"


#ifndef OUTPUT_PIN
#define OUTPUT_PIN 0
#endif


int main()
{
    stdio_init_all();
    printf("Hello Start");
    uint32_t freq = 100;
    SquarePIO sig_gen(OUTPUT_PIN, freq);
    while (true) {
        printf("Hello..%d\n", freq);
        sleep_ms(10);
        sig_gen.enable();
        sleep_ms(10);
        sig_gen.disable();
        sig_gen.setFrequency(freq);
        freq++;
        
    }
}
