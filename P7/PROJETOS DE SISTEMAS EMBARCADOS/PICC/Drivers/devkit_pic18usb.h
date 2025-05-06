///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        devkit_pic18usb.h                          ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS PIC18USB  ////
//// development kit (with PIC18F45K50).  Will compile the program     ////
//// compatible with the USB bootloader.  Bootloader will be disabled  ////
//// if you use an ICD for debugging.  Will #import the bootloader     ////
//// HEX so you can use an ICD to reprogram a unit to put the          ////
//// bootloader back on the device.                                    ////
////                                                                   ////
//// Bootloader uses USB CDC.  To use bootloader, connect USB          ////
//// cable to USB port.  Then, while holding down the A4 push-button,  ////
//// press the reset button.  Open the bootloader COM port in SIOW     ////
//// or a serial terminal program, and you should then see the         ////
//// following string:                                                 ////
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
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __DEVKIT_PIC18USB_H__
#define __DEVKIT_PIC18USB_H__

   #include <18f45k50.h>
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif
   
   #if defined(DEVICE_USE_ANSI)
      #device ANSI
   #endif
   
   #if defined(__XC8_COMPAT__)
      #device ANSI
      
      //this is not technically how XC8 operates, but this gets us closest there.
      #device CONST=READ_ONLY
      
      #device PASS_STRINGS=IN_RAM
      
      #type signed
      #type short=16 int=16 long=32
      
      #zero_ram
      #zero_local_ram
      
      #if defined(TRUE)
      #undef TRUE
      #endif
      
      #if defined(FALSE)
      #undef FALSE
      #endif
      
      #if defined(BYTE)
      #undef BYTE
      #endif
      
      #if defined(BOOLEAN)
      #undef BOOLEAN
      #endif
   #endif
   
   #device ADC=10

   #if 1
      // use the external crystal on the board
      #use delay(crystal=16MHz, clock=48MHz, USB_FULL)
   #else
      // use the internal oscillator with active clock tuning
      #use delay(int, clock=48MHz, USB_FULL, act=USB)
   #endif
   
   #fuses WDT_SW         //No Watch Dog Timer, enabled in Software
   #fuses WDT32768       //Watch Dog Timer uses 1:32768 Postscale
   #fuses NOFCMEN        //Fail-safe clock monitor disabled
   #fuses NOIESO         //Internal External Switch Over mode disabled
   #fuses PUT            //Power Up Timer
   #fuses BROWNOUT_SW    //Brownout controlled by configuration bit in special file register
   #fuses BORV19         //Brownout reset at 1.9V
   #fuses NOLPBOR        //Low-Power Brownout reset is disabled
   #fuses CCP2C1         //CCP2 input/output multiplexed with RC1
   #fuses NOPBADEN       //PORTB pins are configured as digital I/O on RESET
   #fuses T3CKB5         //T3 Clock In is on B5
   #fuses SDOB3          //SDO is on RB3
   #fuses MCLR           //Master Clear pin enabled
   #fuses STVREN         //Stack full/underflow will cause reset
   #fuses NOLVP          //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
   #fuses NOXINST        //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
   #fuses ICSP1          //ICD uses PGC1/PGD1 pins
   #fuses NOPROTECT      //Code not protected from reading
   #fuses NOCPB          //No Boot Block code protection
   #fuses NOCPD          //No EE protection
   #fuses NOWRT          //Program memory not write protected
   #fuses WRTC           //Configuration registers write protected
   #fuses NOWRTB         //Boot block not write protected
   #fuses NOWRTD         //Data EEPROM not write protected
   #fuses NOEBTR         //Memory not protected from table reads
   #fuses NOEBTRB        //Boot block not protected from table reads
   
   #define PROGRAM_LOADER_SIZE   0x1800
   #define PROGRAM_LOADER_START  0
   #define PROGRAM_LOADER_END    (PROGRAM_LOADER_SIZE-1)
   #define APPLICATION_START     (PROGRAM_LOADER_SIZE+0)
   #define APPLICATION_ISR       (PROGRAM_LOADER_SIZE+8)
   #define APPLICATION_END       (getenv("PROGRAM_MEMORY")-1)
   
   #if !defined(__NO_LOADER) && !getenv("ICD") && !defined(_bootloader)
      #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR)
      
      #org 0, PROGRAM_LOADER_END {}
      
      #ignore_warnings 228
      #import(HEX, file="bootloader_pic18usb.hex", RANGE=PROGRAM_LOADER_START:PROGRAM_LOADER_END)
      #ignore_warnings NONE
   #endif
   
   #if defined(_bootloader)
      #org APPLICATION_START,APPLICATION_END {}
      
      #int_global
      void _bootloader_isr_redirect(void)
      {
         jump_to_isr(APPLICATION_ISR);
      }
   #endif
   
   // pin_c6 and pin_c7 are tied to RS232 driver

   #define LED1 PIN_A5     //green
   #define LED2 PIN_B4    //yellow
   #define LED3 PIN_B5    //red
   #define PIN_LED_GREEN   LED1
   #define PIN_LED_YELLOW  LED2
   #define PIN_LED_RED     LED3
   #define LEDS_OFF()   LED_OFF(LED1); LED_OFF(LED2); LED_OFF(LED3)
   #define LED_ON(x) output_low(x)
   #define LED_OFF(x) output_high(x)

   #define PIN_USB_SENSE   PIN_B2
   #define USB_CABLE_IS_ATTACHED() input(PIN_USB_SENSE)
   
   #define PIN_INPUT_BUTTON0  PIN_A4
   #define BUTTON_PRESSED() (!input(PIN_INPUT_BUTTON0))
   
   // pin_c4 and pin_c5 are wired to USB D+ and D-

   #define AN_POT          0  //PIN_A0
   #define HW_ADC_PORTS    sAN0   
   #define HW_ADC_CHANNEL  0
   #define HW_ADC_CONFIG   ADC_CLOCK_INTERNAL
   
   // this is called by the bootloader, so it doesn't need to be called again if
   // application is started by the bootloader.
   #define HW_INIT() setup_adc_ports(HW_ADC_PORTS)
   
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
      ret *= (unsigned int16)256;
     #elif (getenv("ADC_RESOLUTION")==10)
      ret *= (unsigned int16)64;
     #elif (getenv("ADC_RESOLUTION")==12)
      ret *= (unsigned int16)16;
     #elif (getenv("ADC_RESOLUTION")==16)
     #else
      #error huh?
     #endif
   
      return(ret);
   }
#endif
