#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (isButtonPressed(1) == true)
  {
    startBuzzing(200);
    turnOnLED(1);
  }
  if (isButtonPressed(1) == false)
  {
    turnOffLED(1);
    stopBuzzing();
  }
}
