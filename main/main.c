#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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
#define MASTER_PORT 0

#define PIN_I2C_SLAVE1_DATA 2
#define PIN_I2C_SLAVE1_CLOCK 4
#define I2C_SLAVE1_ADRESS 0
#define SLAVE1_PORT 0

#define I2C_RECEVING_BUFFER_SIZE 0
#define I2C_SENDING_BUFFER_SIZE 0
#define I2C_DATA_LENGTH 1024

#define i2C_TIMEOUT 1000
#define CLOCK_SPEED 1000


void I2cMasterInit(){
    i2c_config_t i2cMasterConfig;
    i2cMasterConfig.mode = I2C_MODE_MASTER;
    i2cMasterConfig.sda_io_num = PIN_I2C_MASTER_DATA;
    i2cMasterConfig.sda_pullup_en = true;
    i2cMasterConfig.scl_io_num = PIN_I2C_MASTER_CLOCK;
    i2cMasterConfig.scl_pullup_en = true;
    i2cMasterConfig.master.clk_speed = CLOCK_SPEED;
    i2c_driver_install(MASTER_PORT,I2C_MODE_MASTER,I2C_RECEVING_BUFFER_SIZE,I2C_SENDING_BUFFER_SIZE,0);
    i2c_param_config(MASTER_PORT,&i2cMasterConfig);
    i2c_set_timeout(MASTER_PORT,i2C_TIMEOUT);
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
    i2c_driver_install(MASTER_PORT,I2C_MODE_SLAVE,I2C_RECEVING_BUFFER_SIZE,I2C_SENDING_BUFFER_SIZE,0);
}

int I2cTest(void* pvParameters){
    uint8_t data_read = 0 ;
    //esp_err_t testRead;
    printf("Teste de la connection I2C \n");
    i2c_cmd_handle_t testCmd = i2c_cmd_link_create();
    i2c_master_start(testCmd);
    i2c_master_read_byte(testCmd,I2C_SLAVE1_ADRESS,0);
    i2c_master_read(testCmd,data_read,I2C_DATA_LENGTH,0);
    i2c_master_stop(testCmd);
    i2c_cmd_link_delete(testCmd);
    return(0);
}

void app_main(){
    printf("Initialisation en cours ... \n");
    xTaskCreatePinnedToCore(&I2cMasterInit,"I2cMasterInit",NULL,128,0,NULL,0);
    xTaskCreatePinnedToCore(&I2CSalve1Init,"I2CSalve1Init",NULL,128,0,NULL,1);
    xTaskCreatePinnedToCore(&I2cTest,"I2Ctest",NULL,128,0,NULL,1);
}
