///////////////////////////////////////////////////////////////////////////////
////                                J1939.c                                ////
////                                                                       ////
//// J1939 Library for the Data Link Layer of the SAE J1939 specification. ////
//// Refer to the SAE J1939-21 for more information on spec.               ////
////                                                                       ////
//// See J1939.h for API.                                                  ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////                                                                       ////
//// Version History                                                       ////
////                                                                       ////
////  Dec 13 19 - Updated to use new CAN drivers and CAN API.              ////
////                                                                       ////
////  Jan 14 14 - Updated internal PIC18 devices to setup CAN for Enhanced ////
////              FIFO mode.  Updated internal PIC18, PIC24 and dsPIC33 to ////
////              call can_fifo_getd() function instead of can_getd()      ////
////              function.                                                ////
////                                                                       ////
////  May 19 15 - Fixed a typecasting issue when calling can_putd()        ////
////              function.                                                ////
////                                                                       ////
////  Dec 18 15 - Fixed an issue that was causing driver to incorrectly    ////
////              respond to address claim messages for addresses that     ////
////              wasn't claimed by the device with the address not        ////
////              claimed message.                                         ////
////                                                                       ////
////  Feb 02 17 - Fixed an issue with CAN Filter Mask setup for dsPIC30F   ////
////              devices that caused them to receive messages that they   ////
////              should not receive.                                      ////
////                                                                       ////
////  Oct 13 17 - Changed calls to can_set_id() function to get rid of     ////
////              Pointer types do not match Warning.                      ////
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

#include "j1939.h"

#if getenv("VERSION") < 5.092
 #error This driver is only supported in compiler version 5.092 and newer
#endif

#ifndef J1939_BAUD_RATE
#define J1939_BAUD_RATE                   250000
#endif

#ifndef J1939_USE_INTERNAL_CAN
 #if getenv("CAN") > 0
  #define J1939_USE_INTERNAL_CAN          TRUE
 #else
  #define J1939_USE_INTERNAL_CAN          FALSE
 #endif
#endif

#if J1939_USE_INTERNAL_CAN == FALSE
 #ifndef J1939_USE_EXT_FD_CONTROLLER
  #define J1939_USE_EXT_FD_CONTROLLER     FALSE
 #endif
#endif

#ifndef J1939_RECEIVE_BUFFERS
 #define J1939_RECEIVE_BUFFERS            16
#endif

#if J1939_RECEIVE_BUFFERS < 2
 #undef J1939_RECEIVE_BUFFERS
 #define J1939_RECEIVE_BUFFERS            2  //required to be able to receive one RTS/CTS session and one BAM session at same time
#endif

#ifndef J1939_TRANSMIT_BUFFERS
 #define J1939_TRANSMIT_BUFFERS           2
#endif

#if J1939_TRANSMIT_BUFFERS == 0
 #undef J1939_TRANSMIT_BUFFERS
 #define J1939_TRANSMIT_BUFFERS           1
#endif

#define CAN_BAUD_RATE      J1939_BAUD_RATE

// include the CAN drivers
#if (J1939_USE_INTERNAL_CAN == TRUE)
 #if defined(__PCD__)
  #if getenv("CAN") == 1
   #include <can-dspic30f.c>     //dsPIC30
  #elif getenv("CAN") == 2
   #define CAN_TX_BUFFERS     2
   
   #include <can-PIC24_dsPIC33.c>      //ECAN peripheral
  #elif getenv("CAN") == 3
   #define CAN_TX_BUFFERS     2
   
   #include <can-dspic33_fd.c>         //CANFD peripheral
  #else
   #error Device does not have CAN/ECAN peripheral
  #endif  
 #else
  #if getenv("CAN") == 2
   #include <can-pic18f_ecan.c>    //PIC18
  #elif getenv("CAN") > 0
   #error CAN peripheral not supported by this driver
  #else
   #error Device does not have ECAN peripheral
  #endif
 #endif
#else
 #if J1939_USE_EXT_FD_CONTROLLER == TRUE
  #define CAN_TX_BUFFERS     2
  
  #include <can-mcp2517.c>    //external CANFD controller
 #else
  #include <can-mcp2515.c>    //external CAN controller
 #endif
#endif

#ifdef CAN_DEVICE_HAS_FD
 #define J1939_CAN_OP_MODE CAN_OP_NORMAL_FD
#else
 #define J1939_CAN_OP_MODE CAN_OP_NORMAL
#endif

#define j1939can_getd      can_getd

///////////////////////////////// Types 

//J1939 Message Structure
typedef struct _J1939_MESSAGE_STRUCT {
   J1939_PDU_STRUCT PDU;
   uint8_t Length;
   uint8_t Data[8];
} J1939_MESSAGE_STRUCT;

//J1939 Flag structure
typedef struct _J1939_FLAGS_STRUCT {
   int1    AddressClaimed;       //Unit Successfully claimed an address
   int1    AddressClaimSent;     //Unit has sent a claim request
   int1    AddressNewClaim;      //Used to specify if claim request is for a new address
   int1    AddressCannotClaim;   //If not arbitrary address capable, is set if unit can't claim address
   uint8_t unused4_1:4;
   uint8_t ReceiveBufferCount;   //Keep track of number of stored messages in receive buffer
   uint8_t XmitBufferCount;      //Keep track of number of messages that still need transmitted
} J1939_FLAGS_STRUCT;

//global J1939 Flag structure variable
J1939_FLAGS_STRUCT g_J1939Flags;

/////////////////////////////// Global variables //////////////////////////////

//global variables containing unit's J1939 Address and Name
uint8_t g_MyJ1939Address;
uint8_t g_J1939Name[8];

//global J1939 tick variables
J1939_TICK_TYPE g_J1939CurrentClaimTick, g_J1939PreviousClaimTick;
J1939_TICK_TYPE g_J1939PreviousCannotClaimTick;
J1939_TICK_TYPE g_J1939CannotClaimDelay;

//global J1939 Receive and Transmit buffers
J1939_MESSAGE_STRUCT g_J1939ReceiveBuffer[J1939_RECEIVE_BUFFERS];
J1939_MESSAGE_STRUCT g_J1939XmitBuffer[J1939_TRANSMIT_BUFFERS];

//global J1939 variable for indexing J1939 Receive and Transmit buffers
static uint8_t g_J1939ReceiveNextIn;
static uint8_t g_J1939ReceiveNextOut;
static uint8_t g_J1939XmitNextIn;
static uint8_t g_J1939XmitNextOut;

//global variable used in generating pseudo-random 8-bit number
uint8_t rand_seed;

//////////////////////////////// J1939 Defines ////////////////////////////////

//PDU Format Defines
#define J1939_PF_BROADCAST          254
#define J1939_PF_REQUEST            234
#define J1939_PF_ACK                232
#define J1939_PF_PROPRIETARY_A      239
#define J1939_PF_PROPRIETARY_B      255
#define J1939_PF_REQUEST2           201
#define J1939_PF_TRANSFER           202
#define J1939_PF_PT_CM              236
#define J1939_PF_PT_DT              235
#define J1939_PF_ADDR_CLAIMED       238
#define J1939_PF_ADDR_CANNOT_CLAIM  238

//PDU Default Priorities Defines
#define J1939_CONTROL_PRIORITY         3
#define J1939_REQUEST_PRIORITY         6
#define J1939_ACK_PRIORITY             6
#define J1939_PROPRIETARY_A_PRIORITY   6
#define J1939_PROPRIETARY_B_PRIORITY   6
#define J1939_REQUEST2_PRIORITY        6
#define J1939_TRANSFER_PRIORITY        6
#define J1939_TP_CM_PRIORITY           7
#define J1939_TP_DT_PRIORITY           7

//Defines used with Transport Protocol Messages (refer to J1939-21 for spec)
#define J1939_TP_CM_CTS          16
#define J1939_TP_CM_DTS          17
#define J1939_TP_CM_EOF          19
#define J1939_TP_CM_ABORT        255
#define J1939_TP_CM_BAM          32

//J1939 Address Defines
#define J1939_NULL_ADDRESS       254
#define J1939_GLOBAL_ADDRESS     255

////////////////////////////////// Prototypes /////////////////////////////////

void J1939ClaimAddress(void);
int1 J1939CheckName(uint8_t *data);
void J1939HandleAddressRequest(J1939_PDU_STRUCT PDU);
void J1939LoadReceiveBuffer(J1939_PDU_STRUCT ReceivedPDU,uint8_t *Data,uint8_t length);
void J1939HandleAddressClaim(J1939_PDU_STRUCT ReceivedPDU, uint8_t *Name);
void J1939SetCANFilter(uint8_t address);
uint8_t xor8(void);

///////////////////////////////////// API /////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//J1939Init()
// Initializes the CAN for J1939 Baud Rate and sets up the CAN filters, and 
// initial J1939 Address Claim.
//  Parameters: None
//  Returns:    Nothing
////////////////////////////////////////////////////////////////////////////////
void J1939Init(void)
{
   memset(&g_J1939Flags,0,sizeof(J1939_FLAGS_STRUCT));   //clear the J1939 Flag structure
   
   J1939InitAddress();  //Initialize unit's J1939 Preferred Address
   J1939InitName();     //Initialize unit's J1939 Name
   
   rand_seed = 128;  //Initialize random generator seed number

   can_init();    //Initialize the CAN, sets up Baud Rate and puts it in normal mode
   
   can_set_mode(CAN_OP_CONFIG);  //put CAN in configuration mode
   
  #ifdef CAN_DEVICE_HAS_FD
   can_set_filter_id(CAN_FILTER_0, 0x0000FF00, 0x0000FF00, CAN_FILTER_TYPE_EID_ONLY);  //Filter 0 set to look for messages to the Global Address 255
   can_set_filter_id(CAN_FILTER_1, 0x0000FF00, 0x0000FF00, CAN_FILTER_TYPE_EID_ONLY);  //Filter 1 set to look for messages to the Global Address 255, which change to unit's address once it gets one
   can_set_filter_id(CAN_FILTER_2, 0x00F00000, 0x00F00000, CAN_FILTER_TYPE_EID_ONLY);  //Filter 2 set to look for Broadcast messages PDU 240 to 255
   
   can_enable_filter(CAN_FILTER_0, CAN_OBJECT_FIFO_1);
   can_enable_filter(CAN_FILTER_1, CAN_OBJECT_FIFO_1);
   can_enable_filter(CAN_FILTER_2, CAN_OBJECT_FIFO_1);
  #elif (getenv("CAN") == 1) && (J1939_USE_INTERNAL_CAN == TRUE) //dsPIC30F
   can_set_mask_id(CAN_FILTER_MASK_0, 0x0000FF00, CAN_MASK_ID_TYPE_EID, CAN_FILTER_MASK_TYPE_SID_OR_EID);   //Set Mask 0 to look at Destination Address of PDU
   can_set_mask_id(CAN_FILTER_MASK_1, 0x00F00000, CAN_MASK_ID_TYPE_EID, CAN_FILTER_MASK_TYPE_SID_OR_EID);   //Set Mask 1 to look at upper nibble of PDU Format
   
   can_set_filter_id(CAN_FILTER_0, 0x0000FF00, CAN_FILTER_TYPE_EID);    //Filter 0 set to look for messages to the Global Address 255
   can_set_filter_id(CAN_FILTER_1, 0x0000FF00, CAN_FILTER_TYPE_EID);    //Filter 1 set to look for messages to the Global Address 255, will change to unit's address once it gets one
   can_set_filter_id(CAN_FILTER_2, 0x00F00000, CAN_FILTER_TYPE_EID);    //Filter 2 set to look for Broadcast messages PDU 240 to 255
  
  #elif (getenv("CAN") == 2) && (J1939_USE_INTERNAL_CAN == TRUE) //ECAN peripheral
   can_set_mask_id(CAN_FILTER_MASK_0, 0x0000FF00, CAN_MASK_ID_TYPE_EID, CAN_FILTER_MASK_TYPE_SID_OR_EID);   //Set Mask 0 to look at Destination Address of PDU
   can_set_mask_id(CAN_FILTER_MASK_1, 0x00F00000, CAN_MASK_ID_TYPE_EID, CAN_FILTER_MASK_TYPE_SID_OR_EID);   //Set Mask 1 to look at upper nibble of PDU Format
   
   can_set_filter_id(CAN_FILTER_0, 0x0000FF00, CAN_FILTER_TYPE_EID);    //Filter 0 set to look for messages to the Global Address 255
   can_set_filter_id(CAN_FILTER_1, 0x0000FF00, CAN_FILTER_TYPE_EID);    //Filter 1 set to look for messages to the Global Address 255, will change to unit's address once it gets one
   can_set_filter_id(CAN_FILTER_2, 0x00F00000, CAN_FILTER_TYPE_EID);    //Filter 2 set to look for Broadcast messages PDU 240 to 255
   
   can_enable_filter(CAN_FILTER_0, CAN_FILTER_BUFFER_FIFO, CAN_FILTER_MASK_0);   //Enable Filter 0
   can_enable_filter(CAN_FILTER_1, CAN_FILTER_BUFFER_FIFO, CAN_FILTER_MASK_0);   //Enable Filter 1
   can_enable_filter(CAN_FILTER_2, CAN_FILTER_BUFFER_FIFO, CAN_FILTER_MASK_1);   //Enable Filter 2
  #else  //MCP2515 External CAN controller
   can_set_mask_id(CAN_FILTER_MASK_0, 0x0000FF00, CAN_MASK_TYPE_EID);   //Set Mask 0 to look at Destination Address of PDU
   can_set_mask_id(CAN_FILTER_MASK_1, 0x00F00000, CAN_MASK_TYPE_EID);   //Set Mask 1 to look at upper nibble of PDU Format
   
   can_set_filter_id(CAN_FILTER_0, 0x0000FF00, CAN_FILTER_TYPE_EID);    //Filter 0 set to look for messages to the Global Address 255
   can_set_filter_id(CAN_FILTER_1, 0x0000FF00, CAN_FILTER_TYPE_EID);    //Filter 1 set to look for messages to the Global Address 255, will change to unit's address once it gets one
   can_set_filter_id(CAN_FILTER_2, 0x00F00000, CAN_FILTER_TYPE_EID);    //Filter 2 set to look for Broadcast messages PDU 240 to 255
   can_set_filter_id(CAN_FILTER_3, 0x00F00000, CAN_FILTER_TYPE_EID);    //Filter 3 set to look for Broadcast messages PDU 240 to 255
   can_set_filter_id(CAN_FILTER_4, 0x00F00000, CAN_FILTER_TYPE_EID);    //Filter 4 set to look for Broadcast messages PDU 240 to 255
   can_set_filter_id(CAN_FILTER_5, 0x00F00000, CAN_FILTER_TYPE_EID);    //Filter 5 set to look for Broadcast messages PDU 240 to 255
  #endif
   
   can_set_mode(J1939_CAN_OP_MODE);
   
   J1939ClaimAddress();  //Attempt to Claim unit's address
}

////////////////////////////////////////////////////////////////////////////////
//J1939ReceiveTask()
// Checks for new CAN messages and loads into J1939 Receive Buffer
//  Parameters: None
//  Returns:    Nothing
//
// Warning - This function will continue to check CAN buffers until all
//           messages are retrieved from CAN buffers, if J1939 Receive buffer
//           isn't large enough it will throw away any messages that will
//           overflow the buffer.  This should only be a problem for PIC24 and
//           dsPIC33 chips which can have up to 32 CAN receive buffers, but
//           should be OK as long as J1939_RECEIVE_BUFFERS is set high enough
//           and J1939GetMessage() is called frequently to clear data.
////////////////////////////////////////////////////////////////////////////////
void J1939ReceiveTask(void)
{
   J1939_PDU_STRUCT ReceivedPDU;
   uint8_t Data[8];
   CAN_RX_HEADER Header;
   
   rand_seed++;
   
   while(can_kbhit())
   {
      j1939can_getd(&Header, Data);
      
      if(g_J1939Flags.ReceiveBufferCount < J1939_RECEIVE_BUFFERS)
      {
         memcpy(&ReceivedPDU, &Header.Id, sizeof(J1939_PDU_STRUCT));
      
         switch(ReceivedPDU.PDUFormat)
         {
            case J1939_PF_ADDR_CLAIMED:
               J1939HandleAddressClaim(ReceivedPDU,Data);
               
               if((ReceivedPDU.SourceAddress != g_MyJ1939Address) && (ReceivedPDU.SourceAddress != J1939_NULL_ADDRESS))
               {
                  J1939LoadReceiveBuffer(ReceivedPDU, Data, Header.Length);  //so you can keep a list of J1939Names to J1939Addresses, if desired
               }
               break;
            case J1939_PF_REQUEST:
               if((Data[0] == 0x00) && (Data[1] == 0xEE) && (Data[2] == 0x00))
               {
                  J1939HandleAddressRequest(ReceivedPDU);
                  break;
               }
            default:
               J1939LoadReceiveBuffer(ReceivedPDU, Data, Header.Length);
               break;
         }
      }
   }
   
   if((g_J1939Flags.AddressClaimed == FALSE) && (g_J1939Flags.AddressClaimSent == TRUE) && (g_J1939Flags.AddressCannotClaim == FALSE))
   {
      g_J1939CurrentClaimTick = J1939GetTick();
      
      if(J1939GetTickDifference(g_J1939CurrentClaimTick, g_J1939PreviousClaimTick) >= (J1939_TICK_TYPE)J1939_TICKS_PER_SECOND/4)
      {
         g_J1939Flags.AddressClaimed = TRUE;
         J1939SetCANFilter(g_MyJ1939Address);      //unit claimed address so setup filter to start looking for 
                                                   //J1939 Messages sent to unit's address 
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
//J1939XmitTask()
// Checks for message in Xmit Buffer and loads into CAN buffers to transmit.
//  Parameters: None
//  Returns:    Nothing
////////////////////////////////////////////////////////////////////////////////
void J1939XmitTask(void)
{
   J1939_TICK_TYPE CurrentTick;
   CAN_TX_Header Header;

   while((g_J1939Flags.XmitBufferCount > 0) && can_tbe())
   {
      if((g_J1939Flags.AddressClaimed == TRUE) || (g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.PDUFormat == J1939_PF_ADDR_CLAIMED) || 
         ((g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.PDUFormat == J1939_PF_REQUEST) && (g_J1939XmitBuffer[g_J1939XmitNextOut].Data[0] == 0x00) &&
          (g_J1939XmitBuffer[g_J1939XmitNextOut].Data[1] == 0xEE) && (g_J1939XmitBuffer[g_J1939XmitNextOut].Data[2] == 0x00)))
      {
         if((g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.PDUFormat == J1939_PF_ADDR_CLAIMED) && (g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.DestinationAddress == J1939_NULL_ADDRESS))
         {
            CurrentTick = J1939GetTick();
            
            if(J1939GetTickDifference(CurrentTick, g_J1939PreviousCannotClaimTick) <= g_J1939CannotClaimDelay)
               break;
         }
         
         Header.Id = (uint32_t)g_J1939XmitBuffer[g_J1939XmitNextOut].PDU;
         Header.Length = g_J1939XmitBuffer[g_J1939XmitNextOut].Length;
        #ifdef CAN_DEVICE_HAS_FD
         Header.Format = CAN_MESSAGE_FORMAT_CAN_2_0;
        #else
         Header.Priority = 3;
        #endif
         Header.ext = TRUE;
         Header.rtr = FALSE;
               
         can_putd(&Header, g_J1939XmitBuffer[g_J1939XmitNextOut].Data);
         
         if((g_J1939Flags.AddressClaimed == FALSE) && (g_J1939Flags.AddressNewClaim == TRUE) && (g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.PDUFormat == J1939_PF_ADDR_CLAIMED) && (g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.DestinationAddress != J1939_NULL_ADDRESS))
         {
            if((bit_test(g_J1939Name[7],7) == FALSE) && ((g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.DestinationAddress <= 128) || 
               ((g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.DestinationAddress >= 248) && (g_J1939XmitBuffer[g_J1939XmitNextOut].PDU.DestinationAddress <=253))))
            {
               g_J1939Flags.AddressClaimed = TRUE;
               g_J1939Flags.AddressClaimSent = TRUE;
               g_J1939Flags.AddressNewClaim = FALSE;
               
               J1939SetCANFilter(g_MyJ1939Address);   //unit claimed address so setup filter to start looking for 
                                                      //J1939 Messages sent to unit's address
            }
            else
            {
               g_J1939PreviousClaimTick = J1939GetTick();
               g_J1939Flags.AddressClaimSent = TRUE;
               g_J1939Flags.AddressNewClaim = FALSE;
            }
         }            
      }
               
      if(++g_J1939XmitNextOut >= J1939_TRANSMIT_BUFFERS)
         g_J1939XmitNextOut = 0;
         
       g_J1939Flags.XmitBufferCount--;
   }
}

////////////////////////////////////////////////////////////////////////////////
//J1939Kbhit()
// Checks for a new message in receive buffer
//  Parameters: None
//  Returns: True - if a new message is in buffer
//           False - if no new message was in buffer
////////////////////////////////////////////////////////////////////////////////
int1 J1939Kbhit(void)
{
   if(g_J1939Flags.ReceiveBufferCount > 0)
      return(TRUE);
   else
      return(FALSE);
}

////////////////////////////////////////////////////////////////////////////////
//J1939GetMessage()
// Retrieves a message from buffer
//  Parameters: PDU - PDU structure to return message's PDU to
//              Data - pointer to return data to
//              Length - variable to return message length to
//  Returns:    True - if new message was retrieved
//              False - if there was no new message to retrieve
////////////////////////////////////////////////////////////////////////////////
int1 J1939GetMessage(J1939_PDU_STRUCT &PDU, uint8_t *Data, uint8_t &Length)
{
   uint8_t i;

   if(g_J1939Flags.ReceiveBufferCount > 0)
   {
      Length = g_J1939ReceiveBuffer[g_J1939ReceiveNextOut].Length;
      memcpy(&PDU,&g_J1939ReceiveBuffer[g_J1939ReceiveNextOut].PDU,sizeof(J1939_PDU_STRUCT));
      
      for(i=0;i<Length;i++)
         Data[i] = g_J1939ReceiveBuffer[g_J1939ReceiveNextOut].Data[i];
         
      if(++g_J1939ReceiveNextOut >= J1939_RECEIVE_BUFFERS)
         g_J1939ReceiveNextOut = 0;
         
      g_J1939Flags.ReceiveBufferCount--;
      
      return(TRUE);
   }
   else
      return(FALSE);
}

////////////////////////////////////////////////////////////////////////////////
//J1939PutMessage()
// Load message into transmit buffer
//  Parameters: PDU - PDU to send with message
//              Data - pointer to data to send
//              Bytes - number of bytes to send
//  Returns:    True - if message was successfully loaded into an empty xmit buffer
//              False - if xmit buffer was full
////////////////////////////////////////////////////////////////////////////////
int1 J1939PutMessage(J1939_PDU_STRUCT PDU, uint8_t *Data, uint8_t Bytes)
{
   uint8_t i;

   if(g_J1939Flags.XmitBufferCount < J1939_TRANSMIT_BUFFERS)
   {
      memcpy(&g_J1939XmitBuffer[g_J1939XmitNextIn].PDU,&PDU,sizeof(J1939_PDU_STRUCT));
      g_J1939XmitBuffer[g_J1939XmitNextIn].Length = Bytes;
      for(i=0;i<Bytes;i++)
        g_J1939XmitBuffer[g_J1939XmitNextIn].Data[i] = Data[i];
      
      if(++g_J1939XmitNextIn >= J1939_TRANSMIT_BUFFERS)
         g_J1939XmitNextIn = 0;
         
      g_J1939Flags.XmitBufferCount++;
      
      return(TRUE);
   }
   else
      return(FALSE);
}

void J1939RequestAddress(uint8_t address)
{
   J1939_PDU_STRUCT PDU;
   uint8_t data[3];
   
   if(g_J1939Flags.AddressClaimed == FALSE)
      PDU.SourceAddress = J1939_NULL_ADDRESS;
   else
      PDU.SourceAddress = g_MyJ1939Address;
      
   PDU.DestinationAddress = address;
   PDU.PDUFormat = J1939_PF_REQUEST;
   PDU.DataPage = 0;
   PDU.ExtendedDataPage = 0;
   PDU.Priority = J1939_REQUEST_PRIORITY;
   
   data[0] = 0;
   data[1] = 0xEE;
   data[2] = 0;
   
   J1939PutMessage(PDU,data,3);
}

////////////////////////////// Internal Functions /////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//J1939ClaimAddress()
// Sends an Address Claimed message to claim address in g_MyJ1939Address.
//  Parameters: None
//  Returns:    Nothing
///////////////////////////////////////////////////////////////////////////////
void J1939ClaimAddress(void)
{
   J1939_PDU_STRUCT RequestPDU;

   RequestPDU.SourceAddress = g_MyJ1939Address;
   RequestPDU.DestinationAddress = J1939_GLOBAL_ADDRESS;
   RequestPDU.PDUFormat = J1939_PF_ADDR_CLAIMED;
   RequestPDU.DataPage = 0;
   RequestPDU.ExtendedDataPage = 0;
   RequestPDU.Priority = J1939_REQUEST_PRIORITY;
   
   g_J1939Flags.AddressNewClaim = TRUE;
   
   J1939PutMessage(RequestPDU,g_J1939Name,8);
}

////////////////////////////////////////////////////////////////////////////////
//J1939CompareName()
// Compares our name with received name to determine which has priority
//  Parameters: data - pointer to received name
//  Returns:    True - if our name is high priority
//              False - if our name is lower priority
////////////////////////////////////////////////////////////////////////////////
int1 J1939CompareName(uint8_t *data)
{
   uint8_t i;
   
   for(i=0;i<8;i++)
   {
      if(g_J1939Name[i] > data[i])
         return(FALSE);
   }
   
   return(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
//J1939HandleAddressRequest()
// Generates response to J1939 Address Requests
//  Parameters: None
//  Returns:    Nothing
////////////////////////////////////////////////////////////////////////////////
void J1939HandleAddressRequest(J1939_PDU_STRUCT PDU)
{
   J1939_PDU_STRUCT RequestPDU;
   
   if(g_J1939Flags.AddressClaimed != TRUE)
      RequestPDU.SourceAddress = J1939_NULL_ADDRESS;
   else
      RequestPDU.SourceAddress = g_MyJ1939Address;
   
   RequestPDU.DestinationAddress = J1939_GLOBAL_ADDRESS;
   RequestPDU.PDUFormat = J1939_PF_ADDR_CLAIMED;            //value is the same for both Address Claimed and Cannot Claim Address
   RequestPDU.DataPage = 0;
   RequestPDU.ExtendedDataPage = 0;
   RequestPDU.Priority = J1939_REQUEST_PRIORITY;
   
   if((g_J1939Flags.AddressClaimed == TRUE) || (g_J1939Flags.AddressClaimSent == TRUE))  //don't respond to request if an address claim hasn't been sent
      J1939PutMessage(RequestPDU,g_J1939Name,8);
}

////////////////////////////////////////////////////////////////////////////////
//J1939LoadReceiveBuffer()
// Loads g_J1939ReceiveBuffer with passed data and updates global indexes.
//  Parameters: ReceivedPDU - the PDU of the received CAN message
//              Data - pointer to the received CAN data
//              length - number of bytes received in CAN message
//  Returns:    Nothing
////////////////////////////////////////////////////////////////////////////////
void J1939LoadReceiveBuffer(J1939_PDU_STRUCT ReceivedPDU,uint8_t *Data,uint8_t length)
{
   uint8_t i;
   
   memcpy(&g_J1939ReceiveBuffer[g_J1939ReceiveNextIn].PDU,&ReceivedPDU,sizeof(J1939_PDU_STRUCT));
   g_J1939ReceiveBuffer[g_J1939ReceiveNextIn].Length = length;
   for(i=0;i<length;i++)
      g_J1939ReceiveBuffer[g_J1939ReceiveNextIn].Data[i] = Data[i];
   
   if(++g_J1939ReceiveNextIn >= J1939_RECEIVE_BUFFERS)
      g_J1939ReceiveNextIn = 0;
      
   g_J1939Flags.ReceiveBufferCount++;
}

////////////////////////////////////////////////////////////////////////////////
//J1939HandleAddressClaim()
// Responses to a J1939 Address Claim message.  Compares unit's Address and Name
// to the Source Address and Name of the received J1939 Address Claim message,
// and either response with Address Claimed, Cannot Claim Address or if unit is
// Arbitrary Address Capable and if received Name is higher priority then unit's
// name it sends a new Address Request with a randomly generated address from
// 128 to 247.
//  Parameters: ReceivedPDU - the PDU of received Address Claim message
//              Name - pointer to the J1939 Name of Address Claimer
//  Returns:    Nothing
////////////////////////////////////////////////////////////////////////////////
void J1939HandleAddressClaim(J1939_PDU_STRUCT ReceivedPDU, uint8_t *Name)
{
   J1939_PDU_STRUCT RequestPDU;
   
   if(((ReceivedPDU.SourceAddress == g_MyJ1939Address) && (g_J1939Flags.AddressClaimSent)) || ((ReceivedPDU.SourceAddress != J1939_NULL_ADDRESS) && (g_J1939Flags.AddressCannotClaim == TRUE)))
   {
      RequestPDU.DestinationAddress = J1939_GLOBAL_ADDRESS;
      RequestPDU.PDUFormat = J1939_PF_ADDR_CLAIMED;            //value is the same for both Address Claimed and Cannot Claim Address
      RequestPDU.DataPage = 0;
      RequestPDU.ExtendedDataPage = 0;
      RequestPDU.Priority = J1939_REQUEST_PRIORITY;

      if(g_J1939Flags.AddressCannotClaim == FALSE)
      {
         if(J1939CompareName(Name))
         {
            RequestPDU.SourceAddress = g_MyJ1939Address;
         }
         else
         {
            if(g_J1939Flags.AddressClaimed)
               J1939SetCANFilter(J1939_GLOBAL_ADDRESS);  //Only do this if unit already claimed address,
                                                         //because this switches CAN to CONFIG mode.
            //Clear Address Claim Flags
            g_J1939Flags.AddressClaimed = FALSE;
            
            //Clear Transmit Buffer
            g_J1939XmitNextOut = 0;
            g_J1939XmitNextIn = 0;
            g_J1939Flags.XmitBufferCount = 0;
            
            if(bit_test(g_J1939Name[7],7) == FALSE)   //If not Arbitrary Address Capable send Cannot Claim Address
            {
               RequestPDU.SourceAddress = J1939_NULL_ADDRESS;
               g_J1939Flags.AddressCannotClaim = TRUE;
            }
            else  //If Arbitrary Address Capable Generate Random address from 128 to 247 and request
            {
               g_MyJ1939Address = (((uint32_t)xor8() * 46875) / 100000) + 128;
               RequestPDU.SourceAddress = g_MyJ1939Address;
               g_J1939Flags.AddressNewClaim = TRUE;
            }
         }
      }
      else
         RequestPDU.SourceAddress = J1939_NULL_ADDRESS;
      
      if(RequestPDU.SourceAddress == J1939_NULL_ADDRESS)
      {
         g_J1939PreviousCannotClaimTick = J1939GetTick();
         g_J1939CannotClaimDelay = ((uint32_t)xor8() * 53125) / 100000;    //Generate Random delay from 0 to 135ms
      }
         
      J1939PutMessage(RequestPDU,g_J1939Name,8);
   }
}
      
////////////////////////////////////////////////////////////////////////////////
//J1939SetCANFilter()
// Sets filter 1 of CAN module to receive unit's address after unit it has
// successfully claimed an address.
//  Parameters: address - address to set filter to
//  Returns:    Nothing
////////////////////////////////////////////////////////////////////////////////
void J1939SetCANFilter(uint8_t address)
{
   can_set_mode(CAN_OP_CONFIG);  //put CAN in Config mode
   
  #ifdef CAN_DEVICE_HAS_FD
   can_set_filter_id(CAN_FILTER_1, (uint32_t)address << 8, 0x0000FF00, CAN_FILTER_TYPE_EID_ONLY);
  #else
   can_set_filter_id(CAN_FILTER_1, (uint32_t)address << 8, CAN_FILTER_TYPE_EID);    //Set Filter 1
  #endif
   
   can_set_mode(J1939_CAN_OP_MODE);  //put CAN in Normal mode
}

////////////////////////////////////////////////////////////////////////////////
//xor8()
// Generates a pseudo-random 8-bit number.  rand_seed is used as a seed
//   for the algorithm.
//  Parameters: None
//  Returns:    uint8_t - Pseudo-random value
////////////////////////////////////////////////////////////////////////////////
uint8_t xor8(void)
{
   static uint8_t y = 69;
   static uint8_t z = 29;
   static uint8_t w = 23;
   uint8_t t;

   t = rand_seed ^ (rand_seed << 3);
   rand_seed = y; y = z; z = w;
   w = w ^ (w >> 5) ^ (t ^ (t >> 2));
   return (w);
}
     
