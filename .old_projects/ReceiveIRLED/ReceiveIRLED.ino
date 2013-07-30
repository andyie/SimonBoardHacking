#define V_OUT_PIN A5

void setup()
{
  pinMode(V_OUT_PIN, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(V_OUT_PIN) == HIGH)
  {
    Serial.println("ON");
  }
  else
  {
    Serial.println("OFF");
  }
  
  /*
   * Delay 10 ms = 100 Hz update
   */
  delay(10);
}
