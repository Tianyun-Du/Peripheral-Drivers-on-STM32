/*
The data parse function is from the API of BOSCH.
https://github.com/boschsensortec/BME280_SensorAPI
*/

#include "bme280.h"

SPI_HandleTypeDef *hspi_ome280 = &hspi2;
GPIO_TypeDef *BME280_CS_GPIO_Port = GPIOB;
uint16_t BME280_CS_Pin = GPIO_PIN_12;

static uint8_t set_bit(uint8_t value, uint8_t bitIndex, uint8_t bitValue)
{
    if (bitIndex > 7) return value;

    if (bitValue)
        value |= (1 << bitIndex);
    else
        value &= ~(1 << bitIndex);

    return value;
};

static HAL_StatusTypeDef bme280_read_reg(uint8_t reg_addr, uint8_t *reg_data, uint8_t length){
    //ADDR Auto Incremented
    //Set Buffer
    uint8_t tx_buffer[length+1] = {0};
    uint8_t rx_buffer[length+1] = {0};

    //Set Read Reg Addr
    tx_buffer[0] = reg_addr;

    //Read Reg Data
    HAL_GPIO_WritePin(BME280_CS_GPIO_Port, BME280_CS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_ome280, tx_buffer, rx_buffer, length+1);
    HAL_GPIO_WritePin(BME280_CS_GPIO_Port, BME280_CS_Pin, GPIO_PIN_SET);
    if(status != HAL_OK){
        return status;
    }

    //Copy Reg Data
    for(uint8_t i = 0; i < length; i++){
        reg_data[i] = rx_buffer[i+1];
    }

    return status;

};
static HAL_StatusTypeDef bme280_write_reg(uint8_t reg_addr, uint8_t *reg_data){
    //ADDR Not-Auto Incremented
    //Set Buffer
    uint8_t tx_buffer[2] = {0};

    //Set Write Reg Addr
    tx_buffer[0] = reg_addr;
    //Set Write Reg Data
    tx_buffer[1] = reg_data[0];

    //Write Reg Data
    HAL_GPIO_WritePin(BME280_CS_GPIO_Port, BME280_CS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_ome280, tx_buffer, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(BME280_CS_GPIO_Port, BME280_CS_Pin, GPIO_PIN_SET);

    return status;
};

void bme280_read_data(void){
    //read press, temp, hum value in register
    uint8_t reg_data[8] = {0};
    bme280_read_reg(BME280_REG_PRESSURE_MSB, reg_data, 8);

    if(status != HAL_OK){
        return;
    }
    
    //save data into struct bme280_data
    bme280_data.pressure_msb_raw = reg_data[0];
    bme280_data.pressure_lsb_raw = reg_data[1];
    bme280_data.pressure_xlsb_raw = reg_data[2];

    bme280_data.temp_msb_raw = reg_data[3];
    bme280_data.temp_lsb_raw = reg_data[4];
    bme280_data.temp_xlsb_raw = reg_data[5];

    bme280_data.hum_msb_raw = reg_data[6];
    bme280_data.hum_lsb_raw = reg_data[7];

    //without parse data, just save raw data
    bme280_data.pressure_uncompensated = (uint32_t)bme280_data.pressure_msb_raw << 12 | (uint32_t)bme280_data.pressure_lsb_raw << 4 | (uint32_t)bme280_data.pressure_xlsb_raw >> 4;
    bme280_data.temperature_uncompensated = (uint32_t)bme280_data.temp_msb_raw << 12 | (uint32_t)bme280_data.temp_lsb_raw << 4 | (uint32_t)bme280_data.temp_xlsb_raw >> 4;
    bme280_data.humidity_uncompensated = (uint32_t)bme280_data.hum_msb_raw << 8 | (uint32_t)bme280_data.hum_lsb_raw;

};
void bme280_read_config(void){
    //read config value in register
    uint8_t reg_data[1] = {0};
    bme280_read_reg(BME280_REG_CONFIG, reg_data, 1);

    if(status != HAL_OK){
        return;
    }

    //save data into struct bme280_config
    bme280_config.config_raw = reg_data[0];
    
    //parse config value
    bme280_config.t_standby = (bme280_config.config_raw & 0xE0) >> 5;
    bme280_config.filter = (bme280_config.config_raw & 0x1C) >> 2;
    bme280_config.spi3w_en = bme280_config.config_raw & 0x01;

};
void bme280_read_ctrl_meas(void){
    //read ctrl_meas value in register
    uint8_t reg_data[1] = {0};
    bme280_read_reg(BME280_REG_CTRL_MEAS, reg_data, 1);

    if(status != HAL_OK){
        return;
    }

    //save data into struct bme280_ctrl_meas
    bme280_ctrl_meas.ctrl_meas_raw = reg_data[0];

    //parse ctrl_meas value
    bme280_ctrl_meas.osrs_t = (bme280_ctrl_meas.ctrl_meas_raw & 0xE0) >> 5;
    bme280_ctrl_meas.osrs_p = (bme280_ctrl_meas.ctrl_meas_raw & 0x1C) >> 2;
    bme280_ctrl_meas.mode = bme280_ctrl_meas.ctrl_meas_raw & 0x03;
};
void bme280_read_status(void){
    //read status value in register
    uint8_t reg_data[1] = {0};
    bme280_read_reg(BME280_REG_STATUS, reg_data, 1);

    if(status != HAL_OK){
        return;
    }

    //save data into struct bme280_status
    bme280_status.status_raw = reg_data[0];
    
    //parse status value
    bme280_status.im_update = (bme280_status.status_raw & 0x08) >> 3;
    bme280_status.measuring = bme280_status.status_raw & 0x01;
};
void bme280_read_ctrl_hum(void){
    //read ctrl_hum value in register
    uint8_t reg_data[1] = {0};
    bme280_read_reg(BME280_REG_CTRL_HUM, reg_data, 1);

    if(status != HAL_OK){
        return;
    }

    //save data into struct bme280_ctrl_hum
    bme280_ctrl_hum.ctrl_hum_raw = reg_data[0];

    //parse ctrl_hum value
    bme280_ctrl_hum.osrs_h = bme280_ctrl_hum.ctrl_hum_raw & 0x07;
};
void bme280_read_calibration(void){
    uint8_t reg_data[24] = {0};
    bme280_read_reg(0x88, reg_data, 24);

    bme280_calibration.dig_T1 = (reg_data[1] << 8) | reg_data[0];
    bme280_calibration.dig_T2 = (reg_data[3] << 8) | reg_data[2];
    bme280_calibration.dig_T3 = (reg_data[5] << 8) | reg_data[4];

    bme280_calibration.dig_P1 = (reg_data[7] << 8) | reg_data[6];
    bme280_calibration.dig_P2 = (reg_data[9] << 8) | reg_data[8];
    bme280_calibration.dig_P3 = (reg_data[11] << 8) | reg_data[10];
    bme280_calibration.dig_P4 = (reg_data[13] << 8) | reg_data[12];
    bme280_calibration.dig_P5 = (reg_data[15] << 8) | reg_data[14];
    bme280_calibration.dig_P6 = (reg_data[17] << 8) | reg_data[16];
    bme280_calibration.dig_P7 = (reg_data[19] << 8) | reg_data[18];
    bme280_calibration.dig_P8 = (reg_data[19] << 8) | reg_data[18];
    bme280_calibration.dig_P9 = (reg_data[19] << 8) | reg_data[18];

    uint8_t reg_data_1[1] = {0};
    bme280_read_reg(0xA1, reg_data_1, 1);
    bme280_calibration.dig_H1 = reg_data_1[0];

    uint8_t reg_data_2[7] = {0};
    bme280_read_reg(0xE1, reg_data_2, 7);
    bme280_calibration.dig_H2 = (reg_data_2[0] << 8) | reg_data_2[1];
    bme280_calibration.dig_H3 = reg_data_2[2];
    bme280_calibration.dig_H4 = (reg_data_2[3] << 4) | (reg_data_2[4] & 0x0F);
    bme280_calibration.dig_H5 = (reg_data_2[5] << 4) | (reg_data_2[4] >> 4);
    bme280_calibration.dig_H6 = reg_data_2[6];

};


void bme280_write_config(uint8_t t_sb_f, uint8_t filter_f, uint8_t spi3w_en_f){
    //set config value in register

    if (t_sb_f > 7) return;
    if (filter_f > 7) return;
    if (spi3w_en_f > 1) return;

    bme280_config.config_raw = (t_sb_f << 5) | (filter_f << 2) | spi3w_en_f;
    
    uint8_t reg_data[1] = {0};
    reg_data[0] = bme280_config.config_raw;
    bme280_write_reg(BME280_REG_CONFIG, reg_data);

    return;
};
void bme280_write_ctrl_meas(uint8_t osrs_t_f, uint8_t osrs_p_f, uint8_t mode_f){
    //set ctrl_meas value in register
    if (osrs_t_f > 7) return;
    if (osrs_p_f > 7) return;
    if (mode_f > 3) return;

    bme280_ctrl_meas.ctrl_meas_raw = (osrs_t_f << 5) | (osrs_p_f << 2) | mode_f;
    
    uint8_t reg_data[1] = {0};
    reg_data[0] = bme280_ctrl_meas.ctrl_meas_raw;
    bme280_write_reg(BME280_REG_CTRL_MEAS, reg_data);

    return;
};
void bme280_write_ctrl_hum(uint8_t osrs_h_f){
    //set ctrl_hum value in register, must use bme280_write_ctrl_meas() first, then use this function.
    if (osrs_h_f > 7) return;

    bme280_ctrl_hum.ctrl_hum_raw = osrs_h_f;
    
    uint8_t reg_data[1] = {0};
    reg_data[0] = bme280_ctrl_hum.ctrl_hum_raw;
    bme280_write_reg(BME280_REG_CTRL_HUM, reg_data);

    return;
};
void bme280_write_reset(void){
    //set reset value in register
    uint8_t reg_data[1] = {0};
    reg_data[0] = 0xB6;
    bme280_write_reg(BME280_REG_RESET, reg_data);

    return;
};
void bme280_set_opr_mode(BME_280_OPR_MODE mode){
    switch(){
        case(WeatherMonitoring):
            // SensorMode: Forced, 1sample/min
            // OVSR Settings: press * 1, temp * 1, hum * 1
            // IIR filter settings: OFF
            bme280_write_config(5, 0, 0);
            bme280_write_ctrl_meas(1, 1, 1);
            bme280_write_ctrl_hum(1);
            break;
        case(HumiditySensing):
            // SensorMode: Forced, 1sample/second
            // OVSR Settings: press * 0, temp * 1, hum * 1
            // IIR filter settings: OFF
            bme280_write_config(5, 0, 0);
            bme280_write_ctrl_meas(1, 0, 1);
            bme280_write_ctrl_hum(1);
            break;
        case(IndoorNavigation):
            // SensorMode: Normal, t_standby = 0.5ms
            // OVSR Settings: press * 16, temp * 2, hum * 1
            // IIR filter settings: x16
            bme280_write_config(5, 4, 0);
            bme280_write_ctrl_meas(2, 5, 3);
            bme280_write_ctrl_hum(1);
            break;
        case(Gaming):
            // SensorMode: Normal, t_standby = 0.5ms
            // OVSR Settings: press * 4, temp * 1, hum * 0
            // IIR filter settings: x16
            bme280_write_config(5, 4, 0);
            bme280_write_ctrl_meas(1, 3, 3);
            bme280_write_ctrl_hum(1);
            break;
    }
};

void bme280_Init(BME_280_OPR_MODE mode){
    bme280_write_reset();
    bme280_set_opr_mode(mode);
    bme280_read_calibration();
};

static double bme280_compensate_temperature(void){
    double var1;
    double var2;
    double temperature;
    double temperature_min = -40.0;
    double temperature_max = 85.0;

    var1 = (((double)bme280_data.temperature_uncompensated) / 16384.0 - ((double)bme280_calibration.dig_T1) / 1024.0);
    var1 = var1 * ((double)bme280_calibration.dig_T2);
    var2 = (((double)bme280_data.temperature_uncompensated) / 131072.0 - ((double)bme280_calibration.dig_T1) / 8192.0);
    var2 = (var2 * var2) * ((double)bme280_calibration.dig_T3);
    bme280_calibration.t_fine = (int32_t)(var1 + var2);
    temperature = (var1 + var2) / 5120.0;
 
    if (temperature < temperature_min)
    {
         temperature = temperature_min;
    }
    else if (temperature > temperature_max)
    {
         temperature = temperature_max;
    }
 
    return temperature;

};
static double bme280_compensate_pressure(void){
     double var1;
     double var2;
     double var3;
     double pressure;
     double pressure_min = 30000.0;
     double pressure_max = 110000.0;
 
     var1 = ((double)bme280_calibration.t_fine / 2.0) - 64000.0;
     var2 = var1 * var1 * ((double)bme280_calibration.dig_P6) / 32768.0;
     var2 = var2 + var1 * ((double)bme280_calibration.dig_P5) * 2.0;
     var2 = (var2 / 4.0) + (((double)bme280_calibration.dig_P4) * 65536.0);
     var3 = ((double)bme280_calibration.dig_P3) * var1 * var1 / 524288.0;
     var1 = (var3 + ((double)bme280_calibration.dig_P2) * var1) / 524288.0;
     var1 = (1.0 + var1 / 32768.0) * ((double)bme280_calibration.dig_P1);
 
     /* Avoid exception caused by division by zero */
     if (var1 > (0.0))
     {
         pressure = 1048576.0 - (double) bme280_data.pressure_uncompensated;
         pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
         var1 = ((double)bme280_calibration.dig_P9) * pressure * pressure / 2147483648.0;
         var2 = pressure * ((double)bme280_calibration.dig_P8) / 32768.0;
         pressure = pressure + (var1 + var2 + ((double)bme280_calibration.dig_P7)) / 16.0;
 
         if (pressure < pressure_min)
         {
             pressure = pressure_min;
         }
         else if (pressure > pressure_max)
         {
             pressure = pressure_max;
         }
     }
     else /* Invalid case */
     {
         pressure = pressure_min;
     }
 
     return pressure;
};
static double bme280_compensate_humidity(void){
     double humidity;
     double humidity_min = 0.0;
     double humidity_max = 100.0;
     double var1;
     double var2;
     double var3;
     double var4;
     double var5;
     double var6;
 
     var1 = ((double)bme280_calibration.t_fine) - 76800.0;
     var2 = (((double)bme280_calibration.dig_H4) * 64.0 + (((double)bme280_calibration.dig_H5) / 16384.0) * var1);
     var3 = (double) bme280_data.humidity_uncompensated - var2;
     var4 = ((double)bme280_calibration.dig_H2) / 65536.0;
     var5 = (1.0 + (((double)bme280_calibration.dig_H3) / 67108864.0) * var1);
     var6 = 1.0 + (((double)bme280_calibration.dig_H6) / 67108864.0) * var1 * var5;
     var6 = var3 * var4 * (var5 * var6);
     humidity = var6 * (1.0 - ((double)bme280_calibration.dig_H1) * var6 / 524288.0);
 
     if (humidity > humidity_max)
     {
         humidity = humidity_max;
     }
     else if (humidity < humidity_min)
     {
         humidity = humidity_min;
     }
 
     return humidity;
};


void bme280_Data_Parse(void){
    double bme280_compensate_temperature(void);
    double bme280_compensate_pressure(void);
    double bme280_compensate_humidity(void);

};
