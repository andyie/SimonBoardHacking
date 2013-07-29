#include <Ir.h>
#include <RingBuffer.h>

extern RingBuffer<IR_TX_BUFFER_SIZE> ir_tx_buffer;
extern RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

void setup()
{
  ir_tx_buffer.put('A');
  ir_tx_buffer.put('B');
  ir_tx_buffer.put('C');

  ir_init();

  Serial.begin(57600);
}

void loop() {
  char c;
  if (ir_rx_buffer.get(c) == 0) {
    Serial.print("got rx: ");
    Serial.println(c);
  }
}
