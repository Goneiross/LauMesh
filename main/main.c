#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "LoRa.h"

#define PIN_IN_1 NULL
#define PIN_IN_2 NULL
#define PIN_IN_3 NULL

#define PIN_OUT_1 NULL
#define PIN_OUT_2 NULL
#define PIN_OUT_3 NULL

void test(void* pvParamters){
    time_t time0;
    while(1){
        time0 = time(NULL);
        printf("En cours d'execution depuis %ld minutes\n",time0/60);
        vTaskDelay(60000/portTICK_RATE_MS);
        esp_restart();
    }  
    vTaskDelete(NULL);
}

void I2cMasterInit(void* pvParameters){
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
    printf("Master Init Done\n");
    vTaskDelete(NULL);
}

void I2CSlave1Init(void* pvParameters){
    i2c_config_t i2cSlaveConfig;
    i2cSlaveConfig.mode = I2C_MODE_SLAVE;
    i2cSlaveConfig.sda_io_num = PIN_I2C_SLAVE1_DATA;
    i2cSlaveConfig.sda_pullup_en = true;
    i2cSlaveConfig.scl_io_num = PIN_I2C_SLAVE1_CLOCK;
    i2cSlaveConfig.scl_pullup_en = true;
    i2cSlaveConfig.slave.addr_10bit_en = true;
    i2cSlaveConfig.slave.slave_addr = I2C_SLAVE1_ADRESS;
    i2c_driver_install(MASTER_PORT,I2C_MODE_SLAVE,I2C_RECEVING_BUFFER_SIZE,I2C_SENDING_BUFFER_SIZE,0);
    printf("Slave init done\n");
    vTaskDelete(NULL);
}

void I2cTest(void* pvParameters){
    int info;
    uint8_t data_read = 0 ;
    printf("Teste de la connection I2C \n");
    i2c_cmd_handle_t testCmd = i2c_cmd_link_create();
    i2c_master_start(testCmd);
    i2c_master_write_byte(testCmd,I2C_SLAVE1_ADRESS << 1 | I2C_MASTER_READ,ACK_EN);
    i2c_master_read_byte(testCmd,I2C_SLAVE1_ADRESS,0x0);
    i2c_master_read(testCmd,&data_read,I2C_DATA_LENGTH,0);
    i2c_master_stop(testCmd);
    info = i2c_master_cmd_begin(SLAVE1_PORT,testCmd, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(testCmd);
    if(info == 0) {printf(data_read);}
    else {printf("erreur");}
    vTaskDelete(NULL);
}

void LoRaConf(){
    int done = 0;
    done = i2c_write_reg_adress(LORA_I2C_ADR, LoRA_REG_OPMODE,0x81);
    if (done != 0) {printf("LoRaConfig ERROR");}
}

void app_main(){
    printf("Initialisation en cours ... \n");
    xTaskCreatePinnedToCore(&test,"test",2048,NULL,0,NULL,0);
    xTaskCreatePinnedToCore(&I2cMasterInit,"I2cMasterInit",2048,NULL,4,NULL,0);
    xTaskCreatePinnedToCore(&I2CSlave1Init,"I2CSlave1Init",2048,NULL,4,NULL,1);
    xTaskCreatePinnedToCore(&I2cTest,"I2Ctest",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(&LoRaConf,"LoRaConf",2048,NULL,4,NULL,0);
}
