#include "Arduino.h"
#include "Ir.h"
#include "../RingBuffer/RingBuffer.h"
#include "stdint.h"

/*
 * Hardware allocations:
 *
 * Timer 0: TX.
 * Timer 1: RX.
 * Timer 2: LED PWM.
 */

/**
 * The Arduino pin connected to the IR LED. Set to 11 for OC2A.
 */
#define IR_LED_PIN 11

/**
 * The Arduino pin connected to the IR receiver.
 */
#define IR_RECEIVER_PIN A5

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
#define INTER_SYMBOL_LENGTH 2

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
#define MAX_SYMBOL_SAMPLES (4 * SAMPLES_PER_UNIT_TIME + \
    SAMPLES_PER_UNIT_TIME / 2)

/**
 * The number of gap samples which may be accepted before the next symbol times
 * out.
 */
#define MAX_GAP_SAMPLES (INTER_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME + \
    SAMPLES_PER_UNIT_TIME / 2)

/**
 * Fudge interval in sample counts. Naturally, this interval should not be so
 * large that a given sample count could be taken as more than one symbol.
 */
#define SAMPLE_FUDGE 6

/**
 * Start byte for all datagrams that are sent and received
 */
#define DATAGRAM_START_CHAR ((char)0xA7)

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
 * RX worker state.
 */

/**
 * The current signal that is being tracked by the receiver.
 */
static bool rx_level = false;

/**
 * The RX byte currently being received.
 */
static uint8_t rx_byte;

/**
 * Tracks the current symbol being received. Symbol 0 is the start symbol, and
 * symbol 9 is the parity symbol. The intervening symbols are data bits.
 */
static unsigned short rx_symbol_count = 0;

/**
 * The number of contiguous samples that have indicated a constant level.
 */
static unsigned short rx_level_count = 0;

/**
 * The number of contiguous samples what have indicated an edge has occurred. If
 * this count reaches MIN_SAMPLES_FOR_EDGE, the current symbol will be
 * concluded. If the signal returns to the original level before this count is
 * reached, this count is added to rx_level_count (the glitch is ignored).
 */
static unsigned short rx_edge_count = 0;

/*
 * The TX schedule length.
 */
size_t tx_schedule_len = 0;

/*
 * The length of the datagram that is currently being decoded
 */ 
static size_t current_rx_datagram_length = 0;

static void ir_tx_init();
static void ir_rx_init();

static void ir_rx_start();
static void ir_rx_stop();

static void ir_rx_on();
static void ir_rx_off();

static void ir_tx_start();
static void ir_tx_stop();

static void calculate_crc8();

static void tx_schedule_prepare( uint8_t tx_byte);
static void tx_schedule_append_pulse(bool level, size_t len);

static int build_and_stage_datagram(char* chars, size_t num_chars);
static void decode_datagram(char* & chars, size_t & num_chars);

/**
 * Initialize the IR communications module.
 */
void ir_init() {
  ir_tx_init();
  ir_rx_init();
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

  /*
   * Each run of the timer should be UNIT_TIME / 16. CPU = 8MHz, and it takes
   * 250 counts at 8MHz to reach 500us/16 = 31.25us.
   */
  OCR1A = 250;

  /*
   * Enable compare match interrupt.
   */
  TIMSK1 |= _BV(OCIE1A);
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

  /*
   * Each run of the timer should be UNIT_TIME. CPU / 64 = 125kHz, and it takes
   * 62.5 counts at 125kHz to reach 500us.
   */
  OCR0A = 62;

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

  /*
   * Signal toggles every run of the timer: Frequency = 8 MHz / (2 * 104) =
   * 38.46kHz.
   */
  OCR2A = 104;

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
   * Reset RX state. Since IR sampling should be started on a falling edge, the
   * starting level is false (low).
   */
  rx_level = false;
  rx_symbol_count = 0;
  rx_level_count = 0;
  rx_edge_count = 0;

  /*
   * Reset the timer and set CPU/1 prescaling.
   */
  TCNT1 = 0;
  TCCR1B = _BV(CS10);
}

/**
 * Stops the RX worker.
 */
void ir_rx_stop() {
  TCCR1B &= ~_BV(CS10);
}

/**
 * Starts the TX worker.
 */
void ir_tx_start() {
  /*
   * Reset TX state.
   */
  tx_schedule_cursor = 0;

  /*
   * Reset the timer and set CPU/64 prescaling.
   */
  TCNT0 = 0;
  TCCR0B = _BV(CS01) | _BV(CS00);
}

/**
 * Stops the TX worker.
 */
void ir_tx_stop()
{
  TCCR0B &= ~(_BV(CS01) | _BV(CS00));
}

/**
 * Turn the IR LED on.
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
 * TX worker. This function serves as an ISR for performing physical-layer work
 * necessary to transmit messages a word at a time. If data is available for
 * transmission, the worker should be started with a call to ir_tx_start(). When
 * no words are left remaining for transmission, the worker will automatically
 * disable itself with isr_tx_end().
 */
ISR(TIMER0_COMPA_vect) {
  static unsigned long slowdown = 200;
  if (slowdown-- != 0) {
    return;
  }
  slowdown = 200;

  if (tx_schedule_cursor == 0) {
    /*
     * TODO: Possibly enable interrupts during this part. And disable the timer
     * 0 interrupt!
     */
    /*
     * Another word must be fetched.
     */
    tx_schedule_prepare(0xAA);
  }

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
   * Increment the cursor, resetting to zero if the schedule is exhausted.
   */
  ++tx_schedule_cursor;

  if (tx_schedule_cursor == tx_schedule_len) {
    tx_schedule_cursor = 0;
    ir_led_off();
  }
}

/**
 * RX worker. This function serves as an ISR for performing physical-layer work
 * in receiving data a byte at a time. This worker should be activated when a
 * falling edge is detected on the IR receiver. The worker will shut itself off
 * when either a word is received or the signal is determined to be noise.
 */
ISR(TIMER1_COMPA_vect) {
  bool level = digitalRead(IR_RECEIVER_PIN);

  if (level == rx_level) {
    /*
     * The level is remaining constant. Increment rx_level_count.\
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
     //TODO:Check
    const unsigned int max_samples = level
        ? MAX_GAP_SAMPLES
        : MAX_SYMBOL_SAMPLES;
    if (rx_level_count > max_samples) {
      ir_rx_off();
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
     * Update the current level.
     */
    rx_level = level;

    /*
     * If the last level was a gap, no further action is required.
     */
    if (level) {
      return;
    }

    /*
     * This is the end of a symbol.
     */
    if (rx_symbol_count == 0) {
      /*
       * Expecting a start symbol.
       */
      if (rx_level_count < START_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME -
              SAMPLE_FUDGE ||
          rx_level_count > START_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME +
              SAMPLE_FUDGE)
      {
        ir_rx_off();
        return;
      }
    }
    else if (rx_symbol_count >= 1 && rx_symbol_count <= 8) {
      /*
       * Expecting a logical 1 or 0.
       */
      if (rx_level_count >= LOGIC_0_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME -
              SAMPLE_FUDGE &&
          rx_level_count <= LOGIC_0_SYMBOL_LENGTH * SAMPLES_PER_UNIT_TIME +
              SAMPLE_FUDGE) {
        
      }
    }
  }
}

/**
 * Prepares the TX schedule for a new byte. The TX schedule cursor is
 * reiniitalized to zero, and the schedule length is set.
 *
 * @param tx_byte The byte to transmit.
 */
static void tx_schedule_prepare(uint8_t tx_byte) {
  tx_schedule_cursor = 0;

  /*
   * Record the start symbol.
   */
  tx_schedule_append_pulse(true, START_SYMBOL_LENGTH);

  /*
   * Gap.
   */
  tx_schedule_append_pulse(false, INTER_SYMBOL_LENGTH);

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
      tx_schedule_append_pulse(true, LOGIC_1_SYMBOL_LENGTH);
    } else {
      tx_schedule_append_pulse(true, LOGIC_0_SYMBOL_LENGTH);
    }

    /*
     * Gap.
     */
    tx_schedule_append_pulse(false, INTER_SYMBOL_LENGTH);
  }

  /*
   * Write parity bit.
   */
  if (parity) {
    tx_schedule_append_pulse(true, LOGIC_1_SYMBOL_LENGTH);
  } else {
    tx_schedule_append_pulse(true, LOGIC_0_SYMBOL_LENGTH);
  }

  tx_schedule_len = tx_schedule_cursor;
  tx_schedule_cursor = 0;
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

char calculate_crc8(char* chars, uint8_t num_chars)
{
  return (char)0xDF;
}

/**
 * Creates a datagram using the chars stored in data as the payload
 *
 * @param data The chars that should be in the payload
 * @param len The number of chars that should be sent from data
 *
 * @return An error code; 0 = success, other = failure
 */
int build_and_stage_datagram(char* data, size_t len)
{
  /*
   * Packet format:
   * [start byte] [length byte] [byte 0] [byte 1] … [byte len - 1] [checksum]
   */

  if (ir_tx_buffer.put(DATAGRAM_START_CHAR))
  {
    //Failed to put data into ir_tx_buffer
    return -1;
  }

  if (ir_tx_buffer.put((char)len))
  {
    return -1;
  }

  for(size_t i = 0; i < len; i++)
  {
    if (ir_tx_buffer.put(data[i]))
    {
      return -1;
    }
  }
  
  char crc8 = calculate_crc8(data, len);
  
  if (ir_tx_buffer.put(crc8))
  {
    return -1;
  }
  
  return 0;
}

/**
 * Attempts to read a datagram from the ir_rx_buffer;
 * If successful, fills data with the datagram's payload
 * and fills len with the number of chars in the payload;
 * Otherwise, sets data to NULL and len to 0
 *
 * @param data A pointer to the datagram's payload, or NULL
 *             if no datagram was present
 *
 * @param len The number of chars in the decoded datagram's payload
 *            or 0 if no datagram was present
 *
 */
void decode_datagram(char* & data, size_t & len)
{
  char current_char = 0;

  if (current_rx_datagram_length == 0)
  {
     /*
      * We aren't in the middle of a datagram;
      * Find a start byte and a length byte:
      *
      * Loop until either only one char is left or 
      * until we find a start byte
      */
    while(ir_rx_buffer.num_elements() >= 2 && current_char != DATAGRAM_START_CHAR)
    {
      ir_rx_buffer.get(current_char);
    }

    /*
     * Exit if a datagram start byte wasn't found
     */
    if (current_char != DATAGRAM_START_CHAR)
    {
      data = NULL;
      len = 0;
      return;
    }

    /*
     * Found a datagram start byte; now set the length byte
     */
     ir_rx_buffer.get(current_char);

     current_rx_datagram_length = (size_t)current_char;
  }

  /*
   * Either we found a datagram start byte or we are already in the middle
   * of a datagram; check if enough bytes exist to complete the datagram;
   *
   * We require current_rx_datagram_length + 1 bytes;
   * The +1 is because of the CRC
   */
   if (ir_rx_buffer.num_elements() < current_rx_datagram_length + 1)
   {
      //We're missing bytes; exit
      data = NULL;
      len = 0;
      return;
   }

    /*
     * An entire datagram is present;
     * Store all the bytes
     */
   data = new char[current_rx_datagram_length];
   len = current_rx_datagram_length;

   for (size_t i = 0; i < current_rx_datagram_length; ++i)
   {
       ir_rx_buffer.get(data[i]);
   }

   /*
    * Check the CRC
    */
   char crc8 = calculate_crc8(data, len);
   ir_rx_buffer.get(current_char);

   if (current_char != crc8)
   {
     //This datagram is invalid; free memory
     //and exit
     delete[] data;

     data = NULL;
     len = 0;
     return;
   }

   /*
    * Datagram is valid; reset the current_datagram_length
    * to look for a new datagram the next time this function is called
    */
    current_rx_datagram_length = 0;

    return;
}

/**
 * Sends len chars from data over ir
 *
 * @param data The chars that should be sent
 * @param len The number of chars that should be sent
 *
 * @return An error code; 0 = success, other = failure
 */
int transmit_chars(char* data, size_t len)
{
  build_and_stage_datagram(data, len);
}

/**
 * Attempts to pull a string from the ir rx buffer;
 * If a full segment of data is present, fills data with the string
 * and fills len with the number of chars in the string;
 * Otherwise, sets data to NULL and len to 0
 *
 * @param data A pointer to the received data, or NULL
 *             if incomplete data was present
 *
 * @param len The number of chars in the received data
 *            or 0 if incomplete data was present
 *
 */
void receive_chars(char* & data, size_t & len)
{
  decode_datagram(data, len);

  //TODO: Check if payload matches signal
  //Call callback functions
  //Clear data
}