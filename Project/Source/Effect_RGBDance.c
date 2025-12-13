#include "Effect_RGBDance.h"
#include "Effect.h"
#include "Debug.H"
extern RGB_DATA RGB_DataRam;
uint8_t xdata Step_Color=0,_FlagMusic=0,_Flag_color=0,_FlagFade=0,Step_Duty=0;
uint16_t Time_Wait=0,Time_Color=0,_Flagcount=10000;

uint8_t EFFECT_MoveColor (uint8_t Duty,uint8_t Bright) 
{
		if (++Time_Wait>=5000)
		{
			Time_Wait=0;
			if (Bright<(170/value))
				Step_Duty=0;
		}
	 if (Bright>(200/value))
	 {
		 if (++Step_Duty>=10)
		 {
			 Step_Duty=0;
			 Time_Color=0;
			 if (++Step_Color>=7)
			 {
				 Step_Color=0;
			 }
		 }
	 }
	 else
	 {
		 if (++Time_Color>=1000)
		 {
			 Time_Color=0;
			 if (++Step_Color>=7)
			 {
				 Step_Color=0;
			 }
		 }
		 mDelaymS(1);
	 }
	 Duty=Bright;
	 //if (Duty>100) Duty=100;
	 //if (Bright>10)
	 switch (Step_Color)
	 {
		 case 0:
		 {
			SetColor(RED,Duty,Bright);
			SetColor(BLUE,0,Bright);
			SetColor(GREEN,0,Bright);
		 }break;
		 case 1:
		 {
			SetColor(RED,0,Bright);
			SetColor(BLUE,0,Bright);
			SetColor(GREEN,Duty,Bright);
		 }break;
		 case 2:
		 {
			SetColor(RED,0,Bright);
			SetColor(BLUE,Duty,Bright);
			SetColor(GREEN,0,Bright);
		 }break;
		 case 3:
		 {
			SetColor(RED,Duty,Bright);
			SetColor(BLUE,Duty,Bright);
			SetColor(GREEN,0,Bright);
		 }break;
		 case 4:
		 {
			SetColor(RED,Duty,Bright);
			SetColor(BLUE,0,Bright);
			SetColor(GREEN,Duty,Bright);
		 }break;
		 case 5:
		 {
			SetColor(RED,0,Bright);
			SetColor(BLUE,Duty,Bright);
			SetColor(GREEN,Duty,Bright);
		 }break;
		 case 6:
		 {
			SetColor(RED,Duty,Bright);
			SetColor(BLUE,Duty,Bright);
			SetColor(GREEN,Duty,Bright);
		 }break;
	 }
	 /*else
	 {
		SetColor(RED,0,0);
			SetColor(BLUE,0,0);
			SetColor(GREEN,0,0);
	 }*/
	 return 0;
}

uint8_t EFFECT_MoveColor2 (uint8_t Duty,uint8_t _dBand) 
{
	if (!_FlagMusic)
		_FlagMusic=1;
	if (_dBand>=60)
 	{
    	_Flagcount=10;//(230-_dBand);
 	}
 	
	if (_dBand<=50) 
 	{
		_Flagcount+=50;
   	if (_Flagcount>=1000);
      _Flagcount=1000;
 	}

	if (++Time_Color>=_Flagcount)
	{
		Time_Color=0;
		if (++Step_Duty>=MAX_STEP)
		{
			Step_Duty=0;
			if (++Step_Color>=6)
      	Step_Color=0;
		}
	}
	switch (Step_Color)
	{
	case 0:
	{
		SetColor(RED,Step_Duty,100);
		SetColor(BLUE,MAX_STEP-Step_Duty,100);
		SetColor(GREEN,0,100);
	}break;
	case 1:
	{
		SetColor(RED,MAX_STEP-Step_Duty,100);
		SetColor(BLUE,0,100);
		SetColor(GREEN,Step_Duty,100);
	}break;
	case 2:
	{
		SetColor(RED,0,100);
		SetColor(BLUE,Step_Duty,100);
		SetColor(GREEN,MAX_STEP-Step_Duty,100);
	}break;
	case 3:
	{
		SetColor(RED,Step_Duty,100);
		SetColor(BLUE,MAX_STEP-Step_Duty,100);
		SetColor(GREEN,Step_Duty,100);
	}break;
	case 4:
	{
		SetColor(RED,MAX_STEP-Step_Duty,100);
		SetColor(BLUE,0,100);
		SetColor(GREEN,MAX_STEP,100);
	}break;
	case 5:
	{
		SetColor(RED,0,100);
		SetColor(BLUE,Step_Duty,100);
		SetColor(GREEN,MAX_STEP-Step_Duty,100);
	}break;
	}
 return 0;
}

