#include "SimonBoardHacking.h"

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
