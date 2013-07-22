#include "Utility.h"
#include "SimonBoardHacking.h"

void repeat()
{
  if (is_button_pressed(1) == true) {
    turn_on_LED(1);
  }
  if (is_button_pressed(1) == false) {
    turn_off_LED(1);
  }

  if (is_button_pressed(2) == true) {
    turn_on_LED(2);
  }
  if (is_button_pressed(2) == false) {
    turn_off_LED(2);
  }

  if (is_button_pressed(3) == true) {
    turn_on_LED(3);
  }
  if (is_button_pressed(3) == false) {
    turn_off_LED(3);
  }
}
