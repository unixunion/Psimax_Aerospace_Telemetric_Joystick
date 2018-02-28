/*
  Called repeatedly to detect changes on pins debouncing signals
*/

#ifndef DebounceButtonController_h
#define DebounceButtonController_h

#include "DebounceButton.h"
#ifndef UNIT_TEST
#include "Arduino.h"
#endif
// #include <HashMap.h>

const byte NUM_BUTTONS = 25;  //define the max size arrays

// all the button functions
enum buttonsTypes {
  SAS, 
  RCS,
  GEAR,
  LIGHT,
  STAGE,
  AG1,
  AG2,
  AG3,
  AG4,
  AG5,
  AG6,
  AG7,
  AG8,
  AG9,
  AG10
};

// pin to action struct
struct PinToAction {
  uint8_t pin;
  buttonsTypes type;
} ;

// a buton instance to a type matching
struct ButtonToAction {
  DebounceButton btn;
  buttonsTypes type;
} ;

class DebounceButtonController
{
public:
  inline DebounceButtonController() {};

  inline void configure(const PinToAction *pta, uint8_t arraySize) 
  {
    for (uint8_t i = 0; i < arraySize; i++) {
      setButton(pta[i].pin, pta[i].type);
    }
  };

  inline void setButton(int pin, buttonsTypes btnType) 
  {
    _buttons[_buttonIndex] = {DebounceButton(), btnType};
    _buttons[_buttonIndex].btn.init(pin);
    pinMode(pin, INPUT_PULLUP);
    _buttonIndex++;
  };

  inline uint8_t numberButtons() 
  {
    return _buttonIndex;
  }

  inline ButtonToAction getButton(uint8_t btnIndex) 
  {
    return _buttons[btnIndex];
  }

  // inline ButtonToAction * toggled() 
  // {
  //   uint8_t toggledIndx = 0;
  //   _toggledButtons[NUM_BUTTONS] = {};

  //   for (int i = 0; i<_buttonIndex; i++) {
  //     if (_buttons[i].btn.triggered()) {
  //       _toggledButtons[toggledIndx] = _buttons[i];
  //       toggledIndx++;
  //     }
  //   }

  //   return _toggledButtons;
  // }

private:
  ButtonToAction _buttons[NUM_BUTTONS] = {};
  // ButtonToAction _toggledButtons[NUM_BUTTONS] = {};
  uint8_t _buttonIndex = 0;
};

#endif
