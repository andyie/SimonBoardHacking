#ifndef IRCOMM_H
#define IRCOMM_H

//TODO: Remove unnecessary #defs

#define UNIT_TIME_US            10
#define SAMPLES_PER_UNIT_TIME   5

#define RING_BUFFER_SIZE        128

#define SIGNAL_1       0b11001000
#define SIGNAL_2       0b11001001
#define SIGNAL_3       0b11001010
#define SIGNAL_4       0b11001011

/**************************/
/***** Physical Layer *****/
/**************************/

void ir_on();
void ir_off();

void tx_led_on();
void tx_led_off();

void rx_led_on();
void rx_led_off();

void connection_led_on();
void connection_led_off();

void enable_tx_timer();
void disable_tx_timer();

void enable_rx_timer();
void disable_rx_timer();

//Stores the states of the LED required to send one full byte
//Each bit corresponds to the state of the LED during one UNIT_TIME
static int8 next_tx_state;
static int8 last_tx_state;
static uint64_t tx_states;

void transmit_led_state();


static int8 rx_bit_index;
static byte rx_byte;

static uint8 num_rx_low_samples;
static uint8 num_rx_high_samples;

bool read_receiver_state();
void on_connection_detected();
void on_first_symbol_edge();
void take_symbol_sample();

/**********************/
/***** Word Layer *****/
/**********************/

void stage_tx_byte(); 
void store_rx_byte();
 
/**************************/
/***** Datagram Layer *****/
/**************************/

#define DATAGRAM_START_BYTE 0xA7

static RingBuffer<RING_BUFFER_SIZE> tx_buffer;
static RingBuffer<RING_BUFFER_SIZE> rx_buffer;

int build_and_stage_datagram(char* chars, uint_8 num_chars);
void decode_datagram(char* & chars, uint_8 & num_chars);

char calculate_crc8(char* chars, uint8 num_chars);

/***********************************/
/***** Program Interface Layer *****/
/***********************************/

int transmit_chars(char* chars, uint_8 num_chars); 
void receive_chars(char* & chars, uint_8 & num_chars);

/*****************************/
/***** Arduino Callbacks *****/
/*****************************/

//TODO: Check string value in main loop instead of using callbacks?
extern void on_receive_signal_1();
extern void on_receive_signal_2();
extern void on_receive_signal_3();
extern void on_receive_signal_4();
 
#endif