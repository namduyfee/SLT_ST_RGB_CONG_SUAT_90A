#include "Effect.h"
#include "savedata.h"
#include "stdlib.h"
#include "Effect_RGBDance.h"
#include "Debug.H"
#include <stdio.h>
RGB_PIN  RedGreenBlue[3];
extern uint8_t idata ADC_Value;
uint8_t Delay_Music_MONO=10;
extern RGB_DATA RGB_DataRam;
const uint32_t xdata ColorTable[7] = {RGB_RED, RGB_GREEN, RGB_BLUE, RGB_YELLOW, RGB_CYAN, RGB_PURPLE, RGB_WHITE};
extern uint32_t xdata i,j;
extern uint8_t idata Run_Effect_Done;
static  uint8_t xdata k=0, m=0, Flag_Button =0,Flag_Music=0,__Flag_mode=0;
static uint16_t xdata  t=0;
static uint8_t xdata _dBLast = 0;
extern uint8_t xdata _FlagMusic,_FlagFade;
static const uint8_t xdata chu_so[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
extern uint8_t Effect_Time4;
extern uint8_t xdata u8TH1_Tmp,u8TL1_Tmp;
uint8_t idata But_Count = 0;
static const uint8_t xdata Prog_Auto[4][7] =
{  //Chuong trinh hieu ung cho cac che do auto khac nhau
       1, 2, 3, 4, 5, 6,7,    //Auto 1
       3, 5, 3, 5, 3, 5,3,    //Auto 2
       6, 3, 5, 6, 3, 5,6,    //Auto 3
       7, 5, 6, 7, 5, 6,7,    //Auto 4
};
extern uint8_t xdata DataSave[11];
void RGB_PinInit(void)
{
    RedGreenBlue[0].Duty      = 0;
    RedGreenBlue[1].Duty      = 0;
    RedGreenBlue[2].Duty      = 0;
}
uint8_t Audio_Convert(uint8_t ADC_Value_In)
{
    uint8_t _dBData;
    _dBData = ((ADC_Value_In/value)*2);
//		if(_dBData >254)
//			_dBData = 255;
    if(_dBData > _dBLast)
		{
			 Effect_Time4 = 0;
			 _dBLast = _dBData;
    }
		else
		{
			 if(Effect_Time4>Delay_Music_MONO)
				{
					Effect_Time4 = 0;
					if(_dBLast > _dBData)_dBLast--;  //Lam muot khi bien do giam
				}
    }
    return _dBLast;
}
 uint8_t LastMode = 0xff;
void RGB_SetModeSpeed(uint8_t Mode)
{
	if(LastMode!=Mode)
	{
		TR1 = 0;
		if(Mode == 1)
			T2MOD |= (bTMR_CLK | bT1_CLK);
		else if(Mode == 4)
		{
			T2MOD |= bT1_CLK;
			T2MOD &= ~bTMR_CLK;
		}
		else
			T2MOD &= ~bT1_CLK;
		TR1 = 1;
		LastMode = Mode;
	}
//    if(Mode < 8)
//    {
//        T3CON &= ~0x07; T3CON |= Mode&0x07;   //Bo chia tan
//    }
}
void RGB_SetSpeed(uint8_t Speed)
{
    uint16_t Temp;
    if(Speed < MIN_SPEED || Speed > MAX_SPEED)return;
    TR1 = 0;
    Temp = VALUE_TEMP*((MAX_SPEED+1)-Speed);
		u8TH1_Tmp = (65536-Temp)/256;
		u8TL1_Tmp = (65536-Temp)%256;  
		TH1 = u8TH1_Tmp;
		TL1 = u8TL1_Tmp;  
    ET1 = 1;           //Cau hinh ngat
    TR1 = 1;
}
void RGB_SetBright(uint8_t Brightness)
{
    RedGreenBlue[RED].Duty = (uint16_t)(RGB_DataRam.Color.DutyRed * Brightness) / MAX_BRIGHTNESS;
    RedGreenBlue[GREEN].Duty = (uint16_t)(RGB_DataRam.Color.DutyGreen * Brightness) / MAX_BRIGHTNESS;
    RedGreenBlue[BLUE].Duty = (uint16_t)(RGB_DataRam.Color.DutyBlue * Brightness) / MAX_BRIGHTNESS;

}
void RGB_SetColor(uint32_t Color, uint8_t Brightness)
{
    RGB_DataRam.Color.DutyRed = (uint8_t)(Color>>16);
    RGB_DataRam.Color.DutyGreen = (uint8_t)(Color>>8);
    RGB_DataRam.Color.DutyBlue = (uint8_t)(Color);
    RGB_SetBright(Brightness);
}
void SetColor(uint8_t color, uint8_t duty, uint8_t bright)
{
    if(color == RED)RGB_DataRam.Color.DutyRed = duty;
    else if(color == GREEN)RGB_DataRam.Color.DutyGreen = duty;
    else if(color == BLUE)RGB_DataRam.Color.DutyBlue = duty;
    else return;
		if(!_FlagMusic)
			RedGreenBlue[color].Duty = (uint16_t)(duty * bright) / MAX_BRIGHTNESS;
		else
			RedGreenBlue[color].Duty = duty;//(uint16_t)(duty * bright) / MAX_BRIGHTNESS;
}
void LED7_OutData(uint8_t Data)
{
  LED7_DATA_A  =  ((Data&0x01)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
  LED7_DATA_B  =  ((Data&0x02)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
  LED7_DATA_C  =  ((Data&0x04)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
  LED7_DATA_D  =  ((Data&0x08)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
  LED7_DATA_E  =  ((Data&0x10)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
  LED7_DATA_F  =  ((Data&0x20)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
  LED7_DATA_G  =  ((Data&0x40)!=0)?LED7_DATA_ON:LED7_DATA_OFF;
}
void Display_LED_7(uint8_t Num)
{
    if(Num > 9)return;
    LED7_OutData(chu_so[Num]);
}
void RGB_OnDisplay(void)
{
    TR0 = 1;
}
void RGB_OffDisplay(void)
{
    TR0 = 0;
		GPIO_RED_OFF();
		GPIO_GREEN_OFF();
		GPIO_BLUE_OFF();
}
void SetProcessAutoEffect(uint8_t AutoEffect)    //Save  o day
{
     RGB_DataRam.LastEffect = 2;
     RGB_DataRam.AutoEffect = AutoEffect;
     RGB_DataRam.Effect = Prog_Auto[AutoEffect-1][0];
//     RGB_DataRom.AutoEffect = RGB_DataRam.AutoEffect;
//     RGB_DataRom.Effect = RGB_DataRam.Effect;
		 //IAP_AP_Erase_Pages(17920,1);
		 SAVEDATA();
     i = j = m = 0;
     RGB_SetColor(RGB_BLACK, RGB_DataRam.Brightness);
}
FLAG_KEY KEY_GetKey (void)
{
	uint8_t i=0;
	if (RGB_DataRam.Flags)
	{
		if ((BUT_DON_DA==0)&&(BUT_SPEED!=0)&&(BUT_MUSIC!=0))
		{
			mDelaymS(40);
			if ((BUT_DON_DA==0)&&(BUT_SPEED!=0)&&(BUT_MUSIC!=0))
				return __KEY_FLASH;
//			{
//				for (i=0;i<50;i++)
//				{
//				 if (!((BUT_DON_DA==0)&&(BUT_SPEED!=0)&&(BUT_MUSIC!=0)))
//					 break;
//				 mDelaymS(10);
//				}
//			}
//			if (i>=50) return __KEY_FLASH_LONG;
//		else return __KEY_FLASH;
		}

		else if ((BUT_DON_DA!=0)&&(BUT_SPEED==0)&&(BUT_MUSIC!=0))
		{
			mDelaymS(40);
			if ((BUT_DON_DA!=0)&&(BUT_SPEED==0)&&(BUT_MUSIC!=0))
				return __KEY_SPEED;
//			{
//				for (i=0;i<50;i++)
//				{
//				 if (!((BUT_DON_DA!=0)&&(BUT_SPEED==0)&&(BUT_MUSIC!=0)))
//					 break;
//				 mDelaymS(10);
//				}
//			}
//			if (i>=50) return __NO_KEY;
//			else return __KEY_SPEED;
		}
		else if ((BUT_DON_DA!=0)&&(BUT_SPEED!=0)&&(BUT_MUSIC==0))
		{
			mDelaymS(40);
			if ((BUT_DON_DA!=0)&&(BUT_SPEED!=0)&&(BUT_MUSIC==0))
				return __KEY_MUSIC;
//			{
//				for (i=0;i<50;i++)
//				{
//				 if (!((BUT_DON_DA!=0)&&(BUT_SPEED!=0)&&(BUT_MUSIC==0)))
//					 break;
//				 mDelaymS(10);
//				}
//			}
//			if (i>=50) return __NO_KEY;
//			else return __KEY_MUSIC;
		}
	}
	return __NO_KEY;
}
void Check_Button(uint8_t Bright) 									// han nay chua xu ly
{
	uint8_t Key=KEY_GetKey();
	switch (Key)
	{
    case __KEY_FLASH:
    {
			But_Count++;
			if(But_Count >= 7) //DA
			{
				if (_FlagMusic) _FlagMusic=0;
      	if(RGB_DataRam.AutoEffect==6)
       	 RGB_OnDisplay();
      	if(RGB_DataRam.LastEffect == 2)
      	  RGB_DataRam.Effect = RGB_DataRam.SaveEffect;
     	 	else
      	{
					if(++RGB_DataRam.Effect > 7)
					{
						But_Count = 0;
						RGB_DataRam.Effect = 1;
					}
						RGB_DataRam.SaveEffect = RGB_DataRam.Effect;
      	}
      	RGB_DataRam.LastEffect = 1;
      	RGB_DataRam.AutoEffect = 0;
      	i = j = m = 0;
      	RGB_SetColor(RGB_BLACK, RGB_DataRam.Brightness);
				SAVEDATA();
			}
			if(But_Count < 7) //DON
			{
				if (RGB_DataRam.Effect == 10)
				{
					Delay_Music_MONO=20;
					RGB_DataRam.Effect = 0;
					//if (_FlagFade==0)_FlagFade=1;
					if(RGB_DataRam.AutoEffect==6)
         		RGB_OnDisplay();
       	  if(++k>6)k=0;
       			RGB_SetColor(ColorTable[k],Bright);
					if(_FlagFade == 1) 
						RGB_DataRam.AutoEffect=8;
					else
						RGB_DataRam.AutoEffect=0;
				}
				else
				{
					RGB_DataRam.Effect = 0;
					if(RGB_DataRam.AutoEffect==6)
         		RGB_OnDisplay();
       	  if(++k>6)k=0;
       			RGB_SetColor(ColorTable[k],RGB_DataRam.Brightness);
					if(_FlagFade == 1) 
						RGB_DataRam.AutoEffect=8;
					else
						RGB_DataRam.AutoEffect=0;
				}
				SAVEDATA();
			}
			break;
		}
//    case __KEY_MONO:
//    {
//				if (RGB_DataRam.Effect == 10)
//				{
//					Delay_Music_MONO=20;
//					RGB_DataRam.Effect = 0;
//					//if (_FlagFade==0)_FlagFade=1;
//					if(RGB_DataRam.AutoEffect==6)
//         		RGB_OnDisplay();
//       	  if(++k>6)k=0;
//       			RGB_SetColor(ColorTable[k],Bright);
//					if(_FlagFade == 1) 
//						RGB_DataRam.AutoEffect=8;
//					else
//						RGB_DataRam.AutoEffect=0;
//				}
//				else
//				{
//					RGB_DataRam.Effect = 0;
//					if(RGB_DataRam.AutoEffect==6)
//         		RGB_OnDisplay();
//       	  if(++k>6)k=0;
//       			RGB_SetColor(ColorTable[k],RGB_DataRam.Brightness);
//					if(_FlagFade == 1) 
//						RGB_DataRam.AutoEffect=8;
//					else
//						RGB_DataRam.AutoEffect=0;
//				}
//				SAVEDATA();
//				break;
//    }
		case __KEY_MONO_LONG:
    {
			
				_FlagFade = !_FlagFade;
				if(_FlagFade == 1) 
					RGB_DataRam.AutoEffect=8;
				else
					RGB_DataRam.AutoEffect=0;			
				RGB_SetColor(RGB_RED,RGB_DataRam.Brightness);
				SAVEDATA();
			break;
		}
    case __KEY_MUSIC:
    {
			if (++__Flag_mode>=2)
				__Flag_mode=0;
			if (__Flag_mode==1)
			{

        if (_FlagMusic) _FlagMusic=0;
        	RGB_DataRam.LastEffect = 2;
        	RGB_DataRam.AutoEffect = 6;
        	RGB_DataRam.Effect = 10;
        	i = j = m = 0;
       	 RGB_SetColor(RGB_BLACK, RGB_DataRam.Brightness); 
			}
			else
			{
					if (_FlagMusic) _FlagMusic=0;
        	RGB_DataRam.LastEffect = 2;
        	RGB_DataRam.AutoEffect = 7;
        	RGB_DataRam.Effect = 10;
        	i = j = m = 0;
       	 RGB_SetColor(RGB_BLACK, RGB_DataRam.Brightness); 
			}
			SAVEDATA();
			break;
    }
    case __KEY_SPEED:
    {
       if (RGB_DataRam.Effect == 0)
       {
         if(++RGB_DataRam.Brightness > MAX_BRIGHTNESS)
         {
             RGB_DataRam.Brightness = MIN_BRIGHTNESS;
         }
         if(!RGB_DataRam.Effect)
            RGB_SetBright(RGB_DataRam.Brightness);
       }
       else
       {
				 RGB_DataRam.Speed += 2;
        if(RGB_DataRam.Speed > MAX_SPEED)
        {
             RGB_DataRam.Speed = MIN_SPEED;
        }
         RGB_SetSpeed(RGB_DataRam.Speed);
       }
			SAVEDATA();
			break;
    }
	}
	while ((Key=KEY_GetKey())!=__NO_KEY);
}

void RGB_ProcessKey(uint32_t Key, uint8_t Bright)
{
  if((Key==KEY_RED_MH)||(Key==KEY_RED))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_RED, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_BLUE_MH)||(Key==KEY_BLUE))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_BLUE, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_GREEN_MH)||(Key==KEY_GREEN))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_GREEN, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_WHITE_MH)||(Key==KEY_WHITE))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_WHITE, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_ORANGE_MH)||(Key==KEY_ORANGE))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_ORANGE, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_YELLOW_MH)||(Key==KEY_YELLOW))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_YELLOW, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_CYAN_MH)||(Key==KEY_CYAN))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_CYAN, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_PURPLE_MH)||(Key==KEY_PURPLE))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_PURPLE, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_GREEN2_MH)||(Key==KEY_GREEN2))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_GREEN2, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_BLUE2_MH)||(Key==KEY_BLUE2))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_BLUE2, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_ORANGE2_MH)||(Key==KEY_ORANGE2))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_ORANGE2, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_ORANGE3_MH)||(Key==KEY_ORANGE3))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_ORANGE3, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_PURPLE2_MH)||(Key==KEY_PURPLE2))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_PURPLE2, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_PURPLE3_MH)||(Key==KEY_PURPLE3))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_PURPLE3, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_BLUE3_MH)||(Key==KEY_BLUE3))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_BLUE3, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    else if((Key==KEY_BLUE4_MH)||(Key==KEY_BLUE4))
    {
       RGB_DataRam.Effect = 0;
       RGB_SetColor(RGB_BLUE4, Bright);
			if (_FlagMusic) _FlagMusic=0;
    }
    /*else if(Key==KEY_AUTO_MH)
    {
        RGB_DataRam.AutoEffect = 5; //auto 5 Random
        j = m = 0;
        i = (rand()%7)+1;
        while(i == RGB_DataRam.Effect)
        { //Chon gia tri Random khac gia tri cu
            i = (rand()%7)+1;
        }
        RGB_DataRam.Effect = i;
        i=0;
        RGB_DataRom.LastEffect = RGB_DataRam.LastEffect = 2;
        RGB_DataRom.AutoEffect = RGB_DataRam.AutoEffect;
        RGB_DataRom.Effect = RGB_DataRam.Effect;
        RGB_SetColor(RGB_BLACK, Bright);
    }*/
    else if((Key==KEY_FLASH_MH)||(Key==KEY_FLASH))
    {
        if(RGB_DataRam.LastEffect == 2)
        {
             RGB_DataRam.Effect = RGB_DataRam.SaveEffect;
        }
        else
        {
            if(++RGB_DataRam.Effect > 7)RGB_DataRam.Effect = 1;
            RGB_DataRam.SaveEffect = RGB_DataRam.Effect;
        }
					RGB_DataRam.LastEffect = 1;
					RGB_DataRam.AutoEffect = 0;
					i = j = m = 0;
					RGB_SetColor(RGB_BLACK, Bright);
					SAVEDATA();
    }
    else if((Key==KEY_STROBE_MH)||(Key==KEY_STROBE))
    {
        SetProcessAutoEffect(1);  //auto 1
    }
 /*   else if((Key==KEY_JUMP7)||(Key==KEY_JUMP7_MH))
    {
        SetProcessAutoEffect(2);  //auto 2
    }
    else if((Key==KEY_FADE3)||(Key==KEY_FADE3_MH))
    {
        SetProcessAutoEffect(3);  //auto 3
    }
    else if((Key==KEY_FADE7)||(Key==KEY_FADE7_MH))
    {
        SetProcessAutoEffect(4);  //auto 4
    }*/
    
  else if((Key==KEY_UP_MH)||(Key==KEY_UP))  //UP Bright)
    {
  		if (RGB_DataRam.Effect == 0)
			{
        if(RGB_DataRam.Brightness < MAX_BRIGHTNESS)
        {
             RGB_DataRam.Brightness++;
             if(!RGB_DataRam.Effect)
                RGB_SetBright(RGB_DataRam.Brightness);
        }
			}
			else
			{
				 if(RGB_DataRam.Speed < MAX_SPEED)
        {
             RGB_DataRam.Speed++;
             RGB_SetSpeed(RGB_DataRam.Speed);
        }
			}
			SAVEDATA();
				
    }
    else if((Key==KEY_DOWN_MH)||(Key==KEY_DOWN))  //DOWN Bright
    {
			if (RGB_DataRam.Effect == 0)
			{
        if(RGB_DataRam.Brightness > MIN_BRIGHTNESS)
        {
             RGB_DataRam.Brightness--;
             if(!RGB_DataRam.Effect)
                RGB_SetBright(RGB_DataRam.Brightness);
        }
			}
			else
			{
				if(RGB_DataRam.Speed > MIN_SPEED)
        {
             RGB_DataRam.Speed--;
             RGB_SetSpeed(RGB_DataRam.Speed);
        }
			}
			SAVEDATA();
				
    }
    else if((Key == KEY_SPEED_U)||(Key == KEY_SPEED_U_MH))
    {
				if (RGB_DataRam.Effect!=0)
					{
					 RGB_DataRam.Effect = 0;
					 RGB_SetColor(RGB_RED, Bright);
					 if (_FlagMusic) _FlagMusic=0;
					 if (_FlagFade==0)_FlagFade=1;
					 if(_FlagFade == 1) 
						 RGB_DataRam.AutoEffect=8;
					 else
						 RGB_DataRam.AutoEffect=0;
					}
					else
					{
					 if (++_FlagFade>=2)
						 _FlagFade=0;
					 if(_FlagFade == 1) 
						 RGB_DataRam.AutoEffect=8;
					 else
						 RGB_DataRam.AutoEffect=0;
					 if (_FlagFade==0)
						RGB_SetBright(RGB_DataRam.Brightness);
					}
      SAVEDATA();
    }
    else if((Key == KEY_SPEED_D)||(Key == KEY_SPEED_D_MH))
    {
			if (++Flag_Music>=2)
					Flag_Music=0;
				if (Flag_Music==0)
        	RGB_DataRam.AutoEffect = 7;
				if (Flag_Music==1) 
				{
					if (_FlagMusic)
					_FlagMusic=0;
					RGB_DataRam.AutoEffect = 6;
				}
				RGB_DataRam.LastEffect = 2;
        RGB_DataRam.Effect = 10;
        i = j = m = 0;
        RGB_SetColor(RGB_BLACK, RGB_DataRam.Brightness);
				SAVEDATA();
    }
}
void Program_AutoEffect(uint8_t Auto_Effect)
{
    if(Auto_Effect < 5 && Auto_Effect > 0)
    {
        if(Run_Effect_Done == 1)
        {
            Run_Effect_Done = 0; //Clear Flag
            RGB_DataRam.Effect = Prog_Auto[Auto_Effect-1][m]; //Chuyen chuong trinh
            //RGB_DataRom.Effect = RGB_DataRam.Effect;
            if(++m > 6)m=0;
        }
    }
    else if(Auto_Effect == 5)    //Auto Random
    {
        if(Run_Effect_Done == 1)
        {
            Run_Effect_Done = 0; //Clear Flag
            i = j = m = k = 0; //Reset all count
            RGB_DataRam.Effect = rand()%7+1; 
            //RGB_DataRom.Effect = RGB_DataRam.Effect;
        }
    }
}
void Program_Effect(uint8_t Prog_Effect, uint8_t Bright)
{
	if(Prog_Effect==1) //Program Effect 1
	{  // Doi mau luan chuyen 3 mau
//            RGB_SetModeSpeed(7);
		RGB_SetModeSpeed(12);
			if(++j>2)
            {  //i = Lap hieu ung
                j=0;
                if(++i>7)
								{ 
									i=0;
									Run_Effect_Done = 1; 
								} //Done Effect
            }
			if(j==0)
            {
				RGB_SetColor(RGB_RED, Bright);
            }
			else if(j==1)
            {
				RGB_SetColor(RGB_GREEN, Bright);
            }
			else if(j==2)
            {
				RGB_SetColor(RGB_BLUE, Bright);
            }
	}//end E1
	else if(Prog_Effect==2) //Program Effect 2
	{  // Toi dan luan chuyen 3 mau
            //RGB_SetModeSpeed(0x02);
//			RGB_SetModeSpeed(0);
		RGB_SetModeSpeed(1);
			if(++j>MAX_STEP)
			{
				j=0;
				if(++i>2){ i=0; if(++k>4){k=0; Run_Effect_Done = 1;} } //Done Effect
	  	}
			if(i==0)
			{
                SetColor(RED, MAX_STEP-j, Bright);
			}
			else if(i==1)
			{
                SetColor(GREEN, MAX_STEP-j, Bright);
			}
			else if(i==2)
			{
                SetColor(BLUE, MAX_STEP-j, Bright);
			}
	}//end E2
	else if(Prog_Effect==3) //Program Effect 3
	{  // Doi mau luan chuyen 7 mau
           // RGB_SetModeSpeed(0x09);
//			RGB_SetModeSpeed(7);
		RGB_SetModeSpeed(12);
			if(++i>6)
            {
                i=0;
                if(++j>14)
                {
                    j=0;
                    Run_Effect_Done = 1;  //Done Effect
                }
            }
            RGB_SetColor(ColorTable[i], Bright);
	}//end E3
	else if(Prog_Effect==4) //Program Effect 4
	{  // Toi dan luan chuyen 7 mau
            //RGB_SetModeSpeed(0x02);
//			RGB_SetModeSpeed(0);
		RGB_SetModeSpeed(1);
	  		if(++j>MAX_STEP)
			{
				j=0;
				if(++i>6){ i=0; Run_Effect_Done = 1; } //Done Effect
	  		}
			if(i==0)
			{
                SetColor(GREEN, MAX_STEP-j, Bright);
			}
			else if(i==1)
			{
								SetColor(RED, MAX_STEP-j, Bright);
               
			}
			else if(i==2)
			{
                SetColor(BLUE, MAX_STEP-j, Bright);
			}
			else if(i==3)
			{
                SetColor(RED, MAX_STEP-j, Bright);
                SetColor(GREEN, (MAX_STEP-j)/2, Bright);
			}
			else if(i==4)
			{
                SetColor(BLUE, MAX_STEP-j, Bright);
                SetColor(GREEN, MAX_STEP-j, Bright);
			}
			else if(i==5)
			{
                SetColor(RED, MAX_STEP-j, Bright);
                SetColor(BLUE, (MAX_STEP-j)/2, Bright);
			}
			else if(i==6)
			{
                SetColor(RED, MAX_STEP-j, Bright);
                SetColor(GREEN, MAX_STEP-j, Bright);
                SetColor(BLUE, MAX_STEP-j, Bright);
			}
	} //end E4
	else if(Prog_Effect==5) //Program Effect 5
	{  // Sang Toi dan luan chuyen 7 mau
            //RGB_SetModeSpeed(0x02);
//					RGB_SetModeSpeed(0);
		RGB_SetModeSpeed(1);
	  		if(++j>MAX_STEP)
			{
				j=0;
				if(++i>13){ i=0; Run_Effect_Done = 1; } //Done Effect
	  		}
			if(i==0)
				SetColor(BLUE, MAX_STEP-j, Bright);
			else if(i==1)
				SetColor(RED, MAX_STEP-j, Bright);
			else if(i==2)
				SetColor(GREEN, j, Bright);
			else if(i==3)
				SetColor(GREEN, MAX_STEP-j, Bright);
			else if(i==4)
				SetColor(BLUE, j, Bright);
			else if(i==5)
				SetColor(RED, j, Bright);
			else if(i==6)
			{
                SetColor(RED, j, Bright);
                SetColor(GREEN, j/2, Bright);
			}
			else if(i==7)
			{
                SetColor(RED, MAX_STEP-j, Bright);
                SetColor(GREEN, (MAX_STEP-j)/2, Bright);
			}
			else if(i==8)
			{
			    SetColor(BLUE, j, Bright);
                SetColor(GREEN, j, Bright);
			}
			else if(i==9)
			{
                SetColor(BLUE, MAX_STEP-j, Bright);
                SetColor(GREEN, MAX_STEP-j, Bright);
			}
			else if(i==10)
			{
                SetColor(BLUE, j/2, Bright);
                SetColor(RED, j, Bright);
			}
			else if(i==11)
			{
                SetColor(BLUE, (MAX_STEP-j)/2, Bright);
                SetColor(RED, MAX_STEP-j, Bright);
			}
			else if(i==12)
			{
			    SetColor(RED, j, Bright);
                SetColor(GREEN, j, Bright);
                SetColor(BLUE, j, Bright);
			}
			else if(i==13)
			{
			  	SetColor(RED, MAX_STEP-j, Bright);
                SetColor(GREEN, MAX_STEP-j, Bright);
                SetColor(BLUE, MAX_STEP-j, Bright);
			}
	}//end E5
	else if(Prog_Effect==6) //Program Effect 6
    {    //Loan 7 mau
        //RGB_SetModeSpeed(0x03);
				RGB_SetModeSpeed(12);
	  	if(++j>MAX_STEP)
		{
			j=0;
			if(++i>6){ i=0; Run_Effect_Done = 1; } //Done Effect
	  	}
		if(i==0)
		{
		
            RedGreenBlue[RED].Duty =  MAX_STEP;
            RedGreenBlue[GREEN].Duty = j;
            RedGreenBlue[BLUE].Duty = 0;
		}
		else if(i==1)
		{
			RedGreenBlue[RED].Duty =  MAX_STEP-j;
            RedGreenBlue[GREEN].Duty = MAX_STEP;
            RedGreenBlue[BLUE].Duty = 0;
		}
		else if(i==2)
		{
			RedGreenBlue[RED].Duty =  0;
            RedGreenBlue[GREEN].Duty = MAX_STEP;
            RedGreenBlue[BLUE].Duty = j;
		}
        else if(i==3)
		{
			RedGreenBlue[RED].Duty =  0;
            RedGreenBlue[GREEN].Duty = MAX_STEP-j;
            RedGreenBlue[BLUE].Duty = MAX_STEP;
		}
		else if(i==4)
		{
            RedGreenBlue[RED].Duty =  j;
            RedGreenBlue[GREEN].Duty = 0;
            RedGreenBlue[BLUE].Duty = MAX_STEP;
		}
        else if(i==5)
		{
			RedGreenBlue[RED].Duty =  MAX_STEP;
            RedGreenBlue[GREEN].Duty = 0;
            RedGreenBlue[BLUE].Duty = MAX_STEP-j;
		}
	}//end E6
    else if(Prog_Effect==7) //Program Effect 7
	{  // Bat tat 6 lan tung mau trong 7 mau
//        RGB_SetModeSpeed(0x05);
		RGB_SetModeSpeed(12);
        if(++k>1)
        {   //k = on/off
            k=0;
            if(++j>5)
            {  //j = lan chop
                j=0;
                if(++i>6){ i=0; Run_Effect_Done = 1; } //i = sang so mau, Done Effect
            }
        }
        if(k == 1)RGB_SetColor(ColorTable[i], Bright); //ON COLOR
        else RGB_SetColor(RGB_BLACK, Bright); //OFF COLOR
    }//end E7
}