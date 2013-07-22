#include "Utility.h"
#include "SimonBoardHacking.h"
#include "Morse.h"

void whatToDoOnStartDot()
{
  turn_on_LED(1);
}

void whatToDoOnEndDot()
{
  turn_off_LED(1);
}

void whatToDoOnStartDash()
{
  turn_on_LED(1);
}

void whatToDoOnEndDash()
{
  turn_off_LED(1);
}

void repeat()
{
  morse("TAKE ME TO YOUR LEADER");

  wait_this_many_seconds(1);
}
