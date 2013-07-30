#include "Events.h"
#include "../Ir/Ir.h"
#include <stddef.h>

#include "../Utility/Utility.h"

/**
 * The size of the local buffer.
 */
#define BUFFER_SIZE 128

#define DIM(array) (sizeof(array) / sizeof((array)[0]))

extern void hear_event_event_upper_left_light_on();
extern void hear_event_event_upper_left_light_off();
extern void hear_event_event_upper_right_light_on();
extern void hear_event_event_upper_right_light_off();
extern void hear_event_event_lower_left_light_on();
extern void hear_event_event_lower_left_light_off();
extern void hear_event_event_lower_right_light_on();
extern void hear_event_event_lower_right_light_off();

/**
 * The different message types.
 */
enum message_type_t {
  message_type_event,
  message_type_string
};

/**
 * An array of event callback handlers.
 */
void (*event_handlers[])() = {
  hear_event_event_upper_left_light_on,
  hear_event_event_upper_left_light_off,
  hear_event_event_upper_right_light_on,
  hear_event_event_upper_right_light_off,
  hear_event_event_lower_left_light_on,
  hear_event_event_lower_left_light_off,
  hear_event_event_lower_right_light_on,
  hear_event_event_lower_right_light_off
};

/**
 * A buffer for characters.
 */
char buf[BUFFER_SIZE];

int process_message(const char *buf, size_t len);
int process_event(event_t event);
int process_string(const char *buf, size_t len);

/**
 * Main program loop, as required by SimonBoardHacking.
 */
void repeat() {
  /*
   * Initialize IR interface.
   */
  ir_init();

  /* FAKE! */
  announce_event(event_upper_left_light_on);

  /*
   * Main program loop. The repeat() function is not allowed to complete, since
   * this would cause IR to become reinitialized.
   */
  for (;;) {
    /*
     * Process any pending events.
     */
    size_t len = sizeof(buf);
    while (ir_read(buf, len)) {
      process_message(buf, len);
    }
  }
}

/**
 * Announces the occurrence of an event to a remote IR host.
 *
 * @param event The event that has occurred.
 *
 * @return 0 on success, -1 on failure.
 */
int announce_event(event_t event) {
  buf[0] = message_type_event;
  buf[1] = event;

  return ir_write(buf, 2);
}

/**
 * Processes an inbound message, possibly invoking callback functions.
 *
 * @param buf The buffer with the message.
 * @param len The buffer length.
 *
 * @return 0 on success, -1 on failure.
 */
int process_message(const char *buf, size_t len) {
  /*
   * Cancel if the message is too small.
   */
  if (len < 2) {
    return -1;
  }

  /*
   * Extract the message type.
   */
  message_type_t message_type = static_cast<message_type_t>(buf[0]);
  switch (message_type) {
    case message_type_event:
      return process_event(static_cast<event_t>(buf[1]));

    case message_type_string:
      return process_string(buf + 1, len - 1);

    default:
      return -1;
  }
}

/**
 * Processes a received event. The appropriate callback is invoked.
 *
 * @param event The received event.
 *
 * @return 0 on success, -1 on failure.
 */
int process_event(event_t event) {
  size_t index = static_cast<size_t>(event);
  if (index >= DIM(event_handlers)) {
    return -1;
  }

  event_handlers[index]();

  return 0;
}

/**
 * Processes a received string.
 *
 * @param buf The string.
 * @param len The string length.
 *
 * @return 0 on success, -1 on failure.
 */
int process_string(const char *buf, size_t len) {
  return 0;
}
