/*
   UART4.c
   
   Library for sending/receiving data on the PIC's UART 4 peripheral.
   
   See UART4.h for documentation
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __UART4_C__
#define __UART4_C__

#include <UART4.h>

#if defined(__XC32)
#warning This library has not been tested yet
#endif

#if defined(__XC8) || defined(__XC16) || defined(__XC32)
#include <string.h>

// these look at the internal peripheral buffer, unlike the normal getc() and kbhit() which will look at the ISR buffer
static char _UART4PhyGetc(void);
static bool _UART4PhyKbhit(void);

#if (UART4_RX_BUFFER_SIZE > 0)
volatile struct
{
   volatile uart4_rx_size_t in;
   volatile uart4_rx_size_t out;
   volatile uart4_rx_size_t num;
   volatile char b[UART4_RX_BUFFER_SIZE];
} _g_UART4RxBuffer;

static void _UART4PhyRxIsrEnable(bool enable);

inline void UART4RxIsrTask(void)
{
   char c;
   
   while(_UART4PhyKbhit())
   {
      c = _UART4PhyGetc();
      
      _g_UART4RxBuffer.b[_g_UART4RxBuffer.in] = c;
      
      if (++_g_UART4RxBuffer.in >= UART4_RX_BUFFER_SIZE)
         _g_UART4RxBuffer.in = 0;
         
      if (++_g_UART4RxBuffer.num > UART4_RX_BUFFER_SIZE)
      {
         _g_UART4RxBuffer.num = UART4_RX_BUFFER_SIZE;
         
         // overflow, move the out index up too because we just lost it
         if (++_g_UART4RxBuffer.out >= UART4_RX_BUFFER_SIZE)
            _g_UART4RxBuffer.out = 0;
      }
   }
}

char UART4Getc(void)
{
   char c;

   while (!UART4Kbhit()) {}

   _UART4PhyRxIsrEnable(false);
   
   c = _g_UART4RxBuffer.b[_g_UART4RxBuffer.out];
   
   if (++_g_UART4RxBuffer.out >= UART4_RX_BUFFER_SIZE)
      _g_UART4RxBuffer.out = 0;
      
   _g_UART4RxBuffer.num -= 1;
   
   _UART4PhyRxIsrEnable(true);
   
   return(c);
}

uart4_rx_size_t UART4Kbhit(void)
{
   uart4_rx_size_t num;

   _UART4PhyRxIsrEnable(false);
   
   num = _g_UART4RxBuffer.num;
   
   _UART4PhyRxIsrEnable(true);
   
   return(num);
}
#else
char UART4Getc(void)
{
   return(_UART4PhyGetc());
}

uart4_rx_size_t UART4Kbhit(void)
{
   return(_UART4PhyKbhit());
}
#endif   //if (UART4_RX_BUFFER_SIZE > 0)
#endif   //defined(__XC8) || defined(__XC16) || defined(__XC32)

#if defined(__PCB__)||defined(__PCM__)||defined(__PCH__)||defined(__PCD__)
   //BEGIN section: CCS C Compilers

#use rs232(UART4, stream=STREAM_UART_UART4, noinit, RECEIVE_BUFFER=UART4_RX_BUFFER_SIZE, errors)

#if (UART4_RX_BUFFER_SIZE > 0)
static void _UART4PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA4);
   }
   else
   {
      disable_interrupts(INT_RDA4);
   }
}
#endif

void UART4Init(uint32_t baud)
{
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART4);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART4);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART4);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART4);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART4);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART4);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART4);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART4);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART4);

  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(true);
  #endif
}

void UART4Putc(char c)
{
   fputc(c, STREAM_UART_UART4);
}

static char UART4Getc(void)
{
   return(fgetc(STREAM_UART_UART4));
}

uart4_rx_size_t UART4Kbhit(void)
{
  #if (UART4_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART4));
  #else
   return(kbhit(STREAM_UART_UART4));
  #endif
}

   //END section: CCS C Compilers
#endif

   
#if defined(__XC16) || defined(__XC32)
   //BEGIN section: Microchip XC16 or XC32 compiler

static uint32_t _g_UART4Baud;

static void _UART4OpenPerif(uint32_t baud) 
{
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(false);
  #endif
  
   _g_UART4Baud = baud;
  
   U4MODE = 0x8008;
   U4STA = 0x0000;
   U4BRG = (SYS_CLK_FrequencyPeripheralGet() / (4 * baud)) - 1;
   U4STAbits.UTXEN = 1;
   
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(true);
  #endif   
}

void UART4Init(uint32_t baud)
{  
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART4RxBuffer, 0, sizeof(_g_UART4RxBuffer));
  #endif

   _UART4OpenPerif(baud);
}

static bool _UART4PhyKbhit(void) 
{
   return(U4STAbits.URXDA);
}

static char _UART4PhyGetc(void) 
{
   while (!_UART4PhyKbhit());
   
   if (U4STAbits.OERR)
   {
      // if overrun, reset the peripheral
      _UART4OpenPerif(_g_UART4Baud);
   }
   
   return(U4RXREG);
}

void UART4Putc(char c) 
{
   while(U4STAbits.UTXBF == 1);
   U4TXREG = c;
}

#if (UART4_RX_BUFFER_SIZE > 0)
static void _UART4PhyRxIsrEnable(bool enable) 
{
   IEC5bits.U4RXIE = enable;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4RXInterrupt ( void )
{
   IFS5bits.U4RXIF = 0;
   UART4RxIsrTask();
}
#endif   

   //END section: Microchip XC16 or XC32 compiler
#endif

#if defined(__XC8)
   //BEGIN section: Microchip XC8 compiler with plib   
 
static uint32_t _g_UART4Baud;
 
static void _UART4OpenPerif(uint32_t baud)
{
   _g_UART4Baud = baud;

   /*
   From the PLIB manual:
   spbrg is the value that is written to the baud rate generator
   register which determines the baud rate at which the usart
   operates. The formulas for baud rate are:
   
   Asynchronous mode, high speed:
   baud = Fosc / (16 * (spbrg + 1))
   spbrg = (Fosc/baud)/16 - 1
   Asynchronous mode, low speed:
   baud = Fosc / (64 * (spbrg + 1))   
   spbrg = (Fosc/baud)/64 - 1
   */   
   uint32_t spbrg;
   char config;
   
  #if defined(USE_OR_MASKS)
   #error can not use USE_OR_MASKS with this library
  #endif  
  
  #if (UART4_RX_BUFFER_SIZE > 0)
   #define __UART4_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
   
   _UART4PhyRxIsrEnable(false);
  #else
   #define __UART4_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
  #endif

  #if !defined(_BAUDCON4_BRG16_POSN) && !defined(_BAUDCON4_BRG16_POSITION)
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
   if (spbrg > 255)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/64 - 1;
      config = __UART4_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART4_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }
  #else
   BAUDCON4 |= (1 << _BAUDCON4_BRG16_POSN);
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/4 - 1;
   if (spbrg > 0xFFFF)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
      config = __UART4_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART4_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }   
  #endif
   
   Open4USART(config, spbrg);
   
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(true);
  #endif
}
 
void UART4Init(uint32_t baud)
{  
  #if (UART4_RX_BUFFER_SIZE > 0)
   _UART4PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART4RxBuffer, 0, sizeof(_g_UART4RxBuffer));
  #endif

   _UART4OpenPerif(baud);
}

static bool _UART4PhyKbhit(void)
{
   return(DataRdy4USART());
}

static char _UART4PhyGetc(void)
{
   if (USART4_Status.OVERRUN_ERROR)
   {
      // if overrun, reset the peripheral
      _UART4OpenPerif(_g_UART4Baud);
   }
   
   return(getc4USART());
}

void UART4Putc(char c)
{
   while(Busy4USART());
   putc4USART(c);
}

#if (UART4_RX_BUFFER_SIZE > 0)
static void _UART4PhyRxIsrEnable(bool enable)
{
   PIE6bits.RC4IE = enable;
}   
#endif

   //END section: Microchip XC8 compiler with plib
#endif

void UART4Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART4Putc(c);
   }
}

void UART4PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART4Putc(c);
   }
}

#endif   //ifndef __UART4_C__
