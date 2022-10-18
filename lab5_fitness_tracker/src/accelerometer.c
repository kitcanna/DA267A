#include "accelerometer.h"
#include "math.h"

/*
 * From the Datasheet, page 33:
 * The slave address of the MPU-60X0 is b110100X which is 7 bits long.
 * The LSB bit of the 7 bit address is determined by the logic level on pin AD0.
 * This allows two MPU-60X0s to be connected to the same I2C bus.
 * 
 * If AD0 is LOW, the I2C address of the board will be 0x68. Otherwise, the address will be 0x69.
 */
#define MPU6050_ADDR 0x68

/*
 * From Datasheet, page 40:
 * Register (Hex): 6B
 * Bit7 DEVICE_RESET
 * Bit6 SLEEP
 * Bit5 CYCLE
 * Bit4 -
 * Bit3 TEMP_DIS
 * Bit2, Bit1, Bit0 CLKSEL[2:0]
 */
#define MPU6050_PWR_MGMT_1 0x6B

/*
 * Register (Hex) 19
 * Bit7, Bit6, Bit5, Bit4, Bit3, Bit2, Bit1, Bit0: SMPLRT_DIV[7:0]
 */
#define MPU6050_SMPLRT_DIV 0x19

/*
 * From Datasheet, page 45:
 * Register (Hex): 75
 * Bit7 -
 * Bit6, Bit5, Bit4, Bit3, Bit2, Bit1: WHO_AM_I[6:1]
 * Bit0: -
 */
#define MPU6050_WHO_AM_I 0x75

/*
 * From Datasheet, page 30:
 * Register (Hex) 41
 * Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0: TEMP_OUT[15:8]
 * Register (Hex) 42
 * Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0: TEMP_OUT[7:0]
 */
// #define MPU6050_TEMP_OUT_H 0x41
// #define MPU6050_TEMP_OUT_L 0x42

/*
 * From Datasheet, page 29:
 * Register (Hex) 3B | Bit 7-0: ACCEL_XOUT[15:8]
 * Register (Hex) 3C | Bit 7-0: ACCEL_XOUT[7:0]
 * Register (Hex) 3D | Bit 7-0: ACCEL_YOUT[15:8]
 * Register (Hex) 3E | Bit 7-0: ACCEL_YOUT[7:0]
 * Register (Hex) 3F | Bit 7-0: ACCEL_ZOUT[15:8]
 * Register (Hex) 40 | Bit 7-0: ACCEL_ZOUT[7:0]
 */
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
//#define MPU6050_ACCEL_LSB 16384.f

i2c_cmd_handle_t cmd;
esp_err_t res;

void init () {
    /* 
     * Configure and install driver
     */ 
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,                    // ESP32 acts as master
        .sda_io_num = 33,                           // pin used for SDA
        .sda_pullup_en = GPIO_PULLUP_ENABLE,        // The SDA and SCL lines are active low, so they should be pulled up with resistors
        .scl_io_num = 32,                           // SCL pin number
        .scl_pullup_en = GPIO_PULLUP_ENABLE,        // enables pullup on SDA
        .master.clk_speed = 100000,                 // Standard mode (100 Kbit/s)
    };

    /*
     * Configure I2C controller 0
     */ 
    esp_err_t res = i2c_param_config(I2C_NUM_0, &conf);
    ESP_ERROR_CHECK(res);

    /* 
     * Install driver, no buffers needed in master mode nor special interrupts config
     */
    res = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);

    /* 
     * Configure power mode
     * Here we set all bits of the PWR_MGMT_1 register to 0
     * Create command
     */
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // start command
    res = i2c_master_start(cmd);                
    ESP_ERROR_CHECK(res);

    // set address + write and check for ack
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1);
    ESP_ERROR_CHECK(res);                       

    // write the register address and check for ack
    res = i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
    ESP_ERROR_CHECK(res);

    // write value of the regiter: 0, and check for ack
    res = i2c_master_write_byte(cmd, 0x00, 1);
    ESP_ERROR_CHECK(res);

    // end of command
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    // send the command, 1 second timeout
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);

    // delete command now that it's not needed
    i2c_cmd_link_delete(cmd);


    /** 
     * Reads len bytes and places them into a buffer, buffer must be pre-allocated
     * 
     * Set the sampling frequency
     * The sampling freq is gyro sampling freq / (1 + divider)
     * Setting divider to 250 leads to sampling freq. of 32 Hz
     */
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_SMPLRT_DIV, 1); // write to SMPLRT_DIV
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, 250, 1); // set SMPLRT_DIV to 250
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    // ESP_ERROR_CHECK(res);

    i2c_cmd_link_delete(cmd);

}

u_int32_t getMagnitude() {
    double ACCEL_X = readAccelerometer(MPU6050_ACCEL_XOUT_H, MPU6050_ACCEL_XOUT_L);
    double ACCEL_Y = readAccelerometer(MPU6050_ACCEL_YOUT_H, MPU6050_ACCEL_YOUT_L);
    double ACCEL_Z = readAccelerometer(MPU6050_ACCEL_ZOUT_H, MPU6050_ACCEL_ZOUT_L);

    //Base 2 because of the binary numeral system 
    u_int32_t returnVal = sqrt(pow(ACCEL_X, 2) + pow(ACCEL_Y, 2) + pow(ACCEL_Z, 2));

    return returnVal;
}

int16_t readAccelerometer(uint16_t regH, uint16_t regL) {
        /*
         * Read the latest sampled temperature
         * We need to combine TEMP_OUT_L and TEMP_OUT_H into one 16-bit signed integer
         * and then convert that into C using the formula: t = temp_out /340 + 36.53
         * Create a little buffer where to store the answer
         */
        uint8_t buffer;             
        int16_t accRaw = 0;

        /*
         * Read low register
         * End just the register number with no other data
         */
        cmd = i2c_cmd_link_create();
        res = i2c_master_start(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
        ESP_ERROR_CHECK(res);

        res = i2c_master_write_byte(cmd, regL, 1); // read low first
        ESP_ERROR_CHECK(res);

        res = i2c_master_stop(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        ESP_ERROR_CHECK(res);

        i2c_cmd_link_delete(cmd);

        // wait a little
        vTaskDelay(10 / portTICK_RATE_MS);

        // now read the answer
        cmd = i2c_cmd_link_create();
        res = i2c_master_start(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ, 1); // READ bit set!
        ESP_ERROR_CHECK(res);

        res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
        ESP_ERROR_CHECK(res);

        res = i2c_master_stop(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        ESP_ERROR_CHECK(res);

        i2c_cmd_link_delete(cmd);

        accRaw = buffer;    

        /* 
         * Read high register
         */ 
        cmd = i2c_cmd_link_create();
        res = i2c_master_start(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
        ESP_ERROR_CHECK(res);

        res = i2c_master_write_byte(cmd, regH, 1); // read high
        ESP_ERROR_CHECK(res);

        res = i2c_master_stop(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        ESP_ERROR_CHECK(res);

        i2c_cmd_link_delete(cmd);
        vTaskDelay(10 / portTICK_RATE_MS);

        cmd = i2c_cmd_link_create();
        res = i2c_master_start(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ, 1); // READ bit set!
        ESP_ERROR_CHECK(res);

        res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
        ESP_ERROR_CHECK(res);

        res = i2c_master_stop(cmd);
        ESP_ERROR_CHECK(res);

        res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        ESP_ERROR_CHECK(res);

        i2c_cmd_link_delete(cmd);

        /* 
         * Combine high and low registers into a signed integer
         */
        accRaw |= ((int16_t)buffer) << 8;
        //printf("Acc raw is: %d\n", accRaw);

        return accRaw;

        /*
         * Convert raw value to temperature
         */
        // float temp = ((float)tempRaw) / 340 + 36.53;
        // printf("temperature is: %.2f C\n", temp);
        // vTaskDelay(500 / portTICK_RATE_MS);
}