/////////////////////////////////////////////////////////////////////////
////                        ex_usb_scope.c                           ////
////                                                                 ////
//// This example shows how to develop a USB bulk device, and is     ////
//// accompanied by an example Windows program (oscope.exe) and      ////
//// an example Windows USB Bulk driver (usbdemo.sys).  This example ////
//// will run on CCS's USB Prototype board.                          ////
////                                                                 ////
//// As of this writing, this code only supports Nationals USB960x   ////
//// full speed USB peripheral.                                      ////
////                                                                 ////
//// When the PIC with USBN960x is connected to the PC, an install   ////
//// driver wizard should pop-up in Windows.  Install the            ////
//// provided usbdemo.sys driver.  You may have to reboot the        ////
//// machine.  Once installed, you can use oscope.exe to see the     ////
//// data from the PIC.                                              ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.         ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F877.h>
#device *=16 //You must use 16bit pointers or everything will fall apart.
#fuses HS,NOWDT,NOPROTECT,NOLVP

#elif defined(__PCH__)
#include <18F452.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#endif

#use delay(clock=20000000) //when using national demo board
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#DEFINE LED_1  PIN_B5
#DEFINE LED_2  PIN_B4
#DEFINE LED_3  PIN_B3

//The following defines are used to configure the CCS USB PIC driver
#DEFINE USB_HID_DEVICE     FALSE 
#define USB_EP1_TX_ENABLE  1  //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  1  //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_RX_SIZE    2   //host will only send us 8 bytes, but max_packet size for bulk must be x8,x10,x18,x20,etc

#include "usbn960x.c"   //National 960x hardware layer for usb.c
#include "usb.c"        //handles usb setup tokens and get descriptor reports

#DEFINE OSCDEMO_MESSAGE_SIZE 512

#define SINE_TABLE_SIZE 256

const char sine_table[SINE_TABLE_SIZE]={
   127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169,
   172, 175, 178, 181, 184, 186, 189, 192, 195, 197, 200, 202, 205, 207, 210,
   212, 214, 216, 219, 221, 223, 225, 227, 229, 230, 232, 234, 236, 237, 239,
   240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252, 253,
   253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250,
   249, 248, 247, 246, 245, 244, 242, 241, 240, 238, 237, 235, 234, 232, 230,
   228, 226, 224, 222, 220, 218, 216, 214, 211, 209, 207, 204, 202, 199, 197,
   194, 191, 189, 186, 183, 180, 178, 175, 172, 169, 166, 163, 160, 157, 154,
   151, 148, 145, 142, 138, 135, 132, 129, 127, 124, 121, 118, 114, 111, 108,
   105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 73, 70, 67, 64,
   62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 37, 35, 33, 31, 29,
   27, 25, 23, 22, 20, 18, 17, 15, 14, 12, 11, 10, 9, 8, 7,
   6, 5, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9, 11, 12,
   13, 14, 16, 17, 19, 21, 22, 24, 26, 28, 30, 32, 34, 36, 38,
   41, 43, 45, 48, 50, 53, 55, 58, 60, 63, 66, 69, 71, 74, 77,
   80, 83, 86, 89, 92, 95, 98, 101, 104, 107, 110, 113, 116, 119, 122, 125
};

void main() {
   int8 rxdata[2];
   int8 packet[USB_MAX_PACKET_LENGTH], packet_next=0, packet_count=0, sample_rate, threshold,data;
   int16 sine_next=0;
   int1 run=0, overthresh=0;

   output_low(LED_1);

   usb_init();

   printf("\r\nUSBN: 0x%X", usbn_get_version());

   while (TRUE) {
      output_high(LED_2);
      printf("\r\n\r\nWaiting for enumeration...\r\n");

      while(!usb_enumerated()) {}
      
      output_low(LED_2);
      printf("\r\n***Enumerated***\r\n");

      while(usb_enumerated()) {
         if (run) {
            data=sine_table[sine_next];
            sine_next+=sample_rate;
            if (sine_next >= SINE_TABLE_SIZE) {sine_next-=SINE_TABLE_SIZE;}
            if ((data > threshold) || overthresh) {
               if (!overthresh) {printf("\r\nSending packet #: ");}
               overthresh=1;
               packet[packet_next++]=data;
               if (packet_next >= USB_MAX_PACKET_LENGTH) {
                  packet_count++;
                  packet_next=0;
                  usb_put_packet(1,packet,USB_MAX_PACKET_LENGTH, TOGGLE);
                  printf("%U ",packet_count);
                  if (packet_count >= (OSCDEMO_MESSAGE_SIZE / USB_MAX_PACKET_LENGTH)) {
                     packet_count=0;
                     overthresh=0;
                     usb_put_packet(1,0,0,TOGGLE); //send a 0len (end of message marker) packet
                     printf("\r\n    Message completed");
                     delay_ms(1000);
                  }
               }
            }
         }
         if (usb_kbhit(1)) {
            usb_gets(1,rxdata,2);
            threshold=rxdata[0];
            sample_rate=rxdata[1] + 1;
            printf("\r\n--> Received 2 bytes: Thresh=%U Delay=%U",threshold,sample_rate);
            run=1;
            output_low(LED_3);
         }
      }
      printf("\r\n\r\nDevice un-configured.\r\n");
   }
}




