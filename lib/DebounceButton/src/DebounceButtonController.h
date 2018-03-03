// /*
//   Manage buttons
// */

// #ifndef DebounceButtonController_h
// #define DebounceButtonController_h

// #include "DebounceButton.h"
// #ifndef UNIT_TEST
// #include "Arduino.h"
// #endif

// // all the button functions
// enum ActionType {
//   NONE,
//   SAS, 
//   RCS,
//   GEAR,
//   LIGHT,
//   STAGE,
//   AG1,
//   AG2,
//   AG3,
//   AG4,
//   AG5,
//   AG6,
//   AG7,
//   AG8,
//   AG9,
//   AG10
// };

// // a button instance to a type matching
// struct ButtonToAction {
//   DebounceButton btn;
//   ActionType type;
// } ;

// // the controller
// class DebounceButtonController
// {
// public:
//   inline DebounceButtonController() {};

//   // initialize the buttons, called from main::setup()
//   inline void init(ButtonToAction *bta, uint8_t arraySize) 
//   {
//     _bta = bta;
//     numButtons = arraySize;
//     for (int i = 0; i<numButtons; i++) {
//       _bta[i].btn.init();
//     }
//   }
 
//   // getter for numButtons initialized with
//   inline uint8_t numberButtons() 
//   {
//     return numButtons;
//   }

//   // get states of all buttons
//   inline ButtonToAction * read()
//   {
//     return _bta;
//   }

//   // return triggered actions, one per call
//   inline ActionType triggered() {
//     for (int i=0; i<numButtons; i++) {
//       if (_bta[i].btn.triggered()) {
//         return _bta[i].type;
//       }
//     }
//     return NONE;
//   }
// private:
//   ButtonToAction *_bta;
// protected:
//   uint8_t numButtons = 0;
// };

// #endif
