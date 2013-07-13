#include "Utility.h"
#include "SimonBoardHacking.h"
#include "Morse.h"

void whatToDoOnStartDot()
{
  turnOnLED(1);
}

void whatToDoOnEndDot()
{
  turnOffLED(1);
}

void whatToDoOnStartDash()
{
  turnOnLED(1);
}

void whatToDoOnEndDash()
{
  turnOffLED(1);
}

void repeat()
{
  morse("TAKE ME TO YOUR LEADER");

  waitThisManySeconds(1);
}
