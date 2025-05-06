/**
 * EZApp.c
 *   
 * Main library file for the EZ App Lynx.
 *  
 * See EZApp.h for documentation.
 *
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

/**
 * \file EZApp.c
 */


#ifndef __EZAPP_C__
#define __EZAPP_C__

#if defined(__XC__)
   //microchip XC Compiler (XC8, XC16 or XC32)

   #ifndef __CCI__
      #error CCI is required to use the EZ App Lynx library in XC.  Make sure 'Use CCI Syntax' is checked in the MPLAB X project properties.
   #endif

   #include <system.h>
#else
   #define _PCD_QUIRK_1625
#endif

#include "EZApp.h"

#ifndef __DO_DEBUG_EZAPP
   #define debug_ezapp0(a)
   #define debug_ezapp1(a,b)

   #define debug_ezapp_array(p, n)
#else
   #define debug_ezapp0(a)                debug_printf0(a)
   #define debug_ezapp1(a,b)              debug_printf1(a,b)

   #define debug_ezapp_array(p, n) debug_array(p, n)
#endif

#include <string.h>
#include <stdlib.h>

#if !defined(EZAPP_STATIC_MEMORY_ALLOCATION)
   #if defined(__PCD__) || defined(__PCH__) || defined(__PCM__)
      #include <stdlibm.h>
   #endif

   #if defined(__XC8)
      #error XC8 compiler does not suppport malloc
   #endif
#endif

#if defined(__PCM__) || defined(__PCH__) || defined(__PCD__)
   #if !getenv("PSV")
      #define __EZAPP_NEEDS_ROM_POINTERS__
   #endif
#elif defined(__XC8)
   #define __EZAPP_NEEDS_ROM_POINTERS__
#endif

// original API called this macro EZ_APP_****, but it should have been
// called EZAPP_**** - so maintain backwards compatibility with any old code.
#if defined(EZ_APP_TICK_TYPE) && !defined(EZAPP_TICK_TYPE)
   #define EZAPP_TICK_TYPE EZ_APP_TICK_TYPE
#endif
#if defined(EZ_APP_TICKGET) && !defined(EZAPP_TICKGET)
   #define EZAPP_TICKGET EZ_APP_TICKGET
#endif
#if defined(EZ_APP_TICKS_PER_SECOND) && !defined(EZAPP_TICKS_PER_SECOND)
   #define EZAPP_TICKS_PER_SECOND EZ_APP_TICKS_PER_SECOND
#endif

#if defined(EZAPP_TICK_TYPE) && defined(EZAPP_TICKGET) && defined(EZAPP_TICKS_PER_SECOND)
   #define _EZAPP_DO_CONNECT_TIMEOUT_CHECK
#endif

#ifndef EZAPP_EEPROM_WRITE
   #if (EZAPP_EEPROM_SIZE > 0)
      #if defined(__PCM__) || defined(__PCH__) || defined(__PCD__)
         // CCS C Compiler
         #define EZAPP_EEPROM_WRITE(ee, b)   write_eeprom(ee, b)
      #elif defined(_PLIB)
        // XC8 C Compiler with plib
        #define EZAPP_EEPROM_WRITE(ee, b)   eeprom_write(ee, b)
      #endif
   #else
      #define EZAPP_EEPROM_WRITE(ee, b)
   #endif
#endif

#ifndef EZAPP_EEPROM_READ
   #if (EZAPP_EEPROM_SIZE > 0)
      #if defined(__PCM__) || defined(__PCH__) || defined(__PCD__)
         // CCS C Compiler
         #define EZAPP_EEPROM_READ(ee)   read_eeprom(ee)
      #elif defined(_PLIB)
         // XC8 C Compiler with plib
         #define EZAPP_EEPROM_READ(ee)   eeprom_read(ee)
      #endif
   #else
      #define EZAPP_EEPROM_READ(ee)    0
   #endif
#endif

#ifdef DEBUG_CFG_RESET_ON_REQ_CONFIG
uint8_t g_DEBUG_CFG_RESET_ON_REQ_CONFIG = 0;
#endif

typedef uint16_t _ezapp_fval_t; 

static void _EZAppPacketTxByte(char c);

#if 1 //region: rom/ram pointer
typedef struct
{
   #if defined(__XC8)
   struct   //packing the three types of pointers into on union to overlap the same RAM seeps to trip up the XC8 optimizer and causes these pointers to not work
   #else
   union
   #endif
   {
      char * ramPtr;
     #if (EZAPP_EEPROM_SIZE>0)
      EZAPP_EEPROM_ADDRESS eeAddr;
     #endif
     #if defined(__EZAPP_NEEDS_ROM_POINTERS__)
      rom char * romPtr;
     #endif
   };
   uint8_t loc;
} _ezapp_ptr_t;

#define _EZ_APP_PTR_BITMAP_ROM   0
#define _EZ_APP_PTR_BITMAP_EE    1
#define  _EZAppPtrSetRAM(ptr, address)    ptr.ramPtr = (char *)address; ptr.loc = 0
#if defined(__EZAPP_NEEDS_ROM_POINTERS__)
#define  _EZAppPtrSetROM(ptr, address)    ptr.romPtr = (rom char *)address; ptr.loc = 1
#else
#define  _EZAppPtrSetROM(ptr, address)    ptr.ramPtr = (char *)address; ptr.loc = 0
#endif
#if (EZAPP_EEPROM_SIZE>0)
#define  _EZAppPtrSetEE(ptr, address)    ptr.eeAddr = address; ptr.loc = 2
#endif

#if defined(__EZAPP_NEEDS_ROM_POINTERS__) && (EZAPP_EEPROM_SIZE>0)
#define  _EZAppPtrIsntNull(ptr)           (((ptr.loc==0)&&(ptr.ramPtr!=0))||((ptr.loc==1)&&(ptr.romPtr!=0))||((ptr.loc==2)&&(ptr.eeAddr!=0)))
#elif (EZAPP_EEPROM_SIZE>0) && !defined(__EZAPP_NEEDS_ROM_POINTERS__)
#define  _EZAppPtrIsntNull(ptr)           (((ptr.loc==0)&&(ptr.ramPtr!=0))||((ptr.loc==2)&&(ptr.eeAddr!=0)))
#elif defined(__EZAPP_NEEDS_ROM_POINTERS__) && (EZAPP_EEPROM_SIZE==0)
#define  _EZAppPtrIsntNull(ptr)           (((ptr.loc==0)&&(ptr.ramPtr!=0))||((ptr.loc==1)&&(ptr.romPtr!=0)))
#else
#define  _EZAppPtrIsntNull(ptr)           (ptr.ramPtr!=0)
#endif

typedef struct
{
   _ezapp_ptr_t ptr;    //use _EZAppPtrSetROM() or _EZAppPtrSetRAM() for assignment
   union {
      struct {
        uint8_t lenL:8;
        uint8_t lenH:7;
        uint8_t isString:1;
      };
      uint16_t len;
   };
   
   _ezapp_ptr_t extraPtr;
   union {
      struct {
        uint8_t extraLenL:8;
        uint8_t extraLenH:7;
        uint8_t isExtraString:1;
      };
      uint16_t extraLen;
   };
} _ezapp_fext_t, _ezapp_ptrinfo_t;

#define _EZAPP_IS_STRING_MARKER  15
#define _EZAPP_MASK_STRING_MARKER   0x7FFF

#define _EZAppSetFextBytesROM(struct_ptrinfo, pData, n)  _EZAppPtrSetROM(struct_ptrinfo.ptr, pData);  struct_ptrinfo.len=n;  struct_ptrinfo.isString=0;  _EZAppPtrSetRAM(struct_ptrinfo.extraPtr, NULL);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=0
#define _EZAppSetFextBytesRAM(struct_ptrinfo, pData, n)  _EZAppPtrSetRAM(struct_ptrinfo.ptr, pData);  struct_ptrinfo.len=n;  struct_ptrinfo.isString=0;  _EZAppPtrSetRAM(struct_ptrinfo.extraPtr, NULL);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=0
#define _EZAppSetFextBytesEE(struct_ptrinfo, pData, n)   _EZAppPtrSetEE(struct_ptrinfo.ptr, pData);   struct_ptrinfo.len=n;  struct_ptrinfo.isString=0;  _EZAppPtrSetRAM(struct_ptrinfo.extraPtr, NULL);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=0 

#define _EZAppSetFextStrROM(struct_ptrinfo, pStr)       _EZAppPtrSetROM(struct_ptrinfo.ptr, pStr);  struct_ptrinfo.len=0;    struct_ptrinfo.isString=1;  _EZAppPtrSetRAM(struct_ptrinfo.extraPtr, NULL);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=0
#define _EZAppSetFextStrRAM(struct_ptrinfo, pStr, max)  _EZAppPtrSetRAM(struct_ptrinfo.ptr, pStr);  struct_ptrinfo.len=max;  struct_ptrinfo.isString=1;  _EZAppPtrSetRAM(struct_ptrinfo.extraPtr, NULL);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=0 
#define _EZAppSetFextStrEE(struct_ptrinfo, pStr, max)   _EZAppPtrSetEE(struct_ptrinfo.ptr, pStr);   struct_ptrinfo.len=max;  struct_ptrinfo.isString=1;  _EZAppPtrSetRAM(struct_ptrinfo.extraPtr, NULL);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=0 

// When Fext is sent, the data in both pointers is "concatinated" together in the transmission
#define _EZAppSetFextBytesRAMStrROM(struct_ptrinfo, pData, n, p2)  _EZAppPtrSetRAM(struct_ptrinfo.ptr, pData);  struct_ptrinfo.len=n;  struct_ptrinfo.isString=0;  _EZAppPtrSetROM(struct_ptrinfo.extraPtr, p2);  struct_ptrinfo.extraLen=0;  struct_ptrinfo.isExtraString=1 

char _EZAppPtrDeref(_ezapp_ptr_t *pp)
{
   char c;
   _ezapp_ptr_t p;
   
   memcpy(&p, pp, sizeof(_ezapp_ptr_t));

  #if defined(__EZAPP_NEEDS_ROM_POINTERS__)
   if (bit_test(p.loc, _EZ_APP_PTR_BITMAP_ROM))
   {
      c = *p.romPtr;
      p.romPtr++;
   }
   else 
  #endif
  #if (EZAPP_EEPROM_SIZE>0)
   if (bit_test(p.loc, _EZ_APP_PTR_BITMAP_EE))
   {
      c = EZAPP_EEPROM_READ(p.eeAddr);
      p.eeAddr++;
   }
   else
  #endif
   {
      c = *p.ramPtr;
      p.ramPtr++;
   }
   
   memcpy(pp, &p, sizeof(_ezapp_ptr_t));
   
   return(c);
}

// returns 0 if equal, non-zero if not
bool _EZAppPtrStrcmp(_ezapp_ptr_t p1, _ezapp_ptr_t p2)
{
   char c1, c2;

   if (_EZAppPtrIsntNull(p1) && _EZAppPtrIsntNull(p2))
   {
      for(;;)
      {
         c1 = _EZAppPtrDeref(&p1);
         c2 = _EZAppPtrDeref(&p2);
        
         if (c1 != c2)
            break;
         
         if (c1 == 0)
         {
            return(false);
         }
      }
   }

   return(true);
}

void _EZAppTxPtrStr(_ezapp_ptr_t p)
{
   char c;
   
   if (_EZAppPtrIsntNull(p))
   {
      for(;;)
      {
         c = _EZAppPtrDeref(&p);
         if (!c || (c >= 0x7f))
            break;
         //debug_ezapp1("%c", c);
         _EZAppPacketTxByte(c);
      }
    }
    
   _EZAppPacketTxByte(0);  //transmit a null character
}

// transmit contents of p.  p may be a rom or ram pointer, and may be a
// string with dynamic length or bytes with a fixed length.
void _EZAppTxPtrInfo(_ezapp_ptrinfo_t p)
{
   debug_ezapp1("PUTINFO_%lX_", p.ptr.ramPtr);
  #if defined(__XC8__)
   debug_ezapp1("%lX_", p.ptr.romPtr);
  #endif
   debug_ezapp1("%X_", p.ptr.loc);
   debug_ezapp1("%lX ", p.len);
   
   if (p.isString)
   {
      _EZAppTxPtrStr(p.ptr);
   }
   else
   {
      if (_EZAppPtrIsntNull(p.ptr))
      {
         while(p.len--)
         {
            _EZAppPacketTxByte(_EZAppPtrDeref(&p.ptr));
         }
      }
   }
   
   // check if we have more data in the extra pointer
   if (p.isExtraString)
   {
      _EZAppTxPtrStr(p.extraPtr);
   }
   else
   {
      if (_EZAppPtrIsntNull(p.extraPtr))
      {
         while(p.extraLen--)
         {
            _EZAppPacketTxByte(_EZAppPtrDeref(&p.extraPtr));
         }
      }
   }
}
#endif //region end: rom/ram pointer

#if 1 //region: global variables/typedefs

#define _EZAPP_REQ_CONFIG     0x00
#define _EZAPP_RES_CONFIG     0x40
#define _EZAPP_REQ_FIELD      0x01
#define _EZAPP_RES_FIELD      0x41
#define _EZAPP_REQ_POLL       0x02
#define _EZAPP_RES_POLL       0x42
#define _EZAPP_REQ_UPDATE     0x03
#define _EZAPP_RES_UPDATE     0x43
#define _EZAPP_REQ_EXTENDED   0x04
#define _EZAPP_RES_EXTENDED   0x44
#define _EZAPP_REQ_LOGIN      0x05
#define _EZAPP_RES_LOGIN      0x45
#define _EZAPP_REQ_UPEXT      0x06
#define _EZAPP_RES_UPEXT      0x46
#if defined(EZ_APP_TICKS_PER_SECOND)
#define _EZAPP_REQ_MONITOR    0x07
#define _EZAPP_RES_MONITOR    0x47
#endif
#if defined(EZAPP_REQ_VENDOR)
#define _EZAPP_REQ_VENDOR     0x08
#define _EZAPP_RES_VENDOR     0x48
#endif
#if defined(EZAPP_BOOT_RESET_POINTER) && defined(EZAPP_BOOT_RESET_PIC) && defined(EZAPP_BOOT_DATA)
#define _EZAPP_REQ_BOOT_CMD   0x09
#define _EZAPP_RES_BOOT_CMD   0x49
#define _EZAPP_REQ_BOOT_DATA  0x0a
#define _EZAPP_RES_BOOT_DATA  0x4a
#endif

typedef enum
{
   _EZAPP_REQUPD_MASK_IGNORE = 0,
   _EZAPP_REQUPD_MASK_SET = 1,
   _EZAPP_REQUPD_MASK_OR = 2,
   _EZAPP_REQUPD_MASK_AND = 3,
   _EZAPP_REQUPD_MASK_XOR = 4
} _ezapp_requpd_mask;

typedef enum
{
   _EZAPP_FTYPE_INVALID       = 0,
   _EZAPP_FTYPE_STRING        = 1,
   _EZAPP_FTYPE_ANALOG        = 2,
   _EZAPP_FTYPE_BUTTON        = 3,
   _EZAPP_FTYPE_DIGITAL       = 4,
   _EZAPP_FTYPE_SPACER        = 5,
   _EZAPP_FTYPE_IMAGE         = 6,
   _EZAPP_FTYPE_STYLE         = 7,
   _EZAPP_FTYPE_GRAPH         = 8,
   _EZAPP_FTYPE_GRAPH_SERIES  = 9,
   _EZAPP_FTYPE_COLOR_PICKER  = 10,
   _EZAPP_FTYPE_COLOR_PICKER_DUMMY  = 11,
   _EZAPP_FTYPE_JOYSTICK      = 12
} _ezapp_ftype_t;

typedef struct
{
   uint8_t b[8];
} _ezapp_fconfig_t;

typedef struct
{
   _ezapp_fval_t fVal;
   _ezapp_fconfig_t fConfig;
   rom char * header;
   _ezapp_fext_t fExt;
} _ezapp_element_t;

typedef union __EZAPP_PACKED
{
   struct __EZAPP_PACKED
   {
      uint8_t fType;
      unsigned int kbhit: 1;
      unsigned int readOnly: 1;    //this data type is read-only by the Android/iOS app
      unsigned int clearFValOnReadFExt: 1; //clear the fVal when read by the fext has been read Android/iOS app
      unsigned int hasFVal: 1;     //has a 16bit value
      unsigned int hasFConfig: 1;  //has an 8 byte fconfig array
      unsigned int hasHeader: 1;   //has a rom* pointer
      unsigned int hasFExt: 1;     //has a _ezapp_fext_t
      unsigned int reserved: 1;
   };
   
   uint16_t val;
} _ezapp_field_cfg;

typedef struct
{
   _ezapp_field_cfg cfg;
   _ezapp_element_t element;
} _ezapp_scratch_t;

typedef union __EZAPP_PACKED
{
   struct __EZAPP_PACKED
   {
      unsigned int authLevel:2;    //see ezapp_authlevel_t for valid values
      unsigned int configChanged: 1;
   };
   uint8_t b[2];
   uint16_t val;
} _ezapp_pollflags_t;

struct
{  
  #if defined(EZAPP_STATIC_MEMORY_ALLOCATION)
   _ezapp_scratch_t fields[EZAPP_STATIC_MEMORY_ALLOCATION];
  #else
   void *fieldsHead; //see _ezapp_field_node_t
   void *fieldsLast; //see _ezapp_field_node_t
  #endif
  
  #if defined(_EZAPP_DO_CONNECT_TIMEOUT_CHECK)
   struct
   {
      bool valid;
      EZAPP_TICK_TYPE t;
      uint8_t seconds;
   } timeoutCheck;
  #else
   unsigned int connected: 1;
  #endif
  
  #if defined(_EZAPP_REQ_MONITOR)
   struct
   {
      uint8_t longTicks;
      EZ_APP_TICK_TYPE shortTicks;
      EZ_APP_TICK_TYPE rxTicks;
      EZ_APP_TICK_TYPE txHoldoffStart;
      EZ_APP_TICK_TYPE txHoldoffDur;
      unsigned int txHoldoffDo: 1;
      unsigned int getRx: 1;
      unsigned int valid: 1;
      unsigned int doTx: 1;
   } monitor;
  #endif
  
   // this is the same order as the dynamic fields in RES_CONFIG
   struct __EZAPP_PACKED
   {
      uint16_t fieldsNum;
      _ezapp_pollflags_t pollFlags;
      uint16_t rate;
      uint8_t fgColor[3];  //rgb 0=b 1=g 2=r (little endian)
      uint8_t bgColor[3];  //rgb
      uint8_t appColor[3];  //rgb
      uint32_t key;
      uint8_t cfgFlags;
      uint8_t max;
   } unitConfig;

   // workspace for reading/writing a field
   _ezapp_scratch_t scr;
   
   // packet handling
   struct
   {
      union
      {
         struct __EZAPP_PACKED
         {
            uint8_t seq;
            uint8_t cmd;
            uint8_t payload[EZAPP_MAX_STRING_SIZE + 1];
         };

         struct __EZAPP_PACKED
         {
            uint8_t seq;
            uint8_t cmd;
             uint16_t findex;
            _ezapp_requpd_mask mask;
            _ezapp_fval_t data;
         } reqUpdate;

         uint8_t b[EZAPP_MAX_STRING_SIZE + 3];
      };
      
      uint8_t len;   //len of payload on rx, not used on tx
      
      uint8_t cs;    //used on tx, not rx
      //bool enableHandling;
      //bool packetKbhit;
     #if defined(_EZAPP_REQ_MONITOR)
      uint8_t txSeq;
     #endif
      unsigned int ready: 1;
     #if !defined(EZAPP_USES_UDP)
      unsigned int escaped: 1;
     #endif
      unsigned int waitingForSOF: 1;
   } packet;
} _g_EZApp;

typedef union __EZAPP_PACKED
{
   ezapp_graph_cfg_t user;
   
   struct __EZAPP_PACKED
   {
      uint8_t fconfig[8];
      uint8_t fext[12];
      rom char* pLabels;
      rom char* pTitle;
   } pack_data;
} ezapp_graph_cfg_internal_t;

#define _EZAppMarkConfigChanged()   _EZAppMonitorPrepareRes(); _g_EZApp.unitConfig.pollFlags.configChanged = true
#endif //region: global variables/typedefs

#if 1 //region: packet handling
#ifndef __DO_DEBUG_PACKET
   #define debug_packet0(a)
   #define debug_packet1(a,b)

   #define debug_packet_array(p, n)
#else
   #define debug_packet0(a)                debug_printf0(a)
   #define debug_packet1(a,b)              debug_printf1(a,b)

   #define debug_packet_array(p, n) debug_array(p, n)
#endif

////////// end config

#define _EZAPP_PACKET_OVERHEAD_BYTES (3)   //seq, cmd and cs

static void _EZAppPacketInit(void)
{
   _g_EZApp.packet.ready = false;

   _g_EZApp.packet.waitingForSOF = true;

  #if !defined(EZAPP_USES_UDP)  
   _g_EZApp.packet.escaped = false;
  #endif
}

#define _EZAPP_PACKET_CHAR_SOF       0x7E
#if !defined(EZAPP_USES_UDP)
#define _EZAPP_PACKET_CHAR_EOF       0x7D
#define _EZAPP_PACKET_CHAR_ESCAPED   0x7C
#endif

#if !defined(EZAPP_USES_UDP)
static void _EZAppPacketSendNack(void);
#endif

static bool _EZAppPacketTask(void)
{
   static uint8_t i, cs;
   char c;
  #ifdef RN4020_CHECK_FOR_CONNPARAM
   static char _connparam_scr[32];
   static char _connparam_expect[] = "ConnParam:001C";
   size_t _connparam_len;
  #endif
  
   if (_g_EZApp.packet.ready)
      return(true);
   
   while (BtSppKbhit())
   {
      c = BtSppGetc();
      
      debug_packet1("%02X", c);
      //debug_printf1("%02X", c);

#ifdef RN4020_CHECK_FOR_CONNPARAM
   if (c == 0x0d)
   {
      debug_printf1("GOT '%s'\r\n", _connparam_scr);
      _connparam_scr[strlen(_connparam_expect)] = 0;
      if (strcmp(_connparam_scr, _connparam_expect) == 0)
      {
         output_low(PIN_BT_MLDP_CMD_MODE);
         delay_ms(1);
         output_high(PIN_BT_MLDP_CMD_MODE);
      }
      _connparam_scr[0] = 0;
   }
   else if (c != 0x0a)
   {
      _connparam_len = strlen(_connparam_scr);
      if (_connparam_len < (sizeof(_connparam_scr)-1))
      {
         _connparam_scr[_connparam_len] = c;
         _connparam_scr[_connparam_len + 1] = 0;
      }
   }
#endif
   
      if
      (
         (c == _EZAPP_PACKET_CHAR_SOF)
        #if defined(EZAPP_USES_UDP)
         && (UDPGetRxBuffer() == 1)
        #endif
      )
      {
         i = 0;
         cs = 0;
        #if !defined(EZAPP_USES_UDP)
         _g_EZApp.packet.escaped = false;
        #endif
         _g_EZApp.packet.waitingForSOF = false;
         //debug_putc('~');
      }
      else if (_g_EZApp.packet.waitingForSOF)
      {
         // do nothing
      }
     #if !defined(EZAPP_USES_UDP)
      else if (c == _EZAPP_PACKET_CHAR_EOF)
      {
#ifdef RN4020_CHECK_FOR_CONNPARAM
         _connparam_scr[0] = 0;
#endif
         _g_EZApp.packet.waitingForSOF = true;
         if ((cs == 0) && (i >=_EZAPP_PACKET_OVERHEAD_BYTES))
         {
            _g_EZApp.packet.ready = true;
            _g_EZApp.packet.len = i - _EZAPP_PACKET_OVERHEAD_BYTES;
            debug_packet1("PACKET RX CMD=%X ", _g_EZApp.packet.cmd);
            debug_packet1("LEN=%u\r\n", _g_EZApp.packet.len);
            return(true);
         }
         else
         {
            debug_packet0("PACKET CS/LEN FAIL\r\n");
            _EZAppPacketSendNack();
         }
      }
     #endif
     #if !defined(EZAPP_USES_UDP)
      else if (c == _EZAPP_PACKET_CHAR_ESCAPED)
      {
         _g_EZApp.packet.escaped = true;
      }
     #endif
      else 
      {
         #if !defined(EZAPP_USES_UDP)
         if (_g_EZApp.packet.escaped)
         {
            _g_EZApp.packet.escaped = false;
            c ^= 0x20;
         }
         #endif
         cs ^= c;
         if (i < sizeof(_g_EZApp.packet.b))
         {
            _g_EZApp.packet.b[i] = c;
         }
         i++;
         #if defined(EZAPP_USES_UDP)
         if 
         (
            (UDPIsGetReady(_g_EzAppUdp.socket) == 0) &&
            (cs == 0) && 
            (i >=_EZAPP_PACKET_OVERHEAD_BYTES)
         )
         {
            _g_EZApp.packet.waitingForSOF = true;
            _g_EZApp.packet.len = i - _EZAPP_PACKET_OVERHEAD_BYTES;
            debug_packet1("PACKET RX CMD=%X ", _g_EZApp.packet.cmd);
            debug_packet1("LEN=%u\r\n", _g_EZApp.packet.len);
            _g_EZApp.packet.ready = true;
            return(true);
         }
         else
         {
            debug_packet0("PACKET CS/LEN FAIL\r\n");
         }         
         #endif
      }
   }
   return(false);
}

static void _EZAppPacketCharPutcEscaped(char c)
{
   debug_packet1("%02X ", c);
  #if !defined(EZAPP_USES_UDP)
   if ((c==_EZAPP_PACKET_CHAR_SOF) || (c==_EZAPP_PACKET_CHAR_ESCAPED) || (c==_EZAPP_PACKET_CHAR_EOF))
   {
      BtSppPutc(_EZAPP_PACKET_CHAR_ESCAPED);
      c ^= 0x20;
   }
  #endif
   BtSppPutc(c);
}

static void _EZAppPacketTxByte(char c)
{
   _g_EZApp.packet.cs ^= c;
   _EZAppPacketCharPutcEscaped(c);
}

static void _EZAppPacketTxBytes(char *p, uint8_t n)
{   
   char c;
   while(n--)
   {
      c = *p++;
      _EZAppPacketTxByte(c);
   }
}

static void _EZAppPacketTxStart(void)
{
   //debug_printf2(debug_putc2, "EZ TX 0x%X\r\n", _g_EZApp.packet.cmd);
   
   BtSppPutc(_EZAPP_PACKET_CHAR_SOF);

   _g_EZApp.packet.cs = 0;

   debug_packet0("PacketTxStart() ");

   _EZAppPacketTxByte(_g_EZApp.packet.seq);
   _EZAppPacketTxByte(_g_EZApp.packet.cmd);

   debug_packet0("- ");
}

static void _EZAppPacketTxEnd(void)
{
   _EZAppPacketCharPutcEscaped(_g_EZApp.packet.cs);
   
  #if defined(_EZAPP_PACKET_CHAR_EOF)
   BtSppPutc(_EZAPP_PACKET_CHAR_EOF);
  #endif
   
   BtSppFlush();
   
   debug_packet0("\r\n");
}

#if !defined(EZAPP_USES_UDP)
static void _EZAppPacketSendNack(void)
{
   _g_EZApp.packet.cmd = 0x6e; //nack
   
   _EZAppPacketTxStart();
   _EZAppPacketTxEnd();
}
#endif
#endif //region: packet handling

#if 1 //region: dynamic fields and linked list
#define _EZAppCreateFieldCfg(fType, hasFVal, hasFConfig, hasHeader, hasFExt, appIsReadOnly, clearFValOnFextRead)   \
   memset(&_g_EZApp.scr, 0x00, sizeof(_g_EZApp.scr));   \
   _g_EZApp.scr.cfg.val = (uint16_t)fType | \
      (appIsReadOnly << (uint16_t)9) |  \
      (clearFValOnFextRead << (uint16_t)10) | \
      (hasFVal << (uint16_t)11) | \
      (hasFConfig << (uint16_t)12) | \
      (hasHeader << (uint16_t)13) | \
      (hasFExt << (uint16_t)14)

typedef struct _ezapp_field_node
{
   // defines the content of pBuffer
   // also holds fType
   _ezapp_field_cfg cfg;
   
   // see 'cfg' for the contents/size that this points to.
   // if'cfg'==0 then nothing is allocated here and this is NULL.
   // order of data:
   //    fVal (always 16bit, see _ezapp_fval_t)
   //    fConfig (always 8 bytes)
   //    header (a rom char *)
   //    fExt (a ezapp_univ_ptr_t)
   // if this isn't null, this must be free() when node is removed.
   // any RAM contents given by the user need to be global/statically allocted
   // and the user has to manage their usage.
   // also see _EZAppGetFieldIndexes()
   void *pBuffer;
   
   struct _ezapp_field_node *pNext;
} _ezapp_field_node_t;

static _ezapp_field_node_t* _EZAppGetField(ezapp_field_index_t index);

#if defined(EZAPP_STATIC_MEMORY_ALLOCATION)
static void _EZAppIndexToScratch(ezapp_field_index_t index)
{
   if (index >= _g_EZApp.unitConfig.fieldsNum)
      memset(&_g_EZApp.scr, 0x00, sizeof(_ezapp_scratch_t));
   else
      memcpy(&_g_EZApp.scr, &_g_EZApp.fields[index], sizeof(_ezapp_scratch_t));
}

static void _EZAppScratchToIndex(ezapp_field_index_t index)
{
   if (index < _g_EZApp.unitConfig.fieldsNum)
   {
      memcpy(&_g_EZApp.fields[index], &_g_EZApp.scr, sizeof(_ezapp_scratch_t));
   }
}
#else
static void _EZAppIndexToScratch(ezapp_field_index_t index)
{
   _ezapp_field_node_t *pNode;
   void *pBuffer;

   memset(&_g_EZApp.scr, 0x00, sizeof(_g_EZApp.scr));

   pNode = _EZAppGetField(index);
   if (pNode == NULL)
      return;
  
   _g_EZApp.scr.cfg = pNode->cfg;
   pBuffer = pNode->pBuffer;

   if (_g_EZApp.scr.cfg.hasFVal)
   {
      _g_EZApp.scr.element.fVal = *((uint16_t*)pBuffer);
      pBuffer += sizeof(uint16_t);
   }

  
   if (_g_EZApp.scr.cfg.hasFConfig)
   {
      memcpy(&_g_EZApp.scr.element.fConfig, pBuffer, sizeof(_ezapp_fconfig_t));
      pBuffer += sizeof(_ezapp_fconfig_t);
   }
   
   if (_g_EZApp.scr.cfg.hasHeader)
   {
      //_g_EZApp.scr.element.header = ((rom char *)pBuffer);
      memcpy(&_g_EZApp.scr.element.header, pBuffer, sizeof(rom char *));
      pBuffer += sizeof(rom char *);
   }


   if (_g_EZApp.scr.cfg.hasFExt)
   {
      memcpy(&_g_EZApp.scr.element.fExt, pBuffer, sizeof(_ezapp_fext_t));
   }
}

static void _EZAppScratchToIndex(ezapp_field_index_t index)
{
   _ezapp_field_node_t *pNode;
   void *pBuffer;
   
   pNode = _EZAppGetField(index);
   if (pNode == NULL)
      return;
   
   pNode->cfg = _g_EZApp.scr.cfg;
   pBuffer = pNode->pBuffer;

   if (_g_EZApp.scr.cfg.hasFVal)
   {
      //*((uint16_t*)pBuffer) = _g_EZApp.scr.element.fVal;
      memcpy(pBuffer, &_g_EZApp.scr.element.fVal, sizeof(uint16_t));
      pBuffer += sizeof(uint16_t);
   }
   
   if (_g_EZApp.scr.cfg.hasFConfig)
   {
      memcpy(pBuffer, &_g_EZApp.scr.element.fConfig, sizeof(_ezapp_fconfig_t));
      pBuffer += sizeof(_ezapp_fconfig_t);
   }
   
   if (_g_EZApp.scr.cfg.hasHeader)
   {
      //*((rom char *)pBuffer) = _g_EZApp.scr.element.header;
      memcpy(pBuffer, &_g_EZApp.scr.element.header, sizeof(rom char *));
      pBuffer += sizeof(rom char *);
   }

   if (_g_EZApp.scr.cfg.hasFExt)
   {
      memcpy(pBuffer, &_g_EZApp.scr.element.fExt, sizeof(_ezapp_fext_t));
   }
}
#endif

#if defined(_EZAPP_REQ_MONITOR)
   #define _EZ_APP_SET_FVAL(newFVAL)  \
      if (_g_EZApp.scr.element.fVal != newFVAL) \
         _EZAppMonitorPrepareRes(); \
      _g_EZApp.scr.element.fVal=newFVAL
#else
   #define _EZ_APP_SET_FVAL(newFVAL)   _g_EZApp.scr.element.fVal=newFVAL
#endif

#if defined(_EZAPP_REQ_MONITOR)
static void _EZAppMonitorSendRes(void)
{
   debug_ezapp0("SEND RES_MONITOR\r\n");
 
   _g_EZApp.packet.cmd = _EZAPP_RES_MONITOR;
   _g_EZApp.packet.payload[0] = _g_EZApp.unitConfig.pollFlags.b[0];
   _g_EZApp.packet.payload[1] = _g_EZApp.unitConfig.pollFlags.b[1];
   _EZAppPacketTxStart();
   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, 2);
   _EZAppPacketTxEnd();
   
   _g_EZApp.monitor.txHoldoffStart = EZ_APP_TICKGET();
   _g_EZApp.monitor.txHoldoffDo = _g_EZApp.monitor.valid;   
   _g_EZApp.monitor.txHoldoffDur = (EZ_APP_TICKS_PER_SECOND)*(EZ_APP_TICK_TYPE)_g_EZApp.unitConfig.rate;
   _g_EZApp.monitor.txHoldoffDur /= (EZ_APP_TICK_TYPE)1000;
}

static void _EZAppMonitorClearReq(void)
{
   debug_ezapp0("CLEAR REQ_MONITOR\r\n");
   
   _g_EZApp.monitor.valid = false;
   _g_EZApp.monitor.getRx = false;
}

static void _EZAppMonitorHandleReq(void)
{
   debug_ezapp1("HANDLE REQ_MONITOR t=%X\r\n", _g_EZApp.packet.payload[0]);
   
   _g_EZApp.monitor.longTicks = _g_EZApp.packet.payload[0];
   
   _g_EZApp.monitor.getRx = false;
   
   if (_g_EZApp.monitor.longTicks != 0)
   {
      _g_EZApp.monitor.valid = true;
      _g_EZApp.monitor.shortTicks = EZ_APP_TICKGET();
   }
   else
   {
      _EZAppMonitorClearReq();
   }
   
   _EZAppMonitorSendRes();
}

static void _EZAppMonitorTask(void)
{
   EZ_APP_TICK_TYPE t;

   if (_g_EZApp.monitor.valid)
   {
      t = EZ_APP_TICKGET();

      if ((t - _g_EZApp.monitor.shortTicks) >= (EZ_APP_TICKS_PER_SECOND*15))
      {
         _g_EZApp.monitor.shortTicks = t;
         
         if (--_g_EZApp.monitor.longTicks == 0)
         {
            debug_ezapp0("TASK REQ_MONITOR DONE\r\n");
            _EZAppMonitorClearReq();
         }
      }
   }
   
   if (_g_EZApp.monitor.getRx)
   {
      t = EZ_APP_TICKGET();
      
      if ((t - _g_EZApp.monitor.rxTicks) >= (EZ_APP_TICKS_PER_SECOND*5))
      {
         debug_ezapp0("TASK REQ_MONITOR NO_ACK\r\n");
         _EZAppMonitorClearReq();
      }
   }
   
   if 
   (
      (_g_EZApp.monitor.txHoldoffDo) &&
      ((EZ_APP_TICKGET() - _g_EZApp.monitor.txHoldoffStart) >= _g_EZApp.monitor.txHoldoffDur)
   )
   {
      debug_ezapp0("TASK REQ_MONITOR TX_HOLDOFF\r\n");
   
      _g_EZApp.monitor.txHoldoffDo = false;
   }
   
   if
   (
      _g_EZApp.monitor.doTx &&
      !_g_EZApp.monitor.txHoldoffDo &&
      BtSppIsPutReady() &&
      (  //not busy receiving a packet and don't have a packet in memory
         _g_EZApp.packet.waitingForSOF ||
         !_g_EZApp.packet.ready
      )
   )
   {
      _g_EZApp.monitor.doTx = false;
      _g_EZApp.monitor.getRx = true;
      _g_EZApp.monitor.rxTicks = EZ_APP_TICKGET();

      if (++_g_EZApp.packet.txSeq == _g_EZApp.packet.seq)
      {
         _g_EZApp.packet.txSeq = _g_EZApp.packet.seq - 3;
      }

      _EZAppMonitorSendRes();
   }
}

static void _EZAppMonitorAckedRes(void)
{
   //debug_ezapp0("RES_MONITOR ACKED\r\n");

   _g_EZApp.monitor.getRx = false;
}

static void _EZAppMonitorPrepareRes(void)
{
   if (_g_EZApp.monitor.valid)
   {
      debug_ezapp0("PREPARE RES_MONITOR\r\n");

      _g_EZApp.monitor.doTx = true;
   }
}
#else
#define _EZAppMonitorClearReq()
#define _EZAppMonitorTask()
#define _EZAppMonitorAckedRes()
#define _EZAppMonitorPrepareRes()
#endif

#if defined(EZAPP_REQ_VENDOR)
static void _EZAppHandleReqVendor(void)
{
   uint8_t resLen;
   
   resLen = EZAPP_REQ_VENDOR(_g_EZApp.packet.payload, _g_EZApp.packet.len);
   
   _g_EZApp.packet.cmd = _EZAPP_RES_VENDOR;
   
   _EZAppPacketTxStart();
   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, resLen);
   _EZAppPacketTxEnd();   
}
#endif

#if defined(_EZAPP_REQ_BOOT_CMD)
static void _EZAppHandleReqBootCmd(void)     //_EZAPP_REQ_BOOT_CMD
{
   int1 reboot;
   unsigned int32 writePointer;
   
   if (_g_EZApp.packet.payload[0] == 0)
   {
      memcpy(&writePointer, &_g_EZApp.packet.payload[1], 4);
      
      EZAPP_BOOT_RESET_POINTER(writePointer);
      
      reboot = 0;
   }
   else
   {
      writePointer = 0;
      
      reboot = 1;
   }
   
   _g_EZApp.packet.cmd = _EZAPP_RES_BOOT_CMD;
   
   memcpy(&_g_EZApp.packet.payload[0], &writePointer, 4);
  
  #if 1
   _g_EZApp.packet.payload[4] = EZAPP_MAX_STRING_SIZE;
   _g_EZApp.packet.payload[5] = EZAPP_MAX_STRING_SIZE >> 8;
  #else
   #warning !! debug code
   //_g_EZApp.packet.payload[4] = 16;  // no workie
   _g_EZApp.packet.payload[4] = 8;  //works
   _g_EZApp.packet.payload[5] = 0;
  #endif

   _EZAppPacketTxStart();
   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, 6);
   _EZAppPacketTxEnd();
  
   if (reboot)
   {
      EZAPP_BOOT_RESET_PIC();
   }
}

static void _EZAppHandleReqBootData(void)    //_EZAPP_REQ_BOOT_DATA
{
   uint32_t ptrLoc;
   
   ptrLoc = EZAPP_BOOT_DATA(_g_EZApp.packet.payload, _g_EZApp.packet.len);
   
   _g_EZApp.packet.cmd = _EZAPP_RES_BOOT_DATA;
   _g_EZApp.packet.payload[0] = ptrLoc;
   _g_EZApp.packet.payload[1] = ptrLoc >> 8;
   _g_EZApp.packet.payload[2] = ptrLoc >> 16;
   _g_EZApp.packet.payload[3] = ptrLoc >> 24;

   _EZAppPacketTxStart();
   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, 4);
   _EZAppPacketTxEnd();
}
#endif

#if defined(EZAPP_STATIC_MEMORY_ALLOCATION)
static ezapp_field_index_t _EZAppAddField(void)
{
   ezapp_field_index_t ret = EZAPP_FIELD_INDEX_INVALID;
   
   if (_g_EZApp.unitConfig.fieldsNum < EZAPP_STATIC_MEMORY_ALLOCATION)
   {
      ret = _g_EZApp.unitConfig.fieldsNum++;
      
      _EZAppScratchToIndex(ret);
      
      _EZAppMarkConfigChanged();
   }
   
   return(ret);
}
#else
static ezapp_field_index_t _EZAppAddField(void)
{
   _ezapp_field_node_t* pNode;
   uint8_t n;
   void *pBuffer;
   
   pNode = malloc(sizeof(_ezapp_field_node_t));
   
   if (pNode != NULL)
   {
      n = 0;
      if (_g_EZApp.scr.cfg.hasFVal)
         n += sizeof(_ezapp_fval_t);
      if (_g_EZApp.scr.cfg.hasFConfig)
         n += sizeof(_ezapp_fconfig_t);
      if (_g_EZApp.scr.cfg.hasHeader)
         n += sizeof(rom char *);
      if (_g_EZApp.scr.cfg.hasFExt)
         n += sizeof(_ezapp_fext_t);
      
      if (n)
      {
         pBuffer = malloc(n);
         if (pBuffer == NULL)
         {
            // not enough space to allocate pBuffer, so free everything
            // and return the error.
            free((unsigned int8*)pNode);
            pNode = NULL;
         }
      }
      else
      {
         pBuffer = NULL;
      }
   }
   
   debug_ezapp1("EZAppAddField() cfg=%X ", _g_EZApp.scr.cfg.val);
   debug_ezapp1("n=%X ",  n);
   
   if (pNode != NULL)
   {
      pNode->pBuffer = pBuffer;
      pNode->pNext = NULL;

      if (_g_EZApp.unitConfig.fieldsNum == 0)
      {
         _g_EZApp.fieldsHead = pNode;
         _g_EZApp.fieldsLast = pNode;
      }
      else
      {
         ((_ezapp_field_node_t*)_g_EZApp.fieldsLast)->pNext = pNode;
         _g_EZApp.fieldsLast = pNode;
      }

      _EZAppMarkConfigChanged();
      
      _EZAppScratchToIndex(_g_EZApp.unitConfig.fieldsNum);
     
      debug_ezapp1("fext=%X ", _g_EZApp.scr.element.fExt.ptr.ramPtr);
      debug_ezapp1("fHdr=%X ", _g_EZApp.scr.element.header);
      debug_ezapp1("idx=%X\r\n", _g_EZApp.unitConfig.fieldsNum);
      
     #if defined(__PCD__)
     //#if 0
      //fixed in 5.048
      uint16_t ret;
      ret = _g_EZApp.unitConfig.fieldsNum;
      _g_EZApp.unitConfig.fieldsNum += 1;
      return(ret);
     #else
      return(_g_EZApp.unitConfig.fieldsNum++);
     #endif
   }
  #if defined(__DO_DEBUG_EZAPP)
   else
   {
      debug_ezapp0("ERR_MALLOC\r\n");
   }
  #endif
   
   return(EZAPP_FIELD_INDEX_INVALID);
}

static _ezapp_field_node_t* _EZAppGetField(ezapp_field_index_t index)
{
   _ezapp_field_node_t *ret;
   
   ret = _g_EZApp.fieldsHead;

   while(index-- && (ret!=NULL))
   {
      ret = ((_ezapp_field_node_t*)ret)->pNext;
   }
   
   return(ret);
}
#endif


void EZAppFieldsClearAll(void)
{
  #if !defined(EZAPP_STATIC_MEMORY_ALLOCATION)
   void *p;
   while (_g_EZApp.fieldsHead != NULL)
   {
      _g_EZApp.fieldsLast = ((_ezapp_field_node_t*)_g_EZApp.fieldsHead)->pNext;
      p = ((_ezapp_field_node_t*)_g_EZApp.fieldsHead)->pBuffer;
      if (p)
         free(p);
      free(_g_EZApp.fieldsHead);
      _g_EZApp.fieldsHead = _g_EZApp.fieldsLast;
   }
      
   _g_EZApp.fieldsHead = NULL;
   _g_EZApp.fieldsLast = NULL;
  #endif
  
   _g_EZApp.unitConfig.fieldsNum = 0;
   
   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char *)"CCS EZ App Lynx");
}

void EZAppSetValue(ezapp_field_index_t index, uint16_t newVal)
{
   _EZAppIndexToScratch(index);
      
   _EZ_APP_SET_FVAL(newVal);
   
   _EZAppScratchToIndex(index);
}

void EZAppSetValue32(ezapp_field_index_t index, uint32_t newVal)
{
   _EZAppIndexToScratch(index);
      
   _EZ_APP_SET_FVAL((uint16_t)newVal);
   
   _EZAppScratchToIndex(index);
   
   _EZAppIndexToScratch(index+1);
      
   _EZ_APP_SET_FVAL((uint16_t)(newVal>>16));
   
   _EZAppScratchToIndex(index+1);
}

uint16_t EZAppGetValue(ezapp_field_index_t index)
{
   _EZAppIndexToScratch(index);
   
   if (_g_EZApp.scr.cfg.kbhit)
   {
      _g_EZApp.scr.cfg.kbhit = false;
      _EZAppScratchToIndex(index);
   }
   
   return(_g_EZApp.scr.element.fVal);
}

uint32_t EZAppGetValue32(ezapp_field_index_t index)
{
   uint32_t val;
   
   _EZAppIndexToScratch(index);
   
   if (_g_EZApp.scr.cfg.kbhit)
   {
      _g_EZApp.scr.cfg.kbhit = false;
      _EZAppScratchToIndex(index);
   }
   
   val = _g_EZApp.scr.element.fVal;
   
   _EZAppIndexToScratch(index+1);
   
   if (_g_EZApp.scr.cfg.kbhit)
   {
      _g_EZApp.scr.cfg.kbhit = false;
      _EZAppScratchToIndex(index+1);
   }
   
   val |= (uint32_t)(_g_EZApp.scr.element.fVal) << 16;
   
   return(val);
}
#endif //region: dynamic fields and linked list

/*
#define _EZAppGetRFromRGB(i32) (make8(i32,2))
#define _EZAppGetGFromRGB(i32) (make8(i32,1))
#define _EZAppGetBFromRGB(i32) (make8(i32,0))
*/

void EZAppSetFGColor(uint32_t rgb)
{
    /*
   _g_EZApp.unitConfig.fgColor[0] = _EZAppGetBFromRGB(rgb);
   _g_EZApp.unitConfig.fgColor[1] = _EZAppGetGFromRGB(rgb);
   _g_EZApp.unitConfig.fgColor[2] = _EZAppGetRFromRGB(rgb);
     */
    memcpy(_g_EZApp.unitConfig.fgColor, &rgb, 3);
   _EZAppMarkConfigChanged();
}

void EZAppSetBGColor(uint32_t rgb)
{
    /*
   _g_EZApp.unitConfig.bgColor[0] = _EZAppGetBFromRGB(rgb);
   _g_EZApp.unitConfig.bgColor[1] = _EZAppGetGFromRGB(rgb);
   _g_EZApp.unitConfig.bgColor[2] = _EZAppGetRFromRGB(rgb);
     */
    memcpy(_g_EZApp.unitConfig.bgColor, &rgb, 3);
   _EZAppMarkConfigChanged();
}

void EZAppSetAppColor(uint32_t rgb)
{
    /*
   _g_EZApp.unitConfig.appColor[0] = _EZAppGetBFromRGB(rgb);
   _g_EZApp.unitConfig.appColor[1] = _EZAppGetGFromRGB(rgb);
   _g_EZApp.unitConfig.appColor[2] = _EZAppGetRFromRGB(rgb);
   */
   memcpy(_g_EZApp.unitConfig.appColor, &rgb, 3);
   _EZAppMarkConfigChanged();
}


//////////////////////////////////////////////////
/// Initializes the bluetooth module. Initlizes
/// packets, Initlizes timer, and clears fields            
//////////////////////////////////////////////////
void EZAppInit(void)
{
   debug_ezapp0("EZAppInit()\r\n");
   
   BtSppInit();
  
   _EZAppPacketInit();
  
   memset(&_g_EZApp, 0x00, sizeof(_g_EZApp));
   
  #if defined(__PCD__)
   //fixed in 5.048
   memset(&_g_EZApp.unitConfig.bgColor[0], 0xFF, 3);
   memset(&_g_EZApp.unitConfig.appColor[0], 0xCC, 3);
  #else
   memset(&_g_EZApp.unitConfig.bgColor[0], 0xFF, sizeof(_g_EZApp.unitConfig.bgColor));
   memset(&_g_EZApp.unitConfig.appColor[0], 0xCC, sizeof(_g_EZApp.unitConfig.appColor));
  #endif
   
   _g_EZApp.unitConfig.rate = 500;   //500ms
  
  #if defined(_EZAPP_REQ_MONITOR)
   #if defined(__PCM__) || defined(__PCH__) || defined(__PCD__)
    bit_set(_g_EZApp.unitConfig.cfgFlags, 0);
   #else
    _g_EZApp.unitConfig.cfgFlags |= 1;
   #endif
  #endif

   EZAppFieldsClearAll();
}

//////////////////////////////////////////////////
/// Sends the PIC configuration to the android   
/// device over the blutooth connection.         
//////////////////////////////////////////////////
static void _EZAppSendResConfig(void)
{
   _g_EZApp.packet.cmd = _EZAPP_RES_CONFIG;
   
   _EZAppPacketTxStart();
   _EZAppPacketTxByte(1);  //protocol version
   _EZAppPacketTxBytes((char *)&_g_EZApp.unitConfig, sizeof(_g_EZApp.unitConfig));
   _EZAppPacketTxEnd();
   
   debug_ezapp1("NEW CONFIGURATION SENT!, NUM FIELDS=%lu\r\n", _g_EZApp.unitConfig.fieldsNum);
}


//////////////////////////////////////////////////
/// Sends configuration and field type of the    
/// requested field to the android device.       
//////////////////////////////////////////////////
#if !defined(EZAPP_NO_REQ_FIELD_SUPPORT)
static void _EZAppSendResField(void)
{
   // format of packet sent by app
   /*
   typedef struct __EZAPP_PACKED
   {
      uint16_t findex;
   } _ezapp_req_field_packet_t;
   */
   
   // format of response sent to app
   typedef struct __EZAPP_PACKED
   {
      uint16_t findex;
      _ezapp_ftype_t ftype;
      _ezapp_fconfig_t fconfig;
   } _ezapp_res_field_packet_t;
  
   uint16_t index;
   _ezapp_ptrinfo_t pinfo;
   
   _g_EZApp.packet.cmd = _EZAPP_RES_FIELD;
   
   index = _g_EZApp.packet.payload[0];
   _EZAppIndexToScratch(index);
   
   _g_EZApp.packet.payload[2] = _g_EZApp.scr.cfg.fType;

   debug_ezapp1("RES FIELD IDX=%lu ", index);
   debug_ezapp1("FTYPE=%u ", _g_EZApp.scr.cfg.fType);
   debug_ezapp1("CFG=%lX ", _g_EZApp.scr.cfg.val);
   debug_ezapp1("HDR=%lX ", _g_EZApp.scr.element.header);

   memcpy(&_g_EZApp.packet.payload[3], &_g_EZApp.scr.element.fConfig, sizeof(_g_EZApp.scr.element.fConfig));
   
   _EZAppPacketTxStart();

   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, sizeof(_ezapp_res_field_packet_t));   //send the findex back

  #if defined(__XC8)
   // xc8 1.34 banking bug workaround
   pinfo.ptr.romPtr += 1;
  #endif

   _EZAppSetFextStrROM(pinfo, _g_EZApp.scr.element.header);

   _EZAppTxPtrInfo(pinfo);

   _EZAppPacketTxEnd();

   debug_ezapp0("\r\n");
}
#endif

//////////////////////////////////////////////////
/// Updates the requested field's value to the   
/// value given by the update request.           
//////////////////////////////////////////////////
static void _EZAppHandleReqUdpate(void)
{
   _ezapp_fval_t data;
   
   data = _g_EZApp.packet.reqUpdate.data;
      
   if (_g_EZApp.unitConfig.pollFlags.authLevel != EZAPP_AUTH_OPEN)
   {
      return;
   }
   
   _EZAppIndexToScratch(_g_EZApp.packet.reqUpdate.findex);

   debug_ezapp1("HandleReqUpdate() idx%X ", _g_EZApp.packet.reqUpdate.findex);
   debug_ezapp1("m%X ", _g_EZApp.packet.reqUpdate.mask);
   debug_ezapp1("d%X ", _g_EZApp.packet.reqUpdate.data);
   debug_ezapp1("f%X ", _g_EZApp.scr.element.fVal);

   if (_g_EZApp.packet.reqUpdate.mask == _EZAPP_REQUPD_MASK_SET)
      _g_EZApp.scr.element.fVal = data; 
   else if (_g_EZApp.packet.reqUpdate.mask == _EZAPP_REQUPD_MASK_OR)
      _g_EZApp.scr.element.fVal |= data; 
   else if (_g_EZApp.packet.reqUpdate.mask == _EZAPP_REQUPD_MASK_AND)
      _g_EZApp.scr.element.fVal &= data; 
   else if (_g_EZApp.packet.reqUpdate.mask == _EZAPP_REQUPD_MASK_XOR)
      _g_EZApp.scr.element.fVal ^= data; 
   
   _g_EZApp.scr.cfg.kbhit = true;
   
   _EZAppScratchToIndex(_g_EZApp.packet.reqUpdate.findex);
   
    debug_ezapp1("f%X\r\n", _g_EZApp.scr.element.fVal);
}

//////////////////////////////////////////////////
/// Sends the field value of the requested field 
/// to the android device.                       
//////////////////////////////////////////////////
static void _EZAppSendResPoll(uint8_t responseCmd)
{
   typedef struct __EZAPP_PACKED
   {
      uint16_t findex;
      uint8_t num;
      uint16_t pollFlags;
      // and there are 'uint8_t b[num]' values here too
   } _ezapp_res_poll_packet_t;
   
   ezapp_field_index_t index;
   uint8_t num;

   index = _g_EZApp.packet.payload[0];
   num = _g_EZApp.packet.payload[2];
   
  #if 1
   debug_ezapp1("RES POLL IDX=%U ", index);
   debug_ezapp1("N=%U ", num);
  #endif
   
   _g_EZApp.packet.cmd = responseCmd;
   
   //((_ezapp_res_poll_packet_t)_g_EZApp.packet.payload[0]).pollFlags = _g_EZApp.unitConfig.pollFlags;
   _g_EZApp.packet.payload[3] = _g_EZApp.unitConfig.pollFlags.b[0];
   _g_EZApp.packet.payload[4] = _g_EZApp.unitConfig.pollFlags.b[1];
   
   _EZAppPacketTxStart();
   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, sizeof(_ezapp_res_poll_packet_t));
    
   while(num--)
   {
      _EZAppIndexToScratch(index);
      
      if (_g_EZApp.unitConfig.pollFlags.authLevel == EZAPP_AUTH_LOCKED)
      {
         _g_EZApp.scr.element.fVal = 0;
      }
      
     #if 1
      debug_ezapp1("%LX ", _g_EZApp.scr.element.fVal);      
     #endif
      
      _EZAppPacketTxBytes((char *)&_g_EZApp.scr.element.fVal, sizeof(_ezapp_fval_t));

      index++;
   }
  
  #if 1
   debug_ezapp0("\r\n");
  #endif
   
   _EZAppPacketTxEnd();
}

//////////////////////////////////////////////////
/// Sends the extended value of the requested    
/// field to the android device.                 
//////////////////////////////////////////////////
static void _EZAppSendExtended(uint8_t cmd)
{
   #ifdef _PCD_QUIRK_1625
   unsigned int8 v;
   #endif
   /*
   typedef struct __EZAPP_PACKED
   {
      uint16_t findex;
   } _ezapp_req_extended_packet_t;
   */

   typedef struct __EZAPP_PACKED
   {
      uint16_t findex;
      uint16_t pollFlags;
   } _ezapp_res_extended_packet_t;
     
   ezapp_field_index_t index;

   //index = ((_ezapp_req_extended_packet_t)_g_EZApp.packet.payload[0]).findex;
   index = _g_EZApp.packet.payload[0];

   _EZAppIndexToScratch(index);

   _g_EZApp.packet.cmd = cmd;

   //((_ezapp_res_extended_packet_t)g_PacketRx.payload[0]).pollFlags = _g_EZApp.unitConfig.pollFlags.val;
   // some reason the above is giving me a compile error
#ifdef _PCD_QUIRK_1625
   v = _g_EZApp.unitConfig.pollFlags.val & 0xFF;
   _g_EZApp.packet.payload[2] = v;
   v = (_g_EZApp.unitConfig.pollFlags.val >> 8) & 0xFF;
   _g_EZApp.packet.payload[3] = v;
#else
   _g_EZApp.packet.payload[2] = _g_EZApp.unitConfig.pollFlags.val & 0xFF;

   _g_EZApp.packet.payload[3] = (_g_EZApp.unitConfig.pollFlags.val >> 8) & 0xFF;
#endif


   _EZAppPacketTxStart();

   _EZAppPacketTxBytes((char *)_g_EZApp.packet.payload, sizeof(_ezapp_res_extended_packet_t));   //send the findex back
   debug_ezapp1("ResExt_%X_", index);
   debug_ezapp1("%X ", _g_EZApp.scr.cfg.val);
   _EZAppTxPtrInfo(_g_EZApp.scr.element.fExt);
   debug_ezapp0("\r\n");
   _EZAppPacketTxEnd();
   
   if (_g_EZApp.scr.cfg.clearFValOnReadFExt)
   {
      EZAppSetValue(index, 0);   //mark the field as read
   }   
}

#if !defined(EZAPP_NO_LOGIN_SUPPORT)
void EZAppSetAuthLevel(ezapp_authlevel_t level)
{
  //#if defined(__PCD__)
  #if 0
   //fixed in 5.047
   uint8_t b;
   b = _g_EZApp.unitConfig.pollFlags.b[0];
   b &= 0xFC;
   b |= (level & 0x03);
   _g_EZApp.unitConfig.pollFlags.b[0] = b;
  #else
   _g_EZApp.unitConfig.pollFlags.authLevel = level;
  #endif
}

//////////////////////////////////////////////////
/// Determines if the requested login was        
/// successfull or not (if the entered password  
/// matched the set password) and creates a     
/// response packet dependent on the results.    
//////////////////////////////////////////////////
static void _EZAppHandleReqLogin(void)
{
   // this is the format of the reponse expected by the host
   /*
   typedef struct __EZAPP_PACKED
   {
      uint8_t result;
      uint16_t pollFlags;
   } _ezapp_res_login_packet_t;
   */
   
   _g_EZApp.packet.cmd = _EZAPP_RES_LOGIN;
   
   _EZAppPacketTxStart();
   
   // see _ezapp_res_login_packet_t above for format
   _EZAppPacketTxByte(EZAPP_CHECK_AUTHENTICATION(_g_EZApp.packet.payload));
   _EZAppPacketTxByte(_g_EZApp.unitConfig.pollFlags.b[0]);
   _EZAppPacketTxByte(_g_EZApp.unitConfig.pollFlags.b[1]);
   
   _EZAppPacketTxEnd();
}
#endif

static bool _EZAppExtIsWritable(ezapp_field_index_t index)
{
   if (index >= _g_EZApp.unitConfig.fieldsNum)
      return(false);
      
   _EZAppIndexToScratch(index);
   
   return(
         (!bit_test(_g_EZApp.scr.element.fExt.ptr.loc, _EZ_APP_PTR_BITMAP_ROM)) &&
         //(_g_EZApp.scr.element.fExt.len != -1) &&
         ((_g_EZApp.scr.element.fExt.len & _EZAPP_MASK_STRING_MARKER) != 0) &&
         !_g_EZApp.scr.cfg.readOnly
      );
}

#if (EZAPP_EEPROM_SIZE > 0)
static void _EZAppEEWriteBytes(EZAPP_EEPROM_ADDRESS eeDst, char * pSrc, uint8_t n)
{
   while(n--)
   {
      EZAPP_EEPROM_WRITE(eeDst++, *pSrc++);
   }
}
#endif

#if !defined(EZAPP_NO_FEXT_UPDATE_SUPPORT)
static void _EZAppHandleReqUpext(void)
{
   /*
      payload of message:
      0..1: field index
      2..n: new fextended
   */   
   
   ezapp_field_index_t index;
   uint8_t len;
   
   index = _g_EZApp.packet.payload[0];
   
   _EZAppIndexToScratch(index);

   len = _g_EZApp.scr.element.fExt.len;   // & _EZAPP_MASK_STRING_MARKER;

   debug_ezapp1("_EZAppHandleReqUpext() idx%X ", index);
   debug_ezapp1("ok%X ", _EZAppExtIsWritable(index));
   debug_ezapp1("loc%X ", _g_EZApp.scr.element.fExt.ptr.loc);
  #if defined(__PCM__) || defined(__PCH__)
   debug_ezapp1("ptr%LX ", _g_EZApp.scr.element.fExt.ptr.romPtr);
  #else
   debug_ezapp1("ptr%X ", _g_EZApp.scr.element.fExt.ptr.romPtr);
  #endif
   debug_ezapp1("len%X\r\n", len);

   if (_EZAppExtIsWritable(index))
   {      
      if (_g_EZApp.scr.cfg.fType == _EZAPP_FTYPE_STRING)
      {
         // for sanity, make sure there is a null terminator at the end
         // of the string.
         _g_EZApp.packet.payload[len] = 0;
      }
     #if (EZAPP_EEPROM_SIZE > 0)
      if (bit_test(_g_EZApp.scr.element.fExt.ptr.loc, _EZ_APP_PTR_BITMAP_EE))
      {
         _EZAppEEWriteBytes(_g_EZApp.scr.element.fExt.ptr.eeAddr, &_g_EZApp.packet.payload[2], len);
      }
      else
     #endif
      {
         memcpy(_g_EZApp.scr.element.fExt.ptr.ramPtr, &_g_EZApp.packet.payload[2], len);
      }
      _g_EZApp.scr.cfg.kbhit = true;
      
      _EZAppScratchToIndex(index);
   }

   _EZAppSendExtended(_EZAPP_RES_UPEXT);
}
#endif

#if defined(_EZAPP_DO_CONNECT_TIMEOUT_CHECK)
void _EZAppConnectTimeoutCheck(void)
{
   if
   (
      BtSppIsConnected() &&
      _g_EZApp.timeoutCheck.valid &&
      ((EZAPP_TICKGET() - _g_EZApp.timeoutCheck.t) >= EZAPP_TICKS_PER_SECOND)
   )
   {
      _g_EZApp.timeoutCheck.t = EZAPP_TICKGET();
     
     #if defined(_EZAPP_REQ_MONITOR)
      if (_g_EZApp.monitor.valid)
      {
      }
      else
     #endif
      if (++_g_EZApp.timeoutCheck.seconds >= 8)
      {
         debug_ezapp0("EZAPP COMMS TIMEOUT\r\n");
         
         _g_EZApp.timeoutCheck.valid = false;
         
         BtSppInit();
         
         _EZAppPacketInit();
      }
   }
   
   if (!BtSppIsConnected())
   {
      _g_EZApp.timeoutCheck.valid = false;
   }
}

void _EZAppConnectTimeoutClear(void)
{
   _g_EZApp.timeoutCheck.valid = true;
   _g_EZApp.timeoutCheck.t = EZAPP_TICKGET();
   _g_EZApp.timeoutCheck.seconds = 0;
}
#endif

bool EZAppIsConnected(void)
{
  #if defined(_EZAPP_DO_CONNECT_TIMEOUT_CHECK)
   return(_g_EZApp.timeoutCheck.valid);
  #else
   return(_g_EZApp.connected);
  #endif
}

void EZAppTask(void)
{
   //bool repeat;

   if (_EZAppPacketTask() && BtSppIsPutReady())
   {
      /*repeat = 
      (
         (memcmp(&_g_EZApp.lastHdr, &_g_EZApp.newHdr, sizeof(_g_EZApp.lastHdr)) == 0) &&
         (!EZBlueTimerIsExpired(_g_EZApp.lastT, EZAPP_TIMER_PER_SECOND))
      );*/
      
     #if defined(__DO_DEBUG_EZAPP)
      //if (_g_EZApp.packet.cmd != _EZAPP_REQ_POLL)
      {
         debug_ezapp1("EZAppTask() CMD=%X ", _g_EZApp.packet.cmd);
         debug_ezapp1("SEQ=%X\r\n", _g_EZApp.packet.seq);
      }
     #endif
      
      _g_EZApp.packet.ready = false;
      
      //debug_printf2(debug_putc2, "EZ RX 0x%X\r\n", _g_EZApp.packet.cmd);
      
      switch(_g_EZApp.packet.cmd)
      {
         case _EZAPP_REQ_CONFIG:
#ifdef DEBUG_CFG_RESET_ON_REQ_CONFIG
            if (++g_DEBUG_CFG_RESET_ON_REQ_CONFIG >= DEBUG_CFG_RESET_ON_REQ_CONFIG)
            {
               debug_printf2(debug_putc2, "DEBUG_CFG_RESET_ON_REQ_CONFIG\r\n");
               debug_printf2(debug_putc2, "....");
               for(;;)
               {
                  reset_cpu();
               }
            }
#endif
            _EZAppMonitorAckedRes();
            _EZAppSendResConfig();
            _g_EZApp.unitConfig.pollFlags.configChanged = false;
            break;

#if !defined(EZAPP_NO_REQ_FIELD_SUPPORT)
         case _EZAPP_REQ_FIELD:
            _EZAppSendResField();
            break;
#endif
            
         case _EZAPP_REQ_POLL:
            _EZAppMonitorAckedRes();
            _EZAppSendResPoll(_EZAPP_RES_POLL);
            break;
            
         case _EZAPP_REQ_UPDATE:
            _EZAppHandleReqUdpate();
            _g_EZApp.packet.payload[2] = 1;  //change mask from req_update to num of req_poll
            _EZAppSendResPoll(_EZAPP_RES_UPDATE);
            break;
            
         case _EZAPP_REQ_EXTENDED:
            _EZAppSendExtended(_EZAPP_RES_EXTENDED);
            break;

#if !defined(EZAPP_NO_LOGIN_SUPPORT)
         case _EZAPP_REQ_LOGIN:
            _EZAppHandleReqLogin();
            break;
#endif
            
#if !defined(EZAPP_NO_FEXT_UPDATE_SUPPORT)
         case _EZAPP_REQ_UPEXT:
            _EZAppHandleReqUpext();
            break;
#endif
        
        #if defined(_EZAPP_REQ_MONITOR)
         case _EZAPP_REQ_MONITOR:
            _EZAppMonitorHandleReq();
            break;
        #endif
        
       #if defined(EZAPP_REQ_VENDOR)
         case _EZAPP_REQ_VENDOR:
            _EZAppHandleReqVendor();
            break;
        #endif
        
#if defined(_EZAPP_REQ_BOOT_CMD)
         case _EZAPP_REQ_BOOT_CMD:
            _EZAppHandleReqBootCmd();
            break;
         
         case _EZAPP_REQ_BOOT_DATA:
            _EZAppHandleReqBootData();
            break;
#endif
      }
      
      //memcpy(&_g_EZApp.lastHdr, &g_PacketRx.hdr, sizeof(_g_EZApp.lastHdr));
      
      EZAPP_IS_CONNECTED();
      
     #if defined(_EZAPP_DO_CONNECT_TIMEOUT_CHECK)
      _EZAppConnectTimeoutClear();
     #else
      _g_EZApp.connected = true;
     #endif
   }
   
   #if defined(_EZAPP_DO_CONNECT_TIMEOUT_CHECK)
    _EZAppConnectTimeoutCheck();
   #endif   
   
   _EZAppMonitorTask();
}

ezapp_field_index_t EZAppAddFieldStringDynamic(uint32_t rgb, int8_t size)
{
   ezapp_field_index_t idx;
   
   idx = EZAppAddFieldString();
   
   EZAppSetStringStyle(idx, rgb, size);
   
   return(idx);
}

ezapp_field_index_t EZAppAddFieldString(void)
{
   return(EZAppAddFieldText(0, 0));
}

ezapp_field_index_t EZAppAddFieldText(rom char * pHeader, uint8_t editSize)
{
   if (editSize)
      editSize--; //make sure there is space for the null terminator

   _EZAppCreateFieldCfg(_EZAPP_FTYPE_STRING, true, true, (pHeader != NULL), true, (editSize == 0), true);
   
   _g_EZApp.scr.element.header = pHeader;

   if (editSize != 0)
      _g_EZApp.scr.element.fConfig.b[0] |= 2;

   _EZAppSetFextStrRAM(_g_EZApp.scr.element.fExt, NULL, editSize);

   return(_EZAppAddField());
}

void EZAppSetStringStyle(ezapp_field_index_t idx, uint32_t rgb, int8_t size)
{
   _EZAppIndexToScratch(idx);
   
   _g_EZApp.scr.element.fConfig.b[0] |= 1;   //set bit0
   /*
   _g_EZApp.scr.element.fConfig.b[1] = _EZAppGetBFromRGB(rgb);
   _g_EZApp.scr.element.fConfig.b[2] = _EZAppGetGFromRGB(rgb);
   _g_EZApp.scr.element.fConfig.b[3] = _EZAppGetRFromRGB(rgb);
   */
   memcpy(&_g_EZApp.scr.element.fConfig.b[1], &rgb, 3);
   _g_EZApp.scr.element.fConfig.b[4] = size;  
   
   _EZAppScratchToIndex(idx);
}

#if (EZAPP_EEPROM_SIZE > 0)
EZAPP_EEPROM_ADDRESS EZAppGetValueStringEE(ezapp_field_index_t index)
{
   if (!_EZAppExtIsWritable(index))
      return(-1);
      
   if (_g_EZApp.scr.cfg.kbhit)
   {
      debug_ezapp1("EZAppGetValueStringEE(%X) ", index);
      debug_ezapp1("%X\r\n", _g_EZApp.scr.element.fExt.ptr.eeAddr);
      
      _g_EZApp.scr.cfg.kbhit = false;
      
      _EZAppScratchToIndex(index);
   }
   
   return(_g_EZApp.scr.element.fExt.ptr.eeAddr);
}
#endif

char * EZAppGetValueString(ezapp_field_index_t index)
{
   if (!_EZAppExtIsWritable(index))
      return(NULL);
   
   if (_g_EZApp.scr.cfg.kbhit)
   {
      debug_ezapp1("EZAppGetValueString(%X) ", index);
      debug_ezapp1("%X\r\n", _g_EZApp.scr.element.fExt.ptr.ramPtr);
      
      _g_EZApp.scr.cfg.kbhit = false;
      
      _EZAppScratchToIndex(index);
   }
   
   return(_g_EZApp.scr.element.fExt.ptr.ramPtr);
}

#define EZAPP_FVAL_MARK_STRING_CHANGED    1

static void _EZAppSetValueString(ezapp_field_index_t index, _ezapp_ptr_t ptr)
{
   _EZAppIndexToScratch(index);
   
  //#if !defined(EZAPP_STRING_NO_CHANGE_CHECK)
   //if (_EZAppPtrStrcmp(ptr, _g_EZApp.scr.element.fExt.ptr))
  //#endif
   {
      _EZ_APP_SET_FVAL(_g_EZApp.scr.element.fVal | EZAPP_FVAL_MARK_STRING_CHANGED);
   }
   
   _g_EZApp.scr.element.fExt.ptr = ptr;
   
   _EZAppScratchToIndex(index);
}

#if (EZAPP_EEPROM_SIZE > 0)
void EZAppSetValueStringEE(ezapp_field_index_t index, EZAPP_EEPROM_ADDRESS address)
{
   _ezapp_ptr_t ptr;
   
   _EZAppPtrSetEE(ptr, address);
   
   _EZAppSetValueString(index, ptr);
}
#endif

void EZAppSetValueStringROM(ezapp_field_index_t index, rom char *prString)
{
   _ezapp_ptr_t ptr;
   
   _EZAppPtrSetROM(ptr, prString);
   
   _EZAppSetValueString(index, ptr);
}

void EZAppSetValueString(ezapp_field_index_t index, char *pString)
{
   _ezapp_ptr_t ptr;
   
   _EZAppPtrSetRAM(ptr, pString);
   
   _EZAppSetValueString(index, ptr);
}

//////////////////////////////////////////////////
/// Sets the application title String to the     
/// value of 'prTitle'. The application title is 
/// always stored at index 0.                    
//////////////////////////////////////////////////
void EZAppSetTitleROM(rom char * prTitle)
{
   debug_ezapp1("EZAppSetTitleROM(pr) '%s'\r\n", prTitle);

   EZAppSetValueStringROM(0, prTitle);
}
void EZAppSetTitle(char * pTitle)
{
   debug_ezapp1("EZAppSetTitle(p) '%s'\r\n", pTitle);

   EZAppSetValueString(0, pTitle);
}

#define _EZAppAddFieldAnalogValue(l, t, m)  \
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_ANALOG, true, true, true, false, false, false);   \
   _g_EZApp.scr.element.fConfig.b[0] = t; \
   _g_EZApp.scr.element.fConfig.b[4] = m;  \
   _g_EZApp.scr.element.fConfig.b[5] = m>>8;  \
   _g_EZApp.scr.element.header = l
   
ezapp_field_index_t EZAppAddFieldAnalogValue(rom char * prLabel, ezapp_analog_type_t analogType, uint16_t maxValue)
{  
   return(EZAppAddFieldAnalogValueScaled(prLabel, analogType, 0, maxValue, 0, 0));
}

ezapp_field_index_t EZAppAddFieldAnalogValueScaled(rom char * prLabel,
   ezapp_analog_type_t analogType, 
   uint16_t minValue,
   uint16_t maxValue,
   uint16_t scaling,
   uint8_t decimalPlaces
   )
{
   _EZAppAddFieldAnalogValue(prLabel, analogType, maxValue);
   
   _g_EZApp.scr.element.fConfig.b[1] = (decimalPlaces << 1) & 0xFF; 
   _g_EZApp.scr.element.fConfig.b[2] = scaling & 0xFF;
   _g_EZApp.scr.element.fConfig.b[3] = (scaling >> 8) & 0xFF;
   _g_EZApp.scr.element.fConfig.b[6] = minValue & 0xFF;
   _g_EZApp.scr.element.fConfig.b[7] = (minValue >> 8) & 0xFF;
   
   return(_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldPulldownValue(rom char * prLabel, uint8_t numChoices, rom char * prChoices)
{
   _EZAppAddFieldAnalogValue(prLabel, EZAPP_ANALOG_TYPE_SPINNER, numChoices);
   
   if (prChoices != NULL)
   {
      _g_EZApp.scr.cfg.hasFExt = true;

      _EZAppSetFextStrROM(_g_EZApp.scr.element.fExt, prChoices);
   }

   return(_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldButtonsROM(ezapp_buttons_t cfg, rom char * prHeader, rom char * prLabels)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_BUTTON, true, true, (prHeader != NULL), (prLabels != NULL), false, false);

   _g_EZApp.scr.element.fConfig.b[0] = 0x01; //subtype
   memcpy(&_g_EZApp.scr.element.fConfig.b[1], &cfg, sizeof(cfg));

   _g_EZApp.scr.element.header = prHeader;
   
   _EZAppSetFextStrROM(_g_EZApp.scr.element.fExt, prLabels);
   
   return(_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldButtons(ezapp_buttons_t cfg, rom char * prHeader, char * prLabels)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_BUTTON, true, true, (prHeader != NULL), (prLabels != NULL), false, false);

   _g_EZApp.scr.element.fConfig.b[0] = 0x01; //subtype
   memcpy(&_g_EZApp.scr.element.fConfig.b[1], &cfg, sizeof(cfg));

   _g_EZApp.scr.element.header = prHeader;
   
   _EZAppSetFextStrRAM(_g_EZApp.scr.element.fExt, prLabels, 0);
   
   return(_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldButtonTwoState(rom char * prHeader, rom char * prLabels)
{
   ezapp_buttons_t cfg;
   memset(&cfg, 0x00, sizeof(cfg));
   cfg.numButtons = 1;
   return(EZAppAddFieldButtonsROM(cfg, prHeader, prLabels));
}

void EZAppSetButtonLED(ezapp_field_index_t idx, uint8_t column, bool value)
{
   uint16_t val;
   uint16_t mask;
   
   val = EZAppGetValue(idx);
   
   column += 8;
   
  #if defined(__PCM__) || defined(__PCH__) || defined(__PCD__)
   mask = 0;
   bit_set(mask, column);
  #else
   mask = ((uint16_t)1 << column);
  #endif

   debug_ezapp1("EZAppSetButtonLED(%X,", idx);
   debug_ezapp1("%X,", column);
   debug_ezapp1("%X) ", value);
  #if defined(__PCM__) || defined(__PCH__) 
   debug_ezapp1("%LX ", mask);
   debug_ezapp1("%LX->", val);
  #else
   debug_ezapp1("%X ", mask);
   debug_ezapp1("%X->", val);
  #endif
  
   if (value)
   {
      val |= mask;
   }
   else
   {
      mask = ~mask;
      val &= mask;
   }

  #if defined(__PCM__) || defined(__PCH__) 
   debug_ezapp1("%LX\r\n", val);
  #else
   debug_ezapp1("%X\r\n", val);
  #endif

   EZAppSetValue(idx, val);
}

ezapp_field_index_t EZAppAddFieldDigitalValue(rom char * prLabel, uint8_t num)
{
   /*
   typedef struct __EZAPP_PACKED
   {
      uint8_t flags;
      uint8_t num;
      uint8_t rgbOff[3];
      uint8_t rgbOn[3];
   } _ezapp_fconfig_digital_t;
   */
   
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_DIGITAL, true, true, true, false, true, false);

   _g_EZApp.scr.element.header = prLabel;
   
   _g_EZApp.scr.element.fConfig.b[1] = num;

   return(_EZAppAddField());
}

//////////////////////////////////////////////////
/// Creates a horizontal spacer to help seperate   
/// fields on the android device GUI.           
//////////////////////////////////////////////////
ezapp_field_index_t EZAppAddFieldSpacer()
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_SPACER, false, false, false, false, true, false);

   return(_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldSpacerDynamic(uint32_t rgb, uint8_t size, rom char *prString)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_SPACER, false, true, false, true, true, false);
   
   //fconfig for this type
   /*
   typedef struct __EZAPP_PACKED
   {
      struct __EZAPP_PACKED
      {
         unsigned int useRgb: 1;
         unsigned int inForeground: 1;   //if clear, then in app color area
         unsigned int hasText: 1;
      } flags;
      uint8_t rgb[3];
      uint8_t thick;
   } _ezapp_fconfig_spacer;
   */
   
   _g_EZApp.scr.element.fConfig.b[0] = 3;
   if (prString != NULL)
      _g_EZApp.scr.element.fConfig.b[0] |= 4;
   /*
   _g_EZApp.scr.element.fConfig.b[1] = _EZAppGetBFromRGB(rgb);
   _g_EZApp.scr.element.fConfig.b[2] = _EZAppGetGFromRGB(rgb);
   _g_EZApp.scr.element.fConfig.b[3] = _EZAppGetRFromRGB(rgb);
   */
   memcpy(&_g_EZApp.scr.element.fConfig.b[1], &rgb, 3);
   _g_EZApp.scr.element.fConfig.b[4] = size;

   _EZAppSetFextStrROM(_g_EZApp.scr.element.fExt, prString);
   
   return(_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldImage(ezapp_image_cfg_t cfg, rom char *urls)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_IMAGE, true, true, false, true, true, false);
   
   uint8_t * config = (uint8_t *)&cfg; 
   _g_EZApp.scr.element.fConfig.b[0] = *config;

   _EZAppSetFextStrROM(_g_EZApp.scr.element.fExt, urls);
   
   return (_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldGraph(ezapp_graph_cfg_t *cfg)
{

   _EZAppCreateFieldCfg(_EZAPP_FTYPE_GRAPH, true, true, true, true, false, false);
   
   _g_EZApp.scr.element.header = cfg->title;
   
   ezapp_graph_cfg_internal_t *temp = (ezapp_graph_cfg_internal_t *)cfg;
   
   memcpy(_g_EZApp.scr.element.fConfig.b, temp->pack_data.fconfig, 8);

  _EZAppSetFextBytesRAMStrROM(_g_EZApp.scr.element.fExt, temp->pack_data.fext, 12, cfg->labels);
   
   return (_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldGraphSeries( 
   ezapp_graph_line_type_t type, uint32_t color, 
   uint16_t increment, rom char *name)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_GRAPH_SERIES, true, true, true, true, false, false);
   
   uint8_t configFlags = type;
   
   _g_EZApp.scr.element.fConfig.b[0] = configFlags;
   memcpy(&_g_EZApp.scr.element.fConfig.b[1], &color, 3);
   memcpy(&_g_EZApp.scr.element.fConfig.b[4], &increment, 2);
   
   _EZAppSetFextStrROM(_g_EZApp.scr.element.fExt, name);
   
   return (_EZAppAddField());
}

ezapp_field_index_t EZAppAddFieldColorPicker(rom char * prHeader)
{
   ezapp_field_index_t idx;

   _EZAppCreateFieldCfg(_EZAPP_FTYPE_COLOR_PICKER, true, false, true, false, false, false);

   _g_EZApp.scr.element.header = prHeader;

   idx = _EZAppAddField();
   
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_COLOR_PICKER_DUMMY, true, false, false, false, false, false);
   
   _EZAppAddField();

   return(idx);
}

ezapp_field_index_t EZAppAddFieldJoystick(ezapp_joystick_cfg_t *cfg, rom char * prHeader)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_JOYSTICK, true, false, true, true, false, false);

   _g_EZApp.scr.element.header = prHeader;
   
   _EZAppSetFextBytesRAM(_g_EZApp.scr.element.fExt, cfg, sizeof(ezapp_graph_cfg_t));

   return(_EZAppAddField());
}

bool EZAppGetKbhit(ezapp_field_index_t index)
{
   _EZAppIndexToScratch(index);
   
   return(_g_EZApp.scr.cfg.kbhit);
}

void EZAppAddStylesROM(rom void* pStyles, size_t bytes)
{
   _EZAppCreateFieldCfg(_EZAPP_FTYPE_STYLE, false, false, false, true, true, false);
 
   debug_ezapp1("EZAddStyle %X ", pStyles);
   debug_ezapp1("%X\r\n", bytes);
   
   _EZAppSetFextBytesROM(_g_EZApp.scr.element.fExt, pStyles, bytes);
   
   _EZAppAddField();
}

/* this function's functionality reserved for a future date */
void EZAppSetKey(uint32_t newKey)
{
   _g_EZApp.unitConfig.key = newKey;
   _EZAppMarkConfigChanged();
}


#endif
