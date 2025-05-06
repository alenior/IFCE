///////////////////////////////////////////////////////////////////////////
////                              pr9200.h                             ////
////                                                                   ////
//// Driver for RED5 UHF RFID Reader module containing a PR9200.  See  ////
//// pr9200.c for API.                                                 ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2019 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                       http://www.ccsinfo.com                      ////
///////////////////////////////////////////////////////////////////////////

#ifndef __PR9200_H__
#define __PR9200_H__

#ifndef PR9200_TX_PIN
 #define PR9200_TX_PIN              PIN_B6
#endif

#ifndef PR9200_RX_PIN
 #define PR9200_RX_PIN              PIN_C10
#endif

#ifndef PR9200_BAUD_RATE
 #define PR9200_BAUD_RATE           115200
#endif

#ifndef PR9200_RX_BUFFER_SIZE
 #define PR9200_RX_BUFFER_SIZE      3
#endif

#ifndef PR9200_TX_BUFFER_SIZE
 #define PR9200_TX_BUFFER_SIZE      3
#endif

#ifndef PR9200_USE_DYNAMIC_MEMORY
 #define PR9200_USE_DYNAMIC_MEMORY  TRUE
#endif

#if PR9200_USE_DYNAMIC_MEMORY == FALSE
 #ifndef PR9200_MAX_PAYLOAD_SIZE
  #define PR9200_MAX_PAYLOAD_SIZE   256
 #endif
#endif

#if getenv("CRC") > 0
 #ifndef PR9200_FORCE_SW_CRC
  #define PR9200_FORCE_SW_CRC    FALSE
 #endif
#else
 #ifndef PR9200_FORCE_SW_CRC
  #define PR9200_FORCE_SW_CRC    TRUE
 #elif PR9200_FORCE_SW_CRC == FALSE
  #undef PR9200_FORCE_SW_CRC
  #define PR9200_FORCE_SW_CRC    TRUE
 #endif
#endif

#ifndef PR9200_UART_RX_BUFFER_SIZE
 #define PR9200_UART_RX_BUFFER_SIZE    256
#endif

#ifndef PR9200_UART_TX_BUFFER_SIZE
 #define PR9200_UART_TX_BUFFER_SIZE    256
#endif

#ifndef PR9200_USE_EXTERNAL_ANT_SWITCH
 #define PR9200_USE_EXTERNAL_ANT_SWITCH   FALSE
#endif

#ifndef PR9200_RESTART_WDT
 #define PR9200_RESTART_WDT   FALSE
#endif

#if PR9200_UART_TX_BUFFER_SIZE > 0
 #use rs232(xmit=PR9200_TX_PIN, rcv=PR9200_RX_PIN, baud=PR9200_BAUD_RATE, RECEIVE_BUFFER=PR9200_UART_RX_BUFFER_SIZE, TRANSMIT_BUFFER=PR9200_UART_TX_BUFFER_SIZE, TXISR, stream=PR9200_STREAM)
#else
 #use rs232(xmit=PR9200_TX_PIN, rcv=PR9200_RX_PIN, baud=PR9200_BAUD_RATE, RECEIVE_BUFFER=PR9200_UART_RX_BUFFER_SIZE, stream=PR9200_STREAM)
#endif

#include <stdint.h>

#if PR9200_USE_DYNAMIC_MEMORY == TRUE
 #include <stdlibm.h>
#endif

#ifndef NULL
 #define NULL     0
#endif

#if defined(__PCD__)
 #define __PACKED    __attribute__((packed))
#else
 #define __PACKED
#endif

///////////////////////////////////////////////////////////////////////////////

#define PR9200_MESSAGE_PREAMBLE        0xBB
#define PR9200_MESSAGE_END_MASK        0x7E

#define PR9200_ERROR_REPSONSE_LENGTH   3

//////////////////////////////// Message Types ////////////////////////////////

//Message Types
typedef enum
{
   PR9200_MESSAGE_TYPE_COMMAND,
   PR9200_MESSAGE_TYPE_RESPONSE,
   PR9200_MESSAGE_TYPE_NOTIFICATION,
} PR9200_MESSAGE_TYPE;

//Message Command Codes
typedef enum
{
   PR9200_MESSAGE_CODE_SET_READER_POWER_CONTROL=0x01,
   PR9200_MESSAGE_CODE_GET_READER_INFO=0x03,
   PR9200_MESSAGE_CODE_GET_REGION=0x06,
   PR9200_MESSAGE_CODE_SET_REGION,
   PR9200_MESSAGE_CODE_SET_SYS_RESET,
   PR9200_MESSAGE_CODE_GET_TYPE_C_AI_SELECT_PARAMS=0x0B,
   PR9200_MESSAGE_CODE_SET_TYPE_C_AI_SELECT_PARAMS,
   PR9200_MESSAGE_CODE_GET_TYPE_C_AI_QUERY_PARAMS,
   PR9200_MESSAGE_CODE_SET_TYPE_C_AI_QUERY_PARAMS,
   PR9200_MESSAGE_CODE_GET_RF_CHANNEL=0x11,
   PR9200_MESSAGE_CODE_SET_RF_CHANNEL,
   PR9200_MESSAGE_CODE_GET_FH_LBT_PARAMS,
   PR9200_MESSAGE_CODE_SET_FH_LBT_PARAMS,
   PR9200_MESSAGE_CODE_GET_TX_POWER_LEVEL,
   PR9200_MESSAGE_CODE_SET_TX_POWER_LEVEL,
   PR9200_MESSAGE_CODE_RF_CW_SIGNAL_CTRL,
   PR9200_MESSAGE_CODE_READ_TYPE_C_UII=0x22,
   PR9200_MESSAGE_CODE_READ_TYPE_C_UII_RSSI,
   PR9200_MESSAGE_CODE_READ_TYPE_C_UII_TID=0x25,
   PR9200_MESSAGE_CODE_AUTO_READ_ANTENNA_RESPONSE,
   PR9200_MESSAGE_CODE_READ_TYPE_C_TAG_DATA=0x29,
   PR9200_MESSAGE_CODE_READ_TYPE_C_TAG_LONG_DATA,
   PR9200_MESSAGE_CODE_GET_SESSION=0x2E,
   PR9200_MESSAGE_CODE_SET_SESSION,
   PR9200_MESSAGE_CODE_GET_FREQ_HOPPING_TABLE,
   PR9200_MESSAGE_CODE_SET_FREQ_HOPPING_TABLE,
   PR9200_MESSAGE_CODE_GET_MODULATION,
   PR9200_MESSAGE_CODE_SET_MODULATION,
   PR9200_MESSAGE_CODE_GET_ANTI_COLLISION_MODE,
   PR9200_MESSAGE_CODE_SET_ANTI_COLLISION_MODE,
   PR9200_MESSAGE_CODE_START_AUTO_READ2,
   PR9200_MESSAGE_CODE_STOP_AUTO_READ2,
   PR9200_MESSAGE_CODE_START_AUTO_READ_RSSI,
   PR9200_MESSAGE_CODE_STOP_AUTO_READ_RSSI,
   PR9200_MESSAGE_CODE_START_AUTO_READ_ANTENNA,
   PR9200_MESSAGE_CODE_WRITE_TYPE_C_TAG=0x46,
   PR9200_MESSAGE_CODE_BLOCK_WRITE_TYPE_C_TAG,
   PR9200_MESSAGE_CODE_BLOCK_ERASE_TYPE_C_TAG,
   PR9200_MESSAGE_CODE_KILL_TYPE_C_TAG=0x65,
   PR9200_MESSAGE_CODE_LOCK_TYPE_C_TAG=0x82,
   PR9200_MESSAGE_CODE_BLOCK_PERMALOCK_TYPE_C_TAG,
   PR9200_MESSAGE_CODE_AUTHENTICATE=0x84,    //DNA tags
   PR9200_MESSAGE_CODE_UNTRACEABLE=0x85,     //DNA tag
   PR9200_MESSAGE_CODE_ANTENNA_CHECK=0xAC,
   PR9200_MESSAGE_CODE_GET_TEMP=0xB7,
   PR9200_MESSAGE_CODE_GET_RSSI=0xC5,
   PR9200_MESSAGE_CODE_SCAN_RSSI,
   PR9200_MESSAGE_CODE_GET_DTC_RESULT=0xCA,
   PR9200_MESSAGE_CODE_UPDATE_REGISTRY=0xD2,
   PR9200_MESSAGE_CODE_GET_REGISTRY_ITEM=0xD4,
   PR9200_MESSAGE_CODE_SET_OPTIMUM_FREQ_HOPPING_TABLE=0xE4,
   PR9200_MESSAGE_CODE_GET_FREQ_HOPPING_MODE,
   PR9200_MESSAGE_CODE_SET_FREQ_HOPPING_MODE,
   PR9200_MESSAGE_CODE_GET_TX_LEAKAGE_RSSI_LEVEL,
   PR9200_MESSAGE_CODE_SET_TX_LEAKAGE_RSSI_LEVEL,
   PR9200_MESSAGE_CODE_START_READ_WITH_FAST_LEAKAGE_CAL=0xEC,
   PR9200_MESSAGE_CODE_REQUEST_FAST_LEAKAGE_CAL,
   PR9200_MESSAGE_CODE_COMMAND_FAILURE=0xFF,
} PR9200_MESSAGE_CODE;

typedef struct
{
   PR9200_MESSAGE_TYPE Type;  //Message Type see above
   PR9200_MESSAGE_CODE Code;  //Message Code see above
   uint16_t Length;           //Message Payload length
} PR9200_MESSAGE_HEADER;

typedef struct
{
   PR9200_MESSAGE_HEADER Header;                //Message Header
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;                            //Pointer to Payload data
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE];    //Payload data
  #endif
   uint16_t Crc;                                //Message CRC16
} PR9200_MESSAGE_STRUCT;

typedef enum
{
   PR9200_RX_MODE_PREAMBLE,
   PR9200_RX_MODE_MESSAGE_TYPE,
   PR9200_RX_MODE_CODE,
   PR9200_RX_MODE_LENGTH,
   PR9200_RX_MODE_PAYLOAD,
   PR9200_RX_MODE_END_MASK,
   PR9200_RX_MODE_CRC16,
} PR9200_RX_MODE;

typedef union
{
   uint8_t b[2];
   uint16_t w;
} UINT16_UNION;

typedef union
{
   uint8_t b[4];
   uint32_t dw;
} UINT32_UNION;

/////////////////////////////// Global Variables //////////////////////////////

struct
{
   PR9200_MESSAGE_STRUCT Message[PR9200_RX_BUFFER_SIZE];
  #if PR9200_RX_BUFFER_SIZE > 256
   uint16_t NextIn;
   uint16_t NextOut;
   uint16_t Count;
  #else
   uint8_t NextIn;
   uint8_t NextOut;
   uint8_t Count;
  #endif
} g_RxMessages;

struct
{
   PR9200_MESSAGE_STRUCT Message[PR9200_TX_BUFFER_SIZE];
  #if PR9200_TX_BUFFER_SIZE > 256
   uint16_t NextIn;
   uint16_t NextOut;
   uint16_t Count;
  #else
   uint8_t NextIn;
   uint8_t NextOut;
   uint8_t Count;
  #endif
} g_TxMessages;

////////////////////////////// Message Sub Types //////////////////////////////

typedef enum {
   PR9200_READER_INFO_MODEL,
   PR9200_READER_INFO_SN,
   PR9200_READER_INFO_MANUFACTURE,
   PR9200_READER_INFO_FREQUENCY,
   PR9200_READER_INFO_TAG_TYPE
} PR9200_READER_INFO;

typedef enum {
   PR9200_REGION_KOREA = 0x11,
   PR9200_REGION_US_WIDE = 0x21,
   PR9200_REGION_US_NARROW,
   PR9200_REGION_EUROPE = 0x31,
   PR9200_REGION_JAPAN = 0x41,
   PR9200_REGION_CHINA = 0x52,
   PR9200_REGION_BRAZIL = 0x61,
} PR9200_REGION;

typedef struct __PACKED
{
   uint8_t MemoryBank:2;   //RFU (00), EPC (01), TID (10), User (11)
   uint8_t Action:3;       //Refer to ISO18000-6C
   uint8_t Target:3;       //S0 (000), S1 (001), S2 (010), S3 (011), SL (100)
   uint32_t Pointer;       //Starting mask address
   uint8_t Length;         //mask length bits
   uint8_t Reserve:7;      //Reserved, should be 0000000
   uint8_t Truncate:1;     //enable (1), disable (0)
   uint8_t Mask[32];       //Mask value
} PR9200_TYPE_C_AI_SELECT_PARAMS;

typedef struct __PACKED
{
   uint8_t Session:2;      //S0 (00), S1 (01), S2 (10), S3 (11)
   uint8_t Sel:2;          //All (00 or 01), ~SL (10), SL (11)
   uint8_t TRext:1;        //No pilot tone (0), Use pilot tone (1)
   uint8_t M:2;            //M=1 (00), M=2 (01), M=4 (10), M=8 (11)
   uint8_t DR:1;           //DR=8 (0), DR=64/3 (1)
   uint8_t Reserve:3;      //Reserved
   uint8_t Q:4;            //0-15 number of slots in the round
   uint8_t Target:1;       //A (0), B(1)
} PR9200_TYPE_C_AI_QUERY_PARAMS;

typedef struct __PACKED
{
   uint16_t RT;   //Read Time (1 = 1ms)
   uint16_t IT;   //Idle Time (1 = 1ms)
   uint16_t CST;  //Carrier Sense Time
   uint16_t RFL;  //Target RF power level (-dBm x 10)
   uint8_t FH;    //enable (0x01 or over), disable (0x00)
   uint8_t LBT;   //enable (0x01 or over), disable (0x00)
   uint8_t CW;    //enable (0x01 or over), disable (0x00)
} PR9200_FH_LBT_PARAMS;

typedef struct
{
   uint16_t Power;
   uint16_t MinPower;
   uint16_t MaxPower;
} PR9200_TX_POWER_LEVEL;

#define TYPE_C_MAX_EPC_LENGTH    16

typedef struct __PACKED
{
   uint16_t PC;
   uint8_t EPC[TYPE_C_MAX_EPC_LENGTH];
   uint16_t Length;
} TYPE_C_UII;

typedef enum
{
   TYPE_C_MEMORY_BANK_RFU,   //Reserved for Future Use - I think
   TYPE_C_MEMORY_BANK_EPC,   //The EPC of the Tag
   TYPE_C_MEMORY_BANK_TID,   //The TID of the Tag
   TYPE_C_MEMORY_BANK_USER,  //User Memory of the Tag
} TYPE_C_MEMORY_BANK;

typedef enum
{
   PR9200_SESSION_S0,
   PR9200_SESSION_S1,
   PR9200_SESSION_S2,
   PR9200_SESSION_S3,
   PR9200_SESSION_DEV=0xF0,
} PR9200_SESSION;

typedef enum
{
   PR9200_RF_MOD_FM0,
   PR9200_RF_MOD_M2,
   PR9200_RF_MOD_M4,
   PR9200_RF_MOD_M8,
} PR9200_RF_MOD;

typedef struct __PACKED
{
   uint16_t BLF;        //Backscatter link freq, in KHz
   PR9200_RF_MOD RxMod;
   uint8_t DR;          //Divide ratio, 0-8, 1-64/3
} PR9200_MODULATION;

typedef struct __PACKED
{
   uint8_t Mode;  //0-fixed, 1-dynamic
   uint8_t Start;
   uint8_t Max;
   uint8_t Min;
} PR9200_ANTI_COLLISION;

typedef enum
{
   TAG_TYPE_B = 0x01,
   TAG_TYPE_C,
} TAG_TYPE;

typedef struct       // Tag RSSI Calculation:                     
{                    //    RFIN_I' = 20log(RSSI_I) - Gain_I - 63 
   uint8_t RSSI_I;   //    RFIN_Q' = 20log(RSSI_Q) - Gain_Q - 63 
   uint8_t RSSI_Q;   //    RFIN_I" = 10^(RFIN_I'/20)             
   uint8_t Gain_I;   //    RFIN_Q" = 10^(RFIN_Q'/20)             
   uint8_t Gain_Q;   //    RFIN = sqrt((RFIN_I")^2 + (RFIN_Q")^2)
} TAG_RSSI;          //    Tag RSSI = 20log(RFIN)                

typedef struct __PACKED
{
   uint8_t RL;             //Read/Lock, 0x00 Read, 0x01 Permalock
   TYPE_C_MEMORY_BANK MB;  //Target Memory bank
   uint16_t BP;            //Mask starting address, specified in units of 16 blocks
   uint8_t BR;             //Mask range, specified in units of 16 blocks
} TAG_PERMALOCK;

typedef struct __PACKED
{
   uint8_t IDT;   //inductor number for digital tune
   uint8_t DTC1;  //digital tunable capacitor 1
   uint8_t DTC2;  //digital tunable capacitor 2
   uint8_t RSSI;  //leakage RSSI value to check leakage cancellation results
   uint8_t State; //state number of leakage cancellation algorithm
   uint8_t CC;    //current channel
   uint8_t OT;    //operation time of leakage cancellation (ms)
} PR9200_DTC_RESULT;

typedef enum
{
   PR9200_REGISTRY_ITEM_VERSION,             //Register Version
   PR9200_REGISTRY_ITEM_DATE,                //Firmware Date
   PR9200_REGISTRY_ITEM_BAND,
   PR9200_REGISTRY_ITEM_TX_POWER,
   PR9200_REGISTRY_ITEM_FH_LBT,
   PR9200_REGISTRY_ITEM_ANTI_COLLISION,      //Anti-collision mode
   PR9200_REGISTRY_ITEM_MODULATION,          //Modulation mode
   PR9200_REGISTRY_ITEM_QUERY,
   PR9200_REGISTRY_ITEM_FREQ_HOPPING_TABLE,
   PR9200_REGISTRY_ITEM_TX_POWER_TABLE,
} PR9200_REGISTRY_ITEM;

typedef enum
{
   PR9200_ERROR_CODE_NOT_SUPPORTED=0x01,
   PR9200_ERROR_CODE_INSUF_PRIVILEGES,
   PR9200_ERROR_CODE_MEMORY_OVERRUN,
   PR9200_ERROR_CODE_MEMORY_LOCKED,
   PR9200_ERROR_CODE_CYPTO_SUITE,
   PR9200_ERROR_CODE_COMMAND_NOT_ENCAPSULATED,
   PR9200_ERROR_CODE_RESPONSE_BUF_OVERFLOW,
   PR9200_ERROR_CODE_SECURITY_TIMEOUT,
   PR9200_ERROR_CODE_INSUF_POWER,
   PR9200_ERROR_CODE_NON_SPECIFIC,
   PR9200_ERROR_CODE_SENSOR_SCHEDULING_CONFIG=0x11,
   PR9200_ERROR_CODE_TAG_BUSY,
   PR9200_ERROR_CODE_MEASURMENT_TYPE_NOT_SUPPORTED,
   PR9200_ERROR_CODE_NO_TAG_DETECTED=0x80,
   PR9200_ERROR_CODE_HANDLE_ACQ_FAILURE,
   PR9200_ERROR_CODE_ACCESS_PASSWORD_FAILURE,
   PR9200_ERROR_CODE_CRC=0x90,
   PR9200_ERROR_CODE_RX_TIMEOUT,
   PR9200_ERROR_CODE_REGISTRY_UPDATE_FAILURE=0xA0,
   PR9200_ERROR_CODE_REGISTRY_ERASE_FAILURE,
   PR9200_ERROR_CODE_REGISTRY_WRITE_FAILURE,
   PR9200_ERROR_CODE_REGISTRY_NOT_EXIST,
   PR9200_ERROR_CODE_UART_FAILURE=0xB0,
   PR9200_ERROR_CODE_SPI_FAILURE,
   PR9200_ERROR_CODE_I2C_FAILURE,
   PR9200_ERROR_CODE_GPIO_FAILURE,
   PR9200_ERROR_CODE_COMMAND_NOT_SUPPORTED=0xE0,
   PR9200_ERROR_CODE_COMMAND_UNDEFINED,
   PR9200_ERROR_CODE_INVALID_PARAMETER,
   PR9200_ERROR_CODE_PARAMETER_TOO_HIGH,
   PR9200_ERROR_CODE_PARAMETER_TOO_LOW,
   PR9200_ERROR_CODE_AUTO_READ_FAILURE,
   PR9200_ERROR_CODE_NOT_AUTO_READ_MODE,
   PR9200_ERROR_CODE_FAILURE_TO_GET_LAST_RESPONSE,
   PR9200_ERROR_CODE_FAILURE_TO_CONTROL_TEST,
   PR9200_ERROR_CODE_FAILURE_TO_RESET_READER,
   PR9200_ERROR_CODE_RFIDBLOCK_CONTROL_FAILURE,
   PR9200_ERROR_CODE_AUTO_READ_IN_OPEATION,
   PR9200_ERROR_CODE_UNDEFINED=0xF0,
   PR9200_ERROR_CODE_FAILURE_TO_VERIFY_WRITE,
   PR9200_ERROR_CODE_ABNOMAL_ANTENNA=0xFC,
   PR9200_ERROR_CODE_NONE=0xFF
} PR9200_ERROR_CODE;

typedef struct
{
   uint8_t Reserved;
   PR9200_MESSAGE_CODE Command;
   PR9200_ERROR_CODE Error;
} PR9200_ERROR;

typedef enum
{
   TYPE_C_PASSWORD_KILL,
   TYPE_C_PASSWORD_ACCESS,
} TYPE_C_PASSWORD;

typedef enum
{
   LOCK_MEMORY_REGION_USER,
   LOCK_MEMORY_REGION_TID,
   LOCK_MEMORY_REGION_EPC,
   LOCK_MEMORY_REGION_ACCESS_PASSWORD,
   LOCK_MEMORY_REGION_KILL_PASSWORD,
} LOCK_MEMORY_REGION;

typedef struct
{
   uint8_t U;  //0x00-Deassert, 0x01-assert
   union 
   {
      struct
      {
         uint8_t Length:5; //The length of the EPC returned/needed for commands that return or require the UII
         uint8_t SH:1;     //EPC 0-show, 1-hide
         uint8_t R:2;      //reserved;
      };
      uint8_t EPC;
   };
   uint8_t TID;   //0x00-hide none, 0x01-hide some, 0x02 hide all
   uint8_t User;  //0x00-view, 0x01-hide
   uint8_t Range; //A Tag's operating range, 0x00-normal, 0x01-toggle temporarily, 0x02-reduced
} UNTRACEABLE_DNA_SETTINGS;

////////////////////////////////// Prototypes /////////////////////////////////

void pr9200_init(void);
void pr9200_rx_task(void);
void pr9200_tx_task(void);
int1 pr9200_has_rx_data(void);
int1 pr9200_get_message(PR9200_MESSAGE_TYPE *Type, PR9200_MESSAGE_CODE *Code, uint8_t *Payload, uint16_t *Length);
int1 pr9200_put_message(PR9200_MESSAGE_CODE Code, uint8_t *Payload, uint16_t Length);

///////////////////////// Command Functions Prototypes ////////////////////////

uint16_t GetReaderInfo(PR9200_READER_INFO Type, uint8_t *Response=NULL);
uint16_t GetRegion(uint8_t *Response=NULL);
uint16_t SetRegion(PR9200_REGION Region, uint8_t *Response=NULL);
uint16_t SetSytemReset(uint8_t *Response=NULL);
uint16_t GetTypeC_AI_SelectParameters(PR9200_TYPE_C_AI_SELECT_PARAMS *Response=NULL);
uint16_t SetTypeC_AI_SelectParameters(PR9200_TYPE_C_AI_SELECT_PARAMS *Params, uint8_t *Response=NULL);
uint16_t GetTypeC_AI_QueryParameters(PR9200_TYPE_C_AI_QUERY_PARAMS *Response=NULL);
uint16_t SetTypeC_AI_QueryParameters(PR9200_TYPE_C_AI_QUERY_PARAMS *Params, uint8_t *Response=NULL);
uint16_t GetRFChannel(uint8_t *Response=NULL);
uint16_t SetRFChannel(uint8_t ChannelNumber, uint8_t ChannelOffset, uint8_t *Response=NULL);
uint16_t GetFH_LBT_Parameters(PR9200_FH_LBT_PARAMS *Response=NULL);
uint16_t SetFH_LBT_Parameters(PR9200_FH_LBT_PARAMS *Params, uint8_t *Response=NULL);
uint16_t GetTxPowerLevel(PR9200_TX_POWER_LEVEL *Response=NULL);
uint16_t SetTxPowerLevel(uint16_t TxPower, uint8_t *Response=NULL);
uint16_t TurnCWSignalOnOff(int1 On, uint8_t *Response=NULL);
uint16_t ReadTypeC_UII(TYPE_C_UII *Response=NULL);
uint16_t ReadTypeC_UII_TID(uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL);
uint16_t ReadTypeCTagData(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK rMemory, uint16_t rAddress, uint16_t rWords, uint32_t Password, uint8_t *Response=NULL);
uint16_t ReadTypeCTagDataLong(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK rMemory, uint16_t rAddress, uint16_t rWords, uint32_t Password, uint8_t *Response=NULL);
uint16_t GetSession(uint8_t *Response=NULL);
uint16_t SetSession(PR9200_SESSION Session, uint8_t *Response=NULL);
uint16_t GetFrequencyHoppingTable(uint8_t *Response=NULL);
uint16_t SetFrequencyHoppingTable(uint8_t Size, uint8_t *Table, uint8_t *Response=NULL);
uint16_t GetModulationMode(PR9200_MODULATION *Response=NULL);
uint16_t SetModulationMode(PR9200_MODULATION Mode, uint8_t *Response=NULL);
uint16_t GetAntiCollisionMode(PR9200_ANTI_COLLISION *Response = NULL);
uint16_t SetAntiCollisionMode(PR9200_ANTI_COLLISION Mode, uint8_t *Response=NULL);
uint16_t StartAutoRead(TAG_TYPE Type, uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL);
uint16_t StartAutoRead_RSSI(TAG_TYPE Type, uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL);
uint16_t StopAutoRead(uint8_t *Response=NULL);
#if PR9200_USE_EXTERNAL_ANT_SWITCH == FALSE
uint16_t StartAutoReadAntenna(int1 TagRSSI, uint8_t Antenna, uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL);
#endif
uint16_t WriteTypeCTagDataWithCode(PR9200_MESSAGE_CODE Code, TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK wMemory, uint16_t wAddress, uint8_t *Data, uint16_t wWords, uint32_t Password, uint8_t *Response=NULL);
uint16_t WriteTypeCTagData(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK wMemory, uint16_t wAddress, uint8_t *Data, uint16_t wWords, uint32_t Password, uint8_t *Response=NULL);
uint16_t WriteTypeCTagDataBlock(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK wMemory, uint16_t wAddress, uint8_t *Data, uint16_t wWords, uint32_t Password, uint8_t *Response=NULL);
uint16_t BlockEraseTypeCTagData(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK eMemory, uint16_t eAddress, uint16_t eWords, uint32_t Password, uint8_t *Response=NULL);
uint16_t BlockPermalockTypeCTag(TYPE_C_UII *Tag, uint32_t Password, TAG_PERMALOCK pLock, uint8_t *Mask, uint8_t *Response=NULL);
uint16_t KillTypeCTag(TYPE_C_UII *Tag, uint32_t Password, uint8_t *Response=NULL);
uint16_t LockTypeCTag(TYPE_C_UII *Tag, uint32_t Password, uint32_t LD, uint8_t *Response=NULL);
uint16_t AntennaCheck(uint8_t Level, uint8_t *Response=NULL);
uint16_t GetTemperature(uint8_t *Response=NULL);
uint16_t GetRSSI(uint8_t *Response=NULL);
uint16_t ScanRSSI(uint8_t *Response=NULL);
uint16_t GetDTCResult(PR9200_DTC_RESULT *Response=NULL);
uint16_t UpdateRegistry(uint8_t *Response=NULL);
uint16_t GetRegistryItem(PR9200_REGISTRY_ITEM Item, uint8_t *Response=NULL);
uint16_t SetOptimumFrequencyHoppingTable(uint8_t *Response=NULL);
uint16_t GetFrequencyHoppingMode(uint8_t *Response=NULL);
uint16_t SetFrequencyHoppingMode(uint8_t Mode, uint8_t *Response=NULL);
uint16_t GetTxLeakageRSSILevelForSmartHoppingMode(uint8_t *Response=NULL);
uint16_t SetTxLeakageRSSILevelForSmartHoppingMode(uint8_t Level, uint8_t *Response=NULL);
uint16_t StartReadWithFastLeakageCal(uint8_t Channel, uint8_t IDT, uint8_t DTC1, uint8_t DTC2, uint8_t *Response=NULL);
uint16_t AuthenticateDNATag(TYPE_C_UII *Tag, uint8_t *Code, uint16_t cLength, uint8_t *Response=NULL);
uint16_t UntraceableDNATagSettings(TYPE_C_UII *Tag, uint32_t Password, UNTRACEABLE_DNA_SETTINGS *Settings, uint8_t *Response=NULL);

// Helper Function Prototypes
uint16_t SetPassword(TYPE_C_UII *Tag, uint32_t aPassword, TYPE_C_PASSWORD pType, uint32_t Password, uint8_t *Response=NULL);
uint16_t ReadPassword(TYPE_C_UII *Tag, uint32_t aPassword, uint8_t pType, uint32_t *Response=NULL);
uint8_t DetectTags(TYPE_C_UII *Tags, uint8_t Max, uint8_t Time, float32 *TagsRSSI=NULL);
uint8_t DetectTagsAntenna(TYPE_C_UII *Tags, uint8_t Max, uint8_t Time, uint8_t Antenna, uint8_t *TagsAntenna, float32 *TagsRSSI=NULL);
PR9200_ERROR_CODE LockTagMemoryRegion(LOCK_MEMORY_REGION Region, TYPE_C_UII *Tag, uint32_t Password, int1 Lock);
int1 PermaLockTagMemoryRegion(LOCK_MEMORY_REGION Region, TYPE_C_UII *Tag, uint32_t Password, int1 Lock);

uint16_t SwapBytes(uint16_t Value);
uint32_t SwapBytes(uint32_t Value);
float32 CalculateTagRSSI(TAG_RSSI Rssi);
int1 CheckIfNewTag(TYPE_C_UII *Tag, TYPE_C_UII *Tags, uint8_t Count);

// Internal Function Prototypes, not intended to be called by user but can be.
int1 pr9200_calculate_crc16(PR9200_MESSAGE_STRUCT *Message, int1 IsCheck=FALSE);
uint16_t pr9200_command_no_params(PR9200_MESSAGE_CODE Code, uint8_t *Response=NULL);
int1 pr9200_get_response(PR9200_MESSAGE_CODE Code, uint8_t *Response, uint16_t *Length);
void pr9200_no_tx_error(uint8_t *Response);

#endif
