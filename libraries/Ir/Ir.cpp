#include "Ir.h"
#include "../RingBuffer/RingBuffer.h"

/*
 * Hardware allocations:
 *
 * Timer 0: RX.
 * Timer 1: TX.
 * Timer 2: LED PWM.
 */

/**
 * TX ring buffer.
 */
RingBuffer<IR_TX_BUFFER_SIZE> ir_tx_buffer;

/**
 * RX ring buffer.
 */
RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

/**
 * The Arduino pin connected to the IR LED. Set to 11 for OC2A.
 */
#define IR_PIN 11

void ir_init()
{
  /*
   * Configure IR output pin.
   */
  pinMode(IR_PIN, OUTPUT);

  /*
   * Configure PWM.
   */
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);

  /*
   * Signal toggles every run of the timer: Frequency = 8 MHz / (2 * 104) =
   * 38.46kHz. Timer starts disabled.
   */
  OCR2A = 104;
}
