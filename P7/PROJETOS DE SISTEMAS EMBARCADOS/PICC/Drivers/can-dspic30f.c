///////////////////////////////////////////////////////////////////////////////
////                             can-dsPIC30f.c                            ////
////                                                                       ////
////  Driver for Microchip's dsPIC30F401x, dsPIC30F501x and dsPIC30F601xA  ////
////  devices with a CAN peripheral.                                       ////
////                                                                       ////
////  see can-dspic30f.h for API.                                          ////
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

#include <can-dsPIC30f.h>

// CAN Baud Rate Settings
#ifndef CAN_CLOCK_DIVISOR
 #define CAN_CLOCK_DIVISOR    1     //1 or 4
#endif

#if (CAN_CLOCK_DIVISOR != 1) && (CAN_CLOCK_DIVISOR != 4)
 #error The CAN Clock Divisor can only be 1 or 4
#endif

#define CAN_CLOCK_SPEED    (getenv("CLOCK") / CAN_CLOCK_DIVISOR)

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

// Other CAN Peripheral Settings
#ifndef CAN_BRG_SAM
 #define CAN_BRG_SAM                0        //sample of the can bus line (def: bus line is sampled 1 times prior to sample point)
#endif

#ifndef CAN_BRG_WAKE_FILTER
 #define CAN_BRG_WAKE_FILTER        FALSE    //selects can bus line filter for wake up bit
#endif

#ifndef CAN_USE_RX_DOUBLE_BUFFER
 #define CAN_USE_RX_DOUBLE_BUFFER   TRUE     //TRUE-if buffer 0 overflows massage will be loaded into buffer 1, FALSE-if buffer 0 overflows message will not be loaded into buffer 1
#endif

#ifndef CAN_ENABLE_CAN_CAPTURE
 #define CAN_ENABLE_CAN_CAPTURE     FALSE
#endif

#ifndef CAN_CONTINUE_IN_IDLE
 #define CAN_CONTINUE_IN_IDLE       FALSE
#endif

#if (USE_CAN2_PERIPHERAL == TRUE)
 // CAN2 Baud Rate Settings
 #ifndef CAN2_CLOCK_DIVISOR
  #define CAN2_CLOCK_DIVISOR     1   //1 or 4
 #endif

 #if (CAN2_CLOCK_DIVISOR != 1) && (CAN2_CLOCK_DIVISOR != 4)
  #error The CAN2 Clock Divisor can only be 1 or 4
 #endif
 
 #define CAN2_CLOCK_SPEED        (getenv("CLOCK") / CAN2_CLOCK_DIVISOR)

 #ifndef CAN2_BAUD_RATE
  #define CAN2_BAUD_RATE         125000
 #endif

 #if (CAN2_CLOCK_SPEED % CAN2_BAUD_RATE) != 0
  #error CAN2 Clock Speed not evenly divisible by CAN2 Baud Rate
 #endif

 #ifndef CAN2_DEFAULT_SAMPLE_POINT
  #define CAN2_DEFAULT_SAMPLE_POINT    875      //in tenth of a percent, e.g. 875 = 87.5% of bit time
 #endif

 #if CAN2_DEFAULT_SAMPLE_POINT < 500
  #undef CAN2_DEFAULT_SAMPLE_POINT
  #define CAN2_DEFAULT_SAMPLE_POINT    500
  #warning CAN2 Sample point changed to 50% of bit time
 #endif

 // Other CAN2 Peripheral Settings
 #ifndef CAN2_BRG_SAM
  #define CAN2_BRG_SAM                 0        //sample of the can bus line (def: bus line is sampled 1 times prior to sample point)
 #endif

 #ifndef CAN2_BRG_WAKE_FILTER
  #define CAN2_BRG_WAKE_FILTER         FALSE    //selects can bus line filter for wake up bit
 #endif

 #ifndef CAN2_USE_RX_DOUBLE_BUFFER
  #define CAN2_USE_RX_DOUBLE_BUFFER    TRUE     //TRUE-if buffer 0 overflows massage will be loaded into buffer 1, FALSE-if buffer 0 overflows message will not be loaded into buffer 1
 #endif

 #ifndef CAN2_ENABLE_CAN_CAPTURE
  #define CAN2_ENABLE_CAN_CAPTURE      FALSE
 #endif

 #ifndef CAN2_CONTINUE_IN_IDLE
  #define CAN2_CONTINUE_IN_IDLE        FALSE
 #endif
#endif

///////////////////////////// CAN Filter Settings /////////////////////////////

/*
   If no CAN RX Filters are going to be used then nothing needs to be done, by
   default this driver will initialize the CAN controller to use no filters.
   
   If CAN RX Filters are going to be used, the can_init() function can be used
   to initialize them by making the following defines before this driver is
   included:
   
   #define CAN_USE_FILTERS                TRUE
   
   #define CAN_USER_FILT_x                aa    // x being the filter to enable (0-5), and aa being the Id to filter for.
                                                //    Filters 0 and 1 are for RX Buffer 0, and filters 2 to 5 are for RX
                                                //    Buffer 1
   #define CAN_USER_FILT_x_TYPE           bb    // x being the filter to setup (0-5), and bb can be one of the following:
                                                //    CAN_FILTER_TYPE_SID - filter Id is a SID Id, only the SID bit of filter are written the EID bits are set to 0.
                                                //    CAN_FILTER_TYPE_EID - filter Id is an EID Id, the SID and EID bits of filter are written.
   
   #define CAN_USER_MASK_x                cc    // x being the mask to setup (0-1), and cc being the ID mask to set.
                                                //    Mask 0 is for RX Buffer 0, and mask 1 is for RX Buffer 1
   #define CAN_USER_MASK_x_ID_TYPE        dd    // x being the mask to setup (0-1), and cc being one of the following:
                                                //       CAN_MASK_TYPE_SID - only write the SID bits of mask, EID bits are set to 0
                                                //       CAN_MASK_TYPE_EID - write both the SID and EID bit of mask
   #define CAN_USER_MASK_y_FILTER_TYPE    gg    // y being the mask to setup (0-1), and gg being the filter mask type:
                                                //    CAN_FILTER_MASK_TYPE_EITHER - match either SID or EID messages, ignores EXIDE bit of filter.
                                                //    CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, EXIDE bit of filter determines type of messages received
*/

#ifndef CAN_USE_FILTERS
 #define CAN_USE_FILTERS               FALSE
#endif

#if CAN_USE_FILTERS == TRUE
 #ifndef CAN_USER_FILT_0
  #ifdef CAN_USER_FILT_1
   #define CAN_USER_FILT_0             CAN_USER_FILT_1
   
   #ifndef CAN_USER_FILT_0_TYPE
    #ifdef CAN_USER_FILT_1_TYPE
     #define CAN_USER_FILT_0_TYPE      CAN_USER_FILT_1_TYPE
    #endif
   #endif
  #else
   #define CAN_USER_FILT_0             0
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_1
  #define CAN_USER_FILT_1              CAN_USER_FILT_0
 #endif
 
 #ifndef CAN_USER_FILT_1_TYPE
  #define CAN_USER_FILT_1_TYPE         CAN_USER_FILT_0_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_2
  #ifdef CAN_USER_FILT_3
   #define CAN_USER_FILT_2             CAN_USER_FILT_3
   
   #ifndef CAN_USER_FILT_2_TYPE
    #ifdef CAN_USER_FILT_3_TYPE
     #define CAN_USER_FILT_2_TYPE      CAN_USER_FILT_3_TYPE
    #endif
   #endif
  #elif defined(CAN_USER_FILT_4)
   #define CAN_USER_FILT_2             CAN_USER_FILT_4
   
   #ifndef CAN_USER_FILT_2_TYPE
    #ifdef CAN_USER_FILT_4_TYPE
     #define CAN_USER_FILT_2_TYPE      CAN_USER_FILT_4_TYPE
    #endif
   #endif
  #elif defined(CAN_USER_FILT_5)
   #define CAN_USER_FILT_2             CAN_USER_FILT_5
   
   #ifndef CAN_USER_FILT_2_TYPE
    #ifdef CAN_USER_FILT_5_TYPE
     #define CAN_USER_FILT_2_TYPE      CAN_USER_FILT_5_TYPE
    #endif
   #endif
  #else
   #define CAN_USER_FILT_2             CAN_USER_FILT_0
   
   #ifndef CAN_USER_FILT_2_TYPE
    #define CAN_USER_FILT_2_TYPE       CAN_USER_FILT_0_TYPE
   #endif
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_2_TYPE
  #define CAN_USER_FILT_2_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_3
  #define CAN_USER_FILT_3              CAN_USER_FILT_2
  
  #ifndef CAN_USER_FILT_3_TYPE
   #define CAN_USER_FILT_3_TYPE        CAN_USER_FILT_2_TYPE
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_3_TYPE
  #define CAN_USER_FILT_3_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_4
  #define CAN_USER_FILT_4              CAN_USER_FILT_2
  
  #ifndef CAN_USER_FILT_4_TYPE
   #define CAN_USER_FILT_4_TYPE        CAN_USER_FILT_2_TYPE
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_4_TYPE
  #define CAN_USER_FILT_4_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_5
  #define CAN_USER_FILT_5              CAN_USER_FILT_2
  
  #ifndef CAN_USER_FILT_5_TYPE
   #define CAN_USER_FILT_5_TYPE        CAN_USER_FILT_2_TYPE
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_5_TYPE
  #define CAN_USER_FILT_5_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_MASK_0
  #define CAN_USER_MASK_0              0
 #endif
 
 #ifndef CAN_USER_MASK_0_ID_TYPE
  #define CAN_USER_MASK_0_ID_TYPE      CAN_MASK_ID_TYPE_EID
 #endif
 
 #ifndef CAN_USER_MASK_0_FILTER_TYPE
  #define CAN_USER_MASK_0_FILTER_TYPE  CAN_FILTER_MASK_TYPE_EITHER
 #endif
 
 #ifndef CAN_USER_MASK_1
  #define CAN_USER_MASK_1              CAN_USER_MASK_0 
 #endif
 
 #ifndef CAN_USER_MASK_1_ID_TYPE
  #define CAN_USER_MASK_1_ID_TYPE      CAN_USER_MASK_0_ID_TYPE
 #endif
 
 #ifndef CAN_USER_MASK_1_FILTER_TYPE
  #define CAN_USER_MASK_1_FILTER_TYPE  CAN_USER_MASK_0_FILTER_TYPE
 #endif
#else
 //Filter 0
 #if defined(CAN_USER_FILT_0) && (CAN_USER_FILT_0 != 0)
  #undef CAN_USER_FILT_0
 #endif
 
 #ifndef CAN_USER_FILT_0
  #define CAN_USER_FILT_0              0
 #endif
 
 #if defined(CAN_USER_FILT_0_TYPE) && (CAN_USER_FILT_0_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_0_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 //Filter 1
 #if defined(CAN_USER_FILT_1) && (CAN_USER_FILT_1 != 0)
  #undef CAN_USER_FILT_1
 #endif
 
 #ifndef CAN_USER_FILT_1
  #define CAN_USER_FILT_1              0
 #endif
 
 #if defined(CAN_USER_FILT_1_TYPE) && (CAN_USER_FILT_1_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_1_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_1_TYPE
  #define CAN_USER_FILT_1_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 //Filter 2
 #if defined(CAN_USER_FILT_2) && (CAN_USER_FILT_2 != 0)
  #undef CAN_USER_FILT_2
 #endif
 
 #ifndef CAN_USER_FILT_2
  #define CAN_USER_FILT_2              0
 #endif
 
 #if defined(CAN_USER_FILT_2_TYPE) && (CAN_USER_FILT_2_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_2_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_2_TYPE
  #define CAN_USER_FILT_2_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 //Filter 3
 #if defined(CAN_USER_FILT_3) && (CAN_USER_FILT_3 != 0)
  #undef CAN_USER_FILT_3
 #endif
 
 #ifndef CAN_USER_FILT_3
  #define CAN_USER_FILT_3              0
 #endif
 
 #if defined(CAN_USER_FILT_3_TYPE) && (CAN_USER_FILT_3_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_3_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_3_TYPE
  #define CAN_USER_FILT_3_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 //Filter 4
 #if defined(CAN_USER_FILT_4) && (CAN_USER_FILT_4 != 0)
  #undef CAN_USER_FILT_4
 #endif
 
 #ifndef CAN_USER_FILT_4
  #define CAN_USER_FILT_4              0
 #endif
 
 #if defined(CAN_USER_FILT_4_TYPE) && (CAN_USER_FILT_4_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_4_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_4_TYPE
  #define CAN_USER_FILT_4_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 //Filter 5
 #if defined(CAN_USER_FILT_5) && (CAN_USER_FILT_5 != 0)
  #undef CAN_USER_FILT_5
 #endif
 
 #ifndef CAN_USER_FILT_5
  #define CAN_USER_FILT_5              0
 #endif
 
 #if defined(CAN_USER_FILT_5_TYPE) && (CAN_USER_FILT_5_TYPE != CAN_FILTER_TYPE_EID)
  #undef CAN_USER_FILT_5_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_5_TYPE
  #define CAN_USER_FILT_5_TYPE         CAN_FILTER_TYPE_EID
 #endif
 
 //Mask 0
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
 
 //Mask 1
 #if defined(CAN_USER_MASK_1) && (CAN_USER_MASK_1 != 0)
  #undef CAN_USER_MASK_1
 #endif
 
 #ifndef CAN_USER_MASK_1
  #define CAN_USER_MASK_1              0
 #endif
 
 #if defined(CAN_USER_MASK_1_ID_TYPE) && (CAN_USER_MASK_1_ID_TYPE != CAN_MASK_ID_TYPE_EID)
  #undef CAN_USER_MASK_1_ID_TYPE
 #endif
 
 #ifndef CAN_USER_MASK_1_ID_TYPE
  #define CAN_USER_MASK_1_ID_TYPE      CAN_MASK_ID_TYPE_EID
 #endif
 
 #if defined(CAN_USER_MASK_1_FILTER_TYPE) && (CAN_USER_MASK_1_FILTER_TYPE != CAN_FILTER_MASK_TYPE_EITHER)
  #undef CAN_USER_MASK_1_FILTER_TYPE
 #endif
 
 #ifndef CAN_USER_MASK_1_FILTER_TYPE
  #define CAN_USER_MASK_1_FILTER_TYPE  CAN_FILTER_MASK_TYPE_EITHER
 #endif
#endif

#if (USE_CAN2_PERIPHERAL == TRUE)
 /*
   If no CAN2 RX Filters are going to be used then nothing needs to be done, by
   default this driver will initialize the CAN2 controller to use no filters.
   
   If CAN2 RX Filters are going to be used, the can2_init() function can be
   used to initialize them by making the following defines before this driver
   is included:
   
   #define CAN2_USE_FILTERS               TRUE
   
   #define CAN2_USER_FILT_x               aa    // x being the filter to enable (0-5), and aa being the Id to filter for.
                                                //    Filters 0 and 1 are for RX Buffer 0, and filters 2 to 5 are for RX
                                                //    Buffer 1
   #define CAN2_USER_FILT_x_TYPE          bb    // x being the filter to setup (0-5), and bb can be one of the following:
                                                //       CAN_FILTER_TYPE_SID - filter Id is a SID Id, only the SID bit of filter are written the EID bits are set to 0
                                                //       CAN_FILTER_TYPE_EID - filter Id is an EID Id, the SID and EID bits of filter are written
   
   #define CAN2_USER_MASK_x               cc    // x being the mask to setup (0-1), and cc being the ID mask to set.
                                                //    Mask 0 is for RX Buffer 0, and mask 1 is for RX Buffer 1
   #define CAN2_USER_MASK_x_ID_TYPE       dd    // x being the mask to setup (0-1), and cc being one of the following:
                                                //       CAN_MASK_ID_TYPE_SID - only write the SID bits of mask, EID bits are set to 0
                                                //       CAN_MASK_ID_TYPE_EID - write both the SID and EID bit of mask
   #define CAN2_USER_MASK_y_FILTER_TYPE   gg    // y being the mask to setup (0-1), and gg being the filter mask type:
                                                //    CAN_FILTER_MASK_TYPE_EITHER - match either SID or EID messages, ignores EXIDE bit of filter.
                                                //    CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, EXIDE bit of filter determines type of messages received
 */

 #ifndef CAN2_USE_FILTERS
  #define CAN2_USE_FILTERS             FALSE
 #endif

 #if CAN2_USE_FILTERS == TRUE
  #ifndef CAN2_USER_FILT_0
   #ifdef CAN2_USER_FILT_1
    #define CAN2_USER_FILT_0           CAN2_USER_FILT_1
   
    #ifndef CAN2_USER_FILT_0_TYPE
     #ifdef CAN2_USER_FILT_1_TYPE
      #define CAN2_USER_FILT_0_TYPE    CAN2_USER_FILT_1_TYPE
     #endif
    #endif
   #else
    #define CAN2_USER_FILT_0           0
   #endif
  #endif
 
  #ifndef CAN2_USER_FILT_0_TYPE
   #define CAN2_USER_FILT_0_TYPE       CAN_FILTER_TYPE_EID
  #endif
 
  #ifndef CAN2_USER_FILT_1
   #define CAN2_USER_FILT_1            CAN2_USER_FILT_0
  #endif
 
  #ifndef CAN2_USER_FILT_1_TYPE
   #define CAN2_USER_FILT_1_TYPE       CAN2_USER_FILT_0_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_2
   #ifdef CAN2_USER_FILT_3
    #define CAN2_USER_FILT_2           CAN2_USER_FILT_3
   
    #ifndef CAN2_USER_FILT_2_TYPE
     #ifdef CAN2_USER_FILT_3_TYPE
      #define CAN2_USER_FILT_2_TYPE    CAN2_USER_FILT_3_TYPE
     #endif
    #endif
   #elif defined(CAN2_USER_FILT_4)
    #define CAN2_USER_FILT_2           CAN2_USER_FILT_4
   
    #ifndef CAN2_USER_FILT_2_TYPE
     #ifdef CAN2_USER_FILT_4_TYPE
      #define CAN2_USER_FILT_2_TYPE    CAN2_USER_FILT_4_TYPE
     #endif
    #endif
   #elif defined(CAN2_USER_FILT_5)
    #define CAN2_USER_FILT_2           CAN2_USER_FILT_5
   
    #ifndef CAN2_USER_FILT_2_TYPE
     #ifdef CAN2_USER_FILT_5_TYPE
      #define CAN2_USER_FILT_2_TYPE    CAN2_USER_FILT_5_TYPE
     #endif
    #endif
   #else
    #define CAN2_USER_FILT_2           CAN2_USER_FILT_0
   
    #ifndef CAN2_USER_FILT_2_TYPE
     #define CAN2_USER_FILT_2_TYPE     CAN2_USER_FILT_0_TYPE
    #endif
   #endif
  #endif
 
  #ifndef CAN2_USER_FILT_2_TYPE
   #define CAN2_USER_FILT_2_TYPE       CAN_FILTER_TYPE_EID
  #endif
 
  #ifndef CAN2_USER_FILT_3
   #define CAN2_USER_FILT_3            CAN2_USER_FILT_2
  
   #ifndef CAN2_USER_FILT_3_TYPE
    #define CAN2_USER_FILT_3_TYPE      CAN2_USER_FILT_2_TYPE
   #endif
  #endif
 
  #ifndef CAN2_USER_FILT_3_TYPE
   #define CAN2_USER_FILT_3_TYPE       CAN_FILTER_TYPE_EID
  #endif
 
  #ifndef CAN2_USER_FILT_4
   #define CAN2_USER_FILT_4            CAN2_USER_FILT_2
  
   #ifndef CAN2_USER_FILT_4_TYPE
    #define CAN2_USER_FILT_4_TYPE      CAN2_USER_FILT_2_TYPE
   #endif
  #endif
 
  #ifndef CAN2_USER_FILT_4_TYPE
   #define CAN2_USER_FILT_4_TYPE       CAN_FILTER_TYPE_EID
  #endif
 
  #ifndef CAN2_USER_FILT_5
   #define CAN2_USER_FILT_5            CAN2_USER_FILT_2
  
   #ifndef CAN2_USER_FILT_5_TYPE
    #define CAN2_USER_FILT_5_TYPE         CAN2_USER_FILT_2_TYPE
   #endif
  #endif
 
  #ifndef CAN2_USER_FILT_5_TYPE
   #define CAN2_USER_FILT_5_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  #ifndef CAN2_USER_MASK_0
   #define CAN2_USER_MASK_0               0
  #endif
 
  #ifndef CAN2_USER_MASK_0_ID_TYPE
   #define CAN2_USER_MASK_0_ID_TYPE       CAN_MASK_ID_TYPE_EID
  #endif
 
  #ifndef CAN2_USER_MASK_0_FILTER_TYPE
   #define CAN2_USER_MASK_0_FILTER_TYPE   CAN_FILTER_MASK_TYPE_EITHER
  #endif
 
  #ifndef CAN2_USER_MASK_1
   #define CAN2_USER_MASK_1               CAN2_USER_MASK_0 
  #endif
  
  #ifndef CAN2_USER_MASK_1_ID_TYPE
   #define CAN2_USER_MASK_1_ID_TYPE       CAN2_USER_MASK_0_ID_TYPE
  #endif
  
  #ifndef CAN2_USER_MASK_1_FILTER_TYPE
   #define CAN2_USER_MASK_1_FILTER_TYPE   CAN2_USER_MASK_0_FILTER_TYPE
  #endif
 #else
  //Filter 0
  #if defined(CAN2_USER_FILT_0) && (CAN2_USER_FILT_0 != 0)
   #undef CAN2_USER_FILT_0
  #endif
 
  #ifndef CAN2_USER_FILT_0
   #define CAN2_USER_FILT_0               0
  #endif
 
  #if defined(CAN2_USER_FILT_0_TYPE) && (CAN2_USER_FILT_0_TYPE != CAN_FILTER_TYPE_EID)
   #undef CAN2_USER_FILT_0_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_0_TYPE
   #define CAN2_USER_FILT_0_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  //Filter 1
  #if defined(CAN2_USER_FILT_1) && (CAN2_USER_FILT_1 != 0)
   #undef CAN2_USER_FILT_1
  #endif
 
  #ifndef CAN2_USER_FILT_1
   #define CAN2_USER_FILT_1               0
  #endif
 
  #if defined(CAN2_USER_FILT_1_TYPE) && (CAN2_USER_FILT_1_TYPE != CAN_FILTER_TYPE_EID)
   #undef CAN2_USER_FILT_1_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_1_TYPE
   #define CAN2_USER_FILT_1_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  //Filter 2
  #if defined(CAN2_USER_FILT_2) && (CAN2_USER_FILT_2 != 0)
   #undef CAN2_USER_FILT_2
  #endif
 
  #ifndef CAN2_USER_FILT_2
   #define CAN2_USER_FILT_2               0
  #endif
 
  #if defined(CAN2_USER_FILT_2_TYPE) && (CAN2_USER_FILT_2_TYPE != CAN_FILTER_TYPE_EID)
   #undef CAN2_USER_FILT_2_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_2_TYPE
   #define CAN2_USER_FILT_2_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  //Filter 3
  #if defined(CAN2_USER_FILT_3) && (CAN2_USER_FILT_3 != 0)
   #undef CAN2_USER_FILT_3
  #endif
 
  #ifndef CAN2_USER_FILT_3
   #define CAN2_USER_FILT_3               0
  #endif
 
  #if defined(CAN2_USER_FILT_3_TYPE) && (CAN2_USER_FILT_3_TYPE != CAN_FILTER_TYPE_EID)
   #undef CAN2_USER_FILT_3_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_3_TYPE
   #define CAN2_USER_FILT_3_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  //Filter 4
  #if defined(CAN2_USER_FILT_4) && (CAN2_USER_FILT_4 != 0)
   #undef CAN2_USER_FILT_4
  #endif
 
  #ifndef CAN2_USER_FILT_4
   #define CAN2_USER_FILT_4               0
  #endif
 
  #if defined(CAN2_USER_FILT_4_TYPE) && (CAN2_USER_FILT_4_TYPE != CAN_FILTER_TYPE_EID)
   #undef CAN2_USER_FILT_4_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_4_TYPE
   #define CAN2_USER_FILT_4_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  //Filter 5
  #if defined(CAN2_USER_FILT_5) && (CAN2_USER_FILT_5 != 0)
   #undef CAN2_USER_FILT_5
  #endif
 
  #ifndef CAN2_USER_FILT_5
   #define CAN2_USER_FILT_5               0
  #endif
 
  #if defined(CAN2_USER_FILT_5_TYPE) && (CAN2_USER_FILT_5_TYPE != CAN_FILTER_TYPE_EID)
   #undef CAN2_USER_FILT_5_TYPE
  #endif
 
  #ifndef CAN2_USER_FILT_5_TYPE
   #define CAN2_USER_FILT_5_TYPE          CAN_FILTER_TYPE_EID
  #endif
 
  //Mask 0
  #if defined(CAN2_USER_MASK_0) && (CAN2_USER_MASK_0 != 0)
   #undef CAN2_USER_MASK_0
  #endif
 
  #ifndef CAN2_USER_MASK_0
   #define CAN2_USER_MASK_0               0
  #endif
 
  #if defined(CAN2_USER_MASK_0_ID_TYPE) && (CAN2_USER_MASK_0_ID_TYPE != CAN_MASK_ID_TYPE_EID)
   #undef CAN2_USER_MASK_0_ID_TYPE
  #endif
 
  #ifndef CAN2_USER_MASK_0_ID_TYPE
   #define CAN2_USER_MASK_0_ID_TYPE       CAN_MASK_ID_TYPE_EID
  #endif
 
  #if defined(CAN2_USER_MASK_0_FILTER_TYPE) && (CAN2_USER_MASK_0_FILTER_TYPE != CAN_FILTER_MASK_TYPE_EITHER)
   #undef CAN2_USER_MASK_0_FILTER_TYPE
  #endif
 
  #ifndef CAN2_USER_MASK_0_FILTER_TYPE
   #define CAN2_USER_MASK_0_FILTER_TYPE   CAN_FILTER_MASK_TYPE_EITHER
  #endif
 
  //Mask 1
  #if defined(CAN2_USER_MASK_1) && (CAN2_USER_MASK_1 != 0)
   #undef CAN2_USER_MASK_1
  #endif
 
  #ifndef CAN2_USER_MASK_1
   #define CAN2_USER_MASK_1               0
  #endif
 
  #if defined(CAN2_USER_MASK_1_ID_TYPE) && (CAN2_USER_MASK_1_ID_TYPE != CAN_MASK_ID_TYPE_EID)
   #undef CAN2_USER_MASK_1_ID_TYPE
  #endif
 
  #ifndef CAN2_USER_MASK_1_ID_TYPE
   #define CAN2_USER_MASK_1_ID_TYPE       CAN_MASK_ID_TYPE_EID
  #endif
 
  #if defined(CAN2_USER_MASK_1_FILTER_TYPE) && (CAN2_USER_MASK_1_FILTER_TYPE != CAN_FILTER_MASK_TYPE_EITHER)
   #undef CAN2_USER_MASK_1_FILTER_TYPE
  #endif
 
  #ifndef CAN2_USER_MASK_1_FILTER_TYPE
   #define CAN2_USER_MASK_1_FILTER_TYPE   CAN_FILTER_MASK_TYPE_EITHER
  #endif
 #endif
#endif

////////////////////////////////// CAN Types //////////////////////////////////

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

// CAN Control & Status Register
typedef struct 
{
   uint16_t u0:1;
   CAN_INT_CODE icode:3;   //Interrupt Flag Code bits
   uint16_t u4:1;
   CAN_OP_MODE opmode:3;   //Operation Mode bits
   CAN_OP_MODE reqop:3;    //Request Operation Mode bits
   uint16_t cancks:1;      //CAN Master Clock Select bit
   uint16_t abat:1;        //Abort All Pending Transmissions bit
   uint16_t csidl:1;       //Stop in Idle Mode bit
   uint16_t u14:1;
   uint16_t cancap:1;      //CAN Message Receive Capture Enable bit
} CiCTRL;

CiCTRL CAN_C1CTRL;
#locate CAN_C1CTRL = getenv("SFR:C1CTRL")

#if getenv("SFR_VALID:C2CTRL") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCTRL CAN_C2CTRL;
 #locate CAN_C2CTRL = getenv("SFR:C2CTRL")
#endif

// Transmit Buffer Status and Control Register
typedef struct 
{
   uint16_t txpri:2;       //Message Transmission Priority bits
   uint16_t u2:1;
   uint16_t txreq:1;       //Message Send Request bit
   uint16_t txerr:1;       //Error Detected During Transmission bit
   uint16_t txlarb:1;      //Message Lost Arbitration bit
   uint16_t txabt:1;       //Message Aborted bit
   uint16_t u7_16:9;
} CiTXnCON;

CiTXnCON CAN_C1TX0CON;
CiTXnCON CAN_C1TX1CON;
CiTXnCON CAN_C1TX2CON;

#locate CAN_C1TX0CON = getenv("SFR:C1TX0CON")
#locate CAN_C1TX1CON = getenv("SFR:C1TX1CON")
#locate CAN_C1TX2CON = getenv("SFR:C1TX2CON")

#if getenv("SFR_VALID:C2TX0CON") && (USE_CAN2_PERIPHERAL == TRUE)
 CiTXnCON CAN_C2TX0CON;
 CiTXnCON CAN_C2TX1CON;
 CiTXnCON CAN_C2TX2CON;

 #locate CAN_C2TX0CON = getenv("SFR:C2TX0CON")
 #locate CAN_C2TX1CON = getenv("SFR:C2TX1CON")
 #locate CAN_C2TX2CON = getenv("SFR:C2TX2CON")
#endif

// Transmit Buffer Identifier Registers
typedef union
{
   struct
   {
      uint16_t exide:1;       //Extended Identifier Enable bit
      uint16_t srr:1;         //Substitute Remote Request bit
      uint16_t sid0_5:6;      //Standard Identifier bits 0-5
      uint16_t u8_10:3;
      uint16_t sid6_10:5;     //Standard Identifier bits 6-10
      uint16_t eid6_13:8;     //Extended Identifier bits 6-13
      uint16_t u24_27:4;
      uint16_t eid14_17:4;    //Extended Identifier bits 14-17
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} CiTXnID;

CiTXnID CAN_C1TX0ID;
CiTXnID CAN_C1TX1ID;
CiTXnID CAN_C1TX2ID;

#locate CAN_C1TX0ID = getenv("SFR:C1TX0SID")
#locate CAN_C1TX1ID = getenv("SFR:C1TX1SID")
#locate CAN_C1TX2ID = getenv("SFR:C1TX2SID")

#if getenv("SFR_VALID:C2TX0SID") && (USE_CAN2_PERIPHERAL == TRUE)
 CiTXnID CAN_C2TX0ID;
 CiTXnID CAN_C2TX1ID;
 CiTXnID CAN_C2TX2ID;

 #locate CAN_C2TX0ID = getenv("SFR:C2TX0SID")
 #locate CAN_C2TX1ID = getenv("SFR:C2TX1SID")
 #locate CAN_C2TX2ID = getenv("SFR:C2TX2SID")
#endif

// Transmit Buffer Data length Control Registers
typedef struct
{
   uint16_t u0_2:3;
   uint16_t dlc:4;      //Data Length Code bits
   uint16_t txrb0:1;    //Reserved bit 0, must be set to zero
   uint16_t txrb1:1;    //Reserved bit 1, must be set to zero
   uint16_t txrtr:1;    //Remote Transmission Request bit
   uint16_t eid0_5:6;   //Extended Identifier bits 0-5
} CiTXnDLC;

CiTXnDLC CAN_C1TX0DLC;
CiTXnDLC CAN_C1TX1DLC;
CiTXnDLC CAN_C1TX2DLC;

#locate CAN_C1TX0DLC = getenv("SFR:C1TX0DLC")
#locate CAN_C1TX1DLC = getenv("SFR:C1TX1DLC")
#locate CAN_C1TX2DLC = getenv("SFR:C1TX2DLC")

#if getenv("SFR_VALID:C2TX0DLC") && (USE_CAN2_PERIPHERAL == TRUE)
 CiTXnDLC CAN_C2TX0DLC;
 CiTXnDLC CAN_C2TX1DLC;
 CiTXnDLC CAN_C2TX2DLC;

 #locate CAN_C2TX0DLC = getenv("SFR:C2TX0DLC")
 #locate CAN_C2TX1DLC = getenv("SFR:C2TX1DLC")
 #locate CAN_C2TX2DLC = getenv("SFR:C2TX2DLC")
#endif

// Transmit Buffer Data Field Registers
uint8_t CAN_C1TX0DATA[8];
uint8_t CAN_C1TX1DATA[8];
uint8_t CAN_C1TX2DATA[8];

#locate CAN_C1TX0DATA = getenv("SFR:C1TX0B1")
#locate CAN_C1TX1DATA = getenv("SFR:C1TX1B1")
#locate CAN_C1TX2DATA = getenv("SFR:C1TX2B1")

#if getenv("SFR_VALID:C2TX0B1") && (USE_CAN2_PERIPHERAL == TRUE)
 uint8_t CAN_C2TX0DATA[8];
 uint8_t CAN_C2TX1DATA[8];
 uint8_t CAN_C2TX2DATA[8];

 #locate CAN_C2TX0DATA = getenv("SFR:C2TX0B1")
 #locate CAN_C2TX1DATA = getenv("SFR:C2TX1B1")
 #locate CAN_C2TX2DATA = getenv("SFR:C2TX2B1")
#endif

// Receive Buffer 0 Status & Control Register
typedef struct
{
   uint16_t filhit0:1;  //Indicates Which Acceptance Filter Enabled the Message Reception bit
   uint16_t jtoff:1;    //Jump Table Offset bit (read only copy of DBEN)
   uint16_t dben:1;     //Receive Buffer 0 Double Buffer Enable bit
   uint16_t rxrtrro:1;  //Received Remote Transfer Request bit (read only)
   uint16_t u4_6:3;
   uint16_t rxful:1;    //Receive Full Status bit
   uint16_t u8_15:8;
} CiRX0CON;

CiRX0CON CAN_C1RX0CON;
#locate CAN_C1RX0CON = getenv("SFR:C1RX0CON")

#if getenv("SFR_VALID:C2RX0CON") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRX0CON CAN_C2RX0CON;
 #locate CAN_C2RX0CON = getenv("SFR:C2RX0CON")
#endif

// Receive Buffer 1 Status & Control Register
typedef struct 
{
   CAN_FILTER filhit:3;    //Indicates Which Acceptance Filter Enabled the Message Reception bits
   uint16_t rxrtrro:1;     //Received Remote Transfer Request bit (read only)
   uint16_t u4_6:3;
   uint16_t rxful:1;       //Receive Full Status bit
   uint16_t u8_15:8;
} CiRX1CON;

CiRX1CON CAN_C1RX1CON;
#locate CAN_C1RX1CON = getenv("SFR:C1RX1CON")

#if getenv("SFR_VALID:C2RX1CON") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRX1CON CAN_C2RX1CON;
 #locate CAN_C2RX1CON = getenv("SFR:C2RX1CON")
#endif

// Receive Buffer Identifier Registers
typedef union
{
   struct
   {
      uint16_t exide:1;       //Extended Identifier Enable bit
      uint16_t srr:1;         //Substitute Remote Request bit
      uint16_t sid0_10:11;    //Standard Identifier bits 0-10
      uint16_t u13_15:3;
      uint16_t eid6_17:12;    //Extended Identifier bits 6-17
      uint16_t u28_31:4;
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} CiRXnID;

CiRXnID CAN_C1RX0ID;
CiRXnID CAN_C1RX1ID;

#locate CAN_C1RX0ID = getenv("SFR:C1RX0SID")
#locate CAN_C1RX1ID = getenv("SFR:C1RX1SID")

#if getenv("SFR_VALID:C2RX0SID") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXnID CAN_C2RX0ID;
 CiRXnID CAN_C2RX1ID;
 
 #locate CAN_C2RX0ID = getenv("SFR:C2RX0SID")
 #locate CAN_C2RX1ID = getenv("SFR:C2RX1SID")
#endif

// Receive Buffer Data Length Control Registers
typedef struct
{
   uint16_t dlc:4;      //Data Length Code bits
   uint16_t rb0:1;      //Reserved bit 0
   uint16_t u5_7:3;
   uint16_t rb1:1;      //Reserved bit 0
   uint16_t rxrtr:1;    //Received Remote Transmission Request Control bit
   uint16_t eid0_5:6;   //Extended Identifier bits 0-5
} CiRXnDLC;

CiRXnDLC CAN_C1RX0DLC;
CiRXnDLC CAN_C1RX1DLC;

#locate CAN_C1RX0DLC = getenv("SFR:C1RX0DLC")
#locate CAN_C1RX1DLC = getenv("SFR:C1RX1DLC")

#if getenv("SFR_VALID:C2RX0DLC") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXnDLC CAN_C2RX0DLC;
 CiRXnDLC CAN_C2RX1DLC;
 
 #locate CAN_C2RX0DLC = getenv("SFR:C2RX0DLC")
 #locate CAN_C2RX1DLC = getenv("SFR:C3RX1DLC")
#endif

// Receive Buffer Data Field Registers
uint8_t CAN_C1RX0DATA[8];
uint8_t CAN_C1RX1DATA[8];

#locate CAN_C1RX0DATA = getenv("SFR:C1RX0B1")
#locate CAN_C1RX1DATA = getenv("SFR:C1RX1B1")

#if getenv("SFR_VALID:C2RX0B1") && (USE_CAN2_PERIPHERAL == TRUE)
 uint8_t CAN_C2RX0DATA[8];
 uint8_t CAN_C2RX1DATA[8];
 
 #locate CAN_C2RX0DATA = getenv("SFR:C2RX0B1")
 #locate CAN_C2RX1DATA = getenv("SFR:C2RX1B1")
#endif

// Acceptance Filter Identifier Registers
typedef union
{
   struct
   {
      uint16_t exide:1;       //Extended Identifier Enable bit
      uint16_t u1:1;
      uint16_t sid0_10:11;    //Standard Identifier bits 0-10
      uint16_t u13_15:3;
      uint16_t eid6_17:12;    //Extended Identifier bits 6-17
      uint16_t u28_31:4;
      uint16_t u32_41:10;
      uint16_t eid0_5:6;      //Extended Identifier bits 0-5
   };
   uint8_t b[6];
   uint16_t w[3];
} CiRXFnID;

CiRXFnID CAN_C1RXF0ID;
CiRXFnID CAN_C1RXF1ID;
CiRXFnID CAN_C1RXF2ID;
CiRXFnID CAN_C1RXF3ID;
CiRXFnID CAN_C1RXF4ID;
CiRXFnID CAN_C1RXF5ID;

#locate CAN_C1RXF0ID = getenv("SFR:C1RXF0SID")
#locate CAN_C1RXF1ID = getenv("SFR:C1RXF1SID")
#locate CAN_C1RXF2ID = getenv("SFR:C1RXF2SID")
#locate CAN_C1RXF3ID = getenv("SFR:C1RXF3SID")
#locate CAN_C1RXF4ID = getenv("SFR:C1RXF4SID")
#locate CAN_C1RXF5ID = getenv("SFR:C1RXF5SID")

#if getenv("SFR_VALID:C2RXF0SID") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXFnID CAN_C2RXF0ID;
 CiRXFnID CAN_C2RXF1ID;
 CiRXFnID CAN_C2RXF2ID;
 CiRXFnID CAN_C2RXF3ID;
 CiRXFnID CAN_C2RXF4ID;
 CiRXFnID CAN_C2RXF5ID;
 
 #locate CAN_C2RXF0ID = getenv("SFR:C2RXF0SID")
 #locate CAN_C2RXF1ID = getenv("SFR:C2RXF1SID")
 #locate CAN_C2RXF2ID = getenv("SFR:C2RXF2SID")
 #locate CAN_C2RXF3ID = getenv("SFR:C2RXF3SID")
 #locate CAN_C2RXF4ID = getenv("SFR:C2RXF4SID")
 #locate CAN_C2RXF5ID = getenv("SFR:C2RXF5SID")
#endif

// Acceptance Filter Mask Identifier Registers
typedef union
{
   struct
   {
      uint16_t mide:1;        //Identifier Mode Selection bit
      uint16_t u1:1;
      uint16_t sid0_10:11;    //Standard Identifier bits 0-10
      uint16_t u13_15:3;
      uint16_t eid6_17:12;    //Extended Identifier bits 6-17
      uint16_t u28_31:4;
      uint16_t u32_41:10;
      uint16_t eid0_5:6;      //Extended Identifier bits 0-5
   };
   uint8_t b[6];
   uint16_t w[3];
} CiRXMnID;

CiRXMnID CAN_C1RXM0ID;
CiRXMnID CAN_C1RXM1ID;

#locate CAN_C1RXM0ID = getenv("SFR:C1RXM0SID")
#locate CAN_C1RXM1ID = getenv("SFR:C1RXM1SID")

#if getenv("SFR_VALID:C2RXM0SID") && (USE_CAN2_PERIPHERAL == TRUE)
 CiRXMnID CAN_C2RXM0ID;
 CiRXMnID CAN_C2RXM1ID;
 
 #locate CAN_C2RXM0ID = getenv("SFR:C2RXM0SID")
 #locate CAN_C2RXM1ID = getenv("SFR:C2RXM1SID")
#endif

// Baud Rate Configuration Register 1
typedef struct
{
   uint16_t brp:6;      //Baud Rate Prescaler bits
   uint16_t sjw:2;      //Synchronized Jump Width bits
   uint16_t u8_16:8;
} CiCFG1;

CiCFG1 CAN_C1CFG1;
#locate CAN_C1CFG1 = getenv("SFR:C1CFG1")

#if getenv("SFR_VALID:C2CFG1") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCFG1 CAN_C2CFG1;
 #locate CAN_C2CFG1 = getenv("SFR:C2CFG1")
#endif

// Baud Rate Configuration Register 2
typedef struct
{
   uint16_t prseg:3;       //Propagation Time Segment bits
   uint16_t seg1ph:3;      //Phase Buffer Segment 1 bits
   uint16_t sam:1;         //Sample of the CAN bus Line bit
   uint16_t seg2phts:1;    //Phase Segment 2 Time Select bit
   uint16_t seg2ph:3;      //Phase Buffer Segment 2 bits
   uint16_t u11_13:3;
   uint16_t wakfil:1;      //Select CAN bus Line Filter for Wake-up bit
   uint16_t u15;
} CiCFG2;

CiCFG2 CAN_C1CFG2;
#locate CAN_C1CFG2 = getenv("SFR:C1CFG2")

#if getenv("SFR_VALID:C2CFG2") && (USE_CAN2_PERIPHERAL == TRUE)
 CiCFG2 CAN_C1CFG2;
 #locate CAN_C1CFG2 = getenv("SFR:C1CFG2")
#endif

// Interrupt Enable Register
typedef struct
{
   uint16_t rx0ie:1;    //Receive Buffer 0 Interrupt Enable bit
   uint16_t rx1ie:1;    //Receive Buffer 1 Interrupt Enable bit
   uint16_t tx0ie:1;    //Transmit Buffer 0 Interrupt Enable bit
   uint16_t tx1ie:1;    //Transmit Buffer 1 Interrupt Enable bit
   uint16_t tx2ie:1;    //Transmit Buffer 2 Interrupt Enable bit
   uint16_t errie:1;    //Error Interrupt Enable bit
   uint16_t wakie:1;    //Bus Wake Up Activity Interrupt Enable bit
   uint16_t ivrie:1;    //Invalid Message Received Interrupt Enable bit
   uint16_t u8_15:8;
} CiINTE;

CiINTE CAN_C1INTE;
#locate CAN_C1INTE = getenv("SFR:C1INTE")

#if getenv("SFR_VALID:C2INTE") && (USE_CAN2_PERIPHERAL == TRUE)
 CiINTE CAN_C2INTE;
 #locate CAN_C2INTE = getenv("SFR:C2INTE")
#endif

// Interrupt Flag Register
typedef struct
{
   uint16_t rx0if:1;    //Receive Buffer 0 Interrupt Flag bit
   uint16_t rx1if:1;    //Receive Buffer 1 Interrupt Flag bit
   uint16_t tx0if:1;    //Transmit Buffer 0 Interrupt Flag bit
   uint16_t tx1if:1;    //Transmit Buffer 1 Interrupt Flag bit
   uint16_t tx2if:1;    //Transmit Buffer 2 Interrupt Flag bit
   uint16_t errif:1;    //Error Interrupt Flag bit
   uint16_t wakif:1;    //Bus Wake Up Activity Interrupt Flag bit
   uint16_t ivrif:1;    //Invalid Message Received Interrupt Flag bit
   uint16_t ewarn:1;    //Transmitter or Receiver is in Error State, Warning bit
   uint16_t rxwar:1;    //Receiver in Error State, Warning bit
   uint16_t txwar:1;    //Transmitter in Error State, Warning bit
   uint16_t rxep:1;     //Receiver in Error State, Bus Passive bit
   uint16_t txep:1;     //Transmitter in Error State, Bus Passive bit
   uint16_t txb0:1;     //Transmitter in Error State, Bus Off bit
   uint16_t rx1ovr:1;   //Receive Buffer 1 Overflowed bit
   uint16_t rx0ovr:1;   //Receive Buffer 0 Overflowed bit
} CiINTF;

CiINTF CAN_C1INTF;
#locate CAN_C1INTF = getenv("SFR:C1INTF")

#if getenv("SFR_VALID:C2INTF") && (USE_CAN2_PERIPHERAL == TRUE)
 CiINTF CAN_C2INTF;
 #locate CAN_C2INTF = getenv("SFR:C2INTF")
#endif

/////////////////////////////// Global Variables //////////////////////////////

CAN_OP_MODE g_CANOpmode = CAN_OP_CONFIG;

#if USE_CAN2_PERIPHERAL == TRUE
 CAN_OP_MODE g_CAN2Opmode = CAN_OP_CONFIG;
#endif

////////////////////////////////// Prototypes /////////////////////////////////

void can_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type);
void can_set_buffer_id(CAN_TX_HEADER *Header, uint8_t Buffer, uint8_t Peripheral=1);
void can_get_rx_data(CAN_RX_HEADER *Header, uint8_t *Data, uint8_t Peripheral=1);

////////////////////////////////// Functions //////////////////////////////////

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
   can_set_mode(CAN_OP_CONFIG);   //must be in config mode before params can be set
   
   CAN_C1CTRL.cancap = CAN_ENABLE_CAN_CAPTURE;
   CAN_C1CTRL.cancks = (CAN_CLOCK_DIVISOR == 4);
   CAN_C1CTRL.csidl = CAN_CONTINUE_IN_IDLE;
   
   can_set_baud(CAN_CLOCK_SPEED, CAN_BAUD_RATE);
   
   CAN_C1RX0CON.dben = CAN_USE_RX_DOUBLE_BUFFER;
   
   can_set_mask_id(CAN_FILTER_MASK_0, CAN_USER_MASK_0, CAN_USER_MASK_0_ID_TYPE, CAN_USER_MASK_0_FILTER_TYPE);
   can_set_filter_id(CAN_FILTER_0, CAN_USER_FILT_0, CAN_USER_FILT_0_TYPE);
   can_set_filter_id(CAN_FILTER_1, CAN_USER_FILT_1, CAN_USER_FILT_1_TYPE);
   
   can_set_mask_id(CAN_FILTER_MASK_1, CAN_USER_MASK_1, CAN_USER_MASK_1_ID_TYPE, CAN_USER_MASK_1_FILTER_TYPE);
   can_set_filter_id(CAN_FILTER_2, CAN_USER_FILT_2, CAN_USER_FILT_2_TYPE);
   can_set_filter_id(CAN_FILTER_3, CAN_USER_FILT_3, CAN_USER_FILT_3_TYPE);
   can_set_filter_id(CAN_FILTER_4, CAN_USER_FILT_4, CAN_USER_FILT_4_TYPE);
   can_set_filter_id(CAN_FILTER_5, CAN_USER_FILT_5, CAN_USER_FILT_5_TYPE);
   
   can_set_mode(Mode);
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_init(CAN_OP_MODE Mode=CAN_OP_NORMAL)
 {
   can2_set_mode(CAN_OP_CONFIG);   //must be in config mode before params can be set
   
   CAN_C2CTRL.cancap = CAN2_ENABLE_CAN_CAPTURE;
   CAN_C2CTRL.cancks = (CAN2_CLOCK_DIVISOR == 1);
   CAN_C2CTRL.csidl = CAN2_CONTINUE_IN_IDLE;
   
   can2_set_baud(CAN2_CLOCK_SPEED, CAN2_BAUD_RATE);
   
   CAN_C2RX0CON.dben = CAN2_USE_RX_DOUBLE_BUFFER;
   
   can2_set_mask_id(CAN2_FILTER_MASK_0, CAN2_USER_MASK_0, CAN2_USER_MASK_0_ID_TYPE, CAN2_USER_MASK_0_FILTER_TYPE);
   can2_set_filter_id(CAN2_FILTER_0, CAN2_USER_FILT_0, CAN2_USER_FILT_0_TYPE);
   can2_set_filter_id(CAN2_FILTER_1, CAN2_USER_FILT_1, CAN2_USER_FILT_1_TYPE);
   
   can2_set_mask_id(CAN2_FILTER_MASK_1, CAN2_USER_MASK_1, CAN2_USER_MASK_1_ID_TYPE, CAN2_USER_MASK_1_FILTER_TYPE);
   can2_set_filter_id(CAN2_FILTER_2, CAN2_USER_FILT_2, CAN2_USER_FILT_2_TYPE);
   can2_set_filter_id(CAN2_FILTER_3, CAN2_USER_FILT_3, CAN2_USER_FILT_3_TYPE);
   can2_set_filter_id(CAN2_FILTER_4, CAN2_USER_FILT_4, CAN2_USER_FILT_4_TYPE);
   can2_set_filter_id(CAN2_FILTER_5, CAN2_USER_FILT_5, CAN2_USER_FILT_5_TYPE);
   
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
//    Clock - the clock speed that is presented to the CAN peripheral.
// 
//    Rate - the bit rate, max speed is 1 Mbps and the Clock must be evenly
//           divisible by the Rate
//
// Return - can_ec_t error code, see can-dspic30f.h for a description of error
//          codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate)
{
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
   
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   CAN_C1CFG1.brp = BRP - 1;
   
   if(tSeg1Bits > 17)
   {
      CAN_C1CFG2.prseg = 7;
      CAN_C1CFG2.seg1ph = 7;
      
      tSeg1Bits = 17;
   }
   else
   {
      while((tSeg1Bits < (tBits - tSeg1Bits - 1)) || (tSeg1Bits < 2))
         tSeg1Bits++;
      
      CAN_C1CFG2.prseg = (tSeg1Bits / 2) - 1;
      CAN_C1CFG2.seg1ph = (tSeg1Bits - (tSeg1Bits / 2))  - 1;
   }
   
   CAN_C1CFG2.seg2ph = (tBits - tSeg1Bits) - 2;
   
   CAN_C1CFG2.sam = CAN_BRG_SAM;
   CAN_C1CFG2.seg2phts = TRUE;      //Phase Segment 2 Freely programmable
   CAN_C1CFG2.wakfil = CAN_BRG_WAKE_FILTER;
   
   if(CAN_C1CFG2.seg2ph == 0)
      CAN_C1CFG1.sjw = 0;
   else if(CAN_C1CFG2.seg2ph < 4)
      CAN_C1CFG1.sjw = CAN_C1CFG2.seg2ph - 1;
   else
      CAN_C1CFG1.sjw = 3;
   
   can_set_mode(g_CANOpmode, FALSE);
   
   return(CAN_EC_OK);
}

#if USE_CAN2_PERIPHERAL == TRUE
 can_ec_t can2_set_baud(uint32_t Clock, uint32_t Rate)
 {
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
   
   can2_set_mode(CAN_OP_CONFIG, FALSE);
   
   CAN_C2CFG1.brp = BRP - 1;
   
   if(tSeg1Bits > 17)
   {
      CAN_C2CFG2.prseg = 7;
      CAN_C2CFG2.seg1ph = 7;
      
      tSeg1Bits = 17;
   }
   else
   {
      while((tSeg1Bits < (tBits - tSeg1Bits - 1)) || (tSeg1Bits < 2))
         tSeg1Bits++;
      
      CAN_C2CFG2.prseg = (tSeg1Bits / 2) - 1;
      CAN_C2CFG2.seg1ph = (tSeg1Bits - (tSeg1Bits / 2))  - 1;
   }
   
   CAN_C2CFG2.seg2ph = (tBits - tSeg1Bits) - 2;
   
   CAN_C2CFG2.sam = CAN2_BRG_SAM;
   CAN_C2CFG2.seg2phts = TRUE;      //Phase Segment 2 Freely programmable
   CAN_C2CFG2.wakfil = CAN2_BRG_WAKE_FILTER;
   
   if(CAN_C2CFG2.seg2ph == 0)
      CAN_C2CFG1.sjw = 0;
   else if(CAN_C2CFG2.seg2ph < 4)
      CAN_C2CFG1.sjw = CAN_C2CFG2.seg2ph - 1;
   else
      CAN_C2CFG1.sjw = 3;
   
   can2_set_mode(g_CAN2Opmode, FALSE);
   
   return(CAN_EC_OK);
 }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_set_mode()
// can2_set_mode()
//
// Set the operational mode of the CAN peripheral.
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
   if(CAN_C1CTRL.opmode != Mode)
   {
      CAN_C1CTRL.reqop = Mode;
      
      while(CAN_C1CTRL.opmode != Mode);
   }
   
   if(Update)
      g_CANOpmode = Mode;
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE)
 {
   if(CAN_C2CTRL.opmode != Mode)
   {
      CAN_C2CTRL.reqop = Mode;
      
      while(CAN_C2CTRL.opmode != Mode);
   }
   
   if(Update)
      g_CAN2Opmode = Mode;
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
   
   switch(Filter)
   {
      case CAN_FILTER_0:
         Address = &CAN_C1RXF0ID;
         break;
      case CAN_FILTER_1:
         Address = &CAN_C1RXF1ID;
         break;
      case CAN_FILTER_2:
         Address = &CAN_C1RXF2ID;
         break;
      case CAN_FILTER_3:
         Address = &CAN_C1RXF3ID;
         break;
      case CAN_FILTER_4:
         Address = &CAN_C1RXF4ID;
         break;
      case CAN_FILTER_5:
         Address = &CAN_C1RXF5ID;
         break;
      default:
         return;
         break;
   }
   
   can_set_id(Address, Id, Type, Type);
}
         
#if USE_CAN2_PERIPHERAL == TRUE
 void can2_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type)
 {
   uint16_t *Address;
   
   switch(Filter)
   {
      case CAN_FILTER_0:
         Address = &CAN_C2RXF0ID;
         break;
      case CAN_FILTER_1:
         Address = &CAN_C2RXF1ID;
         break;
      case CAN_FILTER_2:
         Address = &CAN_C2RXF2ID;
         break;
      case CAN_FILTER_3:
         Address = &CAN_C2RXF3ID;
         break;
      case CAN_FILTER_4:
         Address = &CAN_C2RXF4ID;
         break;
      case CAN_FILTER_5:
         Address = &CAN_C2RXF5ID;
         break;
      default:
         return;
         break;
   }
   
   can_set_id(Address, Id, Type, Type);
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
         Address = &CAN_C1RXM0ID;
         break;
      case CAN_FILTER_MASK_1:
         Address = &CAN_C1RXM1ID;
         break;
      default:
         return;
         break;
   }
   
   can_set_id(Address, Id, iType, fType);
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType)
 {
   uint16_t *Address;
   
   switch(Mask)
   {
      case CAN_FILTER_MASK_0:
         Address = &CAN_C2RXM0ID;
         break;
      case CAN_FILTER_MASK_0:
         Address = &CAN_C2RXM1ID;
         break;
      default:
         return;
         break;
   }
   
   can_set_id(Address, Id, iType, fType);
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
//             CAN_TX_HEADER type in can-dspic30f.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
//    Buffer - optional specifies the buffer to load the message to transmit
//             into.  Defaults to CAN_TX_BUFFER_ANY, meaning messages will be 
//             loaded into first available TX buffer that is free and not setup
//             as an RTR TX buffer.
//
// Returns - can_ec_t error code, see can-dspic30f.h for a description of
//           error codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data)
{
   can_ec_t Result = CAN_EC_OK;
   uint8_t *dPtr;
   uint8_t Buffer;
   
   if(!CAN_C1TX0CON.txreq)
   {
      Buffer = 0;      
      dPtr = &CAN_C1TX0DATA;
   }
   else if(!CAN_C1TX1CON.txreq)
   {
      Buffer = 1;      
      dPtr = &CAN_C1TX1DATA;
   }
   else if(!CAN_C1TX2CON.txreq)
   {
      Buffer = 2;      
      dPtr = &CAN_C1TX2DATA;
   }
   else
      Result = CAN_EC_BUFFER_TX_FULL;
   
   if(Result == CAN_EC_OK)
   {
      can_set_buffer_id(Header, Buffer);
      memcpy(dPtr, Data, Header->Length);
      
      switch(Buffer)
      {
         case 0:
            CAN_C1TX0CON.txpri = Header->Priority;
            CAN_C1TX0CON.txreq = TRUE;
            break;
         case 1:
            CAN_C1TX1CON.txpri = Header->Priority;
            CAN_C1TX1CON.txreq = TRUE;
            break;
         case 2:
            CAN_C1TX2CON.txpri = Header->Priority;
            CAN_C1TX2CON.txreq = TRUE;
            break;
      }     
   }
   
   return(Result);
}

#if USE_CAN2_PERIPHERAL == TRUE
 can_ec_t can2_putd(CAN_TX_HEADER *Header, uint8_t *Data)
 {
   can_ec_t Result = CAN_EC_OK;
   uint8_t *dPtr;
   uint8_t Buffer;
   
   if(!CAN_C2TX0CON.txreq)
   {
      Buffer = 0;      
      dPtr = &CAN_C2TX0DATA;
   }
   else if(!CAN_C2TX1CON.txreq)
   {
      Buffer = 1;      
      dPtr = &CAN_C2TX1DATA;
   }
   else if(!CAN_C2TX2CON.txreq)
   {
      Buffer = 2;      
      dPtr = &CAN_C2TX2DATA;
   }
   else
      Result = CAN_EC_BUFFER_TX_FULL;
   
   if(Result == CAN_EC_OK)
   {
      can_set_buffer_id(Header, Buffer, 2);
      memcpy(dPtr, Data, Header->Length);
      
      switch(Buffer)
      {
         case 0:
            CAN_C2TX0CON.txpri = Header->Priority;
            CAN_C2TX0CON.txreq = TRUE;
            break;
         case 1:
            CAN_C2TX1CON.txpri = Header->Priority;
            CAN_C2TX1CON.txreq = TRUE;
            break;
         case 2:
            CAN_C2TX2CON.txpri = Header->Priority;
            CAN_C2TX2CON.txreq = TRUE;
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
// Function used to get message from a RX Buffer.
//
// Parameters:
//    Header - pointer to CAN_RX_HEADER variable to store info about the
//             retrieved message, e.g. the message ID.  See CAN_RX_HEADER type
//             in can_dspic30f.h for more info.
//
//    Data - pointer to an uint8_t array to return data portion of message to.
//
// Returns - can_ec_t error code, see can-dspic30f.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data)
{
   can_ec_t Result = CAN_EC_OK;
   uint8_t Buffer;
   
   if(CAN_C1RX0CON.rxful)
      Buffer = 0;
   else if(CAN_C1RX1CON.rxful)
      Buffer = 1;
   else
      Result = CAN_EC_BUFFER_RX_EMPTY;
   
   if(Result == CAN_EC_OK)
   {
      Header->Buffer = Buffer;
      can_get_rx_data(Header, Data);
      
      CAN_C1INTF.ivrif = 0;
      
      switch(Buffer)
      {
         case 0:
            CAN_C1INTF.rx0if = 0;
            CAN_C1INTF.rx0ovr = 0;
            CAN_C1RX0CON.rxful = 0;
            break;
         case 1:
            CAN_C1INTF.rx1if = 0;
            CAN_C1INTF.rx1ovr = 0;
            CAN_C1RX1CON.rxful = 0;
            break;
      }
   }
   
   return(Result);
}

#if USE_CAN2_PERIPHERAL == TRUE
 can_ec_t can2_getd(CAN_RX_HEADER *Header, uint8_t *Data)
 {
   can_ec_t Result = CAN_EC_OK;
   uint8_t Buffer;
   
   if(CAN_C2RX0CON.rxful)
      Buffer = 0;
   else if(CAN_C2RX1CON.rxful)
      Buffer = 1;
   else
      Result = CAN_EC_BUFFER_RX_EMPTY;
   
   if(Result == CAN_EC_OK)
   {
      Header->Buffer = Buffer;
      can_get_rx_data(Header, Data);
      
      CAN_C2INTF.ivrif = 0;
      
      switch(Buffer)
      {
         case 0:
            CAN_C2INTF.rx0if = 0;
            CAN_C2INTF.rx0ovr = 0;
            CAN_C2RX0CON.rxful = 0;
            break;
         case 1:
            CAN_C2INTF.rx1if = 0;
            CAN_C2INTF.rx1ovr = 0;
            CAN_C2RX1CON.rxful = 0;
            break;
      }
   }
   
   return(Result);
 }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_kbhit()
// can2_kbhit()
//
// Used to determine if there is at least one message to retrieve from the 
// RX buffers.
//
// Parameters - None.
//
// Returns:
//    TRUE - if a buffer has a new message to retrieve.
//    FALSE - if no buffers have a new message to retrieve.
///////////////////////////////////////////////////////////////////////////////
int1 can_kbhit(void)
{
   if(CAN_C1RX0CON.rxful || CAN_C1RX1CON.rxful)
      return(TRUE);
   else
      return(FALSE);
}

#if USE_CAN2_PERIPHERAL == TRUE
 int1 can2_kbhit(void)
 {
   if(CAN_C2RX0CON.rxful || CAN_C2RX1CON.rxful)
      return(TRUE);
   else
      return(FALSE);
 }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_tbe()
// can2_tbe()
//
// Used to determine if at least one TX buffer can accept a new message to 
// transmit.
//
// Parameters - None.
//
// Returns:
//    TRUE - at least one TX buffer can accept a new message.
//    FALSE - all TX buffers are full.
///////////////////////////////////////////////////////////////////////////////
int1 can_tbe(void)
{
   if(!CAN_C1TX0CON.txreq || !CAN_C1TX1CON.txreq || !CAN_C1TX2CON.txreq)
      return(TRUE);
   else
      return(FALSE);
}

#if USE_CAN2_PERIPHERAL == TRUE
 int1 can2_tbe(void)
 {
   if(!CAN_C2TX0CON.txreq || !CAN_C2TX1CON.txreq || !CAN_C2TX2CON.txreq)
      return(TRUE);
   else
      return(FALSE);
 }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_tx_is_empty()
// can2_tx_is_empty()
//
// Used to determine if all TX buffers are empty.
//
// Parameters - None.
//
// Returns:
//    TRUE - all TX buffer are empty.
//    FALSE - at least one TX buffer is not empty.
///////////////////////////////////////////////////////////////////////////////
int1 can_tx_is_empty(void)
{
   if(!CAN_C1TX0CON.txreq && !CAN_C1TX1CON.txreq && !CAN_C1TX2CON.txreq)
      return(TRUE);
   else
      return(FALSE);
}

#if USE_CAN2_PERIPHERAL == TRUE
 int1 can2_tx_is_empty(void)
 {
   if(!CAN_C2TX0CON.txreq && !CAN_C2TX1CON.txreq && !CAN_C2TX2CON.txreq)
      return(TRUE);
   else
      return(FALSE);
 }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_abort()
//
// Used to abort all pending transmissions.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_abort(void)
{
   CAN_C1CTRL.abat = 1;
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_abort(void)
 {
   CAN_C2CTRL.abat = 1;
 }
#endif

///////////////////////////////////////////////////////////////////////////////
// can_enable_interrupts()
// can2_enable_interrupts()
//
// Used to enable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to enable, can be the following:
//       CAN_INTERRUPT_RX0
//       CAN_INTERRUPT_RX1
//       CAN_INTERRUPT_TX0
//       CAN_INTERRUPT_TX1
//       CAN_INTERRUPT_TX2
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
   (uint8_t)CAN_C1INTE |= Setting;
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_enable_interrupts(CAN_INTERRUPT Setting)
 {
   (uint8_t)CAN_C2INTE |= Setting;
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
//       CAN_INTERRUPT_RX0
//       CAN_INTERRUPT_RX1
//       CAN_INTERRUPT_TX0
//       CAN_INTERRUPT_TX1
//       CAN_INTERRUPT_TX2
//       CAN_INTERRUPT_ERR
//       CAN_INTERRUPT_WAKE
//       CAN_INTERRUPT_INVALID
//
//    You can or these together (via |) to enable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_interrupts(CAN_INTERRUPT Setting)
{
   (uint8_t)CAN_C1INTE &= (~((uint8_t)Setting));
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_disable_interrupts(CAN_INTERRUPT Setting)
 {
   (uint8_t)CAN_C2INTE &= (~((uint8_t)Setting));
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
//           bit will be set, set CAN_INTERRUPT type in can-dspic30f.h to
//           determine which bits correspond to which interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_INTERRUPT can_interrupts_active(void)
{
   return((CAN_INTERRUPT)CAN_C1INTF);
}

#if USE_CAN2_PERIPHERAL == TRUE
 CAN_INTERRUPT can2_interrupts_active(void)
 {
   return((CAN_INTERRUPT)CAN_C2INTF);
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

#if USE_CAN2_PERIPHERAL == TRUE
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
   
   (CAN_INTERRUPT)CAN_C1INTF &= Interrupt;
}

#if USE_CAN2_PERIPHERAL == TRUE
 void can2_clear_interrupt(CAN_INTERRUPT Interrupt)
 {
   Interrupt = ~Interrupt;
   
   (CAN_INTERRUPT)CAN_C2INTF &= Interrupt;
 }
#endif

///////////////////////////////////////////////////////////////////////////////

void can_set_id(uint16_t *Addr, uint32_t Id, int1 Ext, int1 Type)
{
   if(Ext)    //Extended Id
   {
      //eid
      ((CiRXFnID *)Addr)->eid0_5 = (Id & 0x3F);
      ((CiRXFnID *)Addr)->eid6_17 = ((Id >> 6) & 0xFFF);
   
      //sid
      ((CiRXFnID *)Addr)->sid0_10 = ((Id >> 18) & 0x7FF);
      
      ((CiRXFnID *)Addr)->exide = Type;
   }
   else  //Standard ID
   {
      //eid
      memset(Addr, 0x00, sizeof(CiRXFnID));
      
      //sid
      ((CiRXFnID *)Addr)->sid0_10 = (Id & 0x7FF);
      
      ((CiRXFnID *)Addr)->exide = Type;
   }
}

void can_set_buffer_id(CAN_TX_HEADER *Header, uint8_t Buffer, uint8_t Peripheral=1)
{
   CiTXnID *iPtr;
   CiTXnDLC *lPtr;
   uint32_t Id;
   
   Id = Header->Id;
   
   switch(Buffer)
   {
      case 0:
        #if USE_CAN2_PERIPHERAL == TRUE
         if(Peripheral == 2)
         {
            iPtr = &CAN_C2TX0ID;
            lPtr = &CAN_C2TX0DLC;
         }
         else
        #endif
         {
            iPtr = &CAN_C1TX0ID;
            lPtr = &CAN_C1TX0DLC;
         }
         break;
      case 1:
        #if USE_CAN2_PERIPHERAL == TRUE
         if(Peripheral == 2)
         {
            iPtr = &CAN_C2TX1ID;
            lPtr = &CAN_C2TX1DLC;
         }
         else
        #endif
         {
            iPtr = &CAN_C1TX1ID;
            lPtr = &CAN_C1TX1DLC;
         }
         break;
      case 2:
        #if USE_CAN2_PERIPHERAL == TRUE
         if(Peripheral == 2)
         {
            iPtr = &CAN_C2TX2ID;
            lPtr = &CAN_C2TX2DLC;
         }
         else
        #endif
         {
            iPtr = &CAN_C1TX2ID;
            lPtr = &CAN_C1TX2DLC;
         }
         break;
      default:
         return;
         break;
   }
   
   if(Header->ext)
   {
      //sid
      iPtr->sid0_5 = ((Id >> 18) & 0x3F);
      iPtr->sid6_10 = ((Id >> 24) & 0x1F);
      
      //eid
      lPtr->eid0_5 = (Id & 0x3F);
      iPtr->eid6_13 = ((Id >> 6) & 0xFF);
      iPtr->eid14_17 = ((Id >> 14) & 0x0F);
      
      iPtr->exide = 1;
      iPtr->srr = 1;
      lPtr->txrb0 = 0;
      lPtr->txrb1 = 0;
      lPtr->dlc = Header->Length;
      lPtr->txrtr = Header->rtr;
   }
   else
   {
      //eid
      iPtr->dw = 0;
      memset(lPtr, 0x00, sizeof(CiTXnDLC));
      
      //sid
      iPtr->sid0_5 = (Id & 0x3F);
      iPtr->sid6_10 = ((Id >> 6) & 0x1F);
      
      iPtr->srr = Header->rtr;
   }
}

void can_get_rx_data(CAN_RX_HEADER *Header, uint8_t *Data, uint8_t Peripheral=1)
{
   CiRXnID *iPtr;
   CiRXnDLC *lPtr;
   uint8_t *dPtr;
   uint8_t Buffer = Header->Buffer;
   uint16_t Temp;
   
   switch(Buffer)
   {
      case 0:
        #if USE_CAN2_PERIPHERAL == TRUE
         if(Peripheral == 2)
         {
            Header->Filter = CAN_C2RX0CON.filhit0;
            Header->rtr = CAN_C2RX0CON.rxrtrro;
            Header->err_ovfl = CAN_C2INTF.rx0ovr;
            iPtr = &CAN_C2RX0ID;
            lPtr = &CAN_C2RX0DLC;
            dPtr = &CAN_C2RX0DATA;
         }
         else
        #endif
         {
            Header->Filter = CAN_C1RX0CON.filhit0;
            Header->rtr = CAN_C1RX0CON.rxrtrro;
            Header->err_ovfl = CAN_C1INTF.rx0ovr;
            iPtr = &CAN_C1RX0ID;
            lPtr = &CAN_C1RX0DLC;
            dPtr = &CAN_C1RX0DATA;
         }
         break;
      case 1:
         #if USE_CAN2_PERIPHERAL == TRUE
         if(Peripheral == 2)
         {
            Header->Filter = CAN_C2RX1CON.filhit;
            Header->rtr = CAN_C2RX1CON.rxrtrro;
            Header->err_ovfl = CAN_C2INTF.rx1ovr;
            iPtr = &CAN_C2RX1ID;
            lPtr = &CAN_C2RX1DLC;
            dPtr = &CAN_C2RX1DATA;
         }
         else
        #endif
         {
            Header->Filter = CAN_C1RX1CON.filhit;
            Header->rtr = CAN_C1RX1CON.rxrtrro;
            Header->err_ovfl = CAN_C1INTF.rx1ovr;
            iPtr = &CAN_C1RX1ID;
            lPtr = &CAN_C1RX1DLC;
            dPtr = &CAN_C1RX1DATA;
         }
         break;
   }
   
   Header->Length = lPtr->dlc;
   Header->ext = iPtr->exide;
   
   memset(&Header->Id, 0x00, sizeof(uint32_t));
   
   if(Header->ext)
   {
      Header->Id = lPtr->eid0_5;
      Header->Id |= (((uint32_t)iPtr->eid6_17) << 6);
      Temp = ((*(uint16_t *)iPtr) >> 2) & 0x3FF;
      Header->Id |= (((uint32_t)Temp) << 18);
   }
   else
      Header->Id = iPtr->sid0_10;
   
   memcpy(Data, dPtr, Header->Length);
}
