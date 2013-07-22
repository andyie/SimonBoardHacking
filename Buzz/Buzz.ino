#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (is_button_pressed(1) == true)
  {
    start_buzzing(200);
    turn_on_LED(1);
  }
  if (is_button_pressed(1) == false)
  {
    turn_off_LED(1);
    stop_buzzing();
  }
}
