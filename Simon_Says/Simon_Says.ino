#include "Utility.h"
#include "Simon_Says.h"

void startupDisplayRepeat()
{
  turnOnLED(1);
  waitThisManySeconds(0.05);

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
