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
#include "bridgeDriverPWM.h"

#ifndef OUTPUT_PIO0_CH0  
#define OUTPUT_PIO0_CH0 0
#define OUTPUT_PIO0_CH1 2
#endif


int main()   
{
    stdio_init_all();
    sleep_ms(1000);  // just to give time for Serial to initialise

    printf("System clock: %d MHz", SYS_CLK_MHZ);      

    printf("\n\nOuputs on GPIO0 and GPIO1\n\n");
 
    //uint32_t init_vals = 0<<16 | 0;
    uint32_t init_vals = 197<<16 | 47;
    

    bridgeDriverPWM bridge_driver0( OUTPUT_PIO0_CH0, init_vals);     // if pio not specified, use pio0
    bridge_driver0.enable(0, true);
    sleep_ms(2000);

    bridge_driver0.setFrequency(init_vals);


}