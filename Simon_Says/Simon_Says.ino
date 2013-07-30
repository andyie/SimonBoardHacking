#include "Utility.h"
#include "Simon_Says.h"

void startupDisplayRepeat()
{
  turn_on_light(1);
  wait_this_many_seconds(0.05);

  turn_on_light(2);
  wait_this_many_seconds(0.05);

  turn_on_light(3);
  wait_this_many_seconds(0.05);

  turn_on_light(4);
  wait_this_many_seconds(0.05);

  turn_on_light(5);
  wait_this_many_seconds(0.05);

  turn_on_light(6);
  wait_this_many_seconds(0.05);

  turn_on_light(7);
  wait_this_many_seconds(0.05);

  turn_on_light(8);
  wait_this_many_seconds(0.05);

  turn_on_light(9);
  wait_this_many_seconds(0.05);

  turn_on_light(10);
  wait_this_many_seconds(0.05);

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
