## PIO-signal-generator

This is a simple library to use the PIO processors on the Raspberry Pi Pico microcontrollers to generate square waves with exactly 50% duty cycle, and no processor overhead.

Each PIO unit (two on pico, three on pico 2) can generate 4 independent square wave outputs, maximum frequency of 125MHz/8 = 15.625MHz.(pico1) or 150MHz/8 = 18.75MHz

Simplest possible useage - single channel on pio0 (default):
```
#include "square50.h"
...

SquarePIO sig_gen0(0, 10000);   // 10kHz square wave on output 1 using pio0
sig_gen0.enable(true);          // enable output (false to disable)
sig_gen0.setFrequency(120000);  // set frequency to 120kHz
```

For a more complex example using multipe PIO processors with multiple output channels on each see `square50_example.cpp`.

Note: the frequency is always even integer divisors of system clock (125Mhz for pico1, 150MHz for pico2) which means that the actual frequency may not exactly match the requested frequency. 
The actual frequency is returned when the frequency is set. At frequencies <8kHz the error is less than 1Hz, and the actual output frequency is never less than the requested frequency.

This software is in an early state of development, and the following will be modified/improved/debuged in the near future:
* Correct output frequency for pico2 devices
* Overload 'setFrequency' to allow floating point frequencies (and hence frequencies <1Hz)  

 
