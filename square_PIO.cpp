#include "square_PIO.h"
#include "square.pio.h"      //header generated by CMakeLists RENAME!

//SquarePIO::SquarePIO(PIO PIO_instance, uint pin, uint32_t initial_frequency) {
SquarePIO::SquarePIO(uint pin, uint32_t initial_frequency) {
    _pio = pio0;   
    _pin = pin;
    _freq = initial_frequency;

    _sm = 0;        // Maually uses state machine 0? CHECK
    _offset = pio_add_program(_pio, &square_program);
    square_program_init(_pio, _sm, _offset, _pin);
    _actual_freq = setFrequency(_freq);
}

uint32_t SquarePIO::setFrequency(uint32_t frequency) {
    // sends the coutner value using the FIFO register. We could also write directly
    // see blink pio example, or just make a blank project with pio enabled

    // we can only generate frequencies that are divisors of the system clock/2 (ie 62.5Mhz)
    // with a min divisor of 4 when PIO_counter_val=0
    // with a max freq of 15.6MHz (clock/8)  when PIO_counter_val = 0
    //                    12.4MHz (clock/10) when PIO_counter_val = 1
    //                    10.4MHz (clock/12) when PIO_counter_val = 2
    // the frequency is then given by: freq = 62500000/(PIO_counter_val+4)
    // or PIO_counter_val = (62500000/freq) - 4
    // error is less than 1Hz up to 7976Hz, error is always positive 
    // we can probably reduce the error by makin it symmetrical
    
    uint32_t PIO_counter_val = 62500000/frequency - 4;
    uint32_t actual_freq = 62500000/(PIO_counter_val+4);
    pio_sm_put_blocking(_pio, _sm, PIO_counter_val); 
    return actual_freq; 
}

void SquarePIO::enable() {
    // enable the output
    pio_sm_set_enabled(_pio, _sm, true);
}

void SquarePIO::disable() {
    // disable the output and set low. Ideally finish the timing cycle first?
    pio_sm_set_enabled(_pio, _sm, false);
    pio_sm_set_pins(_pio, _sm, 0);
}


