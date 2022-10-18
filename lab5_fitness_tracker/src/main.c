#include "accelerometer.h"
#include "circular_buffer.h"

#include <esp_pm.h>
#include <esp_task_wdt.h>

#include <math.h>
#include <stdio.h>

#include <driver/gpio.h>
#include <esp_sleep.h>
#include <driver/i2c.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

/////////////  DEFINITIONS /////////////
/**
 * The reason I have chosen this sampling frequency is because
 * I estimate 180 steps/min = 3 steps/s. 
 * 100 should cover everything. 
 */ 
#define SAMPLING_PERIOD 100   

/**
 * The reason I have chosen this buffer size is because
 * data is stored every 100Ms and emptied every 3000Ms which means 
 * 30 but I increased it to 50 to make sure to cover all.
 */ 
#define BUFF_SIZE 50

/**
 * I have chosen to run the algorithm every 3000Ms 
 * to make sure to cover some periods. 
 * Tried this value back and forth but 10 periods seemed ok. 
 */ 
#define ALGO_PERIOD 3000

/**
 * Minimum SD to avoid converging to 0
 */ 
#define MIN_SD 500

/*
 * Constant applied to SD to detect steps
 * Should be somewhere between 1-3. 
 * Tried 1.1 
 * I HAVE A QUESTION! WHY DOES IT RANGE UP TO 3??
 */ 
#define K 1.1

/**
 * Minimum time between steps, this value is chosen because
 * I'm expecting a delay of approx 300Ms for steps (3 steps/sec)
 */ 
#define MIN_INTRA_STEP_TIME 300

/**
 * Daily goal of steps
 */ 
#define STEPS_GOAL 10

#define LED_PIN 26
#define BUTTON_PIN 14 
//#define SCL_PIN 32
//#define SDA_PIN 33
///////////////////////////////////////




/////////////  INITIATIONS /////////////
SemaphoreHandle_t xSemaphore = NULL;
struct circularBuffer buffer;
int step_count = 0;
///////////////////////////////////////




/////////////  SAMPLING TASK /////////////
static void sampling_task(void *arg) {

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (1) {
     /**
      * Get the acceleration
      * Compute the magnitude - done in accelerometer
      * Place the magnitude into the buffer
      * Print the content of the magnitude - too much text
      */ 
        u_int32_t acc = getMagnitude();
        addElement(&buffer, acc);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SAMPLING_PERIOD));
    } 
}
///////////////////////////////////////




/////////////  BUTTON PRESSED /////////////
void button_handler(void *arg) {
    // give semaphore (pay attention: this is in an ISR!)
    xSemaphoreGiveFromISR(xSemaphore, NULL);
}
///////////////////////////////////////




/////////////  ALGO TASK /////////////
static void algo_task(void *arg) {

    TickType_t xLastWakeTime = xTaskGetTickCount();
    
        while (1) {
            
            // get size of the buffer
            u_int32_t size = getsize(&buffer);
            // get tail for loops
            u_int32_t tail = buffer.tail;
        
            if (size > 0) {
                /**
                 * Compute mean, here do NOT empty the queue when reading it! 
                 * Gets the average of buffer data.
                 */ 
                double mean = 0;

                for (int i = buffer.head; i != tail; i = modulus_inc(buffer.maxLength, i)) {
                    mean += buffer.data[i];
                }
                
                mean = (mean / (double)size);


                /**
                 * Compute SD, here queue must not be emptied yet
                 * Gets the standard deviation of buffer data 
                 * by subtracting the average, in pow. 
                 * pow() = calculate the power (data-average) raised to the base value (2). 
                 */
                double sd = 0;
                
                for (int i = buffer.head; i != tail; i = modulus_inc(buffer.maxLength, i)) {
                    sd += pow((buffer.data[i] - mean), 2);
                }

                //sqrt() = returns the argument's square root 
                sd = sqrt((sd / (double)size));
                if (sd < MIN_SD) sd = MIN_SD;

                /*
                 * Now do the step counting, while also emptying the queue
                 */
                u_int32_t sample;
                u_int32_t lastStepTS = -MIN_INTRA_STEP_TIME;
                
                for (int i = 0; i < size; i++) {
                    // get sample, removing it from queue
                    sample = removeHead(&buffer);

                    // if sample > mean + K * sd
                    if (sample > mean + K * sd) {

                        // AND if time between last step and this sample is > MIN_INTRA_STEP_TIME
                        // we want to check for increase in acceleration
                        if (i * SAMPLING_PERIOD - lastStepTS > MIN_INTRA_STEP_TIME) {
                                    // step found! step_count++;
                                    step_count++;
                                    lastStepTS = i * SAMPLING_PERIOD;
                        }
                    }
                }
            }
            printf("Steps: %d\n", step_count);
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ALGO_PERIOD));
        }
}
///////////////////////////////////////


/////////////  LED /////////////
void led_task(void *arg) {
    while (1) {
        // wait for semaphore
        /**
         * https://www.freertos.org/a00122.html
         * The semaphore must have previously been created with a call to xSemaphoreCreateBinary(), 
         * xSemaphoreCreateMutex() or xSemaphoreCreateCounting().
         * 
         * pdTRUE if the semaphore was obtained. 
         * pdFALSE if xTicksToWait expired without the semaphore becoming available.
        */
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            // flash LED with sequence depending on if step_count > STEPS_GOAL
            if (step_count >= STEPS_GOAL) {
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(200));               
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(LED_PIN, 0);
            }
        }
    }
}
///////////////////////////////////////



/////////////  MAIN /////////////
void app_main() {
    /*
     * LIGHT SLEEP configuration
     * Configure light sleep mode with esp_pm_configure()
     * 
     * From espressif website: 
     * Driver for a peripheral clocked from APB can request the APB frequency 
     * to be set to 80 MHz while the peripheral is used.
     */
    esp_pm_config_esp32_t light_sleep_config = {
        .max_freq_mhz = 80,
        .min_freq_mhz = 15,  
        .light_sleep_enable = 1,
    };
    esp_err_t errr = esp_pm_configure(&light_sleep_config);
    //ESP_ERROR_CHECK(errr); 

    //Creating binary semaphore
    xSemaphore = xSemaphoreCreateBinary();
    
    //Starts task that handles the button
    xTaskCreate(led_task, "led_task", 2048, NULL, 10, NULL);

    // configure button and led pins
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // setup the btn with an internal pullup
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    // enable interrupt on falling (1->0) edge for button pin
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // install ISR service with default configuration
    gpio_install_isr_service(0);

    // attach the interrupt service routine
    gpio_isr_handler_add(BUTTON_PIN, button_handler, NULL);

    // initialise the buffer
    u_int32_t *data = (uint32_t *)malloc(BUFF_SIZE * sizeof(uint32_t));
    initCircularBuffer(&buffer, data, BUFF_SIZE);

    // initialise the I2C bus and the MPU6050
    init();

    /**
     * Create sampling task
     * Priority 0 = idle task 
     * Highest priority is stated in the FreeRTOSConfig.h 
     * NOTE! Task priority ≄ Interrupt priority! 
    */
    xTaskCreate(sampling_task, "sampling", 2048, NULL, 1, NULL);
    xTaskCreate(algo_task, "algo", 2048, NULL, 0, NULL);
}
 ///////////////////////////////////////
