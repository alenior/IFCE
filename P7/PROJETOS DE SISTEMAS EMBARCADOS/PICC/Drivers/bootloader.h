///////////////////////////////////////////////////////////////////////////
////                       BOOTLOADER.H                                ////
////                                                                   ////
////  This include file must be included by any application loaded     ////
////  by the example bootloader (ex_bootloader.c).                     ////
////                                                                   ////
////  The directives in this file relocate the reset and interrupt     ////
////  vectors as well as reserving space for the bootloader.           ////
////                                                                   ////
////  LOADER_END and LOADER_SIZE may need to be adjusted for a         ////
////  specific chip and bootloader.                                    ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2004 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////


#if defined(__PCM__)
   #define LOADER_END   0x1FF
	#define LOADER_SIZE	0x1BF
#elif defined(__PCH__)
   #define LOADER_END   0x4FF
	#define LOADER_SIZE	0x3FF
#endif

#ifndef _bootloader

#if defined(__PCM__)
   #build(reset=LOADER_END+1, interrupt=LOADER_END+5)
#elif defined(__PCH__)
   #build(reset=LOADER_END+2, interrupt=LOADER_END+8)
#endif

#org 0, LOADER_END {}

#endif
