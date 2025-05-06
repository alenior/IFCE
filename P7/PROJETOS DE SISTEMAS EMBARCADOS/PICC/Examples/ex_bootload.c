///////////////////////////////////////////////////////////////////////////
////                      EX_BOOTLOAD.C                                ////
////                                                                   ////
////  This program is an example application program that can be       ////
////  be loaded, recompiled and reloaded by a stand alone bootloader.  ////
////                                                                   ////
////  Before loading this program the bootloader (ex_bootloader.c)     ////
////  must be compiled and loaded into the target chip.                ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load this HEX file ////
////  into the target chip.  See ex_bootloader.c for more information. ////
////                                                                   ////
////  This example will work with the PCM and PCH compilers.  The      ////
////  following conditional compilation lines are used to include a    ////
////  valid device for each compiler.  Change the device, clock and    ////
////  RS232 pins for your hardware if needed.                          ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2004 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////


#if defined(__PCM__)
#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // Jumpers: 8 to 11, 7 to 12

#elif defined(__PCH__)
#include <18F452.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // Jumpers: 8 to 11, 7 to 12
#endif

#include <bootloader.h>

void main(void) {
   int i=0;

   printf("\r\nApplication program version 1.00 \r\n");

   while(TRUE)
     printf("%u ",++i);

}
