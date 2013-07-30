#include "Utility.h"
#include "SimonBoardHacking.h"
#include "Morse.h"

void whatToDoOnStartDot()
{
  turn_on_light(upper_left_light);
}

void whatToDoOnEndDot()
{
  turn_off_light(upper_left_light);
}

void whatToDoOnStartDash()
{
  turn_on_light(upper_left_light);
}

void whatToDoOnEndDash()
{
  turn_off_light(upper_left_light);
}

void repeat()
{
  morse("HELLO WORLD");

  wait_this_many_seconds(1);
}
