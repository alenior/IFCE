///////////////////////////////////////////////////////////////////////////
////                             sky13418.c                            ////
////                                                                   ////
////               Driver for a SKY13418 Antenna Switch.               ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// API:                                                              ////
////                                                                   ////
////  init_antenna_switch() - initializes antenna switch and sets it   ////
////                          to connect to antenna 1, call before any ////
////                          other functions.                         ////
////                                                                   ////
////  select_antenna() - sets the antenna that is selected (1-8).      ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Defines:                                                          ////
////                                                                   ////
////  SKY_V1_PIN - sets the PIC pin connected to the V1 pin of switch, ////
////               defaults to PIN_B0 is not defined.                  ////
////                                                                   ////
////  SKY_V2_PIN - sets the PIC pin connected to the V2 pin of switch, ////
////               defaults to PIN_B1 is not defined.                  ////
////                                                                   ////
////  SKY_V3_PIN - sets the PIC pin connected to the V3 pin of switch, ////
////               defaults to PIN_B2 is not defined.                  ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2019 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                       http://www.ccsinfo.com                      ////
///////////////////////////////////////////////////////////////////////////

#ifndef __SKY13418_C__
#define __SKY13418_C__

#include <stdint.h>

#ifndef SKY_V1_PIN
 #define SKY_V1_PIN     PIN_B0
#endif

#ifndef SKY_V2_PIN
 #define SKY_V2_PIN     PIN_B1
#endif

#ifndef SKY_V3_PIN
 #define SKY_V3_PIN     PIN_B2
#endif

///////////////////////////////////////////////////////////////////////////////
// init_antenna_switch()
//
// Initializes the antenna switch pins and sets it to connect to antennal 1.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void init_antenna_switch(void)
{
   output_low(SKY_V1_PIN);
   output_low(SKY_V2_PIN);
   output_low(SKY_V3_PIN);
   
   output_drive(SKY_V1_PIN);
   output_drive(SKY_V2_PIN);
   output_drive(SKY_V3_PIN);
}

///////////////////////////////////////////////////////////////////////////////
// select_antenna()
//
// Sets the antenna that is selected.
//
// Parameters:
//    Antenna - The antenna that is selected, i.e. which antenna is connected
//              to ANT pin of switch.  Can be 1-8 for antennas 1 to 8.
//
// Returns - TRUE is antenna was switched, FALSE is antenna wasn't switch.
///////////////////////////////////////////////////////////////////////////////
int1 select_antenna(uint8_t Antenna)
{
   int1 Result = TRUE;
   
   switch(Antenna)
   {
      case 1:
         output_low(SKY_V1_PIN);
         output_low(SKY_V2_PIN);
         output_low(SKY_V3_PIN);
         break;
      case 2:
         output_low(SKY_V1_PIN);
         output_low(SKY_V2_PIN);
         output_high(SKY_V3_PIN);
         break;
      case 3:
         output_low(SKY_V1_PIN);
         output_high(SKY_V2_PIN);
         output_low(SKY_V3_PIN);
         break;
      case 4:
         output_low(SKY_V1_PIN);
         output_high(SKY_V2_PIN);
         output_high(SKY_V3_PIN);
         break;
      case 5:
         output_high(SKY_V1_PIN);
         output_low(SKY_V2_PIN);
         output_low(SKY_V3_PIN);
         break;
      case 6:
         output_high(SKY_V1_PIN);
         output_low(SKY_V2_PIN);
         output_high(SKY_V3_PIN);
         break;
      case 7:
         output_high(SKY_V1_PIN);
         output_high(SKY_V2_PIN);
         output_low(SKY_V3_PIN);
         break;
      case 8:
         output_high(SKY_V1_PIN);
         output_high(SKY_V2_PIN);
         output_high(SKY_V3_PIN);
         break;
      default:
         Result = FALSE;
         break;
   }
   
   delay_us(5);
   
   return(Result);
}

#endif
