#include "random.h"

int getRandommms(int min, int max) {
    // 0 <= A%B > B
    return esp_random() % (max - min + 1) + min;
}