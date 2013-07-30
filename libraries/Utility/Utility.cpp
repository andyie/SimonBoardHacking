#include "Arduino.h"
#include "Utility.h"

/**
 * Mapping from Simon pin to Arduino pin.
 *
 * Mapping:
 *
 * 1: Upper left button LED.
 * 2: Upper right button LED.
 * 3: Lower left button LED.
 * 4: Lower right button LED.
 * 5: A0 pin.
 * 6: A1 pin.
 * 7: A2 pin.
 * 8: A3 pin.
 * 9: A4 pin.
 * 10: A5 pin.
 */
static unsigned int simon_led_pin_mapping[] =
{
  UPPER_LEFT_LED_ARDUINO,
  UPPER_RIGHT_LED_ARDUINO,
  LOWER_LEFT_LED_ARDUINO,
  LOWER_RIGHT_LED_ARDUINO,
  A0,
  A1,
  A2,
  A3,
  A4,
  A5
};

static unsigned int simon_button_pin_mapping[] =
{
  UPPER_LEFT_BUTTON_ARDUINO,
  UPPER_RIGHT_BUTTON_ARDUINO,
  LOWER_LEFT_BUTTON_ARDUINO,
  LOWER_RIGHT_BUTTON_ARDUINO
};

static bool valid_simon_led_pin(unsigned int simon_pin)
{
  // Range [1, dim(simon_led_pin_mapping)] okay.
  return simon_pin >= 0 && simon_pin < DIM(simon_led_pin_mapping);
}

static bool valid_simon_button_pin(unsigned int simon_pin)
{
  // Range [1, dim(simon_button_pin_mapping)] okay.
  return simon_pin >= 0 && simon_pin < DIM(simon_button_pin_mapping);
}

void wait_this_many_seconds(float seconds)
{
  delay((unsigned int)(seconds * 1000));
}

bool turn_on_light(unsigned int simon_pin)
{
  if (!valid_simon_led_pin(simon_pin))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin], HIGH);
  return true;
}

bool turn_off_light(unsigned int simon_pin)
{
  if (!valid_simon_led_pin(simon_pin))
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
  for (unsigned int i = 0; i <= 4; ++i) {
    if (!turn_on_light(i)) {
      return false;
    }
  }
  return true;
}

bool is_button_pressed(unsigned int button)
{
  if (!valid_simon_button_pin(button))
  {
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
