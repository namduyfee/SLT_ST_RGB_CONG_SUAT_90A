#include "savedata.h"
#include "WriteFlashROM.h"
#include "Effect.h"

extern const UINT32 code VALUE1, VALUE2, VALUE3;
extern const UINT16 code BRTNESS;

extern UINT32 VALUE1Ram,VALUE2Ram,VALUE3Ram;
extern UINT16 BRIGHTNESSRam;

void SAVEDATA(void)
{
	WriteDoubleByteROM(&VALUE1,(uint16_t*)&VALUE1Ram,2);
	WriteDoubleByteROM(&VALUE2,(uint16_t*)&VALUE2Ram,2);
	WriteDoubleByteROM(&VALUE3,(uint16_t*)&VALUE3Ram,2);
	WriteDoubleByteROM(&BRTNESS,(uint16_t*)&BRIGHTNESSRam,1);
}
void LOADSAVEDATA(void)
{
	VALUE1Ram = VALUE1;
	VALUE2Ram = VALUE2;
	VALUE3Ram = VALUE3;
	BRIGHTNESSRam = BRTNESS; 
}