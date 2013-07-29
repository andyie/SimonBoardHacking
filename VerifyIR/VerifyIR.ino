#include <Ir.h>
#include <RingBuffer.h>

extern RingBuffer<IR_TX_BUFFER_SIZE> ir_tx_buffer;
extern RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

extern bool samples[];
extern size_t sample_cursor;
extern int tripcount;

bool schedule[64];
size_t schedule_len = 0;

void setup()
{
  ir_tx_buffer.put('A');
  ir_tx_buffer.put('B');
  ir_tx_buffer.put('C');

  ir_init();

  Serial.begin(57600);
}

size_t cursor = 0;

void loop() {
  char c;
  if (ir_rx_buffer.get(c) == 0) {
    Serial.print("got rx: ");
    Serial.println(c);
  }

  /*
  while(cursor < sample_cursor) {
    Serial.print("sample: ");
    Serial.println((int)samples[cursor]);
    cursor++;
  }
  */

  /*
  static bool done = false;

  if (!done && schedule_len > 0) {
    for (size_t i = 0; i < schedule_len; ++i) {
      Serial.print("schedule: ");
      Serial.println((int)schedule[i]);
    }

    done = true;
  }
  */
}
