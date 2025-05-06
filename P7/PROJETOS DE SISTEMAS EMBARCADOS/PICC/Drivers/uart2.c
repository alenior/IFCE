/*
   UART2.c
   
   Library for sending/receiving data on the PIC's UART 2 peripheral.
   
   See UART2.h for documentation
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

#ifndef __UART2_C__
#define __UART2_C__

#include <UART2.h>

#if defined(__XC32)
#warning This library has not been tested yet
#endif

#if defined(__XC8) || defined(__XC16) || defined(__XC32)
#include <string.h>

// these look at the internal peripheral buffer, unlike the normal getc() and kbhit() which will look at the ISR buffer
static char _UART2PhyGetc(void);
static bool _UART2PhyKbhit(void);

#if (UART2_RX_BUFFER_SIZE > 0)
volatile struct
{
   volatile uart2_rx_size_t in;
   volatile uart2_rx_size_t out;
   volatile uart2_rx_size_t num;
   volatile char b[UART2_RX_BUFFER_SIZE];
} _g_UART2RxBuffer;

static void _UART2PhyRxIsrEnable(bool enable);

inline void UART2RxIsrTask(void)
{
   char c;
   
   while(_UART2PhyKbhit())
   {
      c = _UART2PhyGetc();
      
      _g_UART2RxBuffer.b[_g_UART2RxBuffer.in] = c;
      
      if (++_g_UART2RxBuffer.in >= UART2_RX_BUFFER_SIZE)
         _g_UART2RxBuffer.in = 0;
         
      if (++_g_UART2RxBuffer.num > UART2_RX_BUFFER_SIZE)
      {
         _g_UART2RxBuffer.num = UART2_RX_BUFFER_SIZE;
         
         // overflow, move the out index up too because we just lost it
         if (++_g_UART2RxBuffer.out >= UART2_RX_BUFFER_SIZE)
            _g_UART2RxBuffer.out = 0;
      }
   }
}

char UART2Getc(void)
{
   char c;

   while (!UART2Kbhit()) {}

   _UART2PhyRxIsrEnable(false);
   
   c = _g_UART2RxBuffer.b[_g_UART2RxBuffer.out];
   
   if (++_g_UART2RxBuffer.out >= UART2_RX_BUFFER_SIZE)
      _g_UART2RxBuffer.out = 0;
      
   _g_UART2RxBuffer.num -= 1;
   
   _UART2PhyRxIsrEnable(true);
   
   return(c);
}

uart2_rx_size_t UART2Kbhit(void)
{
   uart2_rx_size_t num;

   _UART2PhyRxIsrEnable(false);
   
   num = _g_UART2RxBuffer.num;
   
   _UART2PhyRxIsrEnable(true);
   
   return(num);
}
#else
char UART2Getc(void)
{
   return(_UART2PhyGetc());
}

uart2_rx_size_t UART2Kbhit(void)
{
   return(_UART2PhyKbhit());
}
#endif   //if (UART2_RX_BUFFER_SIZE > 0)
#endif   //defined(__XC8) || defined(__XC16) || defined(__XC32)

#if defined(__PCB__)||defined(__PCM__)||defined(__PCH__)||defined(__PCD__)
   //BEGIN section: CCS C Compilers

#use rs232(UART2, stream=STREAM_UART_UART2, noinit, RECEIVE_BUFFER=UART2_RX_BUFFER_SIZE, errors)

#if (UART2_RX_BUFFER_SIZE > 0)
static void _UART2PhyRxIsrEnable(bool enable)
{
   if (enable)
   {
      enable_interrupts(INT_RDA2);
   }
   else
   {
      disable_interrupts(INT_RDA2);
   }
}
#endif

void UART2Init(uint32_t baud)
{
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(false);
  #endif
  
   if (baud==300)  setup_uart(300, STREAM_UART_UART2);
   else if (baud==1200)  setup_uart(1200, STREAM_UART_UART2);
   else if (baud==2400)  setup_uart(2400, STREAM_UART_UART2);
   else if (baud==4800)  setup_uart(4800, STREAM_UART_UART2);
   else if (baud==9600)  setup_uart(9600, STREAM_UART_UART2);
   else if (baud==19200)  setup_uart(19200, STREAM_UART_UART2);
   else if (baud==38400)  setup_uart(38400, STREAM_UART_UART2);
   else if (baud==57600)  setup_uart(57600, STREAM_UART_UART2);
   else if (baud==115200)  setup_uart(115200, STREAM_UART_UART2);

  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(true);
  #endif
}

void UART2Putc(char c)
{
   fputc(c, STREAM_UART_UART2);
}

static char UART2Getc(void)
{
   return(fgetc(STREAM_UART_UART2));
}

uart2_rx_size_t UART2Kbhit(void)
{
  #if (UART2_RX_BUFFER_SIZE > 0)
   return(rcv_buffer_bytes(STREAM_UART_UART2));
  #else
   return(kbhit(STREAM_UART_UART2));
  #endif
}

   //END section: CCS C Compilers
#endif

   
#if defined(__XC16) || defined(__XC32)
   //BEGIN section: Microchip XC16 or XC32 compiler

static uint32_t _g_UART2Baud;

static void _UART2OpenPerif(uint32_t baud) 
{
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(false);
  #endif
  
   _g_UART2Baud = baud;
  
   U2MODE = 0x8008;
   U2STA = 0x0000;
   U2BRG = (SYS_CLK_FrequencyPeripheralGet() / (4 * baud)) - 1;
   U2STAbits.UTXEN = 1;
   
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(true);
  #endif   
}

void UART2Init(uint32_t baud)
{  
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART2RxBuffer, 0, sizeof(_g_UART2RxBuffer));
  #endif

   _UART2OpenPerif(baud);
}

static bool _UART2PhyKbhit(void) 
{
   return(U2STAbits.URXDA);
}

static char _UART2PhyGetc(void) 
{
   while (!_UART2PhyKbhit());
   
   if (U2STAbits.OERR)
   {
      // if overrun, reset the peripheral
      _UART2OpenPerif(_g_UART2Baud);
   }
   
   return(U2RXREG);
}

void UART2Putc(char c) 
{
   while(U2STAbits.UTXBF == 1);
   U2TXREG = c;
}

#if (UART2_RX_BUFFER_SIZE > 0)
static void _UART2PhyRxIsrEnable(bool enable) 
{
   IEC1bits.U2RXIE = enable;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U2RXInterrupt ( void )
{
   IFS1bits.U2RXIF = 0;
   UART2RxIsrTask();
}
#endif   

   //END section: Microchip XC16 or XC32 compiler
#endif

#if defined(__XC8)
   //BEGIN section: Microchip XC8 compiler with plib   
 
static uint32_t _g_UART2Baud;
 
static void _UART2OpenPerif(uint32_t baud)
{
   _g_UART2Baud = baud;

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
  
  #if (UART2_RX_BUFFER_SIZE > 0)
   #define __UART2_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
   
   _UART2PhyRxIsrEnable(false);
  #else
   #define __UART2_BASE_OPEN_CONFIG USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_ADDEN_OFF & USART_CONT_RX & USART_EIGHT_BIT
  #endif

  #if !defined(_BAUDCON2_BRG16_POSN) && !defined(_BAUDCON2_BRG16_POSITION)
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
   if (spbrg > 255)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/64 - 1;
      config = __UART2_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART2_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }
  #else
   BAUDCON2 |= (1 << _BAUDCON2_BRG16_POSN);
   spbrg = (SYS_CLK_FrequencySystemGet()/baud)/4 - 1;
   if (spbrg > 0xFFFF)
   {
      spbrg = (SYS_CLK_FrequencySystemGet()/baud)/16 - 1;
      config = __UART2_BASE_OPEN_CONFIG & USART_BRGH_LOW;
   }
   else
   {
      config = __UART2_BASE_OPEN_CONFIG & USART_BRGH_HIGH;
   }   
  #endif
   
   Open2USART(config, spbrg);
   
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(true);
  #endif
}
 
void UART2Init(uint32_t baud)
{  
  #if (UART2_RX_BUFFER_SIZE > 0)
   _UART2PhyRxIsrEnable(false);
   
   memset((void*)&_g_UART2RxBuffer, 0, sizeof(_g_UART2RxBuffer));
  #endif

   _UART2OpenPerif(baud);
}

static bool _UART2PhyKbhit(void)
{
   return(DataRdy2USART());
}

static char _UART2PhyGetc(void)
{
   if (USART2_Status.OVERRUN_ERROR)
   {
      // if overrun, reset the peripheral
      _UART2OpenPerif(_g_UART2Baud);
   }
   
   return(getc2USART());
}

void UART2Putc(char c)
{
   while(Busy2USART());
   putc2USART(c);
}

#if (UART2_RX_BUFFER_SIZE > 0)
static void _UART2PhyRxIsrEnable(bool enable)
{
   PIE3bits.RC2IE = enable;
}   
#endif

   //END section: Microchip XC8 compiler with plib
#endif

void UART2Puts(char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART2Putc(c);
   }
}

void UART2PutsROM(rom char *p)
{
   char c;
   for(;;)
   {
      c = *p++;
      if (!c)
         break;
      UART2Putc(c);
   }
}

#endif   //ifndef __UART2_C__
