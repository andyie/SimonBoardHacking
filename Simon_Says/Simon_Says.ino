/*

SIMON SAYS

****** 1. What's this? **********

Back to Simon Says! This is a part of the Simon Says program. Most of it
is in another file on the computer (we "import" it with the "#include "Simon_Says.h").

****** 2. What's your job? **********

Your job is to invent light and sound shows that will play when you start the
game and if you lose the game.

 */

// Don't worry about this stuff.
#include "Utility.h"
#include "Simon_Says.h"

/**
 * This function is executed at the start of the game. A working one is
 * already written for you; in fact, this is the exact program that your
 * boards had programmed on them today. It just blinks all the LEDs on
 * and off.
 *
 * Bet you can make a more exciting one.
 */
void startupDisplayRepeat()
{
  turnOnAllLEDs();
  waitThisManySeconds(0.5);
  
  turnOffAllLEDs();
  waitThisManySeconds(0.5);
}

/**
 * This function is run if you lose the game. A working example is already written
 * for you. This one turns on all the LEDs on the board for 0.5 seconds, then
 * turns them off for 0.2 seconds. It does this twice.
 */
void loserDisplay()
{
  turnOnAllLEDs();
  waitThisManySeconds(0.5);
  turnOffAllLEDs();
  waitThisManySeconds(0.2);

  turnOnAllLEDs();
  waitThisManySeconds(0.5);
  turnOffAllLEDs();
  waitThisManySeconds(0.2);
}
