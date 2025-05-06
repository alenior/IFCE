/******************************************************************************

   xc_ezapp_helpers.h
   
   Maps functions and syntax used in the EZ App Lynx library to be compatible 
   with Microchip's XC C compiler's syntax.

*******************************************************************************/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __XC_EZAPP_HELPERS_H__
#define __XC_EZAPP_HELPERS_H__

#define rom const

#if defined(__XC8__)
   //_XTAL_FREQ needs to be defined

   #define restart_wdt CLRWDT
#endif

#if defined(__XC16__) || defined(__XC32__)   
   #define restart_wdt ClrWdt
#endif

#if defined(__XC16__)
   //FCY needs to be defined
   
   #include <libpic30.h>
#endif

#if defined(__XC16__) || defined(__XC8__)
#define __delay_10ms(ms)   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms);   \
      __delay_ms(ms)

#define __delay_100ms(ms)  \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms);   \
      __delay_10ms(ms)

#define delay_ms(ms) \
      __delay_100ms(ms / 100);   \
      __delay_10ms((ms % 100) / 10);  \
      __delay_ms((ms % 100) % 10)

#define delay_us(ms) __delay_us(ms)
#endif

// User must manually set TRIS to 0 before calling these function to init
// the GPIO to output mode.
#define output_high(latbit)   latbit=1
#define output_low(latbit)    latbit=0
#define output_drive(x) //doesn't really work since the CCS C function output_drive()'s param takes the GPIO, and we can't translate that here to the TRIS

// User must manually set TRIS to 1 before calling these functions to init
// the GPIO to input mode.
#define input(portbit)  (portbit)
#define output_float(x) //doesn't really work since the CCS C function output_drive()'s param takes the GPIO, and we can't translate that here to the TRIS

#define bit_test(var,bit)  ((var & (1<<bit)) == (1<<bit))

#endif   //__XC_EZAPP_HELPERS_H__
