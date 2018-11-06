#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"

// #include "ssd1306.h"

#include "LoRa868T20D.h"
#include "LoRaI2C.h"
#include "otaUpdate.h"

void runTest(void* pvParamters){
    time_t time0;
    while(1){
        time0 = time(NULL);
        printf("En cours d'execution depuis %ld minutes\n",time0/60);
        vTaskDelay(60000/portTICK_RATE_MS);
    }  
    vTaskDelete(NULL);
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
