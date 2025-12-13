#include "WriteFlashROM.h"
#include "CH554.H"
#include <intrins.h>
// ghi du lieu vao vung nho code flash : 12k doi voi CH551, 14k doi voi CH552
#define INVERT(X) ((X<<8)|(X>>8))
void WriteDoubleByteROM(UINT16 Addr, UINT16 *Data, UINT16 lenght)
{
	data UINT8 i;
	UINT16 *Pointer;
	SAFE_MOD = 0x55; 								// unlock flash-ROM
	SAFE_MOD = 0x0AA;								// unlock flash-ROM
	GLOBAL_CFG |= bCODE_WE ;				// enable write data and erase
	Pointer = Data;
	for(i = 0; i<lenght; i++)
	{
		ROM_ADDR = Addr; 							// bit thap nhat luon la 0
		ROM_DATA = INVERT(*Pointer);
		ROM_CTRL = 0x09A;
		Addr+=2;
		Pointer++;
	}
	SAFE_MOD = 0x55;
	SAFE_MOD = 0x0AA;
	GLOBAL_CFG &= (~(bCODE_WE|bDATA_WE));
}
//void df_read(uint8_t u8Address)
//{
//	
//	ROM_ADDR = 0xC000 + (u8Address << 1);
//	ROM_CTRL = 0x8E;

//	return ROM_DATA_L;
//}

/* ham ghi gia tri vao data flash */
/* u8Address: dia chi 0 - 127 : 0x00 - 0x7F */
/* u8Data: gia tri muon ghi */
/* ket qua tra ve 1 neu thanh cong va 0 neu loi */
//uint8_t df_write(uint8_t u8Address, uint8_t u8Data)
//{
//	SAFE_MOD = 0x55;
//	SAFE_MOD = 0xAA;
//	GLOBAL_CFG = 0x04;
//	ROM_ADDR = 0xC000 + (u8Address << 1);
//	ROM_DATA_L = u8Data;
//	ROM_CTRL = 0x9A;
//	if (ROM_STATUS & 0x02) {
//		SAFE_MOD = 0x55;
//		SAFE_MOD = 0xAA;
//		GLOBAL_CFG = 0x00;
//		return 0;
//	}
//	if (ROM_STATUS & 0x40 ) {
//		SAFE_MOD = 0x55;
//		SAFE_MOD = 0xAA;
//		GLOBAL_CFG = 0x00;
//		return 1;
//	}
//	SAFE_MOD = 0x55;
//	SAFE_MOD = 0xAA;
//	GLOBAL_CFG = 0x00;
//	return 0;
//}