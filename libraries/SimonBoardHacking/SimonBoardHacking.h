#ifndef SIMON_BOARD_HACKING_H
#define SIMON_BOARD_HACKING_H

// Arduino-required functions.
extern "C" void setup();
extern "C" void loop();

// Time seconds.
void waitThisManySeconds(float seconds);

// LED functions.
bool turnOnLED(unsigned int simon_pin);
bool turnOffLED(unsigned int simon_pin);
bool turnOffAllLEDs();
bool turnOnAllLEDs();

// Button functions.
bool isButtonPressed(unsigned int button);

// Tone functions.
void buzz(unsigned int freq);

#endif
