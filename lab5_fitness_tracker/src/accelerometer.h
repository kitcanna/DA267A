#include <driver/i2c.h>

#ifndef accelerometer_h
#define accelerometer_h

void init();

int16_t readAccelerometer(uint16_t regH, uint16_t regL);

u_int32_t getMagnitude();

#endif