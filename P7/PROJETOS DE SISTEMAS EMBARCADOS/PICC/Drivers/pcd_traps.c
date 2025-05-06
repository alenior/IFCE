///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          pcd_traps.c                              ////
////                                                                   ////
//// On 16bit PICs (PIC24, dsPIC30, dsPIC33) there are special trap    ////
//// vectors that are called when a fault has happened.  This library  ////
//// will place routines on those vectors to display debugging         ////
//// information on the serial port about the fault.  The PC,          ////
//// working registers and other important PIC status registers        ////
//// are displayed on the serial port.                                 ////
////                                                                   ////
//// The last #use_rs232() will be used.  Alternately, if the          ////
//// STREAM_SERIAL_INPUT is defind it will use that stream.            ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2012 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#if defined(__PCD__)

#ifndef __PCD_TRAPS_C__
#define __PCD_TRAPS_C__

#ifndef TRAPS_HAS_NO_PUTC
   #ifndef TRAPS_PUTC_PROVIDED
      #if defined(STREAM_SERIAL_INPUT)
         #inline
         void TrapPutc(char c)
         {
            fputc(c, STREAM_SERIAL_INPUT);
         }
      #else
         #inline
         void TrapPutc(char c)
         {
            putc(c);
         }
      #endif
   #endif
#endif

#word PCD_SFR_W0REG=0
#word PCD_SFR_W1REG=2
#word PCD_SFR_W2REG=4
#word PCD_SFR_W3REG=6
#word PCD_SFR_W4REG=8
#word PCD_SFR_W5REG=10
#word PCD_SFR_W6REG=12
#word PCD_SFR_W7REG=14
#word PCD_SFR_W8REG=16
#word PCD_SFR_W9REG=18
#word PCD_SFR_W10REG=20
#word PCD_SFR_W11REG=22
#word PCD_SFR_W12REG=24
#word PCD_SFR_W13REG=26
#word PCD_SFR_W14REG=28
#word PCD_SFR_W15REG=30

#if (getenv("SFR_VALID:PSVPAG"))
   #word PCD_SFR_PSVPAG=getenv("SFR:PSVPAG")
#endif

#if (getenv("SFR_VALID:DSRPAG"))
   #word PCD_SFR_DSRPAG=getenv("SFR:DSRPAG")
#endif

#if (getenv("SFR_VALID:DSWPAG"))
   #word PCD_SFR_DSWPAG=getenv("SFR:DSWPAG")
#endif

#if (getenv("SFR_VALID:TBLPAG"))
   #word PCD_SFR_TBLPAG=getenv("SFR:TBLPAG")
   unsigned int16 g_TrapTBLPAG;
#endif

#word PCD_SFR_CORCON=getenv("SFR:CORCON")

#if (getenv("SFR_VALID:INTCON2"))
   #word PCD_SFR_INTCON2=getenv("SFR:INTCON2")
#endif
#if (getenv("SFR_VALID:INTCON3"))
   #word PCD_SFR_INTCON3=getenv("SFR:INTCON3")
#endif
#if (getenv("SFR_VALID:INTCON4"))
   #word PCD_SFR_INTCON4=getenv("SFR:INTCON4")
#endif

struct
{
   unsigned int16 w[16];
   unsigned int32 addy;
} g_Trap;

unsigned int16 g_TrapH, g_TrapL;

#if (getenv("SFR_VALID:TBLPAG"))
   #define PCD_TRAPS_MEMORY_USED 42
#else
   #define PCD_TRAPS_MEMORY_USED 40
#endif

#inline 
void TRAP_START(void)
{
  #asm 
   POP g_TrapH;
   POP g_TrapL;
  #endasm
      
   g_Trap.w[0] = PCD_SFR_W0REG;
   g_Trap.w[1] = PCD_SFR_W1REG;
   g_Trap.w[2] = PCD_SFR_W2REG;
   g_Trap.w[3] = PCD_SFR_W3REG;
   g_Trap.w[4] = PCD_SFR_W4REG;
   g_Trap.w[5] = PCD_SFR_W5REG;
   g_Trap.w[6] = PCD_SFR_W6REG;
   g_Trap.w[7] = PCD_SFR_W7REG;
   g_Trap.w[8] = PCD_SFR_W8REG;
   g_Trap.w[9] = PCD_SFR_W9REG;
   g_Trap.w[10] = PCD_SFR_W10REG;
   g_Trap.w[11] = PCD_SFR_W11REG;
   g_Trap.w[12] = PCD_SFR_W12REG;
   g_Trap.w[13] = PCD_SFR_W13REG;
   g_Trap.w[14] = PCD_SFR_W14REG;
   g_Trap.w[15] = PCD_SFR_W15REG;

  #if (getenv("SFR_VALID:TBLPAG"))
   g_TrapTBLPAG = PCD_SFR_TBLPAG;
  #endif

   g_TrapH &= 0x00FF;

   g_Trap.addy = make32(g_TrapH, g_TrapL);
   g_Trap.addy -= 2;
   
   #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "\r\n\n");
   #endif
}

#inline 
void TRAP_DISPLAY(void)
{
  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "PC:%LX\n", g_Trap.addy);
  #endif
   
  #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, " PC:%LX ", g_Trap.addy);
   
   for (g_TrapL=0; g_TrapL<16; g_TrapL++)
   {
      printf(TrapPutc, "W%U:%LX ", g_TrapL, g_Trap.w[g_TrapL]);
   }
  
  #if (getenv("SFR_VALID:PSVPAG"))
   printf(TrapPutc, "PSVPAG:%LX ", PCD_SFR_PSVPAG);
  #endif

  #if (getenv("SFR_VALID:DSRPAG"))
   printf(TrapPutc, "DSRPAG:%LX ", PCD_SFR_DSRPAG);
  #endif

  #if (getenv("SFR_VALID:DSWPAG"))
   printf(TrapPutc, "DSWPAG:%LX ", PCD_SFR_DSWPAG);
  #endif

  #if (getenv("SFR_VALID:TBLPAG"))
   printf(TrapPutc, "TBLPAG:%LX ", g_TrapTBLPAG);
  #endif

  #if (getenv("SFR_VALID:INTCON2"))
   printf(TrapPutc, "INTCON2:%LX ", PCD_SFR_INTCON2);
  #endif

  #if (getenv("SFR_VALID:INTCON3"))
   printf(TrapPutc, "INTCON3:%LX ", PCD_SFR_INTCON3);
  #endif
  
  #if (getenv("SFR_VALID:INTCON4"))
   printf(TrapPutc, "INTCON4:%LX ", PCD_SFR_INTCON4);
  #endif

   printf(TrapPutc, "CORCON:%LX ", PCD_SFR_CORCON);
  
   printf(TrapPutc, "\r\n");
   
   g_TrapL = PCD_SFR_W15REG;
   g_TrapL -= (unsigned int16)&_STACK_[0];
   g_TrapL /= 2;
   printf(TrapPutc, "STACK (%LX) ", g_TrapL);
   if (g_TrapL > 64) g_TrapL=64;
   while(g_TrapL--)
   {
      #asm 
      POP g_TrapH
      #endasm
      printf(TrapPutc, "%LX ", g_TrapH);
   }
   printf(TrapPutc, "\r\n");
   #endif
}

#define TRAP_END() while(1)

#if defined(INT_ADDRERR)
#INT_ADDRERR FAST
void handle_addrerr(void)
{
   TRAP_START();
   
  #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "ADDRESS FAULT");
  #endif
  
  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "\fADDRESS FAULT\n");
  #endif
  
   TRAP_DISPLAY();
   
   TRAP_END();
}
#endif

#if defined(INT_MATHERR)
#INT_MATHERR FAST
void handle_matherr(void)
{
   TRAP_START();
   
   #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "MATH FAULT");
   #endif

  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "\fMATH FAULT\n");
  #endif

  
   TRAP_DISPLAY();
   
   TRAP_END();
}
#endif

#if defined(INT_STACKERR)
#INT_STACKERR FAST
void handle_stackerr(void)
{
   TRAP_START();
   
   #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "STACK FAULT");
   #endif
   
  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "\fSTACK FAULT\n");
  #endif

   TRAP_DISPLAY();
   
   TRAP_END();
}
#endif

#if defined(INT_OSCFAIL)
#INT_OSCFAIL FAST
void handle_oscfail(void)
{
   TRAP_START();
   
   #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "OSCILLATOR FAULT");
   #endif

  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "\fOSC FAULT\n");
  #endif 
  
   TRAP_DISPLAY();
   
   TRAP_END();
}
#endif

#if defined(INT_SOFTWARE)
#INT_SOFTWARE FAST
void handle_INT_SOFTWARE(void)
{
   TRAP_START();
   
   #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "INT_SOFTWARE FAULT");
   #endif

  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "\fINT_SW FAULT\n");
  #endif 
  
   TRAP_DISPLAY();
   
   TRAP_END();
}
#endif

#if defined(INT_GENERIC)
#INT_GENERIC FAST
void handle_INT_GENERIC(void)
{
   TRAP_START();
   
   #ifndef TRAPS_HAS_NO_PUTC
   printf(TrapPutc, "INT_GENERIC FAULT");
   #endif
   
  #ifdef TRAPS_HAS_LCDC
   printf(TrapLCDc, "\fINT_GEN FAULT\n");
  #endif 
   
   TRAP_DISPLAY();
   
   TRAP_END();
}
#endif

#endif   //__PCD_TRAPS_C__

#endif   //defined(__PCD__)
