#ifndef RANDOM_H_
#define RANDOM_H_
#include <esp_system.h>

/* Returns a random number contained between min and max in ms.
 min: minimum number of ms
 max: maximum number of ms
*/
int getRandommms(int min, int max);

#endif