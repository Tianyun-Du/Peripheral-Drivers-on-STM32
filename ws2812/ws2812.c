/*
@Author Tianyun Du
@Date 2025/11/28
@Email tyndu1996@gmail.com
*/
#include "ws2812.h"

WS2812SingleLED_HSV ledBuffer_hsv[NUM_LEDS_ROW][NUM_LEDS_COL] = {0};
static WS2812SingleLED_HSV ledBuffer_hsv_renumber[NUM_LEDS_ROW*NUM_LEDS_COL] = {0};
static WS2812SingleLED_RGB ledBuffer_grb [NUM_LEDS_ROW*NUM_LEDS_COL] = {0};
static uint8_t colorBuffer[NUM_LEDS_ROW*NUM_LEDS_COL*3] = {0};
static uint8_t bitBuffer[NUM_LEDS_ROW*NUM_LEDS_COL*24] = {0};
static uint8_t dataBuffer[NUM_LEDS_ROW*NUM_LEDS_COL*24+150] = {0}; 



static void ws2812LoadBuffer(void){
	//renumber the LEDs
	ledBuffer_hsv_renumber[0]=ledBuffer_hsv[7][31];
	ledBuffer_hsv_renumber[1]=ledBuffer_hsv[6][31];
	ledBuffer_hsv_renumber[2]=ledBuffer_hsv[5][31];
	ledBuffer_hsv_renumber[3]=ledBuffer_hsv[4][31];
	ledBuffer_hsv_renumber[4]=ledBuffer_hsv[3][31];
	ledBuffer_hsv_renumber[5]=ledBuffer_hsv[2][31];
	ledBuffer_hsv_renumber[6]=ledBuffer_hsv[1][31];
	ledBuffer_hsv_renumber[7]=ledBuffer_hsv[0][31];
	
	ledBuffer_hsv_renumber[8]=ledBuffer_hsv[0][30];
	ledBuffer_hsv_renumber[9]=ledBuffer_hsv[1][30];
	ledBuffer_hsv_renumber[10]=ledBuffer_hsv[2][30];
	ledBuffer_hsv_renumber[11]=ledBuffer_hsv[3][30];
	ledBuffer_hsv_renumber[12]=ledBuffer_hsv[4][30];
	ledBuffer_hsv_renumber[13]=ledBuffer_hsv[5][30];
	ledBuffer_hsv_renumber[14]=ledBuffer_hsv[6][30];
	ledBuffer_hsv_renumber[15]=ledBuffer_hsv[7][30];	

	ledBuffer_hsv_renumber[16]=ledBuffer_hsv[7][29];
	ledBuffer_hsv_renumber[17]=ledBuffer_hsv[6][29];
	ledBuffer_hsv_renumber[18]=ledBuffer_hsv[5][29];
	ledBuffer_hsv_renumber[19]=ledBuffer_hsv[4][29];
	ledBuffer_hsv_renumber[20]=ledBuffer_hsv[3][29];
	ledBuffer_hsv_renumber[21]=ledBuffer_hsv[2][29];
	ledBuffer_hsv_renumber[22]=ledBuffer_hsv[1][29];
	ledBuffer_hsv_renumber[23]=ledBuffer_hsv[0][29];	

	ledBuffer_hsv_renumber[24]=ledBuffer_hsv[0][28];
	ledBuffer_hsv_renumber[25]=ledBuffer_hsv[1][28];
	ledBuffer_hsv_renumber[26]=ledBuffer_hsv[2][28];
	ledBuffer_hsv_renumber[27]=ledBuffer_hsv[3][28];
	ledBuffer_hsv_renumber[28]=ledBuffer_hsv[4][28];
	ledBuffer_hsv_renumber[29]=ledBuffer_hsv[5][28];
	ledBuffer_hsv_renumber[30]=ledBuffer_hsv[6][28];
	ledBuffer_hsv_renumber[31]=ledBuffer_hsv[7][28];	

	ledBuffer_hsv_renumber[32]=ledBuffer_hsv[7][27];
	ledBuffer_hsv_renumber[33]=ledBuffer_hsv[6][27];
	ledBuffer_hsv_renumber[34]=ledBuffer_hsv[5][27];
	ledBuffer_hsv_renumber[35]=ledBuffer_hsv[4][27];
	ledBuffer_hsv_renumber[36]=ledBuffer_hsv[3][27];
	ledBuffer_hsv_renumber[37]=ledBuffer_hsv[2][27];
	ledBuffer_hsv_renumber[38]=ledBuffer_hsv[1][27];
	ledBuffer_hsv_renumber[39]=ledBuffer_hsv[0][27];	

	ledBuffer_hsv_renumber[40]=ledBuffer_hsv[0][26];
	ledBuffer_hsv_renumber[41]=ledBuffer_hsv[1][26];
	ledBuffer_hsv_renumber[42]=ledBuffer_hsv[2][26];
	ledBuffer_hsv_renumber[43]=ledBuffer_hsv[3][26];
	ledBuffer_hsv_renumber[44]=ledBuffer_hsv[4][26];
	ledBuffer_hsv_renumber[45]=ledBuffer_hsv[5][26];
	ledBuffer_hsv_renumber[46]=ledBuffer_hsv[6][26];
	ledBuffer_hsv_renumber[47]=ledBuffer_hsv[7][26];	

	ledBuffer_hsv_renumber[48]=ledBuffer_hsv[7][25];
	ledBuffer_hsv_renumber[49]=ledBuffer_hsv[6][25];
	ledBuffer_hsv_renumber[50]=ledBuffer_hsv[5][25];
	ledBuffer_hsv_renumber[51]=ledBuffer_hsv[4][25];
	ledBuffer_hsv_renumber[52]=ledBuffer_hsv[3][25];
	ledBuffer_hsv_renumber[53]=ledBuffer_hsv[2][25];
	ledBuffer_hsv_renumber[54]=ledBuffer_hsv[1][25];
	ledBuffer_hsv_renumber[55]=ledBuffer_hsv[0][25];	
	
	ledBuffer_hsv_renumber[56]=ledBuffer_hsv[0][24];
	ledBuffer_hsv_renumber[57]=ledBuffer_hsv[1][24];
	ledBuffer_hsv_renumber[58]=ledBuffer_hsv[2][24];
	ledBuffer_hsv_renumber[59]=ledBuffer_hsv[3][24];
	ledBuffer_hsv_renumber[60]=ledBuffer_hsv[4][24];
	ledBuffer_hsv_renumber[61]=ledBuffer_hsv[5][24];
	ledBuffer_hsv_renumber[62]=ledBuffer_hsv[6][24];
	ledBuffer_hsv_renumber[63]=ledBuffer_hsv[7][24];	

	ledBuffer_hsv_renumber[64]=ledBuffer_hsv[7][23];
	ledBuffer_hsv_renumber[65]=ledBuffer_hsv[6][23];
	ledBuffer_hsv_renumber[66]=ledBuffer_hsv[5][23];
	ledBuffer_hsv_renumber[67]=ledBuffer_hsv[4][23];
	ledBuffer_hsv_renumber[68]=ledBuffer_hsv[3][23];
	ledBuffer_hsv_renumber[69]=ledBuffer_hsv[2][23];
	ledBuffer_hsv_renumber[70]=ledBuffer_hsv[1][23];
	ledBuffer_hsv_renumber[71]=ledBuffer_hsv[0][23];

	ledBuffer_hsv_renumber[72]=ledBuffer_hsv[0][22];
	ledBuffer_hsv_renumber[73]=ledBuffer_hsv[1][22];
	ledBuffer_hsv_renumber[74]=ledBuffer_hsv[2][22];
	ledBuffer_hsv_renumber[75]=ledBuffer_hsv[3][22];
	ledBuffer_hsv_renumber[76]=ledBuffer_hsv[4][22];
	ledBuffer_hsv_renumber[77]=ledBuffer_hsv[5][22];
	ledBuffer_hsv_renumber[78]=ledBuffer_hsv[6][22];
	ledBuffer_hsv_renumber[79]=ledBuffer_hsv[7][22];

	ledBuffer_hsv_renumber[80]=ledBuffer_hsv[7][21];
	ledBuffer_hsv_renumber[81]=ledBuffer_hsv[6][21];
	ledBuffer_hsv_renumber[82]=ledBuffer_hsv[5][21];
	ledBuffer_hsv_renumber[83]=ledBuffer_hsv[4][21];
	ledBuffer_hsv_renumber[84]=ledBuffer_hsv[3][21];
	ledBuffer_hsv_renumber[85]=ledBuffer_hsv[2][21];
	ledBuffer_hsv_renumber[86]=ledBuffer_hsv[1][21];
	ledBuffer_hsv_renumber[87]=ledBuffer_hsv[0][21];

	ledBuffer_hsv_renumber[88]=ledBuffer_hsv[0][20];
	ledBuffer_hsv_renumber[89]=ledBuffer_hsv[1][20];
	ledBuffer_hsv_renumber[90]=ledBuffer_hsv[2][20];
	ledBuffer_hsv_renumber[91]=ledBuffer_hsv[3][20];
	ledBuffer_hsv_renumber[92]=ledBuffer_hsv[4][20];
	ledBuffer_hsv_renumber[93]=ledBuffer_hsv[5][20];
	ledBuffer_hsv_renumber[94]=ledBuffer_hsv[6][20];
	ledBuffer_hsv_renumber[95]=ledBuffer_hsv[7][20];
	
	ledBuffer_hsv_renumber[96]=ledBuffer_hsv[7][19];
	ledBuffer_hsv_renumber[97]=ledBuffer_hsv[6][19];
	ledBuffer_hsv_renumber[98]=ledBuffer_hsv[5][19];
	ledBuffer_hsv_renumber[99]=ledBuffer_hsv[4][19];
	ledBuffer_hsv_renumber[100]=ledBuffer_hsv[3][19];
	ledBuffer_hsv_renumber[101]=ledBuffer_hsv[2][19];
	ledBuffer_hsv_renumber[102]=ledBuffer_hsv[1][19];
	ledBuffer_hsv_renumber[103]=ledBuffer_hsv[0][19];	
	
	ledBuffer_hsv_renumber[104]=ledBuffer_hsv[0][18];
	ledBuffer_hsv_renumber[105]=ledBuffer_hsv[1][18];
	ledBuffer_hsv_renumber[106]=ledBuffer_hsv[2][18];
	ledBuffer_hsv_renumber[107]=ledBuffer_hsv[3][18];
	ledBuffer_hsv_renumber[108]=ledBuffer_hsv[4][18];
	ledBuffer_hsv_renumber[109]=ledBuffer_hsv[5][18];
	ledBuffer_hsv_renumber[110]=ledBuffer_hsv[6][18];
	ledBuffer_hsv_renumber[111]=ledBuffer_hsv[7][18];	
	
	ledBuffer_hsv_renumber[112]=ledBuffer_hsv[7][17];
	ledBuffer_hsv_renumber[113]=ledBuffer_hsv[6][17];
	ledBuffer_hsv_renumber[114]=ledBuffer_hsv[5][17];
	ledBuffer_hsv_renumber[115]=ledBuffer_hsv[4][17];
	ledBuffer_hsv_renumber[116]=ledBuffer_hsv[3][17];
	ledBuffer_hsv_renumber[117]=ledBuffer_hsv[2][17];
	ledBuffer_hsv_renumber[118]=ledBuffer_hsv[1][17];
	ledBuffer_hsv_renumber[119]=ledBuffer_hsv[0][17];		
	
	ledBuffer_hsv_renumber[120]=ledBuffer_hsv[0][16];
	ledBuffer_hsv_renumber[121]=ledBuffer_hsv[1][16];
	ledBuffer_hsv_renumber[122]=ledBuffer_hsv[2][16];
	ledBuffer_hsv_renumber[123]=ledBuffer_hsv[3][16];
	ledBuffer_hsv_renumber[124]=ledBuffer_hsv[4][16];
	ledBuffer_hsv_renumber[125]=ledBuffer_hsv[5][16];
	ledBuffer_hsv_renumber[126]=ledBuffer_hsv[6][16];
	ledBuffer_hsv_renumber[127]=ledBuffer_hsv[7][16];	

	ledBuffer_hsv_renumber[128]=ledBuffer_hsv[7][15];
	ledBuffer_hsv_renumber[129]=ledBuffer_hsv[6][15];
	ledBuffer_hsv_renumber[130]=ledBuffer_hsv[5][15];
	ledBuffer_hsv_renumber[131]=ledBuffer_hsv[4][15];
	ledBuffer_hsv_renumber[132]=ledBuffer_hsv[3][15];
	ledBuffer_hsv_renumber[133]=ledBuffer_hsv[2][15];
	ledBuffer_hsv_renumber[134]=ledBuffer_hsv[1][15];
	ledBuffer_hsv_renumber[135]=ledBuffer_hsv[0][15];	

	ledBuffer_hsv_renumber[136]=ledBuffer_hsv[0][14];
	ledBuffer_hsv_renumber[137]=ledBuffer_hsv[1][14];
	ledBuffer_hsv_renumber[138]=ledBuffer_hsv[2][14];
	ledBuffer_hsv_renumber[139]=ledBuffer_hsv[3][14];
	ledBuffer_hsv_renumber[140]=ledBuffer_hsv[4][14];
	ledBuffer_hsv_renumber[141]=ledBuffer_hsv[5][14];
	ledBuffer_hsv_renumber[142]=ledBuffer_hsv[6][14];
	ledBuffer_hsv_renumber[143]=ledBuffer_hsv[7][14];	

	ledBuffer_hsv_renumber[144]=ledBuffer_hsv[7][13];
	ledBuffer_hsv_renumber[145]=ledBuffer_hsv[6][13];
	ledBuffer_hsv_renumber[146]=ledBuffer_hsv[5][13];
	ledBuffer_hsv_renumber[147]=ledBuffer_hsv[4][13];
	ledBuffer_hsv_renumber[148]=ledBuffer_hsv[3][13];
	ledBuffer_hsv_renumber[149]=ledBuffer_hsv[2][13];
	ledBuffer_hsv_renumber[150]=ledBuffer_hsv[1][13];
	ledBuffer_hsv_renumber[151]=ledBuffer_hsv[0][13];	

	ledBuffer_hsv_renumber[152]=ledBuffer_hsv[0][12];
	ledBuffer_hsv_renumber[153]=ledBuffer_hsv[1][12];
	ledBuffer_hsv_renumber[154]=ledBuffer_hsv[2][12];
	ledBuffer_hsv_renumber[155]=ledBuffer_hsv[3][12];
	ledBuffer_hsv_renumber[156]=ledBuffer_hsv[4][12];
	ledBuffer_hsv_renumber[157]=ledBuffer_hsv[5][12];
	ledBuffer_hsv_renumber[158]=ledBuffer_hsv[6][12];
	ledBuffer_hsv_renumber[159]=ledBuffer_hsv[7][12];	

	ledBuffer_hsv_renumber[160]=ledBuffer_hsv[7][11];
	ledBuffer_hsv_renumber[161]=ledBuffer_hsv[6][11];
	ledBuffer_hsv_renumber[162]=ledBuffer_hsv[5][11];
	ledBuffer_hsv_renumber[163]=ledBuffer_hsv[4][11];
	ledBuffer_hsv_renumber[164]=ledBuffer_hsv[3][11];
	ledBuffer_hsv_renumber[165]=ledBuffer_hsv[2][11];
	ledBuffer_hsv_renumber[166]=ledBuffer_hsv[1][11];
	ledBuffer_hsv_renumber[167]=ledBuffer_hsv[0][11];	

	ledBuffer_hsv_renumber[168]=ledBuffer_hsv[0][10];
	ledBuffer_hsv_renumber[169]=ledBuffer_hsv[1][10];
	ledBuffer_hsv_renumber[170]=ledBuffer_hsv[2][10];
	ledBuffer_hsv_renumber[171]=ledBuffer_hsv[3][10];
	ledBuffer_hsv_renumber[172]=ledBuffer_hsv[4][10];
	ledBuffer_hsv_renumber[173]=ledBuffer_hsv[5][10];
	ledBuffer_hsv_renumber[174]=ledBuffer_hsv[6][10];
	ledBuffer_hsv_renumber[175]=ledBuffer_hsv[7][10];	
	
	ledBuffer_hsv_renumber[176]=ledBuffer_hsv[7][9];
	ledBuffer_hsv_renumber[177]=ledBuffer_hsv[6][9];
	ledBuffer_hsv_renumber[178]=ledBuffer_hsv[5][9];
	ledBuffer_hsv_renumber[179]=ledBuffer_hsv[4][9];
	ledBuffer_hsv_renumber[180]=ledBuffer_hsv[3][9];
	ledBuffer_hsv_renumber[181]=ledBuffer_hsv[2][9];
	ledBuffer_hsv_renumber[182]=ledBuffer_hsv[1][9];
	ledBuffer_hsv_renumber[183]=ledBuffer_hsv[0][9];	
	
	ledBuffer_hsv_renumber[184]=ledBuffer_hsv[0][8];
	ledBuffer_hsv_renumber[185]=ledBuffer_hsv[1][8];
	ledBuffer_hsv_renumber[186]=ledBuffer_hsv[2][8];
	ledBuffer_hsv_renumber[187]=ledBuffer_hsv[3][8];
	ledBuffer_hsv_renumber[188]=ledBuffer_hsv[4][8];
	ledBuffer_hsv_renumber[189]=ledBuffer_hsv[5][8];
	ledBuffer_hsv_renumber[190]=ledBuffer_hsv[6][8];
	ledBuffer_hsv_renumber[191]=ledBuffer_hsv[7][8];		

	ledBuffer_hsv_renumber[192]=ledBuffer_hsv[7][7];
	ledBuffer_hsv_renumber[193]=ledBuffer_hsv[6][7];
	ledBuffer_hsv_renumber[194]=ledBuffer_hsv[5][7];
	ledBuffer_hsv_renumber[195]=ledBuffer_hsv[4][7];
	ledBuffer_hsv_renumber[196]=ledBuffer_hsv[3][7];
	ledBuffer_hsv_renumber[197]=ledBuffer_hsv[2][7];
	ledBuffer_hsv_renumber[198]=ledBuffer_hsv[1][7];
	ledBuffer_hsv_renumber[199]=ledBuffer_hsv[0][7];	

	ledBuffer_hsv_renumber[200]=ledBuffer_hsv[0][6];
	ledBuffer_hsv_renumber[201]=ledBuffer_hsv[1][6];
	ledBuffer_hsv_renumber[202]=ledBuffer_hsv[2][6];
	ledBuffer_hsv_renumber[203]=ledBuffer_hsv[3][6];
	ledBuffer_hsv_renumber[204]=ledBuffer_hsv[4][6];
	ledBuffer_hsv_renumber[205]=ledBuffer_hsv[5][6];
	ledBuffer_hsv_renumber[206]=ledBuffer_hsv[6][6];
	ledBuffer_hsv_renumber[207]=ledBuffer_hsv[7][6];

	ledBuffer_hsv_renumber[208]=ledBuffer_hsv[7][5];
	ledBuffer_hsv_renumber[209]=ledBuffer_hsv[6][5];
	ledBuffer_hsv_renumber[210]=ledBuffer_hsv[5][5];
	ledBuffer_hsv_renumber[211]=ledBuffer_hsv[4][5];
	ledBuffer_hsv_renumber[212]=ledBuffer_hsv[3][5];
	ledBuffer_hsv_renumber[213]=ledBuffer_hsv[2][5];
	ledBuffer_hsv_renumber[214]=ledBuffer_hsv[1][5];
	ledBuffer_hsv_renumber[215]=ledBuffer_hsv[0][5];

	ledBuffer_hsv_renumber[216]=ledBuffer_hsv[0][4];
	ledBuffer_hsv_renumber[217]=ledBuffer_hsv[1][4];
	ledBuffer_hsv_renumber[218]=ledBuffer_hsv[2][4];
	ledBuffer_hsv_renumber[219]=ledBuffer_hsv[3][4];
	ledBuffer_hsv_renumber[220]=ledBuffer_hsv[4][4];
	ledBuffer_hsv_renumber[221]=ledBuffer_hsv[5][4];
	ledBuffer_hsv_renumber[222]=ledBuffer_hsv[6][4];
	ledBuffer_hsv_renumber[223]=ledBuffer_hsv[7][4];

	ledBuffer_hsv_renumber[224]=ledBuffer_hsv[7][3];
	ledBuffer_hsv_renumber[225]=ledBuffer_hsv[6][3];
	ledBuffer_hsv_renumber[226]=ledBuffer_hsv[5][3];
	ledBuffer_hsv_renumber[227]=ledBuffer_hsv[4][3];
	ledBuffer_hsv_renumber[228]=ledBuffer_hsv[3][3];
	ledBuffer_hsv_renumber[229]=ledBuffer_hsv[2][3];
	ledBuffer_hsv_renumber[230]=ledBuffer_hsv[1][3];
	ledBuffer_hsv_renumber[231]=ledBuffer_hsv[0][3];
	
	ledBuffer_hsv_renumber[232]=ledBuffer_hsv[0][2];
	ledBuffer_hsv_renumber[233]=ledBuffer_hsv[1][2];
	ledBuffer_hsv_renumber[234]=ledBuffer_hsv[2][2];
	ledBuffer_hsv_renumber[235]=ledBuffer_hsv[3][2];
	ledBuffer_hsv_renumber[236]=ledBuffer_hsv[4][2];
	ledBuffer_hsv_renumber[237]=ledBuffer_hsv[5][2];
	ledBuffer_hsv_renumber[238]=ledBuffer_hsv[6][2];
	ledBuffer_hsv_renumber[239]=ledBuffer_hsv[7][2];	
	
	ledBuffer_hsv_renumber[240]=ledBuffer_hsv[7][1];
	ledBuffer_hsv_renumber[241]=ledBuffer_hsv[6][1];
	ledBuffer_hsv_renumber[242]=ledBuffer_hsv[5][1];
	ledBuffer_hsv_renumber[243]=ledBuffer_hsv[4][1];
	ledBuffer_hsv_renumber[244]=ledBuffer_hsv[3][1];
	ledBuffer_hsv_renumber[245]=ledBuffer_hsv[2][1];
	ledBuffer_hsv_renumber[246]=ledBuffer_hsv[1][1];
	ledBuffer_hsv_renumber[247]=ledBuffer_hsv[0][1];	
	
	ledBuffer_hsv_renumber[248]=ledBuffer_hsv[0][0];
	ledBuffer_hsv_renumber[249]=ledBuffer_hsv[1][0];
	ledBuffer_hsv_renumber[250]=ledBuffer_hsv[2][0];
	ledBuffer_hsv_renumber[251]=ledBuffer_hsv[3][0];
	ledBuffer_hsv_renumber[252]=ledBuffer_hsv[4][0];
	ledBuffer_hsv_renumber[253]=ledBuffer_hsv[5][0];
	ledBuffer_hsv_renumber[254]=ledBuffer_hsv[6][0];
	ledBuffer_hsv_renumber[255]=ledBuffer_hsv[7][0];
	
	//transfer the HSV value of buffer "ledBuffer_hsv_renumber" into rgb value of buffer "ledBuffer_grb"
	for(int i=0; i< NUM_LEDS_ROW*NUM_LEDS_COL; i++){
			HSV_to_RGB(ledBuffer_hsv_renumber[i].hue,ledBuffer_hsv_renumber[i].saturation,ledBuffer_hsv_renumber[i].value,
								&ledBuffer_grb[i].red,&ledBuffer_grb[i].green,&ledBuffer_grb[i].blue);
	};
	
	//transfer the rgb value of buffer "ledBuffer" into g7, g6, g5... value of buffer "colorBuffer"
	for(int i=0; i< NUM_LEDS_ROW*NUM_LEDS_COL; i++){
		colorBuffer[3*i] = ledBuffer_grb[i].green;
		colorBuffer[3*i+1] = ledBuffer_grb[i].red;
		colorBuffer[3*i+2] = ledBuffer_grb[i].blue;
	};
	
	//transfer the g7, g6, g5... value of buffer "colorBuffer" into binary value of buffer "bitBuffer"
	for(int i=0; i< NUM_LEDS_ROW*NUM_LEDS_COL*3; i++){
		bitBuffer[8*i] = ((colorBuffer[i] & 0x80)>>7);
		bitBuffer[8*i+1] = ((colorBuffer[i] & 0x40)>>6);
		bitBuffer[8*i+2] = ((colorBuffer[i] & 0x20)>>5);
		bitBuffer[8*i+3] = ((colorBuffer[i] & 0x10)>>4);
		bitBuffer[8*i+4] = ((colorBuffer[i] & 0x08)>>3);
		bitBuffer[8*i+5] = ((colorBuffer[i] & 0x04)>>2);
		bitBuffer[8*i+6] = ((colorBuffer[i] & 0x02)>>1);
		bitBuffer[8*i+7] = ((colorBuffer[i] & 0x01)>>0);
	};
	
	//transfer the binary value of buffer "bitBuffer" into Code0 or Code1 value of buffer "dataBuffer"
	for(int i=0; i< NUM_LEDS_ROW*NUM_LEDS_COL*24; i++){
		if(bitBuffer[i] == 0){
			dataBuffer[i] = Code0;
		}
		else if (bitBuffer[i] == 1){
			dataBuffer[i] = Code1;
		};
	};

};

void ws2812SendBuffer(void){
	ws2812LoadBuffer();
	HAL_SPI_Transmit_DMA(&hspi1,dataBuffer,NUM_LEDS_ROW*NUM_LEDS_COL*24+150);
	HAL_Delay(1);
};

void ws2812ClearBuffer(void){
	for(int i_row = 0; i_row < NUM_LEDS_ROW; i_row++){
		for(int j_col = 0; j_col < NUM_LEDS_COL; j_col++){
			ledBuffer_hsv[i_row][j_col].hue = 0;
			ledBuffer_hsv[i_row][j_col].saturation = 0;
			ledBuffer_hsv[i_row][j_col].value = 0;
		};
	};
};

/*
dig and num
*/
/*
start point of dig is 
			row  col
dig0:  7    3
dig1:  7    8
dig2:  7    14
dig3:  7    19
dig4:  7    25
dig5:  7    30
*/

/*-------------------------------/-------------/-------------/-------------/------------*/
static uint8_t num_array0[28] = {0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0};
static uint8_t num_array1[28] = {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint8_t num_array2[28] = {1,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,1,0,0,0,1,1,1,0,0,0,1,0};
static uint8_t num_array3[28] = {0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0};
static uint8_t num_array4[28] = {1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1};
static uint8_t num_array5[28] = {1,1,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1};
static uint8_t num_array6[28] = {0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0};
static uint8_t num_array7[28] = {1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1};
static uint8_t num_array8[28] = {0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0};
static uint8_t num_array9[28] = {0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,0,0,0,1,1,0};


void ws2812PrintNum(uint8_t col_start, WS2812_NUM num_f, uint8_t brightness_f){
	static uint8_t num_temp[28] = {0};
	switch(num_f){
		case(0):
			memcpy(num_temp,num_array0,28);
			break;
		case(1):
			memcpy(num_temp,num_array1,28);
			break;
		case(2):
			memcpy(num_temp,num_array2,28);
			break;
		case(3):
			memcpy(num_temp,num_array3,28);
			break;
		case(4):
			memcpy(num_temp,num_array4,28);
			break;
		case(5):
			memcpy(num_temp,num_array5,28);
			break;
		case(6):
			memcpy(num_temp,num_array6,28);
			break;
		case(7):
			memcpy(num_temp,num_array7,28);
			break;
		case(8):
			memcpy(num_temp,num_array8,28);
			break;
		case(9):
			memcpy(num_temp,num_array9,28);
			break;
	};
	static int i_temp = 0;
	for (int i_col=0; i_col > -4; i_col--){
		// i_col = 0, -1, -2, -3
		for (int j_row= 7; j_row > 0; j_row--){
			//j_row = 7,6,5,4,3,2,1
			if (col_start+i_col >= 0){
				ledBuffer_hsv[j_row][col_start+i_col].value = num_temp[i_temp]*brightness_f;
				i_temp++;			
			}
			else if(col_start+i_col < 0){
				ledBuffer_hsv[j_row][col_start+i_col+32].value = num_temp[i_temp]*brightness_f;
				i_temp++;					
			};
		};
	};
	i_temp = 0;
};

/*
alphabet
*/
	
static uint8_t A_array [35]={1,1,1,1,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,0};
static uint8_t B_array [35]={0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1};
static uint8_t C_array [35]={0,1,0,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0};
static uint8_t D_array [35]={0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1};
static uint8_t E_array [35]={1,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1};
static uint8_t F_array [35]={0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1};
static uint8_t G_array [35]={1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0};
static uint8_t H_array [35]={1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1};
static uint8_t I_array [35]={0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0};
static uint8_t J_array [35]={0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0};
static uint8_t K_array [35]={1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1};
static uint8_t L_array [35]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1};
static uint8_t M_array [35]={1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1};
static uint8_t N_array [35]={1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1};
static uint8_t O_array [35]={0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0};
static uint8_t P_array [35]={0,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1};
static uint8_t Q_array [35]={1,0,1,1,1,1,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0};
static uint8_t R_array [35]={1,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1};
static uint8_t S_array [35]={0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0};
static uint8_t T_array [35]={0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1};
static uint8_t U_array [35]={0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1};
static uint8_t V_array [35]={0,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1};
static uint8_t W_array [35]={0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1};
static uint8_t X_array [35]={1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,1};
static uint8_t Y_array [35]={0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
static uint8_t Z_array [35]={1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1};

void ws2812PrintAlphabet(uint8_t col_start, WS2812_ALPHABET alphabet_f, uint8_t brightness_f){
	static uint8_t alphabet_temp[35];
	switch(alphabet_f){
		case(alphaA):
			memcpy(alphabet_temp,A_array,35);
			break;
		case(alphaB):
			memcpy(alphabet_temp,B_array,35);
			break;
		case(alphaC):
			memcpy(alphabet_temp,C_array,35);
			break;
		case(alphaD):
			memcpy(alphabet_temp,D_array,35);
			break;
		case(alphaE):
			memcpy(alphabet_temp,E_array,35);
			break;
		case(alphaF):
			memcpy(alphabet_temp,F_array,35);
			break;
		case(alphaG):
			memcpy(alphabet_temp,G_array,35);
			break;
		case(alphaH):
			memcpy(alphabet_temp,H_array,35);
			break;
		case(alphaI):
			memcpy(alphabet_temp,I_array,35);
			break;
		case(alphaJ):
			memcpy(alphabet_temp,J_array,35);
			break;
		case(alphaK):
			memcpy(alphabet_temp,K_array,35);
			break;
		case(alphaL):
			memcpy(alphabet_temp,L_array,35);
			break;
		case(alphaM):
			memcpy(alphabet_temp,M_array,35);
			break;
		case(alphaN):
			memcpy(alphabet_temp,N_array,35);
			break;
		case(alphaO):
			memcpy(alphabet_temp,O_array,35);
			break;
		case(alphaP):
			memcpy(alphabet_temp,P_array,35);
			break;
		case(alphaQ):
			memcpy(alphabet_temp,Q_array,35);
			break;
		case(alphaR):
			memcpy(alphabet_temp,R_array,35);
			break;
		case(alphaS):
			memcpy(alphabet_temp,S_array,35);
			break;
		case(alphaT):
			memcpy(alphabet_temp,T_array,35);
			break;
		case(alphaU):
			memcpy(alphabet_temp,U_array,35);
			break;
		case(alphaV):
			memcpy(alphabet_temp,V_array,35);
			break;
		case(alphaW):
			memcpy(alphabet_temp,W_array,35);
			break;
		case(alphaX):
			memcpy(alphabet_temp,X_array,35);
			break;
		case(alphaY):
			memcpy(alphabet_temp,Y_array,35);
			break;
		case(alphaZ):
			memcpy(alphabet_temp,Z_array,35);
			break;		
	};
	
	static int j_temp = 0;
	for (int i_col=0; i_col > -5; i_col--){
		// i_col = 0, -1, -2, -3, -4
		for (int j_row= 7; j_row > 0; j_row--){
			//j_row = 7,6,5,4,3,2,1
			if (col_start+i_col >= 0){
				ledBuffer_hsv[j_row][col_start+i_col].value = alphabet_temp[j_temp]*brightness_f;
				j_temp++;			
			}
			else if(col_start+i_col < 0){
				ledBuffer_hsv[j_row][col_start+i_col+32].value = alphabet_temp[j_temp]*brightness_f;
				j_temp++;					
			};
		};
	};
	j_temp = 0;
};

void ws2812Set_Whole_Color(uint8_t hue_f, uint8_t saturation_f){
	for(int i = 0; i < NUM_LEDS_ROW; i++){
		for(int j = 0; j < NUM_LEDS_COL; j++){
			ledBuffer_hsv[i][j].hue = hue_f;
			ledBuffer_hsv[i][j].saturation = saturation_f;
		}
	}
};

void HSV_to_RGB(uint16_t H, uint8_t S, uint8_t V,
                uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint8_t region, remainder, p, q, t;

    if (S == 0)
    {
        *r = V;
        *g = V;
        *b = V;
        return;
    }

    region = H / 43;            // 256 / 6 ˜ 43
    remainder = (H - (region * 43)) * 6;

    p = (V * (255 - S)) >> 8;
    q = (V * (255 - ((S * remainder) >> 8))) >> 8;
    t = (V * (255 - ((S * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            *r = V; *g = t; *b = p;
            break;
        case 1:
            *r = q; *g = V; *b = p;
            break;
        case 2:
            *r = p; *g = V; *b = t;
            break;
        case 3:
            *r = p; *g = q; *b = V;
            break;
        case 4:
            *r = t; *g = p; *b = V;
            break;
        default:
            *r = V; *g = p; *b = q;
            break;
    }
};

