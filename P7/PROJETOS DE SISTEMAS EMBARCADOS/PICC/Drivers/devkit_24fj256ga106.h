///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     devkit_24fj256ga106.h                         ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS           ////
//// PIC24FJ256GA106 development kit.  Will compile the program        ////
//// compatible with the UART bootloader shipped on the unit.          ////
//// Bootloader will be disabled if you use an ICD for debugging.      ////
//// Will #import the bootloader HEX so you can use an ICD to          ////
//// reprogram a unit to put the bootloader back on the device.        ////
////                                                                   ////
//// Bootloader uses 19200 baud.  To use bootloader, connect CDC       ////
//// cable to CDC port and run the CCS SIOW software.  In CCS          ////
//// SIOW software, connect to the COM port connected to the           ////
//// hardware's CDC port and use 19200 baud.  Then power-cycle         ////
//// the hardware while holding down the F6 push-button.  In SIOW      ////
//// you should then see the following string:                         ////
////     Bootloader Ready                                              ////
//// Then in SIOW, goto 'Download Software' from the File menu         ////
//// and choose the HEX file you want to program onto the unit.        ////
////                                                                   ////
//// #defining DEVICE_USE_ICD before including this file will add the  ////
//// #device ICD=TRUE line needed to compile with ICD debugging        ////
//// support.                                                          ////
////                                                                   ////
//// #defining __NO_LOADER before including this file will prevent the ////
//// bootloader from being used in the built application.  Normally    ////
//// you will not want to define this.                                 ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __DEVKIT_24FJ256GA106_H__
#define __DEVKIT_24FJ256GA106_H__

   #include <24fj256ga106.h>
   
   // PGC/PGD pins of ICD are connected to the 2nd ICD port of this PIC.
   #device ICSP=2
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif

   #use delay(xtal=20M)
   
   // NOTE: bootloader cannot change the fuses/config-bits!!
   #fuses NOWDT      //WDT is not permanently enabled.  setup_wdt() can be used to turn it on
   #fuses WPOSTS11   //Watch Dog Timer PostScalar 1:1024
   #fuses WDT128     //Watch Dog Timer uses 1:128 Postscale
   #fuses WINDIS     //Watch Dog Timer in non-Window mode
   #fuses NOWRT      //program memory not write protected
   #fuses NOPROTECT  //Code not protected from reading
   #fuses NOJTAG     //JTAG disabled
   #fuses NOIOL1WAY  //Allows multiple reconfigurations of peripheral pins
   #fuses WPBEG      //Flash pages 0 to WPFP are write/erase protected
   #fuses WPFP=0     //A page is 512 instructions.  0x800 bytes is 512 instructions (2048 bytes).  Set this to 1 page.
   #fuses NOWPDIS    //Erase/write-protect active for the selected region(s)
   #fuses WPCFG      //Configuration Words page is erase/write-protected
   
   #define PROGROM_LOADER_SIZE  0x7FF //this maps to the write protected boot-block of this PIC. also, one erase page

   #define PROGROM_LOADER_START  0
   #define PROGROM_LOADER_END    PROGROM_LOADER_SIZE
   #define PROGROM_APP_START     (PROGROM_LOADER_END+1)
   #define PROGROM_ISR_START     (PROGROM_LOADER_END+9)
   #define PROGROM_APP_END 0x2A3FF  //last erase page is blocked out (because of config bits)
   
   #if !defined(__NO_LOADER) && !getenv("ICD")
      #build(reset=PROGROM_APP_START, interrupt=PROGROM_ISR_START)
      
      // prevent application from using space used by bootloader
      #org 0, PROGROM_LOADER_END {}

      #ignore_warnings 228
      #import(HEX, file="bootloader_24fj256ga106.hex", RANGE=PROGROM_LOADER_START:PROGROM_LOADER_END)
      #ignore_warnings NONE
   #else
      #warning Bootloader disabled!  Programming this HEX onto the PIC using an ICD will erase the bootloader!
   #endif
   
   // the last erase page contains the config bits and has been write-protected
   #org PROGROM_APP_END+1, getenv("PROGRAM_MEMORY")-1 {}
   
   // LEDs.
   // set pin high to turn on LED, set pin low to turn off LED.
   #define PIN_LED_RED     PIN_B2
   #define PIN_LED_YELLOW  PIN_B4
   #define PIN_LED_GREEN   PIN_B5
   
   // push-button.
   // the pin will be read low when button is held pressed.
   // the pin will be read high when button is idle (not pressed).
   #define PIN_INPUT_BUTTON   PIN_F6
   
   // UART connections
   #pin_select U1TX = PIN_F3
   #pin_select U1RX = PIN_F2
   #pin_select U2TX = PIN_F5
   #pin_select U2RX = PIN_F4   
   
   // analog channels.
   // you can pass these to set_adc_channel().
   // don't forget setup_adc_ports(sAN0) and setup_adc()
   #define AN_CHANNEL_POT0          0  //PIN_B0
   
unsigned int8 read_adc8(void)
{
   unsigned int16 ret;
   
   ret = read_adc();
  
  #if (getenv("ADC_RESOLUTION")==8)
  #elif (getenv("ADC_RESOLUTION")==10)
   ret /= (unsigned int16)4;
  #elif (getenv("ADC_RESOLUTION")==12)
   ret /= (unsigned int16)16;
  #elif (getenv("ADC_RESOLUTION")==16)
   ret /= (unsigned int16)256;
  #else
   #error huh?
  #endif
  
   return(ret);
}

unsigned int16 read_adc10(void)
{
   unsigned int16 ret;
   
   ret = read_adc();
  
  #if (getenv("ADC_RESOLUTION")==8)
   ret *= (unsigned int16)4;
  #elif (getenv("ADC_RESOLUTION")==10)
  #elif (getenv("ADC_RESOLUTION")==12)
   ret /= (unsigned int16)4;
  #elif (getenv("ADC_RESOLUTION")==16)
   ret /= (unsigned int16)64;
  #else
   #error huh?
  #endif
  
   return(ret);
}

unsigned int16 read_adc12(void)
{
   unsigned int16 ret;
   
   ret = read_adc();
  
  #if (getenv("ADC_RESOLUTION")==8)
   ret *= (unsigned int16)16;
  #elif (getenv("ADC_RESOLUTION")==10)
   ret *= (unsigned int16)4;
  #elif (getenv("ADC_RESOLUTION")==12)
  #elif (getenv("ADC_RESOLUTION")==16)
   ret /= (unsigned int16)16;
  #else
   #error huh?
  #endif
  
   return(ret);
}

unsigned int16 read_adc16(void)
{
   unsigned int16 ret;
   
   ret = read_adc();

  #if (getenv("ADC_RESOLUTION")==8)
   ret /= (unsigned int16)256;
  #elif (getenv("ADC_RESOLUTION")==10)
   ret /= (unsigned int16)64;
  #elif (getenv("ADC_RESOLUTION")==12)
   ret /= (unsigned int16)16;
  #elif (getenv("ADC_RESOLUTION")==16)
  #else
   #error huh?
  #endif

   return(ret);
}

#endif
