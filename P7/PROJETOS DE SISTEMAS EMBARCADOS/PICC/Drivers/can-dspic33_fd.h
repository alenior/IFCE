///////////////////////////////////////////////////////////////////////////////
////                           can-dspic33_fd.h                            ////
////                                                                       ////
////  Driver for CAN FD peripheral for Microchip's dsPIC33CH and dsPIC33CK ////
////  devices with a CAN FD peripheral.                                    ////
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
   By default the TXQ Object is used for transmitting messages and the FIFO1
   object is used for receiving messages.
   
   PREPROCESSORS:
   ----------------------------------------------------------------------------
   
   CAN_OPERATION_MODE
      The CAN operational mode of peripheral after can_init() is called,
      0 - CAN FD mode or 1 - CAN 2.0 mode.  Defaults to CAN FD mode.
   
   CAN_TX_BUFFERS
      The number of buffers the TXQ object has for storing messages to transmit
      0-32, 0 disables it.  Default size is 1.
   
   CAN_TX_MAX_PAYLOAD_SIZE
      The maximum payload size of the TXQ object buffers, see PAYLOAD_SIZE type
      for available sizes.  Defaults to 8 bytes if CAN_OPERATION_MODE is set for
      CAN 2.0 mode, or 64 bytes if CAN_OPERATION_MODE is set for CAN FD mode.
   
   CAN_TX_PRIORITY
      The transmit priority of the TXQ object, 0-31 lowest to highest.
      Default priority is 31.
   
   CAN_TX_RETRANSMISSION_ATTEMPTS
      The number of retransmission attempts for the TXQ object, see 
      RETRANSMIT_ATTEMPTS type for available settings.  Default is unlimited
      retransmission attempts.

   CAN_RX_BUFFERS
      The number of buffers the FIFO1 object has for storing received messages
      0-32, 0 disables it.  Default size if 32.
   
   CAN_RX_MAX_PAYLOAD_SIZE
      The maximum payload size of the FIFO1 object buffers, see PAYLOAD_SIZE
      type for available sizes.  Defaults to 8 bytes if CAN_OPERATION_MODE is 
      set for CAN 2.0 mode, or 64 bytes if CAN_OPERATION_MODE is set for CAN FD
      mode.
   
   CAN_RX_TIME_STAMP
      Enables or disables capturing time stamps for received messages for the
      FIFO1 object, TRUE enabled or FALSE disabled.  Default is disabled.
   
   CAN_CLOCK_SPEED
      The clock speed that will be presented to the CAN FD peripheral, this
      speed is used in the can_init() function for setting the CAN FD
      peripheral's nominal and data bit rates.  Default is 80 Mbps, user is
      responsible for setting up the PIC's clock settings to achieve this
      speed.
   
   CAN_NOMINAL_BAUD_RATE
      The nominal, arbitration, bit rate that the can_init() function will set
      the CAN FD peripheral to. Default is 250 kbps.
   
   CAN_DATA_BAUD_RATE
      The data bit rate that the can_init() function will set the CAN FD
      peripheral to.  Default is 2 Mbps, if defined to 0 the data bit rate will
      be set the same as the nominal bit rate.
   
   CAN_DEFAULT_SAMPLE_POINT
      The sample point used by the can_set_baud() function for both the nominal
      and data bit rates.  Default is 875 for 87.5% of the bit time, the sample
      point the CANopen protocol uses.
   
   CAN_CLOCK_SOURCE
      The clock source to use for the CAN FD peripheral, 0 - CAN Clock
      Generator or 1 - Auxiliary Clock.  Default is the Auxiliary clock.
      Regardless of clock source selected the max clock speed that can be
      presented to the CAN FD peripheral is 80 MHz. 
      
   CAN_CLOCK_GEN_SOURCE
      If CAN_CLOCK_SOURCE is set for CAN Clock Generator this sets the CAN
      clock generator source, can be one of the following:
         1 - FVCO
         2 - FPLLO
         3 - FVCO/2
         4 - FVCO/3
         5 - FVCO/4
         6 - AFPLLO
         7 - AFVCO
         8 - AFVCO/2
         9 - AFVCO/3
         10 - AFVCO/4
      Default is FVCO, regardless of CAN clock generator source selected the
      max speed that can be presented to the CAN clock generator is 640 MHz.
      #use delay() has options for specifying the FVCO frequency for both the
      Primary and Auxiliary clocks, see #use delay() help file entry.
   
   CAN_CLOCK_GEN_DIVISOR
      If CAN_CLOCK_SOURCE is set for CAN Clock Generator this sets the CAN
      clock generator divisor 1-128.   Defaults is 1, used to divide the CAN
      clock generator input clock down to the clock that will be presented to
      the CAN FD peripheral.  The max speed that can be presented to the CAN FD
      peripheral is 80 MHz.
   
   CAN_TIME_BASE_COUNTER_ENABLE
      Used to enable the counter used for time-stamps, FALSE - disable or
      TRUE - enable.  Default is disabled, only needs enabled if time-stamps
      will be used.
   
   CAN_TIME_BASE_COUNTER_PRESCALAR
      Used to set the time base counter divisor 1-1024.  How fast the time base
      counter ticks compared to the CAN Clock, for example for a CAN Clock of
      64MHz and a divisor of 64, the time base counter will increment once for
      every 64 CAN Clock ticks - 1/(64MHz / 64) = 1us rate.  Default is 1.
   
   CAN_TIME_STAMP_EOF
      Used to set when the time-stamp occurs,  0 - time-stamp at beginning of
      frame, 1 - time-stamp when frame is taken valid, end of frame.  Default is
      time-stamp at beginning of frame.
   
   CAN_TIME_STAMP_RES
      Used to set time-stamp reset bit (CAN FD frames only), 0 - at sample
      point of Start-of-Frame (SOF), 1 - at sample point of the bit following
      the FDF bit. Default is at sample point of Start-of-Frame (SOF).

   OPTIONAL OBJECT PREPROCESSORS:
   ----------------------------------------------------------------------------
   
   CAN_USE_FIFO_x_OBJECT
      Used to enable FIFO2 - FIFO7, or FIFO1 if CAN_RX_BUFFERS is defined to 0.
      x being the FIFO to enable (1-7).
   
   CAN_FIFO_x_OBJECT_TYPE
      Used to set the FIFO type, 0 - RX object or 1 - TX object.  x being the
      FIFO to setup (1-7).
   
   CAN_FIFO_x_OBJECT_COUNT
      Used to set the FIFO buffer size (1-32).  x being the FIFO to setup (1-7).
   
   CAN_FIFO_x_OBJECT_MAX_PAYLOAD_SIZE
      Used to set the FIFO max payload size, see PAYLOAD_SIZE type for sizes.
      x being the FIFO to setup (1-7).
   
   CAN_FIFO_x_OBJECT_TX_PRIORITY
      If CAN_FIFO_x_OBJECT_TYPE is set for TX object, used to set the transmit 
      priority of the object (0-31).  x being the FIFO to setup (1-7).
   
   CAN_FIFO_x_OBJECT_TX_RETRANSMISSION_ATTEMPTS
      If CAN_FIFO_x_OBJECT_TYPE is set for TX object, used to set the
      retransmission attempts of the object, see RETRANSMIT_ATTEMPTS type for
      available settings.  x being the FIFO to setup (1-7).
   
   CAN_FIFO_x_OBJECT_RX_TIME_STAMP
      If CAN_FIFO_x_OBJECT_TYPE is set for RX object, used to enable/disable
      capturing time stamps of received messages.  FALSE - disabled or TRUE - 
      enabled.  x being the FIFO to setup (1-7).
   
   OPTIONAL FILTER PREPROCESSORS:
   ----------------------------------------------------------------------------
   
   CAN_USE_FILTERS
      Used to enable can_init() to setup CAN Object filters, FALSE - disabled
      or TRUE - enabled.  Default is disabled, can_init() sets up CAN FD
      peripheral to receive all messages.
   
   CAN_USER_FILT_x
      Used to set the Id of the filter, x being the filter (0-15) to assign
      the Id to.
   
   CAN_USER_MASK_x
      Used to set the Mask of the filter, x being the filter (0-15) to assign
      the Mask to.
   
   CAN_USER_FILT_x_TYPE
      Used to set the filter type, x being the filter (0-15) to assign the type
      to.  The type can be one of the following:
         CAN_FILTER_TYPE_SID_ONLY -  match only SID messages
         CAN_FILTER_TYPE_EID_ONLY - match only EID messages
         CAN_FILTER_TYPE_EITHER - match either SID or EID messages
   
   CAN_USER_FILT_x_OBJECT
      Used to set the object the filter is assigned to, x being the filter
      (0-15) to assign to object.  See CAN_OBJECT for object that filter can be
      assigned to.
   
   OPTIONAL CAN PERIPHERAL PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_STOP_IN_IDLE
      Sets whether the CAN FD peripheral stops when PIC is put in idle mode.
      Default is TRUE, peripheral stop in idle mode.
   
   CAN_TDC_VALUE
      Sets the Transmitter Delay Compensation Value, 0-63.  Default is 0.
   
   CAN_TDC_OFFSET
      Sets the Transmitter Delay Compensation Offset, -64-63.  Default is 0.
   
   CAN_TDC_MODE
      Sets the Transmitter Delay Compensation Mode, 0 - disabled, 1 - manual, 
      2 or 3 - auto.  Default is 0.
   
   CAN_SID11
      Used to enable/disable 12-bit SID in CAN FD base format messages. 
      Default is FALSE, 11-bit SID.
   
   CAN_EDGE_FLT
      Used to enable/disable edge filtering during bus integration state.
      Default is FALSE, disabled.
   
   CAN_DEVICE_NET_FILTER_BIT_NUMBER
      Sets the DeviceNet Filter bit Number.  Default is 0.
   
   CAN_ENABLE_ISO_CRC
      Used to enable/disable ISO CRC in CAN FD Frames, 0 - does not include
      stuff bit count in CRC field, 1 - includes stuff bit count in CRC field.
      Default is 0, does not include stuff bit count in CRC field.
   
   CAN_DISABLE_EXCEPTION_EVENT
      Used to set Protocol Exception Detection, FALSE - if a protocol exception
      is detected, CAN will enter bus integrating state, TRUE - protocol
      exception is treated as a form error.  Default is FALSE.
   
   CAN_ENABLE_WAKE_FILTER
      Used to enable/disable CAN Bus Line Wake-up Filter.  Default is FALSE,
      disabled.
   
   CAN_WAKE_FILTER_TIME
      Sets the Wake-up Filter Time, 0 - T00, 1 - T01, 2 - T10 or 3 - T11.
      Default is 0, T00.
   
   CAN_DISABLE_BIT_RATE_SWITCHING
      Used to disable bit rate switching, FALSE - bit rate switching depends on
      BRS in the transmit message object, TRUE - bit rate switching is
      disabled.  Default is FALSE, not disabled.
   
   CAN_RESTRICT_RETRANSMISSION_ATTEMPTS
      Used to restrict retransmission attempts, FALSE - unlimited number of
      retransmission attempts TXAT will be ignored, 1 - Restricted
      retransmission attempts uses TXAT.  Defaults to FALSE.
   
   CAN_TRANSMIT_ESI_MODE
      Sets transmit ESI in gateway mode, 0 - ESI reflects error status of CAN
      controller, 1 - ESI is transmitted as recessive when ESI of the message
      is high or CAN controller is error passive.  Default is 0.
   
   CAN_TRANSITION_TO_LISTEN
      Sets transition to listen only mode on system error, 0 - transitions to
      restricted operation mode, 1 - transitions to listen only mode.  Default
      is 0.
   
   CAN_TRANSMISSION_DELAY
      sets the transmit bandwidth sharing delay, see TRANSMISSION_DELAY type of
      all options.  Default is TRANSMISSION_DELAY_NONE, no delay.
   
   API:
   ----------------------------------------------------------------------------
   
   See prototypes and typedefs below for API documentation.
   
   OUTPUT:
   ----------------------------------------------------------------------------
   
   CAN_DEVICE_HAS_FD
      This macro is defined to denote the fact that this hardware supports CAN
      FD.
   
*/

#ifndef __CAN_DSPIC33_FD_H__
#define __CAN_DSPIC33_FD_H__

#define CAN_DEVICE_HAS_FD

#include <stdint.h>

#ifndef CAN_OPERATION_MODE
 #define CAN_OPERATION_MODE   0  //0-CAN FD, 1-CAN 2.0
#endif

#define CAN_ABORT_ALL   0xFF

// CAN Error Codes used by this driver
// 0 is OK, non-zero is some kind of error
typedef enum
{
   CAN_EC_OK = 0,
   CAN_EC_OBJ_RX_EMPTY,          //can_getd can not read a message because no message in Object
   CAN_EC_OBJ_NOT_RX,            //CAN Object is not a RX Object
   CAN_EC_OBJ_RX_NONE,           //no CAN Objects have been initialized as a RX Object
   CAN_EC_OBJ_TX_FULL,           //can_putd failed because TX Object is full
   CAN_EC_OBJ_NOT_TX,            //CAN Object is not a TX Object or not enabled
   CAN_EC_OBJ_TX_NONE,           //no CAN Objects have been initialized as a TX Object
   CAN_EC_OBJ_NO_RTR,            //CAN Object can not be set to respond to remote transmission requests
   CAN_EC_BAUD_NOT_DIVISIBLE,    //can_set_baud failed because clock not divisible by Nominal or Data baud rates
   CAN_EC_BAUD_NOMINAL_INVALID,  //can_set_baud failed because Nominal baud not achievable
   CAN_EC_BAUD_DATA_INVALID,     //can_set_baud failed because Data baud not achievable
} can_ec_t;

// CAN Operational Mode type, operation modes that the CAN FD peripheral can be put into
typedef enum
{
   CAN_OP_NORMAL_FD,             //Normal CAN FD mode, supports mixing of CAN FD and CAN 2.0 frames
   CAN_OP_DISABLE,               //Disable mode
   CAN_OP_LOOPBACK_INTERNAL,     //Internal Loopback mode
   CAN_OP_LISTEN,                //Listen Only mode
   CAN_OP_CONFIG,                //Configuration mode
   CAN_OP_LOOPBACK_EXTERNAL,     //External Loopback mode
   CAN_OP_NORMAL_CAN_2_0,        //Normal CAN 2.0 mode; error frames on CAN FD frames
   CAN_OP_RESTRICTED_OPERATION   //Restricted Operation mode
} CAN_OP_MODE;

// CAN Filter type
typedef enum {CAN_FILTER_0, CAN_FILTER_1, CAN_FILTER_2, CAN_FILTER_3,
              CAN_FILTER_4, CAN_FILTER_5, CAN_FILTER_6, CAN_FILTER_7,
              CAN_FILTER_8, CAN_FILTER_9, CAN_FILTER_10, CAN_FILTER_11,
              CAN_FILTER_12, CAN_FILTER_13, CAN_FILTER_14, CAN_FILTER_15,
} CAN_FILTER;

// CAN Filter Type type
typedef enum
{
   CAN_FILTER_TYPE_SID_ONLY,  //match only SID messages
   CAN_FILTER_TYPE_EID_ONLY,  //match only EID messages
   CAN_FILTER_TYPE_EITHER     //match either SID or EID messages
} CAN_FILTER_TYPE;

// CAN Object type.
typedef enum {CAN_OBJECT_TXQ, CAN_OBJECT_FIFO_1, CAN_OBJECT_FIFO_2, CAN_OBJECT_FIFO_3,
              CAN_OBJECT_FIFO_4, CAN_OBJECT_FIFO_5, CAN_OBJECT_FIFO_6, CAN_OBJECT_FIFO_7
} CAN_OBJECT;

// CAN Message Format type, whether message is a CAN FD or CAN 2.0 frame
typedef enum
{
   CAN_MESSAGE_FORMAT_CAN_2_0,
   CAN_MESSAGE_FORMAT_CAN_FD
} CAN_MESSAGE_FORMAT;

// CAN TX Message Header type
typedef struct
{
   uint32_t Id;               //ID to send message with
   uint8_t Length;            //Number of data bytes to send
   int1 ext;                  //TRUE - send message with extended ID, FALSE - send message with standard ID
   int1 rtr;                  //CAN 2.0 only, TRUE - send RTR (request), FALSE - not an RTR (request)
   CAN_MESSAGE_FORMAT Format; //CAN FD only, CAN_MESSAGE_FORMAT_CAN_2_0 (0) - send as CAN 2.0 message, CAN_MESSAGE_FORMAT_CAN_FD (1) - send as CAN FD message
} CAN_TX_HEADER;

// CAN RX Message Header type
typedef struct
{
   uint32_t Id;               //ID of received message
   uint8_t Length;            //Number of data bytes received
   uint8_t Filter;            //Filter message was received with
   uint32_t TimeStamp;        //Time Stamp when message was received, only if enabled see has Time Stamp see HasTS option
   int1 err_ovfl;             //TRUE - FIFO overflowed, FALSE - FIFO didn't overflow
   int1 ext;                  //TRUE - received extended ID message, FALSE - received standard ID message
   int1 rtr;                  //TRUE - received RTR (request), FALSE - not a RTR (request)
   CAN_MESSAGE_FORMAT Format; //CAN_MESSAGE_FORMAT_CAN_2_0 (0) - received message using CAN 2.0 format, CAN_MESSAGE_FORMAT_CAN_FD (1) - received message using CAN FD formate
   int1 HasTS;                //TRUE - Header includes a Time Stamp, FALSE - Header does not include a Time Stamp
} CAN_RX_HEADER;

// CAN Main Interrupts level type
typedef enum
{
   CAN_INTERRUPT_TX = 0x0001,          //Transmit Object Interrupt
   CAN_INTERRUPT_RX = 0x0002,          //Receive Object Interrupt
   CAN_INTERRUPT_TIMER = 0x0004,       //CAN Timer Interrupt
   CAN_INTERRUPT_MODE_CHANGE = 0x0008, //Mode Change Interrupt
   CAN_INTERRUPT_TX_EVENT = 0x0010,    //Transmit Event FIFO Interrupt
   CAN_INTERRUPT_TX_ATTEMPT = 0x0400,  //Transmit Attempt Interrupt
   CAN_INTERRUPT_RXOV = 0x0800,        //Receive Buffer Overflow Interrupt
   CAN_INTERRUPT_SYS_ERR = 0x1000,     //System Error Interrupt
   CAN_INTERRUPT_BUS_ERR = 0x2000,     //CAN Bus Error Interrupt
   CAN_INTERRUPT_WAKE = 0x4000,        //Wake-up Activity Interrupt
   CAN_INTERRUPT_INVALID = 0x8000      //Invalid Message Interrupt
} CAN_INTERRUPT;

// CAN FIFO Individual Interrupts level type
typedef enum
{
   CAN_FIFO_INTERRUPT_RXNE=0x0001,     //Receive FIFO Not Empty Interrupt
   CAN_FIFO_INTERRUPT_RXHF=0x0002,     //Receive FIFO Half Full Interrupt
   CAN_FIFO_INTERRUPT_RXF=0x0004,      //Receive FIFO Full Interrupt
   CAN_FIFO_INTERRUPT_RXOV=0x0008,     //Receive FIFO Overflow Interrupt
   CAN_FIFO_INTERRUPT_TXNF=0x0001,     //Transmit FIFO Not Full Interrupt
   CAN_FIFO_INTERRUPT_TXHE=0x0002,     //Transmit FIFO Half Empty Interrupt
   CAN_FIFO_INTERRUPT_TXE=0x0004,      //Transmit FIFO Empty Interrupt
   CAN_FIFO_INTERRUPT_TXAT=0x0010      //Transmit FIFO Attempts Exhausted Interrupt
} CAN_FIFO_INTERRUPT;

// CAN Payload Size type
typedef enum
{
   PAYLOAD_SIZE_8_BYTES,
   PAYLOAD_SIZE_12_BYTES,
   PAYLOAD_SIZE_16_BYTES,
   PAYLOAD_SIZE_20_BYTES,
   PAYLOAD_SIZE_24_BYTES,
   PAYLOAD_SIZE_32_BYTES,
   PAYLOAD_SIZE_48_BYTES,
   PAYLOAD_SIZE_64_BYTES
} PAYLOAD_SIZE;

// CAN Retransmit Attempts type
typedef enum
{
   RETRANSMIT_ATTEMPTS_DISABLED,
   RETRANSMIT_ATTEMPTS_THREE,
   RETRANSMIT_ATTEMPTS_UNLIMITED
} RETRANSMIT_ATTEMPTS;

// CAN Transmission Delay type
typedef enum
{
   TRANSMISSION_DELAY_NONE,
   TRANSMISSION_DELAY_2,
   TRANSMISSION_DELAY_4,
   TRANSMISSION_DELAY_8,
   TRANSMISSION_DELAY_16,
   TRANSMISSION_DELAY_32,
   TRANSMISSION_DELAY_64,
   TRANSMISSION_DELAY_128,
   TRANSMISSION_DELAY_256,
   TRANSMISSION_DELAY_512,
   TRANSMISSION_DELAY_1024,
   TRANSMISSION_DELAY_2048,
   TRANSMISSION_DELAY_4096
} TRANSMISSION_DELAY;

// Initialize the driver and peripheral.  After initializing the peripheral, it
// will be put into 'Mode'.
#if CAN_OPERATION_MODE == 0
 void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL_FD);
#else
 void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL_CAN_2_0);
#endif

// Sets the CAN bit rates of the device.  'Clock' is the CAN clock speed that is
// presented to the CAN FD peripheral.  'Nominal' is the arbitration and normal
// data bit rate, 'Data' is the bit rate use for after arbitration when CAN FD
// frames are used.  If faster FD bit rates aren't being using, 'Data' can be
// set to 0.  The math requires that clock be evenly divisible by the bit rates.
can_ec_t can_set_baud(uint32_t Clock, uint32_t Nominal, uint32_t Data);

// Sets the operational mode of the CAN FD peripheral.  'Mode' is the
// operational mode to set.  'Update' specifies whether to update a global
// variable used to save the operation mode, which is used by some functions to 
// return the operational mode back to it's previous operating mode.
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE);

// Sets up the specified filter's Id, Mask and Type.  'Filter' is the filter to
// set up, 'Id', is the Id to set the filter to, 'Mask', is the mask of the
// filter and 'Type' is the filter type to set it to, see CAN_FILTER_TYPE above
// for types it can be set to.
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, uint32_t Mask, CAN_FILTER_TYPE Type);

// Used to enable specified filter and assigning it to the specified object.
// 'Filter' is the filter to enable and 'Object', is the object to assign it to.
void can_enable_filter(CAN_FILTER Filter, CAN_OBJECT Object=CAN_OBJECT_FIFO_1);

// Used to disable the specified filter.  'Filter' is the filter to disable.
void can_disable_filter(CAN_FILTER Filter);

// Used load a message to transmit into a TX object.  'Header' is a pointer to
// CAN_TX_HEADER type with info to send message with, Id, Data length, etc.
// 'Data' is a pointer to the payload to send with message, and 'Object' is an
// optional parameter, specifying the TX object to load the message into.  By 
// default it load the TXQ object with the message to send.
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object=CAN_OBJECT_TXQ);

// Used to retrieve a received message from a RX object.  'Header' is a pointer
// to a CAN_RX_HEADER type variable to save received message's header info to,
// Id, Data length, etc.  'Data' is a pointer to an array to save the received
// message's payload to, and 'Object' is an optional parameter specifying the
// RX object to retrieve message from.  By default it retrieves the message
// from the FIFO1 object.
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object=CAN_OBJECT_FIFO_1);

// Used to determine if there is a least one message to retrieve from a RX
// object.  Returns TRUE there is a message to retrieve and FALSE if there is 
// no message to retrieve.  'Object' is an optional parameter specifying the RX
// object to check, by default it checks the FIFO1 object.
int1 can_kbhit(CAN_OBJECT Object=CAN_OBJECT_FIFO_1);

// Used to discard all messages from a RX object.  'Object' is an optional
// parameter specifying the RX object to discard messages for, by default it
// discards messages from the FIFO1 object.
can_ec_t can_discard(CAN_OBJECT Object=CAN_OBJECT_FIFO_1);

// Used to determine if a TX object can accept at least one more message.
// Returns TRUE if can accept at least one more message, FALSE if it is full.
// 'Object' is an optional parameter specifying the TX object to check, by
// default it checks the TXQ object.
int1 can_tbe(CAN_OBJECT Object=CAN_OBJECT_TXQ);

// Used to determine if a TX object is empty.  Returns TRUE if empty, FALSE if
// not empty.  'Object' is an optional parameter specifying the TX object to
// check, by default it checks the TXQ object.
int1 can_tx_is_empty(CAN_OBJECT Object=CAN_OBJECT_TXQ);

// Used to abort pending transmission for a TX object.  'Object' is an optional
// parameter specifying the TX object to abort pending transmission for, by
// default it aborts messages for all TX objects.
can_ec_t can_abort(CAN_OBJECT Object=CAN_ABORT_ALL);

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

// Used to enable one or more of the CAN FIFO interrupts.  'Object' is the
// object to enable the interrupt for, and 'Setting' is the CAN FIFO interrupt
// to enable, multiple interrupts can be or'ed together to enable multiple CAN
// FIFO interrupts.
void can_enable_fifo_interrupts(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Setting);

// Used to disable one or more of the CAN FIFO interrupts.  'Object' is the
// object to disable the interrupt for, and 'Setting' is the CAN FIFO interrupt
// to disable, multiple interrupts can be or'ed together to disable multiple
// CAN FIFO interrupts.
void can_disable_fifo_interrupts(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Setting);

// Used to determine which CAN FIFO interrupts are active.   Returns 
// CAN_FIFO_INTERRUPT type, if an interrupt is active the corresponding bit
// will be set.  See CAN_FIFO_INTERRUPT type to determine which bits
// corresponds to which interrupt.  'Object' is the object to check if the CAN
// FIFO interrupts are active.
CAN_FIFO_INTERRUPT can_fifo_interrupts_active(CAN_OBJECT Object);

// Used to determine if the specified CAN FIFO interrupt is active.  Returns
// TRUE if active, FALSE is not active.  'Object' is the object to check if the
// CAN FIFO interrupt is active, and 'Interrupt' is the interrupt to test for.
int1 can_fifo_interrupt_active(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Interrupt);

// Used to clear specified CAN FIFO interrupt flag.  'Object' is the object to
// clear the flag for, and 'Interrupt' is the interrupt to clear.
void can_clear_fifo_interrupt(CAN_OBJECT Object, CAN_FIFO_INTERRUPT Interrupt);

// Used to enable RTR messages on a TX object.  'Object' is the object to
// enable RTR messages on.
can_ec_t can_enable_rtr(CAN_OBJECT Object);

// Used to disable RTR messages on a TX object.  'Object' is the object to
// disable RTR messages on.
can_ec_t can_disable_rtr(CAN_OBJECT Object);

// Used to load a message into the TX FIFO object that will be transmitted when
// a remote transmission request is received.  'Header' is a pointer to a
// CAN_TX_HEADER structure containing info about the message to transmit, Id,
// Data length, etc.  'Data' is a pointer to an array contain payload of
// message to transmit, and 'Object', is the FIFO object to load the message
// into.
can_ec_t can_load_rtr(CAN_TX_HEADER *Header, uint8_t *Data, CAN_OBJECT Object);

#endif //__CAN_DSPIC33_FD_H__
