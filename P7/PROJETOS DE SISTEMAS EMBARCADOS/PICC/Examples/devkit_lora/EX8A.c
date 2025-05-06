///////////////////////////////////////////////////////////////////////////
////                               EX8A.c                              ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit that uses CCS's LoRa   ////
////  peer-to-peer (P2P) driver to send message's between devices.     ////
////                                                                   ////
////  The CCS LoRa P2P driver was developed by CCS and doesn't         ////
////  follow any industry standard.  Each device has a unique 8 bit    ////
////  address that is used to determine which device sent and receives ////
////  the message.  The driver uses a predetermined channel hopping    ////
////  table for determine which channels/frequencies are used and the  ////
////  order they are used in.  Additionally the driver allows          ////
////  assigning the device to one of three types, a Base Station, an   ////
////  End Device or a Listening End Devices.                           ////
////                                                                   ////
////  This exercise is an example of a P2P Base Station.  Which        ////
////  receives a message from an End Device containing an ADC reading  ////
////  that it then uses to set the duty cycle of LED.  Additionally it ////
////  sends an acknowledge message back to the End Device that         ////
////  initiated the message.                                           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

/*
   Optional if defined it uses the ICD for the PC serial communication for this
   example.  If this is defined then the program should be programmed with the
   ICD because after program it auto launches the Serial Input/Output Monitor 
   program setup correctly for communicating through the ICD.
*/
//#define USE_ICD_FOR_SERIAL

#include <devkit_lora.h>

// Include a tick timer driver and make appropriate defines, the lora.c driver
// requires a tick timer.
#include <tick.c>
#define TickDifference(a,b)      (a-b)

#define _lora_tick_t                TICK
#define _LORA_TICKS_PER_SECOND      TICKS_PER_SECOND
#define _lora_tick_difference(a,b)  TickDifference(a,b)
#define _lora_tick_get()            TickGet()

// Set device type to a Base Station
#define LORA_DEVICE_TYPE   LORA_DEVICE_TYPE_BS

// Include the LoRa P2P driver's header file, needs to be included before
// rn2903.c so that the required LoRa P2P driver functions in rn2903.c are
// accessible.
#include <lora.h>

// Include the RN2903 module driver and the LoRa P2P driver
#include <rn2903.c>
#include <lora.c>

#define ACK    1
#define NACK   0

// Setup PWM on Big Red LED, CCP2, to output ~1 kHz frequency with 10 bit
// resolution, initialize duty to 0%.
#use pwm(output=PIN_LED_BIG_RED, frequency=1kHz, bits=10, duty=0)

void main(void)
{
   lora_rx_message_t Message;
   uint8_t txData[1] = {ACK};
   uint16_t Reading;
   int1 Initilized;
   
   user_printf(user_putc, "\r\nEX8A.c - %s\r\n", getenv("DEVICE"));
   
   //Initialize the LoRa P2P driver, this always need called before any other
   //LoRa P2P functions.  lora_init() also makes necessary calls to initialize
   //the RN2903 module.
   Initilized = lora_init();
   
   if(Initilized)
      output_high(PIN_LED_GREEN);
   else
   {
      output_high(PIN_LED_RED);
      
      user_printf(user_putc, "\r\nRN2903 Module failed to initialize, try power cycling board.");
   }
      

   while(TRUE)
   {
      if(Initilized)
      {
         //LoRa P2P task function to maintain state machine for sending and 
         //receiving messages.
         lora_task();
         
         //Check if a new message has been received, since this device is setup
         //as a base station it is always listen for a message.  The only 
         //exception to this after it receive a message meant for it.  In that
         //case it will wait for LORA_RX_TIME milliseconds for a message to
         //send to be loaded into it's TX buffer, during that time it's not
         //listening for message.
         if(lora_has_message())
         {
            //Retrieve message from software buffer.
            lora_get_message(&Message);
            
            if(Message.Length == 2)
            {
               Reading = make16(Message.Data[1], Message.Data[0]);
            
               user_printf(user_putc, "\r\nReceived ADC Reading: %04LX", Reading);
               
               //Set PWM duty to the ADC reading received from end-device.
               pwm_set_duty(Reading);
            }
            
            //Load ACK into TX buffer to send as a response.
            lora_put_message(Message.Address, txData, 1);
         }
      }
   }
}
