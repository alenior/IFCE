///////////////////////////////////////////////////////////////////////////////
////                               lorawan.h                               ////
////                                                                       ////
////  Driver for joining and communication on a LoRaWAN network.           ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2020 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                                                                       ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

/*
   LORAWAN PREPROCESSORS
   ----------------------------------------------------------------------------
   
   LORAWAN_JOIN_TYPE
      Define used to set the join type used for joining a LoRaWAN network, can
      be either LORAWAN_JOIN_TYPE_OTAA (over-the-air-activation) or
      LORAWAN_JOIN_TYPE_ABP (activation by personalization).  Defaults to
      LORAWAN_JOIN_TYPE_OTAA if not defined before this driver is included in
      project.
   
   LORAWAN_SYNC_BYTE
      Define used to set the synchronization byte used for communicating with
      the LoRaWAN network.  Defaults to 0x34 if not defined before this driver
      is included in project.
   
   LORAWAN_RETRANSMISSION_ATTEMPTS
      Define used to set the number of the LoRaWAN module will do when sending
      confirmed messages.  Defaults to 5 if not defined before this driver is
      included in project.
   
   LORAWAN_RX_DELAY
      Define used to set the delay in milliseconds for the 1st receive window
      after a message is sent.  Defaults to 1000 if not defined before this
      driver is included in project.
   
   LORAWAN_RX2_DATA_RATE
      Define used to set the data rate used during the 2nd receive window after
      a message is sent.  Can be set from 8 to 13, defaults to 8 if not defined
      before this driver is included in project.
   
   LORAWAN_RX2_FREQ
      Define used to set the frequency used during the 2nd receive window after
      a message is sent.  Defaults to 923300000 if not defined before this 
      driver is included in project.
   
   LORAWAN_AUTO_REPLY
      Define used to set if the auto reply feature is enabled when a confirmed
      downlink message is received.  Defaults to FALSE if not defined before
      this is included in project.
   
   LORAWAN_PAUSED_RETRY_TIME
      When LoRaWAN stack is paused this define sets how often the driver will
      check to see if stack is still paused, in seconds.  Defaults to 10 if not
      defined before this driver is included in project.
   
   LORAWAN_BUSY_RETRY_TIME
      When the LoRaWAN stack is busy this define sets how often the driver
      will retry, in milliseconds, doing operation that returned the busy
      state.  Defaults to 2000 if not defined before this driver is included in
      project.
      
   LORAWAN_JOIN_DENIED_RETRY_TIME
      Define for setting how often, in seconds, the driver will try to join
      LoRaWAN network if it was denied.  Defaults to 30 if not defined before
      this driver is included in project.
   
   LORAWAN_MINIMUM_DATA_RATE
      Define for setting the minimum data used by a channel to transmit
      messages (0 to 3), 0 is the lowest data rate and 3 is the highest data
      rate.  Defaults to 0 if not defined before this driver is included in 
      project.
   
   LORAWAN_MAXIMUM_DATA_RATE
      Define for setting the maximum data used by a channel to transmit
      messages (0 to 3), 0 is the lowest data rate and 3 is the highest data
      rate.  Defaults to 3 if not defined before this driver is included in 
      project.
   
   LORAWAN_DEFAULT_TX_DATA_RATE
      Define for setting the data rate used to transmit a message with.  This
      value should be set so that it's in the range defined by 
      LORAWAN_MINIMUM_DATA_RATE and LORAWAN_MAXIMUM_DATA_RATE.  Defaults to 
      0 if not defined before this driver is included in project.
   
   LORAWAN_MINIMUM_CHANNEL
      Define for setting the first channel that can be used to transmit
      messages (0 to 63).  Defaults to 0 if not defined before this driver is
      included in project.
   
   LORAWAN_MAXIMUM_CHANNEL
      Define for setting the last channel that can be used to transmit messages
      (0 to 63).  Defaults to 0 if not defined before this driver is included
      in project.  Both the LORAWAN_MINIMUM_CHANNEL and LORAWAN_MAXIMUM_CHANNEL
      defines sets the channels that the LoRaWAN module will use to transmit
      messages with for example if LORAWAN_MINIMUM_CHANNEL is defined as 0 and
      LORAWAN_MAXIMUM_CHANNEL is defined as 7, the module will only use
      channels 0, 1, 2, 3, 4, 5, 6 and 7 to transmit message with.
   
   LORAWAN_DEFAULT_APP_EUI
      If LORAWAN_JOIN_TYPE is defined as LORAWAN_JOIN_TYPE_OTAA, this sets the 
      default Application EUI used by the driver.  It can be a const or
      variable uint8_t arrays of size LORAWAN_EUI_SIZE or a function that 
      returns a pointer to an uint8_t array of size LORAWAN_EUI_SIZE, must be 
      declared before lorawan.c is included in project.  No default value is
      declared.
   
   LORAWAN_DEFAULT_APP_KEY
      If LORAWAN_JOIN_TYPE is defined as LORAWAN_JOIN_TYPE_OTAA, this sets the 
      default Application Key used by the driver.  It can be a const or 
      variable uint8_t arrays of size LORAWAN_KEY_SIZE or a function that 
      returns a pointer to an uint8_t array of size LORAWAN_KEY_SIZE, must be
      declared before lorawan.c is included in project.  No default value is
      declared.
   
   LORAWAN_DEFAULT_DEVICE_ADDR
      If LORAWAN_JOIN_TYPE is defined as LORAWAN_JOIN_TYPE_ABP, this sets the 
      default Device Address used by the driver.  It must be declared before
      lorawan.c is included in project.  No default value is defined.
   
   LORAWAN_DEFAULT_NWK_SESSION_KEY
      If LORAWAN_JOIN_TYPE is defined as LORAWAN_JOIN_TYPE_ABP, this sets the 
      default Network Session Key used by the driver.  It can be a const or 
      variable uint8_t arrays of size LORAWAN_KEY_SIZE or a function that 
      returns a pointer to an uint8_t array of size LORAWAN_KEY_SIZE, must be
      declared before lorawan.c is included in project.  No default value is
      declared.
   
   LORAWAN_DEFAULT_APP_SESSION_KEY
      If LORAWAN_JOIN_TYPE is defined as LORAWAN_JOIN_TYPE_ABP, this sets the 
      default Application Session Key used by the driver.  It can be a const or 
      variable uint8_t arrays of size LORAWAN_KEY_SIZE or a function that 
      returns a pointer to an uint8_t array of size LORAWAN_KEY_SIZE, must be
      declared before lorawan.c is included in project.  No default value is
      declared.
   
   LORAWAN_CHECK_JOIN_STATUS_TIME
      Used to defined how often in seconds driver will check if module is still
      joined to LoRaWAN network after it has successfully join.  No default
      time is defined, if not define driver will not check the join state of 
      the module.
   
   LORAWAN REQUIRED TICK FUNCTION TYPES and DEFINES
   ----------------------------------------------------------------------------
   
   ** The following functions, types and defines need to be provided to the
      lorawan.c driver before it's include in your project.
   
   _lorawan_tick_t - the tick type.
   
   _LORAWAN_TICKS_PER_SECOND - a define indicating the number of ticks per
                               second.
   
   _lorawan_tick_get() - function to get the current tick timer count.
   
   _lorawan_tick_difference(a,b) - function to calculate the difference between
                                   two tick timer count values, first parameter
                                   is the most recent tick timer count value
                                   and the second parameter is the older tick
                                   timer count value.
   
   LORAWAN REQUIRED MODULE FUNCTIONS
   ----------------------------------------------------------------------------
   
   ** The following function need to be provided to the lorawan.c driver before
      it's include in your project.  The CCS C driver rn2903.c already has all
      the following functions provided in it.
   
   _lorawan_init_radio() - function for initialize the communication with the
         LoRaWAN radio module.
   
   _lorawan_rx_task() - task function for receiving info from the radio module.
   
   _lorawan_kbhit() - function for determining if radio module has a message to
         read, returns TRUE if it has a message to read and FALSE if it doesn't
         have a message to read.
   
   _lorawan_putd(Port, Data, Length, Confirmed) - function for loading a
         message to send with the radio module using the LoRaWAN stack.  Port
         is an uint8_t value indicating port to send message with, Data is a
         pointer to uint8_t array containing data to send, Length is an uint8_t
         value indicating number of data bytes to send and Confirmed indicates
         if messages is sent as a confirmed or unconfirmed message.  Function
         returns _lorawan_module_error_t indicating if messages was sent to 
         module correctly or if an error occurred.
   
   _lorawan_reset() - function to reset the LoRaWAN module.  Return TRUE if no
         error occurred and FALSE if an error occurred.
   
   _lorawan_get_config() - function to read the whether the radio module is
         joined to a LoRaWAN network and sets the join state in the joinState
         element in the _lorawan_config structure variable.  If joined it also
         reads the remaining elements, that can be read from the module, for
         the _lorawan_config structure variable.  Returns TRUE if no error
         occurred and FALSE if an error occurred.
    
   _lorawan_set_config() - function to set the radio module to the settings in
         the _lorawan_config structure variable.  Returns TRUE if no error
         occurred and FALSE if an error occurred.
   
   _lorawan_join(Type) - function to have the radio initiate joining a LoRaWAN
         network.  Type is a lorawan_join_type_t value indicate how to join the
         network.  Returns _lorawan_module_error_t value indicating if there
         was an error or not when the command was sent to the radio module.
    
   _lorawan_get_radio_message(dPtr, lPtr) - function used to retrieve a 
         message from the radio module.  dPtr is a pointer to an uint8_t array
         to return rx data to, lPtr is a pointer to an uint8_t variable to
         return number of data bytes returned to dPtr.  Function returns 
         lorawan_module_resp_t value indicating message type return from radio
         module, dPtr and lPtr only need to be valid when return value is 
         LORAWAN_MODULE_RESP_RX.  In that case dPtr should only be the RX data
         and RX data length.
    
   _lorawan_save_settings() - function to save the current configuration to
         the radio module.  Returns TRUE if the settings were saved and FALSE
         if an error occurred.
    
   _lorawan_paused() - function to determined if the LoRaWAN stack is paused.
         Returns TRUE if paused and FALSE if not paused.
   
   _lorawan_joined() - function to determine if the radio module is connected
         to a LoRaWAN network.  Returns TRUE if connected and FALSE if not
         connected or it indicates that a rejoin is needed.
   
   _lorawan_set_sync_byte(v) - function to set the synchronization byte used by
         the radio module.  v is the synchronization byte to set, function
         returns TRUE if successful and FALSE if unsuccessful.
  
   _lorawan_set_device_eui(v) - function to set the Device EUI used by the
         radio module when joining via over-the-air activation (OTAA).  v is a
         pointer to an uint8_t array containing the EUI to set.  Returns TRUE
         if successful and FALSE if unsuccessful.
   
   _lorawan_set_app_eui(v) - function to set the Application EUI used by the
         radio module when joining via OTAA.  v is a pointer to an uint8_t
         array containing the EUI to set.  Returns TRUE if successful and FALSE
         if unsuccessful.
   
   _lorawan_set_app_key(v) - function to set the Application Key used by the
         radio module when joining via OTAA.  v is a pointer to an uint8_t
         array containing the key to set.  Returns TRUE if successful and FALSE
         if unsuccessful.
   
   _lorawan_set_network_session_key(v) - function to set the Network Session
         Key used by the radio module when joining via activation by 
         personalization (ABP).   v is a pointer to an uint8_t array
         containing the key to set.  Returns TRUE if successful and FALSE if
         unsuccessful.
   
   _lorawan_set_app_session_key(v) - function to set the Application Session 
         Key used by the radio module when joining via ABP.  v is a pointer to
         an uint8_t array containing the key to set.  Returns TRUE if
         successful and FALSE if unsuccessful.
   
   _lorawan_set_device_address(v) - function to set the Network Device Address
         used by the radio module when joining via ABP.  v is an uint32_t value
         to set the address to.  Returns TRUE if successful and FALSE if
         unsuccessful.
   
   _lorawan_set_retransmission_attempts(v) - function to set the number of 
         retransmission attempts to do when sending a confirmed message.  v is
         an uint8_t value to set the retransmission attempts to.  Returns TRUE
         if successful and FALSE if unsuccessful.
   
   _lorawan_set_rx_delay(v) - function to set the delay for the 1st RX window
         after a message is sent.  v is an uint16_t value representing the time
         in milliseconds to set the delay to.  Returns TRUE if successful and
         FALSE if unsuccessful.
   
   _lorawan_set_rx2_data_rate(Rate) - function to set the data rate for the 2nd
         RX window after a message is sent.  Rate is an uint8_t value to set
         the data rate to.  Returns TRUE if successful and FALSE if
         unsuccessful.
   
   _lorawan_set_rx2_frequency(Frequency) - function to set the frequency for
         the 2nd RX window after a message is sent.  frequency is an uint32_t 
         value to set the frequency to.  Returns TRUE if successful and FALSE
         if unsuccessful.
   
   _lorawan_set_auto_reply(v) - function to enable/disable auto replay for
         downlink confirmed messages are received.  v will be TRUE to enable
         auto replay and FALSE to disable auto reply.  Returns TRUE if
         successful and FALSE if unsuccessful.
   
   _lorawan_set_ch_dr_range(ch, min, max) - function to set a channel's data
         rate range.  ch is uint8_t value specifying the channel to set, min
         is an uint8_t value to set the minimum data rate to and max is an
         uint8_t value to set the maximum data rate to.  Returns TRUE if
         successful and FALSE if unsuccessful.
   
   _lorawan_enable_ch(ch) - function to enable a channel, ch is the channel to 
         enable.  Returns TRUE if successful and FALSE if unsuccessful.
   
   _lorawan_disable_ch(ch) - function to disable a channel, ch is the channel
         to disable.  Returns TRUE if successful and FALSE if unsuccessful.
   
   _lorawan_set_power_index(v) - function to set the power index used for the
         next transmission.  v is an uint8_t value to set the power index to.
         Returns TRUE if successful and FALSE if unsuccessful.
   
   _lorawan_get_ch_status(ch, status) - function to get a channel's status.  ch
         is the channel to get the status for, status is a pointer to a
         lorawan_ch_status_t variable to return the status to.  Returns TRUE if
         successful and FALSE if unsuccessful.
   
   _lorawan_set_downlink_counter(v) - function to set the downlink frame
         counter that will be used for the next downlink reception.  v is an
         uint32_t value to set the downlink frame counter to.  Returns TRUE if
         successful and FALSE if unsuccessful.
   
   _lorawan_set_uplink_counter(v) - function to set the uplink frame counter
         that will be used for teh next uplink transmission.  v is an uint32_t
         value to set the uplink frame counter to.  Returns TRUE if successful
         and FALSE if unsuccessful.
   
   _lorawan_get_downlink_counter() - function to get the downlink frame counter
         that will be used for the next downlink reception.  Returns uint32_t
         value indicating the downlink frame counter.
   
   _lorawan_get_uplink_counter() - function to get the uplink frame counter
         that will be used for the next uplink transmission.  Returns uint32_t
         value indicating the uplink frame counter.
   
   _lorawan_set_tx_dr() - function to set the data rate to use for the next
         transmission.  Returns TRUE if successful and FALSE if unsuccessful.
   
   ** Some of the above functions, _lorawan_get_config() for example, require
      some types and global variables that are declared in lorawan.h.  So 
      you'll need to include the lorawan.h file before those function are
      implemented and/or declared.   
*/

#ifndef __LORAWAN_H__
#define __LORAWAN_H__

#ifndef debug_lorawan_printf
 #define debug_lorawan_printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#else
 #ignore_warnings 236

 #if !defined(debug_lorawan_putc)
  #if definedinc(debug_lorawan_putc) == 0
   void debug_lorawan_putc(c)
   {
      putc(c);
   }
  #endif
 #endif
 
 #ignore_warnings none
#endif

#ifdef __PCD__
 #ifndef _PACKED_STRUCT
  #define _PACKED_STRUCT   __attribute__((__packed__))
 #endif
#else
 #ifndef _PACKED_STRUCT
  #define _PACKED_STRUCT
 #endif
#endif

#ifndef LORAWAN_MAX_PAYLOAD_SIZE
 #define LORAWAN_MAX_PAYLOAD_SIZE      64
#endif

typedef enum
{
   LORAWAN_JOIN_TYPE_OTAA,    //over-the-air activation
   LORAWAN_JOIN_TYPE_ABP      //activation by personalization
} lorawan_join_type_t;

typedef enum
{
   LORAWAN_MODE_JOIN,
   LORAWAN_MODE_JOIN_ERROR,
   LORAWAN_MODE_JOIN_DENIED,
   LORAWAN_MODE_PAUSED,
   LORAWAN_MODE_BUSY,
   LORAWAN_MODE_IDLE,
   LORAWAN_MODE_TX_DATA,
   LORAWAN_MODE_ERROR,
   LORAWAN_MODE_UNKNOWN=0xFF
} lorawan_mode_t;

typedef enum
{
   LORAWAN_MODULE_RESP_NONE,
   LORAWAN_MODULE_RESP_DENIED,      //the LoRaWAN join request was denied
   LORAWAN_MODULE_RESP_ACCEPTED,    //the LoRaWAN join request was accepted
   LORAWAN_MODULE_RESP_TX_OK,       //transmission was successful, no data was received back from the server
   LORAWAN_MODULE_RESP_RX,          //transmission was successful, data was received back from the server
   LORAWAN_MODULE_RESP_ERR,         //transmission was unsuccessful
   LORAWAN_MODULE_RESP_LENGTH,      //application payload length is greater then maximum application payload length corresponding to the current data rate
   LORAWAN_MODULE_RESP_UNKNOWN
} lorawan_module_resp_t;

typedef enum
{
   LORAWAN_JOIN_STATE_NOT_JOINED,
   LORAWAN_JOIN_STATE_JOINING,
   LORAWAN_JOIN_STATE_JOINED,
   LORAWAN_JOIN_STATE_ERROR,
   LORAWAN_JOIN_STATE_ERROR_NOT_INIT,     //Join info not initialized, depends on join type being used.
                                          //For OTAA the AppEUI and AppKey need initialized, and for
                                          //ABP the DevAddr, NwkSKey and AppSKey need initialized.
   LORAWAN_JOIN_STATE_DENIED
} lorawan_join_state_t;

typedef enum
{
   LORAWAN_MODULE_ERROR_OK,               //no errors have occurred
   LORAWAN_MODULE_ERROR_PARAMETER,        //invalid parameter passed to command
   LORAWAN_MODULE_ERROR_NOT_INIT,         //settings for join type not set
   LORAWAN_MODULE_ERROR_CHANNEL,          //no channel available
   LORAWAN_MODULE_ERROR_SILENT,           //device is in Silent Immediately State
   LORAWAN_MODULE_ERROR_BUSY,             //device's LoRaWAN stack is busy
   LORAWAN_MODULE_ERROR_PAUSED,           //device's LoRaWAN stack is paused
   LORAWAN_MODULE_ERROR_DENIED,           //join attempt was unsuccessful
   LORAWAN_MODULE_ERROR_NOT_JOINED,       //module not joined to a network
   LORAWAN_MODULE_ERROR_FRAME_COUNTER,    //frame counter, rejoin needed
   LORAWAN_MODULE_ERROR_LENGTH,           //application payload length is greater then maximum application payload length corresponding to the current data rate
   LORAWAN_MODULE_ERROR_UNKNOWN=0xFF
} _lorawan_module_error_t;

typedef enum
{
   LORAWAN_TX_ERROR_OK,
   LORAWAN_TX_ERROR_PARAM,
   LORAWAN_TX_ERROR_LENGTH,
   LORAWAN_TX_ERROR_ERROR
} lorawan_tx_error_t;

typedef enum
{
   LORAWAN_PWR_IDX_5=5,    //20 dBm
   LORAWAN_PWR_IDX_6,      //18 dBm
   LORAWAN_PWR_IDX_7,      //16 dBm
   LORAWAN_PWR_IDX_8,      //14 dBm 
   LORAWAN_PWR_IDX_9,      //12 dBm
   LORAWAN_PWR_IDX_10      //10 dBm
} lorawan_pwr_idx_t;

typedef struct _PACKED_STRUCT
{
   uint8_t Port;
   uint8_t Data[LORAWAN_MAX_PAYLOAD_SIZE];
   uint8_t Length;
} lorawan_rx_message_t;

typedef struct
{
   uint32_t Frequency;
   uint8_t DataRateMin;
   uint8_t DataRateMax;
   int1 Enabled;
} lorawan_ch_status_t;   

#define LORAWAN_EUI_SIZE      8
#define LORAWAN_KEY_SIZE      16

///////////////////////////////////////////////////////////////////////////////

struct _PACKED_STRUCT
{
   uint8_t Sync;
   uint8_t DeviceEUI[LORAWAN_EUI_SIZE];            //used for OTAA join
   uint8_t AppEUI[LORAWAN_EUI_SIZE];               //used for OTAA join
   uint8_t AppKey[LORAWAN_KEY_SIZE];               //used for OTAA join
   uint8_t NetworkSessionKey[LORAWAN_KEY_SIZE];    //used for ABP join
   uint8_t AppSessionKey[LORAWAN_KEY_SIZE];        //used for ABP join
   uint32_t DeviceAddr;                            //used for ABP join
   uint8_t RetransmissionAttempts;
   uint16_t RXDelay;                               //in milliseconds
   uint8_t RX2DataRate;
   uint32_t RX2Frequency;
   uint8_t MinDataRate;
   uint8_t MaxDataRate;
   uint8_t TxDataRate;
   uint8_t FirstChannel;
   uint8_t LastChannel;
   lorawan_join_type_t joinType;
   int1 AutoReply;
} _lorawan_config;

///////////////////////////////////// API /////////////////////////////////////

/*
   lorawan_reset()
   
   Macro for resetting the LoRa Radio module.
   
   Returns:
      TRUE - if module reset sucessfully.
      FALSE - if an error occurred.
*/
#define lorawan_reset()    (_lorawan_reset() == TRUE)

/*
   lorawan_init()
   
   Function for initializing the LoRa Radio communication and reading the
   current status of the LoRaWAN stack.  If joined to a network it will read
   the current configuration, if not joined it initializes the LoRaWAN stack
   to the default configuration, except for initialization info need for for 
   the module to join the network.  AppEUI and AppKey for OTAA joining and
   DevAddr, NwkSKey and AppSKey for ABP joining.  Those can be initialized with
   the lorawan_init_join_info() function.
   
   Returns:
      TRUE - successfully initialized communication and get/set LoRaWAN config.
      FALSE - failed to initialize communication or get/set LoRaWAN config.
*/
int1 lorawan_init(void);

/*
   lorawan_init_join_info()
   
   Function for initializing the info necessary to joining a LoRaWAN network, 
   AppEUI and AppKey for OTAA joining and DevAddr, NwkSKey and AppSKey for ABP
   joining.  It's recommended to make sure this function is only called when 
   initialally setting up the module during production or when one of the
   settings need changed, to keep the keys secure.  
   
   Returns:
      TRUE - successfully initialized the join info.
      FALSE - failed to initialize the join info.
*/
int1 lorawan_init_join_info(void);

/*
   lorawan_task()
   
   Task function used to join LoRaWAN network, send messages and receive
   messages.  Should be called relatively frequently, recommended at least once
   per iteration of main loop.
*/
void lorawan_task(void);

/*
   lorawan_tbe()
   
   Used to determine if the transmit buffer is empty.  This can be polled to
   make sure previous message has been sent before trying to load next message
   to send.

   Returns:
      TRUE - if empty and buffer is free to receive next message.
      FALSE - if buffer is not empty, new message can not be loaded into buffer
              to send.
*/
int1 lorawan_tbe(void);

/*
   lorawan_busy()
   
   Function that can be used to determine if driver is busy and can't send a 
   new message to send.
   
   Returns:
      TRUE - driver is busy can't send a new message, either busy join network,
             sending another message or in error state.
      FALSE - driver not busy can send a new message.
*/
int1 lorawan_busy(void);

/*
   lorawan_put_message()
   
   Function used to load message into transmit buffer to send.
   
   Parameters:
      Port - uint8_t value indicating port to use.
  
      Data - pointer to uint8_t array containing data to send.
  
      Length - uint8_t value indicating number of bytes to send.
 
      Confirm - determines if message is sent as a confirmed or unconfirmed
                message.
  
   Returns:
      TRUE - message was loaded into buffer correctly.
      FALSE - message was not loaded into buffer, buffer was full or driver
              is in wrong mode.
*/
int1 lorawan_put_message(uint8_t Port, uint8_t *Data, uint8_t Length, int1 Confirm);

/*
   lorawan_has_message()
   
   Function used to determine if driver has a new message to retrieve from 
   receive buffer.
   
   Returns:
      TRUE - has a new message to retrieve.
      FALSE - no new message to retrieve.
*/
int1 lorawan_has_message(void);

/*
   Function used to get a new message from receive buffer.
   
   Parameters:
      Message - pointer to an lorawan_rx_message_t variable to save retrieved
                message to.
   
   Returns:
      TRUE - a new message was retrieved from buffer.
      FALSE - no message was retrieved from buffer.
*/
int1 lorawan_get_message(lorawan_rx_message_t *Message);

/*
   lorawan_set_sync_byte()
   
   Function used to change the synchronization byte used for LoRaWAN
   communication.  Changing the value will cause driver to change to not joined
   state and attempt to join network using specified synchronization byte.
   
   Parameters:
      Sync - uint8_t value to set the synchronization byte to.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_sync_byte(uint8_t Sync);

/*
   lorawan_set_device_eui()
   
   Function for changing the Device EUI used for joining when set for OTAA
   joining.  Changing this value will cause driver to change to not joined
   state and attempt to join network using specified Device EUI.
   
   Parameters:
      eui - pointer to uint8_t array containing Device EUI.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_device_eui(uint8_t *eui);

/*
   lorawan_set_app_eui()
   
   Function for changing the Application EUI used for joining when set for OTAA
   joining.  Changing this value will cause driver to change to not joined
   state and attempt to join network using specified Application EUI.
   
   Parameters:
      eui - pointer to uint8_t array containing Application EUI.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_app_eui(uint8_t *eui);

/*
   lorawan_set_app_key()
   
   Function for changing the Application Key used for joining when set for OTAA
   joining.  Changing this value will cause driver to change to not joined
   state and attempt to join network using specified Application Key.
   
   Parameters:
      Key - pointer to uint8_t array containing Application Key.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_app_key(uint8_t *Key);

/*
   lorawan_set_network_session_key()
   
   Function for changing the Network Session Key used for joining when set for
   ABP joining.  Changing this value will cause driver to change to not joined
   state and attempt to join network using specified Network Session Key.
   
   Parameters:
      Key - pointer to uint8_t array containing Network Session Key.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_network_session_key(uint8_t *Key);

/*
   lorawan_set_app_session_key()
   
   Function for changing the Application Session Key used for joining when set
   for ABP joining.  Changing this value will cause driver to change to not 
   joined state and attempt to join network using specified Application Session
   Key.
   
   Parameters:
      Key - pointer to uint8_t array containing Application Session Key.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_app_session_key(uint8_t *Key);

/*
   lorawan_set_device_address()
   
   Function for changing the Device Address used for joining when set for ABP
   joining.  Changing this value will cause driver to change to not joined
   state and attempt to join network using specified Device Address.
   
   Parameters:
      Address - uint32_t value to set the Device Address to.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_device_address(uint32_t Address);

/*
   lorawan_set_retransmission_attempts()
   
   Function for changing the number of retransmission attempts done when
   messages is sent as a confirmed messages, and no confirmed response is
   received.
   
   Parameters:
      Attempts - uint8_t value representing the number of retransmission
                attempts to make.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_retransmission_attempts(uint8_t Attempts);

/*
   lorawan_set_rx_delay()
   
   Function for changing the delay for the 1st RX window after a message is
   sent.
   
   Parameters:
      Delay - uint16_t value representing the delay in milliseconds.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_rx_delay(uint16_t Delay);

/*
   lorawan_set_rx2_data_rate()
   
   Function used to set the data rate used for the 2nd RX window after a
   message is sent.
   
   Parameters:
      Rate - uint8_t value, from 8 to 13, to set the data rate to.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_rx2_data_rate(uint8_t Rate);

/*
   lorawan_set_rx2_frequency()
   
   Function used to set the frequency used for the 2nd RX window after a 
   message is sent.
   
   Parameters:
      Frequency - uint32_t representing the frequency to set.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_rx2_frequency(uint32_t Frequency);

/*
   lorawan_set_join_type()
   
   Macro for setting the join type to used for joining a LoRaWAN network.
   
   Parameters:
      Type - lorawan_join_type_t value representing the join type to use.
*/
#define lorawan_set_join_type(Type)    (_lorawan_config.joinType = Type)

/*
   lorawan_set_auto_reply()
   
   Function used to enable/disable the automatic reply, when enabled the
   LoRaWAN stack will transmit a packet without a payload immediately after a
   confirmed downlink message is received, or when the Frame Pending bit has
   been set by the server.
   
   Parameters:
      AutoReply - TRUE enable auto reply, FALSE disable auto reply.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
int1 lorawan_set_auto_reply(int1 AutoReply);

/*
   lorawan_set_ch_dr_range()
   
   Macro used to set the data rate range for the specified channel.
   
   Parameters:
      Ch - uint8_t value, from 0 to 71, specifying the channel to set the data
           rate range for.
      
      Min - uint8_t value, from 0 to 3, specifying the minimum data rate to use.
      
      Max - uint8_t value, from 0 to 3, specifying the maximum data rate to use.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
#define lorawan_set_ch_dr_range(Ch, Min, Max)   (_lorawan_set_ch_dr_range(Ch, Min, Max))

/*
   lorawan_enable_ch()
   
   Macro used to enable the specified channel for use by the LoRaWAN stack.
   
   Parameters:
      Ch - uint8_t value, from 0 to 71, specifying the channel to enable.
   
   Returns:
      TRUE - if channel was successfully enabled.
      FALSE - if an error occurred.
*/
#define lorawan_enable_ch(Ch)    (_lorawan_enable_ch(Ch))

/*
   lorawan_disable_ch()
   
   Macro used to disable the specified channel for use by the LoRaWAN stack.
   
   Parameters:
      Ch - uint8_t value, from 0 to 71, specifying the channel to disable.
   
   Returns:
      TRUE - if channel was successfully disabled.
      FALSE - if an error occurred.
*/
#define lorawan_disable_ch(Ch)      (_lorawan_disable_ch(Ch))


/*
   lorawan_set_power_index()
   
   Macro used set the output power to be used for the next transmission.
   
   Parameters:
      Power - lorawan_pwr_idx_t value to set representing the power index
              to set it to, Tx Power = 30 dBm - (2 * Power).
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
#define lorawan_set_power_index(Power)    (_lorawan_set_power_index(Power))

/*
   lorawan_set_uplink_counter()
   
   Macro used to set the uplink frame counter value to be used with the next
   uplink transmission.
   
   Parameters:
      Count - uint32_t value to set the uplink frame counter to.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
#define lorawan_set_uplink_counter(Count)    (_lorawan_set_uplink_counter(Count))

/*
   lorawan_set_downlink_counter()
   
   Macro used to set the downlink frame counter value to be used with the next
   downlink reception.
   
   Parameters:
      Count - uint32_t value to set the downlink frame counter to.
   
   Returns:
      TRUE - if setting was changed.
      FALSE - if setting was not changed.
*/
#define lorawan_set_downlink_counter(Count)     (_lorawan_set_downlink_counter(Count))

/*
   lorawan_get_sync_byte()
   
   Macro to get the currently configured synchronization byte.
   
   Returns:
      uint8_t value representing the current set synchronization byte.
*/
#define lorawan_get_sync_byte()     (_lorawan_config.Sync)

/*
   lorawan_get_device_eui()
   
   Macro to get the currently configured Device EUI.
   
   Parameters:
      eui - pointer to an uint8_t array to return Device EUI to.
*/
#define lorawan_get_device_eui(eui)    (memcpy(eui, _lorawan_config.DeviceEUI, LORAWAN_EUI_SIZE))

/*
   lorawan_get_app_eui()
   
   Macro to get the currently configured Application EUI.
   
   Parameters:
      eui - pointer to an uint8_t array to return Application EUI to.
*/
#define lorawan_get_app_eui(eui)    (memcpy(eui, _lorawan_config.AppEUI, LORAWAN_EUI_SIZE))

/*
   lorawan_get_app_key()
   
   Macro to get the currently configured Application Key.  The Application Key
   is used for OTAA joining and can not be read from module.  If joining type
   is ABP this value is unknown.
   
   Parameter:
      key - pointer to an uint8_t array to return the Application Key to.
*/
#define lorawan_get_app_key(key)    (memcpy(key, _lorawan_config.AppKey, LORAWAN_KEY_SIZE))

/*
   lorawan_get_network_session_key()
   
   Macro to get the currently configured Network Session Key.  The Network
   Session Key is used for ABP joining and can not be read from the module.  If
   joining type is OTAA this value is unknown.
   
   Parameters:
      key - pointer to an uint8_t array to return the Network Session Key to.
*/
#define lorawan_get_network_session_key(key)    (memcpy(key, _lorawan_config.NetworkSessionKey, LORAWAN_KEY_SIZE))

/*
   lorawan_get_app_session_key()
   
   Macro to get the currently configured Application Session Key.  The 
   Application Session Key is used for ABP joining and can not be read from the
   module.  If joining type is OTAA this value is unknown.
*/
#define lorawan_get_app_session_key(key)     (memcpy(key, _lorawan_config.AppSessionKey, LORAWAN_KEY_SIZE))

/*
   lorawan_get_device_address()
   
   Macro to get the currently configured Network Device Address.
   
   Returns:
      uint32_t value representing the Network Device Address.
*/
#define lorawan_get_device_address()      (_lorawan_config.DeviceAddr)

/*
   lorawan_get_retransmission_attempts()
   
   Macro to get the currently configured retransmission attempts done when
   messages is sent as a confirmed messages, and no confirmed response is
   received.
   
   Returns:
      uint8_t value representing the currently configured value.
*/
#define lorawan_get_retransmission_attempts()      (_lorawan_config.RetransmissionAttempts)

/*
   lorawan_get_rx_delay()
   
   Macro to get the currently configured delay time for the 1st RX window after
   a message is sent.
   
   Returns:
      uint16_t value representing the delay in milliseconds.
*/
#define lorawan_get_rx_delay()      (_lorawan_config.RXDelay)

/*
   lorawan_get_rx2_data_rate()
   
   Macro to get the currently configured data rate for the 2nd RX window after
   a message is sent.
   
   Returns:
      uint8_t value, from 8 to 13, representing the data rate.
*/
#define lorawan_get_rx2_data_rate()    (_lorawan_config.RX2DataRate)

/*
   lorawan_get_rx2_frequency()
   
   Macro to get the currently configured frequency for the 2nd RX window after 
   a message is sent.
   
   Returns:
      uint32_t value representing the frequency.
*/
#define lorawan_get_rx2_frequency()    (_lorawan_config.RX2Frequency)

/*
   lorawan_get_join_type()
   
   Macro to get the currently configured join type.
   
   Returns:
      lorawan_join_type_t value representing the configured join type.
*/
#define lorawan_get_join_type()        (_lorawan_config.joinType)

/*
   lorawan_get_auto_reply()
   
   Macro to get the currently configured auto reply setting.
   
   Returns:
      TRUE - auto reply enabled.
      FALSE - auto reply disabled.
*/
#define lorawan_get_auto_reply()    (_lorawan_config.AutoReply)

/*
   lorawan_get_ch_status()
   
   Macro used to get the current status of the specified channel, including
   the channel frequency, minimum data rate, maximum data rate and if channel
   is enabled to be used by the LoRaWAN stack.
   
   Parameters:
      Ch - uint8_t value, from 0 to 71, specifying the channel to get the info
           for.
      
      Status - pointer to lorawan_ch_status_t variable to return the channel 
               info to.
   
   Returns:
      TRUE - if info was read successfully from module.
      FALSE - if an error occurred while reading info.
*/
#define lorawan_get_ch_status(Ch, Status)    (_lorawan_get_ch_status(Ch, Status))

/*
   lorawan_get_uplink_counter()
   
   Macro to get the uplink frame counter value that will used with the next
   uplink transmission.
   
   Returns:
      uint32_t value representing the uplink frame counter value.
*/
#define lorawan_get_uplink_counter()      (_lorawan_get_uplink_counter())

/*
   lorawan_get_downlink_counter()
   
   Macro to get the uplink frame counter value that will be used with the next
   downlink reception.
   
   Returns:
      uint32_t value representing the downlink frame counter value.
*/
#define lorawan_get_downlink_counter()    (_lorawan_get_downlink_counter())

/*
   lorawan_get_join_state()
   
   Function to get the current join state of the module.
   
   Returns:
      lorawan_join_state_t - indicating the join state of the module.
*/
lorawan_join_state_t lorawan_get_join_state(void);

/*
   lorawan_get_tx_error()
   
   Function to get the error state for the last sent message.
   
   Returns:
      lorawan_tx_error_t - indicating the error for the last transmitted
                           message.
*/
lorawan_tx_error_t lorawan_get_tx_error(void);

/*
   lorawan_get_mode()
   
   Function to get the current mode of the lorawan driver's state machine.
   
   Returns:
      lorawan_mode_t - indicating the current mode of the lorawan driver's
                       state machine.
*/
lorawan_mode_t lorawan_get_mode(void);
///////////////////////////////////////////////////////////////////////////////

void _lorawan_set_join_state(lorawan_join_state_t State);


#endif
