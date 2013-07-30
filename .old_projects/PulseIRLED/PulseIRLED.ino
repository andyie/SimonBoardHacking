/*
 *Use PB3 (MOSI/OC2A/Pin 11 and Timer2 to control the PWM)
 */

void setup()
{
  pinMode(11, OUTPUT);

  // Set up PWM
  // Configure output on Pin 11 using OC2A as top limit for timer | Use Fast PWM | Use Fast PWM
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);

  // Timer top limit; Duty cycle is defaulted to 50%
  // Signal toggles every run of the timer: PWM frequency = 8 MHz / (2 * 104) =
  // 38.46kHz.
  OCR2A = 104;

  // Use OCR2A as top limit | Prescaler value of 1
  TCCR2B = _BV(WGM22) | _BV(CS20);
}

void loop()
{
  //Do nothing; PWM is already configured
}
