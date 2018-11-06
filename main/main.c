#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "nvs.h"
#include "nvs_flash.h"
// #include "ssd1306.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"
#include "lib.h"

#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "esp_vfs_fat.h"
#include "soc/uart_struct.h"
#include "string.h"

#define PIN_M0 2
#define PIN_M1 4

void runTest(void* pvParamters){
    time_t time0;
    while(1){
        time0 = time(NULL);
        printf("En cours d'execution depuis %ld minutes\n",time0/60);
        vTaskDelay(60000/portTICK_RATE_MS);
    }  
    vTaskDelete(NULL);
}

/*
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
    else {printf("Error\n");}
    vTaskDelete(NULL);
}

void loadUpdate(){
    //WIFI
    nvs_flash_init();
    esp_event_loop_init(NULL,NULL);
    tcpip_adapter_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    esp_wifi_set_mode( WIFI_MODE_STA);
    wifi_config_t wifi_cfg = {
        .sta = {
            .ssid = "MPSI",
            .password = "LivingForMaths"
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg));
    esp_wifi_start();
    //esp_wifi_stop();
    esp_wifi_connect();
    //nvs_flash_deinit();
    //esp_wifi_deinit();
    vTaskDelete(NULL);
    printf("done\n");
}
    */

void LoRa868T20D_ini(uart_port_t uart_num){ 
    
    gpio_pad_select_gpio(PIN_M0);
    gpio_set_direction(PIN_M0, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(PIN_M1);
    gpio_set_direction(PIN_M1, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_M0, 0); //SET M1 and M2 to 0 to select normal mode
    gpio_set_level(PIN_M1, 0);
    
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_2, &uart_config);

    esp_err_t err1 = uart_set_pin(uart_num, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err1 != ESP_OK){
        ESP_LOGI("UART_DRIVER", "ERROR during pinSetting");
    }
    
    /*
    uart_set_baudrate(uart_num, 9600);
    uart_set_word_length(uart_num, UART_DATA_8_BITS);
    uart_set_parity(uart_num, UART_PARITY_DISABLE);
    uart_set_stop_bits(uart_num, UART_STOP_BITS_1);
    uart_set_hw_flow_ctrl(uart_num,UART_HW_FLOWCTRL_DISABLE, 0);
    */

    
    esp_err_t err2 = uart_driver_install(UART_NUM_2,2048, 2048, 0, NULL, 0);
    if (err2 != ESP_OK){
        ESP_LOGI("UART_DRIVER", "ERROR during installation");
    }
    vTaskDelete(NULL);
}

int UART_sendData(uart_port_t uart_num, const char* log, const char* data){
    const int length = strlen(data);
    const int writeBytes = uart_write_bytes(uart_num, data, length);
    ESP_LOGI(log, "Wrote %d bytes", writeBytes);
    return (writeBytes);
}

void LoRa868T20D_write(uart_port_t uart_num){
    vTaskDelay(4000/portTICK_RATE_MS);
    while(true){
        
        UART_sendData(uart_num, "LoRa868T20D_read", "test");
        vTaskDelay(6000/portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void LoRa868T20D_read(uart_port_t uart_num){
    vTaskDelay(5000/portTICK_RATE_MS);
    bool end = false;
    while(!end){
        uint16_t* data[2048];
        int readBytes = uart_read_bytes(uart_num, &data, 2048, 5000);
        ESP_LOGI("LoRa868T20D","Read %d bytes", readBytes);
        if (readBytes > 0){
            printf(data);
        }
        else{
            printf("ERROR no data\n");
        }
        vTaskDelay(6000/portTICK_RATE_MS);
    } 
    vTaskDelete(NULL);
}


void LoRa868T20D_paramTest(uart_port_t uart_num){
    gpio_set_level(PIN_M0, 1); //SET M1 and M2 to 11 to select config mode
    gpio_set_level(PIN_M1, 1);
}

void app_main(){
    printf("Initialisation en cours ... \n");
    xTaskCreatePinnedToCore(&runTest,"runTest",2048,NULL,0,NULL,0);
    //xTaskCreatePinnedToCore(&I2cMasterInit,"I2cMasterInit",2048,NULL,4,NULL,0);
    //(&I2CSlave1Init,"I2CSlave1Init",2048,NULL,4,NULL,1);
    //xTaskCreatePinnedToCore(&I2cTest,"I2Ctest",2048,NULL,3,NULL,1);
    //LoRaOPMode(LORA_MODE_STB);
    //xTaskCreate(&loadUpdate,"loadUpdate",10000,NULL,4,NULL);
    xTaskCreatePinnedToCore(&LoRa868T20D_ini,"LoRa_ini",16384,UART_NUM_2,5,NULL,1);
    //xTaskCreatePinnedToCore(&LoRa868T20D_read,"LoRa_read",8192,UART_NUM_2,4,NULL,1);
    xTaskCreatePinnedToCore(&LoRa868T20D_write,"LoRa_write",16384,UART_NUM_2,4,NULL,1);

}
