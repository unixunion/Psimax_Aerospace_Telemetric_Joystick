/*

SPI 595
https://forum.arduino.cc/index.php?topic=149470.0

Schematic:
http://jumptuck.com/wp-content/uploads/2011/11/shift-register-hardware1.png

pinOutputReguster:
https://forum.arduino.cc/index.php?topic=75962.0

portRegster:
https://www.arduino.cc/en/Reference/PortManipulation

https://www.arduino.cc/en/Reference/SPI

**/

// custom includes
#include <DebounceButton.h>
#include <DebounceButtonController.h>
#include <FastShiftOut.h>
#include <Arduino_FreeRTOS.h>

// defines
#define MOSI 11 // ---> Shift Register Serial Data Input / DS (14)
#define SCK 13 // ---> Shift Register Clock Input / SHCP (11)
#define SHIFTOUT1 0 // slave select ---> STCP (12)

// Fast Shiftout
FastShiftOut fo = FastShiftOut(SCK, MOSI);

// Button to action mapping
const uint8_t const_numButtons = 4;
ButtonToAction buttonAction[const_numButtons] = {
    {DebounceButton(1), SAS},
    {DebounceButton(2), RCS},
    {DebounceButton(10), GEAR},
    {DebounceButton(7), LIGHT},
};

// the button controller
DebounceButtonController bc = DebounceButtonController();

// blinky
#define BLINK_LEDPIN 13
#define BLINK_INTERVAL 1000
long lastBlink = millis();
bool ledState = HIGH;

// test harness
#ifndef UNIT_TEST
#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>

// put your setup code here, to run once:
void setup() {
    pinMode(BLINK_LEDPIN, OUTPUT);
    Serial.begin(9600);
    bc.init(buttonAction, const_numButtons);
    SPI.begin();
}

// shift bytes out onto the bus, make sure to
// latch the correct device before caling this.
void updateShiftOut(uint8_t value) {
  digitalWrite(SHIFTOUT1, LOW);
  fo.ShiftOut(value);
  digitalWrite(SHIFTOUT1, HIGH);
}

// blink every BLINK_INTERVAL millis
void blink()
{
  if (millis() - lastBlink > BLINK_INTERVAL) {
    ledState = !ledState;
    lastBlink = millis();
    digitalWrite(BLINK_LEDPIN, ledState);
  }
}

// main program loop
void loop() {

  // get one triggered action
  ActionType action = bc.triggered();

  // match the action to control changes
  switch(action) {
    case NONE:
      break;
    case SAS:
      // toggle sas
      break;
    default:
      break;
    };

    // call blink
    blink();
}

#endif