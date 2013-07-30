#include <SimonBoardHacking.h>
#include <Ir.h>
#include <Utility.h>
#include <Events.h>

hear_event(event_upper_left_light_on) {
  turn_on_light(upper_right_light);
  announce_event(event_upper_right_light_on);
}

hear_event(event_upper_left_light_off) {
  turn_off_light(upper_right_light);
  announce_event(event_upper_right_light_off);
}

hear_event(event_upper_right_light_on) {
  turn_on_light(lower_right_light);
  announce_event(event_lower_right_light_on);
}

hear_event(event_upper_right_light_off) {
  turn_off_light(lower_right_light);
  announce_event(event_lower_right_light_off);
}

hear_event(event_lower_right_light_on) {
  turn_on_light(lower_left_light);
  announce_event(event_lower_left_light_on);
}

hear_event(event_lower_right_light_off) {
  turn_off_light(lower_left_light);
  announce_event(event_lower_left_light_off);
}

hear_event(event_lower_left_light_on) {
  turn_on_light(upper_left_light);
  announce_event(event_upper_left_light_on);
}

hear_event(event_lower_left_light_off) {
  turn_off_light(upper_left_light);
  announce_event(event_upper_left_light_off);
}
