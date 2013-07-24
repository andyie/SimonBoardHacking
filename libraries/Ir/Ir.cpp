#include "Ir.h"

// The Arduino pin connected to the IR LED. Set to 11 for OC2A.
#define IR_PIN 11

/**
 * Indicates to the PHY layer that there is a character to send.
 */
static bool tx_ready;

static void phy_tx_init();
static void phy_rx_init();

/*
 * Application-layer stuffs.
 */

// Initialize IR transmission.
void ir_tx_init()
{
  phy_tx_init();
  phy_rx_init();
}

/*
 * Physical-layer stuff.
 */

/**
 * Initailize IR transmission.
 */
static void phy_tx_init()
{
  // Configure IR output pin.
  pinMode(IR_PIN, OUTPUT);

  // Set up PWM
  // Configure output on Pin 11 using OC2A as top limit for timer | Use Fast PWM | Use Fast PWM
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);

  // Timer top limit; Duty cycle is defaulted to 50%
  // Signal toggles every run of the timer: PWM frequency = 8 MHz / (2 * 104) =
  // 38.46kHz.
  OCR2A = 104;

  tx_ready = false;
}

/**
 * Initialize IR reception.
 */
static void phy_rx_init()
{
  // Nothing to do for now.
}
