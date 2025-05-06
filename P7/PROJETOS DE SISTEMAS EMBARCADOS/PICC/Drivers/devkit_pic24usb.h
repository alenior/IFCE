///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        devkit_pic24usb.h                          ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS PIC24USB  ////
//// development kit (with 24FJ256GB206).  Will compile the program    ////
//// compatible with the USB bootloader.  Bootloader will be disabled  ////
//// if you use an ICD for debugging.  Will #import the bootloader     ////
//// HEX so you can use an ICD to reprogram a unit to put the          ////
//// bootloader back on the device.                                    ////
////                                                                   ////
//// Bootloader uses USB CDC.  To use bootloader, connect USB          ////
//// cable to USB port.  Then, while holding down the F0 push-button,  ////
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
////        (C) Copyright 1996,2015 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __DEVKIT_PIC24USB_H__
#define __DEVKIT_PIC24USB_H__

   #include <24FJ256GB206.h>
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif

   #if defined(DEVICE_USE_ANSI)
      #device ANSI
   #endif

   #if defined(__XC16_COMPAT__)
      #device ANSI

      #device PSV=16
      
      #if defined(__CONST_IS_READONLY__)
         #device CONST=READ_ONLY
      #else
         //this is normally how C30 operates
         #device CONST=ROM
      #endif
      
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
  
   #device ICSP=2
   
   #device ADC=10

   #fuses HS,PR_PLL,NOWDT,PLL5,SOSC_DIG
   #fuses WPOSTS15     //Watch Dog Timer PostScalar 1:16384
   #fuses WDT128       //Watch Dog Timer uses 1:128 Postscale
   #fuses NOWINDIS     //Watch Dog Timer in Window mode
   #fuses NOWRT        //Program memory not write protected
   #fuses NOPROTECT    //Code not protected from reading
   #fuses NOJTAG       //JTAG disabled
   #fuses NOIOL1WAY    //Allows multiple reconfigurations of peripheral pins
   #fuses NOOSCIO      //OSC2 is clock output
   #fuses NOIESO       //Internal External Switch Over mode disabled
   #fuses NOWPCFG      //Configuration Words page is not erase/write-protected
   #use delay(clock=32M)   //20MHz crystal set to run at 32MHz
   
   #define PROGRAM_LOADER_SIZE   0x2000
   #define PROGRAM_LOADER_START  0
   #define PROGRAM_LOADER_END    (PROGRAM_LOADER_SIZE-1)
   #define APPLICATION_START     (PROGRAM_LOADER_SIZE+0)
   #define APPLICATION_ISR       (PROGRAM_LOADER_SIZE+8)
   #define APPLICATION_END       (getenv("PROGRAM_MEMORY")-1)
   
   #if !defined(__NO_LOADER) && !getenv("ICD") && !defined(_bootloader)
      #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR)
      
      #org 0, PROGRAM_LOADER_END {}
      
      #ignore_warnings 228
      #import(HEX, file="bootloader_pic24usb.hex", RANGE=PROGRAM_LOADER_START:PROGRAM_LOADER_END)
      #ignore_warnings NONE
   #endif
   
   #if defined(_bootloader)
      #org APPLICATION_START,APPLICATION_END {}
      
      #int_default
      void _bootloader_isr_redirect(void)
      {
         jump_to_isr(APPLICATION_ISR);
      }
   #endif
   
   // connect PIC's UART1 peripheral to the RS232 driver on the PCB
   #pin_select U1TX = PIN_D5
   #pin_select U1RX = PIN_D4

   // connect PIC's SPI2 peripheral to the SD card connector on the PCB
   #pin_select SDI2 = PIN_F5
   #pin_select SDO2 = PIN_B15
   #pin_select SCK2OUT = PIN_F4
   
   // the other SD card connections on the PCB
   #define PIN_SDIO_CS   PIN_B14
   #define PIN_SDIO_CD   PIN_B12
   
   // ds1305e rtc
   #define RTC_MOSI  PIN_F5
   #define RTC_MISO  PIN_B15
   #define RTC_SCLK  PIN_F4
   #define RTC_RST   PIN_B4
   #define RTC_INT0  PIN_B1

   #define LED1 PIN_B11     //green
   #define LED2 PIN_B10    //yellow
   #define LED3 PIN_B9    //red
   #define PIN_LED_GREEN   LED1
   #define PIN_LED_YELLOW  LED2
   #define PIN_LED_RED     LED3
   #define LEDS_OFF()   LED_OFF(LED1); LED_OFF(LED2); LED_OFF(LED3)
   #define LED_ON(x) output_high(x)
   #define LED_OFF(x) output_low(x)
   
   #define PIN_INPUT_BUTTON0  PIN_F0
   #define BUTTON_PRESSED() (!input(PIN_INPUT_BUTTON0))
   
   // pin_g2 and pin_g3 are wired to USB D+ and D-
   
   #define PIN_5V_CONTROL  PIN_B3
   
   #bit U1OTGSTAT_SESVD=getenv("BIT:SESVD")
   #define USB_CABLE_IS_ATTACHED() (U1OTGSTAT_SESVD)

   #define AN_POT          0  //PIN_B0
   #define AN_5V_FEEDBACK  2  //PIN_B2
   #define HW_ADC_PORTS    sAN0|sAN2   //AN0/PIN_B0=POT, AN2/PIN_B2=VBus
   #define HW_ADC_CHANNEL  0
   #define HW_ADC_CONFIG   ADC_CLOCK_INTERNAL | ADC_TAD_MUL_31

   // LCD connection
   // these definitions are compatible with CCS's lcd.c driver
   #define LCD_ENABLE_PIN  PIN_D0
   #define LCD_RS_PIN      PIN_D1
   #define LCD_RW_PIN      PIN_D2
   #define LCD_DATA4       PIN_E6
   #define LCD_DATA5       PIN_E7
   #define LCD_DATA6       PIN_D6
   #define LCD_DATA7       PIN_D7
   
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
