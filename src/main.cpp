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

#include <avr/pgmspace.h>

// custom includes
#include <DebounceButton.h>
#include <DebounceButtonController.h>
#include <FastShiftOut.h>

// defines
#define MOSI 11 // ---> Shift Register Serial Data Input / DS (14)
#define SCK 13 // ---> Shift Register Clock Input / SHCP (11)
#define SHIFTOUT1 0 // slave select ---> STCP (12)


// Fast Shiftout
FastShiftOut fo = FastShiftOut(SCK, MOSI);

// Button collection
// const uint8_t buttonCount = 10;
// DebounceButton buttons[buttonCount];

const PROGMEM PinToAction buttonPins[4] = {
    {1, SAS},
    {2, RCS},
    {3, GEAR},
    {4, LIGHT},
}; // pins buttons are on

// the button controller
DebounceButtonController bc = DebounceButtonController();

long lastBlink = millis();
bool ledState = HIGH;
uint8_t ledPin = 13;

#ifndef UNIT_TEST
#include <Arduino.h>
#include <SPI.h>

// put your setup code here, to run once:
void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);


    bc.configure(buttonPins, 4);
    // Serial.println("Registering Buttons");
    // for (int i = 0; i<buttonCount; i++)
    // {
    //     bc.setButton(buttonPins[i].pin, buttonPins[i].type);
    // }

    SPI.begin(); // start SPI
}

// shift bytes out onto the bus, make sure to
// latch the correct device before caling this.
void updateShiftOut(uint8_t value) {
  digitalWrite(SHIFTOUT1, LOW);
  fo.ShiftOut(value);
  digitalWrite(SHIFTOUT1, HIGH);
}

// blink every 1000 millis
void blink()
{
  if (millis() - lastBlink > 1000) {
    ledState = !ledState;
    lastBlink = millis();
    digitalWrite(ledPin, ledState);
  }
}


void loop() {


    for (int i = 0; i<bc.numberButtons(); i++)
    {
        
      if (bc.getButton(i).btn.triggered())
      {
        Serial.print(i);
        Serial.println(" triggered");
      }
    }

    // check all buttons
    // for (int i = 0; i<buttonCount; i++)
    // {
    //   if (buttons[i].triggered())
    //   {
    //     Serial.print(i);
    //     Serial.println(" triggered");
    //   }
    // }
    // ButtonToAction triggered[NUM_BUTTONS] = &bc.toggled();

    blink();
}

#endif