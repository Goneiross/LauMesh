#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "string.h"

#define PIN_M0 2
#define PIN_M1 4

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
    if (writeBytes != length){
        ESP_LOGI(log, "ERROR : %d bytes were not written", length - writeBytes);
    } 
    else {
        ESP_LOGI(log, "Wrote %d bytes", writeBytes);
    }
    return (writeBytes);
}

void LoRa868T20D_write(uart_port_t uart_num){
    vTaskDelay(4000/portTICK_RATE_MS);
    while(true){
        
        UART_sendData(uart_num, "LoRa868T20D_write", "test");
        vTaskDelay(100/portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void LoRa868T20D_read(uart_port_t uart_num){
    vTaskDelay(5000/portTICK_RATE_MS);
    bool end = false;
    while(!end){
        uint16_t* data[2048];
        int readBytes = uart_read_bytes(uart_num, &data, 2048, 5000);
        ESP_LOGI("LoRa868T20D_READ","Read %d bytes", readBytes);
        if (readBytes > 0){
            printf(data);
        }
        else{
            ESP_LOGI("LoRa868T20D_READ","No data read");
        }
        vTaskDelay(100/portTICK_RATE_MS);
    } 
    vTaskDelete(NULL);
}

void LoRa868T20D_paramTest(uart_port_t uart_num){
    gpio_set_level(PIN_M0, 1); //SET M0 and M1 to 11 to select config mode
    gpio_set_level(PIN_M1, 1);
}