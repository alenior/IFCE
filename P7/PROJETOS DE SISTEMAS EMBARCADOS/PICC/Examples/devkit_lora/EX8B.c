///////////////////////////////////////////////////////////////////////////
////                               EX8B.c                              ////
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
////  This exercise is an example of P2P End Device that sends a       ////
////  message to a P2P Base Station any time the push button is        ////
////  pressed.  Additionally it expects the Base Station to send an    ////
////  acknowledge response back on the same channel.  Since an End     ////
////  Device only listens for messages after it send a message, it's   ////
////  possible that Base Station received a message from another End   ////
////  Device and is listening on a different channel.  This exercise   ////
////  uses the response to determine if the message was sent           ////
////  successfully.  If it didn't receive a response within the time   ////
////  period, it will retry sending the message on the next channel in ////
////  hopping table until it receives the acknowledge response.        ////
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

#include <devkit_lora.h>

// Include a tick timer driver and make appropriate defines, the lora.c driver
// requires a tick timer.
#include <tick.c>
#define TickDifference(a,b)      (a-b)

#define _lora_tick_t                TICK
#define _LORA_TICKS_PER_SECOND      TICKS_PER_SECOND
#define _lora_tick_difference(a,b)  TickDifference(a,b)
#define _lora_tick_get()            TickGet()

// Set device type to an End-Device
#define LORA_DEVICE_TYPE   LORA_DEVICE_TYPE_ED

// Include the LoRa P2P driver's header file, needs to be included before
// rn2903.c so that the required LoRa P2P driver functions in rn2903.c are
// accessible.
#include <lora.h>

// Include the RN2903 module driver and the LoRa P2P driver
#include <rn2903.c>
#include <lora.c>

#define ACK    1
#define NACK   0

void InitHW(void)
{
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(AN_CHANNEL_POT0);
}

void main(void)
{
   int1 Initialized;
   int1 cLevel, pLevel;
   uint16_t Reading;
   int1 Sent = FALSE;
   lora_rx_message_t Message;
   
   //Initialize the LoRa P2P driver, this always need called before any other
   //LoRa P2P functions.  lora_init() also makes necessary calls to initialize
   //the RN2903 module.
   Initialized = lora_init();
   
   if(Initialized)
   {
      output_high(PIN_LED_GREEN);
      
      InitHW();
      
      pLevel = input(PIN_INPUT_BUTTON1);
   }
   else
      output_high(PIN_LED_RED);
   
   while(TRUE)
   {
      if(Initialized)
      {
         //LoRa P2P task function to maintain state machine for sending and 
         //receiving messages.
         lora_task();
         
         //Check if a new message has been received, since this device is setup
         //as an End-Device it can only receive a messages after sending a
         //message.  After sending a message it will listen for LORA_RX_TIME
         //milliseconds.  The time can be adjusted by defining LORA_RX_TIME
         //before the lora.c driver file is include, the default time is 2000
         //milliseconds.
         if(lora_has_message())
         {
            //Retrieve message from software buffer.
            lora_get_message(&Message);
            
            if(Message.Address == LORA_BS_DEVICE_ADDR)
            {
               if(Sent)
               {
                  Sent = FALSE;
                  
                  output_low(PIN_LED_YELLOW);
               
                  if(Message.Data[0] == NACK)
                     output_high(PIN_LED_RED);
               }
            }
         }
      
         if(Sent == FALSE)
         {
            cLevel = input(PIN_INPUT_BUTTON1);
            
            if(cLevel != pLevel)
            {
               delay_us(50);
               
               if(input(PIN_INPUT_BUTTON1) != pLevel)
               {
                  if(cLevel == BUTTON_PRESSED)
                  {
                     output_low(PIN_LED_RED);
                  
                     Reading = read_adc();   //Read current ADC reading of pot A0.
                     
                     //Send ADC reading to base station.
                     if(lora_put_message(LORA_BS_DEVICE_ADDR, (uint8_t *)&Reading, 2))
                     {
                        Sent = TRUE;
                        
                        output_high(PIN_LED_YELLOW);
                     }
                     else
                        output_high(PIN_LED_RED);
                  }
                  
                  pLevel = cLevel;
               }
            }
         }
         else
         {
            //Check if driver is busy sending/receive a message.  If not busy
            //and response wasn't received try resending.  The most likely cause
            //that this if() will evaluate as TRUE is if the channels got out
            //of sync, one of the prototyping boards were powered cycled for
            //example.
            if(lora_busy() == FALSE)
            {
               //Send ADC reading to base station.
               if(lora_put_message(LORA_BS_DEVICE_ADDR, (uint8_t *)&Reading, 2) == FALSE)
               {
                  Sent = FALSE;
                  
                  output_low(PIN_LED_YELLOW);
                  output_high(PIN_LED_RED);
               }
            }
         }
      }
   }
}
