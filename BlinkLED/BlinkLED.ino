#include "SimonBoardHacking.h"
#include "Utility.h"

void repeat()
{
  turn_on_LED(1);
  wait_this_many_seconds(0.5);

  turn_off_LED(1);
  wait_this_many_seconds(0.5);
}
