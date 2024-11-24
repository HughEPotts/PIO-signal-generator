#include "square_PIO.h"
#include "square.pio.h"      //header generated by CMakeLists RENAME!

//TODO: add function to delete programs and free up PIO units

SquarePIO::SquarePIO(uint pin, uint32_t initial_frequency, PIO pio) {
    // load program onto pio and activate state machine sm0
    _pio = pio;   
    _pin[0] = pin;
    _offset = pio_add_program(_pio, &square_program);
    uint channel=0;  // channel 0 is always on state machine 0
    square_program_init(_pio, channel, _offset, _pin[0]);
    setFrequency(0, initial_frequency);
    _n_channels = 1;
}

bool SquarePIO::addChannel(uint pin, uint32_t initial_frequency) {
   //add additional square wave generators to other state machines within the PIO
   hard_assert (_n_channels <= 3);    // max 4 state machines (channels) per PIO
   uint new_channel = _n_channels;  
   pio_sm_claim(_pio, new_channel);   // put new channel on next available state machine
   square_program_init(_pio, new_channel, _offset, pin);
   
   setFrequency(new_channel, initial_frequency);
   _pin[new_channel] = pin;
   _n_channels ++;       // total number of state machines (channels) used on this PIO device

   return true;
}

uint32_t SquarePIO::setFrequency(uint channel, uint32_t frequency) {
    // sends the counter value using the FIFO register. We could also write directly
    // see blink pio example, or just make a blank project with pio enabled

    // we can only generate frequencies that are divisors of the system clock/2 (ie 62.5Mhz)
    // with a min divisor of 4 when PIO_counter_val=0
    // with a max freq of 15.6MHz (clock/8)  when PIO_counter_val = 0
    //                    12.4MHz (clock/10) when PIO_counter_val = 1
    //                    10.4MHz (clock/12) when PIO_counter_val = 2
    // the frequency is then given by: freq = 62500000/(PIO_counter_val+4)
    // or PIO_counter_val = (62500000/freq) - 4
    // error is less than 1Hz up to 7976Hz, error is always positive 
    // we can probably reduce the error by making it symmetrical
    uint32_t PIO_counter_val = 62500000/frequency - 4;
    _requested_freq[channel] = frequency;
    _actual_freq[channel] = 62500000/(PIO_counter_val+4);
    pio_sm_put_blocking(_pio, channel, PIO_counter_val); 
    return _actual_freq[channel] ;
}


void SquarePIO::enable(uint channel, bool state) {
    if (state==true) {
        // enable the output
        pio_sm_set_enabled(_pio, channel, true);
    } else {
        // disable the output and set low.
        pio_sm_set_enabled(_pio, channel, false);
        pio_sm_set_pins(_pio, channel, 0);
    }
}

void SquarePIO::allOff() {
    // disable all channels, and set all outputs low
    for (uint channel=0; channel<_n_channels; channel++) {
        enable(channel, false);
    }
}


