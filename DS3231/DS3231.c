/*
@Author Tianyun Du
@Date 2025/11/28
@Email tyndu1996@gmail.com
*/

#include "ds3231.h"

I2C_HandleTypeDef *hi2c_ds3231 = &hi2c1;

static uint8_t rx_buffer[18] = {0};
static uint8_t tx_buffer[18] = {0};
uint8_t ds3231_date = 0;
uint8_t ds3231_day = 0;
uint8_t ds3231_hours = 0;
uint8_t ds3231_minutes = 0;
uint8_t ds3231_month = 0;
uint8_t ds3231_year = 0;
uint8_t ds3231_seconds = 0;
DS3231_12_24 time_12_24 = DS3231_24;
DS3231_AM_PM time_am_pm = DS3231_AM;

static void ds3231_dec_to_bcd(uint8_t *buffer, uint8_t dec)
{
  *buffer = ((dec / 10) << 4) | (dec % 10);
};

static uint8_t set_bit(uint8_t value, uint8_t bitIndex, uint8_t bitValue)
{
    if (bitIndex > 7) return value;

    if (bitValue)
        value |= (1 << bitIndex);
    else
        value &= ~(1 << bitIndex);

    return value;
};

extern HAL_StatusTypeDef ds3231_write_time(uint8_t esp32s3_year,
																					uint8_t esp32s3_month,
																					uint8_t esp32s3_date,
																					uint8_t esp32s3_day,
																					uint8_t esp32s3_hours,
																					uint8_t esp32s3_minutes,
																					uint8_t esp32s3_seconds,
																					DS3231_12_24 time_12_24_f,
																					DS3231_AM_PM am_pm_f){

	ds3231_dec_to_bcd(&tx_buffer[6],esp32s3_year);
	ds3231_dec_to_bcd(&tx_buffer[5],esp32s3_month);
	ds3231_dec_to_bcd(&tx_buffer[4],esp32s3_date);
	ds3231_dec_to_bcd(&tx_buffer[3],esp32s3_day);
	switch(time_12_24_f){
		case(DS3231_12):
			//bit6 HIGH
			if(am_pm_f == DS3231_AM){
				//bit5 LOW
				ds3231_dec_to_bcd(&tx_buffer[2],esp32s3_hours);
				tx_buffer[2] = set_bit(tx_buffer[2],6,1);
				tx_buffer[2] = set_bit(tx_buffer[2],5,0);
			}
			else if(am_pm_f == DS3231_PM){
				//bit5 HIGH
				ds3231_dec_to_bcd(&tx_buffer[2],esp32s3_hours);
				tx_buffer[2] = set_bit(tx_buffer[2],6,1);
				tx_buffer[2] = set_bit(tx_buffer[2],5,1);
			};
			break;
		case(DS3231_24):
			//bit6 LOW
			ds3231_dec_to_bcd(&tx_buffer[2],esp32s3_hours);
			break;
	};											
	ds3231_dec_to_bcd(&tx_buffer[1],esp32s3_minutes);
	ds3231_dec_to_bcd(&tx_buffer[0],esp32s3_seconds);
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Write(hi2c_ds3231,DS3231_ADDR,0x00,I2C_MEMADD_SIZE_8BIT,tx_buffer,18,HAL_MAX_DELAY);
	return status;
};

extern HAL_StatusTypeDef ds3231_read_time(void){
	//MODE of i2c1_rx DMA is circular. Run once.
	HAL_StatusTypeDef status;
	HAL_I2C_Master_Receive_DMA(hi2c_ds3231,DS3231_ADDR,rx_buffer,18);
	return status;
};

extern HAL_StatusTypeDef ds3231_Init(void){
	HAL_StatusTypeDef status;
	uint8_t tx_control_buffer = 0; //default value for me is 0b0000 0000
	status = HAL_I2C_Mem_Write(hi2c_ds3231,DS3231_ADDR,DS3231_CONTROL_REG_ADDR,I2C_MEMADD_SIZE_8BIT,&tx_control_buffer,1,HAL_MAX_DELAY);
	return status;
};

/*
extern uint8_t ds3231_year;
extern uint8_t ds3231_month;
extern uint8_t ds3231_date;
extern uint8_t ds3231_day;
extern uint8_t ds3231_hours;
extern uint8_t ds3231_minutes;
extern uint8_t ds3231_seconds;
*/

extern void ds3231_time_parse(void){
	//callback of TIM interrupt
	ds3231_year = ((rx_buffer[6]&0xF0)>>4)*10 +
								((rx_buffer[6]&0x0F)>>0);
	
	ds3231_month = ((rx_buffer[5]&0x10)>>4)*10 + 
								((rx_buffer[5]&0x0F)>>0);
	
	ds3231_date = ((rx_buffer[4]&0x30)>>4)*10 + 
								((rx_buffer[4]&0x0F)>>0);
	
	ds3231_day = ((rx_buffer[3]&0x07)>>0);
	
	if (	((rx_buffer[2]&0x40)>>6)	== 1){
		//bit6 HIGH, 12h
		time_12_24 = DS3231_12;
		ds3231_hours = ((rx_buffer[2]&0x10)>>4)*10 +
										((rx_buffer[2]&0x0F)>>0);
		if(	((rx_buffer[5]&0x20)>>5)	== 1){
			//bit5 HIGH, PM
			time_am_pm = DS3231_PM;
		}
		else if(	((rx_buffer[5]&0x20)>>5)	== 0){
			time_am_pm = DS3231_AM;
			//bit5 LOW, AM
		};		
	}
	else if(	((rx_buffer[2]&0x40)>>6)	== 0){
		//bit6 LOW, 24h
		time_12_24 = DS3231_24;
		ds3231_hours = ((rx_buffer[2]&0x30)>>4)*10+
										((rx_buffer[2]&0x0F)>>0);
	};
	
	ds3231_minutes = ((rx_buffer[1]&0x70)>>4)*10 + 
										((rx_buffer[1]&0x0F)>>0);
	
	ds3231_seconds = ((rx_buffer[0]&0x70)>>4)*10 + 
										((rx_buffer[0]&0x0F)>>0);
};
