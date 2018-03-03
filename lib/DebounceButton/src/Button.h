/*

http://en.cppreference.com/book/intro/abstract_classes
http://www.cplusplus.com/forum/articles/10627/

*/

#ifndef Button_h
#define Button_h

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include <Arduino.h>
#endif

#if defined(__AVR_ATmega32U4__)
#include <AStar32U4.h>
#endif

#define DEBOUNCE_DELAY 50
#define VIRTUALSWITCH_DEBOUNCETIME 125

// button base class
class Button
{
    public:
        inline Button(uint8_t pin) {
            _pin = pin;
        }; 
        inline void init()
        {
            pinMode(_pin, INPUT_PULLUP);
        }
        int read();
    protected:
        uint8_t _pin;
};

// momentary debouncing switch
class Momentary : public Button
{
    public:
        inline Momentary(uint8_t pin) : Button(pin) {};
        inline void init() {
            Button::init();
        }
        int read();
    protected:
        int _state;
        int _lastState;
        long _lastTransitionTime;
};

// momentary button which has switch like holding of a virtual state
// default LOW, press once, HIGH, press again, LOW...
class VirtualSwitch : public Button
{
    public:
        inline VirtualSwitch(uint8_t pin) : Button(pin)
        {
            _value = HIGH;
            _lastTransitionTime = millis();
        };
        inline void init() {
            Button::init();
        }
        int read();
    protected:
        int _lastState;
        int _value;
        long _lastTransitionTime;
};

#endif