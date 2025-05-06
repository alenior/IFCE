///////////////////////////////////////////////////////////////////////////////
////                               rn2903.c                                ////
////                                                                       ////
////  Driver for Microchip's RN2903 Low-Power Long Range LoRa transceiver  ////
////  module.                                                              ////
////                                                                       ////
////  See rn2903.h for API.                                                ////
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

#include <rn2903.h>

#ignore_warnings  242

////////////////////////////// Types and Defines //////////////////////////////

#ifndef RN2903_TX_PIN
 #define RN2903_TX_PIN     PIN_C6
#endif

#ifndef RN2903_RX_PIN
 #define RN2903_RX_PIN     PIN_C7
#endif

#ifndef RN2903_BAUD
 #define RN2903_BAUD       57600 //default of RN2903 module
#endif

#ifndef RN2903_SERIAL_BUFFER_SIZE
 #define RN2903_SERIAL_BUFFER_SIZE  64 //set this to greater then 0 causes #use rs232() to use receive buffer, 
#endif                                 //which use either the UART peripheral's RDA interrupt or the one of the
                                       //external interrupt interrupts.

#use rs232(xmit=RN2903_TX_PIN, rcv=RN2903_RX_PIN, baud=RN2903_BAUD, RECEIVE_BUFFER=RN2903_SERIAL_BUFFER_SIZE, stream=RN2903_SERIAL_STREAM, errors)

#ifndef RN2903_RX_MESSAGE_BUFFERS
 #define RN2903_RX_MESSAGE_BUFFERS        1
#endif

#ifndef RN2903_RX_MESSAGE_BUFFER_SIZE
 #define RN2903_RX_MESSAGE_BUFFER_SIZE    64
#endif

#define RN2903_MAX_EC_RESPONSE_BYTES      30
#define RN2903_MAX_RESP_BYTES             16

#define EUI_SIZE_BYTES       8
#define EUI_RESPONSE_SIZE    16

#define KEY_SIZE_BYTES        16

typedef enum
{
   RN2903_CMD_TYPE_SYS,
   RN2903_CMD_TYPE_SYS_SET,
   RN2903_CMD_TYPE_SYS_GET,
   RN2903_CMD_TYPE_MAC,
   RN2903_CMD_TYPE_MAC_SET,
   RN2903_CMD_TYPE_MAC_GET,
   RN2903_CMD_TYPE_RADIO,
   RN2903_CMD_TYPE_RADIO_SET,
   RN2903_CMD_TYPE_RADIO_GET
} _rn2903_cmd_type_t;

typedef enum
{
   SYS_CMD_SLEEP,
   SYS_CMD_RESET,
   SYS_CMD_ERASE_FW,
   SYS_CMD_FACTORY_RESET
} _sys_cmd_t;

typedef enum
{
   SYS_SET_CMD_NVM,
   SYS_SET_CMD_PINDIG,
   SYS_SET_CMD_PINMODE
} _sys_set_cmd_t;

typedef enum
{
   SYS_GET_CMD_VER,
   SYS_GET_CMD_NVM,
   SYS_GET_CMD_VDD,
   SYS_GET_CMD_HWEUI,
   SYS_GET_CMD_PINDIG,
   SYS_GET_CMD_PINANA
} _sys_get_cmd_t;

typedef enum
{
   MAC_CMD_RESET,
   MAC_CMD_TX,
   MAC_CMD_JOIN,
   MAC_CMD_SAVE,
   MAC_CMD_FORCE_ENABLE,
   MAC_CMD_PAUSE,
   MAC_CMD_RESUME
} _mac_cmd_t;

typedef enum
{
   MAC_SET_CMD_APPKEY,
   MAC_SET_CMD_APPSKEY,
   MAC_SET_CMD_APPEUI,
   MAC_SET_CMD_AR,
   MAC_SET_CMD_BAT,
   MAC_SET_CMD_CH_DR_RANGE,
   MAC_SET_CMD_CH_STATUS,
   MAC_SET_CMD_CLASS,
   MAC_SET_CMD_DEVADDR,
   MAC_SET_CMD_DEVEUI,
   MAC_SET_CMD_DNCTR,
   MAC_SET_CMD_DR,
   MAC_SET_CMD_LINKCHK,
   MAC_SET_CMD_MCAST,
   MAC_SET_CMD_MCASTAPPSKEY,
   MAC_SET_CMD_MCASTDEVADDR,
   MAC_SET_CMD_MCASTDNCTR,
   MAC_SET_CMD_MCASTNWKSKEY,
   MAC_SET_CMD_NWKSKEY,
   MAC_SET_CMD_PWRIDX,
   MAC_SET_CMD_RETX,
   MAC_SET_CMD_RX2,
   MAC_SET_CMD_RXDELAY1,
   MAC_SET_CMD_SYNC,
   MAC_SET_CMD_UPCTR
} _mac_set_cmd_t;

typedef enum
{
   MAC_GET_CMD_ADR = 0x80,
   MAC_GET_CMD_APPEUI,
   MAC_GET_CMD_AR,
   MAC_GET_CMD_CH_FREQ,
   MAC_GET_CMD_CH_DR_RANGE,
   MAC_GET_CMD_CH_STATUS,
   MAC_GET_CMD_CLASS,
   MAC_GET_CMD_DCYCLEPS,
   MAC_GET_CMD_DEVADDR,
   MAC_GET_CMD_DEVEUI,
   MAC_GET_CMD_DNCTR,
   MAC_GET_CMD_DR,
   MAC_GET_CMD_GWNB,
   MAC_GET_CMD_MCAST,
   MAC_GET_CMD_MCASTDEVADDR,
   MAC_GET_CMD_MCASTDNCTR,
   MAC_GET_CMD_MRGN,
   MAC_GET_CMD_PWRIDX,
   MAC_GET_CMD_RETX,
   MAC_GET_CMD_RX2,
   MAC_GET_CMD_RXDELAY1,
   MAC_GET_CMD_RXDELAY2,
   MAC_GET_CMD_STATUS,
   MAC_GET_CMD_SYNC,
   MAC_GET_CMD_UPCTR
} _mac_get_cmd_t;

typedef enum
{
   RADIO_CMD_RX,
   RADIO_CMD_TX,
   RADIO_CMD_CW,
   RADIO_CMD_RXSTOP
} _radio_cmd_t;

typedef enum
{
   RADIO_SET_CMD_AFCBW,
   RADIO_SET_CMD_BITRATE,
   RADIO_SET_CMD_BT,
   RADIO_SET_CMD_BW,
   RADIO_SET_CMD_CR,
   RADIO_SET_CMD_CRC,
   RADIO_SET_CMD_FDEV,
   RADIO_SET_CMD_FREQ,
   RADIO_SET_CMD_IQI,
   RADIO_SET_CMD_MOD,
   RADIO_SET_CMD_PRLEN,
   RADIO_SET_CMD_PWR,
   RADIO_SET_CMD_RXBW,
   RADIO_SET_CMD_SF,
   RADIO_SET_CMD_SYNC,
   RADIO_SET_CMD_WDT
} _radio_set_cmd_t;

typedef enum
{
   RADIO_GET_CMD_AFCBW = 0x80,
   RADIO_GET_CMD_BITRATE,
   RADIO_GET_CMD_BT,
   RADIO_GET_CMD_BW,
   RADIO_GET_CMD_CR,
   RADIO_GET_CMD_CRC,
   RADIO_GET_CMD_FDEV,
   RADIO_GET_CMD_FREQ,
   RADIO_GET_CMD_IQI,
   RADIO_GET_CMD_MOD,
   RADIO_GET_CMD_PRLEN,
   RADIO_GET_CMD_PWR,
   RADIO_GET_CMD_RSSI,
   RADIO_GET_CMD_RXBW,
   RADIO_GET_CMD_SF,
   RADIO_GET_CMD_SNR,
   RADIO_GET_CMD_SYNC,
   RADIO_GET_CMD_WDT
} _radio_get_cmd_t;

/////////////////////////////// Global Variable ///////////////////////////////

struct
{
   char Data[RN2903_RX_MESSAGE_BUFFERS][RN2903_RX_MESSAGE_BUFFER_SIZE + 1];   //+1 for null terminator
   uint8_t Count;
   uint8_t NextIn;
   uint8_t NextOut;
} g_RN2903RxMessageBuffer;

////////////////////////////// Local Prototypes ///////////////////////////////

#if RN2903_RX_MESSAGE_BUFFER_SIZE >= 256
uint16_t RN2903GetMessage(unsigned char *Message);
#else
uint8_t RN2903GetMessage(unsigned char *Message);
#endif
void RN2903ClearSerialBuffer(void);
void RN2903TxSerial(unsigned char *Str, int1 DoStrEnd=TRUE);
uint8_t RN2903RxSerial(unsigned char *Str, uint16_t TimeOut=1000);

uint8_t RN2903SendCommand(unsigned char *Str, uint16_t TimeOut=1000);
rn2903_ec_t RN2903SendCommand_EC(unsigned char *Str, uint16_t TimeOut=1000);
rn2903_ec_t RN2903SendCommandM_EC(unsigned char *Str1, unsigned char *Str2, uint16_t TimeOut=1000);
rn2903_ec_t RN2903SendCommandMBytes_EC(unsigned char *Str, uint8_t *Data, uint8_t dCount, uint16_t TimeOut=1000);
int1 RN2903SendCommand_OnOffResponse(unsigned char *Str, uint16_t TimeOut=1000);

void ECToStr(rn2903_ec_t ErrorCode, unsigned char *ErrorStr);
rn2903_ec_t StrToEC(unsigned char *Str);
void RespToStr(rn2903_resp_t Response, unsigned char *RespStr);
rn2903_resp_t StrToResp(unsigned char *Str);
void HexStrToInt(unsigned char *Str, uint8_t *Data, uint8_t Bytes);
void ChangeEndian(uint8_t *Data, uint8_t Bytes);
uint32_t HexStrToInt32(unsigned char *Str);
uint16_t HexStrToInt16(unsigned char *Str);
uint8_t HexStrToInt8(unsigned char *Str);
void HexStrToInt8Array(unsigned char *Str, uint8_t *Data, uint8_t Bytes);
radio_bw_freq_t StrToBW(unsigned char *Str);
radio_coding_rate_t StrToCR(unsigned char * Str);
radio_modulation_t  StrToModulation(unsigned char *Str);
radio_spreading_factor_t StrToSF(unsigned char *Str);
void StrToEUI(unsigned char *Str, uint8_t *EUI);

void AddOnOff(int1 On, unsigned char *Str);

uint8_t RN2903GetPinName(rn2903_pin_t Name, unsigned char *Str);
uint8_t RN2903GetMACString(uint8_t Cmd, unsigned char *Str);
uint8_t RN2903GetRadioString(uint8_t Cmd, unsigned char *Str);
uint8_t RN2903GetCmdString(_rn2903_cmd_type_t Type, uint8_t Cmd, unsigned char *CmdStr);

///////////////////////////////////// API ///////////////////////////////////////

///////////////////////////////////
// RN2903 System Functions       //
///////////////////////////////////

rn2903_ec_t RN2903Sleep(uint32_t milliseconds)
{
   rn2903_ec_t Error;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS, SYS_CMD_SLEEP, Str);
   sprintf(&Str[Count], "%lu", milliseconds);
   
   Error = RN2903SendCommand_EC(Str, 100);
   
   if(Error == RN2903_EC_NO_RESPONSE)
      Error = RN2903_EC_OK;
   
   return(Error);
}

void RN2903Reset(unsigned char *Str)
{
   uint8_t Count;

  #ifdef RN2903_RESET_PIN
   output_low(RN2903_RESET_PIN);
   output_drive(RN2903_RESET_PIN);
   
   RN2903ClearSerialBuffer();
   
   delay_ms(10);
   
   output_float(RN2903_RESET_PIN);
   
   Count = RN2903RxSerial(Str);
  #else
   RN2903GetCmdString(RN2903_CMD_TYPE_SYS, SYS_CMD_RESET, Str);
   
   Count = RN2903SendCommand(Str);
  #endif
   
   if(Count == 0)
      Str[0] = '\0';
}

void RN2903EraseFW(void)
{
   unsigned char Str[12];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_SYS, SYS_CMD_ERASE_FW, Str);
   
   RN2903TxSerial(Str);
}

void RN2903FactoryReset(unsigned char *Str)
{
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_SYS, SYS_CMD_FACTORY_RESET, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 0)
      Str[0] = '\0';
}

rn2903_ec_t RN2903WriteNVM(uint16_t Address, uint8_t Data)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS_SET, SYS_SET_CMD_NVM, Str);
   Count += sprintf(&Str[Count], "%03LX %02X", Address, Data);
   
   //This is a quirk of the RN2903 it won't take 00 for 0
   if(Data == 0)
      Str[Count - 1] = '\0';
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RN2903OutputPin(rn2903_pin_t Pin, int1 Level)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS_SET, SYS_SET_CMD_PINDIG, Str);
   Count += RN2903GetPinName(Pin, &Str[Count]);
   sprintf(&Str[Count], " %u", Level);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RN2903SetPinMode(rn2903_pin_t Pin, rn2903_pin_mode_t Mode)
{
   unsigned char Str[32];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS_SET, SYS_SET_CMD_PINMODE, Str);
   Count += RN2903GetPinName(Pin, &Str[Count]);
   
   switch(Mode)
   {
      case RN2903_PIN_MODE_DIGITAL_OUTPUT:
         sprintf(&Str[Count], " digout");
         break;
      case RN2903_PIN_MODE_DIGITAL_INPUT:
         sprintf(&Str[Count], " digin");
         break;
      case RN2903_PIN_MODE_ANALOG:
         sprintf(&Str[Count], " ana");
         break;
   }
   
   return(RN2903SendCommand_EC(Str));
}

void RN2903Version(unsigned char *Str)
{
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_SYS_GET, SYS_GET_CMD_VER, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 0)
      Str[0] = '\0';
}

rn2903_ec_t RN2903ReadNVM(uint16_t Address, uint8_t *Result)
{
   rn2903_ec_t Error = RN2903_EC_OK;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS_GET, SYS_GET_CMD_NVM, Str);
   sprintf(&Str[Count], "%03LX", Address);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 0)
      Error = RN2903_EC_UNKNOWN;
   else if(Count <= 2)
   {
      if(Count == 1)
      {
         Str[1] = Str[0];
         Str[0] = '0';
         Str[2] = '\0';
      }
   
      *Result = HexStrToInt8(Str);
   }
   else
      Error = StrToEC(Str);
   
   return(Error);
}

uint16_t RN2903GetVdd(void)
{
   uint16_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_SYS_GET, SYS_GET_CMD_VDD, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 4))
      Result = atol(Str);
   else
      Result = 0;
   
   return(Result);
}

rn2903_ec_t RN2903GetHWEUI(uint8_t *EUI)
{
   rn2903_ec_t Result = RN2903_EC_OK;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_SYS_GET, SYS_GET_CMD_HWEUI, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == EUI_RESPONSE_SIZE)
      StrToEUI(Str, EUI);
   else if(Count > 0)
      Result = StrToEC(Str);
   else
      Result = RN2903_EC_NO_RESPONSE;
   
   return(Result);
}

int1 RN2903InputPin(rn2903_pin_t Pin)
{
   int1 Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS_GET, SYS_GET_CMD_PINDIG, Str);
   RN2903GetPinName(Pin, &Str[Count]);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 1)
      Result = atoi(Str);
   else
      Result = 0;
   
   return(Result);
}

uint16_t RN2903ReadAnalog(rn2903_pin_t Pin)
{
   uint16_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_SYS_GET, SYS_GET_CMD_PINANA, Str);
   RN2903GetPinName(Pin, &Str[Count]);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 4))
      Result = atol(Str);
   else
      Result = 0xFFFF;
   
   return(Result);
}

///////////////////////////////////
// RN290 MAC Functions           //
///////////////////////////////////

rn2903_ec_t MACReset(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_RESET, Str);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACTxData(mac_tx_type_t Type, uint8_t Port, uint8_t *Data, uint8_t dCount)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_TX, Str);
   
   if(Type == MAC_TX_TYPE_CONFIRMED)
      Count += sprintf(&Str[Count], "cnf");
   else
      Count += sprintf(&Str[Count], "uncnf");
   
   sprintf(&Str[Count], " %u ", Port);
   
   return(RN2903SendCommandMBytes_EC(Str, Data, dCount));
}

rn2903_ec_t MACJoin(mac_join_mode_t Mode)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_JOIN, Str);
   
   if(Mode == MAC_JOIN_MODE_OTAA)
      sprintf(&Str[Count], "otaa");
   else
      sprintf(&Str[Count], "abp");
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSave(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_SAVE, Str);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACForceEnable(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_FORCE_ENABLE, Str);
   
   return(RN2903SendCommand_EC(Str));
}

uint32_t MACPause(void)
{
   uint32_t Result;
   uint8_t Count;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_PAUSE, Str);
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi32(Str);
   else
      Result = 0;
   
   return(Result);
}

rn2903_ec_t MACResume(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC, MAC_CMD_RESUME, Str);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetKey(mac_key_type_t Which, uint8_t *Key)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   switch(Which)
   {
      case MAC_KEY_TYPE_APP:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_APPKEY, Str);
         break;
      case MAC_KEY_TYPE_APPS:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_APPSKEY, Str);
         break;
      case MAC_KEY_TYPE_MCAST_APPS:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_MCASTAPPSKEY, Str);
         break;
      case MAC_KEY_TYPE_MCAST_NWKS:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_MCASTNWKSKEY, Str);
         break;
      case MAC_KEY_TYPE_NWKS:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_NWKSKEY, Str);
         break;
   }
   
   return(RN2903SendCommandMBytes_EC(Str, Key, 16));
}

rn2903_ec_t MACEnableDisableAutoReply(int1 Enable)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_AR, Str);
   
   if(Enable)
      sprintf(&Str[Count], "on");
   else
      sprintf(&Str[Count], "off");
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetBatteryLevel(uint8_t Level)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_BAT, Str);
   
   sprintf(&Str[Count], "%u", Level);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetChannelDataRate(uint8_t Ch, uint8_t Min, uint8_t Max)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_CH_DR_RANGE, Str);
   sprintf(&Str[Count], "%u %u %u\r\n", Ch, Min, Max);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACEnableDisableChannel(uint8_t Ch, int1 Enable)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_CH_STATUS, Str);
   
   if(Enable)
      sprintf(&Str[Count], "%u on", Ch);
   else
      sprintf(&Str[Count], "%u off", Ch);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetClass(mac_device_class_t Class)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_CLASS, Str);
   
   if(Class == MAC_DEVICE_CLASS_A)
      sprintf(&Str[Count], "a");
   else
      sprintf(&Str[Count], "c");
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetDevAddr(mac_network_addr_type_t which, uint32_t Address)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   switch(which)
   {
      case MAC_NETWORK_ADDR_TYPE_DEV_ADDR:
         Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_DEVADDR, Str);
         break;
      case MAC_NETWORK_ADDR_TYPE_MCAST_DEV_ADDR:
         Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_MCASTDEVADDR, Str);
         break;
   }
   
   sprintf(&Str[Count], "%08LX", Address);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetEUI(mac_eui_type_t which, uint8_t *EUI)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   switch(which)
   {
      case MAC_EUI_TYPE_APP:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_APPEUI, Str);
         break;
      case MAC_EUI_TYPE_DEV:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_DEVEUI, Str);
         break;
   }
   
   return(RN2903SendCommandMBytes_EC(Str, EUI, 8));
}

rn2903_ec_t MACSetFrameCounter(mac_frame_counter_type_t which, uint32_t Value)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   switch(which)
   {
      case MAC_FRAME_COUNTER_TYPE_DL:
         Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_DNCTR, Str);
         break;
      case MAC_FRAME_COUNTER_TYPE_MCAST_DL:
         Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_MCASTDNCTR, Str);
         break;
      case MAC_FRAME_COUNTER_TYPE_UL:
         Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_UPCTR, Str);
         break;
   }
   
   sprintf(&Str[Count], "%lu", Value);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetDataRate(uint8_t Rate)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_DR, Str);
   sprintf(&Str[Count],"%u", Rate);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetLinkCheck(uint16_t Value)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_LINKCHK, Str);
   sprintf(&Str[Count], "%lu", Value);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACEnableDisableMCast(int1 Enable)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_MCAST, Str);
   AddOnOff(Enable, &Str[Count]);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetTxPowerIdx(uint8_t Index)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_PWRIDX, Str);
   sprintf(&Str[Count], "%u", Index);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetRetransmissions(uint8_t Value)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_RETX, Str);
   sprintf(&Str[Count], "%u", Value);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetRx2Setting(rx2_struct_t Setting)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_RX2, Str);
   sprintf(&Str[Count], "%u %lu", Setting.DataRate, Setting.Frequency);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetRxDelay1(uint16_t Delay)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_RXDELAY1, Str);
   sprintf(&Str[Count], "%lu", Delay);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t MACSetSyncByte(uint8_t Sync)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_SET, MAC_SET_CMD_SYNC, Str);
   sprintf(&Str[Count], "%02X", Sync);
   
   return(RN2903SendCommand_EC(Str));
}

int1 MACGetADRState(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_ADR, Str);
   
   return(RN2903SendCommand_OnOffResponse(Str));
}

rn2903_ec_t MACGetEUI(mac_eui_type_t which, uint8_t *EUI)
{
   rn2903_ec_t Result = RN2903_EC_OK;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   switch(which)
   {
      case MAC_EUI_TYPE_APP:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_APPEUI, Str);
         break;
      case MAC_EUI_TYPE_DEV:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_DEVEUI, Str);
         break;
   }
   
   Count = RN2903SendCommand(Str);
   
   if(Count == EUI_RESPONSE_SIZE)
      StrToEUI(Str, EUI);
   else if(Count > 0)
      Result = StrToEC(Str);
   else
      Result = RN2903_EC_NO_RESPONSE;
   
   return(Result);
}

int1 MACGetAutoReplyState(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_AR, Str);
   
   return(RN2903SendCommand_OnOffResponse(Str));
}

rn2903_ec_t MACGetChannelInfo(uint8_t Ch, mac_ch_info_t *Info)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   uint8_t i;
   unsigned char OnStr[] = "on";
   rn2903_ec_t Result = RN2903_EC_OK;
   
   for(i=0;i<3;i++)
   {
      switch(i)
      {
         case 0:
            Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_CH_FREQ, Str);
            break;
         case 1:
            Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_CH_DR_RANGE, Str);
            break;
         case 2:
            Count = RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_CH_STATUS, Str);
            break;
      }
      
      if(Count != 0)
      {
         sprintf(&Str[Count], "%u", Ch);
      
         Count = RN2903SendCommand(Str);
         
         if(Count != 0)
         {
            switch(i)
            {
               case 0:
                  Info->Frequency = atoi32(Str);
                  break;
               case 1:
                  Str[1] = '\0';
                  Info->DataRateMin = atoi(Str);
                  Info->DataRateMax = atoi(&Str[2]);
                  break;
               case 2:
                  if(strcmp(Str, OnStr) == 0)
                     Info->Status = 1;
                  else
                     Info->Status = 0;
                  break;
            }
         }
         else
         {
            Result = RN2903_EC_NO_RESPONSE;
            break;
         }
      }
      else
      {
         Result = RN2903_EC_UNKNOWN;
         break;
      }
   }
   
   return(Result);
}

unsigned char MACGetClass(void)
{
   unsigned char Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_CLASS, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 1)
      Result = Str[0];
   else
      Result = 'E';
   
   return(Result);
}

uint16_t MACGetDutyCyclePrescalar(void)
{
   uint16_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_DCYCLEPS, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 5))
      Result = atol(Str);
   else
      Result = 0;
   
   return(Result);
}

uint32_t MACGetDevAddr(mac_network_addr_type_t Type)
{
   uint32_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   switch(Type)
   {
      case MAC_NETWORK_ADDR_TYPE_DEV_ADDR:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_DEVADDR, Str);
         break;
      case MAC_NETWORK_ADDR_TYPE_MCAST_DEV_ADDR:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_MCASTDEVADDR, Str);
         break;
   }
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 8)
      Result = HexStrToInt32(Str);
   else
      Result = 0;
   
   return(Result);
}

uint32_t MACGetFrameCounter(mac_frame_counter_type_t which)
{
   uint32_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   switch(which)
   {
      case MAC_FRAME_COUNTER_TYPE_DL:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_DNCTR, Str);
         break;
      case MAC_FRAME_COUNTER_TYPE_MCAST_DL:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_MCASTDNCTR, Str);
         break;
      case MAC_FRAME_COUNTER_TYPE_UL:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_UPCTR, Str);
         break;
   }
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi32(Str);
   else
      Result = 0;
   
   return(Result);
}

uint8_t MACGetDataRate(void)
{
   uint8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_DR, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi(Str);
   else
      Result = 0xFF;
   
   return(Result);
}

uint8_t MACGetGatawayCount(void)
{
   uint8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_GWNB, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi(Str);
   else
      Result = 0;
   
   return(Result);
}

int1 MACGetMCastStatus(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_MCAST, Str);
   
   return(RN2903SendCommand_OnOffResponse(Str));
}

uint8_t MACGetDemodMargin(void)
{
   uint8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_MRGN, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi(Str);
   else
      Result = 0;
   
   return(Result);
}

uint8_t MACGetTxPowerIdx(void)
{
   uint8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_PWRIDX, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi(Str);
   else
      Result = 0;
   
   return(Result);
}

uint8_t MACGetRetransmissions(void)
{
   uint8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_RETX, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atoi(Str);
   else
      Result = 0;
   
   return(Result);
}

rx2_struct_t MACGetRx2Setting(void)
{
   rx2_struct_t Result = {0,0};
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   uint8_t i;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_RX2, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
   {
      for(i=0;i<Count;i++)
      {
         if(Str[i] == ' ')
         {
            Str[i] = '\0';
            break;
         }
      }
      
      if((i > 0) && (i < Count))
      {
         Result.DataRate = atoi(Str);
         i++;
         Result.Frequency = atoi32(&Str[i]);
      }  
   }
   
   return(Result);
}

uint16_t MACGetRxDelay(mac_rx_delay_type_t which)
{
   uint16_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   switch(which)
   {
      case MAC_RX_DELAY_TYPE_DELAY_1:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_RXDELAY1, Str);
         break;
      case MAC_RX_DELAY_TYPE_DELAY_2:
         RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_RXDELAY2, Str);
         break;
   }
   
   Count = RN2903SendCommand(Str);
   
   if(Count != 0)
      Result = atol(Str);
   else
      Result = 0;
   
   return(Result);
}

mac_status_t MACGetStatus(void)
{
   mac_status_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_STATUS, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 8)
      Result.dw = HexStrToInt32(Str);
   else
   {
      Result.dw = 0;
      Result.r24_31 = 0xFF;
   }
   
   return(Result);
}

uint8_t MACGetSyncByte(void)
{
   uint8_t Result;
   
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_MAC_GET, MAC_GET_CMD_SYNC, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 2)
      Result = HexStrToInt8(Str);
   else
      Result = 0;
   
   return(Result);
}

///////////////////////////////////
// RN2903 Radio Functions        //
///////////////////////////////////

rn2903_ec_t RadioStartRx(uint16_t Timeout)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO, RADIO_CMD_RX, Str);
   sprintf(&Str[Count], "%lu", Timeout);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioTxData(uint8_t *Data, uint8_t Count)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO, RADIO_CMD_TX, Str);
   
   return(RN2903SendCommandMBytes_EC(Str, Data, Count));
}

rn2903_ec_t RadioSetCWMode(int1 Enable)
{
   rn2903_ec_t Error;
   unsigned char Str[RN2903_RX_MESSAGE_BUFFER_SIZE];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO, RADIO_CMD_CW, Str);
   AddOnOff(Enable, &Str[Count]);
   
   Error = RN2903SendCommand_EC(Str);
   
   if((Error == RN2903_EC_UNKNOWN) && (Enable == FALSE))
      Error = RN2903_EC_OK;
   
   return(Error);
}

rn2903_ec_t RadioStopRx(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO, RADIO_CMD_RXSTOP, Str);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetAFCBW(radio_bw_freq_t Frequency)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   uint8_t Decimal;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_AFCBW, Str);
   Count += sprintf(&Str[Count], "%lu", Frequency / 10);
   
   Decimal = (Frequency % 10);
   
   if(Decimal != 0)
      sprintf(&Str[Count], ".%u", Decimal);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetBitRate(uint32_t BitRate)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_BITRATE, Str);
   sprintf(&Str[Count], "%lu", BitRate);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetFSKDataShaping(radio_gfsk_modulation_t Shape)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_BT, Str);
   
   switch(Shape)
   {
      case RADIO_GFSK_MODULATION_NONE:
         sprintf(&Str[Count], "none");
         break;
      case RADIO_GFSK_MODULATION_1P0:
         sprintf(&Str[Count], "1.0");
         break;
      case RADIO_GFSK_MODULATION_0P5:
         sprintf(&Str[Count], "0.5");
         break;
      case RADIO_GFSK_MODULATION_0P3:
         sprintf(&Str[Count], "0.3");
         break;
   }
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetBandWidth(radio_bandwidth_t BandWidth)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_BW, Str);
   sprintf(&Str[Count], "%lu", BandWidth);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetCodingRate(radio_coding_rate_t Rate)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_CR, Str);
   
   switch(Rate)
   {
      case RADIO_CODING_RATE_4BY5:
         sprintf(&Str[Count], "4/5");
         break;
      case RADIO_CODING_RATE_4BY6:
         sprintf(&Str[Count], "4/6");
         break;
      case RADIO_CODING_RATE_4BY7:
         sprintf(&Str[Count], "4/7");
         break;
      case RADIO_CODING_RATE_4BY8:
         sprintf(&Str[Count], "4/8");
         break;
   }
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioEnableDisableCRC(int1 Enable)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_CRC, Str);
   AddOnOff(Enable, &Str[Count]);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetFrequencyDeviation(uint32_t Deviation)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_FDEV, Str);
   sprintf(&Str[Count], "%lu", Deviation);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetFrequency(uint32_t Frequency)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_FREQ, Str);
   sprintf(&Str[Count], "%lu", Frequency);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioEnableDisableIQI(int1 Enable)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_IQI, Str);
   AddOnOff(Enable, &Str[Count]);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetModulation(radio_modulation_t Modulation)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_MOD, Str);
   
   switch(Modulation)
   {
      case RADIO_MODULATION_LORA:
         sprintf(&Str[Count], "lora");
         break;
      case RADIO_MODULATION_FSK:
         sprintf(&Str[Count], "fsk");
         break;
   }
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetPreambleLength(uint16_t Length)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_PRLEN, Str);
   sprintf(&Str[Count], "%lu", Length);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetTxPower(uint8_t Power)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_PWR, Str);
   sprintf(&Str[Count], "%u", Power);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetRxBW(radio_bw_freq_t Frequency)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   uint8_t Decimal;
   
   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_RXBW, Str);
   Count += sprintf(&Str[Count], "%lu", Frequency / 10);
   
   Decimal = Frequency % 10;
   
   if(Decimal != 0)
      sprintf(&Str[Count], ".%u", Decimal);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetSpreadingFactor(radio_spreading_factor_t Factor)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;

   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_SF, Str);
   sprintf(&Str[Count], "sf%u", Factor);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetSyncByte(uint8_t Sync)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;

   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_SYNC, Str);
   sprintf(&Str[Count], "%02X", Sync);
   
   return(RN2903SendCommand_EC(Str));
}

rn2903_ec_t RadioSetWDT(uint32_t Time)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;

   Count = RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_SET, RADIO_SET_CMD_WDT, Str);
   sprintf(&Str[Count], "%lu", Time);
   
   return(RN2903SendCommand_EC(Str));
}

radio_bw_freq_t RadioGetAFCBW(void)
{
   radio_bw_freq_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_AFCBW, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 5))
      Result = StrToBW(Str);
   else
      Result = 0;
   
   return(Result);
}

uint32_t RadioGetBitRate(void)
{
   uint32_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_BITRATE, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 6))
      Result = atoi32(Str);
   else
      Result = 0;
   
   return(Result);
}

radio_gfsk_modulation_t RadioGetFSKDataShaping(void)
{
   radio_gfsk_modulation_t Result = 0;
   radio_gfsk_modulation_t i;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   unsigned char cStr[5];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_BT, Str);
   
   Count = RN2903SendCommand(Str);
   
   switch(Count)
   {
      case 3:
         for(i=2;i<5;i++)
         {
            switch(i)
            {
               case 2:
                  sprintf(cStr, "1.0");
                  break;
               case 3:
                  sprintf(cStr, "0.5");
                  break;
               case 4:
                  sprintf(cStr, "0.3");
                  break;
            }
            
            if(strcmp(Str, cStr) == 0)
            {
               Result = i;
               break;
            }
         }
         break;
      case 4:
         sprintf(cStr, "none");
         
         if(strcmp(Str, cStr) == 0)
            Result = RADIO_GFSK_MODULATION_NONE;
         break;
   }
   
   return(Result);
}

radio_bandwidth_t RadioGetBandWidth(void)
{
   radio_bandwidth_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_BW, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 3)
      Result = (radio_bandwidth_t)atol(Str);
   else
      Result = 0;
   
   return(Result);
}

radio_coding_rate_t RadioGetCodingRate(void)
{
   radio_coding_rate_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_CR, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 3)
      Result = StrToCR(Str);
   else
      Result = 0;
   
   return(Result);
}

int1 RadioGetCRCStatus(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_CRC, Str);
   
   return(RN2903SendCommand_OnOffResponse(Str));
}

uint32_t RadioGetFrequencyDeviation(void)
{
   uint32_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_FDEV, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 6))
      Result = atoi32(Str);
   else
      Result = 0;
   
   return(Result);
}

uint32_t RadioGetFrequency(void)
{
   uint32_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_FREQ, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count == 9)
      Result = atoi32(Str);
   else
      Result = 0;
   
   return(Result);
}

int1 RadioGetIQIStatus(void)
{
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_IQI, Str);
   
   return(RN2903SendCommand_OnOffResponse(Str));
}

radio_modulation_t RadioGetModulation(void)
{
   radio_modulation_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_MOD, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count == 3) || (Count == 4))
      Result = StrToModulation(Str);
   else
      Result = 0;
   
   return(Result);
}

uint16_t RadioGetPreambleLength(void)
{
   uint16_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_PRLEN, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 5))
      Result = atol(Str);
   else
      Result = 0;
   
   return(Result);
}

uint8_t RadioGetTxPower(void)
{
   uint8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_PWR, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 2))
      Result = atoi(Str);
   else
      Result = 0;
   
   return(Result);
}

int8_t RadioGetRSSI(void)
{
   int8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_RSSI, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count > 0)
      Result = atoi(Str);
   else
      Result = -128;
   
   return(Result);
}

radio_bw_freq_t RadioGetRxBW(void)
{
   radio_bw_freq_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_BW, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count > 0) && (Count <= 5))
      Result = StrToBW(Str);
   else
      Result = 0;
   
   return(Result);
}

radio_spreading_factor_t RadioGetSpreadingFactor(void)
{
   radio_spreading_factor_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_SF, Str);
   
   Count = RN2903SendCommand(Str);
   
   if((Count == 3) || (Count == 4))
      Result = StrToSF(Str);
   else
      Result = 0;
   
   return(Result);
}

int8_t RadioGetSNR(void)
{
   int8_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_SNR, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count > 0)
      Result = atoi(Str);
   else
      Result = -128;
   
   return(Result);
}

uint32_t RadioGetWDT(void)
{
   uint32_t Result;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   uint8_t Count;
   
   RN2903GetCmdString(RN2903_CMD_TYPE_RADIO_GET, RADIO_GET_CMD_WDT, Str);
   
   Count = RN2903SendCommand(Str);
   
   if(Count > 0)
      Result = atoi32(Str);
   else
      Result = 0;
   
   return(Result);
}

///////////////////////////////////
// RN2903 Serial Functions       //
///////////////////////////////////

void RN2903SerialInit(unsigned char *Str=NULL)
{
   unsigned char vStr[64];
   uint8_t Length;
   
  #ifdef RN2903_RESET_PIN
   output_float(RN2903_RESET_PIN);
  #endif
   
  #if RN2903_SERIAL_BUFFER_SIZE > 0
   enable_interrupts(GLOBAL);
  #endif
   
   RN2903ClearSerialBuffer();
  
   Length = RN2903RxSerial(vStr, 1000);
   
   if(Str != NULL)
   {
      if(Length > 0)
         memcpy(Str, vStr, Length);
      
      Str[Length] = '\0';
   }
}

void RN2903SerialRxTask(void)
{
   unsigned char c;
  #if RN2903_RX_MESSAGE_BUFFER_SIZE >= 255
   static uint16_t Index = 0;
  #else
   static uint8_t Index = 0;
  #endif
   
   if(kbhit(RN2903_SERIAL_STREAM))
   {
      c = fgetc(RN2903_SERIAL_STREAM);
      
      if(c == '\r')
      {
         g_RN2903RxMessageBuffer.Data[g_RN2903RxMessageBuffer.NextIn][Index] = '\0';
      }
      else if(c == '\n')
      {
         if(Index > 0) 
         {
            if(g_RN2903RxMessageBuffer.Count < RN2903_RX_MESSAGE_BUFFERS)
            {
               if(++g_RN2903RxMessageBuffer.NextIn >= RN2903_RX_MESSAGE_BUFFERS)
                  g_RN2903RxMessageBuffer.NextIn = 0;
               
               g_RN2903RxMessageBuffer.Count++;
            }
            
            Index = 0;
         }
      }
      else
      {
         if(Index < RN2903_RX_MESSAGE_BUFFER_SIZE)
            g_RN2903RxMessageBuffer.Data[g_RN2903RxMessageBuffer.NextIn][Index++] = c;
      }
   }
}

int1 RN2903SerialKbhit(void)
{
   if(g_RN2903RxMessageBuffer.Count > 0)
      return(TRUE);
   else
      return(FALSE);
}

rn2903_resp_t RN2903SerialGetResponse(uint8_t *Data, uint8_t *Length)
{
   unsigned char Message[RN2903_RX_MESSAGE_BUFFER_SIZE];
   rn2903_resp_t Response;
   uint8_t rLength;
   uint8_t Index;
   
   if(RN2903SerialKbhit())
   {
      rLength = RN2903GetMessage(Message);
      
      Response = StrToResp(Message);
      
      if(Response == RN2903_RESP_RADIO_RX)
      {
         rLength -= 9;
         Index = 9;
         
         while(!isxdigit(Message[Index]) && (rLength > 0))
         {
            Index++;
            rLength--;
         }
         
         rLength /= 2;
         
         if(rLength)
            HexStrToInt8Array(&Message[Index], Data, rLength);
         
         *Length = rLength;
      }
      else if(Response == RN2903_RESP_MAC_RX)
      {
         rLength -= 7;
         Index = 7;
         
         while(!isdigit(Message[Index]) && (rLength > 0))
         {
            Index++;
            rLength--;
         }
         
         if(rLength)
         {
            Data[0] = atoi(&Message[Index]);
            
            while(isdigit(Message[Index]) && (rLength > 0))
            {
               Index++;
               rLength--;
            }
            
            while(!isxdigit(Message[Index]) && (rLength > 0))
            {
               Index++;
               rLength--;
            }
            
            rLength /= 2;
            
            if(rLength)
            {
               HexStrToInt8Array(&Message[Index], &Data[1], rLength);
               
               rLength++;
            }
            else
               rLength = 1;
         }
         
         *Length = rLength;
      }
      else if(Response == RN2903_RESP_VALUE)
      {
         strcpy(Data, Message);
         
         *Length = rLength;
      }
      else
         *Length = 0;
   }
   else
   {
      Response = RN2903_RESP_NONE;
      *Length = 0;
   }
   
   return(Response);
}

///////////////////////////////////////////////////////////////////////////////

#if RN2903_RX_MESSAGE_BUFFER_SIZE >= 256
uint16_t RN2903GetMessage(unsigned char *Message)
#else
uint8_t RN2903GetMessage(unsigned char *Message)
#endif
{
   if(!RN2903SerialKbhit())
      return(0);
   
   strcpy(Message, g_RN2903RxMessageBuffer.Data[g_RN2903RxMessageBuffer.NextOut]);
   
   if(++g_RN2903RxMessageBuffer.NextOut >= RN2903_RX_MESSAGE_BUFFERS)
      g_RN2903RxMessageBuffer.NextOut = 0;
   
   g_RN2903RxMessageBuffer.Count--;
   
   return(strlen(Message));
} 

void RN2903ClearSerialBuffer(void)
{
   unsigned char c;
   
   g_RN2903RxMessageBuffer.Count = 0;
   g_RN2903RxMessageBuffer.NextIn = 0;
   g_RN2903RxMessageBuffer.NextOut = 0;

   while(kbhit(RN2903_SERIAL_STREAM))
      c = fgetc(RN2903_SERIAL_STREAM);
}

void RN2903TxSerial(unsigned char *Str, int1 DoStrEnd=TRUE)
{
   fprintf(RN2903_SERIAL_STREAM, "%s", Str);
   
   if(DoStrEnd)
      fprintf(RN2903_SERIAL_STREAM, "\r\n");
}

uint8_t RN2903RxSerial(unsigned char *Str, uint16_t TimeOut=1000)
{
   unsigned char c = '\0';
   uint8_t Count = 0;
   uint8_t usDelayCount = 20;
   
   do
   {
      if(kbhit(RN2903_SERIAL_STREAM))
      {
         c = fgetc(RN2903_SERIAL_STREAM);
         
         if((c == '\r') || (c == '\n'))
            Str[Count] = '\0';
         else
            Str[Count++] = c;
      }
      
      delay_us(50);
      
      if(--usDelayCount == 0)
      {
         TimeOut--;
         usDelayCount = 20;
      }
   } while((c != '\n') && (TimeOut != 0));
   
   if(c == '\n')
      return(Count);
   else
      return(0);
}

///////////////////////////////////////////////////////////////////////////////

uint8_t RN2903SendCommand(unsigned char *Str, uint16_t TimeOut=1000)
{
   uint8_t Count;
   
   RN2903ClearSerialBuffer();
   RN2903TxSerial(Str);
   Count = RN2903RxSerial(Str, TimeOut);
   
   return(Count);
}

rn2903_ec_t RN2903SendCommand_EC(unsigned char *Str, uint16_t TimeOut=1000)
{
   rn2903_ec_t Error;
   uint8_t Count;

   Count = RN2903SendCommand(Str, TimeOut);
   
   if(Count != 0)
      Error = StrToEC(Str);
   else
      Error = RN2903_EC_NO_RESPONSE;
   
   return(Error);
}

rn2903_ec_t RN2903SendCommandM_EC(unsigned char *Str1, unsigned char *Str2, uint16_t TimeOut=1000)
{
   rn2903_ec_t Error;
   uint8_t Count;
   
   RN2903ClearSerialBuffer();
   RN2903TxSerial(Str1, FALSE);
   RN2903TxSerial(Str2);
   Count = RN2903RxSerial(Str1, TimeOut);
   
   if(Count != 0)
      Error = StrToEC(Str1);
   else
      Error = RN2903_EC_NO_RESPONSE;
   
   return(Error);
}

rn2903_ec_t RN2903SendCommandMBytes_EC(unsigned char *Str, uint8_t *Data, uint8_t dCount, uint16_t TimeOut=1000)
{
   rn2903_ec_t Error;
   uint8_t Count;
   uint8_t i;
   
   RN2903ClearSerialBuffer();
   RN2903TxSerial(Str, FALSE);
   
   for(i=0;i<dCount;i++)
      fprintf(RN2903_SERIAL_STREAM, "%02X", Data[i]);
   
   fprintf(RN2903_SERIAL_STREAM, "\r\n");
   
   Count = RN2903RxSerial(Str, TimeOut);
   
   if(Count != 0)
      Error = StrToEC(Str);
   else
      Error = RN2903_EC_NO_RESPONSE;
   
   return(Error);
}

int1 RN2903SendCommand_OnOffResponse(unsigned char *Str, uint16_t TimeOut=1000)
{
   int1 Result = FALSE;
   uint8_t Count;
   unsigned char OnStr[] = "on";
   
   Count = RN2903SendCommand(Str, TimeOut);
   
   if(Count > 0)
   {
      if(strcmp(Str, OnStr) == 0)
         Result = TRUE;
   }
   
   return(Result);
}

void ECToStr(rn2903_ec_t ErrorCode, unsigned char *ErrorStr)
{
   switch(ErrorCode)
   {
      case RN2903_EC_OK:
         sprintf(ErrorStr, "ok");
         break;
      case RN2903_EC_BUSY:
         sprintf(ErrorStr, "busy");
         break;
      case RN2903_EC_FRAM_COUNTER_ERR_REJOIN_NEEDED:
         sprintf(ErrorStr, "fram_counter_err_rejoin_needed");
         break;
      case RN2903_EC_INVALID_CLASS:
         sprintf(ErrorStr, "invalid_class");
         break;
      case RN2903_EC_INVALID_DATA_LEN:
         sprintf(ErrorStr, "invalid_data_len");
         break;
      case RN2903_EC_INVALID_PARAM:
         sprintf(ErrorStr, "invalid_param");
         break;
      case RN2903_EC_KEYS_NOT_INIT:
         sprintf(ErrorStr, "keys_not_init");
         break;
      case RN2903_EC_MAC_PAUSED:
         sprintf(ErrorStr, "mac_paused");
         break;
      case RN2903_EC_MULICAST_KEYS_NO_SET:
         sprintf(ErrorStr, "multicast_keys_not_set");
         break;
      case RN2903_EC_NO_FREE_CH:
         sprintf(ErrorStr, "no_free_ch");
         break;
      case RN2903_EC_NOT_JOINED:
         sprintf(ErrorStr, "not_joined");
         break;
      case RN2903_EC_SILENT:
         sprintf(ErrorStr, "silent");
         break;
      case RN2903_EC_ERR:
         sprintf(ErrorStr, "err");
         break;
      default:
         ErrorStr[0] = '\0';
         break;
   }
}

rn2903_ec_t StrToEC(unsigned char *Str)
{
   rn2903_ec_t Error = RN2903_EC_UNKNOWN;
   unsigned char cStr[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   rn2903_ec_t i;
   
   if(strlen(Str) <= RN2903_MAX_EC_RESPONSE_BYTES)
   {
      for(i=0;i<13;i++)
      {
         ECToStr(i, cStr);
         
         if(strcmp(cStr, Str) == 0)
         {
            Error = i;
            break;
         }
      }
   }
   
   return(Error);
}

void RespToStr(rn2903_resp_t Response, unsigned char *RespStr)
{
   switch(Response)
   {
      case RN2903_RESP_MAC_TX_OK:
         sprintf(RespStr, "mac_tx_ok");
         break;
      case RN2903_RESP_MAC_RX:
         sprintf(RespStr, "mac_rx");
         break;
      case RN2903_RESP_MAC_ERR:
         sprintf(RespStr, "mac_err");
         break;
      case RN2903_RESP_INVALID_DATA_LEN:
         sprintf(RespStr, "invalid_data_len");
         break;
      case RN2903_RESP_DENIED:
         sprintf(RespStr, "denied");
         break;
      case RN2903_RESP_ACCEPTED:
         sprintf(RespStr, "accepted");
         break;
      case RN2903_RESP_RADIO_RX:
         sprintf(RespStr, "radio_rx");
         break;
      case RN2903_RESP_RADIO_ERR:
         sprintf(RespStr, "radio_err");
         break;
      case RN2903_RESP_RADIO_TX_OK:
         sprintf(RespStr, "radio_tx_ok");
         break;
      default:
         RespStr[0] = '\0';
         break;
   }
}

rn2903_resp_t StrToResp(unsigned char *Str)
{
   rn2903_resp_t Response = RN2903_RESP_VALUE;
   rn2903_resp_t i;
   unsigned char cStr[RN2903_MAX_RESP_BYTES + 1];
   
   for(i=RN2903_RESP_MAC_TX_OK;i<RN2903_RESP_VALUE;i++)
   {
      RespToStr(i, cStr);
      
      if(strstr(Str, cStr) != 0)
      {
         Response = i;
         break;
      }
   }
   
   return(Response);
}

void HexStrToInt(unsigned char *Str, uint8_t *Data, uint8_t Bytes)
{
   uint8_t i;
   unsigned char c;
   uint8_t Value;
   uint8_t Count;
   union
   {
      uint8_t b[4];
      uint32_t dw;
   } Temp;
   
   Temp.dw = 0;
   Count = Bytes * 2;
   
   for(i=0;i<Count;i++)
   {
      shift_left(Temp.b, 4, 0);
      shift_left(Temp.b, 4, 0);
      shift_left(Temp.b, 4, 0);
      shift_left(Temp.b, 4, 0);
      
      c = Str[i];
      
      if(isdigit(c))
         Value = c - '0';
      else if(isalpha(c))
         Value = (uint8_t )toupper(c) - 55;
      else
         Value = 0;
         
      Temp.b[0] += Value;
   }
   
   memcpy(Data, Temp.b, Bytes);
}

void ChangeEndian(uint8_t *Data, uint8_t Bytes)
{
   uint8_t Temp;
   uint8_t i,j;
   
   Temp = Data[0];
   
   for(i=0,j=(Bytes-1);j>0;i++,j--)
      Data[i] = Data[j];
   
   Data[i] = Temp;
}

uint32_t HexStrToInt32(unsigned char *Str)
{
   uint32_t Result;
   
   HexStrToInt(Str, (uint8_t *)&Result, 4);
   
   return(Result);
}

uint16_t HexStrToInt16(unsigned char *Str)
{
   uint16_t Result;
   
   HexStrToInt(Str, (uint8_t *)&Result, 2);
   ChangeEndian((uint8_t *)&Result, 2);
   
   return(Result);
}

uint8_t HexStrToInt8(unsigned char *Str)
{
   uint8_t Result;
   
   HexStrToInt(Str, &Result, 1);
   
   return(Result);
}

void HexStrToInt8Array(unsigned char *Str, uint8_t *Data, uint8_t Bytes)
{
   uint8_t i;
   
   for(i=0;i<Bytes;i++)
      Data[i] = HexStrToInt8(&Str[i*2]);
}

radio_bw_freq_t StrToBW(unsigned char *Str)
{
   radio_bw_freq_t Result = 0;
   uint8_t i;
   int1 hasDecimal;
   unsigned char c;
   unsigned char tStr[5];
   
   i = 0;
   hasDecimal = FALSE;
   
   do
   {
      c = *Str++;
      
      if(isdigit(c))
      {
         tStr[i++] = c;
         tStr[i] = '\0';
      }
      else if(c == '.')
         hasDecimal = TRUE;
         
   } while(*Str != '\0');
   
   if(i > 0)
   {
      if(hasDecimal == FALSE)
      {
         tStr[i++] = '0';
         tStr[i] = '\0';
      }
      
      Result = (radio_bw_freq_t)atol(tStr);
   }
   else
      Result = 0;
   
   return(Result);
}

radio_coding_rate_t StrToCR(unsigned char *Str)
{
   radio_coding_rate_t Result = 0;
   
   if((Str[0] == '4') && (Str[1] == '/'))
   {
      switch(Str[2])
      {
         case '5':
            Result = RADIO_CODING_RATE_4BY5;
            break;
         case '6':
            Result = RADIO_CODING_RATE_4BY6;
            break;
         case '7':
            Result = RADIO_CODING_RATE_4BY7;
            break;
         case '8':
            Result = RADIO_CODING_RATE_4BY8;
            break;
      }
   }
   
   return(Result);
}

unsigned char* CRToStr(radio_coding_rate_t cr)
{
   static unsigned char Str[8];
   
   switch(cr)
   {
      case RADIO_CODING_RATE_4BY5:
         sprintf(Str, "4/5");
         break;
      case RADIO_CODING_RATE_4BY6:
         sprintf(Str, "4/6");
         break;
      case RADIO_CODING_RATE_4BY7:
         sprintf(Str, "4/7");
         break;
      case RADIO_CODING_RATE_4BY8:
         sprintf(Str, "4/8");
         break;
      default:
         sprintf(Str, "Unknown");
         break;
   }
   
   return(Str);
}

radio_modulation_t StrToModulation(unsigned char *Str)
{
   radio_modulation_t Result;
   unsigned char loraStr[] = "lora";
   unsigned char fskStr[] = "fsk";
   
   if(strcmp(Str, loraStr) == 0)
      Result = RADIO_MODULATION_LORA;
   else if(strcmp(Str, fskStr) == 0)
      Result = RADIO_MODULATION_FSK;
   else
      Result = 0;
   
   return(Result);
}

radio_spreading_factor_t StrToSF(unsigned char *Str)
{
   radio_spreading_factor_t Result = 0;
   char tStr[3];
   
   if((Str[0] == 's') && (Str[1] == 'f'))
   {
      memcpy(tStr, &Str[2], 2);
      tStr[2] = '\0';
      
      Result = (radio_spreading_factor_t)atoi(tStr);
   }
   
   return(Result);
}

void StrToEUI(unsigned char *Str, uint8_t *EUI)
{
   uint8_t i;
      
   for(i=0;i<EUI_SIZE_BYTES;i++)
      EUI[i] = HexStrToInt8(&Str[i*2]);
}

void AddOnOff(int1 On, unsigned char *Str)
{
   if(On)
      sprintf(Str, "on");
   else
      sprintf(Str, "off");
}

uint8_t RN2903GetPinName(rn2903_pin_t Name, unsigned char *Str)
{
   uint8_t Count;
   
   if(Name <= RN2903_PIN_GPIO13)
      Count = sprintf(Str, "GPIO%u", Name);
   else
   {
      switch(Name)
      {
         case RN2903_PIN_UART_CTS:
            Count = sprintf(Str, "UART_CTS");
            break;
         case RN2903_PIN_UART_RTS:
            Count = sprintf(Str, "UART_RTS");
            break;
         case RN2903_PIN_TEST0:
            Count = sprintf(Str, "TEST0");
            break;
         case RN2903_PIN_TEST1:
            Count = sprintf(Str, "TEST1");
            break;
         default:
            Count = 0;
            break;
      }
   }
   
   return(Count);
}

uint8_t RN2903GetMACString(uint8_t Cmd, unsigned char *Str)
{
   uint8_t Count;

   switch(Cmd)
   {
      case MAC_SET_CMD_APPKEY:
         Count = sprintf(Str, "appkey");
         break;
      case MAC_SET_CMD_APPSKEY:
         Count = sprintf(Str, "appskey");
         break;
      case MAC_SET_CMD_APPEUI:
      case MAC_GET_CMD_APPEUI:
         Count = sprintf(Str, "appeui");
         break;
      case MAC_SET_CMD_AR:
      case MAC_GET_CMD_AR:
         Count = sprintf(Str, "ar");
         break;
      case MAC_SET_CMD_BAT:
         Count = sprintf(Str, "bat");
         break;
      case MAC_SET_CMD_CH_DR_RANGE:
      case MAC_GET_CMD_CH_DR_RANGE:
         Count = sprintf(Str, "ch drrange");
         break;
      case MAC_SET_CMD_CH_STATUS:
      case MAC_GET_CMD_CH_STATUS:
         Count = sprintf(Str, "ch status");
         break;
      case MAC_SET_CMD_CLASS:
      case MAC_GET_CMD_CLASS:
         Count = sprintf(Str, "class");
         break;
      case MAC_SET_CMD_DEVADDR:
      case MAC_GET_CMD_DEVADDR:
         Count = sprintf(Str, "devaddr");
         break;
      case MAC_SET_CMD_DEVEUI:
      case MAC_GET_CMD_DEVEUI:
         Count = sprintf(Str, "deveui");
         break;
      case MAC_SET_CMD_DNCTR:
      case MAC_GET_CMD_DNCTR:
         Count = sprintf(Str, "dnctr");
         break;
      case MAC_SET_CMD_DR:
      case MAC_GET_CMD_DR:
         Count = sprintf(Str, "dr");
         break;
      case MAC_SET_CMD_LINKCHK:
         Count = sprintf(Str, "linkchk");
         break;
      case MAC_SET_CMD_MCAST:
      case MAC_GET_CMD_MCAST:
         Count = sprintf(Str, "mcast");
         break;
      case MAC_SET_CMD_MCASTAPPSKEY:
         Count = sprintf(Str, "mcastappskey");
         break;
      case MAC_SET_CMD_MCASTDEVADDR:
      case MAC_GET_CMD_MCASTDEVADDR:
         Count = sprintf(Str, "mcastdevaddr");
         break;
      case MAC_SET_CMD_MCASTDNCTR:
      case MAC_GET_CMD_MCASTDNCTR:
         Count = sprintf(Str, "mcastdnctr");
         break;
      case MAC_SET_CMD_MCASTNWKSKEY:
         Count = sprintf(Str, "mcastnwkskey");
         break;
      case MAC_SET_CMD_NWKSKEY:
         Count = sprintf(Str, "nwkskey");
         break;
      case MAC_SET_CMD_PWRIDX:
      case MAC_GET_CMD_PWRIDX:
         Count = sprintf(Str, "pwridx");
         break;
      case MAC_SET_CMD_RETX:
      case MAC_GET_CMD_RETX:
         Count = sprintf(Str, "retx");
         break;
      case MAC_SET_CMD_RX2:
      case MAC_GET_CMD_RX2:
         Count = sprintf(Str, "rx2");
         break;
      case MAC_SET_CMD_RXDELAY1:
      case MAC_GET_CMD_RXDELAY1:
         Count = sprintf(Str, "rxdelay1");
         break;
      case MAC_SET_CMD_SYNC:
      case MAC_GET_CMD_SYNC:
         Count = sprintf(Str, "sync");
         break;
      case MAC_SET_CMD_UPCTR:
      case MAC_GET_CMD_UPCTR:
         Count = sprintf(Str, "upctr");
         break;
      case MAC_GET_CMD_ADR:
         Count = sprintf(Str, "adr");
         break;
      case MAC_GET_CMD_CH_FREQ:
         Count = sprintf(Str, "ch freq");
         break;
      case MAC_GET_CMD_DCYCLEPS:
         Count = sprintf(Str, "dcycleps");
         break;
      case MAC_GET_CMD_GWNB:
         Count = sprintf(Str, "gwnb");
         break;
      case MAC_GET_CMD_MRGN:
         Count = sprintf(Str, "mrgn");
         break;
      case MAC_GET_CMD_RXDELAY2:
         Count = sprintf(Str, "rxdelay2");
         break;
      case MAC_GET_CMD_STATUS:
         Count = sprintf(Str, "status");
         break;
      default:
         Count = 0;
         break;
   }
   
   return(Count);
}

uint8_t RN2903GetRadioString(uint8_t Cmd, unsigned char *Str)
{
   uint8_t Count;
   
   switch(Cmd)
   {
      case RADIO_SET_CMD_AFCBW:
      case RADIO_GET_CMD_AFCBW:
          Count = sprintf(Str, "afcbw");
         break;
      case RADIO_SET_CMD_BITRATE:
      case RADIO_GET_CMD_BITRATE:
          Count = sprintf(Str, "bitrate");
         break;
      case RADIO_SET_CMD_BT:
      case RADIO_GET_CMD_BT:
          Count = sprintf(Str, "bt");
         break;
      case RADIO_SET_CMD_BW:
      case RADIO_GET_CMD_BW:
          Count = sprintf(Str, "bw");
         break;
      case RADIO_SET_CMD_CR:
      case RADIO_GET_CMD_CR:
         Count = sprintf(Str, "cr");
         break;
      case RADIO_SET_CMD_CRC:
      case RADIO_GET_CMD_CRC:
         Count = sprintf(Str, "crc");
         break;
      case RADIO_SET_CMD_FDEV:
      case RADIO_GET_CMD_FDEV:
         Count = sprintf(Str, "fdev");
         break;
      case RADIO_SET_CMD_FREQ:
      case RADIO_GET_CMD_FREQ:
         Count = sprintf(Str, "freq");
         break;
      case RADIO_SET_CMD_IQI:
      case RADIO_GET_CMD_IQI:
         Count = sprintf(Str, "iqi");
         break;
      case RADIO_SET_CMD_MOD:
      case RADIO_GET_CMD_MOD:
         Count = sprintf(Str, "mod");
         break;
      case RADIO_SET_CMD_PRLEN:
      case RADIO_GET_CMD_PRLEN:
         Count = sprintf(Str, "prlen");
         break;
      case RADIO_SET_CMD_PWR:
      case RADIO_GET_CMD_PWR:
         Count = sprintf(Str, "pwr");
         break;
      case RADIO_SET_CMD_RXBW:
      case RADIO_GET_CMD_RXBW:
         Count = sprintf(Str, "rxbw");
         break;
      case RADIO_SET_CMD_SF:
      case RADIO_GET_CMD_SF:
         Count = sprintf(Str, "sf");
         break;
      case RADIO_SET_CMD_SYNC:
      case RADIO_GET_CMD_SYNC:
         Count = sprintf(Str, "sync");
         break;
      case RADIO_SET_CMD_WDT:
      case RADIO_GET_CMD_WDT:
         Count = sprintf(Str, "wdt");
         break;
      case RADIO_GET_CMD_RSSI:
         Count = sprintf(Str, "rssi");
         break;
      case RADIO_GET_CMD_SNR:
         Count = sprintf(Str, "snr");
         break;
      default:
         Count = 0;
         break;
   }
   
   return(Count);
}

uint8_t RN2903GetCmdString(_rn2903_cmd_type_t Type, uint8_t Cmd, unsigned char *CmdStr)
{
   uint8_t Count;
   uint8_t sCount;
   
   switch(Type)
   {
      case RN2903_CMD_TYPE_SYS:
         Count = sprintf(CmdStr, "sys ");
      
         switch(Cmd)
         {
            case SYS_CMD_SLEEP:
               Count += sprintf(&CmdStr[Count], "sleep ");
               break;
            case SYS_CMD_RESET:
               Count += sprintf(&CmdStr[Count], "reset");
               break;
            case SYS_CMD_ERASE_FW:
               Count += sprintf(&CmdStr[Count], "eraseFW");
               break;
            case SYS_CMD_FACTORY_RESET:
               Count += sprintf(&CmdStr[Count], "factoryRESET");
               break;
            default:
               Count = 0;
               break;
         }  
         break;
      case RN2903_CMD_TYPE_SYS_SET:
         Count = sprintf(CmdStr, "sys set ");
         
         switch(Cmd)
         {
            case SYS_SET_CMD_NVM:
               Count += sprintf(&CmdStr[Count], "nvm ");
               break;
            case SYS_SET_CMD_PINDIG:
               Count += sprintf(&CmdStr[Count], "pindig ");
               break;
            case SYS_SET_CMD_PINMODE:
               Count += sprintf(&CmdStr[Count], "pinmode ");
               break;
            default:
               Count = 0;
               break;
         }
         break;
      case RN2903_CMD_TYPE_SYS_GET:
         Count = sprintf(CmdStr, "sys get ");
         
         switch(Cmd)
         {
            case SYS_GET_CMD_VER:
               Count += sprintf(&CmdStr[Count], "ver");
               break;
            case SYS_GET_CMD_NVM:
               Count += sprintf(&CmdStr[Count], "nvm ");
               break;
            case SYS_GET_CMD_VDD:
               Count += sprintf(&CmdStr[Count], "vdd");
               break;
            case SYS_GET_CMD_HWEUI:
               Count += sprintf(&CmdStr[Count], "hweui");
               break;
            case SYS_GET_CMD_PINDIG:
               Count += sprintf(&CmdStr[Count], "pindig ");
               break;
            case SYS_GET_CMD_PINANA:
               Count += sprintf(&CmdStr[Count], "pinana ");
               break;
            default:
               Count = 0;
         }
         break;
      case RN2903_CMD_TYPE_MAC:
         Count = sprintf(CmdStr, "mac ");
         
         switch(Cmd)
         {
            case MAC_CMD_RESET:
               Count += sprintf(&CmdStr[Count], "reset");
               break;
            case MAC_CMD_TX:
               Count += sprintf(&CmdStr[Count], "tx ");
               break;
            case MAC_CMD_JOIN:
               Count += sprintf(&CmdStr[Count], "join ");
               break;
            case MAC_CMD_SAVE:
               Count += sprintf(&CmdStr[Count], "save");
               break;
            case MAC_CMD_FORCE_ENABLE:
               Count += sprintf(&CmdStr[Count], "forceENABLE");
               break;
            case MAC_CMD_PAUSE:
               Count += sprintf(&CmdStr[Count], "pause");
               break;
            case MAC_CMD_RESUME:
               Count += sprintf(&CmdStr[Count], "resume");
               break;
            default:
               Count = 0;
               break;
         }
         break;
      case RN2903_CMD_TYPE_MAC_SET:
         Count = sprintf(CmdStr, "mac set ");
         sCount = RN2903GetMACString(Cmd, &CmdStr[Count]);
         
         if(sCount != 0)
         {
            Count += sCount;
            
            CmdStr[Count++] = ' ';
            CmdStr[Count] = '\0';
         }
         else
            Count = 0;
         break;
      case RN2903_CMD_TYPE_MAC_GET:
         Count = sprintf(CmdStr, "mac get ");
         sCount = RN2903GetMACString(Cmd, &CmdStr[Count]);
         
         if(sCount != 0)
         {
            Count += sCount;
         
            if((Cmd >= MAC_GET_CMD_CH_FREQ) && (Cmd <= MAC_GET_CMD_CH_STATUS))
            {
               CmdStr[Count++] = ' ';
               CmdStr[Count] = '\0';
            }
         }
         else
            Count = 0;
         break;
      case RN2903_CMD_TYPE_RADIO:
         Count = sprintf(CmdStr, "radio ");
         
         switch(Cmd)
         {
            case RADIO_CMD_RX:
               Count += sprintf(&CmdStr[Count], "rx ");
               break;
            case RADIO_CMD_TX:
               Count += sprintf(&CmdStr[Count], "tx ");
               break;
            case RADIO_CMD_CW:
               Count += sprintf(&CmdStr[Count], "cw ");
               break;
            case RADIO_CMD_RXSTOP:
               Count += sprintf(&CmdStr[Count], "rxstop");
               break;
            default:
               Count = 0;
               break;
         }
         break;
      case RN2903_CMD_TYPE_RADIO_SET:
         Count = sprintf(CmdStr, "radio set ");
         sCount = RN2903GetRadioString(Cmd, &CmdStr[Count]);
         
         if(sCount != 0)
         {
            Count += sCount;
            
            CmdStr[Count++] = ' ';
            CmdStr[Count] = '\0';
         }
         else
            Count = 0;
         
         break;
      case RN2903_CMD_TYPE_RADIO_GET:
         Count = sprintf(CmdStr, "radio get ");
         sCount = RN2903GetRadioString(Cmd, &CmdStr[Count]);
         
         if(sCount != 0)
            Count += sCount;
         else
            Count = 0;
         break;
      default:
         Count = 0;
         break;
   }
   
   return(Count);
}

///////////////////////////////////////////////////////////////////////////////
////                           RN2903 Bootloader                           ////
///////////////////////////////////////////////////////////////////////////////

#define RN2903_BL_START_BYTE           0x55
#define RN2903_BL_KEY1_VAL             0x55
#define RN2903_BL_KEY2_VAL             0xAA

#define RN2903_BL_CMD_VERSION          0x00
#define RN2903_BL_CMD_WRITE            0x02
#define RN2903_BL_CMD_ERASE            0x03
#define RN2903_BL_CMD_CHECKSUM         0x08
#define RN2903_BL_CMD_RESET            0x09

#define RN2903_BL_MAX_RESP_SIZE        16

#define RN2903_APP_START_ADDR          0x300
#define RN2903_APP_LAST_ADDR           0xFFFF

#define RN2903_BL_ERASE_SIZE           64
#define RN2903_BL_WRITE_SIZE           64

#define RN2903_BL_MAX_ERASE_PAGES      256
#define RN2903_BL_MAX_WRITE_SIZE       255
#define RN2903_BL_MAX_CHECKSUM_SIZE    256

#define RN2903_BL_START_PAGE           (RN2903_APP_START_ADDR / RN2903_BL_ERASE_SIZE)
#define RN2903_BL_LAST_PAGE            (RN2903_APP_LAST_ADDR / RN2903_BL_ERASE_SIZE)

#define RN2903_BL_CMD_SUCCESSFUL       1
#define RN2902_BL_CMD_FAILED           0

typedef struct _PACKED_STRUCT
{
   uint8_t Cmd;
   uint16_t Length;
   uint8_t Key1;
   uint8_t Key2;
   uint32_t Address;
} _rn2903_bl_tx_header_t;

typedef struct _PACKED_STRUCT
{
   uint8_t Cmd;
   uint8_t Data[RN2903_BL_MAX_RESP_SIZE];
} _rn2903_bl_resp_t;

typedef enum
{
   RN2903_BL_RX_MODE_START,
   RN2903_BL_RX_MODE_CMD,
   RN2903_BL_RX_MODE_LENGTH,
   RN2903_BL_RX_MODE_KEY1,
   RN2903_BL_RX_MODE_KEY2,
   RN2903_BL_RX_MODE_ADDR,
   RN2903_BL_RX_MODE_DATA
} _rn2903_bl_rx_mode_t;

struct
{
   _rn2903_bl_resp_t Buffer;
   int1 IsNew;
} _g_RN2903BLResponse;

///////////////////////////////////////////////////////////////////////////////

void _RN2903BLRxTask(void);
void _RN2903BLPutData(_rn2903_bl_tx_header_t *Header, uint8_t *Data);
int1 _RN2903BLGetData(_rn2903_bl_resp_t *Response);
void _RN2903BLGetResponse(uint8_t Cmd, _rn2903_bl_resp_t *Response);

#define _RN2903BLHasData()    (_g_RN2903BLResponse.IsNew)

///////////////////////////////////////////////////////////////////////////////

void RN2903BLGetVersion(rn2903_bl_version_t *Version)
{
   _rn2903_bl_tx_header_t Header;
   _rn2903_bl_resp_t Response;
   
   memset(&Header, 0x00, sizeof(_rn2903_bl_tx_header_t));
   
   Header.Cmd = RN2903_BL_CMD_VERSION;
   
   _RN2903BLPutData(&Header, NULL);
   _RN2903BLGetResponse(RN2903_BL_CMD_VERSION, &Response);
   
   memcpy(Version, &Response.Data, sizeof(rn2903_bl_version_t));
}

int1 RN2903BLWriteFW(uint32_t Address, uint8_t *Data, uint16_t Count)
{
   _rn2903_bl_tx_header_t Header;
   _rn2903_bl_resp_t Response;
   int1 Failed = FALSE;
   
   if((Count <= RN2903_BL_MAX_WRITE_SIZE) && (Address >= RN2903_APP_START_ADDR) && ((Address + Count) <= (RN2903_APP_LAST_ADDR + 1)))
   {
      Header.Cmd = RN2903_BL_CMD_WRITE;
      Header.Length = Count;
      Header.Key1 = RN2903_BL_KEY1_VAL;
      Header.Key2 = RN2903_BL_KEY2_VAL;
      Header.Address = Address;
      
      _RN2903BLPutData(&Header, Data);
      _RN2903BLGetResponse(RN2903_BL_CMD_WRITE, &Response);
      
      if(Response.Data[0] != RN2903_BL_CMD_SUCCESSFUL)
         Failed = TRUE;
   }
   else
      Failed = TRUE;
   
   return(Failed == FALSE);
}

int1 RN2903BLEraseFW(void)
{
   uint16_t ePage;
   uint32_t PageAddr;
   _rn2903_bl_tx_header_t Header;
   _rn2903_bl_resp_t Response;
   int1 Failed = FALSE;
   
   PageAddr = RN2903_APP_START_ADDR;
   ePage = RN2903_BL_START_PAGE;
   
   Header.Cmd = RN2903_BL_CMD_ERASE;
   Header.Length = 1;   //only LSB is used, a value of 0 for the LSB is equal to RN2903_BL_MAX_ERASE_PAGES
   Header.Key1 = RN2903_BL_KEY1_VAL;
   Header.Key2 = RN2903_BL_KEY2_VAL;
   
   //The RN2903 Bootloader appears to not erase correctly if 0 is passed for the
   //length.
   for(;ePage<(RN2903_BL_LAST_PAGE + 1);ePage++)
   {
      Header.Address = PageAddr;
      
      _RN2903BLPutData(&Header, NULL);
      _RN2903BLGetResponse(RN2903_BL_CMD_ERASE, &Response);
      
      if(Response.Data[0] != RN2903_BL_CMD_SUCCESSFUL)
      {
         Failed = TRUE;
         break;
      }
      
      PageAddr += RN2903_BL_ERASE_SIZE;
   }
   
   return(Failed == FALSE);
}

uint16_t RN2903BLGetCheckSum(void)
{
   uint16_t Checksum;
   _rn2903_bl_tx_header_t Header;
   _rn2903_bl_resp_t Response;
   
   Header.Cmd = RN2903_BL_CMD_CHECKSUM;
   Header.Length = 0;
   Header.Key1 = 0;
   Header.Key2 = 0;
   Header.Address = RN2903_APP_START_ADDR;
   
   _RN2903BLPutData(&Header, NULL);
   _RN2903BLGetResponse(RN2903_BL_CMD_CHECKSUM, &Response);
      
   Checksum = make16(Response.Data[1], Response.Data[0]);
      
   return(Checksum);
}

int1 RN2903BLReset(void)
{
   _rn2903_bl_tx_header_t Header;
   uint8_t Count;
   unsigned char Str[RN2903_MAX_EC_RESPONSE_BYTES + 1];
   
   memset(&Header, 0x00, sizeof(_rn2903_bl_tx_header_t));
   
   Header.Cmd = RN2903_BL_CMD_RESET;
   
   _RN2903BLPutData(&Header, NULL);
   
   Count = RN2903RxSerial(Str, 2000);
   
   if(Count == 0)
      return(FALSE);
   else
      return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////

void _RN2903BLRxTask(void)
{
   static _rn2903_bl_rx_mode_t Mode = RN2903_BL_RX_MODE_START;
   static uint8_t Length;
   static _rn2903_bl_resp_t Response;
   static uint8_t Index;
   uint8_t rData;
   
   if(kbhit(RN2903_SERIAL_STREAM))
   {
      rData = fgetc(RN2903_SERIAL_STREAM);
      
      switch(Mode)
      {
         case RN2903_BL_RX_MODE_START:
            if(rData == RN2903_BL_START_BYTE)
               Mode++;
            break;
         case RN2903_BL_RX_MODE_CMD:
            if((rData == RN2903_BL_CMD_VERSION) || (rData == RN2903_BL_CMD_WRITE) ||
               (rData == RN2903_BL_CMD_ERASE) || (rData == RN2903_BL_CMD_CHECKSUM))
            {
               Response.Cmd = rData;
               Length = 2;
               Mode++;
            }
            else
               Mode = RN2903_BL_RX_MODE_START;
            break;
         case RN2903_BL_RX_MODE_LENGTH:
            if(--Length == 0)
               Mode++;
            break;
         case RN2903_BL_RX_MODE_KEY1:
            Mode++;
            break;
         case RN2903_BL_RX_MODE_KEY2:
            Length = 4;
            Mode++;
            break;
         case RN2903_BL_RX_MODE_ADDR:
            if(--Length == 0)
            {
               switch(Response.Cmd)
               {
                  case RN2903_BL_CMD_VERSION:
                     Length = 16;
                     break;
                  case RN2903_BL_CMD_WRITE:
                  case RN2903_BL_CMD_ERASE:
                     Length = 1;
                     break;
                  case RN2903_BL_CMD_CHECKSUM:
                     Length = 2;
                     break;
               }
               
               Index = 0;
               Mode++;
            }
            break;
         case RN2903_BL_RX_MODE_DATA:
            Response.Data[Index++] = rData;
            
            if(--Length == 0)
            {
               memcpy(&_g_RN2903BLResponse.Buffer, &Response, sizeof(_rn2903_bl_resp_t));
               _g_RN2903BLResponse.IsNew = TRUE;
               
               Mode = RN2903_BL_RX_MODE_START;
            }
            
            break;
         default:
            Mode = RN2903_BL_RX_MODE_START;
            break;
      }
   }  
}

void _RN2903BLPutData(_rn2903_bl_tx_header_t *Header, uint8_t *Data)
{
   uint8_t *Ptr;
   uint16_t i;
   uint16_t Length;
   
   Ptr = (uint8_t *)Header;
   
   fputc(RN2903_BL_START_BYTE, RN2903_SERIAL_STREAM);
   
   for(i=0;i<sizeof(_rn2903_bl_tx_header_t);i++)
      fputc(Ptr[i], RN2903_SERIAL_STREAM);
   
   Length = Header->Length;
   
   for(i=0;i<Length;i++)
      fputc(Data[i], RN2903_SERIAL_STREAM);
}

int1 _RN2903BLGetData(_rn2903_bl_resp_t *Response)
{
   if(_RN2903BLHasData())
   {
      memcpy(Response, &_g_RN2903BLResponse.Buffer, sizeof(_rn2903_bl_resp_t));
      
      _g_RN2903BLResponse.IsNew = FALSE;
   }
   
   return(FALSE);
}

void _RN2903BLGetResponse(uint8_t Cmd, _rn2903_bl_resp_t *Response)
{
   _g_RN2903BLResponse.IsNew = FALSE;
      
   while(!_RN2903BLHasData())
   {
      _RN2903BLRxTask();
      
      if(_RN2903BLHasData() && (_g_RN2903BLResponse.Buffer.Cmd != Cmd))
         _RN2903BLGetData(Response);
   }
   
   _RN2903BLGetData(Response);
}

///////////////////////////////////////////////////////////////////////////////
////                         API for lora.c driver                         ////
///////////////////////////////////////////////////////////////////////////////

#ifdef __LORA_H__

#define _lora_set_sync(v)     (RadioSetSyncByte(v) == RN2903_EC_OK)
#define _lora_set_pwr(v)      (RadioSetTxPower(v) == RN2903_EC_OK)
#define _lora_set_sf(v)       (RadioSetSpreadingFactor(v) == RN2903_EC_OK)
#define _lora_set_cr(v)       (RadioSetCodingRate(v) == RN2903_EC_OK)
#define _lora_set_bw(v)       (RadioSetBandWidth(v) == RN2903_EC_OK)
#define _lora_enable_crc(v)   (RadioEnableDisableCRC(v) == RN2903_EC_OK)
#define _lora_enable_iqi(v)   (RadioEnableDisableIQI(v) == RN2903_EC_OK)

int1 _lora_init_radio(void)
{
   char Str[RN2903_RX_MESSAGE_BUFFER_SIZE + 1] = "";
   uint32_t Time;
   rn2903_ec_t Error = RN2903_EC_UNKNOWN;
   
   debug_lora_printf(debug_lora_putc, "_lora_init_radio()\r\n");
   
   RN2903SerialInit();
   
   RN2903Reset(Str);
   
   debug_lora_printf(debug_lora_putc, "   RN2903Reset() - %s\r\n", Str);
   
   if(Str[0] != '\0')
   {
      Time = MACPause();
      
      debug_lora_printf(debug_lora_putc, "   MACPause() - %lu\r\n", Time);
      
      if(Time != 0)
      {
         Error = RadioSetWDT(0); //disable WDT
         
         debug_lora_printf(debug_lora_putc, "   RadioSetWDT() - Error: %u\r\n", Error);
      }
   }
   
   return(Error == RN2903_EC_OK);
}

int1 _lora_set_radio_config(void)
{
   rn2903_ec_t Error = RN2903_EC_OK;
   uint8_t sIndex = 0;
   
   debug_lora_printf(debug_lora_putc, "set_lora_radio_conif()\r\n");
   
   while((Error == RN2903_EC_OK) && (sIndex < 7))
   {
      switch(sIndex)
      {
         case 0:
            Error = RadioSetSyncByte(_lora_config.sync);
            debug_lora_printf(debug_lora_putc, "   RadioSetSyncByte() - Error: %u\r\n", Error);
            break;
         case 1:
            Error = RadioSetTxPower(_lora_config.pwr);
            debug_lora_printf(debug_lora_putc, "   RadioSetTxPower() - Error: %u\r\n", Error);
            break;
         case 2:
            Error = RadioSetSpreadingFactor(_lora_config.sf);
            debug_lora_printf(debug_lora_putc, "   RadioSetSpreadingFactor() - Error: %u\r\n", Error);
            break;
         case 3:
            Error = RadioSetCodingRate(_lora_config.cr);
            debug_lora_printf(debug_lora_putc, "   RadioSetCodingRate() - Error: %u\r\n", Error);
            break;
         case 4:
            Error = RadioSetBandWidth(_lora_config.bw);
            debug_lora_printf(debug_lora_putc, "   RadioSetBandWidth() - Error: %u\r\n", Error);
            break;
         case 5:
            Error = RadioEnableDisableCRC(_lora_config.crc);
            debug_lora_printf(debug_lora_putc, "   RadioEnableDisableCRC() - Error: %u\r\n", Error);
            break;
         case 6:
            Error = RadioEnableDisableIQI(_lora_config.iqi);
            debug_lora_printf(debug_lora_putc, "   RadioEnableDisableIQI() - Error: %u\r\n", Error); 
            break;
      }
      
      sIndex++;
   }
   
   return(Error == RN2903_EC_OK);
}

int1 _lora_stop_rx(void)
{
   rn2903_ec_t Error;
   
   Error = RadioStopRx();
   
   return(Error == RN2903_EC_OK);
}

int1 _lora_set_channel(uint8_t Channel)
{
   rn2903_ec_t Error;
   
   Error = RadioSetFrequency(_g_LoraStartFreq + ((uint32_t)Channel * _g_LoraIncFreq));
   
   return(Error == RN2903_EC_OK);
}

int1 _lora_start_listening(void)
{
   rn2903_ec_t Error;
   
   Error = RadioStartRx(0);
   
   return(Error == RN2903_EC_OK);
}

#define _lora_rx_task      RN2903SerialRxTask
#define _lora_kbhit        RN2903SerialKbhit
#define _lora_get_rx_rssi  RadioGetRSSI
#define _lora_get_rx_snr   RadioGetSNR

lora_module_resp_t _lora_get_radio_message(uint8_t *Data, uint8_t *Length)
{
   lora_module_resp_t Result;
   rn2903_resp_t Response;
   
   if(RN2903SerialKbhit())
   {
      Response = RN2903SerialGetResponse(Data, Length);
      
      if(Response == RN2903_RESP_RADIO_RX)
         Result = LORA_MODULE_RESP_RX_DATA;
      else if(Response == RN2903_RESP_RADIO_TX_OK)
         Result = LORA_MODULE_RESP_TX_OK;
      else if(Response == RN2903_RESP_RADIO_ERR)
         Result = LORA_MODULE_RESP_ERR;
      else
         Result = LORA_MODULE_RESP_UNKNOWN;
   }
   else
      Result = LORA_MODULE_RESP_NONE;
   
   return(Result);
}

int1 _lora_putd(uint8_t *Data, uint8_t Count)
{
   rn2903_ec_t Error;
  
  #ifdef DEBUG_LORA
   uint8_t i;
   
   debug_lora_printf(debug_lora_putc, "   _lora_putd() - Count: %u, Data:", Count);
   
   for(i=0;i<Count;i++)
      debug_lora_printf(debug_lora_putc, " %02X", Data[i]);
   
   debug_lora_printf(debug_lora_putc, "\r\n");
  #endif
  
   Error = RadioTxData(Data, Count);
   
   return(Error == RN2903_EC_OK);
}

#endif

///////////////////////////////////////////////////////////////////////////////
////                        API for lorawan.c driver                       ////
///////////////////////////////////////////////////////////////////////////////

#ifdef __LORAWAN_H__

#define _lorawan_set_sync_byte(v)                  (MACSetSyncByte(v) == RN2903_EC_OK)
#define _lorawan_set_device_eui(v)                 (MACSetEUI(MAC_EUI_TYPE_DEV, v) == RN2903_EC_OK)
#define _lorawan_set_app_eui(v)                    (MACSetEUI(MAC_EUI_TYPE_APP, v) == RN2903_EC_OK)
#define _lorawan_set_app_key(v)                    (MACSetKey(MAC_KEY_TYPE_APP, v) == RN2903_EC_OK)
#define _lorawan_set_network_session_key(v)        (MACSetKey(MAC_KEY_TYPE_NWKS, v) == RN2903_EC_OK)
#define _lorawan_set_app_session_key(v)            (MACSetKey(MAC_KEY_TYPE_APPS, v) == RN2903_EC_OK)
#define _lorawan_set_device_address(v)             (MACSetDevAddr(MAC_NETWORK_ADDR_TYPE_DEV_ADDR, v) == RN2903_EC_OK)
#define _lorawan_set_retransmission_attempts(v)    (MACSetRetransmissions(v) == RN2903_EC_OK)
#define _lorawan_set_rx_delay(v)                   (MACSetRxDelay1(v) == RN2903_EC_OK)
#define _lorawan_set_auto_reply(v)                 (MACEnableDisableAutoReply(v) == RN2903_EC_OK)
#define _lorawan_set_ch_dr_range(ch, min, max)     (MACSetChannelDataRate(ch, min, max) == RN2903_EC_OK)
#define _lorawan_enable_ch(ch)                     (MACEnableDisableChannel(Ch, TRUE) == RN2903_EC_OK)
#define _lorawan_disable_ch(ch)                    (MACEnableDisableChannel(Ch, FALSE) == RN2903_EC_OK)
#define _lorawan_set_power_index(v)                (MACSetTxPowerIdx(v) == RN2903_EC_OK)
#define _lorawan_get_ch_status(ch, status)         (MACGetChannelInfo(ch, status) == RN2903_EC_OK)
#define _lorawan_set_downlink_counter(v)           (MACSetFrameCounter(MAC_FRAME_COUNTER_TYPE_DL, v) == RN2903_EC_OK)
#define _lorawan_set_uplink_counter(v)             (MACSetFrameCounter(MAC_FRAME_COUNTER_TYPE_UL, v) == RN2903_EC_OK)
#define _lorawan_get_downlink_counter()            (MACGetFrameCounter(MAC_FRAME_COUNTER_TYPE_DL))
#define _lorawan_get_uplink_counter()              (MACGetFrameCounter(MAC_FRAME_COUNTER_TYPE_UL))
#define _lorawan_set_tx_dr()                       (MACSetDataRate(_lorawan_config.TxDataRate) == RN2903_EC_OK)

_lorawan_module_error_t _lorawan_get_module_error_from_error(rn2903_ec_t Error);

#define _lorawan_init_radio()       RN2903SerialInit()
#define _lorawan_rx_task()          RN2903SerialRxTask()
#define _lorawan_kbhit()            RN2903SerialKbhit()
#define _lorawan_putd(a,b,c,d)      _lorawan_get_module_error_from_error(MACTxData(d, a, b, c))
#define _lorawan_save_settings()    (MACSave() == RN2903_EC_OK)

int1 _lorawan_reset(void)
{
   char rStr[40];
   
   RN2903Reset(rStr);
   
   if(rStr[0] != '\0')
      return(TRUE);
   else
      return(FALSE);
}

int1 _lorawan_get_config(void)
{
   int1 Result;
   mac_status_t Status;
   rx2_struct_t Rx2Settings;
   mac_ch_info_t ChInfo;
   
   debug_lorawan_printf(debug_lorawan_putc, "   _lorawan_get_config()\r\n");
   
   Status = MACGetStatus();
   
   debug_lorawan_printf(debug_lorawan_putc, "      MACGetStatus() - %08LX\r\n", Status.dw);
   
   if(Status.r24_31 == 0)
   {
      MACGetEUI(MAC_EUI_TYPE_DEV, _lorawan_config.DeviceEUI);
   
      if((Status.Joined == TRUE) && (Status.RejoinNeeded == FALSE))
      {
         _lorawan_set_join_state(LORAWAN_JOIN_STATE_JOINED);
         
         _lorawan_config.AutoReply = Status.AutoReply;
         _lorawan_config.Sync = MACGetSyncByte();
         _lorawan_config.RetransmissionAttempts = MACGetRetransmissions();
         _lorawan_config.RXDelay = MACGetRxDelay(MAC_RX_DELAY_TYPE_DELAY_1);
         Rx2Settings = MACGetRx2Setting();
         _lorawan_config.RX2DataRate = Rx2Settings.DataRate;
         _lorawan_config.RX2Frequency = Rx2Settings.Frequency;
         
         MACGetEUI(MAC_EUI_TYPE_APP, _lorawan_config.AppEUI);
         _lorawan_config.DeviceAddr = MACGetDevAddr(MAC_NETWORK_ADDR_TYPE_DEV_ADDR);
         
         MACGetChannelInfo(0, &ChInfo);
         _lorawan_config.MaxDataRate = ChInfo.DataRateMax;
         _lorawan_config.MinDataRate = ChInfo.DataRateMin;
         
         //can't read keys from module  
      }
      else
         _lorawan_set_join_state(LORAWAN_JOIN_STATE_NOT_JOINED);
      
      Result = TRUE;
   }
   else
      Result = FALSE;
  
   return(Result);
}

int1 _lorawan_set_config(void)
{
   rn2903_ec_t Error = RN2903_EC_OK;
   uint8_t sIndex = 0;
   rx2_struct_t Settings;
   uint8_t i;
   
   debug_lorawan_printf(debug_lorawan_putc, "   _lorawan_set_config()\r\n");
   
   while((Error == RN2903_EC_OK) && (sIndex < 6))
   {
      switch(sIndex)
      {
         case 0:
            Error = MACSetSyncByte(_lorawan_config.Sync);
            
            debug_lorawan_printf(debug_lorawan_putc, "      MACSetSyncByte() - %u\r\n", Error);
            break;
         case 1:
            Error = MACSetRetransmissions(_lorawan_config.RetransmissionAttempts);
            
            debug_lorawan_printf(debug_lorawan_putc, "      MACSetRetransmissions() - %u\r\n", Error);
            break;
         case 2:
            Error = MACSetRxDelay1(_lorawan_config.RXDelay);
            
            debug_lorawan_printf(debug_lorawan_putc, "      MACSetRxDelay1() - %u\r\n", Error);
            break;
         case 3:
            Settings.DataRate = _lorawan_config.RX2DataRate;
            Settings.Frequency = _lorawan_config.RX2Frequency;
            
            Error = MACSetRx2Setting(Settings);
            
            debug_lorawan_printf(debug_lorawan_putc, "      MACSetRx2Setting() - %u\r\n", Error);
            break;
         case 4:
            for(i=_lorawan_config.FirstChannel;i<_lorawan_config.LastChannel;i++)
            {
               Error = MACSetChannelDataRate(i, _lorawan_config.MinDataRate, _lorawan_config.MaxDataRate);
               
               if(Error != RN2903_EC_OK)
                  break;
               
               delay_ms(10);  //without this delay it would return and unknown error, I don't know why.
            }
            
            debug_lorawan_printf(debug_lorawan_putc, "      MACSetChannelDataRate() - %u\r\n", Error);
            break;
         case 5:
            for(i=0;i<64;i++)
            {
               if((i >= _lorawan_config.FirstChannel) && (i <= _lorawan_config.LastChannel))
                  Error = MACEnableDisableChannel(i, TRUE);
               else
                  Error = MACEnableDisableChannel(i, FALSE);
               
               if(Error != RN2903_EC_OK)
                  break;
            }
            
            debug_lorawan_printf(debug_lorawan_putc, "     MACEnableDisableChannel() - %u\r\n", Error);
            break;
      }
      
      sIndex++;
   }
   
   return(Error == RN2903_EC_OK);
}

_lorawan_module_error_t _lorawan_join(lorawan_join_type_t Type)
{
   _lorawan_module_error_t Error;
   
   Error = _lorawan_get_module_error_from_error(MACJoin(Type));
   
   debug_lorawan_printf(debug_lorawan_putc, "_lorawan_join() - %u\r\n", Error);
   
   return(Error);
}

lorawan_module_resp_t _lorawan_get_radio_message(uint8_t *Data, uint8_t *Length)
{
   lorawan_module_resp_t Result;
   rn2903_resp_t Response;
   
   if(RN2903SerialKbhit())
   {
      Response = RN2903SerialGetResponse(Data, Length);
      
      switch(Response)
      {
         case RN2903_RESP_MAC_TX_OK:
            Result = LORAWAN_MODULE_RESP_TX_OK;
            break;
         case RN2903_RESP_MAC_RX:
            Result = LORAWAN_MODULE_RESP_RX;
            break;
         case RN2903_RESP_MAC_ERR:
            Result = LORAWAN_MODULE_RESP_ERR;
            break;
         case RN2903_RESP_INVALID_DATA_LEN:
            Result = LORAWAN_MODULE_RESP_LENGTH;
            break;
         case RN2903_RESP_DENIED:
            Result = LORAWAN_MODULE_RESP_DENIED;
            break;
         case RN2903_RESP_ACCEPTED:
            Result = LORAWAN_MODULE_RESP_ACCEPTED;
            break;
         default:
            Result = LORAWAN_MODULE_RESP_UNKNOWN;
            break;
      }
   }
   else
      Result = LORAWAN_MODULE_RESP_NONE;
   
   return(Result);
}

int1 _lorawan_paused(void)
{
   mac_status_t Status;
   
   Status = MACGetStatus();
   
   return(Status.Paused);
}

int1 _lorawan_joined(void)
{
   mac_status_t Status;
   
   Status = MACGetStatus();
   
   if(Status.Joined && (Status.RejoinNeeded == FALSE))
      return(TRUE);
   else
      return(FALSE);
}

int1 _lorawan_set_rx2_data_rate(uint8_t Rate)
{
   int1 Result;
   rx2_struct_t Settings;
   
   Settings = MACGetRx2Setting();
   
   if((Settings.DataRate != 0) && (Settings.Frequency != 0))
   {
      Settings.DataRate = Rate;
      
      Result = (MACSetRx2Setting(Settings) == RN2903_EC_OK);
   }
   else
      Result = FALSE;
   
   return(Result);
}

int1 _lorawan_set_rx2_frequency(uint32_t Frequency)
{
   int1 Result;
   rx2_struct_t Settings;
   
   Settings = MACGetRx2Setting();
   
   if((Settings.DataRate != 0) && (Settings.Frequency != 0))
   {
      Settings.Frequency = Frequency;
      
      Result = (MACSetRx2Setting(Settings) == RN2903_EC_OK);
   }
   else
      Result = FALSE;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////

_lorawan_module_error_t _lorawan_get_module_error_from_error(rn2903_ec_t Error)
{
   _lorawan_module_error_t aError;

   switch(Error)
   {
      case RN2903_EC_OK:
         aError = LORAWAN_MODULE_ERROR_OK;
         break;
      case RN2903_EC_BUSY:
         aError = LORAWAN_MODULE_ERROR_BUSY;
         break;
      case RN2903_EC_FRAM_COUNTER_ERR_REJOIN_NEEDED:
         aError = LORAWAN_MODULE_ERROR_FRAME_COUNTER;
         break;
      case RN2903_EC_INVALID_DATA_LEN:
         aError = LORAWAN_MODULE_ERROR_LENGTH;
         break;
      case RN2903_EC_INVALID_PARAM:
         aError = LORAWAN_MODULE_ERROR_PARAMETER;
         break;
      case RN2903_EC_KEYS_NOT_INIT:
         aError = LORAWAN_MODULE_ERROR_NOT_INIT;
         break;
      case RN2903_EC_MAC_PAUSED:
         aError = LORAWAN_MODULE_ERROR_PAUSED;
         break;
      case RN2903_EC_NO_FREE_CH:
         aError = LORAWAN_MODULE_ERROR_CHANNEL;
         break;
      case RN2903_EC_NOT_JOINED:
         aError = LORAWAN_MODULE_ERROR_NOT_JOINED;
         break;
      case RN2903_EC_SILENT:
         aError = LORAWAN_MODULE_ERROR_SILENT;
         break;
      default:
         aError = LORAWAN_MODULE_ERROR_UNKNOWN;
         break;
   }
   
   return(aError);
}

#endif

#ignore_warnings none
