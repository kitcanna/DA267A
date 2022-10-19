#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "circular_buffer.h"

#define LED_PIN_LEVEL_UP        12
#define LED_PIN_LEVEL_MIDDLE    14
#define LED_PIN_LEVEL_DOWN      27
#define BUTTON_PIN              26
#define PUSH_TIME_US            250000 // 250 ms
#define BUFFER_SIZE             30

esp_err_t res;
struct circularBuffer buffer;

// Used to represent a travel need of a passenger.
struct travel_need {
    int origin;
    int destination;
};

// Used to not allow button pushes that are too close to each other in time
static volatile uint64_t lastPush = -PUSH_TIME_US;

//Just a counter keeping track of which travel need is next to process.
static volatile int travel_need_counter = 0;

// This data structure holds information about 
static volatile struct travel_need travel_needs[50];


/**
 * This function is called when button is pushed
 */
static void button_handler(void *arg) {

    // Disable interrupts
    gpio_intr_disable(BUTTON_PIN);

    // Get the current time 
    uint64_t now = esp_timer_get_time();

    // If enough time passed, we should consider this event as a genuine push
    if ((now - lastPush) > PUSH_TIME_US) {
        
        //SET TIME OF LAST PUSH TO CURRENT TIMER 
        lastPush = now;

        //ADD TRAVEL TO BUFFER
        addElement(&buffer, travel_need_counter);

        //INCREASE TRAVEL COUNTER
        travel_need_counter++;

    } 

    // Re-enable interrupts
    gpio_intr_enable(BUTTON_PIN);
}

void show_level(int level) {
        gpio_set_level(LED_PIN_LEVEL_UP, 0);
        gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
        gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));

        //FIRST FLOOR
        if (level == 0) {
            gpio_set_level(LED_PIN_LEVEL_DOWN, 1); 
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_DOWN, 1); 
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_DOWN, 1); 
        } 

        //SECOND FLOOR
        else if (level == 1) {
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 1);
        }
        
        //THIRD FLOOR
        else {
            gpio_set_level(LED_PIN_LEVEL_UP, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_UP, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_UP, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_UP, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(LED_PIN_LEVEL_UP, 1);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
}
   
void initTravelNeeds () {
    //Initialize travel needs (50 randomly generated travel needs)
    travel_needs[0].origin = 2; travel_needs[0].destination = 1;
    travel_needs[1].origin = 1; travel_needs[1].destination = 2;
    travel_needs[2].origin = 1; travel_needs[2].destination = 2;
    travel_needs[3].origin = 0; travel_needs[3].destination = 2;
    travel_needs[4].origin = 2; travel_needs[4].destination = 1;
    travel_needs[5].origin = 0; travel_needs[5].destination = 2;
    travel_needs[6].origin = 1; travel_needs[6].destination = 2;
    travel_needs[7].origin = 1; travel_needs[7].destination = 0;
    travel_needs[8].origin = 0; travel_needs[8].destination = 1;
    travel_needs[9].origin = 1; travel_needs[9].destination = 0;
    travel_needs[10].origin = 1; travel_needs[10].destination = 2;
    travel_needs[11].origin = 0; travel_needs[11].destination = 1;
    travel_needs[12].origin = 0; travel_needs[12].destination = 2;
    travel_needs[13].origin = 0; travel_needs[13].destination = 1;
    travel_needs[14].origin = 0; travel_needs[14].destination = 2;
    travel_needs[15].origin = 0; travel_needs[15].destination = 1;
    travel_needs[16].origin = 2; travel_needs[16].destination = 1;
    travel_needs[17].origin = 2; travel_needs[17].destination = 1;
    travel_needs[18].origin = 1; travel_needs[18].destination = 0;
    travel_needs[19].origin = 2; travel_needs[19].destination = 1;
    travel_needs[20].origin = 1; travel_needs[20].destination = 0;
    travel_needs[21].origin = 0; travel_needs[21].destination = 1;
    travel_needs[22].origin = 1; travel_needs[22].destination = 2;
    travel_needs[23].origin = 0; travel_needs[23].destination = 2;
    travel_needs[24].origin = 2; travel_needs[24].destination = 1;
    travel_needs[25].origin = 1; travel_needs[25].destination = 0;
    travel_needs[26].origin = 1; travel_needs[26].destination = 2;
    travel_needs[27].origin = 0; travel_needs[27].destination = 2;
    travel_needs[28].origin = 1; travel_needs[28].destination = 0;
    travel_needs[29].origin = 1; travel_needs[29].destination = 2;
    travel_needs[30].origin = 0; travel_needs[30].destination = 1;
    travel_needs[31].origin = 1; travel_needs[31].destination = 2;
    travel_needs[32].origin = 0; travel_needs[32].destination = 2;
    travel_needs[33].origin = 0; travel_needs[33].destination = 2;
    travel_needs[34].origin = 1; travel_needs[34].destination = 2;
    travel_needs[35].origin = 2; travel_needs[35].destination = 1;
    travel_needs[36].origin = 0; travel_needs[36].destination = 2;
    travel_needs[37].origin = 1; travel_needs[37].destination = 0;
    travel_needs[38].origin = 0; travel_needs[38].destination = 2;
    travel_needs[39].origin = 2; travel_needs[39].destination = 1;
    travel_needs[40].origin = 0; travel_needs[40].destination = 1;
    travel_needs[41].origin = 0; travel_needs[41].destination = 1;
    travel_needs[42].origin = 0; travel_needs[42].destination = 1;
    travel_needs[43].origin = 1; travel_needs[43].destination = 0;
    travel_needs[44].origin = 0; travel_needs[44].destination = 2;
    travel_needs[45].origin = 2; travel_needs[45].destination = 1;
    travel_needs[46].origin = 2; travel_needs[46].destination = 1;
    travel_needs[47].origin = 2; travel_needs[47].destination = 1;
    travel_needs[48].origin = 0; travel_needs[48].destination = 2;
    travel_needs[49].origin = 1; travel_needs[49].destination = 0;
}

void app_main() {

    initTravelNeeds();

    //INIT THE BUFFER
    int *data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, data, BUFFER_SIZE);

    gpio_config_t config; 

    // Configure pin LED_PIN_LEVEL_UP as output 
    // Configure pin LED_PIN_LEVEL_MIDDLE as output 
    // Configure pin LED_PIN_LEVEL_DOWN as output 
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_UP;
    config.mode = GPIO_MODE_OUTPUT;
    config.intr_type = GPIO_PIN_INTR_DISABLE;
    config.pull_up_en = 0;
    config.pull_down_en = 0;
    gpio_config(&config);

    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_MIDDLE;
    gpio_config(&config);
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_DOWN;
    gpio_config(&config);

    // Configure pin BUTTON_PIN as input, pull up and with interrupts on the negative edge
    config.pin_bit_mask = (u_int64_t)1 << BUTTON_PIN;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = 1;
    config.pull_down_en = 0;
    config.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&config);

    // Activate the interrupts for the GPIOs
    res = gpio_install_isr_service(0);
    ESP_ERROR_CHECK(res);

    // Add a handler to the ISR for pin BUTTON_PIN
    res = gpio_isr_handler_add(BUTTON_PIN, button_handler, NULL);
    ESP_ERROR_CHECK(res);

    int current_floor = 0; 
    struct travel_need current_travel; 
    int current_data = INT_MIN; 

    // This is where you most likely put your main elevator code. 
    while(1) {
        //REMOVE THE HEAD AND COLLECT IT
        current_data = removeHead(&buffer);

        if (current_data != INT_MIN) {

            //GET THE DATA FOR THE CURRENT TRAVEL
            current_travel = travel_needs[current_data];

            printf("Going to origin: %d\n", current_travel.origin);
            show_level(current_travel.origin);

            vTaskDelay(pdMS_TO_TICKS(100));

            printf("Going to destination: %d\n", current_travel.destination);
            show_level(current_travel.destination);

            //SET NEW DESTINATION AS "ORIGIN"
            current_floor = current_travel.destination;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}