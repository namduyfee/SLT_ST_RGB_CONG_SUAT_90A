#ifndef	__TIMER0_H__
#define __TIMER0_H__

#define KEY_0									0XFF6897	
#define KEY_1									0xFF30CF
#define KEY_2									0xFF18E7
#define KEY_3									0xFF7A85
#define KEY_4									0xFF10EF
#define KEY_5									0xFF38C7
#define KEY_6									0xFF5AA5
#define KEY_7									0xFF42BD
#define KEY_8									0xFF4AB5
#define KEY_9									0xFF52AD

void Timer0_Init(uint16_t Counter);
void Timer1_Init(uint16_t Counter);
void Timer2_InputCapture_Init();
//void Timer0_Update(void) interrupt 1
#endif