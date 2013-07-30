/**
 * @brief Events library for events processing.
 *
 * @author andy
 * @date 30-7-2013
 */

#ifndef EVENTS_H
#define EVENTS_H

#define hear_event(e) void hear_event_##e()

/**
 * The different events.
 */
enum event_t {
    UPPER_LEFT_LED_ON,
    UPPER_LEFT_LED_OFF,

    UPPER_RIGHT_LED_ON,
    UPPER_RIGHT_LED_OFF,

    LOWER_LEFT_LED_ON,
    LOWER_LEFT_LED_OFF,

    LOWER_RIGHT_LED_ON,
    LOWER_RIGHT_LED_OFF
};

int announce_event(event_t event);

#endif
