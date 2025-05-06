///////////////////////////////////////////////////////////////////////////////
////                           can-dspic33_fd.c                            ////
////                                                                       ////
////  Driver for CAN FD peripheral for Microchip's dsPIC33CH and dsPIC33CK ////
////  devices with a CAN FD peripheral.                                    ////
////                                                                       ////
////  See can-dspic33_fd.h for API.                                        ////
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

#ifndef __CAN_DSPIC33_FD_C__
#define __CAN_DSPIC33_FD_C__

#include <can-dspic33_fd.h>

///////////////////////////////// CAN Settings ////////////////////////////////

// CAN Peripheral Clock Settings
#ifndef CAN_CLOCK_SOURCE
 #define CAN_CLOCK_SOURCE                    1     //0-CAN Clock Generator, 1-Auxiliary Clock
#endif

#if CAN_CLOCK_SOURCE == 0
 #ifndef  CAN_CLOCK_GEN_SOURCE
  #define CAN_CLOCK_GEN_SOURCE               1     //0-None, 1-FVCO, 2-FPLLO, 3-FVCO/2, 4-FVCO/3, 5-FVCO/4
 #endif                                            //6-AFPPLO, 7-AFVCO, 8-AFVCO/2, 9-AFVCO/3, 10-AFVCO/4
 
 #ifndef CAN_CLOCK_GEN_DIVISOR
  #define CAN_CLOCK_GEN_DIVISOR              1     //1-128
 #endif
#endif

//CAN Baud Rate Settings
#ifndef CAN_CLOCK_SPEED
 #define CAN_CLOCK_SPEED         80000000    //The speed of the Clock source provided to CANFD peripheral
#endif

#ifndef CAN_NOMINAL_BAUD_RATE
 #define CAN_NOMINAL_BAUD_RATE   250000      //The Nominal CAN Baud Rate
#endif

#ifndef CAN_DATA_BAUD_RATE
 #define CAN_DATA_BAUD_RATE      2000000     //The Data CAN Baud Rate, when using CANFD
#endif

#if (CAN_CLOCK_SPEED % CAN_NOMINAL_BAUD_RATE) != 0
 #error CAN Clock Speed not evenly divisible by CAN Nominal Baud Rate
#endif

#if (CAN_DATA_BAUD_RATE != 0) && ((CAN_CLOCK_SPEED % CAN_DATA_BAUD_RATE) != 0)
 #error CAN Clock Speed not evenly divisible by CAN Data Baud Rate
#endif

#ifndef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT      875   //in tenth of a percent, e.g. 875 = 87.5% of bit time
#endif

//Other CAN Peripheral Settings
#ifndef CAN_STOP_IN_IDLE
 #define CAN_STOP_IN_IDLE  TRUE  //TRUE-Stops module operation in Idle mode, FALSE-Continue module operation in Idle mode
#endif

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

#ifndef CAN_RESTRICT_RETRANSMISSION_ATTEMPTS
 #define CAN_RESTRICT_RETRANSMISSION_ATTEMPTS   FALSE //TRUE-Restrict retransmission attempts, FALSE-unlimited number of retransmission attempts
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

#ifndef CAN_MESSAGE_START_ADDR
 #define CAN_MESSAGE_START_ADDR     0x1000   //Messages buffers must be aligned on a 4 word address boundary, e.g. 0x1000, 0x1004, etc. by default
#endif                                       //buffer will be located starting at address 0x1000, if another memory region is preferred define
                                             //CAN_MESSAGE_START_ADDR to the desired memory start address

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
 #define CAN_TX_BUFFERS       1     //0 to 32 - The number of FIFO's in the TX Queue Object
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
 #define CAN_RX_BUFFERS       32    // 0 - 32
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
   2-7 the following defines can be made to before this driver is included to
   enable them:
      #define CAN_USE_FIFO_x_OBJECT                               // x being the FIFO Object to enable (1-7)
      #define CAN_FIFO_x_OBJECT_TYPE                        aa    // x being the FIFO Object (1-7), and aa being 0-RX Object or 1-TX Object
      #define CAN_FIFO_x_OBJECT_COUNT                       bb    // x begin the FIFO Object (1-7), and bb being the number of FIFOs Object has (1-32)
      #define CAN_FIFO_x_OBJECT_MAX_PAYLOAD_SIZE            cc    // x being the FIFO Object (1-7), and cc being PAYLOAD_SIZE type.
      #define CAN_FIFO_x_OBJECT_TX_PRIORITY                 dd    // x being the FIFO Object (1-7), and dd being 0 to 31 (0-lowest priority, 31-highest priority)
                                                                  // Only needed when Object is a TX Object
      #define CAN_FIFO_x_OBJECT_TX_RETRANSMISSION_ATTEMPTS  ee    // x being the FIFO Object (1-7), and ee being RETRANSMIT_ATTEMPTS type.
                                                                  // Only needed when Object is a TX Object
      #define CAN_FIFO_x_OBJECT_RX_TIME_STAMP               ff    // x being the FIFO Object (1-7), and ff being TRUE-Capture time stamped or FALSE Don't capture time stamp.
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

///////////////////////////// CAN FILTER Settings /////////////////////////////

/*
   If no CAN RX Filters are going to be used then nothing needs to be done, by
   default this driver will initialize the CAN FD controller to use no filters.
   
   If CAN RX Filters are going to be used, the can_init() function can be used
   to initialize them by making the following defines before this driver is
   included:
   
   #define CAN_USE_FILTERS          TRUE
   
   #define CAN_USER_FILT_x          aa    // x being the filter to enable (0-15),and aa being the Id to filter for.
   #define CAN_USER_MASK_x          bb    // x being the filter to enable (0-15),and bb being the Mask of Id to filter for.
                                          // Setting a bit in the Mask means the corresponding bit in the Id must match.
                                          // For example using a Mask of 0x1FFFFFF8 with Id of 0x1000, means that all Id's
                                          // from 0x1000 to 0x1007 will be accepted by the filter.
   #define CAN_USER_FILT_x_TYPE     cc    // x being the filter to enable (0-15), and cc being one of the following:
                                          //    CAN_FILTER_TYPE_SID_ONLY - match only SID messages
                                          //    CAN_FILTER_TYPE_EID_ONLY - match only EID messages 
                                          //    CAN_FILTER_TYPE_EITHER   - match either SID or EID messages
   #define CAN_USER_FILT_x_OBJECT   dd    // x being the filter to enable (0-15), and dd being CAN_OBJECT type, see CAN_OBJECT
                                          // type below.  This define is optional by default they are set to CAN_OBJECT_FIFO_1
                                          // because by default this driver only use CAN_OBJECT_FIFO_1 to receive messages.
   
   The CAN RX Filters can also be setup, enabled and disabled at run time using 
   the can_set_filter_id(), can_enable_filter() and can_disable_filter() 
   functions.
*/

#ifndef CAN_USE_FILTERS
 #define CAN_USE_FILTERS   FALSE
#endif

#if CAN_USE_FILTERS == FALSE
 /*
   The CAN peripheral requires at least one filter to receive messages.  If
   CAN_USE_FILTERS is defined to FALSE the following ensures that Filter 0 is
   setup to receive all messages.
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
#endif

////////////////////////////////// CAN Types //////////////////////////////////

// CAN Clock Control
typedef struct struct_CANCLKCON
{
   uint8_t CLKDIV;
   uint8_t CLKSEL:4;
   uint8_t u12_14:3;
   uint8_t CLKEN:1;
} CAN_CLKCON;

CAN_CLKCON CAN_CANCLKCON;
#locate CAN_CANCLKCON=getenv("SFR:CANCLKCON")

// CAN Control
typedef struct struct_CiCON
{
   uint8_t DNCNT:5;              //Device Net Filter Bit Number bits
   uint8_t ISOCRC:1;             //Enable ISO CRC in CAN FD Frame bit
   uint8_t PXE:1;                //Protocol Exception Event Detection bit
   uint8_t CLKSEL:1;             //Module Clock Source Select bit, 0-CAN clock generator, 1-Auxiliary clock
   uint8_t WAKFIL:1;             //Enable CAN Bus Line Wake-up Filter bit
   uint8_t WFT:2;                //Wake-up Filter Time bits
   uint8_t BUSY:1;               //CAN Module is Busy bit
   uint8_t BRS:1;                //Bit Rate Switching bit
   uint8_t SIDL:1;               //CAN Stop in Idle Control bit, 1-Stop module operation in Idle, 0-Does not stop
   uint8_t u14:1;
   uint8_t CON:1;                //CAN Enable bit, 1-Enable, 0-Disable
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

CiCON CAN_C1CON;
#locate CAN_C1CON=getenv("SFR:C1CONL")

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

CiNBTCFG CAN_C1NBTCFG;
#locate CAN_C1NBTCFG=getenv("SFR:C1NBTCFGL")

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

CiDBTCFG CAN_C1DBTCFG;
#locate CAN_C1DBTCFG=getenv("SFR:C1DBTCFGL")

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

CiTDC CAN_C1TDC;
#locate CAN_C1TDC=getenv("SFR:C1TDCL")

// CAN Time Base Counter
typedef uint32_t CiTBC;

CiTBC CAN_C1TBC;
#locate CAN_C1TBC=getenv("SFR:C1TBCL")

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

CiTSCON CAN_C1TSCON;
#locate CAN_C1TSCON=getenv("SFR:C1TSCONL")

// CAN Interrupt Code
typedef enum {CAN_INT_TXQ, CAN_INT_FIFO_1, CAN_INT_FIFO_2, CAN_INT_FIFO_3,
              CAN_INT_FIFO_4, CAN_INT_FIFO_5, CAN_INT_FIFO_6, CAN_INT_FIFO_7,
              CAN_INT_NONE=64, CAN_INT_ERR, CAN_INT_WAK, CAN_INT_RX_FIFO_OVERFLOW,
              CAN_INT_ADDR_ERR, CAN_INT_MAB_OVERFLOW_UNDERFLOW, CAN_INT_TBC_OVERFLOW,
              CAN_INT_OPMOD_CHANGED, CAN_INT_INVALID_MESSAGE, CAN_INT_TX_EVENT_FIFO,
              CAN_INT_TX_ATTEMPT
} CAN_INT_CODE;
              
typedef enum {CAN_TX_INT_TXQ, CAN_TX_INT_FIFO_1, CAN_TX_INT_FIFO_2, CAN_TX_INT_FIFO_3,
              CAN_TX_INT_FIFO_4, CAN_TX_INT_FIFO_5, CAN_TX_INT_FIFO_6, CAN_TX_INT_FIFO_7
} CAN_TX_INT_CODE;

typedef enum {CAN_RX_INT_FIFO_1=1, CAN_RX_INT_FIFO_2, CAN_RX_INT_FIFO_3, CAN_RX_INT_FIFO_4,
              CAN_RX_INT_FIFO_5, CAN_RX_INT_FIFO_6, CAN_RX_INT_FIFO_7
} CAN_RX_INT_CODE;

typedef struct struct_CiVEC
{
   CAN_INT_CODE ICODE;        //Interrupt Flag Code bits
   uint8_t FILHIT;            //Filter Hit Number bits
   CAN_TX_INT_CODE TXCODE;    //Transmit Interrupt Flag Code bits
   CAN_RX_INT_CODE RXCODE;    //Receive Interrupt Flag Code bits
} CiVEC;

CiVEC CAN_C1VEC;
#locate CAN_C1VEC=getenv("SFR:C1VECL")

// CAN Interrupt
typedef struct struct_CiINT
{
   uint8_t TXIF:1;      //Transmit FIFO Interrupt Flag bit
   uint8_t RXIF:1;      //Receive FIFO Interrupt Flag bit
   uint8_t TBCIF:1;     //Time Base Count Overflow Interrupt Flag bit
   uint8_t MODIF:1;     //Operation Mode Change Interrupt Flag bit
   uint8_t TEFIF:1;     //Transmit Event FIFO Interrupt Flag bit
   uint8_t u5_7:3;
   uint8_t u8_9:2;
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
   uint8_t u24_25:2;
   uint8_t TXATIE:1;    //Transmit Attempt Interrupt Enable bit
   uint8_t RXOVIE:1;    //Receive FIFO Overflow Interrupt Enable bit
   uint8_t SERRIE:1;    //System Error Interrupt Enable bit
   uint8_t CERRIE:1;    //CAN Bus Error Interrupt Enable bit
   uint8_t WAKIE:1;     //Bus Wake Up Interrupt Enable bit
   uint8_t IVMIE:1;     //Invalid Message Interrupt Enable bit
} CiINT;

CiINT CAN_C1INT;
#locate CAN_C1INT=getenv("SFR:C1INTL")

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
      uint8_t u8_15;
      uint16_t u16_31;
   };
   uint32_t dw;
} CiRXIF;

CiRXIF CAN_C1RXIF;
#locate CAN_C1RXIF=getenv("SFR:C1RXIFL")

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
      uint8_t u8_15;
      uint16_t u16_31;
   };
   uint32_t dw;
} CiRXOVIF;

CiRXOVIF CAN_C1RXOVIF;
#locate CAN_C1RXOVIF=getenv("SFR:C1RXOVIFL")

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
      uint8_t u8_15;
      uint16_t u16_31;
   };
   uint32_t dw;
} CiTXIF;

CiTXIF CAN_C1TXIF;
#locate CAN_C1TXIF=getenv("SFR:C1TXIFL")

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
      uint8_t u8_15;
      uint16_t u16_31;
   };
   uint32_t dw;
} CiTXATIF;

CiTXATIF CAN_C1TXATIF;
#locate CAN_C1TXATIF=getenv("SFR:C1TXATIFL")

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
      uint8_t u8_15;
      uint16_t u16_31;
   };
   uint32_t dw;
} CiTXREQ;

CiTXREQ CAN_C1TXREQ;
#locate CAN_C1TXREQ=getenv("SFR:C1TXREQL")

//CAN Message Memory Base Address
typedef uint32_t CiFIFOBA;

CiFIFOBA CAN_C1FIFOBA;
#locate CAN_C1FIFOBA=getenv("SFR:C1FIFOBAL")

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

CiTXQCON CAN_C1TXQCON;
#locate CAN_C1TXQCON=getenv("SFR:C1TXQCONL")

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
} CiTXQSTA;

CiTXQSTA CAN_C1TXQSTA;
#locate CAN_C1TXQSTA=getenv("SFR:C1TXQSTA")

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
   uint8_t TXREQ:1;              //Message Send Request bit
   uint8_t FRESET:1;             //FIFO Reset bit
   uint8_t u11_15:5;
   uint8_t TXPRI:5;              //Message Transmit Priority bits
   RETRANSMIT_ATTEMPTS TXAT:2;   //Retransmission Attempts bits
   uint8_t u23:1;
   uint8_t FSIZE:5;              //FIFO Size bits
   PAYLOAD_SIZE PLSIZE:3;        //Payload Size bits
} CiFIFOCON;

CiFIFOCON CAN_C1FIFOCON1;
CiFIFOCON CAN_C1FIFOCON2;
CiFIFOCON CAN_C1FIFOCON3;
CiFIFOCON CAN_C1FIFOCON4;
CiFIFOCON CAN_C1FIFOCON5;
CiFIFOCON CAN_C1FIFOCON6;
CiFIFOCON CAN_C1FIFOCON7;
#locate CAN_C1FIFOCON1=getenv("SFR:C1FIFOCON1L")
#locate CAN_C1FIFOCON2=getenv("SFR:C1FIFOCON2L")
#locate CAN_C1FIFOCON3=getenv("SFR:C1FIFOCON3L")
#locate CAN_C1FIFOCON4=getenv("SFR:C1FIFOCON4L")
#locate CAN_C1FIFOCON5=getenv("SFR:C1FIFOCON5L")
#locate CAN_C1FIFOCON6=getenv("SFR:C1FIFOCON6L")
#locate CAN_C1FIFOCON7=getenv("SFR:C1FIFOCON7L")

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
} CiFIFOSTA;

CiFIFOSTA CAN_C1FIFOSTA1;
CiFIFOSTA CAN_C1FIFOSTA2;
CiFIFOSTA CAN_C1FIFOSTA3;
CiFIFOSTA CAN_C1FIFOSTA4;
CiFIFOSTA CAN_C1FIFOSTA5;
CiFIFOSTA CAN_C1FIFOSTA6;
CiFIFOSTA CAN_C1FIFOSTA7;
#locate CAN_C1FIFOSTA1=getenv("SFR:C1FIFOSTA1")
#locate CAN_C1FIFOSTA2=getenv("SFR:C1FIFOSTA2")
#locate CAN_C1FIFOSTA3=getenv("SFR:C1FIFOSTA3")
#locate CAN_C1FIFOSTA4=getenv("SFR:C1FIFOSTA4")
#locate CAN_C1FIFOSTA5=getenv("SFR:C1FIFOSTA5")
#locate CAN_C1FIFOSTA6=getenv("SFR:C1FIFOSTA6")
#locate CAN_C1FIFOSTA7=getenv("SFR:C1FIFOSTA7")

// CAN FIFO User Address
typedef uint32_t CiFIFOUA;

CiFIFOUA CAN_C1FIFOUA1;
CiFIFOUA CAN_C1FIFOUA2;
CiFIFOUA CAN_C1FIFOUA3;
CiFIFOUA CAN_C1FIFOUA4;
CiFIFOUA CAN_C1FIFOUA5;
CiFIFOUA CAN_C1FIFOUA6;
CiFIFOUA CAN_C1FIFOUA7;
#locate CAN_C1FIFOUA1=getenv("SFR:C1FIFOUA1L")
#locate CAN_C1FIFOUA2=getenv("SFR:C1FIFOUA2L")
#locate CAN_C1FIFOUA3=getenv("SFR:C1FIFOUA3L")
#locate CAN_C1FIFOUA4=getenv("SFR:C1FIFOUA4L")
#locate CAN_C1FIFOUA5=getenv("SFR:C1FIFOUA5L")
#locate CAN_C1FIFOUA6=getenv("SFR:C1FIFOUA6L")
#locate CAN_C1FIFOUA7=getenv("SFR:C1FIFOUA7L")

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

CiTEFCON CAN_C1TEFCON;
#locate CAN_C1TEFCON=getenv("SFR:C1TEFCONL")

// CAN Transmit Event FIFO Status
typedef struct struct_CiTEFSTA
{
   uint8_t TEFNEIF:1;   //Transmit Event FIFO Not Empty Interrupt Flag bit
   uint8_t TEFHIF:1;    //Transmit Event FIFO Half Full Interrupt Flag bit
   uint8_t TEFFIF:1;    //Transmit Event FIFO Full Interrupt Flag bit
   uint8_t TEFOVIF:1;   //Transmit Event FIFO Overflow Interrupt Flag bit
   uint8_t u4_7:4;
   uint8_t u8_15;
} CiTEFSTA;

CiTEFSTA CAN_C1TEFSTA;
#locate CAN_C1TEFSTA=getenv("SFR:C1TEFSTA")

// CAN Transmit Event FIFO User Address
typedef uint32_t CiTEFUA;

CiTEFUA CAN_C1TEFUA;
#locate CAN_C1TEFUA=getenv("SFR:C1TEFUAL")

// CAN Transmit Queue User Address
typedef uint32_t CiTXQUA;

CiTXQUA CAN_C1TXQUA;
#locate CAN_C1TXQUA=getenv("SFR:C1TXQUAL")

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

CiTREC CAN_C1TREC;
#locate CAN_C1TREC=getenv("SFR:C1TRECL")

// CAN Bus Diagnostic
typedef struct struct_CiBDIAG0
{
   uint8_t NRERRCNT; //Nominal Bit Rate Receive Error Counter bits
   uint8_t NTERRCNT; //Nominal Bit Rate Transmit Error Counter bits
   uint8_t DRERRCNT; //Data Bit Rate Receive Error Counter bits
   uint8_t DTERRCNT; //Data Bit Rate Transmit Error Counter bits
} CiBDIAG0;

CiBDIAG0 CAN_C1BDIAG0;
#locate CAN_C1BDIAG0=getenv("SFR:C1BDIAG0L")

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

CiBDIAG1 CAN_C1BDIAG1;
#locate CAN_C1BDIAG1=getenv("SFR:C1BDIAG1L")

// CAN Filter Control
typedef struct struct_FLTCON
{
   uint8_t BP:5;     //Pointer to FIFO when Filter hits
   uint8_t u5_6:2;
   uint8_t FLTEN:1;  //Enable Filter
} FLTCON;

typedef struct struct_CiFLTCON
{
   FLTCON FA;  //Filter A - A = 0, 4, 6, 12
   FLTCON FB;  //Filter B - B = 1, 5, 9, 13
   FLTCON FC;  //Filter C - C = 2, 6, 10, 14
   FLTCON FD;  //Filter D - D = 3, 7, 11, 15
} CiFLTCON;

CiFLTCON CAN_C1FLTCON0;
CiFLTCON CAN_C1FLTCON1;
CiFLTCON CAN_C1FLTCON2;
CiFLTCON CAN_C1FLTCON3;
#locate CAN_C1FLTCON0=getenv("SFR:C1FLTCON0L")
#locate CAN_C1FLTCON1=getenv("SFR:C1FLTCON1L")
#locate CAN_C1FLTCON2=getenv("SFR:C1FLTCON2L")
#locate CAN_C1FLTCON3=getenv("SFR:C1FLTCON3L")

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

CiFLTOBJ CAN_C1FLTOBJ0;
CiFLTOBJ CAN_C1FLTOBJ1;
CiFLTOBJ CAN_C1FLTOBJ2;
CiFLTOBJ CAN_C1FLTOBJ3;
CiFLTOBJ CAN_C1FLTOBJ4;
CiFLTOBJ CAN_C1FLTOBJ5;
CiFLTOBJ CAN_C1FLTOBJ6;
CiFLTOBJ CAN_C1FLTOBJ7;
CiFLTOBJ CAN_C1FLTOBJ8;
CiFLTOBJ CAN_C1FLTOBJ9;
CiFLTOBJ CAN_C1FLTOBJ10;
CiFLTOBJ CAN_C1FLTOBJ11;
CiFLTOBJ CAN_C1FLTOBJ12;
CiFLTOBJ CAN_C1FLTOBJ13;
CiFLTOBJ CAN_C1FLTOBJ14;
CiFLTOBJ CAN_C1FLTOBJ15;
#locate CAN_C1FLTOBJ0=getenv("SFR:C1FLTOBJ0L")
#locate CAN_C1FLTOBJ1=getenv("SFR:C1FLTOBJ1L")
#locate CAN_C1FLTOBJ2=getenv("SFR:C1FLTOBJ2L")
#locate CAN_C1FLTOBJ3=getenv("SFR:C1FLTOBJ3L")
#locate CAN_C1FLTOBJ4=getenv("SFR:C1FLTOBJ4L")
#locate CAN_C1FLTOBJ5=getenv("SFR:C1FLTOBJ5L")
#locate CAN_C1FLTOBJ6=getenv("SFR:C1FLTOBJ6L")
#locate CAN_C1FLTOBJ7=getenv("SFR:C1FLTOBJ7L")
#locate CAN_C1FLTOBJ8=getenv("SFR:C1FLTOBJ8L")
#locate CAN_C1FLTOBJ9=getenv("SFR:C1FLTOBJ9L")
#locate CAN_C1FLTOBJ10=getenv("SFR:C1FLTOBJ10L")
#locate CAN_C1FLTOBJ11=getenv("SFR:C1FLTOBJ11L")
#locate CAN_C1FLTOBJ12=getenv("SFR:C1FLTOBJ12L")
#locate CAN_C1FLTOBJ13=getenv("SFR:C1FLTOBJ13L")
#locate CAN_C1FLTOBJ14=getenv("SFR:C1FLTOBJ14L")
#locate CAN_C1FLTOBJ15=getenv("SFR:C1FLTOBJ15L")

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

CiMASK CAN_C1MASK0;
CiMASK CAN_C1MASK1;
CiMASK CAN_C1MASK2;
CiMASK CAN_C1MASK3;
CiMASK CAN_C1MASK4;
CiMASK CAN_C1MASK5;
CiMASK CAN_C1MASK6;
CiMASK CAN_C1MASK7;
CiMASK CAN_C1MASK8;
CiMASK CAN_C1MASK9;
CiMASK CAN_C1MASK10;
CiMASK CAN_C1MASK11;
CiMASK CAN_C1MASK12;
CiMASK CAN_C1MASK13;
CiMASK CAN_C1MASK14;
CiMASK CAN_C1MASK15;
#locate CAN_C1MASK0=getenv("SFR:C1MASK0L")
#locate CAN_C1MASK1=getenv("SFR:C1MASK1L")
#locate CAN_C1MASK2=getenv("SFR:C1MASK2L")
#locate CAN_C1MASK3=getenv("SFR:C1MASK3L")
#locate CAN_C1MASK4=getenv("SFR:C1MASK4L")
#locate CAN_C1MASK5=getenv("SFR:C1MASK5L")
#locate CAN_C1MASK6=getenv("SFR:C1MASK6L")
#locate CAN_C1MASK7=getenv("SFR:C1MASK7L")
#locate CAN_C1MASK8=getenv("SFR:C1MASK8L")
#locate CAN_C1MASK9=getenv("SFR:C1MASK9L")
#locate CAN_C1MASK10=getenv("SFR:C1MASK10L")
#locate CAN_C1MASK11=getenv("SFR:C1MASK11L")
#locate CAN_C1MASK12=getenv("SFR:C1MASK12L")
#locate CAN_C1MASK13=getenv("SFR:C1MASK13L")
#locate CAN_C1MASK14=getenv("SFR:C1MASK14L")
#locate CAN_C1MASK15=getenv("SFR:C1MASK15L")

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
   uint16_t SEQ7_22;
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
    (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     64
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     48
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     32
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     24
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     20
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     16
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     12
#elif ((CAN_TX_BUFFERS > 0) && (CAN_TX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 1) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 1) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 1) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 1) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 1) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 1) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 1) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES))
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     8
#else
 #define CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE     0
#endif

#if ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES)) || \
    (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_64_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     64
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_48_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     48
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_32_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     32
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_24_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     24
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_20_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     20
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_16_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     16
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_12_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     12
#elif ((CAN_RX_BUFFERS > 0) && (CAN_RX_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_1_OBJECT) && (CAN_FIFO_1_OBJECT_TYPE == 0) && (CAN_FIFO_1_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_2_OBJECT) && (CAN_FIFO_2_OBJECT_TYPE == 0) && (CAN_FIFO_2_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_3_OBJECT) && (CAN_FIFO_3_OBJECT_TYPE == 0) && (CAN_FIFO_3_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_4_OBJECT) && (CAN_FIFO_4_OBJECT_TYPE == 0) && (CAN_FIFO_4_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_5_OBJECT) && (CAN_FIFO_5_OBJECT_TYPE == 0) && (CAN_FIFO_5_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_6_OBJECT) && (CAN_FIFO_6_OBJECT_TYPE == 0) && (CAN_FIFO_6_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES)) || \
      (defined(CAN_USE_FIFO_7_OBJECT) && (CAN_FIFO_7_OBJECT_TYPE == 0) && (CAN_FIFO_7_OBJECT_MAX_PAYLOAD_SIZE == PAYLOAD_SIZE_8_BYTES))
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     8
#else
 #define CAN_ACTUAL_MAX_RX_PAYLOAD_SIZE     0
#endif

/*
   The following is used to determine the amount of RAM required to hold all
   the messages for all the Objects, TX , TX Queue and FIFO 1 to 7.  Which
   is then used to create a global array that the CAN peripheral will use to
   store received messages to and transmit messages from.
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

#if CAN_ENABLE_TX_EVENT_OBJECT == TRUE
 #define CAN_MESSAGE_RAM_BYTES   (((8 + (4 * CAN_TX_EVENT_TIME_STAMP)) * CAN_TX_EVENT_FIFO_SIZE) + \
                                  (CAN_TX_MESSAGE_SIZE * CAN_TX_BUFFERS) + \
                                  (CAN_RX_MESSAGE_SIZE * CAN_RX_BUFFERS) + \
                                  CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE)
#else
 #define CAN_MESSAGE_RAM_BYTES   ((CAN_TX_MESSAGE_SIZE * CAN_TX_BUFFERS) + \
                                  (CAN_RX_MESSAGE_SIZE * CAN_RX_BUFFERS) + \
                                  CAN_FIFO_1_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_2_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_3_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_4_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_5_OBJECT_MESSAGE_BUFFER_SIZE + CAN_FIFO_6_OBJECT_MESSAGE_BUFFER_SIZE + \
                                  CAN_FIFO_7_OBJECT_MESSAGE_BUFFER_SIZE)
#endif

/////////////////////////////// Global Variables //////////////////////////////

CAN_OP_MODE g_CANOpmode = CAN_OP_CONFIG;
uint32_t g_CANTXSequence = 0;

uint8_t g_CANMessages[CAN_MESSAGE_RAM_BYTES];
#locate g_CANMessages=CAN_MESSAGE_START_ADDR

////////////////////////////////// Prototypes /////////////////////////////////

void can_set_tx_obj_header(CAN_TX_MESSAGE_OBJECT *Ptr, CAN_TX_HEADER *Header);
uint8_t can_set_tx_obj_data(uint8_t *wData, uint8_t *rData, uint8_t Length);
void can_get_rx_header(CAN_RX_HEADER *Header, CAN_RX_MESSAGE_OBJECT *Ptr);
DATA_LENGTH can_get_dlength_from_bytes(uint8_t dBytes);
uint8_t can_get_bytes_from_dlength(DATA_LENGTH dLength);

////////////////////////////////// Functions //////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// can_init() 
//
// Initializes the CAN to the default settings that were defined before this
// driver was included or the default defines set by the can-dspic33_fd.h file.
//
// Parameters:
//    Mode - Optional parameter specifying the operational mode to put the CAN FD
//           peripheral in before exiting, defaults to CAN_OP_NORMAL_FD mode if
//           CAN_OPERATION_MODE is defined to 0 or CAN_OP_NORMAL_CAN_2_0 if
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
   
   CAN_C1CON.CON = TRUE;    //enabled CAN Peripheral
   
   can_set_mode(CAN_OP_CONFIG);
   
   //Setup CAN Message Memory Base Address
   CAN_C1FIFOBA = &g_CANMessages;
   
   //Setup CAN Clock Generator
  #if CAN_CLOCK_SOURCE == 0
   ((CAN_CLKCON)Config1).CLKEN = TRUE;
   ((CAN_CLKCON)Config1).CLKSEL = CAN_CLOCK_GEN_SOURCE;
   ((CAN_CLKCON)Config1).CLKDIV = (CAN_CLOCK_GEN_DIVISOR - 1);
   
   CAN_CANCLKCON = (CAN_CLKCON)Config1;
  #else
   CAN_CANCLKCON.CLKEN = FALSE;
  #endif
   
   //Setup CAN Bit rates
   can_set_baud(CAN_CLOCK_SPEED, CAN_NOMINAL_BAUD_RATE, CAN_DATA_BAUD_RATE);
   
   //Setup CAN Transmitter Delay Compensation Register
   ((CiTDC)Config1).TDCV = CAN_TDC_VALUE;
   ((CiTDC)Config1).TDCO = CAN_TDC_OFFSET;
   ((CiTDC)Config1).TDCMOD = CAN_TDC_MODE;
   ((CiTDC)Config1).SID11 = CAN_SID11;
   ((CiTDC)Config1).EDGFLT = CAN_EDGE_FLT;
   
   CAN_C1TDC = (CiTDC)Config1;
   
   //Setup CAN Time Stamp Control Register
   ((CiTSCON)Config1).TBCPRE = (CAN_TIME_BASE_COUNTER_PRESCALAR + 1);
   ((CiTSCON)Config1).TBCEN = CAN_TIME_BASE_COUNTER_ENABLE;
   ((CiTSCON)Config1).TSEOF = CAN_TIME_STAMP_EOF;
   ((CiTSCON)Config1).TSRES = CAN_TIME_STAMP_RES;
   
   CAN_C1TSCON = (CiTSCON)Config1;
   
   //Setup Can Control Register
   (CiCON)Config1 = CAN_C1CON;
   
   ((CiCON)Config1).DNCNT = CAN_DEVICE_NET_FILTER_BIT_NUMBER;
   ((CiCON)Config1).ISOCRC = CAN_ENABLE_ISO_CRC;
   ((CiCON)Config1).PXE = CAN_DISABLE_EXCEPTION_EVENT;
   ((CiCON)Config1).CLKSEL = CAN_CLOCK_SOURCE;
   ((CiCON)Config1).WAKFIL = CAN_ENABLE_WAKE_FILTER;
   ((CiCON)Config1).WFT = CAN_WAKE_FILTER_TIME;
   ((CiCON)Config1).BRS = CAN_DISABLE_BIT_RATE_SWITCHING;
   ((CiCON)Config1).SIDL = CAN_STOP_IN_IDLE;
   ((CiCON)Config1).RTXAT = CAN_RESTRICT_RETRANSMISSION_ATTEMPTS;
   ((CiCON)Config1).ESIGM = CAN_TRANSMIT_ESI_MODE;
   ((CiCON)Config1).SERR2LOM = CAN_TRANSITION_TO_LISTEN;
   ((CiCON)Config1).STEF = CAN_ENABLE_TX_EVENT_OBJECT;
  #if CAN_TX_BUFFERS > 0
   ((CiCON)Config1).TXQ = TRUE;
  #else
   ((CiCON)Config1).TXQ = FALSE;
  #endif
   ((CiCON)Config1).TXBWS = CAN_TRANSMISSION_DELAY;
   
   CAN_C1CON = (CiCON)Config1;
   
   //Setup CAN TX Event
   Config1 = 0;
   
  #if CAN_ENABLE_TX_EVENT_OBJECT == TRUE
   ((CiTEFCON)Config1).TEFTSEN = CAN_TX_EVENT_TIME_STAMP;
   ((CiTEFCON)Config1).FSIZE = (CAN_TX_EVENT_FIFO_SIZE - 1);
  #endif
   
   CAN_C1TEFCON = (CiTEFCON)Config1;
   
   //Setup TX Queue
   Config1 = 0;
   
  #if CAN_TX_BUFFERS > 0
   ((CiTXQCON)Config1).TXEN = 1;
   ((CiTXQCON)Config1).TXPRI = CAN_TX_PRIORITY;
   ((CiTXQCON)Config1).TXAT = CAN_TX_RETRANSMISSION_ATTEMPTS;
   ((CiTXQCON)Config1).FSIZE = (CAN_TX_BUFFERS - 1);
   ((CiTXQCON)Config1).PLSIZE = CAN_TX_MAX_PAYLOAD_SIZE;
  #endif
   
   CAN_C1TXQCON = (CiTXQCON)Config1;
   
   //Setup FIFO 1 Object
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
   
   CAN_C1FIFOCON1 = (CiFIFOCON)Config1;
   
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
   
   CAN_C1FIFOCON2 = (CiFIFOCON)Config1;
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
   
   CAN_C1FIFOCON3 = (CiFIFOCON)Config1;
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
   
   CAN_C1FIFOCON4 = (CiFIFOCON)Config1;
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
   
   CAN_C1FIFOCON5 = (CiFIFOCON)Config1;
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
   
   CAN_C1FIFOCON6 = (CiFIFOCON)Config1;
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
   
   CAN_C1FIFOCON7 = (CiFIFOCON)Config1;
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
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
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
      
      if(tBits < 50)
      {
         if((Clock / (tBits * BRP)) != Data)
            continue;
      }
      
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
   
   CAN_C1NBTCFG = nBaud;
   CAN_C1DBTCFG = dBaud;
   
   can_set_mode(g_CANOpmode, FALSE);
   
   return(CAN_EC_OK);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_mode()
//
// Sets the operational mode of the CAN FD peripheral.
//
// Parameters:
//    Mode - the CAN_OP_MODE to change to, can one of the following:
//             CAN_OP_NORMAL_FD
//             CAN_OP_DISABLE
//             CAN_OP_LOOPBACK_INTERNAL
//             CAN_OP_LISTEN
//             CAN_OP_CONFIG
//             CAN_OP_LOOPBACK_EXTERNAL
//             CAN_OP_NORMAL_CAN_2_0
//             CAN_OP_RESTRICTED_OPERATION
//
//    Update - optional specifies whether to update the global operation mode
//             variable, TRUE update or FALSE don't update.  Default is TRUE if
//             not specified.  This is useful for allowing the option to put it
//             in a different operating mode for a little while and then
//             returning it back to the previous operating mode.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE)
{
   CiCON rCon;
   
   rCon = CAN_C1CON;
   
   if(rCon.OPMOD != Mode)
   {
      rCon.REQOP = Mode;
      CAN_C1CON = rCon;
      
      while(CAN_C1CON.OPMOD != Mode);
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
   FLTCON *pFilter;
   CiFLTOBJ *pId;
   CiMASK *pMask;
   int1 Enabled;
   
   pFilter = ((uint8_t *)&CAN_C1FLTCON0 + Filter);
   pId = ((uint8_t *)&CAN_C1FLTOBJ0 + (Filter * 8));
   pMask = ((uint8_t *)&CAN_C1MASK0 + (Filter * 8));
   
   Enabled = pFilter->FLTEN;
   
   //The Filter's ID and Mask can only be changed when Filter is disabled
   if(Enabled)
      pFilter->FLTEN = FALSE;
   
   //Clear Filter's ID and Mask
   memset(pId, 0x00, sizeof(uint32_t));
   memset(pMask, 0x00, sizeof(uint32_t));
   
   //Write Filter's ID and Mask
   switch(Type)
   {
      case CAN_FILTER_TYPE_SID_ONLY:
         #if (CAN_DEVICE_NET_FILTER_BIT_NUMBER > 0) && ((CAN_OPERATION_MODE == 1) || ((CAN_OPERATION_MODE == 0) && (CAN_SID11 == 0)))
            pId->EID = Id & 0x1FFFFFFF;
            pMask->MEID = Mask & 0x1FFFFFFF;
         #else
            pId->SID0_7 = make8(Id, 0);
            pId->SID8_10 = make8(Id, 1);
            
            pMask->MSID0_7 = make8(Mask, 0);
            pMask->MSID8_10 = make8(Mask, 1);
            
          #if (CAN_OPERATION_MODE == 0) && (CAN_SID11 == 1) //CAN FD and set to use 12 bit SID
            pId->SID11 = bit_test(make8(Id, 1), 3);
            pMask->MSID11 = bit_test(make8(Mask, 1), 3);
            
           #if CAN_DEVICE_NET_FILTER_BIT_NUMBER > 0
            pId->EID0_4 = ((make8(Id, 2) << 5) | (make8(Id, 1) >> 4));
            pMask->MEID0_4 = ((mak8(Mask, 2) << 5) | (make8(Mask, 1) >> 4));
            
            pId->EID5_12 = ((make8(Id, 2) >> 1) | (make8(Id, 3) << 7));
            pMask->MEID5_12 = ((make8(Mask, 2) >> 1) | (make8(Mask, 3) << 7));
            
            pId->EID13_17 = (make8(Id, 3) >> 1);
            pId->MEID13_17 = (make8(Mask, 3) >> 1);
           #endif
          #endif
         #endif
         
         pId->EXIDE = 0;
         pMask->MIDE = 1;
         break;
      case CAN_FILTER_TYPE_EID_ONLY:
      case CAN_FILTER_TYPE_EITHER:
         pId->EID0_4 = make8(Id, 0);
         pMask->MEID0_4 = make8(Mask, 0);
         
         pId->EID5_12 = ((make8(Id, 0) >> 5) | (make8(Id, 1) << 3));
         pMask->MEID5_12 = ((make8(Mask, 0) >> 5) | (make8(Mask, 1) << 3));
         
         pId->EID13_17 = ((make8(Id, 1) >> 5) | ((make8(Id, 2) & 0x03) << 3));
         pMask->MEID13_17 = ((make8(Mask, 1) >> 5) | ((make8(Mask, 2) & 0x03) << 3));
         
         pId->SID0_7 = ((make8(Id, 2) >> 2) | (make8(Id, 3) << 6));
         pMask->MSID0_7 = ((make8(Mask, 2) >> 2) | (make8(Mask, 3) << 6));
         
         pId->SID8_10 = (make8(Id, 3) >> 2);
         pMask->MSID8_10 = (make8(Mask, 3) >> 2);
         
         if(Type == CAN_FILTER_TYPE_EID_ONLY)
         {
            pId->EXIDE = 1;
            pMask->MIDE = 1;
         }
         else
            pMask->MIDE = 0;
         break;
   }
   
   if(Enabled)
      pFilter->FLTEN = TRUE;
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
   FLTCON *pFilter;
   
   pFilter = ((uint8_t *)&CAN_C1FLTCON0 + Filter);
   
   pFilter->FLTEN = FALSE;    //can only change Object when filter is disabled
   
   pFilter->BP = Object;
   pFilter->FLTEN = TRUE;
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
   FLTCON *pFilter;
   
   pFilter = ((uint8_t *)&CAN_C1FLTCON0 + Filter);
   
   pFilter->FLTEN = FALSE;
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
//             CAN_TX_HEADER type in can-dspic33_fd.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Object - the CAN TX Object to load the message into, optional defaults
//             to CAN_OBJECT_TXQ if not specified.  By default this driver only
//             sets up the CAN to use the TX Queue Object to send messages.
//
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object=CAN_OBJECT_TXQ)
{  
  #ifdef CAN_HAS_TX_OBJECT
   CiFIFOCON *pFIFOCon;
   CiFIFOSTA *pFIFOSta;
   CiFIFOUA *pFIFOUa;
   uint16_t *Address;
   struct
   {
      CAN_TX_MESSAGE_OBJECT Header;
      uint8_t Data[CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE];
   } TxData;
   uint16_t aCount; 
   uint16_t *tPtr;
   
   if(Object == CAN_OBJECT_TXQ)
   {
      pFIFOCon = &CAN_C1TXQCON;
      pFIFOSta = &CAN_C1TXQSTA;
      pFIFOUa = &CAN_C1TXQUA;
   }
   else
   {
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + ((Object - 1) * 12));
      pFIFOSta = ((uint8_t *)pFIFOCon + 4);
      pFIFOUa = ((uint8_t *)pFIFOSta + 4);
   }
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      //Check TXEN bit in C1FIFOCONm register to determine if Object is a TX Object
      if(pFIFOCon->TXEN == 0)
         return(CAN_EC_OBJ_NOT_TX);
   }
   
   //Check TFNRFNIF bit in C1FIFOSTAm register to determine if FIFO is full
   if(pFIFOSta->TFNRFNIF == 0)
      return(CAN_EC_OBJ_TX_FULL);
   
   Address = *pFIFOUa;
   
   can_set_tx_obj_header(&TxData.Header, Header);
   aCount = can_set_tx_obj_data(TxData.Data, Data, Header->Length);
   
   //Copy Message to CAN TX Object's FIFO buffer
   memcpy(Address, &TxData, (uint16_t)aCount);
   
   //Set UINC and TXREG bits in C1FIFOCONm register to increment FIFO pointer and Request message transmission
   tPtr = pFIFOCon;
   *tPtr |= 0x0300;
   
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
//             type in can-dspic33_fd.h for more info.
//
//    Data - pointer to uint8_t array to return data portion of message to.
//
//    Object - the CAN RX Object to get the message from, optional defaults
//             to CAN_OBJECT_FIFO_1 if not specified.  By default this driver
//             only sets up the CAN to use the FIFO 1 Object to receive
//             messages.
//
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object=CAN_OBJECT_FIFO_1)
{
  #ifdef CAN_HAS_RX_OBJECT
   CiFIFOCON *pFIFOCon;
   CiFIFOSTA *pFIFOSta;
   CiFIFOUA *pFIFOUa;
   uint16_t *Address;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Object -= 1;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object * 12));
      pFIFOSta = ((uint8_t *)pFIFOCon + 4);
      pFIFOUa = ((uint8_t *)pFIFOSta + 4);
      
      //Check TXEN bit in C1FIFOCONm register to determine if Object is a RX Object
      if(pFIFOCon->TXEN == 1)
         return(CAN_EC_OBJ_NOT_RX);
      
      //Check TFNRFNIF bit in C1FIFOSTAm register to determine if FIFO has any messages to read
      if(pFIFOSta->TFNRFNIF == 0)
         return(CAN_EC_OBJ_RX_EMPTY);
      
      Address = *pFIFOUa;
      
      //Read Message Header from memory
      can_get_rx_header(Header, (CAN_RX_MESSAGE_OBJECT *)Address);
      
      (uint8_t *)Address += 8;
      
      //Check RXTSEN bit in C1FIFOCONm registers to determine if Object uses Time Stamp
      if(pFIFOCon->RXTSEN == TRUE)
      {
         Header->HasTS = TRUE;
         
         //Read Messages Time Stamp from memory
         memcpy(&Header->TimeStamp, Address, sizeof(uint32_t));
         
         (uint8_t *)Address += 4;
      }
      else
         Header->HasTS = FALSE;
      
      //Read Message Data from memory
      if(Header->Length > 0)
         memcpy(Data, Address, Header->Length);
      
      //Get receive FIFO overflow interrupt flag and clear if set 
      Header->err_ovfl = pFIFOSta->RXOVIF;
      
      if(pFIFOSta->RXOVIF)
         pFIFOSta->RXOVIF = 0;
      
      //Set UINC bit in C1FIFOCONm register to increment FIFO pointer
      pFIFOCon->UINC = TRUE;
      
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
   CiFIFOCON *pFIFOCon;
   CiFIFOSTA *pFIFOSta;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Object--;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object * 12));
      pFIFOSta = ((uint8_t *)pFIFOCon + 4);
      
      //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a RX Object
      if(pFIFOCon->TXEN == 0)
      {
         //Check the TFNRFNIF bit in C1FIFOSTAm register to determine if Object has any messages
         Result = pFIFOSta->TFNRFNIF;
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
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_discard(CAN_OBJECT Object=CAN_OBJECT_FIFO_1)
{
  #ifdef CAN_HAS_RX_OBJECT
   can_ec_t Result = CAN_EC_OK;
   
   CiFIFOCON *pFIFOCon;
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      Object--;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object * 12));
      
      //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a RX Object
      if(pFIFOCon->TXEN == 0)
      {
         pFIFOCon->FRESET = 1;
         
         while(pFIFOCon->FRESET == 1);
      }
      else
         Result = CAN_EC_OBJ_NOT_RX;
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
// Used to determine if specified TX Object can accept at least one more 
// message.
//
// Parameters:
//    Object - the CAN TX Object to check, optional defaults to 
//             CAN_OBJECT_TXQ if not specified.  By default this driver only
//             sets up the CAN to use the TX Queue Object to transmit messages.
//
// Returns:
//    TRUE - if Object can accept at least one more message.
//    FALSE - if Object is full.
///////////////////////////////////////////////////////////////////////////////
int1 can_tbe(CAN_OBJECT Object=CAN_OBJECT_TXQ)
{
   int1 Result = FALSE;
   
  #ifdef CAN_HAS_TX_OBJECT
   CiFIFOCON *pFIFOCon;
   CiFIFOSTA *pFIFOSta;
   
   if(Object > CAN_OBJECT_TXQ)
   {
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + ((Object - 1) * 12));
      pFIFOSta = ((uint8_t *)pFIFOCon + 4);
      
      //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a TX Object
      if(pFIFOCon->TXEN == 0)
         return(Result);
   }
   else
      pFIFOSta = &CAN_C1TXQSTA;
   
   //Check the TFNRFNIF bit in the C1FIFOSTAm register to determine if Object FIFO is full
   Result = pFIFOSta->TFNRFNIF;
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_tx_is_empty()
//
// Used to determine if the specified TX Object is empty.
//
// Parameters:
//    Object - the CAN TX Object to check, optional defaults to 
//             CAN_OBJECT_TXQ if not specified.  By default this driver only
//             sets up the CAN to use the TX Queue Object to transmit messages.
//
// Returns:
//    TRUE - if Object is empty.
//    FALSE - if Object isn't empty, has at least one more message to send.
///////////////////////////////////////////////////////////////////////////////
int1 can_tx_is_empty(CAN_OBJECT Object=CAN_OBJECT_TXQ)
{
   int1 Result = FALSE;
   
  #ifdef CAN_HAS_TX_OBJECT
   CiFIFOCON *pFIFOCon;
   CiFIFOSTA *pFIFOSta;
   
   if(Object > CAN_OBJECT_TXQ)
   {
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + ((Object - 1) * 12));
      pFIFOSta = ((uint8_t *)pFIFOCon + 4);
      
      //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a TX Object
      if(pFIFOCon->TXEN == 0)
         return(Result);
   }
   else
      pFIFOSta = &CAN_C1TXQSTA;
   
   //Check the TFERFFIF bit in the C1FIFOSTAm register to determine if Object FIFO is empty
   Result = pFIFOSta->TFERFFIF;
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
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_abort(CAN_OBJECT Object=CAN_ABORT_ALL)
{
  #ifdef CAN_HAS_TX_OBJECT
   can_ec_t Result = CAN_EC_OK;
   CiFIFOCON *pFIFOCon;
   
   if(Object == CAN_ABORT_ALL)
   {
      CAN_C1CON.ABAT = TRUE;
      
      while(CAN_C1CON.ABAT);
   }
   else
   {
      if(Object == CAN_OBJECT_TXQ)
      {
         if(CAN_C1CON.TXQ)
            pFIFOCon = &CAN_C1TXQCON;
         else
            Result = CAN_EC_OBJ_NOT_TX;
      }
      else
      {
         pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + ((Object - 1) * 12));
         
         if(pFIFOCon->TXEN == FALSE)
            Result = CAN_EC_OBJ_NOT_TX;
      }
      
      if(Result == CAN_EC_OK)
         pFIFOCon->TXREQ = 0;
   }
   
   return(Result);
  #else
   return(CAN_EC_OBJ_TX_NONE);
  #endif
}

// CAN FD Interrupts Functions

///////////////////////////////////////////////////////////////////////////////
// can_enable_interrupts()
//
// Used to enable one or more of the CAN Interrupts.  The CANFD peripheral has
// multiple layers of interrupts that fall into the following three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This functions enables interrupts at
// the Main Interrupts level.  Depending on the interrupt being enabled it may
// require enabling a FIFO Individual level interrupt, a Main level interrupt
// and a peripheral interrupt for the interrupt to occur.  For example for the
// C1RX interrupt to occur it requires at the Receive FIFO not empty FIFO
// individual level interrupt, the Receive Object main level interrupt and the
// C1RX peripheral interrupt be enabled to function:
//    can_enable_interrupts(CAN_INTERRUPT_RX);
//    can_enable_fifo_interrupts(CAN_OBJECT_FIFO_1, CAN_FIFO_INTERRUPT_RXNE);
//    can_enable_interrupts(INT_C1RX);
//
// Parameters:
//    Setting - CAN_INTERRUPT type specifying the interrupt to enable, multiple
//              interrupts can be enabled by or'ing the interrupts together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_interrupts(CAN_INTERRUPT Setting)
{
   *((uint16_t *)&CAN_C1INT + 1) |= Setting;
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_interrupts()
//
// Used to disable one or more of the CAN Interrupts.  The CANFD peripheral has
// multiple layers of interrupts that fall into the following three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This functions disables interrupts 
// at the Main Interrupts level.
//
// Parameter:
//    Setting - CAN_INTERRUPT type specifying the interrupt to disable, multiple
//              interrupts can be  disabled by or'ing the interrupts together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_interrupts(CAN_INTERRUPT Setting)
{
   Setting = ~Setting;

   *((uint16_t *)&CAN_C1INT + 1) &= Setting;
}

///////////////////////////////////////////////////////////////////////////////
// can_interrupts_active()
//
// Used to determine which CAN Interrupts are active.  The CANFD peripheral
// has multiple layers of interrupts that fall into the following three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This function determine which Main
// level interrupts are active.  
//
// Parameters - None.
//
// Returns - CAN_INTERRUPT type.  If an interrupt is active the corresponding
//           bit will be set, see CAN_INTERRUPT type in can-dspic33_fd.h to
//           determine which bits correspond to which interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_INTERRUPT can_interrupts_active(void)
{
   return(*((uint16_t *)&CAN_C1INT) & 0xFC1F);
}

///////////////////////////////////////////////////////////////////////////////
// can_interrupt_active()
//
// Used to determine if specified CAN Interrupt is active.  The CANFD 
// peripheral has multiple layers of interrupts that fall into the following
// three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This function determines if the
// specified Main level interrupt is active.
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
   if(*((uint16_t *)&CAN_C1INT) & Interrupt)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_clear_interrupt()
//
// Used to clear specified CAN Interrupt flag.  The CAN FD peripheral has
// multiple layers of interrupts that fall into the following three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This function clears the specified
// Main level interrupt flag.
//
// Parameters:
//    Interrupt - the CAN_INTERRUPT type to clear the flag for.  Multiple 
//                interrupts can be cleared by or'ing the interrupts together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_clear_interrupt(CAN_INTERRUPT Interrupt)
{
   Interrupt = ~Interrupt;
   
   *((uint16_t *)&CAN_C1INT) &= Interrupt;
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_fifo_interrupts()
//
// Used to enable one or more of the CAN FIFO Interrupts.  The CANFD peripheral
// has multiple layers of interrupts that fall into the following three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This functions enables interrupts at
// the FIFO Individual Interrupts level.  Depending on the interrupt being
// enabled it may require enabling a FIFO Individual level interrupt, a Main
// level interrupt and a peripheral interrupt for the interrupt to occur.  For
// example for the C1RX interrupt to occur it requires at the Receive FIFO not
// empty FIFO individual level interrupt, the Receive Object main level
// interrupt and the C1RX peripheral interrupt be enabled to function:
//    can_enable_interrupts(CAN_INTERRUPT_RX);
//    can_enable_fifo_interrupts(CAN_OBJECT_FIFO_1, CAN_FIFO_INTERRUPT_RXNE);
//    can_enable_interrupts(INT_C1RX);
//
// Parameters:
//    Object - the CAN object to enable the CAN FIFO interrupt for.
//
//    Setting - CAN_FIFO_INTERRUPT type specifying the interrupt to enable,
//              multiple interrupts can be enabled by or'ing the interrupts
//              together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_fifo_interrupts(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Setting)
{
   CiFIFOCON *pFIFOCon;
   uint16_t Temp;
   
   if(Object == CAN_OBJECT_TXQ)
      pFIFOCon = &CAN_C1TXQCON;
   else
   {
      Object--;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object * 12));
   }
   
   Temp = Setting;
   *((uint16_t *)pFIFOCon) |= Temp;
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_fifo_interrupts()
//
// Used to disable one or more of the CAN FIFO Interrupts.  The CANFD
// peripheral has multiple layers of interrupts that fall into the following
// three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This functions disables interrupts
// at the FIFO Individual Interrupts level.
//
// Parameters:
//    Object - the CAN object to disable the CAN FIFO interrupt for.
//
//    Setting - CAN_FIFO_INTERRUPT type specifying the interrupt to disable,
//              multiple interrupts can be disabled by or'ing the interrupts
//              together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_fifo_interrupts(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Setting)
{
   CiFIFOCON *pFIFOCon;
   uint16_t Temp;
   
   if(Object == CAN_OBJECT_TXQ)
      pFIFOCon = &CAN_C1TXQCON;
   else
   {
      Object--;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object * 12));
   }
   
   Temp = ~Setting;
   *((uint16_t *)pFIFOCon) &= Temp;
}

///////////////////////////////////////////////////////////////////////////////
// can_fifo_interrupts_active()
//
// Used to determine which CAN FIFO Interrupts are active.  The CANFD
// peripheral has multiple layers of interrupts that fall into the following
// three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This function determines which
// interrupts are active at the FIFO Individual Interrupt level.
//
// Parameters:
//    Object - the CAN object to determine which CAN FIFO interrupts are 
//             active for.
//
// Returns - CAN_FIFO_INTERRUPT type.  If an interrupt is active the
//           corresponding bit will be set, see CAN_FIFO_INTERRUPT type in
//           can-dspic33_fd.h to determine which bits correspond to which
//           interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_FIFO_INTERRUPT can_fifo_interrupts_active(CAN_OBJECT Object)
{
   CiFIFOSTA *pFIFOSta;
   uint16_t Temp;
   
   if(Object == CAN_OBJECT_TXQ)
      pFIFOSta = &CAN_C1TXQSTA;
   else
      pFIFOSta = ((uint8_t *)&CAN_C1FIFOSTA1 + ((Object - 1) * 12));
   
   Temp = *(uint16_t *)pFIFOSta;
   
   if(Object == CAN_OBJECT_TXQ)
      return(Temp & 0x0015);
   else
      return(Temp & 0x001F);
}

///////////////////////////////////////////////////////////////////////////////
// can_fifo_interrupt_active()
//
// Used to determine if the specified CAN FIFO Interrupts is active.  The CANFD
// peripheral has multiple layers of interrupts that fall into the following
// three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This function determines if the
// specified interrupt is active at the FIFO Individual Interrupt level.
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
   CiFIFOSTA *pFIFOSta;
   uint16_t Temp;
   
   if(Object == CAN_OBJECT_TXQ)
      pFIFOSta = &CAN_C1TXQSTA;
   else
   {
      Object--;
      
      pFIFOSta = ((uint8_t *)&CAN_C1FIFOSTA1 + (Object * 12));
   }
   
   Temp = Interrupt;
   
   return(*(uint16_t *)pFIFOSta & Temp);
}

///////////////////////////////////////////////////////////////////////////////
// can_clear_fifo_interrupt()
//
// Used to clear specified CAN FIFO Interrupt flag.  The CAN FD peripheral has
// multiple layers of interrupts that fall into the following three levels:
//    * FIFO Individual Interrupts
//    * FIFO Combined Interrupts
//    * Main Interrupts
//
// See Figure 12-1 in CAN Flexible Data-Rate (FD) Protocol Module from the
// dsPIC33/PIC24 Family Reference Manual.  This function clears the specified
// interrupt at the FIFO Individual Interrupts level.
//
// Parameters:
//    Object - the CAN object to clear the specified CAN FIFO interrupt flag
//             for.
//
//    Interrupt - CAN_FIFO_INTERRUPT type specifying the interrupt flag to
//                clear.  Multiple interrupts can be cleared by or'ing the
//                interrupts together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_clear_fifo_interrupt(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Interrupt)
{
   CiFIFOSTA *pFIFOSta;
   uint16_t Temp;
   
   if(Object == CAN_OBJECT_TXQ)
      pFIFOSta = &CAN_C1TXQSTA;
   else
   {
      Object--;
      
      pFIFOSta = ((uint8_t *)&CAN_C1FIFOSTA1 + (Object * 12));
   }
   
   Temp = ~Interrupt;
   
   *(uint16_t *)pFIFOSta &= Temp;
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_rtr()
//
// Used to enable RTR messages on one of the TX Objects.
//
// Parameters:
//    Object - the CAN TX Object to enable RTR messages for.
//
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_enable_rtr(CAN_OBJECT Object)
{
  #ifdef CAN_HAS_TX_OBJECT
   can_ec_t Result;
   CiFIFOCON *pFIFOCon;
   
   if(Object > CAN_OBJECT_TXQ)
   {
      Object--;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object* 12));
      
      //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a TX Object
      if(pFIFOCon->TXEN == 1)
      {
         pFIFOCon->RTREN = 1;
         
         Result = CAN_EC_OK;
      }
      else
         Result = CAN_EC_OBJ_NOT_TX;
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
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_disable_rtr(CAN_OBJECT Object)
{
  #ifdef CAN_HAS_TX_OBJECT
   can_ec_t Result;
   CiFIFOCON *pFIFOCon;
   
   if(Object > CAN_OBJECT_TXQ)
   {
      Object--;
      
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + (Object* 12));
      
      //Check the TXEN bit in the C1FIFOCONm register to determine if Object is a TX Object
      if(pFIFOCon->TXEN == 1)
      {
         pFIFOCon->RTREN = 0;
         
         Result = CAN_EC_OK;
      }
      else
         Result = CAN_EC_OBJ_NOT_TX;
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
//             CAN_TX_HEADER type in can-dspic33_fd.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Object - the CAN TX Object to load the message into.  Only FIFO TX
//             objects can be use to responded to remote transmission requests.
//
// Returns - can_ec_t error code, see can-dspic33_fd.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_load_rtr(CAN_TX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object)
{
  #ifdef CAN_HAS_TX_OBJECT
   CiFIFOCON *pFIFOCon;
   CiFIFOSTA *pFIFOSta;
   CiFIFOUA *pFIFOUa;
   uint16_t *Address;
   struct
   {
      CAN_TX_MESSAGE_OBJECT Header;
      uint8_t Data[CAN_ACTUAL_MAX_TX_PAYLOAD_SIZE];
   } TxData;
   uint16_t aCount; 
   
   if(Object >= CAN_OBJECT_FIFO_1)
   {
      pFIFOCon = ((uint8_t *)&CAN_C1FIFOCON1 + ((Object - 1) * 12));
      pFIFOSta = ((uint8_t *)pFIFOCon + 4);
      pFIFOUa = ((uint8_t *)pFIFOSta + 4);
   
      //Check TXEN bit in C1FIFOCONm register to determine if Object is a TX Object
      if(pFIFOCon->TXEN == 0)
         return(CAN_EC_OBJ_NOT_TX);
   
      //Check TFNRFNIF bit in C1FIFOSTAm register to determine if FIFO is full
      if(pFIFOSta->TFNRFNIF == 0)
         return(CAN_EC_OBJ_TX_FULL);
   
      Address = *pFIFOUa;
      
      can_set_tx_obj_header(&TxData.Header, Header);
      aCount = can_set_tx_obj_data(TxData.Data, Data, Header->Length);
   
      //Copy Message to CAN TX Object's FIFO buffer
      memcpy(Address, &TxData, (uint16_t)aCount);
   
      //Set UINC bit in C1FIFOCONm register to increment FIFO pointer
      pFIFOCon->UINC = 1;
      
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
   Ptr->SEQ = g_CANTXSequence;
   Ptr->SEQ7_22 = (g_CANTXSequence >> 7);
   
   g_CANTXSequence++;
   g_CANTXSequence &= 0x7FFFFF;
}

uint8_t can_set_tx_obj_data(uint8_t *wData, uint8_t *rData, uint8_t Length)
{
   uint8_t Count;
   
   Count = 8;
   
   memcpy(wData, rData, Length);
   
   while((Length % 4) != 0)
      wData[Length++] = 0;
   
   Count += Length;
   
   return(Count);
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


#endif //__CAN_DSPIC33_FD_C__
