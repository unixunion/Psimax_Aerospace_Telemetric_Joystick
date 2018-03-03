
#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include <Arduino.h>
#endif

#if defined(__AVR_ATmega32U4__)
#include <AStar32U4.h>
#endif

#include "Button.h"

// all the button functions
enum ActionType {
  ATNONE,
  ATSAS, 
  ATRCS,
  ATLight,
  ATGear,
  ATBrakes,
  ATAbort,
  ATCustom01,
  ATCustom02,
  ATCustom03,
  ATCustom04,
  ATCustom05,
  ATCustom06,
  ATCustom07,
  ATCustom08,
  ATCustom09,
  ATCustom10,
};

// a button instance to a type matching
struct ButtonToAction {
  Button btn;
  ActionType type;
};

class ButtonController
{
    public:
        inline ButtonController() {};
        inline void init(ButtonToAction *bta, uint8_t arraySize) 
        {
            _bta = bta;
            numButtons = arraySize;
            for (int i = 0; i<numButtons; i++) {
            _bta[i].btn.init();
            }
        }
        inline uint8_t numberButtons() 
        {
            return numButtons;
        }
        inline ButtonToAction * read()
        {
            return _bta;
        }

    private:
        ButtonToAction *_bta;

    protected:
        uint8_t numButtons = 0;

};

#endif