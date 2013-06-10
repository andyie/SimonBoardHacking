/*

SIMON SAYS

****** 1. What's this? **********

Back to Simon Says! This is a part of the Simon Says program. Most of it
is in another file on the computer (we "import" it with the "#include "Simon_Says.h").

****** 2. What's your job? **********

Your job is to invent light and sound shows that will play when you start the
game and if you lose the game.

1) As a start, make a small change to the current program. In the startup light show,
   Turn off each LED before turning on the next one. This way it will look like the
   light is moving across the board.

2) Whatever! Play around with lights. Programming is awesome :)

 */

#include "Utility.h"
#include "Simon_Says.h"

/**
 * This function is executed at the start of the game. A working one is
 * already written for you; in fact, this is the exact program that your
 * boards had programmed on them today. It just turns all the LEDs on
 * one by one.
 *
 * Bet you can make a more exciting one.
 *
 * To start, try turning off each of the LEDs after its been on.
 */
void startupDisplayRepeat()
{
  turnOnLED(1);
  waitThisManySeconds(0.05);
  // Hint: Add a function call to "turnOffLED(1)" here, and repeat across the other LEDs.
  
  turnOnLED(2);
  waitThisManySeconds(0.05);
  
  turnOnLED(3);
  waitThisManySeconds(0.05);
  
  turnOnLED(4);
  waitThisManySeconds(0.05);
  
  turnOnLED(5);
  waitThisManySeconds(0.05);
  
  turnOnLED(6);
  waitThisManySeconds(0.05);
  
  turnOnLED(7);
  waitThisManySeconds(0.05);
  
  turnOnLED(8);
  waitThisManySeconds(0.05);
  
  turnOnLED(9);
  waitThisManySeconds(0.05);

  turnOnLED(10);
  waitThisManySeconds(0.05);
  
  turnOffAllLEDs();
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
