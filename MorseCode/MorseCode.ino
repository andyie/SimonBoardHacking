#include "SimonBoardHacking.h"
#include "Morse.h"

/**
 * Whenever we need to start making a Morse code dot, this function is executed.
 * If we want to blink a light to indicate a dot, this is where we would turn
 * that light on.
 */
void whatToDoOnStartDot()
{
  /*
   * In this example, we turn on LED #1 at the beginning of a dot.
   */
  turnOnLED(1);
}

/**
 * Whenever a Morse code dot is "finished," this function is executed. If we
 * turned on a light at the start of the dot, this is where we would turn that
 * light off again.
 */
void whatToDoOnEndDot()
{
  turnOffLED(1);
}

/**
 * This function is called at the beginning of each Morse code dash.
 */
void whatToDoOnStartDash()
{
  turnOnLED(1);
}

/**
 * This function is called at the end of each Morse code dash.
 */
void whatToDoOnEndDash()
{
  turnOffLED(1);
}

/**
 * Here's the familiar main program function.
 */
void repeat()
{
  /*
   * So this is where we actually encode stuff into Morse Code. Put
   * the stuff you want encoded between the quotes....
   */
  morse("PUT YOUR WORDS HERE");
  
  /*
   * Take a break for a second before repeating....
   */
  waitThisManySeconds(1);
}
