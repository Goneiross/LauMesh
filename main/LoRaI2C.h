#include "esp_spi_flash.h"
#include "driver/i2c.h"

#define PIN_I2C_MASTER_DATA 18
#define PIN_I2C_MASTER_CLOCK 19
#define MASTER_PORT 0

#define PIN_I2C_SLAVE1_DATA 25
#define PIN_I2C_SLAVE1_CLOCK 26
#define I2C_SLAVE1_ADRESS 0
#define SLAVE1_PORT 0

#define I2C_RECEVING_BUFFER_SIZE 2048
#define I2C_SENDING_BUFFER_SIZE 2048
#define I2C_DATA_LENGTH 1024

#define i2C_TIMEOUT 1000
#define CLOCK_SPEED 1000 

#define ACK_EN 0x01

#define PIN_IN_1 NULL
#define PIN_IN_2 NULL
#define PIN_IN_3 NULL

#define PIN_OUT_1 NULL
#define PIN_OUT_2 NULL
#define PIN_OUT_3 NULL

/* ########## LORA REGISTER ########## */

#define LORA_PORT_CLK 19
#define LORA_PORT_DATA 18
#define LORA_I2C_ADR 0x00

/* config */
#define LORA_REG_FIFO 0x00
#define LORA_REG_OPMODE 0x01
// ox02 - 0x05 reserved
#define LORA_REG_FRMSB 0x06
#define LORA_REG_FRMID 0x07
#define LORA_REG_FRLSB 0x08

/* RF blocks */
#define LORA_REG_PACONFIG 0x09
#define LORA_REG_PARAMP 0x0A
#define LORA_REG_OCP 0x0b
#define LORA_REG_REGLNA 0x0C

/* Lora page */
#define LORA_REG_FIFO_ADDR_PTR 0x0D
#define LORA_REG_FIFO_ADDR_TX 0x0E
#define LORA_REG_FIFO_ADDR_RX 0x0F
#define LORA_REG_FIFO_ADDR_CURRENT 0x10
#define LORA_REG_FLAGS_MASK 0x11
#define LORA_REG_MASK 0x12
#define LORA_REG_FIFO_RX_BYTES_NB 0x13
#define LORA_REG_RX_VALID_HD_MSB_CURRENT 0x14
#define LORA_REG_RX_VALID_HD_LSB_CURRENT 0x15
#define LORA_REG_RX_VALID_PACKET_MSB_CURRENT 0x16
#define LORA_REG_RX_VALID_PACKET_LSB_CURRENT 0x17
#define LORA_REG_MODEM_STATUS 0x18
#define LORA_REG_PACKET_SNR 0x19
#define LORA_REG_RSSI_PACKET 0x1A
#define LORA_REG_RSSI_VALUE 0x1B
#define LORA_REG_HOP_CHANNEL 0x1C
#define LORA_REG_MODEM_CONFIG1 0x1D
#define LORA_REG_MODEM_CONFIG2 0x1E
#define LORA_REG_SYMB_TIMEOUT_LSB 0x1F
#define LORA_REG_MSB_PREAMBLE 0x20
#define LORA_REG_LSB_PREAMBLE 0x21
#define LORA_REG_PAYLOAD_LENGTH 0x22
#define LORA_REG_PAYLOAD_MAX_LENGTH 0x23
#define LORA_REG_HOP_PERIOD 0x24
#define LORA_REG_FIFO_RX_BYTE_ADDR_PTR 0x25
#define LORA_REG_MODEM_CONFIG3 0x26
#define LORA_REG_PPM_CORRECTION 0x27
#define LORA_REG_FEI_MSB 0x28
#define LORA_REG_FEI_MID 0x29
#define LORA_REG_FEI_LSB 0x2a
// 0x2B reserved
#define LORA_REG_RSSI_WIDEBAND 0x2c
// ox2D - 0x30 reserved
#define LORA_REG_DETECT_OPTI 0x31
// 0x32 reserved
#define LORA_REG_INVERT_IQ 0x33
// 0x34 - 0x36 reserved
#define LORA_REG_DETECT_THRESHOLD 0x37
// 0x38 reserved
#define LORA_REG_SYNC_WORD 0x39
// 0x3A - 0X3F reserved

/* LoRa Mode */
#define LORA_MODE_SLEEP 0x00
#define LORA_MODE_STB 0x01
#define LORA_MODE_TX 0x03
#define LORA_MODE_RX_CONTINUOUS 0x05
#define LORA_MODE_RX_SINGLE 0x06

int i2c_write_reg_adress(uint8_t i2c_adress,uint8_t reg_adress,uint8_t data){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, i2c_adress << 1 | I2C_MASTER_WRITE, ACK_EN);
    i2c_master_write_byte(cmd, i2c_adress << reg_adress | I2C_MASTER_WRITE, ACK_EN);
    i2c_master_write_byte(cmd, i2c_adress << data, ACK_EN);
    i2c_master_stop(cmd);
    i2c_cmd_link_delete(cmd);
    return(0);
}

int i2c_read_reg_adress(uint8_t i2c_adress,uint8_t reg_adress,uint8_t data){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, i2c_adress << 1 | I2C_MASTER_WRITE,ACK_EN);
    i2c_master_write_byte(cmd, i2c_adress << reg_adress, I2C_MASTER_READ);
    i2c_master_read(cmd,&data,I2C_DATA_LENGTH, 0x00);
    i2c_master_stop(cmd);
    return(0);
}

void LoRaGetStatus(){
    uint8_t data_read = 0;
    int signalDetected, signalSyncronixed, rxOn, headerInfValid, modemClear, codingRate = 0;
    i2c_write_reg_adress(LORA_I2C_ADR,0x18,data_read);
    while(data_read > 0)
    {
        if(data_read - 0x80 >= 0){
            codingRate += 4;
            data_read -= 0x80;
        }
        else if(data_read - 0x40 >= 0){
            data_read -= 0x40;
            codingRate += 2;
        }
        else if(data_read - 0x20 >= 0){
            data_read -= 0x20;
            codingRate += 1;
        }
        else if(data_read - 0x10 >= 0){
            data_read -= 0x10;
            modemClear = 1;
        }
        else if(data_read - 0x08 >= 0){
            data_read -= 0x08;
            headerInfValid = 1;
        }
        else if(data_read - 0x04>= 0){
            data_read -= 0x04;
            rxOn = 1;
        }
        else if(data_read - 0x02 >= 0){
            data_read -= 0x02;
            signalSyncronixed = 1;
        }
        else if(data_read - 0x01 >= 0){
            data_read -= 0x01;
            signalDetected = 1;
        }
    }
    //add return
}

uint8_t LoraBufferRead(void* pvParameters){
    uint8_t data_read = 0;
    i2c_read_reg_adress(LORA_I2C_ADR,LORA_REG_FIFO_ADDR_RX,data_read);
    return(data_read);
}

void LoRaBufferWrite(uint8_t data){
    i2c_write_reg_adress(LORA_I2C_ADR,LORA_REG_FIFO_ADDR_TX,data);
}

void LoRaOPMode(uint8_t mode){ //AJOUTER MODE
    int done = 1;
    if (mode == LORA_MODE_SLEEP){ //SLEEP
        done = i2c_write_reg_adress(LORA_I2C_ADR, LORA_REG_OPMODE,0x80);
    }
    else if (mode == LORA_MODE_STB){ //StandBy
        done = i2c_write_reg_adress(LORA_I2C_ADR, LORA_REG_OPMODE,0x81);
    }
    else if (mode == LORA_MODE_TX){ //TX
        done = i2c_write_reg_adress(LORA_I2C_ADR, LORA_REG_OPMODE,0x83);
    }
    else if (mode == LORA_MODE_RX_CONTINUOUS){ //RX continuous
        done = i2c_write_reg_adress(LORA_I2C_ADR, LORA_REG_OPMODE,0x85);
    }
    else if (mode == LORA_MODE_RX_SINGLE){ //RX single
        done = i2c_write_reg_adress(LORA_I2C_ADR, LORA_REG_OPMODE,0x86);
    }
    if (done != 0) {printf("LoRaConfig ERROR\n");}
    else {printf("LoRa Config done\n");}
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
    else {printf("Error\n");}
    vTaskDelete(NULL);
}
