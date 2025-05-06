///////////////////////////////////////////////////////////////////////////////
////                                lora.h                                 ////
////                                                                       ////
////  Driver for creating a P2P LoRa network.                              ////
////                                                                       ////
////  This driver is setup so that an end device initiates communication   ////
////  by sending a message to a base station or other end device and       ////
////  receiving a response back from the base station or end device the    ////
////  message was sent to.  Meaning the driver only handles one message    ////
////  pair at a time.                                                      ////
////                                                                       ////
////  The topology of the network can be setup in several ways, 1 end      ////
////  device and 1 gateway, multiple end devices and 1 gateway or multiple ////
////  end devices.  If there are to be multiple end device they should all ////
////  be setup as listening end devices, so they can keep track of which   ////
////  channel the network is currently communicating on.                   ////
////                                                                       ////
////  This driver uses a channel hopping to determine which channel is     ////
////  currently being used by the network for communication, each channel  ////
////  is used once then it increments to the next channel in the table     ////
////  until all hopping channels are used then it starts at the beginning  ////
////  of the table.  Additionally it has a tick time associated with each  ////
////  hopping channel to keep it from transmitting more frequently then is ////
////  allowed by FCC regulations.  The driver assumes that each channel is ////
////  transmitted on for the maximum time allowed by each device.          ////
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
   LORA PREPROCESSOR
   ----------------------------------------------------------------------------
   
   LORA_DEFAULT_SYNC_BYTE
      The sync byte used during communication, defaults to 0x66 if not defined
      before this driver is included in project.
   
   LORA_DEFAULT_PWR
      The default transceiver output power, 2 to 20 dBm, defaults to 2 dBm if
      not defined before this driver is included in project.
   
   LORA_DEFAULT_SF
      The default spreading factor to use, LORA_SF_7 to LORA_SF_12, default to
      LORA_SF_7 if not defined before this driver is included.  This combined
      with Bandwidth sets the data rate that data is transmitted with.
      LORA_SF_7 is the highest and LORA_SF_12 is the lowest.
   
   LORA_CRC_ENABLED
      Used to enable or disable the hardware CRC header for communication,
      defaults to TRUE, enabled, if not defined before this driver is included.
   
   LORA_IQI_ENABLED
      Used to enable of disable the invert IQ for communication, defaults to 
      FALSE, disabled, if not defined before this driver is included.
   
   LORA_DEFAULT_CR
      The default coding rate to use, LORA_CR_45 to LORA_CR_48, defaults to
      LORA_CR_45 if not defined before this driver is included.  The coding
      rate sets the proportion of the transmitted bit that actually carry
      information.
   
   LORA_DEFAULT_BW
      The default band width to use, LORA_BW_125, LORA_BW_250 or LORA_BW_500, 
      defaults to LORA_BW_125 if not defined before this driver is include.
      This combined with the spreading factor sets the data rate that data is
      transmitted with.  LORA_BW_500 is the highest and LORA_BW_125 is the
      lowest.
   
   LORA_DEVICE_TYPE
      This set the type of device this unit is, can be one of the following:
         LORA_DEVICE_TYPE_ED - End Device, initiates communication with other
                               devices by sending a message, only listens for a
                               message for LORA_RX_TIME after sending a
                               message.
         LORA_DEVICE_TYPE_BS - Base Station, listens for messages all the time,
                               except when transmitting.  Never initiates
                               communication, only responses after receiving a
                               message from an End Device.
         LORA_DEVICE_TYPE_LED - Listening End Device, listens for message all
                                the time, except when transmitting.  Can either
                                initiate communication, like an End Device, or
                                responded to a message like a Base Station.
      
      Defaults to LORA_DEVICE_TYPE_ED if not defined before this driver is
      included.
      
   LORA_BS_DEVICE_ADDR
      This sets the address of the Base Station in the network, default to 
      0 if not defined before this driver is included.
   
   LORA_DEVICE_ADDR
      This sets the address of this unit.  If this unit is the Base Station it
      should be set to the same as LORA_BS_DEVICE_ADDR.  If not defined before
      this driver is include, defaults to LORA_BS_DEVICE_ADDR if
      LORA_DEVICE_TYPE is defined as LORA_DEVICE_TYPE_BS, or
      (LORA_BS_DEVICE_ADDR + 1) if LORA_DEVICE_TYPE is not defined as
      LORA_DEVICE_TYPE_BS.
   
   LORA_MAX_PAYLOAD_SIZE
      This sets the maximum payload size that can be sent or received, defaults
      to 64 bytes if not defined before this driver is included.  Also be aware
      the maximum number of bytes that can be transmitted is dependent on the
      bit rate, so the bit rate may effect the max size of the payload that can
      be sent.
   
   LORA_CHANNELS
      Sets the number of channels in the frequency spectrum that the unit uses,
      defaults to 25 if not specified.  This only sets the number of
      frequencies that will be used, not which frequencies are use.
   
   LORA_RX_TIME
      This set the time in milliseconds that an end device will wait for a 
      response after initiating communication, defaults to 2000 ms if not
      defined before this driver is included.
   
   LORA_USE_MESSAGE_CRC
      Optional if defined it will add a 16-bit CRC value to message done on the
      data portion of the message, not defined by default.  The data portion is
      everything from the destination address to the end of the utilized 
      payload data, see _lora_message_t.  When the CRC is used it requires some
      extra functions be provided to the driver, see LORA REQUIRED CRC
      FUNCTIONS below.
   
   LORA_USE_MESSAGE_CHECKSUM
      Optional if defined it will add an 8-bit Checksum value to the message
      done on the data portion of the message, not defined by default.  The
      data portion is everything from the destination address to the end of the 
      utilized payload data, see _lora_message_t.  Only one data integrity 
      check method can be used, either LORA_USE_MESSAGE_CRC or 
      LORA_USE_MESSAGE_CHECKSUM, the way the code is implemented
      LORA_USE_MESSAGE_CRC takes precedence.
   
   LORA REQUIRED TICK FUNCTION TYPES and DEFINES
   ----------------------------------------------------------------------------
   
   ** The following functions, types and defines need to be provided to the
      lora.c driver before it's include in your project.
   
   _lora_tick_t - the tick type.
   
   _LORA_TICKS_PER_SECOND - a define indicating the number of tick per second.
   
   _lora_tick_get() - function to get the current tick timer count.
   
   _lora_tick_difference(a,b) - function to calculate the difference between
                                two tick timer count values, first parameter is
                                the most recent tick timer count value and the
                                second parameter is the older tick timer count
                                value.
   
   LORA REQUIRED MODULE FUNCTIONS
   ----------------------------------------------------------------------------
   
   ** The following function need to be provided to the lora.c driver before
      it's include in your project.  The CCS C driver rn2903.c already has all
      the following functions provided in it.
   
   _lora_init_radio() - function to initialize the radio module, returns TRUE
         if successful and FALSE if unsuccessful.
   
   _lora_set_radio_config() - function to set the radio configuration, returns
         TRUE if successful and FALSE if unsuccessful.
   
   _lora_stop_rx() - function to stop the radio for receiving.
   
   _lora_set_channel(ch) - function to set the radio to the specified channel.
   
   _lora_start_listening() - function to set the radio into receive mode, 
         returns TRUE if successful and FALSE if unsuccessful.
                             
   _lora_rx_task() - task function to receive info from the radio module.
   
   _lora_kbhit() - function to determine if radio module has a message to read,
         returns TRUE if it has a message to read and FALSE if it doesn't have
         a message to read.
                   
   _lora_get_radio_message(dPtr, lPtr) - function used to retrieve a message
         from the radio module.  dPtr is a pointer to an uint8_t array to
         return rx data to, lPtr is a pointer to an uint8_t variable to return
         number of data bytes returned to dPtr.  Function returns 
         lora_module_resp_t value indicating message type return from radio
         module, dPtr and lPtr only need to be valid when return value is 
         LORA_MODULE_RESP_RX_DATA.  In that case dPtr should only be the RX
         data and RX data length.
   
   _lora_putd(dPtr, c) - function used to transmit data with the radio module.
         dPtr is a pointer to an uint8_t array containing data to transmit, and
         c is the number of bytes to transmit.  Function returns TRUE if it 
         accepted the data, and returns FALSE if an error occurred.
   
   _lora_get_rx_rssi() - function used to get the RSSI value of the last
         received message.  Return value is an int8_t value representing the 
         RSSI of the last received message.
   
   _lora_get_rx_snr() - function used to get the SNR value of the last received
         message.  Return value is an int8_t value representing the SNR of the
         last received message.
      
   _lora_set_sync(v) - function used to set the synchronization byte used by
         the radio module.  v is the synchronization byte to set, function
         returns TRUE if successful and FALSE if unsuccessful.
   
   _lora_set_pwr(v) - function used to set the transmit power used by the radio
         module to send messages.  v is the power to set, the driver will pass
         an uint8_t from 2 to 20, in dBm, to the function.  Function returns
         TRUE if successful and FALSE if unsuccessful.
   
   _lora_set_sf(v) - function used to set the spreading factor used by the
         radio module.  v is the spreading factor to set, the driver will pass
         a lora_sf_t value from LORA_SF_7 to LORA_SF_12 to the function.
         Function returns TRUE if successful, and FALSE if unsuccessful.
   
   _lora_set_cr(v) - function used to set the coding rate used by the radio 
         module.  v is the coding rate to set, the driver will pass a lora_cr_t
         value from LORA_CR_45 to LORA_CR_48 to the function.  Function returns
         TRUE if successful and FALSE if unsuccessful.
   
   _lora_set_bw(v) - function used to set the bandwidth used by the radio
         module.  v is the bandwidth to set, the driver will pass a lora_bw_t
         value from LORA_BW_125 to LORA_BW_500 to the function.  Function
         returns TRUE if successful and FALSE if unsuccessful.
   
   _lora_enable_crc(v) - function used to enable and disable whether the radio
         module uses it's CRC when sending/receiving messages.  v will be
         TRUE to enable and FALSE to disable.  Function will return TRUE if
         successful and FALSE if unsuccessful.
   
   _lora_enable_iqi(v) - function used to enable and disable the radio module's
         iqi function.  v will be TRUE to enable and FALSE to disable.  
         Function will return TRUE if successful and FALSE if unsuccessful.
   
   ** Some of the above functions, _lora_set_radio_config() for example, 
      require some types and global variables that are declared in lora.h.  So
      you'll need to include the lora.h file before those functions are
      implemented and/or declared.
   
   LORA REQUIRED CRC FUNCTIONS (only need if LORA_USE_MESSAGE_CRC is defined)
   ----------------------------------------------------------------------------
   
   _lora_crc_init() - function for initializing the CRC.
   
   _lora_crc_init_value() - function for initializing the CRC generator to a 
         specific value before doing the calculation.
  
   _lora_crc_calc() - function for calculating the CRC value, this driver is 
         expecting the CRC to be a 16-bit CRC.
*/


#ifndef __LORA_H__
#define __LORA_H__

#ifndef debug_lora_printf
 #define debug_lora_printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#else
 #if !definedinc(debug_lora_putc)
  void debug_lora_putc(c)
  {
   putc(c);
  }
 #endif
#endif

#ifndef LORA_MAX_PAYLOAD_SIZE
 #define LORA_MAX_PAYLOAD_SIZE      64
#endif

typedef enum
{
   LORA_SF_7 = 7,
   LORA_SF_8,
   LORA_SF_9,
   LORA_SF_10,
   LORA_SF_11,
   LORA_SF_12
} lora_sf_t;

typedef enum
{
   LORA_CR_45 = 1,
   LORA_CR_46,
   LORA_CR_47,
   LORA_CR_48
} lora_cr_t;

typedef enum
{
   LORA_BW_125 = 125,   //125 kHz
   LORA_BW_250 = 250,   //250 kHz
   LORA_BW_500 = 500    //500 kHz
} lora_bw_t;

typedef enum
{
   LORA_DEVICE_TYPE_ED,    //End Device - idle when not transmitting, only listens for LORA_RX_TIME after transmitting data.  Always initiates communication with BS or Listening End Device.
   LORA_DEVICE_TYPE_BS,    //Base Station - always listening for incoming message, except when transmitting.  Never initiates transmission only responses after it receive a message from an End Device or Listening End Device.
   LORA_DEVICE_TYPE_LED    //Listening End Device - always listening for incoming message, except when transmitting.  Can initiate communication with BS or other Listening End Devices, can respond to an End Device or a Listening End Device that initiated communication with it.
} lora_device_type_t;

typedef enum
{
   LORA_MODULE_RESP_NONE,     //No response received from LoRa module
   LORA_MODULE_RESP_RX_DATA,  //RX data was received from LoRa module
   LORA_MODULE_RESP_TX_OK,    //TX was successful by LoRa module
   LORA_MODULE_RESP_ERR,      //RX timed out or TX was unsuccessful
   LORA_MODULE_RESP_UNKNOWN   //Unknown response from LoRa module  
} lora_module_resp_t;

typedef struct
{
   uint8_t Address;
   uint8_t Data[LORA_MAX_PAYLOAD_SIZE];
   uint8_t Length;
   int8_t rssi;
   int8_t snr;
} lora_rx_message_t;

#ifndef NULL
 #define NULL  0
#endif

///////////////////////////////////////////////////////////////////////////////

struct
{
   uint8_t sync;           //Sync byte
   uint8_t pwr;            //TX Power 2 to 20 dB
   lora_sf_t sf;           //Spreading Factor
   lora_cr_t cr;           //Coding Rate
   lora_bw_t bw;           //Bandwidth
   int1 crc;
   int1 iqi;
} _lora_config;

uint32_t _g_LoraStartFreq;
uint32_t _g_LoraIncFreq;

///////////////////////////////////// API /////////////////////////////////////

/*
   lora_init()
   
   Used to initialize the LoRa P2P driver and LoRa transceiver module, must be
   called before any other LoRa functions or macros.  
   
   Parameters:
      ChHoppingTable - optional parameter used to set the LoRa channel hopping
      table that will be used.  If used size of array, in bytes, must be the
      same as LORA_CHANNELS.  Additionally the channels uses must be range
      valid for the bandwidth being used, 0-129 for 125 kHz, 0-64 for 250 kHz
      and 0-31 for 500 kHz.
   
   Returns:
      TRUE - if initialized successfully.
      FALSE - if it failed initialization.
*/
int1 lora_init(uint8_t *ChHoppingTable=NULL);

/*
   lora_task()
   
   Task function used to send and receive messages.  Should be called fairly
   frequently, at least once per iteration of main() loop is recommended.
*/
void lora_task(void);

/*
   lora_tbe()
   
   Function used to determine if transmit buffer is empty.
   
   Returns:
      TRUE - if transmit buffer is empty.
      FALSE - if transmit buffer is not empty.
*/
int1 lora_tbe(void);

/*
   lora_put_message()
   
   Function used to load message into transmit buffer to send.
   
   Parameters:
      DestAddr - address of device to send message to, e.g. 0 default base
                 station address.
      Payload - pointer to an uint8_t array containing payload of message to
                send.
      Length - number of bytes in payload.
   
   Returns:
      TRUE - if message was successfully loaded into transmit buffer to send.
      FALSE - if message was not loaded into transmit buffer, an error occurred
              e.g. buffer was already full or state machine is in incorrect
              mode.
*/
int1 lora_put_message(uint8_t DestAddr, uint8_t *Payload, uint8_t Length);

/*
   lora_busy()
   
   Function used to determine if lora driver is currently busy sending and 
   receiving a message.
   
   Returns:
      TRUE - if busy.
      FALSE - if not busy.
*/
int1 lora_busy(void);

/*
   lora_has_message()
   
   Function used to determine if there is a new message to retrieve from 
   receive buffer.
   
   Returns: 
      TRUE - if receive buffer has a new message to retrieve from buffer.
      FALSE - if there is no new message to retrieve from buffer.
*/
int1 lora_has_message(void);

/*
   lora_get_message()
   
   Function used to retrieve a new message from receive buffer.
   
   Parameters:
      Message - pointer to lora_rx_message_t type variable to return the
                receive message to.
   
   Returns:
      TRUE - if a new message was retrieve from receive buffer.
      FALSE - if no new message was retrieved from receive buffer.
*/
int1 lora_get_message(lora_rx_message_t *Message);

/*
   lora_set_channel_index()
   
   Function used to set the index in the channel hopping table to use for
   transmitting/receiving messages.  This function is useful for end-devices
   that only power on periodically to send a message, this way assuming the
   base station hasn't change channels it will limit the amount of time sends
   power on and transmitting.  If used on a base station or listening end-
   device that is listening it will cause it to stop listening, switch channels
   and start listening on new channel.
   
   Parameters:
      Index - the index in the channel hopping table to transmit/receive
              messages on.
      
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_set_channel_index(uint8_t Index);

/*
   lora_set_sync_byte()
   
   Function that can be used to set the synchronization byte used as the
   preamble during communication.
   
   Parameters:
      sync - the uint8_t value to set the synchronization byte to.
   
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_set_sync_byte(uint8_t sync);

/*
   lora_set_power()
   
   Function that can be used to set the transceivers TX power.
   
   Parameters:
      pwr - the power to set the transceiver to, 2 to 20 dBm.
   
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_set_power(uint8_t pwr);

/*
   lora_set_spreading_factor()
  
   Function that can be used to set the spreading factor to use.
  
   Parameters:
      sf - lora_sf_t value to set the spreading factor to, LORA_SF_7 to 
           LORA_SF_12.
   
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_set_spreading_factor(lora_sf_t sf);

/*
   lora_set_coding_rate()
   
   Function that can be used to set the coding rate to use.
   
   Parameters:
      cr - lora_cr_t value to set the coding rate to, LORA_CR_45 to LORA_CR_48.
   
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_set_coding_rate(lora_cr_t cr);

/*
   lora_set_bandwidth()
   
   Function that can be used to set the bandwidth to use.
   
   Parameters:
      bw - lora_bw_t value to set the bandwidth to, LORA_BW_125, LORA_BW_250 or
           LORA_BW_500.
   
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_set_bandwidth(lora_bw_t bw);

/*
   lora_enable_crc()
   
   Function that can be used to enable or disable the LoRa modules crc.
   
   Parameters:
      enable - TRUE enables crc. and FALSE disables crc.
   
   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_enable_crc(int1 enable);

/*
   lora_enable_iqi()
   
   Function that can be used to enable or disable the LoRa modules iqi.
   
   Parameters:
      enable - TRUE enables iqi, and FALSE disables iqi.

   Returns:
      TRUE - if successful.
      FALSE - if unsuccessful.
*/
int1 lora_enable_iqi(int1 enable);

/*
   lora_set_channels()
   
   Function that can be used to set the number of channels the driver uses.
   
   Parameters:
      channels - the number of channels to use, must be less then or equal to
                 the LORA_CHANNELS define.
   
   Returns:
      TRUE - if set successfully.
      FALSE - if not set.
*/
int1 lora_set_channels(uint8_t channels);

/*
   lora_set_hopping_table()
   
   Function that can be used to set the channel hopping table to use.
   
   Parameters:
      Table - pointer to uint8_t array contain channel hopping table.  Tables
              size must be at least the same size as what is currently set
              for the number of channels.  No check is made to make sure
              channels aren't reused, if that is done it will cause a situation
              were it's possible to violate FCC regulations on how long a
              device can transmit on a single channel.
*/
void lora_set_hopping_table(uint8_t *Table);

/*
   lora_set_device_type()
   
   Function that can be used to set the device type.
   
   Parameters:
      Type - lora_device_type_t value to set the device to.
   
   Returns:
      TRUE - if successful.
      FALSE - if not successful.
*/
int1 lora_set_device_type(lora_device_type_t Type);

/*
   lora_get_hopping_table()
   
   Function to get the channel hopping table that is currently being used.
   
   Parameters:
      Table - pointer to an uint8_t array to return the channel hopping table
              to.
   
   Returns:
      uint8_t value representing the size of the table in bytes.
*/
uint8_t lora_get_hopping_table(uint8_t *Table);

/*
   lora_set_device_address()
   
   Macro that can be used to set the device address of the device.
   
   Parameters:
      Address - uint8_t value to set the device address to.
*/
#define lora_set_device_address(v)     (_g_Lora.DeviceAddress = v)

/*
   lora_get_sync_byte()
   
   Macro to get the synchronization byte being used by the LoRa module.
   
   Returns:
      uint8_t value that represents the currently configured synchronization
      byte.
*/
#define lora_get_sync_byte()           (_lora_config.sync)

/*
   lora_get_power()
   
   Macro to get the transmit power used by the LoRa module.
   
   Returns:
      uint8_t value that represents the current configured tx power in dBm.
*/
#define lora_get_power()               (_lora_config.pwr)

/*
   lora_get_spreading_factor()
   
   Macro to get the spreading factor used by the LoRa module.
   
   Returns:
      lora_sf_t value that represents the current configured spread factor.
*/
#define lora_get_spreading_factor()    (_lora_config.sf)

/*
   lora_get_coding_rate()
   
   Macro to get the coding rate used by the LoRa module.
   
   Returns:
      lora_cr_t value that represents the current configured coding rate.
*/
#define lora_get_coding_rate()         (_lora_config.cr)

/*
   lora_get_bandwidth()
   
   Macro to get the bandwidth used by the LoRa module.
   
   Returns:
      lora_bw_t value that represents the current configured bandwidth.
*/
#define lora_get_bandwidth()           (_lora_config.bw)

/*
   lora_get_crc()
   
   Macro to get whether the LoRa modules CRC function is enabled.
   
   Returns:
      TRUE - if enabled.
      FALSE - if disabled.
*/
#define lora_get_crc()                 (_lora_config.crc)

/*
   lora_get_iqi()
   
   Macro to get whether the LoRa modules IQI function is enabled.
   
   Returns:
      TRUE - if enabled.
      FALSE - if disabled.
*/
#define lora_get_iqi()                 (_lora_config.iqi)

/*
   lora_get_channels()
   
   Macro to get the number of channels the LoRa P2P driver is setup to use.
   
   Returns:
      uint8_t value representing the number of channels driver is setup to use.
*/
#define lora_get_channels()            (_g_LoraCh.Channels)


/*
   lora_get_device_type()
   
   Macro that can be used to get the device type of the device.
   
   Returns:
      lora_device_type_t value representing the device type of the device.
*/
#define lora_get_device_type()         (_g_Lora.DeviceType)

/*
   lora_get_device_address()
   
   Marco that can be used to get the device address of the device.
   
   Returns:
      uint8_t value representing the device address of the device
*/
#define lora_get_device_address()      (_g_Lora.DeviceAddress)

#endif
