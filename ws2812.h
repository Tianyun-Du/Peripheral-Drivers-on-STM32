/*
@Author Tianyun Du
@Date 2025/11/28
@Email tyndu1996@gmail.com
*/
#ifndef __WS2812_H_
#define __WS2812_H_

#include "stm32f1xx.h"
#include "spi.h"
#include "tim.h"
#include "string.h"
#include "dma.h"

#define NUM_LEDS_ROW 8
#define NUM_LEDS_COL 32
#define Code0 0x80 //1000 0000
#define Code1 0xE0 //1110 0000
#define CodeRESET 0x00 //0000 0000
/*DATA BUFFER*/

typedef struct{
	uint8_t hue;
	uint8_t saturation;
	uint8_t value;
} WS2812SingleLED_HSV;

typedef struct{
	uint8_t green;
	uint8_t red;
	uint8_t blue;
} WS2812SingleLED_RGB;

extern WS2812SingleLED_HSV ledBuffer_hsv[NUM_LEDS_ROW][NUM_LEDS_COL];
static WS2812SingleLED_HSV ledBuffer_hsv_renumber[NUM_LEDS_ROW*NUM_LEDS_COL];
static WS2812SingleLED_RGB ledBuffer_grb [NUM_LEDS_ROW*NUM_LEDS_COL];
static uint8_t colorBuffer[NUM_LEDS_ROW*NUM_LEDS_COL*3];
static uint8_t bitBuffer[NUM_LEDS_ROW*NUM_LEDS_COL*24];
static uint8_t dataBuffer[NUM_LEDS_ROW*NUM_LEDS_COL*24+150]; 

/*DATA BUFFER*/

/*enum of number and alphabet*/

typedef enum{
	num0 = 0,
	num1,
	num2,
	num3,
	num4,
	num5,
	num6,
	num7,
	num8,
	num9,
} WS2812_NUM;
	
typedef enum{
	alphaA = 0,
	alphaB,
	alphaC,
	alphaD,
	alphaE,
	alphaF,
	alphaG,
	alphaH,
	alphaI,
	alphaJ,
	alphaK,
	alphaL,
	alphaM,
	alphaN,
	alphaO,
	alphaP,
	alphaQ,
	alphaR,
	alphaS,
	alphaT,
	alphaU,
	alphaV,
	alphaW,
	alphaX,
	alphaY,
	alphaZ,
} WS2812_ALPHABET;




static void ws2812LoadBuffer(void);
void ws2812SendBuffer(void);
void ws2812ClearBuffer(void);
void ws2812PrintNum(uint8_t dig_f, WS2812_NUM num_f, uint8_t brightness_f);
void ws2812PrintAlphabet(uint8_t col_start, WS2812_ALPHABET alphabet_f, uint8_t brightness_f);

void ws2812Set_Whole_Color(uint8_t hue, uint8_t saturation);

void HSV_to_RGB(uint16_t H, uint8_t S, uint8_t V,
                uint8_t *r, uint8_t *g, uint8_t *b);

#endif

