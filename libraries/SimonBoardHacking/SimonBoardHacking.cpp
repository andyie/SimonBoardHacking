#include "Arduino.h"
#include "SimonBoardHacking.h"

extern void repeat();

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

  pinMode(UPPER_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(UPPER_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(LOWER_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(LOWER_RIGHT_BUTTON, INPUT_PULLUP);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  digitalWrite(BUZZER1, LOW);
}

void loop()
{
  repeat();
}
