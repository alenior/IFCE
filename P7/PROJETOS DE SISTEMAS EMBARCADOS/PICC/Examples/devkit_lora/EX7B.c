///////////////////////////////////////////////////////////////////////////
////                               EX7B.c                              ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit used to demonstrate    ////
////  using the RN2903 to send a message between two RN2903 modules    ////
////  using LoRa modulation.  This program receives the message.       ////
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
//#define  USE_ICD_FOR_SERIAL

#include <devkit_lora.h>

#include <rn2903.c>

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
   int1 Initilized;
   int1 Listening = FALSE;
   rn2903_ec_t Error;
   rn2903_resp_t Response;
   unsigned char respStr[128];
   uint8_t respLength;
   
   delay_ms(10);
   
   user_printf(user_putc, "\r\nEX7B.c - %s\r\n", getenv("DEVICE"));
   
   Initilized = init_rn2903_module();  //initialize serial interface and RN2903 module
   
   if(Initilized)
      output_high(PIN_LED_GREEN);
   else
   {
      user_printf(user_putc, "\r\nRN2903 Module failed to initialize, try power cycling board.");
      
      output_high(PIN_LED_RED);
   }
   
   while(TRUE)
   {
      if(Initilized)
      {
         if(Listening == FALSE)
         {
            Error = RadioStartRx(0);   //Start listen for a message, passing 0 causes it to listen until it receive a message
            
            if(Error == RN2903_EC_OK)
               Listening = TRUE;
         }
         else
         {
            RN2903SerialRxTask();      //serial RX task to receive response from RN2903 module
            
            if(RN2903SerialKbhit())    //check for new response from RN2903 modlue
            {
               Response = RN2903SerialGetResponse(respStr, &respLength);
               
               if((Response == RN2903_RESP_RADIO_RX) && (respLength > 0))
               {
                  respStr[respLength] = '\0'; //make sure string is null terminated
                  
                  user_printf(user_putc, "\r\nReceived Message - %s", respStr);
               }
               
               Listening = FALSE;
            }
         }
      }
   }
}
