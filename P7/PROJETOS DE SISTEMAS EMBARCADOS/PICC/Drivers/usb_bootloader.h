///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     USB_BOOTLOADER.H                              ////
////                                                                   ////
////  This include file must be included by any application loaded     ////
////  by the example USB bootloader (ex_usb_bootloader                 ////
////                                                                   ////
////  The directives in this file relocate the reset and interrupt     ////
////  vectors as well as reserving space for the bootloader.           ////
////                                                                   ////
////  Normally the bootloader will not try to the write to the flash   ////
////  page that contains the config bits.  This page will also be      ////
////  blocked from use by the application, since this page is usually  ////
////  write protected.  If you want to allow the bootloader to write   ////
////  to this page and let the application use this page, then         ////
////  #define USB_BOOTLOADER_CONFIG_BITS_WRITABLE to enable this       ////
////  option.  Caution should be taken when using this option.         ////
////  Code prior to 2017 was using this option, but starting in 2017   ////
////  the default was to turn this option off.  This option has no     ////
////  effect on PICs that have config bits stored in a special         ////
////  location and not the last flash page.                            ////
////                                                                   ////
////  For more documentation see ex_usb_bootloader.c                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// VERSION HISTORY                                                   ////
////                                                                   ////
//// January 26th, 2017:                                               ////
////  Added PCM support (see ex_usb_bootloader.c).                     ////
////  Added USB_BOOTLOADER_CONFIG_BITS_WRITABLE option.                ////
////                                                                   ////
//// July 9th, 2012:                                                   ////
////  Added support for PCD (see ex_usb_bootloader.c).                 ////
////                                                                   ////
//// March 5th, 2009:                                                  ////
////   Cleanup for Wizard.                                             ////
////   PIC24 Initial release.                                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2009 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#define LOADER_START       (0)

#if defined(__PCM__)
   #define LOADER_SIZE        (0x1000)
#elif defined(__PCH__)
   #define LOADER_SIZE        (0x2000)
#elif defined(__PCD__)
   #define LOADER_SIZE        (0x2000)
#else
   #error not supported
#endif

#define LOADER_END      (LOADER_SIZE-1)

#define APPLICATION_START  LOADER_SIZE

#define APPLICATION_ISR (APPLICATION_START+8)

// bytes per address
#if defined(__PCH__)
   #define _LOADER_BPA 1
#else //PCM and PCD
   #define _LOADER_BPA 2
#endif

#define _LOADER_FIND_PAGE_START(address) ((__ADDRESS__)address & ~(((__ADDRESS__)getenv("FLASH_ERASE_SIZE")/_LOADER_BPA)-(__ADDRESS__)1))

// total size of program memory of this controller, including config bits.  this value is in instructions (NOT BYTES)
#if (_LOADER_FIND_PAGE_START(getenv("PROGRAM_MEMORY")) != getenv("PROGRAM_MEMORY"))
   #define _LOADER_CONFIG_BITS_IN_ROM  1
   
   #define _LOADER_CONFIG_BITS_PAGE_ADDRESS  _LOADER_FIND_PAGE_START(getenv("PROGRAM_MEMORY"))
#else   
   #define _LOADER_CONFIG_BITS_IN_ROM  0
#endif

#ifdef _bootloader
   // bootloader won't use interrupts, instead it will poll USB IF
   #define USB_ISR_POLLING
   
   /*
   Provide an empty application, so if you load this .HEX file into the pic
   without an application this will prevent the pic from executing unknown code.
   */
   #org APPLICATION_START,APPLICATION_START+0x3F
   void BlankApplication(void) 
   {
      while(1);
   }
   
   #define APPLICATION_END    (getenv("PROGRAM_MEMORY")-1)
   
   #org APPLICATION_START+0x40, APPLICATION_END {}


   #define USB_STRINGS_OVERWRITTEN
   
   #define USB_CONFIG_PID  0x000c

   // Here is where the "CCS" Manufacturer string and "SERIAL DEMO" are stored.
   // Strings are saved as unicode.
   // These strings are mostly only displayed during the add hardware wizard.
   // Once the operating system drivers have been installed it will usually display
   // the name from the drivers .INF.
   char const USB_STRING_DESC[]={
      //string 0
            4, //length of string index
            0x03, //USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
            0x09,0x04,   //Microsoft Defined for US-English
      //string 1  - manufacturer
            8, //length of string index
            0x03, //USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
            'C',0,
            'C',0,
            'S',0,
      //string 2 - product
            38, //length of string index
            0x03, //USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
            'C',0,
            'C',0,
            'S',0,
            ' ',0,
            'C',0,
            'D',0,
            'C',0,
            ' ',0,
            'B',0,
            'o',0,
            'o',0,
            't',0,
            'l',0,
            'o',0,
            'a',0,
            'd',0,
            'e',0,
            'r',0
   };
#endif   //_bootloader

#ifndef _bootloader
   //in the application, this moves the reset and isr vector out of the bootload
   //space.  it then reserves the loader space from being used by the application.
   #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR)
   
   #org 0, LOADER_END {}
   
   #if ((_LOADER_CONFIG_BITS_IN_ROM==1) && !defined(USB_BOOTLOADER_CONFIG_BITS_WRITABLE))
      // the page containing the config bits is usually write protected.
      // so prevent the application from using this page.
      #org _LOADER_CONFIG_BITS_PAGE_ADDRESS,getenv("PROGRAM_MEMORY")-1 {}
   #endif
#endif
