///////////////////////////////////////////////////////////////////////////////
////                                lora.c                                 ////
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
////  See lora.h for API.                                                  ////
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

#include <lora.h>

#ifndef LORA_DEFAULT_SYNC_BYTE
 #define LORA_DEFAULT_SYNC_BYTE     0x66
#endif

#ifndef LORA_DEFAULT_PWR
 #define LORA_DEFAULT_PWR           2     //2 to 20 dBm
#endif

#ifndef LORA_DEFAULT_SF
 #define LORA_DEFAULT_SF            LORA_SF_7   //see lora_sf_t in lora.h for options
#endif

#ifndef LORA_CRC_ENABLED
 #define LORA_CRC_ENABLED           TRUE
#endif

#ifndef LORA_IQI_ENABLED
 #define LORA_IQI_ENABLED           FALSE
#endif

#ifndef LORA_DEFAULT_CR
 #define LORA_DEFAULT_CR            LORA_CR_45  //see lora_cr_t in lora.h for options
#endif

#ifndef LORA_DEFAULT_BW
 #define LORA_DEFAULT_BW            LORA_BW_125 //see lora_bw_t in lora.h for options
#endif

#if !defined(_lora_tick_t) || !defined(_LORA_TICKS_PER_SECOND) || !defined(_lora_tick_difference) || !defined(_lora_tick_get)
 #error drive needs a tick timer provided to it
#endif

#ifndef LORA_DEVICE_TYPE
 #define LORA_DEVICE_TYPE           LORA_DEVICE_TYPE_ED
#endif

#ifndef LORA_BS_DEVICE_ADDR
 #define LORA_BS_DEVICE_ADDR        0
#endif

#if LORA_DEVICE_TYPE == LORA_DEVICE_TYPE_BS
 #if defined(LORA_DEVICE_ADDR) && (LORA_DEVICE_ADDR != LORA_BS_DEVICE_ADDR )
  #undef LORA_DEVICE_ADDR
 #endif
 
 #ifndef LORA_DEVICE_ADDR
  #define LORA_DEVICE_ADDR          LORA_BS_DEVICE_ADDR
 #endif
#else
 #if defined(LORA_DEVICE_ADDR) && (LORA_DEVICE_ADDR == LORA_BS_DEVICE_ADDR )
  #undef LORA_DEVICE_ADDR
 #endif
 
 #ifndef LORA_DEVICE_ADDR
  #define LORA_DEVICE_ADDR          (LORA_BS_DEVICE_ADDR + 1)
 #endif
#endif

#ifndef LORA_CHANNELS
 #define LORA_CHANNELS              25    //max number of channels is dependent on bandwidth being used, 130 for 125 kHz, 65 for 250 kHz and 32 for 500 kHz
#endif

#define LORA_START_FREQ             902000000   //902.0 MHz is first LoRa Frequency in US
#define LORA_LAST_FREQ              928000000   //928.0 MHz is last LoRa Frequency in US

#define LORA_INC_FREQ_125_BW        200000
#define LORA_INC_FREQ_250_BW        400000
#define LORA_INC_FREQ_500_BW        800000

#ifndef LORA_RX_TIME
 #define LORA_RX_TIME               2000  //in ms, how long after ED transmit data it will wait for a response
#endif
#define LORA_RX_TICKS               ((LORA_RX_TIME * _LORA_TICKS_PER_SECOND) / 1000)

#define LORA_MAX_TX_TIME            400   //in ms, this is an FCC part 15 requirement don't change this time
#define LORA_MAX_TX_TICKS           ((LORA_MAX_TX_TIME * _LORA_TICKS_PER_SECOND) / 1000)

#define LORA_500_BW_TX_RATE         10    //can transmit once every 10 seconds with this bandwidth on each channel
#define LORA_125_250_BW_TX_RATE     20    //can transmit once every 20 seconds with these bandwidths on each channel

//////////////////////////////////// Types ////////////////////////////////////

#ifdef __PCD__
 #ifndef _PACKED_STRUCT
  #define _PACKED_STRUCT   __attribute__((__packed__))
 #endif
#else
 #ifndef _PACKED_STRUCT
  #define _PACKED_STRUCT
 #endif
#endif

typedef enum
{
   LORA_MODE_IDLE,
   LORA_MODE_LISTEN,
   LORA_MODE_TX,
   LORA_MODE_TX_RESPONSE_WAIT,
   LORA_MODE_TX_RESPONSE_SEND,
   LORA_MODE_RX
} _lora_mode_t;

typedef struct _PACKED_STRUCT
{
   uint8_t Length;
   uint8_t Data[LORA_MAX_PAYLOAD_SIZE];
} _lora_payload_t;

typedef struct _PACKED_STRUCT
{
   uint8_t Destination;
   uint8_t Source;
   _lora_payload_t Payload;
  #ifdef LORA_USE_MESSAGE_CRC
   uint16_t crc;
  #elif defined(LORA_USE_MESSAGE_CHECKSUM)
   uint8_t CheckSum;
  #endif
} _lora_message_t;

/////////////////////////////// Global Variables //////////////////////////////

struct
{
   lora_device_type_t DeviceType;
   uint8_t DeviceAddress;
   _lora_mode_t cMode;
   _lora_mode_t pMode;
   uint8_t Channel;
   _lora_tick_t t;
} _g_Lora;

struct
{
   _lora_message_t Buffer;
   int1 HasMessage;
} _g_LoraTx;

struct
{
   _lora_message_t Buffer;
   int8_t rssi;
   int8_t snr;
   int1 HasMessage;
} _g_LoraRx;

struct
{
   _lora_tick_t TxTick[LORA_CHANNELS];
   _lora_tick_t TxTicks;
   uint8_t Channels;
   uint8_t HoppingTable[LORA_CHANNELS];
   uint8_t HoppingIndex;
} _g_LoraCh;

////////////////////////////// Local Prototypes ///////////////////////////////

void _lora_push_tx_message(_lora_message_t *Message);
void _lora_push_rx_message(_lora_message_t *Message);
int1 _lora_tx_message(void);

void _lora_reset_mode(void);
void _lora_rx_mode_task(_lora_tick_t cTick);

#define _lora_set_mode(mode)  _g_Lora.cMode = mode
#define _lora_set_tick()      _g_Lora.t = _lora_tick_get()

///////////////////////////////////// API /////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// lora_init()
//
// Initializes the Lora driver to communicate as P2P devices.
//
// Parameters:
//    ChHoppingTable - optional parameter, pointer to an uint8_t array 
//                     containing the channels and order they will be used.  
//                     Size of array must be the same as the LORA_CHANNELS
//                     define.  If not defined the channels will be used
//                     sequentially from 0 to (LORA_CHANNELS - 1).  This
//                     parameter must be the same on all devices using the same
//                     network setup.
//
// Returns:
//    TRUE - if successfully initialized.
//    FALSE - if an error occurred during initialization.
///////////////////////////////////////////////////////////////////////////////
int1 lora_init(uint8_t *ChHoppingTable=NULL)
{
   int1 Result;
   _lora_tick_t cTick;
   uint8_t i;

   _lora_config.sync = LORA_DEFAULT_SYNC_BYTE;
   _lora_config.pwr = LORA_DEFAULT_PWR;
   _lora_config.sf = LORA_DEFAULT_SF;
   _lora_config.cr = LORA_DEFAULT_CR;
   _lora_config.bw = LORA_DEFAULT_BW;
   _lora_config.crc = LORA_CRC_ENABLED;
   _lora_config.iqi = LORA_IQI_ENABLED;
   
   if(_lora_config.bw == LORA_BW_125)
      _g_LoraIncFreq = LORA_INC_FREQ_125_BW;
   else if(_lora_config.bw == LORA_BW_250)
      _g_LoraIncFreq = LORA_INC_FREQ_250_BW;
   else
      _g_LoraIncFreq = LORA_INC_FREQ_500_BW;
   
   _g_LoraStartFreq = LORA_START_FREQ + (_g_LoraIncFreq / 2);
   
   if(_lora_config.bw == LORA_BW_500)
      _g_LoraCh.TxTicks = (TICKS_PER_SECOND * LORA_500_BW_TX_RATE);
   else
      _g_LoraCh.TxTicks = (TICKS_PER_SECOND * LORA_125_250_BW_TX_RATE);
   
   memset(&_g_LoraTx, 0x00, sizeof(_g_LoraTx));
   memset(&_g_LoraRx, 0x00, sizeof(_g_LoraRx));
   
   _g_Lora.DeviceType = LORA_DEVICE_TYPE;
   _g_Lora.DeviceAddress = LORA_DEVICE_ADDR;
   
   if(_g_Lora.DeviceType == LORA_DEVICE_TYPE_ED)
      _lora_set_mode(LORA_MODE_IDLE);
   else
      _lora_set_mode(LORA_MODE_LISTEN);
   
   _g_Lora.pMode = LORA_MODE_IDLE;
   
   Result = _lora_init_radio();
   
   if(Result == TRUE)
      Result = _lora_set_radio_config();
   
   _g_LoraCh.Channels = LORA_CHANNELS;
   
   if(ChHoppingTable == NULL)
   {
      for(i=0;i<_g_LoraCh.Channels;i++)
         _g_LoraCh.HoppingTable[i] = i;
   }
   else
   {
      for(i=0;i<_g_LoraCh.Channels;i++)
         _g_LoraCh.HoppingTable[i] = ChHoppingTable[i];
   }
   
   _g_LoraCh.HoppingIndex = 0;
  
   cTick = _lora_tick_get() - _g_LoraCh.TxTicks;
  
   for(i=0;i<_g_LoraCh.Channels;i++)
      _g_LoraCh.TxTick[i] = cTick;
   
  #ifdef LORA_USE_MESSAGE_CRC
   _lora_crc_init();
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// lora_task()
//
// Task function responsible for sending and receiving messages.  Should be
// called relatively frequently, at least once per iteration of the main()
// loop.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void lora_task(void)
{
   _lora_tick_t cTick;
   lora_module_resp_t Response;
   uint8_t Data[(LORA_MAX_PAYLOAD_SIZE * 2) + 7];  
   uint8_t Length;
   _lora_message_t RxMessage;
  #ifdef LORA_USE_MESSAGE_CRC
   uint16_t crc;
  #elif defined(LORA_USE_MESSAGE_CHECKSUM)
   uint8_t checksum;
   uint8_t i;
  #endif
   
   _lora_rx_task();
   cTick = _lora_tick_get();
   
   if(_g_Lora.cMode != _g_Lora.pMode)
   {
      debug_lora_printf(debug_lora_putc, "cMode: %u, pMode: %u\r\n", _g_Lora.cMode, _g_Lora.pMode);
     
      switch(_g_Lora.cMode)
      {
         case LORA_MODE_IDLE:
            if(_g_Lora.pMode == LORA_MODE_RX)
            {
               _lora_stop_rx();
            }
            break;
         case LORA_MODE_LISTEN:
            if(_g_Lora.DeviceType >= LORA_DEVICE_TYPE_BS)
            {
               if(_g_Lora.pMode == LORA_MODE_RX)
               {
                  _lora_stop_rx();
               }
               
               _g_Lora.Channel = _g_LoraCh.HoppingTable[_g_LoraCh.HoppingIndex];
               
               debug_lora_printf(debug_lora_putc, "   LORA_MODE_LISTEN - CH: %u\r\n", _g_Lora.Channel);
               
               if(++_g_LoraCh.HoppingIndex >= _g_LoraCh.Channels)
                  _g_LoraCh.HoppingIndex = 0;
               
               _lora_set_channel(_g_Lora.Channel);
               _lora_start_listening();
            }
            else
            {
               _lora_set_mode(LORA_MODE_IDLE);
               return;
            }
            break;
         case LORA_MODE_TX:
            if(_g_LoraTx.HasMessage)
            {
               if(_g_Lora.DeviceType == LORA_DEVICE_TYPE_ED)
               {
                  _g_Lora.Channel = _g_LoraCh.HoppingTable[_g_LoraCh.HoppingIndex];
                  
                  if(_lora_tick_difference(cTick, _g_LoraCh.TxTick[_g_Lora.Channel]) >= _g_LoraCh.TxTicks)
                  {
                     _lora_set_channel(_g_Lora.Channel);
                     
                     debug_lora_printf(debug_lora_putc, "   LORA_MODE_TX - CH: %u\r\n", _g_Lora.Channel);
                     
                     if(_lora_tx_message())
                     {
                        if(++_g_LoraCh.HoppingIndex >= _g_LoraCh.Channels)
                           _g_LoraCh.HoppingIndex = 0;
                     
                        _lora_set_tick();
                        _g_LoraCh.TxTick[_g_Lora.Channel] = _lora_tick_get();
                     }
                     else
                        return;
                  }
                  else
                     return;
               }
               else  //LORA_DEVICE_TYPE_LED - already set to correct channel
               {
                  if(_g_Lora.pMode == LORA_MODE_LISTEN)
                  {
                     _lora_stop_rx();
                  }
               
                  if(_lora_tx_message())
                  {
                     _lora_set_tick();
                     _g_LoraCh.TxTick[_g_Lora.Channel] = _lora_tick_get();
                  }
                  else
                     return;
               }
            }
            else
            {
               _lora_reset_mode();               
               return;
            }
            break;
         case LORA_MODE_TX_RESPONSE_WAIT:
            if(_lora_tick_difference(cTick, _g_Lora.t) < LORA_MAX_TX_TICKS) //wait for other device to be done transmitting
               return;
            else
               _lora_set_tick();
            break;
         case LORA_MODE_TX_RESPONSE_SEND:
            if(_lora_tx_message())
            {
               _lora_set_tick();
               _g_LoraCh.TxTick[_g_Lora.Channel] = _lora_tick_get();
            }
            else
            {
               debug_lora_printf(debug_lora_putc, "   _lora_tx_message() == FALSE\r\n");
               
               _g_LoraTx.HasMessage = FALSE;
               _lora_set_mode(LORA_MODE_LISTEN);
               return;
            }
            break;
         case LORA_MODE_RX:
            if(_lora_start_listening() == FALSE)
            {
               debug_lora_printf(debug_lora_putc, "   lora_start_lisening() == FALSE\r\n");
            }
            break;
      }
      
      _g_Lora.pMode = _g_Lora.cMode;
   }
   else
   {
      switch(_g_Lora.cMode)
      {
         case LORA_MODE_IDLE:
            //do nothing
            break;
         case LORA_MODE_LISTEN:
            if(_lora_kbhit())
            {
               Response = _lora_get_radio_message(Data, &Length);
               
               if(Response == LORA_MODULE_RESP_RX_DATA)
               {
                 #ifdef LORA_USE_MESSAGE_CRC
                  memcpy(&RxMessage, Data, Length - 2);
                  RxMessage.crc = make16(Data[Length - 1], Data[Length - 2]);
                  
                  //check crc
                  _lora_crc_init_value();
                  crc = _lora_crc_calc(Data, Length - 2);
                  
                  if(crc != RxMessage.crc)
                  {
                     debug_lora_printf(debug_lora_putc, "   CRC didn't match\r\n");
                     
                     _g_Lora.pMode = LORA_MODE_IDLE;  //error in crc force it to return to listen mode
                     break;
                  }
                 #elif defined(LORA_USE_MESSAGE_CHECKSUM)
                  memcpy(&RxMessage, Data, Length - 1);
                  
                  //check checksum
                  checksum = 0;
                  for(i=0;i<Length;i++)
                     checksum += Data[i];
                  
                  if(checksum != 0)
                  {
                     debug_lora_printf(debug_lora_putc, "   Checksum didn't match\r\n");
                  
                     _g_Lora.pMode = LORA_MODE_IDLE;  //error in checksum force it to return to listen mode
                     break;
                  }
                  else
                     RxMessage.CheckSum = Data[Length - 1];
                 #else
                  memcpy(&RxMessage, Data, Length);
                 #endif
                 
                #ifdef DEBUG_LORA
                 #if !definedinc(i) != 7
                  uint8_t i;
                 #endif
                 
                  debug_lora_printf(debug_lora_putc, "   RxMessage - Dest: %u, Source: %u, pLength: %u, Payload:", RxMessage.Destination, RxMessage.Source, RxMessage.Payload.Length);
                  
                  for(i=0;i<RxMessage.Payload.Length;i++)
                     debug_lora_printf(debug_lora_putc, " %02X", RxMessage.Payload.Data[i]);
                  
                  debug_lora_printf(debug_lora_putc, "\r\n");
                #endif
                  
                  if(RxMessage.Destination == _g_Lora.DeviceAddress)
                  {
                     _lora_push_rx_message(&RxMessage);
                     
                     _lora_set_tick();
                     _lora_set_mode(LORA_MODE_TX_RESPONSE_WAIT);
                  }
                  else
                     _g_Lora.pMode = LORA_MODE_IDLE;  //force back into listen mode
               }
               else  //error occurred while listen for message
                  _g_Lora.pMode = LORA_MODE_IDLE;
            }
            break;
         case LORA_MODE_TX:
            if(_lora_kbhit())
            {
               Response = _lora_get_radio_message(Data, &Length);
               
               if(Response == LORA_MODULE_RESP_TX_OK)
               {
                  _g_LoraTx.HasMessage = FALSE;
                  _lora_set_mode(LORA_MODE_RX);
               }
               else  //timeout or error
               {
                  debug_lora_printf(debug_lora_putc, "   TX Mode - Response: %u\r\n", Response);
                  
                  _g_LoraTx.HasMessage = FALSE;
                  _lora_reset_mode();
               }
            }
            break;
         case LORA_MODE_TX_RESPONSE_WAIT:
            if(_g_LoraTx.HasMessage)
               _lora_set_mode(LORA_MODE_TX_RESPONSE_SEND);
            else if(_lora_tick_difference(cTick, _g_Lora.t) >= (LORA_RX_TICKS - LORA_MAX_TX_TICKS))
            {
               debug_lora_printf(debug_lora_putc, "   TX Response Wait Mode - No TX Message to send\r\n");
            
               _lora_set_mode(LORA_MODE_LISTEN);
            }
            
            break;
         case LORA_MODE_TX_RESPONSE_SEND:
            if(_lora_kbhit())
            {
               Response = _lora_get_radio_message(Data, &Length);
               
               if(Response != LORA_MODULE_RESP_TX_OK)
               {
                  debug_lora_printf(debug_lora_putc, "TX Response Send Mode - Response: %u\r\n", Response);
               }
               
               _g_LoraTx.HasMessage = FALSE;
               _lora_set_mode(LORA_MODE_LISTEN);
            }
            break;
         case LORA_MODE_RX:
            if(_lora_kbhit())
            {
               Response = _lora_get_radio_message(Data, &Length);
               
               if(Response == LORA_MODULE_RESP_RX_DATA)
               {
                 #ifdef LORA_USE_MESSAGE_CRC
                  memcpy(&RxMessage, Data, Length - 2);
                  RxMessage.crc = make16(Data[Length - 1], Data[Length - 2]);
                  
                  //check crc
                  _lora_crc_init_value();
                  crc = _lora_crc_calc(Data, Length - 2);
                  
                  if(crc != RxMessage.crc)
                  {
                     debug_lora_printf(debug_lora_putc, "   CRC didn't match\r\n");
                     
                     _lora_rx_mode_task(cTick);
                     break;
                  }
                 #elif defined(LORA_USE_MESSAGE_CHECKSUM)
                  memcpy(&RxMessage, Data, Length - 1);
                  
                  //check checksum
                  checksum = 0;
                  for(i=0;i<Length;i++)
                     checksum += Data[i];
                  
                  if(checksum != 0) //error in checksum
                  {
                     _lora_rx_mode_task(cTick);
                     break;
                  }
                  else
                     RxMessage.CheckSum = Data[Length - 1];
                 #else
                  memcpy(&RxMessage, Data, Length);
                 #endif
                  
                  debug_lora_printf(debug_lora_putc, "   RxMessage - Dest: %u, Source: %u, pLength: %u\r\n", RxMessage.Destination, RxMessage.Source, RxMessage.Payload.Length);
                  
                  if(RxMessage.Destination == _g_Lora.DeviceAddress)
                  {
                     _lora_push_rx_message(&RxMessage);
                     _lora_reset_mode();
                  }
                  else
                     _lora_rx_mode_task(cTick);
               }
               else
                  _lora_rx_mode_task(cTick);
            }
            else if(_lora_tick_difference(cTick, _g_Lora.t) >= (LORA_RX_TICKS + LORA_MAX_TX_TICKS))
               _lora_reset_mode();
            break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// lora_tbe()
//
// Used to determine if the transmit buffer is empty.  This can be polled to
// to make sure previous message has been sent before trying to load next
// message to send.
//
// Parameters - None.
//
// Returns:
//    TRUE - if empty and buffer is free to receive next message.
//    FALSE - if buffer is not empty, new message can not be loaded into buffer
//            to send.
///////////////////////////////////////////////////////////////////////////////
int1 lora_tbe(void)
{
   if(_g_LoraTx.HasMessage == FALSE)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// lora_put_message()
//
// Function used to load message into transmit buffer to send.
//
// Parameters:
//    DestAddr - address to send the message to, e.g. 0 default base station 
//               address.
//
//    Payload - pointer to uint8_t array containing payload data of message.
//
//    Length - uint8_t value specifying the size in bytes of the payload.
//
// Returns:
//    TRUE - if message was successfully loaded into transmit buffer to send.
//    FALSE - if message was not loaded into transmit buffer, an error occurred
//            e.g. buffer was already full or state machine is in incorrect
//            mode.
///////////////////////////////////////////////////////////////////////////////
int1 lora_put_message(uint8_t DestAddr, uint8_t *Payload, uint8_t Length)
{
   int1 Result;
   _lora_message_t Message;
   
   debug_lora_printf(debug_lora_putc, "lora_put_message() - Dest: %u, Length: %u", DestAddr, Length);
   
  #ifdef DEBUG_LORA
   uint8_t i;
   
   debug_lora_printf(debug_lora_putc, ", Data:");
   
   for(i=0;i<Length;i++)
      debug_lora_printf(debug_lora_putc, " %02X", Payload[i]);
  #endif
   
   debug_lora_printf(debug_lora_putc, "\r\n");
   
   if(lora_tbe())
   {
      if
      (
         ((_g_Lora.DeviceType == LORA_DEVICE_TYPE_ED) && (_g_Lora.cMode == LORA_MODE_IDLE)) ||
         ((_g_Lora.DeviceType == LORA_DEVICE_TYPE_BS) && (_g_Lora.cMode == LORA_MODE_TX_RESPONSE_WAIT)) || 
         ((_g_Lora.DeviceType == LORA_DEVICE_TYPE_LED) && ((_g_Lora.cMode == LORA_MODE_LISTEN) || (_g_Lora.cMode == LORA_MODE_TX_RESPONSE_WAIT)))
      )
      {
         Message.Destination = DestAddr;
         Message.Source = _g_Lora.DeviceAddress;
         Message.Payload.Length = Length;
         memcpy(&Message.Payload.Data, Payload, Length);
      
         _lora_push_tx_message(&Message);
         
         if((_g_Lora.cMode == LORA_MODE_IDLE) || (_g_Lora.cMode == LORA_MODE_LISTEN))
            _lora_set_mode(LORA_MODE_TX);
            
         Result = TRUE;
      }
      else
         Result = FALSE;
   }
   else
      Result = FALSE;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// lora_busy()
//
// Used to determine if lora driver is currently busy sending and receiving a
// message.
//
// Parameters - None.
//
// Returns:
//    TRUE - if busy sending/receiving a message.
//    FALSE - if not busy sending/receiving a message.
///////////////////////////////////////////////////////////////////////////////
int1 lora_busy(void)
{
   int1 Result = TRUE;

   switch(_g_Lora.DeviceType)
   {
      case LORA_DEVICE_TYPE_ED:
         if((_g_Lora.cMode == LORA_MODE_IDLE) && (_g_Lora.cMode == _g_Lora.pMode))
            Result = FALSE;
         break;
      case LORA_DEVICE_TYPE_BS:
      case LORA_DEVICE_TYPE_LED:
         if((_g_Lora.cMode == LORA_MODE_LISTEN) && (_g_Lora.cMode == _g_Lora.pMode))
            Result = FALSE;
         break;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// lora_has_message()
//
// Function used to determine if there is a new message to retrieve from
// receive buffer.
//
// Parameters - None.
//
// Returns: 
//    TRUE - if receive buffer has a new message to retrieve from buffer.
//    FALSE - if there is no new message to retrieve from buffer.
///////////////////////////////////////////////////////////////////////////////
int1 lora_has_message(void)
{
   if(_g_LoraRx.HasMessage)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// lora_get_message()
//
// Function used to retrieve a new message from receive buffer.
//
// Parameters:
//    Message - pointer to lora_rx_message_t type variable to return the
//              receive message to.
//
// Returns:
//    TRUE - if a new message was retrieve from receive buffer.
//    FALSE - if no new message was retrieved from receive buffer.
///////////////////////////////////////////////////////////////////////////////
int1 lora_get_message(lora_rx_message_t *Message)
{
   int1 Result;
   
   if(lora_has_message())
   {
      Message->Address = _g_LoraRx.Buffer.Source;
      Message->Length = _g_LoraRx.Buffer.Payload.Length;
      memcpy(Message->Data, _g_LoraRx.Buffer.Payload.Data, _g_LoraRx.Buffer.Payload.Length);
      Message->rssi = _g_LoraRx.rssi;
      Message->snr = _g_LoraRx.snr;
      
      _g_LoraRx.HasMessage = FALSE;
      Result = TRUE;
   }
   else
      Result = FALSE;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_channel_index()
//
// Function used to set the index in the channel hopping table to use for
// transmitting/receiving messages.  This will be useful for end-devices that
// only power on periodically to send a message, this way assuming the base 
// station hasn't change channels it will limit the amount of time sends power
// on and transmitting.  If used on a base station or listening end-device that
// is listening it will cause it to stop listen, switch channels and start 
// listening on new channel.
//
// Parameters:
//    Index - the channal index to transmit/receive messages on.
//
// Returns:
//    TRUE - if sucessfully changed
//    FALSE - if not changed, e.g. index is outside valid range.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_channel_index(uint8_t Index)
{
   int1 Result;

   if(Index < _g_LoraCh.Channels)
   {
      _g_LoraCh.HoppingIndex = Index;
      
      if((_g_Lora.DeviceType >= LORA_DEVICE_TYPE_BS) && (_g_Lora.cMode == LORA_MODE_LISTEN) && (_g_Lora.cMode == _g_Lora.pMode))
         _g_Lora.pMode = LORA_MODE_RX; //setting pMode to LORA_MODE_RX will cause it to stop listening
   }
   else
      Result = FALSE;
   
   return(Result);   
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_sync_byte()
//
// Function that can be used to set the synchronization byte used as the
// preamble during communication.
//
// Parameters:
//    sync - the uint8_t value to set the synchronization byte to.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_sync_byte(uint8_t sync)
{
   if(_lora_set_sync(sync))
      _lora_config.sync = sync;
   
   return(_lora_config.sync == sync);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_power()
//
// Function that can be used to set the transceivers TX power.
//
// Parameters:
//    pwr - the power to set the transceiver to, 2 to 20 dBm.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_power(uint8_t pwr)
{
   if(_lora_set_pwr(pwr))
      _lora_config.pwr = pwr;
   
   return(_lora_config.pwr == pwr);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_spreading_factor()
//
// Function that can be used to set the spreading factor to use.
//
// Parameters:
//    sf - lora_sf_t value to set the spreading factor to, LORA_SF_7 to 
//         LORA_SF_12.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_spreading_factor(lora_sf_t sf)
{
   if(_lora_set_sf(sf))
      _lora_config.sf = sf;
   
   return(_lora_config.sf == sf);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_coding_rate()
//
// Function that can be used to set the coding rate to use.
//
// Parameters:
//    cr - lora_cr_t value to set the coding rate to, LORA_CR_45 to LORA_CR_48.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_coding_rate(lora_cr_t cr)
{
   if(_lora_set_cr(cr))
      _lora_config.cr = cr;
   
   return(_lora_config.cr == cr);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_bandwidth()
//
// Function that can be used to set the bandwidth to use.
//
// Parameters:
//    bw - lora_bw_t value to set the bandwidth to, LORA_BW_125, LORA_BW_250 or
//         LORA_BW_500.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_bandwidth(lora_bw_t bw)
{
   if(_lora_set_bw(bw))
   {
      _lora_config.bw = bw;
      
      if(_lora_config.bw == LORA_BW_125)
         _g_LoraIncFreq = LORA_INC_FREQ_125_BW;
      else if(_lora_config.bw == LORA_BW_250)
         _g_LoraIncFreq = LORA_INC_FREQ_250_BW;
      else
         _g_LoraIncFreq = LORA_INC_FREQ_500_BW;
      
      _g_LoraStartFreq = LORA_START_FREQ + (_g_LoraIncFreq / 2);
   }
   
   return(_lora_config.bw == bw);
}

///////////////////////////////////////////////////////////////////////////////
// lora_enable_crc()
//
// Function that can be used to enable or disable the LoRa modules crc.
//
// Parameters:
//    enable - TRUE enables crc. and FALSE disables crc.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_enable_crc(int1 enable)
{
   if(_lora_enable_crc(enable))
      _lora_config.crc = enable;
   
   return(_lora_config.crc == enable);
}

///////////////////////////////////////////////////////////////////////////////
// lora_enable_iqi()
//
// Function that can be used to enable or disable the LoRa modules iqi.
//
// Parameters:
//    enable - TRUE enables iqi, and FALSE disables iqi.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_enable_iqi(int1 enable)
{
   if(_lora_enable_iqi(enable))
      _lora_config.iqi = enable;
   
   return(_lora_config.iqi == enable);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_channels()
//
// Function that can be used to set the number of channels the driver uses.
//
// Parameters:
//    channels - the number of channels to use, must be less then or equal to
//               the LORA_CHANNELS define.
//
// Returns:
//    TRUE - if set successfully.
//    FALSE - if not set.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_channels(uint8_t channels)
{
   if(channels <= LORA_CHANNELS)
   {
      _g_LoraCh.Channels = channels;
      
      if(_g_LoraCh.HoppingIndex >= _g_LoraCh.Channels)
         _g_LoraCh.HoppingIndex = 0;
   }
   
   return(_g_LoraCh.Channels == channels);
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_hopping_table()
//
// Function that can be used to set the channel hopping table to use.
//
// Parameters:
//    Table - pointer to uint8_t array contain channel hopping table.  Tables
//            size must be at least the same size as what is currently set
//            for the number of channels.  No check is made to make sure 
//            channels aren't reused, if that is done it will cause a situation
//            were it's possible to violate FCC regulations on how long a 
//            device can transmit on a single channel.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void lora_set_hopping_table(uint8_t *Table)
{
   uint8_t i;
   
   for(i=0;i<_g_LoraCh.Channels;i++)
      _g_LoraCh.HoppingTable[i] = Table[i];
}

///////////////////////////////////////////////////////////////////////////////
// lora_set_device_type()
//
// Function that can be used to set the device type.
//
// Parameters:
//    Type - lora_device_type_t value to set the device to.
//
// Returns:
//    TRUE - if successful.
//    FALSE - if not successful.
///////////////////////////////////////////////////////////////////////////////
int1 lora_set_device_type(lora_device_type_t Type)
{
   if(Type <= LORA_DEVICE_TYPE_LED)
   {
      _g_Lora.DeviceType = Type;
      
      if(_g_Lora.DeviceType == LORA_DEVICE_TYPE_ED)
      {
         if(_g_Lora.cMode == LORA_MODE_LISTEN)
         {
            _lora_stop_rx();
            _lora_set_mode(LORA_MODE_IDLE);
         }
      }
      else
      {
         if(_g_Lora.cMode == LORA_MODE_IDLE)
            _lora_set_mode(LORA_MODE_LISTEN);
      }
   }
   
   return(_g_Lora.DeviceType == Type);
}

///////////////////////////////////////////////////////////////////////////////
// lora_get_hopping_table()
//
// Function to get the channel hopping table that is currently being used.
//
// Parameters:
//    Table - pointer to an uint8_t array to return the channel hopping table
//            to.
//
// Returns:
//    uint8_t value representing the size of the table in bytes.
///////////////////////////////////////////////////////////////////////////////
uint8_t lora_get_hopping_table(uint8_t *Table)
{
   memcpy(Table, _g_LoraCh.HoppingTable, _g_LoraCh.Channels);
   
   return(_g_LoraCh.Channels);
}  

///////////////////////////////////////////////////////////////////////////////
// Local functions for driver, not intended to be called by user.
///////////////////////////////////////////////////////////////////////////////

void _lora_push_tx_message(_lora_message_t *Message)
{
   memcpy(&_g_LoraTx.Buffer, Message, sizeof(_lora_message_t));
      
   _g_LoraTx.HasMessage = TRUE;
}

void _lora_push_rx_message(_lora_message_t *Message)
{
   memcpy(&_g_LoraRx.Buffer, Message, sizeof(_lora_message_t));
   _g_LoraRx.rssi = _lora_get_rx_rssi();
   _g_LoraRx.snr = _lora_get_rx_snr();
   
   _g_LoraRx.HasMessage = TRUE;   
}

int1 _lora_tx_message(void)
{
   int1 Result;
   uint8_t len;
  #ifdef LORA_USE_MESSAGE_CRC
   uint8_t Data[LORA_MAX_PAYLOAD_SIZE + 5];
   uint16_t crc;
  #elif defined(LORA_USE_MESSAGE_CHECKSUM)
   uint8_t Data[LORA_MAX_PAYLOAD_SIZE + 4];
   uint8_t checksum;
   uint8_t i;
  #else
   uint8_t Data[LORA_MAX_PAYLOAD_SIZE + 3];
  #endif
  
   if(lora_tbe() == FALSE)
   {
      memcpy(Data, &_g_LoraTx.Buffer, 3);
      memcpy(&Data[3], _g_LoraTx.Buffer.Payload.Data, _g_LoraTx.Buffer.Payload.Length);
      
      len = 3 + _g_LoraTx.Buffer.Payload.Length;
      
     #ifdef LORA_USE_MESSAGE_CRC
      _lora_crc_init_value();
      crc = _lora_crc_calc(Data, len);
      
      Data[len++] = make8(crc, 0);
      Data[len++] = make8(crc, 1);
     #elif defined(LORA_USE_MESSAGE_CHECKSUM)
      checksum = 0;
      
      for(i=0;i<len;i++)
         checksum += Data[i];
      
      Data[len] = 0 - checksum;
      len++;
     #endif
      
      Result = _lora_putd(Data, len);
   }
   else
      Result = FALSE;
   
   return(Result);
}

void _lora_reset_mode(void)
{
   if(_g_Lora.DeviceType == LORA_DEVICE_TYPE_ED)
      _lora_set_mode(LORA_MODE_IDLE);
   else
      _lora_set_mode(LORA_MODE_LISTEN);
}

void _lora_rx_mode_task(_lora_tick_t cTick)
{
   if(_lora_tick_difference(cTick, _g_Lora.t) < (LORA_RX_TICKS + LORA_MAX_TX_TICKS))
      _g_Lora.pMode = LORA_MODE_IDLE;
   else
      _lora_reset_mode();
}
