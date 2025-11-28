/*
@Author Tianyun Du
@Date 2025/11/28
@Email tyndu1996@gmail.com
*/



#ifndef __DS3231_H_
#define __DS3231_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"

extern I2C_HandleTypeDef *hi2c_ds3231;
#define DS3231_ADDR 0x68
#define DS3231_CONTROL_REG_ADDR 0x0E

static uint8_t rx_buffer[18];
static uint8_t tx_buffer[18];

typedef enum{
	DS3231_12 = 0,
	DS3231_24,
} DS3231_12_24;

typedef enum{
	DS3231_AM = 0,
	DS3231_PM,
} DS3231_AM_PM;

extern uint8_t ds3231_year;
extern uint8_t ds3231_month;
extern uint8_t ds3231_date;
extern uint8_t ds3231_day;
extern uint8_t ds3231_hours;
extern uint8_t ds3231_minutes;
extern uint8_t ds3231_seconds;
extern DS3231_12_24 time_12_24;
extern DS3231_AM_PM time_am_pm;


extern uint8_t esp32s3_year;
extern uint8_t esp32s3_month;
extern uint8_t esp32s3_date;
extern uint8_t esp32s3_day;
extern uint8_t esp32s3_hours;
extern uint8_t esp32s3_minutes;
extern uint8_t esp32s3_seconds;



extern uint8_t ds3231_minutes;
extern uint8_t ds3231_seconds;

extern HAL_StatusTypeDef ds3231_write_time(uint8_t esp32s3_year,
																					uint8_t esp32s3_month,
																					uint8_t esp32s3_date,
																					uint8_t esp32s3_day,
																					uint8_t esp32s3_hours,
																					uint8_t esp32s3_minutes,
																					uint8_t esp32s3_seconds,
																					DS3231_12_24 time_12_24_f,
																					DS3231_AM_PM am_pm_f);

extern HAL_StatusTypeDef ds3231_read_time(void);

extern HAL_StatusTypeDef ds3231_Init(void);

extern void ds3231_time_parse(void);




#endif
