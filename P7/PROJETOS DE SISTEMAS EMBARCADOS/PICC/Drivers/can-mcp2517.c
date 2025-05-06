///////////////////////////////////////////////////////////////////////////////
////                             can-mcp2517.c                             ////
////                                                                       ////
////  Driver for Microchip's MCP2517FD external CAN FD controller with SPI ////
////  interface.                                                           ////
////                                                                       ////
////  See can-mcp2517.h for API.                                           ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2019 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

#ifndef __CAN_MCP2517_C__
#define __CAN_MCP2517_C__

#include <can-mcp2517.h>

/////////////////////////// MCP2517FD Clock Settings //////////////////////////

#ifndef MCP2517_EXT_CLOCK_SPEED
 #define MCP2517_EXT_CLOCK_SPEED    20000000 //4 MHz, 20 MHz or 40 MHz
#endif

#if (MCP2517_EXT_CLOCK_SPEED != 4000000) && (MCP2517_EXT_CLOCK_SPEED != 20000000) && (MCP2517_EXT_CLOCK_SPEED != 40000000)
 #error The MCP2517 can only be clocked from an external 4 MHz, 20 MHz or 40 MHz crystal
#endif

#if MCP2517_EXT_CLOCK_SPEED == 4000000
 #define MCP2517_ENABLE_PLL      TRUE
#else
 #define MCP2517_ENABLE_PLL      FALSE
#endif

#ifndef MCP2517_ENABLE_CLOCK_OUTPUT
 #define MCP2517_ENABLE_CLOCK_OUTPUT      FALSE
#endif

#ifndef MCP2517_CLOCK_OUT_DIVISOR
 #define MCP2517_CLOCK_OUT_DIVISOR        1  //1, 2, 4 or 10
#endif

#ifndef MCP2517_SYSTEM_CLOCK_DIVISOR
 #define MCP2517_SYSTEM_CLOCK_DIVISOR     1  //1 or 2
#endif

#if MCP2517_ENABLE_PLL == TRUE
 #define MCP2517_SYSTEM_CLOCK    ((MCP2517_EXT_CLOCK_SPEED * 10 ) / MCP2517_SYSTEM_CLOCK_DIVISOR)
#else
 #define MCP2517_SYSTEM_CLOCK    (MCP2517_EXT_CLOCK_SPEED / MCP2517_SYSTEM_CLOCK_DIVISOR)
#endif

//////////////////////////// MCP2517FD SPI Settings ///////////////////////////

#if !definedinc(MCP2517Xfer)
 #ifndef MCP2517_SPI_STREAM
  #ifndef MCP2517_SPI_SCK_PIN
   #define MCP2517_SPI_SCK_PIN   PIN_C3
  #endif
 
  #ifndef MCP2517_SPI_SDI_PIN
   #define MCP2517_SPI_SDI_PIN   PIN_C5   //MOSI
  #endif
 
  #ifndef MCP2517_SPI_SDO_PIN
   #define MCP2517_SPI_SDO_PIN   PIN_C4   //MISO
  #endif
 
  #ifndef MCP2517_SPI_BAUD
   #if MCP2517_SYSTEM_CLOCK == 40000000
    #define MCP2517_SPI_BAUD     20000000 //20MHz is max supported baud rate
   #elif MCP2517_SYSTEM_CLOCK == 20000000
    #define MCP2517_SPI_BAUD     10000000
   #else
    #define MCP2517_SPI_BAUD     5000000
   #endif
  #endif
 
  #use spi(MASTER, CLK=MCP2517_SPI_SCK_PIN, DI=MCP2517_SPI_SDO_PIN, DO=MCP2517_SPI_SDI_PIN, BAUD=MCP2517_SPI_BAUD, BITS=8, MODE=0, STREAM=MCP2517_SPI_STREAM)
 #endif
 
 #define MCP2517Xfer(a)    spi_xfer(MCP2517_SPI_STREAM, a)
#endif

#ifndef MCP2517_SPI_CS_PIN
 #define MCP2517_SPI_CS_PIN      PIN_B0
#endif

#ifdef MCP2517_SPI_USE_CRC
 #if getenv("CRC") > 0
  #ifndef MCP2517_SPI_FORCE_SW_CRC
   #define MCP2517_SPI_FORCE_SW_CRC    FALSE
  #endif
 #else
  #ifndef MCP2517_SPI_FORCE_SW_CRC
   #define MCP2517_SPI_FORCE_SW_CRC    TRUE
  #elif MCP2517_SPI_FORCE_SW_CRC == FALSE
   #undef MCP2517_SPI_FORCE_SW_CRC
   #define MCP2517_SPI_FORCE_SW_CRC    TRUE
  #endif
 #endif
 
 #if MCP2517_SPI_FORCE_SW_CRC == TRUE
  #ifndef MCP2517_SPI_USE_LOOKUP_TABLE
   #define MCP2517_SPI_USE_LOOKUP_TABLE   TRUE
  #endif
  
  #if MCP2517_SPI_USE_LOOKUP_TABLE == TRUE
   #ifndef MCP2517_SPI_USE_RAM_LOOKUP_TABLE
    #define MCP2517_SPI_USE_RAM_LOOKUP_TABLE    FALSE
   #endif
  #endif
 #endif
 
 #ifndef MCP2517_SPI_DO_SAFE_WRITE
  #define MCP2517_SPI_DO_SAFE_WRITE    FALSE
 #endif
#endif

#ifndef MCP2517_INT_OUTPUT_MODE
 #define MCP2517_INT_OUTPUT_MODE    1  //0-Push/Pull, 1-Open Drain
#endif

#ifndef MCP2517_TXCAN_OUTPUT_MODE
 #define MCP2517_TXCAN_OUTPUT_MODE  0  //0-Push/Pull, 1-Open Drain
#endif

#ifndef MCP2517_CLKO_TYPE
 #define MCP2517_CLKO_TYPE          0  //0-Clock on CLKO pin, 1-SOF signal on CLKO pin
#endif

#ifndef MCP2517_INT0_PIN
 #ifndef MCP2517_XSTBY
  #define MCP2517_XSTBY             0  //0-XSTBY control disabled, 1-XSTBY control enabled
 #endif

 #if (MCP2517_XSTBY == 1)
  #ifdef MCP2517_TRIS0
   #if MCP2517_TRIS0 != 0
    #undef MCP2517_TRIS0
    #define MCP2517_TRIS0           0
   #endif
  #else
   #define MCP2517_TRIS0            0
  #endif
 #else
  #ifndef MCP2517_TRIS0
   #define MCP2517_TRIS0            1  //0-GPIO0 Output pin, 1-GPIO0 Input pin
  #endif
  
  #if (MCP2517_TRIS0 == 0)
   #ifndef MCP2517_DEFAULT_LAT0
    #define MCP2517_DEFAULT_LAT0    0  //0-Drive pin low, 1-Drive pin high
   #endif
  #endif
 #endif
#endif

#ifndef MCP2517_INT1_PIN
 #ifndef MCP2517_TRIS1
  #define MCP2517_TRIS1             1  //0-GPIO1 Output pin, 1-GPIO1 Input pin
 #endif
 
 #if (MCP2517_TRIS1 == 0)
  #ifndef MCP2517_DEFAULT_LAT1
   #define MCP2517_DEFAULT_LAT1     0  //0-Drive pin low, 1-Drive pin high
  #endif
 #endif
#endif

///////////////////////////// MCP2517 CAN Settings ////////////////////////////

//CAN Baud Rate Setting
#if MCP2517_ENABLE_PLL == TRUE
 #define CAN_CLOCK_SPEED      (MCP2517_EXT_CLOCK_SPEED * 10)
#else
 #define CAN_CLOCK_SPEED      MCP2517_EXT_CLOCK_SPEED
#endif

#ifndef CAN_NOMINAL_BAUD_RATE
 #define CAN_NOMINAL_BAUD_RATE   250000      //The Nominal CAN Baud Rate
#endif

#ifndef CAN_DATA_BAUD_RATE
 #define CAN_DATA_BAUD_RATE      2000000     //The Data CAN Baud Rate, when using CANFD
#endif

#if (CAN_NOMINAL_BAUD_RATE > 1000000)
 #error CAN Nominal Baud Rate max is 1000000
#endif

#if (CAN_CLOCK_SPEED % CAN_NOMINAL_BAUD_RATE) != 0
 #error CAN Clock Speed not evenly divisible by CAN Nominal Baud Rate
#endif

#if (CAN_DATA_BAUD_RATE > 8000000)
 #error CAN Data Baud Rate max is 8000000
#endif

#if (CAN_CLOCK_SPEED % CAN_DATA_BAUD_RATE) != 0
 #error CAN Clock Speed not evenly disisiable by CAN Data Baud Rate
#endif

#ifndef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT      875   //in tenth of a percent, e.g. 875 = 87.5% of bit time
#endif

//CAN Peripheral Settings

#ifndef CAN_TDC_VALUE
 #define CAN_TDC_VALUE  0  //0 to 63 (CAN_TDC_VALUE * System Clock)
#endif

#ifndef CAN_TDC_OFFSET
 #define CAN_TDC_OFFSET 0  //-64 to 63 (CAN_TDC_OFFSET * System Clock)
#endif

#ifndef CAN_TDC_MODE
 #define CAN_TDC_MODE   0  //0-Disabled, 1-Manual (use CAN_TDC_VALUE and CAN_TDC_OFFSET), 2 or 3 - Auto
#endif

#ifndef CAN_SID11
 #define CAN_SID11      0  //0-SID <10:0>, 1-SID <11:0>
#endif

#ifndef CAN_EDGE_FLT
 #define CAN_EDGE_FLT   0  //0-Edge Filtering disabled, 1-Edge Filtering enabled
#endif

#ifndef CAN_TIME_BASE_COUNTER_PRESCALAR
 #define CAN_TIME_BASE_COUNTER_PRESCALAR     1  //1 to 1024
#endif

#ifndef CAN_TIME_BASE_COUNTER_ENABLE
 #define CAN_TIME_BASE_COUNTER_ENABLE        0  //0-Stop and Reset, 1-Enable
#endif

#ifndef CAN_TIME_STAMP_EOF
 #define CAN_TIME_STAMP_EOF   0  //0-Time Stamp at beginning of Frame, 1-Time Stamp when frame is taken valid
#endif

#ifndef CAN_TIME_STAMP_RES
 #define CAN_TIME_STAMP_RES   0  //0-at sample point of SOF, 1-at sample point of the bit following the FDF bit (FD Frames only)
#endif

#ifndef CAN_OPERATION_MODE
 #define CAN_OPERATION_MODE   0  //0-CAN FD, 1-CAN 2.0
#endif

#ifndef CAN_DEVICE_NET_FILTER_BIT_NUMBER
 #define CAN_DEVICE_NET_FILTER_BIT_NUMBER    0  //when set for receiving Standard ID Messages this sets the number of bits the EID part
#endif                                          //of the Filter and Mask Object can be selected to filter on data bytes (0-18)

#ifndef CAN_ENABLE_ISO_CRC
 #define CAN_ENABLE_ISO_CRC   FALSE //TRUE-use ISO CRC in CAN FD Frames
#endif

#ifndef CAN_DISABLE_EXCEPTION_EVENT
 #define CAN_DISABLE_EXCEPTION_EVENT   FALSE //TRUE-Protocol Exception is treated as a Form Error, FALSE-Protocol Exception causes
#endif                                       //controller to enter Bus Integration state

#ifndef CAN_ENABLE_WAKE_FILTER
 #define CAN_ENABLE_WAKE_FILTER  FALSE    //TRUE-Use CAN bus line filter to wake-up, FALSE-CAN bus line filter not used to wake-up
#endif

#ifndef CAN_WAKE_FILTER_TIME
 #define CAN_WAKE_FILTER_TIME    0  //0-T00FILTER, 1-T01FILTER, 2-T10FILTER, 3-T11FILTER
#endif

#ifndef CAN_DISABLE_BIT_RATE_SWITCHING
 #define CAN_DISABLE_BIT_RATE_SWITCHING   FALSE //TRUE-Bit Rate Switching Disabled, FALSE-Bit Rate Switching depends on Transmit Message Object
#endif

#ifndef CAN_RESTRICT_RETANSMISSION_ATTEMPTS
 #define CAN_RESTRICT_RETANSMISSION_ATTEMPTS    FALSE //TRUE-Restrict retransmission attempts, FALSE-unlimited number of retransmission attempts
#endif

#ifndef CAN_TRANSMIT_ESI_MODE
 #define CAN_TRANSMIT_ESI_MODE      0   //0-ESI reflects error status of CAN controller, 1-ESI is transmitted recessive
#endif

#ifndef CAN_TRANSITION_TO_LISTEN
 #define CAN_TRANSITION_TO_LISTEN   0  //0-Transition to Restricted Operation Mode on System Error, 1-Transition to Listen Only Mode on System Error
#endif

#ifndef CAN_TRANSMISSION_DELAY
 #define CAN_TRANSMISSION_DELAY     TRANSMISSION_DELAY_NONE  //Delay between to consecutive transmissions, see TRANSMISSION_DELAY type below for options
#endif

///////////////////////////// CAN Object Settings /////////////////////////////

#ifndef CAN_ENABLE_TX_EVENT_OBJECT
 #define CAN_ENABLE_TX_EVENT_OBJECT    FALSE
#endif

#if CAN_ENABLE_TX_EVENT_OBJECT == TRUE
 #ifndef CAN_TX_EVENT_FIFO_SIZE
  #define CAN_TX_EVENT_FIFO_SIZE    1     //1 to 32 FIFOs, uses 8 or 12 bytes of RAM depending on whether Time Stamp is enabled
 #endif
 
 #ifndef CAN_TX_EVENT_TIME_STAMP
  #define CAN_TX_EVENT_TIME_STAMP   FALSE //TRUE-enable Time Stamp, FALSE-disable Time Stamp
 #endif
#endif

/*
   The CAN_TX_BUFFERS define sets the size of the FIFO for the TX Queue Object, 
   defining it to 0 will disable the TX Queue Object.  
*/
#ifndef CAN_TX_BUFFERS
 #define CAN_TX_BUFFERS       1     //0 to 32, This driver uses the TX Queue Object to send messages
#endif

#if CAN_TX_BUFFERS > 0
 /*
   The following defines, CAN_TX_MAX_PAYLOAD_SIZE, CAN_TX_PRIORITY and 
   CAN_TX_RETRANSMISSION_ATTEMPTS, are for the TX Queue Object.
 */
 #ifndef CAN_TX_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_TX_MAX_PAYLOAD_SIZE     PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_TX_MAX_PAYLOAD_SIZE     PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #ifndef CAN_TX_PRIORITY
  #define CAN_TX_PRIORITY     31    //0 - 31, 0-lowest priority, 31-highest priority.
 #endif
 
 #ifndef CAN_TX_RETRANSMISSION_ATTEMPTS
  #define CAN_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //see RETRANSMIT_ATTEMPTS type below for settings that can be made.
 #endif                                                                    //Only used if CAN_RESTRICT_RETRANSMISSION_ATTEMPTS is set to TRUE.
 
 #define CAN_HAS_TX_OBJECT
#endif

/*
   The CAN_RX_BUFFERS define sets up FIFO Object 1 as a RX Object and sets
   the size of the FIFO for it.  If it is desired to use the FIFO Object 1 as a
   TX Object then set this define to 0 and define CAN_USE_FIFO_1_OBJECT and its
   corresponding defines to set it up as a TX Object.
*/
#ifndef CAN_RX_BUFFERS
 #define CAN_RX_BUFFERS       16    // 0 - 32, This driver uses FIFO 1 for receiving messages
#endif

#if CAN_RX_BUFFERS > 0
 /*
   The following defines, CAN_RX_MAX_PAYLOAD_SIZE and CAN_RX_TIME_STAMP, are 
   for setting the FIFO Object 1 RX settings when CAN_RX_BUFFERS is defined to
   a value greater then 0.
 */
 #ifndef CAN_RX_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_RX_MAX_PAYLOAD_SIZE     PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_RX_MAX_PAYLOAD_SIZE     PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #ifndef CAN_RX_TIME_STAMP
  #define CAN_RX_TIME_STAMP      FALSE //TRUE-Capture time stamp for received messages, FALSE-Don't capture time stamp
 #endif
 
 #define CAN_HAS_RX_OBJECT
#endif

/*
   If it is desired to use FIFO Object 1 as a TX Object, or to use FIFO Objects
   2-31 the following defines can be made to before this driver is included to
   enable them:
      #define CAN_USE_FIFO_x_OBJECT                               // x being the FIFO Object to enable (1-31)
      #define CAN_FIFO_x_OBJECT_TYPE                        aa    // x being the FIFO Object (1-31), and aa being 0-RX Object or 1-TX Object
      #define CAN_FIFO_x_OBJECT_COUNT                       bb    // x begin the FIFO Object (1-31), and bb being the number of FIFOs Object has (1-32)
      #define CAN_FIFO_x_OBJECT_MAX_PAYLOAD_SIZE            cc    // x being the FIFO Object (1-31), and cc being PAYLOAD_SIZE type.
      #define CAN_FIFO_x_OBJECT_TX_PRIORITY                 dd    // x being the FIFO Object (1-31), and dd being 0 to 31 (0-lowest priority, 31-highest priority)
                                                                  // Only needed when Object is a TX Object
      #define CAN_FIFO_x_OBJECT_TX_RETRANSMISSION_ATTEMPTS  ee    // x being the FIFO Object (1-31), and ee being RETRANSMIT_ATTEMPTS type.
                                                                  // Only needed when Object is a TX Object
      #define CAN_FIFO_x_OBJECT_RX_TIME_STAMP               ff    // x being the FIFO Object (1-31), and ff being TRUE-Capture time stamped or FALSE Don't capture time stamp.
                                                                  // Only needed when Object is a RX Object
*/

// FIFO Object 1
#ifdef CAN_USE_FIFO_1_OBJECT
 #if CAN_RX_BUFFERS > 0
  #error Can not use FIFO 1 Object when CAN_RX_BUFFERS is defined to a value greater then 0
 #endif
 
 #ifndef CAN_FIFO_1_OBJECT_TYPE
  #define CAN_FIFO_1_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_1_OBJECT_COUNT
  #define CAN_FIFO_1_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_1_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_1_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_1_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_1_OBJECT_TX_PRIORITY
   #define CAN_FIFO_1_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_1_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_1_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 2
#ifdef CAN_USE_FIFO_2_OBJECT
 #ifndef CAN_FIFO_2_OBJECT_TYPE
  #define CAN_FIFO_2_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_2_OBJECT_COUNT
  #define CAN_FIFO_2_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_2_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_2_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_2_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_2_OBJECT_TX_PRIORITY
   #define CAN_FIFO_2_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_2_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_2_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 3
#ifdef CAN_USE_FIFO_3_OBJECT
 #ifndef CAN_FIFO_3_OBJECT_TYPE
  #define CAN_FIFO_3_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_3_OBJECT_COUNT
  #define CAN_FIFO_3_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_3_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_3_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_3_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_3_OBJECT_TX_PRIORITY
   #define CAN_FIFO_3_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_3_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_3_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 4
#ifdef CAN_USE_FIFO_4_OBJECT
 #ifndef CAN_FIFO_4_OBJECT_TYPE
  #define CAN_FIFO_4_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_4_OBJECT_COUNT
  #define CAN_FIFO_4_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_4_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_4_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_4_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_4_OBJECT_TX_PRIORITY
   #define CAN_FIFO_4_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_4_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_4_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 5
#ifdef CAN_USE_FIFO_5_OBJECT
 #ifndef CAN_FIFO_5_OBJECT_TYPE
  #define CAN_FIFO_5_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_5_OBJECT_COUNT
  #define CAN_FIFO_5_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_5_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_5_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_5_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_5_OBJECT_TX_PRIORITY
   #define CAN_FIFO_5_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_5_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_5_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 6
#ifdef CAN_USE_FIFO_6_OBJECT
 #ifndef CAN_FIFO_6_OBJECT_TYPE
  #define CAN_FIFO_6_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_6_OBJECT_COUNT
  #define CAN_FIFO_6_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_6_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_6_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_6_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_6_OBJECT_TX_PRIORITY
   #define CAN_FIFO_6_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_6_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_6_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 7
#ifdef CAN_USE_FIFO_7_OBJECT
 #ifndef CAN_FIFO_7_OBJECT_TYPE
  #define CAN_FIFO_7_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_7_OBJECT_COUNT
  #define CAN_FIFO_7_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_7_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_7_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_7_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_7_OBJECT_TX_PRIORITY
   #define CAN_FIFO_7_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_7_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_7_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 8
#ifdef CAN_USE_FIFO_8_OBJECT
 #ifndef CAN_FIFO_8_OBJECT_TYPE
  #define CAN_FIFO_8_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_8_OBJECT_COUNT
  #define CAN_FIFO_8_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_8_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_8_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_8_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_8_OBJECT_TX_PRIORITY
   #define CAN_FIFO_8_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_8_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_8_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 9
#ifdef CAN_USE_FIFO_9_OBJECT
 #ifndef CAN_FIFO_9_OBJECT_TYPE
  #define CAN_FIFO_9_OBJECT_TYPE    0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_9_OBJECT_COUNT
  #define CAN_FIFO_9_OBJECT_COUNT   32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE   PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_9_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_9_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_9_OBJECT_RX_TIME_STAMP   FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_9_OBJECT_TX_PRIORITY
   #define CAN_FIFO_9_OBJECT_TX_PRIORITY     31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_9_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_9_OBJECT_TX_RETRANSMISSION_ATTEMPTS     RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 10
#ifdef CAN_USE_FIFO_10_OBJECT
 #ifndef CAN_FIFO_10_OBJECT_TYPE
  #define CAN_FIFO_10_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_10_OBJECT_COUNT
  #define CAN_FIFO_10_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_10_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_10_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_10_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_10_OBJECT_TX_PRIORITY
   #define CAN_FIFO_10_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_10_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_10_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 11
#ifdef CAN_USE_FIFO_11_OBJECT
 #ifndef CAN_FIFO_11_OBJECT_TYPE
  #define CAN_FIFO_11_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_11_OBJECT_COUNT
  #define CAN_FIFO_11_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_11_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_11_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_11_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_11_OBJECT_TX_PRIORITY
   #define CAN_FIFO_11_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_11_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_11_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 12
#ifdef CAN_USE_FIFO_12_OBJECT
 #ifndef CAN_FIFO_12_OBJECT_TYPE
  #define CAN_FIFO_12_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_12_OBJECT_COUNT
  #define CAN_FIFO_12_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_12_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_12_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_12_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_12_OBJECT_TX_PRIORITY
   #define CAN_FIFO_12_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_12_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_12_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 13
#ifdef CAN_USE_FIFO_13_OBJECT
 #ifndef CAN_FIFO_13_OBJECT_TYPE
  #define CAN_FIFO_13_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_13_OBJECT_COUNT
  #define CAN_FIFO_13_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_13_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_13_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_13_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_13_OBJECT_TX_PRIORITY
   #define CAN_FIFO_13_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_13_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_13_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 14
#ifdef CAN_USE_FIFO_14_OBJECT
 #ifndef CAN_FIFO_14_OBJECT_TYPE
  #define CAN_FIFO_14_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_14_OBJECT_COUNT
  #define CAN_FIFO_14_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_14_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_14_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_14_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_14_OBJECT_TX_PRIORITY
   #define CAN_FIFO_14_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_14_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_14_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 15
#ifdef CAN_USE_FIFO_15_OBJECT
 #ifndef CAN_FIFO_15_OBJECT_TYPE
  #define CAN_FIFO_15_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_15_OBJECT_COUNT
  #define CAN_FIFO_15_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_15_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_15_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_15_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_15_OBJECT_TX_PRIORITY
   #define CAN_FIFO_15_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_15_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_15_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 16
#ifdef CAN_USE_FIFO_16_OBJECT
 #ifndef CAN_FIFO_16_OBJECT_TYPE
  #define CAN_FIFO_16_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_16_OBJECT_COUNT
  #define CAN_FIFO_16_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_16_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_16_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_16_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_16_OBJECT_TX_PRIORITY
   #define CAN_FIFO_16_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_16_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_16_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 17
#ifdef CAN_USE_FIFO_17_OBJECT
 #ifndef CAN_FIFO_17_OBJECT_TYPE
  #define CAN_FIFO_17_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_17_OBJECT_COUNT
  #define CAN_FIFO_17_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_17_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_17_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_17_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_17_OBJECT_TX_PRIORITY
   #define CAN_FIFO_17_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_17_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_17_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 18
#ifdef CAN_USE_FIFO_18_OBJECT
 #ifndef CAN_FIFO_18_OBJECT_TYPE
  #define CAN_FIFO_18_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_18_OBJECT_COUNT
  #define CAN_FIFO_18_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_18_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_18_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_18_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_18_OBJECT_TX_PRIORITY
   #define CAN_FIFO_18_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_18_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_18_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 19
#ifdef CAN_USE_FIFO_19_OBJECT
 #ifndef CAN_FIFO_19_OBJECT_TYPE
  #define CAN_FIFO_19_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_19_OBJECT_COUNT
  #define CAN_FIFO_19_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_19_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_19_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_19_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_19_OBJECT_TX_PRIORITY
   #define CAN_FIFO_19_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_19_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_19_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 20
#ifdef CAN_USE_FIFO_20_OBJECT
 #ifndef CAN_FIFO_20_OBJECT_TYPE
  #define CAN_FIFO_20_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_20_OBJECT_COUNT
  #define CAN_FIFO_20_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_20_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_20_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_20_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_20_OBJECT_TX_PRIORITY
   #define CAN_FIFO_20_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_20_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_20_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 21
#ifdef CAN_USE_FIFO_21_OBJECT
 #ifndef CAN_FIFO_21_OBJECT_TYPE
  #define CAN_FIFO_21_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_21_OBJECT_COUNT
  #define CAN_FIFO_21_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_21_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_21_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_21_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_21_OBJECT_TX_PRIORITY
   #define CAN_FIFO_21_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_21_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_21_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 22
#ifdef CAN_USE_FIFO_22_OBJECT
 #ifndef CAN_FIFO_22_OBJECT_TYPE
  #define CAN_FIFO_22_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_22_OBJECT_COUNT
  #define CAN_FIFO_22_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_22_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_22_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_22_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_22_OBJECT_TX_PRIORITY
   #define CAN_FIFO_22_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_22_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_22_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 23
#ifdef CAN_USE_FIFO_23_OBJECT
 #ifndef CAN_FIFO_23_OBJECT_TYPE
  #define CAN_FIFO_23_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_23_OBJECT_COUNT
  #define CAN_FIFO_23_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_23_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_23_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_23_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_23_OBJECT_TX_PRIORITY
   #define CAN_FIFO_23_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_23_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_23_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 24
#ifdef CAN_USE_FIFO_24_OBJECT
 #ifndef CAN_FIFO_24_OBJECT_TYPE
  #define CAN_FIFO_24_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_24_OBJECT_COUNT
  #define CAN_FIFO_24_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_24_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_24_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_24_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_24_OBJECT_TX_PRIORITY
   #define CAN_FIFO_24_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_24_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_24_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 25
#ifdef CAN_USE_FIFO_25_OBJECT
 #ifndef CAN_FIFO_25_OBJECT_TYPE
  #define CAN_FIFO_25_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_25_OBJECT_COUNT
  #define CAN_FIFO_25_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_25_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_25_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_25_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_25_OBJECT_TX_PRIORITY
   #define CAN_FIFO_25_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_25_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_25_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 26
#ifdef CAN_USE_FIFO_26_OBJECT
 #ifndef CAN_FIFO_26_OBJECT_TYPE
  #define CAN_FIFO_26_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_26_OBJECT_COUNT
  #define CAN_FIFO_26_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_26_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_26_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_26_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_26_OBJECT_TX_PRIORITY
   #define CAN_FIFO_26_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_26_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_26_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 27
#ifdef CAN_USE_FIFO_27_OBJECT
 #ifndef CAN_FIFO_27_OBJECT_TYPE
  #define CAN_FIFO_27_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_27_OBJECT_COUNT
  #define CAN_FIFO_27_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_27_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_27_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_27_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_27_OBJECT_TX_PRIORITY
   #define CAN_FIFO_27_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_27_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_27_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 28
#ifdef CAN_USE_FIFO_28_OBJECT
 #ifndef CAN_FIFO_28_OBJECT_TYPE
  #define CAN_FIFO_28_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_28_OBJECT_COUNT
  #define CAN_FIFO_28_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_28_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_28_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_28_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_28_OBJECT_TX_PRIORITY
   #define CAN_FIFO_28_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_28_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_28_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 29
#ifdef CAN_USE_FIFO_29_OBJECT
 #ifndef CAN_FIFO_29_OBJECT_TYPE
  #define CAN_FIFO_29_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_29_OBJECT_COUNT
  #define CAN_FIFO_29_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_29_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_29_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_29_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_29_OBJECT_TX_PRIORITY
   #define CAN_FIFO_29_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_29_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_29_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 30
#ifdef CAN_USE_FIFO_30_OBJECT
 #ifndef CAN_FIFO_30_OBJECT_TYPE
  #define CAN_FIFO_30_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_30_OBJECT_COUNT
  #define CAN_FIFO_30_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_30_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_30_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_30_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_30_OBJECT_TX_PRIORITY
   #define CAN_FIFO_30_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_30_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_30_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

// FIFO Object 31
#ifdef CAN_USE_FIFO_31_OBJECT
 #ifndef CAN_FIFO_31_OBJECT_TYPE
  #define CAN_FIFO_31_OBJECT_TYPE   0  //defaults to an RX Object
 #endif
 
 #ifndef CAN_FIFO_31_OBJECT_COUNT
  #define CAN_FIFO_31_OBJECT_COUNT  32 //default to max number of FIFOs
 #endif
 
 #ifndef CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE
  #if CAN_OPERATION_MODE == 1 //CAN 2.0
   #define CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_8_BYTES    //max data bytes for CAN 2.0 messages is 8 bytes
  #else
   #define CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE  PAYLOAD_SIZE_64_BYTES   //see PAYLOAD_SIZE type below for size that can be set
  #endif
 #endif
 
 #if CAN_FIFO_31_OBJECT_TYPE == 0
  #ifndef CAN_FIFO_31_OBJECT_RX_TIME_STAMP
   #define CAN_FIFO_31_OBJECT_RX_TIME_STAMP     FALSE //defaults to no Time Stamp when an RX Object
  #endif
  
  #ifndef CAN_HAS_RX_OBJECT
   #define CAN_HAS_RX_OBJECT
  #endif
 #else
  #ifndef CAN_FIFO_31_OBJECT_TX_PRIORITY
   #define CAN_FIFO_31_OBJECT_TX_PRIORITY    31    //default to max TX Priority
  #endif
  
  #ifndef CAN_FIFO_31_OBJECT_TX_RETRANSMISSION_ATTEMPTS
   #define CAN_FIFO_31_OBJECT_TX_RETRANSMISSION_ATTEMPTS    RETRANSMIT_ATTEMPTS_UNLIMITED //default to unlimited retransmission attempts
  #endif
  
  #ifndef CAN_HAS_TX_OBJECT
   #define CAN_HAS_TX_OBJECT
  #endif
 #endif
#endif

///////////////////////////// CAN FILTER Settings /////////////////////////////

/*
   If no CAN RX Filters are going to be used then nothing needs to be done, by
   default this driver will initialize the CAN FD controller to use no filters.
   
   If CAN RX Filters are going to be used, the can_init() function can be used
   to initialize them by making the following defines before this driver is
   included:
   
   #define CAN_USE_FILTERS          TRUE
   
   #define CAN_USER_FILT_x          aa    // x being the filter to enable (0-31),and aa being the Id to filter for.
   #define CAN_USER_MASK_x          bb    // x being the filter to enable (0-31),and bb being the Mask of Id to filter for.
                                          // Setting a bit in the Mask means the corresponding bit in the Id must match.
                                          // For example using a Mask of 0xFFF8 with Id of 0x1000, means that all Id's from
                                          // 0x1000 to 0x1007 will be accepted by the filter.
   #define CAN_USER_FILT_x_TYPE     cc    // x being the filter to enable (0-31), and cc being one of the following:
                                          //    CAN_FILTER_TYPE_SID_ONLY  //match only SID messages
                                          //    CAN_FILTER_TYPE_EID_ONLY  //match only EID messages 
                                          //    CAN_FILTER_TYPE_EITHER    //match either SID or EID messages
   #define CAN_USER_FILT_x_OBJECT   dd    // x being the filter to enable (0-31), and dd being CAN_OBJECT type, see CAN_OBJECT
                                          // type below.  This define is optional by default they are set to CAN_OBJECT_FIFO_1
                                          // because by default this driver only use CAN_OBJECT_FIFO_1 to receive messages.
   
   The CAN RX Filters can also be setup, enable and disabled at run time using 
   the can_set_filter_id(), can_enable_filter() and can_disable_filter() 
   functions.
*/

#ifndef CAN_USE_FILTERS
 #define CAN_USE_FILTERS      FALSE
#endif

#if CAN_USE_FILTERS == FALSE
 /*
   The CAN peripheral requires at least one filter to receive messages.  If
   CAN_USE_FILTERS is defined to FALSE the following ensures that Filter 0 
   is setup to receive all messages.
 */
 #ifdef CAN_USER_FILT_0
  #undef CAN_USER_FILT_0
 #endif
 #define CAN_USER_FILT_0   0
 
 #ifdef CAN_USER_MASK_0
  #undef CAN_USER_MASK_0
 #endif
 #define CAN_USER_MASK_0   0
 
 #ifdef CAN_USER_FILT_0_TYPE
  #undef CAN_USER_FILT_0_TYPE
 #endif
 #define CAN_USER_FILT_0_TYPE    CAN_FILTER_TYPE_EITHER
 
 #ifdef CAN_USER_FILT_0_OBJECT
  #undef CAN_USER_FILT_0_OBJECT
 #endif
 #define CAN_USER_FILT_0_OBJECT  CAN_OBJECT_FIFO_1
 
#else
 /*
   The CAN peripheral requires at least one filter to receive messages.  If
   CAN_USE_FILTERS is defined to TRUE and Filter 0 isn't setup the following
   sets up Filter 0 to receive all messages.  If using filters they should be
   used in order from 0 to 15. For example if one filter is required, Filter 0
   should be used, if two filters are required, Filter 0 and Filter 1 should be
   used, etc.
 */
 #ifndef CAN_USER_FILT_0
  #define CAN_USER_FILT_0  0
 #endif
 
 #ifndef CAN_USER_MASK_0
  #define CAN_USER_MASK_0  0
 #endif
 
 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE   CAN_FILTER_TYPE_EITHER
 #endif
 
 #ifndef CAN_USER_FILT_0_OBJECT
  #define CAN_USER_FILT_0_OBJECT CAN_OBJECT_FIFO_1
 #endif
 
 #ifdef CAN_USER_FILT_1
  #ifndef CAN_USER_FILT_1_OBJECT
   #define CAN_USER_FILT_1_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_2
  #ifndef CAN_USER_FILT_2_OBJECT
   #define CAN_USER_FILT_2_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_3
  #ifndef CAN_USER_FILT_3_OBJECT
   #define CAN_USER_FILT_3_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_4
  #ifndef CAN_USER_FILT_4_OBJECT
   #define CAN_USER_FILT_4_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_5
  #ifndef CAN_USER_FILT_5_OBJECT
   #define CAN_USER_FILT_5_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_6
  #ifndef CAN_USER_FILT_6_OBJECT
   #define CAN_USER_FILT_6_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_7
  #ifndef CAN_USER_FILT_7_OBJECT
   #define CAN_USER_FILT_7_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_8
  #ifndef CAN_USER_FILT_8_OBJECT
   #define CAN_USER_FILT_8_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_9
  #ifndef CAN_USER_FILT_9_OBJECT
   #define CAN_USER_FILT_9_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_10
  #ifndef CAN_USER_FILT_10_OBJECT
   #define CAN_USER_FILT_10_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_11
  #ifndef CAN_USER_FILT_11_OBJECT
   #define CAN_USER_FILT_11_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_12
  #ifndef CAN_USER_FILT_12_OBJECT
   #define CAN_USER_FILT_12_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_13
  #ifndef CAN_USER_FILT_13_OBJECT
   #define CAN_USER_FILT_13_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_14
  #ifndef CAN_USER_FILT_14_OBJECT
   #define CAN_USER_FILT_14_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_15
  #ifndef CAN_USER_FILT_15_OBJECT
   #define CAN_USER_FILT_15_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_16
  #ifndef CAN_USER_FILT_16_OBJECT
   #define CAN_USER_FILT_16_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_17
  #ifndef CAN_USER_FILT_17_OBJECT
   #define CAN_USER_FILT_17_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_18
  #ifndef CAN_USER_FILT_18_OBJECT
   #define CAN_USER_FILT_18_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_19
  #ifndef CAN_USER_FILT_19_OBJECT
   #define CAN_USER_FILT_19_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_20
  #ifndef CAN_USER_FILT_20_OBJECT
   #define CAN_USER_FILT_20_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_21
  #ifndef CAN_USER_FILT_21_OBJECT
   #define CAN_USER_FILT_21_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_22
  #ifndef CAN_USER_FILT_22_OBJECT
   #define CAN_USER_FILT_22_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_23
  #ifndef CAN_USER_FILT_23_OBJECT
   #define CAN_USER_FILT_23_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_24
  #ifndef CAN_USER_FILT_24_OBJECT
   #define CAN_USER_FILT_24_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_25
  #ifndef CAN_USER_FILT_25_OBJECT
   #define CAN_USER_FILT_25_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_26
  #ifndef CAN_USER_FILT_26_OBJECT
   #define CAN_USER_FILT_26_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_27
  #ifndef CAN_USER_FILT_27_OBJECT
   #define CAN_USER_FILT_27_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_28
  #ifndef CAN_USER_FILT_28_OBJECT
   #define CAN_USER_FILT_28_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_29
  #ifndef CAN_USER_FILT_29_OBJECT
   #define CAN_USER_FILT_29_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_30
  #ifndef CAN_USER_FILT_30_OBJECT
   #define CAN_USER_FILT_30_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
 
 #ifdef CAN_USER_FILT_31
  #ifndef CAN_USER_FILT_31_OBJECT
   #define CAN_USER_FILT_31_OBJECT CAN_OBJECT_FIFO_1
  #endif
 #endif
#endif

///////////////////////////////////////////////////////////////////////////////

// MCP2517FD SPI Commands
#define MCP2517_CMD_RESET           0x00
#define MCP2517_CMD_READ            0x30
#define MCP2517_CMD_WRITE           0x20
#define MCP2517_CMD_READ_CRC        0xB0
#define MCP2517_CMD_WRITE_CRC       0xA0
#define MCP2517_CMD_WRITE_SAFE      0xC0

// MCP2517FD Register Addresses
#define MCP2517_ADDR_REG_OSC        0xE00
#define MCP2517_ADDR_REG_IOCON      0xE04
#define MCP2517_ADDR_REG_CRC        0xE08
#define MCP2517_ADDR_REG_ECCCON     0xE0C
#define MCP2517_ADDR_REG_ECCSTAT    0xE10

// MCP2517FD CAN FD Controller Register Address
#define MCP2517_ADDR_C1CON          0x00
#define MCP2517_ADDR_C1NBTCFG       0x04
#define MCP2517_ADDR_C1DBTCFG       0x08
#define MCP2517_ADDR_C1TDC          0x0C
#define MCP2517_ADDR_C1TBC          0x10
#define MCP2517_ADDR_C1TSCON        0x14
#define MCP2517_ADDR_C1VEC          0x18
#define MCP2517_ADDR_C1INT          0x1C
#define MCP2517_ADDR_C1RXIF         0x20
#define MCP2517_ADDR_C1TXIF         0x24
#define MCP2517_ADDR_C1RXOVIF       0x28
#define MCP2517_ADDR_C1TXATIF       0x2C
#define MCP2517_ADDR_C1TXREG        0x30
#define MCP2517_ADDR_C1TREC         0x34
#define MCP2517_ADDR_C1BDIAG0       0x38
#define MCP2517_ADDR_C1BDIAG1       0x3C
#define MCP2517_ADDR_C1TEFCON       0x40
#define MCP2517_ADDR_C1TEFSTA       0x44
#define MCP2517_ADDR_C1TEFUA        0x48
#define MCP2517_ADDR_C1TXQCON       0x50
#define MCP2517_ADDR_C1TXQSTA       0x54
#define MCP2517_ADDR_C1TXQUA        0x58

#define MCP2517_ADDR_C1FIFOCON1     0x5C
#define MCP2517_ADDR_C1FIFOSTA1     0x60
#define MCP2517_ADDR_C1FIFOUA1      0x64
#define MCP2517_ADDR_C1FIFOCON2     0x68
#define MCP2517_ADDR_C1FIFOSTA2     0x6C
#define MCP2517_ADDR_C1FIFOUA2      0x70
#define MCP2517_ADDR_C1FIFOCON3     0x74
#define MCP2517_ADDR_C1FIFOSTA3     0x78
#define MCP2517_ADDR_C1FIFOUA3      0x7C
#define MCP2517_ADDR_C1FIFOCON4     0x80
#define MCP2517_ADDR_C1FIFOSTA4     0x84
#define MCP2517_ADDR_C1FIFOUA4      0x88
#define MCP2517_ADDR_C1FIFOCON5     0x8C
#define MCP2517_ADDR_C1FIFOSTA5     0x90
#define MCP2517_ADDR_C1FIFOUA5      0x94
#define MCP2517_ADDR_C1FIFOCON6     0x98
#define MCP2517_ADDR_C1FIFOSTA6     0x9C
#define MCP2517_ADDR_C1FIFOUA6      0xA0
#define MCP2517_ADDR_C1FIFOCON7     0xA4
#define MCP2517_ADDR_C1FIFOSTA7     0xA8
#define MCP2517_ADDR_C1FIFOUA7      0xAC
#define MCP2517_ADDR_C1FIFOCON8     0xB0
#define MCP2517_ADDR_C1FIFOSTA8     0xB4
#define MCP2517_ADDR_C1FIFOUA8      0xB8
#define MCP2517_ADDR_C1FIFOCON9     0xBC
#define MCP2517_ADDR_C1FIFOSTA9     0xC0
#define MCP2517_ADDR_C1FIFOUA9      0xC4
#define MCP2517_ADDR_C1FIFOCON10    0xC8
#define MCP2517_ADDR_C1FIFOSTA10    0xCC
#define MCP2517_ADDR_C1FIFOUA10     0xD0
#define MCP2517_ADDR_C1FIFOCON11    0xD4
#define MCP2517_ADDR_C1FIFOSTA11    0xD8
#define MCP2517_ADDR_C1FIFOUA11     0xDC
#define MCP2517_ADDR_C1FIFOCON12    0xE0
#define MCP2517_ADDR_C1FIFOSTA12    0xE4
#define MCP2517_ADDR_C1FIFOUA12     0xE8
#define MCP2517_ADDR_C1FIFOCON13    0xEC
#define MCP2517_ADDR_C1FIFOSTA13    0xF0
#define MCP2517_ADDR_C1FIFOUA13     0xF4
#define MCP2517_ADDR_C1FIFOCON14    0xF8
#define MCP2517_ADDR_C1FIFOSTA14    0xFC
#define MCP2517_ADDR_C1FIFOUA14     0x100
#define MCP2517_ADDR_C1FIFOCON15    0x104
#define MCP2517_ADDR_C1FIFOSTA15    0x108
#define MCP2517_ADDR_C1FIFOUA15     0x10C
#define MCP2517_ADDR_C1FIFOCON16    0x110
#define MCP2517_ADDR_C1FIFOSTA16    0x114
#define MCP2517_ADDR_C1FIFOUA16     0x118
#define MCP2517_ADDR_C1FIFOCON17    0x11C
#define MCP2517_ADDR_C1FIFOSTA17    0x120
#define MCP2517_ADDR_C1FIFOUA17     0x124
#define MCP2517_ADDR_C1FIFOCON18    0x128
#define MCP2517_ADDR_C1FIFOSTA18    0x12C
#define MCP2517_ADDR_C1FIFOUA18     0x130
#define MCP2517_ADDR_C1FIFOCON19    0x134
#define MCP2517_ADDR_C1FIFOSTA19    0x138
#define MCP2517_ADDR_C1FIFOUA19     0x13C
#define MCP2517_ADDR_C1FIFOCON20    0x140
#define MCP2517_ADDR_C1FIFOSTA20    0x144
#define MCP2517_ADDR_C1FIFOUA20     0x148
#define MCP2517_ADDR_C1FIFOCON21    0x14C
#define MCP2517_ADDR_C1FIFOSTA21    0x150
#define MCP2517_ADDR_C1FIFOUA21     0x154
#define MCP2517_ADDR_C1FIFOCON22    0x158
#define MCP2517_ADDR_C1FIFOSTA22    0x15C
#define MCP2517_ADDR_C1FIFOUA22     0x160
#define MCP2517_ADDR_C1FIFOCON23    0x164
#define MCP2517_ADDR_C1FIFOSTA23    0x168
#define MCP2517_ADDR_C1FIFOUA23     0x16C
#define MCP2517_ADDR_C1FIFOCON24    0x170
#define MCP2517_ADDR_C1FIFOSTA24    0x174
#define MCP2517_ADDR_C1FIFOUA24     0x178
#define MCP2517_ADDR_C1FIFOCON25    0x17C
#define MCP2517_ADDR_C1FIFOSTA25    0x180
#define MCP2517_ADDR_C1FIFOUA25     0x184
#define MCP2517_ADDR_C1FIFOCON26    0x188
#define MCP2517_ADDR_C1FIFOSTA26    0x18C
#define MCP2517_ADDR_C1FIFOUA26     0x190
#define MCP2517_ADDR_C1FIFOCON27    0x194
#define MCP2517_ADDR_C1FIFOSTA27    0x198
#define MCP2517_ADDR_C1FIFOUA27     0x19C
#define MCP2517_ADDR_C1FIFOCON28    0x1A0
#define MCP2517_ADDR_C1FIFOSTA28    0x1A4
#define MCP2517_ADDR_C1FIFOUA28     0x1A8
#define MCP2517_ADDR_C1FIFOCON29    0x1AC
#define MCP2517_ADDR_C1FIFOSTA29    0x1B0
#define MCP2517_ADDR_C1FIFOUA29     0x1B4
#define MCP2517_ADDR_C1FIFOCON30    0x1B8
#define MCP2517_ADDR_C1FIFOSTA30    0x1BC
#define MCP2517_ADDR_C1FIFOUA30     0x1C0
#define MCP2517_ADDR_C1FIFOCON31    0x1C4
#define MCP2517_ADDR_C1FIFOSTA31    0x1C8
#define MCP2517_ADDR_C1FIFOUA31     0x1CC

#define MCP2517_ADDR_C1FLTCON0      0x1D0
#define MCP2517_ADDR_C1FLTCON1      0x1D4
#define MCP2517_ADDR_C1FLTCON2      0x1D8
#define MCP2517_ADDR_C1FLTCON3      0x1DC
#define MCP2517_ADDR_C1FLTCON4      0x1E0
#define MCP2517_ADDR_C1FLTCON5      0x1E4
#define MCP2517_ADDR_C1FLTCON6      0x1E8
#define MCP2517_ADDR_C1FLTCON7      0x1EC

#define MCP2517_ADDR_C1FLTOBJ0      0x1F0
#define MCP2517_ADDR_C1MASK0        0x1F4
#define MCP2517_ADDR_C1FLTOBJ1      0x1F8
#define MCP2517_ADDR_C1MASK1        0x1FC
#define MCP2517_ADDR_C1FLTOBJ2      0x200
#define MCP2517_ADDR_C1MASK2        0x204
#define MCP2517_ADDR_C1FLTOBJ3      0x208
#define MCP2517_ADDR_C1MASK3        0x20C
#define MCP2517_ADDR_C1FLTOBJ4      0x210
#define MCP2517_ADDR_C1MASK4        0x214
#define MCP2517_ADDR_C1FLTOBJ5      0x218
#define MCP2517_ADDR_C1MASK5        0x21C
#define MCP2517_ADDR_C1FLTOBJ6      0x220
#define MCP2517_ADDR_C1MASK6        0x224
#define MCP2517_ADDR_C1FLTOBJ7      0x228
#define MCP2517_ADDR_C1MASK7        0x22C
#define MCP2517_ADDR_C1FLTOBJ8      0x230
#define MCP2517_ADDR_C1MASK8        0x234
#define MCP2517_ADDR_C1FLTOBJ9      0x238
#define MCP2517_ADDR_C1MASK9        0x23C
#define MCP2517_ADDR_C1FLTOBJ10     0x240
#define MCP2517_ADDR_C1MASK10       0x244
#define MCP2517_ADDR_C1FLTOBJ11     0x248
#define MCP2517_ADDR_C1MASK11       0x24C
#define MCP2517_ADDR_C1FLTOBJ12     0x250
#define MCP2517_ADDR_C1MASK12       0x254
#define MCP2517_ADDR_C1FLTOBJ13     0x258
#define MCP2517_ADDR_C1MASK13       0x25C
#define MCP2517_ADDR_C1FLTOBJ14     0x260
#define MCP2517_ADDR_C1MASK14       0x264
#define MCP2517_ADDR_C1FLTOBJ15     0x268
#define MCP2517_ADDR_C1MASK15       0x26C
#define MCP2517_ADDR_C1FLTOBJ16     0x270
#define MCP2517_ADDR_C1MASK16       0x274
#define MCP2517_ADDR_C1FLTOBJ17     0x278
#define MCP2517_ADDR_C1MASK17       0x27C
#define MCP2517_ADDR_C1FLTOBJ18     0x280
#define MCP2517_ADDR_C1MASK18       0x284
#define MCP2517_ADDR_C1FLTOBJ19     0x288
#define MCP2517_ADDR_C1MASK19       0x28C
#define MCP2517_ADDR_C1FLTOBJ20     0x290
#define MCP2517_ADDR_C1MASK20       0x294
#define MCP2517_ADDR_C1FLTOBJ21     0x298
#define MCP2517_ADDR_C1MASK21       0x29C
#define MCP2517_ADDR_C1FLTOBJ22     0x2A0
#define MCP2517_ADDR_C1MASK22       0x2A4
#define MCP2517_ADDR_C1FLTOBJ23     0x2A8
#define MCP2517_ADDR_C1MASK23       0x2AC
#define MCP2517_ADDR_C1FLTOBJ24     0x2B0
#define MCP2517_ADDR_C1MASK24       0x2B4
#define MCP2517_ADDR_C1FLTOBJ25     0x2B8
#define MCP2517_ADDR_C1MASK25       0x2BC
#define MCP2517_ADDR_C1FLTOBJ26     0x2C0
#define MCP2517_ADDR_C1MASK26       0x2C4
#define MCP2517_ADDR_C1FLTOBJ27     0x2C8
#define MCP2517_ADDR_C1MASK27       0x2CC
#define MCP2517_ADDR_C1FLTOBJ28     0x2D0
#define MCP2517_ADDR_C1MASK28       0x2D4
#define MCP2517_ADDR_C1FLTOBJ29     0x2D8
#define MCP2517_ADDR_C1MASK29       0x2DC
#define MCP2517_ADDR_C1FLTOBJ30     0x2E0
#define MCP2517_ADDR_C1MASK30       0x2E4
#define MCP2517_ADDR_C1FLTOBJ31     0x2E8
#define MCP2517_ADDR_C1MASK31       0x2EC

////////////////////////////////// CAN Types //////////////////////////////////

typedef uint16_t MCP2517_ADDRESS;

// MCP2517FD Register
typedef struct
{
   uint8_t PLLEN:1;              //1 - System Clock from 10x PLL, 0 - System Clock comes from crystal
   uint8_t u1:1;
   uint8_t OSCDIS:1;             //1 - Clock disabled, 0 - Enable Clock
   uint8_t u3:1;
   uint8_t SCLKDIV:1;            //1 - SCLK divided by 2, 0 - SCLK divided by 1
   uint8_t CLKODIV:2;            //0 - CLKO divided by 1 - CLKO divided by 2, 2 - CLKO divided by 4, 3 - CLCKO divided by 10
   uint8_t u7:1;
}  MCP2517_OSC;

typedef struct
{
   uint8_t PLLRDY:1;
   uint8_t u9:1;
   uint8_t OSCRDY:1;
   uint8_t u11:1;
   uint8_t SCKRDY:1;
   uint8_t u13_15:3;
} MCP2517_OSC_STATUS;

// CAN Control
typedef struct struct_CiCON
{
   uint8_t DNCNT:5;              //Device Net Filter Bit Number bits
   uint8_t ISOCRC:1;             //Enable ISO CRC in CAN FD Frame bit
   uint8_t PXE:1;                //Protocol Exception Event Detection bit
   uint8_t u7:1;
   uint8_t WAKFIL:1;             //Enable CAN Bus Line Wake-up Filter bit
   uint8_t WFT:2;                //Wake-up Filter Time bits
   uint8_t BUSY:1;               //CAN Module is Busy bit
   uint8_t BRS:1;                //Bit Rate Switching bit
   uint8_t u13_15:3;
   uint8_t RTXAT:1;              //Restricted Retransmission Attempts bit
   uint8_t ESIGM:1;              //Transmit ESI in Gateway Mode bit
   uint8_t SERR2LOM:1;           //Transition to Listen Only Mode on System Error bit
   uint8_t STEF:1;               //Store in Transmit Event FIFO bit
   uint8_t TXQ:1;                //Enable Transmit Queue bit
   CAN_OP_MODE OPMOD:3;          //Operation Mode Status bits
   CAN_OP_MODE REQOP:3;          //Request Operation Mode bits
   uint8_t ABAT:1;               //Abort All Pending Transmissions bit
   TRANSMISSION_DELAY TXBWS:4;   //Transmit Bandwidth Sharing bits, Delay between two consecutive transmissions (in arbitration bit times)
} CiCON;

// CAN Nominal Bit Time Configuration
typedef struct struct_CiNBTCFG
{
   uint8_t SJW:7;    //Synchronization Jump Width (SJW + 1) 1-128 Tq
   uint8_t u7:1;
   uint8_t TSEG2:7;  //Phase Segment 2 (TSEG2 + 1) 1-128 Tq
   uint8_t u15:1;
   uint8_t TSEG1;    //Phase Segment 1 (TSEG1 + 1) 1-256 Tq
   uint8_t BRP;      //Baud Rate Prescaler (BRP + 1) Tq = (BRP+1)/Fsys
} CiNBTCFG;
                     
// CAN Data Bit Time Configuration
typedef struct struct_CiDBTCFG
{
   uint8_t SJW:4;    //Synchronization Jump Width (SJW + 1) 1-16 Tq
   uint8_t u4_7:4;
   uint8_t TSEG2:4;  //Phase Segment 2 (TSEG2 + 1) 1-16 Tq
   uint8_t u12_15:4;
   uint8_t TSEG1:5;  //Phase Segment 1 (TSEG1 + 1) 1-32 Tq
   uint8_t u21_23:3; //Baud Rate Prescaler (BRP + 1) Tq = (BRP+1)/Fsys
   uint8_t BRP;
} CiDBTCFG;

// CAN Transmitter Delay Compensation
typedef enum
{
   CAN_TDC_DISABLED,
   CAN_TDC_MANUAL,
   CAN_TDC_AUTO
} CAN_TDC;

typedef struct struct_CiTDC
{
   uint8_t TDCV;        //Transmitter Delay Compensation Value bits
   uint8_t TDCO;        //Transmitter Delay Compensation Offset bits
   CAN_TDC TDCMOD;      //Transmitter Delay Compensation Mode bits
   uint8_t SID11:1;     //Enable 12-Bit SID in CAN FD Base Format Messages bit
   uint8_t EDGFLT:1;    //Enable Edge Filtering during Bus Integration state bit
   uint8_t u26_31:6;
} CiTDC;

// CAN Time Base Counter
typedef struct struct_CiTBC
{
   uint32_t TBC;
} CiTBC;

// CAN Time Stamp Control
typedef struct struct_CiTSCON
{
   uint16_t TBCPRE;  //Timer Base Counter Prescaler bits, 0 to 1023 (1-1024)
   uint8_t TBCEN:1;  //Time Base Counter Enable Bit
   uint8_t TSEOF:1;  //Time Stamp EOF bit
   uint8_t TSRES:1;  //Time Stamp res bit (FD Frames only)
   uint8_t u19_23:5;
   uint8_t u24_31;
} CiTSCON;

// CAN Interrupt Code
typedef enum {CAN_INT_TXQ, CAN_INT_FIFO_1, CAN_INT_FIFO_2, CAN_INT_FIFO_3, CAN_INT_FIFO_4, CAN_INT_FIFO_5, CAN_INT_FIFO_6, CAN_INT_FIFO_7,
              CAN_INT_FIFO_8, CAN_INT_FIFO_9, CAN_INT_FIFO_10, CAN_INT_FIFO_11, CAN_INT_FIFO_12, CAN_INT_FIFO_13, CAN_INT_FIFO_14, CAN_INT_FIFO_15,
              CAN_INT_FIFO_16, CAN_INT_FIFO_17, CAN_INT_FIFO_18, CAN_INT_FIFO_19, CAN_INT_FIFO_20, CAN_INT_FIFO_21, CAN_INT_FIFO_22, CAN_INT_FIFO_23,
              CAN_INT_FIFO_24, CAN_INT_FIFO_25, CAN_INT_FIFO_26, CAN_INT_FIFO_27, CAN_INT_FIFO_28, CAN_INT_FIFO_29, CAN_INT_FIFO_30, CAN_INT_FIFO_31,
              CAN_INT_NONE=64, CAN_INT_ERR, CAN_INT_WAK, CAN_INT_RX_FIFO_OVERFLOW, CAN_INT_ADDR_ERR, CAN_INT_MAB_OVERFLOW_UNDERFLOW, CAN_INT_TBC_OVERFLOW,
              CAN_INT_OPMOD_CHANGED, CAN_INT_INVALID_MESSAGE, CAN_INT_TX_EVENT_FIFO, CAN_INT_TX_ATTEMPT
} CAN_INT_CODE;
              
typedef enum {CAN_TX_INT_TXQ, CAN_TX_INT_FIFO_1, CAN_TX_INT_FIFO_2, CAN_TX_INT_FIFO_3, CAN_TX_INT_FIFO_4, CAN_TX_INT_FIFO_5, CAN_TX_INT_FIFO_6, CAN_TX_INT_FIFO_7,
              CAN_TX_INT_FIFO_8, CAN_TX_INT_FIFO_9, CAN_TX_INT_FIFO_10, CAN_TX_INT_FIFO_11, CAN_TX_INT_FIFO_12, CAN_TX_INT_FIFO_13, CAN_TX_INT_FIFO_14, CAN_TX_INT_FIFO_15,
              CAN_TX_INT_FIFO_16, CAN_TX_INT_FIFO_17, CAN_TX_INT_FIFO_18, CAN_TX_INT_FIFO_19, CAN_TX_INT_FIFO_20, CAN_TX_INT_FIFO_21, CAN_TX_INT_FIFO_22, CAN_TX_INT_FIFO_23,
              CAN_TX_INT_FIFO_24, CAN_TX_INT_FIFO_25, CAN_TX_INT_FIFO_26, CAN_TX_INT_FIFO_27, CAN_TX_INT_FIFO_28, CAN_TX_INT_FIFO_29, CAN_TX_INT_FIFO_30, CAN_TX_INT_FIFO_31
} CAN_TX_INT_CODE;

typedef enum {CAN_RX_INT_FIFO_1=1, CAN_RX_INT_FIFO_2, CAN_RX_INT_FIFO_3, CAN_RX_INT_FIFO_4, CAN_RX_INT_FIFO_5, CAN_RX_INT_FIFO_6, CAN_RX_INT_FIFO_7,
              CAN_RX_INT_FIFO_8, CAN_RX_INT_FIFO_9, CAN_RX_INT_FIFO_10, CAN_RX_INT_FIFO_11, CAN_RX_INT_FIFO_12, CAN_RX_INT_FIFO_13, CAN_RX_INT_FIFO_14, CAN_RX_INT_FIFO_15,
              CAN_RX_INT_FIFO_16, CAN_RX_INT_FIFO_17, CAN_RX_INT_FIFO_18, CAN_RX_INT_FIFO_19, CAN_RX_INT_FIFO_20, CAN_RX_INT_FIFO_21, CAN_RX_INT_FIFO_22, CAN_RX_INT_FIFO_23,
              CAN_RX_INT_FIFO_24, CAN_RX_INT_FIFO_25, CAN_RX_INT_FIFO_26, CAN_RX_INT_FIFO_27, CAN_RX_INT_FIFO_28, CAN_RX_INT_FIFO_29, CAN_RX_INT_FIFO_30, CAN_RX_INT_FIFO_31
} CAN_RX_INT_CODE;

typedef struct struct_CiVEC
{
   CAN_INT_CODE ICODE;        //Interrupt Flag Code bits
   uint8_t FILHIT;            //Filter Hit Number bits
   CAN_TX_INT_CODE TXCODE;    //Transmit Interrupt Flag Code bits
   CAN_RX_INT_CODE RXCode;    //Receive Interrupt Flag Code bits
} CiVEC;

// CAN Interrupt
typedef struct struct_CiINT
{
   uint8_t TXIF:1;      //Transmit FIFO Interrupt Flag bit
   uint8_t RXIF:1;      //Receive FIFO Interrupt Flag bit
   uint8_t TBCIF:1;     //Time Base Count Overflow Interrupt Flag bit
   uint8_t MODIF:1;     //Operation Mode Change Interrupt Flag bit
   uint8_t TEFIF:1;     //Transmit Event FIFO Interrupt Flag bit
   uint8_t u5_7:3;
   uint8_t ECCIF:1;     //ECC Error Interrupt Flag bit
   uint8_t SPICRCIF:1;  //SPI CRC Error Interrupt Flag bit
   uint8_t TXATIF:1;    //Transmit Attempt Interrupt Flag bit
   uint8_t RXOVIF:1;    //Receive Object Overflow Interrupt Flag bit
   uint8_t SERRIF:1;    //System Error Interrupt Flag bit
   uint8_t CERRIF:1;    //CAN Bus Error Interrupt Flag bit
   uint8_t WAFIF:1;     //Bus Wake Up Interrupt Flag bit
   uint8_t IVMIF:1;     //Invalid Message Interrupt Flag bit
   uint8_t TXIE:1;      //Transmit FIFO Interrupt Enable bit
   uint8_t RXIE:1;      //Receive FIFO Interrupt Enable bit
   uint8_t TBCIE:1;     //Time Base Counter Interrupt Enable bit
   uint8_t MODIE:1;     //Mode Change Interrupt Enable bit
   uint8_t TEFIE:1;     //Transmit Event FIFO Interrupt Enable bit
   uint8_t u21_23:3;
   uint8_t ECCIE:1;     //ECC Error Interrupt Enable bit
   uint8_t SPICRCIE:1;  //SPI CRC Error Interrupt Enable bit
   uint8_t TXATIE:1;    //Transmit Attempt Interrupt Enable bit
   uint8_t RXOVIE:1;    //Receive FIFO Overflow Interrupt Enable bit
   uint8_t SERRIE:1;    //System Error Interrupt Enable bit
   uint8_t CERRIE:1;    //CAN Bus Error Interrupt Enable bit
   uint8_t WAKIE:1;     //Bus Wake Up Interrupt Enable bit
   uint8_t IVMIE:1;     //Invalid Message Interrupt Enable bit
} CiINT;

// CAN Receive Interrupt Status
typedef union union_CiRXIF
{
   struct
   {
      uint8_t u0:1;
      uint8_t RF1:1;    //Receive FIFO 1 Interrupt Pending bit
      uint8_t RF2:1;    //Receive FIFO 2 Interrupt Pending bit
      uint8_t RF3:1;    //Receive FIFO 3 Interrupt Pending bit
      uint8_t RF4:1;    //Receive FIFO 4 Interrupt Pending bit
      uint8_t RF5:1;    //Receive FIFO 5 Interrupt Pending bit
      uint8_t RF6:1;    //Receive FIFO 6 Interrupt Pending bit
      uint8_t RF7:1;    //Receive FIFO 7 Interrupt Pending bit
      uint8_t RF8:1;    //Receive FIFO 8 Interrupt Pending bit
      uint8_t RF9:1;    //Receive FIFO 9 Interrupt Pending bit
      uint8_t RF10:1;   //Receive FIFO 10 Interrupt Pending bit
      uint8_t RF11:1;   //Receive FIFO 11 Interrupt Pending bit
      uint8_t RF12:1;   //Receive FIFO 12 Interrupt Pending bit
      uint8_t RF13:1;   //Receive FIFO 13 Interrupt Pending bit
      uint8_t RF14:1;   //Receive FIFO 14 Interrupt Pending bit
      uint8_t RF15:1;   //Receive FIFO 15 Interrupt Pending bit
      uint8_t RF16:1;   //Receive FIFO 16 Interrupt Pending bit
      uint8_t RF17:1;   //Receive FIFO 17 Interrupt Pending bit
      uint8_t RF18:1;   //Receive FIFO 18 Interrupt Pending bit
      uint8_t RF19:1;   //Receive FIFO 19 Interrupt Pending bit
      uint8_t RF20:1;   //Receive FIFO 20 Interrupt Pending bit
      uint8_t RF21:1;   //Receive FIFO 21 Interrupt Pending bit
      uint8_t RF22:1;   //Receive FIFO 22 Interrupt Pending bit
      uint8_t RF23:1;   //Receive FIFO 23 Interrupt Pending bit
      uint8_t RF24:1;   //Receive FIFO 24 Interrupt Pending bit
      uint8_t RF25:1;   //Receive FIFO 25 Interrupt Pending bit
      uint8_t RF26:1;   //Receive FIFO 26 Interrupt Pending bit
      uint8_t RF27:1;   //Receive FIFO 27 Interrupt Pending bit
      uint8_t RF28:1;   //Receive FIFO 28 Interrupt Pending bit
      uint8_t RF29:1;   //Receive FIFO 29 Interrupt Pending bit
      uint8_t RF30:1;   //Receive FIFO 30 Interrupt Pending bit
      uint8_t RF31:1;   //Receive FIFO 31 Interrupt Pending bit
   };
   uint32_t dw;
} CiRXIF;

// CAN Receive Overflow Interrupt Status
typedef union union_CiRXOVIF
{
   struct
   {
      uint8_t u0:1;
      uint8_t RFOV1:1;     //Receive FIFO Overflow 1 Interrupt Pending bit
      uint8_t RFOV2:1;     //Receive FIFO Overflow 2 Interrupt Pending bit
      uint8_t RFOV3:1;     //Receive FIFO Overflow 3 Interrupt Pending bit
      uint8_t RFOV4:1;     //Receive FIFO Overflow 4 Interrupt Pending bit
      uint8_t RFOV5:1;     //Receive FIFO Overflow 5 Interrupt Pending bit
      uint8_t RFOV6:1;     //Receive FIFO Overflow 6 Interrupt Pending bit
      uint8_t RFOV7:1;     //Receive FIFO Overflow 7 Interrupt Pending bit
      uint8_t RFOV8:1;     //Receive FIFO Overflow 8 Interrupt Pending bit
      uint8_t RFOV9:1;     //Receive FIFO Overflow 9 Interrupt Pending bit
      uint8_t RFOV10:1;    //Receive FIFO Overflow 10 Interrupt Pending bit
      uint8_t RFOV11:1;    //Receive FIFO Overflow 11 Interrupt Pending bit
      uint8_t RFOV12:1;    //Receive FIFO Overflow 12 Interrupt Pending bit
      uint8_t RFOV13:1;    //Receive FIFO Overflow 13 Interrupt Pending bit
      uint8_t RFOV14:1;    //Receive FIFO Overflow 14 Interrupt Pending bit
      uint8_t RFOV15:1;    //Receive FIFO Overflow 15 Interrupt Pending bit
      uint8_t RFOV16:1;    //Receive FIFO Overflow 16 Interrupt Pending bit
      uint8_t RFOV17:1;    //Receive FIFO Overflow 17 Interrupt Pending bit
      uint8_t RFOV18:1;    //Receive FIFO Overflow 18 Interrupt Pending bit
      uint8_t RFOV19:1;    //Receive FIFO Overflow 19 Interrupt Pending bit
      uint8_t RFOV20:1;    //Receive FIFO Overflow 20 Interrupt Pending bit
      uint8_t RFOV21:1;    //Receive FIFO Overflow 21 Interrupt Pending bit
      uint8_t RFOV22:1;    //Receive FIFO Overflow 22 Interrupt Pending bit
      uint8_t RFOV23:1;    //Receive FIFO Overflow 23 Interrupt Pending bit
      uint8_t RFOV24:1;    //Receive FIFO Overflow 24 Interrupt Pending bit
      uint8_t RFOV25:1;    //Receive FIFO Overflow 25 Interrupt Pending bit
      uint8_t RFOV26:1;    //Receive FIFO Overflow 26 Interrupt Pending bit
      uint8_t RFOV27:1;    //Receive FIFO Overflow 27 Interrupt Pending bit
      uint8_t RFOV28:1;    //Receive FIFO Overflow 28 Interrupt Pending bit
      uint8_t RFOV29:1;    //Receive FIFO Overflow 29 Interrupt Pending bit
      uint8_t RFOV30:1;    //Receive FIFO Overflow 30 Interrupt Pending bit
      uint8_t RFOV31:1;    //Receive FIFO Overflow 31 Interrupt Pending bit
   };
   uint32_t dw;
} CiRXOVIF;

// CAN Transmit Interrupt Status
typedef union union_CiTXIF
{
   struct
   {
      uint8_t TXQ:1;    //Transmit TXQ Interrupt Pending bit
      uint8_t TF1:1;    //Transmit FIFO 1 Interrupt Pending bit
      uint8_t TF2:1;    //Transmit FIFO 2 Interrupt Pending bit
      uint8_t TF3:1;    //Transmit FIFO 3 Interrupt Pending bit
      uint8_t TF4:1;    //Transmit FIFO 4 Interrupt Pending bit
      uint8_t TF5:1;    //Transmit FIFO 5 Interrupt Pending bit
      uint8_t TF6:1;    //Transmit FIFO 6 Interrupt Pending bit
      uint8_t TF7:1;    //Transmit FIFO 7 Interrupt Pending bit
      uint8_t TF8:1;    //Transmit FIFO 8 Interrupt Pending bit
      uint8_t TF9:1;    //Transmit FIFO 9 Interrupt Pending bit
      uint8_t TF10:1;   //Transmit FIFO 10 Interrupt Pending bit
      uint8_t TF11:1;   //Transmit FIFO 11 Interrupt Pending bit
      uint8_t TF12:1;   //Transmit FIFO 12 Interrupt Pending bit
      uint8_t TF13:1;   //Transmit FIFO 13 Interrupt Pending bit
      uint8_t TF14:1;   //Transmit FIFO 14 Interrupt Pending bit
      uint8_t TF15:1;   //Transmit FIFO 15 Interrupt Pending bit
      uint8_t TF16:1;   //Transmit FIFO 16 Interrupt Pending bit
      uint8_t TF17:1;   //Transmit FIFO 17 Interrupt Pending bit
      uint8_t TF18:1;   //Transmit FIFO 18 Interrupt Pending bit
      uint8_t TF19:1;   //Transmit FIFO 19 Interrupt Pending bit
      uint8_t TF20:1;   //Transmit FIFO 20 Interrupt Pending bit
      uint8_t TF21:1;   //Transmit FIFO 21 Interrupt Pending bit
      uint8_t TF22:1;   //Transmit FIFO 22 Interrupt Pending bit
      uint8_t TF23:1;   //Transmit FIFO 23 Interrupt Pending bit
      uint8_t TF24:1;   //Transmit FIFO 24 Interrupt Pending bit
      uint8_t TF25:1;   //Transmit FIFO 25 Interrupt Pending bit
      uint8_t TF26:1;   //Transmit FIFO 26 Interrupt Pending bit
      uint8_t TF27:1;   //Transmit FIFO 27 Interrupt Pending bit
      uint8_t TF28:1;   //Transmit FIFO 28 Interrupt Pending bit
      uint8_t TF29:1;   //Transmit FIFO 29 Interrupt Pending bit
      uint8_t TF30:1;   //Transmit FIFO 30 Interrupt Pending bit
      uint8_t TF31:1;   //Transmit FIFO 31 Interrupt Pending bit
   };
   uint32_t dw;
} CiTXIF;

// CAN Transmit Attempt Interrupt Status
typedef union union_CiTXATIF
{
   struct
   {
      uint8_t TXQAT:1;     //Transmit TXQ Attempt Interrupt Pending bit
      uint8_t TFAT1:1;     //Transmit FIFO Attempt 1 Interrupt Pending bit
      uint8_t TFAT2:1;     //Transmit FIFO Attempt 2 Interrupt Pending bit
      uint8_t TFAT3:1;     //Transmit FIFO Attempt 3 Interrupt Pending bit
      uint8_t TFAT4:1;     //Transmit FIFO Attempt 4 Interrupt Pending bit
      uint8_t TFAT5:1;     //Transmit FIFO Attempt 5 Interrupt Pending bit
      uint8_t TFAT6:1;     //Transmit FIFO Attempt 6 Interrupt Pending bit
      uint8_t TFAT7:1;     //Transmit FIFO Attempt 7 Interrupt Pending bit
      uint8_t TFAT8:1;     //Transmit FIFO Attempt 8 Interrupt Pending bit
      uint8_t TFAT9:1;     //Transmit FIFO Attempt 9 Interrupt Pending bit
      uint8_t TFAT10:1;    //Transmit FIFO Attempt 10 Interrupt Pending bit
      uint8_t TFAT11:1;    //Transmit FIFO Attempt 11 Interrupt Pending bit
      uint8_t TFAT12:1;    //Transmit FIFO Attempt 12 Interrupt Pending bit
      uint8_t TFAT13:1;    //Transmit FIFO Attempt 13 Interrupt Pending bit
      uint8_t TFAT14:1;    //Transmit FIFO Attempt 14 Interrupt Pending bit
      uint8_t TFAT15:1;    //Transmit FIFO Attempt 15 Interrupt Pending bit
      uint8_t TFAT16:1;    //Transmit FIFO Attempt 16 Interrupt Pending bit
      uint8_t TFAT17:1;    //Transmit FIFO Attempt 17 Interrupt Pending bit
      uint8_t TFAT18:1;    //Transmit FIFO Attempt 18 Interrupt Pending bit
      uint8_t TFAT19:1;    //Transmit FIFO Attempt 19 Interrupt Pending bit
      uint8_t TFAT20:1;    //Transmit FIFO Attempt 20 Interrupt Pending bit
      uint8_t TFAT21:1;    //Transmit FIFO Attempt 21 Interrupt Pending bit
      uint8_t TFAT22:1;    //Transmit FIFO Attempt 22 Interrupt Pending bit
      uint8_t TFAT23:1;    //Transmit FIFO Attempt 23 Interrupt Pending bit
      uint8_t TFAT24:1;    //Transmit FIFO Attempt 24 Interrupt Pending bit
      uint8_t TFAT25:1;    //Transmit FIFO Attempt 25 Interrupt Pending bit
      uint8_t TFAT26:1;    //Transmit FIFO Attempt 26 Interrupt Pending bit
      uint8_t TFAT27:1;    //Transmit FIFO Attempt 27 Interrupt Pending bit
      uint8_t TFAT28:1;    //Transmit FIFO Attempt 28 Interrupt Pending bit
      uint8_t TFAT29:1;    //Transmit FIFO Attempt 29 Interrupt Pending bit
      uint8_t TFAT30:1;    //Transmit FIFO Attempt 30 Interrupt Pending bit
      uint8_t TFAT31:1;    //Transmit FIFO Attempt 31 Interrupt Pending bit
   };
   uint32_t dw;
} CiTXATIF;

//CAN Transmit Request
typedef union union_CiTXREQ
{
   struct
   {
      uint8_t TXQ:1;    //Queue Message Send Request bit
      uint8_t TXO1:1;   //Message Object 1 Send Request bit
      uint8_t TXO2:1;   //Message Object 2 Send Request bit
      uint8_t TXO3:1;   //Message Object 3 Send Request bit
      uint8_t TXO4:1;   //Message Object 4 Send Request bit
      uint8_t TXO5:1;   //Message Object 5 Send Request bit
      uint8_t TXO6:1;   //Message Object 6 Send Request bit
      uint8_t TXO7:1;   //Message Object 7 Send Request bit
      uint8_t TXO8:1;   //Message Object 8 Send Request bit
      uint8_t TXO9:1;   //Message Object 9 Send Request bit
      uint8_t TXO10:1;  //Message Object 10 Send Request bit
      uint8_t TXO11:1;  //Message Object 11 Send Request bit
      uint8_t TXO12:1;  //Message Object 12 Send Request bit
      uint8_t TXO13:1;  //Message Object 13 Send Request bit
      uint8_t TXO14:1;  //Message Object 14 Send Request bit
      uint8_t TXO15:1;  //Message Object 15 Send Request bit
      uint8_t TXO16:1;  //Message Object 16 Send Request bit
      uint8_t TXO17:1;  //Message Object 17 Send Request bit
      uint8_t TXO18:1;  //Message Object 18 Send Request bit
      uint8_t TXO19:1;  //Message Object 19 Send Request bit
      uint8_t TXO20:1;  //Message Object 20 Send Request bit
      uint8_t TXO21:1;  //Message Object 21 Send Request bit
      uint8_t TXO22:1;  //Message Object 22 Send Request bit
      uint8_t TXO23:1;  //Message Object 23 Send Request bit
      uint8_t TXO24:1;  //Message Object 24 Send Request bit
      uint8_t TXO25:1;  //Message Object 25 Send Request bit
      uint8_t TXO26:1;  //Message Object 26 Send Request bit
      uint8_t TXO27:1;  //Message Object 27 Send Request bit
      uint8_t TXO28:1;  //Message Object 28 Send Request bit
      uint8_t TXO29:1;  //Message Object 29 Send Request bit
      uint8_t TXO30:1;  //Message Object 30 Send Request bit
      uint8_t TXO31:1;  //Message Object 31 Send Request bit
   };
   uint32_t dw;
} CiTXREQ;

// CAN Transmit/Receive Error Count
typedef struct struct_CiTREC
{
   uint8_t REC;      //Receive Error Counter bits
   uint8_t TEC;      //Transmit Error Counter bits
   uint8_t EWARN:1;  //Transmitter or Receiver is in Error Warning State bit
   uint8_t RXWARN:1; //Receiver in Error Warning State bit
   uint8_t TXWARN:1; //Transmitter in Error Warning State bit
   uint8_t RXBP:1;   //Receiver in Error Passive State bit
   uint8_t TXBP:1;   //Transmitter in Error Passive State bit
   uint8_t TXBO:1;   //Transmitter in Bus Off State bit
   uint8_t u22_23:2;
   uint8_t u24_31;
} CiTREC;

// CAN Bus Diagnostic
typedef struct struct_CiBDIAG0
{
   uint8_t NRERRCNT; //Nominal Bit Rate Receive Error Counter bits
   uint8_t NTERRCNT; //Nominal Bit Rate Transmit Error Counter bits
   uint8_t DRERRCNT; //Data Bit Rate Receive Error Counter bits
   uint8_t DTERRCNT; //Data Bit Rate Transmit Error Counter bits
} CiBDIAG0;

typedef struct struct_CiBDIAG1
{
   uint16_t EFMSGCNT;   //Error Free Message Counter bits
   uint8_t NBIT0ERR:1;  //Nominal Bit Rate Dominant Level Error bit
   uint8_t NBIT1ERR:1;  //Nominal Bit Rate Recessive Level Error bit 
   uint8_t NACKERR:1;   //Nominal Bit Rate Transmitted Message was not Acknowledged bit
   uint8_t NFORMERR:1;  //Nominal Bit Rate Fixed Format Part of a Received Frame has the Wrong Format bit
   uint8_t NSTUFERR:1;  //Nominal Bit Rate Stuff Error bit
   uint8_t NCRCERR:1;   //Nominal Bit Rate CRC of Received Message Error bit
   uint8_t u22:1;
   uint8_t TXBOERR:1;   //Device went to bus-off
   uint8_t DBIT0ERR:1;  //Data Bit Rate Dominant Level Error bit
   uint8_t DBIT1ERR:1;  //Data Bit Rate Recessive Level Error bit 
   uint8_t u26:1;
   uint8_t DFORMERR:1;  //Data Bit Rate Fixed Format Part of a Received Frame has the Wrong Format bit
   uint8_t DSTUFERR:1;  //Data Bit Rate Stuff Error bit
   uint8_t ESI:1;       //ESI Flag of a Received CAN FD Message was Set bit
   uint8_t DLCMM:1;     //DLC Mismatch bit
} CiBDIAG1;

// CAN Transmit Event FIFO Control
typedef struct struct_CiTEFCON
{
   uint8_t TEFNEIE:1;   //Transmit Event FIFO Not Empty Interrupt Enable bit
   uint8_t TEFHIE:1;    //Transmit Event FIFO Half Full Interrupt Enable bit
   uint8_t TEFFIE:1;    //Transmit Event FIFO Full Interrupt Enable bit
   uint8_t TEFOVIE:1;   //Transmit Event FIFO Overflow Interrupt Enable bit
   uint8_t u4:1;
   uint8_t TEFTSEN:1;   //Transmit Event FIFO Time Stamp Enable bit
   uint8_t u6_7:2;
   uint8_t UINC:1;      //Increment Tail bit
   uint8_t u9:1;
   uint8_t FRSET:1;     //FIFO Reset bit
   uint8_t u11_15:5;
   uint8_t u16_23;
   uint8_t FSIZE;       //FIFO Size bits (0-31), Size=FSIZE+1
} CiTEFCON;

// CAN Transmit Event FIFO Status
typedef struct struct_CiTEFSTA
{
   uint8_t TEFNEIF:1;   //Transmit Event FIFO Not Empty Interrupt Flag bit
   uint8_t TEFHIF:1;    //Transmit Event FIFO Half Full Interrupt Flag bit
   uint8_t TEFFIF:1;    //Transmit Event FIFO Full Interrupt Flag bit
   uint8_t TEFOVIF:1;   //Transmit Event FIFO Overflow Interrupt Flag bit
   uint8_t u4_7:4;
   uint8_t u8_15;
   uint8_t u16_23;
   uint8_t u24_31;
} CiTEFSTA;

// CAN Transmit Event FIFO User Address
typedef uint32_t CiTEFUA;

// CAN Transmit Queue Control
typedef struct struct_CiTXQCON
{
   uint8_t TXQNIE:1;             //Transmit Queue Not Full Interrupt Enable bit
   uint8_t u1:1;
   uint8_t TXQEIE:1;             //Transmit Queue Empty Interrupt Enable bit
   uint8_t u3:1;
   uint8_t TXATIE:1;             //Transmit Attempts Exhausted Interrupt Enable bit
   uint8_t u5_6:2;
   uint8_t TXEN:1;               //TX Enable
   uint8_t UINC:1;               //Increment Head
   uint8_t TXREQ:1;              //Message Send Request bit
   uint8_t FRESET:1;             //FIFO Reset bit
   uint8_t u11_15:5;
   uint8_t TXPRI:5;              //Message Transmit Priority bits
   RETRANSMIT_ATTEMPTS TXAT:2;   //Retransmission Attempts bits
   uint8_t u23:1;
   uint8_t FSIZE:5;              //FIFO Size bits (0-31), Size=FSIZE + 1
   PAYLOAD_SIZE PLSIZE:3;        //Payload Size bits
} CiTXQCON;

// CAN Transmit Queue Status
typedef struct struct_CiTXQSTA
{
   uint8_t TXQNIF:1; //Transmit Queue not Full Interrupt Flag bit
   uint8_t u1:1;
   uint8_t TXQEIF:1; //Transmit Queue Empty Interrupt Flag bit
   uint8_t U3:1;
   uint8_t TXATIF:1; //Transmit Attempts Exhausted Pending bit
   uint8_t TXERR:1;
   uint8_t TXLARB:1; //Message Lost Arbitration Status bit
   uint8_t TXABT:1;  //Message Aborted Status bit
   uint8_t TXQCI;    //Transmit Queue Message Index bits
   uint8_t u16_23;
   uint8_t u24_31;
} CiTXQSTA;

// CAN Transmit Queue User Address
typedef uint32_t CiTXQUA;

// CAN FIFO Control
typedef struct struct_CiFIFOCON
{
   uint8_t TFNRFNIE:1;           //Transmit/Receive FIFO Not Full/Empty Interrupt Enable bit
   uint8_t TFHRFHIE:1;           //Transmit/Receive FIFO Half Empty/Full Interrupt Enable bit
   uint8_t TFERFFIE:1;           //Transmit/Receive FIFO Empty/Full Interrupt Enable bit
   uint8_t RXOVIE:1;             //Overflow Interrupt Enable bit
   uint8_t TXATIE:1;             //Transmit Attempts Exhausted Interrupt Enable bit
   uint8_t RXTSEN:1;             //Received Message Time Stamp Enable bit
   uint8_t RTREN:1;              //Auto RTR Enable bit
   uint8_t TXEN:1;               //TX/RX FIFO Selection bit
   uint8_t UINC:1;               //Increment Head/Tail bit
   uint8_t TRXREQ:1;             //Message Send Request bit
   uint8_t FRESET:1;             //FIFO Reset bit
   uint8_t u11_15:5;
   uint8_t TXPRI:5;              //Message Transmit Priority bits
   RETRANSMIT_ATTEMPTS TXAT:2;   //Retransmission Attempts bits
   uint8_t u23:1;
   uint8_t FSIZE:5;              //FIFO Size bits
   PAYLOAD_SIZE PLSIZE:3;        //Payload Size bits
} CiFIFOCON;

// CAN FIFO Status
typedef struct struct_CiFIFOSTA
{
   uint8_t TFNRFNIF:1;     //Transmit/Receive FIFO Not Full/Empty Interrupt Flag bit
   uint8_t TFHRFHIF:1;     //Transmit/Receive FIFO Half Empty/Full Interrupt Flag bit
   uint8_t TFERFFIF:1;     //Transmit/Receive FIFO Empty/Full Interrupt Flag bit
   uint8_t RXOVIF:1;       //Receive FIFO Overflow Interrupt Flag bit
   uint8_t TXATIF:1;       //Transmit Attempts Exhausted Interrupt Pending bit
   uint8_t TXERR:1;        //Error Detected During Transmission bit
   uint8_t TXLARB:1;       //Message Lost Arbitration Status bit
   uint8_t TXABT:1;        //Message Aborted Status bit
   uint8_t FIFOCI;         //FIFO Message Index bits.
   uint8_t u16_23;
   uint8_t u24_31;
} CiFIFOSTA;

// CAN FIFO User Address
typedef uint32_t CiFIFOUA;

// CAN Filter Control
typedef struct struct_FLTCON
{
   uint8_t BP:5;     //Pointer to FIFO when Filter hits
   uint8_t u5_6:2;
   uint8_t FLTEN:1;  //Enable Filter
} FLTCON;

typedef struct struct_CiFLTCON
{
   FLTCON F0;  //Filter 0
   FLTCON F1;  //Filter 1
   FLTCON F2;  //Filter 2
   FLTCON F3;  //Filter 3
} CiFLTCON;

// CAN Filter Object
typedef union union_CiFLTOBJ
{
   struct
   {
      uint8_t SID0_7;
      uint8_t SID8_10:3;
      uint8_t EID0_4:5;
      uint8_t EID5_12;
      uint8_t EID13_17:5;
      uint8_t SID11:1;
      uint8_t EXIDE:1;
      uint8_t u31:1;
   };
   uint16_t SID;     //need to mask off upper 5 bits if this element is used
   uint32_t EID;     //need to mask off upper 3 bits if this element is used
} CiFLTOBJ;

// CAN Mask
typedef union union_CiMASK
{
   struct
   {
      uint8_t MSID0_7;
      uint8_t MSID8_10:3;
      uint8_t MEID0_4:5;
      uint8_t MEID5_12;
      uint8_t MEID13_17:5;
      uint8_t MSID11:1;
      uint8_t MIDE:1;
      uint8_t u31:1;
   };
   uint16_t MSID;    //need to mask off upper 5 bits if this element is used
   uint32_t MEID;    //need to mask off upper 3 bits if this element is used
} CiMASK;

typedef enum 
{
   DATA_LENGTH_0,
   DATA_LENGTH_1,
   DATA_LENGTH_2,
   DATA_LENGTH_3,
   DATA_LENGTH_4,
   DATA_LENGTH_5,
   DATA_LENGTH_6,
   DATA_LENGTH_7,
   DATA_LENGTH_8,
   DATA_LENGTH_12,   //CAN FD Only, 8 bytes for CAN 2.0
   DATA_LENGTH_16,   //CAN FD Only, 8 bytes for CAN 2.0
   DATA_LENGTH_20,   //CAN FD Only, 8 bytes for CAN 2.0
   DATA_LENGTH_24,   //CAN FD Only, 8 bytes for CAN 2.0
   DATA_LENGTH_32,   //CAN FD Only, 8 bytes for CAN 2.0
   DATA_LENGTH_48,   //CAN FD Only, 8 bytes for CAN 2.0
   DATA_LENGTH_64    //CAN FD Only, 8 bytes for CAN 2.0
} DATA_LENGTH;

// CAN Transmit Message Object
typedef struct
{
   union
   {
      struct
      {
         uint8_t SID0_7;
         uint8_t SID8_10:3;
         uint8_t EID0_4:5;
         uint8_t EID5_12;
         uint8_t EID13_17:5;
         uint8_t SID11:1;
         uint8_t u30_31:2;
      };
      uint16_t SID;     //need to mask off upper 5 bits if this element is used
      uint32_t EID;     //need to mask off upper 3 bits if this element is used
   };
   DATA_LENGTH DLC:4;
   uint8_t IDE:1;
   uint8_t RTR:1;       //CAN 2.0 only
   uint8_t BRS:1;
   uint8_t FDF:1;
   uint8_t ESI:1;
   uint8_t SEQ:7;
   uint8_t u16_32;
   uint8_t u24_31;
} CAN_TX_MESSAGE_OBJECT;

// CAN Receive Message Object
typedef struct
{
   union
   {
      struct
      {
         uint8_t SID0_7;
         uint8_t SID8_10:3;
         uint8_t EID0_4:5;
         uint8_t EID5_12;
         uint8_t EID13_17:5;
         uint8_t SID11:1;
         uint8_t u30_31:2;
      };
      uint16_t SID;     //need to mask off upper 5 bits if this element is used
      uint32_t EID;     //need to mask off upper 3 bits if this element is used
   };
   DATA_LENGTH DLC:4;
   uint8_t IDE:1;
   uint8_t RTR:1;       //CAN 2.0 only
   uint8_t BRS:1;
   uint8_t FDF:1;
   uint8_t ESI:1;
   uint8_t u9_10:2;
   uint8_t FILHIT:5;
   uint8_t u16_23;
   uint8_t u24_31;
} CAN_RX_MESSAGE_OBJECT;

//CAN Transmit Event FIFO Object
typedef struct
{
   union
   {
      struct
      {
         uint8_t SID0_7;
         uint8_t SID8_10:3;
         uint8_t EID0_4:5;
         uint8_t EID5_12;
         uint8_t EID13_17:5;
         uint8_t SID11:1;
         uint8_t u30_31:2;
      };
      uint16_t SID;     //need to mask off upper 5 bits if this element is used
      uint32_t EID;     //need to mask off upper 3 bits if this element is used
   };
   DATA_LENGTH DLC:4;
   uint8_t IDE:1;
   uint8_t RTR:1;       //CAN 2.0 only
   uint8_t BRS:1;
   uint8_t FDF:1;
   uint8_t ESI:1;
   uint8_t SEQ:7;
   uint8_t u16_32;
   uint8_t u24_31;
   uint32_t TXMSGTX;    //Transmit Message Time Stamp
} CAN_TX_EVENT_OBJECT;

// Other Types

typedef union
{
   uint8_t b[2];
   uint16_t w;
} CRC16_T;

typedef union
{
   uint8_t b[4];
   uint32_t dw;
} DW_T;

////////////////////////// Other CAN Settings/Defines /////////////////////////

/*
   If more then the default TX Queue Object and FIFO 1 Object are used it is
   possible to have multiple RX and/or TX Objects with different payload sizes.
   The following determines the absolute max payload sizes for both the RX and
   TX objects and defines them as CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE and
   CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE.  These defines are used in some of the CAN
   functions and can be used to create data buffers that are large enough to 
   handle the data portion of the messages.
*/

#if ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     64
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     48
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     32
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     24
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     20
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     16
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     12
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     8
#else
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     0
#endif

#if ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     64
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     48
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     32
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     24
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     20
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     16
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     12
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_8_OBJECT) && (CAN_FIFO_8_OBJECT_TYPE == 1) && (CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_9_OBJECT) && (CAN_FIFO_9_OBJECT_TYPE == 1) && (CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_10_OBJECT) && (CAN_FIFO_10_OBJECT_TYPE == 1) && (CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_11_OBJECT) && (CAN_FIFO_11_OBJECT_TYPE == 1) && (CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_12_OBJECT) && (CAN_FIFO_12_OBJECT_TYPE == 1) && (CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_13_OBJECT) && (CAN_FIFO_13_OBJECT_TYPE == 1) && (CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_14_OBJECT) && (CAN_FIFO_14_OBJECT_TYPE == 1) && (CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_15_OBJECT) && (CAN_FIFO_15_OBJECT_TYPE == 1) && (CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_16_OBJECT) && (CAN_FIFO_16_OBJECT_TYPE == 1) && (CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_17_OBJECT) && (CAN_FIFO_17_OBJECT_TYPE == 1) && (CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_18_OBJECT) && (CAN_FIFO_18_OBJECT_TYPE == 1) && (CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_19_OBJECT) && (CAN_FIFO_19_OBJECT_TYPE == 1) && (CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_20_OBJECT) && (CAN_FIFO_20_OBJECT_TYPE == 1) && (CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_21_OBJECT) && (CAN_FIFO_21_OBJECT_TYPE == 1) && (CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_22_OBJECT) && (CAN_FIFO_22_OBJECT_TYPE == 1) && (CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_23_OBJECT) && (CAN_FIFO_23_OBJECT_TYPE == 1) && (CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_24_OBJECT) && (CAN_FIFO_24_OBJECT_TYPE == 1) && (CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_25_OBJECT) && (CAN_FIFO_25_OBJECT_TYPE == 1) && (CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_26_OBJECT) && (CAN_FIFO_26_OBJECT_TYPE == 1) && (CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_27_OBJECT) && (CAN_FIFO_27_OBJECT_TYPE == 1) && (CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_28_OBJECT) && (CAN_FIFO_28_OBJECT_TYPE == 1) && (CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_29_OBJECT) && (CAN_FIFO_29_OBJECT_TYPE == 1) && (CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_30_OBJECT) && (CAN_FIFO_30_OBJECT_TYPE == 1) && (CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_31_OBJECT) && (CAN_FIFO_31_OBJECT_TYPE == 1) && (CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     8
#else
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     0
#endif

/*
   The following is used to determine the amount of RAM required to hold all
   the messages for all the Objects, TX , TX Queue and FIFO 1 to 31.  Which
   is then used to determine if MCP2517FD has enough RAM for all the Objects.
*/

#if CAN_TX_BUFFERS > 0
 #if CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_TX_MESSAGE_SIZE    16
 #elif CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_TX_MESSAGE_SIZE    20
 #elif CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_TX_MESSAGE_SIZE    24
 #elif CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_TX_MESSAGE_SIZE    28
 #elif CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_TX_MESSAGE_SIZE    32
 #elif CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_TX_MESSAGE_SIZE    40
 #elif CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_TX_MESSAGE_SIZE    65
 #else
  #define CAN_TX_MESSAGE_SIZE    72
 #endif
#else
 #define CAN_TX_MESSAGE_SIZE     0
#endif

#if CAN_RX_BUFFERS > 0
 #if CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_RX_MESSAGE_SIZE    (16 + (4 * CAN_RX_TIME_STAMP))
 #elif CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_RX_MESSAGE_SIZE    (20 + (4 * CAN_RX_TIME_STAMP))
 #elif CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_RX_MESSAGE_SIZE    (24 + (4 * CAN_RX_TIME_STAMP))
 #elif CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_RX_MESSAGE_SIZE    (28 + (4 * CAN_RX_TIME_STAMP))
 #elif CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_RX_MESSAGE_SIZE    (32 + (4 * CAN_RX_TIME_STAMP))
 #elif CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_RX_MESSAGE_SIZE    (40 + (4 * CAN_RX_TIME_STAMP))
 #elif CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_RX_MESSAGE_SIZE    (56 + (4 * CAN_RX_TIME_STAMP))
 #else
  #define CAN_RX_MESSAGE_SIZE    (72 + (4 * CAN_RX_TIME_STAMP))
 #endif
#else
 #define CAN_RX_MESSAGE_SIZE     0
#endif

#ifdef CAN_USE_FIFO_1_OBJECT
 #if (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_1_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_1_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_1_OBJECT_COUNT)
 #elif CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_1_OBJECT_COUNT)
 #elif CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_1_OBJECT_COUNT)
 #elif CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_1_OBJECT_COUNT)
 #elif CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_1_OBJECT_COUNT)
 #elif CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_1_OBJECT_COUNT)
 #elif CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_1_OBJECT_COUNT)
 #else
  #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_1_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_1_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_2_OBJECT
 #if (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_2_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_2_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_2_OBJECT_COUNT)
 #elif CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_2_OBJECT_COUNT)
 #elif CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_2_OBJECT_COUNT)
 #elif CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_2_OBJECT_COUNT)
 #elif CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_2_OBJECT_COUNT)
 #elif CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_2_OBJECT_COUNT)
 #elif CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_2_OBJECT_COUNT)
 #else
  #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_2_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_2_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_3_OBJECT
 #if (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_3_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_3_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_3_OBJECT_COUNT)
 #elif CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_3_OBJECT_COUNT)
 #elif CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_3_OBJECT_COUNT)
 #elif CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_3_OBJECT_COUNT)
 #elif CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_3_OBJECT_COUNT)
 #elif CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_3_OBJECT_COUNT)
 #elif CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_3_OBJECT_COUNT)
 #else
  #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_3_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_3_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_4_OBJECT
 #if (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_4_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_4_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_4_OBJECT_COUNT)
 #elif CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_4_OBJECT_COUNT)
 #elif CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_4_OBJECT_COUNT)
 #elif CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_4_OBJECT_COUNT)
 #elif CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_4_OBJECT_COUNT)
 #elif CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_4_OBJECT_COUNT)
 #elif CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_4_OBJECT_COUNT)
 #else
  #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_4_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_4_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_5_OBJECT
 #if (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_5_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_5_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_5_OBJECT_COUNT)
 #elif CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_5_OBJECT_COUNT)
 #elif CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_5_OBJECT_COUNT)
 #elif CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_5_OBJECT_COUNT)
 #elif CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_5_OBJECT_COUNT)
 #elif CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_5_OBJECT_COUNT)
 #elif CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_5_OBJECT_COUNT)
 #else
  #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_5_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_5_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_6_OBJECT
 #if (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_6_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_6_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_6_OBJECT_COUNT)
 #elif CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_6_OBJECT_COUNT)
 #elif CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_6_OBJECT_COUNT)
 #elif CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_6_OBJECT_COUNT)
 #elif CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_6_OBJECT_COUNT)
 #elif CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_6_OBJECT_COUNT)
 #elif CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_6_OBJECT_COUNT)
 #else
  #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_6_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_6_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_7_OBJECT
 #if (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_7_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_7_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_7_OBJECT_COUNT)
 #elif CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_7_OBJECT_COUNT)
 #elif CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_7_OBJECT_COUNT)
 #elif CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_7_OBJECT_COUNT)
 #elif CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_7_OBJECT_COUNT)
 #elif CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_7_OBJECT_COUNT)
 #elif CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_7_OBJECT_COUNT)
 #else
  #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_7_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_7_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_8_OBJECT
 #if (CAN_FIFO_8_OBJECT_TYPE == 0) && (CAN_FIFO_8_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_8_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_8_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_8_OBJECT_COUNT)
 #elif CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_8_OBJECT_COUNT)
 #elif CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_8_OBJECT_COUNT)
 #elif CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_8_OBJECT_COUNT)
 #elif CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_8_OBJECT_COUNT)
 #elif CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_8_OBJECT_COUNT)
 #elif CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_8_OBJECT_COUNT)
 #else
  #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_8_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_8_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_9_OBJECT
 #if (CAN_FIFO_9_OBJECT_TYPE == 0) && (CAN_FIFO_9_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_9_OBJECT_HEADER_SIZE     12
 #else
  #define FIFO_9_OBJECT_HEADER_SIZE     8
 #endif
 
 #if CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_9_OBJECT_COUNT)
 #elif CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_9_OBJECT_COUNT)
 #elif CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_9_OBJECT_COUNT)
 #elif CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_9_OBJECT_COUNT)
 #elif CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_9_OBJECT_COUNT)
 #elif CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_9_OBJECT_COUNT)
 #elif CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_9_OBJECT_COUNT)
 #else
  #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE    ((FIFO_9_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_9_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE     0
#endif

#ifdef CAN_USE_FIFO_10_OBJECT
 #if (CAN_FIFO_10_OBJECT_TYPE == 0) && (CAN_FIFO_10_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_10_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_10_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_10_OBJECT_COUNT)
 #elif CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_10_OBJECT_COUNT)
 #elif CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_10_OBJECT_COUNT)
 #elif CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_10_OBJECT_COUNT)
 #elif CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_10_OBJECT_COUNT)
 #elif CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_10_OBJECT_COUNT)
 #elif CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_10_OBJECT_COUNT)
 #else
  #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_10_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_10_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_11_OBJECT
 #if (CAN_FIFO_11_OBJECT_TYPE == 0) && (CAN_FIFO_11_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_11_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_11_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_11_OBJECT_COUNT)
 #elif CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_11_OBJECT_COUNT)
 #elif CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_11_OBJECT_COUNT)
 #elif CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_11_OBJECT_COUNT)
 #elif CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_11_OBJECT_COUNT)
 #elif CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_11_OBJECT_COUNT)
 #elif CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_11_OBJECT_COUNT)
 #else
  #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_11_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_11_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_12_OBJECT
 #if (CAN_FIFO_12_OBJECT_TYPE == 0) && (CAN_FIFO_12_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_12_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_12_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_12_OBJECT_COUNT)
 #elif CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_12_OBJECT_COUNT)
 #elif CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_12_OBJECT_COUNT)
 #elif CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_12_OBJECT_COUNT)
 #elif CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_12_OBJECT_COUNT)
 #elif CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_12_OBJECT_COUNT)
 #elif CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_12_OBJECT_COUNT)
 #else
  #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_12_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_12_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_13_OBJECT
 #if (CAN_FIFO_13_OBJECT_TYPE == 0) && (CAN_FIFO_13_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_13_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_13_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_13_OBJECT_COUNT)
 #elif CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_13_OBJECT_COUNT)
 #elif CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_13_OBJECT_COUNT)
 #elif CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_13_OBJECT_COUNT)
 #elif CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_13_OBJECT_COUNT)
 #elif CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_13_OBJECT_COUNT)
 #elif CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_13_OBJECT_COUNT)
 #else
  #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_13_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_13_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_14_OBJECT
 #if (CAN_FIFO_14_OBJECT_TYPE == 0) && (CAN_FIFO_14_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_14_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_14_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_14_OBJECT_COUNT)
 #elif CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_14_OBJECT_COUNT)
 #elif CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_14_OBJECT_COUNT)
 #elif CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_14_OBJECT_COUNT)
 #elif CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_14_OBJECT_COUNT)
 #elif CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_14_OBJECT_COUNT)
 #elif CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_14_OBJECT_COUNT)
 #else
  #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_14_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_14_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_15_OBJECT
 #if (CAN_FIFO_15_OBJECT_TYPE == 0) && (CAN_FIFO_15_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_15_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_15_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_15_OBJECT_COUNT)
 #elif CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_15_OBJECT_COUNT)
 #elif CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_15_OBJECT_COUNT)
 #elif CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_15_OBJECT_COUNT)
 #elif CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_15_OBJECT_COUNT)
 #elif CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_15_OBJECT_COUNT)
 #elif CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_15_OBJECT_COUNT)
 #else
  #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_15_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_15_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_16_OBJECT
 #if (CAN_FIFO_16_OBJECT_TYPE == 0) && (CAN_FIFO_16_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_16_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_16_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_16_OBJECT_COUNT)
 #elif CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_16_OBJECT_COUNT)
 #elif CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_16_OBJECT_COUNT)
 #elif CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_16_OBJECT_COUNT)
 #elif CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_16_OBJECT_COUNT)
 #elif CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_16_OBJECT_COUNT)
 #elif CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_16_OBJECT_COUNT)
 #else
  #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_16_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_16_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_17_OBJECT
 #if (CAN_FIFO_17_OBJECT_TYPE == 0) && (CAN_FIFO_17_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_17_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_17_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_17_OBJECT_COUNT)
 #elif CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_17_OBJECT_COUNT)
 #elif CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_17_OBJECT_COUNT)
 #elif CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_17_OBJECT_COUNT)
 #elif CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_17_OBJECT_COUNT)
 #elif CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_17_OBJECT_COUNT)
 #elif CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_17_OBJECT_COUNT)
 #else
  #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_17_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_17_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_18_OBJECT
 #if (CAN_FIFO_18_OBJECT_TYPE == 0) && (CAN_FIFO_18_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_18_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_18_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_18_OBJECT_COUNT)
 #elif CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_18_OBJECT_COUNT)
 #elif CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_18_OBJECT_COUNT)
 #elif CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_18_OBJECT_COUNT)
 #elif CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_18_OBJECT_COUNT)
 #elif CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_18_OBJECT_COUNT)
 #elif CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_18_OBJECT_COUNT)
 #else
  #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_18_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_18_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_19_OBJECT
 #if (CAN_FIFO_19_OBJECT_TYPE == 0) && (CAN_FIFO_19_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_19_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_19_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_19_OBJECT_COUNT)
 #elif CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_19_OBJECT_COUNT)
 #elif CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_19_OBJECT_COUNT)
 #elif CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_19_OBJECT_COUNT)
 #elif CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_19_OBJECT_COUNT)
 #elif CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_19_OBJECT_COUNT)
 #elif CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_19_OBJECT_COUNT)
 #else
  #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_19_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_19_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_20_OBJECT
 #if (CAN_FIFO_20_OBJECT_TYPE == 0) && (CAN_FIFO_20_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_20_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_20_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_20_OBJECT_COUNT)
 #elif CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_20_OBJECT_COUNT)
 #elif CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_20_OBJECT_COUNT)
 #elif CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_20_OBJECT_COUNT)
 #elif CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_20_OBJECT_COUNT)
 #elif CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_20_OBJECT_COUNT)
 #elif CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_20_OBJECT_COUNT)
 #else
  #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_20_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_20_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_21_OBJECT
 #if (CAN_FIFO_21_OBJECT_TYPE == 0) && (CAN_FIFO_21_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_21_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_21_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_21_OBJECT_COUNT)
 #elif CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_21_OBJECT_COUNT)
 #elif CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_21_OBJECT_COUNT)
 #elif CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_21_OBJECT_COUNT)
 #elif CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_21_OBJECT_COUNT)
 #elif CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_21_OBJECT_COUNT)
 #elif CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_21_OBJECT_COUNT)
 #else
  #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_21_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_21_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_22_OBJECT
 #if (CAN_FIFO_22_OBJECT_TYPE == 0) && (CAN_FIFO_22_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_22_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_22_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_22_OBJECT_COUNT)
 #elif CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_22_OBJECT_COUNT)
 #elif CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_22_OBJECT_COUNT)
 #elif CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_22_OBJECT_COUNT)
 #elif CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_22_OBJECT_COUNT)
 #elif CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_22_OBJECT_COUNT)
 #elif CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_22_OBJECT_COUNT)
 #else
  #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_22_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_22_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_23_OBJECT
 #if (CAN_FIFO_23_OBJECT_TYPE == 0) && (CAN_FIFO_23_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_23_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_23_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_23_OBJECT_COUNT)
 #elif CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_23_OBJECT_COUNT)
 #elif CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_23_OBJECT_COUNT)
 #elif CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_23_OBJECT_COUNT)
 #elif CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_23_OBJECT_COUNT)
 #elif CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_23_OBJECT_COUNT)
 #elif CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_23_OBJECT_COUNT)
 #else
  #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_23_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_23_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_24_OBJECT
 #if (CAN_FIFO_24_OBJECT_TYPE == 0) && (CAN_FIFO_24_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_24_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_24_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_24_OBJECT_COUNT)
 #elif CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_24_OBJECT_COUNT)
 #elif CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_24_OBJECT_COUNT)
 #elif CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_24_OBJECT_COUNT)
 #elif CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_24_OBJECT_COUNT)
 #elif CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_24_OBJECT_COUNT)
 #elif CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_24_OBJECT_COUNT)
 #else
  #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_24_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_24_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_25_OBJECT
 #if (CAN_FIFO_25_OBJECT_TYPE == 0) && (CAN_FIFO_25_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_25_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_25_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_25_OBJECT_COUNT)
 #elif CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_25_OBJECT_COUNT)
 #elif CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_25_OBJECT_COUNT)
 #elif CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_25_OBJECT_COUNT)
 #elif CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_25_OBJECT_COUNT)
 #elif CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_25_OBJECT_COUNT)
 #elif CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_25_OBJECT_COUNT)
 #else
  #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_25_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_25_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_26_OBJECT
 #if (CAN_FIFO_26_OBJECT_TYPE == 0) && (CAN_FIFO_26_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_26_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_26_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_26_OBJECT_COUNT)
 #elif CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_26_OBJECT_COUNT)
 #elif CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_26_OBJECT_COUNT)
 #elif CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_26_OBJECT_COUNT)
 #elif CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_26_OBJECT_COUNT)
 #elif CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_26_OBJECT_COUNT)
 #elif CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_26_OBJECT_COUNT)
 #else
  #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_26_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_26_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_27_OBJECT
 #if (CAN_FIFO_27_OBJECT_TYPE == 0) && (CAN_FIFO_27_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_27_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_27_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_27_OBJECT_COUNT)
 #elif CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_27_OBJECT_COUNT)
 #elif CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_27_OBJECT_COUNT)
 #elif CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_27_OBJECT_COUNT)
 #elif CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_27_OBJECT_COUNT)
 #elif CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_27_OBJECT_COUNT)
 #elif CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_27_OBJECT_COUNT)
 #else
  #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_27_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_27_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_28_OBJECT
 #if (CAN_FIFO_28_OBJECT_TYPE == 0) && (CAN_FIFO_28_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_28_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_28_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_28_OBJECT_COUNT)
 #elif CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_28_OBJECT_COUNT)
 #elif CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_28_OBJECT_COUNT)
 #elif CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_28_OBJECT_COUNT)
 #elif CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_28_OBJECT_COUNT)
 #elif CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_28_OBJECT_COUNT)
 #elif CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_28_OBJECT_COUNT)
 #else
  #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_28_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_28_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_29_OBJECT
 #if (CAN_FIFO_29_OBJECT_TYPE == 0) && (CAN_FIFO_29_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_29_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_29_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_29_OBJECT_COUNT)
 #elif CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_29_OBJECT_COUNT)
 #elif CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_29_OBJECT_COUNT)
 #elif CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_29_OBJECT_COUNT)
 #elif CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_29_OBJECT_COUNT)
 #elif CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_29_OBJECT_COUNT)
 #elif CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_29_OBJECT_COUNT)
 #else
  #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_29_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_29_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_30_OBJECT
 #if (CAN_FIFO_30_OBJECT_TYPE == 0) && (CAN_FIFO_30_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_30_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_30_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_30_OBJECT_COUNT)
 #elif CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_30_OBJECT_COUNT)
 #elif CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_30_OBJECT_COUNT)
 #elif CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_30_OBJECT_COUNT)
 #elif CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_30_OBJECT_COUNT)
 #elif CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_30_OBJECT_COUNT)
 #elif CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_30_OBJECT_COUNT)
 #else
  #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_30_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_30_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#ifdef CAN_USE_FIFO_31_OBJECT
 #if (CAN_FIFO_31_OBJECT_TYPE == 0) && (CAN_FIFO_31_OBJECT_RX_TIME_STAMP == TRUE)
  #define FIFO_31_OBJECT_HEADER_SIZE   12
 #else
  #define FIFO_31_OBJECT_HEADER_SIZE   8
 #endif
 
 #if CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 8) * CAN_FIFO_31_OBJECT_COUNT)
 #elif CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 12) * CAN_FIFO_31_OBJECT_COUNT)
 #elif CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 16) * CAN_FIFO_31_OBJECT_COUNT)
 #elif CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 20) * CAN_FIFO_31_OBJECT_COUNT)
 #elif CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 24) * CAN_FIFO_31_OBJECT_COUNT)
 #elif CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 32) * CAN_FIFO_31_OBJECT_COUNT)
 #elif CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 48) * CAN_FIFO_31_OBJECT_COUNT)
 #else
  #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE   ((FIFO_31_OBJECT_HEADER_SIZE + 64) * CAN_FIFO_31_OBJECT_COUNT)
 #endif
#else
 #define CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE    0
#endif

#if CAN_ENABLE_TX_EVENT_OBJECT == TRUE
 #define CAN_MESSAGE_RAM_BYTES   (((8 + (4 * CAN_TX_EVENT_TIME_STAMP)) * CAN_TX_EVENT_FIFO_SIZE) + \
                                  (CAN_TX_MESSAGE_SIZE * CAN_TX_BUFFERS) + \
                                  (CAN_RX_MESSAGE_SIZE * CAN_RX_BUFFERS) + \
                                  CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE)
#else
 #define CAN_MESSAGE_RAM_BYTES   ((CAN_TX_MESSAGE_SIZE * CAN_TX_BUFFERS) + \
                                  (CAN_RX_MESSAGE_SIZE * CAN_RX_BUFFERS) + \
                                  CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_8_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_9_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_10_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_11_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_12_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_13_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_14_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_15_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_16_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_17_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_18_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_19_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_20_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_21_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_22_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_23_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_24_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_25_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_26_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_27_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_28_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_29_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_30_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_31_OBJECT_MESSAGE_BUFFER_SIZE)
#endif

#if CAN_MESSAGE_RAM_BYTES > 2048
 #error MCP2517FD doesn't have enough message RAM for all objects
#endif

/////////////////////////////// Global Variables //////////////////////////////

CAN_OP_MODE g_CANOpmode = CAN_OP_CONFIG;
uint8_t g_CANTXSequence = 0;


////////////////////////////// MCP2517 Prototypes /////////////////////////////

// CAN Prototypes

void can_set_tx_obj_header(CAN_TX_MESSAGE_OBJECT *Ptr, CAN_TX_HEADER *Header);
uint8_t can_set_tx_obj_data(uint8_t *wData, uint8_t *rData, DATA_LENGTH Length);
void can_get_rx_header(CAN_RX_HEADER *Header, CAN_RX_MESSAGE_OBJECT *Ptr);

DATA_LENGTH can_get_dlength_from_bytes(uint8_t dBytes);
uint8_t can_get_bytes_from_dlength(DATA_LENGTH dLength);

// SPI Prototypes

int1 mcp2517_read_sfr(MCP2517_ADDRESS Address, uint8_t *rValue);
int1 mcp2517_read_sfr_bytes(MCP2517_ADDRESS Address, uint8_t *rValue, uint8_t Count);
int1 mcp2517_read_memory(MCP2517_ADDRESS Address, DW_T *rValue);
int1 mcp2517_read_memory_dwords(MCP2517_ADDRESS Address, DW_T *rValue, uint8_t Count);

int1 mcp2517_write_sfr(MCP2517_ADDRESS Address, uint8_t wValue);
int1 mcp2517_write_sfr_bytes(MCP2517_ADDRESS Address, uint8_t *Data, uint8_t Count);
int1 mcp2517_write_memory(MCP2517_ADDRESS Address, DW_T wValue);
int1 mcp2517_write_memory_dwords(MCP2517_ADDRESS Address, DW_T *wValue, uint8_t Count);

void mcp2517_init(void);

#ifdef MCP2517_SPI_USE_CRC
 uint16_t calculate_crc16(uint8_t *Data, uint16_t Length, int1 New=FALSE);
#endif

////////////////////////////////// Functions //////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// can_init() 
//
// Initializes the CAN to the default settings that were defined before this
// driver was included or the default defines set by the can-mcp2517.h file.
//
// Parameters:
//    Mode - Optional parameter specifying the operational mode to put the CAN 
//           FD peripheral in before exiting, defaults to CAN_OP_NORMAL_FD mode
//           if CAN_OPERATION_MODE is defined to 0 or CAN_OP_NORMAL_CAN_2_0 if
//           CAN_OPERATION_MODE is defined to 1 if not specified.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
#if CAN_OPERATION_MODE == 0
 void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL_FD)
#else
 void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL_CAN_2_0)
#endif
{
   uint32_t Config1;
   
   mcp2517_init();
   
   can_set_mode(CAN_OP_CONFIG);
   
   //Setup CAN Bit rates
   can_set_baud(CAN_CLOCK_SPEED, CAN_NOMINAL_BAUD_RATE, CAN_DATA_BAUD_RATE);
   
   //Setup CAN Transmitter Delay Compensation Register
   ((CiTDC)Config1).TDCV = CAN_TDC_VALUE;
   ((CiTDC)Config1).TDCO = CAN_TDC_OFFSET;
   ((CiTDC)Config1).TDCMOD = CAN_TDC_MODE;
   ((CiTDC)Config1).SID11 = CAN_SID11;
   ((CiTDC)Config1).EDGFLT = CAN_EDGE_FLT;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1TDC, (uint8_t *)&Config1, 4));
   
   //Setup CAN Time Stamp Control Register
   ((CiTSCON)Config1).TBCPRE = (CAN_TIME_BASE_COUNTER_PRESCALAR - 1);
   ((CiTSCON)Config1).TBCEN = CAN_TIME_BASE_COUNTER_ENABLE;
   ((CiTSCON)Config1).TSEOF = CAN_TIME_STAMP_EOF;
   ((CiTSCON)Config1).TSRES = CAN_TIME_STAMP_RES;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1TSCON, (uint8_t *)&Config1, 3));
   
   //Setup Can Control Register
   while(!mcp2517_read_sfr_bytes(MCP2517_ADDR_C1CON, (uint8_t *)&Config1, 4));
   
   ((CiCON)Config1).DNCNT = CAN_DEVICE_NET_FILTER_BIT_NUMBER;
   ((CiCON)Config1).ISOCRC = CAN_ENABLE_ISO_CRC;
   ((CiCON)Config1).PXE = CAN_DISABLE_EXCEPTION_EVENT;
   ((CiCON)Config1).WAKFIL = CAN_ENABLE_WAKE_FILTER;
   ((CiCON)Config1).WFT = CAN_WAKE_FILTER_TIME;
   ((CiCON)Config1).BRS = CAN_DISABLE_BIT_RATE_SWITCHING;
   ((CiCON)Config1).RTXAT = CAN_RESTRICT_RETANSMISSION_ATTEMPTS;
   ((CiCON)Config1).ESIGM = CAN_TRANSMIT_ESI_MODE;
   ((CiCON)Config1).SERR2LOM = CAN_TRANSITION_TO_LISTEN;
   ((CiCON)Config1).STEF = CAN_ENABLE_TX_EVENT_OBJECT;
  #if CAN_TX_BUFFERS > 0
   ((CiCON)Config1).TXQ = TRUE;
  #else
   ((CiCON)Config1).TXQ = FALSE;
  #endif
   ((CiCON)Config1).TXBWS = CAN_TRANSMISSION_DELAY;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1CON, (uint8_t *)&Config1, 4));
   
   //Setup CAN TX Event
   Config1 = 0;
   
  #if CAN_ENABLE_TX_EVENT_OBJECT == TRUE
   ((CiTEFCON)Config1).TEFTSEN = CAN_TX_EVENT_TIME_STAMP;
   ((CiTEFCON)Config1).FSIZE = (CAN_TX_EVENT_FIFO_SIZE - 1);
  #endif
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1TEFCON, (uint8_t *)&Config1, 4));
   
   //Setup TX Queue
   Config1 = 0;
   
  #if CAN_TX_BUFFERS > 0
   ((CiTXQCON)Config1).TXEN = 1;
   ((CiTXQCON)Config1).TXPRI = CAN_TX_PRIORITY;
   ((CiTXQCON)Config1).TXAT = CAN_TX_RETRANSMISSION_ATTEMPTS;
   ((CiTXQCON)Config1).FSIZE = (CAN_TX_BUFFERS - 1);
   ((CiTXQCON)Config1).PLSIZE = CAN_TX_MAX_PAYLOAD_SIZE;
  #endif
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1TXQCON, (uint8_t *)&Config1, 4));
   
   //Setup FIFO RX Object
   Config1 = 0;
   
 #if CAN_RX_BUFFERS > 0
   ((CiFIFOCON)Config1).TXEN = 0;    //Receive FIFO
   ((CiFIFOCON)Config1).FSIZE = (CAN_RX_BUFFERS - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_RX_MAX_PAYLOAD_SIZE;
   ((CiFIFOCON)Config1).RXTSEN = CAN_RX_TIME_STAMP;
 #elif defined(CAN_USE_FIFO_1_OBJECT)
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_1_OBJECT_TYPE;
  #if CAN_FIFO_1_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_1_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_1_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_1_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_1_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE;
 #endif
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON1, (uint8_t *)&Config1, 4));
   
   //Setup FIFO 2 Object
 #ifdef CAN_USE_FIFO_2_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_2_OBJECT_TYPE;
  #if CAN_FIFO_2_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_2_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_2_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_2_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_2_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON2, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 3 Object
 #ifdef CAN_USE_FIFO_3_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_3_OBJECT_TYPE;
  #if CAN_FIFO_3_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_3_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_3_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_3_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_3_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON3, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 4 Object
 #ifdef CAN_USE_FIFO_4_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_4_OBJECT_TYPE;
  #if CAN_FIFO_4_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_4_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_4_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_4_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_4_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON4, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 5 Object
 #ifdef CAN_USE_FIFO_5_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_5_OBJECT_TYPE;
  #if CAN_FIFO_5_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_5_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_5_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_5_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_5_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON5, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 6 Object
 #ifdef CAN_USE_FIFO_6_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_6_OBJECT_TYPE;
  #if CAN_FIFO_6_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_6_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_6_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_6_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_6_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON6, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 7 Object
 #ifdef CAN_USE_FIFO_7_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_7_OBJECT_TYPE;
  #if CAN_FIFO_7_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_7_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_7_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_7_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_7_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON7, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 8 Object
 #ifdef CAN_USE_FIFO_8_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_8_OBJECT_TYPE;
  #if CAN_FIFO_8_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_8_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_8_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_8_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_8_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_8_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON8, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 9 Object
 #ifdef CAN_USE_FIFO_9_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_9_OBJECT_TYPE;
  #if CAN_FIFO_9_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_9_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_9_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_9_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_9_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_9_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON9, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 10 Object
 #ifdef CAN_USE_FIFO_10_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_10_OBJECT_TYPE;
  #if CAN_FIFO_10_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_10_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_10_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_10_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_10_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_10_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON10, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 11 Object
 #ifdef CAN_USE_FIFO_11_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_11_OBJECT_TYPE;
  #if CAN_FIFO_11_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_11_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_11_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_11_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_11_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_11_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON11, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 12 Object
 #ifdef CAN_USE_FIFO_12_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_12_OBJECT_TYPE;
  #if CAN_FIFO_12_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_12_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_12_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_12_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_12_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_12_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON12, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 13 Object
 #ifdef CAN_USE_FIFO_13_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_13_OBJECT_TYPE;
  #if CAN_FIFO_13_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_13_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_13_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_13_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_13_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_13_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON13, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 14 Object
 #ifdef CAN_USE_FIFO_14_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_14_OBJECT_TYPE;
  #if CAN_FIFO_14_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_14_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_14_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_14_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_14_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_14_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON14, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 15 Object
 #ifdef CAN_USE_FIFO_15_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_15_OBJECT_TYPE;
  #if CAN_FIFO_15_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_15_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_15_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_15_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_15_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_15_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON15, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 16 Object
 #ifdef CAN_USE_FIFO_16_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_16_OBJECT_TYPE;
  #if CAN_FIFO_16_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_16_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_16_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_16_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_16_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_16_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON16, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 17 Object
 #ifdef CAN_USE_FIFO_17_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_17_OBJECT_TYPE;
  #if CAN_FIFO_17_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_17_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_17_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_17_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_17_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_17_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON17, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 18 Object
 #ifdef CAN_USE_FIFO_18_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_18_OBJECT_TYPE;
  #if CAN_FIFO_18_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_18_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_18_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_18_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_18_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_18_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON18, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 19 Object
 #ifdef CAN_USE_FIFO_19_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_19_OBJECT_TYPE;
  #if CAN_FIFO_19_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_19_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_19_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_19_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_19_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_19_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON19, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 20 Object
 #ifdef CAN_USE_FIFO_20_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_20_OBJECT_TYPE;
  #if CAN_FIFO_20_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_20_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_20_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_20_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_20_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_20_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON20, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 21 Object
 #ifdef CAN_USE_FIFO_21_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_21_OBJECT_TYPE;
  #if CAN_FIFO_21_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_21_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_21_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_21_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_21_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_21_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON21, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 22 Object
 #ifdef CAN_USE_FIFO_22_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_22_OBJECT_TYPE;
  #if CAN_FIFO_22_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_22_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_22_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_22_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_22_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_22_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON22, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 23 Object
 #ifdef CAN_USE_FIFO_23_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_23_OBJECT_TYPE;
  #if CAN_FIFO_23_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_23_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_23_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_23_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_23_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_23_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON23, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 24 Object
 #ifdef CAN_USE_FIFO_24_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_24_OBJECT_TYPE;
  #if CAN_FIFO_24_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_24_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_24_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_24_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_24_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_24_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON24, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 25 Object
 #ifdef CAN_USE_FIFO_25_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_25_OBJECT_TYPE;
  #if CAN_FIFO_25_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_25_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_25_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_25_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_25_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_25_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON25, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 26 Object
 #ifdef CAN_USE_FIFO_26_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_26_OBJECT_TYPE;
  #if CAN_FIFO_26_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_26_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_26_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_26_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_26_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_26_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON26, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 27 Object
 #ifdef CAN_USE_FIFO_27_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_27_OBJECT_TYPE;
  #if CAN_FIFO_27_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_27_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_27_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_27_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_27_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_27_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON27, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 28 Object
 #ifdef CAN_USE_FIFO_28_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_28_OBJECT_TYPE;
  #if CAN_FIFO_28_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_28_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_28_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_28_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_28_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_28_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON28, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 29 Object
 #ifdef CAN_USE_FIFO_29_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_29_OBJECT_TYPE;
  #if CAN_FIFO_29_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_29_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_29_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_29_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_29_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_29_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON29, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 30 Object
 #ifdef CAN_USE_FIFO_30_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_30_OBJECT_TYPE;
  #if CAN_FIFO_30_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_30_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_30_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_30_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_30_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_30_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON30, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup FIFO 31 Object
 #ifdef CAN_USE_FIFO_31_OBJECT
   Config1 = 0;
   
   ((CiFIFOCON)Config1).TXEN = CAN_FIFO_31_OBJECT_TYPE;
  #if CAN_FIFO_31_OBJECT_TYPE == 1
   ((CiFIFOCON)Config1).TXPRI = CAN_FIFO_31_OBJECT_TX_PRIORITY;
   ((CiFIFOCON)Config1).TXAT = CAN_FIFO_31_OBJECT_TX_RETRANSMISSION_ATTEMPTS;
  #else
   ((CiFIFOCON)Config1).RXTSEN = CAN_FIFO_31_OBJECT_RX_TIME_STAMP;
  #endif
   ((CiFIFOCON)Config1).FSIZE = (CAN_FIFO_31_OBJECT_COUNT - 1);
   ((CiFIFOCON)Config1).PLSIZE = CAN_FIFO_31_OBJECT_MAX_PAYLOAD_SIZE;
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FIFOCON31, (uint8_t *)&Config1, 4));
 #endif
   
   //Setup CAN Filters
   can_set_filter_id(CAN_FILTER_0, CAN_USER_FILT_0, CAN_USER_MASK_0, CAN_USER_FILT_0_TYPE);
   can_enable_filter(CAN_FILTER_0, CAN_USER_FILT_0_OBJECT);
   
 #if CAN_USE_FILTERS == TRUE
  #ifdef CAN_USER_FILT_1
   can_set_filter_id(CAN_FILTER_1, CAN_USER_FILT_1, CAN_USER_MASK_1, CAN_USER_FILT_1_TYPE);
   can_enable_filter(CAN_FILTER_1, CAN_USER_FILT_1_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_2
   can_set_filter_id(CAN_FILTER_2, CAN_USER_FILT_2, CAN_USER_MASK_2, CAN_USER_FILT_2_TYPE);
   can_enable_filter(CAN_FILTER_2, CAN_USER_FILT_2_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_3
   can_set_filter_id(CAN_FILTER_3, CAN_USER_FILT_3, CAN_USER_MASK_3, CAN_USER_FILT_3_TYPE);
   can_enable_filter(CAN_FILTER_3, CAN_USER_FILT_3_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_4
   can_set_filter_id(CAN_FILTER_4, CAN_USER_FILT_4, CAN_USER_MASK_4, CAN_USER_FILT_4_TYPE);
   can_enable_filter(CAN_FILTER_4, CAN_USER_FILT_4_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_5
   can_set_filter_id(CAN_FILTER_5, CAN_USER_FILT_5, CAN_USER_MASK_5, CAN_USER_FILT_5_TYPE);
   can_enable_filter(CAN_FILTER_5, CAN_USER_FILT_5_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_6
   can_set_filter_id(CAN_FILTER_6, CAN_USER_FILT_6, CAN_USER_MASK_6, CAN_USER_FILT_6_TYPE);
   can_enable_filter(CAN_FILTER_6, CAN_USER_FILT_6_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_7
   can_set_filter_id(CAN_FILTER_7, CAN_USER_FILT_7, CAN_USER_MASK_7, CAN_USER_FILT_7_TYPE);
   can_enable_filter(CAN_FILTER_7, CAN_USER_FILT_7_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_8
   can_set_filter_id(CAN_FILTER_8, CAN_USER_FILT_8, CAN_USER_MASK_8, CAN_USER_FILT_8_TYPE);
   can_enable_filter(CAN_FILTER_8, CAN_USER_FILT_8_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_9
   can_set_filter_id(CAN_FILTER_9, CAN_USER_FILT_9, CAN_USER_MASK_9, CAN_USER_FILT_9_TYPE);
   can_enable_filter(CAN_FILTER_9, CAN_USER_FILT_9_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_10
   can_set_filter_id(CAN_FILTER_10, CAN_USER_FILT_10, CAN_USER_MASK_10, CAN_USER_FILT_10_TYPE);
   can_enable_filter(CAN_FILTER_10, CAN_USER_FILT_10_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_11
   can_set_filter_id(CAN_FILTER_11, CAN_USER_FILT_11, CAN_USER_MASK_11, CAN_USER_FILT_11_TYPE);
   can_enable_filter(CAN_FILTER_11, CAN_USER_FILT_11_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_12
   can_set_filter_id(CAN_FILTER_12, CAN_USER_FILT_12, CAN_USER_MASK_12, CAN_USER_FILT_12_TYPE);
   can_enable_filter(CAN_FILTER_12, CAN_USER_FILT_12_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_13
   can_set_filter_id(CAN_FILTER_13, CAN_USER_FILT_13, CAN_USER_MASK_13, CAN_USER_FILT_13_TYPE);
   can_enable_filter(CAN_FILTER_13, CAN_USER_FILT_13_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_14
   can_set_filter_id(CAN_FILTER_14, CAN_USER_FILT_14, CAN_USER_MASK_14, CAN_USER_FILT_14_TYPE);
   can_enable_filter(CAN_FILTER_14, CAN_USER_FILT_14_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_15
   can_set_filter_id(CAN_FILTER_15, CAN_USER_FILT_15, CAN_USER_MASK_15, CAN_USER_FILT_15_TYPE);
   can_enable_filter(CAN_FILTER_15, CAN_USER_FILT_15_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_16
   can_set_filter_id(CAN_FILTER_16, CAN_USER_FILT_16, CAN_USER_MASK_16, CAN_USER_FILT_16_TYPE);
   can_enable_filter(CAN_FILTER_16, CAN_USER_FILT_16_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_17
   can_set_filter_id(CAN_FILTER_17, CAN_USER_FILT_17, CAN_USER_MASK_17, CAN_USER_FILT_17_TYPE);
   can_enable_filter(CAN_FILTER_17, CAN_USER_FILT_17_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_18
   can_set_filter_id(CAN_FILTER_18, CAN_USER_FILT_18, CAN_USER_MASK_18, CAN_USER_FILT_18_TYPE);
   can_enable_filter(CAN_FILTER_18, CAN_USER_FILT_18_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_19
   can_set_filter_id(CAN_FILTER_19, CAN_USER_FILT_19, CAN_USER_MASK_19, CAN_USER_FILT_19_TYPE);
   can_enable_filter(CAN_FILTER_19, CAN_USER_FILT_19_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_20
   can_set_filter_id(CAN_FILTER_20, CAN_USER_FILT_20, CAN_USER_MASK_20, CAN_USER_FILT_20_TYPE);
   can_enable_filter(CAN_FILTER_20, CAN_USER_FILT_20_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_21
   can_set_filter_id(CAN_FILTER_21, CAN_USER_FILT_21, CAN_USER_MASK_21, CAN_USER_FILT_21_TYPE);
   can_enable_filter(CAN_FILTER_21, CAN_USER_FILT_21_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_22
   can_set_filter_id(CAN_FILTER_22, CAN_USER_FILT_22, CAN_USER_MASK_22, CAN_USER_FILT_22_TYPE);
   can_enable_filter(CAN_FILTER_22, CAN_USER_FILT_22_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_23
   can_set_filter_id(CAN_FILTER_23, CAN_USER_FILT_23, CAN_USER_MASK_23, CAN_USER_FILT_23_TYPE);
   can_enable_filter(CAN_FILTER_23, CAN_USER_FILT_23_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_24
   can_set_filter_id(CAN_FILTER_24, CAN_USER_FILT_24, CAN_USER_MASK_24, CAN_USER_FILT_24_TYPE);
   can_enable_filter(CAN_FILTER_24, CAN_USER_FILT_24_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_25
   can_set_filter_id(CAN_FILTER_25, CAN_USER_FILT_25, CAN_USER_MASK_25, CAN_USER_FILT_25_TYPE);
   can_enable_filter(CAN_FILTER_25, CAN_USER_FILT_25_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_26
   can_set_filter_id(CAN_FILTER_26, CAN_USER_FILT_26, CAN_USER_MASK_26, CAN_USER_FILT_26_TYPE);
   can_enable_filter(CAN_FILTER_26, CAN_USER_FILT_26_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_27
   can_set_filter_id(CAN_FILTER_27, CAN_USER_FILT_27, CAN_USER_MASK_27, CAN_USER_FILT_27_TYPE);
   can_enable_filter(CAN_FILTER_27, CAN_USER_FILT_27_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_28
   can_set_filter_id(CAN_FILTER_28, CAN_USER_FILT_28, CAN_USER_MASK_28, CAN_USER_FILT_28_TYPE);
   can_enable_filter(CAN_FILTER_28, CAN_USER_FILT_28_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_29
   can_set_filter_id(CAN_FILTER_29, CAN_USER_FILT_29, CAN_USER_MASK_29, CAN_USER_FILT_29_TYPE);
   can_enable_filter(CAN_FILTER_29, CAN_USER_FILT_29_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_30
   can_set_filter_id(CAN_FILTER_30, CAN_USER_FILT_30, CAN_USER_MASK_30, CAN_USER_FILT_30_TYPE);
   can_enable_filter(CAN_FILTER_30, CAN_USER_FILT_30_OBJECT);
  #endif
  
  #ifdef CAN_USER_FILT_31
   can_set_filter_id(CAN_FILTER_31, CAN_USER_FILT_31, CAN_USER_MASK_31, CAN_USER_FILT_31_TYPE);
   can_enable_filter(CAN_FILTER_31, CAN_USER_FILT_31_OBJECT);
  #endif
 #endif
   
   can_set_mode(Mode);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_baud()
//
// Function for setting the Nominal and Data bit rates of the CAN Bus.
//
// Parameters:
//    Clock - the clock speed that is presented to the CANFD peripheral, the
//            speed depends on how the CANFD peripheral and PIC clocks are
//            setup.  
//
//    Nominal - the Nominal (Arbitration) bit rate, max speed is 1 Mbps and 
//              speed must be less then or equal then Clock and be a multiple
//              of Clock.
//
//    Data - the Data bit rate, max speed is 8 Mbps and speed must be less then
//           or equal then Clock and be a multiple of Clock.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_set_baud(uint32_t Clock, uint32_t Nominal, uint32_t Data)
{
   CiNBTCFG nBaud;
   CiDBTCFG dBaud;
   uint16_t BRP = 0;
   uint16_t tBits;
   uint16_t tSeg1Bits;
   
   if((Clock % Nominal) != 0)
      return(CAN_EC_BAUD_NOT_DIVISIBLE);
   
   if((Data != 0) && ((Clock % Data) != 0))
      return(CAN_EC_BAUD_NOT_DIVISIBLE);
   else if(Data == 0)
      Data = Nominal;
   
   do
   {
      BRP++;
      tBits = (Clock / BRP) / Nominal;
      
      if(tBits < 386)
      {
         if((Clock / ((uint32_t)tBits * BRP)) != Nominal)
            continue;
      }
      
      tSeg1Bits = (((uint32_t)tBits * CAN_DEFAULT_SAMPLE_POINT) / 1000) - 1;
   } while(((tBits > 385) && (BRP < 256)) || ((BRP < 256) && (tSeg1Bits > 256)));
   
   if(tBits > 385)
      return(CAN_EC_BAUD_NOMINAL_INVALID);
   
   nBaud.BRP = BRP - 1;
   
   if(tSeg1Bits > 256)
      nBaud.TSEG1 = 255;
   else
      nBaud.TSEG1 = tSeg1Bits - 1;
   
   nBaud.TSEG2 = tBits - ((uint16_t)nBaud.TSEG1 + 3);
   nBaud.SJW = nBaud.TSEG2;
   
   BRP = 0;
   
   do
   {
      BRP++;
      tBits = (Clock / BRP) / Data;
      tSeg1Bits = ((tBits * CAN_DEFAULT_SAMPLE_POINT) / 1000) - 1;
   } while(((tBits > 49) && (BRP < 256)) || ((BRP < 256) && (tSeg1Bits > 32)));
   
   if(tBits > 49)
      return(CAN_EC_BAUD_DATA_INVALID);
   
   dBaud.BRP = BRP - 1;
   
   if(tSeg1Bits > 32)
      dBaud.TSEG1 = 31;
   else
      dBaud.TSEG1 = tSeg1Bits - 1;
   
   dBaud.TSEG2 = tBits - (dBaud.TSEG1 + 3);
   dBaud.SJW = dBaud.TSEG2;
   
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1NBTCFG, (uint8_t *)&nBaud, 4));
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1DBTCFG, (uint8_t *)&dBaud, 4));   
   
   can_set_mode(g_CANOpmode, FALSE);
   
   return(CAN_EC_OK);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_mode()
//
// Sets the operational mode of the MCP2517 CAN FD chip.
//
// Parameters:
//    Mode - the CAN_OP_MODE to change to, can one of the following:
//             CAN_OP_NORMAL_FD
//             CAN_OP_SLEEP
//             CAN_OP_LOOPBACK_INTERNAL
//             CAN_OP_LISTEN
//             CAN_OP_CONFIG
//             CAN_OP_LOOPBACK_EXTERNAL
//             CAN_OP_NORMAL_CAN_2_0
//             CAN_OP_RESTRICTED_OPERATION
//
//    Update - optional specifies whether to update the global operation mode
//             variable, TRUE update or FALSE don't update.  Default is TRUE if
//             not specified.  This is useful for allowing the options to 
//             to put in a different operating mode for a little while and then
//             returning it back to the previous operating mode.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE)
{
   CiCON C1CON;
   
   while(!mcp2517_read_sfr_bytes(MCP2517_ADDR_C1CON, (uint8_t *)&C1CON, 4));
   
   if(C1CON.OPMOD != Mode)
   {
      C1CON.REQOP = Mode;
      
      while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1CON, (uint8_t *)&C1CON, 4));
      
      do
      {
         while(!mcp2517_read_sfr_bytes(MCP2517_ADDR_C1CON, (uint8_t *)&C1CON, 4));
      } while(C1CON.OPMOD != Mode);
   }
   
   if(Update)
      g_CANOpmode = Mode;
}

///////////////////////////////////////////////////////////////////////////////
// can_set_filter_id()
//
// Function for setting up the specified filters's Id, Mask and Type.
//
// Parameters:
//    Filter - CAN_FILTER type specifying the filter to setup.
//
//    Id - the ID to set the filter to.
//
//    Mask - the Mask to set the filter to, when a bit in the mask is set the 
//           corresponding bit of the received ID must match the Filter's ID
//           bit.
//
//    Type - the type of filter to setup, can be one of the following:
//             CAN_FILTER_TYPE_SID_ONLY - only receive SID messages.
//             CAN_FILTER_TYPE_EID_ONLY - only receive EID messages.
//             CAN_FILTER_TYPE_EITHER - can receive both SID or EID messages.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, uint32_t Mask, CAN_FILTER_TYPE Type)
{
   MCP2517_ADDRESS Address;
   FLTCON sFilter, wFilter;
   CiFLTOBJ wId;
   CiMASK wMask;
   
   Address = MCP2517_ADDR_C1FLTCON0 + Filter;
   
   while(!mcp2517_read_sfr(Address, (uint8_t *)&sFilter));
   
   //Can only change Filter's ID and Mask when Filter is disabled
   if(sFilter.FLTEN == TRUE)
   {
      (uint8_t)wFilter = (uint8_t)sFilter;
   
      wFilter.FLTEN = FALSE;
   
      while(!mcp2517_write_sfr(Address, (uint8_t)wFilter));
   }
   
   (uint32_t)wId = 0;
   (uint32_t)wMask = 0;
   
   switch(Type)
   {
      case CAN_FILTER_TYPE_SID_ONLY:
         #if (CAN_DEVICE_NET_FILTER_BIT_NUMBER > 0) && ((CAN_OPERATION_MODE == 1) || ((CAN_OPERATION_MODE == 0) && (CAN_SID == 0)))
            wId.EID = Id & 0x1FFFFFFF;
            wMask.MEID = Mask & 0x1FFFFFFF;
         #else
            wId.SID0_7 = make8(Id, 0);
            wId.SID8_10 = make8(Id, 1);
            
            wMask.MSID0_7 = make8(Mask, 0);
            wMask.MSID8_10 = make8(Mask, 1);
            
          #if (CAN_OPERATION_MODE == 0) && (CAN_SID11 == 1) //CAN FD and set to use 12 bit SID
            wId.SID11 = bit_test(make8(Id, 1), 3);
            wMask.MSID11 = bit_test(make8(Mask, 1), 3);
            
           #if CAN_DEVICE_NET_FILTER_BIT_NUMBER > 0
            wId.EID0_4 = ((make8(Id, 2) << 5) | (make8(Id, 1) >> 4));
            wMask.MEID0_4 = ((mak8(Mask, 2) << 5) | (make8(Mask, 1) >> 4));
            
            wId.EID5_12 = ((make8(Id, 2) >> 1) | (make8(Id, 3) << 7));
            wMask.MEID5_12 = ((make8(Mask, 2) >> 1) | (make8(Mask, 3) << 7));
            
            wId.EID13_17 = (make8(Id, 3) >> 1);
            wId.MEID13_17 = (make8(Mask, 3) >> 1);
           #endif
          #endif
         #endif
         
         wId.EXIDE = 0;
         wMask.MIDE = 1;
         break;
      case CAN_FILTER_TYPE_EID_ONLY:
      case CAN_FILTER_TYPE_EITHER:
         wId.EID0_4 = make8(Id, 0);
         wMask.MEID0_4 = make8(Mask, 0);
         
         wId.EID5_12 = ((make8(Id, 0) >> 5) | (make8(Id, 1) << 3));
         wMask.MEID5_12 = ((make8(Mask, 0) >> 5) | (make8(Mask, 1) << 3));
         
         wId.EID13_17 = ((make8(Id, 1) >> 5) | ((make8(Id, 2) & 0x03) << 3));
         wMask.MEID13_17 = ((make8(Mask, 1) >> 5) | ((make8(Mask, 2) & 0x03) << 3));
         
         wId.SID0_7 = ((make8(Id, 2) >> 2) | (make8(Id, 3) << 6));
         wMask.MSID0_7 = ((make8(Mask, 2) >> 2) | (make8(Mask, 3) << 6));
         
         wId.SID8_10 = (make8(Id, 3) >> 2);
         wMask.MSID8_10 = (make8(Mask, 3) >> 2);
         
         if(Type == CAN_FILTER_TYPE_EID_ONLY)
         {
            wId.EXIDE = 1;
            wMask.MIDE = 1;
         }
         else
            wMask.MIDE = 0;
         break;
   }
   
   //write id and mask
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1FLTOBJ0 + ((MCP2517_ADDRESS)Filter * 8), (uint8_t *)&wId, 4));
   while(!mcp2517_write_sfr_bytes(MCP2517_ADDR_C1MASK0 + ((MCP2517_ADDRESS)Filter * 8), (uint8_t *)&wMask, 4));
   
   if(sFilter.FLTEN == TRUE)
      while(!mcp2517_write_sfr(Address, (uint8_t)sFilter));
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_filter()
//
// Function for enabling specified filter and assigning it to an Object.
//
// Parameters:
//    Filter - CAN_FILTER type specifying the filter to enable.
//
//    Object - the RX Object to assign the filter to, optional defaults to 
//             CAN_OBJECT_FIFO_1 if not specified.  By default this driver
//             only sets up the CAN to use the FIFO 1 Object to receive
//             messages.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_filter(CAN_FILTER Filter, CAN_OBJECT Object=CAN_OBJECT_FIFO_1)
{
   MCP2517_ADDRESS Address;
   FLTCON wFilter;
   
   Address = MCP2517_ADDR_C1FLTCON0 + Filter;
   
   while(!mcp2517_read_sfr(Address, (uint8_t *)&wFilter));
   
   if(wFilter.FLTEN == TRUE)
   {
      wFilter.FLTEN = FALSE;
      
      while(!mcp2517_write_sfr(Address, (uint8_t)wFilter));
   }
   
   wFilter.FLTEN = TRUE;
   wFilter.BP = Object;
   
   while(!mcp2517_write_sfr(Address, (uint8_t)wFilter));
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_filter()
//
// Function for disabling specified filter.
//
// Parameters:
//    Filter - CAN_FILTER type specifying the filter to disable.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_filter(CAN_FILTER Filter)
{
   MCP2517_ADDRESS Address;
   FLTCON wFilter;
   
   Address = MCP2517_ADDR_C1FLTCON0 + Filter;
   
   wFilter.FLTEN = FALSE;
   wFilter.BP = CAN_OBJECT_FIFO_1;
   
   while(!mcp2517_write_sfr(Address, (uint8_t)wFilter));
}

///////////////////////////////////////////////////////////////////////////////
// can_putd()
//
// Function used to load messages into a transmit object, after which when the
// CAN Bus becomes available it will be transmitted.
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, i.e. the message ID.  See
//             CAN_TX_HEADER type in can-mcp2517.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Object - the CAN TX Object to load the message into, optional defaults
//             to CAN_OBJECT_TXQ if not specified.  By default this driver only
//             sets up the CAN to use the TX Queue Object to send messages.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object=CAN_OBJECT_TXQ)
{  
  #ifdef CAN_HAS_TX_OBJECT
   uint8_t Status;
   MCP2517_ADDRESS Address;
   struct
   {
      CAN_TX_MESSAGE_OBJECT Header;
      uint8_t Data[CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE];
   } TxData;
   uint8_t aCount;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      //If Object is a FIFO Object, read C1FIFOCONm register to determine if Object is a TX Object
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      //Check if TXEN bit of Object to determine if Object is a TX or RX Object, if RX return FALSE
      if(bit_test(Status, 7) == 0)
         return(CAN_EC_OBJ_NOT_TX);
   }
   
   //Read Status register of Object to determine if Object FIFO can hold more messages
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQSTA;
   else
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)(Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Status) == FALSE)
      return(CAN_EC_SPI_CRC);
   
   //Check TFNRFNIF bit of Object to determine if FIFO is full
   if(bit_test(Status, 0) == 0)
      return(CAN_EC_OBJ_TX_FULL);
   
   //Read the C1FIFOUAm register, to determine the address in memory to write messages to
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQUA;
   else
      Address = MCP2517_ADDR_C1FIFOUA1 + ((uint16_t)(Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Address, 2) == FALSE)
      return(CAN_EC_SPI_CRC);
   
   Address += 0x400;
   
   can_set_tx_obj_header(&TxData.Header, Header);
   aCount = can_set_tx_obj_data(TxData.Data, Data, Header->Length);
   
   //Write message to memory
   if(mcp2517_write_memory_dwords(Address, (DW_T *)&TxData, aCount) == FALSE)
      return(CAN_EC_SPI_CRC);
   
   //Set UINC and TXREG bits to increment FIFO pointer and Request message transmission
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQCON + 1;
   else
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C) + 1;
   
   if(mcp2517_write_sfr(Address, 0x03) == FALSE)
      return(CAN_EC_SPI_CRC);
   
   return(CAN_EC_OK);
  #else
   return(CAN_EC_OBJ_TX_NONE);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// can_getd()
//
// Function used to get messages from specified RX Object.
//
// Parameters:
//    Header - pointer to CAN_RX_HEADER variable to store info about the
//             retrieved messages, i.e. the message ID.  See CAN_RX_HEADER
//             type in can-mcp2517.h for more info.
//
//    Data - pointer to uint8_t array to return data portion of message to.
//
//    Object - the CAN RX Object to get the message from, optional defaults
//             to CAN_OBJECT_FIFO_1 if not specified.  By default this driver
//             only sets up the CAN to use the FIFO 1 Object to receive
//             messages.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object=CAN_OBJECT_FIFO_1)
{
  #ifdef CAN_HAS_RX_OBJECT
   MCP2517_ADDRESS Address;
   uint8_t Con;
   uint8_t Status;
   CAN_RX_MESSAGE_OBJECT RxObject;
   uint8_t Length;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Object -= 1;
      
      //Read C1FIFOCONm register to determine if Object is a RX Object
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)Object * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Con) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      //Check if TXEN bit of Object to determine if Object is a TX or RX Object, if TX return FALSE
      if(bit_test(Con, 7) == 1)
         return(CAN_EC_OBJ_NOT_RX);
   
      //Read the C1FIFOSTAm register, to determine if Object has any messages to read
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)Object * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      //If FIFO is empty return FALSE
      if(bit_test(Status, 0) == 0)
         return(CAN_EC_OBJ_RX_EMPTY);
      
      //Read the C1FIFOUAm register, to determine the address in memory to read message from
      Address = MCP2517_ADDR_C1FIFOUA1 + ((uint16_t)Object * 0x0C);
      
      if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Address, 2) == FALSE)
         return(CAN_EC_SPI_CRC);
         
      Address += 0x400;
      
      //Read message header from memory
      if(mcp2517_read_memory_dwords(Address, (DW_T *)&RxObject, 2) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      can_get_rx_header(Header, &RxObject);
      
      Address += 8;
      
      if(bit_test(Con, 5) == TRUE)  //Object uses Time Stamp
      {
         Header->HasTS = TRUE;
         
         //Read message Time Stamp from memory
         if(mcp2517_read_memory(Address, (DW_T *)&Header->TimeStamp) == FALSE)
            return(CAN_EC_SPI_CRC);
         
         Address += 4;
      }
      else
         Header->HasTS = FALSE;
      
      Length = Header->Length;
      
      if(Length > 0)
      {
         while((Length & 0x03) != 0x00)   //This should only happen for data lengths less then 8
            Length++;
         
         Length /= 4;   //convert length in bytes to length in double words
         
         if(mcp2517_read_memory_dwords(Address, (DW_T *)Data, Length) == FALSE)
            return(CAN_EC_SPI_CRC);
      }
      
      //Get RX FIFO overflow interrupt flag and clear if set
      Header->err_ovfl = bit_test(Status, 4);
      
      if(bit_test(Status, 4))
      {
         bit_clear(Status, 4);
         
         Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)Object * 0x0C);
         
         if(mcp2517_write_sfr(Address, Status) == FALSE)
            return(CAN_EC_SPI_CRC);
      }
      
      //Set the UINC bit in the C1FIFOCONx register to increment the Object FIFO pointer
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)Object * 0x0C) + 1;
      
      if(mcp2517_write_sfr(Address, 0x01) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      return(CAN_EC_OK);
   }
   else
      return(CAN_EC_OBJ_NOT_RX);
  #else
   return(CAN_EC_OBJ_RX_NONE);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// can_kbhit()
//
// Used to determine if there is at least one message to retrieve from the 
// specified object.
//
// Parameters:
//    Object - the CAN RX Object to check for messages, optional defaults to
//             CAN_OBJECT_FIFO_1 if not specified.  By default this driver only
//             sets up the CAN to use the FIFO 1 Object to receive messages.
//
// Returns:
//    TRUE - if there is at least one message to get.
//    FALSE - if there are no messages to get.
// ////////////////////////////////////////////////////////////////////////////
int1 can_kbhit(CAN_OBJECT Object=CAN_OBJECT_FIFO_1)
{
   int1 Result = FALSE;

  #ifdef CAN_HAS_RX_OBJECT
   MCP2517_ADDRESS Address;
   uint8_t Status;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Object--;
      
      //Read the CiFIFOCONm register to check if Object FIFO is a RX Object
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)Object * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == TRUE)
      {
         //Check the TXEN bit to determine if Object is a TX (1) or RX (0) Object
         if(bit_test(Status, 7) == 0)
         {
            //Read the CiFIFOSTAm register to check if Object FIFO has any messages
            Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)Object * 0x0C);
            
            if(mcp2517_read_sfr(Address, &Status) == TRUE)
            {
               //Check the TFNRFNIF bit to determine if Object has any messages
               Result = bit_test(Status, 0);
            }
         }
      }
   }
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_discard()
//
// Used to discard all messages in specified RX Object.
//
// Parameters:
//    Object - the CAN RX Object to discard messages for, optional defaults to
//             CAN_OBJECT_FIFO_1 if not specified.  By default this driver only
//             sets up the CAN to use the FIFO 1 Object to receive messages.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_discard(CAN_OBJECT Object=CAN_OBJECT_FIFO_1)
{
  #ifdef CAN_HAS_RX_OBJECT
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   uint8_t Status;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Object--;
      
      //Read the CiFIFOCONm register to check if Object FIFO is a RX Object
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)Object * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == TRUE)
      {
         //Check the TXEN bit to determine if Object is a TX (1) or RX (0) Object
         if(bit_test(Status, 7) == 0)
         {
            Status = 0x04;
            Address++;
            
            if(mcp2517_write_sfr(Address, Status) == TRUE)
            {
               do
               {
                  if(mcp2517_read_sfr(Address, &Status) == FALSE)
                     Result = CAN_EC_SPI_CRC;
               }while((Result == CAN_EC_OK) && (bit_test(Status, 2) == 1));
            }
            else
               Result = CAN_EC_SPI_CRC;
         }
         else
            Result = CAN_EC_OBJ_NOT_RX;
      }
      else
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_OBJ_NOT_RX;
   
   return(Result);
  #else
   return(CAN_EC_OBJ_RX_NONE);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// can_tbe()
//
// Used to determine if specified TX Object's FIFO can accept at least one more
// message.
//
// Parameters:
//    Object - the CAN TX Object to check, optional defaults to 
//             CAN_OBJECT_TXQ if not specified.  By default this driver only
//             sets up the CAN to use the TX Queue Object to transmit messages.
//
// Returns:
//    TRUE - if Object's FIFO can accept at least one more message.
//    FALSE - if Object's FIFO is full.
///////////////////////////////////////////////////////////////////////////////
int1 can_tbe(CAN_OBJECT Object=CAN_OBJECT_TXQ)
{
   int1 Result = FALSE;
   
  #ifdef CAN_HAS_TX_OBJECT
   MCP2517_ADDRESS Address;
   uint8_t Status;
   
   //If Object is a FIFO object check to make sure it's TX Object
   if(Object > CAN_OBJECT_TXQ)
   {
      //Read the C1FIFOCONm register
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == FALSE)
         return(Result);
      
      //Check the TXEN bit to determine if Object is a TX (1) or RX (0) Object
      if(bit_test(Status, 7) == 0)
         return(Result);
   }
   
   //Read the Object's STA register to check if Object's 
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQSTA;
   else
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)(Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Status) == TRUE)
   {
      //Check the Transmit Not Full Interrupt Flag
      Result = bit_test(Status, 0);
   }
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_tx_is_empty()
//
// Used to determine if specified TX Object's FIFO is empty.
//
// Parameters:
//    Object - the CAN TX Object to check, optional defaults to 
//             CAN_OBJECT_TXQ if not specified.  By default this driver only
//             sets up the CAN to use the TX Queue Object to transmit messages.
//
// Returns:
//    TRUE - if Object's FIFO is empty.
//    FALSE - if Object's FIFO isn't empty, has at least one more message to 
//            send.
///////////////////////////////////////////////////////////////////////////////
int1 can_tx_is_empty(CAN_OBJECT Object=CAN_OBJECT_TXQ)
{
   int1 Result = FALSE;
   
  #ifdef CAN_HAS_TX_OBJECT
   MCP2517_ADDRESS Address;
   uint8_t Status;
   
   //If Object is a FIFO object check to make sure it's TX Object
   if(Object > CAN_OBJECT_TXQ)
   {
      //Read the C1FIFOCONm register
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == FALSE)
         return(Result);
      
      //Check the TXEN bit to determine if Object is a TX (1) or RX (0) Object
      if(bit_test(Status, 7) == 0)
         return(Result);
   }
   
   //Read the Object's STA register to check if Object's 
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQSTA;
   else
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)(Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Status) == TRUE)
   {
      //Check the Transmit Empty Interrupt Flag
      Result = bit_test(Status, 2);
   }
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_abort()
//
// Use to abort pending transmission of for specified Object or all Objects.
//
// Parameters:
//    Object - the TX Object to abort transmissions for, optional defaults to
//             aborting all transmissions for TX Objects.
//
// Returns:
//    TRUE - if successful
//    FALSE - if unsuccessful
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_abort(CAN_OBJECT Object=CAN_ABORT_ALL)
{
  #ifdef CAN_HAS_TX_OBJECT
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   uint8_t Status;

   if(Object == CAN_ABORT_ALL)
      Address = MCP2517_ADDR_C1CON + 3;
   else if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQCON;
   else 
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Status) == TRUE)
   {
      if(bit_test(Status, 7) == 1)
      {
         if(Object == CAN_ABORT_ALL)
            bit_set(Status, 3);
         else
            bit_clear(Status, 1);
         
         if(mcp2517_write_sfr(Address, Status) == FALSE)
            Result = CAN_EC_SPI_CRC;
      }
      else
         Result = CAN_EC_OBJ_NOT_TX;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
  #else
   return(CAN_EC_OBJ_TX_NONE);
  #endif
}

// CAN FD Interrupts Functions

///////////////////////////////////////////////////////////////////////////////
// can_enable_interrupts()
//
// Used to enable one or more of the CAN Interrupts.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions enables interrupts at the Main Interrupts level.  Depending
// on the interrupt being enabled it may require enabling a FIFO Individual
// level interrupt and a Main level interrupt for the interrupt to occur.  For
// example to enable the Receive FIFO interrupt to occur when a CAN messages
// is received it requires enabling the Receive FIFO not empty FIFO individual
// level interrupt and the Receive Object main level interrupt:
//    can_enable_interrupts(CAN_INTERRUPT_RX);
//    can_enable_fifo_interrupts(CAN_OBJECT_FIFO_1, CAN_FIFO_INTERRUPT_RXNE);
//
// Parameters:
//    Setting - CAN_INTERRUPT type specifying the interrupt to enable, multiple
//              interrupts can be enabled by or'ing the interrupts together.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_enable_interrupts(CAN_INTERRUPT Setting)
{
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address = (MCP2517_ADDR_C1INT + 2);
   uint16_t Value;
   
   if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Value, 2) == TRUE)
   {
      Value |= Setting;
      
      if(mcp2517_write_sfr_bytes(Address, (uint8_t *)&Value, 2) == FALSE)
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_interrupts()
//
// Used to disable one or more of the CAN Interrupts.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions enables interrupts at the Main Interrupts level.
//
// Parameters:
//    Setting - CAN_INTERRUPT type specifying the interrupt to disable, multiple
//              interrupts can be disabled by or'ing the interrupts together.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_disable_interrupts(CAN_INTERRUPT Setting)
{
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address = (MCP2517_ADDR_C1INT + 2);
   uint16_t Value;
   
   if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Value, 2) == TRUE)
   {
      Value &= ~Setting;
      
      if(mcp2517_write_sfr_bytes(Address, (uint8_t *)&Value, 2) == FALSE)
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_interrupts_active()
//
// Used to determine which CAN Interrupts are active.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions determines which Main level interrupts are active.
//
// Parameters - None.
//
// Returns - CAN_INTERRUPT type.  If an interrupt is active the corresponding
//           bit will be set, see CAN_INTERRUPT type in can-mcp2517.h to
//           determine which bits correspond to which interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_INTERRUPT can_interrupts_active(void)
{
   MCP2517_ADDRESS Address = MCP2517_ADDR_C1INT;
   CAN_INTERRUPT Value;
   
   if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Value, 2) == FALSE)
      Value = 0;
   
   return(Value & 0xFF1F);
}

///////////////////////////////////////////////////////////////////////////////
// can_interrupt_active()
//
// Used to determine if specified CAN Interrupt is active.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions determines if the specified Main level interrupt is active.
//
// Parameters:
//    Interrupt - the CAN_INTERRUPT type to test if active.
//
// Returns:
//    TRUE - if active.
//    FALSE - if not active.
///////////////////////////////////////////////////////////////////////////////
int1 can_interrupt_active(CAN_INTERRUPT Interrupt)
{
   CAN_INTERRUPT Value;
   
   Value = can_interrupts_active();
   
   if((Value & Interrupt) != 0)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_clear_interrupt()
//
// Used to clear specified CAN Interrupt flag.  The MCP2517FD has multiple
// levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions clear the specified Main level interrupt flag.
//
// Parameters:
//    Interrupt - the CAN_INTERRUPT type to clear the flag for.  Multiple 
//                interrupts can be cleared by or'ing the interrupts together.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_clear_interrupt(CAN_INTERRUPT Interrupt)
{
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address = MCP2517_ADDR_C1INT;
   CAN_INTERRUPT Value;
   
   if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Value, 2) == TRUE)
   {
      Value &= ~Interrupt;
      
      if(mcp2517_write_sfr_bytes(Address, (uint8_t *)&Value, 2) == FALSE)
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_fifo_interrupts()
//
// Used to enable one or more of the CAN FIFO Interrupts.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions enables interrupts at the FIFO Individual Interrupts level.
// Depending on the interrupt being enabled it may require enabling a FIFO
// Individual level interrupt and a Main level interrupt for the interrupt to
// occur.  For example to enable the Receive FIFO interrupt to occur when a CAN
// messages is received it requires enabling the Receive FIFO not empty FIFO
// individual level interrupt and the Receive Object main level interrupt:
//    can_enable_interrupts(CAN_INTERRUPT_RX);
//    can_enable_fifo_interrupts(CAN_OBJECT_FIFO_1, CAN_FIFO_INTERRUPT_RXNE);
//
// Parameters:
//    Object - the CAN object to enable the CAN FIFO interrupt for.
//
//    Setting - CAN_FIFO_INTERRUPT type specifying the interrupt to enable,
//              multiple interrupts can be enabled by or'ing the interrupts
//              together.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_enable_fifo_interrupts(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Setting)
{
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   uint8_t Value;
   
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQCON;
   else
      Address = MCP2517_ADDR_C1FIFOCON1 + ((Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Value) == TRUE)
   {
      Value |= Setting;
      
      if(mcp2517_write_sfr(Address, Value) == FALSE)
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_fifo_interrupts()
//
// Used to disable one or more of the CAN FIFO Interrupts.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions disables interrupts at the FIFO Individual Interrupts level.
//
// Parameters:
//    Object - the CAN object to disable the CAN FIFO interrupt for.
//
//    Setting - CAN_FIFO_INTERRUPT type specifying the interrupt to disable,
//              multiple interrupts can be disabled by or'ing the interrupts
//              together.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_disable_fifo_interrupts(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Setting)
{
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   uint8_t Value;
   
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQCON;
   else
      Address = MCP2517_ADDR_C1FIFOCON1 + ((Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Value) == TRUE)
   {
      Value &= ~Setting;
      
      if(mcp2517_write_sfr(Address, Value) == FALSE)
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_fifo_interrupts_active()
//
// Used to determine which CAN FIFO Interrupts are active.  The MCP2517FD has
// multiple levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions determines which interrupts are active at the FIFO Individual
// Interrupts level.
//
// Parameters:
//    Object - the CAN object to determine which CAN FIFO interrupts are 
//             active for.
//
// Returns - CAN_FIFO_INTERRUPT type.  If an interrupt is active the
//           corresponding bit will be set, see CAN_FIFO_INTERRUPT type in
//           can-mcp2517.h to determine which bits correspond to which 
//           interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_FIFO_INTERRUPT can_fifo_interrupts_active(CAN_OBJECT Object)
{
   MCP2517_ADDRESS Address;
   CAN_FIFO_INTERRUPT Value;
   
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQSTA;
   else
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Value) == FALSE)
      Value = 0;
   
   if(Object == CAN_OBJECT_TXQ)
      return(Value & 0x15);
   else
      return(Value & 0x1F);
}

///////////////////////////////////////////////////////////////////////////////
// can_fifo_interrupt_active()
//
// Used to determine if the specified CAN FIFO Interrupts is active.  The 
// MCP2517FD has multiple levels of interrupts that fall into the following
// categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions determines if the specified interrupt is active an the FIFO
// Individual Interrupts level.
//
// Parameters:
//    Object - the CAN object to determine if specified CAN FIFO interrupt is 
//             active for.
//
//    Interrupt - CAN_FIFO_INTERRUPT type specifying the interrupt to test for.
//
// Returns:
//    TRUE - if active.
//    FALSE - if not active.
///////////////////////////////////////////////////////////////////////////////
int1 can_fifo_interrupt_active(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Interrupt)
{
   CAN_FIFO_INTERRUPT Value;
   
   Value = can_fifo_interrupts_active(Object);
   
   if((Value & Interrupt) != 0)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_clear_fifo_interrupt()
//
// Used to clear specified CAN FIFO Interrupt flag.  The MCP2517FD has multiple
// levels of interrupts that fall into the following categories:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// This functions clear the specified interrupt at the FIFO Individual
// Interrupts level.
//
// Parameters:
//    Object - the CAN object to clear the specified CAN FIFO interrupt flag
//             for.
//
//    Interrupt - CAN_FIFO_INTERRUPT type specifying the interrupt flag to
//                clear.  Multiple interrupts can be cleared by or'ing the
//                interrupts together.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_clear_fifo_interrupt(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Interrupt)
{
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   CAN_FIFO_INTERRUPT Value;
   
   if(Object == CAN_OBJECT_TXQ)
      Address = MCP2517_ADDR_C1TXQSTA;
   else
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((Object - 1) * 0x0C);
   
   if(mcp2517_read_sfr(Address, &Value) == TRUE)
   {
      Value &= ~Interrupt;
      
      if(mcp2517_write_sfr(Address, Value) == FALSE)
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_SPI_CRC;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_rtr()
//
// Used to enable RTR messages on one of the TX Objects.
//
// Parameters:
//    Object - the CAN TX Object to enable RTR messages for.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_enable_rtr(CAN_OBJECT Object)
{
  #ifdef CAN_HAS_TX_OBJECT
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   uint8_t Value;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Address = MCP2517_ADDR_C1FIFOCON1 + ((Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Value) == TRUE)
      {
         //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a TX Object
         if(bit_test(Value, 7))
         {
            bit_set(Value, 6);
            
            if(mcp2517_write_sfr(Address, Value) == FALSE)
               Result = CAN_EC_SPI_CRC;
         }
         else
            Result = CAN_EC_OBJ_NOT_TX;
      }
      else
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_OBJ_NO_RTR;
   
   return(Result);
  #else
   return(CAN_EC_OBJ_TX_NONE);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_rtr()
//
// Used to disable RTR messages on one of the TX Objects.
//
// Parameters:
//    Object - the CAN TX Object to disable RTR messages for.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_disable_rtr(CAN_OBJECT Object)
{
  #ifdef CAN_HAS_TX_OBJECT
   can_ec_t Result = CAN_EC_OK;
   MCP2517_ADDRESS Address;
   uint8_t Value;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Address = MCP2517_ADDR_C1FIFOCON1 + ((Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Value) == TRUE)
      {
         //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a TX Object
         if(bit_test(Value, 7))
         {
            bit_clear(Value, 6);
            
            if(mcp2517_write_sfr(Address, Value) == FALSE)
               Result = CAN_EC_SPI_CRC;
         }
         else
            Result = CAN_EC_OBJ_NOT_TX;
      }
      else
         Result = CAN_EC_SPI_CRC;
   }
   else
      Result = CAN_EC_OBJ_NO_RTR;
   
   return(Result);
  #else
   return(CAN_EC_OBJ_TX_NONE);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// can_load_rtr()
//
// Used to load a message into a FIFOx TX object that will transmitted when an
// remote transmission request is received.  
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, i.e. the message ID.  See
//             CAN_TX_HEADER type in can-mcp2517.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Object - the CAN TX Object to load the message into.  Only FIFO TX
//             objects can be use to responded to remote transmission requests.
//
// Returns - can_ec_t error code, see can-mcp2517.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_load_rtr(CAN_TX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object)
{
   #ifdef CAN_HAS_TX_OBJECT
   uint8_t Status;
   MCP2517_ADDRESS Address;
   struct
   {
      CAN_TX_MESSAGE_OBJECT Header;
      uint8_t Data[CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE];
   } TxData;
   uint8_t aCount;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      //If Object is a FIFO Object, read C1FIFOCONm register to determine if Object is a TX Object
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      //Check if TXEN bit of Object to determine if Object is a TX or RX Object, if RX return FALSE
      if(bit_test(Status, 7) == 0)
         return(CAN_EC_OBJ_NOT_TX);
   
      //Read Status register of Object to determine if Object FIFO can hold more messages
      Address = MCP2517_ADDR_C1FIFOSTA1 + ((uint16_t)(Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr(Address, &Status) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      //Check TFNRFNIF bit of Object to determine if FIFO is full
      if(bit_test(Status, 0) == 0)
         return(CAN_EC_OBJ_TX_FULL);
   
      //Read the C1FIFOUAm register, to determine the address in memory to write messages to
      Address = MCP2517_ADDR_C1FIFOUA1 + ((uint16_t)(Object - 1) * 0x0C);
      
      if(mcp2517_read_sfr_bytes(Address, (uint8_t *)&Address, 2) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      Address += 0x400;
   
      can_set_tx_obj_header(&TxData.Header, Header);
      aCount = can_set_tx_obj_data(TxData.Data, Data, Header->Length);
      
      //Write message to memory
      if(mcp2517_write_memory_dwords(Address, (DW_T *)&TxData, aCount) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      //Set UINC bit to increment FIFO pointer
      Address = MCP2517_ADDR_C1FIFOCON1 + ((uint16_t)(Object - 1) * 0x0C) + 1;
      
      if(mcp2517_write_sfr(Address, 0x01) == FALSE)
         return(CAN_EC_SPI_CRC);
      
      return(CAN_EC_OK);
   }
   else
      return(CAN_EC_OBJ_NO_RTR);
  #else
   return(CAN_EC_OBJ_TX_NONE);
  #endif
}

///////////////////////////////////////////////////////////////////////////////

void can_set_tx_obj_header(CAN_TX_MESSAGE_OBJECT *Ptr, CAN_TX_HEADER *Header)
{
   uint32_t Id;
   
   Ptr->EID = 0;
   Id = Header->Id;
   
   if(Header->ext == TRUE)
   {
      Ptr->EID0_4 = make8(Id, 0);
      Ptr->EID5_12 = ((make8(Id, 0) >> 5) | ((make8(Id, 1) & 0x1F) << 3));
      Ptr->EID13_17 = ((make8(Id, 1) >> 5) | ((make8(Id, 2) & 0x03) << 3));
      Ptr->SID0_7 = ((make8(Id, 2) >> 2) | (make8(Id, 3) << 6));
      Ptr->SID8_10 = (make8(Id, 3) >> 2);
      
      Ptr->IDE = TRUE;
   }
   else
   {
      Ptr->SID = Id & 0x07FF;
      Ptr->SID11 = bit_test(Id, 11);
      
      Ptr->IDE = FALSE;
   }
   
   Ptr->DLC = can_get_dlength_from_bytes(Header->Length);
   Ptr->RTR = Header->rtr;
   Ptr->BRS = TRUE;
   Ptr->FDF = Header->Format;
   Ptr->SEQ = g_CANTXSequence++;
   
   g_CANTXSequence &= 0x7F;
}

uint8_t can_set_tx_obj_data(uint8_t *wData, uint8_t *rData, uint8_t Length)
{
   uint8_t dwCount;
   
   dwCount = 2;
   
   memcpy(wData, rData, Length);
   
   while((Length % 4) != 0)
      wData[Length++] = 0;
   
   dwCount += (Length / 4);
   
   return(dwCount);
}

void can_get_rx_header(CAN_RX_HEADER *Header, CAN_RX_MESSAGE_OBJECT *Ptr)
{
   Header->Id = 0;
   
   if(Ptr->IDE == TRUE)
   {
      Header->Id = ((Ptr->EID >> 11) & 0x3FFFF) | ((uint32_t)(Ptr->SID & 0x07FF) << 18);
      
      Header->ext = TRUE;
   }
   else
   {
      Header->Id = (Ptr->SID & 0x07FF);
      
      if(bit_test(Ptr->SID, 29))
         bit_set(Header->Id, 11);
      
      Header->ext = FALSE;
   }
   
   Header->Length = can_get_bytes_from_dlength(Ptr->DLC);
   Header->Filter = Ptr->FILHIT;
   Header->rtr = Ptr->RTR;
   Header->Format = Ptr->FDF;
}

DATA_LENGTH can_get_dlength_from_bytes(uint8_t dBytes)
{
   DATA_LENGTH Result;
   
   if(dBytes <= 8)
      Result = (DATA_LENGTH)dBytes;
   else if(dBytes <= 12)
      Result = DATA_LENGTH_12;
   else if(dBytes <= 16)
      Result = DATA_LENGTH_16;
   else if(dBytes <= 20)
      Result = DATA_LENGTH_20;
   else if(dBytes <= 24)
      Result = DATA_LENGTH_24;
   else if(dBytes <= 32)
      Result = DATA_LENGTH_32;
   else if(dBytes <= 48)
      Result = DATA_LENGTH_48;
   else
      Result = DATA_LENGTH_64;
   
   return(Result);
}

uint8_t can_get_bytes_from_dlength(DATA_LENGTH dLength)
{
   uint8_t Result;
   
   if(dLength <= DATA_LENGTH_8)
      Result = (DATA_LENGTH)dLength;
   else
   {
      switch(dLength)
      {
         case DATA_LENGTH_12:
            Result = 12;
            break;
         case DATA_LENGTH_16:
            Result = 16;
            break;
         case DATA_LENGTH_20:
            Result = 20;
            break;
         case DATA_LENGTH_24:
            Result = 24;
            break;
         case DATA_LENGTH_32:
            Result = 32;
            break;
         case DATA_LENGTH_48:
            Result = 48;
            break;
         case DATA_LENGTH_64:
            Result = 64;
            break;
      }
   }
   
   return(Result);
}




/////////////////////////////////// SPI Code //////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// mcp2517_read_sfr()
//
// Function to read a single byte from SFR memory, address ranges 0x000 to 0x2EF
// and 0xE00 to 0xE13.
//
// Parameters:
//    Address - address in SFR memory to read.
//
//    rValue - pointer to uint8_t variable to return read byte to.
//
// Returns:
//    TRUE - if memory was read correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined. 
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_read_sfr(MCP2517_ADDRESS Address, uint8_t *rValue)
{
  #ifdef MCP2517_SPI_USE_CRC
   CRC16_T crc16;
   uint8_t Value;
  #endif

   output_low(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   Value = MCP2517_CMD_READ_CRC | (make8(Address, 1) & 0x0F);
   MCP2517Xfer(Value);
   calculate_crc16(&Value, 1, TRUE);
  #else
   MCP2517Xfer(MCP2517_CMD_READ | (make8(Address, 1) & 0x0F));
  #endif
  
   MCP2517Xfer(make8(Address, 0));
  #ifdef MCP2517_SPI_USE_CRC
   calculate_crc16((uint8_t *)&Address, 1);
  #endif
   
  #ifdef MCP2517_SPI_USE_CRC
   Value = 1;
   MCP2517Xfer(Value);
   calculate_crc16(&Value, 1);
  #endif
   
   *rValue = MCP2517Xfer(0);
   
  #ifdef MCP2517_SPI_USE_CRC
   crc16.b[1] = MCP2517Xfer(0);
   crc16.b[0] = MCP2517Xfer(0);
  #endif
   
   output_high(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   if(crc16.w != calculate_crc16(rValue, 1))
      return(FALSE);
   else
  #endif
      return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// mcp2517_read_sfr_bytes()
//
// Function to read multiple bytes from SFR memory, address ranges 0x000 to
// 0x2EF and 0xE00 to 0xE13.
//
// Parameters:
//    Address - address in SFR memory to start reading.
//
//    rValue - pointer to uint8_t array to return read bytes to.
//
//    Count - number of bytes to read.
//
// Returns:
//    TRUE - if memory was read correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined. 
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_read_sfr_bytes(MCP2517_ADDRESS Address, uint8_t *rValue, uint8_t Count)
{  
  #ifdef MCP2517_SPI_USE_CRC
   CRC16_T crc16;
   uint8_t Value;
  #endif
   uint8_t i;

   output_low(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   Value = MCP2517_CMD_READ_CRC | (make8(Address, 1) & 0x0F);
   MCP2517Xfer(Value);
   calculate_crc16(&Value, 1, TRUE);
  #else
   MCP2517Xfer(MCP2517_CMD_READ | (make8(Address, 1) & 0x0F));
  #endif
  
   MCP2517Xfer(make8(Address, 0));
  #ifdef MCP2517_SPI_USE_CRC
   calculate_crc16((uint8_t *)&Address, 1);
  #endif
   
  #ifdef MCP2517_SPI_USE_CRC
   MCP2517Xfer(Count);
   calculate_crc16(&Count, 1);
  #endif
   
   for(i=0;i<Count;i++)
      rValue[i] = MCP2517Xfer(0);
   
  #ifdef MCP2517_SPI_USE_CRC
   crc16.b[1] = MCP2517Xfer(0);
   crc16.b[0] = MCP2517Xfer(0);
  #endif
   
   output_high(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   if(crc16.w != calculate_crc16(rValue, Count))
      return(FALSE);
   else
  #endif
      return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// mcp2517_read_memory()
//
// Function used to read a single double word from message memory, address 
// range 0x400 to 0xBFF.
//
// Parameters:
//    Address - address in message memory to read.  Memory is byte addressed so
//              Address should be a multiple of 4, no check is made to guarantee
//              this.
//
//    rValue - pointer to DW_T variable to return read value to.
//
// Returns:
//    TRUE - if memory was read correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined.
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_read_memory(MCP2517_ADDRESS Address, DW_T *rValue)
{
  #ifdef MCP2517_SPI_USE_CRC
   CRC16_T crc16;
   uint8_t Value;
  #endif

   output_low(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   Value = MCP2517_CMD_READ_CRC | (make8(Address, 1) & 0x0F);
   MCP2517Xfer(Value);
   calculate_crc16(&Value, 1, TRUE);
  #else
   MCP2517Xfer(MCP2517_CMD_READ | (make8(Address, 1) & 0x0F));
  #endif
  
   MCP2517Xfer(make8(Address, 0));
  #ifdef MCP2517_SPI_USE_CRC
   calculate_crc16((uint8_t *)&Address, 1);
  #endif
   
  #ifdef MCP2517_SPI_USE_CRC
   Value = 1;
   MCP2517Xfer(Value);
   calculate_crc16(&Value, 1);
  #endif
   
   rValue->b[0] = MCP2517Xfer(0);
   rValue->b[1] = MCP2517Xfer(0);
   rValue->b[2] = MCP2517Xfer(0);
   rValue->b[3] = MCP2517Xfer(0);
   
  #ifdef MCP2517_SPI_USE_CRC
   crc16.b[1] = MCP2517Xfer(0);
   crc16.b[0] = MCP2517Xfer(0);
  #endif
   
   output_high(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   if(crc16.w != calculate_crc16((uint8_t *)rValue, 4))
      return(FALSE);
   else
  #endif
      return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//  mcp2517_read_memory_dwords()
//
// Function used to read multiple double word from message memory, address 
// range 0x400 to 0xBFF.
//
// Parameters:
//    Address - address in message memory to start reading.  Memory is byte
//              addressed so Address should be a multiple of 4, no check is made
//              to guarantee this.
//
//    rValue - pointer to DW_T array to return read data to.
//
//    Count - number of double words to read from message memory.
//
// Returns:
//    TRUE - if memory was read correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined.
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_read_memory_dwords(MCP2517_ADDRESS Address, DW_T *rValue, uint8_t Count)
{
  #ifdef MCP2517_SPI_USE_CRC
   CRC16_T crc16;
   uint8_t Value;
  #endif
   DW_T *dPtr;
   uint8_t i;

   output_low(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   Value = MCP2517_CMD_READ_CRC | (make8(Address, 1) & 0x0F);
   MCP2517Xfer(Value);
   calculate_crc16(&Value, 1, TRUE);
  #else
   MCP2517Xfer(MCP2517_CMD_READ | (make8(Address, 1) & 0x0F));
  #endif
  
   MCP2517Xfer(make8(Address, 0));
  #ifdef MCP2517_SPI_USE_CRC
   calculate_crc16((uint8_t *)&Address, 1);
  #endif
   
  #ifdef MCP2517_SPI_USE_CRC
   MCP2517Xfer(Count);
   calculate_crc16(&Count, 1);
  #endif
  
   dPtr = rValue;
   
   for(i=0;i<Count;i++)
   {
      dPtr->b[0] = MCP2517Xfer(0);
      dPtr->b[1] = MCP2517Xfer(0);
      dPtr->b[2] = MCP2517Xfer(0);
      dPtr->b[3] = MCP2517Xfer(0);
      
      dPtr++;
   }
   
  #ifdef MCP2517_SPI_USE_CRC
   crc16.b[1] = MCP2517Xfer(0);
   crc16.b[0] = MCP2517Xfer(0);
  #endif
   
   output_high(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_SPI_USE_CRC
   if(crc16.w != calculate_crc16((uint8_t *)rValue, ((uint16_t)Count * 4)))
      return(FALSE);
   else
  #endif
      return(TRUE);
}

/////////////////////////////// Write Functions ///////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// mcp2517_write_sfr()
//
// Function for write a single byte to SFR memory, address ranges 0x000 to 0x2EF
// and 0xE00 to 0xE13.
//
// Parameters:
//    Address - address in SFR memory to write to.
//
//    wValue - byte to write to memory.
//
// Returns:
//    TRUE - if memory was written correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined.
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_write_sfr(MCP2517_ADDRESS Address, uint8_t wValue)
{
 #ifdef MCP2517_SPI_USE_CRC
   CRC16_T crc16;
   uint32_t Status;
   int1 noError;
  #if MCP2517_SPI_DO_SAFE_WRITE == TRUE 
   uint8_t wData[3];
   
   wData[0] = MCP2517_CMD_WRITE_SAFE | (make8(Address, 1) & 0x0F);
   wData[1] = make8(Address, 0);
   wData[2] = wValue;
   
   crc16.w = calculate_crc16(wData, 3, TRUE);
   
   output_low(MCP2517_SPI_CS_PIN);
   
   MCP2517Xfer(wData[0]);
   MCP2517Xfer(wData[1]);
   MCP2517Xfer(wData[2]);
   MCP2517Xfer(crc16.b[1]);
   MCP2517Xfer(crc16.b[0]);
   
   output_high(MCP2517_SPI_CS_PIN);
  #else
   uint8_t wData[4];
   
   wData[0] = MCP2517_CMD_WRITE_CRC | (make8(Address, 1) & 0x0F);
   wData[1] = make8(Address, 0);
   wData[2] = 1;
   wData[3] = wValue;
   
   crc16.w = calculate_crc16(wData, 4, TRUE);
   
   output_low(MCP2517_SPI_CS_PIN);
   
   MCP2517Xfer(wData[0]);
   MCP2517Xfer(wData[1]);
   MCP2517Xfer(wData[2]);
   MCP2517Xfer(wData[3]);
   MCP2517Xfer(crc16.b[1]);
   MCP2517Xfer(crc16.b[0]);
   
   output_high(MCP2517_SPI_CS_PIN);
  #endif
   
   do
   {
      noError = mcp2517_read_sfr_bytes(MCP2517_ADDR_REG_CRC, (uint8_t *)&Status, 4);
   } while(noError == FALSE);
   
   if(bit_test(Status, 16) == 1)
      return(FALSE);
   else
      return(TRUE);
 #else
   output_low(MCP2517_SPI_CS_PIN);
   
   MCP2517Xfer(MCP2517_CMD_WRITE | (make8(Address, 1) & 0x0F));
   MCP2517Xfer(make8(Address, 0));
   MCP2517Xfer(wValue);
   
   output_high(MCP2517_SPI_CS_PIN);
   
   return(TRUE);
 #endif
}

///////////////////////////////////////////////////////////////////////////////
// mcp2517_write_sfr_bytes()
//
// Function for writing multiple bytes to SFR memory, address ranges 0x000 to 
// 0x2EF and 0xE00 to 0xE13.
//
// Parameters:
//    Address - address in SFR memory to start writing to.
//
//    Data - pointer to uint8_t array containing bytes to write to memory.
//
//    Count - number of bytes to write.
//
// Returns:
//    TRUE - if memory was written correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined.
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_write_sfr_bytes(MCP2517_ADDRESS Address, uint8_t *Data, uint8_t Count)
{
#ifdef MCP2517_SPI_USE_CRC
 #if MCP2517_SPI_DO_SAFE_WRITE == TRUE
   uint8_t i;
   uint8_t eCount;
   
   for(i=0,eCount=0;i<Count;i++,Address++)
   {
      if(mcp2517_write_sfr(Address, Data[i]) == FALSE)
         eCount++;
   }
   
   if(eCount > 0)
      return(FALSE);
   else
      return(TRUE);
 #else
   uint8_t i;
   CRC16_T crc16;
   uint32_t Status;
   int1 noError;
   uint8_t wValue;
   
   output_low(MCP2517_SPI_CS_PIN);
   
   //cmd
   wValue = MCP2517_CMD_WRITE_CRC | (make8(Address, 1) & 0x0F);
   MCP2517Xfer(wValue);
   calculate_crc16(&wValue, 1, TRUE);
   
   //address
   MCP2517Xfer(make8(Address, 0));
   calculate_crc16((uint8_t *)&Address, 1);
   
   //count
   MCP2517Xfer(Count);
   calculate_crc16(&Count, 1);
   
   //data
   for(i=0;i<Count;i++)
      MCP2517Xfer(Data[i]);
   crc16.w = calculate_crc16(Data, Count);
   
   //crc16
   MCP2517Xfer(crc16.b[1]);
   MCP2517Xfer(crc16.b[0]);
   
   output_high(MCP2517_SPI_CS_PIN);
   
   do
   {
      noError = mcp2517_read_sfr_bytes(MCP2517_ADDR_REG_CRC, (uint8_t *)&Status, 4);
   } while(noError == FALSE);
   
   if(bit_test(Status, 16) == 1)
      return(FALSE);
   else
      return(TRUE);
 #endif
#else
   uint8_t i;
   
   output_low(MCP2517_SPI_CS_PIN);
   
   MCP2517Xfer(MCP2517_CMD_WRITE | (make8(Address, 1) & 0x0F));
   MCP2517Xfer(make8(Address, 0));
   
   for(i=0;i<Count;i++)
      MCP2517Xfer(Data[i]);
   
   output_high(MCP2517_SPI_CS_PIN);
   
   return(TRUE);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// mcp2517_write_memory()
//
// Function used to write a single double word to message memory, address range
// 0x400 to 0xBFF.
//
// Parameters:
//    Address - address in message memory to write.  Memory is byte addressed so
//              Address should be a multiple of 4, no check is made to guarantee
//              this.
//
//    wValue - DW_T value to write to memory.
//
// Returns:
//    TRUE - if memory was written correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined.
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_write_memory(MCP2517_ADDRESS Address, DW_T wValue)
{
 #ifdef MCP2517_SPI_USE_CRC
   CRC16_T crc16;
   uint32_t Status;
   int1 noError;
   uint8_t i;
  #if MCP2517_SPI_DO_SAFE_WRITE == TRUE 
   uint8_t wData[6];
   
   wData[0] = MCP2517_CMD_WRITE_SAFE | (make8(Address, 1) & 0x0F);
   wData[1] = make8(Address, 0);
   memcpy(&wData[2], wValue.b, 4);
   
   crc16.w = calculate_crc16(wData, 6, TRUE);
   
   output_low(MCP2517_SPI_CS_PIN);
   
   for(i=0;i<6;i++)
      MCP2517Xfer(wData[i]);
      
   MCP2517Xfer(crc16.b[1]);
   MCP2517Xfer(crc16.b[0]);
   
   output_high(MCP2517_SPI_CS_PIN);
  #else
   uint8_t wData[7];
   
   wData[0] = MCP2517_CMD_WRITE_CRC | (make8(Address, 1) & 0x0F);
   wData[1] = make8(Address, 0);
   wData[2] = 1;
   memcpy(&wData[3], wValue.b, 4);
   
   crc16.w = calculate_crc16(wData, 7, TRUE);
   
   output_low(MCP2517_SPI_CS_PIN);
   
   for(i=0;i<7;i++)
      MCP2517Xfer(wData[i]);
      
   MCP2517Xfer(crc16.b[1]);
   MCP2517Xfer(crc16.b[0]);
   
   output_high(MCP2517_SPI_CS_PIN);
  #endif
   
   do
   {
      noError = mcp2517_read_sfr_bytes(MCP2517_ADDR_REG_CRC, (uint8_t *)&Status, 4);
   } while(noError == FALSE);
   
   if(bit_test(Status, 16) == 1)
      return(FALSE);
   else
      return(TRUE);
 #else
   output_low(MCP2517_SPI_CS_PIN);
   
   MCP2517Xfer(MCP2517_CMD_WRITE | (make8(Address, 1) & 0x0F));
   MCP2517Xfer(make8(Address, 0));
   MCP2517Xfer(wValue.b[0]);
   MCP2517Xfer(wValue.b[1]);
   MCP2517Xfer(wValue.b[2]);
   MCP2517Xfer(wValue.b[3]);
   
   output_high(MCP2517_SPI_CS_PIN);
   
   return(TRUE);
 #endif
}

///////////////////////////////////////////////////////////////////////////////
// mcp2517_write_memory_dwords()
//
// Function used to write multiple double words to message memory, address range
// 0x400 to 0xBFF.
//
// Parameters:
//    Address - address in message memory to start writing.  Memory is byte
//              addressed so Address should be a multiple of 4, no check is made
//              to guarantee this.
//
//    wValue - pointer to DW_T array containing data to write to memory.
//
//    Count - number of double words to write to memory.
//
// Returns:
//    TRUE - if memory was written correctly.
//    FALSE - if an error occurred, e.g. failed CRC check.  CRC is only checked
//            if MCP2517_SPI_USE_CRC is defined.
///////////////////////////////////////////////////////////////////////////////
int1 mcp2517_write_memory_dwords(MCP2517_ADDRESS Address, DW_T *wValue, uint8_t Count)
{
#ifdef MCP2517_SPI_USE_CRC
 #if MCP2517_SPI_DO_SAFE_WRITE == TRUE
   uint8_t i;
   uint8_t eCount;
   
   for(i=0,eCount=0;i<Count;i++,Address+=4)
   {
      if(mcp2517_write_memory(Address, wValue[i]) == FALSE)
         eCount++;
   }
   
   if(eCount > 0)
      return(FALSE);
   else
      return(TRUE);
 #else
   uint8_t i;
   CRC16_T crc16;
   uint32_t Status;
   int1 noError;
   uint8_t Cmd;
   
   output_low(MCP2517_SPI_CS_PIN);
   
   //cmd
   Cmd = MCP2517_CMD_WRITE_CRC | (make8(Address, 1) & 0x0F);
   MCP2517Xfer(Cmd);
   calculate_crc16(&Cmd, 1, TRUE);
   
   //address
   MCP2517Xfer(make8(Address, 0));
   calculate_crc16((uint8_t *)&Address, 1);
   
   //count
   MCP2517Xfer(Count);
   calculate_crc16(&Count, 1);
   
   //data
   for(i=0;i<Count;i++)
   {
      MCP2517Xfer(wValue[i].b[0]);
      MCP2517Xfer(wValue[i].b[1]);
      MCP2517Xfer(wValue[i].b[2]);
      MCP2517Xfer(wValue[i].b[3]);
   }
   crc16.w = calculate_crc16((uint8_t *)wValue, (uint16_t)Count * 4);
   
   //crc16
   MCP2517Xfer(crc16.b[1]);
   MCP2517Xfer(crc16.b[0]);
   
   output_high(MCP2517_SPI_CS_PIN);
   
   do
   {
      noError = mcp2517_read_sfr_bytes(MCP2517_ADDR_REG_CRC, (uint8_t *)&Status, 4);
   } while(noError == FALSE);
   
   if(bit_test(Status, 16) == 1)
      return(FALSE);
   else
      return(TRUE);
 #endif
#else
   uint8_t i;
   DW_T *dPtr;
   
   output_low(MCP2517_SPI_CS_PIN);
   
   MCP2517Xfer(MCP2517_CMD_WRITE | (make8(Address, 1) & 0x0F));
   MCP2517Xfer(make8(Address, 0));
   
   dPtr = wValue;
   
   for(i=0;i<Count;i++)
   {
      MCP2517Xfer(dPtr->b[0]);
      MCP2517Xfer(dPtr->b[1]);
      MCP2517Xfer(dPtr->b[2]);
      MCP2517Xfer(dPtr->b[3]);
      
      dPtr++;
   }
   
   output_high(MCP2517_SPI_CS_PIN);
   
   return(TRUE);
#endif
}


//////////////////////////////// Init Functions ///////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// mcp2517_init()
//
///////////////////////////////////////////////////////////////////////////////
void mcp2517_init(void)
{
   uint8_t Value = 0;

   output_high(MCP2517_SPI_CS_PIN);
   output_drive(MCP2517_SPI_CS_PIN);
   
  #ifdef MCP2517_INT_PIN
   output_float(MCP2517_INT_PIN);
  #endif
  
  #ifdef MCP2517_INT0_PIN
   output_float(MCP2517_INT0_PIN);
  #endif
  
  #ifdef MCP2517_INT1_PIN
   output_float(MCP2517_INT1_PIN);
  #endif
  
  #if defined(MCP2517_SPI_USE_CRC) && (MCP2517_SPI_FORCE_SW_CRC == FALSE)
   setup_crc(16, 15, 2, 0);
  #endif
   
  #if (MCP2517_ENABLE_PLL == TRUE)
   ((MCP2517_OSC)Value).PLLEN = 1;
  #endif
  
  #if (MCP2517_SYSTEM_CLOCK_DIVISOR == 2)
   ((MCP2517_OSC)Value).SCLKDIV = 1;
  #endif
  
  #if MCP2517_CLOCK_OUT_DIVISOR == 2
   ((MCP2517_OSC)Value).CLKODIV = 1;
  #elif (MCP2517_CLOCK_OUT_DIVISOR == 4)
   ((MCP2517_OSC)Value).CLKODIV = 2;
  #elif (MCP2517_CLOCK_OUT_DIVISOR == 10)
   ((MCP2517_OSC)Value).CLKODIV = 3;
  #endif
   
   while(!mcp2517_write_sfr(MCP2517_ADDR_REG_OSC, Value));
   
 #if defined(MCP2517_DEFAULT_LAT0) || defined(MCP2517_DEFAULT_LAT1)
   Value = 0;
  
  #if defined(MCP2517_DEFAULT_LAT0) && (MCP2517_DEFAULT_LAT0 == 1)
   bit_set(Value, 0);
  #endif
  
  #if defined(MCP2517_DEFAULT_LAT1) && (MCP2517_DEFAULT_LAT1 == 1)
   bit_set(Value, 1);
  #endif
   
   while(!mcp2517_write_sfr(MCP2517_ADDR_REG_IOCON + 1, Value));
 #endif
  
 #if defined(MCP2517_XSTBY) || defined(MCP2517_TRIS0) || defined(MCP2517_TRIS0)
   Value = 0;
   
  #if defined(MCP2517_XSTBY) && (MCP2517_XSTBY == 1)
   bit_set(Value, 6);
  #endif
   
  #if defined(MCP2517_TRIS0) && (MCP2517_TRIS0 == 1)
   bit_set(Value, 0);
  #endif
  
  #if defined(MCP2517_TRIS1) && (MCP2517_TRIS1 == 1)
   bit_set(Value, 1);
  #endif
   
   while(!mcp2517_write_sfr(MCP2517_ADDR_REG_IOCON, Value));  
 #endif
 
   Value = 0;
   
  #if (MCP2517_INT_OUTPUT_MODE == 1)
   bit_set(Value, 6);
  #endif
  
  #if (MCP2517_CLKO_TYPE == 1)
   bit_set(Value, 5);
  #endif
  
  #if (MCP2517_TXCAN_OUTPUT_MODE == 1)
   bit_set(Value, 4);
  #endif
  
  #ifndef MCP2517_INT1_PIN
   bit_set(Value, 1);
  #endif
  
  #ifndef MCP2517_INT0_PIN
   bit_set(Value, 0);
  #endif
   
   mcp2517_write_sfr(MCP2517_ADDR_REG_IOCON + 3, Value);
}

#ifdef MCP2517_SPI_USE_CRC
 #if MCP2517_SPI_FORCE_SW_CRC == TRUE
  #if MCP2517_SPI_USE_LOOKUP_TABLE == TRUE
   #if MCP2517_SPI_USE_RAM_LOOKUP_TABLE == TRUE
    uint16_t CRC16_TABLE[256] = 
   #else
    rom uint16_t CRC16_TABLE[256] = 
   #endif
    {
      0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,
      0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,
      0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,
      0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,
      0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,
      0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,
      0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,
      0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,
      0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,
      0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,
      0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,
      0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,
      0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,
      0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,
      0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,
      0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202
    };
  #else
   #define CRC_16    0x8005
  #endif

  uint16_t calculate_crc16(uint8_t *Data, uint16_t Length, int1 New=FALSE)
  {
      static uint16_t crc_Dbyte;
      
      if(New)
         crc_Dbyte = 0xFFFF;
      
     #if MCP2517_SPI_USE_LOOKUP_TABLE == TRUE
      uint8_t Index;
      
      while(Length > 0)
      {
         Index = make8(crc_Dbyte, 1) ^ *Data;
         crc_Dbyte = make16(make8(crc_Dbyte, 0), 0) ^ CRC16_TABLE[Index];
         
         Data++;
         Length--;
      }
     #else
      uint8_t bData;
      uint8_t bit_counter;
      int1 bit_flag;
      
      while(Length > 0)
      {
         bData = *Data;
         crc_Dbyte ^= make16(bData, 0);
         
         for(bit_counter=0;bit_counter<8;bit_counter++)
         {
            bit_flag = bit_test(crc_Dbyte, 15);
            
            crc_Dbyte <<= 1;
            
            if(bit_flag)
               crc_Dbyte ^= CRC_16;
         }
         
         Data++;
         Length--;
      }
     #endif
      
      return(crc_Dbyte);
  }
 #else
  #warning HW CRC hasn't been tested
  
  uint16_t calculate_crc16(uint8_t *Data, uint16_t Length, int1 New=FALSE)
  {
      static uint16_t crc_Dbyte;
   
      if(New)
         crc_Dbyte = 0xFFFF;
         
      crc_init(crc_Dbyte);
    
      return(crc_calc16(Data, Length, 8));
  }
 #endif
#endif

#endif //__CAN_MCP2517_C__
