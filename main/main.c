#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


#include "nvs.h"
#include "nvs_flash.h"
// #include "ssd1306.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"

#include "LoRa868T20D.h"
#include "LoRaI2C.h"



void runTest(void* pvParamters){
    time_t time0;
    while(1){
        time0 = time(NULL);
        printf("En cours d'execution depuis %ld minutes\n",time0/60);
        vTaskDelay(60000/portTICK_RATE_MS);
    }  
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


void app_main(){
    printf("Initialisation en cours ... \n");
    xTaskCreatePinnedToCore(&runTest,"runTest",2048,NULL,0,NULL,0);
    //xTaskCreatePinnedToCore(&I2cMasterInit,"I2cMasterInit",2048,NULL,4,NULL,0);
    //xTaskCreatePinnedToCore(&I2CSlave1Init,"I2CSlave1Init",2048,NULL,4,NULL,1);
    //xTaskCreatePinnedToCore(&I2cTest,"I2Ctest",2048,NULL,3,NULL,1);
    //LoRaOPMode(LORA_MODE_STB);
    //xTaskCreate(&loadUpdate,"loadUpdate",10000,NULL,4,NULL);
    xTaskCreatePinnedToCore(&LoRa868T20D_ini,"LoRa_ini",16384,UART_NUM_2,5,NULL,1);
    //xTaskCreatePinnedToCore(&LoRa868T20D_read,"LoRa_read",8192,UART_NUM_2,4,NULL,1);
    xTaskCreatePinnedToCore(&LoRa868T20D_write,"LoRa_write",16384,UART_NUM_2,4,NULL,1);

}
