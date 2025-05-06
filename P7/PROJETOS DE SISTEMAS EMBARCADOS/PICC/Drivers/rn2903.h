///////////////////////////////////////////////////////////////////////////////
////                               rn2903.h                                ////
////                                                                       ////
////  Driver for Microchip's RN2903 Low-Power Long Range LoRa transceiver  ////
////  module.                                                              ////
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
   RN2903 PREPROCESSORS
   ----------------------------------------------------------------------------
   
   RN2903_TX_PIN
      Used to set the TX pin used for the UART communication with the RN2903
      module.  Defaults to PIN_C6 if not defined before this driver is included
      in the project.
   
   RN2903_RX_PIN
      Used to set the RX pin used for the UART communication with the RN2903
      module.  Defaults to PIN_C7 if not defined before this driver is included
      in the project.
   
   RN2903_BAUD
      Used to set the UART baud rate used to communication with the RN2903 
      module.  Defaults to 57600 if not defined before this driver is included
      in the project.  Currently only 57600 is supported by this driver.
   
   RN2903_SERIAL_BUFFER_SIZE
      Used to set the size of the serial buffer used by the driver, if defined
      to anything greater then 0 driver will use the appropriate interrupt to
      receive the data.  Defaults to 64 if not defined before this driver is
      included in the project.
   
   RN2903_RX_MESSAGE_BUFFERS
      Used to set the RN2903 message buffer size, the number of new received 
      messages the driver can hold before losing messages.  Defaults to 1 if
      not defined before this driver is included in the project.
   
   RN2903_RX_MESSAGE_BUFFER_SIZE
      Used to set the maximum size of the message that the RN2903 message 
      buffer can hold.  Defaults to 64 if not defined before this driver is
      included in the project.
*/

#ifndef __RN2903_H__
#define __RN2903_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __PCD__
 #ifndef _PACKED_STRUCT
  #define _PACKED_STRUCT   __attribute__((__packed__))
 #endif
#else
 #ifndef _PACKED_STRUCT
  #define _PACKED_STRUCT
 #endif
#endif

#ifndef NULL
 #define NULL  0
#endif

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
// RN2903 Types                  //
///////////////////////////////////

typedef struct
{
   uint32_t Frequency;
   uint8_t DataRateMin;
   uint8_t DataRateMax;
   int1 Status;
} mac_ch_info_t;

typedef enum
{
   RN2903_EC_OK,
   RN2903_EC_BUSY,
   RN2903_EC_FRAM_COUNTER_ERR_REJOIN_NEEDED,
   RN2903_EC_INVALID_CLASS,
   RN2903_EC_INVALID_DATA_LEN,
   RN2903_EC_INVALID_PARAM,
   RN2903_EC_KEYS_NOT_INIT,
   RN2903_EC_MAC_PAUSED,
   RN2903_EC_MULICAST_KEYS_NO_SET,
   RN2903_EC_NO_FREE_CH,
   RN2903_EC_NOT_JOINED,
   RN2903_EC_SILENT,
   RN2903_EC_ERR,
   RN2903_EC_NO_RESPONSE = 0xFE,
   RN2903_EC_UNKNOWN = 0xFF
} rn2903_ec_t;

typedef enum
{
   RN2903_RESP_NONE,
   RN2903_RESP_MAC_TX_OK,
   RN2903_RESP_MAC_RX,
   RN2903_RESP_MAC_ERR,
   RN2903_RESP_INVALID_DATA_LEN,
   RN2903_RESP_DENIED,
   RN2903_RESP_ACCEPTED,
   RN2903_RESP_RADIO_RX,
   RN2903_RESP_RADIO_ERR,
   RN2903_RESP_RADIO_TX_OK,
   RN2903_RESP_VALUE
} rn2903_resp_t;

typedef enum
{
   RN2903_PIN_GPIO0, RN2903_PIN_GPIO1, RN2903_PIN_GPIO2, RN2903_PIN_GPIO3,
   RN2903_PIN_GPIO4, RN2903_PIN_GPIO5, RN2903_PIN_GPIO6, RN2903_PIN_GPIO7,
   RN2903_PIN_GPIO8, RN2903_PIN_GPIO9, RN2903_PIN_GPIO10, RN2903_PIN_GPIO11,
   RN2903_PIN_GPIO12, RN2903_PIN_GPIO13, RN2903_PIN_UART_CTS, RN2903_PIN_UART_RTS,
   RN2903_PIN_TEST0, RN2903_PIN_TEST1
} rn2903_pin_t;

typedef enum
{
   RN2903_PIN_MODE_DIGITAL_OUTPUT,
   RN2903_PIN_MODE_DIGITAL_INPUT,
   RN2903_PIN_MODE_ANALOG
} rn2903_pin_mode_t;

typedef enum
{
   MAC_TX_TYPE_CONFIRMED,
   MAC_TX_TYPE_UNCONFIRMED
} mac_tx_type_t;

typedef enum
{
   MAC_JOIN_MODE_OTAA,
   MAC_JOIN_MODE_ABP
} mac_join_mode_t;

typedef enum
{
   MAC_KEY_TYPE_APP,          //application key
   MAC_KEY_TYPE_APPS,         //application session key
   MAC_KEY_TYPE_MCAST_APPS,   //multicast application session key
   MAC_KEY_TYPE_MCAST_NWKS,   //multicast network session key
   MAC_KEY_TYPE_NWKS          //network session key
} mac_key_type_t;

typedef enum
{
   MAC_DEVICE_CLASS_A,
   MAC_DEVICE_CLASS_C
} mac_device_class_t;

typedef enum
{
   MAC_FRAME_COUNTER_TYPE_DL,
   MAC_FRAME_COUNTER_TYPE_MCAST_DL,
   MAC_FRAME_COUNTER_TYPE_UL
} mac_frame_counter_type_t;

typedef enum
{
   MAC_NETWORK_ADDR_TYPE_DEV_ADDR,
   MAC_NETWORK_ADDR_TYPE_MCAST_DEV_ADDR
} mac_network_addr_type_t;

typedef enum
{
   MAC_EUI_TYPE_APP,
   MAC_EUI_TYPE_DEV,
} mac_eui_type_t;

typedef struct
{
   uint8_t DataRate;
   uint32_t Frequency;
} rx2_struct_t;

typedef enum
{
   MAC_RX_DELAY_TYPE_DELAY_1,
   MAC_RX_DELAY_TYPE_DELAY_2
} mac_rx_delay_type_t;

typedef enum
{
   MAC_STATE_IDLE,
   MAC_STATE_TRANSMITTING,
   MAC_STATE_BEFORE_RX_WINDOW_1,
   MAC_STATE_RX_WINDOW_1_OPEN,
   MAC_STATE_BETWEEN_WINDOWS,
   MAC_STATE_RX_WINDOW_2_OPEN,
   MAC_STATE_RETRANSMISSION_DELAY,
   MAC_STATE_APB_DELAY,
   MAC_STATE_CLASS_C_RX2_1_OPEN,
   MAC_STATE_CLASS_C_RX2_2_OPEN
} mac_state_t;

typedef union
{
   struct
   {
      mac_state_t State:4;
      uint8_t Joined:1;                //0 - network not joined, 1 - network joined
      uint8_t AutoReply:1;             //0 - disabled, 1 - enabled
      uint8_t ADR:1;                   //0 - ADR disabled, 1 - ADR enabled
      uint8_t Silent:1;                //0 - disabled, 1 - enabled
      uint8_t Paused:1;                //0 - MAC is not paused, 1 - MAC is paused
      uint8_t RxDone:1;                //0 - RX Data is not ready, 1 - RX Data is ready
      uint8_t LinkCheck:1;             //0 - link check is disabled, 1 - link check is enabled
      uint8_t ChannelsUpdated:1;       //0 - not updated, 1 - updated
      uint8_t OutputPowerUpdated:1;    //0 - not updated, 1 - updated
      uint8_t NbRepUpdated:1;          //0 - not updated, 1 - updated
      uint8_t PrescalerUpdated:1;      //0 - not updated, 1 - updated
      uint8_t RX2WindowUpdated:1;      //0 - not updated, 1 - updated
      uint8_t RXTimingUpdated:1;       //0 - not updated, 1 - updated
      uint8_t RejoinNeeded:1;          //0 - end-device functional, 1 - end-device not functional and rejoin is needed
      uint8_t Multicast:1;             //0 - Multicast disabled, 1 - Multicast enabled
      uint8_t r19_23:5;                //reserved bits 19-23
      uint8_t r24_31;                  //reserved bits 24-31
   };
   uint32_t dw;
} mac_status_t;

typedef enum
{
   RADIO_BW_FREQ_250K = 2500,
   RADIO_BW_FREQ_125K = 1250,
   RADIO_BW_FREQ_62P5K = 625,
   RADIO_BW_FREQ_31P3K = 313,
   RADIO_BW_FREQ_15P6K = 156,
   RADIO_BW_FREQ_7P8K = 78,
   RADIO_BW_FREQ_3P9K = 39,
   RADIO_BW_FREQ_200K = 2000,
   RADIO_BW_FREQ_100K = 1000,
   RADIO_BW_FREQ_50K = 500,
   RADIO_BW_FREQ_25K = 250,
   RADIO_BW_FREQ_12P5K = 125,
   RADIO_BW_FREQ_6P3K = 63,
   RADIO_BW_FREQ_3P1K = 31,
   RADIO_BW_FREQ_166P7K = 1667,
   RADIO_BW_FREQ_83P3K = 833,
   RADIO_BW_FREQ_41P7K = 417,
   RADIO_BW_FREQ_20P8K = 208,
   RADIO_BW_FREQ_10P4K = 104,
   RADIO_BW_FREQ_5P2K = 52,
   RADIO_BW_FREQ_2P6K = 26
} radio_bw_freq_t;

typedef enum
{
   RADIO_GFSK_MODULATION_NONE = 1,
   RADIO_GFSK_MODULATION_1P0,
   RADIO_GFSK_MODULATION_0P5,
   RADIO_GFSK_MODULATION_0P3
} radio_gfsk_modulation_t;

typedef enum
{
   RADIO_BANDWIDTH_125K = 125,
   RADIO_BANDWIDTH_250K = 250,
   RADIO_BANDWIDTH_500K = 500
} radio_bandwidth_t;

typedef enum
{
   RADIO_CODING_RATE_4BY5 = 1,
   RADIO_CODING_RATE_4BY6,
   RADIO_CODING_RATE_4BY7,
   RADIO_CODING_RATE_4BY8
} radio_coding_rate_t;

typedef enum
{
   RADIO_MODULATION_LORA = 1,
   RADIO_MODULATION_FSK
} radio_modulation_t;

typedef enum
{
   RADIO_SPREADING_FACTOR_SF7 = 7,
   RADIO_SPREADING_FACTOR_SF8,
   RADIO_SPREADING_FACTOR_SF9,
   RADIO_SPREADING_FACTOR_SF10,
   RADIO_SPREADING_FACTOR_SF11,
   RADIO_SPREADING_FACTOR_SF12
} radio_spreading_factor_t;

typedef struct _PACKED_STRUCT
{
   uint16_t Version;
   uint16_t MaxPacketSize; //not used by RN2903 version of Microchip's standard 8-bit UART Booloader
   uint16_t AckPacketSize; //not used by RN2903 version of Microchip's standard 8-bit UART Booloader
   uint16_t DeviceID;
   uint16_t res8_9;
   uint8_t EraseSize;
   uint8_t WriteSize;
   union
   {
      struct
      {
         uint8_t UserId1;
         uint8_t UserId2;
         uint8_t UserId3;
         uint8_t UserId4;
      };
      uint32_t UserId;
   };
} rn2903_bl_version_t;
   
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
// RN2903 System Functions       //
///////////////////////////////////

/*
   RN2903Sleep()
   
   Function used to put the RN2903 module to sleep for specified time.
   
   Parameters:
      milliseconds - uint32_t value indicating number of milliseconds to put
                     module to sleep for.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RN2903Sleep(uint32_t milliseconds);

/*
   RN2903Reset()
   
   Function used to reset the RN2903 module.
   
   Parameters:
      Str - pointer to an unsigned char array to return the return the reset
            string to after the module resets.
*/
void RN2903Reset(unsigned char *Str);

/*
   RN2903EraseFW()
   
   Function to erase the FW on the RN2903 module, after which the module will
   enter bootloader mode to load new firmware.
*/
void RN2903EraseFW(void);

/*
   RN2903FactoryReset()
   
   Function to reset the module's configuration data and user EEPROM to factory
   default value and restarts the module.
   
   Parameters:
      Str - pointer to an unsigned char array to return the reset string to
            after the module resets.
*/
void RN2903FactoryReset(unsigned char *Str);

/*
   RN2903WriteNVM()
   
   Function to write a byte of data to the user EEPROM on the module.
   
   Parameters:
      Address - the address to write the byte to, the EEPROM address range
                is 0x300 to 0x3FF on the RN2903 module.
      
      Data - the uint8_t value to write the EEPROM memory.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RN2903WriteNVM(uint16_t Address, uint8_t Data);

/*
   RN2903OutputPin()
   
   Function for output the specified level on one of the pins, this function
   assumes the pin was set as a digital output pin with the RN2903SetPinMode()
   function.
   
   Parameters:
      Pin - rn2903_pin_t value indicating the pin to output the level on.
      
      Level - the level to output on the pin, 0 - low or 1 - high.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RN2903OutputPin(rn2903_pin_t Pin, int1 Level);

/*
   RN2903SetPinMode()
   
   Function for setting one of the pins as a digital input, digital output or
   analog input pin.
   
   Parameters:
      Pin - rn2903_pin_t value indicating the pin to set the mode for.
      
      Mode - rn2903_pin_mode_t value indicating the mode to set the pin to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RN2903SetPinMode(rn2903_pin_t Pin, rn2903_pin_mode_t Mode);

/*
   RN2903Version()
   
   Function for getting the firmware version string, reset string, of the
   RN2903 module.
   
   Parameters:
      Str - pointer to an unsigned char array to return the version string to.
*/
void RN2903Version(unsigned char *Str);

/*
   RN2903ReadNVM()
   
   Function to read a byte from the user EEPROM on the module.
   
   Parameters:
      Address - the address to read the byte from, the user EEPROM address
                range is 0x300 to 0x3FF on the RN2903 module.
   
      Result - pointer to an uint8_t variable to return read value to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RN2903ReadNVM(uint16_t Address, uint8_t *Result);

/*
   RN2903GetVdd()
   
   Function to read the Vdd supplied to RN2903 module.
   
   Returns:
      uint16_t value indicating the Vdd in millivolts suppled to the module.
*/
uint16_t RN2903GetVdd(void);

/*
   RN2903GetHWEUI()
   
   Function to read the unique EUI node address assigned to the RN2903 Module.
   
   Parameters:
      EUI - pointer to an uint8_t array to return the EUI value to.
      
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RN2903GetHWEUI(uint8_t *EUI);

/*
   RN2903InputPin()
   
   Function used to read the level of the specified RN2903 module pin.
   
   Parameters:
       Pin - rn2903_pin_t value indicating the pin to read the level for.
   
   Returns:
      0 - if the pin is low, or 1 - if the pin is high.
*/
int1 RN2903InputPin(rn2903_pin_t Pin);

/*
   RN2903ReadAnalog()
   
   Function to read the analog value on a pin, this function assumes that the 
   pin was made analog with the RN2903SetPinMode() function.
   
   Parameters:
      Pin - rn2903_pin_t value indicating the pin to read the analog value for.
   
   Returns:
      uint16_t value from 0 to 1023 indicating the value read on the pin, 0 
      represents 0V and 1023 represents Vdd.  If 0xFFFF is returned then an
      error occurred when trying to execute command.
*/
uint16_t RN2903ReadAnalog(rn2903_pin_t Pin);

///////////////////////////////////
// RN2903 MAC Functions          //
///////////////////////////////////

/*
   MACReset()
   
   Function to reset the software LoRaWAN stack.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACReset(void);

/*
   MACTxData()
   
   Function to send data using the LoRaWAN stack.  After sending the data the
   module will respond with up to 2 responses.  The first response, which will
   be returned by this function, is an error code indicating if the command was
   received successfully.  If the error code indicates no error occurred then a 
   second response will be received after the uplink transmission attempt
   indicating the data was transmitted successfully.
   
   Parameters:
      Type - mac_tx_type_t value indicating the tx type, either 
             MAC_TX_TYPE_CONFIRMED or MAC_TX_TYPE_UNCONFIRMED.
      
      Port - uint8_t value from 1 to 223 indicating the port to use to send the
             data.
      
      Data - pointer to an uint8_t array containing the data to send.
      
      dCount - the number of Data bytes to send.
      
   Returns:
      rn2903_ec_t value indicating if command was received by correctly.   
*/
rn2903_ec_t MACTxData(mac_tx_type_t Type, uint8_t Port, uint8_t *Data, uint8_t dCount);

/*
   MACJoin()
   
   Function to attempt joining a LoRaWAN network.  There are two types of 
   joining attempts that can be made, either otaa (over-the-air activation) or
   abp (activation by personalization).  Depending on the join type certain
   parameters need to be set before the join attempt is made.  For otaa the
   device EUI, application EUI and application key need to be set.  For abp the
   device address, network session key and application session key need to be
   set.  After making the join attempt the module will response with up to 2
   responses.  The first response, which will be returned by this function, is
   an error code indicating if the command was received successfully.  If the
   error code indicates no error occurred then a second response will be
   received indicating if the join attempt was successful or not.
   
   Parameters:
      Mode - mac_join_mode_t value indicating the join type to make, either
             MAC_JOIN_MODE_OTAA or MAC_JOIN_MODE_ABP.
   
   Returns:
      rn2903_ec_t value indicating if command was received by correctly.
*/
rn2903_ec_t MACJoin(mac_join_mode_t Mode);

/*
   MACSave()
   
   Function used to save LoRaWAN protocol configuration parameters to the 
   RN2903 module's EEPROM, so on the next system reset the LoRaWAN protocol 
   configuration will be initialized with the last saved parameters.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSave(void);

/*
   MACForceEnable()
   
   Function that can be used restore the module's connectivity after receiving
   a go silent command from the network.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACForceEnable(void);

/*
   MACPause()
   
   Function to pause the LoRaWAN stack functionality.  This command is required
   to allow Radio commands, i.e. do a LoRa P2P implementation.
   
   Returns:
      uint32_t value indicating the time in milliseconds the MAC can be paused
*/
uint32_t MACPause(void);

/*
   MACResume()
   
   Function used to resume LoRaWAN stack functionality.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACResume(void);

/*
   MACSetKey() 
   
   Function used to set one of the LoRaWAN keys.
   
   Parameter:
      Which - mac_key_type_t value indicating which key to set.
      
      Key - pointer to an uint8_t array containing the key to set.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetKey(mac_key_type_t Which, uint8_t *Key);

/*
   MACEnableDisableAutoReply()
   
   Function used to enable and disable the state of the automatic reply.  When
   enabled the module will transmit a packet without a payload immediately
   after a confirmed downlink is received, or when the Frame Pending bit has
   been set by the server.
   
   Parameters:
      Enable - 1 enable auto reply, 0 disable auto  reply.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACEnableDisableAutoReply(int1 Enable);

/*
   MACSetBatteryLevel()
   
   Function used to set the battery level required for Device Status Answer
   frame in use with the LoRaWAN Class A protocol.
   
   Parameters:
      Level - uint8_t value from 0 to 255 of level to set.  0 means external 
              power, 1 means low level, 254 means high level, 255 means the
              end device was not able to measure the battery level.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetBatteryLevel(uint8_t Level);

/*
   MACSetChannelDataRate()
   
   Function used to set the operating date rate range, min to max, for the 
   specified channel.  For the actual values of the data rates and the
   corresponding spreading factors (SF), refer to the LoRaWAN Specification 
   v1.0.2.
   
   Parameters:
      Ch - uint8_t value representing the channel number, 0 to 71.
      
      Min - uint8_t value representing the minimum data rate, 0 to 3.
      
      Max - uint8_t value representing the maximum data rate, 0 to 3.
    
    Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetChannelDataRate(uint8_t Ch, uint8_t Min, uint8_t Max);

/*
   MACEnableDisableChannel()
   
   Function used to enable or disable the specified channel.
   
   Parameters:
      Ch - uint8_t value representing the channel number, 0 to 71.
      
      Enable - 1 enable the channel, 0 disable the channel.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACEnableDisableChannel(uint8_t Ch, int1 Enable);

/*
   MACSetClass()
   
   Function used to set the end device LoRaWAN operating class.  The default 
   end device class is Class A.
   
   Parameters:
      Class - mac_device_class_t value representing the class to set, either 
              MAC_DEVICE_CLASS_A or MAC_DEVICE_CLASS_C.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetClass(mac_device_class_t Class);

/*
   MACSetDevAddr()
   
   Function used to set the specified network device address.
   
   Parameters:
      which - mac_network_addr_type_t value representing the network address
              to set, either the device address or the multicast device
              address.
      
      Address - uint32_t value to set as the address.
      
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetDevAddr(mac_network_addr_type_t which, uint32_t Address);

/*
   MACSetEUI()
   
   Function for setting one of the EUI values.
   
   Parameters:
      which - mac_eui_type_t value representing the EUI to set.
      
      EUI - pointer to an uint8_t array containing the EUI value to set.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetEUI(mac_eui_type_t which, uint8_t *EUI);

/*
   MACSetFrameCounter()
   
   Function for setting one of the frame counters.
   
   Parameters:
      which - mac_frame_counter_type_t value representing the frame counter to 
              set.
      
      Value - uint32_t value to set as the frame counter.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetFrameCounter(mac_frame_counter_type_t which, uint32_t Value);

/*
   MACSetDataRate()
   
   Function used to set the data rate to be used for the next transmission.  
   For the description of data rate and the corresponding spreading factors,
   refer to the LoRaWAN Specification v1.0.2.
   
   Parameters:
      Rate - uint8_t value representing the data rate to set, 0 to 4.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetDataRate(uint8_t Rate);

/*
   MACSetLinkCheck()
   
   Function used to set the time interval for the link check process to be
   triggered.  When the time interval expires, the next application packet that
   will be sent to the server will also include a link check MAC command.  For
   more information on the Link Check MAC command, refer to the LoRaWAN
   Specification v1.0.2.
   
   Parameters:
      Value - uint16_t value from 0 to 65535 to set the interval in seconds
              for the link check process, 0 disables the link check process.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetLinkCheck(uint16_t Value);

/*
   MACEnableDisableMCast()
   
   Function for enabling and disabling the end device multicast state.  When
   enabled and the device is operating in Class C Continuous Receive mode, the
   end device can receive multicast messages from the server.  For more
   information on the description of multicast operation, refer to the LoRaWAN
   Specification v1.0.2.
   
   Parameters:
      Enable - 1 enabled mcast, 0 disable mcast.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACEnableDisableMCast(int1 Enable);

/*
   MACSetTxPowerIdx()
   
   Function for setting the power index value for the output power.  The output
   power to be used on the next transmissions.  Refer to the LoRaWAN 
   Specification v1.0.2 for the output power corresponding to the power index.
   
   Parameters:
      Index - uint8_t value to set as the power index, 5 to 10.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetTxPowerIdx(uint8_t Index);

/*
   MACSetRetransmissions()
   
   Function for setting the number of retransmission to be used to an uplink
   confirmed packet, if no downlink acknowledgement is received from the
   server.
   
   Parameters:
      Value - uint8_t value representing the number of retransmissions.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetRetransmissions(uint8_t Value);

/*
   MACSetRx2Setting()
   
   Function for setting the data rate and frequency used for the 2nd Receive
   window.
   
   Parameters:
      Setting - rx2_struct_t value containing the data rate and frequency to
                use for the 2nd Receive window.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetRx2Setting(rx2_struct_t Setting);

/*
   MACSetRxDelay1()
   
   Function used for setting the delay between the transmission and the 1st 
   Receive window.
   
   Parameters:
      Delay - uint16_t value representing the delay in milliseconds.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetRxDelay1(uint16_t Delay);

/*
   Function used to set the synchronization byte used for LoRaWAN
   communication.  The configuration of the synchronization byte should be in
   accordance with the Gateway configuration.
   
   Parameters:
      Sync - uint8_t value representing the synchronization byte to set.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACSetSyncByte(uint8_t Sync);

/*
   MACGetADRState()
   
   Function to get the state of the adaptive data rate (ADR) mechanism.
   
   Returns:
      TRUE - ADR enabled.
      FALSE - ADR disabled.
*/
int1 MACGetADRState(void);

/*
   MACGetEUI()
   
   Function for getting the specified EUI value.
   
   Parameters:
      which - mac_eui_type_t value representing the EUI to get.
      
      EUI - pointer to uint8_t array to return the EUI value to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACGetEUI(mac_eui_type_t which, uint8_t *EUI);

/*
   MACGetAutoReplyState()
   
   Function for getting the automatic reply (AR) state.
   
   Returns:
      TRUE - AR enabled.
      FALSE - AR disabled.
*/
int1 MACGetAutoReplyState(void);

/*
   MACGetChannelInfo()
   
   Function for getting MAC channel info for the specified channel, frequency,
   minimum data rate, maximum data rate and channel status (enabled or
   disabled).
   
   Parameters:
      Ch - uint8_t representing the channel to get the info for.
      
      Info - pointer to mac_ch_info_t variable to return the info to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t MACGetChannelInfo(uint8_t Ch, mac_ch_info_t *Info);

/*
   MACGetClass()
   
   Function for getting the LoRaWAN operation class the module is set to.
   
   Returns:
      unsigned char representing the class, 'a' for Class A, 'c' for Class C or
      'e' if an error occurred.
*/
unsigned char MACGetClass(void);

/*
   MACGetDutyCyclePrescalar()
   
   Function for getting the duty cycle prescaler.
   
   Returns:
      uint16_t value representing the prescaler value.
*/
uint16_t MACGetDutyCyclePrescalar(void);

/*
   MACGetDevAddr()
   
   Function for getting the specified device address.
   
   Parameters:
      Type - mac_network_addr_type_t value representing which device address to
             get.
   
   Returns:
      uint32_t value representing the device address.
*/
uint32_t MACGetDevAddr(mac_network_addr_type_t Type);

/*
   MACGetFrameCounter()
   
   Function for getting the specified frame counter.
   
   Parameters:
      which - mac_frame_counter_type_t value representing which frame counter 
              to get.
   
   Returns:
      uint32_t value representing the frame counter.
*/
uint32_t MACGetFrameCounter(mac_frame_counter_type_t which);

/*
   MACGetDataRate()
   
   Function to get the current data rate.
   
   Returns:
      uint8_t value representing the data rate.
*/
uint8_t MACGetDataRate(void);

/*
   MACGetGatawayCount()
   
   Function to get the number of gateways that successfully received the last
   Link Check Request frame command, as received in the last Link Check
   Answer.
   
   Returns:
      uint8_t value representing the number of gateways.
*/
uint8_t MACGetGatawayCount(void);

/*
   MACGetMCastStatus()
   
   Function to get the multicast state of the device.
   
   Returns:
      TRUE - multicast state on.
      FALSE - multicast state off.
*/
int1 MACGetMCastStatus(void);

/*
   MACGetDemodMargin()
   
   Function to get the demodulation margin as received in the last Link Check
   Answer frame.
   
   Returns:
      uint8_t value representing the demodulation margin.
*/
uint8_t MACGetDemodMargin(void);

/*
   MACGetTxPowerIdx()
   
   Function to get the current output power index value.
   
   Returns:
      uint8_t value from 5 to 10 representing the output power index, 0
      indicates an error occurred.
*/
uint8_t MACGetTxPowerIdx(void);

/*
   MACGetRetransmissions()
   
   Function to get the currently configured number of retransmissions which are
   attempted for a confirmed uplink communication when no downlink response
   has been received.
   
   Returns:
      uint8_t value representing the retransmission attempts.
*/
uint8_t MACGetRetransmissions(void);

/*
   MACGetRx2Setting()
   
   Function to get the data rate and frequency of the 2nd receive window.
   
   Returns:
      rx2_struct_t value with the data rate and frequency, if both the data
      rate and frequency parameters of the struct are 0 then an error occurred.
*/
rx2_struct_t MACGetRx2Setting(void);


/*
   MACGetRxDelay()
   
   Function for getting the specified receive windows delay.
   
   Parameters:
      which - mac_rx_delay_type_t value representing which receive window to
              get the delay value for.
      
   Returns:
      uint16_t value representing the delay in milliseconds.
*/
uint16_t MACGetRxDelay(mac_rx_delay_type_t which);

/*
   MACGetStatus()
   
   Function to get the current status of the module.
   
   Returns:
      mac_status_t value representing the status, if any of the reserved bits 
      24 to 31 are set to a 1 it indicates that an error occurred.
*/
mac_status_t MACGetStatus(void);

/*
   MACGetSyncByte()
   
   Function to get the synchronization byte used for the LoRaWAN communication.
   
   Returns:
      uint8_t value representing the synchronization byte.
*/
uint8_t MACGetSyncByte(void);

///////////////////////////////////
// RN2903 Radio Functions        //
///////////////////////////////////

/*
   RadioStartRx()
   
   Function to put radio in receive mode, the LoRaWAN stack must be paused to
   use this command.  This command returns 2 responses the first, which will
   be returned by this function, indicates if the command was received 
   successfully.  The second will occur when data is received or the modules
   times out.
   
   Parameters:
      Timeout - uint16_t value indicating the time out time in symbols (for
                LoRa modulation) or milliseconds (for FSK modulation).  A value
                of 0 enables continuous reception mode.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioStartRx(uint16_t Timeout);

/*
   RadioTxData()
   
   Function to transmit data with the radio, the LoRaWAN stack must be paused
   to use this command.  This command returns 2 responses the first, which will
   be returned by this function, indicates if the command was received
   successfully.  The second will occur after the data is transmitted
   successfully or unsuccessfully.
   
   Parameters:
      Data - pointer to an uint8_t array containing data to transmit.
      
      Count - number of data bytes to transmit.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioTxData(uint8_t *Data, uint8_t Count);

/*
   RadioSetCWMode()
   
   Function to put radio in Continuous Wave (CW) mode.
   
   Parameters:
      Enable - 1 enable CW mode, 0 disable CW mode.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetCWMode(int1 Enable);

/*
   RadioStopRx()
   
   Function used to stop Continuous Receive mode.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioStopRx(void);

/*
   RadioSetAFCBW()
   
   Function used to set the automatic frequency correction bandwidth for
   receiving/transmitting, only used for FSK modulation.
   
   Parameters:
      Frequency - radio_bw_freq_t value to set the correction bandwidth to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetAFCBW(radio_bw_freq_t Frequency);

/*
   RadioSetBitRate()
   
   Function used to set the FSK bit rate, only used for FSK modulation.
   
   Parameters:
      BitRate - uint32_t value from 1 to 300000 to set the FSK bit rate to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetBitRate(uint32_t BitRate);

/*
   RadioSetFSKDataShaping()
   
   Function used to set the data shaping applied to FSK transmissions, only
   used for FSK modulation.  
   
   Parameters:
      Shape - radio_gfsk_modulation_t value used to set the data shaping.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetFSKDataShaping(radio_gfsk_modulation_t Shape);

/*
   RadioSetBandWidth()
   
   Function used to set the LoRa modulation bandwidth, only used for LoRa
   modulation.
   
   Parameters:
      BandWidth - radio_bandwidth_t value to set the bandwidth to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetBandWidth(radio_bandwidth_t BandWidth);

/*
   RadioSetCodingRate()
   
   Function used to set the LoRa modulation coding rate, only used for LoRa
   modulation.
   
   Parameters:
      Rate - radio_coding_rate_t value to set the coding rate to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetCodingRate(radio_coding_rate_t Rate);

/*
   RadioEnableDisableCRC()
   
   Function used to enable disable the CRC header for communication.
   
   Parameters:
      Enable - 1 enable CRC, 0 disable CRC.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioEnableDisableCRC(int1 Enable);

/*
   RadioSetFrequencyDeviation()
   
   Function used to set the frequency deviation during operation, only used for
   FSK modulation.
   
   Parameters:
      Deviation - uint32_t value from 0 to 200000 to set deviation to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetFrequencyDeviation(uint32_t Deviation);

/*
   RadioSetFrequency()
   
   Function used to set the communication frequency of the radio transceiver.
   
   Parameters:
      Frequency - uint32_t value from 902000000 to 928000000 to set frequency
                  to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetFrequency(uint32_t Frequency);

/*
   RadioEnableDisableIQI()
   
   Function used to enable or disable the invert IQ (IQI) for communication,
   only used for LoRa modulation.
   
   Parameters:
      Enable - 1 enable IQI, 0 disable IQI.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioEnableDisableIQI(int1 Enable);

/*
   RadioSetModulation()
   
   Function used to set the modulation method being used by the module, LoRa or
   FSK modulation.
   
   Parameters:
      Modulation - radio_modulation_t value to set the modulation to use.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetModulation(radio_modulation_t Modulation);

/*
   RadioSetPreambleLength()
   
   Function used to set the preamble length, only used for FSK modulation.
   
   Parameters:
      Length - uint16_t value to set the preamble length to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetPreambleLength(uint16_t Length);

/*
   RadioSetTxPower()
   
   Function used to set the transceiver output power.
   
   Parameters:
      Power - uint8_t value from 2 to 20 to set the output power to in dBm.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetTxPower(uint8_t Power);

/*
   RadioSetRxBW()
   
   Function used to set the signal bandwidth when receiving, only used for FSK
   modulation.
   
   Parameters:
      Frequency - radio_bw_freq_t value to set the bandwidth to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetRxBW(radio_bw_freq_t Frequency);

/*
   RadioSetSpreadingFactor()
   
   Function used to set the spreading factor (SF) used for LoRa modulation,
   used for LoRa modulation only.  The SF has a direct correlation to the data
   rate, lower SF means a high data rate and high SF means a lower data rate.
   
   Parameters:
      Factor - radio_spreading_factor_t value to set the SF to, 
               RADIO_SPREADING_FACTOR_SF7 to RADIO_SPREADING_FACTOR_SF12.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetSpreadingFactor(radio_spreading_factor_t Factor);

/*
   RadioSetSyncByte()
   
   Function used to set the synchronization byte used during communication.
   
   Parameters:
      Sync - uint8_t value to set the synchronization byte to.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetSyncByte(uint8_t Sync);

/*
   RadioSetWDT()
   
   Function used to set the time-out length applied to the radio Watchdog
   Timer (WDT).  If enabled, the WDT is started for every transceiver
   reception or transmission.  The WDT is stopped when the operation in
   progress is finished.
   
   Parameters:
      Time - uint32_t value to set the time-out length to in milliseconds, a
             value of 0 disables the WDT functionality.
   
   Returns:
      rn2903_ec_t value indicating if command was successful or not.
*/
rn2903_ec_t RadioSetWDT(uint32_t Time);

/*
   RadioGetAFCBW()
   
   Function to get the automatic frequency correction bandwidth, only used for
   FSK modulation.
   
   Returns:
      radio_bw_freq_t value indicating the setting, or 0 if an error occurred.
*/
radio_bw_freq_t RadioGetAFCBW(void);

/*
   RadioGetBitRate()
   
   Function to get the configured bit rate, only used for FSK modulation.
   
   Returns:
      uint32_t value from 1 to 300000 indicating the bit rate, or 0 if an error
      occurred.
*/
uint32_t RadioGetBitRate(void);

/*
   RadioGetFSKDataShaping()
   
   Function to get the configured data shaping applied to FSK transmissions, 
   only used for FSK modulation.
   
   Returns:
      radio_gfsk_modulation_t indicating the bit rate, or 0 if an error
      occurred.
*/
radio_gfsk_modulation_t RadioGetFSKDataShaping(void);

/*
   RadioGetBandWidth()
   
   Function to get the current operating radio bandwidth, only used for LoRa
   modulation.
   
   Returns:
      radio_bandwidth_t value indicating the bandwidth, or 0 if an error
      occurred.
*/
radio_bandwidth_t RadioGetBandWidth(void);

/*
   RadioGetCodingRate()
   
   Function to get the current setting used for the coding rate, only used for
   LoRa modulation.
   
   Returns:
      radio_coding_rate_t value indicating the coding rate, or 0 if an error
      occurred.
*/
radio_coding_rate_t RadioGetCodingRate(void);

/*
   RadioGetCRCStatus()
   
   Function to get the status of the CRC header.
   
   Returns:
      TRUE - if enabled.
      FALSE - if disabled.
*/
int1 RadioGetCRCStatus(void);

/*
   RadioGetFrequencyDeviation()
   
   Function to get the current frequency deviation setting, only used for FSK
   modulation.
   
   Returns:
      uint32_t value from 0 to 200000 representing the frequency deviation.
*/
uint32_t RadioGetFrequencyDeviation(void);

/*
   RadioGetFrequency()
   
   Function to get the current operating frequency of the module.
   
   Returns:
      uint32_t value from 902000000 to 928000000 representing the frequency, or
      0 if an error occurred.
*/
uint32_t RadioGetFrequency(void);

/*
   RadioGetIQIStatus()
   
   Function to get the Invert IQ (IQI) state, only used for LoRa modulation.
   
   Returns:
      TRUE - if enabled.
      FALSE - if disabled.
*/
int1 RadioGetIQIStatus(void);

/*
   RadioGetModulation()
   
   Function to get the current mode of operation, FSK or LoRa.
   
   Returns:
      radio_modulation_t value indicating the current modulation, or 0 if an 
      error occurred.
*/
radio_modulation_t RadioGetModulation(void);

/*
   RadioGetPreambleLength()
   
   Function for getting the current preamble length used for communication,
   only used for FSK modulation.
   
   Returns:
      uint16_t value representing the preamble length.
   
*/
uint16_t RadioGetPreambleLength(void);

/*
   RadioGetTxPower()
   
   Function to get the current power level setting used for communication.
   
   Returns:
      uint8_t value from 2 to 20 representing power level in dBm, or 0 if an 
      error occurred.
*/
uint8_t RadioGetTxPower(void);

/*
   RadioGetRSSI()
   
   Function to get the rssi value for the last received frame.
   
   Returns:
      int8_t value representing the RSSI for the last received frame.
*/
int8_t RadioGetRSSI(void);

/*
   RadioGetRxBW()
   
   Function to get the signal bandwidth used for receiving, only used for FSK
   modulation.
   
   Returns:
      radio_bw_freq_t value indicating the signal bandwidth, or 0 if an error
      occurred.
*/
radio_bw_freq_t RadioGetRxBW(void);

/*
   RadioGetSpreadingFactor()
   
   Function to get the current spreading factor (SF) used by the transceiver, 
   only used for LoRa modulation.
   
   Returns:
      radio_spreading_factor_t value indicating SF, or 0 if an error occurred.
*/
radio_spreading_factor_t RadioGetSpreadingFactor(void);

/*
   RadioGetSNR()
   
   Function to get the Signal Noise Ration (SNR) for the last received packet.
   
   Returns:
      int8_t value representing the SNR for the last received packet.
*/
int8_t RadioGetSNR(void);

/*
   RadioGetWDT()
   
   Function to get the length used for the watchdog time-out.
   
   Returns:
      uint32_t value representing the time-out in milliseconds.
*/
uint32_t RadioGetWDT(void);

///////////////////////////////////
// RN2903 Serial Functions       //
///////////////////////////////////

/*
   RN2903SerialInit();
   
   Function to initialize the communication with RN2903 module, should be
   called before any other RN2903 functions.  This function also waits to
   receive the version string on power up, if str is not received it will
   timeout.
   
   Parameters:
      Str - pointer to an unsigned char array to return start string to.  This
            parameter is optional if not passed it won't return the start
            string.
*/
void RN2903SerialInit(unsigned char *Str=NULL);

/*
   RN2903SerialRxTask()
   
   Task function to receive data from the RN2903 module.  This should be called
   frequency, recommended at least once per iteration of the main loop, to
   receive messages from the RN2903 module.  This is primarily used to receive
   responses from command/functions that have more then 1 response, MACJoin(),
   MACTxData(), RadioStartRx() and RadioTxData() for example.
   
*/
void RN2903SerialRxTask(void);

/*
   RN2903SerialKbhit()
   
   Function used to determine if there is a new message that can be retrieved
   from serial receive buffer.
   
   Returns:
      TRUE - if there is a new message.
      FALSE - if there is no new message.
*/
int1 RN2903SerialKbhit(void);

/*
   RN2903SerialGetResponse()
   
   Function used to get a new response from the serial buffer.  This function
   is primarily used to receive the 2nd response from commands/function that
   have more then 1 response, MACJoin(), MACTxData(), RadioStartRx() and 
   RadioTxData() for example.
   
   The parameter Data is only valid if the returned response is 
   RN2903_RESP_MAC_RX, RN2903_RESP_RADIO_RX or RN2903_RESP_VALUE.  When the 
   response is RN2903_RESP_MAC_RX Data[0] of the Data array is the port number
   used for the communication, and Data[1] to Data[Length - 1] is the data
   received.  When the response is RN2903_RESP_RADIO_RX Data[0] to 
   Data[Length - 1] of the Data array is the data received.  When the response
   is RN2903_RESP_VALUE the Data[0] to Data[Length] is the value as receive
   from the module, which should be a null terminated string.
   
   The parameter Length is only a nonzero value when the returned response if
   RN2903_RESP_MAC_RX, RN2903_RESP_RADIO_RX or RN2903_RESP_VALUE.
   
   Parameters:
      Data - pointer to an uint8_t array to return the response to.
      
      Length - pointer to an uint8_t variable to return the length of the 
               data to.
   
   Returns:
      rn2903_resp_t value indicating the response type of the message received
      from the module.
*/
rn2903_resp_t RN2903SerialGetResponse(uint8_t *Data, uint8_t *Length);

///////////////////////////////////
// RN2903 Bootloader Functions   //
///////////////////////////////////

/*
   RN2903BLGetVersion()
   
   Function used to get the RN2903 Bootloader Version and information.  This
   function can only be used if RN2903 Module is in bootloader mode.  For the
   RN2903 to enter bootloader mode the RN2903 Firmware must be erased with the
   RN2903EraseFW() function.  Once the RN2903 firmware is erased it will reset
   and enter bootloader mode, bootloader mode can only be exited by
   successfully loading it with new firmware.
   
   Parameters:
      Version - pointer to rn2903_bl_version_t variable to return the RN2903 
                bootloader version and information to.
*/
void RN2903BLGetVersion(rn2903_bl_version_t *Version);

/*
   RN2903BLWriteFW()
   
   Function used to write firmware data to the RN2903.  This  function can only
   be used if RN2903 Module is in bootloader mode, see RN2903BLGetVersion() for
   info about bootloader mode.
   
   Parameters:
      Address - the address in RN2903 module flash to write data to.  Flash
                address range that can be written in R2903 module is 0x300 to
                0xFFFF.  
      
      Data - pointer to uint8_t array containing data to write to RN2903.
      
      Count - the number of bytes to write, 0 to 255.
      
   Returns:
      TRUE - if data was written successfully.
      FALSE - if an error occured.
*/
int1 RN2903BLWriteFW(uint32_t Address, uint8_t *Data, uint16_t Count);

/*
   RN2903BLEraseFW()
   
   Function used to erase RN2903 modules flash.  This function can only be used
   if RN2903 Module is in bootloader mode, see RN2903BLGetVersion() for ifo
   about bootloader mode.
   
   Returns:
      TRUE - if erase was successful.
      FALSE - if an error occured.
*/
int1 RN2903BLEraseFW(void);


/*
   RN2903BLGetCheckSum()
   
   Function used to get the checksum of the RN2903 Application memory.  This 
   function can only be used if RN2903 Module is in bootloader mode, see 
   RN2903BLGetVersion() for info about bootloader mode.
   
   Returns:
      uint16_t representing the checksum for RN2903 flash from address 0x300 to
      0xFFFF.
*/
uint16_t RN2903BLGetCheckSum(void);

/*
   RN2903BLReset()
   
   FUnction used to reset the RN2903 module when in bootloader mode.  This 
   function can only be used if RN2903 Module is in bootloader mode, see 
   RN2903BLGetVersion() for info about bootloader mode.
   
   Returns:
      TRUE - RN2903 module reset and the version string the module sends when
             reset was seen, indicating that the RN2903 module was bootloaded
             correctly.
      FALSE - RN2903 module didn't reset out of bootloader mode, indicating
              that the RN2903 module was not bootloaded correctly.
      
*/
int1 RN2903BLReset(void);

#endif   //__RN2903_H__
