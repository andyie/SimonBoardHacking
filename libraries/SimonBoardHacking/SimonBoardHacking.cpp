#include "Arduino.h"
#include "SimonBoardHacking.h"
#include "hardware_versions.h"

#define UPPER_LEFT_LED LED_RED
#define UPPER_RIGHT_LED LED_GREEN
#define LOWER_LEFT_LED LED_BLUE
#define LOWER_RIGHT_LED LED_YELLOW

#define DIM(array) (sizeof(array) / sizeof((array)[0]))
#define LAST_LED_INDEX (DIM(simon_led_pin_mapping))

extern void repeat();

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
unsigned int simon_led_pin_mapping[] =
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

void waitThisManySeconds(float seconds)
{
  delay((unsigned int)(seconds * 1000));
}

bool valid_simon_pin(unsigned int simon_pin)
{
  // Range [1, dim(simon_led_pin_mapping)] okay.
  return simon_pin <= DIM(simon_led_pin_mapping);
}

bool turnOnLED(unsigned int simon_pin)
{
  if (!valid_simon_pin(simon_pin))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin - 1], HIGH);
  return true;
}

bool turnOffLED(unsigned int simon_pin)
{
  if (!valid_simon_pin(simon_pin))
  {
    return false;
  }

  digitalWrite(simon_led_pin_mapping[simon_pin - 1], LOW);
  return true;
}

bool turnOffAllLEDs()
{
  for (unsigned int i = 1; i <= LAST_LED_INDEX; ++i)
  {
    turnOffLED(i);
  }
}

bool turnOnAllLEDs()
{
  for (unsigned int i = 0; i <= LAST_LED_INDEX; ++i)
  {
    turnOnLED(i);
  }
}

void setup()
{
  pinMode(UPPER_LEFT_LED, OUTPUT);
  pinMode(UPPER_RIGHT_LED, OUTPUT);
  pinMode(LOWER_LEFT_LED, OUTPUT);
  pinMode(LOWER_RIGHT_LED, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
}

void loop()
{
  repeat();
}
