#include "Utility.h"
#include "Simon_Says.h"

void startupDisplayRepeat()
{
  turn_on_light(upper_left_light);
  wait_this_many_seconds(0.1);

  turn_on_light(upper_right_light);
  wait_this_many_seconds(0.1);

  turn_on_light(lower_right_light);
  wait_this_many_seconds(0.1);

  turn_on_light(lower_left_light);
  wait_this_many_seconds(0.1);

  turn_off_all_lights();
}

void loserDisplay()
{
  turn_on_all_lights();
  wait_this_many_seconds(0.5);
  turn_off_all_lights();
  wait_this_many_seconds(0.2);

  turn_on_all_lights();
  wait_this_many_seconds(0.5);
  turn_off_all_lights();
  wait_this_many_seconds(0.2);
}
