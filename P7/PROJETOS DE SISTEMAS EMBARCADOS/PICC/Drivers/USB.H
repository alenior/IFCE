///////////////////////////////////////////////////////////////////////////
////                              usb.h                                ////
////                                                                   ////
//// Function protypes, defintions and globals used by CCS USB driver  ////
////                                                                   ////
//// This file is part of CCS's PIC USB driver code, which includes:   ////
////   usb_desc.h - an example set of config and device descriptors    ////
////   usb.c - USB token and request handler code                      ////
////   usb.h - definitions, prototypes and global variables            ////
////   And the hardware peripheral level.  At the time of writing,     ////
////    CCS provides two hardware peripheral drivers.  National        ////
////    USBN960x (usbn960x.cc) and PIC16C7x5 (usb_pic.c).              ////
////                                                                   ////
//// Two examples are given using CCS's PIC USB driver.                ////
//// ex_usb_scope.c is a bulk device that sends 512 bytes to the       ////
//// host and uses the USBN960x.  ex_usb_hid.c is a HID device and     ////
//// uses the PIC16C7x5.                                               ////
////                                                                   ////
////                   **** DEFINITIONS *****                          ////
//// The following definitions are declared here, but can be           ////
//// overwritten in your code.  Unless needed otherwise, leave         ////
//// to default value.  If confused about a definition read the        ////
//// comments at each defintion                                        ////
////                                                                   ////
//// USB_SUPPORT_IDLE (FALSE) - Set to TRUE if your device supports    ////
////                            Set_Idle HID class request.  Set to    ////
////                            False if you do not (device will       ////
////                            send a Wrong-state if computer         ////
////                            sends a Set_Idle / Get_Idle command)   ////
////                            NOTE: If you set to TRUE you must      ////
////                            provide your own code.  See            ////
////                            usb_set_idle() and usb_get_idle() in   ////
////                            usb.c                                  ////
////                                                                   ////
//// USB_BOOT_PROTOCOL (FALSE) - Set to TRUE if your device supports   ////
////                            Set_Protocl HID class request. Set to  ////
////                            False if you do not (device will       ////
////                            send a Wrong-state if computer         ////
////                            sends a Set_Protocl / Get_Protocol     ////
////                            command).                              ////
////                            NOTE: If you set to TRUE you must      ////
////                            provide your own code in the           ////
////                            application that properly send boot    ////
////                            or HID packets.                        ////
////                                                                   ////
//// USB_MAX_EP0_PACKET_LENGTH (8) - Max Packet size for Endpoint 0    ////
////                                                                   ////
//// USB_EPx_RX_ENABLE (0) - Where x is the endpoint number. EP0 is    ////
////                         always enabled.  Setting to 0 keeps       ////
////                         endpoint disabled, setting 1 enables      ////
////                         endpoint for bulk/interrupt, and          ////
////                         setting 2 enables endpoint for iso.       ////
////                         Endpoint is enabled when host             ////
////                         sends a configuration.                    ////
////                                                                   ////
//// USB_EPx_RX_SIZE (0) - Size to make RX buffer.                     ////
////                                                                   ////
//// USB_DO_DEBUG (FALSE) - Whether or not to print out debug          ////
////                        information out serial port.  Enabling     ////
////                        may cause problems due to timing.          ////
////                        Uses PIC's Transmit Buffer Empty interrupt ////
////                                                                   ////
//// USB_HID_DEVICE (TRUE) - HID devices require extra code to handle  ////
////                         HID requests.  You can disable to save    ////
////                         ROM space if you are not using a HID      ////
////                         device.  If you are not using a HID       ////
////                         device you must provide your own O/S      ////
////                         (Windows) driver.                         ////
////                                                                   ////
//// The other definitions should not be changed.                      ////
////                                                                   ////
////                 **** SPECIAL VARIABLES ****                       ////
//// The following variables contain either status bits or             ////
//// receive/transmit buffers used by the USB routines.                ////
////                                                                   ////
//// usb_epX_rx_buffer - Array containing data host sent to PIC.       ////
////                     X is endpoint number.                         ////
////                                                                   ////
//// usb_epX_rx_status - Status of RX buffer.  X is endpoint number.   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Version History:                                                  ////
////                                                                   ////
//// October 15th, 2003: Support for boot protocol added.              ////
////         Set USB_BOOT_PROTOCOL to TRUE to support this.            ////
////         The array hid_protocol[] saves which protocol mode each   ////
////         interface is in.  It is your applications job to send     ////
////         data that either fit the boot protocol or HID protocol.   ////
////                                                                   ////
//// May 6th, 2003: Fixed a potential stack overflow using PCM         ////
////                                                                   ////
//// October 29th, 2002: New definition added to USB_STATES            ////
////                                                                   ////
//// August 2nd, 2002: Initial Public Release                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2002 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////


#IFNDEF __USB_PROTOTYPES__
#DEFINE __USB_PROTOTYPES__

#IFNDEF USB_SUPPORT_IDLE
   #DEFINE USB_SUPPORT_IDLE  FALSE
#ENDIF

#IFNDEF USB_BOOT_PROTOCOL
   #DEFINE USB_BOOT_PROTOCOL FALSE
#ENDIF


#ifndef USB_DO_DEBUG
   #DEFINE USB_DO_DEBUG  FALSE
#ENDIF

//should the compiler add the extra HID handler code?  Defaults to yes.
#IFNDEF USB_HID_DEVICE
   #DEFINE USB_HID_DEVICE TRUE
#ENDIF

//CCS code has only been tested with a max packet length of 8, but code
//was written to handle a different size.
//Not recommended to change, however.  Slow speed requires 8.
#IFNDEF USB_MAX_EP0_PACKET_LENGTH
  #DEFINE USB_MAX_EP0_PACKET_LENGTH 8
#ENDIF


#define USB_EP0_RX_SIZE    USB_MAX_EP0_PACKET_LENGTH  //endpoint 0 is setup, and should always be the MAX_PACKET_LENGTH.  Slow speed specifies 8

//*** ENABLE RX ENDPOINTS AND BUFFERS
//ENABLE -> 0 = off, 1 = interrupt or bulk transfers, 2 = iso transfers
#ifndef USB_EP1_RX_ENABLE
   #define USB_EP1_RX_ENABLE  0  //default to off
   #ifndef USB_EP1_RX_SIZE
      #define USB_EP1_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP2_RX_ENABLE
   #define USB_EP2_RX_ENABLE  0  //default to off
   #ifndef USB_EP2_RX_SIZE
      #define USB_EP2_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP3_RX_ENABLE
   #define USB_EP3_RX_ENABLE  0  //default to off
   #ifndef USB_EP3_RX_SIZE
      #define USB_EP3_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP4_RX_ENABLE
   #define USB_EP4_RX_ENABLE  0  //default to off
   #ifndef USB_EP4_RX_SIZE
      #define USB_EP4_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP5_RX_ENABLE
   #define USB_EP5_RX_ENABLE  0  //default to off
   #ifndef USB_EP5_RX_SIZE
      #define USB_EP5_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP6_RX_ENABLE
   #define USB_EP6_RX_ENABLE  0  //default to off
   #ifndef USB_EP6_RX_SIZE
      #define USB_EP6_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP7_RX_ENABLE
   #define USB_EP7_RX_ENABLE  0  //default to off
   #ifndef USB_EP7_RX_SIZE
      #define USB_EP7_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP8_RX_ENABLE
   #define USB_EP8_RX_ENABLE  0  //default to off
   #ifndef USB_EP8_RX_SIZE
      #define USB_EP8_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP9_RX_ENABLE
   #define USB_EP9_RX_ENABLE  0  //default to off
   #ifndef USB_EP9_RX_SIZE
      #define USB_EP9_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP10_RX_ENABLE
   #define USB_EP10_RX_ENABLE  0  //default to off
   #ifndef USB_EP10_RX_SIZE
      #define USB_EP10_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP11_RX_ENABLE
   #define USB_EP11_RX_ENABLE  0  //default to off
   #ifndef USB_EP11_RX_SIZE
      #define USB_EP11_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP12_RX_ENABLE
   #define USB_EP12_RX_ENABLE  0  //default to off
   #ifndef USB_EP12_RX_SIZE
      #define USB_EP12_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP13_RX_ENABLE
   #define USB_EP13_RX_ENABLE  0  //default to off
   #ifndef USB_EP13_RX_SIZE
      #define USB_EP13_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP14_RX_ENABLE
   #define USB_EP14_RX_ENABLE  0  //default to off
   #ifndef USB_EP14_RX_SIZE
      #define USB_EP14_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

#ifndef USB_EP15_RX_ENABLE
   #define USB_EP15_RX_ENABLE  0  //default to off
   #ifndef USB_EP15_RX_SIZE
      #define USB_EP15_RX_SIZE 0  //if buffer length wasn't already defined, set to 8
   #endif
#endif

//*** ENABLE TX ENDPOINTS AND BUFFERS
//ENABLE -> 0 = off, 1 = interrupt or bulk transfers, 2 = iso transfers
#ifndef USB_EP1_TX_ENABLE
   #define USB_EP1_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP2_TX_ENABLE
   #define USB_EP2_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP3_TX_ENABLE
   #define USB_EP3_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP4_TX_ENABLE
   #define USB_EP4_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP5_TX_ENABLE
   #define USB_EP5_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP6_TX_ENABLE
   #define USB_EP6_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP7_TX_ENABLE
   #define USB_EP7_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP8_TX_ENABLE
   #define USB_EP8_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP9_TX_ENABLE
   #define USB_EP9_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP10_TX_ENABLE
   #define USB_EP10_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP11_TX_ENABLE
   #define USB_EP11_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP12_TX_ENABLE
   #define USB_EP12_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP13_TX_ENABLE
   #define USB_EP13_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP14_TX_ENABLE
   #define USB_EP14_TX_ENABLE  0  //default to off
#endif
#ifndef USB_EP15_TX_ENABLE
   #define USB_EP15_TX_ENABLE  0  //default to off
#endif


typedef struct {
   int1 rx;       //set to 1 if we received data
   int1 ov;       //set to 1 if a buffer overrun
   int  len : 6;  //amount of data received in last packet.  (max 64)
} usb_rx_status_type;

int8 usb_ep0_rx_buffer[USB_EP0_RX_SIZE];  //if needed, define EP1 RX buffer
usb_rx_status_type usb_ep0_rx_status;     //holds rx, ov and len status for each rx endpoint.

#if USB_EP1_RX_SIZE && USB_EP1_RX_ENABLE
   int8 usb_ep1_rx_buffer[USB_EP1_RX_SIZE];  //if needed, define EP1 RX buffer
   usb_rx_status_type usb_ep1_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP2_RX_SIZE && USB_EP2_RX_ENABLE
   int8 usb_ep2_rx_buffer[USB_EP2_RX_SIZE];  //if needed, define EP2 RX buffer
   usb_rx_status_type usb_ep2_rx_status; //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP3_RX_SIZE && USB_EP3_RX_ENABLE
   int8 usb_ep3_rx_buffer[USB_EP3_RX_SIZE];  //if needed, define EP3 RX buffer
   usb_rx_status_type usb_ep3_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP4_RX_SIZE && USB_EP4_RX_ENABLE
   int8 usb_ep4_rx_buffer[USB_EP4_RX_SIZE];  //if needed, define EP4 RX buffer
   usb_rx_status_type usb_ep4_rx_status; //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP5_RX_SIZE && USB_EP5_RX_ENABLE
   int8 usb_ep5_rx_buffer[USB_EP5_RX_SIZE];  //if needed, define EP5 RX buffer
   usb_rx_status_type usb_ep5_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP6_RX_SIZE && USB_EP6_RX_ENABLE
   int8 usb_ep6_rx_buffer[USB_EP6_RX_SIZE];  //if needed, define EP6 RX buffer
   usb_rx_status_type usb_ep6_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP7_RX_SIZE && USB_EP7_RX_ENABLE
   int8 usb_ep7_rx_buffer[USB_EP7_RX_SIZE];  //if needed, define EP7 RX buffer
   usb_rx_status_type usb_ep7_rx_status; //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP8_RX_SIZE && USB_EP8_RX_ENABLE
   int8 usb_ep8_rx_buffer[USB_EP8_RX_SIZE];  //if needed, define EP8 RX buffer
   usb_rx_status_type usb_ep8_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP9_RX_SIZE && USB_EP9_RX_ENABLE
   int8 usb_ep9_rx_buffer[USB_EP9_RX_SIZE];  //if needed, define EP9 RX buffer
   usb_rx_status_type usb_ep9_rx_status; //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP10_RX_SIZE && USB_EP10_RX_ENABLE
   int8 usb_ep10_rx_buffer[USB_EP10_RX_SIZE];  //if needed, define EP10 RX buffer
   usb_rx_status_type usb_ep10_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP11_RX_SIZE && USB_EP11_RX_ENABLE
   int8 usb_ep11_rx_buffer[USB_EP11_RX_SIZE];  //if needed, define EP11 RX buffer
   usb_rx_status_type usb_ep11_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP12_RX_SIZE && USB_EP12_RX_ENABLE
   int8 usb_ep12_rx_buffer[USB_EP12_RX_SIZE];  //if needed, define EP12 RX buffer
   usb_rx_status_type usb_ep12_rx_status; //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP13_RX_SIZE && USB_EP13_RX_ENABLE
   int8 usb_ep13_rx_buffer[USB_EP13_RX_SIZE];  //if needed, define EP13 RX buffer
   usb_rx_status_type usb_ep13_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP14_RX_SIZE && USB_EP14_RX_ENABLE
   int8 usb_ep14_rx_buffer[USB_EP14_RX_SIZE];  //if needed, define EP14 RX buffer
   usb_rx_status_type usb_ep14_rx_status; //holds rx, ov and len status for each rx endpoint.
#endif
#if USB_EP15_RX_SIZE && USB_EP15_RX_ENABLE
   int8 usb_ep15_rx_buffer[USB_EP15_RX_SIZE];  //if needed, define EP15 RX buffer
   usb_rx_status_type usb_ep15_rx_status;  //holds rx, ov and len status for each rx endpoint.
#endif

//// DO NOT CHANGE THE FOLLOWING VALUES:

enum USB_STATES {
CLEAR_FEATURE, GET_CONFIGURATION, GET_DESCRIPTOR, GET_STRING_DESCRIPTOR, GET_DEVICE,   //0-4
GET_INTERFACE, GET_STATUS, GET_REPORT, GET_HID_REP, GET_HID_DESC,                      //5-9
SET_CONFIG, SET_ADDRESS, SET_CONFIGURATION, SET_FEATURE, SET_INTERFACE,                //10-14
VEND_SET_MEMORY, SEND_0LEN, SEND_0LEN_TGL, NONE};                                      //15-18

enum PID_TOGGLE {DATA1, TOGGLE, DATA0};

int USB_address_pending;                        //save previous state because packets can take several isrs
USB_STATES USB_dev_req;                         //what did the last setup token set us up to do?.  init at none
int String_ptr=0; int String_end=0; int string; //for reading string and config descriptors
int host_max_length;                            //the max length of data the host states for the next packet of data
int USB_Curr_Config=0;                          //our current config.  start at none/powered
int USB_status_device=1;                        //Holds our state for Set_Feature and Clear_Feature


//PID values for tokens (see page 48 of USB Complete ed.1)
#define PID_OUT      0x01  //host to device transactions
#define PID_ACK      0x02  //receiver accepts error-free data packet
#define PID_DATA0    0x03  //data packet with even sync bit
#define PID_SOF      0x05  //start of framer marker and frame number
#define PID_IN       0x09  //device to host transactions
#define PID_NAK      0x0A  //receiver can't accept data or sender cant send data or has no data to transmit
#define PID_DATA1    0x0B  //data packet with odd sync bit
#define PID_PRE      0x0C  //preamble issued by host.  enables downstream traffic to low-speed device
#define PID_SETUP    0x0D  //host to device setup transaction
#define PID_STALL    0x0E  //a control request isnt supported or the endpoint is halted

//Key which identifies descritpors
#DEFINE USB_DEVICE_DESC_KEY      0x01
#DEFINE USB_CONFIG_DESC_KEY      0x02
#DEFINE USB_STRING_DESC_KEY      0x03
#DEFINE USB_INTERFACE_DESC_KEY   0x04
#DEFINE USB_ENDPOINT_DESC_KEY    0x05
#DEFINE USB_CLASS_DESC_KEY       0x21

//The length of each descriptor
#DEFINE USB_DEVICE_DESC_LEN      18
#DEFINE USB_CONFIG_DESC_LEN      9
#DEFINE USB_INTERFACE_DESC_LEN   9
#DEFINE USB_CLASS_DESC_LEN       9
#DEFINE USB_ENDPOINT_DESC_LEN    7


/// *** User function prototypes
void usb_wait_for_enumeration(void);
int1 usb_unermated(void);
int1 usb_puts(int8 endpoint, int8 * ptr, int16 len, int8 packet_size, int8 timeout);
int1 usb_kbhit(int8 endpoint);
int8 usb_gets(int8 endpoint, int8 * ptr, int8 max);

/// *** Token and request hanlder function prototypes
void usb_isr_tkn_CheckVendor();
#inline
int get_next_string_character();
#inline
void usb_copy_string_seg_to_ep(int8 endpoint, PID_TOGGLE tgl);
#inline
void usb_copy_config_seg_to_ep(int8 endpoint, PID_TOGGLE tgl);
#inline
void usb_copy_device_seg_to_ep(int8 endpoint, PID_TOGGLE tgl);
void usb_isr_tok_in_dne(int8 endpoint);
void usb_isr_tok_out_dne(int8 endpoint);
void usb_isr_tok_setup_dne();
void usb_isr_tkn_HostToDevice();
void usb_isr_tkn_HostToInterface();
void usb_isr_tkn_HostToEndpoint();
void usb_isr_tkn_DeviceToHost();
void usb_isr_tkn_InterfaceToHost();
void usb_isr_tkn_EndpointToHost();
void usb_isr_tkn_ClassRequest();
void usb_Get_Descriptor();
void usb_GetClassSpecificDescriptor();
void usb_GetCh9Descriptor();
void usb_Get_Device_Descriptor();
void usb_Get_Config_Descriptor(int type);
void usb_Get_String_Descriptor();
void usb_Get_Device_Status();
void usb_Get_Interface_Status();
void usb_Get_Endpoint_Status();
void usb_Set_Address_Token();
void usb_finish_set_address();
void usb_Clear_Device_Feature();
void usb_Clear_Endpoint_Feature();
void usb_Clear_Interface_Feature();
void usb_Set_Device_Feature();
void usb_Set_Endpoint_Feature();
void usb_Set_Interface_Feature();
void usb_Get_Configuration_Token();
void usb_Set_Configuration_Token();
void usb_Get_Interface();
void usb_Set_Interface();
void usb_Set_Feature();
void usb_set_idle();
void usb_get_idle();
void usb_set_protocol();
void usb_get_protocol();
void usb_token_reset();

#IF USB_HID_DEVICE
   void usb_Get_HID_Descriptor();
   void usb_Get_HID_Report();
   #inline
   void usb_copy_hid_seg_to_ep(int endpoint, PID_TOGGLE tgl);
#ENDIF



/********************************************************************************
/*
/* The following block of code turns on DEBUGGING outputs, only if the users said so.
/*
/* A transmit buffer is used to help remove CPU load from the USB
/* Debugging doesn't always work, and sometimes will prevent the USB device from
/* working.  Debugging is off by default, and should only be turned on by a user
/* willing to find a way to work around the problems caused by debugging.
/*
/* Only recommended for PIC18, there isn't enough memory to buffer the data in the PIC16
/*
/**********************************************************************************/
#IF USB_DO_DEBUG
 #define debug printf
 #IF defined(__PCH__)
  #define TBE_BUFFER_SIZE 1000
  int16 next_in=0, next_out=0;
  #bit tbe=0xF9E.4
 #ELSE
  #define TBE_BUFFER_SIZE 50
  int8 next_in=0, next_out=0;
  #bit tbe=0x0C.4
 #ENDIF

 int8 tbe_buffer[TBE_BUFFER_SIZE];

 void debug_txb(int8 c) {
   if ((tbe)&&(next_out == next_in)) { //tx buffer empty, our buffer empty
      putc(c);
      enable_interrupts(int_tbe);
   }
   else {   //tx buffer not empty or our buffer not empty
      enable_interrupts(int_tbe);
      tbe_buffer[next_in++]=c;
      if (next_in >= TBE_BUFFER_SIZE) {next_in=0;}
   }
 }

 #int_tbe
 void tbe_isr() {
   if (next_in != next_out) {
      putc(tbe_buffer[next_out++]);
      if (next_out >= TBE_BUFFER_SIZE) {next_out=0;}
   }
   else {
      disable_interrupts(int_tbe);
   }
 }

#ELSE
 #define debug(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)  //this will ignore debug lines
 void debug_txb(int8 c) {}
#ENDIF




#ENDIF
