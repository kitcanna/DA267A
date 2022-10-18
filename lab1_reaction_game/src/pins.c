#include "pins.h"
#include "driver/gpio.h"

#define ledA 13
#define ledB 12
#define buttonA 26
#define buttonB 25

/* initialises the 4 pins */
void initPins() {
    // init the 2 LEDs pins as output and the 2 buttons' pins as input
    // you will need to use gpio_config()
    // create config struct
    gpio_config_t config;

    // ledA (13)
    config.pin_bit_mask = (u_int64_t)1 << ledA;
    config.mode = GPIO_MODE_OUTPUT;
    
    config.pull_down_en = 0;
    config.pull_up_en = 0;
    // set the config, this takes care of the registers
    gpio_config(&config);

    // ledB (12)
    config.pin_bit_mask = (u_int64_t)1 << ledB;
    gpio_config(&config);

    // buttonA (26)
    config.pin_bit_mask = (u_int64_t)1 << buttonA;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = 1;

    gpio_config(&config);

    // buttonB (25)
    config.pin_bit_mask = (u_int64_t)1 << buttonB;

    gpio_config(&config);
}


/* switches LED A on if level!=0 or off if level==0*/
void setLEDA(uint8_t level) {
        // set the pin of LED A to ON
        // you probably need to use gpio_set_level()
        gpio_set_level(ledA, level);
}

/* switches LED B on if level!=0 or off if level==0*/
void setLEDB(uint8_t level) {
        // set the pin of LED B to ON
        // you probably need to use gpio_set_level()
        gpio_set_level(ledB, level);
}

/* tells if button A is currently being pressed */
uint8_t isButtonAPressed() {
    // read the value of button A
    // if using a pull-up, the button is pressed when the pin is LOW
    return !gpio_get_level(buttonA);  // return 1 if A is pressed, 0 otherwise
}

/* tells if button A is currently being pressed */
uint8_t isButtonBPressed() {
    // same as with button A, but another pin
    return !gpio_get_level(buttonB);
}