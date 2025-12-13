#include "CH554.H"
#include "Timer0.H"
#include "Effect.h"
//#include <REGX51.H>
sbit	LED=P1^4;
sbit 	LED_B = P3^1;
sbit 	LED_G = P1^0;
sbit 	LED_R = P1^7;

extern uint8_t idata Flag_IR_Convert, Flag_Speed_Effect;
extern uint32_t xdata IR_Data;
static uint8_t idata IndexBit;
extern RGB_PIN  RedGreenBlue[3];
static uint8_t idata CurrentDuty = 0;
uint8_t xdata u8TH0_Tmp,u8TL0_Tmp,u8TH1_Tmp,u8TL1_Tmp;
static uint8_t idata CurrentCount =0;
idata UINT8 Flag_Capture = 0;
idata UINT16 Time_Capture = 0;
uint8_t Effect_Time4;

/* timer0 config
	- Nguon clock
	 + bT0_CT = 0 -> clock noi
	 + bT0_CT = 1 -> clock ngoai vao chan P3.4
	- Cau hinh bo chia
		|		bT0_CLK	|	bTMR_CLK	|
		|			0			|						|	Ftimer = Fsys/12
		|			1			|			0			|	Ftimer = Fsys/4
		|			1			|			1			|	Ftimer = F
	- Cau hinh bo dem
		|	bT0_GATE	|			TR0		|	INT0(P3.2)	|
		|			0			|			0			|							|	Khong cho phep clock dua vao bo dem
		|			0			|			1			|							| Clock duoc dua vao bo dem, gia trij thanh ghi TH0_TL0 se duoc tang len 1 moi khi co 1 xung
																					|	Khi tran bi TF0 = 1
		|			1			|			1			|				1			| Clock duoc dua vao bo dem
		*/
void Timer0_Init(uint16_t Counter)
{
    T2MOD &= ~bT0_CLK;
    TMOD &= ~(bT0_CT | bT0_GATE | bT0_M1); //timer mode
    TMOD |= bT0_M0;
		u8TH0_Tmp = (65536-Counter)>>8;
		u8TL0_Tmp = (65536-Counter)&0xff;  
	  TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;	
		ET0 = 1;
//		TR0 = 1;
}
void Timer1_Init(uint16_t Counter)
{
	T2MOD &= ~bT1_CLK;
	TMOD &= ~(bT1_CT | bT1_GATE | bT1_M1);
	TMOD |= bT1_M0;
	u8TH1_Tmp = (65536-Counter)>>8;
	u8TL1_Tmp = (65536-Counter)&0xff;  
	TH1 = u8TH1_Tmp;
  TL1 = u8TL1_Tmp; 
  ET1 = 1; 														//enable Timer1 interrupt_	
	TR1 = 1; 														//Timer1 run	
}
//void Timer2_Init()
//{
//    T2MOD |= (bTMR_CLK | bT2_CLK); //clock = Fsys
//		RCAP2H = 0xA2;
//		RCAP2L = 0x40;
//		TH2 = 0xA2;
//		TL2 = 0x40;
//		TR2 = 1;
//		ET2 = 1;
//}
void Timer2_InputCapture_Init()
{
	RCLK = 0;
	TCLK = 0; 								// Set the bits RCLK and TCLK in T2CON to 0, and select the non-serial port baud rate generator mode.
	C_T2 = 0;
	T2MOD &= ~bT2_CLK;
//	T2MOD &= ~bT2_CAP_M1;			// 01: from any edge to any edge (level changing) / x0 suon xuong, 01 ca 2 suon, 11 suon len 
	T2MOD |= bT2_CAP_M1;
	T2MOD |= bT2_CAP_M0; 				// Capture suon len
	EXEN2 = 1;
	CP_RL2 = 1;
	TL2 = 0;
	TH2 = 0;
	PIN_FUNC |= bT2EX_PIN_X; // ánh xa chân bT2EX_ dê su dung chan RST
	ET2 = 1;
//	TR2 = 1;
}
void Timer0_Update(void) interrupt INT_NO_TMR0 
{
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;  
    if(++CurrentDuty>MAX_STEP)
			CurrentDuty = 0;	
		GPIO_RED = (RedGreenBlue[0].Duty > CurrentDuty);
		GPIO_GREEN = (RedGreenBlue[1].Duty > CurrentDuty);
		GPIO_BLUE = (RedGreenBlue[2].Duty > CurrentDuty);
		Effect_Time4++;
}
void Timer1_Update(void) interrupt INT_NO_TMR1 using 3
{
	TH1 = u8TH1_Tmp;
	TL1 = u8TL1_Tmp;
	if(++CurrentCount>= 5)
	{
    Flag_Speed_Effect = 1;
		CurrentCount =0;
	}
}
void Timer2_Capture(void) interrupt INT_NO_TMR2 using 2
{
	if((EXF2==1)&&(Flag_Capture==0)) // capture suon len
	{
		EXF2 = 0;
		TR2 = 0;
		T2MOD &= ~bT2_CAP_M0;								// set capture suon xuong
		Flag_Capture =1;
		TL2 = 0;
		TH2 = 0;
		TR2 = 1;
	}
	else if(EXF2 & Flag_Capture) 					// capture suon xuong
	{
		EXF2 = 0;
		T2MOD |= (bT2_CAP_M1|bT2_CAP_M0);				// set capture suon len
		Flag_Capture =0;
		Time_Capture = RCAP2H;
		Time_Capture = (Time_Capture << 8) | RCAP2L;
//		if(Time_Capture < 2000)
//		{
//			IR_Data<<=1;
//			if(Time_Capture > 1000)
//					IR_Data|=1;
//			if(++IndexBit>=32)
//					Flag_IR_Convert=1;
//		}
		if(Time_Capture < 4000)
		{
			IR_Data<<=1;
			if(Time_Capture > 2000)
					IR_Data|=1;
			if(++IndexBit>=32)
					Flag_IR_Convert=1;
		}
		else
		{	
			IR_Data  = 0;
			IndexBit = 0;
		}
		TR2 = 0;
	}
	if(TF2)
		{
			TF2 = 0;
		}
}