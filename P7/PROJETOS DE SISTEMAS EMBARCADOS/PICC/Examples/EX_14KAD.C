/////////////////////////////////////////////////////////////////////////
////                          EX_14KAD.C                             ////
////                                                                 ////
////  This program displays the filtered and calibrated A/D samples  ////
////  from any analog input pin (chosen by user) over the RS-232     ////
////  interface.  The process is repeated forever.                   ////
////                                                                 ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////                                                                 ////
////     Connect 0.033 uF capacitor between 48 and 27.               ////
////     Use the #9 POT to vary the voltage.                         ////
////                                                                 ////
////     For A/D sampling, insert a jumper from 9 to:                ////
////               15       for        RA0/AN0                       ////
////               16                  RA1/AN1                       ////
////               17                  RA2/AN2                       ////
////               18                  RA3/AN3                       ////
////                1                  RC0/REFA                      ////
////               53                  RD3/REFB                      ////
////               36                  RD4/AN4                       ////
////               37                  RD5/AN5                       ////
////               38                  RD6/AN6                       ////
////               39                  RD7/AN7                       ////
////                                                                 ////
////     See additional connections below.                           ////
////                                                                 ////
////  This example will work with the PCM compiler.  The following   ////
////  conditional compilation lines are used to include a valid      ////
////  device for the compiler.  Change the device, clock and RS232   ////
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
#include <14000.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // Jumpers: 8 to 11, 7 to 12
//#include <cal9717n.h>  // Put your calibration constants here
#include <14kcal.c>
#endif


void main() {

   int source;
   float res;

   setup_adc_ports(ALL_ANALOG);
   setup_adc(CURRENT_34);

   delay_ms(2000);

   do {
      printf("\r\nChoose analog input channel:\r\n");
      printf("\r\n0) RA0/AN0  1) RA1/AN1   2) RA2/AN2  3) RA3/AN3");
      printf("\r\n7) IntTemp  8) RC0/REFA  9) RD3/REFB");
      printf("\r\nA) RD4/AN4  B) RD5/AN5   C) RD6/AN6  D) RD7/AN7");

      source = toupper(getc());
      if (source >= 'A')
         source = source - 'A' + 10;
      else
         source = source - '0';

      res = READ_CALIBRATED_AD(source);
      printf("\r\n\nCalibrated A/D = %E\n", res);

   } while(TRUE);
}
