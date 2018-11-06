#include "nvs.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"

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