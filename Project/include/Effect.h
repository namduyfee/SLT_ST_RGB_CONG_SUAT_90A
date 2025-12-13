#ifndef __EFFECT_H
#define __EFFECT_H 280720151
#include "CH554.H"

sbit BUT_SPEED = P3^0;
sbit BUT_MUSIC = P3^4;
sbit BUT_DON_DA = P3^3;

//*********************************//
#define LED_ON  	0
#define LED_OFF 	1
#define RED         0
#define GREEN       1
#define BLUE        2
#define MAX_STEP		127

#define KEY_UP  		16187647  //UP
#define KEY_DOWN		16220287  //DOWN
#define KEY_ON  		16236607  //ON
#define KEY_OFF  		16203967  //OFF
#define KEY_RED 		16195807  //R
#define KEY_GREEN 		16228447  //G
#define KEY_BLUE  		16212127  //B
#define KEY_WHITE		16244767  //W
#define KEY_ORANGE       	16191727  //C1
#define KEY_GREEN2              16224367  //C2
#define KEY_BLUE2               16208047  //C3
#define KEY_ORANGE2             16199887  //C4
#define KEY_YELLOW	 	16197847  //C10
#define KEY_CYAN		16232527  //C5
#define KEY_PURPLE 		16216207  //C6
#define KEY_ORANGE3             16732335  //C7
#define KEY_PURPLE3      	16773135  //C12
#define KEY_FLASH 		16240687  //FLASH
#define KEY_STROBE		16248847  //STROBE
#define KEY_PURPLE2		16740495  //C9
#define KEY_SPEED_U		16238647 
#define KEY_SPEED_D		16246807  
#define KEY_BLUE3               16222327  //C8
#define KEY_BLUE4               16722135  //C11

/*      NAME            IR CODE    KEY  */
#define KEY_UP_MH  		16748655  //UP
#define KEY_DOWN_MH 		16758855  //DOWN
#define KEY_ON_MH  		16756815  //ON
#define KEY_OFF_MH  		16775175  //OFF
#define KEY_RED_MH 		16750695  //R
#define KEY_GREEN_MH  		16767015  //G
#define KEY_BLUE_MH  		16746615  //B
#define KEY_WHITE_MH 		16754775  //W
#define KEY_ORANGE_MH   	16771095  //C1
#define KEY_GREEN2_MH           16730295  //C2
#define KEY_BLUE2_MH            16738455  //C3
#define KEY_ORANGE2_MH          16712445  //C4
#define KEY_YELLOW_MH 	 	16726215  //C10
#define KEY_CYAN_MH 		16724685  //C5
#define KEY_PURPLE_MH 		16720095  //C6
#define KEY_ORANGE3_MH          16189687  //C7
#define KEY_PURPLE3_MH  	16214167  //C12
#define KEY_FLASH_MH  		16757325  //FLASH
#define KEY_STROBE_MH 		16711935  //STROBE
#define KEY_PURPLE2_MH 		16206007  //C9
#define KEY_SPEED_U_MH		16734375  
#define KEY_SPEED_D_MH		16724175  
#define KEY_BLUE3_MH            16742535  //C8
#define KEY_BLUE4_MH            16230487  //C11
//***********************************************//

#define RGB_FLAG_ON_OFF   0x01

#define RGB_RED      0xff0000  //RED
#define RGB_GREEN    0x00ff00  //GREEN
#define RGB_BLUE     0x0000ff  //BLUE
#define RGB_WHITE    0xffffff  // WHITE
#define RGB_BLACK    0x000000  //
#define RGB_ORANGE   0xff8000  // C1
#define RGB_YELLOW   0xffff00  // C10
#define RGB_CYAN     0x33aa88  // C5
#define RGB_PURPLE   0x9900ff  // C6
#define RGB_GREEN2   0x22ff22  // C2
#define RGB_BLUE2    0x009966  // C3
#define RGB_ORANGE2  0x993300  // C4
#define RGB_ORANGE3  0x996600  // C7
#define RGB_PURPLE2  0xff00c0  // C9
#define RGB_PURPLE3  0xff33ff  // C12
#define RGB_BLUE3    0x00ff33  // C8
#define RGB_BLUE4    0x00ff11  // C11

#define LED7_DATA_ON  1
#define LED7_DATA_OFF 0

sbit LED7_DATA_A  = P3^5;
sbit LED7_DATA_B  = P1^5;
sbit LED7_DATA_C  = P1^6;
sbit LED7_DATA_D  = P1^4;
sbit LED7_DATA_E  = P3^2;
sbit LED7_DATA_F  = P1^3;
sbit LED7_DATA_G  = P1^2;

sbit GPIO_RED		=	P1^7;
sbit GPIO_GREEN	=	P1^0;
sbit GPIO_BLUE	=	P3^1;
#define GPIO_RED_ON()      	GPIO_RED = 1
#define GPIO_RED_OFF()      GPIO_RED = 0
#define GPIO_GREEN_ON()     GPIO_GREEN = 1
#define GPIO_GREEN_OFF()    GPIO_GREEN = 0
#define GPIO_BLUE_ON()      GPIO_BLUE	= 1
#define GPIO_BLUE_OFF()     GPIO_BLUE	= 0

#define MAX_BRIGHTNESS  20
#define MIN_BRIGHTNESS  0
#define CONST_BRIGHTNESS 0.05
#define MAX_SPEED       254
#define MIN_SPEED       1
//#define VALUE_TEMP      (65535/MAX_SPEED)
#define VALUE_TEMP      1000
#define ADC_MAX         200
typedef enum
{
	__NO_KEY,
	__KEY_UP,
	__KEY_SPEED,
	__KEY_MUSIC,
	__KEY_MONO,
	__KEY_FLASH,
	__KEY_MONO_LONG,
	__KEY_FLASH_LONG
}FLAG_KEY;
typedef struct
{
    UINT8            Duty;
}RGB_PIN;
typedef struct
{

    uint8_t DutyBlue;
    uint8_t DutyGreen;
    uint8_t DutyRed;
}COLOR_RGB;
typedef struct
{
    COLOR_RGB Color;
    uint8_t   Speed;
    uint8_t   Brightness;
    uint8_t   Effect;
    uint8_t   Flags;
    uint8_t   Mode;
    uint8_t   LastEffect;
    uint8_t   SaveEffect;
    uint8_t   AutoEffect;
	uint32_t color_but_don_da;
	uint32_t color_but_music;
	uint32_t color_but_sp;
}RGB_DATA;
#define DUTYBLUE 				0
#define DUTYGREEN 			1
#define DUTYRED 				2
#define SPEED 					3
#define BRIGHTNESS 			4
#define EFFECT         	5
#define FLAGS 					6
#define MODE 						7
#define LASTEFFECT 			8
#define SAVEEFFECT  		9
#define AUTOEFFECT 			10

void RGB_PinInit(void);
uint8_t Audio_Convert(uint8_t ADC_Value_In);
void RGB_SetModeSpeed(uint8_t Mode);
void RGB_SetSpeed(uint8_t Speed);
void RGB_SetBright(uint8_t Brightness);
void RGB_SetColor(uint32_t Color, uint8_t Brightness);
void LED7_OutData(uint8_t Data);
void Display_LED_7(uint8_t Num);
FLAG_KEY KEY_GetKey (void);
void SetColor(uint8_t color, uint8_t duty, uint8_t bright);
void RGB_OnDisplay(void);
void RGB_OffDisplay(void);
void Check_Button(uint8_t Bright); 
void RGB_ProcessKey(uint32_t Key, uint8_t Bright);
void Program_AutoEffect(uint8_t Auto_Effect);
void Program_Effect(uint8_t Prog_Effect, uint8_t Bright);

#endif

extern bit BIT_TMP;
