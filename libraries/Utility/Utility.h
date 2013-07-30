#ifndef UTILITY_H
#define UTILITY_H

#include "hardware_versions.h"

/*
 * Custom pin mapping for these utility functions.
 */
#define upper_left_light 1
#define upper_right_light 2
#define lower_left_light 3
#define lower_right_light 4

/*
 * Button LED Arduino pin definitions.
 */
#define UPPER_LEFT_LED_ARDUINO LED_RED
#define UPPER_RIGHT_LED_ARDUINO LED_GREEN
#define LOWER_LEFT_LED_ARDUINO LED_BLUE
#define LOWER_RIGHT_LED_ARDUINO LED_YELLOW

/*
 * Button LED Arduino pin definitions.
 */
#define UPPER_LEFT_BUTTON_ARDUINO BUTTON_RED
#define UPPER_RIGHT_BUTTON_ARDUINO BUTTON_GREEN
#define LOWER_LEFT_BUTTON_ARDUINO BUTTON_BLUE
#define LOWER_RIGHT_BUTTON_ARDUINO BUTTON_YELLOW

/*
 * Buzzer definitions.
 */
#define DEFAULT_BUZZ_FREQUENCY 400

// Time seconds.
void wait_this_many_seconds(float seconds);

// LED functions.
bool turn_on_light(unsigned int simon_pin);
bool turn_off_light(unsigned int simon_pin);
bool turn_off_all_lights();
bool turn_on_all_lights();

// Button functions.
bool is_button_pressed(unsigned int button);

// Tone functions.
void start_buzzing();
void start_buzzing(unsigned int freq);
void stop_buzzing();

#endif
