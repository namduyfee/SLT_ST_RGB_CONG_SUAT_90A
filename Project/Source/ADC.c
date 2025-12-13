#include "ADC.H"
uint8_t idata ADC_Value;
void ADC_Init(void)
{
		ADC_CFG |= bADC_EN;
		ADC_CFG |= bADC_CLK;
		ADC_CHAN1 = 0;
		ADC_CHAN0 = 0;
		ADC_IF = 0;
		IE_ADC = 1;
		ADC_START = 1;
}
void ADC_Interrupt(void) interrupt INT_NO_ADC
{
	ADC_IF = 0;
	ADC_START = 1;
	ADC_Value = ADC_DATA;
}