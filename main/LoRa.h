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

/* ########## LORA REGISTER ########## */

#define LORA_PORT_CLK 19
#define LORA_PORT_DATA 18
#define LORA_I2C_ADR 0x00

/* config */
#define LoRA_REG_FIFO 0x00
#define LoRA_REG_OPMODE 0x01
// ox02 - 0x05 reserved
#define LoRA_REG_FRMSB 0x06
#define LoRA_REG_FRMID 0x07
#define LoRA_REG_FRLSB 0x08

/* RF blocks */
#define LoRA_REG_PACONFIG 0x09
#define LoRA_REG_PARAMP 0x0A
#define LoRA_REG_OCP 0x0b
#define LoRA_REG_REGLNA 0x0C

/* Lora page */
#define LoRA_REG_FIFO_ADDR_PTR 0x0D
#define LoRA_REG_FIFO_ADDR_TX 0x0E
#define LoRA_REG_FIFO_ADDR_RX 0x0F
#define LoRA_REG_FIFO_ADDR_CURRENT 0x10
#define LoRA_REG_FLAGS_MASK 0x11
#define LoRA_REG_MASK 0x12
#define LoRA_REG_FIFO_RX_BYTES_NB 0x13
#define LoRA_REG_RX_VALID_HD_MSB_CURRENT 0x14
#define LoRA_REG_RX_VALID_HD_LSB_CURRENT 0x15
#define LoRA_REG_RX_VALID_PACKET_MSB_CURRENT 0x16
#define LoRA_REG_RX_VALID_PACKET_LSB_CURRENT 0x17
#define LoRA_REG_MODEM_STATUS 0x18
#define LoRA_REG_PACKET_SNR 0x19
#define LoRA_REG_RSSI_PACKET 0x1A
#define LoRA_REG_RSSI_VALUE 0x1B
#define LoRA_REG_HOP_CHANNEL 0x1C
#define LoRA_REG_MODEM_CONFIG1 0x1D
#define LoRA_REG_MODEM_CONFIG2 0x1E
#define LoRA_REG_SYMB_TIMEOUT_LSB 0x1F
#define LoRA_REG_MSB_PREAMBLE 0x20
#define LoRA_REG_LSB_PREAMBLE 0x21
#define LoRA_REG_PAYLOAD_LENGTH 0x22
#define LoRA_REG_PAYLOAD_MAX_LENGTH 0x23
#define LoRA_REG_HOP_PERIOD 0x24
#define LoRA_REG_FIFO_RX_BYTE_ADDR_PTR 0x25
#define LoRA_REG_MODEM_CONFIG3 0x26
#define LoRA_REG_PPM_CORRECTION 0x27
#define LoRA_REG_FEI_MSB 0x28
#define LoRA_REG_FEI_MID 0x29
#define LoRA_REG_FEI_LSB 0x2a
// 0x2B reserved
#define LoRA_REG_RSSI_WIDEBAND 0x2c
// ox2D - 0x30 reserved
#define LoRA_REG_DETECT_OPTI 0x31
// 0x32 reserved
#define LoRA_REG_INVERT_IQ 0x33
// 0x34 - 0x36 reserved
#define LoRA_REG_DETECT_THRESHOLD 0x37
// 0x38 reserved
#define LoRA_REG_SYNC_WORD 0x39
// 0x3A - 0X3F reserved

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