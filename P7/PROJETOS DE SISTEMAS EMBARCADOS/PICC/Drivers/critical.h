/******************************************************************************

   critical.h
   
   Mark a section of code as critical and prevent ISRs from interrupting it's
   execution.  Logic is only applied if ISRs are actively enabled.
   
   CRITICAL_SECTION_ENTER()
      Start a critical section.  There can be only one in a function.
      
   CRITICAL_SECTION_EXIT()
      End a critical section.
      
   CRITICAL_SECTION_REENTER()
      If there are multiple sections in a function that need to be marked
      critical, this function can be used to mark any section after the first.
   
   For an example of usage, see the timeouts.c library in the CCS C Compiler's
   drivers directory.

   Recursive calls of CRITICAL_SECTION_ENTER/CRITICAL_SECTION_REENTER do not
   corrupt the restoration of the ISR when calling CRITICAL_SECTION_EXIT().
   Therefore, this is legal:
      CRITICAL_SECTION_ENTER();
      {
         CRITICAL_SECTION_ENTER();
         CRITICAL_SECTION_EXIT();
      }
      CRITICAL_SECTION_EXIT();
   In the above example, ISRs won't be restored until the second 
   CRITICAL_SECTION_EXIT().

   These functions can be called inside an ISR.
   
*****************************************************************************/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2019 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef CRITICAL_SECTION_ENTER

#define _CRIT_GIE_SET() enable_interrupts(GLOBAL)
#define _CRIT_GIE_CLEAR() disable_interrupts(GLOBAL)
#define _CRIT_GIE_GET() interrupt_enabled(GLOBAL)

#define CRITICAL_SECTION_ENTER() \
   int1 critGieEnabled = _CRIT_GIE_GET();  \
   if (critGieEnabled)  \
   {  \
      _CRIT_GIE_CLEAR();   \
   }

#define CRITICAL_SECTION_REENTER() \
   if (!critGieEnabled) \
   {  \
      critGieEnabled = _CRIT_GIE_GET();  \
      if (critGieEnabled)  \
      {  \
         _CRIT_GIE_CLEAR();   \
      }  \
   }

#define CRITICAL_SECTION_EXIT()  \
   if (critGieEnabled) \
   {  \
      critGieEnabled = 0;  \
      _CRIT_GIE_SET(); \
   }

#endif   //CRITICAL_SECTION_ENTER
