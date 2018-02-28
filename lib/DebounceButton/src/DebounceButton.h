/*
  Called repeatedly to detect changes on pins debouncing signals
*/

#ifndef DebounceButton_h
#define DebounceButton_h

#ifndef UNIT_TEST
#include "Arduino.h"
#endif

class DebounceButton
{
public:
  inline DebounceButton() {}

  inline bool triggered()
  {
    bool state = digitalRead(_pin);
    if (state != _lastState)
    {
       _lastState = state;
       _lastStateTransisionTime = millis();
      return true;
    } else {
      return false;
    }
  }

  inline void init(int pin)
  {
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
    triggered();
  }
protected:
  uint8_t _pin;
  bool _lastState;
  long _lastStateTransisionTime;
};

#endif
