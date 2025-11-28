/*
@Author Tianyun Du
@Date 2025/11/28
@Email tyndu1996@gmail.com
*/

#ifndef __BME280_SPI_H_
#define __BME280_SPI_H_

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "spi.h"

SPI_HandleTypeDef *hspi_ome280 = &hspi2;

/*read only DATA register*/
#define bme280_hum_lsb_addr 0xFE
#define bme280_hum_msb_addr 0xFD

#define bme280_temp_xlsb_addr 0xFC
#define bme280_temp_lsb_addr 0xFB
#define bme280_temp_msb_addr 0xFA

#define bme280_press_xlsb_addr 0xF9
#define bme280_press_lsb_addr 0xF8
#define bme280_press_msb_addr 0xF7

/*read write CONTROL register*/
#define bme280_config_addr 0xF5
#define bme280_ctrl_meas_addr 0xF4

/*read only STATUS register*/
#define bme280_status_addr 0xF3

/*read write CONTROL register*/
#define bme280_ctrl_hum_addr 0xF2

/*write only RESET register*/
#define bme280_reset_addr 0xE0

typedef enum{
	BME280_STATUS_MEASURING = 0,
	BME280_STATUS_NOT_MEASURING,
	BME280_STATUS_IM_UPDATE,
	BME280_STATUS_NOT_IM_UPDATE,
} BME280_STATUS;

typedef enum{
	BME280_MODE_SLEEP = 0,
	BME280_MODE_FORCE,
	BME280_MODE_NORMAL,
} BME280_MODE;

typedef enum{
	BME280_HUM_OVRS_SKIP = 0,
	BME280_HUM_OVRS_X1,
	BME280_HUM_OVRS_X2,
	BME280_HUM_OVRS_X4,
	BME280_HUM_OVRS_X8,
	BME280_HUM_OVRS_X16
} BME280_HUM_OVRS;

typedef enum{
	BME280_TEMP_OVRS_SKIP = 0,
	BME280_TEMP_OVRS_X1,
	BME280_TEMP_OVRS_X2,
	BME280_TEMP_OVRS_X4,
	BME280_TEMP_OVRS_X8,
	BME280_TEMP_OVRS_X16,
} BME280_TEMP_OVRS;

typedef enum{
	BME280_PRESS_OVRS_SKIP = 0,
	BME280_PRESS_OVRS_X1,
	BME280_PRESS_OVRS_X2,
	BME280_PRESS_OVRS_X4,
	BME280_PRESS_OVRS_X8,
	BME280_PRESS_OVRS_X16,
} BME280_PRESS_OVRS;

typedef enum{
	BME280_T_STANDBY_0_5,
	BME280_T_STANDBY_62_5,
	BME280_T_STANDBY_125,
	BME280_T_STANDBY_250,
	BME280_T_STANDBY_500,
	BME280_T_STANDBY_1000,
	BME280_T_STANDBY_10,
	BME280_T_STANDBY_20,
} BME280_T_STANDBY;

typedef enum{
	BME280_IIR_OFF,
	BME280_IIR_2,
	BME280_IIR_4,
	BME280_IIR_8,
	BME280_IIR_16,
} BME280_IIR_FILTER;

typedef enum{
	BME280_SPI_3_WIRE,
	BME280_SPI_4_WIRE,
} BME280_SET_SPI;

typedef enum{
	BME_280_OPERATION_MODE_WEATHER_MONITORING,
	BME_280_OPERATION_MODE_HUMIDITY_SENSING,
	BME_280_OPERATION_MODE_INDOOR_NAVIGATION,
	BME_280_OPERATION_MODE_GAMING,
}BME_280_OPERATION_MODE;

static HAL_StatusTypeDef bme280_Set_SensorMode(BME280_MODE mode_f);
static HAL_StatusTypeDef bme280_Set_Config(BME280_T_STANDBY standby_f, BME280_IIR_FILTER filter_f, BME280_SET_SPI spi_f);
static HAL_StatusTypeDef bme280_Set_OversamplingSettings(BME280_HUM_OVRS hum_ovrs_f,BME280_TEMP_OVRS temp_ovrs_f,BME280_PRESS_OVRS press_ovrs_f, BME280_MODE mode_f);
HAL_StatusTypeDef bme280_Init(BME_280_OPERATION_MODE operation_mode_f);

BME280_STATUS bme280_Read_Measuring_Status(void);
BME280_STATUS bme280_Read_UPDATE_Status(void);

//read hum, press and temp raw data
void bme280_ReadData_to_RAW(void);

//read calibration data once
static void bme280_Read_Calibration(void);

typedef struct{
	uint32_t press_raw;
	uint32_t temp_raw;
	uint32_t hum_raw;
} BME_280_RAW_DATA;
static BME_280_RAW_DATA bme_280_raw_data;

typedef struct{
	int32_t t_fine;
	
	uint16_t comp_dig_T1;	//0x88 0x89	| [7:0] [15:8]
	int16_t comp_dig_T2;		//0x8A 0x8B	| [7:0] [15:8]
	int16_t comp_dig_T3;		//0x8C 0x8D	| [7:0] [15:8]
	
	uint16_t comp_dig_P1;	//0x8E 0x8F	| [7:0] [15:8]
	int16_t comp_dig_P2;		//0x90 0x91	| [7:0] [15:8]
	int16_t comp_dig_P3;		//0x92 0x93	| [7:0] [15:8]
	int16_t comp_dig_P4;		//0x94 0x95	| [7:0] [15:8]
	int16_t comp_dig_P5;		//0x96 0x97	| [7:0] [15:8]
	int16_t comp_dig_P6;		//0x98 0x99	| [7:0] [15:8]
	int16_t comp_dig_P7;		//0x9A 0x9B	| [7:0] [15:8]
	int16_t comp_dig_P8;		//0x9C 0x9D	| [7:0] [15:8]
	int16_t comp_dig_P9;		//0x9E 0x9F	| [7:0] [15:8]

	uint8_t comp_dig_H1;	//0xA1 [7:0]
	int16_t comp_dig_H2;	//0xE1 0xE2	| [7:0] [15:8]
	uint8_t comp_dig_H3;	//0xE3 [7:0]
	int16_t comp_dig_H4;	//0xE4 0xE5	| [11:4] [3:0]
	int16_t comp_dig_H5;	//0xE5 0xE6	| [3:0] [11:4]
	int8_t comp_dig_H6;	//0xE7
}	BME_280_CALIBRATION_DATA;
static BME_280_CALIBRATION_DATA bme_280_calibration_data;

typedef struct{
	uint32_t humidity;
	int32_t temperature;
	uint32_t pressure;
}BME_280_DATA;
	
extern BME_280_DATA bme_280_data;

//compensate the data
static uint32_t bme280_Read_Humidity(void);
static int32_t bme280_Read_Temperature(void);
static uint32_t bme280_Read_Pressure(void);

extern void bme280_parse_data(void);

#endif
