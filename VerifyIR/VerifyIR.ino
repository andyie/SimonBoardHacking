#define V_OUT_PIN A5

void ir_init();

void setup()
{
  ir_init();

  pinMode(V_OUT_PIN, INPUT);

  Serial.begin(57600);
}

void loop()
{
  static uint32_t count = 0;
  static bool on = false;

  if (count % 1 == 0)
  {
    if (digitalRead(V_OUT_PIN) == HIGH)
    {
      Serial.println("ON");
    }
    else
    {
      Serial.println("OFF");
    }
  }

  if (count % 50 == 0)
  {
    if (!on)
    {
      ir_on();
      on = true;
    } else {
      ir_off();
      on = false;
    }
  }

  /*
   * Delay 10 ms = 100 Hz update
   */
  ++count;
  delay(10);
}

void ir_init()
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

void ir_on()
{
  TCCR2A |= _BV(COM2A0);
}

void ir_off()
{
  TCCR2A &= ~_BV(COM2A0);
}
