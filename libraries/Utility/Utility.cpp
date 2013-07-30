#include "Arduino.h"
#include "Utility.h"
#include <util/delay.h>

/**
 * Mapping from Simon pin to Arduino pin.
 *
 * Mapping:
 *
 * 1: Upper left button LED.
 * 2: Upper right button LED.
 * 3: Lower right button LED.
 * 4: Lower left button LED.
 */
static unsigned int simon_led_pin_mapping[] =
{
  UPPER_LEFT_LED_ARDUINO,
  UPPER_RIGHT_LED_ARDUINO,
  LOWER_RIGHT_LED_ARDUINO,
  LOWER_LEFT_LED_ARDUINO
};

static unsigned int simon_button_pin_mapping[] =
{
  UPPER_LEFT_BUTTON_ARDUINO,
  UPPER_RIGHT_BUTTON_ARDUINO,
  LOWER_RIGHT_BUTTON_ARDUINO,
  LOWER_LEFT_BUTTON_ARDUINO
};

void wait_this_many_seconds(float seconds)
{
  _delay_ms((unsigned int)(seconds * 1000));
}

bool turn_on_light(unsigned int simon_pin)
{
  if (simon_pin >= DIM(simon_led_pin_mapping))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin], HIGH);
  return true;
}

bool turn_off_light(unsigned int simon_pin)
{
  if (simon_pin >= DIM(simon_led_pin_mapping))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin], LOW);
  return true;
}

bool turn_off_all_lights()
{
  for (unsigned int i = 0; i < 4; ++i) {
    if (!turn_off_light(i)) {
      return false;
    }
  }
  return true;
}

bool turn_on_all_lights()
{
  for (unsigned int i = 0; i < 4; ++i) {
    if (!turn_on_light(i)) {
      return false;
    }
  }
  return true;
}

bool is_button_pressed(unsigned int button)
{
  if (button >= DIM(simon_button_pin_mapping)) {
    // Nonexistent buttons are not pressed.
    return false;
  }

  return !digitalRead(simon_button_pin_mapping[button]);
}

void start_buzzing(unsigned int freq)
{
  tone(BUZZER1, freq);
}

void start_buzzing()
{
  start_buzzing(DEFAULT_BUZZ_FREQUENCY);
}

void stop_buzzing()
{
  noTone(BUZZER1);
}
