///////////////////////////////////////////////////////////////////////////////
////                             can-dsPIC30f.h                            ////
////                                                                       ////
////  Driver for Microchip's dsPIC30F401x, dsPIC30F501x and dsPIC30F601xA  ////
////  devices with a CAN peripheral.                                       ////
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

#ifndef __CAN_DSPIC30F_H__
#define __CAN_DSPIC30F_H__

/*
   CAN PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_BAUD_RATE
      The bit rate that the can_init() function will set the CAN peripheral
      to.  Default is 125 kbps.
   
   CAN_DEFAULT_SAMPLE_POINT
      The sample point used by the can_set_baud() function when setting the
      CAN bit rate.  Default is 875 for 87.5% of the bit time, the sample
      point the CANopen protocol uses.
   
   OPTIONAL FILTER PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_USE_FILTERS
      Used to enable can_init() to setup CAN filters, FALSE - disabled or 
      TRUE - enabled.  Default is FALSE, disabled, can_init() sets up CAN to
      receive all messages.
   
   CAN_USER_FILT_x
      Used to set the Id of the filter, x being the filter (0-5) to assign
      the Id to.  Filter 0 and 1 are for RX Buffer 0 and Filter 2 to 5 are for
      RX Buffer 1.
   
   CAN_USER_FILT_x_TYPE
      Used to set the filter type, x being the filter (0-5) to assign the type
      to.  The type can be one of the following:
         CAN_FILTER_TYPE_SID - match only SID messages
         CAN_FILTER_TYPE_EID - match only EID messages
   
   CAN_USER_MASK_x
      Used to set the Id of the mask, x being the mask (0-1) to assign the Id
      to.  Mask 0 is for Filters 0 and 1, and Mask 1 is for Filter 2 to 5.
   
   CAN_USER_MASK_x_TYPE
      Used to set the mask type, x being the mask (0-1), to assign the type
      to.  The type can be one of the following:
         CAN_MASK_TYPE_SID - SID mask Id
         CAN_MASK_TYPE_EID - EID mask Id
   
   OPTIONAL CAN PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_BRG_SAM
      Sets how many time the CAN bus is sampled at the sample point, 0-once, 
      1-three times.  Default is 0.
   
   CAN_BRG_WAKE_FILTER
      Sets if CAN bus line filter is used for wake-up, 0-not used, 1-used.
      Default is 0.
   
   CAN_USE_RX_DOUBLE_BUFFER
      Used to enable/disable loading RXB0 messages into Buffer 1 when Buffer 0
      would overflow.  Default is TRUE, enabled.
   
   CAN_ENABLE_CAN_CAPTURE
      Enables/disables input capture based on CAN message receive, 
      FALSE-disable, TRUE-enable.  Default is FALSE, disabled.
   
   CAN_CONTINUE_IN_IDLE
      Sets if CAN peripheral operates when in Idle mode, TRUE-continues in Idle 
      mode, FALSE-discontinued in Idle mode.  Default is FALSE.
   
-------------------------------------------------------------------------------

   OPTIONAL CAN2 PREPROCESSORS
   ----------------------------------------------------------------------------
   
   USE_CAN2_PERIPHERAL
      If device has a CAN2 peripheral this sets whether to the CAN2 functions
      will be available for use.  Default is FALSE, if device doesn't have a
      CAN2 peripheral define is forced to FALSE.
   
** If USE_CAN2_PERIPHERAL is defined to TRUE the following preprocessors can be
   used to setup the CAN2 peripheral.
   
   CAN2_BAUD_RATE
      The bit rate that the can2_init() function will set the CAN2 peripheral
      to.  Default is 125 kbps.
   
   CAN2_DEFAULT_SAMPLE_POINT
      The sample point used by the can2_set_baud() function when setting the
      CAN2 bit rate.  Default is 875 for 87.5% of the bit time, the sample
      point the CANopen protocol uses.
   
   OPTIONAL CAN2 FILTER PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN2_USE_FILTERS
      Used to enable can2_init() to setup CAN2 filters, FALSE - disabled or 
      TRUE - enabled.  Default is FALSE, disabled, can2_init() sets up CAN2 to
      receive all messages.
   
   CAN2_USER_FILT_x
      Used to set the Id of the filter, x being the filter (0-5) to assign
      the Id to.  Filter 0 and 1 are for RX Buffer 0 and Filter 2 to 5 are for
      RX Buffer 1.
   
   CAN2_USER_FILT_x_TYPE
      Used to set the filter type, x being the filter (0-5) to assign the type
      to.  The type can be one of the following:
         CAN_FILTER_TYPE_SID - match only SID messages
         CAN_FILTER_TYPE_EID - match only EID messages
   
   CAN2_USER_MASK_x
      Used to set the Id of the mask, x being the mask (0-1) to assign the Id
      to.  Mask 0 is for Filters 0 and 1, and Mask 1 is for Filter 2 to 5.
   
   CAN2_USER_MASK_x_TYPE
      Used to set the mask type, x being the mask (0-1), to assign the type
      to.  The type can be one of the following:
         CAN_MASK_TYPE_SID - SID mask Id
         CAN_MASK_TYPE_EID - EID mask Id
   
   OPTIONAL CAN2 PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN2_BRG_SAM
      Sets how many time the CAN2 bus is sampled at the sample point, 0-once, 
      1-three times.  Default is 0.
   
   CAN2_BRG_WAKE_FILTER
      Sets if CAN2 bus line filter is used for wake-up, 0-not used, 1-used.
      Default is 0.
   
   CAN2_USE_RX_DOUBLE_BUFFER
      Used to enable/disable loading RXB0 messages into Buffer 1 when Buffer 0
      would overflow.  Default is TRUE, enabled.
   
   CAN2_ENABLE_CAN_CAPTURE
      Enables/disables input capture based on CAN2 message receive, 
      FALSE-disable, TRUE-enable.  Default is FALSE, disabled.
   
   CAN2_CONTINUE_IN_IDLE
      Sets if CAN2 peripheral operates when in Idle mode, TRUE-continues in Idle 
      mode, FALSE-discontinued in Idle mode.  Default is FALSE.

*/

#include <stdint.h>

// CAN Error Codes used by this driver
// 0 is OK, non-zero is some kind of error
typedef enum 
{
   CAN_EC_OK = 0,
   CAN_EC_BUFFER_RX_EMPTY,    //can_getd failed because all RX buffers are empty, or specified buffer is empty
   CAN_EC_BUFFER_TX_FULL,     //can_putd failed because all TX buffers are full, or specified buffer is full
   CAN_EC_BAUD_NOT_DIVISIBLE, //can_set_baud failed because clock not divisible by baud rate
   CAN_EC_BAUD_INVALID,       //can_set_baud failed because baud rate not achievable
} can_ec_t;

// CAN TX Message Header type
typedef struct
{
   uint32_t Id;               //ID to send message with
   uint8_t Length;            //Number of data bytes to send
   int1 ext;                  //TRUE - send message with extended ID, FALSE - send message with standard ID
   int1 rtr;                  //TRUE - send RTR (request), FALSE - not an RTR (request)
   uint8_t Priority:2;        //0-3, use to determine which messages will be sent fist.  The high the priority the sooner the CAN peripheral will send the message
} CAN_TX_HEADER;

// CAN RX Message Header type
typedef struct
{
   uint32_t Id;               //ID of received message
   uint8_t Length;            //Number of data bytes received
   uint8_t Filter;            //Filter message was received with
   uint8_t Buffer;            //Buffer message was retrieved from
   int1 err_ovfl;             //TRUE - Buffer overflowed, FALSE - Buffer didn't overflow
   int1 ext;                  //TRUE - received extended ID message, FALSE - received standard ID message
   int1 rtr;                  //TRUE - received RTR (request), FALSE - not a RTR (request)
} CAN_RX_HEADER;

// CAN_OP_MODE type
typedef enum 
{
   CAN_OP_NORMAL,             //Normal Operation mode
   CAN_OP_DISABLE,            //Disable mode
   CAN_OP_LOOPBACK,           //Loopback mode
   CAN_OP_LISTEN,             //Listen Only mode
   CAN_OP_CONFIG,             //Configuration mode
   CAN_OP_LISTEN_ALL=7        //Listen All Messages Mode
} CAN_OP_MODE;

// CAN_FILTER type
typedef enum
{
   CAN_FILTER_0,     //Filter 0, used for RX0 Buffer
   CAN_FILTER_1,     //Filter 1, used for RX0 Buffer
   CAN_FILTER_2,     //Filter 2, used for RX1 Buffer
   CAN_FILTER_3,     //Filter 3, used for RX1 Buffer
   CAN_FILTER_4,     //Filter 4, used for RX1 Buffer
   CAN_FILTER_5      //Filter 5, used for RX1 Buffer
} CAN_FILTER;

// CAN Filter Mask type
typedef enum
{
   CAN_FILTER_MASK_0,   //Mask 0, used for Filter's 0 and 1
   CAN_FILTER_MASK_1    //Mask 1, used for Filter's 2, 3, 4 and 5
} CAN_FILTER_MASK;

// CAN Filter Type type
typedef enum
{
   CAN_FILTER_TYPE_SID,  //only SID bits of mask are written, EID bits are set to 0
   CAN_FILTER_TYPE_EID,  //both SID and EID bits of mask are written
} CAN_FILTER_TYPE;

// CAN_MASK_ID_TYPE type
typedef enum
{
   CAN_MASK_ID_TYPE_SID,   //Mask Id is a SID Id - only SID bits of Mask are written, EID bits are set to 0
   CAN_MASK_ID_TYPE_EID    //Mask Id is an EID Id - SID and EID bits of Mask are written
} CAN_MASK_ID_TYPE;

// CAN_FILTER_MASK_TYPE type
typedef enum
{
   CAN_FILTER_MASK_TYPE_EITHER,     //match either SID or EID messages, ignores EXIDE bit of filter
   CAN_FILTER_MASK_TYPE_SID_OR_EID  //only match SID or EID messages, EXIDE bit of filter determines type of messages received
} CAN_FILTER_MASK_TYPE;

// CAN_INTERRUPT type
typedef enum
{
   CAN_INTERRUPT_RX0 = 0x0001,      //RX Buffer 0 Interrupt
   CAN_INTERRUPT_RX1 = 0x0002,      //RX Buffer 0 Interrupt
   CAN_INTERRUPT_TX0 = 0x0004,      //TX Buffer 0 Interrupt
   CAN_INTERRUPT_TX1 = 0x0008,      //TX Buffer 1 Interrupt
   CAN_INTERRUPT_TX2 = 0x0010,      //TX Buffer 2 Interrupt
   CAN_INTERRUPT_ERR = 0x0020,      //Error Interrupt
   CAN_INTERRUPT_WAKE = 0x0040,     //Bus Wake-up Activity Interrupt
   CAN_INTERRUPT_INVALID = 0x0080,  //Invalid Message Received Interrupt
   CAN_INTERRUPT_WARN = 0x0100,     //Transmitter or Receiver in Error State, Warning bit - Flag only
   CAN_INTERRUPT_RXWARN = 0x0200,   //Receiver in Error State, Warning bit - Flag only
   CAN_INTERRUPT_TXWARN = 0x0400,   //Transmitter in Error State, Warning bit - Flag only
   CAN_INTERRUPT_RXPASS = 0x0800,   //Receive in Error State, Bus Passive bit - Flag only
   CAN_INTERRUPT_TXPASS = 0x1000,   //Transmitter in Error State, Bus Passive bit - Flag only
   CAN_INTERRUPT_TXOFF = 0x2000,    //Transmitter in Error State, Bus Off bit - Flag only
   CAN_INTERRUPT_RX1OVR = 0x4000,   //Receive Buffer 1 Overflowed bit - Flag only
   CAN_INTERRUPT_RX0OVR = 0x8000    //Receive Buffer 0 Overflowed bit - Flag only
} CAN_INTERRUPT;

//////////////////////////////// CAN Prototypes ///////////////////////////////

// Initialize the CAN peripheral.  After initializing the peripheral it will be
// put into 'Mode'.
void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL);

// Sets the CAN bit rate of the device.  'Clock' is the clock speed presented
// to the CAN peripheral, defined as CAN_CLOCK_SPEED  'Rate' is the bit rate
// to use.  The math requires that Clock be evenly divisible by Rate.
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate);

// Sets the operational mode of the CAN peripheral.  'Mode' is the operational
// mode to set.  'Update' specifies whether to update a global variable used to
// save the operation mode, which is used by some functions to return the
//operational mode back to it's previous operating mode.
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE);

// Sets up the specified Filter's Id, and Type.  'Filter' is the filter to
// setup (0-5), filters 0 and 1 are for RX Buffer 0 and filters 2 to 5 are for
// RX Buffer 1.  'Id' is the Id to assign to the filter and 'Type' is sets the
// type of messages filter accepts, either SID or EID, see CAN_FILTER_TYPE
// above for values to pass to function.
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type);

// Sets up the specified Filter Mask's Id, Id Type and mask type.  'Mask' is 
// the Filter Mask to setup (0-2), 'Id' is the mask to assign to the filter
// mask, 'iType' is the Id type of the filter mask, see CAN_MASK_ID_TYPE above
// for what it can be set to and 'fType' sets the filter mask type, see 
// CAN_FILTER_MASK_TYPE above for types it can be set to.
void can_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType);

// Used to load a message to transmit into a TX buffer.  'Header' is a pointer
// to CAN_TX_HEADER type with info to send message with, Id, Data Length, etc.,
// 'Data' is pointer to the payload to send with message.  The functions loads 
// the messages into the first available TX buffer.
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data);

// Used to retrieve a received message from a RX buffer.  'Header' is a pointer
// to a CAN_RX_HEADER type variable to save the received message's header info
// to, Id, Data length, etc., 'Data' is a pointer to an array to save the
// received messages's payload to.  The function checks/retrieves the message
// from the RX buffer 0.
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data);

// Used to determine if there is a least one message to retrieve from a RX
// buffer.  Returns TRUE if there is a message to retrieve and FALSE if there
// is no message to retrieve.
int1 can_kbhit(void);

// Used to determine if a least one of the TX Buffers is free to accept a new
// message to transmit.  Returns TRUE if there is a least one free buffer and
// FALSE if there are no free buffers.
int1 can_tbe(void);

// Used to determine if all the TX Buffers are free.  Returns TRUE if all
// buffers are free and FALSE is at least one buffer has a message to send.
int1 can_tx_is_empty(void);

// Used to abort all pending transmissions.
void can_abort(void);

// Used to enable one or more of the CAN interrupts.  'Setting' is the CAN
// interrupt to enable, multiple interrupts can be or'ed together to enable
// multiple CAN interrupts.
void can_enable_interrupts(CAN_INTERRUPT Setting);

// Used to disable one or more of the CAN interrupts.  'Setting' is the CAN
// interrupt to disable, multiple interrupts can be or'ed together to disable
// multiple CAN interrupts.
void can_disable_interrupts(CAN_INTERRUPT Setting);

// Used to determine which CAN interrupts are active.  Returns CAN_INTERRUPT 
// type, if an interrupt is active the corresponding bit will be set.  See
// CAN_INTERRUPT type to determine which bits corresponds to which interrupt.
CAN_INTERRUPT can_interrupts_active(void);

// Used to determine if specified CAN interrupt is active.  Returns TRUE if
// active, FALSE if not active.  'Interrupt' is the interrupt to test for.
int1 can_interrupt_active(CAN_INTERRUPT Interrupt);

// Used to clear specified CAN interrupt flag.  'Interrupt' is the interrupt
// to clear.
void can_clear_interrupt(CAN_INTERRUPT Interrupt);

/////////////////////////////// CAN2 Prototype ////////////////////////////////

#if getenv("SFR_VALID:C2CTRL") == FALSE
 #if defined(USE_CAN2_PERIPHERAL) && (USE_CAN2_PERIPHERAL == TRUE)
  #undef USE_CAN2_PERIPHERAL
 #endif
#endif
 
#ifndef USE_CAN2_PERIPHERAL
 #define USE_CAN2_PERIPHERAL     FALSE
#endif

#if USE_CAN2_PERIPHERAL == TRUE
 // Initialize the CAN2 peripheral.  After initializing the peripheral it will
 // be put into 'Mode'.
 void can2_init(CAN_OP_MODE Mode=CAN_OP_NORMAL);

 // Sets the CAN2 bit rate of the device.  'Clock' is the clock speed presented
 // to the CAN2 peripheral, defined as CAN2_CLOCK_SPEED.  'Rate' is the bit
 // rate to use.  The math requires that Clock be evenly divisible by Rate.
 can_ec_t can2_set_baud(uint32_t Clock, uint32_t Rate);

 // Sets the operational mode of the CAN2 peripheral.  'Mode' is the operational
 // mode to set.  'Update' specifies whether to update a global variable used to
 // save the operation mode, which is used by some functions to return the
 //operational mode back to it's previous operating mode.
 void can2_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE);

 // Sets up the specified Filter's Id, and Type.  'Filter' is the filter to
 // setup (0-5), filters 0 and 1 are for RX Buffer 0 and filters 2 to 5 are for
 // RX Buffer 1.  'Id' is the Id to assign to the filter and 'Type' is sets the
 // type of messages filter accepts, either SID or EID, see CAN_FILTER_TYPE
 // above for values to pass to function.
 void can2_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type);

// Sets up the specified Filter Mask's Id, Id Type and mask type.  'Mask' is 
// the Filter Mask to setup (0-2), 'Id' is the mask to assign to the filter
// mask, 'iType' is the Id type of the filter mask, see CAN_MASK_ID_TYPE above
// for what it can be set to and 'fType' sets the filter mask type, see 
// CAN_FILTER_MASK_TYPE above for types it can be set to.
void can2_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType);

 // Used to load a message to transmit into a TX buffer.  'Header' is a pointer
 // to CAN_TX_HEADER type with info to send message with, Id, Data Length, etc.,
 // 'Data' is pointer to the payload to send with message.  The functions loads 
 // the messages into the first available TX buffer.
 can_ec_t can2_putd(CAN_TX_HEADER *Header, uint8_t *Data);

 // Used to retrieve a received message from a RX buffer.  'Header' is a pointer
 // to a CAN_RX_HEADER type variable to save the received message's header info
 // to, Id, Data length, etc., 'Data' is a pointer to an array to save the
 // received messages's payload to.  The function checks/retrieves the message
 // from the RX buffer 0.
 can_ec_t can2_getd(CAN_RX_HEADER *Header, uint8_t *Data);

 // Used to determine if there is a least one message to retrieve from a RX
 // buffer.  Returns TRUE if there is a message to retrieve and FALSE if there
 // is no message to retrieve.
 int1 can2_kbhit(void);

 // Used to determine if a least one of the TX Buffers is free to accept a new
 // message to transmit.  Returns TRUE if there is a least one free buffer and
 // FALSE if there are no free buffers.
 int1 can2_tbe(void);

 // Used to determine if all the TX Buffers are free.  Returns TRUE if all
 // buffers are free and FALSE is at least one buffer has a message to send.
 int1 can2_tx_is_empty(void);

 // Used to abort all pending transmissions.
 void can2_abort(void);

 // Used to enable one or more of the CAN2 interrupts.  'Setting' is the CAN
 // interrupt to enable, multiple interrupts can be or'ed together to enable
 // multiple CAN2 interrupts.
 void can2_enable_interrupts(CAN_INTERRUPT Setting);

 // Used to disable one or more of the CAN2 interrupts.  'Setting' is the CAN
 // interrupt to disable, multiple interrupts can be or'ed together to disable
 // multiple CAN2 interrupts.
 void can2_disable_interrupts(CAN_INTERRUPT Setting);

 // Used to determine which CAN2 interrupts are active.  Returns CAN_INTERRUPT 
 // type, if an interrupt is active the corresponding bit will be set.  See
 // CAN_INTERRUPT type to determine which bits corresponds to which interrupt.
 CAN_INTERRUPT can2_interrupts_active(void);

 // Used to determine if specified CAN2 interrupt is active.  Returns TRUE if
 // active, FALSE if not active.  'Interrupt' is the interrupt to test for.
 int1 can2_interrupt_active(CAN_INTERRUPT Interrupt);

 // Used to clear specified CAN2 interrupt flag.  'Interrupt' is the interrupt
 // to clear.
 void can2_clear_interrupt(CAN_INTERRUPT Interrupt);
 
#endif

#endif   //__CAN_DSPIC30F_H__
