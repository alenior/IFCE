///////////////////////////////////////////////////////////////////////////////
////                           can-pic18f_ecan.h                           ////
////                                                                       ////
////  Driver for ECAN peripheral for Microchip's PIC18 devices with an     ////
////  ECAN peripheral.                                                     ////
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

#ifndef __CAN_PIC18F_ECAN_H__
#define __CAN_PIC18F_ECAN_H__

/*
   By default this driver sets up the ECAN peripheral for Enhanced FIFO mode 
   (Mode 2) and uses the 3 dedicated transmit buffers for transmitting CAN
   messages and the 2 dedicated receive buffer and 6 programmable RX/TX buffers
   as the receive FIFO to receive CAN messages.
   
   CAN PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_TX_BUFFERS
      The number of buffers used for transmitting CAN messages 3-9.  Default
      size is 3, the dedicated transmit buffers.  If defined to a value larger
      then 3 it starts making the programmable RX/TX buffer 5 (B5) a TX buffer
      and works its way down towards buffer B0.  This is so the receive FIFO 
      buffer is as large as possible.  The three dedicated TX buffers can not
      be used to automatically respond to remote transmission request (RTR)
      messages, so if that feature is desired CAN_TX_BUFFERS needs to be 
      defined to at least 4.
   
   CAN_USE_RX_FIFO
      Used to set whether the receive buffers are setup as a FIFO buffer or
      individual buffers, which can have their own acceptance filters assigned
      to them.  Default is TRUE, use FIFO buffer.
   
   CAN_FIFO_INT
      Used to set when the FIFO interrupt will occur, 0 - FIFO interrupt will 
      occur when four receive buffers remain, 1 - FIFO will interrupt when one
      receive buffer remains.  Default is 1, interrupt when one receive buffer
      remains.
   
   CAN_BAUD_RATE
      The bit rate that the can_init() function will set the ECAN peripheral
      to.  Default is 125 kbps.
   
   CAN_DEFAULT_SAMPLE_POINT
      The sample point used by the can_set_baud() function when setting the
      ECAN bit rate.  Default is 875 for 87.5% of the bit time, the sample
      point the CANopen protocol uses.
   
   OPTIONAL CAN FILTER PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_USE_FILTERS
      Used to enable can_init() to setup CAN filters, FALSE - disabled or 
      TRUE - enabled.  Default is disabled, can_init() sets up ECAN peripheral
      to receive all messages.
   
   CAN_USER_FILT_x
      Used to set the Id of the filter, x being the filter (0-15) to assign
      the Id to.
   
   CAN_USER_FILT_x_TYPE
      Used to set the filter type, x being the filter (0-15) to assign the type
      to.  The type can be one of the following:
         CAN_FILTER_TYPE_SID - SID Filter Id
         CAN_FILTER_TYPE_EID - EID Filter Id
   
   CAN_USER_FILT_x_BUFFER
      Used to set the buffer the filter is assigned to, x being the filter
      (0-15) to assign to object.  See CAN_FILTER_BUFFER type for buffers that 
      filter can be assigned to.
   
   CAN_USER_FILT_x_MASK
      Use to set the mask used with filter, x being the filter (0-15).  See
      CAN_FILTER_MASK type for masks that filter can use.
   
   CAN_USER_MASK_y
      Used to set the Id of the mask, y being the mask (0-2) to assign the Id
      to.
   
   CAN_USER_MASK_y_ID_TYPE
      Used to set the mask type, y being the mask (0-2), to assign the type
      to.  The type can be one of the following:
         CAN_MASK_ID_TYPE_SID - SID mask Id
         CAN_MASK_ID_TYPE_EID - EID mask Id
   
   CAN_USER_MASK_y_FILTER_TYPE
      Used to set if filter accepts both SID and EID messages, or only SID or
      EID messages.  The type can be one of the following:
         CAN_FILTER_MASK_TYPE_EITHER - match both SID and EID messages
         CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, 
                                           determined by CAN_USER_FILT_x_TYPE
   
   OPTIONAL CAN PERIPHERAL PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_BRG_SAM
      Sets how many time the CAN bus is sampled at the sample point, 0-once, 
      1-three times.  Default is 0.
   
   CAN_BRG_WAKEUP
      Enabled/disables CAN Bus activity wake-up feature, TRUE-enable feature,
      FALSE-disable feature.  Default is FALSE, disabled.
   
   CAN_BRG_WAKE_FILTER
      Sets if CAN bus line filter is used for wake-up, FALSE-not used, 
      TRUE-used.  Default is FALSE, not used.
   
   CAN_ENABLE_DRIVE_HIGH
      Sets if CANTX pin will be driven to VDD when recessive, or will be 
      tri-state when recessive.  TRUE-drive pin, FALSE-tri-state pin.  Default
      is FALSE, tri-state pin.
   
   CAN_RX_PIN
      Only used on devices with remappable peripheral pins, PIC18FxxK83
      devices, this sets the PIC pin to use as the CAN RX pin.  Additionally the
      driver uses this define to assign the pin as the peripheral's RX pin with
      the #pin_select directive.  Default is PIN_B3.
   
   CAN_TX_PIN
      Only used on devices with remappable peripheral pins, PIC18FxxK83
      devices, this sets the PIC pin to use as the CAN TX pin.  Additionally the
      driver uses this define to assign the pin as the peripheral's TX pin with
      the #pin_select directive.  Default is PIN_B2.
   
   CAN_ENABLE_CAN_CAPTURE
      Enables/disables input capture based on CAN message receive, 
      FALSE-disable, TRUE-enable.  Default is FALSE, disabled.
   
   CAN_ENABLE_CANTX2
      Used to enable/disable the CANTX2 pin.  TRUE-enable CANTX2 pin, 
      FALSE-disable CANTX2 pin, pin will have digital I/O function.  Default is
      FALSE, disabled.
   
   CAN_CANTX2_SOURCE
      When CAN_ENABLE_CANTX2 is defined to TRUE used to select what is 
      outputted on the CANTX2 pin.  0-CANTX2 pin will output inverse of CANTX
      pin, 1-CANTX2 pin will output the CAN clock.  Default is 0, inverse of 
      CANTX pin.
   
*/

#include <stdint.h>

////////////////////////////////// CAN Types //////////////////////////////////

// CAN Error Codes used by this driver
// 0 is OK, non-zero is some kind of error
typedef enum 
{
   CAN_EC_OK = 0,
   CAN_EC_BUFFER_RX_EMPTY,    //can_getd failed because all RX buffers are empty, or specified buffer is empty
   CAN_EC_BUFFER_NOT_RX,      //can_getd failed because buffer is not a RX buffer
   CAN_EC_BUFFER_TX_FULL,     //can_putd failed because all TX buffers are full, or specified buffer is full
   CAN_EC_BUFFER_NOT_TX,      //can_putd failed because buffer is not a TX buffer
   CAN_EC_BUFFER_IS_RTR,      //can_putd failed because buffer is setup as an RTR TX Buffer
   CAN_EC_BUFFER_NO_RTR,      //CAN Buffer can not be set to respond to RTR requests
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
   uint8_t Priority:2;        //0-3, use to determine which messages will be sent fist.  The high the priority the sooner the ECAN peripheral will send the message
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
   CAN_OP_NORMAL,             //Normal operation mode
   CAN_OP_DISABLE,            //Disable mode
   CAN_OP_LOOPBACK,           //Loopback mode
   CAN_OP_LISTEN,             //Listen only mode
   CAN_OP_CONFIG,             //Configuration mode
} CAN_OP_MODE;

// CAN_FILTER type
typedef enum
{
   CAN_FILTER_0, CAN_FILTER_1, CAN_FILTER_2, CAN_FILTER_3,
   CAN_FILTER_4, CAN_FILTER_5, CAN_FILTER_6, CAN_FILTER_7,
   CAN_FILTER_8, CAN_FILTER_9, CAN_FILTER_10, CAN_FILTER_11,
   CAN_FILTER_12, CAN_FILTER_13, CAN_FILTER_14, CAN_FILTER_15
} CAN_FILTER;

// CAN_FILTER_TYPE type
typedef enum {
   CAN_FILTER_TYPE_SID,    //Filter Id is a SID Id - only SID bits of filter are written, EID bits are set to 0
   CAN_FILTER_TYPE_EID     //Filter Id is an EID Id - SID and EID bits of Mask are written
} CAN_FILTER_TYPE;

// CAN_FILTER_BUFFER type
typedef enum
{
   CAN_FILTER_BUFFER_RXB0,    //Filter assigned to dedicated RX Buffer 0
   CAN_FILTER_BUFFER_RXB1,    //Filter assigned to dedicated RX Buffer 1
   CAN_FILTER_BUFFER_B0,      //Filter assigned to programmable RX/TX Buffer 0
   CAN_FILTER_BUFFER_B1,      //Filter assigned to programmable RX/TX Buffer 1
   CAN_FILTER_BUFFER_B2,      //Filter assigned to programmable RX/TX Buffer 2
   CAN_FILTER_BUFFER_B3,      //Filter assigned to programmable RX/TX Buffer 3
   CAN_FILTER_BUFFER_B4,      //Filter assigned to programmable RX/TX Buffer 4
   CAN_FILTER_BUFFER_B5,      //Filter assigned to programmable RX/TX Buffer 5
   CAN_FILTER_BUFFER_ALL=15,  //Filter assigned to all Receive Buffers
   CAN_FILTER_BUFFER_FIFO=15  //Filter assigned to FIFO Receive Buffer (Enhanced FIFO Mode only, Mode 2)
} CAN_FILTER_BUFFER;

// CAN_FILTER_MASK type
typedef enum
{
   CAN_FILTER_MASK_0,   //Mask 0
   CAN_FILTER_MASK_1,   //Mask 1
   CAN_FILTER_MASK_2,   //Filter 15 is used as the mask
   CAN_FILTER_MASK_NONE
} CAN_FILTER_MASK;

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

// CAN_BUFFER type
typedef enum
{
   CAN_BUFFER_RXB0,  //dedicated Receive Buffer 0
   CAN_BUFFER_RXB1,  //dedicated Receive Buffer 1
   CAN_BUFFER_TXB0,  //dedicated Transmit Buffer 0
   CAN_BUFFER_TXB1,  //dedicated Transmit Buffer 1
   CAN_BUFFER_TXB2,  //dedicated Transmit Buffer 2
   CAN_BUFFER_B0,    //programmable RX/TX Buffer 0
   CAN_BUFFER_B1,    //programmable RX/TX Buffer 1
   CAN_BUFFER_B2,    //programmable RX/TX Buffer 2
   CAN_BUFFER_B3,    //programmable RX/TX Buffer 3
   CAN_BUFFER_B4,    //programmable RX/TX Buffer 4
   CAN_BUFFER_B5     //programmable RX/TX Buffer 5
} CAN_BUFFER;

//CAN_INTERRUPT type
typedef enum
{
   CAN_INTERRUPT_RXB0=1,      //dedicated RX Buffer 0 
   CAN_INTERRUPT_RXB1=2,      //dedicated RX Buffer 1
   CAN_INTERRUPT_B0=4,        //programmable RX/TX Buffer 0
   CAN_INTERRUPT_B1=8,        //programmable RX/TX Buffer 1
   CAN_INTERRUPT_B2=16,       //programmable RX/TX Buffer 2
   CAN_INTERRUPT_B3=32,       //programmable RX/TX Buffer 3
   CAN_INTERRUPT_B4=64,       //programmable RX/TX Buffer 4
   CAN_INTERRUPT_B5=128,      //programmable RX/TX Buffer 5
   CAN_INTERRUPT_TXB0=1024,   //dedicated TX Buffer 0
   CAN_INTERRUPT_TXB1=2048,   //dedicated TX Buffer 1
   CAN_INTERRUPT_TXB2=4096    //dedicated TX Buffer 2
} CAN_INTERRUPT;   

#define CAN_TX_BUFFER_ANY     0xFF
#define CAN_TX_BUFFER_ALL     0xFF
#define CAN_RX_BUFFER_ANY     0xFF
#define CAN_ABORT_ALL         0xFF

//////////////////////////////// CAN Prototypes ///////////////////////////////
   
// Initialize the CAN peripheral.  After initializing the peripheral it will be
// put into 'Mode'.
void can_init(CAN_OP_MODE Mode = CAN_OP_NORMAL);

// Sets the CAN bit rate of the device.  'Clock' is the CAN clock speed that is
// presented to the ECAN peripheral.  'Rate' is the bit rate to use.  The math
// requires that clock be evenly divisible by the bit rate.
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate);

// Sets the operational mode of the ECAN peripheral.  'Mode' is the operational
// mode to set.  'Update' specifies whether to update a global variable used to
// save the operation mode, which is used by some functions to return the
// operational mode back to it's previous operating mode.
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE);

// Sets up the specified Filter's Id, and Type.  'Filter' is the filter to
// setup (0-15), 'Id' is the Id to assign to the filter and 'Type' is the Id
// type of the filter, see CAN_FILTER_TYPE above for type it can be set to.
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type);

// Sets up the specified Filter Mask's Id, Id Type and mask type.  'Mask' is 
// the Filter Mask to setup (0-2), 'Id' is the mask to assign to the filter
// mask, 'iType' is the Id type of the filter mask, see CAN_MASK_ID_TYPE above
// for what it can be set to and 'fType' sets the filter mask type, see 
// CAN_FILTER_MASK_TYPE above for types it can be set to.
void can_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType);

// Used to enable specified filter, assigned it a specific buffer and set which
// mask to use with the filter.  'Filter' is the filter to enabled (0-15), 
// 'Buffer' is buffer to load matched messages to, see CAN_FILTER_BUFFER type 
// above. and 'Mask' is the filer mask to use with filter (0-2).
void can_enable_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer, CAN_FILTER_MASK Mask);

// Used to disable a specified filter.  'Filter is the filter to disable 
// (0-15).
void can_disable_filter(CAN_FILTER Filter);

// Used to load a message to transmit into a TX buffer.  'Header' is a pointer
// to CAN_TX_HEADER type with info to send message with, Id, Data Length, etc.,
// 'Data' is pointer to the payload to send with message, and 'Buffer' is an 
// optional parameter specifying the TX buffer to load the message into.  By 
// default it loads the messages into the first available TX buffer.
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

// Used to retrieve a received message from a RX buffer.  'Header' is a pointer
// to a CAN_RX_HEADER type variable to save the received message's header info
// to, Id, Data length, etc., 'Data' is a pointer to an array to save the
// received messages's payload to, and 'Buffer' is an optional parameter
// specifying the RX buffer to retrieve message from, only used when set for 
// Enhanced Legacy mode (Mode 1), CAN_USE_RX_FIFO is defined to FALSE.  When
// set for Mode 1 by default it retrieves the messages from the first buffer
// that has a new message to retrieve.
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer=CAN_RX_BUFFER_ANY);

// Used to determine if there is a least one message to retrieve from a RX
// buffer.  Returns TRUE if there is a message to retrieve and FALSE if there
// is no message to retrieve.  'Buffer' is an optional parameter specifying the
// RX buffer to check, only used when set for Enhanced Legacy mode (Mode 1), 
// CAN_USE_RX_FIFO is defined to FALSE.  When set for Mode 1 by default it
// checks all the receive buffers to see if they have a new message to
// retrieve.
int1 can_kbhit(CAN_BUFFER Buffer = CAN_RX_BUFFER_ANY);

// Used to determine if the specified TX buffer is free to accept a new message
// to transmit.  Returns TRUE if buffer is free and FALSE if not.  'Buffer' is
// an optional parameter for specifying the TX buffer to check.  Default is
// CAN_TX_BUFFER_ANY, which causes to check all buffers and returns TRUE if at
// least one TX buffer is free.
int1 can_tbe(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

// Used to determine if the specified TX buffer is empty, has no message to
// send.  Returns TRUE if TX buffer is empty, FALSE if it has a message to 
// send.  'Buffer' is an optional parameter for specifying the TX buffer to
// check.  Default is CAN_TX_BUFFER_ANY, which means all TX buffers must be
// empty for it to return TRUE.  If value other then CAN_TX_BUFFER_ANY is used
// then this function is equivalent to can_tbe().
int1 can_tx_empty(CAN_BUFFER Buffer = CAN_TX_BUFFER_ALL);

// Used to abort pending transmissions for the specified buffer.  'Buffer' is
// an optional parameter specifying the TX Buffer to abort transmission for.  
// Default is CAN_ABORT_ALL which causes it to abort all pending transmissions
// for all TX buffers.
void can_abort(CAN_BUFFER Buffer = CAN_ABORT_ALL);

// Used to enable RTR messages on a TX buffer.  'Buffer' is the buffer to 
// enable RTR messages on.  Only programmable RX/TX buffers set as a TX buffer
// can be used automatically respond to remote transmission request (RTR)
// messages.
can_ec_t can_enable_rtr(CAN_BUFFER Buffer);

// Used to disable RTR messages on a TX buffer.  'Buffer' is the buffer to
// disable RTR messages on.
can_ec_t can_disable_rtr(CAN_BUFFER Buffer);

// Used to load a message into a TX buffer that will be transmitted when a
// remote transmission request is received.  'Header' is a pointer to a
// CAN_TX_HEADER structure containing info about the message to transmit, Id,
// Data length, etc., 'Data' is a pointer to an array contain payload of
// message to transmit and 'Buffer' is the buffer to load the message into.
can_ec_t can_load_rtr(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer);

// Used to enable one or more of the CAN interrupts.  'Setting' is the CAN
// interrupt to enable, multiple interrupts can be or'ed together to enable
// multiple CAN interrupts.
void can_enable_interrupts(CAN_INTERRUPT Setting);

// Used to disable one or more of the CAN interrupts.  'Setting' is the CAN
// interrupt to disable, multiple interrupts can be or'ed together to disable
// multiple CAN interrupts.
void can_disable_interrupts(CAN_INTERRUPT Setting);


#endif //__CAN_PIC18F_ECAN_H__
