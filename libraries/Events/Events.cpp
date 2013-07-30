#include "Events.h"

#include "../Ir/Ir.h"

extern void hear_event_UPPER_LEFT_LED_ON();
extern void hear_event_UPPER_LEFT_LED_OFF();
extern void hear_event_UPPER_RIGHT_LED_ON();
extern void hear_event_UPPER_RIGHT_LED_OFF();
extern void hear_event_LOWER_LEFT_LED_ON();
extern void hear_event_LOWER_LEFT_LED_OFF();
extern void hear_event_LOWER_RIGHT_LED_ON();
extern void hear_event_LOWER_RIGHT_LED_OFF();

/**
 * An array of event callback handlers.
 */
void (*event_handlers[])() = {
  hear_event_UPPER_LEFT_LED_ON,
  hear_event_UPPER_LEFT_LED_OFF,

  hear_event_UPPER_RIGHT_LED_ON,
  hear_event_UPPER_RIGHT_LED_OFF,

  hear_event_LOWER_LEFT_LED_ON,
  hear_event_LOWER_LEFT_LED_OFF,

  hear_event_LOWER_RIGHT_LED_ON,
  hear_event_LOWER_RIGHT_LED_OFF
};

/**
 * Main program loop, as required by SimonBoardHacking.
 */
void repeat() {
  /*
   * Initialize IR interface.
   */
  ir_init();
}

/**
 * Announces the occurrence of an event to a remote IR host.
 *
 * @param event The event that has occurred.
 *
 * @return 0 on success, -1 on failure.
 */
int announce_event(event_t event) {
  return 0;
}
