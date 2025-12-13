#ifndef USB_SERIAL_H_
#define USB_SERIAL_H_
#include "CH554.H"
#define LOBYTE(x)  ((UINT8)(x & 0x00FF))
#define HIBYTE(x)  ((UINT8)((x & 0xFF00) >>8))

#define USBD_VID                      0x2009
#define USBD_LANGID_STRING            1033
#define USBD_MANUFACTURER_STRING      "Solantech"
// PID 0x69 - Mach led vay, 0x0079 - Mach led vien, 0x0089 - Mach RGB
#define USBD_PID_FS                   0x0089

#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)															//cast

//#ifndef USB_DESCR_TYP_DEVICE
#define USB_DESCR_TYP_DEVICE    0x01 //
#define USB_DESCR_TYP_CONFIG    0x02 //
#define USB_DESCR_TYP_STRING    0x03 //

#define  USB_DESC_TYPE_DEVICE                              1
#define  USB_DESC_TYPE_CONFIGURATION                       2
#define  USB_DESC_TYPE_STRING                              3
#define  USB_DESC_TYPE_INTERFACE                           4
#define  USB_DESC_TYPE_ENDPOINT                            5
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    6
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           7
#define  USB_DESC_TYPE_BOS                                 0x0F

#define CDC_DATA_IN_EP                                   0x82  /* EP1 for data IN */
#define CDC_DATA_OUT_EP                                  0x02  /* EP1 for data OUT */
#define CDC_CMD_OUT_EP                                   0x01  /* EP2 for CDC commands Out */
/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CDC_DATA_HS_MAX_PACKET_SIZE                 512  /* Endpoint IN & OUT Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SIZE                         64  /* Control Endpoint Packet size */ 

#define USB_CDC_CONFIG_DESC_SIZ                     67
#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE

#define CDC_CMD_FS_IN_PACKET_SIZE                  CDC_CMD_PACKET_SIZE
#define CDC_CMD_FS_OUT_PACKET_SIZE                 CDC_CMD_PACKET_SIZE
#define USB_CDC_FS_CONFIG_DESC_SIZ                  (39)

#define STRING_LANGID_SIZE          4
#define STRING_VENDOR_SIZE          14
#define STRING_PRODUCT_SIZE         28
#define STRING_SERIAL_SIZE          18


typedef struct
{
	PUINT8 descr;
	UINT8 Size;
} DescriptorData;
#if   defined ( __CC_ARM )
#pragma pack(1)
#elif defined ( __ICCARM__ ) /* IAR Compiler */ 
//Với IAR thì mỗi struct lại phải viết lại cấu trúc này
#pragma data_alignment=1
__packed
#endif /* defined ( __ICCARM__ ) */
typedef volatile struct
{
  volatile UINT32   CommandCode;
  volatile UINT32   AddressData;
  volatile UINT32   LengthOfData;
  volatile UINT32   Options;
  volatile UINT32   NotUser[12];
}USB_COMMAND;
typedef enum
{
  COMMAND_NONE            =     0,
  COMMAND_WRITE_IAP       =     1,
  COMMAND_READ_IAP        =     2,
  COMMAND_ERASE_IAP       =     3,
  COMMAND_WRITE_HEX       =     4,
  COMMAND_READ_HEX        =     5,
  COMMAND_ERASE_HEX       =     6,
  COMMAND_READ_VERSION    =     7,
  COMMAND_READ_AP_ADDRESS =     8,
  COMMAND_RUN             =     9,
  COMMAND_ACK             =     10,
	COMMAND_ONLINE_RUN      =     11,
}COMMAND_CODE;
typedef union
{
	volatile UINT8 ACCESS_BYTE;
	struct
	{
		volatile UINT8 USB_DATA	:	1;
		volatile UINT8 USB_CMD	:	1;
		volatile UINT8 USB_RUN	:	1;
	}ACCESS_BITS;
}USB_FLAGS;
extern data USB_FLAGS Flags;
extern xdata USB_COMMAND  CommandIn;
extern xdata USB_COMMAND  DataIn;
extern xdata USB_COMMAND CommandAndDataOut;
void JumpToBootloader();
void USBDeviceCfg();
void USBDeviceIntCfg();
void USBDeviceEndPointCfg();
void EcoSendData(PUINT8 SendBuf);
void SendData();
void RecieveData();
//void	USBInterrupt();
//void u16str(UINT16 i_data, char *str_t);

#endif
