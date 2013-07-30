#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  turn_on_light(1);
  wait_this_many_seconds(0.5);
  turn_off_light(1);

  turn_on_light(2);
  wait_this_many_seconds(0.5);
  turn_off_light(2);

  turn_on_light(3);
  wait_this_many_seconds(0.5);
  turn_off_light(3);

  turn_on_light(4);
  wait_this_many_seconds(0.5);
  turn_off_light(4);
}
