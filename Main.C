
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 触摸按键中断和查询方式进行采集并报告当前采样通道按键状态，包含初始化和按键采样等演示函数
*******************************************************************************/
#include "CH554.H"
#include "Debug.H"
#include "TouchKey.H"
#include <stdio.h>
#include "Timer0.H"
#include "Effect.h"
#include "savedata.h"
#include "WriteFlashROM.h"
#include "ADC.H"
#include "Effect_RGBDance.h"
#include "USB_Serial.h"
#pragma  NOAREGS
/* GPIO config
		|	Pn_MOD_OC		|	Pn_DIR_PU		|		Chuc Nang
		|			0				|			0				|	dau vao tro khang cao, khong co dien tro keo len tai cac chan
		|			0				|			1				| Dau ra Push-Pull, Cung cap dong lon
		|			1				|			0				| Dau ra Open-Drain, khong co dien tro keo tai cac chan
		|			1				|			1				| Tuong thich voi 8051 chuan
		trong do n la so port
*/
/* c醕 truong version
	xx - phi阯 ban
	xx - ng鄖
	xx - th醤g
	xx - nam
*/ 
#define VERSION		0x10080721
/* in project */

void handler_color_button(void); 
uint32_t color_disp;
uint32_t color_disp_old = 0;
uint8_t start_select_color = 0;
UINT16 brtnessold = 0; 

const UINT32 code  VALUE1 = 0xff0000, VALUE2 = 0x00ff00, VALUE3 = 0x0000ff;
const UINT16 code BRTNESS = 0x000f;
UINT32 VALUE1Ram,VALUE2Ram,VALUE3Ram;


UINT16 BRIGHTNESSRam = 15; 
//

uint32_t xdata i,j,Tongle_Led =0;
uint8_t idata Flag_IR_Convert, Flag_Speed_Effect, Display_Effect, Run_Effect_Done;
uint32_t xdata IR_Data = 0;
uint8_t xdata dB_Data;
extern uint8_t xdata _FlagFade;
extern uint8_t idata But_Count;
bit BIT_TMP;
RGB_DATA RGB_DataRam; 
uint8_t xdata DataFlash[3] = {0x01,0x02,0x03};
extern uint8_t idata ADC_Value;
sbit	LED2=P1^5;
sbit	LED=P1^4;
sbit 	LED_R = P1^7;
sbit 	LED_G = P1^0;
sbit 	LED_B = P3^1;

main( )
{
	CfgFsys(); 
	mDelaymS(30);
	P3_MOD_OC = 0x18;			//0x19;
	P1_MOD_OC	=	0;
	P3_DIR_PU |= 0x3F;		//0x3F;
	P1_DIR_PU =	0xFD;
		
	CH554WDTModeSelect(1);
	CH554WDTFeed(0x00);

//	RGB_PinInit();
	LOADSAVEDATA(); // load gia tri luu trong APROM
	mDelaymS(10);
		 
	Timer0_Init(8);								//50us
	
	Timer1_Init(8);									// 4us
	Timer2_InputCapture_Init();
	ADC_Init();

	But_Count = 6 + RGB_DataRam.Effect;

    if(!RGB_DataRam.Speed)
        RGB_DataRam.Speed = 12;
    RGB_SetSpeed(RGB_DataRam.Speed);



//			EIP |= SET_BIT2;EIPH |= SET_BIT2;//MUC UU TIEN CAO NHAT
//			IPH |= SET_BIT1;IP &= ~SET_BIT1;//MUC UU TIEN CAO NHI
//			EIP1 &= ~SET_BIT1; EIPH1 &= ~SET_BIT1;//MUC UU TIEN THAP NHAT
	EA = 1; // enable interrup

	color_disp = RGB_GREEN;
	RGB_DataRam.Effect = 0;
	
//	RGB_SetColor(color_disp, (UINT8)BRIGHTNESSRam);

	

	while(1)
	{
		CH554WDTFeed(0x00);	

		handler_color_button();
		if(color_disp_old != color_disp || brtnessold != BRIGHTNESSRam) {
		  RGB_SetColor(color_disp, (UINT8)BRIGHTNESSRam);
		  RGB_OnDisplay();
		  color_disp_old = color_disp;
		  brtnessold = BRIGHTNESSRam; 
		}		
	}
}

#define TIME_SAMLE 6

uint32_t xdata key_color[16] = {KEY_RED, KEY_GREEN, KEY_BLUE, KEY_WHITE, KEY_ORANGE, KEY_GREEN2, KEY_BLUE2, KEY_ORANGE2,
								KEY_YELLOW, KEY_CYAN, KEY_PURPLE, KEY_ORANGE3, KEY_PURPLE3, KEY_PURPLE2, KEY_BLUE3, KEY_BLUE4};

uint32_t xdata key_colorMH[16] = {KEY_RED_MH, KEY_GREEN_MH, KEY_BLUE_MH, KEY_WHITE_MH, KEY_ORANGE_MH, KEY_GREEN2_MH, KEY_BLUE2_MH, KEY_ORANGE2_MH,
								KEY_YELLOW_MH, KEY_CYAN_MH, KEY_PURPLE_MH, KEY_ORANGE3_MH, KEY_PURPLE3_MH, KEY_PURPLE2_MH, KEY_BLUE3_MH, KEY_BLUE4_MH};

uint32_t color[16] = {RGB_RED, RGB_GREEN, RGB_BLUE, RGB_WHITE, RGB_ORANGE, RGB_GREEN2, RGB_BLUE2, RGB_ORANGE2, 
					RGB_YELLOW, RGB_CYAN, RGB_PURPLE, RGB_ORANGE3, RGB_PURPLE3, RGB_PURPLE2, RGB_BLUE3, RGB_BLUE4};
uint32_t stat_but_dd = 0, stat_but_music = 0, stat_but_sp = 0;

void handler_color_button(void) 
{
	uint8_t tm_cnt = 0;

	uint8_t i;
	/* sample button avoid noise (TIME_SAMLE * 1ms) */
	mDelaymS(1);
	if(BUT_DON_DA == 0) {
		stat_but_dd = stat_but_dd + 1;
	}		
	else 
		stat_but_dd = 0;
	if(BUT_MUSIC == 0) {
		stat_but_music = stat_but_music + 1;
	}		
	else 
		stat_but_music = 0;
	 
	if(BUT_SPEED == 0) {
		stat_but_sp = stat_but_sp + 1;

	}		
	else 
		stat_but_sp = 0;

		   
	// pressed loop
	if( (stat_but_dd >= (TIME_SAMLE) && stat_but_music < (TIME_SAMLE) && stat_but_sp < (TIME_SAMLE)) ||
		(stat_but_music >= (TIME_SAMLE) && stat_but_dd < (TIME_SAMLE) && stat_but_sp < (TIME_SAMLE)) ||
		(stat_but_sp >= (TIME_SAMLE) && stat_but_dd < (TIME_SAMLE) && stat_but_music < (TIME_SAMLE)) ) {

		if(start_select_color == 0) {
		    Flag_IR_Convert = 0;
		    IR_Data = 0;
			start_select_color = 1;
		}

		if(Flag_IR_Convert) {
			for(i = 0; i < 16; i++) {
				if(IR_Data == key_color[i] || IR_Data == key_colorMH[i]) {
	
					tm_cnt = 1;
	
					if(stat_but_dd >= TIME_SAMLE) 
						VALUE1Ram = color[i];
					else if (stat_but_music >= TIME_SAMLE)
					   VALUE2Ram = color[i];
					else if (stat_but_sp >= TIME_SAMLE)
						VALUE3Ram = color[i];
			
					break;
	
				}
			}
			if(tm_cnt != 0) {
				start_select_color = 0;
				stat_but_dd = 0;
				stat_but_music = 0;
				stat_but_sp = 0;
			}

			if( (IR_Data == KEY_UP_MH)||(IR_Data==KEY_UP) ) {
			   if(BRIGHTNESSRam < MAX_BRIGHTNESS)
			   		BRIGHTNESSRam++;
			}
			else if( (IR_Data == KEY_DOWN_MH)||(IR_Data==KEY_DOWN) ) {
			   if(BRIGHTNESSRam > 0)
			   		BRIGHTNESSRam--;
			}

			Flag_IR_Convert = 0;
		    IR_Data = 0;

		}

		if(stat_but_dd >= (TIME_SAMLE)) {
		   color_disp = VALUE1Ram;
		   RGB_DataRam.Effect = 0;
		}
		else if (stat_but_music >= (TIME_SAMLE)) {
			color_disp = VALUE2Ram;
		 	RGB_DataRam.Effect = 0;
		}
		else if (stat_but_sp >= (TIME_SAMLE)) {
			color_disp = VALUE3Ram;
		 	RGB_DataRam.Effect = 0;
		}
	} 	
	else {
		start_select_color = 0;	  /* if press button but not press key before release the button then start_select_color not reset to 0 in pressed loop*/
		if(Flag_IR_Convert)
		{

			if( (IR_Data == KEY_UP_MH)||(IR_Data==KEY_UP) ) {
			   if(BRIGHTNESSRam < MAX_BRIGHTNESS)
			   		BRIGHTNESSRam++;
			}
			else if( (IR_Data == KEY_DOWN_MH)||(IR_Data==KEY_DOWN) ) {
			   if(BRIGHTNESSRam > 0)
			   		BRIGHTNESSRam--;
			}

		    Flag_IR_Convert = 0;
		    IR_Data = 0;
		}	 	
	}	
	if(tm_cnt != 0)
		SAVEDATA();
}


