#include <Ir.h>

#define IR_RECV_PIN A5

extern bool tx_schedule[64];
extern size_t tx_schedule_len;

void setup()
{
  ir_init();

  pinMode(IR_RECV_PIN, INPUT);

  Serial.begin(57600);
}

void loop() {
  if (digitalRead(IR_RECV_PIN) == LOW)
  {
    Serial.println("0");
  } else {
    Serial.println("1");
  }
}
