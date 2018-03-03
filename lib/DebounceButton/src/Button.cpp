
#include "Button.h"

// simples buttons dont do anything but return the signal.
int Button::read()
{
    return digitalRead(_pin);
}

// a virtual switch changes its state when pin goes from HIGH to LOW. 
// this exibits switch like behavior for momentary switches.
int VirtualSwitch::read()
{
    // create a holder for the temp state
    int tState;

    // if sufficient time has passed, do the actual read
    if ((millis() - _lastTransitionTime) > VIRTUALSWITCH_DEBOUNCETIME) 
    {
        tState = digitalRead(_pin);

        // if transitioned from HIGH to LOW flip the _value
        if (tState == LOW && _lastState == HIGH)
        {
            _lastTransitionTime = millis();
            _value = !_value;
        }
    }

    _lastState = tState;
    return _value;
}

// a momentary should always just return the pin state, debouncing too.
int Momentary::read()
{   
    int tState = digitalRead(_pin);

    if (tState != _lastState)
    {
        _lastTransitionTime = millis();
    }

    if ((millis() - _lastTransitionTime) > DEBOUNCE_DELAY)
    {
        if (tState != _lastState)
        {
            _state = tState;
        }
    }

    _lastState = tState;
    return _state;
}