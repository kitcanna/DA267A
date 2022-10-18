#include <stdio.h>
#include <esp_task_wdt.h>
#include <driver/adc.h>
#include <soc/adc_channel.h>
#include <math.h>
#include "sampler.h"
#include "pins.h"
#include "notes.h"

char note[] = "idk";
static volatile float frequency;

#define ledR 12 //röd
#define ledW 13 //vit

#define button 18 
#define ESP_INTR_FLAG_DEFAULT 0
#define PUSH_TIME_US 250000 //250ms

static volatile int butt = 1; 
static volatile uint64_t lastPush = PUSH_TIME_US;

static void isr_handler(void *arg)
{ 
  uint64_t now = esp_timer_get_time();
  if((now - lastPush) > PUSH_TIME_US){
      if(butt < 6){
          butt++; 
      }
    else if(butt == 6){
        butt = 1;
    }
     lastPush = now;
  }

}

void startSample() {
        startSampling(6000);
        vTaskDelay(pdMS_TO_TICKS(1000));
        stopSampling();
        frequency = getFrequency();
}

void checkTone(int freq, int max, int min) {
        if (freq >= min && freq <= max) {
                    setLED(ledR,1);
                    setLED(ledW,1);
        }
                //Note too low, white light on
                else if (freq < min) {
                    setLED(ledR,0);
                    setLED(ledW,1);
                }
                //Note too high, red light on
                else if (freq > max) {
                    setLED(ledR,1);
                    setLED(ledW,0);
                }
}

void app_main() {

    initLEDPin(ledW);
    initLEDPin(ledR);
    initButtonPin(button);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(button, isr_handler, NULL);

    while(1) {
        startSample();
        int freq = lround(frequency);
        printf("Frequency: %d\n", freq);

        switch(butt){
            //A 430-450
           case 1:
                printf("Testing note A...\n");
                checkTone(freq, 450, 430); 
                break;

            //B 480 - 500
            case 2:
                printf("Testing note B...\n");
                checkTone(freq, 500, 480); 
                break;

            //C 510 - 550
            case 3:
                printf("Testing note C...\n");
                checkTone(freq, 550, 510); 
                break;

            //D 560 - 600
            case 4:
                printf("Testing note D...\n");
                checkTone(freq, 600, 560); 
                break;

            //F 620 - 740
            case 5:
                printf("Testing note F...\n");
                checkTone(freq, 620, 740); 
                break;

            //G 750 - 800
            case 6:
                printf("Testing note G...\n");
                checkTone(freq, 750, 800); 
                break;
        }
    }

    // while (1) {
    //     printf("--- Start!\n");
    //     // sampling at 6kHz, max freq = 3kHz
    //     startSampling(6000); 

    //     // wait 1 second
    //     vTaskDelay(pdMS_TO_TICKS(1000));

    //     stopSampling();
    //     printf("--- Stop!\n");

    //     frequency = getFrequency();
    //     printf("Frequency is %.2f\n", frequency);

    //     vTaskDelay(pdMS_TO_TICKS(100));

    //     freq2note(frequency, note);
    //     printf("Note: %s\n", note);
    // }
}