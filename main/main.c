#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define PIN_IN_1 NULL
#define PIN_IN_2 NULL
#define PIN_IN_3 NULL

#define PIN_OUT_1 NULL
#define PIN_OUT_2 NULL
#define PIN_OUT_3 NULL

#define PIN_I2C_MASTER_DATA 18
#define PIN_I2C_MASTER_CLOCK 17

#define PIN_I2C_SLAVE1_DATA 2
#define PIN_I2C_SLAVE1_CLOCK 4
#define I2C_SLAVE1_ADRESS 0

#define T_TIMEOUT 1000
#define CLOCK_SPEED 1000

void I2cMasterInit(){
    i2c_config_t i2cMasterConfig;
    i2cMasterConfig.mode = I2C_MODE_MASTER;
    i2cMasterConfig.sda_io_num = PIN_I2C_MASTER_DATA;
    i2cMasterConfig.sda_pullup_en = true;
    i2cMasterConfig.scl_io_num = PIN_I2C_MASTER_CLOCK;
    i2cMasterConfig.scl_pullup_en = true;
    i2cMasterConfig.master.clk_speed = CLOCK_SPEED;
}

void I2CSalve1Init(){
    i2c_config_t i2cSlaveConfig;
    i2cSlaveConfig.mode = I2C_MODE_SLAVE;
    i2cSlaveConfig.sda_io_num = PIN_I2C_SLAVE1_DATA;
    i2cSlaveConfig.sda_pullup_en = true;
    i2cSlaveConfig.scl_io_num = PIN_I2C_SLAVE1_CLOCK;
    i2cSlaveConfig.scl_pullup_en = true;
    i2cSlaveConfig.slave.addr_10bit_en = true;
    i2cSlaveConfig.slave.slave_addr = I2C_SLAVE1_ADRESS;
}

int I2cTest(void* pvParameters){
    printf("Teste de la connection I2C \n");
    return(0);
}

void app_main(){
    printf("Initialisation en cours ... \n");
    xTaskCreatePinnedToCore(&I2cMasterInit,"I2cMasterInit",NULL,128,0,NULL,0);
    xTaskCreatePinnedToCore(&I2CSalve1Init,"I2CSalve1Init",NULL,128,0,NULL,1);
    xTaskCreatePinnedToCore(&I2cTest,"I2Ctest",NULL,128,0,NULL,1);
}
