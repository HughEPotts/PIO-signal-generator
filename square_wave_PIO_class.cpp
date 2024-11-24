/* 
 * Square wave signal generator, using square_PIO
 * Generates square waves on two PIOs, giving a total of 
 * 6 independantly contraollable square wave outputs.
 * Max channels possible:
 *      pico1: 8 channels
 *      pico2: 12 channels

*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "square_PIO.h"

#ifndef OUTPUT_PIO0_CH0  
#define OUTPUT_PIO0_CH0 0
#define OUTPUT_PIO0_CH1 1
#define OUTPUT_PIO0_CH2 2
#define OUTPUT_PIO0_CH3 3
#define OUTPUT_PIO1_CH0 4
#define OUTPUT_PIO1_CH1 5
#endif


int main()
{
    stdio_init_all();
    sleep_ms(1000);  // just to give time for Serial to initialise
    printf("\n\nLow frequency demo\n");
    printf("Loading square.pio onto pio0, with initial frequency of 1000 Hz\n");
 
    SquarePIO sig_gen0( OUTPUT_PIO0_CH0, 1000);     // if pio not specified, use pio0
    printf("Adding 3 extra channels to pio0 at 1001Hz, 1002Hz and 1003Hz\n");
    sig_gen0.addChannel(OUTPUT_PIO0_CH1, 1001);
    sig_gen0.addChannel(OUTPUT_PIO0_CH2, 1002);
    sig_gen0.addChannel(OUTPUT_PIO0_CH3, 1003);

    printf("\nHigh frequency demo\n");
    printf("Loading square.pio onto pio1, with ;initial requested frequency of 15 Mhz\n");    
    SquarePIO sig_gen1(OUTPUT_PIO1_CH0, 15000000, pio1);    
    printf("Adding extra channel at 12.5 MHz");
    sig_gen1.addChannel(OUTPUT_PIO1_CH1, 12500000);         // actual frequency will be 125MHz/8 = 15.625MHz
    printf("Actual frequencies may be slightly different, as they must be divisors of the main system clock/2: \n"); 
    printf("   pio1, CH0: Requested frequency: %d Hz  Actual frequency: %d Hz\n", sig_gen1.getReqFrequency(0), sig_gen1.getFrequency(0) );
    printf("   pio2: CH1: Requested frequency: %d Hz  Actual frequency: %d Hz\n", sig_gen1.getReqFrequency(1), sig_gen1.getFrequency(1) );
    
    for (int i=0; i<=3; i++) {
        sig_gen0.enable(i);
    }
    for (int i=0; i<=1; i++) {
        sig_gen1.enable(i);
    }
    
    int step = 1;
    uint32_t freq = 0;
    bool pulse = true;

    printf("\nModulating outputs - use a scope:\n");
    printf("\n   Frequency sweeping pio0 outputs\n");
    printf("\n   Gating pio1 outputs\n");
    
    while (true) {
        sleep_us(500);
        // sweep the frequency on pio channels 0-3, keeping them 1Hz from each other
        // at these low frequencies the error will be less than 1Hz
        for (int i=0; i<=3; i++) {
            sig_gen0.setFrequency(i, 1000 + freq + i);
        }
        if (freq > 10000) step = -1;
        if (freq < 1000) step =  1;        
        freq += step;

        // Alternately pulse the output for the two pio2 channels
        if (pulse) {
            sig_gen1.enable(0);
            sig_gen1.disable(1);
        } else {
            sig_gen1.disable(0);
            sig_gen1.enable(1);
        }
        pulse = !pulse;
            
    }

}