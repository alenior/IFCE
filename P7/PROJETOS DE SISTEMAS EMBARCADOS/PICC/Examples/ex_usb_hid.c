/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_hid.c                             ////
////                                                                 ////
//// USB HID example, for use with the PIC16C765 slow speed USB      ////
//// peripheral or National's USBN960x full speed USB peripheral.    ////
////                                                                 ////
//// CCS has provided an example Windows program that reads / writes ////
//// to the HID device provided in this example.  This Windows       ////
//// example is called hiddemo.exe.                                  ////
////                                                                 ////
//// For extra documentation on developing your own code, see        ////
//// the header files and comments located in the other files        ////
//// of the CCS PIC USB drivers.                                     ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.         ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// ABOUT HID:                                                      ////
////                                                                 ////
//// HID devices are useful because HID drivers are generally        ////
//// installed and supplied with modern operating systems.           ////
//// However, since HID is a general device with general drivers     ////
//// there are limitations to what a HID device can do:              ////
////                                                                 ////
//// - A report (message) can't be larger than 255 bytes.            ////
////  (A report/message != A packet)                                 ////
////                                                                 ////
//// - On slow speed devices the max packet size is 8.               ////
////   On a full speed device the max packet size is 64.             ////
////                                                                 ////
//// - Data is obtained on the host / PC through polling the HID     ////
////   device (PC polls the PIC)                                     ////
////                                                                 ////
//// - On a full speed device, max polling rate is 1 transaction     ////
////   per 1ms. This is 6400 bytes per second (51.2kbaud).           ////
////                                                                 ////
//// - On a slow speed device, max polling rate is 1 transaction     ////
////   per 10ms.  This is 800 bytes per second (6.4kbaud).           ////
////                                                                 ////
//// - No guarantee that polls will happen at a guaranteed rate.     ////
////   If you want guaranteed transfer rate you must use Isochronous ////
////   transfers, which HID is not.                                  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                      WINDOWS DRIVERS:                           ////
////                                                                 ////
//// USB HID drivers come included with all Windows, starting        ////
//// with Windows 98 and up.  Windows 95 does not include USB        ////
//// support, unless you are running Windows 95 Gold (aka OSR2, or   ////
//// Windows 95b).                                                   ////
////                                                                 ////
//// If you plug in the USB device, and it is working, a "Found new  ////
//// device" window should pop-up.  The Add Device wizard then will  ////
//// install the HID drivers automatically.                          ////
////                                                                 ////
//// If you plug in the USB device and do not get this window then   ////
//// the device is not working, or USB does not work on your         ////
//// computer.  Goto the Device Manager (Right Click on my Computer, ////
//// it is located under one of the Tabs.  It is located under the   ////
//// Hardware tab of Windows2000), and make sure that "Universal     ////
//// Serial Bus controllers" is installed.  Open "Universal Serial   ////
//// Bus controllers" and you should see 1 or more "USB Root Hubs".  ////
//// If you see these then that's a good indication that USB is      ////
//// working on your computer.  If you see a question mark and       ////
//// an unkown USB device then this is quite possibly your USB       ////
//// device that is not working.                                     ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#define USB_PIC16C765 1 //set to 1 to use a PIC16c765 USB Peripheral
                        //set to 0 to use a National USBN960x peripheral

#if USB_PIC16C765 //use the PIC16C7x5 peripheral
 #include <16C765.h>
 #device *=16
 #fuses HS,NOWDT,NOPROTECT
 #use delay(clock=24000000)

#else //use the National USBN960x peripheral
 #if defined(__PCM__)
  #include <16F877.h>
  #device *=16
  #fuses HS,NOWDT,NOPROTECT,NOLVP
 #elif defined(__PCH__)
  #include <18F452.h>
  #fuses HS,NOPROTECT,NOLVP
 #endif
 #use delay(clock=20000000)

#endif   //endif check to see which peripheral to use

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#DEFINE LED_1  PIN_B5
#DEFINE LED_2  PIN_B4
#DEFINE LED_3  PIN_B3
#DEFINE BUTTON PIN_A4

//Tells the CCS PIC USB firmware to include HID handling code.
#DEFINE USB_HID_DEVICE  TRUE

//Set this to FALSE if you only want to test enumeration.  Some USB test programs,
//such as USBCHECK, crash if you are trying to test enumeration and the device
//starts sending data once configured.
#DEFINE USB_RUN_WHEN_CONFIGURED  TRUE

//the following defines needed for the CCS USB PIC driver to enable the TX endpoint 1
#define USB_EP1_TX_ENABLE  1   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_TX_SIZE    2   //if you change this, be sure to change the hid descriptor

//the following defines needed for the CCS USB PIC driver to enable the RX endpoint 1
#define USB_EP1_RX_ENABLE  1   //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_RX_SIZE    2   //if you change this, be sure to change the hid descriptor

#if USB_PIC16C765
 #include "pic_usb.h"   //Microchip PIC16C765 hardware layer for usb.c
#else
 #include <usbn960x.c>   //National 960x hardware layer for usb.c
#endif

#include "usb.c"        //handles usb setup tokens and get descriptor reports

void main() {
   int8 out_data[USB_EP1_TX_SIZE];
   int8 in_data[USB_EP1_RX_SIZE];

#ifdef __PCH__
   printf("\r\n\r\nPCH: v");
   printf(__PCH__);
#else
   printf("\r\n\r\nPCM: v");
   printf(__PCM__);
#endif

   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(RA0_RA1_RA3_ANALOG);
   set_adc_channel(1);

   usb_init();

#if !(USB_PIC16C765)
   printf("\r\nUSBN: 0x%X\r\n", usbn_get_version());
   output_low(LED_1);
   output_low(LED_2);
   output_low(LED_3);
   delay_ms(1000);
   output_high(LED_1);
   output_high(LED_2);
#else
   printf("\r\n");
#ENDIF


   printf("\r\n\r\nWaiting for enumeration...");

   while (TRUE) {
         usb_wait_for_enumeration();
         printf("\r\n\r\n***Enumerated***\r\n");
         while(usb_enumerated()) {
         #IF USB_RUN_WHEN_CONFIGURED
            if (usb_kbhit(1)) {
               usb_gets(1, in_data, USB_EP1_RX_SIZE);
               printf("\r\n--> Received 2 bytes: 0x%X 0x%X",in_data[0],in_data[1]);
               if (in_data[0]) {output_low(LED_1);} else {output_high(LED_1);}
               if (in_data[1]) {output_low(LED_2);} else {output_high(LED_2);}
            }
            out_data[0]=read_adc();
            out_data[1]=!input(BUTTON);
            if (usb_put_packet(1, out_data, USB_EP1_TX_SIZE, TOGGLE)) {
                 printf("\r\n<-- Sending 2 bytes: 0x%X 0x%X", out_data[0], out_data[1]);
            }
            delay_ms(500);
         #ENDIF
         }
         printf("\r\n\r\nDevice Un-configured.r\n");
   }
}

