/********************************************************************************************************
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
********************************************************************************************************/

#include <SPI.h>

#define DAC_14_BIT 16383
#define REF_VOLT (float)4.096

// ===== Calibration (two-point) =====
// Enable to use calibrated inverse: C = (V - b)/a
#define CAL_ENABLE 1

// Fitted from: -3.3V -> -3.2986V @ code 1592, +3.3V -> +3.2943V @ code 14792
// Model: V = a*C + b   (V in volts, C in codes [0..16383])
//*** apply formula from tutorial and adjust these values for your DAC
static constexpr float CAL_A_V_PER_CODE = 0.0004996f;   // slope a [V/code]
static constexpr float CAL_B_V_OFFSET   = -4.09526f;     // offset b [V]

/* ---------- Chip Select pin override if needed) ---------- */
#define PIN_CS 10      

/* ---------- Single Value parameters ---------- */
float outputThisVoltage = 3.300;

/* ---------- Globals ---------- */
// Set SPI clock rate, bit order, and mode 
SPISettings dacSPI(40000000UL, MSBFIRST, SPI_MODE0);   

void setup()
{
    pinMode(PIN_CS, OUTPUT);
    digitalWrite(PIN_CS, HIGH);

    SPI.begin();                       // default hardware SPI pins
    SPI.beginTransaction(dacSPI);

    uint16_t dacCode = dacBipolarVoltageToCode(14, REF_VOLT, outputThisVoltage); // calibrated mapping
    digitalWrite(PIN_CS, LOW);
    SPI.transfer16(dacCode); //set the DAC output value
    digitalWrite(PIN_CS, HIGH);
}

void loop()
{
   
}

// This converts a bipolar voltage in [-Vref, +Vref] into a 14-bit offset-binary code.
// If CAL_ENABLE==1, it uses the calibrated inverse C = (V - b)/a; else ideal mapping.
uint16_t dacBipolarVoltageToCode(uint8_t bits, float vref, float vout)
{
  if (bits == 0) return 0;
  if (bits > 14) bits = 14;

  const uint32_t maxCode = ((uint32_t)1u << bits) - 1u;  // 16383
  const float vfs = vref;                                // ±vref range
  if (vout >  vfs) vout =  vfs;
  if (vout < -vfs) vout = -vfs;

  #if CAL_ENABLE
    // Calibrated inverse: code = (V - b) / a
    float c = (vout - CAL_B_V_OFFSET) / CAL_A_V_PER_CODE;
    if (!isfinite(c)) c = 0.0f;
    if (c < 0.0f) c = 0.0f;
    if (c > (float)maxCode) c = (float)maxCode;
    int32_t code_i = (int32_t)(c + 0.5f);
    if (code_i < 0)          code_i = 0;
    if (code_i > (int32_t)maxCode) code_i = (int32_t)maxCode;
    return (uint16_t)code_i;
  #else
    // Ideal offset-binary mapping
    uint32_t halfScale = (uint32_t)1u << (bits - 1);     // 8192
    float code_f = (vout / vref) * (float)halfScale + (float)halfScale;
    int32_t code_i = (int32_t)(code_f + 0.5f);
    if (code_i < 0)          code_i = 0;
    if (code_i > (int32_t)maxCode) code_i = (int32_t)maxCode;
    return (uint16_t)code_i;
  #endif
}
