#include <stdlib.h>
#include <Ir.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  /* TX test
  char data;
  
  transmit_chars("hello", 5);
  
  for (int i = 0; i < 8; i++)
  {
    pop_tx_char(data);
    Serial.println(data, HEX);
  }
  
  Serial.println();
  Serial.println();
  */

  char* data;
  size_t len;  
  
  /*
  for(size_t i = 0; i < 8; i++)
  {
    char c;
    pop_rx_char(c);
    Serial.println(c, HEX);
  }
  */
  
  receive_chars(data, len);
  
  
  Serial.print("len = ");
  Serial.println(len, DEC);
  
  
  for (size_t i = 0; i < len; i++)
  {
    Serial.print(data[i]);
  }
  
  Serial.println();
  
  
  delay(1000);
}
