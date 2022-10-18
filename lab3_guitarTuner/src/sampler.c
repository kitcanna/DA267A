#include <esp_task_wdt.h>
#include <esp_timer.h>
#include <driver/adc.h>
#include <soc/adc_channel.h>
#include "sampler.h"

#define SAMPLE_SIZE 10

volatile int direction = 1;
volatile int base_sample = 20000000; //max freq. (20000) of the software timer according to documentation
volatile int amount;
int time_at_start;
int time_at_end;
esp_timer_handle_t timer;

void callback(void* args) {
    // channel 4 connected to pin 32
    int adc = adc1_get_raw(ADC1_CHANNEL_4);

    if (direction) {
        if (adc < base_sample) {
            direction = 0;
            amount++;
        }
    } else {
        if (adc > base_sample) {
            direction = 1;
            amount++;
        }
    }
    base_sample = adc;
}

void startSampling(int freq) {
    amount = 0;

    // start ADC
    adc_power_acquire();

    // configure pin
    // BUT! This is deprecated! The functionality itself seems to be folded into the 
    // adc[1,2]_config_channel_atten() functions, so this part should be removable.
    // adc_gpio_init(ADC_UNIT_1, ADC1_GPIO32_CHANNEL);

    // use full 12 bits width
    adc1_config_width(ADC_WIDTH_12Bit);

    // set attenuation to support full scale voltage
    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);
    //adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

    // let's setup a pull-up on that pin
    // this must be called after initialisation of the ADC or it will be ignored
    gpio_pullup_en(32);

    /*
     * Found with help of stack overflow
     * Creates the timer callback 
     * Allows for always resetting the time
     */ 
    esp_timer_create_args_t timer_period = {
        .callback = callback,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "sample"
    };
        ESP_ERROR_CHECK(esp_timer_create(&timer_period, &timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(timer, (uint64_t)((1.0 / (float)freq) * 1000000)));

        time_at_start = esp_timer_get_time();

}

void stopSampling() {
    ESP_ERROR_CHECK(esp_timer_stop(timer));
    time_at_end = esp_timer_get_time();
}


float getFrequency() {
    float interval = 2*(float)((time_at_end - time_at_start)/1000000.0);
    //printf("%f, %d\n", interval, amount);
    return (float)(amount/interval);
}