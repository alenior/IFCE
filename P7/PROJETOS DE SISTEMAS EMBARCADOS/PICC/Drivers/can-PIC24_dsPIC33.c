///////////////////////////////////////////////////////////////////////////////
////                          can-pic24_dspic33.c                          ////
////                                                                       ////
////  Driver for ECAN peripheral for Microchip's PIC24 and dsPIC33 devices ////
////  with an ECAN peripheral.                                             ////
////                                                                       ////
////  See can-pic24_dspic33.h for API.                                     ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////                                                                       ////
////  Version History:                                                     ////
////                                                                       ////
////     12/29/20 - Updated driver to fix a couple typecasting issues.     ////
////                                                                       ////
////     12/14/20 - Updated driver to work with #device PSV=16 on devices  ////
////                were the DMA memory is located in the PSV memory       ////
////                range.                                                 ////
////                                                                       ////
////     04/28/20 - Fixed an issue with can _enable_filter() and           ////
////                can2_enable_filter() funtions with not assign the      ////
////                masks correctly.                                       ////
////                                                                       ////
////     02/28/20 - Fixed an issue with can_enable_filter() and            ////
////                can2_enable_filter() functions not assigned some       ////
////                of the filter buffers correctly.                       ////
////                                                                       ////
////     02/25/20 - Fixed a few issues with CAN2 peripheral defines.       ////
////                                                                       ////
////     01/03/20 - Fixed issues with CAN2 peripheral.                     ////
////                                                                       ////
////                Fixed some issues with can_enable_filter() and         ////
////                can2_enable_filter() functions.                        ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2020 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

#include <can-PIC24_dsPIC33.h>

//////////////

//CAN Peripheral Clock Settings
#ifndef CAN_CLOCK_DIVISOR
 #define CAN_CLOCK_DIVISOR          2  //can be 1 or 2, PIC24HJ and dsPIC33FJ devices only support divide by 2
#endif

#define CAN_CLOCK_SPEED             (getenv("CLOCK") / CAN_CLOCK_DIVISOR)

//CAN Baud Rate Settings
#ifndef CAN_BAUD_RATE
 #define CAN_BAUD_RATE              125000
#endif

#if (CAN_CLOCK_SPEED % CAN_BAUD_RATE) != 0
 #error CAN Clock Speed not evenly divisible by CAN Baud Rate
#endif

#ifndef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT   875   //in tenth of a percent, e.g. 875 = 87.5% of bit time
#endif

#if CAN_DEFAULT_SAMPLE_POINT < 500
 #undef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT   500
 #warning CAN Sample point changed to 50% of bit time
#endif

#if (USE_CAN2_PERIPHERAL == TRUE)
 #ifndef CAN2_CLOCK_DIVISOR
  #define CAN2_CLOCK_DIVISOR        2  //can be 1 or 2, PIC24HJ and dsPIC33FJ devices only support divide by 2
 #endif
 
 #define CAN2_CLOCK_SPEED        (getenv("CLOCK") / CAN2_CLOCK_DIVISOR)
 
 #ifndef CAN2_BAUD_RATE
  #define CAN2_BAUD_RATE            125000
 #endif
 
 #if (CAN2_CLOCK_SPEED % CAN2_BAUD_RATE) != 0
  #error CAN2 Clock Speed not evenly divisible by CAN2 Baud Rate
 #endif
  
 #ifndef CAN2_DEFAULT_SAMPLE_POINT
  #define CAN2_DEFAULT_SAMPLE_POINT   875   //in tenth of a percent, e.g. 875 = 87.5% of bit time
 #endif
  
 #if CAN2_DEFAULT_SAMPLE_POINT < 500
  #undef CAN2_DEFAULT_SAMPLE_POINT
  #define CAN2_DEFAULT_SAMPLE_POINT   500
  #warning CAN2 Sample point change to 50% of bit time
 #endif
#endif

//Other CAN Peripheral Settings

#ifndef CAN_TX_BUFFERS
 #define CAN_TX_BUFFERS       1     //can be any value from 0-8, CAN_TX_BUFFERS +
#endif                              //CAN_RX_BUFFERS must be 4, 6, 8, 12, 16, 24 or 32

#ifndef CAN_RX_BUFFERS
 #define CAN_RX_BUFFERS       (32 - CAN_TX_BUFFERS)   //CAN_TX_BUFFERS + CAN_RX_BUFFERS must be 4, 6, 8,
#endif                                                //12, 16, 24 or 32

#if CAN_TX_BUFFERS > 8
 #error CAN TX Buffers max size is 8
#endif

#if ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 4) && ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 6) && ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 8) && \
    ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 12) && ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 16) && ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 24) && \
    ((CAN_TX_BUFFERS + CAN_RX_BUFFERS) != 32)
 #error CAN TX Buffers + CAN RX Buffers must add up to 4, 6, 8, 12, 16, 24 or 32
#endif

#define CAN_BUFFER_SIZE       (CAN_TX_BUFFERS + CAN_RX_BUFFERS)

#ifndef CAN_BRG_SAM
 #define CAN_BRG_SAM             0 //sample of the can bus line (def: bus line is sampled 1 times prior to sample point)
#endif

#ifndef CAN_BRG_WAKE_FILTER
 #define CAN_BRG_WAKE_FILTER     FALSE   //selects can bus line filter for wake up bit
#endif

#ifndef CAN_ENABLE_CAN_CAPTURE
 #define CAN_ENABLE_CAN_CAPTURE  0
#endif

#ifndef CAN_CONTINUE_IN_IDLE
 #define CAN_CONTINUE_IN_IDLE    FALSE
#endif

#ifndef CAN_TX_DMA_CHANNEL
 #define CAN_TX_DMA_CHANNEL      0
#endif

#ifndef CAN_RX_DMA_CHANNEL
 #define CAN_RX_DMA_CHANNEL      1
#endif

#if (USE_CAN2_PERIPHERAL == TRUE)
 #ifndef CAN2_TX_BUFFERS
  #define CAN2_TX_BUFFERS        1     //can be any value from 0-8, CAN2_TX_BUFFERS +
 #endif                                //CAN2_RX_BUFFERS must be 4, 6, 8, 12, 16, 24 or 32

 #ifndef CAN2_RX_BUFFERS
  #define CAN2_RX_BUFFERS        (32 - CAN2_TX_BUFFERS)  //CAN2_TX_BUFFERS + CAN2_RX_BUFFERS must be 4, 6, 8,
 #endif                                                  //12, 16, 24 or 32
 
 #if ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 4) && ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 6) && ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 8) && \
     ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 12) && ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 16) && ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 24) && \
     ((CAN2_TX_BUFFERS + CAN2_RX_BUFFERS) != 32)
  #error CAN2 TX Buffers + CAN2 RX Buffers must add up to 4, 6, 8, 12, 16, 24 or 32
 #endif
 
 #define CAN2_BUFFER_SIZE        (CAN2_TX_BUFFERS + CAN2_RX_BUFFERS)
 
 #ifndef CAN2_BRG_SAM
  #define CAN2_BRG_SAM              0 //sample of the can bus line (def: bus line is sampled 1 times prior to sample point)
 #endif
 
 #ifndef CAN2_BRG_WAKE_FILTER
  #define CAN2_BRG_WAKE_FILTER      FALSE   //selects can bus line filter for wake up bit
 #endif
 
 #ifndef CAN2_ENABLE_CAN_CAPTURE
  #define CAN2_ENABLE_CAN_CAPTURE   0
 #endif
 
 #ifndef CAN2_CONTINUE_IN_IDLE
  #define CAN2_CONTINUE_IN_IDLE     FALSE
 #endif
 
 #ifndef CAN2_TX_DMA_CHANNEL
  #define CAN2_TX_DMA_CHANNEL       2
 #endif

 #ifndef CAN2_RX_DMA_CHANNEL
  #define CAN2_RX_DMA_CHANNEL       3
 #endif
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
   
   #define CAN_USER_MASK_y                ee    // y being the mask to setup (0-2), and ee being the Mask for the filter Id.
   #define CAN_USER_MASK_y_ID_TYPE        ff    // y being the mask to setup (0-2), and ff being the Mask Id type:
                                                //    CAN_MASK_ID_TYPE_SID - mask Id is a SID Id, only the SID bits of Mask are written the EID bits are set to 0.
                                                //    CAN_MASK_ID_TYPE_EID - mask Id is an EID Id, the SID and EID bits of Mask are written.
   #define CAN_USER_MASK_y_FILTER_TYPE    gg    // y being the mask to setup (0-2), and gg being the filter mask type:
                                                //    CAN_FILTER_MASK_TYPE_EITHER - match either SID or EID messages, ignores EXIDE bit of filter.
                                                //    CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, EXIDE bit of filter determines type of messages received.
*/

#ifndef CAN_USE_FILTERS
 #define CAN_USE_FILTERS      FALSE
#endif

#if CAN_USE_FILTERS == FALSE
 /*
   The CAN peripheral requires at least one filter to receive messages.  If
   CAN_USE_FILTERS is defined to FALSE the following ensures that Filter 0 and 
   Mask 0 are setup to receive all messages.
 */ 
 #ifdef CAN_USER_FILT_0
  #undef CAN_USER_FILT_0
 #endif
 #define CAN_USER_FILT_0               0
 
 #ifdef CAN_USER_FILT_0_TYPE
  #undef CAN_USER_FILT_0_TYPE
 #endif
 #define CAN_USER_FILT_0_TYPE          CAN_FILTER_TYPE_EID
 
 #ifdef CAN_USER_FILT_0_BUFFER
  #undef CAN_USER_FILT_0_BUFFER
 #endif
 #define CAN_USER_FILT_0_BUFFER        CAN_FILTER_BUFFER_FIFO
 
 #ifdef CAN_USER_FILT_0_MASK
  #undef CAN_USER_FILT_0_MASK
 #endif
 #define CAN_USER_FILT_0_MASK          CAN_FILTER_MASK_0
 
 #ifdef CAN_USER_MASK_0
  #undef CAN_USER_MASK_0
 #endif
 #define CAN_USER_MASK_0               0
 
 #ifdef CAN_USER_MASK_0_ID_TYPE
  #undef CAN_USER_MASK_0_ID_TYPE
 #endif
 #define CAN_USER_MASK_0_ID_TYPE       CAN_MASK_ID_TYPE_EID
 
 #ifdef CAN_USER_MASK_0_FILTER_TYPE
  #undef CAN_USER_MASK_0_FILTER_TYPE
 #endif
 #define CAN_USER_MASK_0_FILTER_TYPE   CAN_FILTER_MASK_TYPE_EITHER
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
  #define CAN_USER_FILT_0                 0
 #endif
 
 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE            CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_0_BUFFER
  #define CAN_USER_FILT_0_BUFFER          CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_1_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_2_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_3_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_4_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_5_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_6_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_7_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_8_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_9_BUFFER         CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_10_BUFFER        CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_11_BUFFER        CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_12_BUFFER        CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_13_BUFFER        CAN_FILTER_BUFFER_FIFO
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
   #define CAN_USER_FILT_14_BUFFER        CAN_FILTER_BUFFER_FIFO
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
  #elif CAN_USER_FILTER_41_MASK == CAN_FILTER_MASK_2
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
   #define CAN_USER_FILT_15_BUFFER        CAN_FILTER_BUFFER_FIFO
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
#endif

//////////////////////////// CAN2 Filter Settings /////////////////////////////

#if (USE_CAN2_PERIPHERAL == TRUE)
   /*
      If no CAN2 RX Filters are going to be used then nothing needs to be done, by
      default this driver will initialize the CAN2 controller to use no filter.
      
      If CAN2 RX Filters are going to be used, the can2_init() function can be used
      to initialize them by making the following defines before this driver is
      included:
      
      #define CAN2_USE_FILTERS               TRUE
      
      #define CAN2_USER_FILT_x               aa    // x being the filter to enable (0-15), and aa being the Id to filter for.
      #define CAN2_USER_FILT_x_TYPE          bb    // x being the filter to setup (0-15), and bb being the filter type:
                                                   //    CAN_FILTER_TYPE_SID - match SID messages
                                                   //    CAN_FILTER_TYPE_EID - match EID messages
      #define CAN2_USER_FILT_x_BUFFER        cc    // x being the filter to setup (0-15), and cc being the buffer to assign
                                                   // the filter to, see CAN_FILTER_BUFFER for options.
      #define CAN2_USER_FILT_x_MASK          dd    // x being the filter to setup (0-15), and dd being the mask to use with
                                                   // the filter, see CAN_FILTER_MASK for options.
      
      #define CAN2_USER_MASK_y               ee    // y being the mask to setup (0-2), and ee being the Mask for the filter Id.
      #define CAN2_USER_MASK_y_ID_TYPE       ff    // y being the mask to setup (0-2), and ff being the Mask Id type:
                                                   //    CAN_MASK_ID_TYPE_SID - mask Id is a SID Id, only the SID bits of Mask are written the EID bits are set to 0
                                                   //    CAN_MASK_ID_TYPE_EID - mask Id is an EID Id, the SID and EID bits of Mask are written
      #define CAN2_USER_MASK_y_FILTER_TYPE   gg    // y being the mask to setup (0-2), and gg being the filter mask type:
                                                   //    CAN_FILTER_MASK_TYPE_EITHER - match either SID or EID messages, ignores EXIDE bit of filter
                                                         CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, EXIDE bit of filter determines type of messages received
   */

   #ifndef CAN2_USE_FILTERS
    #define CAN2_USE_FILTERS      FALSE
   #endif
   
   #if CAN2_USE_FILTERS == FALSE
    /*
      The CAN2 peripheral requires at least one filter to receive messages.  If
      CAN2_USE_FILTERS is defined to FALSE the following ensures that Filter 0 and 
      Mask 0 are setup to receive all messages.
    */ 
    #ifdef CAN2_USER_FILT_0
     #undef CAN2_USER_FILT_0
    #endif
    #define CAN2_USER_FILT_0               0
    
    #ifdef CAN2_USER_FILT_0_TYPE
     #undef CAN2_USER_FILT_0_TYPE
    #endif
    #define CAN2_USER_FILT_0_TYPE          CAN_FILTER_TYPE_EID
    
    #ifdef CAN2_USER_FILT_0_BUFFER
     #undef CAN2_USER_FILT_0_BUFFER
    #endif
    #define CAN2_USER_FILT_0_BUFFER        CAN_FILTER_BUFFER_FIFO
    
    #ifdef CAN2_USER_FILT_0_MASK
     #undef CAN2_USER_FILT_0_MASK
    #endif
    #define CAN2_USER_FILT_0_MASK          CAN_FILTER_MASK_0
    
    #ifdef CAN2_USER_MASK_0
     #undef CAN2_USER_MASK_0
    #endif
    #define CAN2_USER_MASK_0               0
    
    #ifdef CAN2_USER_MASK_0_ID_TYPE
     #undef CAN2_USER_MASK_0_ID_TYPE
    #endif
    #define CAN2_USER_MASK_0_ID_TYPE       CAN_MASK_ID_TYPE_EID
    
    #ifdef CAN2_USER_MASK_0_FILTER_TYPE
     #undef CAN2_USER_MASK_0_FILTER_TYPE
    #endif
    #define CAN2_USER_MASK_0_FILTER_TYPE   CAN_FILTER_MASK_TYPE_EITHER
   #else
    /*
      The CAN2 peripheral requires at least one filter to receive messages.  If
      CAN2_USE_FILTERS is defined to TRUE and Filter 0 isn't setup the following
      sets up Filter 0 to receive all messages.  If using filters they should be
      used in order from 0 to 15. For example if one filter is required, Filter 0
      should be used, if two filters are required, Filter 0 and Filter 1 should be
      used, etc.
    */ 
    #ifndef CAN2_USER_FILT_0
     #define CAN2_USER_FILT_0                 0
    #endif
    
    #ifndef CAN2_USER_FILT_0_TYPE
     #define CAN2_USER_FILT_0_TYPE            CAN_FILTER_TYPE_EID
    #endif
    
    #ifndef CAN2_USER_FILT_0_BUFFER
     #define CAN2_USER_FILT_0_BUFFER          CAN_FILTER_BUFFER_FIFO
    #endif
    
    #ifndef CAN2_USER_FILT_0_MASK
     #define CAN2_USER_FILT_0_MASK            CAN_FILTER_MASK_0
    #endif
    
    #if CAN2_USER_FILT_0_MASK == CAN_FILTER_MASK_0
     #ifndef CAN2_USER_MASK_0
      #define CAN2_USER_MASK_0                0
     #endif
     
     #ifndef CAN2_USER_MASK_0_ID_TYPE
      #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_MASK_0_FILTER_TYPE
      #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
     #endif
    #elif CAN2_USER_FILTER_0_MASK == CAN_FILTER_MASK_1
     #ifndef CAN2_USER_MASK_1
      #define CAN2_USER_MASK_1                0
     #endif
     
     #ifndef CAN2_USER_MASK_1_ID_TYPE
      #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_MASK_1_FILTER_TYPE
      #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
     #endif
    #elif CAN2_USER_FILTER_0_MASK == CAN_FILTER_MASK_2
     #ifndef CAN2_USER_MASK_2
      #define CAN2_USER_MASK_2                0
     #endif
     
     #ifndef CAN2_USER_MASK_2_ID_TYPE
      #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_MASK_2_FILTER_TYPE
      #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
     #endif
    #endif
    
    //Filter 1
    #ifdef CAN2_USER_FILT_1
     #ifndef CAN2_USER_FILT_1_TYPE
      #define CAN2_USER_FILT_1_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_1_BUFFER
      #define CAN2_USER_FILT_1_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_1_MASK
      #define CAN2_USER_FILT_1_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_1_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_1_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_1_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 2
    #ifdef CAN2_USER_FILT_2
     #ifndef CAN2_USER_FILT_2_TYPE
      #define CAN2_USER_FILT_2_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_2_BUFFER
      #define CAN2_USER_FILT_2_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_2_MASK
      #define CAN2_USER_FILT_2_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_2_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_2_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_2_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 3
    #ifdef CAN2_USER_FILT_3
     #ifndef CAN2_USER_FILT_3_TYPE
      #define CAN2_USER_FILT_3_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_3_BUFFER
      #define CAN2_USER_FILT_3_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_3_MASK
      #define CAN2_USER_FILT_3_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_3_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_3_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_3_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 4
    #ifdef CAN2_USER_FILT_4
     #ifndef CAN2_USER_FILT_4_TYPE
      #define CAN2_USER_FILT_4_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_4_BUFFER
      #define CAN2_USER_FILT_4_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_4_MASK
      #define CAN2_USER_FILT_4_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_4_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_4_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_4_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 5
    #ifdef CAN2_USER_FILT_5
     #ifndef CAN2_USER_FILT_5_TYPE
      #define CAN2_USER_FILT_5_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_5_BUFFER
      #define CAN2_USER_FILT_5_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_5_MASK
      #define CAN2_USER_FILT_5_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_5_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_5_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_5_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 6
    #ifdef CAN2_USER_FILT_6
     #ifndef CAN2_USER_FILT_6_TYPE
      #define CAN2_USER_FILT_6_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_6_BUFFER
      #define CAN2_USER_FILT_6_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_6_MASK
      #define CAN2_USER_FILT_6_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_6_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_6_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_6_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 7
    #ifdef CAN2_USER_FILT_7
     #ifndef CAN2_USER_FILT_7_TYPE
      #define CAN2_USER_FILT_7_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_7_BUFFER
      #define CAN2_USER_FILT_7_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_7_MASK
      #define CAN2_USER_FILT_7_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_7_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_7_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_7_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 8
    #ifdef CAN2_USER_FILT_8
     #ifndef CAN2_USER_FILT_8_TYPE
      #define CAN2_USER_FILT_8_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_8_BUFFER
      #define CAN2_USER_FILT_8_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_8_MASK
      #define CAN2_USER_FILT_8_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_8_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_8_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_8_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 9
    #ifdef CAN2_USER_FILT_9
     #ifndef CAN2_USER_FILT_9_TYPE
      #define CAN2_USER_FILT_9_TYPE           CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_9_BUFFER
      #define CAN2_USER_FILT_9_BUFFER         CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_9_MASK
      #define CAN2_USER_FILT_9_MASK           CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_9_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_9_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_9_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 10
    #ifdef CAN2_USER_FILT_10
     #ifndef CAN2_USER_FILT_10_TYPE
      #define CAN2_USER_FILT_10_TYPE          CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_10_BUFFER
      #define CAN2_USER_FILT_10_BUFFER        CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_10_MASK
      #define CAN2_USER_FILT_10_MASK          CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_10_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_10_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_10_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 11
    #ifdef CAN2_USER_FILT_11
     #ifndef CAN2_USER_FILT_11_TYPE
      #define CAN2_USER_FILT_11_TYPE          CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_11_BUFFER
      #define CAN2_USER_FILT_11_BUFFER        CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_11_MASK
      #define CAN2_USER_FILT_11_MASK          CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_11_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_11_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_11_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 12
    #ifdef CAN2_USER_FILT_12
     #ifndef CAN2_USER_FILT_12_TYPE
      #define CAN2_USER_FILT_12_TYPE          CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_12_BUFFER
      #define CAN2_USER_FILT_12_BUFFER        CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_12_MASK
      #define CAN2_USER_FILT_12_MASK          CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_12_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_12_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_12_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 13
    #ifdef CAN2_USER_FILT_13
     #ifndef CAN2_USER_FILT_13_TYPE
      #define CAN2_USER_FILT_13_TYPE          CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_13_BUFFER
      #define CAN2_USER_FILT_13_BUFFER        CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_13_MASK
      #define CAN2_USER_FILT_13_MASK          CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_13_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_13_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_13_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 14
    #ifdef CAN2_USER_FILT_14
     #ifndef CAN2_USER_FILT_14_TYPE
      #define CAN2_USER_FILT_14_TYPE          CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_14_BUFFER
      #define CAN2_USER_FILT_14_BUFFER        CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_14_MASK
      #define CAN2_USER_FILT_14_MASK          CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_14_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_14_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_41_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
    
    //Filter 15
    #ifdef CAN2_USER_FILT_15
     #ifndef CAN2_USER_FILT_15_TYPE
      #define CAN2_USER_FILT_15_TYPE          CAN_FILTER_TYPE_EID
     #endif
     
     #ifndef CAN2_USER_FILT_15_BUFFER
      #define CAN2_USER_FILT_15_BUFFER        CAN_FILTER_BUFFER_FIFO
     #endif
     
     #ifndef CAN2_USER_FILT_15_MASK
      #define CAN2_USER_FILT_15_MASK          CAN_FILTER_MASK_0
     #endif
     
     #if CAN2_USER_FILT_15_MASK == CAN_FILTER_MASK_0
      #ifndef CAN2_USER_MASK_0
       #define CAN2_USER_MASK_0                0
      #endif
     
      #ifndef CAN2_USER_MASK_0_ID_TYPE
       #define CAN2_USER_MASK_0_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_0_FILTER_TYPE
       #define CAN2_USER_MASK_0_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_15_MASK == CAN_FILTER_MASK_1
      #ifndef CAN2_USER_MASK_1
       #define CAN2_USER_MASK_1                0
      #endif
     
      #ifndef CAN2_USER_MASK_1_ID_TYPE
       #define CAN2_USER_MASK_1_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_1_FILTER_TYPE
       #define CAN2_USER_MASK_1_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #elif CAN2_USER_FILTER_15_MASK == CAN_FILTER_MASK_2
      #ifndef CAN2_USER_MASK_2
       #define CAN2_USER_MASK_2                0
      #endif
     
      #ifndef CAN2_USER_MASK_2_ID_TYPE
       #define CAN2_USER_MASK_2_ID_TYPE        CAN_MASK_ID_TYPE_EID
      #endif
     
      #ifndef CAN2_USER_MASK_2_FILTER_TYPE
       #define CAN2_USER_MASK_2_FILTER_TYPE    CAN_FILTER_MASK_TYPE_EITHER
      #endif
     #endif 
    #endif
   #endif
#endif

////////////////////////////////////////////////////////////////////////////////
//////////////////////// ECAN Control Registers ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// ECAN Control Register 1                     
typedef struct struct_CiCTRL1 {
   int1 win;               //0      //SFR map window select bit
   uint16_t void21:2;      //1:2    
   int1 cancap;            //3      //CAN message receive timer capture event enable bit
   int1 void4;             //4
   CAN_OP_MODE opmode:3;   //5:7    //Operation mode bits
   CAN_OP_MODE reqop:3;    //8:10   //Request operation mode bits
   int1 cancks;            //11     //CAN master clock select bit
   int1 abat;              //12     //Abort all pending transmissions bit
   int1 csidl;             //13     //Stop in idle mode bit
   uint16_t void15_14:2;   //14:15
} CiCTRL1;

CiCTRL1 CAN_C1CTRL1;
#locate CAN_C1CTRL1=getenv("SFR:C1CTRL1")

#if getenv("SFR_VALID:C2CTRL1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCTRL1 CAN_C2CTRL1;
 #locate CAN_C2CTRL1=getenv("SFR:C2CTRL1")
#endif

// ECAN Control Register 2
typedef struct struct_CiCTRL2 {
   uint16_t dncnt:5;       //0:4    //DeviceNet filter bit number bits
   uint16_t void15_5:11;   //5:15
} CiCTRL2;

CiCTRL2 CAN_C1CTRL2;
#locate CAN_C1CTRL2=getenv("SFR:C1CTRL2")

#if getenv("SFR_VALID:C2CTRL2") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCTRL2 CAN_C2CTRL2;
 #locate CAN_C2CTRL2=getenv("SFR:C2CTRL2")
#endif

////////////////////////////////////////////////////////////////////////////////
/////////////////////// Baud Control Registers /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// ECAN Baud Configuration Register 1
typedef struct struct_CiCFG1 {
   int brp:6;              //0:5    //Baud Rate Prescalar bits
   int sjw:2;              //6:7    //Synchronization jump width bits
   int void15_8:8;         //8:15
} CiCFG1;

CiCFG1 CAN_C1CFG1;
#locate CAN_C1CFG1=getenv("SFR:C1CFG1")

#if getenv("SFR_VALID:C2CFG1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCFG1 CAN_C2CFG1;
 #locate CAN_C2CFG1=getenv("SFR:C2CFG1")
#endif

//ECAN Baud Configuration Register 2
typedef struct struct_CiCFG2 {
   int prseg:3;            //0:2    //Propagation time segment bits
   int seg1ph:3;           //3:5    //Phase segment 1 bits
   int1 sam;               //6      //Sample of the CAN bus line bit
   int1 seg2phts;          //7      //Phase segment 2 time select bit
   int seg2ph:3;           //8:10   //Phase segment 2 bits
   int void13_11:3;        //11:13
   int1 wakfil;            //14       //Select CAN bus line filter for wake-up bit
   int1 void15;            //15
} CiCFG2;

CiCFG2 CAN_C1CFG2;
#locate CAN_C1CFG2=getenv("SFR:C1CFG2")

#if getenv("SFR_VALID:C2CFG2") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCFG2 CAN_C2CFG2;
#locate CAN_C2CFG2=getenv("SFR:C2CFG2")
#endif

////////////////////////////////////////////////////////////////////////////////

//ECAN Transmit/Receive Error Count Register
typedef struct struct_CiEC {
   uint8_t rerrcnt;        //0:7    //Receive error count bits
   uint8_t terrcnt;        //8:15   //Transmit error count bits
} CiEC;

CiEC CAN_C1EC;
#locate CAN_C1EC=getenv("SFR:C1EC")

#if getenv("SFR_VALID:C2EC") && (USE_CAN2_PERIPHERAL == TRUE)
 CiEC CAN_C2EC;
 #locate CAN_C2EC=getenv("SFR:C2EC")
#endif

////////////////////////////////////////////////////////////////////////////////

//ECAN Acceptance Filter Enable register
typedef union struct_CiFEN1 {
   struct
   {
      int1 FLTEN0;      //0 - Disable Filter 0, 1 - Enable Filter 0
      int1 FLTEN1;      //0 - Disable Filter 1, 1 - Enable Filter 1
      int1 FLTEN2;      //0 - Disable Filter 2, 1 - Enable Filter 2
      int1 FLTEN3;      //0 - Disable Filter 3, 1 - Enable Filter 3
      int1 FLTEN4;      //0 - Disable Filter 4, 1 - Enable Filter 4
      int1 FLTEN5;      //0 - Disable Filter 5, 1 - Enable Filter 5
      int1 FLTEN6;      //0 - Disable Filter 6, 1 - Enable Filter 6
      int1 FLTEN7;      //0 - Disable Filter 7, 1 - Enable Filter 7
      int1 FLTEN8;      //0 - Disable Filter 8, 1 - Enable Filter 8
      int1 FLTEN9;      //0 - Disable Filter 9, 1 - Enable Filter 9
      int1 FLTEN10;     //0 - Disable Filter 10, 1 - Enable Filter 10
      int1 FLTEN11;     //0 - Disable Filter 11, 1 - Enable Filter 11
      int1 FLTEN12;     //0 - Disable Filter 12, 1 - Enable Filter 12
      int1 FLTEN13;     //0 - Disable Filter 13, 1 - Enable Filter 13
      int1 FLTEN14;     //0 - Disable Filter 14, 1 - Enable Filter 14
      int1 FLTEN15;     //0 - Disable Filter 25, 1 - Enable Filter 15
   };
   uint16_t w;
} CiFEN1;

CiFEN1 CAN_C1FEN1;
#locate CAN_C1FEN1=getenv("SFR:C1FEN1")

#if getenv("SFR_VALID:C2FEN1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiFEN1 CAN_C2FEN1;
 #locate CAN_C2FEN1=getenv("SFR:C2FEN1")
#endif

///////////////////////////////////////////////////////////////////////////////

//ECAN Filter Buffer Pointer Registers
typedef union union_CiBUFPNT {
   uint16_t w;
   struct {
      uint16_t f0bp:4;     //0:3    //RX Buffer mask for Filter 0
      uint16_t f1bp:4;     //4:7    //RX Buffer mask for Filter 1
      uint16_t f2bp:4;     //8:11   //RX Buffer mask for Filter 2
      uint16_t f3bp:4;     //12:15  //RX Buffer mask for Filter 3
   };
} CiBUFPNT;

CiBUFPNT CAN_C1BUFPNT1;
CiBUFPNT CAN_C1BUFPNT2;
CiBUFPNT CAN_C1BUFPNT3;
CiBUFPNT CAN_C1BUFPNT4;
#locate CAN_C1BUFPNT1=getenv("SFR:C1BUFPNT1")
#locate CAN_C1BUFPNT2=getenv("SFR:C1BUFPNT2")
#locate CAN_C1BUFPNT3=getenv("SFR:C1BUFPNT3")
#locate CAN_C1BUFPNT4=getenv("SFR:C1BUFPNT4")

#if getenv("SFR_VALID:C2BUFPNT1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiBUFPNT CAN_C2BUFPNT1;
 CiBUFPNT CAN_C2BUFPNT2;
 CiBUFPNT CAN_C2BUFPNT3;
 CiBUFPNT CAN_C2BUFPNT4;
 #locate CAN_C2BUFPNT1=getenv("SFR:C2BUFPNT1")
 #locate CAN_C2BUFPNT2=getenv("SFR:C2BUFPNT2")
 #locate CAN_C2BUFPNT3=getenv("SFR:C2BUFPNT3")
 #locate CAN_C2BUFPNT4=getenv("SFR:C2BUFPNT4")
#endif

///////////////////////////////////////////////////////////////////////////////

//ECAN Acceptance Filters
typedef struct struct_CiRXFLT {
   uint16_t EID16_17:2;
   uint16_t u2:1;
   uint16_t EXIDE:1;
   uint16_t u4:1;
   uint16_t SID:11;
   uint16_t EID0_15;
} CiRXFLT;

CiRXFLT CAN_C1RXFLT0;
CiRXFLT CAN_C1RXFLT1;
CiRXFLT CAN_C1RXFLT2;
CiRXFLT CAN_C1RXFLT3;
CiRXFLT CAN_C1RXFLT4;
CiRXFLT CAN_C1RXFLT5;
CiRXFLT CAN_C1RXFLT6;
CiRXFLT CAN_C1RXFLT7;
CiRXFLT CAN_C1RXFLT8;
CiRXFLT CAN_C1RXFLT9;
CiRXFLT CAN_C1RXFLT10;
CiRXFLT CAN_C1RXFLT11;
CiRXFLT CAN_C1RXFLT12;
CiRXFLT CAN_C1RXFLT13;
CiRXFLT CAN_C1RXFLT14;
CiRXFLT CAN_C1RXFLT15;
#locate CAN_C1RXFLT0=getenv("SFR:C1RXF0SID")
#locate CAN_C1RXFLT1=getenv("SFR:C1RXF1SID")
#locate CAN_C1RXFLT2=getenv("SFR:C1RXF2SID")
#locate CAN_C1RXFLT3=getenv("SFR:C1RXF3SID")
#locate CAN_C1RXFLT4=getenv("SFR:C1RXF4SID")
#locate CAN_C1RXFLT5=getenv("SFR:C1RXF5SID")
#locate CAN_C1RXFLT6=getenv("SFR:C1RXF6SID")
#locate CAN_C1RXFLT7=getenv("SFR:C1RXF7SID")
#locate CAN_C1RXFLT8=getenv("SFR:C1RXF8SID")
#locate CAN_C1RXFLT9=getenv("SFR:C1RXF9SID")
#locate CAN_C1RXFLT10=getenv("SFR:C1RXF10SID")
#locate CAN_C1RXFLT11=getenv("SFR:C1RXF11SID")
#locate CAN_C1RXFLT12=getenv("SFR:C1RXF12SID")
#locate CAN_C1RXFLT13=getenv("SFR:C1RXF13SID")
#locate CAN_C1RXFLT14=getenv("SFR:C1RXF14SID")
#locate CAN_C1RXFLT15=getenv("SFR:C1RXF15SID")

#if getenv("SFR_VALID:C2RXF0SID") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXFLT CAN_C2RXFLT0;
 CiRXFLT CAN_C2RXFLT1;
 CiRXFLT CAN_C2RXFLT2;
 CiRXFLT CAN_C2RXFLT3;
 CiRXFLT CAN_C2RXFLT4;
 CiRXFLT CAN_C2RXFLT5;
 CiRXFLT CAN_C2RXFLT6;
 CiRXFLT CAN_C2RXFLT7;
 CiRXFLT CAN_C2RXFLT8;
 CiRXFLT CAN_C2RXFLT9;
 CiRXFLT CAN_C2RXFLT10;
 CiRXFLT CAN_C2RXFLT11;
 CiRXFLT CAN_C2RXFLT12;
 CiRXFLT CAN_C2RXFLT13;
 CiRXFLT CAN_C2RXFLT14;
 CiRXFLT CAN_C2RXFLT15;
 #locate CAN_C2RXFLT0=getenv("SFR:C2RXF0SID")
 #locate CAN_C2RXFLT1=getenv("SFR:C2RXF1SID")
 #locate CAN_C2RXFLT2=getenv("SFR:C2RXF2SID")
 #locate CAN_C2RXFLT3=getenv("SFR:C2RXF3SID")
 #locate CAN_C2RXFLT4=getenv("SFR:C2RXF4SID")
 #locate CAN_C2RXFLT5=getenv("SFR:C2RXF5SID")
 #locate CAN_C2RXFLT6=getenv("SFR:C2RXF6SID")
 #locate CAN_C2RXFLT7=getenv("SFR:C2RXF7SID")
 #locate CAN_C2RXFLT8=getenv("SFR:C2RXF8SID")
 #locate CAN_C2RXFLT9=getenv("SFR:C2RXF9SID")
 #locate CAN_C2RXFLT10=getenv("SFR:C2RXF10SID")
 #locate CAN_C2RXFLT11=getenv("SFR:C2RXF11SID")
 #locate CAN_C2RXFLT12=getenv("SFR:C2RXF12SID")
 #locate CAN_C2RXFLT13=getenv("SFR:C2RXF13SID")
 #locate CAN_C2RXFLT14=getenv("SFR:C2RXF14SID")
 #locate CAN_C2RXFLT15=getenv("SFR:C2RXF15SID")
#endif

// ECAN Filter Mask Selection
typedef union struct_CiFMSKSEL {
   struct
   {
      CAN_FILTER_MASK f0msk:2;      // Mask Source for Filter 0
      CAN_FILTER_MASK f1msk:2;      // Mask Source for Filter 1
      CAN_FILTER_MASK f2msk:2;      // Mask Source for Filter 2
      CAN_FILTER_MASK f3msk:2;      // Mask Source for Filter 3
      CAN_FILTER_MASK f4msk:2;      // Mask Source for Filter 4
      CAN_FILTER_MASK f5msk:2;      // Mask Source for Filter 5
      CAN_FILTER_MASK f6msk:2;      // Mask Source for Filter 6
      CAN_FILTER_MASK f7msk:2;      // Mask Source for Filter 7
      CAN_FILTER_MASK f8msk:2;      // Mask Source for Filter 8
      CAN_FILTER_MASK f9msk:2;      // Mask Source for Filter 9
      CAN_FILTER_MASK f10msk:2;     // Mask Source for Filter 10
      CAN_FILTER_MASK f11msk:2;     // Mask Source for Filter 11
      CAN_FILTER_MASK f12msk:2;     // Mask Source for Filter 12
      CAN_FILTER_MASK f13msk:2;     // Mask Source for Filter 13
      CAN_FILTER_MASK f14msk:2;     // Mask Source for Filter 14
      CAN_FILTER_MASK f15msk:2;     // Mask Source for Filter 15
   };
   uint32_t dw;
} CiFMSKSEL;

CiFMSKSEL CAN_C1FMSKSEL;
#locate CAN_C1FMSKSEL=getenv("SFR:C1FMSKSEL1")

#if getenv("SFR_VALID:C2FMSKSEL1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiFMSKSEL CAN_C2FMSKSEL;
 #locate CAN_C2FMSKSEL=getenv("SFR:C2FMSKSEL1")
#endif

// ECAN Acceptance Filter Mask Standard Identifier Register
typedef struct struct_CiRXMASK {
   uint16_t EID16_17:2;
   uint16_t u2:1;
   uint16_t MIDE:1;
   uint16_t u4:1;
   uint16_t SID:11;
   uint16_t EID0_15;
} CiRXMASK;

CiRXMASK CAN_C1RXMASK0;
CiRXMASK CAN_C1RXMASK1;
CiRXMASK CAN_C1RXMASK2;
#locate CAN_C1RXMASK0=getenv("SFR:C1RXM0SID")
#locate CAN_C1RXMASK1=getenv("SFR:C1RXM1SID")
#locate CAN_C1RXMASK2=getenv("SFR:C1RXM2SID")

#if getenv("SFR_VALID:C2RXM0SID") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXMASK CAN_C2RXMASK0;
 CiRXMASK CAN_C2RXMASK1;
 CiRXMASK CAN_C2RXMASK2;
 #locate CAN_C2RXMASK0=getenv("SFR:C2RXM0SID")
 #locate CAN_C2RXMASK1=getenv("SFR:C2RXM1SID")
 #locate CAN_C2RXMASK2=getenv("SFR:C2RXM2SID")
#endif

////////////////////////////////////////////////////////////////////////////////

// ECAN Receive Buffer Full
typedef union union_CiRXFUL {
   struct {
      int1 rxful0;      //receive buffer 0 full
      int1 rxful1;      //receive buffer 1 full
      int1 rxful2;      //receive buffer 2 full
      int1 rxful3;      //receive buffer 3 full
      int1 rxful4;      //receive buffer 4 full
      int1 rxful5;      //receive buffer 5 full
      int1 rxful6;      //receive buffer 6 full
      int1 rxful7;      //receive buffer 7 full
      int1 rxful8;      //receive buffer 8 full
      int1 rxful9;      //receive buffer 9 full
      int1 rxful10;     //receive buffer 10 full
      int1 rxful11;     //receive buffer 11 full
      int1 rxful12;     //receive buffer 12 full
      int1 rxful13;     //receive buffer 13 full
      int1 rxful14;     //receive buffer 14 full
      int1 rxful15;     //receive buffer 15 full
      int1 rxful16;     //receive buffer 16 full
      int1 rxful17;     //receive buffer 17 full
      int1 rxful18;     //receive buffer 18 full
      int1 rxful19;     //receive buffer 19 full
      int1 rxful20;     //receive buffer 20 full
      int1 rxful21;     //receive buffer 21 full
      int1 rxful22;     //receive buffer 22 full
      int1 rxful23;     //receive buffer 23 full
      int1 rxful24;     //receive buffer 24 full
      int1 rxful25;     //receive buffer 25 full
      int1 rxful26;     //receive buffer 26 full
      int1 rxful27;     //receive buffer 27 full
      int1 rxful28;     //receive buffer 28 full
      int1 rxful29;     //receive buffer 29 full
      int1 rxful30;     //receive buffer 30 full
      int1 rxful31;     //receive buffer 31 full
   };
   int1 array[32];
   uint16_t w[2];
   uint32_t rxful;
} CiRXFUL;

CiRXFUL CAN_C1RXFUL;   
#locate CAN_C1RXFUL=getenv("SFR:C1RXFUL1")

#if getenv("SFR_VALID:C2RXFUL1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXFUL CAN_C2RXFUL;
 #locate CAN_C2RXFUL=getenv("SFR:C2RXFUL1")
#endif

// ECAN Receive Buffer Overflow
typedef union union_CiRXOVF {
   struct {
      int1 rxovf0;      //receive buffer 0 overflow
      int1 rxovf1;      //receive buffer 1 overflow
      int1 rxovf2;      //receive buffer 2 overflow
      int1 rxovf3;      //receive buffer 3 overflow
      int1 rxovf4;      //receive buffer 4 overflow
      int1 rxovf5;      //receive buffer 5 overflow
      int1 rxovf6;      //receive buffer 6 overflow
      int1 rxovf7;      //receive buffer 7 overflow
      int1 rxovf8;      //receive buffer 8 overflow
      int1 rxovf9;      //receive buffer 9 overflow
      int1 rxovf10;     //receive buffer 10 overflow
      int1 rxovf11;     //receive buffer 11 overflow
      int1 rxovf12;     //receive buffer 12 overflow
      int1 rxovf13;     //receive buffer 13 overflow
      int1 rxovf14;     //receive buffer 14 overflow
      int1 rxovf15;     //receive buffer 15 overflow
      int1 rxovf16;     //receive buffer 16 overflow
      int1 rxovf17;     //receive buffer 17 overflow
      int1 rxovf18;     //receive buffer 18 overflow
      int1 rxovf19;     //receive buffer 19 overflow
      int1 rxovf20;     //receive buffer 20 overflow
      int1 rxovf21;     //receive buffer 21 overflow
      int1 rxovf22;     //receive buffer 22 overflow
      int1 rxovf23;     //receive buffer 23 overflow
      int1 rxovf24;     //receive buffer 24 overflow
      int1 rxovf25;     //receive buffer 25 overflow
      int1 rxovf26;     //receive buffer 26 overflow
      int1 rxovf27;     //receive buffer 27 overflow
      int1 rxovf28;     //receive buffer 28 overflow
      int1 rxovf29;     //receive buffer 29 overflow
      int1 rxovf30;     //receive buffer 30 overflow
      int1 rxovf31;     //receive buffer 31 overflow
   };
   int1 array[32];
   uint16_t w[2];
   uint32_t rxovf;
} CiRXOVF;

CiRXOVF CAN_C1RXOVF;
#locate CAN_C1RXOVF=getenv("SFR:C1RXOVF1")

#if getenv("SFR_VALID:C2RXOVF1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXOVF CAN_C2RXOVF;
 #locate CAN_C2RXOVF=getenv("SFR:C2RXOVF1")
#endif

////////////////////////////////////////////////////////////////////////////////

// ECAN Tx/Rx Buffer m Control Register

typedef struct {
   uint16_t txpri:2;      //0:1    // Message Transmission Priority bits
   int1 rtren;            //2      // Auto-Remote Transmit Enable bit
   int1 txreq;            //3      // Message Send Request bit
   int1 txerr;            //4      // Error Detected During Transmission bit
   int1 txlarb;           //5      // Message Lost Arbitration bit
   int1 txabt;            //6      // Message Aborted bit
   int1 txen;             //7      // Tx/Rx Buffer Selection bit
} TRCON_STRUCT;

typedef struct struct_CiTRCON
{
   TRCON_STRUCT buf0;
   TRCON_STRUCT buf1;
   TRCON_STRUCT buf2;
   TRCON_STRUCT buf3;
   TRCON_STRUCT buf4;
   TRCON_STRUCT buf5;
   TRCON_STRUCT buf6;
   TRCON_STRUCT buf7;
} CiTRCON;

CiTRCON CAN_C1TRCON;
#locate CAN_C1TRCON=getenv("SFR:C1TR01CON")

#if getenv("SFR_VALID:C2TR01CON") && (USE_CAN2_PERIPHERAL == TRUE)
 CiTRCON CAN_C2TRCON;
 #locate CAN_C2TRCON=getenv("SFR:C2TR01CON")
#endif

#word CAN_C1RXD=getenv("SFR:C1RXD")
#word CAN_C1TXD=getenv("SFR:C1TXD")

#if getenv("SFR_VALID:C2RXD") && (USE_CAN2_PERIPHERAL == TRUE)
 #word CAN_C2RXD=getenv("SFR:C2RXD")
 #word CAN_C2TXD=getenv("SFR:C2TXD")
#endif

////////////////////////////////////////////////////////////////////////////////

// ECAN Interrupt Code Register
typedef struct struct_CiVEC {
   uint16_t icode:7;    //0:6    // Interrupt Flag Code bits
   int1 u7;             //7
   uint16_t filhit:5;   //8:12   // Filter Hit Number bits
   uint16_t u13_115:3;  //13:15
} CiVEC;

CiVEC CAN_C1VEC;
#locate CAN_C1VEC=getenv("SFR:C1VEC")

#if getenv("SFR_VALID:C2VEC") && (USE_CAN2_PERIPHERAL == TRUE)
 CiVEC CAN_C2VEC;
 #locate CAN_C2VEC=getenv("SFR:C2VEC")
#endif

// ECAN FIFO Control Register
typedef struct struct_CiFCTRL {
   uint16_t fsa:5;      //0:4    // FIFO Area Starts with Buffer bits
   uint16_t u5_12:8;    //5:12
   uint16_t dmabs:3;    //13:15  // DMA Buffer Size bits
} CiFCTRL;

CiFCTRL CAN_C1FCTRL;
#locate CAN_C1FCTRL=getenv("SFR:C1FCTRL")

#if getenv("SFR_VALID:C2FCTRL") && (USE_CAN2_PERIPHERAL == TRUE)
 CiFCTRL CAN_C2FCTRL;
 #locate CAN_C2FCTRL=getenv("SFR:C2FCTRL")
#endif

// ECAN FIFO Status Register
typedef struct struct_CiFIFO {
   uint16_t fnrb:6;     //0:5    // FIFO Next Read Buffer Pointer bits
   uint16_t u6_7:2;     //6:7
   uint16_t fbp:6;      //8:13   // FIFO Buffer Pointer bits
   uint16_t u14_15:2;   //14:15
} CiFIFO;

CiFIFO CAN_C1FIFO;
#locate CAN_C1FIFO=getenv("SFR:C1FIFO")

#if getenv("SFR_VALID:C2FIFO") && (USE_CAN2_PERIPHERAL == TRUE)
 CiFIFO CAN_C2FIFO;
 #locate CAN_C2FIFO=getenv("SFR:C2FIFO")
#endif

// ECAN Interrupt Flag Register
typedef union union_CiINTF
{
   struct
   {
      int1 tbif;              //0      // TX Buffer Interrupt flag bit
      int1 rbif;              //1      // RX buffer interrupt flag bit
      int1 rbobif;            //2      // RX buffer overflow interrupt flag bit
      int1 fifoif;            //3      // FIFO almost full interrupt flag bit
      int1 u4;                //4
      int1 errif;             //5      // error interrupt flag bit
      int1 wakif;             //6      // bus wake-up activity interrupt flag bit
      int1 ivrif;             //7      // invalid message received interrupt flag bit
      int1 ewarn;             //8      // transmitter or receiver in error state warning bit
      int1 rxwar;             //9      // receiver in error state warning bit
      int1 txwar;             //10     // transmitter in error state warning bit
      int1 rxbp;              //11     // receiver in error state bus passive bit
      int1 txbp;              //12     // transmitter in error state bus passive bit
      int1 txbo;              //13     // transmitter in error state bus off bit
      uint16_t u14_15:2;      //14:15
   };
   uint8_t b[2];
   uint16_t w;
} CiINTF;

CiINTF CAN_C1INTF;
#locate CAN_C1INTF=getenv("SFR:C1INTF")

#if getenv("SFR_VALID:C2INTF") && (USE_CAN2_PERIPHERAL == TRUE)
 CiINTF CAN_C2INTF;
 #locate CAN_C2INTF=getenv("SFR:C2INTF")
#endif

// ECAN Interrupt Enable Register
typedef union union_CiINTE
{
   struct
   {
      int1 tbie;              //0      //TX buffer interrupt enable bit
      int1 rbie;              //1      //RX buffer interrupt enable bit
      int1 rbovie;            //2      //RX buffer overflow interrupt enable bit
      int1 fifoie;            //3      //FIFO almost full interrupt enable bit
      int1 void4;             //4
      int1 errie;             //5      //error interrupt enable bit
      int1 wakie;             //6      //bus wake-up activity interrupt enable bit
      int1 ivrie;             //7      //invalid message received interrupt enable bit
      uint8_t void15_8;       //8:15
   };
   uint8_t b[2];
   uint16_t w;
} CiINTE;

CiINTE CAN_C1INTE;
#word CAN_C1INTE=getenv("SFR:C1INTE")

#if getenv("SFR_VALID:C2INTE") && (USE_CAN2_PERIPHERAL == TRUE)
 CiINTE CAN_C2INTE;
 #locate CAN_C2INTE=getenv("SFR:C2INTE")
#endif

///////////////////////////////////////////////////////////////////////////////

// CAN Global Variables

CAN_OP_MODE g_CANOpmode;

// Buffer for CAN1
#BANK_DMA
uint16_t ecan1_message_buffer[CAN_BUFFER_SIZE][8];
uint8_t ecan1_last_fifo_buffer = (CAN_BUFFER_SIZE - 1);

#if getenv("ENH24") == TRUE
 #if (getenv("DEVICE") != "PIC24EP256GU810") && (getenv("DEVICE") != "PIC24EP256GU814") && \
     (getenv("DEVICE") != "PIC24EP512GP806") && (getenv("DEVICE") != "PIC24EP512GU810") && (getenv("DEVICE") != "PIC24EP512GU814") && \
     (getenv("DEVICE") != "DSPIC33EP256MU806") && (getenv("DEVICE") != "DSPIC33EP256MU810") && (getenv("DEVICE") != "DSPIC33EP256MU814") && \
     (getenv("DEVICE") != "DSPIC33EP512GP806") && (getenv("DEVICE") != "DSPIC33EP512MC806") && (getenv("DEVICE") != "DSPIC33EP512MU810") && (getenv("DEVICE") != "DSPIC33EP512MU814")
  #locate ecan1_message_buffer = 0x1200  //using this address incase code is being debugged
 #else
  #if getenv("PSV") &&  ((getenv("DEVICE") == "PIC24EP512GP806") || (getenv("DEVICE") == "PIC24EP512GU810") || (getenv("DEVICE") == "PIC24EP512GU814") || \
                         (getenv("DEVICE") == "DSPIC33EP512GP806") || (getenv("DEVICE") == "DSPIC33EP512MC806") || (getenv("DEVICE") == "DSPIC33EP512MU810") || (getenv("DEVICE") == "DSPIC33EP512MU814"))
   #locate ecan1_message_buffer = 0xD000
  #endif
 #endif
#endif

// Buffer for CAN2
#if (USE_CAN2_PERIPHERAL == TRUE)
 CAN_OP_MODE g_CAN2Opmode;

 #BANK_DMA
 uint16_t ecan2_message_buffer[CAN2_BUFFER_SIZE][8];
 uint8_t ecan2_last_fifo_buffer = CAN2_BUFFER_SIZE;
   
 #if getenv("ENH24") == TRUE
  #if (getenv("DEVICE") != "PIC24EP256GU810") && (getenv("DEVICE") != "PIC24EP256GU814") && \
      (getenv("DEVICE") != "PIC24EP512GP806") && (getenv("DEVICE") != "PIC24EP512GU810") && (getenv("DEVICE") != "PIC24EP512GU814") && \
      (getenv("DEVICE") != "DSPIC33EP256MU806") && (getenv("DEVICE") != "DSPIC33EP256MU810") && (getenv("DEVICE") != "DSPIC33EP256MU814") && \
      (getenv("DEVICE") != "DSPIC33EP512GP806") && (getenv("DEVICE") != "DSPIC33EP512MC806") && (getenv("DEVICE") != "DSPIC33EP512MU810") && (getenv("DEVICE") != "DSPIC33EP512MU814")
   #locate ecan2_message_buffer = 0x1400  //using this address incase code is being debugged
  #else
   #if getenv("PSV") &&  ((getenv("DEVICE") == "PIC24EP512GP806") || (getenv("DEVICE") == "PIC24EP512GU810") || (getenv("DEVICE") == "PIC24EP512GU814") || \
                         (getenv("DEVICE") == "DSPIC33EP512GP806") || (getenv("DEVICE") == "DSPIC33EP512MC806") || (getenv("DEVICE") == "DSPIC33EP512MU810") || (getenv("DEVICE") == "DSPIC33EP512MU814"))
    #locate ecan2_message_buffer = 0xD200
   #endif
  #endif
 #endif
#endif

#if getenv("PSV") &&  ((getenv("DEVICE") == "PIC24EP512GP806") || (getenv("DEVICE") == "PIC24EP512GU810") || (getenv("DEVICE") == "PIC24EP512GU814") || \
                       (getenv("DEVICE") == "DSPIC33EP512GP806") || (getenv("DEVICE") == "DSPIC33EP512MC806") || (getenv("DEVICE") == "DSPIC33EP512MU810") || (getenv("DEVICE") == "DSPIC33EP512MU814"))
#define _CAN_GIE_SET()     enable_interrupts(GLOBAL)
#define _CAN_GIE_CLEAR()   disable_interrupts(GLOBAL)
#define _CAN_GIE_GET()     interrupt_enabled(GLOBAL)

int1 canGieEnabled;
uint16_t PSVSave = 0;
#word DSRPAG_CAN = getenv("SFR:DSRPAG")

void can_disable_psv(void)
{
   if(DSRPAG_CAN != 0x0001)
   {
      canGieEnabled = _CAN_GIE_GET();
      
      if(canGieEnabled)
         _CAN_GIE_CLEAR();
      
      PSVSave = DSRPAG_CAN;
      
      DSRPAG_CAN = 0x0001;
   }  
}

void can_enable_psv(void)
{
   if(PSVSave != 0)
   {
      DSRPAG_CAN = PSVSave;
      PSVSave = 0;
      
      if(canGieEnabled)
         _CAN_GIE_SET();
   }
}
#else
#define can_disable_psv()
#define can_enable_psv()
#endif

///////////////////////////////////////////////////////////////////////////////

// Prototypes

void can_config_DMA(void);
void can_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type);
void can_set_buffer_id(CAN_TX_HEADER *Header, CAN_BUFFER Buffer);
void can_get_rx_header(CAN_RX_HEADER *Header, uint16_t *Buffer);

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_config_DMA(void);
 void can2_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type);
 void can2_set_buffer_id(CAN_TX_HEADER *Header, CAN_BUFFER Buffer);
#endif

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// can_init()
// can2_init()
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
void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL)
{
  #if CAN_TX_BUFFERS > 0
   CAN_BUFFER i;
  #endif
   
   can_set_mode(CAN_OP_CONFIG, TRUE);   //must be in config mode before params can be set
   
   CAN_C1CTRL1.cancap = CAN_ENABLE_CAN_CAPTURE;
  #if (getenv("DEVICE") == "PIC24EP256GU810") || (getenv("DEVICE") == "PIC24EP256GU814") || \
      (getenv("DEVICE") == "PIC24EP512GP806") || (getenv("DEVICE") == "PIC24EP512GU810") || (getenv("DEVICE") == "PIC24EP512GU814") || \
      (getenv("DEVICE") == "DSPIC33EP256MU806") || (getenv("DEVICE") == "DSPIC33EP256MU810") || (getenv("DEVICE") == "DSPIC33EP256MU814") || \
      (getenv("DEVICE") == "DSPIC33EP512GP806") || (getenv("DEVICE") == "DSPIC33EP512MC806") || (getenv("DEVICE") == "DSPIC33EP512MU810") || (getenv("DEVICE") == "DSPIC33EP512MU814")
   CAN_C1CTRL1.cancks = (CAN_CLOCK_DIVISOR - 1);
  #else
   CAN_C1CTRL1.cancks = (CAN_CLOCK_DIVISOR == 1);
  #endif
   CAN_C1CTRL1.csidl = (CAN_CONTINUE_IN_IDLE == FALSE);
   
   can_set_baud(CAN_CLOCK_SPEED, CAN_BAUD_RATE);
   
   can_set_buffer_size(CAN_BUFFER_SIZE);      // sets size of DMA buffer to use
   can_config_DMA();
   
   //Enable Transmit Buffers
  #if CAN_TX_BUFFERS > 0
   for(i=0;i<CAN_TX_BUFFERS;i++)
      can_enable_b_transfer(i);
  #endif
   
   CAN_C1FEN1.w = 0;    //Disable all filters
   
   //Setup Filters
   can_set_mask_id(CAN_FILTER_MASK_0, CAN_USER_MASK_0, CAN_USER_MASK_0_ID_TYPE, CAN_USER_MASK_0_FILTER_TYPE);
   can_set_filter_id(CAN_FILTER_0, CAN_USER_FILT_0, CAN_USER_FILT_0_TYPE);
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
   
   can_set_mode(Mode);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_init(CAN_OP_MODE Mode=CAN_OP_NORMAL)
   {
     #if CAN2_TX_BUFFERS > 0
      uint8_t i;
     #endif
   
      can2_set_mode(CAN_OP_CONFIG, TRUE);   //must be in config mode before params can be set
      
      CAN_C2CTRL1.cancap = CAN2_ENABLE_CAN_CAPTURE;
     #if (getenv("DEVICE") == "PIC24EP256GU810") || (getenv("DEVICE") == "PIC24EP256GU814") || \
         (getenv("DEVICE") == "PIC24EP512GP806") || (getenv("DEVICE") == "PIC24EP512GU810") || (getenv("DEVICE") == "PIC24EP512GU814") || \
         (getenv("DEVICE") == "DSPIC33EP256MU806") || (getenv("DEVICE") == "DSPIC33EP256MU810") || (getenv("DEVICE") == "DSPIC33EP256MU814") || \
         (getenv("DEVICE") == "DSPIC33EP512GP806") || (getenv("DEVICE") == "DSPIC33EP512MC806") || (getenv("DEVICE") == "DSPIC33EP512MU810") || (getenv("DEVICE") == "DSPIC33EP512MU814")
      CAN_C2CTRL1.cancks = (CAN2_CLOCK_DIVISOR - 1);
     #else
      CAN_C2CTRL1.cancks = (CAN2_CLOCK_DIVISOR == 1);
     #endif
      CAN_C2CTRL1.csidl = (CAN2_CONTINUE_IN_IDLE == FALSE);
      
      can2_set_baud(CAN2_CLOCK_SPEED, CAN2_BAUD_RATE);
      
      can2_set_buffer_size(CAN2_BUFFER_SIZE);      // sets size of DMA buffer to use
      can2_config_DMA();
      
      //Enable Transmit Buffers
     #if CAN2_TX_BUFFERS > 0
      for(i=0;i<CAN2_TX_BUFFERS;i++)
         can2_enable_b_transfer(i);
     #endif
      
      CAN_C2FEN1.w = 0;   //Disable all filters
      
      //Setup Filters
      can2_set_mask_id(CAN_FILTER_MASK_0, CAN2_USER_MASK_0, CAN2_USER_MASK_0_ID_TYPE, CAN2_USER_MASK_0_FILTER_TYPE);
      can2_set_filter_id(CAN_FILTER_0, CAN2_USER_FILT_0, CAN2_USER_FILT_0_TYPE);
      can2_enable_filter(CAN_FILTER_0, CAN2_USER_FILT_0_BUFFER, CAN2_USER_FILT_0_MASK);
      
    #if CAN2_USE_FILTERS
     #ifdef CAN2_USER_MASK_1
      can2_set_mask_id(CAN_FILTER_MASK_1, CAN2_USER_MASK_1, CAN2_USER_MASK_1_ID_TYPE, CAN2_USER_MASK_1_FILTER_TYPE);
     #endif
     
     #ifdef CAN2_USER_MASK_2
      can2_set_mask_id(CAN_FILTER_MASK_2, CAN2_USER_MASK_2, CAN2_USER_MASK_2_ID_TYPE, CAN2_USER_MASK_2_FILTER_TYPE);
     #endif
      
     #ifdef CAN2_USER_FILT_1
      can2_set_filter_id(CAN_FILTER_1, CAN2_USER_FILT_1, CAN2_USER_FILT_1_TYPE);
      can2_enable_filter(CAN_FILTER_1, CAN2_USER_FILT_1_BUFFER, CAN2_USER_FILT_1_MASK);
     #endif
     
     #ifdef CAN_USER_FILT_2
      can2_set_filter_id(CAN_FILTER_2, CAN2_USER_FILT_2, CAN2_USER_FILT_2_TYPE);
      can2_enable_filter(CAN_FILTER_2, CAN2_USER_FILT_2_BUFFER, CAN2_USER_FILT_2_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_3
      can2_set_filter_id(CAN_FILTER_3, CAN2_USER_FILT_3, CAN2_USER_FILT_3_TYPE);
      can2_enable_filter(CAN_FILTER_3, CAN2_USER_FILT_3_BUFFER, CAN2_USER_FILT_3_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_4
      can2_set_filter_id(CAN_FILTER_4, CAN2_USER_FILT_4, CAN2_USER_FILT_4_TYPE);
      can2_enable_filter(CAN_FILTER_4, CAN2_USER_FILT_4_BUFFER, CAN2_USER_FILT_4_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_5
      can2_set_filter_id(CAN_FILTER_5, CAN2_USER_FILT_5, CAN2_USER_FILT_5_TYPE);
      can2_enable_filter(CAN_FILTER_5, CAN2_USER_FILT_5_BUFFER, CAN2_USER_FILT_5_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_6
      can2_set_filter_id(CAN_FILTER_6, CAN2_USER_FILT_6, CAN2_USER_FILT_6_TYPE);
      can2_enable_filter(CAN_FILTER_6, CAN2_USER_FILT_6_BUFFER, CAN2_USER_FILT_6_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_7
      can2_set_filter_id(CAN_FILTER_7, CAN2_USER_FILT_7, CAN2_USER_FILT_7_TYPE);
      can2_enable_filter(CAN_FILTER_7, CAN2_USER_FILT_7_BUFFER, CAN2_USER_FILT_7_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_8
      can2_set_filter_id(CAN_FILTER_8, CAN2_USER_FILT_8, CAN2_USER_FILT_8_TYPE);
      can2_enable_filter(CAN_FILTER_8, CAN2_USER_FILT_8_BUFFER, CAN2_USER_FILT_8_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_9
      can2_set_filter_id(CAN_FILTER_9, CAN2_USER_FILT_9, CAN2_USER_FILT_9_TYPE);
      can2_enable_filter(CAN_FILTER_9, CAN2_USER_FILT_9_BUFFER, CAN2_USER_FILT_9_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_10
      can2_set_filter_id(CAN_FILTER_10, CAN2_USER_FILT_10, CAN2_USER_FILT_10_TYPE);
      can2_enable_filter(CAN_FILTER_10, CAN2_USER_FILT_10_BUFFER, CAN2_USER_FILT_10_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_11
      can2_set_filter_id(CAN_FILTER_11, CAN2_USER_FILT_11, CAN2_USER_FILT_11_TYPE);
      can2_enable_filter(CAN_FILTER_11, CAN2_USER_FILT_11_BUFFER, CAN2_USER_FILT_11_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_12
      can2_set_filter_id(CAN_FILTER_12, CAN2_USER_FILT_12, CAN2_USER_FILT_12_TYPE);
      can2_enable_filter(CAN_FILTER_12, CAN2_USER_FILT_12_BUFFER, CAN2_USER_FILT_12_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_13
      can2_set_filter_id(CAN_FILTER_13, CAN2_USER_FILT_13, CAN2_USER_FILT_13_TYPE);
      can2_enable_filter(CAN_FILTER_13, CAN2_USER_FILT_13_BUFFER, CAN2_USER_FILT_13_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_14
      can2_set_filter_id(CAN_FILTER_14, CAN2_USER_FILT_14, CAN2_USER_FILT_14_TYPE);
      can2_enable_filter(CAN_FILTER_14, CAN2_USER_FILT_14_BUFFER, CAN2_USER_FILT_14_MASK);
     #endif
     
     #ifdef CAN2_USER_FILT_15
      can2_set_filter_id(CAN_FILTER_15, CAN2_USER_FILT_15, CAN2_USER_FILT_15_TYPE);
      can2_enable_filter(CAN_FILTER_15, CAN2_USER_FILT_15_BUFFER, CAN2_USER_FILT_15_MASK);
     #endif
    #endif
      
      can2_set_mode(Mode);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_baud()
// can2_set_baud()
//
// Function for setting the bit rate of the CAN Bus.
//
// Parameters:
//    Clock - the clock speed that is presented to the ECAN peripheral, the
//            speed depends on how the ECAN peripheral and PIC clocks are
//            setup.
// 
//    Rate - the bit rate, max speed is 1 Mbps and speed must be less then or
//           then or equal to Clock and be a multiple of Clock.
//
// Return - can_ec_t error code, see can-PIC24_dsPIC33.h for a description of
//          error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate)
{
   CiCFG1 config1;
   CiCFG2 config2;
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
   
   config1.brp = BRP - 1;
   
   if(tSeg1Bits > 17)
   {
      config2.prseg = 7;
      config2.seg1ph = 7;
      
      tSeg1Bits = 17;
   }
   else
   {
      while((tSeg1Bits < (tBits - tSeg1Bits - 1)) || (tSeg1Bits < 2))
         tSeg1Bits++;
   
      config2.prseg = (tSeg1Bits / 2) - 1;
      config2.seg1ph = (tSeg1Bits - (tSeg1Bits / 2))  - 1;
   }
   
   config2.seg2ph = (tBits - tSeg1Bits) - 2;
   
   config2.sam = CAN_BRG_SAM;
   config2.seg2phts = 1;   //Phase Segment 2 Freely programmable
   config2.wakfil = CAN_BRG_WAKE_FILTER;
   
   if(config2.seg2ph == 0)
      config1.sjw = 0;
   else if(config2.seg2ph < 4)
      config1.sjw = config2.seg2ph - 1;
   else
      config1.sjw = 3;
   
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   CAN_C1CFG1 = config1;
   CAN_C1CFG2 = config2;
   
   can_set_mode(g_CANOpmode, FALSE);
   
   return(CAN_EC_OK);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   can_ec_t can2_set_baud(uint32_t Clock, uint32_t Rate) 
   {
      CiCFG1 config1;
      CiCFG2 config2;
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
         
         tSeg1Bits = (((uint32_t)tBits * CAN2_DEFAULT_SAMPLE_POINT) / 1000) - 1;
      } while((((tBits > 25) || (tBits < 8)) && (BRP < 64)) || ((BRP < 64) && (tSeg1Bits > 16)));
      
      if((tBits > 25) || (tBits < 8))
         return(CAN_EC_BAUD_INVALID);
      
      config1.brp = BRP - 1;
      
      if(tSeg1Bits > 17)
      {
         config2.prseg = 7;
         config2.seg1ph = 7;
         
         tSeg1Bits = 17;
      }
      else
      {
         while((tSeg1Bits < (tBits - tSeg1Bits - 1)) || (tSeg1Bits < 2))
            tSeg1Bits++;
      
         config2.prseg = (tSeg1Bits / 2) - 1;
         config2.seg1ph = (tSeg1Bits - (tSeg1Bits / 2))  - 1;
      }
      
      config2.seg2ph = (tBits - tSeg1Bits) - 2;
      
      config2.sam = CAN2_BRG_SAM;
      config2.seg2phts = 1;   //Phase Segment 2 Freely programmable
      config2.wakfil = CAN2_BRG_WAKE_FILTER;
      
      if(config2.seg2ph == 0)
         config1.sjw = 0;
      else if(config2.seg2ph < 4)
         config1.sjw = config2.seg2ph - 1;
      else
         config1.sjw = 3;
      
      can2_set_mode(CAN_OP_CONFIG, FALSE);
      
      CAN_C2CFG1 = config1;
      CAN_C2CFG2 = config2;
      
      can2_set_mode(g_CAN2Opmode, FALSE);
      
      return(CAN_EC_OK);
   }
#endif
   
///////////////////////////////////////////////////////////////////////////////
// can_set_mode()
// can2_set_mode()
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
//             CAN_OP_LISTEN_ALL
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
   CAN_C1CTRL1.reqop = Mode;
   
   while(CAN_C1CTRL1.opmode != Mode);
   
   if(Update)
      g_CANOpmode = Mode;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE)
   {
      CAN_C2CTRL1.reqop = Mode;
   
      while(CAN_C2CTRL1.opmode != Mode);
      
      if(Update)
         g_CAN2Opmode = Mode;
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_mask_id()
// can2_set_mask_id()
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
         Address = &CAN_C1RXMASK0;
         break;
      case CAN_FILTER_MASK_1:
         Address = &CAN_C1RXMASK1;
         break;
      case CAN_FILTER_MASK_2:
         Address = &CAN_C1RXMASK2;
         break;
      default:
         Address = 0;
         break;
   }
   
   if(Address != 0)
      can_set_id(Address, Id, iType, fType);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType)
   {
      uint16_t *Address;
   
      switch(Mask)
      {
         case CAN_FILTER_MASK_0:
            Address = &CAN_C2RXMASK0;
            break;
         case CAN_FILTER_MASK_1:
            Address = &CAN_C2RXMASK1;
            break;
         case CAN_FILTER_MASK_2:
            Address = &CAN_C2RXMASK2;
            break;
         default:
            Address = 0;
            break;
      }
      
      if(Address != 0)
         can2_set_id(Address, Id, iType, fType);
   
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_filter_id()
// can2_set_filter_id()
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
   
   if(Filter <= CAN_FILTER_15)
   {
      Address = &CAN_C1RXFLT0 + Filter;
      
      can_set_id(Address, Id, Type, Type);
   }
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type)
   {
      uint16_t *Address;
      
      if(Filter <= CAN_FILTER_15)
      {
         Address = &CAN_C2RXFLT0 + Filter;
         
         can2_set_id(Address, Id, Type, Type);
      }
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_buffer_size()
// can2_set_buffer_size()
//
// Configures the number of DMA buffers the ECAN module uses.
//
// Parameters:
//       size - the desired buffer size to be used.  Valid sizes 4, 6, 8, 12,
//              16, 24, and 32.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_buffer_size(uint8_t size)
{
   if (size==4)
   {
      CAN_C1FCTRL.dmabs=0;
      ecan1_last_fifo_buffer = 3;
   }
   else if (size==6)
   {
      CAN_C1FCTRL.dmabs=1;
      ecan1_last_fifo_buffer = 5;
   }
   else if (size==8)
   {
      CAN_C1FCTRL.dmabs=2;
      ecan1_last_fifo_buffer = 7;
   }
   else if (size==12)
   {
      CAN_C1FCTRL.dmabs=3;
      ecan1_last_fifo_buffer = 11;
   }
   else if (size==16)
   {
      CAN_C1FCTRL.dmabs=4;
      ecan1_last_fifo_buffer = 15;
   }
   else if (size==24)
   {
      CAN_C1FCTRL.dmabs=5;
      ecan1_last_fifo_buffer = 23;
   }
   else
   {
      CAN_C1FCTRL.dmabs=6;
      ecan1_last_fifo_buffer = 31;
   }
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_buffer_size(uint8_t size)
   {
      if (size==4)
      {
         CAN_C2FCTRL.dmabs=0;
         ecan2_last_fifo_buffer = 4;
      }
      else if (size==6)
      {
         CAN_C2FCTRL.dmabs=1;
         ecan2_last_fifo_buffer = 6;
      }
      else if (size==8)
      {
         CAN_C2FCTRL.dmabs=2;
         ecan2_last_fifo_buffer = 8;
      }
      else if (size==12)
      {
         CAN_C2FCTRL.dmabs=3;
         ecan2_last_fifo_buffer = 12;
      }
      else if (size==16)
      {
         CAN_C2FCTRL.dmabs=4;
         ecan2_last_fifo_buffer = 16;
      }
      else if (size==24)
      {
         CAN_C2FCTRL.dmabs=5;
         ecan2_last_fifo_buffer = 24;
      }
      else
      {
         CAN_C2FCTRL.dmabs=6;
         ecan2_last_fifo_buffer = 32;
      }
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_fifo_start_buffer()
// can2_set_fifo_start_buffer()
//
// Sets the message buffer that is to be the start of the FIFO buffer.  Value
// must be greater then the last transmit buffer and less then or equal to 
// CAN_BUFFER_SIZE for CAN1 and CAN2_BUFFER_SIZE for CAN2.
//
// By default the can_enable_b_transfer(), can_enable_b_receiver(), 
// can2_enable_b_transfer() and can2_enable_b_receiver() functions set the start
// FIFO buffer to the first available receive buffer for their respective CAN
// peripherals, so the can_set_fifo_start_buffer() and can2_set_fifo_buffer()
// functions should be called after those functions.
//
// Parameters:
//       Buffer - the buffer to be the first message buffer of the CAN FIFO
//                buffer.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_fifo_start_buffer(CAN_BUFFER Buffer)
{
   if(Buffer < 32)
      CAN_C1FCTRL.fsa = Buffer;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_fifo_start_buffer(CAN_BUFFER Buffer)
   {
      if(Buffer < 32)
         CAN_C2FCTRL.fsa = Buffer;
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_enable_filter()
// can2_enable_filter()
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
////////////////////////////////////////////////////////////////////////////////
void can_enable_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer, CAN_FILTER_MASK Mask)
{
   CiBUFPNT *pFilterBuffer;
   uint16_t Index;
   uint32_t Temp;
   
   if(Filter <= CAN_FILTER_15)
   {
      pFilterBuffer = &CAN_C1BUFPNT1 + (Filter / 4);
      Index = (Filter % 4);
   
      can_set_mode(CAN_OP_CONFIG, FALSE);
      
      //Assign Filter to Buffer
      CAN_C1CTRL1.win = 1;
      
      pFilterBuffer->w &= (~(0x000F << (Index * 4)));
      pFilterBuffer->w |= (((uint16_t)(Buffer & 0x0F)) << (Index * 4));
      
      CAN_C1CTRL1.win = 0;
      
      //Assign Mask to Filter
      Temp = 0x00000003;
      Temp <<= (Filter * 2);
      CAN_C1FMSKSEL.dw &= ~Temp;
      Temp = Mask & 0x03;
      Temp <<= (Filter * 2);
      CAN_C1FMSKSEL.dw |= Temp;
      
      //Enable Filter
      CAN_C1FEN1.w |= (0x0001 << Filter);
   
      can_set_mode(g_CANOpmode, FALSE);
   }
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_enable_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer, CAN_FILTER_MASK Mask)
   {
      CiBUFPNT *pFilterBuffer;
      uint16_t Index;
      uint32_t Temp;
      
      if(Filter <= CAN_FILTER_15)
      {
         pFilterBuffer = &CAN_C2BUFPNT1 + (Filter / 4);
         Index = (Filter % 4);
      
         can2_set_mode(CAN_OP_CONFIG, FALSE);
         
         //Assign Filter to Buffer
         CAN_C2CTRL1.win = 1;
         
         pFilterBuffer->w &= (~(0x000F << (Index * 4)));
         pFilterBuffer->w |= (((uint16_t)(Buffer & 0x0F)) << (Index * 4));
         
         CAN_C2CTRL1.win = 0;
         
         //Assign Mask to Filter
         Temp = 0x00000003;
         Temp <<= (Filter * 2);
         CAN_C2FMSKSEL.dw &= ~Temp;
         Temp = Mask & 0x03;
         Temp <<= (Filter * 2);
         CAN_C2FMSKSEL.dw |= Temp;
         
         //Enable Filter
         CAN_C2FEN1.w |= (0x0001 << Filter);
      
         can2_set_mode(g_CAN2Opmode, FALSE);
      }
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_disable_filter()
// can2_disable_filter()
//
// Disables a given acceptance filter.
//
// Parameters:
//    Filter - the filter that is to be disabled.
//
// Returns - Nothing.
////////////////////////////////////////////////////////////////////////////////
void can_disable_filter(CAN_FILTER Filter)
{
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   //Disable Filter
   CAN_C1FEN1.w &= (~(0x0001 << Filter));

   can_set_mode(g_CANOpmode, FALSE);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_disable_filter(CAN_FILTER Filter)
   {
      can2_set_mode(CAN_OP_CONFIG, FALSE);
   
      //Disable Filter
      CAN_C2FEN1.w &= (~(0x0001 << Filter));
   
      can2_set_mode(g_CANOpmode, FALSE);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_putd()
// can2_putd()
//
// Function used to load a message into a transmit buffer, after which when the
// CAN Bus becomes available it will be transmitted.
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, e.g. the message ID.  See
//             CAN_TX_HEADER type in can-PIC24_dsPIC33.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Buffer - optional specifies the buffer to load the message to transmit
//             into.  Defaults to CAN_TX_BUFFER_ANY, meaning messages will be 
//             loaded into first available TX buffer that is free and not setup
//             as an RTR TX buffer.
//
// Returns - can_ec_t error code, see can-PIC24_dsPIC33.h for a description of
//           error codes.
////////////////////////////////////////////////////////////////////////////////
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
{
   can_ec_t Result = CAN_EC_OK;
   
   if(Buffer == CAN_TX_BUFFER_ANY)
   {  
      // find empty transmitter
      if (CAN_C1TRCON.buf0.txen && !CAN_C1TRCON.buf0.txreq && !CAN_C1TRCON.buf0.rtren)
         Buffer = 0;
      else if (CAN_C1TRCON.buf1.txen && !CAN_C1TRCON.buf1.txreq && !CAN_C1TRCON.buf1.rtren)
         Buffer = 1;
      else if (CAN_C1TRCON.buf2.txen && !CAN_C1TRCON.buf2.txreq && !CAN_C1TRCON.buf2.rtren)
         Buffer = 2;
      else if (CAN_C1TRCON.buf3.txen && !CAN_C1TRCON.buf3.txreq && !CAN_C1TRCON.buf3.rtren)
         Buffer = 3;
      else if (CAN_C1TRCON.buf4.txen && !CAN_C1TRCON.buf4.txreq && !CAN_C1TRCON.buf4.rtren)
         Buffer = 4;
      else if (CAN_C1TRCON.buf5.txen && !CAN_C1TRCON.buf5.txreq && !CAN_C1TRCON.buf5.rtren)
         Buffer = 5;
      else if (CAN_C1TRCON.buf6.txen && !CAN_C1TRCON.buf6.txreq && !CAN_C1TRCON.buf6.rtren)
         Buffer = 6;
      else if (CAN_C1TRCON.buf7.txen && !CAN_C1TRCON.buf7.txreq && !CAN_C1TRCON.buf7.rtren)
         Buffer = 7;
      else 
          Result = CAN_EC_BUFFER_TX_FULL;
   }
   else
   {
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            if(CAN_C1TRCON.buf0.txen)
            {
               if(!CAN_C1TRCON.buf0.txreq)
               {
                  if(CAN_C1TRCON.buf0.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_1:
            if(CAN_C1TRCON.buf1.txen)
            {
               if(!CAN_C1TRCON.buf1.txreq)
               {
                  if(CAN_C1TRCON.buf1.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_2:
            if(CAN_C1TRCON.buf2.txen)
            {
               if(!CAN_C1TRCON.buf2.txreq)
               {
                  if(CAN_C1TRCON.buf2.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_3:
            if(CAN_C1TRCON.buf3.txen)
            {
               if(!CAN_C1TRCON.buf3.txreq)
               {
                  if(CAN_C1TRCON.buf3.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_4:
            if(CAN_C1TRCON.buf4.txen)
            {
               if(!CAN_C1TRCON.buf4.txreq)
               {
                  if(CAN_C1TRCON.buf4.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_5:
            if(CAN_C1TRCON.buf5.txen)
            {
               if(!CAN_C1TRCON.buf5.txreq)
               {
                  if(CAN_C1TRCON.buf5.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_6:
            if(CAN_C1TRCON.buf6.txen)
            {
               if(!CAN_C1TRCON.buf6.txreq)
               {
                  if(CAN_C1TRCON.buf6.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_7:
            if(CAN_C1TRCON.buf7.txen)
            {
               if(!CAN_C1TRCON.buf7.txreq)
               {
                  if(CAN_C1TRCON.buf7.rtren)
                     Result = CAN_EC_BUFFER_IS_RTR;
               }
               else
                  Result = CAN_EC_BUFFER_TX_FULL;   
            }
            else
                Result = CAN_EC_BUFFER_NOT_TX;
            break;
         default:
            Result = CAN_EC_BUFFER_NOT_TX;
            break;
      }
   }
   
   if(Result == CAN_EC_OK)
   {
      can_set_buffer_id(Header, Buffer);
      
      can_disable_psv();
      memcpy(&ecan1_message_buffer[Buffer][3], Data, Header->Length);
      can_enable_psv();
      
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            CAN_C1TRCON.buf0.txpri = Header->Priority;
            CAN_C1TRCON.buf0.txreq = TRUE;
            break;
         case CAN_BUFFER_1:
            CAN_C1TRCON.buf1.txpri = Header->Priority;
            CAN_C1TRCON.buf1.txreq = TRUE;
            break;
         case CAN_BUFFER_2:
            CAN_C1TRCON.buf2.txpri = Header->Priority;
            CAN_C1TRCON.buf2.txreq = TRUE;
            break;
         case CAN_BUFFER_3:
            CAN_C1TRCON.buf3.txpri = Header->Priority;
            CAN_C1TRCON.buf3.txreq = TRUE;
            break;
         case CAN_BUFFER_4:
            CAN_C1TRCON.buf4.txpri = Header->Priority;
            CAN_C1TRCON.buf4.txreq = TRUE;
            break;
         case CAN_BUFFER_5:
            CAN_C1TRCON.buf5.txpri = Header->Priority;
            CAN_C1TRCON.buf5.txreq = TRUE;
            break;
         case CAN_BUFFER_6:
            CAN_C1TRCON.buf6.txpri = Header->Priority;
            CAN_C1TRCON.buf6.txreq = TRUE;
            break;
         case CAN_BUFFER_7:
            CAN_C1TRCON.buf7.txpri = Header->Priority;
            CAN_C1TRCON.buf7.txreq = TRUE;
            break;
      }
   }
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   can_ec_t can2_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
   {
      can_ec_t Result = CAN_EC_OK;
      
      if(Buffer == CAN_TX_BUFFER_ANY)
      {  
         // find empty transmitter
         if (CAN_C2TRCON.buf0.txen && !CAN_C2TRCON.buf0.txreq && !CAN_C2TRCON.buf0.rtren)
            Buffer = 0;
         else if (CAN_C2TRCON.buf1.txen && !CAN_C2TRCON.buf1.txreq && !CAN_C2TRCON.buf1.rtren)
            Buffer = 1;
         else if (CAN_C2TRCON.buf2.txen && !CAN_C2TRCON.buf2.txreq && !CAN_C2TRCON.buf2.rtren)
            Buffer = 2;
         else if (CAN_C2TRCON.buf3.txen && !CAN_C2TRCON.buf3.txreq && !CAN_C2TRCON.buf3.rtren)
            Buffer = 3;
         else if (CAN_C2TRCON.buf4.txen && !CAN_C2TRCON.buf4.txreq && !CAN_C2TRCON.buf4.rtren)
            Buffer = 4;
         else if (CAN_C2TRCON.buf5.txen && !CAN_C2TRCON.buf5.txreq && !CAN_C2TRCON.buf5.rtren)
            Buffer = 5;
         else if (CAN_C2TRCON.buf6.txen && !CAN_C2TRCON.buf6.txreq && !CAN_C2TRCON.buf6.rtren)
            Buffer = 6;
         else if (CAN_C2TRCON.buf7.txen && !CAN_C2TRCON.buf7.txreq && !CAN_C2TRCON.buf7.rtren)
            Buffer = 7;
         else 
             Result = CAN_EC_BUFFER_TX_FULL;
      }
      else
      {
         switch(Buffer)
         {
            case CAN_BUFFER_0:
               if(CAN_C2TRCON.buf0.txen)
               {
                  if(!CAN_C2TRCON.buf0.txreq)
                  {
                     if(CAN_C2TRCON.buf0.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_1:
               if(CAN_C2TRCON.buf1.txen)
               {
                  if(!CAN_C2TRCON.buf1.txreq)
                  {
                     if(CAN_C2TRCON.buf1.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_2:
               if(CAN_C2TRCON.buf2.txen)
               {
                  if(!CAN_C2TRCON.buf2.txreq)
                  {
                     if(CAN_C2TRCON.buf2.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_3:
               if(CAN_C2TRCON.buf3.txen)
               {
                  if(!CAN_C2TRCON.buf3.txreq)
                  {
                     if(CAN_C2TRCON.buf3.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_4:
               if(CAN_C2TRCON.buf4.txen)
               {
                  if(!CAN_C2TRCON.buf4.txreq)
                  {
                     if(CAN_C2TRCON.buf4.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_5:
               if(CAN_C2TRCON.buf5.txen)
               {
                  if(!CAN_C2TRCON.buf5.txreq)
                  {
                     if(CAN_C2TRCON.buf5.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_6:
               if(CAN_C2TRCON.buf6.txen)
               {
                  if(!CAN_C2TRCON.buf6.txreq)
                  {
                     if(CAN_C2TRCON.buf6.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            case CAN_BUFFER_7:
               if(CAN_C2TRCON.buf7.txen)
               {
                  if(!CAN_C2TRCON.buf7.txreq)
                  {
                     if(CAN_C2TRCON.buf7.rtren)
                        Result = CAN_EC_BUFFER_IS_RTR;
                  }
                  else
                     Result = CAN_EC_BUFFER_TX_FULL;   
               }
               else
                   Result = CAN_EC_BUFFER_NOT_TX;
               break;
            default:
               Result = CAN_EC_BUFFER_NOT_TX;
               break;
         }
      }
      
      if(Result == CAN_EC_OK)
      {
         can2_set_buffer_id(Header, Buffer);
         
         can_disable_psv();
         memcpy(&ecan2_message_buffer[Buffer][3], Data, Header->Length);
         can_enable_psv();
         
         switch(Buffer)
         {
            case CAN_BUFFER_0:
               CAN_C2TRCON.buf0.txpri = Header->Priority;
               CAN_C2TRCON.buf0.txreq = TRUE;
               break;
            case CAN_BUFFER_1:
               CAN_C2TRCON.buf1.txpri = Header->Priority;
               CAN_C2TRCON.buf1.txreq = TRUE;
               break;
            case CAN_BUFFER_2:
               CAN_C2TRCON.buf2.txpri = Header->Priority;
               CAN_C2TRCON.buf2.txreq = TRUE;
               break;
            case CAN_BUFFER_3:
               CAN_C2TRCON.buf3.txpri = Header->Priority;
               CAN_C2TRCON.buf3.txreq = TRUE;
               break;
            case CAN_BUFFER_4:
               CAN_C2TRCON.buf4.txpri = Header->Priority;
               CAN_C2TRCON.buf4.txreq = TRUE;
               break;
            case CAN_BUFFER_5:
               CAN_C2TRCON.buf5.txpri = Header->Priority;
               CAN_C2TRCON.buf5.txreq = TRUE;
               break;
            case CAN_BUFFER_6:
               CAN_C2TRCON.buf6.txpri = Header->Priority;
               CAN_C2TRCON.buf6.txreq = TRUE;
               break;
            case CAN_BUFFER_7:
               CAN_C2TRCON.buf7.txpri = Header->Priority;
               CAN_C2TRCON.buf7.txreq = TRUE;
               break;
         }
      }
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_getd()
// can2_getd()
//
// Function used to get message from specified RX Buffer.
//
// Parameters:
//    Header - pointer to CAN_RX_HEADER variable to store info about the
//             retrieved message, e.g. the message ID.  See CAN_RX_HEADER type
//             in can_PIC24_dsPIC33.h for more info.
//
//    Data - pointer to an uint8_t array to return data portion of message to.
//
//    Buffer - optional specifies the buffer to get message from.  Can be 
//             CAN_BUFFER_0 to CAN_BUFFER_31, CAN_RX_BUFFER_ANY or 
//             CAN_RX_BUFFER_FIFO, defaults to CAN_RX_BUFFER_FIFO if not
//             specified.
//
// Returns - can_ec_t error code, see can-PIC24_dsPIC33.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer=CAN_RX_BUFFER_FIFO)
{
   can_ec_t Result = CAN_EC_OK;
   uint16_t tBuffer[8];
   
   if(Buffer == CAN_RX_BUFFER_FIFO)
   {
      Buffer = CAN_C1FIFO.fnrb;
      
      if(!CAN_C1RXFUL.array[Buffer])
         Result = CAN_EC_BUFFER_RX_EMPTY;
   }
   else if(Buffer == CAN_RX_BUFFER_ANY)
   {
      for(Buffer=CAN_BUFFER_0;Buffer<=CAN_BUFFER_31;Buffer++)
      {
         if(CAN_C1RXFUL.array[Buffer])
            break;
      }
      
      if(Buffer > CAN_BUFFER_31)
         Result = CAN_EC_BUFFER_RX_EMPTY;
   }
   else if(Buffer <= CAN_BUFFER_31)
   {
      if(!CAN_C1RXFUL.array[Buffer])
         Result = CAN_EC_BUFFER_RX_EMPTY;
   }
   
   if(Result == CAN_EC_OK)
   {
      can_disable_psv();
      memcpy(tBuffer, &ecan1_message_buffer[Buffer][0], sizeof(tBuffer));
      can_enable_psv();
      
      Header->err_ovfl = CAN_C1RXOVF.array[Buffer];
      Header->Buffer = Buffer;
      
     #ifndef DONT_USE_ERRATA_WORKAROUND
      if(Buffer < CAN_BUFFER_16)
      {
         CAN_C1RXFUL.w[0] = (~(0x0001 << Buffer));
         CAN_C1RXOVF.w[0] &= (~(0x0001 << Buffer));
      }
      else
      {
         Buffer -= 16;
         
         CAN_C1RXFUL.w[1] = (~(0x0001 << Buffer));
         CAN_C1RXOVF.w[1] &= (~(0x0001 << Buffer));
      }
     #else
      CAN_C1RXFUL.array[Buffer] = 0;
      CAN_C1RXOVF.array[Buffer] = 0;
     #endif
      
      CAN_C1INTF.ivrif = 0;
      
      if(CAN_C1RXFUL.rxful == 0)
         CAN_C1INTF.rbif = 0;
      
      can_get_rx_header(Header, tBuffer);
      memcpy(Data, &tBuffer[3], Header->Length);
   }
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   can_ec_t can2_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer=CAN_RX_BUFFER_FIFO)
   {
      can_ec_t Result = CAN_EC_OK;
      uint16_t tBuffer[8];
      
      if(Buffer == CAN_RX_BUFFER_FIFO)
      {
         Buffer = CAN_C2FIFO.fnrb;
         
         if(!CAN_C2RXFUL.array[Buffer])
            Result = CAN_EC_BUFFER_RX_EMPTY;
      }
      else if(Buffer == CAN_RX_BUFFER_ANY)
      {
         for(Buffer=CAN_BUFFER_0;Buffer<=CAN_BUFFER_31;Buffer++)
         {
            if(CAN_C2RXFUL.array[Buffer])
               break;
         }
         
         if(Buffer > CAN_BUFFER_31)
            Result = CAN_EC_BUFFER_RX_EMPTY;
      }
      else if(Buffer <= CAN_BUFFER_31)
      {
         if(!CAN_C2RXFUL.array[Buffer])
            Result = CAN_EC_BUFFER_RX_EMPTY;
      }
      
      if(Result == CAN_EC_OK)
      {
         can_disable_psv();
         memcpy(tBuffer, &ecan2_message_buffer[Buffer][0], sizeof(tBuffer));
         can_enable_psv();
         
         Header->err_ovfl = CAN_C2RXOVF.array[Buffer];
         Header->Buffer = Buffer;
         
        #ifndef DONT_USE_ERRATA_WORKAROUND
         if(Buffer < CAN_BUFFER_16)
         {
            CAN_C2RXFUL.w[0] = (~(0x0001 << Buffer));
            CAN_C2RXOVF.w[0] &= (~(0x0001 << Buffer));
         }
         else
         {
            Buffer -= 16;
            
            CAN_C2RXFUL.w[1] = (~(0x0001 << Buffer));
            CAN_C2RXOVF.w[1] &= (~(0x0001 << Buffer));
         }
        #else
         CAN_C2RXFUL.array[Buffer] = 0;
         CAN_C2RXOVF.array[Buffer] = 0;
        #endif
         
         CAN_C2INTF.ivrif = 0;
         
         if(CAN_C2RXFUL.rxful == 0)
            CAN_C2INTF.rbif = 0;
         
         can_get_rx_header(Header, tBuffer);
         memcpy(Data, &tBuffer[3], Header->Length);
      }
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_kbhit()
// can2_kbhit()
//
// Used to determine if there is at least one message to retrieve from the 
// specified buffer.
//
// Parameters:
//    Buffer - optional specifies the CAN RX Buffer to check for message.  Can
//             be CAN_BUFFER_0 to CAN_BUFFER_31, CAN_RX_BUFFER_ANY or 
//             CAN_RX_BUFFER_FIFO, defaults to CAN_RX_BUFFER_FIFO if not
//             specified.
//
// Returns:
//    TRUE - if specified buffer has new message to retrieve.
//    FALSE - if specified buffer does not have new message to retrieve.
///////////////////////////////////////////////////////////////////////////////
int1 can_kbhit(CAN_BUFFER Buffer = CAN_RX_BUFFER_FIFO)
{
   if(Buffer == CAN_RX_BUFFER_FIFO)
      Buffer = CAN_C1FIFO.fnrb;
   
   if(Buffer == CAN_RX_BUFFER_ANY)
   {
      for(Buffer=CAN_BUFFER_0;Buffer<=CAN_BUFFER_31;Buffer++)
      {
         if(CAN_C1RXFUL.array[Buffer])
            break;
      }
      
      if(Buffer > CAN_BUFFER_31)
         return(FALSE);
      else
         return(TRUE);
   }
   else
   {
      if(CAN_C1RXFUL.array[Buffer])
         return(TRUE);
      else
         return(FALSE);
   } 
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   int1 can2_kbhit(CAN_BUFFER Buffer = CAN_RX_BUFFER_FIFO)
   {
      if(Buffer == CAN_RX_BUFFER_FIFO)
         Buffer = CAN_C2FIFO.fnrb;
      
      if(Buffer == CAN_RX_BUFFER_ANY)
      {
         for(Buffer=CAN_BUFFER_0;Buffer<=CAN_BUFFER_31;Buffer++)
         {
            if(CAN_C2RXFUL.array[Buffer])
               break;
         }
         
         if(Buffer > CAN_BUFFER_31)
            return(FALSE);
         else
            return(TRUE);
      }
      else
      {
         if(CAN_C2RXFUL.array[Buffer])
            return(TRUE);
         else
            return(FALSE);
      } 
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_tbe()
// can2_tbe()
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
      if((CAN_C1TRCON.buf0.txen && !CAN_C1TRCON.buf0.txreq && !CAN_C1TRCON.buf0.rtren) || (CAN_C1TRCON.buf1.txen && !CAN_C1TRCON.buf1.txreq && !CAN_C1TRCON.buf1.rtren) ||
         (CAN_C1TRCON.buf2.txen && !CAN_C1TRCON.buf2.txreq && !CAN_C1TRCON.buf2.rtren) || (CAN_C1TRCON.buf3.txen && !CAN_C1TRCON.buf3.txreq && !CAN_C1TRCON.buf3.rtren) ||
         (CAN_C1TRCON.buf4.txen && !CAN_C1TRCON.buf4.txreq && !CAN_C1TRCON.buf4.rtren) || (CAN_C1TRCON.buf5.txen && !CAN_C1TRCON.buf5.txreq && !CAN_C1TRCON.buf5.rtren) ||
         (CAN_C1TRCON.buf6.txen && !CAN_C1TRCON.buf6.txreq && !CAN_C1TRCON.buf6.rtren) || (CAN_C1TRCON.buf7.txen && !CAN_C1TRCON.buf7.txreq && !CAN_C1TRCON.buf7.rtren))
         Result = TRUE;
   }
   else
   {
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            if(CAN_C1TRCON.buf0.txen && !CAN_C1TRCON.buf0.txreq && !CAN_C1TRCON.buf0.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_1:
            if(CAN_C1TRCON.buf1.txen && !CAN_C1TRCON.buf1.txreq && !CAN_C1TRCON.buf1.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_2:
            if(CAN_C1TRCON.buf2.txen && !CAN_C1TRCON.buf2.txreq && !CAN_C1TRCON.buf2.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_3:
            if(CAN_C1TRCON.buf3.txen && !CAN_C1TRCON.buf3.txreq && !CAN_C1TRCON.buf3.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_4:
            if(CAN_C1TRCON.buf4.txen && !CAN_C1TRCON.buf4.txreq && !CAN_C1TRCON.buf4.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_5:
            if(CAN_C1TRCON.buf5.txen && !CAN_C1TRCON.buf5.txreq && !CAN_C1TRCON.buf5.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_6:
            if(CAN_C1TRCON.buf6.txen && !CAN_C1TRCON.buf6.txreq && !CAN_C1TRCON.buf6.rtren)
               Result = TRUE;
            break;
         case CAN_BUFFER_7:
            if(CAN_C1TRCON.buf7.txen && !CAN_C1TRCON.buf7.txreq && !CAN_C1TRCON.buf7.rtren)
               Result = TRUE;
            break;
      }
   }
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   int1 can2_tbe(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
   {
      int1 Result = FALSE;
      
      if(Buffer == CAN_TX_BUFFER_ANY)
      {
         if((CAN_C2TRCON.buf0.txen && !CAN_C2TRCON.buf0.txreq && !CAN_C2TRCON.buf0.rtren) || (CAN_C2TRCON.buf1.txen && !CAN_C2TRCON.buf1.txreq && !CAN_C2TRCON.buf1.rtren) ||
            (CAN_C2TRCON.buf2.txen && !CAN_C2TRCON.buf2.txreq && !CAN_C2TRCON.buf2.rtren) || (CAN_C2TRCON.buf3.txen && !CAN_C2TRCON.buf3.txreq && !CAN_C2TRCON.buf3.rtren) ||
            (CAN_C2TRCON.buf4.txen && !CAN_C2TRCON.buf4.txreq && !CAN_C2TRCON.buf4.rtren) || (CAN_C2TRCON.buf5.txen && !CAN_C2TRCON.buf5.txreq && !CAN_C2TRCON.buf5.rtren) ||
            (CAN_C2TRCON.buf6.txen && !CAN_C2TRCON.buf6.txreq && !CAN_C2TRCON.buf6.rtren) || (CAN_C2TRCON.buf7.txen && !CAN_C2TRCON.buf7.txreq && !CAN_C2TRCON.buf7.rtren))
            Result = TRUE;
      }
      else
      {
         switch(Buffer)
         {
            case CAN_BUFFER_0:
               if(CAN_C2TRCON.buf0.txen && !CAN_C2TRCON.buf0.txreq && !CAN_C2TRCON.buf0.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_1:
               if(CAN_C2TRCON.buf1.txen && !CAN_C2TRCON.buf1.txreq && !CAN_C2TRCON.buf1.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_2:
               if(CAN_C2TRCON.buf2.txen && !CAN_C2TRCON.buf2.txreq && !CAN_C2TRCON.buf2.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_3:
               if(CAN_C2TRCON.buf3.txen && !CAN_C2TRCON.buf3.txreq && !CAN_C2TRCON.buf3.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_4:
               if(CAN_C2TRCON.buf4.txen && !CAN_C2TRCON.buf4.txreq && !CAN_C2TRCON.buf4.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_5:
               if(CAN_C2TRCON.buf5.txen && !CAN_C2TRCON.buf5.txreq && !CAN_C2TRCON.buf5.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_6:
               if(CAN_C2TRCON.buf6.txen && !CAN_C2TRCON.buf6.txreq && !CAN_C2TRCON.buf6.rtren)
                  Result = TRUE;
               break;
            case CAN_BUFFER_7:
               if(CAN_C2TRCON.buf7.txen && !CAN_C2TRCON.buf7.txreq && !CAN_C2TRCON.buf7.rtren)
                  Result = TRUE;
               break;
         }
      }
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_tx_empty()
// can2_tx_empty()
//
// Used to determine if the specified TX buffer is empty.
//
// Parameters:
//    Buffer - optional TX buffer to check.  Default is CAN_TX_BUFFER_ANY, all
//             TX buffers must be empty to return TRUE.
//
// Returns:
//    TRUE - specified TX buffer is empty.
//    FALSE - specified TX buffer is not empty.
///////////////////////////////////////////////////////////////////////////////
int1 can_tx_empty(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
{
   int1 Result = FALSE;
   
   if(Buffer == CAN_TX_BUFFER_ANY)
   {
      if((!CAN_C1TRCON.buf0.txen || CAN_C1TRCON.buf0.rtren || !CAN_C1TRCON.buf0.txreq) && (!CAN_C1TRCON.buf1.txen || CAN_C1TRCON.buf1.rtren || !CAN_C1TRCON.buf1.txreq) &&
         (!CAN_C1TRCON.buf2.txen || CAN_C1TRCON.buf2.rtren || !CAN_C1TRCON.buf2.txreq) && (!CAN_C1TRCON.buf3.txen || CAN_C1TRCON.buf3.rtren || !CAN_C1TRCON.buf3.txreq) &&
         (!CAN_C1TRCON.buf4.txen || CAN_C1TRCON.buf4.rtren || !CAN_C1TRCON.buf4.txreq) && (!CAN_C1TRCON.buf5.txen || CAN_C1TRCON.buf5.rtren || !CAN_C1TRCON.buf5.txreq) &&
         (!CAN_C1TRCON.buf6.txen || CAN_C1TRCON.buf6.rtren || !CAN_C1TRCON.buf6.txreq) && (!CAN_C1TRCON.buf7.txen || CAN_C1TRCON.buf7.rtren || !CAN_C1TRCON.buf7.txreq))
         Result = TRUE;
   }
   else
      Result = can_tbe(Buffer);
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   int1 can2_tx_empty(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY)
   {
      int1 Result = FALSE;
      
      if(Buffer == CAN_TX_BUFFER_ANY)
      {
         if((!CAN_C2TRCON.buf0.txen || CAN_C2TRCON.buf0.rtren || !CAN_C2TRCON.buf0.txreq) && (!CAN_C2TRCON.buf1.txen || CAN_C2TRCON.buf1.rtren || !CAN_C2TRCON.buf1.txreq) &&
            (!CAN_C2TRCON.buf2.txen || CAN_C2TRCON.buf2.rtren || !CAN_C2TRCON.buf2.txreq) && (!CAN_C2TRCON.buf3.txen || CAN_C2TRCON.buf3.rtren || !CAN_C2TRCON.buf3.txreq) &&
            (!CAN_C2TRCON.buf4.txen || CAN_C2TRCON.buf4.rtren || !CAN_C2TRCON.buf4.txreq) && (!CAN_C2TRCON.buf5.txen || CAN_C2TRCON.buf5.rtren || !CAN_C2TRCON.buf5.txreq) &&
            (!CAN_C2TRCON.buf6.txen || CAN_C2TRCON.buf6.rtren || !CAN_C2TRCON.buf6.txreq) && (!CAN_C2TRCON.buf7.txen || CAN_C2TRCON.buf7.rtren || !CAN_C2TRCON.buf7.txreq))
            Result = TRUE;
      }
      else
         Result = can2_tbe(Buffer);
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_abort()
// can2_abort()
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
      case CAN_BUFFER_0:
         CAN_C1TRCON.buf0.txabt = 1;
         break;
      case CAN_BUFFER_1:
         CAN_C1TRCON.buf1.txabt = 1;
         break;
      case CAN_BUFFER_2:
         CAN_C1TRCON.buf2.txabt = 1;
         break;
      case CAN_BUFFER_3:
         CAN_C1TRCON.buf3.txabt = 1;
         break;
      case CAN_BUFFER_4:
         CAN_C1TRCON.buf4.txabt = 1;
         break;
      case CAN_BUFFER_5:
         CAN_C1TRCON.buf5.txabt = 1;
         break;
      case CAN_BUFFER_6:
         CAN_C1TRCON.buf6.txabt = 1;
         break;
      case CAN_BUFFER_7:
         CAN_C1TRCON.buf7.txabt = 1;
         break;
      case CAN_ABORT_ALL:
         CAN_C1CTRL1.abat = 1;
         break;
   }
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_abort(CAN_BUFFER Buffer = CAN_ABORT_ALL)
   {
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            CAN_C2TRCON.buf0.txabt = 1;
            break;
         case CAN_BUFFER_1:
            CAN_C2TRCON.buf1.txabt = 1;
            break;
         case CAN_BUFFER_2:
            CAN_C2TRCON.buf2.txabt = 1;
            break;
         case CAN_BUFFER_3:
            CAN_C2TRCON.buf3.txabt = 1;
            break;
         case CAN_BUFFER_4:
            CAN_C2TRCON.buf4.txabt = 1;
            break;
         case CAN_BUFFER_5:
            CAN_C2TRCON.buf5.txabt = 1;
            break;
         case CAN_BUFFER_6:
            CAN_C2TRCON.buf6.txabt = 1;
            break;
         case CAN_BUFFER_7:
            CAN_C2TRCON.buf7.txabt = 1;
            break;
         case CAN_ABORT_ALL:
            CAN_C2CTRL1.abat = 1;
            break;
      }
   }
#endif

////////////////////////////////////////////////////////////////////////////////
// can_enable_b_transfer()
// can2_enable_b_transfer()
//
// Sets one of the 8 programmable buffers to be a transfer buffer, and sets the 
// start of the FIFO buffer to be the first buffer after the highest buffer set 
// as a transmit buffer. 
//
// Parameters:
//    Buffer - the buffer which is to become a transfer buffer.
//
// Returns - Nothing.
////////////////////////////////////////////////////////////////////////////////
void can_enable_b_transfer(CAN_BUFFER Buffer)
{
   switch(Buffer)
   {
      case CAN_BUFFER_0:
         CAN_C1TRCON.buf0.txen = 1;
         break;
      case CAN_BUFFER_1:
         CAN_C1TRCON.buf1.txen = 1;
         break;
      case CAN_BUFFER_2:
         CAN_C1TRCON.buf2.txen = 1;
         break;
      case CAN_BUFFER_3:
         CAN_C1TRCON.buf3.txen = 1;
         break;
      case CAN_BUFFER_4:
         CAN_C1TRCON.buf4.txen = 1;
         break;
      case CAN_BUFFER_5:
         CAN_C1TRCON.buf5.txen = 1;
         break;
      case CAN_BUFFER_6:
         CAN_C1TRCON.buf6.txen = 1;
         break;
      case CAN_BUFFER_7:
         CAN_C1TRCON.buf7.txen = 1;
         break;
   }
   
   if(CAN_C1TRCON.buf7.txen)
      CAN_C1FCTRL.fsa = 8;
   else if(CAN_C1TRCON.buf6.txen)
      CAN_C1FCTRL.fsa = 7;
   else if(CAN_C1TRCON.buf5.txen)
      CAN_C1FCTRL.fsa = 6;
   else if(CAN_C1TRCON.buf4.txen)
      CAN_C1FCTRL.fsa = 5;
   else if(CAN_C1TRCON.buf3.txen)
      CAN_C1FCTRL.fsa = 4;
   else if(CAN_C1TRCON.buf2.txen)
      CAN_C1FCTRL.fsa = 3;
   else if(CAN_C1TRCON.buf1.txen)
      CAN_C1FCTRL.fsa = 2;
   else if(CAN_C1TRCON.buf0.txen)
      CAN_C1FCTRL.fsa = 1;   
   else
      CAN_C1FCTRL.fsa = 0;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_enable_b_transfer(uint8_t Buffer)
   {
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            CAN_C2TRCON.buf0.txen = 1;
            break;
         case CAN_BUFFER_1:
            CAN_C2TRCON.buf1.txen = 1;
            break;
         case CAN_BUFFER_2:
            CAN_C2TRCON.buf2.txen = 1;
            break;
         case CAN_BUFFER_3:
            CAN_C2TRCON.buf3.txen = 1;
            break;
         case CAN_BUFFER_4:
            CAN_C2TRCON.buf4.txen = 1;
            break;
         case CAN_BUFFER_5:
            CAN_C2TRCON.buf5.txen = 1;
            break;
         case CAN_BUFFER_6:
            CAN_C2TRCON.buf6.txen = 1;
            break;
         case CAN_BUFFER_7:
            CAN_C2TRCON.buf7.txen = 1;
            break;
      }
      
      if(CAN_C2TRCON.buf7.txen)
         CAN_C2FCTRL.fsa = 8;
      else if(CAN_C2TRCON.buf6.txen)
         CAN_C2FCTRL.fsa = 7;
      else if(CAN_C2TRCON.buf5.txen)
         CAN_C2FCTRL.fsa = 6;
      else if(CAN_C2TRCON.buf4.txen)
         CAN_C2FCTRL.fsa = 5;
      else if(CAN_C2TRCON.buf3.txen)
         CAN_C2FCTRL.fsa = 4;
      else if(CAN_C2TRCON.buf2.txen)
         CAN_C2FCTRL.fsa = 3;
      else if(CAN_C2TRCON.buf1.txen)
         CAN_C2FCTRL.fsa = 2;
      else if(CAN_C2TRCON.buf0.txen)
         CAN_C2FCTRL.fsa = 1;   
      else
         CAN_C2FCTRL.fsa = 0;
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_enable_b_receiver()
// can2_enable_b_receiver()
//
// Sets one of the 8 programmable buffers to be a receiver buffer, and sets the 
// start of the FIFO buffer to be the first buffer after the highest buffer set 
// as a transmit buffer.
//
// Parameters:
//    Buffer - the buffer which is to become a receiver buffer.
//
// Returns - Nothing.
////////////////////////////////////////////////////////////////////////////////
void can_enable_b_receiver(CAN_BUFFER Buffer)
{
   switch(Buffer)
   {
      case CAN_BUFFER_0:
         CAN_C1TRCON.buf0.txen = 0;
         break;
      case CAN_BUFFER_1:
         CAN_C1TRCON.buf1.txen = 0;
         break;
      case CAN_BUFFER_2:
         CAN_C1TRCON.buf2.txen = 0;
         break;
      case CAN_BUFFER_3:
         CAN_C1TRCON.buf3.txen = 0;
         break;
      case CAN_BUFFER_4:
         CAN_C1TRCON.buf4.txen = 0;
         break;
      case CAN_BUFFER_5:
         CAN_C1TRCON.buf5.txen = 0;
         break;
      case CAN_BUFFER_6:
         CAN_C1TRCON.buf6.txen = 0;
         break;
      case CAN_BUFFER_7:
         CAN_C1TRCON.buf7.txen = 0;
         break;
   }
   
   if(CAN_C1TRCON.buf7.txen)
      CAN_C1FCTRL.fsa = 8;
   else if(CAN_C1TRCON.buf6.txen)
      CAN_C1FCTRL.fsa = 7;
   else if(CAN_C1TRCON.buf5.txen)
      CAN_C1FCTRL.fsa = 6;
   else if(CAN_C1TRCON.buf4.txen)
      CAN_C1FCTRL.fsa = 5;
   else if(CAN_C1TRCON.buf3.txen)
      CAN_C1FCTRL.fsa = 4;
   else if(CAN_C1TRCON.buf2.txen)
      CAN_C1FCTRL.fsa = 3;
   else if(CAN_C1TRCON.buf1.txen)
      CAN_C1FCTRL.fsa = 2;
   else if(CAN_C1TRCON.buf0.txen)
      CAN_C1FCTRL.fsa = 1;   
   else
      CAN_C1FCTRL.fsa = 0;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_enable_b_receiver(CAN_BUFFER Buffer)
   {
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            CAN_C2TRCON.buf0.txen = 0;
            break;
         case CAN_BUFFER_1:
            CAN_C2TRCON.buf1.txen = 0;
            break;
         case CAN_BUFFER_2:
            CAN_C2TRCON.buf2.txen = 0;
            break;
         case CAN_BUFFER_3:
            CAN_C2TRCON.buf3.txen = 0;
            break;
         case CAN_BUFFER_4:
            CAN_C2TRCON.buf4.txen = 0;
            break;
         case CAN_BUFFER_5:
            CAN_C2TRCON.buf5.txen = 0;
            break;
         case CAN_BUFFER_6:
            CAN_C2TRCON.buf6.txen = 0;
            break;
         case CAN_BUFFER_7:
            CAN_C2TRCON.buf7.txen = 0;
            break;
      }
      
      if(CAN_C2TRCON.buf7.txen)
         CAN_C2FCTRL.fsa = 8;
      else if(CAN_C2TRCON.buf6.txen)
         CAN_C2FCTRL.fsa = 7;
      else if(CAN_C2TRCON.buf5.txen)
         CAN_C2FCTRL.fsa = 6;
      else if(CAN_C2TRCON.buf4.txen)
         CAN_C2FCTRL.fsa = 5;
      else if(CAN_C2TRCON.buf3.txen)
         CAN_C2FCTRL.fsa = 4;
      else if(CAN_C2TRCON.buf2.txen)
         CAN_C2FCTRL.fsa = 3;
      else if(CAN_C2TRCON.buf1.txen)
         CAN_C2FCTRL.fsa = 2;
      else if(CAN_C2TRCON.buf0.txen)
         CAN_C2FCTRL.fsa = 1;   
      else
         CAN_C2FCTRL.fsa = 0;
   }
#endif

////////////////////////////////////////////////////////////////////////////////
// can_enable_rtr()
// can2_enable_rtr()
//
// Used to enable RTR messages on one of the TX buffers.
//
// Parameters:
//    Buffer - the buffer to enable as an RTR buffer.
//
// Returns - can_ec_t error code, see can-PIC24_dsPIC33.h for a description of
//           error codes.
////////////////////////////////////////////////////////////////////////////////
can_ec_t can_enable_rtr(CAN_BUFFER Buffer)
{
   can_ec_t Result = CAN_EC_OK;

   switch(Buffer)
   {
      case CAN_BUFFER_0:
         if(CAN_C1TRCON.buf0.txen)
            CAN_C1TRCON.buf0.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_1:
         if(CAN_C1TRCON.buf1.txen)
            CAN_C1TRCON.buf1.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_2:
         if(CAN_C1TRCON.buf2.txen)
            CAN_C1TRCON.buf2.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_3:
         if(CAN_C1TRCON.buf3.txen)
            CAN_C1TRCON.buf3.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_4:
         if(CAN_C1TRCON.buf4.txen)
            CAN_C1TRCON.buf4.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_5:
         if(CAN_C1TRCON.buf5.txen)
            CAN_C1TRCON.buf5.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_6:
         if(CAN_C1TRCON.buf6.txen)
            CAN_C1TRCON.buf6.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_7:
         if(CAN_C1TRCON.buf7.txen)
            CAN_C1TRCON.buf7.rtren = 1;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      default:
         Result = CAN_EC_BUFFER_NO_RTR;
         break;
   }
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   can_ec_t can2_enable_rtr(CAN_BUFFER Buffer)
   {
      can_ec_t Result = CAN_EC_OK;

      switch(Buffer)
      {
         case CAN_BUFFER_0:
            if(CAN_C2TRCON.buf0.txen)
               CAN_C2TRCON.buf0.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_1:
            if(CAN_C2TRCON.buf1.txen)
               CAN_C2TRCON.buf1.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_2:
            if(CAN_C2TRCON.buf2.txen)
               CAN_C2TRCON.buf2.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_3:
            if(CAN_C2TRCON.buf3.txen)
               CAN_C2TRCON.buf3.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_4:
            if(CAN_C2TRCON.buf4.txen)
               CAN_C2TRCON.buf4.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_5:
            if(CAN_C2TRCON.buf5.txen)
               CAN_C2TRCON.buf5.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_6:
            if(CAN_C2TRCON.buf6.txen)
               CAN_C2TRCON.buf6.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_7:
            if(CAN_C2TRCON.buf7.txen)
               CAN_C2TRCON.buf7.rtren = 1;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         default:
            Result = CAN_EC_BUFFER_NO_RTR;
            break;
      }
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_disable_rtr()
// can2_disable_rtr()
//
// Used to disable RTR messages on one of the TX buffers.
//
// Parameters:
//    Buffer - the CAN TX Buffer to disable RTR messages for.
//
// Returns - can_ec_t error code, see can-PIC24_dsPIC33.h for a description of
//           error codes.
////////////////////////////////////////////////////////////////////////////////
can_ec_t can_disable_rtr(CAN_BUFFER Buffer)
{
   can_ec_t Result = CAN_EC_OK;
   
   switch(Buffer)
   {
      case CAN_BUFFER_0:
         if(CAN_C1TRCON.buf0.txen)
            CAN_C1TRCON.buf0.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_1:
         if(CAN_C1TRCON.buf1.txen)
            CAN_C1TRCON.buf1.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_2:
         if(CAN_C1TRCON.buf2.txen)
            CAN_C1TRCON.buf2.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_3:
         if(CAN_C1TRCON.buf3.txen)
            CAN_C1TRCON.buf3.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_4:
         if(CAN_C1TRCON.buf4.txen)
            CAN_C1TRCON.buf4.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_5:
         if(CAN_C1TRCON.buf5.txen)
            CAN_C1TRCON.buf5.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_6:
         if(CAN_C1TRCON.buf6.txen)
            CAN_C1TRCON.buf6.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_7:
         if(CAN_C1TRCON.buf7.txen)
            CAN_C1TRCON.buf7.rtren = 0;
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      default:
         Result = CAN_EC_BUFFER_NO_RTR;
         break;
   }
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   can_ec_t can2_disable_rtr(CAN_BUFFER Buffer)
   {
      can_ec_t Result = CAN_EC_OK;
   
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            if(CAN_C2TRCON.buf0.txen)
               CAN_C2TRCON.buf0.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_1:
            if(CAN_C2TRCON.buf1.txen)
               CAN_C2TRCON.buf1.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_2:
            if(CAN_C2TRCON.buf2.txen)
               CAN_C2TRCON.buf2.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_3:
            if(CAN_C2TRCON.buf3.txen)
               CAN_C2TRCON.buf3.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_4:
            if(CAN_C2TRCON.buf4.txen)
               CAN_C2TRCON.buf4.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_5:
            if(CAN_C2TRCON.buf5.txen)
               CAN_C2TRCON.buf5.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_6:
            if(CAN_C2TRCON.buf6.txen)
               CAN_C2TRCON.buf6.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_7:
            if(CAN_C2TRCON.buf7.txen)
               CAN_C2TRCON.buf7.rtren = 0;
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         default:
            Result = CAN_EC_BUFFER_NO_RTR;
            break;
      }
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_load_rtr()
// can2_load_rtr()
//
// Used to load a message into a TX buffer that will be transmitted when an
// remote transmission request is received. 
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, e.g. the message ID.  See
//             CAN_TX_HEADER type in can-PIC24_dsPIC33.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Buffer - The CAN TX buffer to load the message into.  Only TX Buffers
//             can be used to responded to remote transmission requests.
//
//
// Returns - can_ec_t error code, see can-PIC24_dsPIC33.h for a description of
//           error codes.
////////////////////////////////////////////////////////////////////////////////
can_ec_t can_load_rtr(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer)
{
   can_ec_t Result = CAN_EC_OK;
   
   switch(Buffer)
   {
      case CAN_BUFFER_0:
         if(CAN_C1TRCON.buf0.txen)
         {
            if(CAN_C1TRCON.buf0.rtren)
               CAN_C1TRCON.buf0.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
               
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_1:
         if(CAN_C1TRCON.buf1.txen)
         {
            if(CAN_C1TRCON.buf1.rtren)
               CAN_C1TRCON.buf1.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_2:
         if(CAN_C1TRCON.buf2.txen)
         {
            if(CAN_C1TRCON.buf2.rtren)
               CAN_C1TRCON.buf2.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_3:
         if(CAN_C1TRCON.buf3.txen)
         {
            if(CAN_C1TRCON.buf3.rtren)
               CAN_C1TRCON.buf3.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_4:
         if(CAN_C1TRCON.buf4.txen)
         {
            if(CAN_C1TRCON.buf4.rtren)
               CAN_C1TRCON.buf4.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_5:
         if(CAN_C1TRCON.buf5.txen)
         {
            if(CAN_C1TRCON.buf5.rtren)
               CAN_C1TRCON.buf5.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_6:
         if(CAN_C1TRCON.buf6.txen)
         {
            if(CAN_C1TRCON.buf6.rtren)
               CAN_C1TRCON.buf6.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      case CAN_BUFFER_7:
         if(CAN_C1TRCON.buf7.txen)
         {
            if(CAN_C1TRCON.buf7.rtren)
               CAN_C1TRCON.buf7.txpri = Header->Priority;
            else
               Result = CAN_EC_BUFFER_NO_RTR;
         }
         else
            Result = CAN_EC_BUFFER_NOT_TX;
         break;
      default:
         Result = CAN_EC_BUFFER_NO_RTR;
         break;
   }
   
   if(Result == CAN_EC_OK)
   {
      can_set_buffer_id(Header, Buffer);
      
      can_disable_psv();
      memcpy(&ecan1_message_buffer[Buffer][3], Data, Header->Length);
      can_enable_psv();
   }
   
   return(Result);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   can_ec_t can2_load_rtr(CAN_BUFFER Buffer, CAN_TX_HEADER *Header, uint8_t *Data)
   {
      can_ec_t Result = CAN_EC_OK;
      
      switch(Buffer)
      {
         case CAN_BUFFER_0:
            if(CAN_C2TRCON.buf0.txen)
            {
               if(CAN_C2TRCON.buf0.rtren)
                  CAN_C2TRCON.buf0.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
                  
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_1:
            if(CAN_C2TRCON.buf1.txen)
            {
               if(CAN_C2TRCON.buf1.rtren)
                  CAN_C2TRCON.buf1.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_2:
            if(CAN_C2TRCON.buf2.txen)
            {
               if(CAN_C2TRCON.buf2.rtren)
                  CAN_C2TRCON.buf2.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_3:
            if(CAN_C2TRCON.buf3.txen)
            {
               if(CAN_C2TRCON.buf3.rtren)
                  CAN_C2TRCON.buf3.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_4:
            if(CAN_C2TRCON.buf4.txen)
            {
               if(CAN_C2TRCON.buf4.rtren)
                  CAN_C2TRCON.buf4.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_5:
            if(CAN_C2TRCON.buf5.txen)
            {
               if(CAN_C2TRCON.buf5.rtren)
                  CAN_C2TRCON.buf5.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_6:
            if(CAN_C2TRCON.buf6.txen)
            {
               if(CAN_C2TRCON.buf6.rtren)
                  CAN_C2TRCON.buf6.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         case CAN_BUFFER_7:
            if(CAN_C2TRCON.buf7.txen)
            {
               if(CAN_C2TRCON.buf7.rtren)
                  CAN_C2TRCON.buf7.txpri = Header->Priority;
               else
                  Result = CAN_EC_BUFFER_NO_RTR;
            }
            else
               Result = CAN_EC_BUFFER_NOT_TX;
            break;
         default:
            Result = CAN_EC_BUFFER_NO_RTR;
            break;
      }
      
      if(Result == CAN_EC_OK)
      {
         can2_set_buffer_id(Header, Buffer);
         
         can_disable_psv();
         memcpy(&ecan2_message_buffer[Buffer][3], Data, Header->Length);
         can_enable_psv();
      }
      
      return(Result);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_enable_interrupts()
// can2_enable_interrupts()
//
//  Used to enable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to enable, can be the following:
//       CAN_INTERRUPT_TX
//       CAN_INTERRUPT_RX
//       CAN_INTERRUPT_RXOV
//       CAN_INTERRUPT_FIFO
//       CAN_INTERRUPT_ERR
//       CAN_INTERRUPT_WAKE
//       CAN_INTERRUPT_INVALID
//
//    You can or these together (via |) to enable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_interrupts(CAN_INTERRUPT Setting)
{
   CAN_C1INTE.b[0] |= Setting;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_enable_interrupts(CAN_INTERRUPT Setting)
   {
      CAN_C2INTE.b[0] |= Setting;
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_disable_interrupts()
// can2_disable_interrupts()
//
// Used to disable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to disable, can be the following:
//       CAN_INTERRUPT_TX
//       CAN_INTERRUPT_RX
//       CAN_INTERRUPT_RXOV
//       CAN_INTERRUPT_FIFO
//       CAN_INTERRUPT_ERR
//       CAN_INTERRUPT_WAKE
//       CAN_INTERRUPT_INVALID
//
//    You can or these together (via |) to disable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_interrupts(CAN_INTERRUPT Setting)
{
   CAN_C1INTE.b[0] &= ~Setting;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_disable_interrupts(CAN_INTERRUPT Setting)
   {
      CAN_C2INTE.b[0] &= ~Setting;
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_interrupts_active()
// can2_interrupts_active()
//
// Used to determined which CAN Interrupts are active.  
//
// Parameters - None.
//
// Returns - CAN_INTERRUPT type.  If an interrupt is active the corresponding
//           bit will be set, set CAN_INTERRUPT type in can-PIC24_dsPIC33.h to
//           determine which bits correspond to which interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_INTERRUPT can_interrupts_active(void)
{
   return(CAN_C1INTF.b[0] & 0xEF);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   CAN_INTERRUPT can2_interrupts_active(void)
   {
      return(CAN_C2INTF.b[0] & 0xEF);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_interrupt_active()
// can2_interrupt_active()
//
// Used to determine if specified CAN Interrupt is active. 
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
   if((can_interrupts_active() & Interrupt) != 0)
      return(TRUE);
   else
      return(FALSE);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   int1 can2_interrupt_active(CAN_INTERRUPT Interrupt)
   {
      if((can2_interrupts_active() & Interrupt) != 0)
         return(TRUE);
      else
         return(FALSE);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_clear_interrupt()
// can2_clear_interrupt()
//
// Used to clear specified CAN Interrupt flag.
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
   
   CAN_C1INTF.b[0] &= Interrupt;
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_clear_interrupt(CAN_INTERRUPT Interrupt)
   {
      Interrupt = ~Interrupt;
      
      CAN_C2INTF.b[0] &= Interrupt;
   }
#endif

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// can_config_DMA()
// can2_config_DMA()
//
// Configures the DMA buffer to use the ECAN Peripheral.
//
// Parameters - None.
//
// Returns - Nothing.       
////////////////////////////////////////////////////////////////////////////////
void can_config_DMA(void)
{
   // transmission
   setup_dma(CAN_TX_DMA_CHANNEL, DMA_OUT_ECAN1, DMA_WORD);
   dma_start(CAN_TX_DMA_CHANNEL, DMA_PERIF_ADDR, &ecan1_message_buffer[0][0], 7);
   
   // receive
   setup_dma(CAN_RX_DMA_CHANNEL, DMA_IN_ECAN1, DMA_WORD);
   dma_start(CAN_RX_DMA_CHANNEL, DMA_PERIF_ADDR, &ecan1_message_buffer[0][0], 7);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_config_DMA(void)
   {
      // transmission
      setup_dma(CAN2_TX_DMA_CHANNEL, DMA_OUT_ECAN2, DMA_WORD);
      dma_start(CAN2_TX_DMA_CHANNEL, DMA_PERIF_ADDR, &ecan2_message_buffer[0][0], 7);
      
      // receive
      setup_dma(CAN2_RX_DMA_CHANNEL, DMA_IN_ECAN2, DMA_WORD);
      dma_start(CAN2_RX_DMA_CHANNEL, DMA_PERIF_ADDR, &ecan2_message_buffer[0][0], 7);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_id()
// can2_set_id()
//
// Sets the xxxxEID and xxxxSID registers for the acceptance filter or
// acceptance filter mask to the specified ID and settings.
//
// Parameters:
//    Addr - pointer to start address of the of ID registers for the acceptance
//           filter or acceptance filter mask to set.
//
//    Id - the ID of the filter or mask to set.
//
//    Ext - if TRUE sets an EID ID, if FALSE sets a SID ID.
//
//    Type - for acceptance filter mask this sets the MIDE bit.  The MIDE bit
//           controls whether the EXIDE bit of the filter is used.  TRUE match
//           only messages types that corresponds to the EXIDE bit in filter, 
//           FALSE match either standard or extended address message if filter
//           matches.  For acceptance filter if MIDE bit in mask is set, TRUE
//           match only extended identifier addresses or FALSE match only
//           standard identifier addresses.
//
// Returns - Nothing.
////////////////////////////////////////////////////////////////////////////////
void can_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type)
{
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   CAN_C1CTRL1.win = 1;
   
   if(Ext)   //extended
   {
      //eid
      ((CiRXFLT *)Addr)->EID0_15 = (Id & 0xffff);               //0:15
   
      //sid
      ((CiRXFLT *)Addr)->EID16_17 = ((Id & 0x30000) >> 16);     //16:17
      ((CiRXFLT *)Addr)->SID = ((Id & 0x1FFC0000) >> 18);       //18:28
      ((CiRXFLT *)Addr)->EXIDE = Type;
   }  
   else      //standard
   {
      //eid
      ((CiRXFLT *)Addr)->EID0_15 = 0;
      ((CiRXFLT *)Addr)->EID16_17 = 0;

      //sid
      ((CiRXFLT *)Addr)->SID = Id & 0x7FF;    //0:10
      ((CiRXFLT *)Addr)->EXIDE = Type;
   }
   
   CAN_C1CTRL1.win = 0;
   
   can_set_mode(g_CANOpmode, FALSE);
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type)
   {
      can2_set_mode(CAN_OP_CONFIG, FALSE);
   
      CAN_C2CTRL1.win = 1;
      
      if(Ext)   //extended
      {
         //eid
         ((CiRXFLT *)Addr)->EID0_15 = (Id & 0xffff);               //0:15
      
         //sid
         ((CiRXFLT *)Addr)->EID16_17 = ((Id & 0x30000) >> 16);     //16:17
         ((CiRXFLT *)Addr)->SID = ((Id & 0x1FFC0000) >> 18);       //18:28
         ((CiRXFLT *)Addr)->EXIDE = Type;
      }  
      else      //standard
      {
         //eid
         ((CiRXFLT *)Addr)->EID0_15 = 0;
         ((CiRXFLT *)Addr)->EID16_17 = 0;
   
         //sid
         ((CiRXFLT *)Addr)->SID = Id & 0x7FF;    //0:10
         ((CiRXFLT *)Addr)->EXIDE = Type;
      }
      
      CAN_C2CTRL1.win = 0;
      
      can2_set_mode(g_CAN2Opmode, FALSE);
   }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_buffer_id()
// can2_set_buffer_id()
//
// Used to load the TX Buffer's Id, RTR bit and Data length into the message
// buffer.
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER structure containing the Id, RTR bit and
//             message length to send.
//    
//    Buffer - the buffer to load.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_buffer_id(CAN_TX_HEADER *Header, CAN_BUFFER Buffer)
{
   uint16_t *Ptr;
   uint16_t Temp;
   uint32_t Id;
   
   can_disable_psv();
   
   Ptr = &ecan1_message_buffer[Buffer][2];
   Id = Header->Id;
   
   if(Header->ext)
   {
      //eid
      *Ptr &= 0x0000;
      Temp = ((Id & 0x3F) << 10) | ((uint16_t)Header->rtr << 9) | (Header->Length & 0x0F);
      *Ptr |= Temp;
      Ptr--;
      Temp = ((Id & 0x3FFC0) >> 6);
      *Ptr = Temp;
      
      //sid
      Ptr--;
      Temp = (((Id & 0x1FFC0000) >> 16) | 0x0003);
      *Ptr = Temp;
   }
   else
   {
      //eid
      *Ptr = (uint16_t)(Header->Length & 0x0F);
      Ptr--;
      *Ptr = 0;
     
      //sid
      Ptr--;
      Temp = ((Id & 0x7FF) << 2) | ((uint16_t)Header->rtr << 1);
      *Ptr = Temp;
   }
   
   can_enable_psv();
}

#if (USE_CAN2_PERIPHERAL == TRUE)
   void can2_set_buffer_id(CAN_TX_HEADER *Header, CAN_BUFFER Buffer)
   {
      uint16_t *Ptr;
      uint16_t Temp;
      uint32_t Id;
      
      can_disable_psv();
      
      Ptr = &ecan2_message_buffer[Buffer][2];
      Id = Header->Id;
      
      if(Header->ext)
      {
         //eid
         *Ptr &= 0x0000;
         Temp = ((Id & 0x3F) << 10) | ((uint16_t)Header->rtr << 9) | (Header->Length & 0x0F);
         *Ptr |= Temp;
         Ptr--;
         Temp = ((Id & 0x3FFC0) >> 6);
         *Ptr = Temp;
         
         //sid
         Ptr--;
         Temp = (((Id & 0x1FFC0000) >> 16) | 0x0003);
         *Ptr = Temp;
      }
      else
      {
         //eid
         *Ptr = (uint16_t)(Header->Length & 0x0F);
         Ptr--;
         *Ptr = 0;
        
         //sid
         Ptr--;
         Temp = ((Id & 0x7FF) << 2) | ((uint16_t)Header->rtr << 1);
         *Ptr = Temp;
      }
      
      can_enable_psv();
   }
#endif

void can_get_rx_header(CAN_RX_HEADER *Header, uint16_t *Buffer)
{
   uint16_t Temp, Temp1;
   
   Header->ext = bit_test(Buffer[0], 0);
      
   if(Header->ext)
   {
      //EID
      Temp = ((Buffer[2] & 0xFC00) >> 10) & 0x3F;
      Header->rtr = bit_test(Buffer[2], 9);
      
      Temp |= ((Buffer[1] & 0x3FF) << 6);
      Temp1 = (Buffer[1] & 0xC00) >> 10;
      
      //SID
      Temp1 |= (Buffer[0] & 0x1FFC);
      Header->Id = make32(Temp1, Temp);
   }
   else 
   {
      //SID
      Header->Id = ((Buffer[0] & 0x1FFC) >> 2) & 0x7FF;
      Header->rtr = bit_test(Buffer[0], 1);
   }
   
   Header->Length = Buffer[2] & 0x0F;
   Header->Filter = make8(Buffer[7], 1) & 0x1F;
}
