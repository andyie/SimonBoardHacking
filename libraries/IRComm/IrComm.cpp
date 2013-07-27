#include "Arduino.h"
#include "SimonBoardHacking.h"

//TODO: Check correct pins

void ir_on()
{
  TCCR2A |= _BV(COM2A0);
}

void ir_off()
{
  TCCR2A &= ~_BV(COM2A0);
}

void tx_led_on()
{
  //TODO: Implement
}

void tx_led_off()
{
  //TODO: Implement
}

void rx_led_on()
{
  //TODO: Implement
}

void rx_led_off()
{
  //TODO: Implement
}

void connection_led_on()
{
  //TODO: Implement
}

void connection_led_off()
{
  //TODO: Implement
}

void enable_tx_timer()
{
  //TODO: Implement
  //Timer calls transmit_led_state
}

void disable_tx_timer()
{
  //TODO: Implement
}

void enable_rx_timer()
{
  //TODO:Implement
  //Timer calls take_symbol_sample()
}

void disable_rx_timer()
{
  //TODO: Implement
}

void transmit_led_state()
{
  if(next_tx_state < last_tx_state)
  {
    //There are still more states to send
    if(bitRead(tx_states, next_tx_state))
    {
      ir_on();
    }
    else
    {
      ir_off();
    }
  }
  else
  {
    //There are no more states to send
    disable_tx_timer();
  }
}

bool read_receiver_state()
{
  //TODO: Implement
  //Return 0 if receiver reads a LED low
  //Return 1 if receiver reads a LED high
}

void on_connection_detected()
{
  connection_led_on();
  enable_rx_timer();
}

void on_first_symbol_edge()
{
  rx_bit_index = 0;
  num_rx_low_samples = 0;
  num_rx_high_samples = 0;
  start_rx_timer();
}

void take_symbol_sample()
{
   if(read_receiver_state())
   {
      //Received IR LED is on
      num_rx_high_samples++;
   }
   else
   {
      //Received IR LED is off
      num_rx_low_samples++;
   }
   
   //TODO: Decide on thresholding for symbols
}

void stage_tx_sequence()
{
  //TODO: Enable global interrupts
  
  char tx_char;
  
  //If the buffer is not empty
  if(!tx_buffer.get_front(tx_char))
  {
    //Start symbol = 4 * UNIT_TIME
    bitClear(tx_states, 0);
    bitClear(tx_states, 1);
    bitClear(tx_states, 2);
    bitClear(tx_states, 3);

    //Symbol spacing
    bitWrite(tx_states, 4);
    bitWrite(tx_states, 5);
    
    uint8_t state_index = 6;
    
    
    for(uint8_t i = 0; i < 8; i++)
    {
      if(bitRead(tx_char, i))
      {
        //1 == 2 * UNIT_TIME
        bitClear(tx_states, state_index++);
        bitClear(tx_states, state_index++);
      }
      else
      {
        //0 == 1 * UNIT_TIME
        bitClear(tx_states, state_index++);
      }
      
      //Symbol spacing = 2 * UNIT_TIME
      bitWrite(tx_states, state_index++);
      bitWrite(tx_states, state_index++);
    }
    
    next_tx_state = 0;
    last_tx_state = state_index;
  }
  
  //TODO: Disable global interrupts
}

void store_rx_sequence()
{
  
}

char calculate_parity(char data)
{
  //TODO: Implement
}

int build_and_stage_datagram(char* chars, uint_8 num_chars)
{
  int error = 0;

  /*
   * Packet format:
   * [start byte] [length byte] [byte 0] [byte 1] … [byte N - 1] [checksum]
   */
  error = tx_buffer.put_back(DATAGRAM_START_BYTE) || error;
  error = tx_buffer.put_back((char*)num_chars) || error;
  
  for(uint_8 i = 0; i < num_chars; i++)
  {
    error = tx_buffer.put_back(chars[i]) || error;
  }
  
  char crc8 = calculate_crc8(chars, num_chars);
  
  error = tx_buffer.put_back(crc8) || error;
  
  return error;
}

void decode_datagram(char* & chars, uint_8 & num_chars)
{
  //TODO: Receive partial datagrams if calling decode_datagram too quickly?
  
  char current_char;
  int buffer_empty = rx_buffer.get_front(current_char);
  
  /*
   * Find a start byte:
   *
   * Loop until either no characters are left or 
   * until we find a start byte
   */
  while(!buffer_empty && current_char != DATAGRAM_START_BYTE)
  {
    buffer_empty = rx_buffer.get_front(current_char);
  }
  
  if(!buffer_empty)
  {
    //Found start byte, get the length byte
    uint8_t datagram_length;
    buffer_empty = rx_buffer.get_front((char)datagram_length);
    
    if(!buffer_empty && datagram_length > 0)
    {
      //Allocate space to store the datagram
      char* datagram_data = new char[datagram_length];
      
      //Pull the data bytes off the rx_buffer
      for(uint8_t i = 0; i < datagram_length && !buffer_empty; i++)
      {
        buffer_empty = rx_buffer.get_front(datagram_data[i]);
      }
      
      if(!buffer_empty)
      {
        //Got data bytes, now get the CRC
        char crc8;
        buffer_empty = rx_buffer.get_front(crc8);
        
        //If the CRC checks out, return the data
        if(crc8 == calculate_crc8(datagram_data, datagram_length)
        {
          chars = datagram_data;
          num_chars = datagram_length;
          return;
        }
      }
      
      //If the CRC doesn't check out, clear the allocated char array
      delete[] datagram_data;
    }
  }
  
  chars = NULL;
  num_chars = 0;
  
  return;  
}

char calculate_crc8(char* chars, uint8_t num_chars)
{
  //TODO: Implement
}

int transmit_chars(char* chars, uint_8 num_chars)
{
  build_and_stage_datagram(chars, num_chars);
}
 
void receive_chars(char* & chars, uint_8 & num_chars)
{
  decode_datagram(chars, num_chars);
}