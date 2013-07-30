/**
 * @brief IR communications library.
 *
 * Includes all layers.
 */

#ifndef IR_H
#define IR_H

#include <stddef.h>

#define IR_TX_BUFFER_SIZE 128
#define IR_RX_BUFFER_SIZE 128

void ir_init();

int ir_read(char *buf, size_t &len);
int ir_write(const char *buf, size_t len);

#endif
