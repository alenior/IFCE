///////////////////////////////////////////////////////////////////////////////
////                           can-pic18f_ecan.c                           ////
////                                                                       ////
////  Driver for ECAN peripheral for Microchip's PIC18 devices with an     ////
////  ECAN peripheral.                                                     ////
////                                                                       ////
////  See can-pic18f_ecan.h for API.                                       ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////                                                                       ////
////  Version History:                                                     ////
////                                                                       ////
////     01/24/2020 - Fixed an issue with can_getd() function returning    ////
////                  incorrect data for PIC18F26K83 family.               ////
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

#include <can-pic18f_ecan.h>

// CAN Buffer Settings

#ifndef CAN_TX_BUFFERS
 #define CAN_TX_BUFFERS       3     //3-9
#endif

#if (CAN_TX_BUFFERS < 3) || (CAN_TX_BUFFERS > 9)
 #error CAN TX Buffers must be set to 3 to 9
#endif

#define CAN_RX_BUFFERS        (2 + (6 - (CAN_TX_BUFFERS - 3)))

#ifndef CAN_USE_RX_FIFO
 #define CAN_USE_RX_FIFO      TRUE     //TRUE - use Enhanced FIFO mode (Mode 2), FALSE - use Enhanced Legacy Mode (Mode 1)
#endif

#ifndef CAN_FIFO_INT
 #define CAN_FIFO_INT         1        //0 - FIFO interrupt will occur when four receive buffers remain, 1 - FIFO will interrupt when one receive buffer remains
#endif

// CAN Baud Rate Settings
#define CAN_CLOCK_SPEED       getenv("CLOCK")

#ifndef CAN_BAUD_RATE
 #define CAN_BAUD_RATE        125000
#endif

#if (CAN_CLOCK_SPEED % CAN_BAUD_RATE) != 0
 #error CAN Clock Speed not evenly divisible by CAN Baud Rate
#endif

#ifndef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT   875      //in tenth of a percent, e.g. 875 = 87.5% of bit time
#endif

#if CAN_DEFAULT_SAMPLE_POINT < 500
 #undef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT   500
 #warning CAN Sample point changed to 50% of bit time
#endif

#ifndef CAN_BRG_SAM
 #define CAN_BRG_SAM                0        //sample of the can bus line (def: bus line is sampled 1 times prior to sample point)
#endif

#ifndef CAN_BRG_WAKEUP
 #define CAN_BRG_WAKEUP             FALSE    //FALSE-Disable CAN bus activity wake-up feature, TRUE-Enable CAN bus activity wake-up feature
#endif

#ifndef CAN_BRG_WAKE_FILTER
 #define CAN_BRG_WAKE_FILTER        FALSE    //selects can bus line filter for wake up bit
#endif

#ifndef CAN_ENABLE_DRIVE_HIGH
 #define CAN_ENABLE_DRIVE_HIGH      FALSE
#endif

#ifndef CAN_ENABLE_CAN_CAPTURE
 #define CAN_ENABLE_CAN_CAPTURE     FALSE
#endif

#ifndef CAN_ENABLE_CANTX2
   #define CAN_ENABLE_CANTX2        FALSE
#endif

#ifndef CAN_CANTX2_SOURCE
   #define CAN_CANTX2_SOURCE        0     // 0 source is invert of CANTX1, 1 source is CAN Clock
#endif

#if ((getenv("DEVICE") == "PIC18F25K83") || (getenv("DEVICE") == "PIC18LF25K83") || \
     (getenv("DEVICE") == "PIC18F26K83") || (getenv("DEVICE") == "PIC18LF26K83"))
 #define IS_ENH18
 
 #ifndef CAN_RX_PIN
  #warning CAN RX pin not defined set to default pin, B3
 
  #define CAN_RX_PIN    PIN_B3
 #endif
 
 #pin_select CANRX = CAN_RX_PIN
 
 #ifndef CAN_TX_PIN
  #warning CAN TX pin not defined set to default pin, B2
  
  #define CAN_TX_PIN    PIN_B2
 #endif
 
 #pin_select CANTX0 = CAN_TX_PIN
#endif

///////////////////////////// CAN FILTER Settings /////////////////////////////

/*
   If no CAN RX Filters are going to be used then nothing needs to be done, by
   default this driver will initialize the CAN controller to use no filter.
   
   If CAN RX Filters are going to be used, the can_init() function can be used
   to initialize them by making the following defines before this driver is
   included:
   
   #define CAN_USE_FILTERS                TRUE
   
   #define CAN_USER_FILT_x                aa    // x being the filter to enable (0-15), and aa being the Id to filter for.
   #define CAN_USER_FILT_x_TYPE           bb    // x being the filter to setup (0-15), and bb being the filter type:
                                                //    CAN_FILTER_TYPE_SID - filter Id is a SID Id, only the SID bit of filter are written the EID bits are set to 0.
                                                //    CAN_FILTER_TYPE_EID - filter Id is an EID Id, the SID and EID bits of filter are written.
   #define CAN_USER_FILT_x_BUFFER         cc    // x being the filter to setup (0-15), and cc being the buffer to assign.
                                                // the filter to, see CAN_FILTER_BUFFER for options.
   #define CAN_USER_FILT_x_MASK           dd    // x being the filter to setup (0-15), and dd being the mask to use with 
                                                // the filter, see CAN_FILTER_MASK for options.
   
   #define CAN_USER_MASK_y                ee    // y being the mask to setup (0-1), and ee being the Mask for the filter Id.
   #define CAN_USER_MASK_y_ID_TYPE        ff    // y being the mask to setup (0-1), and ff being the Mask Id type:
                                                //    CAN_MASK_ID_TYPE_SID - mask Id is a SID Id, only the SID bits of Mask are written the EID bits are set to 0.
                                                //    CAN_MASK_ID_TYPE_EID - mask Id is an EID Id, the SID and EID bits of Mask are written.
   #define CAN_USER_MASK_y_FILTER_TYPE    gg    // y being the mask to setup (0-1), and gg being the filter mask type:
                                                //    CAN_FILTER_MASK_TYPE_EITHER - match either SID or EID messages, ignores EXIDE bit of filter.
                                                //    CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, EXIDE bit of filter determines type of messages received.
*/

#ifndef CAN_USE_FILTERS
 #define CAN_USE_FILTERS                  FALSE
#endif

#if CAN_USE_FILTERS == TRUE
 //Filter 0
 #ifndef CAN_USER_FILT_0
  #define CAN_USER_FILT_0                 0
 #endif

 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE            CAN_FILTER_TYPE_EID
 #endif
  
 #ifndef CAN_USER_FILT_0_BUFFER
  #if CAN_USE_RX_FIFO == TRUE
   #define CAN_USER_FILT_0_BUFFER         CAN_FILTER_BUFFER_FIFO
  #else
   #define CAN_USER_FILT_0_BUFFER         CAN_FILTER_BUFFER_ALL
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_0_MASK
  #define CAN_USER_FILT_0_MASK            CAN_FILTER_MASK_0
 #endif
 
 #if CAN_USER_FILT_0_MASK == CAN_FILTER_MASK_0
  #ifndef CAN_USER_MASK_0
   #define CAN_USER_MASK_0                0
  #endif
  
  #ifndef CAN_USER_MASK_0_ID_TYPE
   #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
  #endif
  
  #ifndef CAN_USER_MASK_0_FILTER_TYPE
   #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
  #endif
 #elif CAN_USER_FILTER_0_MASK == CAN_FILTER_MASK_1
  #ifndef CAN_USER_MASK_1
   #define CAN_USER_MASK_1                0
  #endif
  
  #ifndef CAN_USER_MASK_1_ID_TYPE
   #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
  #endif
  
  #ifndef CAN_USER_MASK_1_FILTER_TYPE
   #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
  #endif
 #elif CAN_USER_FILTER_0_MASK == CAN_FILTER_MASK_2
  #ifndef CAN_USER_MASK_2
   #define CAN_USER_MASK_2                0
  #endif
  
  #ifndef CAN_USER_MASK_2_ID_TYPE
   #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
  #endif
  
  #ifndef CAN_USER_MASK_2_FILTER_TYPE
   #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
  #endif
 #endif
 
 //Filter 1
 #ifdef CAN_USER_FILT_1
  #ifndef CAN_USER_FILT_1_TYPE
   #define CAN_USER_FILT_1_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_1_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_1_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_1_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_1_MASK
   #define CAN_USER_FILT_1_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_1_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_1_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_1_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 2
 #ifdef CAN_USER_FILT_2
  #ifndef CAN_USER_FILT_2_TYPE
   #define CAN_USER_FILT_2_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_2_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_2_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_2_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_2_MASK
   #define CAN_USER_FILT_2_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_2_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_2_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_2_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 3
 #ifdef CAN_USER_FILT_3
  #ifndef CAN_USER_FILT_3_TYPE
   #define CAN_USER_FILT_3_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_3_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_3_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_3_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_3_MASK
   #define CAN_USER_FILT_3_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_3_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_3_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_3_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 4
 #ifdef CAN_USER_FILT_4
  #ifndef CAN_USER_FILT_4_TYPE
   #define CAN_USER_FILT_4_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_4_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_4_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_4_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_4_MASK
   #define CAN_USER_FILT_4_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_4_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_4_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_4_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 5
 #ifdef CAN_USER_FILT_5
  #ifndef CAN_USER_FILT_5_TYPE
   #define CAN_USER_FILT_5_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_5_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_5_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_5_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_5_MASK
   #define CAN_USER_FILT_5_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_5_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_5_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_5_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 6
 #ifdef CAN_USER_FILT_6
  #ifndef CAN_USER_FILT_6_TYPE
   #define CAN_USER_FILT_6_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_6_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_6_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_6_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_6_MASK
   #define CAN_USER_FILT_6_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_6_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_6_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_6_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 7
 #ifdef CAN_USER_FILT_7
  #ifndef CAN_USER_FILT_7_TYPE
   #define CAN_USER_FILT_7_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_7_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_7_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_7_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_7_MASK
   #define CAN_USER_FILT_7_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_7_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_7_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_7_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 8
 #ifdef CAN_USER_FILT_8
  #ifndef CAN_USER_FILT_8_TYPE
   #define CAN_USER_FILT_8_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_8_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_8_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_8_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_8_MASK
   #define CAN_USER_FILT_8_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_8_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_8_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_8_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 9
 #ifdef CAN_USER_FILT_9
  #ifndef CAN_USER_FILT_9_TYPE
   #define CAN_USER_FILT_9_TYPE           CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_9_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_9_BUFFER        CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_9_BUFFER        CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_9_MASK
   #define CAN_USER_FILT_9_MASK           CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_9_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_9_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_9_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 10
 #ifdef CAN_USER_FILT_10
  #ifndef CAN_USER_FILT_10_TYPE
   #define CAN_USER_FILT_10_TYPE          CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_10_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_10_BUFFER       CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_10_BUFFER       CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_10_MASK
   #define CAN_USER_FILT_10_MASK          CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_10_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_10_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_10_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 11
 #ifdef CAN_USER_FILT_11
  #ifndef CAN_USER_FILT_11_TYPE
   #define CAN_USER_FILT_11_TYPE          CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_11_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_11_BUFFER       CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_11_BUFFER       CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_11_MASK
   #define CAN_USER_FILT_11_MASK          CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_11_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_11_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_11_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 12
 #ifdef CAN_USER_FILT_12
  #ifndef CAN_USER_FILT_12_TYPE
   #define CAN_USER_FILT_12_TYPE          CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_12_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_12_BUFFER       CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_12_BUFFER       CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_12_MASK
   #define CAN_USER_FILT_12_MASK          CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_12_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_12_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_12_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 13
 #ifdef CAN_USER_FILT_13
  #ifndef CAN_USER_FILT_13_TYPE
   #define CAN_USER_FILT_13_TYPE          CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_13_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_13_BUFFER       CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_13_BUFFER       CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_13_MASK
   #define CAN_USER_FILT_13_MASK          CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_13_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_13_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_13_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 14
 #ifdef CAN_USER_FILT_14
  #ifndef CAN_USER_FILT_14_TYPE
   #define CAN_USER_FILT_14_TYPE          CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_14_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_14_BUFFER       CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_14_BUFFER       CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_14_MASK
   #define CAN_USER_FILT_14_MASK          CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_14_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_14_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_14_MASK == CAN_FILTER_MASK_2
   #ifndef CAN_USER_MASK_2
    #define CAN_USER_MASK_2                0
   #endif
  
   #ifndef CAN_USER_MASK_2_ID_TYPE
    #define CAN_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_2_FILTER_TYPE
    #define CAN_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #endif
 #endif
 
 //Filter 15
 #ifdef CAN_USER_FILT_15
  #ifndef CAN_USER_FILT_15_TYPE
   #define CAN_USER_FILT_15_TYPE          CAN_FILTER_TYPE_EID
  #endif
  
  #ifndef CAN_USER_FILT_15_BUFFER
   #if CAN_USE_RX_FIFO == TRUE
    #define CAN_USER_FILT_15_BUFFER       CAN_FILTER_BUFFER_FIFO
   #else
    #define CAN_USER_FILT_15_BUFFER       CAN_FILTER_BUFFER_RXB0
   #endif
  #endif
  
  #ifndef CAN_USER_FILT_15_MASK
   #define CAN_USER_FILT_15_MASK          CAN_FILTER_MASK_0
  #endif
  
  #if CAN_USER_FILT_15_MASK == CAN_FILTER_MASK_0
   #ifndef CAN_USER_MASK_0
    #define CAN_USER_MASK_0                0
   #endif
  
   #ifndef CAN_USER_MASK_0_ID_TYPE
    #define CAN_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_0_FILTER_TYPE
    #define CAN_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_15_MASK == CAN_FILTER_MASK_1
   #ifndef CAN_USER_MASK_1
    #define CAN_USER_MASK_1                0
   #endif
  
   #ifndef CAN_USER_MASK_1_ID_TYPE
    #define CAN_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
   #endif
  
   #ifndef CAN_USER_MASK_1_FILTER_TYPE
    #define CAN_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
   #endif
  #elif CAN_USER_FILTER_15_MASK == CAN_FILTER_MASK_2
   #error Mask 2 is Filter 15, Mask 2 should n0t be assigned to Filter 15
  #endif
 #endif
 
 #if defined(CAN_USER_FILT_15) && defined(CAN_USER_MASK_2)
  #error Mask 2 is Filter 15, only Filter 15 or Mask 2 should be defined
 #endif
#else
 #if defined(CAN_USER_FILT_0) && (CAN_USER_FILT_0 != 0)
  #undef CAN_USER_FILT_0
 #endif
 
 #ifndef CAN_USER_FILT_0
  #define CAN_USER_FILT_0           0
 #endif
 
 #if defined(CAN_USER_FILT_0_TYPE) && (CAN_USER_FILT_0_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_0_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 #if defined(CAN_USER_FILT_0_BUFFER) && (CAN_USER_FILT_0_BUFFER != CAN_FILTER_BUFFER_FIFO)
  #undef CAN_USER_FILT_0_BUFFER
 #endif
 
 #ifndef CAN_USER_FILT_0_BUFFER
  #if CAN_USE_RX_FIFO
   #define CAN_USER_FILT_0_BUFFER   CAN_FILTER_BUFFER_FIFO
  #else
   #define CAN_USER_FILT_0_BUFFER   CAN_FILTER_BUFFER_ALL
  #endif
 #endif
 
 #if defined(CAN_USER_FILT_0_MASK) && (CAN_USER_FILT_0_MASK != CAN_FILTER_MASK_0)
  #undef CAN_USER_FILT_0_MASK
 #endif
 
 #ifndef CAN_USER_FILT_0_MASK
  #define CAN_USER_FILT_0_MASK         CAN_FILTER_MASK_0
 #endif
 
 #if defined(CAN_USER_MASK_0) && (CAN_USER_MASK_0 != 0)
  #undef CAN_USER_MASK_0
 #endif
 
 #ifndef CAN_USER_MASK_0
  #define CAN_USER_MASK_0              0
 #endif
 
 #if defined(CAN_USER_MASK_0_ID_TYPE) && (CAN_USER_MASK_0_ID_TYPE != CAN_MASK_ID_TYPE_EID)
  #undef CAN_USER_MASK_0_ID_TYPE
 #endif
 
 #ifndef CAN_USER_MASK_0_ID_TYPE
  #define CAN_USER_MASK_0_ID_TYPE      CAN_MASK_ID_TYPE_EID
 #endif
 
 #if defined(CAN_USER_MASK_0_FILTER_TYPE) && (CAN_USER_MASK_0_FILTER_TYPE != CAN_FILTER_MASK_TYPE_EITHER)
  #undef CAN_USER_MASK_0_FILTER_TYPE
 #endif
 
 #ifndef CAN_USER_MASK_0_FILTER_TYPE
  #define CAN_USER_MASK_0_FILTER_TYPE  CAN_FILTER_MASK_TYPE_EITHER
 #endif
#endif


///////////////////////////////////////////////////////////////////////////////
//////////////////////////// CAN Control Registers ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum 
{
   CAN_WIN_RX0 = 0,  //Receive Buffer 0
   CAN_WIN_TX2 = 2,  //Transmit Buffer 2
   CAN_WIN_TX1,      //Transmit Buffer 1
   CAN_WIN_TX0,      //Transmit Buffer 0
   CAN_WIN_RX1       //Receive Buffer 1
} CAN_WIN_ADDRESS;

typedef enum 
{
   CAN_FIFO_BUFFER_0,
   CAN_FIFO_BUFFER_1,
   CAN_FIFO_BUFFER_2,
   CAN_FIFO_BUFFER_3,
   CAN_FIFO_BUFFER_4,
   CAN_FIFO_BUFFER_5,
   CAN_FIFO_BUFFER_6,
   CAN_FIFO_BUFFER_7
} CAN_FIFO_BUFFER;

// CAN Control Register Mode 0
typedef struct
{
   uint8_t u0:1;
   CAN_WIN_ADDRESS win:3;   //Window Address bits
   uint8_t abat:1;          //Abort All Pending Transmission bit
   CAN_OP_MODE reqop:3;     //Request CAN Operation Mode bits
} CiCANCON_MODE0;

CiCANCON_MODE0 CAN_CANCON_MODE0;
#locate CAN_CANCON_MODE0 = getenv("SFR:CANCON")

// CAN Control Register Mode 1
typedef struct
{
   uint8_t u0_3:4;
   uint8_t abat:1;          //Abort All Pending Transmission bit
   CAN_OP_MODE reqop:3;     //Request CAN Operation Mode bits
} CiCANCON_MODE1;

CiCANCON_MODE1 CAN_CANCON_MODE1;
#locate CAN_CANCON_MODE1 = getenv("SFR:CANCON")

// CAN Control Register Mode 2
typedef struct
{
   CAN_FIFO_BUFFER fp:4;   //FIFO Read Pointer bits
   uint8_t abat:1;          //Abort All Pending Transmission bit
   CAN_OP_MODE reqop:3;     //Request CAN Operation Mode bits
} CiCANCON_MODE2;

CiCANCON_MODE2 CAN_CANCON_MODE2;
#locate CAN_CANCON_MODE2 = getenv("SFR:CANCON")

typedef enum 
{
   CAN_INT_CODE_NONE,
   CAN_INT_CODE_ERR,
   CAN_INT_CODE_TXB2,
   CAN_INT_CODE_TXB1,
   CAN_INT_CODE_TXB0,
   CAN_INT_CODE_RXB1,
   CAN_INT_CODE_RXB0,
   CAN_INT_CODE_WAKE
} CAN_INT_CODE;

typedef enum 
{
   CAN_EINT_CODE_NONE,
   CAN_EINT_CODE_ERR=2,
   CAN_EINT_CODE_TXB2=4,
   CAN_EINT_CODE_TXB1=6,
   CAN_EINT_CODE_TXB0=8,
   CAN_EINT_CODE_WAKE=14,
   CAN_EINT_CODE_RXB0=16,
   CAN_EINT_CODE_RXB1,
   CAN_EINT_CODE_TRB0,
   CAN_EINT_CODE_TRB1,
   CAN_EINT_CODE_TRB2,
   CAN_EINT_CODE_TRB3,
   CAN_EINT_CODE_TRB4,
   CAN_EINT_CODE_TRB5
} CAN_EINT_CODE;

// CAN Status Register Mode 0
typedef struct
{
   uint8_t u0:1;
   CAN_INT_CODE icode:3;   //Interrupt Code bits
   uint8_t u4:1;
   CAN_OP_MODE opmode:3;   //Operation Mode Status bits
} CiCANSTAT_MODE0;

CiCANSTAT_MODE0 CAN_CANSTAT_MODE0;
#locate CAN_CANSTAT_MODE0 = getenv("SFR:CANSTAT")

// CAN Status Register Mode 1
typedef struct
{
   CAN_EINT_CODE icode:5;  //Interrupt Code bits
   CAN_OP_MODE opmode:3;   //Operation Mode Status bits
} CiCANSTAT_MODE1;

CiCANSTAT_MODE1 CAN_CANSTAT_MODE1;
#locate CAN_CANSTAT_MODE1 = getenv("SFR:CANSTAT")

// CAN Status Register Mode 2
typedef struct
{
   CAN_EINT_CODE icode:5;  //Interrupt Code bits
   CAN_OP_MODE opmode:3;   //Operation Mode Status bits
} CiCANSTAT_MODE2;

CiCANSTAT_MODE2 CAN_CANSTAT_MODE2;
#locate CAN_CANSTAT_MODE2 = getenv("SFR:CANSTAT")

typedef enum
{
   CAN_MODE_LEGACY,
   CAN_MODE_ENHANCED_LEGACY,
   CAN_MODE_ENHANCED_FIFO
} CAN_MODE;

typedef enum 
{
   ENHANCED_WINDOW_AF0TO2_BRGCON23,
   ENHANCED_WINDOW_AF2TO5_BRGCON1_CIOCON,
   ENHANCED_WINDOW_AFM_ERROR_ICON,
   ENHANCED_WINDOW_TXB0,
   ENHANCED_WINDOW_TXB1,
   ENHANCED_WINDOW_TXB2,
   ENHANCED_WINDOW_AF6TO8,
   ENHANCED_WINDOW_AF9TO11,
   ENHANCED_WINDOW_AF12TO14,
   ENHANCED_WINDOW_AF15,
   ENHANCED_WINDOW_RXINT0TO1=15,
   ENHANCED_WINDOW_RXB0,
   ENHANCED_WINDOW_RXB1,
   ENHANCED_WINDOW_TRB0,
   ENHANCED_WINDOW_TRB1,
   ENHANCED_WINDOW_TRB2,
   ENHANCED_WINDOW_TRB3,
   ENHANCED_WINDOW_TRB4,
   ENHANCED_WINDOW_TRB5
} ENHANCED_WINDOW;

// Enhanced CAN Control Register
typedef struct
{
   ENHANCED_WINDOW ewin:5;    //Enhanced Window Address bits
   uint8_t fifowm:1;          //FIFO High Water Mark bit
   CAN_MODE mdsel:2;          //Mode Select bits
} CiECANCON;

CiECANCON CAN_ECANCON;
#locate CAN_ECANCON = getenv("SFR:ECANCON")

// Communication Status Register Mode 0
typedef struct
{
   uint8_t ewarn:1;     //Error Warning bit
   uint8_t rxwarn:1;    //Receiver Warning bit
   uint8_t txwarn:1;    //Transmitter Warning bit
   uint8_t rxbp:1;      //Receiver Bus Passive bit
   uint8_t txbp:1;      //Transmitter Bus Passive bit
   uint8_t txbo:1;      //Transmitter Bus-Off bit
   uint8_t rx1ovfl:1;   //Receive Buffer 1 Overflow bit
   uint8_t rx0ovfl:1;   //Receive Buffer 0 Overflow bit
} CiCOMSTAT_MODE0;

CiCOMSTAT_MODE0 CAN_COMSTAT_MODE0;
#locate CAN_COMSTAT_MODE0 = getenv("SFR:COMSTAT")

// Communication Status Register Mode 1
typedef struct
{
   uint8_t ewarn:1;     //Error Warning bit
   uint8_t rxwarn:1;    //Receiver Warning bit
   uint8_t txwarn:1;    //Transmitter Warning bit
   uint8_t rxbp:1;      //Receiver Bus Passive bit
   uint8_t txbp:1;      //Transmitter Bus Passive bit
   uint8_t txbo:1;      //Transmitter Bus-Off bit
   uint8_t rxovfl:1;    //Receive Buffer Overflow bit
   uint8_t u7:1;
} CiCOMSTAT_MODE1;

CiCOMSTAT_MODE1 CAN_COMSTAT_MODE1;
#locate CAN_COMSTAT_MODE1 = getenv("SFR:COMSTAT")

// Communication Status Register Mode 2
typedef struct
{
   uint8_t ewarn:1;     //Error Warning bit
   uint8_t rxwarn:1;    //Receiver Warning bit
   uint8_t txwarn:1;    //Transmitter Warning bit
   uint8_t rxbp:1;      //Receiver Bus Passive bit
   uint8_t txbp:1;      //Transmitter Bus Passive bit
   uint8_t txbo:1;      //Transmitter Bus-Off bit
   uint8_t rxovfl:1;    //Receive Buffer Overflow bit
   uint8_t fifoempty:1; //FIFO Not Empty bit
} CiCOMSTAT_MODE2;

CiCOMSTAT_MODE2 CAN_COMSTAT_MODE2;
#locate CAN_COMSTAT_MODE2 = getenv("SFR:COMSTAT")

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// CAN TX Registers //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Transmit Buffer n Control Registers
typedef struct 
{
   uint8_t txpri:2;     //Transmit Priority bits
   uint8_t u2:1;
   uint8_t txreq:1;     //Transmit Request Status bit
   uint8_t txerr:1;     //Transmission Error Detected Status bit
   uint8_t txlarb:1;    //Transmission Lost Arbitration Status bit
   uint8_t txabt:1;     //Transmission Aborted Status bit
   uint8_t txbif:1;     //Transmit Buffer Interrupt Flag bit
} TXBnCON;

TXBnCON CAN_TXB0CON;
TXBnCON CAN_TXB1CON;
TXBnCON CAN_TXB2CON;

#locate CAN_TXB0CON = getenv("SFR:TXB0CON")
#locate CAN_TXB1CON = getenv("SFR:TXB1CON")
#locate CAN_TXB2CON = getenv("SFR:TXB2CON")

// Transmit Buffer Identifier Registers
typedef union
{
   struct
   {
      uint8_t sid3_10;     //Standard Identifier bits 3-10
      uint8_t eid16_17:2;  //Extended Identifier bits 16-18
      uint8_t u10:1;
      uint8_t exide:1;     //Extended Identifier Enable bit
      uint8_t u12:1;
      uint8_t sid0_2:3;    //Standard Identifier bits 0-2
      uint8_t eid8_15;     //Extended Identifier bits 8-15
      uint8_t eid0_7;      //Extended Identifier bits 0-7
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} TXBnID;

TXBnID CAN_TXB0ID;
TXBnID CAN_TXB1ID;
TXBnID CAN_TXB2ID;

#locate CAN_TXB0ID = getenv("SFR:TXB0SIDH")
#locate CAN_TXB1ID = getenv("SFR:TXB1SIDH")
#locate CAN_TXB2ID = getenv("SFR:TXB2SIDH")

typedef struct
{
   uint8_t dlc:4;       //Data Length Code bits
   uint8_t u4_5:2;
   uint8_t txrtr:1;     //Transmit Remote Frame Transmission Request bit
   uint8_t u7:1;
} TXBnDLC;

TXBnDLC CAN_TXB0DLC;
TXBnDLC CAN_TXB1DLC;
TXBnDLC CAN_TXB2DLC;

#locate CAN_TXB0DLC = getenv("SFR:TXB0DLC")
#locate CAN_TXB1DLC = getenv("SFR:TXB1DLC")
#locate CAN_TXB2DLC = getenv("SFR:TXB2DLC")

uint8_t CAN_TXB0DATA[8];
uint8_t CAN_TXB1DATA[8];
uint8_t CAN_TXB2DATA[8];

#locate CAN_TXB0DATA = getenv("SFR:TXB0D0")
#locate CAN_TXB1DATA = getenv("SFR:TXB1D0")
#locate CAN_TXB2DATA = getenv("SFR:TXB2D0")

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// CAN RX Registers //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum
{
   CAN_RX_MODE_VALID_ALL,
   CAN_RX_MODE_VALID_SID,
   CAN_RX_MODE_VALID_EID,
   CAN_RX_MODE_ALL
} CAN_RX_MODE;

// Receive Buffer 0 Control Register Mode 0
typedef struct
{
   uint8_t filhit0:1;      //Filter Hit bit
   uint8_t jtoff:1;        //Jump Table Offset bit (read-only)
   uint8_t rb0dben:1;      //Receive Buffer 0 Double-Buffer Enable bit
   uint8_t rxrtrro:1;      //Remote Transmission Request bit (read-only)
   uint8_t u4;
   CAN_RX_MODE rxm:2;      //Receive Buffer Mode bits
   uint8_t rxful:1;        //Receive Full Status bit
} CiRXB0CON_MODE_0;

CiRXB0CON_MODE_0 CAN_RXB0CON_MODE_0;
#locate CAN_RXB0CON_MODE_0 = getenv("SFR:RXB0CON")

// Receive Buffer 0 Control Register Mode 1 and 2
typedef struct
{
   uint8_t filhit:5;       //Filter Hit bits
   uint8_t rxrtrro:1;      //Remote Transmission Request bit (read-only)
   uint8_t rxm:1;          //Receive Buffer Mode bit
   uint8_t rxful:1;        //Receive Full Status bit
} CiRXB0CON_MODE_12;

CiRXB0CON_MODE_12 CAN_RXB0CON_MODE_1;
CiRXB0CON_MODE_12 CAN_RXB0CON_MODE_2;

#locate CAN_RXB0CON_MODE_1 = getenv("SFR:RXB0CON")
#locate CAN_RXB0CON_MODE_2 = getenv("SFR:RXB0CON")

// Receive Buffer 1 Control Register Mode 0
typedef struct
{
   uint8_t filhit:3;       //Filter Hit bit
   uint8_t rxrtrro:1;      //Remote Transmission Request bit (read-only)
   uint8_t u4;
   CAN_RX_MODE rxm:2;      //Receive Buffer Mode bits
   uint8_t rxful:1;        //Receive Full Status bit
} CiRXB1CON_MODE_0;

CiRXB1CON_MODE_0 CAN_RXB1CON_MODE_0;
#locate CAN_RXB1CON_MODE_0 = getenv("SFR:RXB1CON")

// Receive Buffer 1 Control Register Mode 1 and 2
typedef struct
{
   uint8_t filhit:5;       //Filter Hit bit
   uint8_t rxrtrro:1;      //Remote Transmission Request bit (read-only)
   uint8_t rxm:1;          //Receive Buffer Mode bit
   uint8_t rxful:1;        //Receive Full Status bit
} CiRXB1CON_MODE_12;

CiRXB1CON_MODE_12 CAN_RXB1CON_MODE_1;
CiRXB1CON_MODE_12 CAN_RXB1CON_MODE_2;

#locate CAN_RXB1CON_MODE_1 = getenv("SFR:RXB1CON")
#locate CAN_RXB1CON_MODE_2 = getenv("SFR:RXB1CON")

// Receive Buffer Identifier Registers
typedef union
{
   struct
   {
      uint8_t sid3_10;     //Standard Identifier bits 3-10
      uint8_t eid16_17:2;  //Extended Identifier bits 16-18
      uint8_t u10:1;
      uint8_t exide:1;     //Extended Identifier Enable bit
      uint8_t srr:1;       //Substitute Remote Request bit
      uint8_t sid0_2:3;    //Standard Identifier bits 0-2
      uint8_t eid8_15;     //Extended Identifier bits 8-15
      uint8_t eid0_7;      //Extended Identifier bits 0-7
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} RXBnID;

RXBnID CAN_RXB0ID;
RXBnID CAN_RXB1ID;

#locate CAN_RXB0ID = getenv("SFR:RXB0SIDH")
#locate CAN_RXB1ID = getenv("SFR:RXB1SIDH")

typedef struct
{
   uint8_t dlc:4;       //Data Length Code bits
   uint8_t rb0:1;       //Reserved bit 0
   uint8_t rb1:1;       //Reserved bit 1
   uint8_t rxrtr:1;     //Receiver Transmission Request bit
   uint8_t u7:1;
} RXBnDLC;

RXBnDLC CAN_RXB0DLC;
RXBnDLC CAN_RXB1DLC;

#locate CAN_RXB0DLC = getenv("SFR:RXB0DLC")
#locate CAN_RXB1DLC = getenv("SFR:RXB1DLC")

uint8_t CAN_RXB0DATA[8];
uint8_t CAN_RXB1DATA[8];

#locate CAN_RXB0DATA = getenv("SFR:RXB0D0")
#locate CAN_RXB1DATA = getenv("SFR:RXB1D0")

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// CAN TX/RX Registers /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// TX/RX Buffer Control Registers Receive Mode
typedef struct
{
   uint8_t filhit:5;    //Filter Hit bits
   uint8_t rxrtrro:1;   //Remote Transmission Request bit (read-only)
   uint8_t rxm:1;       //Receive Buffer Mode bit
   uint8_t rxful:1;     //Receive Full Status bit
} BnCON_RX;

BnCON_RX CAN_B0CON_RX;
BnCON_RX CAN_B1CON_RX;
BnCON_RX CAN_B2CON_RX;
BnCON_RX CAN_B3CON_RX;
BnCON_RX CAN_B4CON_RX;
BnCON_RX CAN_B5CON_RX;

#locate CAN_B0CON_RX = getenv("SFR:B0CON")
#locate CAN_B1CON_RX = getenv("SFR:B1CON")
#locate CAN_B2CON_RX = getenv("SFR:B2CON")
#locate CAN_B3CON_RX = getenv("SFR:B3CON")
#locate CAN_B4CON_RX = getenv("SFR:B4CON")
#locate CAN_B5CON_RX = getenv("SFR:B5CON")

// TX/RX Buffer Control Registers Transmit Mode
typedef struct
{
   uint8_t txpri:2;     //Transmit Priority bits
   uint8_t rtren:1;     //Automatic Remote Transmission Request Enable bit
   uint8_t txreq:1;     //Transmit Request Status bit
   uint8_t txerr:1;     //Transmission Error Detect Status bit
   uint8_t txlarb:1;    //Transmission Lost Arbitration Status bit
   uint8_t txabt:1;     //Transmission Aborted Status bit
   uint8_t txbif:1;     //Transmit Buffer Interrupt Flag bit
} BnCON_TX;

BnCON_TX CAN_B0CON_TX;
BnCON_TX CAN_B1CON_TX;
BnCON_TX CAN_B2CON_TX;
BnCON_TX CAN_B3CON_TX;
BnCON_TX CAN_B4CON_TX;
BnCON_TX CAN_B5CON_TX;

#locate CAN_B0CON_TX = getenv("SFR:B0CON")
#locate CAN_B1CON_TX = getenv("SFR:B1CON")
#locate CAN_B2CON_TX = getenv("SFR:B2CON")
#locate CAN_B3CON_TX = getenv("SFR:B3CON")
#locate CAN_B4CON_TX = getenv("SFR:B4CON")
#locate CAN_B5CON_TX = getenv("SFR:B5CON")

RXBnID CAN_B0ID_RX;
RXBnID CAN_B1ID_RX;
RXBnID CAN_B2ID_RX;
RXBnID CAN_B3ID_RX;
RXBnID CAN_B4ID_RX;
RXBnID CAN_B5ID_RX;

#locate CAN_B0ID_RX = getenv("SFR:B0SIDH")
#locate CAN_B1ID_RX = getenv("SFR:B1SIDH")
#locate CAN_B2ID_RX = getenv("SFR:B2SIDH")
#locate CAN_B3ID_RX = getenv("SFR:B3SIDH")
#locate CAN_B4ID_RX = getenv("SFR:B4SIDH")
#locate CAN_B5ID_RX = getenv("SFR:B5SIDH")

TXBnID CAN_B0ID_TX;
TXBnID CAN_B1ID_TX;
TXBnID CAN_B2ID_TX;
TXBnID CAN_B3ID_TX;
TXBnID CAN_B4ID_TX;
TXBnID CAN_B5ID_TX;

#locate CAN_B0ID_TX = getenv("SFR:B0SIDH")
#locate CAN_B1ID_TX = getenv("SFR:B1SIDH")
#locate CAN_B2ID_TX = getenv("SFR:B2SIDH")
#locate CAN_B3ID_TX = getenv("SFR:B3SIDH")
#locate CAN_B4ID_TX = getenv("SFR:B4SIDH")
#locate CAN_B5ID_TX = getenv("SFR:B5SIDH")

RXBnDLC CAN_B0DLC_RX;
RXBnDLC CAN_B1DLC_RX;
RXBnDLC CAN_B2DLC_RX;
RXBnDLC CAN_B3DLC_RX;
RXBnDLC CAN_B4DLC_RX;
RXBnDLC CAN_B5DLC_RX;

#locate CAN_B0DLC_RX = getenv("SFR:B0DLC")
#locate CAN_B1DLC_RX = getenv("SFR:B1DLC")
#locate CAN_B2DLC_RX = getenv("SFR:B2DLC")
#locate CAN_B3DLC_RX = getenv("SFR:B3DLC")
#locate CAN_B4DLC_RX = getenv("SFR:B4DLC")
#locate CAN_B5DLC_RX = getenv("SFR:B5DLC")

TXBnDLC CAN_B0DLC_TX;
TXBnDLC CAN_B1DLC_TX;
TXBnDLC CAN_B2DLC_TX;
TXBnDLC CAN_B3DLC_TX;
TXBnDLC CAN_B4DLC_TX;
TXBnDLC CAN_B5DLC_TX;

#locate CAN_B0DLC_TX = getenv("SFR:B0DLC")
#locate CAN_B1DLC_TX = getenv("SFR:B1DLC")
#locate CAN_B2DLC_TX = getenv("SFR:B2DLC")
#locate CAN_B3DLC_TX = getenv("SFR:B3DLC")
#locate CAN_B4DLC_TX = getenv("SFR:B4DLC")
#locate CAN_B5DLC_TX = getenv("SFR:B5DLC")

uint8_t CAN_B0DATA[8];
uint8_t CAN_B1DATA[8];
uint8_t CAN_B2DATA[8];
uint8_t CAN_B3DATA[8];
uint8_t CAN_B4DATA[8];
uint8_t CAN_B5DATA[8];

#locate CAN_B0DATA = getenv("SFR:B0D0")
#locate CAN_B1DATA = getenv("SFR:B1D0")
#locate CAN_B2DATA = getenv("SFR:B2D0")
#locate CAN_B3DATA = getenv("SFR:B3D0")
#locate CAN_B4DATA = getenv("SFR:B4D0")
#locate CAN_B5DATA = getenv("SFR:B5D0")

typedef union
{
   struct 
   {
      uint8_t u0_1:2;
      uint8_t b0txen:1;    //Buffer 0 Transmit Enable bits
      uint8_t b1txen:1;    //Buffer 1 Transmit Enable bits
      uint8_t b2txen:1;    //Buffer 2 Transmit Enable bits
      uint8_t b3txen:1;    //Buffer 3 Transmit Enable bits
      uint8_t b4txen:1;    //Buffer 4 Transmit Enable bits
      uint8_t b5txen:1;    //Buffer 5 Transmit Enable bits
   };
   uint8_t b;
} CiBSEL0;

CiBSEL0 CAN_BSEL0;
#locate CAN_BSEL0 = getenv("SFR:BSEL0")

///////////////////////////////////////////////////////////////////////////////
///////////////////// CAN Acceptance Filter/Mask Registers ////////////////////
///////////////////////////////////////////////////////////////////////////////

TXBnID CAN_RXF0ID;
TXBnID CAN_RXF1ID;
TXBnID CAN_RXF2ID;
TXBnID CAN_RXF3ID;
TXBnID CAN_RXF4ID;
TXBnID CAN_RXF5ID;
TXBnID CAN_RXF6ID;
TXBnID CAN_RXF7ID;
TXBnID CAN_RXF8ID;
TXBnID CAN_RXF9ID;
TXBnID CAN_RXF10ID;
TXBnID CAN_RXF11ID;
TXBnID CAN_RXF12ID;
TXBnID CAN_RXF13ID;
TXBnID CAN_RXF14ID;
TXBnID CAN_RXF15ID;

#locate CAN_RXF0ID = getenv("SFR:RXF0SIDH")
#locate CAN_RXF1ID = getenv("SFR:RXF1SIDH")
#locate CAN_RXF2ID = getenv("SFR:RXF2SIDH")
#locate CAN_RXF3ID = getenv("SFR:RXF3SIDH")
#locate CAN_RXF4ID = getenv("SFR:RXF4SIDH")
#locate CAN_RXF5ID = getenv("SFR:RXF5SIDH")
#locate CAN_RXF6ID = getenv("SFR:RXF6SIDH")
#locate CAN_RXF7ID = getenv("SFR:RXF7SIDH")
#locate CAN_RXF8ID = getenv("SFR:RXF8SIDH")
#locate CAN_RXF9ID = getenv("SFR:RXF9SIDH")
#locate CAN_RXF10ID = getenv("SFR:RXF10SIDH")
#locate CAN_RXF11ID = getenv("SFR:RXF11SIDH")
#locate CAN_RXF12ID = getenv("SFR:RXF12SIDH")
#locate CAN_RXF13ID = getenv("SFR:RXF13SIDH")
#locate CAN_RXF14ID = getenv("SFR:RXF14SIDH")
#locate CAN_RXF15ID = getenv("SFR:RXF15SIDH")

TXBnID CAN_RXM0ID;
TXBnID CAN_RXM1ID;
TXBnID CAN_RXM2ID;

#locate CAN_RXM0ID = getenv("SFR:RXM0SIDH")
#locate CAN_RXM1ID = getenv("SFR:RXM1SIDH")
#locate CAN_RXM2ID = getenv("SFR:RXF15SIDH")

typedef union
{
   struct
   {
      uint8_t rxf0en:1;
      uint8_t rxf1en:1;
      uint8_t rxf2en:1;
      uint8_t rxf3en:1;
      uint8_t rxf4en:1;
      uint8_t rxf5en:1;
      uint8_t rxf6en:1;
      uint8_t rxf7en:1;
      uint8_t rxf8en:1;
      uint8_t rxf9en:1;
      uint8_t rxf10en:1;
      uint8_t rxf11en:1;
      uint8_t rxf12en:1;
      uint8_t rxf13en:1;
      uint8_t rxf14en:1;
      uint8_t rxf15en:1;
   };
   uint8_t b[2];
   uint16_t w;
} RXFCON;

RXFCON CAN_RXFCON;
#locate CAN_RXFCON = getenv("SFR:RXFCON0")

// Standard Data Bytes Filter Length Count Register
typedef union
{
   struct
   {
      uint8_t flc:5;
      uint8_t u5_7:3;
   };
   uint8_t b;
} CiSDFLC;

CiSDFLC CAN_SDFLC;
#locate CAN_SDFLC = getenv("SFR:SDFLC")

// Receive Filter Buffer Control Registers
typedef union
{
   struct
   {
      CAN_FILTER_BUFFER f0bp:4;
      CAN_FILTER_BUFFER f1bp:4;
      CAN_FILTER_BUFFER f2bp:4;
      CAN_FILTER_BUFFER f3bp:4;
      CAN_FILTER_BUFFER f4bp:4;
      CAN_FILTER_BUFFER f5bp:4;
      CAN_FILTER_BUFFER f6bp:4;
      CAN_FILTER_BUFFER f7bp:4;
      CAN_FILTER_BUFFER f8bp:4;
      CAN_FILTER_BUFFER f9bp:4;
      CAN_FILTER_BUFFER f10bp:4;
      CAN_FILTER_BUFFER f11bp:4;
      CAN_FILTER_BUFFER f12bp:4;
      CAN_FILTER_BUFFER f13bp:4;
      CAN_FILTER_BUFFER f14bp:4;
      CAN_FILTER_BUFFER f15bp:4;
   };
   uint8_t b[8];
} RXFBCON;

RXFBCON CAN_RXFBCON;
#locate CAN_RXFBCON = getenv("SFR:RXFBCON0")

// Mask Select Registers
typedef union
{
   struct
   {
      CAN_FILTER_MASK fil0:2;
      CAN_FILTER_MASK fil1:2;
      CAN_FILTER_MASK fil2:2;
      CAN_FILTER_MASK fil3:2;
      CAN_FILTER_MASK fil4:2;
      CAN_FILTER_MASK fil5:2;
      CAN_FILTER_MASK fil6:2;
      CAN_FILTER_MASK fil7:2;
      CAN_FILTER_MASK fil8:2;
      CAN_FILTER_MASK fil9:2;
      CAN_FILTER_MASK fil10:2;
      CAN_FILTER_MASK fil11:2;
      CAN_FILTER_MASK fil12:2;
      CAN_FILTER_MASK fil13:2;
      CAN_FILTER_MASK fil14:2;
      CAN_FILTER_MASK fil15:2;
   };
   uint8_t b[4];
} MSEL;

MSEL CAN_MSEL;
#locate CAN_MSEL = getenv("SFR:MSEL0")

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// CAN Baud Rate Registers ///////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
   uint8_t brp:6;       //Baud Rate Prescaler bits
   uint8_t sjw:2;       //Synchronized Jump Width bits
   uint8_t prseg:3;     //Propagation Time Select bits
   uint8_t seg1ph:3;    //Phase Segment 1 bits
   uint8_t sam:1;       //Sample of the CAN bus Line bit
   uint8_t seg2phts:1;  //Phase Segment 2 Time Select bit
   uint8_t seg2ph:3;    //Phase Segment 2 Time Select bits
   uint8_t u19_21:3;
   uint8_t wakfil:1;    //CAN Bus Line Filter Wake-up Select bit
   uint8_t wakdis:1;    //Wake-up Disable bit
} BRGCON;

BRGCON CAN_BRGCON;
#locate CAN_BRGCON = getenv("SFR:BRGCON1")

///////////////////////////////////////////////////////////////////////////////
////////////////////////// CAN I/O Control Registers //////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
   uint8_t clksel:1;    //CAN Clock Source Selection bit
   uint8_t u1_3:3;
   uint8_t cancap:1;    //CAN Message Receive Capture Enable bit
   uint8_t endrhi:1;    //Enable Drive High bit
   uint8_t tx2en:1;     //CANTX2 Pin Enable bit
   uint8_t tx2src:1;    //CANTX2 Pin Data Source bit
} CiCIOCON;

CiCIOCON CAN_CIOCON;
#locate CAN_CIOCON = getenv("SFR:CIOCON")

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// CAN Interrupt Registers ///////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef union
{
   struct
   {
      uint8_t u0_1:2;
      uint8_t txb0ie:1;
      uint8_t txb1ie:1;
      uint8_t txb2ie:1;
      uint8_t u5_7:3;
   };
   uint8_t b;
} CiTXBIE;

CiTXBIE CAN_TXBIE;
#locate CAN_TXBIE = getenv("SFR:TXBIE")

typedef union
{
   struct
   {
      uint8_t rxb0ie:1;
      uint8_t rxb1ie:1;
      uint8_t b0ie:1;
      uint8_t b1ie:1;
      uint8_t b2ie:1;
      uint8_t b3ie:1;
      uint8_t b4ie:1;
      uint8_t b5ie:1;
   };
   uint8_t b;
} CiBIE;

CiBIE CAN_BIE0;
#locate CAN_BIE0 = getenv("SFR:BIE0")

// CAN Interrupt Flags
#bit CAN_INT_IRXIF = getenv("BIT:IRXIF")
#bit CAN_INT_WAKIF = getenv("BIT:WAKIF")
#bit CAN_INT_ERRIF = getenv("BIT:ERRIF")
#bit CAN_INT_TXB2IF = getenv("BIT:TXB2IF")
#bit CAN_INT_TXB1IF = getenv("BIT:TXB1IF")
#bit CAN_INT_TXB0IF = getenv("BIT:TXB0IF")
#bit CAN_INT_RXB1IF = getenv("BIT:RXB1IF")
#bit CAN_INT_RXB0IF = getenv("BIT:RXB0IF")

///////////////////////////////////////////////////////////////////////////////

// CAN Global Variables

CAN_OP_MODE g_CANOpmode;

///////////////////////////////////////////////////////////////////////////////

// Prototypes

void can_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type);
void can_set_buffer_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer);
void can_set_filter_mask(CAN_FILTER Filter, CAN_FILTER_MASK Mask);
void can_set_buffer_id(CAN_TX_HEADER *Header, CAN_BUFFER Buffer);
uint16_t *can_buffer_data_address(CAN_BUFFER Buffer);
void can_get_rx_data(CAN_RX_HEADER *Header, uint8_t *Data);

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// can_init()
//
// Initializes the CAN to the default settings that were defined before this
// driver was included or the default defines set by the driver.
//
// Parameters:
//    Mode - optional parameter specifying the operational mode to put the ECAN
//           peripheral in before exiting, defaults to CAN_OP_NORMAL mode.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_init(CAN_OP_MODE Mode = CAN_OP_NORMAL)
{
   uint8_t Value;
   uint8_t i;

   can_set_mode(CAN_OP_CONFIG);
   
   ((CiCIOCON)Value).clksel = 0;
   ((CiCIOCON)Value).cancap = CAN_ENABLE_CAN_CAPTURE;
   ((CiCIOCON)Value).endrhi = CAN_ENABLE_DRIVE_HIGH;
   ((CiCIOCON)Value).tx2en = CAN_ENABLE_CANTX2;
   ((CiCIOCON)Value).tx2src = CAN_CANTX2_SOURCE;
   
   CAN_CIOCON = Value;
   
   can_set_baud(CAN_CLOCK_SPEED, CAN_BAUD_RATE);
   
   Value = 0;
   
   ((CiECANCON)Value).fifowm = CAN_FIFO_INT;
  #if CAN_USE_RX_FIFO == TRUE
   ((CiECANCON)Value).mdsel = CAN_MODE_ENHANCED_FIFO;
  #else
   ((CiECANCON)Value).mdsel = CAN_MODE_ENHANCED_LEGACY;
  #endif
  
   CAN_ECANCON = Value;
   
   CAN_RXB0CON_MODE_1.rxm = 0;
   CAN_RXB1CON_MODE_1.rxm = 0;
   
  #if CAN_TX_BUFFERS > 3
   Value = 0;
   
   for(i=3;i<CAN_TX_BUFFERS;i++)
      Value |= (0x01 << (10 - i));
   
   CAN_BSEL0.b = Value;
  #endif
  
  #if CAN_RX_BUFFERS > 2
   Value = 0;
  
   for(i=2;i<CAN_RX_BUFFERS;i++)
      *(&CAN_B0CON_RX + (0x10 * (i - 2))) = Value;
  #endif
  
   //Setup Filters
   can_set_filter_id(CAN_FILTER_0, CAN_USER_FILT_0, CAN_USER_FILT_0_TYPE);
   can_set_mask_id(CAN_FILTER_MASK_0, CAN_USER_MASK_0, CAN_USER_MASK_0_ID_TYPE, CAN_USER_MASK_0_FILTER_TYPE);
   can_enable_filter(CAN_FILTER_0, CAN_USER_FILT_0_BUFFER, CAN_USER_FILT_0_MASK);
   
 #if CAN_USE_FILTERS == TRUE
  #ifdef CAN_USER_MASK_1
   can_set_mask_id(CAN_FILTER_MASK_1, CAN_USER_MASK_1, CAN_USER_MASK_1_ID_TYPE, CAN_USER_MASK_1_FILTER_TYPE);
  #endif
  
  #ifdef CAN_USER_MASK_2
   can_set_mask_id(CAN_FILTER_MASK_2, CAN_USER_MASK_2, CAN_USER_MASK_2_ID_TYPE, CAN_USER_MASK_2_FILTER_TYPE);
  #endif
  
  #ifdef CAN_USER_FILT_1
   can_set_filter_id(CAN_FILTER_1, CAN_USER_FILT_1, CAN_USER_FILT_1_TYPE);
   can_enable_filter(CAN_FILTER_1, CAN_USER_FILT_1_BUFFER, CAN_USER_FILT_1_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_2
   can_set_filter_id(CAN_FILTER_2, CAN_USER_FILT_2, CAN_USER_FILT_2_TYPE);
   can_enable_filter(CAN_FILTER_2, CAN_USER_FILT_2_BUFFER, CAN_USER_FILT_2_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_3
   can_set_filter_id(CAN_FILTER_3, CAN_USER_FILT_3, CAN_USER_FILT_3_TYPE);
   can_enable_filter(CAN_FILTER_3, CAN_USER_FILT_3_BUFFER, CAN_USER_FILT_3_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_4
   can_set_filter_id(CAN_FILTER_4, CAN_USER_FILT_4, CAN_USER_FILT_4_TYPE);
   can_enable_filter(CAN_FILTER_4, CAN_USER_FILT_4_BUFFER, CAN_USER_FILT_4_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_5
   can_set_filter_id(CAN_FILTER_5, CAN_USER_FILT_5, CAN_USER_FILT_5_TYPE);
   can_enable_filter(CAN_FILTER_5, CAN_USER_FILT_5_BUFFER, CAN_USER_FILT_5_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_6
   can_set_filter_id(CAN_FILTER_6, CAN_USER_FILT_6, CAN_USER_FILT_6_TYPE);
   can_enable_filter(CAN_FILTER_6, CAN_USER_FILT_6_BUFFER, CAN_USER_FILT_6_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_7
   can_set_filter_id(CAN_FILTER_7, CAN_USER_FILT_7, CAN_USER_FILT_7_TYPE);
   can_enable_filter(CAN_FILTER_7, CAN_USER_FILT_7_BUFFER, CAN_USER_FILT_7_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_8
   can_set_filter_id(CAN_FILTER_8, CAN_USER_FILT_8, CAN_USER_FILT_8_TYPE);
   can_enable_filter(CAN_FILTER_8, CAN_USER_FILT_8_BUFFER, CAN_USER_FILT_8_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_9
   can_set_filter_id(CAN_FILTER_9, CAN_USER_FILT_9, CAN_USER_FILT_9_TYPE);
   can_enable_filter(CAN_FILTER_9, CAN_USER_FILT_9_BUFFER, CAN_USER_FILT_9_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_10
   can_set_filter_id(CAN_FILTER_10, CAN_USER_FILT_10, CAN_USER_FILT_10_TYPE);
   can_enable_filter(CAN_FILTER_10, CAN_USER_FILT_10_BUFFER, CAN_USER_FILT_10_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_11
   can_set_filter_id(CAN_FILTER_11, CAN_USER_FILT_11, CAN_USER_FILT_11_TYPE);
   can_enable_filter(CAN_FILTER_11, CAN_USER_FILT_11_BUFFER, CAN_USER_FILT_11_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_12
   can_set_filter_id(CAN_FILTER_12, CAN_USER_FILT_12, CAN_USER_FILT_12_TYPE);
   can_enable_filter(CAN_FILTER_12, CAN_USER_FILT_12_BUFFER, CAN_USER_FILT_12_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_13
   can_set_filter_id(CAN_FILTER_13, CAN_USER_FILT_13, CAN_USER_FILT_13_TYPE);
   can_enable_filter(CAN_FILTER_13, CAN_USER_FILT_13_BUFFER, CAN_USER_FILT_13_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_14
   can_set_filter_id(CAN_FILTER_14, CAN_USER_FILT_14, CAN_USER_FILT_14_TYPE);
   can_enable_filter(CAN_FILTER_14, CAN_USER_FILT_14_BUFFER, CAN_USER_FILT_14_MASK);
  #endif
  
  #ifdef CAN_USER_FILT_15
   can_set_filter_id(CAN_FILTER_15, CAN_USER_FILT_15, CAN_USER_FILT_15_TYPE);
   can_enable_filter(CAN_FILTER_15, CAN_USER_FILT_15_BUFFER, CAN_USER_FILT_15_MASK);
  #endif
 #endif
 
  #if (getenv("DEVICE") == "PIC18F6585") || (getenv("DEVICE") == "PIC18LF6585") || \
      (getenv("DEVICE") == "PIC18F6680") || (getenv("DEVICE") == "PIC18LF6680") || \
      (getenv("DEVICE") == "PIC18F8585") || (getenv("DEVICE") == "PIC18LF8585") || \
      (getenv("DEVICE") == "PIC18F8680") || (getenv("DEVICE") == "PIC18LF8680")
   set_tris_g((*getenv("SFR:TRISG") & 0xFE) | 0x04);  //G2 in, G0 out
   if(CAN_ENABLE_CANTX2)
    bit_clear(*getenv("SFR:TRISG"), 1);  //G1 out
  #elif getenv("FUSE_SET:CANE")
   set_tris_e((*getenv("SFR:TRISE") & 0xDF) | 0x10);  //E4 in, E5 out
  #elif getenv("FUSE_SET:CANC")
   set_tris_c((*getenv("SFR:TRISC") & 0xBF) | 0x80);  //C7 in, C6 out
  #elif !defined(IS_ENH18)
   set_tris_b((*getenv("SFR:TRISB") & 0xFB) | 0x08);  //B3 in, B2 out
  #endif
  
  #ifdef IS_ENH18
   output_float(CAN_RX_PIN);
   output_drive(CAN_TX_PIN);     
  #endif
   
   can_set_mode(Mode);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_baud()
//
// Function for setting the bit rate of the CAN Bus.
//
// Parameters:
//    Clock - the clock speed that is presented to the ECAN peripheral, the
//            speed is the PIC's clock speed.
// 
//    Rate - the bit rate, max speed is 1 Mbps and speed must be less then or
//           then or equal to Clock and be a multiple of Clock.
//
// Return - can_ec_t error code, see can-pic18f_ecan.h for a description of
//          error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate)
{
   BRGCON BaudRate;
   uint8_t BRP = 0;
   uint16_t tBits;
   uint16_t tSeg1Bits;
   
   if((Clock % Rate) != 0)
      return(CAN_EC_BAUD_NOT_DIVISIBLE);
   
   do
   {
      BRP++;
      tBits = (Clock / (BRP * 2)) / Rate;
      
      if(tBits < 26)
      {
         if((Clock / (tBits * (BRP * 2))) != Rate)
            continue;
      }
      
      tSeg1Bits = (((uint32_t)tBits * CAN_DEFAULT_SAMPLE_POINT) / 1000) - 1;
   } while((((tBits > 25) || (tBits < 8)) && (BRP < 64)) || ((BRP < 64) && (tSeg1Bits > 16)));
   
   if((tBits > 25) || (tBits < 8))
      return(CAN_EC_BAUD_INVALID);
   
   BaudRate.brp = BRP - 1;
   
   if(tSeg1Bits > 17)
   {
      BaudRate.prseg = 7;
      BaudRate.seg1ph = 7;
      
      tSeg1Bits = 17;
   }
   else
   {
      while((tSeg1Bits < (tBits - tSeg1Bits - 1)) || (tSeg1Bits < 2))
         tSeg1Bits++;
   
      BaudRate.prseg = (tSeg1Bits / 2) - 1;
      BaudRate.seg1ph = (tSeg1Bits - (tSeg1Bits / 2))  - 1;
   }
   
   BaudRate.seg2ph = (tBits - tSeg1Bits) - 2;
   
   BaudRate.sam = CAN_BRG_SAM;
   BaudRate.seg2phts = 1;   //Phase Segment 2 Freely programmable
   BaudRate.wakfil = CAN_BRG_WAKE_FILTER;
   BaudRate.wakdis = (CAN_BRG_WAKEUP != TRUE);
   
   if(BaudRate.seg2ph == 0)
      BaudRate.sjw = 0;
   else if(BaudRate.seg2ph < 4)
      BaudRate.sjw = BaudRate.seg2ph - 1;
   else
      BaudRate.sjw = 3;
   
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   memcpy(&CAN_BRGCON, &BaudRate, sizeof(BRGCON));
   
   can_set_mode(g_CANOpmode, FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_mode()
//
// Set the operational mode of the ECAN peripheral.
//
// Parameters:
//    Mode - the CAN_OP_MODE to change to, can be one of the following:
//             CAN_OP_NORMAL
//             CAN_OP_DISABLE
//             CAN_OP_LOOPBACK
//             CAN_OP_LISTEN
//             CAN_OP_CONFIG
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
   if(CAN_CANSTAT_MODE0.opmode != Mode)
   {
      CAN_CANCON_MODE0.reqop = Mode;
      
      while(CAN_CANSTAT_MODE0.opmode != Mode);
   }
   
   if(Update)
      g_CANOpmode = Mode;
}

///////////////////////////////////////////////////////////////////////////////
// can_set_filter_id()
//
// Used to set the specified Filter Id and Type.
//
// Parameters:
//    Filter - the filter to setup.
//
//    Id - the Id to assign to the filter.
//
//    Type - the filter type.  This specifies if the Id is a SID or EID, which
//            controls how the Id is written to the Filter's registers.
//            Additionally if the mask that is assigned to be used by this 
//            filter is set to receive only SID or EID messages then this
//            controls what type of messages can be received by this filter.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type)
{
   uint16_t *Address;
   
   switch(Filter)
   {
      case CAN_FILTER_0:
         Address = &CAN_RXF0ID;
         break;
      case CAN_FILTER_1:
         Address = &CAN_RXF1ID;
         break;
      case CAN_FILTER_2:
         Address = &CAN_RXF2ID;
         break;
      case CAN_FILTER_3:
         Address = &CAN_RXF3ID;
         break;
      case CAN_FILTER_4:
         Address = &CAN_RXF4ID;
         break;
      case CAN_FILTER_5:
         Address = &CAN_RXF5ID;
         break;
      case CAN_FILTER_6:
         Address = &CAN_RXF6ID;
         break;
      case CAN_FILTER_7:
         Address = &CAN_RXF7ID;
         break;
      case CAN_FILTER_8:
         Address = &CAN_RXF8ID;
         break;
      case CAN_FILTER_9:
         Address = &CAN_RXF9ID;
         break;
      case CAN_FILTER_10:
         Address = &CAN_RXF10ID;
         break;
      case CAN_FILTER_11:
         Address = &CAN_RXF11ID;
         break;
      case CAN_FILTER_12:
         Address = &CAN_RXF12ID;
         break;
      case CAN_FILTER_13:
         Address = &CAN_RXF13ID;
         break;
      case CAN_FILTER_14:
         Address = &CAN_RXF14ID;
         break;
      case CAN_FILTER_15:
         Address = &CAN_RXF15ID;
         break;
      default:
         return;
         break;
   };
   
   can_set_id(Address, Id, Type, Type);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_mask_id()
//
// Used to set the specified Filter Mask's Id, Id Type and Mask Type.
//
// Parameters:
//    Mask - the filter mask to setup.
//
//    Id - the Id to assign to the filter mask, setting a bit means the
//         corresponding bit of the received message Id must match corresponding
//         bit of the Filter's Id.
//
//    iType - the filter mask Id type.  This basically just specifies if the
//            Id is a SID or EID, which controls how the Id is written to the
//            Filter Mask's registers.
//
//    fType - the mask type.  This controls whether a filter assigned to use 
//            this mask can receive both SID and EID messages or only SID or
//            EID.  If set to receive only SID or EID messages the type
//            parameter passed to the can_set_filter_id() controls which it 
//            will receive.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType)
{
   uint16_t *Address;
   
   switch(Mask)
   {
      case CAN_FILTER_MASK_0:
         Address = &CAN_RXM0ID;
         break;
      case CAN_FILTER_MASK_1:
         Address = &CAN_RXM1ID;
         break;
      case CAN_FILTER_MASK_2:
         Address = &CAN_RXM2ID;
         break;
      default:
         return;
         break;
   }
   
   can_set_id(Address, Id, iType, fType);
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_filter()
//
// Enables a given acceptance filter, assigns it to the specified buffer and 
// associates the specified mask to it.
//
// Parameters:
//    Filter - the filter that is to be enabled.
//
//    Buffer - the buffer the filter is to be assigned to.
//
//    Mask - that acceptance filter mask to associate with the filter.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer, CAN_FILTER_MASK Mask)
{
   if(Filter <= CAN_FILTER_15)
   {
      can_set_mode(CAN_OP_CONFIG, FALSE);
      
      //Assign Filter to Buffer
     #if CAN_USE_RX_FIFO == FALSE
      if(Buffer == CAN_FILTER_BUFFER_ALL)
      {
         can_set_buffer_filter(Filter, CAN_FILTER_BUFFER_RXB0);
         can_set_buffer_filter(Filter, CAN_FILTER_BUFFER_RXB1);
      
         for(Buffer=CAN_FILTER_BUFFER_B0;Buffer<=CAN_FILTER_BUFFER_B5;Buffer++)
         {
            if(bit_test(CAN_BSEL0.b, Buffer) == 0)
               can_set_buffer_filter(Filter, Buffer);
         }
      }
      else
         can_set_buffer_filter(Filter, Buffer);
     #else
      if(Buffer == CAN_FILTER_BUFFER_FIFO)
         can_set_buffer_filter(Filter, CAN_FILTER_BUFFER_RXB0);
      else
         can_set_buffer_filter(Filter, Buffer);
     #endif
     
      //Assign Mask to Filter
      can_set_filter_mask(Filter, Mask);
      
      //Enable Filter
      bit_set(CAN_RXFCON.w, Filter);
      
      can_set_mode(g_CANOpmode, FALSE);
   }
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_filter()
//
// Disables a given acceptance filter.
//
// Parameters:
//    Filter - the filter that is to be disabled.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_filter(CAN_FILTER Filter)
{
   bit_clear(CAN_RXFCON.w, Filter);
}

///////////////////////////////////////////////////////////////////////////////
// can_putd()
//
// Function used to load a message into a transmit buffer, after which when the
// CAN Bus becomes available it will be transmitted.
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, e.g. the message ID.  See
//             CAN_TX_HEADER type in can-pic18f_ecan.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Buffer - optional specifies the buffer to load the message to transmit
//             into.  Defaults to CAN_TX_BUFFER_ANY, meaning messages will be 
//             loaded into first available TX buffer that is free and not setup
//             as an RTR TX buffer.
//
// Returns - can_ec_t error code, see can-pic18f_ecan.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
{
   can_ec_t Result = CAN_EC_OK;
   
   if(Buffer == CAN_TX_BUFFER_ANY)
   {
      //find empty transmitter
      if(!CAN_TXB0CON.txreq)
         Buffer = CAN_BUFFER_TXB0;
      else if(!CAN_TXB1CON.txreq)
         Buffer = CAN_BUFFER_TXB1;
      else if(!CAN_TXB2CON.txreq)
         Buffer = CAN_BUFFER_TXB2;
      else if(CAN_BSEL0.b0txen && !CAN_B0CON_TX.txreq && !CAN_B0CON_TX.rtren)
         Buffer = CAN_BUFFER_B0;
      else if(CAN_BSEL0.b1txen && !CAN_B1CON_TX.txreq && !CAN_B1CON_TX.rtren)
         Buffer = CAN_BUFFER_B1;
      else if(CAN_BSEL0.b2txen && !CAN_B2CON_TX.txreq && !CAN_B2CON_TX.rtren)
         Buffer = CAN_BUFFER_B2;
      else if(CAN_BSEL0.b3txen && !CAN_B3CON_TX.txreq && !CAN_B3CON_TX.rtren)
         Buffer = CAN_BUFFER_B3;
      else if(CAN_BSEL0.b4txen && !CAN_B4CON_TX.txreq && !CAN_B4CON_TX.rtren)
         Buffer = CAN_BUFFER_B4;
      else if(CAN_BSEL0.b5txen && !CAN_B5CON_TX.txreq && !CAN_B5CON_TX.rtren)
         Buffer = CAN_BUFFER_B5;
      else
         Result = CAN_EC_BUFFER_TX_FULL;
   }
   else
   {
      switch(Buffer)
      {
         case CAN_BUFFER_TXB0:
            if(CAN_TXB0CON.txreq)
               Result = CAN_EC_BUFFER_TX_FULL;
            break;
         case CAN_BUFFER_TXB1:
            if(CAN_TXB1CON.txreq)
               Result = CAN_EC_BUFFER_TX_FULL;
            break;
         case CAN_BUFFER_TXB2:
            if(CAN_TXB2CON.txreq)
               Result = CAN_EC_BUFFER_TX_FULL;
            break;
         case CAN_BUFFER_B0:
            if(CAN_BSEL0.b0txen)
            {
               if(!CAN_B0CON_TX.txreq)
               {
                  if(CAN_B0CON_TX.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_B1:
            if(CAN_BSEL0.b1txen)
            {
               if(!CAN_B1CON_TX.txreq)
               {
                  if(CAN_B1CON_TX.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_B2:
            if(CAN_BSEL0.b2txen)
            {
               if(!CAN_B2CON_TX.txreq)
               {
                  if(CAN_B2CON_TX.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_B3:
            if(CAN_BSEL0.b3txen)
            {
               if(!CAN_B3CON_TX.txreq)
               {
                  if(CAN_B3CON_TX.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_B4:
            if(CAN_BSEL0.b4txen)
            {
               if(!CAN_B4CON_TX.txreq)
               {
                  if(CAN_B4CON_TX.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_B5:
            if(CAN_BSEL0.b5txen)
            {
               if(!CAN_B5CON_TX.txreq)
               {
                  if(CAN_B5CON_TX.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         
      }
   }
   
   if(Result == CAN_EC_OK)
   {
      can_set_buffer_id(Header, Buffer);
      memcpy(can_buffer_data_address(Buffer), Data, Header->Length);
      
      switch(Buffer)
      {
         case CAN_BUFFER_TXB0:
            CAN_TXB0CON.txpri = Header->Priority;
            CAN_TXB0CON.txreq = TRUE;
            break;
         case CAN_BUFFER_TXB1:
            CAN_TXB1CON.txpri = Header->Priority;
            CAN_TXB1CON.txreq = TRUE;
            break;
         case CAN_BUFFER_TXB2:
            CAN_TXB2CON.txpri = Header->Priority;
            CAN_TXB2CON.txreq = TRUE;
            break;
         case CAN_BUFFER_B0:
            CAN_B0CON_TX.txpri = Header->Priority;
            CAN_B0CON_TX.txreq = TRUE;
            break;
         case CAN_BUFFER_B1:
            CAN_B1CON_TX.txpri = Header->Priority;
            CAN_B1CON_TX.txreq = TRUE;
            break;
         case CAN_BUFFER_B2:
            CAN_B2CON_TX.txpri = Header->Priority;
            CAN_B2CON_TX.txreq = TRUE;
            break;
         case CAN_BUFFER_B3:
            CAN_B3CON_TX.txpri = Header->Priority;
            CAN_B3CON_TX.txreq = TRUE;
            break;
         case CAN_BUFFER_B4:
            CAN_B4CON_TX.txpri = Header->Priority;
            CAN_B4CON_TX.txreq = TRUE;
            break;
         case CAN_BUFFER_B5:
            CAN_B5CON_TX.txpri = Header->Priority;
            CAN_B5CON_TX.txreq = TRUE;
            break;
      }
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_getd()
//
// Function used to get message from specified RX Buffer.
//
// Parameters:
//    Header - pointer to CAN_RX_HEADER variable to store info about the
//             retrieved message, e.g. the message ID.  See CAN_RX_HEADER type
//             in can_pic18f_ecan.h for more info.
//
//    Data - pointer to an uint8_t array to return data portion of message to.
//
//    Buffer - optional specifies the buffer to get message from.  Can be 
//             CAN_BUFFER_RXB0, CAN_BUFFER_RXB1, CAN_BUFFER_B0 to CAN_BUFFER_B5
//             or CAN_RX_BUFFER_ANY.  This option is only used when driver sets 
//             up CAN for Enhanced Legacy mode (Mode 1), CAN_USE_RX_FIFO is
//             defined to FALSE.  If Mode 1 is used this parameter defaults to
//             CAN_RX_BUFFER_ANY if not specified.
//
// Returns - can_ec_t error code, see can-pic18f_ecan.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer=CAN_RX_BUFFER_ANY)
{
   can_ec_t Result = CAN_EC_OK;
   BnCON_RX rBnCON;
  #ifdef IS_ENH18
   #locate rBnCON = 0x3F80
  #else
   #locate rBnCON = 0xF60
  #endif
   
  #if CAN_USE_RX_FIFO == TRUE
   if(CAN_COMSTAT_MODE2.fifoempty)
   {
      Buffer = CAN_CANCON_MODE2.fp;
      
      if(Buffer > 1)
         Buffer += 3;
   }
   else
      Result = CAN_EC_BUFFER_RX_EMPTY;
  #else
   if(Buffer == CAN_RX_BUFFER_ANY)
   {
      if(CAN_RXB0CON_MODE_1.rxful)
         Buffer = CAN_BUFFER_RXB0;
      else if(CAN_RXB1CON_MODE_1.rxful)
         Buffer = CAN_BUFFER_RXB1;
      else if(!CAN_BSEL0.b0txen && CAN_B0CON_RX.rxful)
         Buffer = CAN_BUFFER_B0;
      else if(!CAN_BSEL0.b1txen && CAN_B1CON_RX.rxful)
         Buffer = CAN_BUFFER_B1;
      else if(!CAN_BSEL0.b2txen && CAN_B2CON_RX.rxful)
         Buffer = CAN_BUFFER_B2;
      else if(!CAN_BSEL0.b3txen && CAN_B3CON_RX.rxful)
         Buffer = CAN_BUFFER_B3;
      else if(!CAN_BSEL0.b4txen && CAN_B4CON_RX.rxful)
         Buffer = CAN_BUFFER_B4;
      else if(!CAN_BSEL0.b5txen && CAN_B5CON_RX.rxful)
         Buffer = CAN_BUFFER_B5;
      else
         Result = CAN_EC_BUFFER_RX_EMPTY;
   }
   else
   {
      switch(Buffer)
      {
         case CAN_BUFFER_RXB0:
            if(!CAN_RXB0CON_MODE_1.rxful)
               Result = CAN_EC_BUFFER_RX_EMPTY;
            break;
         case CAN_BUFFER_RXB1:
            if(!CAN_RXB1CON_MODE_1.rxful)
               Result = CAN_EC_BUFFER_RX_EMPTY;
            break;
         case CAN_BUFFER_B0:
            if(!CAN_BSEL0.b0txen)
            {
               if(!CAN_B0CON_RX.rxful)
                  Result = CAN_EC_BUFFER_RX_EMPTY;
            }
            else
               Result = CAN_EC_BUFFER_NOT_RX;
            break;
         case CAN_BUFFER_B1:
            if(!CAN_BSEL0.b1txen)
            {
               if(!CAN_B1CON_RX.rxful)
                  Result = CAN_EC_BUFFER_RX_EMPTY;
            }
            else
               Result = CAN_EC_BUFFER_NOT_RX;
            break;
         case CAN_BUFFER_B2:
            if(!CAN_BSEL0.b2txen)
            {
               if(!CAN_B2CON_RX.rxful)
                  Result = CAN_EC_BUFFER_RX_EMPTY;
            }
            else
               Result = CAN_EC_BUFFER_NOT_RX;
            break;
         case CAN_BUFFER_B3:
            if(!CAN_BSEL0.b3txen)
            {
               if(!CAN_B3CON_RX.rxful)
                  Result = CAN_EC_BUFFER_RX_EMPTY;
            }
            else
               Result = CAN_EC_BUFFER_NOT_RX;
            break;
         case CAN_BUFFER_B4:
            if(!CAN_BSEL0.b4txen)
            {
               if(!CAN_B4CON_RX.rxful)
                  Result = CAN_EC_BUFFER_RX_EMPTY;
            }
            else
               Result = CAN_EC_BUFFER_NOT_RX;
            break;
         case CAN_BUFFER_B5:
            if(!CAN_BSEL0.b5txen)
            {
               if(!CAN_B5CON_RX.rxful)
                  Result = CAN_EC_BUFFER_RX_EMPTY;
            }
            else
               Result = CAN_EC_BUFFER_NOT_RX;
            break;
      }
   }
  #endif
   
   if(Result == CAN_EC_OK)
   {
      Header->Buffer = Buffer;
      
      if(Buffer <= CAN_BUFFER_RXB1)
         CAN_ECANCON.ewin = 0x10 | Buffer;
      else
         CAN_ECANCON.ewin = 0x10 | (Buffer - 3);
     
      can_get_rx_data(Header, Data);
      
      CAN_COMSTAT_MODE2.rxovfl = FALSE;
      rBnCON.rxful = FALSE;
      
      CAN_INT_RXB1IF = 0;
      CAN_INT_IRXIF = 0;
      
      CAN_ECANCON.ewin = 0;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_kbhit()
//
// Used to determine if there is at least one message to retrieve from the 
// specified buffer.
//
// Parameters:
//    Buffer - optional specifies the CAN RX Buffer to check for message.  Can
//             be CAN_BUFFER_RXB0, CAN_BUFFER_RXB1, CAN_BUFFER_B0 to
//             CAN_BUFFER_B0, or CAN_RX_BUFFER_ANY.  This option is only used
//             when driver sets up CAN for Enhanced Legacy mode (Mode 1),
//             CAN_USE_RX_FIFO is defined to FALSE.  If Mode 1 is used this
//             parameter defaults to CAN_RX_BUFFER_ANY if not specified.
//
// Returns:
//    TRUE - if specified buffer has new message to retrieve.
//    FALSE - if specified buffer does not have new message to retrieve.
///////////////////////////////////////////////////////////////////////////////
int1 can_kbhit(CAN_BUFFER Buffer = CAN_RX_BUFFER_ANY)
{
  #if CAN_USE_RX_FIFO == TRUE
   if(CAN_COMSTAT_MODE2.fifoempty)
      return(TRUE);
   else
      return(FALSE);
  #else
   int1 Result = FALSE;
   
   if(Buffer == CAN_RX_BUFFER_ANY)
   {
      if(CAN_RXB0CON_MODE_1.rxful)
         Result = TRUE;
      else if(CAN_RXB1CON_MODE_1.rxful)
         Result = TRUE;
      else if(!CAN_BSEL0.b0txen && CAN_B0CON_RX.rxful)
         Result = TRUE;
      else if(!CAN_BSEL0.b1txen && CAN_B1CON_RX.rxful)
         Result = TRUE;
      else if(!CAN_BSEL0.b2txen && CAN_B2CON_RX.rxful)
         Result = TRUE;
      else if(!CAN_BSEL0.b3txen && CAN_B3CON_RX.rxful)
         Result = TRUE;
      else if(!CAN_BSEL0.b4txen && CAN_B4CON_RX.rxful)
         Result = TRUE;
      else if(!CAN_BSEL0.b5txen && CAN_B5CON_RX.rxful)
         Result = TRUE;
   }
   else
   {
      switch(Buffer)
      {
         case CAN_BUFFER_RXB0:
            if(CAN_RXB0CON_MODE_1.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_RXB1:
            if(CAN_RXB1CON_MODE_1.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_B0:
            if(!CAN_BSEL0.b0txen && CAN_B0CON_RX.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_B1:
            if(!CAN_BSEL0.b1txen && CAN_B1CON_RX.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_B2:
            if(!CAN_BSEL0.b2txen && CAN_B2CON_RX.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_B3:
            if(!CAN_BSEL0.b3txen && CAN_B3CON_RX.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_B4:
            if(!CAN_BSEL0.b4txen && CAN_B4CON_RX.rxful)
               Result = TRUE;
            break;
         case CAN_BUFFER_B5:
            if(!CAN_BSEL0.b5txen && CAN_B5CON_RX.rxful)
               Result = TRUE;
            break;
      }
   }
   
   return(Result);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// can_tbe()
//
// Used to determine if specified TX buffer is not full.
//
// Parameters:
//    Buffer - optional used to specify the TX buffer to check.  Default is 
//             CAN_TX_BUFFER_ANY, meaning it checks all the TX buffers and if
//             at least one if not full it will return TRUE.
//
// Returns:
//    TRUE - TX buffer is not full.
//    FALSE - TX buffer is full.
///////////////////////////////////////////////////////////////////////////////
int1 can_tbe(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
{
   int1 Result = FALSE;
   
   if(Buffer == CAN_TX_BUFFER_ANY)
   {
      if(!CAN_TXB0CON.txreq || !CAN_TXB1CON.txreq || !CAN_TXB2CON.txreq ||
         (CAN_BSEL0.b0txen && !CAN_B0CON_TX.txreq && !CAN_B0CON_TX.rtren) || (CAN_BSEL0.b1txen && !CAN_B1CON_TX.txreq && !CAN_B1CON_TX.rtren) ||
         (CAN_BSEL0.b2txen && !CAN_B2CON_TX.txreq && !CAN_B2CON_TX.rtren) || (CAN_BSEL0.b3txen && !CAN_B3CON_TX.txreq && !CAN_B3CON_TX.rtren) ||
         (CAN_BSEL0.b4txen && !CAN_B4CON_TX.txreq && !CAN_B4CON_TX.rtren) || (CAN_BSEL0.b5txen && !CAN_B5CON_TX.txreq && !CAN_B5CON_TX.rtren))
         Result = TRUE;
   }
   else
   {
      switch(Buffer)
      {
         case CAN_BUFFER_TXB0:
            if(!CAN_TXB0CON.txreq)
               Result = TRUE;
            break;
         case CAN_BUFFER_TXB1:
            if(!CAN_TXB1CON.txreq)
               Result = TRUE;
            break;
         case CAN_BUFFER_TXB2:
            if(!CAN_TXB2CON.txreq)
               Result = TRUE;
            break;
         case CAN_BUFFER_B0:
            if(CAN_BSEL0.b0txen && !CAN_B0CON_TX.txreq && !CAN_B0CON_TX.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_B1:
            if(CAN_BSEL0.b1txen && !CAN_B1CON_TX.txreq && !CAN_B1CON_TX.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_B2:
            if(CAN_BSEL0.b2txen && !CAN_B2CON_TX.txreq && !CAN_B2CON_TX.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_B3:
            if(CAN_BSEL0.b3txen && !CAN_B3CON_TX.txreq && !CAN_B3CON_TX.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_B4:
            if(CAN_BSEL0.b4txen && !CAN_B4CON_TX.txreq && !CAN_B4CON_TX.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_B5:
            if(CAN_BSEL0.b5txen && !CAN_B5CON_TX.txreq && !CAN_B5CON_TX.rtren)
               Result = TRUE;
            break;
      }
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_tx_empty()
//
// Used to determine if the specified TX buffer is empty.
//
// Parameters:
//    Buffer - optional TX buffer to check.  Default is CAN_TX_BUFFER_ALL, all
//             TX buffers must be empty to return TRUE.
//
// Returns:
//    TRUE - specified TX buffer is empty.
//    FALSE - specified TX buffer is not empty.
///////////////////////////////////////////////////////////////////////////////
int1 can_tx_empty(CAN_BUFFER Buffer = CAN_TX_BUFFER_ALL)
{
   int1 Result = FALSE;
   
   if(Buffer == CAN_TX_BUFFER_ALL)
   {
      if(!CAN_TXB0CON.txreq && !CAN_TXB1CON.txreq && !CAN_TXB2CON.txreq &&
         (!CAN_BSEL0.b0txen || CAN_B0CON_TX.rtren || !CAN_B0CON_TX.txreq) && (!CAN_BSEL0.b1txen || CAN_B1CON_TX.rtren || !CAN_B1CON_TX.txreq) &&
         (!CAN_BSEL0.b2txen || CAN_B2CON_TX.rtren || !CAN_B2CON_TX.txreq) && (!CAN_BSEL0.b3txen || CAN_B3CON_TX.rtren || !CAN_B3CON_TX.txreq) &&
         (!CAN_BSEL0.b4txen || CAN_B4CON_TX.rtren || !CAN_B4CON_TX.txreq) && (!CAN_BSEL0.b5txen || CAN_B5CON_TX.rtren || !CAN_B5CON_TX.txreq))
         Result = TRUE;
   }
   else
      Result = can_tbe(Buffer);
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_abort()
//
// Used to abort pending transmissions for specified TX buffer.
//
// Parameters:
//    Buffer - optional specifying the buffer to abort pending transmissions
//             for, default is CAN_ABORT_ALL to abort all pending
//             transmissions.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_abort(CAN_BUFFER Buffer = CAN_ABORT_ALL)
{
   switch(Buffer)
   {
      case CAN_BUFFER_TXB0:
         CAN_TXB0CON.txreq = 0;
         break;
      case CAN_BUFFER_TXB1:
         CAN_TXB1CON.txreq = 0;
         break;
      case CAN_BUFFER_TXB2:
         CAN_TXB2CON.txreq = 0;
         break;
      case CAN_BUFFER_B0:
         if(CAN_BSEL0.b0txen)
            CAN_B0CON_TX.txreq = 0;
         break;
      case CAN_BUFFER_B1:
         if(CAN_BSEL0.b1txen)
            CAN_B1CON_TX.txreq = 0;
         break;
      case CAN_BUFFER_B2:
         if(CAN_BSEL0.b2txen)
            CAN_B2CON_TX.txreq = 0;
         break;
      case CAN_BUFFER_B3:
         if(CAN_BSEL0.b3txen)
            CAN_B3CON_TX.txreq = 0;
         break;
      case CAN_BUFFER_B4:
         if(CAN_BSEL0.b4txen)
            CAN_B4CON_TX.txreq = 0;
         break;
      case CAN_BUFFER_B5:
         if(CAN_BSEL0.b5txen)
            CAN_B5CON_TX.txreq = 0;
         break;
      case CAN_ABORT_ALL:
         CAN_CANCON_MODE2.abat = 1;
         break;
   }
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_rtr()
//
// Used to enable RTR messages on one of the TX buffers.
//
// Parameters:
//    Buffer - the buffer to enable as an RTR buffer.
//
// Returns - can_ec_t error code, see can-pic18f_ecan.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_enable_rtr(CAN_BUFFER Buffer)
{
   can_ec_t Result = CAN_EC_OK;
   
   switch(Buffer)
   {
      case CAN_BUFFER_B0:
         if(CAN_BSEL0.b0txen)
            CAN_B0CON_TX.rtren = TRUE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B1:
         if(CAN_BSEL0.b1txen)
            CAN_B1CON_TX.rtren = TRUE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B2:
         if(CAN_BSEL0.b2txen)
            CAN_B2CON_TX.rtren = TRUE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B3:
         if(CAN_BSEL0.b3txen)
            CAN_B3CON_TX.rtren = TRUE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B4:
         if(CAN_BSEL0.b4txen)
            CAN_B4CON_TX.rtren = TRUE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B5:
         if(CAN_BSEL0.b5txen)
            CAN_B5CON_TX.rtren = TRUE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_TXB0:
      case CAN_BUFFER_TXB1:
      case CAN_BUFFER_TXB2:
         Result = CAN_EC_BUFFER_NO_RTR;
         break;
      default:
         Result = CAN_EC_BUFFER_NOT_TX;
         break;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_rtr()
//
// Used to disable RTR messages on one of the TX buffers.
//
// Parameters:
//    Buffer - the CAN TX Buffer to disable RTR messages for.
//
// Returns - can_ec_t error code, see can-pic18f_ecan.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_disable_rtr(CAN_BUFFER Buffer)
{
   can_ec_t Result = CAN_EC_OK;
   
   switch(Buffer)
   {
      case CAN_BUFFER_B0:
         if(CAN_BSEL0.b0txen)
            CAN_B0CON_TX.rtren = FALSE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B1:
         if(CAN_BSEL0.b1txen)
            CAN_B1CON_TX.rtren = FALSE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B2:
         if(CAN_BSEL0.b2txen)
            CAN_B2CON_TX.rtren = FALSE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B3:
         if(CAN_BSEL0.b3txen)
            CAN_B3CON_TX.rtren = FALSE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B4:
         if(CAN_BSEL0.b4txen)
            CAN_B4CON_TX.rtren = FALSE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B5:
         if(CAN_BSEL0.b5txen)
            CAN_B5CON_TX.rtren = FALSE;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_TXB0:
      case CAN_BUFFER_TXB1:
      case CAN_BUFFER_TXB2:
         Result = CAN_EC_BUFFER_NO_RTR;
         break;
      default:
         Result = CAN_EC_BUFFER_NOT_TX;
         break;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_load_rtr()
//
// Used to load a message into a TX buffer that will be transmitted when an
// remote transmission request is received. 
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, e.g. the message ID.  See
//             CAN_TX_HEADER type in can_pic18f_ecan.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Buffer - The CAN TX buffer to load the message into.  Only programmable 
//             RX/TX buffers set as a TX buffer can be used to responded to
//             remote transmission requests.
//
//
// Returns - can_ec_t error code, see can_pic18f_ecan.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_load_rtr(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer)
{
   can_ec_t Result = CAN_EC_OK;
   
   switch(Buffer)
   {
      case CAN_BUFFER_B0:
         if(CAN_BSEL0.b0txen)
         {
            if(CAN_B0CON_TX.rtren)
               CAN_B0CON_TX.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B1:
         if(CAN_BSEL0.b1txen)
         {
            if(CAN_B1CON_TX.rtren)
               CAN_B1CON_TX.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B2:
         if(CAN_BSEL0.b2txen)
         {
            if(CAN_B2CON_TX.rtren)
               CAN_B2CON_TX.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B3:
         if(CAN_BSEL0.b3txen)
         {
            if(CAN_B3CON_TX.rtren)
               CAN_B3CON_TX.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B4:
         if(CAN_BSEL0.b4txen)
         {
            if(CAN_B4CON_TX.rtren)
               CAN_B4CON_TX.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_B5:
         if(CAN_BSEL0.b5txen)
         {
            if(CAN_B5CON_TX.rtren)
               CAN_B5CON_TX.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_TXB0:
      case CAN_BUFFER_TXB1:
      case CAN_BUFFER_TXB2:
         Result = CAN_EC_BUFFER_NO_RTR;
         break;
      default:
         Result = CAN_EC_BUFFER_NOT_TX;
         break;
   }
   
   if(Result == CAN_EC_OK)
   {
      can_set_buffer_id(Header, Buffer);
      memcpy(can_buffer_data_address(Buffer), Data, Header->Length);
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_interrupts()
//
//  Used to enable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to enable, can be the following:
//       CAN_INTERRUPT_RXB0
//       CAN_INTERRUPT_RXB1
//       CAN_INTERRUPT_B0
//       CAN_INTERRUPT_B1
//       CAN_INTERRUPT_B2
//       CAN_INTERRUPT_B3
//       CAN_INTERRUPT_B4
//       CAN_INTERRUPT_B5
//       CAN_INTERRUPT_TXB0
//       CAN_INTERRUPT_TXB1
//       CAN_INTERRUPT_TXB2
//
//    You can or these together (via |) to enable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_interrupts(CAN_INTERRUPT Setting)
{
   if(make8(Setting, 0) != 0)
      CAN_BIE0.b |= make8(Setting, 0);
   
   if(make8(Setting, 1) != 0)
      CAN_TXBIE.b |= make8(Setting, 1);
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_interrupts()
//
//  Used to disable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to enable, can be the following:
//       CAN_INTERRUPT_RXB0
//       CAN_INTERRUPT_RXB1
//       CAN_INTERRUPT_B0
//       CAN_INTERRUPT_B1
//       CAN_INTERRUPT_B2
//       CAN_INTERRUPT_B3
//       CAN_INTERRUPT_B4
//       CAN_INTERRUPT_B5
//       CAN_INTERRUPT_TXB0
//       CAN_INTERRUPT_TXB1
//       CAN_INTERRUPT_TXB2
//
//    You can or these together (via |) to enable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_interrupts(CAN_INTERRUPT Setting)
{
   if(make8(Setting, 0) != 0)
      CAN_BIE0.b &= ~make8(Setting, 0);
   
   if(make8(Setting, 1) != 0)
      CAN_TXBIE.b &= ~make8(Setting, 1);
}

///////////////////////////////////////////////////////////////////////////////

void can_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type)
{
   if(Ext)  //Extended Id
   {
      //eid
      ((TXBnID *)Addr)->eid0_7 = make8(Id, 0);                 //eid 0-7
      ((TXBnID *)Addr)->eid8_15 = make8(Id, 1);                //eid 8-15
      ((TXBnID *)Addr)->eid16_17 = (make8(Id, 2) & 0x03);      //eid 16-17
      
      //sid
      ((TXBnID *)Addr)->sid0_2 = ((make8(Id, 2) >> 2) & 0x07); //sid 0-2
      ((TXBnID *)Addr)->sid3_10 = ((make8(Id, 2) >> 5) | ((make8(Id, 3) & 0x1F) << 3));   //sid 3-10
      
      ((TXBnID *)Addr)->exide = Type;
   }
   else     //Standard Id
   {
      //eid
      memset(Addr, 0x00, sizeof(TXBnID));
      
      //sid
      ((TXBnID *)Addr)->sid0_2 = (make8(Id, 0) & 0x07);
      ((TXBnID *)Addr)->sid3_10 = ((make8(Id, 0) >> 3) | ((make8(Id, 1) & 0x07) << 5));
      
      ((TXBnID *)Addr)->exide = Type;
   }
}

void can_set_buffer_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer)
{
   switch(Filter)
   {
      case CAN_FILTER_0:
         CAN_RXFBCON.f0bp = Buffer;
         break;
      case CAN_FILTER_1:
         CAN_RXFBCON.f1bp = Buffer;
         break;
      case CAN_FILTER_2:
         CAN_RXFBCON.f2bp = Buffer;
         break;
      case CAN_FILTER_3:
         CAN_RXFBCON.f3bp = Buffer;
         break;
      case CAN_FILTER_4:
         CAN_RXFBCON.f4bp = Buffer;
         break;
      case CAN_FILTER_5:
         CAN_RXFBCON.f5bp = Buffer;
         break;
      case CAN_FILTER_6:
         CAN_RXFBCON.f6bp = Buffer;
         break;
      case CAN_FILTER_7:
         CAN_RXFBCON.f7bp = Buffer;
         break;
      case CAN_FILTER_8:
         CAN_RXFBCON.f8bp = Buffer;
         break;
      case CAN_FILTER_9:
         CAN_RXFBCON.f9bp = Buffer;
         break;
      case CAN_FILTER_10:
         CAN_RXFBCON.f10bp = Buffer;
         break;
      case CAN_FILTER_11:
         CAN_RXFBCON.f11bp = Buffer;
         break;
      case CAN_FILTER_12:
         CAN_RXFBCON.f12bp = Buffer;
         break;
      case CAN_FILTER_13:
         CAN_RXFBCON.f13bp = Buffer;
         break;
      case CAN_FILTER_14:
         CAN_RXFBCON.f14bp = Buffer;
         break;
      case CAN_FILTER_15:
         CAN_RXFBCON.f15bp = Buffer;
         break;
   }
}

void can_set_filter_mask(CAN_FILTER Filter, CAN_FILTER_MASK Mask)
{
   switch(Filter)
   {
      case CAN_FILTER_0:
         CAN_MSEL.fil0 = Mask;
         break;
      case CAN_FILTER_1:
         CAN_MSEL.fil1 = Mask;
         break;
      case CAN_FILTER_2:
         CAN_MSEL.fil2 = Mask;
         break;
      case CAN_FILTER_3:
         CAN_MSEL.fil3 = Mask;
         break;
      case CAN_FILTER_4:
         CAN_MSEL.fil4 = Mask;
         break;
      case CAN_FILTER_5:
         CAN_MSEL.fil5 = Mask;
         break;
      case CAN_FILTER_6:
         CAN_MSEL.fil6 = Mask;
         break;
      case CAN_FILTER_7:
         CAN_MSEL.fil7 = Mask;
         break;
      case CAN_FILTER_8:
         CAN_MSEL.fil8 = Mask;
         break;
      case CAN_FILTER_9:
         CAN_MSEL.fil9 = Mask;
         break;
      case CAN_FILTER_10:
         CAN_MSEL.fil10 = Mask;
         break;
      case CAN_FILTER_11:
         CAN_MSEL.fil11 = Mask;
         break;
      case CAN_FILTER_12:
         CAN_MSEL.fil12 = Mask;
         break;
      case CAN_FILTER_13:
         CAN_MSEL.fil13 = Mask;
         break;
      case CAN_FILTER_14:
         CAN_MSEL.fil14 = Mask;
         break;
      case CAN_FILTER_15:
         CAN_MSEL.fil15 = Mask;
         break;
   }
}

void can_set_buffer_id(CAN_TX_HEADER *Header, CAN_BUFFER Buffer)
{
   TXBnID *iPtr;
   TXBnDLC *dPtr;
   uint32_t Id;
   
   switch(Buffer)
   {
      case CAN_BUFFER_TXB0:
         iPtr = &CAN_TXB0ID;
         dPtr = &CAN_TXB0DLC;
         break;
      case CAN_BUFFER_TXB1:
         iPtr = &CAN_TXB1ID;
         dPtr = &CAN_TXB1DLC;
         break;
      case CAN_BUFFER_TXB2:
         iPtr = &CAN_TXB2ID;
         dPtr = &CAN_TXB2DLC;
         break;
      case CAN_BUFFER_B0:
         iPtr = &CAN_B0ID_TX;
         dPtr = &CAN_B0DLC_TX;
         break;
      case CAN_BUFFER_B1:
         iPtr = &CAN_B1ID_TX;
         dPtr = &CAN_B1DLC_TX;
         break;
      case CAN_BUFFER_B2:
         iPtr = &CAN_B2ID_TX;
         dPtr = &CAN_B2DLC_TX;
         break;
      case CAN_BUFFER_B3:
         iPtr = &CAN_B3ID_TX;
         dPtr = &CAN_B3DLC_TX;
         break;
      case CAN_BUFFER_B4:
         iPtr = &CAN_B4ID_TX;
         dPtr = &CAN_B4DLC_TX;
         break;
      case CAN_BUFFER_B5:
         iPtr = &CAN_B5ID_TX;
         dPtr = &CAN_B5DLC_TX;
         break;
   }
   
   Id = Header->Id;

   if(Header->ext)   //Extended Id
   {
      //eid
      iPtr->eid0_7 = make8(Id, 0);               //eid 0-7
      iPtr->eid8_15 = make8(Id, 1);              //eid 8-15
      iPtr->eid16_17 = (make8(Id, 2) & 0x03);    //eid 16-17
      
      //sid
      iPtr->sid0_2 = ((make8(Id, 2) >> 2) & 0x07);  //sid 0-2
      iPtr->sid3_10 = ((make8(Id, 2) >> 5) | ((make8(Id, 3) & 0x1F) << 3)); //sid 3_10
      
      iPtr->exide = TRUE;
   }
   else  //Standard Id
   {
      //eid
      memset(iPtr, 0x00, sizeof(TXBnID));
      
      //sid
      iPtr->sid0_2 = (make8(Id, 0) & 0x07);
      iPtr->sid3_10 = ((make8(Id, 0) >> 3) | ((make8(Id, 1) & 0x07) << 5));
      
      iPtr->exide = FALSE;
   }
   
   dPtr->dlc = Header->Length;
   dPtr->txrtr = Header->rtr;
}

uint16_t *can_buffer_data_address(CAN_BUFFER Buffer)
{
   uint16_t *Address;
   
   switch(Buffer)
   {
      case CAN_BUFFER_RXB0:
         Address = CAN_RXB0DATA;
         break;
      case CAN_BUFFER_RXB1:
         Address = CAN_RXB1DATA;
         break;
      case CAN_BUFFER_TXB0:
         Address = CAN_TXB0DATA;
         break;
      case CAN_BUFFER_TXB1:
         Address = CAN_TXB1DATA;
         break;
      case CAN_BUFFER_TXB2:
         Address = CAN_TXB2DATA;
         break;
      case CAN_BUFFER_B0:
         Address = CAN_B0DATA;
         break;
      case CAN_BUFFER_B1:
         Address = CAN_B1DATA;
         break;
      case CAN_BUFFER_B2:
         Address = CAN_B2DATA;
         break;
      case CAN_BUFFER_B3:
         Address = CAN_B3DATA;
         break;
      case CAN_BUFFER_B4:
         Address = CAN_B4DATA;
         break;
      case CAN_BUFFER_B5:
         Address = CAN_B5DATA;
         break;
   }
   
   return(Address);
}

void can_get_rx_data(CAN_RX_HEADER *Header, uint8_t *Data)
{  
   BnCON_RX rBnCON;
   RXBnID rBnID;
   RXBnDLC rBnDLC;
   uint8_t rBnDATA[8];
  #ifdef IS_ENH18
   #locate rBnCON = 0x3F80
   #locate rBnID = 0x3F81
   #locate rBnDLC = 0x3F85
   #locate rBnDATA = 0x3F86
  #else
   #locate rBnCON = 0xF60
   #locate rBnID = 0xF61
   #locate rBnDLC = 0xF65
   #locate rBnDATA = 0xF66
  #endif
   union
   {
      uint32_t dw;
      uint16_t w[2];
      uint8_t b[4];
   } Id;
   
   Header->Filter = rBnCON.filhit;
   Header->Length = rBnDLC.dlc;
   
   Header->err_ovfl = CAN_COMSTAT_MODE2.rxovfl;
   Header->ext = rBnID.exide;
   Header->rtr = rBnDLC.rxrtr;
   
   if(Header->ext)
   {
      Id.w[0] = make16(rBnID.eid8_15, rBnID.eid0_7);
      Id.b[2] = rBnID.eid16_17 | (rBnID.sid0_2 << 2) | ((rBnID.sid3_10 & 0x07) << 5);
      Id.b[3] = (rBnID.sid3_10 >> 3) & 0x1F;
   }
   else
   {
      Id.dw = 0;
      
      Id.b[0] = rBnID.sid0_2 | (rBnID.sid3_10 << 3);
      Id.b[1] = rBnID.sid3_10 >> 5;
   }
   
   Header->Id = Id.dw;
   memcpy(Data, rBnDATA, Header->Length);
}
