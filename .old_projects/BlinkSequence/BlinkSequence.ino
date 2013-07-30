#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  turn_on_light(upper_left_light);
  wait_this_many_seconds(0.5);
  turn_off_light(upper_left_light);

  turn_on_light(upper_right_light);
  wait_this_many_seconds(0.5);
  turn_off_light(upper_right_light);

  turn_on_light(lower_right_light);
  wait_this_many_seconds(0.5);
  turn_off_light(lower_right_light);

  turn_on_light(lower_left_light);
  wait_this_many_seconds(0.5);
  turn_off_light(lower_left_light);
}
