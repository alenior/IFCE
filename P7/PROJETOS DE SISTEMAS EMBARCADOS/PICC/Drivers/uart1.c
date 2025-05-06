/*
   UART1.c
   
   Library for sending/receiving data on the PIC's UART 1 peripheral.
   
   See UART1.h for documentation
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

#ifndef __UART1_C__
#define __UART1_C__

#include <UART1.h>

#if defined(__XC32)
#warning This library has not been tested yet
#endif

#if defined(__XC8) || defined(__XC16) || defined(__XC32)
#include <string.h>

// these look at the internal peripheral buffer, unlike the normal getc() and kbhit() which will look at the ISR buffer
static char _UART1PhyGetc(void);
static bool _UART1PhyKbhit(void);

#if (UART1_RX_BUFFER_SIZE > 0)
volatile struct
{
   volatile uart1_rx_size_t in;
   volatile uart1_rx_size_t out;
   volatile uart1_rx_size_t num;
   volatile char b[UART1_RX_BUFFER_SIZE];
} _g_UART1RxBuffer;

static void _UART1PhyRxIsrEnable(bool enable);

inline void UART1RxIsrTask(void)
{
   char c;
   
   while(_UART1PhyKbhit())
   {
      c = _UART1PhyGetc();
      
      _g_UART1RxBuffer.b[_g_UART1RxBuffer.in] = c;
      
      if (++_g_UART1RxBuffer.in >= UART1_RX_BUFFER_SIZE)
         _g_UART1RxBuffer.in = 0;
         
      if (++_g_UART1RxBuffer.num > UART1_RX_BUFFER_SIZE)
      {
         _g_UART1RxBuffer.num = UART1_RX_BUFFER_SIZE;
         
         // overflow, move the out index up too because we just lost it
         if (++_g_UART1RxBuffer.out >= UART1_RX_BUFFER_SIZE)
            _g_UART1RxBuffer.out = 0;
      }
   }
}

char UART1Getc(void)
{
   char c;

   while (!UART1Kbhit()) {}

   _UART1PhyRxIsrEnable(false);
   
   c = _g_UART1RxBuffer.b[_g_UART1RxBuffer.out];
   
   if (++_g_UART1RxBuffer.out >= UART1_RX_BUFFER_SIZE)
      _g_UART1RxBuffer.out = 0;
      
   _g_UART1RxBuffer.num -= 1;
   
   _UART1PhyRxIsrEnable(true);
   
   return(c);
}

uart1_rx_size_t UART1Kbhit(void)
{
   uart1_rx_size_t num;

   _UART1PhyRxIsrEnable(false);
   
   num = _g_UART1RxBuffer.num;
   
   _UART1PhyRxIsrEnable(true);
   
   return(num);
}
#else
char UART1Getc(void)
{
   return(_UART1PhyGetc());
}

uart1_rx_size_t UART1Kbhit(void)
{
   return(_UART1PhyKbhit());
}
#endif   //if (UART1_RX_BUFFER_SIZE > 0)
#endif   //defined(__XC8) || defined(__XC16) || defined(__XC32)

#if defined(__PCB__)||defined(__PCM__)||defined(__PCH__)||defined(__PCD__)
   //BEGIN section: CCS C Compilers

#use rs232(UART1, stream=STREAM_UART_UART1, noinit, RECEIVE_BUFFER=UART1_RX_BUFFER_SIZE, errors)

#if (UART1_RX_BUFFER_SIZE > 0)
static void _UART1PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA);
   }
   else
   {
      disable_interrupts(INT_RDA);
   }
}
#endif

void UART1Init(uint32_t baud)
{
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART1);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART1);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART1);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART1);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART1);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART1);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART1);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART1);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART1);

  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(true);
  #endif
}

void UART1Putc(char c)
{
   fputc(c, STREAM_UART_UART1);
}

static char UART1Getc(void)
{
   return(fgetc(STREAM_UART_UART1));
}

uart1_rx_size_t UART1Kbhit(void)
{
  #if (UART1_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART1));
  #else
   return(kbhit(STREAM_UART_UART1));
  #endif
}

   //END section: CCS C Compilers
#endif

   
#if defined(__XC16) || defined(__XC32)
   //BEGIN section: Microchip XC16 or XC32 compiler

static uint32_t _g_UART1Baud;

static void _UART1OpenPerif(uint32_t baud) 
{
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(false);
  #endif
  
   _g_UART1Baud = baud;
  
   U1MODE = 0x8008;
   U1STA = 0x0000;
   U1BRG = (SYS_CLK_FrequencyPeripheralGet() / (4 * baud)) - 1;
   U1STAbits.UTXEN = 1;
   
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(true);
  #endif   
}

void UART1Init(uint32_t baud)
{  
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART1RxBuffer, 0, sizeof(_g_UART1RxBuffer));
  #endif

   _UART1OpenPerif(baud);
}

static bool _UART1PhyKbhit(void) 
{
   return(U1STAbits.URXDA);
}

static char _UART1PhyGetc(void) 
{
   while (!_UART1PhyKbhit());
   
   if (U1STAbits.OERR)
   {
      // if overrun, reset the peripheral
      _UART1OpenPerif(_g_UART1Baud);
   }
   
   return(U1RXREG);
}

void UART1Putc(char c) 
{
   while(U1STAbits.UTXBF == 1);
   U1TXREG = c;
}

#if (UART1_RX_BUFFER_SIZE > 0)
static void _UART1PhyRxIsrEnable(bool enable) 
{
   IEC0bits.U1RXIE = enable;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt ( void )
{
   IFS0bits.U1RXIF = 0;
   UART1RxIsrTask();
}
#endif   

   //END section: Microchip XC16 or XC32 compiler
#endif

#if defined(__XC8)
   //BEGIN section: Microchip XC8 compiler with plib   
 
static uint32_t _g_UART1Baud;
 
static void _UART1OpenPerif(uint32_t baud)
{
   _g_UART1Baud = baud;

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
  
  #if (UART1_RX_BUFFER_SIZE > 0)
   #define __UART1_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
   
   _UART1PhyRxIsrEnable(false);
  #else
   #define __UART1_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
  #endif

  #if !defined(_BAUDCON1_BRG16_POSN) && !defined(_BAUDCON1_BRG16_POSITION)
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
   if (spbrg > 255)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/64 - 1;
      config = __UART1_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART1_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }
  #else
   BAUDCON1 |= (1 << _BAUDCON1_BRG16_POSN);
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/4 - 1;
   if (spbrg > 0xFFFF)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
      config = __UART1_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART1_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }   
  #endif
   
   Open1USART(config, spbrg);
   
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(true);
  #endif
}
 
void UART1Init(uint32_t baud)
{  
  #if (UART1_RX_BUFFER_SIZE > 0)
   _UART1PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART1RxBuffer, 0, sizeof(_g_UART1RxBuffer));
  #endif

   _UART1OpenPerif(baud);
}

static bool _UART1PhyKbhit(void)
{
   return(DataRdy1USART());
}

static char _UART1PhyGetc(void)
{
   if (USART1_Status.OVERRUN_ERROR)
   {
      // if overrun, reset the peripheral
      _UART1OpenPerif(_g_UART1Baud);
   }
   
   return(getc1USART());
}

void UART1Putc(char c)
{
   while(Busy1USART());
   putc1USART(c);
}

#if (UART1_RX_BUFFER_SIZE > 0)
static void _UART1PhyRxIsrEnable(bool enable)
{
   PIE1bits.RC1IE = enable;
}   
#endif

   //END section: Microchip XC8 compiler with plib
#endif

void UART1Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART1Putc(c);
   }
}

void UART1PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART1Putc(c);
   }
}

#endif   //ifndef __UART1_C__
