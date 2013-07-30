#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (is_button_pressed(upper_left_button) == true)
  {
    start_buzzing(200);
    turn_on_light(upper_left_light);
  }
  if (is_button_pressed(upper_left_button) == false)
  {
    turn_off_light(upper_left_light);
    stop_buzzing();
  }
}
