/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_mouse.c                           ////
////                                                                 ////
//// An example of creating a USB mouse.  Uses your operating        ////
//// system's HID drivers, which on most systems should already be   ////
//// installed.                                                      ////
////                                                                 ////
//// For a more generic USB HID example (as well as some HID         ////
//// documentation) see ex_usb_hid.c                                 ////
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

#define USB_PIC16C765 0 //set to 1 to use a PIC16c765 USB Peripheral
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
  #fuses HS,NOPROTECT,NOLVP,NOWDT
 #endif
 #use delay(clock=20000000)

#endif   //endif check to see which peripheral to use

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

//Tells the CCS PIC USB firmware to include HID handling code.
#DEFINE USB_HID_DEVICE  TRUE
#DEFINE USB_BOOT_PROTOCOL TRUE

//the following defines needed for the CCS USB PIC driver to enable the TX endpoint 1
#define USB_EP1_TX_ENABLE  1   //turn on EP1 for IN bulk/interrupt transfers

#include <mouse_desc.h>

#if USB_PIC16C765
 #include <pic_usb.h>   //Microchip PIC16C765 hardware layer for usb.c
#else
 #include <usbn960x.c>   //National 960x hardware layer for usb.c
#endif

#include <usb.c>        //handles usb setup tokens and get descriptor reports

void main() {
   int8 out_data[4];
   int8 x=1,y=0,buttons=0,wheel=0;
   int8 count=0;
  
#ifdef __PCH__
   printf("\r\n\r\nPCH: v");
   printf(__PCH__);
#else
   printf("\r\n\r\nPCM: v");
   printf(__PCM__);
#endif

   usb_init();

#if !(USB_PIC16C765)
   printf("\r\nUSBN: 0x%X\r\n", usbn_get_version());
#else
   printf("\r\n");
#ENDIF

   printf("\r\n\r\nWaiting for enumeration...");

   while (TRUE) {
         usb_wait_for_enumeration();
         printf("\r\n\r\n***Enumerated***\r\n");
         while(usb_enumerated()) {
            out_data[0]=buttons;
            out_data[1]=x;
            out_data[2]=y;
            out_data[3]=wheel;

            usb_put_packet(1,out_data,4,TOGGLE);
            
            delay_ms(10);
            count++;
            if (count>=100) {
               count=0;
               switch(x) {
                  case 1:  x=0xFF;
                     break;
                  case 0xFF:   x=1;
                     break;
                  default: x=1;
                     break;
               }
            }
         }
         printf("\r\n\r\nDevice Un-configured.\r\n");
   }
}

