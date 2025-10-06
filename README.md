# ReflexDACExample_4_1V_2PntCal
This example Arduino sketch is meant to work with Anabit's Reflex DAC 14 Bit 2MSPS bipolar output +/- 4.096 
open source reference design

Product link: https://anabit.co/products/reflex-dac-14-bit-2msps-4v-range/

This example sketch demonstrates how to output a single bipolar voltage value from the anabit Reflex DAC.
With the option to apply a two point calibration correction factor to compenstate for the DAC and op amp's
gain and offset error. You can learn more about the two point calibration method and how to use this sketch
from the following links. 
Anabit YouTube video: https://youtu.be/0ndLKB3INxk
Anabit website tutorial: 
https://anabit.co/blogs/data_converter_u/increasing-a-dacs-output-accuracy-using-the-two-point-calibration-method

Note this sketch should work with any Arduino board that supports the hardware SPI library. The Reflex DAC is 
driven over SPI using the Arduino board's default SPI pins, but the user can set the chip select pin.

Please report any issue with the sketch to the Anagit forum: 
https://anabit.co/community/forum/digital-to-analog-converters-dacs

Example code developed by Your Anabit LLC © 2025
Licensed under the Apache License, Version 2.0.
