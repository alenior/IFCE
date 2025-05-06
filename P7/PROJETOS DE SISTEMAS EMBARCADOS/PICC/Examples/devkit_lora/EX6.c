///////////////////////////////////////////////////////////////////////////
////                               EX6.c                               ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit used to demonstrate    ////
////  communicating with the RN2903 module.                            ////
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

void main(void)
{
   char vStr[50];
   
   RN2903SerialInit();  //initialize the RN2903 serial interface
   
   RN2903Version(vStr); //read the RN2903 modules FW version
   
   if(vStr[0] != '\0')
   {
      user_printf(user_putc, "\r\nRN2903 FW Version - %s\r\n", vStr);
      output_high(PIN_LED_GREEN);   //turn Green LED on indicating that version was read from RN2903 module
      
   }
   else
      output_high(PIN_LED_RED);     //turn Red LED on indicating that version wasn't read from RN2903 module
   
   while(TRUE)
   {
   }
}
