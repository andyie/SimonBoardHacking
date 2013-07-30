#include <Ir.h>
#include <RingBuffer.h>

#define IR_RECEIVER_PIN A1
#define CONNECTION_INDICATOR_PIN A5

static const char str[] = "Hello, world";
static char buf[128];

extern RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

void setup()
{
  pinMode(CONNECTION_INDICATOR_PIN, OUTPUT);
  digitalWrite(CONNECTION_INDICATOR_PIN, LOW);

  Serial.begin(57600);

  ir_init();
}

void loop()
{
  static int count = 0;

  size_t len = sizeof(buf);
  if (ir_read(buf, len) == 0) {
    Serial.println("Got message: ");
    for (size_t i = 0; i < len; ++i) {
      Serial.print(buf[i]);
    }
    Serial.println();
  }

  if (count % 100 == 0) {
    if (ir_write(str, sizeof(str)) == 0) {
      Serial.println("Sending.");
    }
  }

  if (!digitalRead(IR_RECEIVER_PIN)) {
    digitalWrite(CONNECTION_INDICATOR_PIN, HIGH);
  } else {
    digitalWrite(CONNECTION_INDICATOR_PIN, LOW);
  }

  ++count;
}
