#ifndef SQUARE_PI0_H_INCLUDED
#define SQUARE_PI0_H_INCLUDED


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"


class SquarePIO {
  public:
 //   SquarePIO(PIO PIO_instance, uint pin, uint32_t initial_frequency);     //pio0 or pio1
    SquarePIO(uint pin, uint32_t initial_frequency);     //pio0 or pio1
    uint32_t setFrequency(uint32_t frequency);
    uint32_t getFrequency() {return _freq;};
    
    void enable(void);
    void disable(void);

  private:
    PIO _pio;         // which PIO we are using pio0 or pio1
    uint _sm;         // which state machine (4 available per PIO)
    uint _offset;     // memory offset of the program
    uint _pin;
    uint32_t _freq;    // set frequency
    uint32_t _actual_freq;    // actual frequency
    
};

#endif