#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  turn_on_LED(1);
  wait_this_many_seconds(0.5);
  turn_off_LED(1);

  turn_on_LED(2);
  wait_this_many_seconds(0.5);
  turn_off_LED(2);

  turn_on_LED(3);
  wait_this_many_seconds(0.5);
  turn_off_LED(3);

  turn_on_LED(4);
  wait_this_many_seconds(0.5);
  turn_off_LED(4);
}
