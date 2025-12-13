#include "CH554.H"
#include "Debug.H"
#include <string.h>
#include "USB_Serial.h"
#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE
#define BOOT_ADDR								0x3800

UINT8X	Ep0Buffer[THIS_ENDP0_SIZE] _at_ 0x0000;
xdata	USB_COMMAND CommandIn _at_ (sizeof(USB_COMMAND));
xdata	USB_COMMAND DataIn _at_ (2*sizeof(USB_COMMAND));
xdata	USB_COMMAND CommandAndDataOut _at_ (3*sizeof(USB_COMMAND));

data UINT8	SetupReq,SetupLen;
//data UINT8  UsbConfig;
//data UINT8	SetReqtp
PUINT8C  pDescr;	 
data USB_SETUP_REQ	           SetupReqBuf;  

void (* data bootloader)(void) = BOOT_ADDR;
void JumpToBootloader()
{
	USB_INT_EN = 0;
	USB_CTRL = 0x06;
	EA = 0;
	bootloader();
	while(1)
	{
		CH554WDTFeed(0x00);
	}
}

UINT8C DevDesc[18] =
{
    0x12,                       /*bLength */
    0x01,								       /*bDescriptorType*/
    0x00,                       /* bcdUSB */  
    0x02,
    0x00,                        /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x08,          							/*bMaxPacketSize*/
    LOBYTE(USBD_VID),           /*idVendor*/
    HIBYTE(USBD_VID),           /*idVendor*/
    LOBYTE(USBD_PID_FS),           /*idVendor*/
    HIBYTE(USBD_PID_FS),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    0x01,           						/*Index of manufacturer  string*/
    0x02,       								/*Index of product string*/
    0x03,     								   /*Index of serial number string*/
    0x01  											/*bNumConfigurations*/
};
 UINT8C CfgDesc[USB_CDC_FS_CONFIG_DESC_SIZ]=
{
	  /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CONFIGURATION DESCRIPTOR - CONFIGURATION DESCRIPTOR - CONFIGURATION DESCRIPTOR
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/******************************************************************************
bLength :  K thu?c c?a Descriptor.
******************************************************************************/
    0x09,                        // K thu?c c?a Configuration Descriptor l࠹ Byte
/******************************************************************************
bDescriptorType :  Lo?i Descriptor.
******************************************************************************/
    USB_DESC_TYPE_CONFIGURATION,  // Lo?i Descriptor l࠺ Configuration Descriptor
/******************************************************************************
wTotalLength :  T?ng s? byte c󠴲ong Descriptor vࡴ?t c? c⣠Descriptor kh⣍
                b뮠trong n󮍊******************************************************************************/
    USB_CDC_FS_CONFIG_DESC_SIZ,
    0x00,
/******************************************************************************
bNumInterfaces :  S? interface c?a c?u h􊀮
******************************************************************************/
    0x01,     //5
/******************************************************************************
bConfigurationValue :  Ch? th? giᡴr? c?u h􊀮 L࠱ ho?c l?n hon.
******************************************************************************/
    0x01,
/******************************************************************************
iConfiguration :  Ch? s? c?a chu?i Descriptor mi뵠t? c?u h􊀮
******************************************************************************/
    0x00,
/******************************************************************************
bmAttributes   :  C᩠d?t c⣠giᡴr? T? c?p/Bus c?p ngu?n. Ho?c Remote Wakeup.
                  Bit s? 7 lu𮠬࠱.
                  Bit s? 6: 1 T? c?p ngu?n
                            0 Bus c?p ngu?n.
                  Bit s? 5: 1 Cho ph갠suspended USB
                            0 Kh𮧠cho ph갠suspended.
									C⣠bit kh⣠lu𮠢?ng 0.
******************************************************************************/
    0xC0,
/******************************************************************************
bMaxPower   :  X⣠d?nh d򮧠di?n t?i da mࡴhi?t b? c?n c?p.
               Giᡴr? d򮧠di?n = bMaxPower*2 (mA).
               Giᡴr? max l࠵00mA.
******************************************************************************/
    0x32,
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       INTERFACE DESCRIPTOR - INTERFACE DESCRIPTOR - INTERFACE DESCRIPTOR
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/******************************************************************************
bLength :  K thu?c c?a Descriptor.
******************************************************************************/
    0x09, //10
/******************************************************************************
bDescriptorType :  Lo?i Descriptor.
******************************************************************************/
    USB_DESC_TYPE_INTERFACE,  //Lo?i Descriptor lࡉnterface
/******************************************************************************
bInterfaceNumber :  Ch? s? c?a interface. Giᡴr? nṠlࡣhuy뮠bi?t cho m?i
                    Interface. M?c d?nh l࠰.
******************************************************************************/
    0x00,
/******************************************************************************
bAlternateSetting : Khi c?u h􊀠s? d?ng nhi?u interface, m?i interface s? c󠭿t
                    Descriptor tr񮧠v?i bInterfaceNumber vࡧiᡴr? chuy뮠bi?t
                    ? bAlternateSetting. Get_Interface s? l?y setting hi?n t?i,
                    Set_Interface s? c᩠d?t setting s? d?ng. M?c d?nh giᡴr? l࠰
******************************************************************************/
    0x00,
/******************************************************************************
bNumEndpoints :  S? lu?ng EndPoint s? d?ng trong interface ngo᩠EndPoint 0.
******************************************************************************/
    0x03,
/******************************************************************************
bInterfaceClass :  Gi?ng nhu bDeviceClass, nhung ch? cho nh?ng thi?t b? v?i
                   Class du?c x⣠d?nh b?i Interface.
       0x01     :  Audio.
       0x02     :  Communication Interface
       0x03     :  Human Interface Device.
       0x05     :  Physical.
       0x06     :  Image
       0x07     :  Printer
       0x08     :  Mass Storage.
       0x09     :  Hub.
       0x0A     :  Data Interface.
       0x0B     :  Smart Card.
       0x0D     :  Content Security.
       0x0E     :  Video.
       0xDC     :  Diagnostic device.
       0xE0     :  Wireless controller.
       0xFE     :  Application Specific.
                   bInterfaceSubClass:   1  cho Device Firmware Update.
                   bInterfaceSubClass:   2  cho IrDA Bridge
                   bInterfaceSubClass:   3  cho Test vࡍeasurement.
******************************************************************************/
    0xFF, //15
/******************************************************************************
bInterfaceSubClass :  Cung gi?ng nhu bDeviceSubClass  nhung ch? cho nh?ng thi?t b? v?i
                      Class du?c x⣠d?nh b?i Interface
******************************************************************************/
    0xFF,   // Control Model
/******************************************************************************
bInterfaceProtocol :  Cung gi?ng nhu bDeviceProtocol  nhung ch? cho nh?ng thi?t b? v?i
                      Class du?c x⣠d?nh b?i Interface
******************************************************************************/
    0xFF,
/******************************************************************************
iInterface :          ch? s? t?i String mi뵠t? Interface. Giᡴr? l࠰ n?u kh𮧠c󍊠                     chu?i Descriptor.
******************************************************************************/
    0x00,
    
/******************************************************************************
bLength :  K thu?c c?a EndPoint Descriptor.
******************************************************************************/
    0x07,
/******************************************************************************
bDescriptorType :  Lo?i Descriptor.
******************************************************************************/
    USB_DESC_TYPE_ENDPOINT, //20  // Lo?i Descriptor lࡅndPoint
/******************************************************************************
bEndpointAddress :  ??a ch? c?a EndPoint.
******************************************************************************/
		CDC_DATA_IN_EP,      // пa ch? EndPoint IN1
/******************************************************************************
bmAttributes : Bit 1 v࠰ x⣠d?nh ki?u truy?n.
               00:   Control
               01:   Isochronous
               10:   Bulk
               11:   Interrupt.
               //C⣠bit sau ch? ho?t d?ng ? ch? d? Isochronous
               Bit 2 v࠳ x⣠d?nh ch? d? d?ng b?
               00:   Kh𮧠d?ng b?
               01:   B?t d?ng b?
               10:   Th ?ng.
               11:   ??ng b?.
               Bit 4 v࠵ x⣠d?nh ki?u x? d?ng.
               00:   EndPoint d? li?u
               01:   Endpoint ph?n h?i
               10:   Endpoint d? li?u, ph?n h?i ?n.
               11:   Kh𮧠s? d?ng
               Bit 6 v࠷ lu𮠢?ng 0.
******************************************************************************/
    0x02,      //Ki?u truy?n lࡢulk
/******************************************************************************
wMaxPacketSize :  K thu?c t?i da c?a m?t Endpoint trong m?t phi뮠truy?n.
******************************************************************************/
    CDC_DATA_FS_IN_PACKET_SIZE,
    0x00,
/******************************************************************************
bInterval :  X⣠d?nh kho?ng tr? c?a m?t phi뮠truy?n Endpoint. Chi ti?t xem
             USB Spec.
******************************************************************************/
    0x00,    //25
    
/******************************************************************************
bLength :  K thu?c c?a EndPoint Descriptor.
******************************************************************************/
    0x07,
/******************************************************************************
bDescriptorType :  Lo?i Descriptor.
******************************************************************************/
    USB_DESC_TYPE_ENDPOINT,   // Lo?i Descriptor lࡅndPoint
/******************************************************************************
bEndpointAddress :  ??a ch? c?a EndPoint.
******************************************************************************/
		CDC_DATA_OUT_EP,      // ??a ch? EndPoint OUT1
/******************************************************************************
bmAttributes : Bit 1 v࠰ x⣠d?nh ki?u truy?n.
               00:   Control
               01:   Isochronous
               10:   Bulk
               11:   Interrupt.
               //C⣠bit sau ch? ho?t d?ng ? ch? d? Isochronous
               Bit 2 v࠳ x⣠d?nh ch? d? d?ng b?
               00:   Kh𮧠d?ng b?
               01:   B?t d?ng b?
               10:   Th ?ng.
               11:   ??ng b?.
               Bit 4 v࠵ x⣠d?nh ki?u x? d?ng.
               00:   EndPoint d? li?u
               01:   Endpoint ph?n h?i
               10:   Endpoint d? li?u, ph?n h?i ?n.
               11:   Kh𮧠s? d?ng
               Bit 6 v࠷ lu𮠢?ng 0.
******************************************************************************/
    0x02,      //Ki?u truy?n lࡢulk
/******************************************************************************
wMaxPacketSize :  K thu?c t?i da c?a m?t Endpoint trong m?t phi뮠truy?n.
******************************************************************************/
    CDC_DATA_FS_OUT_PACKET_SIZE,   //30
    0x00,
/******************************************************************************
bInterval :  X⣠d?nh kho?ng tr? c?a m?t phi뮠truy?n Endpoint. Chi ti?t xem
             USB Spec.
******************************************************************************/
    0x00,
/******************************************************************************
bLength :  K thu?c c?a EndPoint Descriptor.
******************************************************************************/
    0x07,     //40
/******************************************************************************
bDescriptorType :  Lo?i Descriptor.
******************************************************************************/
    USB_DESC_TYPE_ENDPOINT,
/******************************************************************************
bEndpointAddress :  ??a ch? c?a EndPoint.
******************************************************************************/
		CDC_CMD_OUT_EP,// пa ch? EndPoint OUT2
/******************************************************************************
bmAttributes : Bit 1 v࠰ x⣠d?nh ki?u truy?n.
               00:   Control
               01:   Isochronous
               10:   Bulk
               11:   Interrupt.
               //C⣠bit sau ch? ho?t d?ng ? ch? d? Isochronous
               Bit 2 v࠳ x⣠d?nh ch? d? d?ng b?
               00:   Kh𮧠d?ng b?
               01:   B?t d?ng b?
               10:   Th ?ng.
               11:   ??ng b?.
               Bit 4 v࠵ x⣠d?nh ki?u x? d?ng.
               00:   EndPoint d? li?u
               01:   Endpoint ph?n h?i
               10:   Endpoint d? li?u, ph?n h?i ?n.
               11:   Kh𮧠s? d?ng
               Bit 6 v࠷ lu𮠢?ng 0.
******************************************************************************/
    0x02,    //Truy?n ki?u Bulk
/******************************************************************************
wMaxPacketSize :  K thu?c t?i da c?a m?t Endpoint trong m?t phi뮠truy?n.
******************************************************************************/
    CDC_CMD_FS_OUT_PACKET_SIZE,             /* wMaxPacketSize: */
    0x00,       //45
/******************************************************************************
bInterval :  X⣠d?nh kho?ng tr? c?a m?t phi뮠truy?n Endpoint. Chi ti?t xem
             USB Spec.
******************************************************************************/
    0x00,   //46// B? qua v?i ki?u Bulk transfer.
};
/**************************************************************************************************
 H?T VڎG THAY пI - H?T VڎG THAY пI - H?T VڎG THAY пI - H?T VڎG THAY пI - H?T VڎG THAY пI 
**************************************************************************************************/
data USB_FLAGS Flags = {0};	
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Function name	:USBDeviceCfg()
Description		:USB device mode configuration

DataSheet			:PP40-46
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void USBDeviceCfg()
{
		USB_CTRL = 0x00;
//		USB_CTRL &= ~bUC_HOST_MODE; 
		USB_CTRL |= (bUC_DEV_PU_EN | bUC_INT_BUSY| bUC_DMA_EN);  //USB device and internal pull-up enable, automatically return to NAK before interrupt flag is cleared during interrupt
		USB_DEV_AD = 0x00;

////  USB_CTRL |= bUC_LOW_SPEED;    						//1.5Mbps
////  UDEV_CTRL |= bUD_LOW_SPEED;                                  

    USB_CTRL &= ~(bUC_LOW_SPEED|bUD_LOW_SPEED);								//12Mbps
//    UDEV_CTRL &= ~bUD_LOW_SPEED;   
		UDEV_CTRL |= (bUD_PD_DIS|bUD_PORT_EN);										//
//		UDEV_CTRL |= bUD_PORT_EN;   							//Enable physical port
}

/*******************************************************************************
* Function Name  : USBDeviceIntCfg()
* Description    : USB device mode interrupt initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceIntCfg()
{
    USB_INT_EN |= (bUIE_SUSPEND|bUIE_TRANSFER|bUIE_BUS_RST);              //Enable device hang interrupt                                  
 //   USB_INT_EN |= bUIE_TRANSFER;             //Enable USB transfer completion interrupt                                  
//    USB_INT_EN |= bUIE_BUS_RST;              //Enable device mode USB bus reset interrupt                                  
    USB_INT_FG |= 0x1F;                      //Clear interrupt flag                                  
    IE_USB = 1;                              //Enable USB interrupt                                  
    																                                   
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Function name	:USBDeviceEndPointCfg()
Description		:USB device mode endpoint configuration

DataSheet			:PP40-46
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void USBDeviceEndPointCfg()
{

    UEP2_DMA = (PUINT8X)&DataIn;                                       //Endpoint 2 data transfer address     																	                                         
    UEP2_3_MOD |= (bUEP2_TX_EN|bUEP2_RX_EN);                                  //Endpoint 2 send enable          
 //   UEP2_3_MOD |= bUEP2_RX_EN;                                  //Endpoint 2 Receive Enable        
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;                               //Endpoint 2 single 64 byte send buffer, single 64 byte receive buffer, a total of 128 bytes           
    UEP2_CTRL = bUEP_AUTO_TOG|UEP_T_RES_NAK | UEP_R_RES_ACK;	//Endpoint 2 automatically flips the sync flag, IN transaction returns NAK, OUT returns ACK
    
		UEP1_DMA = (PUINT8X)&CommandIn;
		UEP4_1_MOD |= bUEP1_RX_EN;  
//		UEP4_1_MOD &= ~bUEP1_BUF_MOD;
		UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
	
		UEP0_DMA = Ep0Buffer;																				//Endpoint 0 data transfer address
		UEP4_1_MOD &= ~(bUEP1_BUF_MOD|bUEP4_RX_EN | bUEP4_TX_EN);									//Endpoint 0 single 64 byte send and receive buffer
		UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;									//OUT transaction returns ACK, IN transaction returns NAK
}
/*******************************************************************************
* Function Name  : SendData( PUINT8 SendBuf )
* Description    : Send data to the host serial port
* Input          : PUINT8 SendBuf
* Output         : None
* Return         : None
*******************************************************************************/
void SendData()
{
	UEP2_T_LEN = sizeof(USB_COMMAND);
	UEP2_CTRL &= ~(bUEP_T_RES1 | bUEP_T_RES0);
	UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;	
}

/*******************************************************************************
* Function Name  : RecieveData()
* Description    : USB device mode endpoint configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void RecieveData()
//{
//	TR0 = ET0 = TR2 = ET2 = 0;
//	Flags.ACCESS_BITS.USB_DATA = 1;	
//}
//void RecieveCommand()
//{
//	TR0 = ET0 = TR2 = ET2 = 0;
//	Flags.ACCESS_BITS.USB_CMD = 1;	
//}
static UINT8C StringLangID[STRING_LANGID_SIZE] = 
{
	STRING_LANGID_SIZE,
	USB_DESCR_TYP_STRING,
	0x09, 0x04
};

static UINT8C StringVecdor[STRING_VENDOR_SIZE] = 
{
	STRING_VENDOR_SIZE,
	USB_DESCR_TYP_STRING,
	/* vendor */
	'S', 0,
	'T', 0,
	' ', 0,
	'R', 0,
	'G', 0,
	'B', 0,
};
/*******************************************************************************
* Function Name  : USBInterrupt()
* Description    : USB interrupt handler
*******************************************************************************/
void	USBInterrupt( void ) interrupt INT_NO_USB using 1   //Không được đặt ngắt này trong using 0
{   
	data UINT8 len; 
	if(UIF_TRANSFER)
	{
		switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
		{
			case UIS_TOKEN_SETUP | 0:                                                 
					len = USB_RX_LEN;
					if(len == (sizeof(USB_SETUP_REQ)))
					{   
						 //SetReqtp = UsbSetupBuf->bRequestType;
						 SetupLen = UsbSetupBuf->wLengthL;
						 len = 0;                                                                                                                        
						 SetupReq = UsbSetupBuf->bRequest;
//						 if(SetReqtp == 0xc0)
//						 {
//								len = 2;
//						 }
//						 else if(SetReqtp == 0x40)
//						 {
//								len = 9;                                                        
//						 }
						 //else
						 { 
								switch(SetupReq)                                                
								{
									 case USB_GET_DESCRIPTOR:
												switch(UsbSetupBuf->wValueH)
												{
													 case USB_DESCR_TYP_DEVICE:	                                            
															 pDescr = DevDesc;                                
															 len = sizeof(DevDesc);								       
													 break;	 
													 case USB_DESCR_TYP_CONFIG:									                            
															 pDescr = CfgDesc;                                
															 len = sizeof(CfgDesc);
													 break;	
													 case USB_DESCR_TYP_STRING:	
														 if(UsbSetupBuf->wValueL ==0)
															 {
																	pDescr = StringLangID;
																	len = sizeof(StringLangID);
																}
																else
																{
																	pDescr = StringVecdor;
																	len = sizeof(StringVecdor);
																}
													 break;	
													 default:
															 //len = 0xff;                                      
													 break;
												 }
												 if ( SetupLen > len ) SetupLen = len;                  
												 len = SetupLen >= 8 ? 8 : SetupLen;                    
												 memcpy(Ep0Buffer,pDescr,len);                          
												 SetupLen -= len;
												 pDescr += len;
												 break;						 
										case USB_SET_ADDRESS:
												 SetupLen = UsbSetupBuf->wValueL;                       
												 break;
//										case USB_GET_CONFIGURATION:
//												 Ep0Buffer[0] = UsbConfig;
//												 if ( SetupLen >= 1 ) len = 1;
//												 break;
//										case USB_SET_CONFIGURATION:
//												 UsbConfig = UsbSetupBuf->wValueL;
//												 break;
										default:
												 //len = 0xff;                                            
												 break;    
									 }
								}
						}
//						else
//						{
//								len = 0xff;                                                     
//						}

//						if(len == 0xff)
//						{
//								SetupReq = 0xFF;
//								UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL				     
//						}
//						else 
							//if(len <= 8)                                                   
						{
								UEP0_T_LEN = len;
								UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  
						}
//						else
//						{
//								UEP0_T_LEN = 0;                                                       
//								UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  				     
//						}
						break;
			 case UIS_TOKEN_IN | 0:                                                         //endpoint0 IN
						switch(SetupReq)
						{
							 case USB_GET_DESCRIPTOR:
										len = SetupLen >= 8 ? 8 : SetupLen;                               
										memcpy( Ep0Buffer, pDescr, len );                                 
										SetupLen -= len;
										pDescr += len;
										UEP0_T_LEN = len;
										UEP0_CTRL ^= bUEP_T_TOG;                                          
										break;
							 case USB_SET_ADDRESS:
										USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
										UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
										break;
							 default:
										UEP0_T_LEN = 0;                                                
										UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
										break;
						}
						break;
//			 case UIS_TOKEN_OUT | 0:                                                 // endpoint0 OUT
//						len = USB_RX_LEN;
//						UEP0_T_LEN = 0;                                                    
//						UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;                         									
//						break;
			 case UIS_TOKEN_OUT | 1:    //DATA FROM PC 
					//RecieveCommand();
					TR0 = TR2 = 0;
					Flags.ACCESS_BITS.USB_CMD = 1;
					UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK; 
					break;
//			case UIS_TOKEN_IN | 1:                                                 
//					UEP1_T_LEN = 0;	                                                  						         					 
//					UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;            
//					break;
			 case UIS_TOKEN_OUT | 2:     
					//RecieveData();		
					TR0 = TR2 = 0;
					Flags.ACCESS_BITS.USB_DATA = 1;
					UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK;//UEP_R_RES_ACK;         					 
					break;
			 case UIS_TOKEN_IN | 2:                                                 
					UEP2_T_LEN = 0;                                                    //
					UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;				 
					break;
				default:
						break;
			}
			UIF_TRANSFER = 0;                                         
	}
	if(UIF_BUS_RST)                                             
	{
		USB_DEV_AD = 0x00;
		UIF_SUSPEND = 0;
		UIF_TRANSFER = 0;
		UIF_BUS_RST = 0;                                     
	}
	if (UIF_SUSPEND) 
	{                                                      
		UIF_SUSPEND = 0;
//		if ( USB_MIS_ST & bUMS_SUSPEND ) 
//		{                                                                          
//			while ( XBUS_AUX & bUART0_TX );                                          
//			SAFE_MOD = 0x55;
//			SAFE_MOD = 0xAA;
//			WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO;                                  
//			PCON |= PD;                                                              
//			SAFE_MOD = 0x55;
//			SAFE_MOD = 0xAA;
//			WAKE_CTRL = 0x00;
//		}
	} 
	else 
	{                                                                            
		USB_INT_FG = 0x00;                                                         
	}      
}
