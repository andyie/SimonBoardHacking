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
void ir_led_on();
void ir_led_off();

#endif
