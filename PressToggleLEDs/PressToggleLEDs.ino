#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (isButtonPressed(1) == true) {
    turnOnLED(1);
  }
  if (isButtonPressed(1) == false) {
    turnOffLED(1);
  }

  if (isButtonPressed(2) == true) {
    turnOnLED(2);
  }
  if (isButtonPressed(2) == false) {
    turnOffLED(2);
  }

  if (isButtonPressed(3) == true) {
    turnOnLED(3);
  }
  if (isButtonPressed(3) == false) {
    turnOffLED(3);
  }
}
