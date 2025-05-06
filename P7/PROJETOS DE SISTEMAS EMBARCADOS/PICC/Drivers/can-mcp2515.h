///////////////////////////////////////////////////////////////////////////////
////                             can-mcp2515.h                             ////
////                                                                       ////
////  Driver for Microchip's MCP2515 external CAN controller with SPI      ////
////  interface.                                                           ////
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
   
   MCP2515 PREPROCESSORS
   ----------------------------------------------------------------------------
   
   MCP2515_EXT_CLOCK_SPEED
       Specifies the speed of the external crystal/oscillator connected to the 
       MCP2515.  Can be 1 MHz to 40 MHz, default is 20000000 (20 MHz).
   
   MCP2515_CLOCK_OUT_DIVISOR
      Sets the output clock divisor, 1, 2, 4 or 8.  Default is 1.
   
   MCP2515 SPI PREPROCESSORS
   ----------------------------------------------------------------------------
   
   MCP2515Xfer
      Optional used to specify the SPI XFER function to use for communicating
      with the MCP2515 device.  Not defined by default.
   
   MCP2515_SPI_STREAM
      Optional used to specify the #use spi() stream to use for communication
      with the MCP2515 device.  If not defined before this driver is
      included, it will be defined to the #use spi() stream added by this
      driver.
   
   MCP2515_SPI_SCK_PIN
      Used to specify the SPI clock pin.  Only need if both MCP2515Xfer and
      MCP2515_SPI_STREAM are not defined before this driver is included.  In
      that case the default is PIN_E5 for PCD compiler and PIN_C3 for PCB, PCM
      and PCH compilers.
   
   MCP2515_SPI_SDI_PIN
      Used to specify the SPI slave DI pin.  Only need if both MCP2515Xfer and
      MCP2515_SPI_STREAM are not defined before this driver is included.  In
      that case the default is PIN_B2 for PCD compiler and PIN_C1 for PCB, PCM
      and PCH compilers.
   
   MCP2515_SPI_SDO_PIN
      Used to specify the SPI slave DO pin.  Only need if both MCP2515Xfer and
      MCP2515_SPI_STREAM are not defined before this driver is included.  In
      that case the default is PIN_B4 for PCD compiler and PIN_C0 for PCB, PCM
      and PCH compilers.
   
   MCP2515_SPI_CS_PIN
      Used to specify the SPI Chip select pin, the default is PIN_B1.
   
   MCP2515_SPI_BAUD
      Used to specify the SPI baud rate to used. Only need if both MCP2515Xfer
      and MCP2515_SPI_STREAM are not defined before this driver is included.
      In that case the default value is 10MHz, max supported by MCP2515.
   
   MCP2515_CLKO_TYPE
      Sets what is outputted on the CLKOUT pin, 0 - Clock, 1 - SOF signal.  The
      defaults is 0, Clock.
   
   MCP2515_CLKO_ENABLED
      Used to enable/disable the CLKOUT pin, TRUE - enabled, FALSE - disabled.
      The default is TRUE, enabled.
      
   MCP2515_INT_PIN
      Optional used to specify the PIC pin connected to the INT pin of the
      MCP2515.  
   
   MCP2515_RX0BF_PIN
      Optional used to specify the PIC pin connected to the RX0BF pin of the
      MCP2515.  Additionally if defined the driver will setup this pin to be
      used as an interrupt when a valid message is loaded into RXB0.
   
   MCP2515_RX0BF_PIN
      Optional used to specify the PIC pin connected to the RX1BF pin of the
      MCP2515.  Additionally if defined the driver will setup this pin to be
      used as an interrupt when a valid message is loaded into RXB1.
   
   MCP2515_TX0RTS_PIN
      Optional used to specify the PIC pin connected to the TX0RTS pin of the 
      MCP2515, which can be pulled low to request sending the message in TXB0.
      Additionally is defined the driver will setup this pin to be used to 
      request message transmission of TXB0, if not the pin can be used as a 
      digital input.
   
   MCP2515_TX1RTS_PIN
      Optional used to specify the PIC pin connected to the TX1RTS pin of the 
      MCP2515, which can be pulled low to request sending the message in TXB1.
      Additionally is defined the driver will setup this pin to be used to 
      request message transmission of TXB1, if not the pin can be used as a 
      digital input.
   
   MCP2515_TX2RTS_PIN
      Optional used to specify the PIC pin connected to the TX2RTS pin of the 
      MCP2515, which can be pulled low to request sending the message in TXB2.
      Additionally is defined the driver will setup this pin to be used to 
      request message transmission of TXB2, if not the pin can be used as a 
      digital input.
   
   MCP2515_RX0_OUTPUT
      If MCP2515_RX0BF_PIN is not defined, used to enable/disable if the RX0BF
      pin of the MCP2515 is used as an output.  Default is FALSE, disabled, 
      both MCP2515_RX0BF_PIN and MCP2515_RX0_OUTPUT are not defined.
   
   MCP2515_RX0_DEFAULT_LEVEL
      If MCP2515_RX0_OUTPUT is defined as TRUE, used to set the default level
      of the RX0BF pin.  Default is 0, low, if MCP2515_RX0_OUTPUT is defined as
      TRUE and MCP2515_RX0_DEFAULT_LEVEL is not defined.
   
   MCP2515_RX1_OUTPUT
      If MCP2515_RX1BF_PIN is not defined, used to enable/disable if the RX1BF
      pin of the MCP2515 is used as an output.  Default is FALSE, disabled, 
      both MCP2515_RX1BF_PIN and MCP2515_RX1_OUTPUT are not defined.
   
   MCP2515_RX1_DEFAULT_LEVEL
      If MCP2515_RX1_OUTPUT is defined as TRUE, used to set the default level
      of the RX1BF pin.  Default is 0, low, if MCP2515_RX1_OUTPUT is defined as
      TRUE and MCP2515_RX1_DEFAULT_LEVEL is not defined.
   
   OPTIONAL CAN PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_BRG_SAM
      Sets how many time the CAN bus is sampled at the sample point, 0-once, 
      1-three times.  Default is 0.
   
   CAN_BRG_WAKE_FILTER
      Sets if CAN bus line filter is used for wake-up, 0-not used, 1-used.
      Default is 0.
   
   CAN_TX_RETRANSMISSION_ATTEMPTS
      The number of retransmission attempts for a TX Buffer.  Can be either
      RETRANSMIT_ATTEMPTS_DISABLED or RETRANSMIT_ATTEMPTS_UNLIMITED, default is
      RETRANSMIT_ATTEMPTS_UNLIMITED.
   
   CAN_USE_RX_DOUBLE_BUFFER
      Used to enable/disable loading RXB0 messages into Buffer 1 when Buffer 0
      would overflow.  Default is TRUE, enabled.
*/

#ifndef __CAN_MCP2515_H__
#define __CAN_MCP2515_H__

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
   int1 err_ovfl;             //TRUE - Buffer overflowed, FALSE - Buffer didn't overflow
   int1 ext;                  //TRUE - received extended ID message, FALSE - received standard ID message
   int1 rtr;                  //TRUE - received RTR (request), FALSE - not a RTR (request)
} CAN_RX_HEADER;

// CAN_OP_MODE type
typedef enum 
{
   CAN_OP_NORMAL,             //Normal operation mode
   CAN_OP_SLEEP,              //Sleep mode
   CAN_OP_LOOPBACK,           //Loopback mode
   CAN_OP_LISTEN,             //Listen only mode
   CAN_OP_CONFIG,             //Configuration mode
} CAN_OP_MODE;


// CAN Retransmit Attempts type
typedef enum
{
   RETRANSMIT_ATTEMPTS_UNLIMITED,      //messages will reattempt transmission if required
   RETRANSMIT_ATTEMPTS_DISABLED        //One-Shot Mode, message will only attempt to transmit one time
} RETRANSMIT_ATTEMPTS;

// CAN Filter type
typedef enum
{
   CAN_FILTER_0,     //Filter 0, assigned to Buffer 0
   CAN_FILTER_1,     //Filter 1, assigned to Buffer 0
   CAN_FILTER_2,     //Filter 2, assigned to Buffer 1
   CAN_FILTER_3,     //Filter 3, assigned to Buffer 1
   CAN_FILTER_4,     //Filter 4, assigned to Buffer 1
   CAN_FILTER_5      //Filter 5, assigned to Buffer 1
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
   CAN_FILTER_TYPE_SID,  //match only SID messages
   CAN_FILTER_TYPE_EID,  //match only EID messages
} CAN_FILTER_TYPE;

// CAN Mask Type type
typedef enum
{
   CAN_MASK_TYPE_SID,   //only SID bits of mask are written, EID bits are set to 0
   CAN_MASK_TYPE_EID    //both SID and EID bits of mask are written
} CAN_MASK_TYPE;

// CAN Interrupt type
typedef enum
{
   CAN_INTERRUPT_RX0 = 0x01,     //Receive Buffer 0 Full Interrupt
   CAN_INTERRUPT_RX1 = 0x02,     //Receive Buffer 1 Full Interrupt
   CAN_INTERRUPT_TX0 = 0x04,     //Transmit Buffer 0 Empty Interrupt
   CAN_INTERRUPT_TX1 = 0x08,     //Transmit Buffer 1 Empty Interrupt
   CAN_INTERRUPT_TX2 = 0x10,     //Transmit Buffer 2 Empty Interrupt
   CAN_INTERRUPT_ERR = 0x20,     //Error Interrupt
   CAN_INTERRUPT_WAKE = 0x40,    //Wake-up Interrupt
   CAN_INTERRUPT_INVALID = 0x80  //Message Error Interrupt
} CAN_INTERRUPT;
   

///////////////////////////////////////////////////////////////////////////////

// Initialize the CAN peripheral.  After initializing the peripheral it will be
// put into 'Mode'.
void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL);

// Sets the CAN bit rate of the device.  'Clock' is the clock speed of the
// external crystal/oscillator connected to the MCP2515.  'Rate' is the bit
// rate to use.  The math requires that Clock be evenly divisible by Rate.
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

// Sets up the specified Mask's Id and Type.  'Mask' is the mask to setup 
// (0-1), mask 0 is for filter 0 and 1 and mask 1 is for filter 2 to 5.  'Id'
// is the mask value to assign to the mask. 'Type' used to specify if the mask
// is for SID or EID messages, controls how the SID and EID bits are set for the
// mask type, see CAN_MASK_TYPE above for types it can be set to.
void can_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_TYPE Type);

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


#endif   //__CAN_MCP2515_H__
