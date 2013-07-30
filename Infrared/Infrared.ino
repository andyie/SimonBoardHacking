#include <SimonBoardHacking.h>
#include <Ir.h>
#include <Utility.h>
#include <Events.h>

hear_event(event_upper_left_light_on) {
  turn_on_light(2);
  announce_event(event_upper_left_light_on);
}

hear_event(event_upper_left_light_off) {
  turn_on_light(
}

hear_event(event_upper_right_light_on) {
}

hear_event(event_upper_right_light_off) {
}

hear_event(event_lower_left_light_on) {
}

hear_event(event_lower_left_light_off) {
}

hear_event(event_lower_right_light_on) {
}

hear_event(event_lower_right_light_off) {
}
