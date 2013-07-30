#include "SimonBoardHacking.h"
#include "Utility.h"

void repeat()
{
  turn_on_light(upper_left_light);
  wait_this_many_seconds(0.5);

  turn_off_light(upper_left_light);
  wait_this_many_seconds(0.5);
}
