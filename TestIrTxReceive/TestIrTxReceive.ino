#include <Ir.h>
#include <RingBuffer.h>

static const char str[] = "Hello, world";
static char buf[128];

extern RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

void setup()
{
  Serial.begin(57600);

  ir_init();
  ir_write(str, sizeof(str));
}

void loop()
{
  size_t len = sizeof(buf);
  if (ir_read(buf, len) == 0) {
    Serial.println("Got message: ");
    for (size_t i = 0; i < len; ++i) {
      Serial.print(buf[i]);
    }
    Serial.println();
  }
}
