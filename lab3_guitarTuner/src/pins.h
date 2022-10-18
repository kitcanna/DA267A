#ifndef PINS_H_
#define PINS_H_
#include <stdint.h>
/* initialises the LED pin */
void initLEDPin(uint8_t pinN);
/* initialises the button pin */
void initButtonPin(uint8_t pinN);
/* switches LED on if level!=0 or off if level==0*/
void setLED(uint8_t pinN, uint8_t level);
#endif