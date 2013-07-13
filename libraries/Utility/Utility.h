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
void wait_this_many_seconds(float seconds);

// LED functions.
bool turn_on_LED(unsigned int simon_pin);
bool turn_off_LED(unsigned int simon_pin);
bool turn_off_all_LEDs();
bool turn_on_all_LEDs();

// Button functions.
bool is_button_pressed(unsigned int button);

// Tone functions.
void start_buzzing();
void start_buzzing(unsigned int freq);
void stop_buzzing();

#endif
