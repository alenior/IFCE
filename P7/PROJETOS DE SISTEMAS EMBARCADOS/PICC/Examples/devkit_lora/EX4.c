///////////////////////////////////////////////////////////////////////////
////                               EX4.c                               ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit that counts how many   ////
////  times a push button has been pressed and shows the count on the  ////
////  LEDs.                                                            ////
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

#define GREEN_LED    PIN_E2
#define YELLOW_LED   PIN_E1
#define RED_LED      PIN_E0
#define PUSH_BUTTON  PIN_D3

void wait_for_one_press(){
   while(input(PUSH_BUTTON));
   while(!input(PUSH_BUTTON));
}

void show_binary_on_leds(int n){
   output_low(GREEN_LED);
   output_low(YELLOW_LED);
   output_low(RED_LED);
   
   if(bit_test(n,0))
      output_high(GREEN_LED);
   if(bit_test(n,1))
      output_high(YELLOW_LED);
   if(bit_test(n,2))
      output_high(RED_LED);
}

void main(){
   int count = 0;
   
   while(TRUE){
      show_binary_on_leds(count);
      wait_for_one_press();
      count++;
   }
}
