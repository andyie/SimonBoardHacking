/*

MAKE YOUR PROGRAM BEEP.

****** 1. What's this? **********

This program causes the Simon board to beep when you press the upper-left button
(button #1). It will also turn on that button's LED when you press it.

We're introducing some new functions here:

isButtonPressed: Tells you if a certain button is being held down.
startBuzzing: Start the beeping.
stopBuzzing: Stop the beeping.

****** 2. What's your job? **********

1) The "startBuzzing" function can be told at what frequency, in Hertz, to buzz at.
   Change the pitch of the buzzer by modifying this value.

 */

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
