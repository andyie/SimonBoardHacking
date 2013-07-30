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
    event_upper_left_light_on,
    event_upper_left_light_off,
    event_upper_right_light_on,
    event_upper_right_light_off,
    event_lower_left_light_on,
    event_lower_left_light_off,
    event_lower_right_light_on,
    event_lower_right_light_off
};

int announce_event(event_t event);

#endif
