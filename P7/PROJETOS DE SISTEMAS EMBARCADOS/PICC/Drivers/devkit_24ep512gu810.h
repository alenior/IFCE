///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     devkit_24ep512gu810.h                         ////
////                                                                   ////
//// Header file containing relevant definitions for the CCS           ////
//// PIC24EP512GU810 development kit.  Will compile the program        ////
//// compatible with the USB CDC bootloader shipped on the unit.       ////
//// Bootloader will be disabled if you use an ICD for debugging.      ////
//// Will #import the bootloader HEX so you can use an ICD to          ////
//// reprogram a unit to put the bootloader back on the device.        ////
////                                                                   ////
//// To use bootloader, connect USB cable to USB port and run the      ////
//// CCS SIOW software.  In CCS SIOW software, connect to the COM      ////
//// port of the assigned to the USB CDC device.  Then power-cycle     ////
//// the hardware while the USB cable is connected to the device       ////
//// and while touching the development kits touch screen.  In SIOW    ////
//// you should then see the following string:                         ////
////     Bootloader Ready                                              ////
//// Additionally the development kit screen's backlight will turn on  ////
//// and the screen will be solid red.  Then in SIOW, goto 'Download   ////
//// Software' from the File menu and choose the HEX file you want to  ////
//// program onto the unit.                                            ////
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

#ifndef __DEVKIT_24EP512GU810_H__
#define __DEVKIT_24EP512GU810_H__

   #define TOUCH_USE_HW_I2C
   
   #include <24EP512GU810.h>
   
   #device ICSP=2
   #build(stack=1024)
   
   #if defined(DEVICE_USE_ICD)
      #device ICD=TRUE
   #endif
   
   #if !defined(__NO_LOADER) && !getenv("ICD")
      #define USB_AUX_SERIAL_BOOTLOADER
      
      #define LOADER_SIZE           0  //Because this bootloader resides in Auxiliary Memory Segment
      #define APPLICATION_START     0
      #define APPLICATION_END       (getenv("PROGRAM_MEMORY") - 2)
      #define PROGRAM_LOADER_START  0x7FC000
      #define PROGRAM_LOADER_END    0x7FFFFE
   
      #ifdef _bootloader
         #build (AUX_MEMORY)
      #endif
      
      #fuses RESET_AUX     //Device will reset to Auxiliary Flash Reset Vector
   #else
      #fuses RESET_PRIMARY //Device will reset to Primary Flash Reset Vector
   #endif
   
   // NOTE: bootloader cannot change the fuses/config-bits!!
   #fuses NOJTAG        //JTAG disabled
   #fuses WPOSTS11      //Watch Dog Timer PostScalar 1:1024
   #fuses WPRES128      //Watch Dog Timer PreScalar 1:128
   #fuses NOWDT         //No Watch Dog Timer.  can still be enabled with setup_wdt()
   #fuses OSCIO         //OSC2 is general purpose output
   #fuses NOBROWNOUT    //No brownout reset
   #fuses NOIOL1WAY     //Allows multiple reconfigurations of peripheral pins
   #fuses NOWRT         //Program memory not write protected
   #fuses NOPROTECT     //Code not protected from reading
   #fuses NOGSSK        //General segment key bits, use if using both NOWRT and NOPROTECT fuses
   #fuses AWRT          //Auxiliary program memory is write protected
   #fuses NOAPROTECT    //Auxiliary program memory is not protected from reading
   #fuses APLK          //Auxiliary segment key bits, use if using either AWRT or APROTECT fuses
   #ifdef TOUCH_USE_HW_I2C
      #fuses ALTI2C2    //I2C2 mapped to ASDA2/ASCL2 pins
   #endif
   
   #use delay(clock=120MHz, oscillator=8MHz, PLL_WAIT, AUX:clock=48MHz)
   
   #if !defined(__NO_LOADER) && !getenv("ICD") && !defined(_bootloader)
      #ignore_warnings 228
      #import(HEX, file="bootloader_24ep512gu810.hex", RANGE=PROGRAM_LOADER_START:PROGRAM_LOADER_END)
      #ignore_warnings NONE
   #endif
   
   #ignore_warnings 216
   
   #case
   
   #include <stdint.h>
   #include <tick.h>
   
   #use fast_io(all)
   
   #ifndef GLCD_PIXELS
      #define GLCD_PIXELS        480
   #endif
   
   #ifndef GLCD_LINES
      #define GLCD_LINES         272
   #endif
   
   //Graphic Controller Defines
   #define GLCD_H_PERIOD         525
   #define GLCD_H_SYNC_WIDTH     1
   #define GLCD_H_FRONT_PORCH    5
   
   #define GLCD_V_PERIOD         288
   #define GLCD_V_SYNC_WIDTH     1
   #define GLCD_V_FRONT_PORCH    8
   
   #define GLCD_SRAM_SIZE        0
   #define GLCD_SRAM_BUFFER_SIZE 0x20
   
   #define GLCD_CLK_D_DIVIDER    8        // 8 MHz Oscillator
   #define GLCD_CLK_PLL_DIV_BY_2 TRUE
   #define GLCD_CLK_N_DIVIDER    27
   #define GLCD_SDCLK            27000000
   
   // Touch Defines
   #define TOUCH_INT             PIN_G9
   
   #define TOUCH_MODE            TOUCH_MODE_PENTRIG
   #define TOUCH_FILTER          TOUCH_FILTER_7_SAMP_AQU
   #define TOUCH_CONV_TIME       TOUCH_CONV_TIME_11
   #define TOUCH_PEN_RESISTOR    TOUCH_PEN_RESISTOR_200K
   #define TOUCH_SETTLING_TIME   TOUCH_SETTLING_TIME_11
   
   // USB Defines
   #define __USB_PIC_PERIF__  1
   
   #bit U1OTGSTAT_SESVD=getenv("BIT:SESVD")
   #define USB_CABLE_IS_ATTACHED() (U1OTGSTAT_SESVD)

   #ifdef GFX_USE_SERIAL_INTERFACE
      #define USB_CONFIG_PID  0x0012
      #define USB_CONFIG_VID  0x2405
   
      #define MAX_DATA_SIZE      MAX_SERIAL_DATA_SIZE
   #else
      #define MAX_DATA_SIZE      1024
   #endif
   #define MAX_WRITE_SIZE        FLASH_PAGE_WORDS
   
   // Backlight Pin Defines
   #define GFX_BACKLIGHT_PIN        PIN_G15
   #define GFX_BACKLIGHT_TIMER      2
   
   //Tick Timer Defines
   #define GFX_TICK                 TICK
   #define GFX_TICKS_PER_SECOND     TICKS_PER_SECOND
   #define gfx_get_tick             TickGet
   #define gfx_tick_difference(x,y) (x - y)
   
   // LCD On/Off Pin
   #define GFX_ON_OFF_PIN          PIN_E0
   
   #define FLASH_CALIBRATION_COLUMN    0
   #define FLASH_CALIBRATION_PAGE      0
   #define FLASH_CALIBRATION_BLOCK     (FLASH_PLANE_BLOCKS - 1)
   #define FLASH_CALIBRATION_PLANE     (FLASH_PLANES - 1)
   
   #include <S1D13517.c>
   #include <S34ML02G104T.c>
   #include <SX8651.c>
   #include <tick.c>
   
   #include <gfx_graphics.h>
   #include <fast_flashLcd.c>
   
   TOUCH_CAL TouchCal;
   
   void InitHW(void)
   {  
      FLASH_ADDR Address;
      
      glcd_Init();
      flash_Init();
      touch_Init();
      
      Address.Column = FLASH_CALIBRATION_PAGE;
      Address.Page = FLASH_CALIBRATION_PAGE;
      Address.Block = FLASH_CALIBRATION_BLOCK;
      Address.Plane = FLASH_CALIBRATION_PLANE;
      
      flash_ReadData(Address, (uint16_t *)&TouchCal, sizeof(TOUCH_CAL) / 2);
         
      if((TouchCal.X.Offset == -1) && (TouchCal.X.Slope == -1) && (TouchCal.Y.Offset == -1) && (TouchCal.Y.Slope == -1))
      {
         TouchCal.X.Offset = 0;
         TouchCal.X.Slope = 0;
         TouchCal.X.Reversed = 0;
         
         TouchCal.Y.Offset = 0;
         TouchCal.Y.Slope = 0;
         TouchCal.Y.Reversed = 0;
         
         TouchCal.Rotated = 0;
      }
      
      enable_interrupts(GLOBAL);
   }

#endif
