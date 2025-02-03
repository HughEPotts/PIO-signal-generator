/*
 * Square wave signal generator, using square_PIO
 * Generates square waves on two PIOs, giving a total of
 * 6 independantly contraollable square wave outputs.
 * Max channels possible:
 *      pico1: 8 channels
 *      pico2: 12 channels

*/

#include "bridgeDriverPWM.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#ifndef OUTPUT_PIO0_CH0
#define OUTPUT_PIO0_CH0 10
#define OUTPUT_PIO0_CH1 1 2
#endif

int main() {
    stdio_init_all();
    sleep_ms(1000); // just to give time for Serial to initialise

    printf("System clock: %d MHz", SYS_CLK_MHZ);

    printf("\n\nOuputs on GPIO0 and GPIO1\n\n");

    // uint32_t init_vals = 0<<16 | 0;
    uint32_t init_vals = 197 << 16 | 47;

    // simplest usage
    bridgeDriverPWM bridge_driver0(OUTPUT_PIO0_CH0, 250000, 100); // if pio not specified, use pio0
    bridge_driver0.enable(0, true);

    while (true) {
        // set maximum frequency, minimum duty - shiould be on for 3 clock cycles
        bridge_driver0.setFreqDuty(bridge_driver0.max_freq, 1);
        sleep_ms(1000);
        // set maximum frequency, maximum duty - shiould be off for 3 clock cycles
        bridge_driver0.setFreqDuty(bridge_driver0.max_freq, 1000);
        sleep_ms(1000);

        // ramp min-max duty at high freq, showing coarse steps
        bridge_driver0.setDuty(0); // working
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 1000; i++) {
                bridge_driver0.setDuty(0, i);
                sleep_ms(1);
            }
        }
        // set to 20kHZ and change the frequency at constant 50% duty
        uint32_t f_start = 20000;
        uint32_t f_stop = 100000;
        uint32_t f_step = 50;
        bridge_driver0.setFreqDuty(f_start, 500);
        sleep_ms(1000);
        for (int j = 0; j < 3; j++) {
            for (uint32_t i = f_start; i < f_stop; i+=f_step) {
                bridge_driver0.setFrequency(i);
                sleep_ms(1);
            }
        }
        
        // change duty and frequency to keep the on time constant, but change the duty by changing the frequency
        // demonstrates using timing registers manually - cam be useful for driving systems with multiple resonances
        uint32_t ontime = (SYS_CLK_HZ * 10) / 1e6;  // (10.0us on time)
        for (int j = 0; j < 3; j++) {
            for (uint32_t offtime = 10*ontime; offtime >0; offtime--) {
                bridge_driver0.setTimingRegister(0, ontime, offtime);   // note channel number (0) is needed
                sleep_ms(1);
            }
        }

    }
}