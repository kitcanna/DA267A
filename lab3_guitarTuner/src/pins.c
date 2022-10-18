#include "pins.h" 
#include "driver/gpio.h"

gpio_config_t config;

/* initialises the LED pin */
void initLEDPin(uint8_t pinN)
{
    config.pin_bit_mask = (u_int64_t)1 << pinN;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = 0;
    config.pull_up_en = 0;
    gpio_config(&config);
}

/* initialises the button pin */
void initButtonPin(uint8_t pinN)
{
    config.pin_bit_mask = (u_int64_t)1 << pinN;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = 1;
    config.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&config);
}

/* switches LED on if level!=0 or off if level==0*/
void setLED(uint8_t pinN, uint8_t level)
{
    if (level)
    {
        // set the pinN to ON
        gpio_set_level(pinN, 1);
    }
    else
    {
        // set the pinN to OFF
        gpio_set_level(pinN, 0);
    }
}

