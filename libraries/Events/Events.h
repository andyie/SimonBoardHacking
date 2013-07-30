/**
 * @brief Events library for events processing.
 *
 * @author andy
 * @date 30-7-2013
 */

#ifndef EVENTS_H
#define EVENTS_H

#include <stddef.h>

#define hear_event(e) void hear_event_##e()

#define hear_chat() void handle_string(const char *chat, size_t chat_len)

#define we_received_chat(str) (strncmp(chat, (str), chat_len) == 0 \
    ? true \
    : false)

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
int announce_string(const char *buf);
int announce_string(const char *buf, size_t len);

#endif
