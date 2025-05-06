///////////////////////////////////////////////////////////////////////////
////                               EX7A.c                              ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit used to demonstrate    ////
////  using the RN2903 to send a message between two RN2903 modules    ////
////  using LoRa modulation.  This program sends the message.          ////
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
   Optional if defined no serial port is used with this program, instead the
   push button labeled as B4 can be used to transmit the test string.  This is
   primarly provided for case were there is no serial port available for typing
   the message to send into.
*/
//#define NO_SERIAL_PORT

#include <devkit_lora.h>

#include <rn2903.c>

#ifndef NO_SERIAL_PORT
#define STREAM_SERIAL_INPUT   PC_STREAM
#include <input.c>
#endif

int1 init_rn2903_module(void)
{
   int1 Result;
   char vStr[64];
   uint32_t Pause;
   rn2903_ec_t Error;

   RN2903SerialInit();  //initialize the serial interface
   
   RN2903Reset(vStr);
   
   if(vStr[0] != '\0')
   {
      user_printf(user_putc, "\r\nRN2903 FW Version - %s", vStr);
   
      Pause = MACPause();  //pause the LoRaWAN stack so radio commands can be used
      
      if(Pause != 0)
      {
         user_printf(user_putc, "\r\n   MAC Paused for %lu ms", Pause);
         
         Error = RadioSetWDT(0);    //disable radio WDT
         
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio WDT disabled");
            
            Error = RadioSetSyncByte(RN2903_RADIO_SYNC_BYTE);  //set radio synchronization byte
         }
         
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio Sync Byte set to %02X", RN2903_RADIO_SYNC_BYTE);
         
            Error = RadioSetSpreadingFactor(RN2903_RADIO_SF);  //set radio spreading factor
         }
            
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio Spreading Factor set to %u", RADIO_SPREADING_FACTOR_SF7);
            
            Error = RadioSetBandWidth(RN2903_RADIO_BANDWIDTH); //set radio bandwidth
         }
         
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio Bandwidth set to %luK Hz", RN2903_RADIO_BANDWIDTH);
            
            Error = RadioSetCodingRate(RN2903_RADIO_CR); //set radio coding rate
         }
         
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio Coding Rate set to %s", CRToStr(RN2903_RADIO_CR));
            
            Error = RadioSetFrequency(RN2903_RADIO_FREQUENCY); //set radio frequency
         }
         
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio Frequency set to %lu Hz", RN2903_RADIO_FREQUENCY);
            
            Error = RadioSetTxPower(RN2903_RADIO_TX_POWER); //set radio TX power
         }
         
         if(Error == RN2903_EC_OK)
         {
            user_printf(user_putc, "\r\n   Radio TX Power set to %u dBm", RN2903_RADIO_TX_POWER);
            
            Result = TRUE;
         }
         else
            Result = FALSE;
      }
      else
         Result = FALSE;
      
      user_printf(user_putc, "\r\n");
   }
   else
      Result = FALSE;
   
   return(Result);
}

void main(void)
{
   char Str[MAX_STRING_LENGTH + 1];
  #ifdef NO_SERIAL_PORT
   int1 cLevel, pLevel;
   uint16_t Count = 0;
  #endif
   int1 Initilized;
   rn2903_ec_t Error;
   rn2903_resp_t Response;
   uint8_t respStr[64];
   uint8_t respLength;
   
   delay_ms(10);
   
   user_printf(user_putc, "\r\nEX7A.c - %s\r\n", getenv("DEVICE"));
   
   Initilized = init_rn2903_module();  //initialize serial interface and RN2903 module
   
   if(Initilized)
      output_high(PIN_LED_GREEN);
   else
   {
      user_printf(user_putc, "\r\nRN2903 Module failed to initialize, try power cycling board.");
      
      output_high(PIN_LED_RED);
   }
   
  #ifdef NO_SERIAL_PORT
   pLevel = input(PIN_INPUT_BUTTON1);
  #endif
      
   while(TRUE)
   {
      if(Initilized)
      {
        #ifndef NO_SERIAL_PORT
         user_printf(user_putc, "\r\nEnter String to Send: ");
         
         get_string(Str, MAX_STRING_LENGTH);    //get string to send
         
         user_printf(user_putc, "\r\n");
        #else
         cLevel = input(PIN_INPUT_BUTTON1);
         
         if(cLevel != pLevel)
         {
            delay_us(50);
            
            if(input(PIN_INPUT_BUTTON1) != pLevel)
            {
               if(cLevel == BUTTON_PRESSED)
               {
                  sprintf(Str, "Current Count: %lu", Count++);
                  
                  output_high(PIN_LED_YELLOW);
        #endif
         
                  Error = RadioTxData(Str, strlen(Str));    //send string
                  
                  if(Error == RN2903_EC_OK)
                  {
                     Response = RN2903_RESP_NONE;
                  
                     do
                     {
                        RN2903SerialRxTask();   //serial RX task to receive response from RN2903 module
                        
                        if(RN2903SerialKbhit()) //check for new response
                        {
                           Response = RN2903SerialGetResponse(respStr, &respLength);   //get response
                        }
                     
                     } while(Response == RN2903_RESP_NONE);
                     
                     if(Response != RN2903_RESP_RADIO_TX_OK)
                        Error = RN2903_EC_UNKNOWN;
                  }
        #ifndef NO_SERIAL_PORT
        
                  if(Error == RN2903_EC_OK)
                     user_printf(user_putc, "\r\nMessage Sent Successfully\r\n");
                  else
                     user_printf(user_putc, "\r\nError Occurred Sending Message\r\n");
        #else
                  output_low(PIN_LED_YELLOW);
               }
               
               pLevel = cLevel;
            }
         }
        #endif
      }
   }
}
