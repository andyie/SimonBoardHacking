/*

COMMAND LEDs TO TURN ON.

****** 1. What's this? **********

This program makes the upper LEDs and the lower-left LED (LEDs 1, 2, and 3) turn on when
you press their buttons.

****** 2. What's your job? **********

1) Add some code to make the lower-right LED turn on when you press its button.

 */

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

  // Add your code here to make LED #4 turn on when you press its button.
}
