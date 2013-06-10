#include "Arduino.h"
#include "Utility.h"

// Other utilities.
#define DIM(array) (sizeof(array) / sizeof((array)[0]))
#define LAST_LED_INDEX (DIM(simon_led_pin_mapping))

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
  UPPER_LEFT_LED,
  UPPER_RIGHT_LED,
  LOWER_LEFT_LED,
  LOWER_RIGHT_LED,
  A0,
  A1,
  A2,
  A3,
  A4,
  A5
};

static unsigned int simon_button_pin_mapping[] =
{
  UPPER_LEFT_BUTTON,
  UPPER_RIGHT_BUTTON,
  LOWER_LEFT_BUTTON,
  LOWER_RIGHT_BUTTON
};

static bool valid_simon_led_pin(unsigned int simon_pin)
{
  // Range [1, dim(simon_led_pin_mapping)] okay.
  return simon_pin > 0 && simon_pin <= DIM(simon_led_pin_mapping);
}

static bool valid_simon_button_pin(unsigned int simon_pin)
{
  // Range [1, dim(simon_button_pin_mapping)] okay.
  return simon_pin > 0 && simon_pin <= DIM(simon_button_pin_mapping);
}

void waitThisManySeconds(float seconds)
{
  delay((unsigned int)(seconds * 1000));
}

bool turnOnLED(unsigned int simon_pin)
{
  if (!valid_simon_led_pin(simon_pin))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin - 1], HIGH);
  return true;
}

bool turnOffLED(unsigned int simon_pin)
{
  if (!valid_simon_led_pin(simon_pin))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin - 1], LOW);
  return true;
}

bool turnOffAllLEDs()
{
  for (unsigned int i = 1; i <= LAST_LED_INDEX; ++i) {
    if (!turnOffLED(i)) {
      return false;
    }
  }
  return true;
}

bool turnOnAllLEDs()
{
  for (unsigned int i = 1; i <= LAST_LED_INDEX; ++i) {
    if (!turnOnLED(i)) {
      return false;
    }
  }
  return true;
}

bool isButtonPressed(unsigned int button)
{
  if (!valid_simon_button_pin(button))
  {
    // Nonexistent buttons are not pressed.
    return false;
  }

  return !digitalRead(simon_button_pin_mapping[button - 1]);
}

void startBuzzing(unsigned int freq)
{
  tone(BUZZER1, freq);
}

void startBuzzing()
{
  startBuzzing(DEFAULT_BUZZ_FREQUENCY);
}

void stopBuzzing()
{
  noTone(BUZZER1);
}
