
#ifdef UNIT_TEST

enum pinModes {
    INPUT_PULLUP,
    HIGH,
    LOW
};

void pinMode(uint8_t pin, pinModes mode) {};

long millis() {
    return 1000;
}

bool digitalRead(uint8_t pin) {
    return true;
}

uint8_t digitalPinToPort(uint8_t pin) {
    return 0;
}

uint8_t portOutputRegister(uint8_t pin) {
    return 0;
}

uint8_t digitalPinToBitMask(uint8_t pin) {
    return 0;
}

void cli() {

}

#endif