#ifndef __SAVEDATA_H
#define __SAVEDATA_H   
#include "CH554.H"

#define  BASE_OBJECT_ADDR     0x2600

void SAVEDATA(void);
void ConfigSaveData(void);
void LOADSAVEDATA(void);

#endif