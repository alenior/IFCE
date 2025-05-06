///////////////////////////////////////////////////////////////////////////
///                          usb_desc.h                                ////
////                                                                   ////
//// An example set of device / configuration descriptors.             ////
////                                                                   ////
//// Descriptors are application dependent (especially HID) so you may ////
//// have to edit them.                                                ////
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
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Version History:                                                  ////
////                                                                   ////
//// May 6th, 2003: Fixed non-HID descriptors pointing to faulty       ////
////                strings                                            ////
////                                                                   ////
//// August 2nd, 2002: Initial Public Release                          ////
////                                                                   ////
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

#IFNDEF __USB_DESCRIPTORS__
#DEFINE __USB_DESCRIPTORS__

#include "usb.h"

#IF USB_HID_DEVICE
   //usb descriptor reports
   #DEFINE USB_NUM_CONFIGURATIONS 1
   #DEFINE USB_NUM_INTERFACES     1
   #DEFINE USB_NUM_ENDPOINTS      2          //endpoint 1 OUT, endpoint 1 IN.  endpoint 0 doesnt count here
   #DEFINE USB_NUM_CLASSES        1            //hid

   //you cant use pointers when storing constants to program memory.
   BYTE CONST USB_CONFIG_DESCRIPTORS[USB_NUM_CONFIGURATIONS]={0};//0
   BYTE CONST USB_INTERFACE_DESCRIPTORS[USB_NUM_INTERFACES]={9};//9
   BYTE CONST USB_CLASS_DESCRIPTORS[USB_NUM_CLASSES]={18};//18
   BYTE CONST USB_ENDPOINT_DESCRIPTORS[USB_NUM_ENDPOINTS]={27,34};//27

   #DEFINE USB_TOTAL_CONFIG_LEN      41  //config+interface+class+endpoint+endpoint (2 endpoints)

   #DEFINE USB_HID_DESC_LEN          28

   BYTE CONST USB_DEVICE_DESC[] ={
      //starts of with device configuration. only one possible
         USB_DEVICE_DESC_LEN, //the length of this report   ==1
         0x01, //the constant DEVICE (DEVICE 0x01)  ==2
         0x10,0x01, //usb version in bcd (pic167xx is 1.1) ==3,4
         0x00, //class code ==5
         0x00, //subclass code ==6
         0x00, //protocol code ==7
         USB_MAX_EP0_PACKET_LENGTH, //max packet size for endpoint 0. (SLOW SPEED SPECIFIES 8) ==8
         0x61,0x04, //vendor id (0x04D8 is Microchip, or is it 0x0461 ??)
         0x20,0x00, //product id   ==11,12  //don't use ffff says usb-by-example guy.  oops
         0x00,0x01, //device release number  ==13,14
         0x01, //index of string description of manufacturer. therefore we point to string_1 array (see below)  ==15
         0x02, //index of string descriptor of the product  ==16
         0x00, //index of string descriptor of serial number  ==17
         USB_NUM_CONFIGURATIONS  //number of possible configurations  ==18
   };


   BYTE CONST USB_CONFIG_DESC[USB_TOTAL_CONFIG_LEN] = {
   //IN ORDER TO COMPLY WITH WINDOWS HOSTS, THE ORDER OF THIS ARRAY MUST BE:
      //    config(s)
      //    interface(s)
      //    class(es)
      //    endpoint(s)

   //config_descriptor for config index 1
         USB_CONFIG_DESC_LEN, //length of descriptor size          ==1
         USB_CONFIG_DESC_KEY, //constant CONFIGURATION (CONFIGURATION 0x02)     ==2
         USB_TOTAL_CONFIG_LEN,0, //size of all data returned for this config      ==3,4
         USB_NUM_INTERFACES, //number of interfaces this device supports       ==5
         0x01, //identifier for this configuration.  (IF we had more than one configurations)      ==6
         0x00, //index of string descriptor for this configuration      ==7
         0xC0, //bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 unused and bit7=1         ==8
         0x32, //maximum bus power required (maximum milliamperes/2)  (0x32 = 100mA)

   //interface descriptor 1
         USB_INTERFACE_DESC_LEN, //length of descriptor      =10
         USB_INTERFACE_DESC_KEY, //constant INTERFACE (INTERFACE 0x04)       =11
         0x00, //number defining this interface (IF we had more than one interface)    ==12
         0x00, //alternate setting     ==13
         USB_NUM_ENDPOINTS, //number of endpoins, except 0 (pic167xx has 3, but we dont have to use all).       ==14
         0x03, //class code, 03 = HID     ==15
         0x00, //subclass code //boot     ==16
         0x00, //protocol code      ==17
         0x00, //index of string descriptor for interface      ==18

   //class descriptor 1  (HID)
         USB_CLASS_DESC_LEN, //length of descriptor    ==19
         USB_CLASS_DESC_KEY, //dscriptor type (0x21 == HID)      ==20
         0x00,0x01, //hid class release number (1.0) (try 1.10)      ==21,22
         0x00, //localized country code (0 = none)       ==23
         0x01, //number of hid class descrptors that follow (1)      ==24
         0x22, //report descriptor type (0x22 == HID)                ==25
         USB_HID_DESC_LEN, 0x00, //length of report descriptor            ==26,27

   //endpoint descriptor
         USB_ENDPOINT_DESC_LEN, //length of descriptor                   ==28
         USB_ENDPOINT_DESC_KEY, //constant ENDPOINT (ENDPOINT 0x05)          ==29
         0x81, //endpoint number and direction (0x81 = EP1 IN)       ==30
         0x03, //transfer type supported (0x03 is interrupt)         ==31
         0x08,0x00, //maximum packet size supported                  ==32,33
         10,  //polling interval, in ms.  (cant be smaller than 10)      ==34

   //endpoint descriptor
         USB_ENDPOINT_DESC_LEN, //length of descriptor                   ==35
         USB_ENDPOINT_DESC_KEY, //constant ENDPOINT (ENDPOINT 0x05)          ==36
         0x01, //endpoint number and direction (0x01 = EP1 OUT)      ==37
         0x03, //transfer type supported (0x03 is interrupt)         ==38
         0x08,0x00, //maximum packet size supported                  ==39,40
         10 //polling interval, in ms.  (cant be smaller than 10)    ==41
   };


   //////////////////////////////////////////////////////////////////
   ///
   ///  HID Report.  Tells HID driver how to handle and deal with
   ///  received data.  HID Reports can be extremely complex,
   ///  see HID specifcation for help on writing your own.
   ///
   ///  CCS example uses a vendor specified usage, that sends and
   ///  receives 2 absolute bytes ranging from 0 to 0xFF.
   ///
   //////////////////////////////////////////////////////////////////
   BYTE CONST USB_HID_DESC[] = {  //len=28
      6, 0, 255,    // Usage Page = Vendor Defined
      9, 1,            // Usage = IO device
      0xa1, 1,       // Collection = Application
      0x19, 1,        // Usage minimum
      0x29, 8,        // Usage maximum

      0x15, 0x80,        // Logical minimum (-128)
      0x25, 0x7F,        // Logical maximum (127)

      0x75, 8,        // Report size = 8 (bits)
      0x95, 2,        // Report count = 16 bits (2 bytes)
      0x81, 2,        // Input (Data, Var, Abs)
      0x19, 1,        // Usage minimum
      0x29, 8,        // Usage maximum
      0x91, 2,        // Output (Data, Var, Abs)
      0xc0            // End Collection
   };

#ELSE

   //usb descriptor reports
   #DEFINE USB_NUM_CONFIGURATIONS 1
   #DEFINE USB_NUM_INTERFACES     1
   #DEFINE USB_NUM_ENDPOINTS      2          //endpoint 1 OUT, endpoint 1 IN.  endpoint 0 doesnt count here
   #DEFINE USB_NUM_CLASSES        0            //hid

   //you cant use pointers when storing constants to program memory.
   BYTE CONST USB_CONFIG_DESCRIPTORS[USB_NUM_CONFIGURATIONS]={0};//0
   BYTE CONST USB_INTERFACE_DESCRIPTORS[USB_NUM_INTERFACES]={9};//9
   BYTE CONST USB_ENDPOINT_DESCRIPTORS[USB_NUM_ENDPOINTS]={18,25};//27

   #DEFINE USB_TOTAL_CONFIG_LEN      32  //config+interface+endpoint+endpoint (2 endpoints)

   //device descriptor
   BYTE CONST USB_DEVICE_DESC[USB_DEVICE_DESC_LEN] ={
         USB_DEVICE_DESC_LEN,          //the length of this report
         0x01,                //constant DEVICE (0x01)
         0x10,0x01,           //usb version in bcd
         0x00,                //class code (if 0, interface defines class.  FF is vendor defined)
         0x00,                //subclass code
         0x00,                //protocol code
         USB_MAX_EP0_PACKET_LENGTH,   //max packet size for endpoint 0. (SLOW SPEED SPECIFIES 8)
         0x61,0x04,           //vendor id (0x04D8 is Microchip)
         0x03,0x00,           //product id
         0x00,0x01,           //device release number
         0x01,                //index of string description of manufacturer. therefore we point to string_1 array (see below)
         0x02,                //index of string descriptor of the product
         0x00,                //index of string descriptor of serial number
         USB_NUM_CONFIGURATIONS   //number of possible configurations
   };


   //configuration descriptor
   BYTE CONST USB_CONFIG_DESC[USB_TOTAL_CONFIG_LEN] = {
   //config_descriptor for config index 1
         USB_CONFIG_DESC_LEN,     //length of descriptor size
         USB_CONFIG_DESC_KEY,         //constant CONFIGURATION (0x02)
         USB_TOTAL_CONFIG_LEN,0,  //size of all data returned for this config
         USB_NUM_INTERFACES,      //number of interfaces this device supports
         0x01,                //identifier for this configuration.  (IF we had more than one configurations)
         0x00,                //index of string descriptor for this configuration
         0xC0,                //bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 reserved and bit7=1
         0x32,                //maximum bus power required (maximum milliamperes/2)  (0x32 = 100mA)

   //interface descriptor 0 alt 0
         USB_INTERFACE_DESC_LEN,  //length of descriptor
         USB_INTERFACE_DESC_KEY,      //constant INTERFACE (0x04)
         0x00,                //number defining this interface (IF we had more than one interface)
         0x00,                //alternate setting
         USB_NUM_ENDPOINTS,       //number of endpoints, except 0.
         0xFF,                //class code, FF = vendor defined
         0xFF,                //subclass code, FF = vendor
         0xFF,                //protocol code, FF = vendor
         0x00,                //index of string descriptor for interface

   //endpoint descriptor
         USB_ENDPOINT_DESC_LEN, //length of descriptor
         USB_ENDPOINT_DESC_KEY,     //constant ENDPOINT (0x05)
         0x81,              //endpoint number and direction (0x81 = EP1 IN)
         0x02,              //transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
         0x40,0x00,         //maximum packet size supported
         0x01,              //polling interval in ms. (for interrupt transfers ONLY)

   //endpoint descriptor
         USB_ENDPOINT_DESC_LEN, //length of descriptor
         USB_ENDPOINT_DESC_KEY,     //constant ENDPOINT (0x05)
         0x01,              //endpoint number and direction (0x01 = EP1 OUT)
         0x02,              //transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
         0x40,0x00,         //maximum packet size supported
         0x01,              //polling interval in ms. (for interrupt transfers ONLY)

  };
#ENDIF


//////////////////////////////////////////////////////////////////
///
///   start string descriptors
///   String 0 is a special language string, and must be defined.  People in U.S.A. can leave this alone.
///
///   You must define the length else get_next_string_character() will not see the string
///   Current code only supports 10 strings (0 thru 9)
///
//////////////////////////////////////////////////////////////////

#DEFINE USB_STRING_0_LEN 4
BYTE CONST USB_STRING_0[4] = {  //string index 0 is special
         USB_STRING_0_LEN, //length of string index
         0x03, //descriptor type 0x03 (STRING)
         0x09,0x04   //Microsoft Defined for US-English
};

#DEFINE USB_STRING_1_LEN 8
BYTE CONST USB_STRING_1[8] = { //we defined string index 1 earlier as manuf string
         USB_STRING_1_LEN, //length of string index
         0x03, //descriptor type 0x03 (STRING)
         'C',0,
         'C',0,
         'S',0
};

#IF USB_HID_DEVICE
   #DEFINE USB_STRING_2_LEN 26
   BYTE CONST USB_STRING_2[26] = {  //we defined string index 2 as description of prodcut
            USB_STRING_2_LEN, //length of string index
            0x03, //descriptor type 0x03 (STRING)
            'C',0,
            'C',0,
            'S',0,
            ' ',0,
            'H',0,
            'I',0,
            'D',0,
            ' ',0,
            'D',0,
            'e',0,
            'm',0,
            'o',0
   };
#else
   #DEFINE USB_STRING_2_LEN 28
   BYTE CONST USB_STRING_2[28] = {  //we defined string index 2 as description of prodcut
            USB_STRING_2_LEN, //length of string index
            0x03, //descriptor type 0x03 (STRING)
            'C',0,
            'C',0,
            'S',0,
            ' ',0,
            'B',0,
            'u',0,
            'l',0,
            'k',0,
            ' ',0,
            'D',0,
            'e',0,
            'm',0,
            'o',0
   };
#endif

#ENDIF
