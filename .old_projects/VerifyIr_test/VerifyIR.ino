#include <Ir.h>
#include <RingBuffer.h>

extern RingBuffer<IR_TX_BUFFER_SIZE> ir_tx_buffer;
extern RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

extern bool done;
extern bool samples[128];

void setup()
{
  ir_init();

  Serial.begin(57600);
}

void loop() {
  if (done) {
    done = false;
    for (size_t i = 0; i < sizeof(samples); ++i) {
      Serial.print("sample ");
      Serial.print(i, DEC);
      Serial.print(": ");
      Serial.println(samples[i], DEC);
    }

    while (1)
    {
    }
  }
}
