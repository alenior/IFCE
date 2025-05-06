/////////////////////////////////////////////////////////////////////////
////                         EX_COMP.C                               ////
////                                                                 ////
////  This example demonstartes the use of the built in comparator.  ////
////  The program compares the input voltage with the internal       ////
////  reference voltage.  Turn pot #9 to change the voltage.         ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     Connect pin 16 to pin 27.                                   ////
////     Connect pin 9 to pin 15.                                    ////
////     See additional connections below.                           ////
////                                                                 ////
////  NOTE: Make sure the #9 pot is turned all the way counter clock ////
////  wise before starting the program.                              ////
////                                                                 ////
////  This example will work with the PCM compiler.  The following   ////
////  conditional compilation lines are used to include a valid      ////
////  device for each compiler.  Change the device, clock and RS232  ////
////  pins for your hardware if needed.                              ////
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
#include <12F675.h>
#fuses HS,WDT,NOPROTECT
#use delay(clock=20000000)
#use rs232(baud=9200, xmit=PIN_A3, rcv=PIN_A4)
#endif

short safe_conditions=TRUE;

#INT_COMP
void isr()  {

   safe_conditions=FALSE;
   printf("WARNING!!  Voltage level is above 3.6 V.   \r\n");
}


main()   {

   printf("\r\nRunning voltage test...\r\n\n");

   setup_comparator(A1_VR_OUT_ON_A2);
   setup_vref(VREF_HIGH|15);
   enable_interrupts(INT_COMP);
   enable_interrupts(GLOBAL);


   while(TRUE)
   {
      if(safe_conditions)
         printf("Voltage level is below 3.6 V.              \r\n");
      safe_conditions=TRUE;
      delay_ms(500);
   }
}
