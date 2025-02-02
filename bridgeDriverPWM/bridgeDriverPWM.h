#ifndef BRIDGE_DRIVER_PWM_H_INCLUDED
#define BRIDGE_DRIVER_PWM_H_INCLUDED

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

class bridgeDriverPWM {
  public:
    bridgeDriverPWM(uint pin, uint32_t initial_frequency, uint32_t initial_duty, PIO pio=pio0);     //pio0 or pio1
    bool addChannel(uint pin, uint32_t initial_frequency, uint32_t initial_duty);
    uint32_t setFrequency(uint channel, uint32_t frequency);
    uint32_t setFrequency(uint32_t frequency) { return setFrequency(0, frequency);};  // default to ch0
    
    uint32_t setDuty(uint channel, uint duty1000);  

    bool setFreqDuty(uint channel, uint32_t frequency, uint32_t duty1000);

    void setTimingRegister(uint channel, uint32_t regval);                      // manually set timing registers
    void setTimingRegister(uint channel, uint16_t on_time, uint16_t off_time) {setTimingRegister(channel, (uint32_t) ((on_time-3)<<16 | (off_time-3)));};   // manually set timing registers
    
    uint32_t getReqFrequency(uint channel=0) {return _requested_freq[channel];};  
    uint32_t getFrequency(uint channel=0) {return _actual_freq[channel];};

    uint32_t getReqDuty(uint channel=0) {return _requested_freq[channel];};  
    uint32_t getDuty(uint channel=0) {return _actual_freq[channel];};

    void enable(uint channel, bool state);
    void enable(bool state) {enable(0, state);}; 

    void allOff(void);

  private:
    PIO _pio;         // which PIO we are using pio0 or pio1
    uint _offset;     // memory offset of the program
    uint _pin[4];
    uint32_t _requested_freq[4] = {0};    // set frequency
    uint32_t _actual_freq[4]    = {0};    // actual frequency
    uint32_t _requested_duty[4] = {0};    // set frequency
    uint32_t _actual_duty[4]    = {0};    // actual frequency
    uint32_t _regval[4]         = {0};
    uint _n_channels;   // number of active channels
    uint32_t _calculateTimingRegisters(uint32_t freq, uint32_t duty);
};

#endif
