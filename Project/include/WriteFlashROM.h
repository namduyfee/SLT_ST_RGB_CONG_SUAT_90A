#ifndef __WriteFlashROM_H
#define __WriteFlashROM_H
#include "CH554.H"
void WriteDoubleByteROM(UINT16 Addr, UINT16 *Data, UINT16 lenght);
//void ReadDataROM(UINT8 Addr, UINT8 *Data);
uint8_t df_write(uint8_t u8Address, uint8_t u8Data);
uint8_t df_read(uint8_t u8Address);

#endif
