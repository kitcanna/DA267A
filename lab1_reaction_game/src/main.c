#include "pins.h"
#include <esp_task_wdt.h>
#include "random.h"

#define ever                                                                   \
    ;                                                                          \
    ;

/*
 Waits for "millis" milliseconds without upsetting the watchdog timer
*/
void waitMs(unsigned int millis) {
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

void startGame() {
    // you can flash LEDs in a certain way for it
    for (int i = 0; i < 10; i++) {
        setLEDA(1);
        setLEDB(1);
        waitMs(100);
        setLEDA(0);
        setLEDB(0);
        waitMs(100);
    }
    // switch both LEDs off
    // get a random duration between 3 and 5 seconds
    int random = getRandommms(3000, 10000);
    // wait that random duration
    waitMs(random);
    // switch both LEDs ON
    setLEDA(1);
    setLEDB(1);
}

void app_main() {
    initPins();
    for (ever) {
        // signal that the game is about to start
        startGame();
        uint8_t winner = 0;
        if (isButtonAPressed()) {
            winner = 2;
        } else if (isButtonBPressed()) {
            winner = 1;
        }
        while (!winner) {
            // check if either button A or B are pressed
            if (isButtonAPressed()) {
                winner = 1;
            } else if (isButtonBPressed()) {
                winner = 2;
            }
        }
        if (winner == 1) {
            setLEDB(0);
        } else {
            setLEDA(0);
        }

        waitMs(3000);
        setLEDA(0);
        setLEDB(0);
    }
}