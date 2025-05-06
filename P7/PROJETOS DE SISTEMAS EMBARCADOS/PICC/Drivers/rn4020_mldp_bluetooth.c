///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                    RN4020_MLDP_Bluetooth.c                        ////
////                                                                   ////
//// Library for configuring Bluetooth module and using it in MLDP     ////
//// mode.                                                             ////
////                                                                   ////
//// See the RN4020_MLDP_Bluetooth.h for public API prototypes and     ////
//// documentation.                                                    ////
////                                                                   ////
////                                                                   ////
//// Version History:                                                  ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
//// January 7th, 2016                                                 ////               
////  * Added BT_FRIENDLY_NAME_MAC and BT_FRIENDLY_NAME.               ////
////  * Added BT_GET_VERSION_INFO.                                     ////
////                                                                   ////
//// December 3rd, 2015                                                ////
////  * __RN4020_USE_OTA__ compile time macro is defined to turn on    ////
////     over-the-air (OTA) DFU updates of firmware on the RN4020.     ////
////     This is not enabled by default.  If enabled, you can use      ////
////     the iPhone/iPad "Bluetooth Smart Data" App from Microchip     ////
////     to perform the OTA.                                           ////
////                                                                   ////
//// October 30th, 2015                                                ////
////  * PIN_BT_MLDP_CONNECTED is now required.  This is used to        ////
////     prevent BtSppPutc() from sending what it thinks is MLDP data  ////
////     when it might be accidentally sending sending CMD data.       ////
////  * On init, send unbond command to erase any accidental bondings. ////
////  * On init, SR configuration changed to prevent bondings saved    ////
////     to the RN4020's NVM.                                          ////
////  * __RN4020_USE_MLDPV2__ compile time macro is defined to turn    ////
////     on MLDPv2.  This requires an RN4020 with 1.20 firmware or     ////
////     later.  MLDPv2 has higher throughput but offers no data       ////
////     transmission gaurantee, so any app/protocol using MLPDv2 must ////
////     provide a retry mechanism.  This feature is not enabled by    ////
////     default.                                                      ////
////                                                                   ////
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

#ifndef __BTSPP_C__
#define __BTSPP_C__

//#define __RN4020_USE_MLDPV2__
//#define __RN4020_USE_OTA__
//#define __RN4020_FORCE_INIT__

/*
   Android 5 and up has a problem with names 9 chars or longer.
   Names with 9 chars or longer work fine on Android 4 or older or iOS just fine, but on 
   Android 5.0 the device usually cannot be found.
   That means the max string length that can be passed to BT_FRIENDLY_NAME or BtSppCommandSetName is 8 characters.
   That means the max string length that can be passed to BT_FRIENDLY_NAME_MAC or BtSppCommandSetNameWithMAC is 4 characters.
   
*/
#warning Android 5 and up has a problem with names 9 chars or longer.  See above comments.

#include "RN4020_MLDP_Bluetooth.h"

#ifndef PIN_BT_MLDP_CMD_MODE
   #define PIN_BT_MLDP_CMD_MODE PIN_D0
#endif

#if (STREAM_BT_SPP_UART==1)
   #define UARTBtInit    UART1Init
   #define UARTBtPutc    UART1Putc
   #define UARTBtGetc    UART1Getc
   #define UARTBtKbhit   UART1Kbhit
   #define UARTBtPuts    UART1Puts
   #define UARTBtPutsROM UART1PutsROM
   #include "UART1.c"
#elif (STREAM_BT_SPP_UART==2)
   #define UARTBtInit    UART2Init
   #define UARTBtPutc    UART2Putc
   #define UARTBtGetc    UART2Getc
   #define UARTBtKbhit   UART2Kbhit
   #define UARTBtPuts    UART2Puts
   #define UARTBtPutsROM UART2PutsROM
   #include "UART2.c"
#elif (STREAM_BT_SPP_UART==3)
   #define UARTBtInit    UART3Init
   #define UARTBtPutc    UART3Putc
   #define UARTBtGetc    UART3Getc
   #define UARTBtKbhit   UART3Kbhit
   #define UARTBtPuts    UART3Puts
   #define UARTBtPutsROM UART3PutsROM
   #include "UART3.c"
#elif (STREAM_BT_SPP_UART==4)
   #define UARTBtInit    UART4Init
   #define UARTBtPutc    UART4Putc
   #define UARTBtGetc    UART4Getc
   #define UARTBtKbhit   UART4Kbhit
   #define UARTBtPuts    UART4Puts
   #define UARTBtPutsROM UART4PutsROM
   #include "UART4.c"
#endif

#if !defined(__DO_DEBUG_MLDP)
   #define debug_mldp0(a)
   #define debug_mldp1(a,b)
#else
   #define debug_mldp0(a)                debug_printf0(a)
   #define debug_mldp1(a,b)              debug_printf1(a,b)
#endif

rom char _Rn4020CmdStr[] = "CMD\r\n";
rom char _Rn4020AokStr[] = "AOK\r\n";

// returns TRUE if ok
static bool _Rn4020GetString(char *str, uint8_t max, uint16_t timeoutMs)
{
   char c;
   
   max--;   //save space for null
   
   while(timeoutMs--)
   {
      while (BtSppKbhit())
      {
         c = BtSppGetc();
         
         if (c == '\r')
         {
            //skip
         }
         else if (c == '\n')
         {
            *str = 0;
            return(true);
         }
         else if (max != 0)
         {
            max--;
            *str++ = c;
         }
      }
   
      delay_ms(1);
   }
   
   *str = 0;
   
   return(false);
}

// returns TRUE if found
static bool _Rn4020WaitForString(rom char * romExpectString, uint16_t timeoutMs)
{
   rom char * pCurr;
   char c;
   
   pCurr = romExpectString;
   
   while(timeoutMs--)
   {
      while (BtSppKbhit())
      {
         c = BtSppGetc();

         if (c != *pCurr)
         {
            pCurr = romExpectString;
         }
         else
         {
            pCurr++;
            if (*pCurr == 0)
            {
               debug_mldp0("RN4020 WAIT: OK\r\n");
               return(true);
            }
         }
      }

      delay_ms(1);
   }

   debug_mldp0("RN4020 WAIT: ERROR\r\n");
 
   return(false);
}

static void _BtSppSwitchDelay(void)
{
   delay_ms(500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppInit(void)
{
   bool ok;
   int tries=0;

  #if defined(PIN_BT_RTS)
   output_low(PIN_BT_RTS);
   output_drive(PIN_BT_RTS);
  #endif
  
   output_low(PIN_BT_MLDP_CMD_MODE);
   output_drive(PIN_BT_MLDP_CMD_MODE);

   output_low(PIN_BT_SWAKE);
   output_drive(PIN_BT_SWAKE);

   delay_ms(100);

   UARTBtInit(115200);
  
   output_high(PIN_BT_SWAKE);

   debug_mldp0("BtSppInit() wait for cmd...\r\n");
  
   ok = _Rn4020WaitForString(_Rn4020CmdStr, 2000);
   
   ///// enable mldp, make unit discoverable.
   ///// try it twice
#if !defined(__RN4020_FORCE_INIT__)   
   if (ok)
#endif
   {
      while (tries < 2)
      {
         debug_mldp0("BtSppInit() send SR\r\n");
         
         // this is what was being used for a while
         //UARTBtPutsROM((rom char *)"SR,3a004c00\r\n");
         
         // this sets the 'do not save bonding' bit
        #if defined(__RN4020_VARIABLE_SR__)
         printf(UARTBtPutc, "SR,%08LX\r\n", (unsigned int32)__RN4020_VARIABLE_SR__);
        #else
        #if defined(__RN4020_USE_MLDPV2__)
         #if defined(__RN4020_USE_OTA__)
          UARTBtPutsROM((rom char *)"SR,3a10ee00\r\n");
         #else
          UARTBtPutsROM((rom char *)"SR,3a106e00\r\n");
         #endif
        #else
         #if defined(__RN4020_USE_OTA__)
          UARTBtPutsROM((rom char *)"SR,3a10ec00\r\n");
         #else
          UARTBtPutsROM((rom char *)"SR,3a106c00\r\n");
         #endif
        #endif
        #endif
         
         ok = _Rn4020WaitForString(_Rn4020AokStr, 500);

#if !defined(__RN4020_FORCE_INIT__)         
         if (ok)
#endif
            break;
      }
   }
   
   /// delete any bondings
#if !defined(__RN4020_FORCE_INIT__)
   if (ok)
#endif
   {
      debug_mldp0("BtSppInit() send U\r\n");
      
      UARTBtPutsROM((rom char *)"U\r\n");
   
      // don't bother waiting for an OK, since it will respond with ERR if not bonded
      delay_ms(250);
   }
   
   ///// iOS required timing
#if !defined(__RN4020_FORCE_INIT__)
   if (ok)
#endif
   {
      debug_mldp0("BtSppInit() send ST\r\n");
      
      UARTBtPutsROM((rom char *)"ST,0010,0000,0064\r\n");
   
      ok = _Rn4020WaitForString(_Rn4020AokStr, 500);
   }
   
  #if defined(BT_GET_VERSION_INFO)
#if !defined(__RN4020_FORCE_INIT__)
   if (ok)
#endif
   {
      UARTBtPutsROM((rom char *)"V\r\n");
      ok = _Rn4020GetString(BT_GET_VERSION_INFO, BT_MAX_VERSION_INFO, 500);
      
      debug_mldp1("Get RN4020 version: '%s'\r\n", BT_GET_VERSION_INFO);
   }
  #endif
  
  #if defined(BT_FRIENDLY_NAME) || defined(BT_FRIENDLY_NAME_MAC)
#if !defined(__RN4020_FORCE_INIT__)
   if (ok)
#endif
   {
     #if defined(BT_FRIENDLY_NAME)
      BtSppCommandSetNameROM(BT_FRIENDLY_NAME);
     #endif
     
     #if defined(BT_FRIENDLY_NAME_MAC)
      BtSppCommandSetNameWithMACROM(BT_FRIENDLY_NAME_MAC);
     #endif
   }
  #endif
  
   ////// reset unit, so the SR command takes effect.
#if !defined(__RN4020_FORCE_INIT__)
   if (ok)
#endif      
   {
      debug_mldp0("BtSppInit() send R\r\n");
      
      UARTBtPutsROM((rom char *)"R,1\r\n");
   
      ok = _Rn4020WaitForString(_Rn4020CmdStr, 2000);
   }
   
   output_high(PIN_BT_MLDP_CMD_MODE);
}

// see RN4020_MLDP_Bluetooth.h for documentation
inline bool BtSppKbhit(void)
{
   return(UARTBtKbhit());
}

// see RN4020_MLDP_Bluetooth.h for documentation
inline char BtSppGetc(void)
{
   return(UARTBtGetc());
}

// see RN4020_MLDP_Bluetooth.h for documentation
inline void BtSppPutc(char c)
{
   if (BtSppIsConnected())
   {
      UARTBtPutc(c);
      
      //Right now, this is needed to send messages >= 120 bytes.
      delay_us(800);

   }
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandEnter(void)
{
   output_low(PIN_BT_MLDP_CMD_MODE);
   output_drive(PIN_BT_MLDP_CMD_MODE);
   _BtSppSwitchDelay();
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandExit(void)
{
   output_high(PIN_BT_MLDP_CMD_MODE);
   output_drive(PIN_BT_MLDP_CMD_MODE);
   _BtSppSwitchDelay();
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetName(char *str)
{
   UARTBtPutsROM((rom char *)"SN,");
   UARTBtPuts(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetNameROM(rom char *str)
{
   UARTBtPutsROM((rom char *)"SN,");
   UARTBtPutsROM(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetNameWithMAC(char *str)
{
   UARTBtPutsROM((rom char *)"S-,");
   UARTBtPuts(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

// see RN4020_MLDP_Bluetooth.h for documentation
void BtSppCommandSetNameWithMACROM(rom char *str)
{
   UARTBtPutsROM((rom char *)"S-,");
   UARTBtPutsROM(str);
   UARTBtPutsROM((rom char *)"\r");
   
   _Rn4020WaitForString(_Rn4020AokStr, 500);
}

#if defined(PIN_BT_SPP_CONNECTED) && !defined(PIN_BT_MLDP_CONNECTED)
   // original driver and examples would use the pin label 
   // PIN_BT_SPP_CONNECTED, but we later switched it to PIN_BT_MLDP_CONNECTED.
   // this check and define will keep old code backwards compatible with
   // the updated library.
   #define PIN_BT_MLDP_CONNECTED PIN_BT_SPP_CONNECTED
#endif

// see RN4020_MLDP_Bluetooth.h for documentation
inline bool BtSppIsConnected(void)
{
   return(input(PIN_BT_MLDP_CONNECTED));
}

#endif
