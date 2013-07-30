#include "Arduino.h"
#include "Ir.h"
#include "../RingBuffer/RingBuffer.h"
#include <stdint.h>
#include <stddef.h>

#include "HardwareSerial.h"

/*
 * Hardware allocations:
 *
 * Timer 0: TX.
 * Timer 1: RX.
 * Timer 2: LED PWM.
 */

/**
 * The Arduino pin connected to the IR LED. Set to 11 for OC2A. This must not
 * change without changes to the rest of the code.
 */
#define IR_LED_PIN 11

/**
 * The Arduino pin connected to the IR receiver. A1 = PCINT9, PC1. This must not
 * change without changes to the rest of the code.
 */
#define IR_RECEIVER_PIN A1

/**
 * The Arduino pin connected to the TX indicator LED.
 */
#define TX_INDICATOR_PIN A3

/**
 * The Arduino pin connected to the RX indicator LED.
 */
#define RX_INDICATOR_PIN A4

/*
 * TX definitions.
 */

/**
 * Start symbol length.
 */
#define START_SYMBOL_LENGTH 4

/**
 * Logic 0 symbol length.
 */
#define LOGIC_0_SYMBOL_LENGTH 1

/**
 * Logic 1 symbol length.
 */
#define LOGIC_1_SYMBOL_LENGTH 2

/**
 * Inter-symbol spacing.
 */
#define SYMBOL_GAP_LENGTH 2

/**
 * The TX timer top value. Each run of the timer should be UNIT_TIME.
 * CPU / 64 = 125kHz, and it takes 62.5 counts at 125kHz to reach 500us.
 */
#define TX_TIMER_TOP 63

/**
 * The IR LED timer top value. The signal toggles each run of the timer:
 * Frequency = 8 MHz / (2 * 104) = 38.46kHz.
 */
#define IR_TIMER_TOP 104

/*
 * RX definitions.
 */

/**
 * The number of samples to take per unit time. Note that if this value is
 * changed, the timer configuration must be changed to match.
 */
#define SAMPLES_PER_UNIT_TIME 16

/**
 * The minimum number of contiguous samples required to indicate that an edge
 * has occurred. If no noise is expected at all, this value may be set to 1.
 */
#define MIN_SAMPLES_FOR_EDGE 3

/**
 * The number of symbol samples which may be accepted before the current symbol
 * times out.
 */
#define MAX_SYMBOL_SAMPLES (8 * SAMPLES_PER_UNIT_TIME)

/**
 * The number of gap samples which may be accepted before the next symbol times
 * out.
 */
#define MAX_GAP_SAMPLES (2 * SYMBOL_GAP_LENGTH * SAMPLES_PER_UNIT_TIME)

/**
 * Fudge interval in sample counts. Naturally, this interval should not be so
 * large that a given sample count could be taken as more than one symbol.
 */
#define SAMPLE_FUDGE 6

/**
 * Start byte for all datagrams that are sent and received
 */
#define DATAGRAM_START_BYTE ((char)0xA7)

/*
 * RX timer top value. Each run of the timer should be UNIT_TIME / 16.
 * CPU = 8MHz, and it takes 250 counts at 8MHz to reach 500us / 16 = 31.25us.
 */
#define RX_TIMER_TOP 250

/**
 * The different symbol types.
 */
enum symbol_t {
  /**
   * Start.
   */
  symbol_start,

  /**
   * Logical 0.
   */
  symbol_0,

  /**
   * Logical 1.
   */
  symbol_1,

  /**
   * Unrecognized symbol.
   */
  symbol_unrecognized
};

/**
 * TX ring buffer.
 */
RingBuffer<IR_TX_BUFFER_SIZE> ir_tx_buffer;

/**
 * RX ring buffer.
 */
RingBuffer<IR_RX_BUFFER_SIZE> ir_rx_buffer;

/*
 * TX worker state.
 */

/**
 * Whether TX is already started. This is so that ir_tx_start() can be made
 * idempotent.
 */
bool tx_started = false;

/**
 * The current TX schedule. No word should take longer than 64 * UNIT_TIME to
 * transmit. Each entry indicates the signal assertion level at the given
 * UNIT_TIME block. In our case, the LED is off when asserted, and on during
 * gaps between symbols. Between words, the LED is on (unasserted).
 */
bool tx_schedule[64];

/**
 * The current TX schedule cursor.
 */
static size_t tx_schedule_cursor = 0;

/*
 * The TX schedule length.
 */
size_t tx_schedule_len;

/*
 * RX worker state.
 */

/**
 * The current signal that is being tracked by the receiver.
 */
static bool rx_level;

/**
 * The RX byte currently being received.
 */
static uint8_t rx_byte;

/**
 * Tracks the current symbol being received. Symbol 0 is the start symbol, and
 * symbol 9 is the parity symbol. The intervening symbols are data bits.
 */
static unsigned short rx_symbol_count;

/**
 * The number of contiguous samples that have indicated a constant level.
 */
static unsigned short rx_level_count;

/**
 * The number of contiguous samples what have indicated an edge has occurred. If
 * this count reaches MIN_SAMPLES_FOR_EDGE, the current symbol will be
 * concluded. If the signal returns to the original level before this count is
 * reached, this count is added to rx_level_count (the glitch is ignored).
 */
static unsigned short rx_edge_count;

/**
 * True if the current RX datagram has been synced. This means that the
 * datagram's start byte has been received, and the rest of the datagram is in
 * the process of being received.
 */
static bool rx_datagram_synced = false;

/**
 * The length of the datagram currently being received. Set to 0 if the length
 * byte has not been received yet.
 */
static size_t rx_datagram_len = 0;

static void ir_rx_init();
static void ir_tx_init();

static void ir_rx_start();
static void ir_rx_stop();

static void ir_tx_start();
static void ir_tx_stop();

static void ir_rx_edge_enable();
static void ir_rx_edge_disable();

static void ir_led_on();
static void ir_led_off();

static void tx_schedule_prepare(uint8_t tx_byte);
static void tx_schedule_append_symbol(symbol_t symbol);
static void tx_schedule_append_gap();
static void tx_schedule_append_pulse(bool level, size_t len);

symbol_t get_symbol_from_pulse(size_t count);

static char crc8(const char *buf, size_t len);

static int build_and_stage_datagram(char* chars, size_t num_chars);
static void decode_datagram(char* chars, size_t & num_chars);

/**
 * Initialize the IR communications module.
 */
void ir_init() {
  ir_tx_init();
  ir_rx_init();

  pinMode(TX_INDICATOR_PIN, OUTPUT);
  pinMode(RX_INDICATOR_PIN, OUTPUT);

  digitalWrite(TX_INDICATOR_PIN, LOW);
  digitalWrite(RX_INDICATOR_PIN, LOW);
}

/**
 * Initialize the RX part of the IR module.
 */
void ir_rx_init() {
  /*
   * Configure IR receiver pin as an input.
   */
  pinMode(IR_RECEIVER_PIN, INPUT);

  /*
   * Configure Timer 1 for sampling.
   */
  TCCR1A = 0;
  TCCR1B = _BV(WGM12);
  OCR1A = RX_TIMER_TOP;

  /*
   * Enable compare match interrupt.
   */
  TIMSK1 |= _BV(OCIE1A);

  /*
   * Enable pin change interrupt 1 for IR receiver falling edge detection. Only
   * enable PCINT9 (PC1, Arduino A1).
   */
  PCMSK1 |= _BV(PCINT9);

  ir_rx_edge_enable();
}

/**
 * Initialize the TX component of the IR module. Note that the IR pin PWM is on
 * all the time. When the IR LED is to be off, the GPIO pin is just muxed away
 * from the output compare to its default set value.
 */
void ir_tx_init() {
  /*
   * TX timer configuration.
   */

  /*
   * Configure Timer 0 for channel A CTC.
   */
  TCCR0A = _BV(WGM01);
  TCCR0B = 0;
  OCR0A = TX_TIMER_TOP;

  /*
   * Enable compare match interrupt.
   */
  TIMSK0 |= _BV(OCIE0A);

  /*
   * IR LED configuration.
   */

  /*
   * Configure IR output pin (Timer 2 PWM). When not overridden by timer, it is
   * set to off.
   */
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, LOW);

  /*
   * Configure PWM to count up to OCR2A and then toggle. The output compare pin
   * is not enabled yet.
   */
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = IR_TIMER_TOP;

  /*
   * Turn on the IR LED. This is the LED's passive state.
   */
  ir_led_on();
}

/**
 * Starts the RX worker.
 */
void ir_rx_start() {
  /*
   * Shine the indicator.
   */
  digitalWrite(RX_INDICATOR_PIN, HIGH);

  /*
   * Reset RX state. Since IR sampling should be started on a falling edge, the
   * starting level is false (low).
   */
  rx_level = false;
  rx_byte = 0;
  rx_symbol_count = 0;
  rx_level_count = 0;
  rx_edge_count = 0;

  /*
   * Reset the timer and set CPU/1 prescaling.
   */
  TCNT1 = RX_TIMER_TOP - 1;
  TCCR1B |= _BV(CS10);
}

/**
 * Stops the RX worker.
 */
void ir_rx_stop() {
  /*
   * Douse the indicator.
   */
  digitalWrite(RX_INDICATOR_PIN, LOW);

  /*
   * Disable the worker's interrupt.
   */
  TCCR1B &= ~_BV(CS10);

  /*
   * Enable the falling edge interrupt again.
   */
  ir_rx_edge_enable();
}

/**
 * Starts the TX worker.
 */
void ir_tx_start() {
  /*
   * Shine the indicator LED.
   */
  digitalWrite(TX_INDICATOR_PIN, HIGH);

  /*
   * Ignore repeated calls. This releases users from having to maintain TX
   * state. It also means TX can turn itself off without telling anyone.
   */
  if (tx_started) {
    return;
  }

  tx_started = true;

  /*
   * Reset TX state.
   */
  tx_schedule_cursor = 0;

  /*
   * Reset the timer and set CPU/64 prescaling.
   *
   * TODO: Initializing TCNT0 to TX_TIMER_TOP - 1 gives weird results. The
   * receiver only receives the last UNIT_TIME's worth of the start symbol.
   * Initializing to 0 is okay...
   */
  TCNT0 = 0;
  TCCR0B = _BV(CS01) | _BV(CS00);
}

/**
 * Stops the TX worker.
 */
void ir_tx_stop()
{
  /*
   * Douse the indicator LED.
   */
  digitalWrite(TX_INDICATOR_PIN, LOW);

  tx_started = false;
  TCCR0B &= ~(_BV(CS01) | _BV(CS00));
}

/**
 * Enables the RX edge interrupt.
 */
void ir_rx_edge_enable() {
  /*
   * Enable pin change interrupt 1.
   */
  PCICR |= _BV(PCIE1);
}

/**
 * Disables the RX edge interrupt.
 */
void ir_rx_edge_disable() {
  /*
   * Disable pin change interrupt 1.
   */
  PCICR &= ~_BV(PCIE1);
}

/**
 * Turn the IR LED on (at its modulation frequency).
 */
void ir_led_on() {
  /*
   * Override IR_LED_PIN and toggle OC2A on compare match.
   */
  TCCR2A |= _BV(COM2A0);
}

/**
 * Turn the IR LED off.
 */
void ir_led_off() {
  /*
   * Stop overriding IR_LED_PIN.
   */
  TCCR2A &= ~_BV(COM2A0);
}

/**
 * IR receiver edge ISR. If a falling edge has occurred, this interrupt will be
 * disabled, and the RX worker will be started.
 */
ISR(PCINT1_vect) {
  /*
   * The level indicates whether an IR signal is present. The negation is to
   * adjust for the IR receiver using negative logic.
   */
  bool level = !digitalRead(IR_RECEIVER_PIN);

  /*
   * The level is high. In practice, this should not occur. Anyway, let it be.
   */
  if (level) {
    return;
  }

  digitalWrite(A2, HIGH);

  /*
   * The level is low. Proceed with RX sampling.
   */
  ir_rx_edge_disable();
  ir_rx_start();
}

/**
 * TX worker. This function serves as an ISR for performing physical-layer work
 * necessary to transmit messages a word at a time. If data is available for
 * transmission, the worker should be started with a call to ir_tx_start(). When
 * no words are left remaining for transmission, the worker will automatically
 * disable itself with isr_tx_end().
 */
ISR(TIMER0_COMPA_vect) {
  if (tx_schedule_cursor == 0) {
    /*
     * TODO: Possibly enable interrupts during this part. And disable the timer
     * 0 interrupt! Also, possibly pause timer 0 while this mapping is going on.
     * Otherwise, the next character might be sent immediately.
     */
    /*
     * Another word must be fetched.
     */
    char c;
    if (ir_tx_buffer.get(c) != 0) {
      ir_tx_stop();
      return;
    }

    tx_schedule_prepare(c);
  }

  if (tx_schedule_cursor < tx_schedule_len) {
    if (tx_schedule[tx_schedule_cursor]) {
      /*
       * An asserted signal means an off LED.
       */
      ir_led_off();
    } else {
      /*
       * An un-asserted signal means an on LED.
       */
      ir_led_on();
    }

    /*
     * Increment the cursor, resetting to zero if the schedule is exhausted. The
     * IR LED is left on at the end of the schedule.
     */
    ++tx_schedule_cursor;
  } else {
    /*
     * Finish up a just-completed transmission.
     */
    tx_schedule_cursor = 0;
    ir_led_on();
  }
}

/**
 * RX worker. This function serves as an ISR for performing physical-layer work
 * in receiving data a byte at a time. This worker should be activated when a
 * falling edge is detected on the IR receiver. The worker will shut itself off
 * when either a word is received or the signal is determined to be noise.
 */
ISR(TIMER1_COMPA_vect) {
      digitalWrite(A2, HIGH);
  /*
   * Level indicates whether an IR signal is present or not. The negation is
   * because the IR receiver has negative logic.
   */
  bool level = !digitalRead(IR_RECEIVER_PIN);

  if (level == rx_level) {
    /*
     * The level is constant. Increment rx_level_count.
     */
    ++rx_level_count;

    /*
     * Add in and clear any edge samples.
     */
    if (rx_edge_count > 0) {
      rx_level_count += rx_edge_count;
      rx_edge_count = 0;
    }

    /*
     * Cancel if the level count has exceeded the maximum.
     */
    const unsigned int max_samples = (level
        ? MAX_GAP_SAMPLES
        : MAX_SYMBOL_SAMPLES);
    if (rx_level_count > max_samples) {
      ir_rx_stop();
    }
  } else {
    /*
     * A possible edge has occurred.
     */
    ++rx_edge_count;

    /*
     * Don't take the change seriously if it hasn't lasted for the required
     * number of samples.
     */
    if (rx_edge_count < MIN_SAMPLES_FOR_EDGE) {
      return;
    }

    /*
     * Save the last pulse count.
     */
    size_t pulse_count = rx_level_count;

    /*
     * Reset the level counter, clear the edge counter, and update the current
     * level.
     */
    rx_level_count = rx_edge_count;
    rx_edge_count = 0;
    rx_level = level;

    /*
     * If the last level was a gap, no further action is required.
     */
    if (!level) {
      return;
    }

    /*
     * This is the end of a symbol.
     */
    symbol_t symbol = get_symbol_from_pulse(pulse_count);
    bool fail = false;

    if (rx_symbol_count == 0) {
      /*
       * Expecting a start symbol.
       */
      if (symbol != symbol_start)
      {
        fail = true;
      }
    }
    else if (rx_symbol_count >= 1 && rx_symbol_count <= 8) {
      /*
       * Expecting a logical 1 or 0.
       */
      switch (symbol) {
        case symbol_0:
          rx_byte >>= 1;
          break;

        case symbol_1:
          rx_byte = (rx_byte >> 1) | 0x80u;
          break;

        default:
          fail = true;
          break;
      }
    }
    else if (rx_symbol_count == 9) {
      /*
       * Expecting a parity bit.
       */
      symbol_t even_parity;
      unsigned short bit_count = 0;
      for (unsigned short i = 0; i < 8; ++i) {
        if (rx_byte & (1u << i)) {
          ++bit_count;
        }
      }

      even_parity = ((bit_count % 2) == 0) ? symbol_0 : symbol_1;

      if (symbol != even_parity) {
        fail = true;
      }
    }

    if (fail) {
      ir_rx_stop();
      return;
    }

    ++rx_symbol_count;

    if (rx_symbol_count == 10) {
      /*
       * Word reception has completed. Push the byte to the ring buffer and
       * deactivate the RX worker.
       */
      //digitalWrite(A2, HIGH);
      ir_rx_buffer.put(rx_byte);
      ir_rx_stop();
    }
  }
}

/**
 * Prepares the TX schedule for a new byte. The TX schedule cursor is
 * reinitialized to zero, and the schedule length is set.
 *
 * @param tx_byte The byte to transmit.
 */
static void tx_schedule_prepare(uint8_t tx_byte) {
  tx_schedule_cursor = 0;

  /*
   * Record the start symbol.
   */
  tx_schedule_append_symbol(symbol_start);
  tx_schedule_append_gap();

  /*
   * Word data.
   */
  unsigned short parity = 0;
  for (size_t i = 0; i < 8; ++i) {
    bool bit = tx_byte & (1u << i);

    /*
     * Write logic 1 or 0.
     */
    if (bit) {
      ++parity;
      tx_schedule_append_symbol(symbol_1);
    } else {
      tx_schedule_append_symbol(symbol_0);
    }
    tx_schedule_append_gap();
  }

  /*
   * Write even parity bit. Don't include a gap at the end.
   */
  if (parity % 2 != 0) {
    tx_schedule_append_symbol(symbol_1);
  } else {
    tx_schedule_append_symbol(symbol_0);
  }

  tx_schedule_len = tx_schedule_cursor;
  tx_schedule_cursor = 0;
}

/**
 * Appends the indicated symbol to the TX schedule. No gap is added.
 *
 * @param symbol The symbol to add.
 */
void tx_schedule_append_symbol(symbol_t symbol) {
  switch (symbol) {
    case symbol_start:
      tx_schedule_append_pulse(true, START_SYMBOL_LENGTH);
      break;

    case symbol_0:
      tx_schedule_append_pulse(true, LOGIC_0_SYMBOL_LENGTH);
      break;

    case symbol_1:
      tx_schedule_append_pulse(true, LOGIC_1_SYMBOL_LENGTH);
      break;

    default:
      break;
  }
}

/**
 * Appends a gap to the TX schedule.
 */
void tx_schedule_append_gap() {
  tx_schedule_append_pulse(false, SYMBOL_GAP_LENGTH);
}

/**
 * Appends a pulse of some level for some length to the TX schedule, updating
 * the TX schedule cursor.
 *
 * @param level The level to set.
 * @param len The pulse length.
 */
void tx_schedule_append_pulse(bool level, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    tx_schedule[tx_schedule_cursor++] = level;
  }
}

/**
 * Returns the symbol corresponding to the given signal pulse width.
 *
 * @param len The number of samples for the pulse.
 *
 * @return The symbol corresponding with the pulse.
 */
symbol_t get_symbol_from_pulse(size_t count) {
  if (count >= START_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME - SAMPLE_FUDGE &&
      count <= START_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME + SAMPLE_FUDGE) {
    return symbol_start;
  }

  if (count >= LOGIC_0_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME - SAMPLE_FUDGE &&
      count <= LOGIC_0_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME + SAMPLE_FUDGE) {
    return symbol_0;
  }

  if (count >= LOGIC_1_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME - SAMPLE_FUDGE &&
      count <= LOGIC_1_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME + SAMPLE_FUDGE) {
    return symbol_1;
  }

  return symbol_unrecognized;
}

/**
 * Calculates an 8-bit CRC over the provided buffer.
 *
 * @param buf The buffer.
 * @param len The buffer length.
 *
 * @return An 8-bit CRC over the buffer.
 */
char crc8(const char *buf, size_t len)
{
  return (char)0xDF;
}

/**
 * Receive a single IR datagram. If a received datagram is available, the
 * provided buffer will be populated with its contents, and the provided length
 * will be updated.
 *
 * @param[out] buf The buffer to read the contents of the datagram into.
 * @param[in,out] len The buffer length. This is updated to reflect the number
 *                    of bytes actually received.
 *
 * @return 0 on success, -1 on failure.
 */
int ir_read(char *buf, size_t &len)
{
  char c;

  /*
   * Sync to the next datagram, if necessary.
   */
  while (!rx_datagram_synced && ir_rx_buffer.get(c) == 0) {
    if (c == DATAGRAM_START_BYTE) {
      rx_datagram_synced = true;
    }
  }

  /*
   * If the datagram hasn't synced, fail.
   */
  if (!rx_datagram_synced) {
    return -1;
  }

  /*
   * If the length byte hasn't been received yet, attempt to receive it.
   */
  if (rx_datagram_len == 0 && ir_rx_buffer.get(c) == 0) {
    rx_datagram_len = static_cast<size_t>(c);

    /*
     * Mark RX as desynced if the length field is 0. Zero-length datagrams are
     * not allowed.
     */
    if (rx_datagram_len == 0) {
      rx_datagram_synced = false;
    }
  }

  /*
   * If the length hasn't been resolved yet or not all characters have arrived
   * yet, fail. One additional character, the CRC, is expected.
   */
  if (rx_datagram_len == 0 ||
      ir_rx_buffer.num_elements() < rx_datagram_len + 1) {
    return -1;
  }

  /*
   * Read the datagram contents.
   */
  for (size_t i = 0; i < rx_datagram_len; ++i) {
    ir_rx_buffer.get(buf[i]);
  }

  /*
   * Fail if the CRC is invalid.
   */
  ir_rx_buffer.get(c);
  if (crc8(buf, rx_datagram_len) != c) {
    rx_datagram_synced = false;
    rx_datagram_len = 0;
    return -1;
  }

  /*
   * Update the user buffer length.
   */
  len = rx_datagram_len;

  /*
   * Update RX datagram state.
   */
  rx_datagram_synced = false;
  rx_datagram_len = 0;

  return 0;
}

/**
 * Send a buffer as a single IR datagram.
 *
 * @param buf The buffer to send.
 * @param len The buffer size.
 *
 * @return 0 on success, -1 on failure.
 */
int ir_write(const char *buf, size_t len)
{
  /*
   * Cancel if the TX ring buffer is not large enough to accommodate the
   * buffer. The (+3) accounts for the start byte, datagram length, and CRC.
   */
  const size_t total_len = len + 3;
  if (ir_tx_buffer.num_free() < total_len) {
    return -1;
  }

  /*
   * Cancel if the buffer has zero length or if its length cannot fit in an
   * 8-bit unsigned integer.
   */
  if (len == 0 || len > 255) {
    return -1;
  }

  /*
   * Compute the CRC over the data.
   */
  char crc = crc8(buf, len);

  /*
   * Write all the things.
   */
  ir_tx_buffer.put(DATAGRAM_START_BYTE);
  ir_tx_buffer.put(len);
  for (size_t i = 0; i < len; ++i) {
    ir_tx_buffer.put(buf[i]);
  }
  ir_tx_buffer.put(crc);

  /*
   * Start the TX worker. This is a no-op if the worker is already started.
   */
  ir_tx_start();

  return 0;
}
