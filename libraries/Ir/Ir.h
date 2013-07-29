/**
 * @brief IR communications library.
 *
 * Includes all layers.
 */

#ifndef IR_H
#define IR_H

#define IR_TX_BUFFER_SIZE 128
#define IR_RX_BUFFER_SIZE 128

void ir_init();

int transmit_chars(char* data, size_t len); 
void receive_chars(char* data, size_t & len);

#endif