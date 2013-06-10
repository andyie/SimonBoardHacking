#ifndef UTILITY_H
#define UTILITY_H

#include "hardware_versions.h"

// Button LED definitions.
#define UPPER_LEFT_LED LED_RED
#define UPPER_RIGHT_LED LED_GREEN
#define LOWER_LEFT_LED LED_BLUE
#define LOWER_RIGHT_LED LED_YELLOW

// Button definitions.
#define UPPER_LEFT_BUTTON BUTTON_RED
#define UPPER_RIGHT_BUTTON BUTTON_GREEN
#define LOWER_LEFT_BUTTON BUTTON_BLUE
#define LOWER_RIGHT_BUTTON BUTTON_YELLOW

// Buzzer definitions.
#define DEFAULT_BUZZ_FREQUENCY 400

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
void startBuzzing();
void startBuzzing(unsigned int freq);
void stopBuzzing();

#endif
