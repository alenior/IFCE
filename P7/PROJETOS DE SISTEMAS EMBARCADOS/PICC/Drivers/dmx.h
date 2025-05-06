////////////////////////////////////////////////////////////////////////////////
////                                 DMX.h                                  ////
////                                                                        ////
//// Includes, defines, variables and prototypes for the DMX library.       ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services                ////
//// This source code may only be used by licensed users of the CCS         ////
//// C compiler.  This source code may only be distributed to other         ////
//// licensed users of the CCS C compiler.  No other use,                   ////
//// reproduction or distribution is permitted without written              ////
//// permission.  Derivative programs created using this software           ////
//// in object code form are not restricted in any way.                     ////
////////////////////////////////////////////////////////////////////////////////

#ifndef _DMX_H
#define _DMX_H

#include <stdint.h>

#define DMX_CONTROLLER     1
#define DMX_SLAVE_DEVICE   0

#define MAX_DMA_CHANNELS      512

#ifndef DMX_TYPE
 #define DMX_TYPE       DMX_CONTROLLER
#endif

#if DMX_TYPE == DMX_CONTROLLER
#ifndef DMX_XMIT
 #define DMX_XMIT       PIN_B3
#endif

#ifndef DMX_ENABLE
 #define DMX_ENABLE     PIN_B4
#endif

#ifndef DMX_UNITS
 #define DMX_UNITS      1
#endif

#ifndef DMX_CHANNELS
 #define DMX_CHANNELS   7
#endif

#if (DMX_CHANNELS*DMX_UNITS) > MAX_DMA_CHANNELS
 #error Max number of DMX channels is MAX_DMA_CHANNELS
#endif

// DMX Controller Variables
uint8_t DMXData[DMX_UNITS][DMX_CHANNELS];

//sets up software UART for DMX protocol
#use rs232(xmit=DMX_XMIT,BAUD=250000,BITS=8,PARITY=N,STOP=2,DISABLE_INTS,stream=DMX_CONTROLLER_STREAM)

//DMX Controller Prototypes
void DMXInit(void);
void DMXSetChannel(uint8_t channel, uint8_t value);
uint8_t DMXGetChannel(uint8_t channel);
void DMXCommit(void);
#else //DMX Slave Device

#ifndef DMX_INT
 #define DMX_INT  INT_RDA
#endif

#ifndef DMX_RCV
 #define DMX_RCV  PIN_C7
#endif

#ifndef DMX_START_CHANNEL
 #define DMX_START_CHANNEL    1
#endif

#ifndef DMX_CHANNELS
 #define DMX_CHANNELS         1  
#endif

#if (DMX_START_CHANNEL + DMX_CHANNELS) > (MAX_DMA_CHANNELS + 1)
 #define MAX_DMX_SLAVE_CHANNELS  ((MAX_DMA_CHANNELS + 1) - DMX_START_CHANNEL)
 #error Max number of DMX channels starting from channel DMX_START_CHANNEL is MAX_DMX_SLAVE_CHANNELS
#endif

#if DMX_INT == INT_RDA
 #define DMX_UART UART1
#elif DMX_INT == INT_RDA2
 #define DMX_UART UART2
#elif DMX_INT == INT_RDA3
 #define DMX_UART UART3
#elif DMX_INT == INT_RDA4
 #define DMX_UART UART4
#endif

#ifndef DMX_BUFFERS
 #define DMX_BUFFERS          3
#endif

typedef enum {DMX_RX_MODE_MARK, DMX_RX_MODE_START, DMX_RX_MODE_DATA} DMX_RX_MODE;

// Setup DMX Slave HW UART from DMX protocol
#use rs232(DMX_UART,BAUD=250000,BITS=8,PARITY=N,STOP=2,stream=DMX_SLAVE_STREAM)

// DMX Slave Variables
uint8_t dmx_RxData[DMX_BUFFERS][DMX_CHANNELS];
uint8_t dmx_NextIn;
uint8_t dmx_NextOut;
DMX_RX_MODE dmx_RxMode;

//DMX Slave Prototypes
void DMXInit(void);
int1 DMXKbhit(void);
void DMXGetd(uint8_t *Data, uint16_t Count);

#endif

#endif
