///////////////////////////////////////////////////////////////////////////////
////                                J1939.h                                ////
////                                                                       ////
//// J1939 Library for the Data Link Layer of the SAE J1939 specification. ////
//// Refer to the SAE J1939-21 for more information on spec.               ////
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

#ifndef _J1939_H
#define _J1939_H

/*
   J1939 PREPROCESSORS
   ----------------------------------------------------------------------------
   
   J1939_BAUD_RATE
      Set the bit rate the CAN wil be set to, default is 250 kbps.
   
   J1939_USE_INTERNAL_CAN
      Sets whether to use the PIC's internal CAN, ECAN or CANFD peripheral.  
      Default is TRUE if PIC has a CAN, ECAN or CANFD peripheral and FALSE if
      PIC does not have a CAN, ECAN or CANFD peripheral.
   
   J1939_USE_EXT_FD_CONTROLLER
      If J1939_USE_INTERNAL_CAN is FALSE, specifies to use the can-mcp2517.c 
      external CANFD driver or the can-mcp2515.c external CAN driver.  Default
      is FALSE, use can-mcp2515.c driver.
   
   J1939_RECEIVE_BUFFERS
      Specifies the size of the software J1939 receive buffer, number of CAN
      messages that buffer can hold, minimum size is 2.  Default size is 16.
   
   J1939_TRANSMIT_BUFFERS
      Specifies the size of the software J1939 transmit buffer, number of CAN
      messages that buffer can hold, minimum size is 1.  Default size if 2.
   
   J1939 REQUIRED MACROS
   ----------------------------------------------------------------------------
   
   J1939InitAddress
      Macro to initialize the g_MyJ1939Address variable, which is the perferred
      J1939 address for this unit.
   
   J1939InitName
      Macro to initalize the g_J1939Name array, which is the J1939 Name of this
      unit.
      
   J1939 REQUIRED TICK TIMER
   ----------------------------------------------------------------------------
   
   J1939GetTick()
      Macro to return the current tick time.
   
   J1939GetTickDifference(a,b)
      Macro to calculate the difference between tick time a and b.
   
   J1939_TICKS_PER_SECOND
      Define specifiny the number of ticks per second.
   
   J1939_TICK_TYPE
      Typedef specifying the variable type that the tick timer uses.
   
   * The tick timer should be setup for a rate of 1 tick per millisecond or
     faster.
   
*/

#include <stdint.h>

//J1939 PDU Structure
typedef struct _J1939_PDU_STRUCT {
   uint8_t SourceAddress;
   uint8_t DestinationAddress;   //Also Group Extension for PDU2 messages
   uint8_t PDUFormat;
   int1    DataPage;
   int1    ExtendedDataPage;     //Set to 0 for J1939
   uint8_t Priority:3;
   uint8_t unused7_5:3;          //unused bits don't do anything with them
} J1939_PDU_STRUCT;

// Initializes the CAN for J1939 communication.
void J1939Init(void);

#separate
// Checks for new received CAN messages and loads them into the J1939 receive
// buffer.  This function should be called frequently, it's recommended to call
// it at least once per ideration of the main() loop.
void J1939ReceiveTask(void);

#separate
// Checks for message in J1939 transmit buffer and loads them into the CAN
// buffers to be transmited.  This function should be called frequently, it's
// recommended to call it a least once per ideration of the main() loop.
void J1939XmitTask(void);

// Checks for a new message in receive buffer that can be retrieved.  Returns
// TRUE if there is a new message to retreive, and FALSE if there is no new
// message to retrieve.
int1 J1939Kbhit(void);

// Retrieves a new message from receive buffer.  'PDU' is a J1939_PDU_STRUCT
// type variable to return the message's PDU to, 'Data' is a pointer to an 
// array to return the data portion of the message to, and 'Length' is variable
// to return the number of bytes in the data portion of the message.  Returns
// TRUE if a new message was reteived from buffer, and FALSE if there was no
// new message in buffer.
int1 J1939GetMessage(J1939_PDU_STRUCT &PDU, uint8_t *Data, uint8_t &Length);

// Load message into transmit buffer to send.  'PDU' is a J1939_PDU STRUCT type
// variable containing PDU of message to send, 'Data' is a pointer to an array
// contain data portion of message to send and 'Bytes' is number of bytes in
// data portion of message.  Returns TRUE if message was succefully loaded into
// transmit buffer and FALSE if messages was not loaded into buffer, i.e. 
// transmit buffer was full.
int1 J1939PutMessage(J1939_PDU_STRUCT PDU, uint8_t *Data, uint8_t Bytes);

// Used to send a messages to see see if specified address has been claimed.
// Use address global address 255 to receive a list of all claimed address.
void J1939RequestAddress(uint8_t address);

#endif
