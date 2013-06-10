/*

MAKE AN LED SHOW.

****** 1. What's this? **********

This program turns on the LEDs on the Simon board one by one.

****** 2. What's your job? **********

1) Currently, the LEDs turn on left-to-right across the top and then left-to-right across
   the bottom. Change this so that the lights turn on in a clock-wise circle.

2) Adjust the wait times so that the lights circle around once per second (hint: they
   take 2 seconds now).

 */

#include "Utility.h"
#include "SimonBoardHacking.h"

/*
 * Here's your program function again. Right now, we're turning on LED #1 for 0.5 seconds,
 * then turning it off and switching LED #2 on, etc. Remember that this function is
 * executed repeatedly.
 */
void repeat()
{
  turnOnLED(1);
  waitThisManySeconds(0.5);
  turnOffLED(1);
  
  turnOnLED(2);
  waitThisManySeconds(0.5);
  turnOffLED(2);
 
  turnOnLED(3);
  waitThisManySeconds(0.5);
  turnOffLED(3);
  
  turnOnLED(4);
  waitThisManySeconds(0.5);
  turnOffLED(4);
}
