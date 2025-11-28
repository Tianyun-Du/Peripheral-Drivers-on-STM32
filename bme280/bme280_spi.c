/*
@Author Tianyun Du
@Date 2025/11/28
@Email tyndu1996@gmail.com
*/

#include "bme280_spi.h"

static uint8_t set_bit(uint8_t value, uint8_t bitIndex, uint8_t bitValue)
{
    if (bitIndex > 7) return value;

    if (bitValue)
        value |= (1 << bitIndex);
    else
        value &= ~(1 << bitIndex);

    return value;
};

static HAL_StatusTypeDef bme280_Set_SensorMode(BME280_MODE mode_f){
	HAL_StatusTypeDef status;
	//change the BIT1 and BIT0 of ctrl_meas register
	uint8_t reg_addr_write = set_bit(bme280_ctrl_meas_addr,7,0);
	uint8_t reg_addr_read = bme280_ctrl_meas_addr;
	
	uint8_t tx_buffer[2] = {0};
	uint8_t rx_buffer[2] = {0};
	
	tx_buffer[0] = reg_addr_read;
	
	//read the value of ctrl_meas register first, then change the bit of BIT1 & BIT0, the other bit reserved
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET); //LOW THE CS PIN
	status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer,rx_buffer,2,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	
	//2025-11-23 assume rx_buffer[1] is the data received from OME280
	switch(mode_f){
		case(BME280_MODE_SLEEP):
			//sleep is 00
			tx_buffer[1] = set_bit(rx_buffer[1],1,0);
			tx_buffer[1] = set_bit(rx_buffer[1],0,0);
			break;	
		case(BME280_MODE_FORCE):
			//force is 01 or 10
			tx_buffer[1] = set_bit(rx_buffer[1],1,1);
			tx_buffer[1] = set_bit(rx_buffer[1],0,0);			
			break;
		case(BME280_MODE_NORMAL):
			//normal is 11
			tx_buffer[1] = set_bit(rx_buffer[1],1,1);
			tx_buffer[1] = set_bit(rx_buffer[1],0,1);		
			break;
	};
	
	tx_buffer[0] = reg_addr_write;
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer,rx_buffer,2,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	return status;
};

static HAL_StatusTypeDef bme280_Set_Config(BME280_T_STANDBY standby_f, BME280_IIR_FILTER filter_f, BME280_SET_SPI spi_f){
	//write bits to config register
	uint8_t reg_addr_write = set_bit(bme280_config_addr,7,0);
	uint8_t reg_addr_read = bme280_config_addr;
	
	uint8_t tx_buffer[2] = {0};
	tx_buffer[0] = reg_addr_write;
	
	//standby BIT7~BIT5
	switch(standby_f){
		case(BME280_T_STANDBY_0_5):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);
			break;
		case(BME280_T_STANDBY_62_5):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);			
			break;
		case(BME280_T_STANDBY_125):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,1);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);			
			break;
		case(BME280_T_STANDBY_250):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,1);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);			
			break;
		case(BME280_T_STANDBY_500):
			tx_buffer[1] = set_bit(tx_buffer[1],7,1);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);			
			break;
		case(BME280_T_STANDBY_1000):
			tx_buffer[1] = set_bit(tx_buffer[1],7,1);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);
			break;
		case(BME280_T_STANDBY_10):
			tx_buffer[1] = set_bit(tx_buffer[1],7,1);
			tx_buffer[1] = set_bit(tx_buffer[1],6,1);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);			
			break;
		case(BME280_T_STANDBY_20):
			tx_buffer[1] = set_bit(tx_buffer[1],7,1);
			tx_buffer[1] = set_bit(tx_buffer[1],6,1);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);			
			break;
	};
	
	//standby BIT4~BIT2
	switch(filter_f){
		case(BME280_IIR_OFF):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);			
			break;
		case(BME280_IIR_2):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);	
			break;
		case(BME280_IIR_4):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,1);
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);				
			break;
		case(BME280_IIR_8):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,1);
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);				
			break;
		case(BME280_IIR_16):
			tx_buffer[1] = set_bit(tx_buffer[1],4,1);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);				
			break;
	};
	
	//set SPI wire, BIT0
	switch(spi_f){
		case(BME280_SPI_3_WIRE):
			tx_buffer[1] = set_bit(tx_buffer[1],0,1);
			break;
		case(BME280_SPI_4_WIRE):
			tx_buffer[1] = set_bit(tx_buffer[1],0,0);
			break;
	};
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_ome280,tx_buffer,2,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
};


static HAL_StatusTypeDef bme280_Set_OversamplingSettings
	(BME280_HUM_OVRS hum_ovrs_f,BME280_TEMP_OVRS temp_ovrs_f,BME280_PRESS_OVRS press_ovrs_f, BME280_MODE mode_f){

	uint8_t reg_addr_write = set_bit(bme280_ctrl_hum_addr,7,0);
	uint8_t reg_addr_read = bme280_ctrl_hum_addr;
		
	uint8_t tx_buffer[2]={0};
	tx_buffer[0] = reg_addr_write;

	switch(hum_ovrs_f){
		//BIT2~BIT0
		case(BME280_HUM_OVRS_SKIP):
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);
			tx_buffer[1] = set_bit(tx_buffer[1],1,0);
			tx_buffer[1] = set_bit(tx_buffer[1],0,0);
			break;
		case(BME280_HUM_OVRS_X1):
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);
			tx_buffer[1] = set_bit(tx_buffer[1],1,0);
			tx_buffer[1] = set_bit(tx_buffer[1],0,1);			
			break;
		case(BME280_HUM_OVRS_X2):
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);
			tx_buffer[1] = set_bit(tx_buffer[1],1,1);
			tx_buffer[1] = set_bit(tx_buffer[1],0,0);			
			break;
		case(BME280_HUM_OVRS_X4):
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);
			tx_buffer[1] = set_bit(tx_buffer[1],1,1);
			tx_buffer[1] = set_bit(tx_buffer[1],0,1);			
			break;
		case(BME280_HUM_OVRS_X8):
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);
			tx_buffer[1] = set_bit(tx_buffer[1],1,0);
			tx_buffer[1] = set_bit(tx_buffer[1],0,0);			
			break;
		case(BME280_HUM_OVRS_X16):
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);
			tx_buffer[1] = set_bit(tx_buffer[1],1,0);
			tx_buffer[1] = set_bit(tx_buffer[1],0,1);			
			break;
	};
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_ome280,tx_buffer,2,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
		
	reg_addr_write = set_bit(bme280_ctrl_meas_addr,7,0);
	reg_addr_read = bme280_ctrl_meas_addr;
	
	tx_buffer[0] = reg_addr_write;

	//set oversampling of temp
	switch(temp_ovrs_f){
		//BIT7~5
		case(BME280_TEMP_OVRS_SKIP):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);
			break;
		case(BME280_TEMP_OVRS_X1):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);			
			break;
		case(BME280_TEMP_OVRS_X2):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,1);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);			
			break;
		case(BME280_TEMP_OVRS_X4):
			tx_buffer[1] = set_bit(tx_buffer[1],7,0);
			tx_buffer[1] = set_bit(tx_buffer[1],6,1);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);			
			break;
		case(BME280_TEMP_OVRS_X8):
			tx_buffer[1] = set_bit(tx_buffer[1],7,1);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,0);			
			break;
		case(BME280_TEMP_OVRS_X16):
			tx_buffer[1] = set_bit(tx_buffer[1],7,1);
			tx_buffer[1] = set_bit(tx_buffer[1],6,0);
			tx_buffer[1] = set_bit(tx_buffer[1],5,1);			
			break;
	};
	//set oversampling of pressure
	switch(press_ovrs_f){
		//BIT4~2
		case(BME280_PRESS_OVRS_SKIP):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);
			break;
		case(BME280_PRESS_OVRS_X1):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);			
			break;
		case(BME280_PRESS_OVRS_X2):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,1);
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);			
			break;
		case(BME280_PRESS_OVRS_X4):
			tx_buffer[1] = set_bit(tx_buffer[1],4,0);
			tx_buffer[1] = set_bit(tx_buffer[1],3,1);
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);			
			break;
		case(BME280_PRESS_OVRS_X8):
			tx_buffer[1] = set_bit(tx_buffer[1],4,1);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,0);			
			break;
		case(BME280_PRESS_OVRS_X16):
			tx_buffer[1] = set_bit(tx_buffer[1],4,1);
			tx_buffer[1] = set_bit(tx_buffer[1],3,0);
			tx_buffer[1] = set_bit(tx_buffer[1],2,1);			
			break;
	};
	
	//set mode
	switch(mode_f){
		//BIT1~0
		case(BME280_MODE_SLEEP):
			tx_buffer[1] = set_bit(tx_buffer[1],1,0);	
			tx_buffer[1] = set_bit(tx_buffer[1],0,0);	
			break;
		case(BME280_MODE_FORCE):
			tx_buffer[1] = set_bit(tx_buffer[1],1,0);	
			tx_buffer[1] = set_bit(tx_buffer[1],0,1);				
			break;
		case(BME280_MODE_NORMAL):
			tx_buffer[1] = set_bit(tx_buffer[1],1,1);	
			tx_buffer[1] = set_bit(tx_buffer[1],0,1);				
			break;
	};
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	
	status = HAL_SPI_Transmit(hspi_ome280,tx_buffer,2,HAL_MAX_DELAY);	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);		
	
	return status;
};
	

HAL_StatusTypeDef bme280_Init(BME_280_OPERATION_MODE operation_mode_f){
	//this function set the operation mode and read the calibration data to variables.
	
	//set operation mode
	switch(operation_mode_f){
		case(BME_280_OPERATION_MODE_WEATHER_MONITORING):
			//force mode, 1sample per minute
			//oversampling: pressure x1, temperature x1, humidity x1;
			//IIR filter: off
			bme280_Set_OversamplingSettings(BME280_HUM_OVRS_X1,BME280_TEMP_OVRS_X1,BME280_PRESS_OVRS_X1,BME280_MODE_FORCE);
			bme280_Set_Config(BME280_T_STANDBY_1000,BME280_IIR_OFF,BME280_SPI_4_WIRE);
			//The parameter "stanby time" is not effective while the mode is FORCE
			//After Init, run bme280_Set_SensorMode(BME280_MODE_FORCE) once a min to acquire data.
			break;
		case(BME_280_OPERATION_MODE_HUMIDITY_SENSING):
			//force mode, 1sample per second
			//oversampling: pressure skip, temperature x1, humidity x1;
			//IIR filter:off
			bme280_Set_OversamplingSettings(BME280_HUM_OVRS_X1,BME280_TEMP_OVRS_X1,BME280_PRESS_OVRS_SKIP,BME280_MODE_FORCE);
			bme280_Set_Config(BME280_T_STANDBY_1000,BME280_IIR_OFF,BME280_SPI_4_WIRE);			
			//The parameter "stanby time" is not effective while the mode is FORCE
			//After Init, run bme280_Set_SensorMode(BME280_MODE_FORCE) once a min to acquire data.
			break;
		case(BME_280_OPERATION_MODE_INDOOR_NAVIGATION):
			//normal mode, standby time 0.5ms
			//oversampling: pressure x16, temperature x2, humidity x1;
			//IIR filter: filter coefficient 16		
			bme280_Set_OversamplingSettings(BME280_HUM_OVRS_X1,BME280_TEMP_OVRS_X2,BME280_PRESS_OVRS_X16,BME280_MODE_NORMAL);
			bme280_Set_Config(BME280_T_STANDBY_0_5,BME280_IIR_16,BME280_SPI_4_WIRE);				
			break;
		case(BME_280_OPERATION_MODE_GAMING):
			//normal mode, standby time 0.5ms
			//oversampling: pressure x4, temperature x1, humidity skip;
			bme280_Set_OversamplingSettings(BME280_HUM_OVRS_SKIP,BME280_TEMP_OVRS_X1,BME280_PRESS_OVRS_X4,BME280_MODE_NORMAL);
			bme280_Set_Config(BME280_T_STANDBY_0_5,BME280_IIR_16,BME280_SPI_4_WIRE);		
			break;
	};
	
	//read calibrate data
	bme280_Read_Calibration();
};

BME280_STATUS bme280_Read_Measuring_Status(void){
	/*
	addr of status register is 0xF3
	BIT3: measuring; BIT0: IM_UPDATE
	*/
	uint8_t reg_addr_read = bme280_status_addr;
	
	uint8_t tx_buffer[2] = {0};
	uint8_t rx_buffer[2] = {0};
	
	tx_buffer[0] = reg_addr_read;
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer,rx_buffer,2,HAL_MAX_DELAY);	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	
	if( (rx_buffer[1] & 0x08)>>3 == 0) return BME280_STATUS_MEASURING;
	if( (rx_buffer[1] & 0x08)>>3 == 1) return BME280_STATUS_NOT_MEASURING;
	
};

BME280_STATUS bme280_Read_UPDATE_Status(void){
	/*
	addr of status register is 0xF3
	BIT3: measuring; BIT0: IM_UPDATE
	*/
	uint8_t reg_addr_read = bme280_status_addr;
	
	uint8_t tx_buffer[2] = {0};
	uint8_t rx_buffer[2] = {0};
	
	tx_buffer[0] = reg_addr_read;

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer,rx_buffer,2,HAL_MAX_DELAY);	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	
	if( (rx_buffer[1] & 0x01)>>0 == 0) return BME280_STATUS_IM_UPDATE;
	if( (rx_buffer[1] & 0x01)>>0 == 1) return BME280_STATUS_NOT_IM_UPDATE;
	
};

//read hum, press and temp raw data
void bme280_ReadData_to_RAW(void){
	uint8_t reg_addr_read = bme280_ctrl_hum_addr;
	
	uint8_t tx_buffer[9] = {0};
	uint8_t rx_buffer[9] = {0};
	
	tx_buffer[0] = reg_addr_read;
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer,rx_buffer,9,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);

/*
PRESSURE RAW DATA
msb[7:0] -> [19:12]
lsb[7:0] -> [11:4]
xlsb[7:4] -> [3:0]
*/
	uint8_t press_msb = (uint32_t)rx_buffer[1] << 12;
	uint8_t press_lsb = (uint32_t)rx_buffer[2] << 4;
	uint8_t press_xlsb = (uint32_t)rx_buffer[3] >> 4;
	bme_280_raw_data.press_raw = press_msb | press_lsb | press_xlsb;
	
/*
TEMPERATURE RAW DATA
msb[7:0] -> [19:12]
lsb[7:0] -> [11:4]
xlsb[7:4] -> [3:0]
*/	
	uint8_t temp_msb = (uint32_t)rx_buffer[4] << 12;
	uint8_t temp_lsb = (uint32_t)rx_buffer[5] << 4;
	uint8_t temp_xlsb = (uint32_t)rx_buffer[6] >> 4;
	bme_280_raw_data.temp_raw = temp_msb | temp_lsb | temp_xlsb;
	
/*
HUMIDITY RAW DATA
msb[7:0] -> [15:8]
lsb[7:0] -> [7:0]
*/	
	uint8_t hum_msb = (uint32_t)rx_buffer[7] << 8;
	uint8_t hum_lsb = rx_buffer[8];
	bme_280_raw_data.hum_raw = hum_msb | hum_lsb;
	
};

//read calibration data once
static void bme280_Read_Calibration(void){
	
/*
read temperature and pressure calibration data	
*/	
	uint8_t reg_addr_read = 0x88;

	uint8_t tx_buffer[25] = {0};
	uint8_t rx_buffer[25] = {0};

	tx_buffer[0] = reg_addr_read;
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer,rx_buffer,25,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	

	bme_280_calibration_data.comp_dig_T1 = (uint16_t)rx_buffer[2]<<8 | (uint16_t)rx_buffer[1];
	bme_280_calibration_data.comp_dig_T2 = (int16_t) ((uint16_t)rx_buffer[4]<<8 | (uint16_t)rx_buffer[3]);
	bme_280_calibration_data.comp_dig_T3 = (int16_t) ((uint16_t)rx_buffer[6]<<8 | (uint16_t)rx_buffer[5]);
	bme_280_calibration_data.comp_dig_P1 = (uint16_t)rx_buffer[8]<<8 | (uint16_t)rx_buffer[7];
	bme_280_calibration_data.comp_dig_P2 = (int16_t) ((uint16_t)rx_buffer[10]<<8 | (uint16_t)rx_buffer[9]);
	bme_280_calibration_data.comp_dig_P3 = (int16_t) ((uint16_t)rx_buffer[12]<<8 | (uint16_t)rx_buffer[11]);
	bme_280_calibration_data.comp_dig_P4 = (int16_t) ((uint16_t)rx_buffer[14]<<8 | (uint16_t)rx_buffer[13]);
	bme_280_calibration_data.comp_dig_P5 = (int16_t) ((uint16_t)rx_buffer[16]<<8 | (uint16_t)rx_buffer[15]);
	bme_280_calibration_data.comp_dig_P6 = (int16_t) ((uint16_t)rx_buffer[18]<<8 | (uint16_t)rx_buffer[17]);
	bme_280_calibration_data.comp_dig_P7 = (int16_t) ((uint16_t)rx_buffer[20]<<8 | (uint16_t)rx_buffer[19]);
	bme_280_calibration_data.comp_dig_P8 = (int16_t) ((uint16_t)rx_buffer[22]<<8 | (uint16_t)rx_buffer[21]);
	bme_280_calibration_data.comp_dig_P9 = (int16_t) ((uint16_t)rx_buffer[24]<<8 | (uint16_t)rx_buffer[23]);

/*
read humidity calibration data	
*/	

	reg_addr_read = 0xA1;

	uint8_t tx_buffer_1[2] = {0};
	uint8_t rx_buffer_1[2] = {0};

	tx_buffer_1[0] = reg_addr_read;

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer_1,rx_buffer_1,2,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	
	
	bme_280_calibration_data.comp_dig_H1 = rx_buffer_1[1];
	
	reg_addr_read = 0xE1;	
	
	uint8_t tx_buffer_2[8] = {0};
	uint8_t rx_buffer_2[8] = {0};	
	
	tx_buffer_2[0] = reg_addr_read;
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	status = HAL_SPI_TransmitReceive(hspi_ome280,tx_buffer_2,rx_buffer_2,8,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	
	
	bme_280_calibration_data.comp_dig_H2 = (int16_t) ((uint16_t)rx_buffer_2[2]<<8 | (uint16_t)rx_buffer_2[1]);
	bme_280_calibration_data.comp_dig_H3 = rx_buffer_2[3];
	int16_t dig_h4_msb;
	int16_t dig_h4_lsb;
	dig_h4_msb = (int16_t)(int8_t)rx_buffer_2[4] * 16;
	dig_h4_lsb = (int16_t)(rx_buffer_2[5] & 0x0F);
	bme_280_calibration_data.comp_dig_H4 = dig_h4_msb | dig_h4_lsb;
	int16_t dig_h5_msb;
	int16_t dig_h5_lsb;
	dig_h5_msb = (int16_t)(int8_t)rx_buffer_2[6] * 16;
	dig_h5_lsb = (int16_t)(rx_buffer_2[5] >> 4);
	bme_280_calibration_data.comp_dig_H5 = dig_h5_msb | dig_h5_lsb;
	bme_280_calibration_data.comp_dig_H6 = (int8_t)rx_buffer_2[7];
	
};


//compensate the data
static int32_t bme280_Read_Temperature(void){

	int32_t var1;
	int32_t var2;
	int32_t temperature;
	int32_t temperature_min = -4000;
	int32_t temperature_max = 8500;
	
	var1 = (int32_t)((bme_280_raw_data.temp_raw / 8) - ((int32_t)bme_280_calibration_data.comp_dig_T1*2));
	//2025-11-24 uint32_t - int32_t ???
	var1 = (var1*((int32_t)bme_280_calibration_data.comp_dig_T2)) / 2048;
	
	var2 = (int32_t)((bme_280_raw_data.temp_raw / 16) - ((int32_t)bme_280_calibration_data.comp_dig_T1));
	var2 = (((var2*var2) / 4096) * ((int32_t)bme_280_calibration_data.comp_dig_T3)) / 16384;
	
	bme_280_calibration_data.t_fine = var1 + var2;
	temperature = (bme_280_calibration_data.t_fine * 5 + 128) / 256;
	
	if (temperature < temperature_min){
		return temperature_min;
	}
	else if (temperature > temperature_max){
		return temperature_max;
	};
	return temperature;

};

static uint32_t bme280_Read_Pressure(void){
  int64_t var1;
  int64_t var2;
  int64_t var3;
  int64_t var4;
  uint32_t pressure;
  uint32_t pressure_min = 3000000;
  uint32_t pressure_max = 11000000;

  var1 = ((int64_t)bme_280_calibration_data.t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bme_280_calibration_data.comp_dig_P6;
  var2 = var2 + ((var1 * (int64_t)bme_280_calibration_data.comp_dig_P5) * 131072);
  var2 = var2 + (((int64_t)bme_280_calibration_data.comp_dig_P4) * 34359738368);
  var1 = ((var1 * var1 * (int64_t)bme_280_calibration_data.comp_dig_P3) / 256) + ((var1 * ((int64_t)bme_280_calibration_data.comp_dig_P2) * 4096));
  var3 = ((int64_t)1) * 140737488355328;
  var1 = (var3 + var1) * ((int64_t)bme_280_calibration_data.comp_dig_P1) / 8589934592;
	
  /* To avoid divide by zero exception */
  if (var1 != 0){
    var4 = 1048576 - bme_280_raw_data.press_raw;
    var4 = (((var4 * 2147483648LL) - var2) * 3125) / var1;
    var1 = (((int64_t)bme_280_calibration_data.comp_dig_P9) * (var4 / 8192) * (var4 / 8192)) / 33554432;
    var2 = (((int64_t)bme_280_calibration_data.comp_dig_P8) * var4) / 524288;
    var4 = ((var4 + var1 + var2) / 256) + (((int64_t)bme_280_calibration_data.comp_dig_P7) * 16);
    pressure = (uint32_t)(((var4 / 2) * 100) / 128);

    if (pressure < pressure_min){
      pressure = pressure_min;
    }
    else if (pressure > pressure_max){
      pressure = pressure_max;
    }
	}
  else{
    pressure = pressure_min;
  }
  return pressure;
};



static uint32_t bme280_Read_Humidity(void){
     
	int32_t var1; 
	int32_t var2;
  int32_t var3;
  int32_t var4;
  int32_t var5;
	uint32_t humidity;
  uint32_t humidity_max = 102400;
 
  var1 = bme_280_calibration_data.t_fine - ((int32_t)76800);
  var2 = (int32_t)(bme_280_raw_data.hum_raw * 16384);
	var3 = (int32_t)(((int32_t)bme_280_calibration_data.comp_dig_H4) * 1048576);
  var4 = ((int32_t)bme_280_calibration_data.comp_dig_H5) * var1;
  var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
  var2 = (var1 * ((int32_t)bme_280_calibration_data.comp_dig_H6)) / 1024;
  var3 = (var1 * ((int32_t)bme_280_calibration_data.comp_dig_H3)) / 2048;
  var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
  var2 = ((var4 * ((int32_t)bme_280_calibration_data.comp_dig_H2)) + 8192) / 16384;
  var3 = var5 * var2;
  var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
  var5 = var3 - ((var4 * ((int32_t)bme_280_calibration_data.comp_dig_H1)) / 16);
  var5 = (var5 < 0 ? 0 : var5);
  var5 = (var5 > 419430400 ? 419430400 : var5);
  humidity = (uint32_t)(var5 / 4096);
 
     if (humidity > humidity_max)
     {
         humidity = humidity_max;
     }
 
     return humidity;
};

void bme280_parse_data(void){

	bme_280_data.temperature = bme280_Read_Temperature();
	bme_280_data.pressure = bme280_Read_Pressure();
	bme_280_data.humidity = bme280_Read_Humidity();
	
};
