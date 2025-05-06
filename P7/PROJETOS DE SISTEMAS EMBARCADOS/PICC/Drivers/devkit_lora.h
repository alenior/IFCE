///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                           devkit_lora.h                           ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS LoRa      ////
//// LoRaWAN development kit.  Will compile the program compatible     ////
//// with the UART2 bootloader shipped on the unit.  Bootloader        ////
//// will be disabled if you use an ICD for debugging.  Will #import   ////
//// the bootloader HEX so you can use an ICD to reprogram a unit to   ////
//// put the bootloader back on the device.                            ////
////                                                                   ////
//// Bootloader uses 19200 baud.  To use bootloader, connect CPC       ////
//// cable to D6/D7 port and run the CCS SIOW software.  In CCS        ////
//// SIOW software, connect to the COM port connected to the           ////
//// hardware's D6/D7 port and use 19200 baud.  Then power-cycle       ////
//// the hardware while holding down the B4 push-button.  In SIOW      ////
//// you should then see the following string:                         ////
////     Bootloader Version 1.0                                        ////
////     Waiting for download...                                       ////
//// Then in SIOW, goto 'Download Software' from the File menu         ////
//// and choose the HEX file you want to program onto the unit.        ////
////                                                                   ////
//// #defining DEVICE_USE_ICD before including this file will add the  ////
//// #device ICD=TRUE line needed to compile with ICD debugging        ////
//// support.                                                          ////
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

#ifndef __DEVKIT_LORA_H__
#define __DEVKIT_LORA_H__

   #include <18F45K22.h>
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif
   
   #device ADC=10

   // Datasheet shows 2% error at 16mhz, any other values aren't documented.
   #use delay(internal=16MHz)
   
   // NOTE: bootloader cannot change the fuses/config-bits!!
   #fuses NOWDT      //WDT is not permanently enabled.  setup_wdt() can be used to turn it on
   #fuses WDT1024    //WDT perdiod (no prescalar) ~4.1ms.  1024*4.1ms = ~4s period
   #fuses NOWRT      //program memory not write protected
   #fuses WRTB       //write protect boot-block
   #fuses WRTC       //write protect config bits
   #fuses NOWRTD     //data EEPROM not write protected
   
   #define PROGROM_LOADER_SIZE  0x7FF //this maps to the write protected boot-block of this PIC
   
   #define PROGROM_LOADER_START  0
   #define PROGROM_LOADER_END    PROGROM_LOADER_SIZE
   #define PROGROM_APP_START     (PROGROM_LOADER_END+1)
   #define PROGROM_ISR_START     (PROGROM_LOADER_END+9)
   
   #if !defined(__NO_LOADER) && !getenv("ICD")
      #build(reset=PROGROM_APP_START, interrupt=PROGROM_ISR_START)
      
      #org 0, PROGROM_LOADER_END {}

      #import(HEX, file="bootloader_ezapp.hex", RANGE=PROGROM_LOADER_START:PROGROM_LOADER_END)
   #else
      #warning Bootloader disabled!  Programming this HEX onto the PIC using an ICD will erase the bootloader!
   #endif
   
   // UART1 connected to PDIP daughterboard socket.
   #define RN2903_TX_PIN               PIN_C6
   #define RN2903_RX_PIN               PIN_C7
   //#define RN2903_SERIAL_BUFFER_SIZE   0
   
   #ifdef NO_SERIAL_PORT
      #define user_printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
      #define user_putc(c)
      #define user_getc()
      #define user_kbhit()
   #else
      #ifndef USE_ICD_FOR_SERIAL
         // UART2 is connected to an RS232 level converter.
         #use rs232(UART2, baud=19200, stream=PC_STREAM)
      #else
         #use rs232(ICD, baud=19200, stream=PC_STREAM, NODELAY)
      #endif
      
      #define user_printf     printf
      
      void user_putc(char c)
      {
         fputc(c, PC_STREAM);
      }
      
      #define user_getc()     fgetc(PC_STREAM)
      #define user_kbhit()    kbhit(PC_STREAM)
  #endif
   
   // Small LEDs.
   // set pin high to turn on LED, set pin low to turn off LED.
   #define PIN_LED_RED     PIN_E0
   #define PIN_LED_YELLOW  PIN_E1
   #define PIN_LED_GREEN   PIN_E2
   
   // BIG LED.
   #define PIN_LED_BIG_RED    PIN_C1
   
   // Relays.
   #define PIN_OUTPUT_RELAY0  PIN_D2
   #define PIN_OUTPUT_RELAY1  PIN_D5
   
   // push-buttons.
   // the pin will be read low when button is held pressed.
   // the pin will be read high when button is idle (not pressed).
   #define PIN_INPUT_BUTTON0  PIN_D3
   #define PIN_INPUT_BUTTON1  PIN_B4
   
   #define BUTTON_PRESSED        0
   #define BUTTON_NOT_PRESSED    1

   // analog channels.
   // you can pass these to read_adc.
   // don't forget setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3) and setup_adc()
   #define AN_CHANNEL_POT0          0  //PIN_A0
   #define AN_CHANNEL_POT1          1  //PIN_A1
   #define AN_CHANNEL_PHOTO         2  //PIN_A2 - photo diode
   #define AN_CHANNEL_THERMISTOR    3  //PIN_A3
   
   //RN2903 LoRa Radio Settings
   #define RN2903_RADIO_SYNC_BYTE      0x66
   #define RN2903_RADIO_SF             RADIO_SPREADING_FACTOR_SF7
   #define RN2903_RADIO_CR             RADIO_CODING_RATE_4BY5
   #define RN2903_RADIO_BANDWIDTH      RADIO_BANDWIDTH_125K
   #define RN2903_RADIO_FREQUENCY      923300000
   #define RN2903_RADIO_TX_POWER       2
   
   #define MAX_STRING_LENGTH           128
   
#endif
