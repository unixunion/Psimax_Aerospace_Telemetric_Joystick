// #ifndef DebounceButton_h
// #define DebounceButton_h

// // debounce time
// #define DEBOUNCE_TIME 250

// #ifndef UNIT_TEST
// #include "Arduino.h"
// #endif

// #ifdef UNIT_TEST
// #include <TestMock.h>
// #endif

// enum ButtonStyle {
//   MOMENTARY,
//   LATCHING
// };

// class DebounceButton
// {
// public:
//   inline DebounceButton(int pin)
//   {
//     _pin = pin;
//   }

//   // init the button, from setup phase of main -> buttoncontroller -> button
//   inline void init() {
//     pinMode(_pin, INPUT_PULLUP);
//     _lastStateTransisionTime = millis();
//     triggered();
//   }

//   // read pin
//   inline bool read() 
//   {
//     return digitalRead(_pin);
//   }

//   // return true if a state transition has occured
//   inline bool triggered()
//   {
//     bool state = digitalRead(_pin);

//     if (state != _lastState && (millis() - _lastStateTransisionTime) > DEBOUNCE_TIME)
//     {
//        _lastState = state;
//        _lastStateTransisionTime = millis();
//       return true;
//     } else {
//       _lastState = state;
//       return false;
//     }
//   }


// protected:
//   uint8_t _pin;
//   // ButtonStyle _style;
//   bool _lastState;
//   long _lastStateTransisionTime;
// };

// #endif
