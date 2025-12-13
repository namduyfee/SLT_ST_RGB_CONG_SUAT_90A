#include "savedata.h"
#include "WriteFlashROM.h"
#include "Effect.h"
xdata UINT8C* Pointer;
extern RGB_DATA RGB_DataRam;


void SAVEDATA(void)
{

	WriteDoubleByteROM(BASE_OBJECT_ADDR,(uint16_t*)&RGB_DataRam,(sizeof(RGB_DATA)+1)>>1);
}
void LOADSAVEDATA(void)
{

	RGB_DataRam = 	*((RGB_DATA code*)(BASE_OBJECT_ADDR));

}