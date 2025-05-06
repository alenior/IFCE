///////////////////////////////////////////////////////////////////////////
////                             kbd3.c                                ////
////                  Generic keypad scan driver                       ////
////                                                                   ////
////  This is a way of configuring the kbd4.c driver to use a 3x4      ////
////  keypad, instead of a 4x4 keypad.                                 ////
////                                                                   ////
////  For more documentation, see kbd4.c.                              ////
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

#ifndef __KBD3_C__
#define __KBD3_C__

#define KBD_NUM_ROWS 4
#define KBD_NUM_COLS 3

#if !defined(KBD_USE_EXTERNAL_KEYS)
#define KBD_USE_EXTERNAL_KEYS
// Keypad layout:
const char KEYS[KBD_NUM_ROWS][KBD_NUM_COLS] =
  {{'1','2','3'},
   {'4','5','6'},
   {'7','8','9'},
   {'*','0','#'}};
#endif

#include "kbd4.c"

#endif   //__KBD3_C__
