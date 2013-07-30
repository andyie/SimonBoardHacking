#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (is_button_pressed(1) == true) {
    turn_on_light(1);
  }
  if (is_button_pressed(1) == false) {
    turn_off_light(1);
  }

  if (is_button_pressed(2) == true) {
    turn_on_light(2);
  }
  if (is_button_pressed(2) == false) {
    turn_off_light(2);
  }

  if (is_button_pressed(3) == true) {
    turn_on_light(3);
  }
  if (is_button_pressed(3) == false) {
    turn_off_light(3);
  }
}
