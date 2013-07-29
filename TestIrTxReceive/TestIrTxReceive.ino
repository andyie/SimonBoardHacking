#include <stdlib.h>
#include <Ir.h>
#include <RingBuffer.h>

static char data[128];

extern RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

void setup()
{
  ir_init();
  Serial.begin(57600);
}

void loop()
{
  
  //Serial.println("Started");
  
  size_t len = 128;
  
  transmit_chars("Hello world", 11);
  
  
  char c;
  int st = ir_rx_buffer.get(c);
  
  //Serial.print("Status = ");
  //Serial.println(st);
  
  /*
  if (st == 0)
  {
      Serial.print("c = ");
      Serial.println((unsigned char)c, HEX);
  }
  */
    
  receive_chars(data, len);
  
  Serial.print("len = ");
  Serial.println(len, DEC);
  
  for (size_t i = 0; i < len; ++i)
  {
    Serial.print(data[i]);
  }
  
}
