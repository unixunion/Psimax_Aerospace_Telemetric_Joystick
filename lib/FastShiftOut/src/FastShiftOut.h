#ifndef UNIT_TEST
#include "Arduino.h"
#endif

#ifndef FastShiftOut_h
#define FastShiftOut_h

/*

SPI_MOSI = dataPin;
dataHigh = (1 << (dataPin-8)); // Used for Or'ing on data bits
dataLow  = ~dataHigh;          // Used for And'ing off data bits

SPI_CLK   = clkPin;
clockHigh = (1 << (clkPin-8)); // Used for Or'ing the clock pin High
clockLow  = ~clockHigh;        // Used for And'ing the clock pin Low

//The following seems backwards but that's the way.  Enabled = Low on CS, Disabled = High on CS
SPI_CS = csPin;
ChipSelectLatch = (1 << (csPin-8)); // High - Used to Or CS High thereby latching putting data into the display
ChipSelectLoad = ~ChipSelectLatch;  // Low -  Used to And CS Low thereby allowing data loading.

void LedControl::shiftOutFast(uint8_t val) // Write a byte by software SPI, MSB first.
{
  for (uint8_t i = 8; i; i--) {          // Run through the 8 data bits outputting them while clocking.

    PORTB &= clockLow;                   // Set Clock pin low.
    PORTB &= dataLow;                    // Set Data pin low (might change quite quickly!)
    if (val & 0x80) PORTB |= dataHigh;   // Data proved to be high, set Data pin High now.
    PORTB |= clockHigh;                  // Set Clock pin high. Data now captured in unit.
    val <<= 1;                           // Shift user data byte one left for next time.
  }
}  // end of shiftOutFast routine.


*/

class FastShiftOut
{
  public:
    inline FastShiftOut(int clockPin, int dataPin)
    {
      _clockPin = clockPin;
      _dataPin = dataPin;
    };
    inline void ShiftOut(uint8_t data)
    {
      // Fast shiftOut function
      volatile uint8_t *sclkPort = portOutputRegister(digitalPinToPort(_clockPin));
      volatile uint8_t *mosiPort = portOutputRegister(digitalPinToPort(_dataPin));
      uint8_t sclkMask = digitalPinToBitMask(_clockPin);
      uint8_t mosiMask = digitalPinToBitMask(_dataPin);

      uint8_t oldSREG = SREG;
      cli();
      for (uint8_t i = 0; i < 8; ++i)
      {
       if (data & 0x80)
       {
         *mosiPort |= mosiMask;
       }
       else
       {
         *mosiPort &= ~mosiMask;
       }
       *sclkPort |= sclkMask;
       data <<= 1;
       *sclkPort &= ~sclkMask;
      }
      SREG = oldSREG;
    };
  private:
    int _clockPin;
    int _dataPin;
};

#endif
