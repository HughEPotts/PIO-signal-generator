#ifndef BRIDGE_DRIVER_PWM_H_INCLUDED
#define BRIDGE_DRIVER_PWM_H_INCLUDED

#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

enum pwm_error_t {
    PWM_OK = 0,
    PWM_FREQ_TOO_HIGH = 11,
    PWM_FREQ_TOO_LOW = 12,
    PWM_DUTY_TOO_HIGH = 21,
    PWM_DUTY_TOO_LOW = 22,
    PWM_SET_TR_INVALID_COUNTS = 31,
    PWM_NOT_INITIALISED = 99
};

// for almist all functions the 'channel' argument is optional, and will default to channel 0 if not specified

class bridgeDriverPWM {
  public:
    bridgeDriverPWM(uint pin, uint32_t initial_frequency, uint32_t initial_duty, PIO pio = pio0); // pio0 or pio1
    bool addChannel(uint pin, uint32_t initial_frequency, uint32_t initial_duty);

    pwm_error_t setFrequency(uint channel, uint32_t frequency);
    pwm_error_t setFrequency(uint32_t frequency) { return setFrequency(0, frequency); }; // default to ch0

    pwm_error_t setDuty(uint channel, uint duty1000);
    pwm_error_t setDuty(uint duty1000) { return setDuty(0, duty1000); };

    pwm_error_t setFreqDuty(uint channel, uint32_t frequency, uint32_t duty1000);
    pwm_error_t setFreqDuty(uint32_t frequency, uint32_t duty1000) { return setFreqDuty(0, frequency, duty1000); };

    void setTimingRegister(uint channel, uint32_t regval);   // manually set timing registers, not normally used
    void setTimingRegister(uint channel, uint16_t on_time, uint16_t off_time); // as above but explicit on and off counts

    uint32_t getRequestedFrequency(uint channel = 0) { return _requested_freq[channel]; };
    uint32_t getFrequency(uint channel = 0);

    uint32_t getRequestedDuty(uint channel = 0) { return _requested_duty[channel]; };
    uint32_t getDuty(uint channel = 0);

    pwm_error_t get_status(uint channel = 0) { return _status[channel]; };
    void enable(uint channel, bool state);
    void enable(bool state) { enable(0, state); };

    void allOff(void);


    const uint32_t max_freq = SYS_CLK_HZ / (2*24);            // max frequency that allows 8 evenly spaced steps of PWM
    const uint32_t min_freq = SYS_CLK_HZ / (2 * 0xffff);      // min frequency with counter at max val

//           RP2040   RP2350
// max_freq  2.604    3.125      MHz
// min_freq  954      1144       Hz      
// XXXTODO: add in clock divider to extend to lower frequency ranges

  private:
    PIO _pio;     // which PIO we are using pio0 or pio1
    uint _offset; // memory offset of the program
    uint _pin[4];
    uint _n_channels;              // number of active channels
    uint32_t _requested_freq[4]; 
    uint32_t _requested_duty[4];
    uint32_t _regval[4];
    pwm_error_t _status[4]; 

    uint32_t _calculateTimingRegisters(uint32_t freq, uint32_t duty);
};

#endif
