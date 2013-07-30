#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (is_button_pressed(upper_left_button) == true) {
    turn_on_light(upper_left_light);
  }
  if (is_button_pressed(upper_left_button) == false) {
    turn_off_light(upper_left_light);
  }

  if (is_button_pressed(upper_right_button) == true) {
    turn_on_light(upper_right_light);
  }
  if (is_button_pressed(upper_right_button) == false) {
    turn_off_light(upper_right_light);
  }

  if (is_button_pressed(lower_right_button) == true) {
    turn_on_light(lower_right_light);
  }
  if (is_button_pressed(lower_right_button) == false) {
    turn_off_light(lower_right_light);
  }
}
