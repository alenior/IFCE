///////////////////////////////////////////////////////////////////////////
////                               EX5.c                               ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit used to demonstrate    ////
////  the debugging capabilities of the CCS Tools.                     ////
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

void main(void)
{
   unsigned int8 a, b, c;
   
   a=11;
   b=5;
   c = a+b;
   c = b-a;
   while(TRUE);
}
