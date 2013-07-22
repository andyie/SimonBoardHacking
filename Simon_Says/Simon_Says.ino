#include "Utility.h"
#include "Simon_Says.h"

void startupDisplayRepeat()
{
  turn_on_LED(1);
  wait_this_many_seconds(0.05);

  turn_on_LED(2);
  wait_this_many_seconds(0.05);

  turn_on_LED(3);
  wait_this_many_seconds(0.05);

  turn_on_LED(4);
  wait_this_many_seconds(0.05);

  turn_on_LED(5);
  wait_this_many_seconds(0.05);

  turn_on_LED(6);
  wait_this_many_seconds(0.05);

  turn_on_LED(7);
  wait_this_many_seconds(0.05);

  turn_on_LED(8);
  wait_this_many_seconds(0.05);

  turn_on_LED(9);
  wait_this_many_seconds(0.05);

  turn_on_LED(10);
  wait_this_many_seconds(0.05);

  turn_off_all_LEDs();
}

void loserDisplay()
{
  turn_on_all_LEDs();
  wait_this_many_seconds(0.5);
  turn_off_all_LEDs();
  wait_this_many_seconds(0.2);

  turn_on_all_LEDs();
  wait_this_many_seconds(0.5);
  turn_off_all_LEDs();
  wait_this_many_seconds(0.2);
}
