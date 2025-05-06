/*
   UART3.c
   
   Library for sending/receiving data on the PIC's UART 3 peripheral.
   
   See UART3.h for documentation
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

#ifndef __UART3_C__
#define __UART3_C__

#include <UART3.h>

#if defined(__XC32)
#warning This library has not been tested yet
#endif

#if defined(__XC8) || defined(__XC16) || defined(__XC32)
#include <string.h>

// these look at the internal peripheral buffer, unlike the normal getc() and kbhit() which will look at the ISR buffer
static char _UART3PhyGetc(void);
static bool _UART3PhyKbhit(void);

#if (UART3_RX_BUFFER_SIZE > 0)
volatile struct
{
   volatile uart3_rx_size_t in;
   volatile uart3_rx_size_t out;
   volatile uart3_rx_size_t num;
   volatile char b[UART3_RX_BUFFER_SIZE];
} _g_UART3RxBuffer;

static void _UART3PhyRxIsrEnable(bool enable);

inline void UART3RxIsrTask(void)
{
   char c;
   
   while(_UART3PhyKbhit())
   {
      c = _UART3PhyGetc();
      
      _g_UART3RxBuffer.b[_g_UART3RxBuffer.in] = c;
      
      if (++_g_UART3RxBuffer.in >= UART3_RX_BUFFER_SIZE)
         _g_UART3RxBuffer.in = 0;
         
      if (++_g_UART3RxBuffer.num > UART3_RX_BUFFER_SIZE)
      {
         _g_UART3RxBuffer.num = UART3_RX_BUFFER_SIZE;
         
         // overflow, move the out index up too because we just lost it
         if (++_g_UART3RxBuffer.out >= UART3_RX_BUFFER_SIZE)
            _g_UART3RxBuffer.out = 0;
      }
   }
}

char UART3Getc(void)
{
   char c;

   while (!UART3Kbhit()) {}

   _UART3PhyRxIsrEnable(false);
   
   c = _g_UART3RxBuffer.b[_g_UART3RxBuffer.out];
   
   if (++_g_UART3RxBuffer.out >= UART3_RX_BUFFER_SIZE)
      _g_UART3RxBuffer.out = 0;
      
   _g_UART3RxBuffer.num -= 1;
   
   _UART3PhyRxIsrEnable(true);
   
   return(c);
}

uart3_rx_size_t UART3Kbhit(void)
{
   uart3_rx_size_t num;

   _UART3PhyRxIsrEnable(false);
   
   num = _g_UART3RxBuffer.num;
   
   _UART3PhyRxIsrEnable(true);
   
   return(num);
}
#else
char UART3Getc(void)
{
   return(_UART3PhyGetc());
}

uart3_rx_size_t UART3Kbhit(void)
{
   return(_UART3PhyKbhit());
}
#endif   //if (UART3_RX_BUFFER_SIZE > 0)
#endif   //defined(__XC8) || defined(__XC16) || defined(__XC32)

#if defined(__PCB__)||defined(__PCM__)||defined(__PCH__)||defined(__PCD__)
   //BEGIN section: CCS C Compilers

#use rs232(UART3, stream=STREAM_UART_UART3, noinit, RECEIVE_BUFFER=UART3_RX_BUFFER_SIZE, errors)

#if (UART3_RX_BUFFER_SIZE > 0)
static void _UART3PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA3);
   }
   else
   {
      disable_interrupts(INT_RDA3);
   }
}
#endif

void UART3Init(uint32_t baud)
{
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART3);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART3);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART3);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART3);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART3);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART3);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART3);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART3);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART3);

  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(true);
  #endif
}

void UART3Putc(char c)
{
   fputc(c, STREAM_UART_UART3);
}

static char UART3Getc(void)
{
   return(fgetc(STREAM_UART_UART3));
}

uart3_rx_size_t UART3Kbhit(void)
{
  #if (UART3_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART3));
  #else
   return(kbhit(STREAM_UART_UART3));
  #endif
}

   //END section: CCS C Compilers
#endif

   
#if defined(__XC16) || defined(__XC32)
   //BEGIN section: Microchip XC16 or XC32 compiler

static uint32_t _g_UART3Baud;

static void _UART3OpenPerif(uint32_t baud) 
{
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(false);
  #endif
  
   _g_UART3Baud = baud;
  
   U3MODE = 0x8008;
   U3STA = 0x0000;
   U3BRG = (SYS_CLK_FrequencyPeripheralGet() / (4 * baud)) - 1;
   U3STAbits.UTXEN = 1;
   
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(true);
  #endif   
}

void UART3Init(uint32_t baud)
{  
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART3RxBuffer, 0, sizeof(_g_UART3RxBuffer));
  #endif

   _UART3OpenPerif(baud);
}

static bool _UART3PhyKbhit(void) 
{
   return(U3STAbits.URXDA);
}

static char _UART3PhyGetc(void) 
{
   while (!_UART3PhyKbhit());
   
   if (U3STAbits.OERR)
   {
      // if overrun, reset the peripheral
      _UART3OpenPerif(_g_UART3Baud);
   }
   
   return(U3RXREG);
}

void UART3Putc(char c) 
{
   while(U3STAbits.UTXBF == 1);
   U3TXREG = c;
}

#if (UART3_RX_BUFFER_SIZE > 0)
static void _UART3PhyRxIsrEnable(bool enable) 
{
   IEC5bits.U3RXIE = enable;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3RXInterrupt ( void )
{
   IFS5bits.U3RXIF = 0;
   UART3RxIsrTask();
}
#endif   

   //END section: Microchip XC16 or XC32 compiler
#endif

#if defined(__XC8)
   //BEGIN section: Microchip XC8 compiler with plib   
 
static uint32_t _g_UART3Baud;
 
static void _UART3OpenPerif(uint32_t baud)
{
   _g_UART3Baud = baud;

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
  
  #if (UART3_RX_BUFFER_SIZE > 0)
   #define __UART3_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
   
   _UART3PhyRxIsrEnable(false);
  #else
   #define __UART3_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
  #endif

  #if !defined(_BAUDCON3_BRG16_POSN) && !defined(_BAUDCON3_BRG16_POSITION)
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
   if (spbrg > 255)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/64 - 1;
      config = __UART3_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART3_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }
  #else
   BAUDCON3 |= (1 << _BAUDCON3_BRG16_POSN);
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/4 - 1;
   if (spbrg > 0xFFFF)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
      config = __UART3_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART3_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }   
  #endif
   
   Open3USART(config, spbrg);
   
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(true);
  #endif
}
 
void UART3Init(uint32_t baud)
{  
  #if (UART3_RX_BUFFER_SIZE > 0)
   _UART3PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART3RxBuffer, 0, sizeof(_g_UART3RxBuffer));
  #endif

   _UART3OpenPerif(baud);
}

static bool _UART3PhyKbhit(void)
{
   return(DataRdy3USART());
}

static char _UART3PhyGetc(void)
{
   if (USART3_Status.OVERRUN_ERROR)
   {
      // if overrun, reset the peripheral
      _UART3OpenPerif(_g_UART3Baud);
   }
   
   return(getc3USART());
}

void UART3Putc(char c)
{
   while(Busy3USART());
   putc3USART(c);
}

#if (UART3_RX_BUFFER_SIZE > 0)
static void _UART3PhyRxIsrEnable(bool enable)
{
   PIE6bits.RC3IE = enable;
}   
#endif

   //END section: Microchip XC8 compiler with plib
#endif

void UART3Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART3Putc(c);
   }
}

void UART3PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART3Putc(c);
   }
}

#endif   //ifndef __UART3_C__
