/*

BLINK AN LED.

****** 1. What's this? **********

Here's a little program that blinks one of the button LEDs on the Simon Board.

****** 2. What's your job? **********

1) Currently the upper-left LED blinks. Try to get the lower-right LED to blink instead.

2) Play around with how much time the LED is on and off

 */

#include "SimonBoardHacking.h"
#include "Utility.h"

/**
 * This is your program. The thing we're making here, "void repeat()" is called a
 * function. Functions are very common and important in programming. We made this
 * particular function special; it will be executed again and again, forever.
 *
 * Right now, this function turns on LED #1, the one in the upper-left corner. The
 * program then waits for 0.5 seconds and then turns off the LED. After turning off
 * the LED, we wait another 0.5 seconds.
 *
 * Question: What's the waiting for, and why do we wait after we turn OFF the LED?
 */
void repeat()
{
  turnOnLED(1);
  waitThisManySeconds(0.5);
  
  turnOffLED(1);
  waitThisManySeconds(0.5);
}
