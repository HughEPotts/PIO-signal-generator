#ifndef BRIDGE_DRIVER_PWM_H_INCLUDED
#define BRIDGE_DRIVER_PWM_H_INCLUDED

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

class bridgeDriverPWM {
  public:
    bridgeDriverPWM(uint pin, uint32_t initial_frequency, PIO pio=pio0);     //pio0 or pio1
    bool addChannel(uint pin, uint32_t initial_frequency);
    uint32_t setFrequency(uint32_t frequency) { return setFrequency(0, frequency);};  // default to ch0
    uint32_t setFrequency(uint channel, uint32_t frequency);
    
    uint32_t getReqFrequency(uint channel=0) {return _requested_freq[channel];};  
    uint32_t getFrequency(uint channel=0) {return _actual_freq[channel];};

    void enable(uint channel, bool state);
    void enable(bool state) {enable(0, state);}; 

    void allOff(void);

  private:
    PIO _pio;         // which PIO we are using pio0 or pio1
    uint _offset;     // memory offset of the program
    uint _pin[4];
    uint32_t _requested_freq[4] = {0};    // set frequency
    uint32_t _actual_freq[4]    = {0};    // actual frequency
    uint _n_channels;   // number of active channels
};

#endif
