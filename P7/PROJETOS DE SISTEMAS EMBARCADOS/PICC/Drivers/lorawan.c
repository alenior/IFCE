///////////////////////////////////////////////////////////////////////////////
////                               lorawan.c                               ////
////                                                                       ////
////  Driver for joining and communication on a LoRaWAN network.           ////
////                                                                       ////
////  See lorawan.h for API.                                               ////
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

#include <lorawan.h>

#ignore_warnings 236

#ifndef LORAWAN_JOIN_TYPE
 #define LORAWAN_JOIN_TYPE      LORAWAN_JOIN_TYPE_OTAA
#endif

#ifndef LORAWAN_SYNC_BYTE
 #define LORAWAN_SYNC_BYTE       0x34
#endif

#ifndef LORAWAN_RETRANSMISSION_ATTEMPTS
 #define LORAWAN_RETRANSMISSION_ATTEMPTS  5
#endif

#ifndef LORAWAN_RX_DELAY
 #define LORAWAN_RX_DELAY        1000  //in milliseconds
#endif

#ifndef LORAWAN_RX2_DATA_RATE
 #define LORAWAN_RX2_DATA_RATE   8
#endif

#ifndef LORAWAN_RX2_FREQ
 #define LORAWAN_RX2_FREQ        923300000
#endif

#ifndef LORAWAN_AUTO_REPLY
 #define LORAWAN_AUTO_REPLY      FALSE
#endif

#ifndef LORAWAN_PAUSED_RETRY_TIME
 #define LORAWAN_PAUSED_RETRY_TIME        10    //in seconds, when the LoRaWAN stack is paused how frequently in seconds it will check to see if the stack is no longer paused
#endif

#ifndef LORAWAN_BUSY_RETRY_TIME
 #define LORAWAN_BUSY_RETRY_TIME          2000  //in milliseconds
#endif

#ifndef LORAWAN_JOIN_DENIED_RETRY_TIME
 #define LORAWAN_JOIN_DENIED_RETRY_TIME   30    //in seconds
#endif

#ifndef LORAWAN_MINIMUM_DATA_RATE
 #define LORAWAN_MINIMUM_DATA_RATE        0     //0 to 3, 0 is lowest data rate, 3 is fastest
#endif

#if LORAWAN_MINIMUM_DATA_RATE > 3
 #error Minimum data rate must be less then 4
#endif

#ifndef LORAWAN_MAXIMUM_DATA_RATE
 #define LORAWAN_MAXIMUM_DATA_RATE        3     //0 to 3, 0 is lowest data rate, 3 is fastest
#endif

#if LORAWAN_MAXIMUM_DATA_RATE < LORAWAN_MAXIMUM_DATA_RATE
 #error Maximum data rate must be greater then or equal to Minimum data rate
#endif

#if LORAWAN_MAXIMUM_DATA_RATE > 3
 #error Maximum data rate must be less then 4
#endif

#ifndef LORAWAN_DEFAULT_TX_DATA_RATE
 #define LORAWAN_DEFAULT_TX_DATA_RATE     0     //0 to 4, 0 is lowest data rate, 4 is fastest
#endif

#if LORAWAN_DEFAULT_TX_DATA_RATE > 4
 #error Default TX data rate must be less then 5
#endif

#ifndef LORAWAN_MINIMUM_CHANNEL
 #define LORAWAN_MINIMUM_CHANNEL    0     //0 to 63, used to set the first channel for sending LoRaWAN messages
#endif

#ifndef LORAWAN_MAXIMUM_CHANNEL
 #define LORAWAN_MAXIMUM_CHANNEL    63    //0 to 63, used to set the last channel used for sending LoRaWAN messages
#endif

#if LORAWAN_JOIN_TYPE == LORAWAN_JOIN_TYPE_OTAA
 #if !defined(LORAWAN_DEFAULT_APP_EUI)
  #if definedinc(LORAWAN_DEFAULT_APP_EUI) == 0
   #error LORAWAN_DEFAULT_APP_EUI needs defined to join via Over-The-Air Activation
  #endif
 #endif
 
 #if !defined(LORAWAN_DEFAULT_APP_KEY)
  #if definedinc(LORAWAN_DEFAULT_APP_KEY) == 0
   #error LORAWAN_DEFAULT_APP_KEY needs defined to join via Over-The-Air Activation
  #endif
 #endif
#else
 #if !defined(LORAWAN_DEFAULT_DEVICE_ADDR)
  #if definedinc(LORAWAN_DEFAULT_DEVICE_ADDR) == 0
   #error LORAWAN_DEFAULT_DEVICE_ADDR needs defined to join via Activation By Personalization
  #endif
 #endif
  
 #if !defined(LORAWAN_DEFAULT_NWK_SESSION_KEY)
  #if definedinc(LORAWAN_DEFAULT_NWK_SESSION_KEY) == 0
   #error LORAWAN_DEFAULT_NWK_SESSION_KEY needs defined to join via Activation By Personalization
  #endif
 #endif
  
 #if !defined(LORAWAN_DEFAULT_APP_SESSION_KEY)
  #if definedinc(LORAWAN_DEFAULT_APP_SESSION_KEY) == 0
   #error LORAWAN_DEFAULT_APP_SESSION_KEY needs defined to join via Activation By Personalization
  #endif
 #endif
#endif

#if !defined(_lorawan_tick_t) || !defined(_LORAWAN_TICKS_PER_SECOND) || !defined(_lorawan_tick_difference) || !defined(_lorawan_tick_get)
 #if !defined(_lorawan_tick_difference)
  #if definedinc(_lorawan_tick_difference) == 0
   #error driver needs a tick timer provided to it
  #endif
 #endif
 
 #if !defined(_lorawan_tick_get)
  #if definedinc(_lorawan_tick_get) == 0
   #error driver needs a tick timer provided to it
  #endif
 #endif
 
 #if !defined(_lorawan_tick_t) || !defined(_LORAWAN_TICK_PER_SECOND)
  #error driver needs a tick timer provided to it
 #endif
#endif

#define LORAWAN_PAUSED_RETRY_TICKS        (LORAWAN_PAUSED_RETRY_TIME * _LORAWAN_TICKS_PER_SECOND)
#define LORAWAN_BUSY_RETRY_TICKS          ((LORAWAN_BUSY_RETRY_TIME * _LORAWAN_TICKS_PER_SECOND) / 1000)
#define LORAWAN_JOIN_DENIED_RETRY_TICKS   (LORAWAN_JOIN_DENIED_RETRY_TIME * _LORAWAN_TICKS_PER_SECOND)

#if defined(LORAWAN_CHECK_JOIN_STATUS_TIME) && (LORAWAN_CHECK_JOIN_STATUS_TIME > 0)
 #define LORAWAN_CHECK_JOIN_STATUS_TICKS  (LORAWAN_CHECK_JOIN_STATUS_TIME * _LORAWAN_TICKS_PER_SECOND)
#endif

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
   LORAWAN_TX_TYPE_CNF,
   LORAWAN_TX_TYPE_UNCNF
} lorawan_tx_type_t;

typedef struct _PACKED_STRUCT
{
   uint8_t Port;
   uint8_t Data[LORAWAN_MAX_PAYLOAD_SIZE];
   uint8_t Length;
   lorawan_tx_type_t Type; 
} _lorawan_tx_message_t;

/////////////////////////////// Global Variables //////////////////////////////

struct
{
   lorawan_join_state_t JoinState;
   _lorawan_module_error_t JoinError;
   lorawan_tx_error_t TxError;
   lorawan_mode_t cMode;
   lorawan_mode_t pMode;
   _lorawan_tick_t t;
} _g_Lorawan;

struct
{
   _lorawan_tx_message_t Buffer;
   int1 HasMessage;
} _g_LorawanTx;

struct
{
   lorawan_rx_message_t Buffer;
   int1 HasMessage;
} _g_LorawanRx;

////////////////////////////// Local Prototypes ///////////////////////////////

#define _lorawan_set_mode(m)  (_g_Lorawan.cMode = m)

void _lorawan_push_tx_message(_lorawan_tx_message_t *Message);
void _lorawan_push_rx_message(uint8_t *Data, uint8_t Length);

_lorawan_module_error_t _lorawan_tx_message(void);
lorawan_tx_error_t _lorawan_get_tx_error_from_module_error(_lorawan_module_error_t Error);
lorawan_tx_error_t _lorawan_get_tx_error_from_module_resp(lorawan_module_resp_t Response);

///////////////////////////////////// API /////////////////////////////////////

int1 lorawan_init(void)
{
   int1 Result;
   
   debug_lorawan_printf(debug_lorawan_putc, "lorawan_init()\r\n");
   
   memset(&_g_LorawanTx, 0x00, sizeof(_g_LorawanTx));
   memset(&_g_LorawanRx, 0x00, sizeof(_g_LorawanRx));
   memset(&_g_Lorawan, 0x00, sizeof(_g_Lorawan));
   
   _lorawan_init_radio();
   
   Result = _lorawan_get_config();
   
   debug_lorawan_printf(debug_lorawan_putc, "   _lorawan_get_config() - %u\r\n", Result);
   
   _lorawan_config.FirstChannel = LORAWAN_MINIMUM_CHANNEL;
   _lorawan_config.LastChannel = LORAWAN_MAXIMUM_CHANNEL;
   
   if(Result == TRUE)
   {
      if(_g_Lorawan.JoinState == LORAWAN_JOIN_STATE_NOT_JOINED)
      {
         _lorawan_config.Sync = LORAWAN_SYNC_BYTE;
         _lorawan_config.joinType = LORAWAN_JOIN_TYPE;
         _lorawan_config.RetransmissionAttempts = LORAWAN_RETRANSMISSION_ATTEMPTS; 
         _lorawan_config.RXDelay = LORAWAN_RX_DELAY;
         _lorawan_config.RX2DataRate = LORAWAN_RX2_DATA_RATE;
         _lorawan_config.RX2Frequency = LORAWAN_RX2_FREQ;
         _lorawan_config.AutoReply = LORAWAN_AUTO_REPLY;
         _lorawan_config.MinDataRate = LORAWAN_MINIMUM_DATA_RATE;
         _lorawan_config.MaxDataRate = LORAWAN_MAXIMUM_DATA_RATE;
         
        #if LORAWAN_JOIN_TYPE == LORAWAN_JOIN_TYPE_OTAA
         memcpy(_lorawan_config.AppEUI, LORAWAN_DEFAULT_APP_EUI, LORAWAN_EUI_SIZE);
         memcpy(_lorawan_config.AppKey, LORAWAN_DEFAULT_APP_KEY, LORAWAN_KEY_SIZE);
        #else
         _lorawan_config.DeviceAddr = LORAWAN_DEFAULT_DEVICE_ADDR;
         memcpy(_lorawan_config.NetworkSessionKey, LORAWAN_DEFAULT_NWK_SESSION_KEY, LORAWAN_KEY_SIZE);
         memcpy(_lorawan_config.AppSessionKey, LORAWAN_DEFAULT_APP_SESSION_KEY, LORAWAN_KEY_SIZE);
        #endif
      
         Result = _lorawan_set_config();
         
         if(Result)
            Result = _lorawan_save_settings();
      }
      else
      {
         //Keys can't be read from module so set them to the default value
        #if LORAWAN_JOIN_TYPE == LORAWAN_JOIN_TYPE_OTAA
         memcpy(_lorawan_config.AppKey, LORAWAN_DEFAULT_APP_KEY, LORAWAN_KEY_SIZE);
         
         //Used for ABP joining, so set them to 0
         memset(_lorawan_config.NetworkSessionKey, 0x00, LORAWAN_KEY_SIZE);
         memset(_lorawan_config.AppSessionKey, 0x00, LORAWAN_KEY_SIZE);
        #else
         memcpy(_lorawan_config.NetworkSessionKey, LORAWAN_DEFAULT_NWK_SESSION_KEY, LORAWAN_KEY_SIZE);
         memcpy(_lorawan_config.AppSessionKey, LORAWAN_DEFAULT_APP_SESSION_KEY, LORAWAN_KEY_SIZE);
         
         //used by OTAA joining, so set to 0
         memset(_lorawan_config.AppKey, 0x00, LORAWAN_KEY_SIZE);
        #endif
      }
      
      _lorawan_config.TxDataRate = LORAWAN_DEFAULT_TX_DATA_RATE;
   }
   else
      Result = FALSE;
   
   if(_g_Lorawan.JoinState == LORAWAN_JOIN_STATE_JOINED)
      _lorawan_set_mode(LORAWAN_MODE_IDLE);
   else
      _lorawan_set_mode(LORAWAN_MODE_JOIN);
   
   _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
   
   return(Result);
}

int1 lorawan_init_join_info(void)
{
   int1 Result = FALSE;

   if(_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA)
   {
      if(_lorawan_set_app_eui(_lorawan_config.AppEUI))
      {
         if(_lorawan_set_app_key(_lorawan_config.AppKey))
            Result = TRUE;
      }
   }
   else
   {
      if(_lorawan_set_device_address(_lorawan_config.DeviceAddr))
      {
         if(_lorawan_set_network_session_key(_lorawan_config.NetworkSessionKey))
         {
            if(_lorawan_set_app_session_key(_lorawan_config.AppSessionKey))
               Result = TRUE;
         }
      }
   }
   
   if(Result == TRUE)
   {
      Result = _lorawan_save_settings();
      
      if(Result)
      {
         _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
         
         _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
         _lorawan_set_mode(LORAWAN_MODE_JOIN);
      }
   }
   
   return(Result);
}

void lorawan_task(void)
{
   static lorawan_mode_t pMode;
   static _lorawan_tick_t retryTicks;
   _lorawan_tick_t cTick;
   lorawan_module_resp_t Response;
   _lorawan_module_error_t Error;
   uint8_t Data[(LORAWAN_MAX_PAYLOAD_SIZE * 2) + 1];
   uint8_t Length;
   
   _lorawan_rx_task();
   cTick = _lorawan_tick_get();
   
   if(_g_Lorawan.cMode != _g_Lorawan.pMode)
   {
      debug_lorawan_printf(debug_lorawan_putc, "lorawan_task() - cMode: %u, pMode: %u\r\n", _g_Lorawan.cMode, _g_Lorawan.pMode);
   
      switch(_g_Lorawan.cMode)
      {
         case LORAWAN_MODE_JOIN:
            if(_g_Lorawan.JoinState == LORAWAN_JOIN_STATE_NOT_JOINED)
            {
               _g_Lorawan.JoinError = _lorawan_join(_lorawan_config.joinType);
               
               if(_g_Lorawan.JoinError != LORAWAN_MODULE_ERROR_OK)
               {
                  switch(_g_Lorawan.JoinError)
                  {
                     case LORAWAN_MODULE_ERROR_PARAMETER:
                     case LORAWAN_MODULE_ERROR_SILENT:
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_JOIN_ERROR);
                        
                        _lorawan_set_join_state(LORAWAN_JOIN_STATE_ERROR);
                        return;
                        
                        break;
                     case LORAWAN_MODULE_ERROR_NOT_INIT:
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_JOIN_ERROR);
                        
                        _lorawan_set_join_state(LORAWAN_JOIN_STATE_ERROR_NOT_INIT);
                        return;
                        
                        break;
                     case LORAWAN_MODULE_ERROR_CHANNEL:
                     case LORAWAN_MODULE_ERROR_BUSY:
                        retryTicks = LORAWAN_BUSY_RETRY_TICKS;
                        
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_BUSY);
                        return;
                        
                        break;
                     case LORAWAN_MODULE_ERROR_PAUSED:
                        retryTicks = LORAWAN_PAUSED_RETRY_TICKS;
                       
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_PAUSED);
                        return;
                        
                        break;
                  }
               }
               else
                  _lorawan_set_join_state(LORAWAN_JOIN_STATE_JOINING);
            }
            else if(_g_Lorawan.JoinState == LORAWAN_JOIN_STATE_JOINED)
            {
               _lorawan_set_mode(LORAWAN_MODE_IDLE);
               return;
            }
            else if(_g_Lorawan.JoinState == LORAWAN_JOIN_STATE_ERROR)
            {
               _g_Lorawan.pMode = _g_Lorawan.cMode;
               _lorawan_set_mode(LORAWAN_MODE_JOIN_ERROR);
               return;
            }

            break;
         case LORAWAN_MODE_JOIN_ERROR:
            pMode = (lorawan_mode_t)_g_Lorawan.pMode;
            break;
         case LORAWAN_MODE_JOIN_DENIED:
         case LORAWAN_MODE_PAUSED:
         case LORAWAN_MODE_BUSY:
            pMode = (lorawan_mode_t)_g_Lorawan.pMode;
            _g_Lorawan.t = cTick;
            break;
         case LORAWAN_MODE_IDLE:
           #ifdef LORAWAN_CHECK_JOIN_STATUS_TICKS
            if(_g_Lorawan.pMode != LORAWAN_MODE_TX_DATA)
               _g_Lorawan.t = cTick;
           #endif
            break;
         case LORAWAN_MODE_TX_DATA:
            if(!lorawan_tbe())
            {
               Error = _lorawan_tx_message();
               _g_Lorawan.TxError = _lorawan_get_tx_error_from_module_error(Error);
               
               if(Error != LORAWAN_MODULE_ERROR_OK)
               {
                  debug_lorawan_printf(debug_lorawan_putc, "_lorawan_tx_message() - Error: %u\r\n", Error);
                  switch(Error)
                  {
                     case LORAWAN_MODULE_ERROR_PARAMETER:
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_ERROR);
                        
                        _g_LorawanTx.HasMessage = FALSE;
                        break;
                     case LORAWAN_MODULE_ERROR_NOT_JOINED:
                     case LORAWAN_MODULE_ERROR_FRAME_COUNTER:
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_JOIN);
                        _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
                        
                        break;
                     case LORAWAN_MODULE_ERROR_CHANNEL:
                     case LORAWAN_MODULE_ERROR_BUSY:
                        retryTicks = LORAWAN_BUSY_RETRY_TICKS;
                        
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_BUSY);
                        
                        break;
                     case LORAWAN_MODULE_ERROR_SILENT:
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_ERROR);
                        
                        break;
                     case LORAWAN_MODULE_ERROR_PAUSED:
                        retryTicks = LORAWAN_PAUSED_RETRY_TICKS;
                       
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_PAUSED);
                        
                        break;
                     case LORAWAN_MODULE_ERROR_LENGTH:
                        _g_Lorawan.pMode = _g_Lorawan.cMode;
                        _lorawan_set_mode(LORAWAN_MODE_IDLE);
                        
                        _g_LorawanTx.HasMessage = FALSE;
                        break;
                  }
                  
                  return;
               }
               else
                  _g_LorawanTx.HasMessage = FALSE;
            }
            else
            {
               _lorawan_set_mode(LORAWAN_MODE_IDLE);
               _g_Lorawan.pMode = LORAWAN_MODE_TX_DATA;
            }
            break;
            
         case LORAWAN_MODE_ERROR:
            pMode = (lorawan_mode_t)_g_Lorawan.pMode;
            break;
      }
      
      _g_Lorawan.pMode = _g_Lorawan.cMode;
   }
   else
   {
      switch(_g_Lorawan.cMode)
      {
         case LORAWAN_MODE_JOIN:
            if(_lorawan_kbhit())
            {
               Response = _lorawan_get_radio_message(Data, &Length);
               
               debug_lorawan_printf(debug_lorawan_putc, "LORAWAN_MODE_JOIN - Response: %u\r\n", Response);
               if(Response == LORAWAN_MODULE_RESP_DENIED)
               {
                  _lorawan_set_mode(LORAWAN_MODE_JOIN_DENIED);
                  _lorawan_set_join_state(LORAWAN_JOIN_STATE_DENIED);
               }
               else if(Response == LORAWAN_MODULE_RESP_ACCEPTED)
               {
                  _lorawan_set_mode(LORAWAN_MODE_IDLE);
                  _lorawan_set_join_state(LORAWAN_JOIN_STATE_JOINED);
               }
               else
               {
                  //Shouldn't be able to get here, the only responses it should
                  //get at this point in the state machine is DENIED or 
                  //ACCEPTED, in case it does restart the join procedure.                  
                  _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
                  _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
               }
            }
            break;
          case LORAWAN_MODE_JOIN_ERROR:
          case LORAWAN_MODE_ERROR:
            //don't do anything error needs cleared by user
            break;
         case LORAWAN_MODE_JOIN_DENIED:
         case LORAWAN_MODE_PAUSED:
         case LORAWAN_MODE_BUSY:
            if(_lorawan_tick_difference(cTick, _g_Lorawan.t) >= retryTicks)
            {
               if(_g_Lorawan.cMode == LORAWAN_MODE_PAUSED)
               {
                  if(!_lorawan_paused())
                     _lorawan_set_mode(pMode);
                  else
                     _g_Lorawan.t = cTick;
               }
               else
               {
                  if(_g_Lorawan.cMode == LORAWAN_MODE_JOIN_DENIED)
                     _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
               
                  _lorawan_set_mode(pMode);
               }
            }
            
            break;
         case LORAWAN_MODE_IDLE:
            if(_lorawan_kbhit())
            {
               Response = _lorawan_get_radio_message(Data, &Length);
               
               switch(Response)
               {
                  case LORAWAN_MODULE_RESP_RX:
                     
                     break;
               }
            }
            
           #ifdef LORAWAN_CHECK_JOIN_STATUS_TICKS
            if(_lorawan_tick_difference(cTick, _g_Lorawan.t) >= LORAWAN_CHECK_JOIN_STATUS_TICKS)
            {
               if(_lorawan_joined() == FALSE)
               {
                  _lorawan_set_mode(LORAWAN_MODE_JOIN);
                  _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
               }
               else
                  _g_Lorawan.t = cTick;
            }
           #endif
            
            break;
         case LORAWAN_MODE_TX_DATA:
            if(_lorawan_kbhit())
            {
               Response = _lorawan_get_radio_message(Data, &Length);
               _g_Lorawan.TxError = _lorawan_get_tx_error_from_module_resp(Response);
               
               switch(Response)
               {
                  case LORAWAN_MODULE_RESP_TX_OK:
                  case LORAWAN_MODULE_RESP_LENGTH:   
                     _lorawan_set_mode(LORAWAN_MODE_IDLE);
                     break;
                  case LORAWAN_MODULE_RESP_RX:
                     _lorawan_push_rx_message(Data, Length);
                     
                     _lorawan_set_mode(LORAWAN_MODE_IDLE);
                     break;
                  case LORAWAN_MODULE_RESP_ERR:
                     _lorawan_set_mode(LORAWAN_MODE_ERROR);
                     break;
               }
            }
            
            break;
      }
   }
}

int1 lorawan_tbe(void)
{
   if(_g_LorawanTx.HasMessage == FALSE)
      return(TRUE);
   else
      return(FALSE);
}

int1 lorawan_busy(void)
{
   return(_g_Lorawan.cMode != LORAWAN_MODE_IDLE);
}

int1 lorawan_put_message(uint8_t Port, uint8_t *Data, uint8_t Length, int1 Confirm)
{
   _lorawan_tx_message_t Message;
   int1 Result;

   if(lorawan_tbe())
   {
      if(_g_Lorawan.cMode == LORAWAN_MODE_IDLE)
      {
         _lorawan_set_tx_dr();
         Message.Port = Port;
         Message.Length = Length;
         if(Confirm)
            Message.Type = LORAWAN_TX_TYPE_CNF;
         else
            Message.Type = LORAWAN_TX_TYPE_UNCNF;
         memcpy(Message.Data, Data, Length);
         
         _lorawan_push_tx_message(&Message);
         _lorawan_set_mode(LORAWAN_MODE_TX_DATA);
         
         Result = TRUE;
      }
      else
         Result = FALSE;
   }
   else
      Result = FALSE;
   
   return(Result);
}

int1 lorawan_has_message(void)
{
   return(_g_LorawanRx.HasMessage);
}

int1 lorawan_get_message(lorawan_rx_message_t *Message)
{
   if(lorawan_has_message())
   {
      Message->Port = _g_LorawanRx.Buffer.Port;
      memcpy(Message->Data, _g_LorawanRx.Buffer.Data, _g_LorawanRx.Buffer.Length);
      Message->Length = _g_LorawanRx.Buffer.Length;
      
      _g_LorawanRx.HasMessage = FALSE;
   }
   else
      return(FALSE);
}

int1 lorawan_set_sync_byte(uint8_t Sync)
{
   if(Sync != _lorawan_config.Sync)
   {
      if(_lorawan_set_sync_byte(Sync))
      {
         if(_lorawan_save_settings())
         {
            _lorawan_config.Sync = Sync;
            
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
            
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   
   return(_lorawan_config.Sync == Sync);
}

int1 lorawan_set_device_eui(uint8_t *eui)
{
   int1 Result;
   uint8_t i;

   for(i=0;i<LORAWAN_EUI_SIZE;i++)
   {
      if(eui[i] != _lorawan_config.DeviceEUI[i])
         break;
   }
   
   if((i < LORAWAN_EUI_SIZE) && (_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA))
   {
      Result = _lorawan_set_device_eui(eui);
      
      if(Result == TRUE)
      {
         Result = _lorawan_save_settings();
         
         if(Result == TRUE)
         {
            memcpy(_lorawan_config.DeviceEUI, eui, LORAWAN_EUI_SIZE);
            
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
            
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   else if(_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA)
      Result = TRUE;
   else
      Result = FALSE;
      
   return(Result);
}

int1 lorawan_set_app_eui(uint8_t *eui)
{
   int1 Result;
   uint8_t i;
   
   for(i=0;i<LORAWAN_EUI_SIZE;i++)
   {
      if(eui[i] != _lorawan_config.AppEUI[i])
         break;
   }
   
   if((i < LORAWAN_EUI_SIZE) && (_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA))
   {
      Result = _lorawan_set_app_eui(eui);
      
      if(Result == TRUE)
      {
         Result = _lorawan_save_settings();
         
         if(Result == TRUE)
         {
            memcpy(_lorawan_config.AppEUI, eui, LORAWAN_EUI_SIZE);
            
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
            
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   else if(_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA)
      Result = TRUE;
   else
      Result = FALSE;
   
   return(Result);
}

int1 lorawan_set_app_key(uint8_t *Key)
{
   int1 Result;
   uint8_t i;
   
   for(i=0;i<LORAWAN_KEY_SIZE;i++)
   {
      if(Key[i] != _lorawan_config.AppKey[i])
         break;
   }
   
   if((i < LORAWAN_KEY_SIZE) && (_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA))
   {
      Result = _lorawan_set_app_key(Key);
      
      if(Result == TRUE)
      {
         Result = _lorawan_save_settings();
         
         if(Result == TRUE)
         {
            memcpy(_lorawan_config.AppKey, Key, LORAWAN_KEY_SIZE);
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
             
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   else if(_lorawan_config.joinType == LORAWAN_JOIN_TYPE_OTAA)
      Result = TRUE;
   else
      Result = FALSE;
   
   return(Result);
}

int1 lorawan_set_network_session_key(uint8_t *Key)
{
   int1 Result;
   uint8_t i;
   
   for(i=0;i<LORAWAN_KEY_SIZE;i++)
   {
      if(Key[i] != _lorawan_config.NetworkSessionKey[i])
         break;
   }
   
   if((i < LORAWAN_KEY_SIZE) && (_lorawan_config.joinType == LORAWAN_JOIN_TYPE_ABP))
   {
      Result = _lorawan_set_network_session_key(Key);
      
      if(Result == TRUE)
      {
         Result = _lorawan_save_settings();
         
         if(Result == TRUE)
         {
            memcpy(_lorawan_config.NetworkSessionKey, Key, LORAWAN_KEY_SIZE);
            
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
            
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   else if(_lorawan_config.joinType == LORAWAN_JOIN_TYPE_ABP)
      Result = TRUE;
   else
      Result = FALSE;
   
   return(FALSE);
}

int1 lorawan_set_app_session_key(uint8_t *Key)
{
   int1 Result;
   uint8_t i;
   
   for(i=0;i<LORAWAN_KEY_SIZE;i++)
   {
      if(Key[i] != _lorawan_config.AppSessionKey[i])
         break;
   }
   
   if((i < LORAWAN_KEY_SIZE) && (_lorawan_config.joinType == LORAWAN_JOIN_TYPE_ABP))
   {
      Result = _lorawan_set_app_session_key(Key);
      
      if(Result == TRUE)
      {
         Result = _lorawan_save_settings();
         
         if(Result == TRUE)
         {
            memcpy(_lorawan_config.AppSessionKey, Key, LORAWAN_KEY_SIZE);
            
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
            
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   else if(_lorawan_config.joinType == LORAWAN_JOIN_TYPE_ABP)
      Result = TRUE;
   else
      Result = FALSE;
   
   return(FALSE);
}

int1 lorawan_set_device_address(uint32_t Address)
{
   if((Address != _lorawan_config.DeviceAddr) && (_lorawan_config.joinType == LORAWAN_JOIN_TYPE_ABP))
   {
      if(_lorawan_set_device_address(Address))
      {
         if(_lorawan_save_settings())
         {
            _lorawan_config.DeviceAddr = Address;
            
            _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
            
            _g_Lorawan.pMode = LORAWAN_MODE_UNKNOWN;
            _lorawan_set_mode(LORAWAN_MODE_JOIN);
         }
      }
   }
   
   return(Address == _lorawan_config.DeviceAddr);
}

int1 lorawan_set_retransmission_attempts(uint8_t Attempts)
{
   if(_lorawan_set_retransmission_attempts(Attempts))
      _lorawan_config.RetransmissionAttempts = Attempts;
   
   return(Attempts == _lorawan_config.RetransmissionAttempts);
}

int1 lorawan_set_rx_delay(uint16_t Delay)
{
   if(_lorawan_set_rx_delay(Delay))
      _lorawan_config.RXDelay = Delay;
   
   return(Delay == _lorawan_config.RXDelay);
}

int1 lorawan_set_rx2_data_rate(uint8_t Rate)
{
   if(_lorawan_set_rx2_data_rate(Rate))
      _lorawan_config.RX2DataRate = Rate;
   
   return(Rate == _lorawan_config.RX2DataRate);
}

int1 lorawan_set_rx2_frequency(uint32_t Frequency)
{
   if(_lorawan_set_rx2_frequency(Frequency))
      _lorawan_config.RX2Frequency = Frequency;
   
   return(Frequency == _lorawan_config.RX2Frequency);
}

int1 lorawan_set_auto_reply(int1 AutoReply)
{
   if(_lorawan_set_auto_reply(AutoReply))
      _lorawan_config.AutoReply = AutoReply;
   
   return(AutoReply == _lorawan_config.AutoReply);
}

lorawan_join_state_t lorawan_get_join_state(void)
{
   return(_g_Lorawan.JoinState);
}

lorawan_tx_error_t lorawan_get_tx_error(void)
{
   return(_g_Lorawan.TxError);
}

lorawan_mode_t lorawan_get_mode(void)
{
   return(_g_Lorawan.cMode);
}
///////////////////////////////////////////////////////////////////////////////

void _lorawan_set_join_state(lorawan_join_state_t State)
{
   _g_Lorawan.JoinState = State;
}

void _lorawan_push_tx_message(_lorawan_tx_message_t *Message)
{
   memcpy(&_g_LorawanTx.Buffer, Message, sizeof(_lorawan_tx_message_t));
   _g_LorawanTx.HasMessage = TRUE;
}

void _lorawan_push_rx_message(uint8_t *Data, uint8_t Length)
{
   _g_LorawanRx.Buffer.Port = Data[0];
   _g_LorawanRx.Buffer.Length = Length - 1;
   memcpy(&_g_LorawanRx.Buffer.Data, &Data[1], _g_LorawanRx.Buffer.Length);
   
   _g_LorawanRx.HasMessage = TRUE;
}

_lorawan_module_error_t _lorawan_tx_message(void)
{
   _lorawan_module_error_t Result;
   
   Result = _lorawan_putd(_g_LorawanTx.Buffer.Port, _g_LorawanTx.Buffer.Data, _g_LorawanTx.Buffer.Length, _g_LorawanTx.Buffer.Type);
   
   return(Result);
}

lorawan_tx_error_t _lorawan_get_tx_error_from_module_error(_lorawan_module_error_t Error)
{
   lorawan_tx_error_t txError;
   
   switch(Error)
   {
      case LORAWAN_MODULE_ERROR_PARAMETER:
         txError = LORAWAN_TX_ERROR_PARAM;
         break;
      case LORAWAN_MODULE_ERROR_LENGTH:
         txError = LORAWAN_TX_ERROR_LENGTH;
         break;
      default:
         txError = LORAWAN_TX_ERROR_OK;
         break;
   }
   
   return(txError);
}

lorawan_tx_error_t _lorawan_get_tx_error_from_module_resp(lorawan_module_resp_t Response)
{
   lorawan_tx_error_t txError;

   switch(Response)
   {
      case LORAWAN_MODULE_RESP_ERR:
         txError = LORAWAN_TX_ERROR_ERROR;
         break;
      case LORAWAN_MODULE_RESP_LENGTH:
         txError = LORAWAN_TX_ERROR_LENGTH;
         break;
      default:
         txError = LORAWAN_TX_ERROR_OK;
         break;
   }
   
   return(txError);
}

#ignore_warnings none
