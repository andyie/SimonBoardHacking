#ifndef SIMON_BOARD_HACKING_H
#define SIMON_BOARD_HACKING_H

extern unsigned int simon_led_pin_mapping[];

extern "C" void setup();
extern "C" void loop();

void waitThisManySeconds(float seconds);
bool valid_simon_pin(unsigned int simon_pin);
bool turnOnLED(unsigned int simon_pin);
bool turnOffLED(unsigned int simon_pin);
bool turnOffAllLEDs();
bool turnOnAllLEDs();

#endif
