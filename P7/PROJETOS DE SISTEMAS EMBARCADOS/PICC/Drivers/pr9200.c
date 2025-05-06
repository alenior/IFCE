///////////////////////////////////////////////////////////////////////////
////                              pr9200.c                             ////
////                                                                   ////
////    Driver for RED5 UHF RFID Reader module containing a PR9200.    ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// API: - see comments before each function for a more detailed      ////
////        description about function and it's parameters and return  ////
////        value.                                                     ////
////                                                                   ////
////  pr9200_init() - initializes RED5 module, call before any other   ////
////                  function.                                        ////
////                                                                   ////
////  pr9200_rx_task() - task function for receiving messages from the ////
////                     RED5 module.                                  ////
////                                                                   ////
////  pr9200_tx_task() - task function for transmitting messages to    ////
////                     the RED5 module.                              ////
////                                                                   ////
////  pr9200_has_rx_data() - function to check if there are any        ////
////                         messages if RX buffer to get.             ////
////                                                                   ////
////  pr9200_get_message() - function to receive a message from RX     ////
////                         buffer.                                   ////
////                                                                   ////
////  pr9200_put_message() - function to load a message to transmit    ////
////                         into TX buffer.                           ////
////                                                                   ////
//// Command Functions:                                                ////
////                                                                   ////
////  GetReaderInfo() - function to info about RED5 module.            ////
////                                                                   ////
////  GetRegion() - gets the region used by RED5 module.               ////
////                                                                   ////
////  SetRegion() - sets the region used by RED5 module.               ////
////                                                                   ////
////  SetSystemReset() - resets the RED5 module.                       ////
////                                                                   ////
////  GetTypeC_AI_SelectParameters() - gets the 18000-6C air interface ////
////                                   protocol select parameters.     ////
////                                                                   ////
////  SetTypeC_AI_SelectParameters() - sets the 18000-6C air interface ////
////                                   protocol select parameters.     ////
////                                                                   ////
////  GetTypeC_AI_QueryParameters() - gets the 18000-6C air interface  ////
////                                  protocol query parameters.       ////
////                                                                   ////
////  SetTypeC_AI_QueryParameters() - sets the 18000-6C air interface  ////
////                                  protocol query parameters.       ////
////                                                                   ////
////  GetRFChannel() - gets RED5 module current RF channel.            ////
////                                                                   ////
////  SetRFChannel() - sets RED5 module current RF channel.            ////
////                                                                   ////
////  GetFH_LBT_Parameters() - gets RED5 module FH and LBT parameters. ////
////                                                                   ////
////  SetFH_LBT_Parameters() - sets RED5 module FH and LBT parameters. ////
////                                                                   ////
////  GetTxPowerLevel() - gets RED5 module TX power level.             ////
////                                                                   ////
////  SetTxPowerLevel() - sets RED5 module TX power level.             ////
////                                                                   ////
////  TurnCWSignalOnOff() - turns the Continuous Wave (CW) signal      ////
////                        on/off.                                    ////
////                                                                   ////
////  ReadTypeC_UII() - function to read UII from a Type C Tag, the    ////
////                    EPC bank (PC + EPC).                           ////
////                                                                   ////
////  ReadTypeC_UII_TID() - function to start read Tag UII (PC + EPC)  ////
////                        and TID memory banks.  Read values         ////
////                        received as notification messages.         ////
////                                                                   ////
////  ReadTypeCTagData() - function to read data from a Tag.           ////
////                                                                   ////
////  ReadTypeCTagDataLong() - function to start a long read from a    ////
////                           Tag.  Read data is returned as          ////
////                           notification messages in 128 word       ////
////                           chunks.                                 ////
////                                                                   ////
////  GetSession() - gets current session of RED5 module.              ////
////                                                                   ////
////  SetSession() - sets current session of RED5 module.              ////
////                                                                   ////
////  GetFrequencyHoppingTable() - gets frequency hopping table of     ////
////                               RED5 module.                        ////
////                                                                   ////
////  SetFrequencyHoppingTable() - sets frequency hopping table of     ////
////                               RED5 module.                        ////
////                                                                   ////
////  GetModulationMode() - gets current modulation mode of RED5       ////
////                        module.                                    ////
////                                                                   ////
////  SetModulationMode() - sets current modulation mode of RED5       ////
////                        module.                                    ////
////                                                                   ////
////  GetAntiCollisionMode() - gets anticollision mode of RED5 module. ////
////                                                                   ////
////  SetAntiCollisionMode() - sets anticollision mode of RED5 module. ////
////                                                                   ////
////  StartAutoRead() - starts auto read of Tag UII (PC + EPC).  Read  ////
////                    values are returned as notification messages.  ////
////                                                                   ////
////  StartAutoRead_RSSI() - starts auto read of Tag UII (PC + EPC)    ////
////                         with tag's RSSI level.  Read values are   ////
////                         returned as notification messages.        ////
////                                                                   ////
////  StopAutoRead() - stops any auto reads that are in progress.      ////
////                                                                   ////
////  StartAutoReadAntenna() - starts auto read of Tag UII (PC + EPC)  ////
////                           with parameter to select antennas to    ////
////                           use.  Only available if RED5 module is  ////
////                           connected directly to sky13148 antenna  ////
////                           switch.                                 ////
////                                                                   ////
////  WriteTypeCTagDataWithCode() - write data to tag, has parameter   ////
////                                to select which write command code ////
////                                to use.  See WriteTypeCTagData()   ////
////                                and WriteTypeCTagDataBlock()       ////
////                                wrapper functions.                 ////
////                                                                   ////
////  WriteTypeCTagData() - wrapper function for                       ////
////                        WriteTypeCTagDataWithCode() function.      ////
////                                                                   ////
////  WriteTypeCTagDataBlock() - wrapper function for                  ////
////                             WriteTypeCTagDataWithCode() function. ////
////                                                                   ////
////  BlockEraseTypeCTagData() - function to erase data on tag.        ////
////                                                                   ////
////  BlockPermalockTypeCTag() - function to permanently lock data on  ////
////                             tag.                                  ////
////                                                                   ////
////  KillTypeCTag() - function to kill a tag.                         ////
////                                                                   ////
////  LockTypeCTag() - function to lock/unlock memory regions on tag,  ////
////                   whether access password is required to access   ////
////                   memory region.                                  ////
////                                                                   ////
////  AntennaCheck() - function to check if antenna is good.           ////
////                                                                   ////
////  GetTemperature() - function to return temperature of RED5        ////
////                     module.                                       ////
////                                                                   ////
////  GetRSSI() - function to return RSSI level of current channel.    ////
////                                                                   ////
////  ScanRSSI() - function to perform an RSSI scan of all channels.   ////
////               Number of channel depends on region setting of RED5 ////
////               module.                                             ////
////                                                                   ////
////  GetDTCResult() - function to get the DTC result.                 ////
////                                                                   ////
////  UpdateRegistry() - function to store data in registry.           ////
////                                                                   ////
////  GetRegisteryItem() - function to get register item.              ////
////                                                                   ////
////  SetOptimumFrequencyHoppingTable() - function to set the optimum  ////
////                                      frequency hopping table.     ////
////                                                                   ////
////  GetFrequencyHoppingMode() - gets the frequency hopping mode of   ////
////                              RED5 module.                         ////
////                                                                   ////
////  SetFrequencyHoppingMode() - sets the frequency hopping mode of   ////
////                              RED5 module.                         ////
////                                                                   ////
////  GetTxLeakageRSSILevelForSmartHoppingMode() - gets TX leakage     ////
///         RSSI level for smart hopping mode, the reference value     ////
///         used when doing SetOptimumFrequencyHoppingTable().         ////
////                                                                   ////
////  SetTxLeakageRSSILevelForSmartHoppingMode() - sets TX leakage     ////
///         RSSI level for smart hopping mode, the reference value     ////
///         used when doing SetOptimumFrequencyHoppingTable().         ////
////                                                                   ////
////  StartReadWithFastLeakageCal() - start Read with Fast Leakage     ////
////                                  Cal.                             ////
////                                                                   ////
////  AuthenticateDNATag() - function used to authenticate a DNA tag.  ////
////                                                                   ////
////  UntraceableDNATagSettings() - function used to set the           ////
////        untraceable settings of a DNA tag.                         ////
////                                                                   ////
//// Helper Functions:                                                 ////
////                                                                   ////
////  SetPassword() - function for setting the tag's Kill and Access   ////
////                  passwords.                                       ////
////                                                                   ////
////  ReadPassword() - function for reading the tag's Kill and Access  ////
////                   passwords.                                      ////
////                                                                   ////
////  DetectTags() - function for detecting tags.                      ////
////                                                                   ////
////  DetectTagsAntenna() - function for detecting tags with multiple  ////
////                        antennas.  This function works when either ////
////                        RED5 is connected to antenna switch or the ////
////                        PIC is connected to antenna switch.        ////
////                                                                   ////
////  LockTagMemoryRegion() - function for locking or unlocking tag    ////
////                          memory regions, whether the access       ////
////                          password is needed to access memory.     ////
////                                                                   ////
////  PermaLockTagMemoryRegion() - function for permanently locking or ////
////                               unlocking memory regions, whether   ////
////                               the access password is needed to    ////
////                               access memory.  If this function    ////
////                               is called for one or more memory    ////
////                               regions the setting can't be        ////
////                               undone.                             ////
////                                                                   ////
////  SwapBytes() - function of swapping the bytes of 16 bit and 32    ////
////                bit values.                                        ////
////                                                                   ////
////  CalculateTagRSSI() - function for calculating the Tag's RSSI     ////
////                       level, function accepts TAG_RSSI structure  ////
////                       and returns it as a float32 value.          ////
////                                                                   ////
////  CheckIfNewTag() - function used to compare a Tag's EPC to an     ////
////                    and array of Tag EPC values to deter mime if   ////
////                    it is new.                                     ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// Defines:                                                          ////
////                                                                   ////
////  PR9200_TX_PIN - The PIC TX pin, defaults to PIN_B6 if not        ////
////                  defined.  If PIC has remappable peripheral pins  ////
////                  this pin should be assigned as the UARTx, x      ////
////                  being UART peripheral being used, TX pin.        ////
////                                                                   ////
////  PR9200_RX_PIN - The PIC RX pin, default to PIN_C10 if not        ////
////                  defined.  If PIC has remappable peripheral pins  ////
////                  this pin should be assigned as the UARTx, x      ////
////                  being UART peripheral being used, RX pin.        ////
////                                                                   ////
////  PR9200_EN_PIN - The PIC pin connected to the RED5's ENABLE pin,  ////
////                  optional no default assignment is made.          ////
////                                                                   ////
////  PR9200_BAUD_RATE - The UART baud rate to use, default to 115200  ////
////                     if not defined.                               ////
////                                                                   ////
////  PR9200_RX_BUFFER_SIZE - Number of receive buffers to use,        ////
////                          defaults to 3 if not defined.            ////
////                                                                   ////
////  PR9200_TX_BUFFER_SIZE - Number of transmit buffer to use,        ////
////                          default to 3 if not defined.             ////
////                                                                   ////
////  PR9200_USE_DYNAMIC_MEMORY - Specifies whether to use dynamic     ////
////                              memory allocation, malloc(),         ////
////                              defaults to TRUE if not defined.     ////
////                                                                   ////
////  PR9200_MAX_PAYLOAD_SIZE - If PR9200_USE_DYNAMIC_MEMORY is FALSE, ////
////                            sets max payload size of TX and RX     ////
////                            messages.  Default to 256 bytes if not ////
////                            defined.                               ////
////                                                                   ////
////  PR9200_FORCE_SW_CRC - Used to select whether driver uses PIC's   ////
////                        HW CRC generator to calculate the messages ////
////                        CRC16 value.  If not defined it defaults   ////
////                        to FALSE if PIC has a HW CRC generator,    ////
////                        otherwise if defaults to TRUE.  This is    ////
////                        only needs to be used if PIC has a HW CRC  ////
////                        and you want to force code to use SW CRC.  ////
////                                                                   ////
////  PR9200_UART_RX_BUFFER_SIZE - Sets the size of the internal UART  ////
////                               receive buffer, defaults to 256 if  ////
////                               not defined.                        ////
////                                                                   ////
////  PR9200_UART_TX_BUFFER_SIZE - Sets the size of the internal UART  ////
////                               transmit buffer, default to 256 if  ////
////                               not defined.                        ////
////                                                                   ////
////  PR9200_USE_EXTERNAL_ANT_SWITCH - If using multiple antennas used ////
////                                   to specify if antenna switch    ////
////                                   pins are connected to RED5      ////
////                                   module or PIC.  TRUE connected  ////
////                                   to PIC, FALSE connected to RED5 ////
////                                   module.  Defaults to FALSE if   ////
////                                   not defined.                    ////
////                                                                   ////
////  PR9200_RESTART_WDT - Sets whether the pr9200_get_response()      ////
////                       restarts the WDT before the while() loop    ////
////                       in the function.  TRUE restart WDT, FALSE   ////
////                       don't restart WDT.  This won't cause it to  ////
////                       get stuck in an infinite loop, it's primary ////
////                       use is so that if multiple functions are    ////
////                       called back to back it will keep the wait   ////
////                       time from accumulating to a point were the  ////
////                       WDT will reset the PIC.                     ////
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

#ifndef __PR9200_C__
#define __PR9200_C__

#include "pr9200.h"

#if PR9200_USE_EXTERNAL_ANT_SWITCH == TRUE
 #include "sky13418.c"
#endif

///////////////////////////////////// API /////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// pr9200_init()
//
// Function to initialize software buffer used to transmitting to and receiving
// from the RED5 module.  Also if the HW CRC generator is being used this 
// initializes the PIC's CRC peripheral, and if an external antenna switch is
// being used it 
//
// Parameters - None
//
// Returns - Nothing
///////////////////////////////////////////////////////////////////////////////
void pr9200_init(void)
{
  #ifdef PR9200_EN_PIN
   output_high(PR9200_EN_PIN);
   output_drive(PR9200_EN_PIN);
  #endif

  #if PR9200_FORCE_SW_CRC == FALSE
   setup_crc(16, 12, 5, 0);
  #endif
  
  #if PR9200_USE_EXTERNAL_ANT_SWITCH == TRUE
   init_antenna_switch();
  #endif
   
   memset(&g_RxMessages, 0x00, sizeof(g_RxMessages));
   memset(&g_TxMessages, 0x00, sizeof(g_TxMessages));
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_rx_task()
//
// Task function for receiving messages from the RED5 module.  This function
// can be called in main() loop for receiving response and notification 
// messages, additionally all the command functions have an option to wait for 
// a response before exiting.  When that option is used this function is also
// called by them.
//
// Parameters - None.
//
// Return - Nothing.
///////////////////////////////////////////////////////////////////////////////
void pr9200_rx_task(void)
{
   static PR9200_RX_MODE RxMode = PR9200_RX_MODE_PREAMBLE;
   static PR9200_MESSAGE_STRUCT Message;
   static UINT16_UNION Length;
   static UINT16_UNION crc16;
   static uint16_t Count;
   static uint8_t Index;
   uint8_t Data;
   
   while(kbhit(PR9200_STREAM))
   {
      Data = fgetc(PR9200_STREAM);
      
      switch(RxMode)
      {
         case PR9200_RX_MODE_PREAMBLE:
            if(Data == PR9200_MESSAGE_PREAMBLE)
            {
               RxMode++;
              #if PR9200_USE_DYNAMIC_MEMORY == TRUE
               Message.Payload = NULL;
              #endif
            }
            break;
         case PR9200_RX_MODE_MESSAGE_TYPE:
            Message.Header.Type = Data;
            RxMode++;
            break;
         case PR9200_RX_MODE_CODE:
            Message.Header.Code = Data;
            Count = 0;
            Index = 1;
            RxMode++;
            break;
         case PR9200_RX_MODE_LENGTH:
            Length.b[Index--] = Data;
            
            if(++Count >= 2)
            {
               Message.Header.Length = Length.w;
               
               if(Length.w > 0)
               {
                  RxMode++;
                  Count = 0;
                  
                 #if PR9200_USE_DYNAMIC_MEMORY == TRUE
                  Message.Payload = malloc(Length.w);
                 #endif
               }
               else
                  RxMode = PR9200_RX_MODE_END_MASK;
            }
            break;
         case PR9200_RX_MODE_PAYLOAD:
           #if PR9200_USE_DYNAMIC_MEMORY == TRUE
            if(Message.Payload != NULL)
           #else
            if(Count < PR9200_MAX_PAYLOAD_SIZE)
           #endif
               Message.Payload[Count] = Data;
            
            if(++Count >= Message.Header.Length)
               RxMode++;
            break;
         case PR9200_RX_MODE_END_MASK:
            if(Data == PR9200_MESSAGE_END_MASK)
            {
               Count = 0;
               Index = 1;
               RxMode++;
            }
            else
            {
              #if PR9200_USE_DYNAMIC_MEMORY == TRUE
               free(Message.Payload);
              #endif
               
               RxMode = PR9200_RX_MODE_PREAMBLE;
            }
            break;
         case PR9200_RX_MODE_CRC16:
            crc16.b[Index--] = Data;
            
            if(++Count >= 2)
            {
               Message.Crc = crc16.w;
            
               if(pr9200_calculate_crc16(&Message, TRUE) == TRUE)
               {
                  if(g_RxMessages.Count < PR9200_RX_BUFFER_SIZE)
                  {
                     memcpy(&g_RxMessages.Message[g_RxMessages.NextIn], &Message, sizeof(PR9200_MESSAGE_STRUCT));
                     
                     if(++g_RxMessages.NextIn >= PR9200_RX_BUFFER_SIZE)
                        g_RxMessages.NextIn = 0;
                     
                     g_RxMessages.Count++;
                  }
               }
               else
               {
                 #if PR9200_USE_DYNAMIC_MEMORY == TRUE
                  free(Message.Payload);
                 #endif
               }
               
               RxMode = PR9200_RX_MODE_PREAMBLE;
            }
            break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_tx_task()
//
// Task function that actually moves the messages to transmit in to the UART's
// transmit buffer.  This function can be calling in main() loop to transmit
// buffered messages, additionally all the command functions have an option to
// wait for a response before exiting.  When that option is used this function
// is also called by them.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void pr9200_tx_task(void)
{
   PR9200_MESSAGE_STRUCT *Ptr;
  #if PR9200_UART_TX_BUFFER_SIZE > 0
   uint16_t TxBytes, Count;
  #endif
   uint16_t pLength;
   uint16_t Index;

   if(g_TxMessages.Count > 0)
   {
      Ptr = &g_TxMessages.Message[g_TxMessages.NextOut];
      pLength = Ptr->Header.Length;
      
     #if PR9200_UART_TX_BUFFER_SIZE > 0
      TxBytes = pLength + 8;
      
      Count = 0;
      Index = 0;
      
      while(Count < TxBytes)
      {
         if(tx_buffer_full(PR9200_STREAM) == FALSE)
         {
            if(Count == 0)
               fputc(PR9200_MESSAGE_PREAMBLE, PR9200_STREAM);
            else if(Count == 1)
               fputc(Ptr->Header.Type, PR9200_STREAM);
            else if(Count == 2)
               fputc(Ptr->Header.Code, PR9200_STREAM);
            else if(Count == 3)
               fputc(make8(pLength, 1), PR9200_STREAM);
            else if(Count == 4)
               fputc(make8(pLength, 0), PR9200_STREAM);
            else if(Count < (pLength + 5))
               fputc(Ptr->Payload[Index++], PR9200_STREAM);
            else if(Count == (pLength + 5))
               fputc(PR9200_MESSAGE_END_MASK, PR9200_STREAM);
            else if(Count == (pLength + 6))
               fputc(make8(Ptr->Crc, 1), PR9200_STREAM);
            else
               fputc(make8(Ptr->Crc, 0), PR9200_STREAM);
            
            Count++;
         }
      }
     #else
      fputc(PR9200_MESSAGE_PREAMBLE, PR9200_STREAM);
      fputc(Ptr->Header.Type, PR9200_STREAM);
      fputc(Ptr->Header.Code, PR9200_STREAM);
      fputc(make8(pLength, 1), PR9200_STREAM);
      fputc(make8(pLength, 0), PR9200_STREAM);
      for(Index=0;Index<pLength;Index++)
         fputc(Ptr->Payload[Index], PR9200_STREAM);
      fputc(PR9200_MESSAGE_END_MASK, PR9200_STREAM);
      fputc(make8(Ptr->Crc, 1), PR9200_STREAM);
      fputc(make8(Ptr->Crc, 0), PR9200_STREAM);
     #endif
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Ptr->Payload);
     #endif
      
      if(++g_TxMessages.NextOut >= PR9200_TX_BUFFER_SIZE)
         g_TxMessages.NextOut = 0;
      
      g_TxMessages.Count--;
   }
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_has_rx_data()
//
// Function to check if there are any response or notification messages in RX
// buffer to retrieve.  This function can be calling in main() loop to check
// for buffered messages, additionally all the command functions have an option
// to wait for a response before exiting.  When that option is used this 
// function is also called by them.
//
// Parameters - None.
//
// Returns - TRUE is there is at least one message to retrieve, FALSE if there
//           are no messages to retrieve.
///////////////////////////////////////////////////////////////////////////////
int1 pr9200_has_rx_data(void)
{
   if(g_RxMessages.Count > 0)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_get_message()
//
// Function used to retrieve a message from the RX buffer.  This function can 
// be calling in main() loop to retrieve a buffered messages, additionally all
// the command functions have an option to wait for a response before exiting.
// When that option is used this function is also called by them.
//
// Parameters:
//    Type - pointer to PR9200_MESSAGE_TYPE indicating type of message retrieved.
//
//    Code - pointer to PR9200_MESSAGE_CODE indicating Code for message retrieved.
//
//    Payload - pointer to uint8_t array to store the message's payload to.
//
//    Length - pointer to uint16_t variable to save the message's payload length to.
//
// Returns - TRUE if a message was retrieved, FALSE if there was not message to
//           retrieve.
///////////////////////////////////////////////////////////////////////////////
int1 pr9200_get_message(PR9200_MESSAGE_TYPE *Type, PR9200_MESSAGE_CODE *Code, uint8_t *Payload, uint16_t *Length)
{
   PR9200_MESSAGE_STRUCT *Ptr;
   uint16_t mLength;

   if(pr9200_has_rx_data())
   {
      Ptr = &g_RxMessages.Message[g_RxMessages.NextOut];
   
      *Type = Ptr->Header.Type;
      *Code = Ptr->Header.Code;
      mLength = Ptr->Header.Length;
      memcpy(Payload, Ptr->Payload, mLength);
      *Length = mLength;
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Ptr->Payload);
     #endif
     
      if(++g_RxMessages.NextOut >= PR9200_RX_BUFFER_SIZE)
         g_RxMessages.NextOut = 0;
     
      g_RxMessages.Count--;
      
      return(TRUE);
   }
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_put_message()
//
// Function to load a message to transmit into transmit buffer.
//
// Parameters:
//    Code - PR9200_MESSAGE_CODE of message being sent.
//
//    Payload - pointer to uint8_t array containing payload of message to send.
//
//    Length - Length of message payload.
//
// Returns - TRUE if message was successfully loaded into transmit buffer, FALSE
//           if message wasn't loaded into transmit buffer.  Possible causes for
//           failure are TX Buffer was full, if using dynamic memory function 
//           couldn't malloc memory for buffer, if using static memory payload
//           length was greater then PR9200_MAX_PAYLOAD_SIZE or failed CRC 
//           calculation.
///////////////////////////////////////////////////////////////////////////////
int1 pr9200_put_message(PR9200_MESSAGE_CODE Code, uint8_t *Payload, uint16_t Length)
{
   int1 Result;
   PR9200_MESSAGE_STRUCT *Ptr;
   
   if(g_TxMessages.Count < PR9200_TX_BUFFER_SIZE)
   {
      Ptr = &g_TxMessages.Message[g_TxMessages.NextIn];
      
      Ptr->Header.Type = PR9200_MESSAGE_TYPE_COMMAND;
      Ptr->Header.Code = Code;
      Ptr->Header.Length = Length;
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      Ptr->Payload = malloc(Length);
      
      if(Ptr->Payload != NULL)
     #else
      if(Length <= PR9200_MAX_PAYLOAD_SIZE)
     #endif
      {
         memcpy(Ptr->Payload, Payload, Length);
         
         if(pr9200_calculate_crc16(Ptr, FALSE))
         {  
            if(++g_TxMessages.NextIn >= PR9200_TX_BUFFER_SIZE)
               g_TxMessages.NextIn = 0;
            
            g_TxMessages.Count++;
            
            Result = TRUE;
         }
         else
            Result = FALSE;
      }
      else
         Result = FALSE;
   }
   else
      Result = FALSE;
   
   return(Result);
}

//////////////////////////// Command Functions API ////////////////////////////

/* All of the following Command Functions have an optional Response parameter
   that can be passed to them.  When a value other then NULL is passed for 
   Response then the function will wait for a response for that command or an
   error to be return before exiting.
   
 When Response isn't NULL:
   If an error occurred the return value of the function will be zero and the 
   first three bytes of Response contains the error info, the first byte is a
   reserved value, the second byte is the code of the command that caused the
   error and the third byte is the error code, see PR9200_ERROR_CODE type in
   pr9200.h for a list of the possible error code.  The only except is if it 
   the first three bytes of Response are all 0xFF, this indicates that an
   error occurred before the command message could be transmitted to the RED5
   module.
   
   Otherwise if the return value is greater then 0 then the message was sent 
   correctly and a response was received.  In this case the return value
   indicates the number of bytes returned to Response.
   
   Because of this when Response is used the array it points to should always
   be at least 3 bytes long.
   
   If a command function has any notification messages as part of their 
   response, those messages must be handled in main() loop.
   
 When Response is NULL:
   A return value of TRUE indicate the message was loaded into the transmit
   buffer correctly, and a return value of FALSE indicates an error occurred
   before message could be loaded into transmit buffer.
   
   When Response is NULL all the response and notification messages must be 
   handled by main() loop.
*/

///////////////////////////////////////////////////////////////////////////////
// GetReaderInfo()
//
// Gets Info about the reader, RED5 module.
//
// Parameters:
//    Type - reader info to read, see PR9200_READER_INFO type in pr9200.h for 
//           types.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetReaderInfo(PR9200_READER_INFO Type, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_GET_READER_INFO, &Type, 1);
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_GET_READER_INFO, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetRegion()
// 
// Gets the region set for the RED5 module.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetRegion(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_REGION, Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetRegion()
//
// Sets the region of the RED5 module.
//
// Parameters:
//    Region - The region to set, see PR9200_REGION type in pr9200.h for a list
//             of regions that can be set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetRegion(PR9200_REGION Region, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_REGION, &Region, 1);
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_REGION, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// SetSystemReset()
//
// Set the system level reset.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetSytemReset(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_SET_SYS_RESET, Response));
}

///////////////////////////////////////////////////////////////////////////////
// GetTypeC_AI_SelectParameters()
//
// Get 18000-6C air interface protocol select parameters.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetTypeC_AI_SelectParameters(PR9200_TYPE_C_AI_SELECT_PARAMS *Response=NULL)
{
   uint16_t Length;
   
   Length = pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_TYPE_C_AI_SELECT_PARAMS, (uint8_t *)Response);
   
   if((Response != NULL) && (Length > 0))
      Response->Pointer = SwapBytes(Response->Pointer);
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// SetTypeC_AI_SelectParameters()
//
// Set 18000-6C air interface protocol select parameters.
//
// Parameters:
//    Params - Pointer to PR9200_TYPE_C_AI_SELECT_PARAMS struct containing the 
//             parameters to set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetTypeC_AI_SelectParameters(PR9200_TYPE_C_AI_SELECT_PARAMS *Params, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[sizeof(PR9200_TYPE_C_AI_SELECT_PARAMS)];
  #endif
   uint16_t Length;
   uint32_t Pointer;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(sizeof(PR9200_TYPE_C_AI_SELECT_PARAMS));
   
   if(Payload == 0)
   {
      if(Response != NULL)
         pr9200_no_tx_error(Response);
      
      return(0);
   }
  #endif
  
   Pointer = SwapBytes(Params->Pointer);
   
   memcpy(Payload, Params, sizeof(PR9200_TYPE_C_AI_SELECT_PARAMS));
   memcpy(&Payload[1], &Pointer, 4);
   
   Length = 7 + (Params->Length) / 8;
   if((Params->Length % 8) != 0)
      Length++;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_TYPE_C_AI_SELECT_PARAMS, Payload, Length);
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_TYPE_C_AI_SELECT_PARAMS, Response, &Length);
         
         if(Result)
            Result = Length;
      }
      else
         pr9200_no_tx_error(Response);
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetTypeC_AI_QueryParameters()
//
// Get 18000-6C air interface protocol query parameters.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetTypeC_AI_QueryParameters(PR9200_TYPE_C_AI_QUERY_PARAMS *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_TYPE_C_AI_QUERY_PARAMS, (uint8_t *)Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetTypeC_AI_QueryParameters()
//
// Set 18000-6C air interface protocol query parameters.
//
// Parameters:
//    Params - Pointer to PR9200_TYPE_C_AI_QUERY_PARAMS struct containing
//             parameters to set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetTypeC_AI_QueryParameters(PR9200_TYPE_C_AI_QUERY_PARAMS *Params, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_TYPE_C_AI_QUERY_PARAMS, (uint8_t *)Params, sizeof(PR9200_TYPE_C_AI_QUERY_PARAMS));
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_TYPE_C_AI_QUERY_PARAMS, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetRFChannel()
//
// Get RF channel.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetRFChannel(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_RF_CHANNEL, Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetRFChannel()
//
// Set RF Channel.
//
// Parameters:
//    ChannelNumber - Channel number to set.  Range of channel numbers depends
//                    on regional settings.
//
//    ChannelOffset - Channel number offset for miller subcarrier.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetRFChannel(uint8_t ChannelNumber, uint8_t ChannelOffset, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[2];
   uint16_t Length;
   
   Payload[0] = ChannelNumber;
   Payload[1] = ChannelOffset;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_RF_CHANNEL, Payload, 2);
    
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_RF_CHANNEL, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetFH_LBT_Parameters()
//
// Get FH and LBT parameters.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetFH_LBT_Parameters(PR9200_FH_LBT_PARAMS *Response=NULL)
{
   uint16_t Length;
   
   Length = pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_FH_LBT_PARAMS, (uint8_t *)Response);
   
   if((Response != NULL) && (Length > 0))
   {
      Response->RT = SwapBytes(Response->RT);
      Response->IT = SwapBytes(Response->IT);
      Response->CST = SwapBytes(Response->CST);
      Response->RFL = SwapBytes(Response->RFL);
   }
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// SetFH_LBT_Parameters()
//
// Set FH and LBT parameters.
//
// Parameters:
//    Params - Pointer to PR9200_FH_LBT_PARAMS struct containing parameter to set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetFH_LBT_Parameters(PR9200_FH_LBT_PARAMS *Params, uint8_t *Response=NULL)
{
   int1 Result;
   PR9200_FH_LBT_PARAMS tParams;
   uint16_t Length;
   
   memcpy(&tParams, Params, sizeof(PR9200_FH_LBT_PARAMS));
   
   tParams.RT = SwapBytes(tParams.RT);
   tParams.IT = SwapBytes(tParams.IT);
   tParams.CST = SwapBytes(tParams.CST);
   tParams.RFL = SwapBytes(tParams.RFL);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_FH_LBT_PARAMS, &tParams, sizeof(PR9200_FH_LBT_PARAMS));
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_FH_LBT_PARAMS, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetTxPowerLevel()
//
// Get current, minimum and maximum TX power levels.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetTxPowerLevel(PR9200_TX_POWER_LEVEL *Response=NULL)
{
   uint16_t Length;
   
   Length = pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_TX_POWER_LEVEL, (uint8_t *)Response);
   
   if((Response != NULL) && (Length > 0))
   {
      Response->Power = SwapBytes(Response->Power);
      Response->MinPower = SwapBytes(Response->MinPower);
      Response->MaxPower = SwapBytes(Response->MaxPower);
   }
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// SetTxPowerLevel()
//
// Set current TX power level.
//
// Parameters:
//    TxPower - TX power to set.  Passed as a tenth of a dBm, for example 200
//              set TX power to 20.0 dBm.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetTxPowerLevel(uint16_t TxPower, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[2];
   uint16_t Length;
   
   Payload[0] = make8(TxPower, 1);
   Payload[1] = make8(TxPower, 0);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_TX_POWER_LEVEL, Payload, 2);
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_TX_POWER_LEVEL, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);      
}

///////////////////////////////////////////////////////////////////////////////
// TurnCWSignalOnOff()
//
// Turn the Continuous Wave (CW) signal on/off.
//
// Parameters:
//    ON - TRUE turns CW on, FALSE turns CW off.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t TurnCWSignalOnOff(int1 On, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[1];
   uint16_t Length;
   
   if(On)
      Payload[0] = 0xFF;
   else
      Payload[0] = 0;
      
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_RF_CW_SIGNAL_CTRL, Payload, 1);
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_RF_CW_SIGNAL_CTRL, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result); 
}

///////////////////////////////////////////////////////////////////////////////
// ReadTypeC_UII()
//
// Read UII from a Type C Tag, the EPC bank (PC + EPC).  If more than one tag
// is present additional tags may be returned as notification messages.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t ReadTypeC_UII(TYPE_C_UII *Response=NULL)
{
   uint16_t Length;
   
   Length = pr9200_command_no_params(PR9200_MESSAGE_CODE_READ_TYPE_C_UII, (uint8_t *)Response);
    
   if((Response != NULL) && (Length > 0))
   {
      Response->PC = SwapBytes(Response->PC);
      Response->Length = Length - 2;
   }
   
   return(Length); 
}

///////////////////////////////////////////////////////////////////////////////
// ReadTypeC_UII_TID()
//
// Function to start read Tag UII (PC & EPC) and TID memory bank.  If the pointer
// Response isn't NULL function will wait for the response from the RED5 module,
// in this case the response is only an acknowledgment that the command was 
// received.  The actual tags that are read are returned as notification
// messages.
//
// Parameters:
//    Max - Max number of tags to read.
//
//    Time - Max time in seconds to search for tags.
//
//    RepeatCycle - Number of times reader performs inventory round.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t ReadTypeC_UII_TID(uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[4];
   uint16_t Length;
   
   Payload[0] = Max;
   Payload[1] = Time;
   Payload[2] = make8(RepeatCycle, 1);
   Payload[3] = make8(RepeatCycle, 0);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_READ_TYPE_C_UII_TID, Payload, 4);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_READ_TYPE_C_UII_TID, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// ReadTypeCTagData()
//
// Used to start a Read from a Type C Tag.  If the pointer Response isn't NULL
// function will wait for the response from the RED5 module.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII containing EPC of tag to read.
//
//    rMemory - Memory bank in Tag to read see TYPE_C_MEMORY_BANK struct 
//              in pr9200.h for description of the memory banks.
//
//    rAddress - Word address to start reading from.
//
//    rWords - Number of Words to read (Max is 128).
//
//    Password - Access Password of target memory bank, use 0x0000000 is not set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t ReadTypeCTagData(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK rMemory, uint16_t rAddress, uint16_t rWords, uint32_t Password, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[(TYPE_C_MAX_EPC_LENGTH + 11)];
  #endif
   uint16_t Length;
   
   if((rWords > 0) && (rWords <= 128))
   {
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      Payload = malloc(Tag->Length + 11);
      
      if(Payload == NULL)
      {
         if(Response != NULL) 
            pr9200_no_tx_error(Response);
         
         return(0);
      }
     #endif
      
      Length = Tag->Length;
   
      Payload[0] = make8(Password, 3);
      Payload[1] = make8(Password, 2);
      Payload[2] = make8(Password, 1);
      Payload[3] = make8(Password, 0);
      Payload[4] = make8(Length, 1);
      Payload[5] = make8(Length, 0);
      memcpy(&Payload[6], Tag->EPC, Length);
      Length += 6;
      Payload[Length++] = rMemory;
      Payload[Length++] = make8(rAddress, 1);
      Payload[Length++] = make8(rAddress, 0);
      Payload[Length++] = make8(rWords, 1);
      Payload[Length++] = make8(rWords, 0);
      
      Result = pr9200_put_message(PR9200_MESSAGE_CODE_READ_TYPE_C_TAG_DATA, Payload, Length);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_READ_TYPE_C_TAG_DATA, Response, &Length);
            
            if(Result)
               Result = Length;
         }
         else
            pr9200_no_tx_error(Response);
      }
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Payload);
     #endif
   }
   else
   {
      if(Response != NULL)
         pr9200_no_tx_error(Response);
         
      Result = FALSE;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// ReadTypeCTagDataLong()
//
// Used to start a Long Read, more then 128 words, from a Type C Tag.  If the
// pointer Response isn't NULL function will wait for the response from the
// RED5 module.  For this type of read it only initiates the long read, and if
// Response isn't NULL the response is only an acknowledge that the command was
// accepted.  The actual read data will be returned as notification messages in
// 128 word, 256 byte, chunks.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII containing EPC of tag to read.
//
//    rMemory - Memory bank in Tag to read see TYPE_C_MEMORY_BANK struct in
//              pr9200.h for description of the memory banks.
//
//    rAddress - Word address to start reading from.
//
//    rWords - Number of Words to read.
//
//    Password - Access Password of target memory bank, use 0x0000000 is not set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t ReadTypeCTagDataLong(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK rMemory, uint16_t rAddress, uint16_t rWords, uint32_t Password, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[(TYPE_C_MAX_EPC_LENGTH + 11)];
  #endif
   uint16_t Length;
   
   if(rWords > 0)
   {
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      Payload = malloc(Tag->Length + 11);
      
      if(Payload == NULL)
      {
         if(Response != NULL) 
            pr9200_no_tx_error(Response);
         
         return(0);
      }
     #endif
      
      Length = Tag->Length;
   
      Payload[0] = make8(Password, 3);
      Payload[1] = make8(Password, 2);
      Payload[2] = make8(Password, 1);
      Payload[3] = make8(Password, 0);
      Payload[4] = make8(Length, 1);
      Payload[5] = make8(Length, 0);
      memcpy(&Payload[6], Tag->EPC, Length);
      Length += 6;
      Payload[Length++] = rMemory;
      Payload[Length++] = make8(rAddress, 1);
      Payload[Length++] = make8(rAddress, 0);
      Payload[Length++] = make8(rWords, 1);
      Payload[Length++] = make8(rWords, 0);
      
      Result = pr9200_put_message(PR9200_MESSAGE_CODE_READ_TYPE_C_TAG_LONG_DATA, Payload, Length);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_READ_TYPE_C_TAG_LONG_DATA, Response, &Length);
         
            if(Result)
               Result = Length;
         }
         else
            pr9200_no_tx_error(Response);
      }
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Payload);
     #endif
   }
   else
   {
      if(Response != NULL)
         pr9200_no_tx_error(Response);
   }
   
   return(Result);         
}

///////////////////////////////////////////////////////////////////////////////
// GetSession()
//
// Get current session.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetSession(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_SESSION, Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetSession()
//
// Set current session.
//
// Parameters:
//    Session - Current session to set, see PR9200_SESSION type in pr9200.h
//              for a list of sessions that can be set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetSession(PR9200_SESSION Session, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_SESSION, &Session, 1);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_SESSION, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetFrequencyHoppingTable()
//
// Get current frequency hopping table.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetFrequencyHoppingTable(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_FREQ_HOPPING_TABLE, Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetFrequencyHoppingTable()
//
// Set current frequency hopping table.
//
// Parameters:
//    Size - Number of table entries.
//
//    Table - Pointer to uint8_t array containing frequency hopping table.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetFrequencyHoppingTable(uint8_t Size, uint8_t *Table, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE + 1];
  #endif
   uint16_t Length;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(Size + 1);
   
   if(Payload == NULL)
   {
      if(Response != NULL) 
         pr9200_no_tx_error(Response);
      
      return(0);
   }
  #endif
   
   Payload[0] = Size;
   memcpy(&Payload[1], Table, Size);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_FREQ_HOPPING_TABLE, Payload, Size + 1);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = (uint16_t)pr9200_get_response(PR9200_MESSAGE_CODE_SET_FREQ_HOPPING_TABLE, Response, &Length);
         
         if(Result)
            Result = Length;
      }
      else
         pr9200_no_tx_error(Response);
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetModulationMode()
//
// Get current modulation mode.  The modulation mode is combination RX
// modulation type and BLF.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetModulationMode(PR9200_MODULATION *Response=NULL)
{
   uint16_t Length;
   
   Length = pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_MODULATION, (uint8_t *)Response);
   
   if((Response != NULL) && (Length > 0))
      Response->BLF = SwapBytes(Response->BLF);
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// SetModulationMode()
//
// Set current modulation mode.  The modulation mode is combination RX
// modulation type and BLF.
//
// Parameters:
//    Mode - PR9200_MODULATION struct containing mode setting to set, see
//           pr9200.h for a description of PR9200_MODULATION struct and the 
//           settings it sets.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetModulationMode(PR9200_MODULATION Mode, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Mode.BLF = SwapBytes(Mode.BLF);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_MODULATION, (uint8_t *)&Mode, 4);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_MODULATION, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetAntiCollisionMode()
//
// Get Anti-collision algorithm.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetAntiCollisionMode(PR9200_ANTI_COLLISION *Response = NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_GET_ANTI_COLLISION_MODE, NULL, 0);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_GET_ANTI_COLLISION_MODE, (uint8_t *)Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error((uint8_t *)Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// SetAntiCollisionMode()
//
// Set Anti-collision algorithm.
//
// Parameters:
//    Mode - PR9200_ANTI_COLLISION struct contain parameters to set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetAntiCollisionMode(PR9200_ANTI_COLLISION Mode, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_ANTI_COLLISION_MODE, (uint8_t *)&Mode, 4);
    
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_ANTI_COLLISION_MODE, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// StartAutoRead()
//
// Start an automatic tag read operation, tag IDs are received as notification
// messages.
//
// Parameters:
//    Type - Type of tag to read, see TAG_TYPE type in pr9200.h for tag types 
//           that can be read.
//
//    Max - Maximum number of tags to read.
//
//    Time - Maximum number of seconds to search for tags.
//
//    RepeatCycle - Repeat cycle, how many times reader performs inventory 
//                  round.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t StartAutoRead(TAG_TYPE Type, uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[5];
   uint16_t Length;
   
   if((Type == TAG_TYPE_B) || (Type == TAG_TYPE_C))
   {
      Payload[0] = Type;
      Payload[1] = Max;
      Payload[2] = Time;
      Payload[3] = make8(RepeatCycle, 1);
      Payload[4] = make8(RepeatCycle, 0);
      
      Result = pr9200_put_message(PR9200_MESSAGE_CODE_START_AUTO_READ2, Payload, 5);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_START_AUTO_READ2, Response, &Length);
            
            if(Result)
               return(Length);
         }
         else
            pr9200_no_tx_error(Response);
      }
   }
   else
      pr9200_no_tx_error(Response);
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// StartAutoRead_RSSI()
//
// Start an automatic tag read operation, tag IDs with RSSI values are received
// as notification messages. 
//
// Parameters:
//    Type - Type of tag to read, see TAG_TYPE type in pr9200.h for tag types 
//           that can be read.
//
//    Max - Maximum number of tags to read.
//
//    Time - Maximum number of seconds to search for tags.
//
//    RepeatCycle - Repeat cycle, how many times reader performs inventory 
//                  round.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t StartAutoRead_RSSI(TAG_TYPE Type, uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[5];
   uint16_t Length;
   
   if((Type == TAG_TYPE_B) || (Type == TAG_TYPE_C))
   {
      Payload[0] = Type;
      Payload[1] = Max;
      Payload[2] = Time;
      Payload[3] = make8(RepeatCycle, 1);
      Payload[4] = make8(RepeatCycle, 0);
      
      Result = pr9200_put_message(PR9200_MESSAGE_CODE_START_AUTO_READ_RSSI, Payload, 5);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_START_AUTO_READ_RSSI, Response, &Length);
            
            if(Result)
               return(Length);
         }
         else
            pr9200_no_tx_error(Response);
      }
   }
   else
      pr9200_no_tx_error(Response);
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// StopAutoRead()
//
// Stops an automatic tag read operation.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t StopAutoRead(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_STOP_AUTO_READ2, Response));
}

#if PR9200_USE_EXTERNAL_ANT_SWITCH == FALSE
///////////////////////////////////////////////////////////////////////////////
// StartAutoReadAntenna()
//
// Start an automatic read operation that includes antenna port information,
// tag IDs are received as notification messages. 
//
// Parameters:
//    TagRSSI - Whether to include Tag RSSI values in notification responses,
//              TRUE - enable, FALSE - disable.
//
//    Antenna - Which antennas to use during read operation.  This is a bitwise
//              value, setting a bit enables that antenna in scan.  Bit 0 is 
//              antenna port 1, Bit 1 is antenna port 2, etc.  When multiple 
//              bits are set it will scan though the enabled antennal ports
//              in sequence.  
//
//    Max - Maximum number of tags to read.
//
//    Time - Maximum number of seconds to search for tags.
//
//    RepeatCycle - Repeat cycle, how many times reader performs inventory 
//                  round.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t StartAutoReadAntenna(int1 TagRSSI, uint8_t Antenna, uint8_t Max, uint8_t Time, uint16_t RepeatCycle, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[7];
   uint16_t Length;
   
   Payload[0] = 0xC0;
   Payload[1] = (uint8_t)TagRSSI;
   Payload[2] = Antenna;
   Payload[3] = Max;
   Payload[4] = Time;
   Payload[5] = make8(RepeatCycle, 1);
   Payload[6] = make8(RepeatCycle, 0);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_START_AUTO_READ_ANTENNA, Payload, 7);
   
   if(Response != NULL)
   {
      if(Result)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_START_AUTO_READ_ANTENNA, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// WriteTypeCTagDataWithCode()
//
// Function to write data to type C tag.  This function isn't intended to be
// called but can be, instead see WriteTypeCTagData() and 
// WriteTypeCTagDataBlock() functions which are wrapper functions for this
// function.
//
// Parameters:
//    Code - The write command code to use, can be either 
//           PR9200_MESSAGE_CODE_WRITE_TYPE_C_TAG or
//           PR9200_MESSAGE_CODE_BLOCK_WRITE_TYPE_C_TAG.
//
//    Tag - Pointer to TYPE_C_UII struct containing EPC of Tag to write.
//
//    wMemory - Memory bank in Tag to write, see TYPE_C_MEMORY_BANK type in
//              pr9200.h for list of options.
//
//    wAddress - Word address to start writing data to.
//
//    Data - Pointer to uint8_t array containing data to write.
//
//    wWords - Number of words to write to tag.
//
//    Password - Access Password of target memory bank, use 0x0000000 if not 
//               set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t WriteTypeCTagDataWithCode(PR9200_MESSAGE_CODE Code, TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK wMemory, uint16_t wAddress, uint8_t *Data, uint16_t wWords, uint32_t Password, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE + TYPE_C_MAX_EPC_LENGTH + 11];
  #endif
   uint16_t Length;
   
   if(wWords > 0)
   {
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      Payload = malloc((wWords * 2) + Tag->Length + 11);
      
      if(Payload == NULL)
      {
         if(Response != NULL) 
            pr9200_no_tx_error(Response);
         
         return(0);
      }
     #endif
      
      Length = Tag->Length;
   
      Payload[0] = make8(Password, 3);
      Payload[1] = make8(Password, 2);
      Payload[2] = make8(Password, 1);
      Payload[3] = make8(Password, 0);
      Payload[4] = make8(Length, 1);
      Payload[5] = make8(Length, 0);
      memcpy(&Payload[6], Tag->EPC, Length);
      Length += 6;
      Payload[Length++] = wMemory;
      Payload[Length++] = make8(wAddress, 1);
      Payload[Length++] = make8(wAddress, 0);
      Payload[Length++] = make8(wWords, 1);
      Payload[Length++] = make8(wWords, 0);
      memcpy(&Payload[Length], Data, wWords*2);
      Length += (wWords * 2);
      
      Result = pr9200_put_message(Code, Payload, Length);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(Code, Response, &Length);
            
            if(Result)
               Result = Length;
         }
         else
            pr9200_no_tx_error(Response);
      }
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Payload);
     #endif
   }
   else
   {
      if(Response != NULL)
         pr9200_no_tx_error(Response);
         
      Result = FALSE;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// WriteTypeCTagData()
//
// Wrapper function for WriteTypeCTagDataWithCode() function for writing data
// to tag.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of Tag to write.
//
//    wMemory - Memory bank in Tag to write, see TYPE_C_MEMORY_BANK type in
//              pr9200.h for list of options.
//
//    wAddress - Word address to start writing data to.
//
//    Data - Pointer to uint8_t array containing data to write.
//
//    wWords - Number of words to write to tag.
//
//    Password - Access Password of target memory bank, use 0x0000000 if not 
//               set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t WriteTypeCTagData(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK wMemory, uint16_t wAddress, uint8_t *Data, uint16_t wWords, uint32_t Password, uint8_t *Response=NULL)
{
   return(WriteTypeCTagDataWithCode(PR9200_MESSAGE_CODE_WRITE_TYPE_C_TAG, Tag, wMemory, wAddress, Data, wWords, Password, Response));
}

///////////////////////////////////////////////////////////////////////////////
// WriteTypeCTagDataBlock()
//
// Wrapper function for WriteTypeCTagDataWithCode() function for writing data
// to tag.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of Tag to write.
//
//    wMemory - Memory bank in Tag to write, see TYPE_C_MEMORY_BANK type in
//              pr9200.h for list of options.
//
//    wAddress - Word address to start writing data to.
//
//    Data - Pointer to uint8_t array containing data to write.
//
//    wWords - Number of words to write to tag.
//
//    Password - Access Password of target memory bank, use 0x0000000 if not 
//               set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t WriteTypeCTagDataBlock(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK wMemory, uint16_t wAddress, uint8_t *Data, uint16_t wWords, uint32_t Password, uint8_t *Response=NULL)
{
   return(WriteTypeCTagDataWithCode(PR9200_MESSAGE_CODE_BLOCK_WRITE_TYPE_C_TAG, Tag, wMemory, wAddress, Data, wWords, Password, Response));
}

///////////////////////////////////////////////////////////////////////////////
// BlockEraseTypeCTagData()
//
// Perform Block Erase on type C tag.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of Tag to erase.
//
//    eMemory - Memory bank in Tag to erase, see TYPE_C_MEMORY_BANK type in
//              pr9200.h for list of options.
//
//    eAddress - Word address to start erasing data.
//
//    eWords - Number of words to erase in tag.
//
//    Password - Access Password of target memory bank, use 0x0000000 if not 
//               set.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t BlockEraseTypeCTagData(TYPE_C_UII *Tag, TYPE_C_MEMORY_BANK eMemory, uint16_t eAddress, uint16_t eWords, uint32_t Password, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[TYPE_C_MAX_EPC_LENGTH + 11];
  #endif
   uint16_t Length;
   
   if(eWords > 0)
   {
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      Payload = malloc(Tag->Length + 11);
      
      if(Payload == NULL)
      {
         if(Response != NULL) 
            pr9200_no_tx_error(Response);
         
         return(0);
      }
     #endif
      
      Length = Tag->Length;
   
      Payload[0] = make8(Password, 3);
      Payload[1] = make8(Password, 2);
      Payload[2] = make8(Password, 1);
      Payload[3] = make8(Password, 0);
      Payload[4] = make8(Length, 1);
      Payload[5] = make8(Length, 0);
      memcpy(&Payload[6], Tag->EPC, Length);
      Length += 6;
      Payload[Length++] = eMemory;
      Payload[Length++] = make8(eAddress, 1);
      Payload[Length++] = make8(eAddress, 0);
      Payload[Length++] = make8(eWords, 1);
      Payload[Length++] = make8(eWords, 0);
      
      Result = pr9200_put_message(PR9200_MESSAGE_CODE_BLOCK_ERASE_TYPE_C_TAG, Payload, Length);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_BLOCK_ERASE_TYPE_C_TAG, Response, &Length);
            
            if(Result)
               Result = Length;
         }
         else
            pr9200_no_tx_error(Response);
      }
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Payload);
     #endif
   }
   else
   {
      if(Response != NULL)
         pr9200_no_tx_error(Response);
      
      Result = FALSE;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// BlockPermalockTypeCTag()
//
// Block Permalock type C tag.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of Tag to Permalock.
//
//    Password - Access Password of target memory bank, use 0x0000000 if not 
//               set.
//
//    pLock - TAG_PERMALOCK struct containing Permalock settings.
//
//    Mask - Pointer to uint8_t containing Permalock Mask.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t BlockPermalockTypeCTag(TYPE_C_UII *Tag, uint32_t Password, TAG_PERMALOCK pLock, uint8_t *Mask, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE];
  #endif
   uint16_t Length;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(Tag->Length + (pLock.BR * 2) + 12);
   
   if(Payload == NULL)
   {
      if(Response != NULL) 
         pr9200_no_tx_error(Response);
      
      return(0);
   }
  #else
   if((Tag->Length + (pLock.BR * 2) + 12) > PR9200_MAX_PAYLOAD_SIZE)
   {
      if(Response != NULL) 
         pr9200_no_tx_error(Response);
      
      return(0);
   }
  #endif
   
   Length = Tag->Length;

   Payload[0] = make8(Password, 3);
   Payload[1] = make8(Password, 2);
   Payload[2] = make8(Password, 1);
   Payload[3] = make8(Password, 0);
   Payload[4] = make8(Length, 1);
   Payload[5] = make8(Length, 0);
   memcpy(&Payload[6], Tag->EPC, Length);
   Length += 6;
   Payload[Length++] = 0x00;     //RFU
   Payload[Length++] = pLock.RL;
   Payload[Length++] = pLock.MB;
   Payload[Length++] = make8(pLock.BP, 1);
   Payload[Length++] = make8(pLock.BP, 0);
   Payload[Length++] = pLock.BR;
   memcpy(&Payload[Length], Mask, (uint16_t)pLock.BR * 2);
   Length += ((uint16_t)pLock.BR * 2);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_BLOCK_PERMALOCK_TYPE_C_TAG, Payload, Length);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_BLOCK_PERMALOCK_TYPE_C_TAG, Response, &Length);
         
         if(Result)
            Result = Length;
      }
      else
         pr9200_no_tx_error(Response);
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// KillTypeCTag()
//
// Kill a tag.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of Tag to kill.
//
//    Password - Kill password, can't be 0x0000000 tag will ignore command if
//               password is 0x00000000.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t KillTypeCTag(TYPE_C_UII *Tag, uint32_t Password, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[TYPE_C_MAX_EPC_LENGTH + 6];
  #endif
   uint16_t Length;
   
   if(Password != 0)
   {
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      Payload = malloc(Tag->Length + 6);
      
      if(Payload == NULL)
      {
         if(Response != NULL) 
            pr9200_no_tx_error(Response);
         
         return(0);
      }
     #endif
      
      Length = Tag->Length;
   
      Payload[0] = make8(Password, 3);
      Payload[1] = make8(Password, 2);
      Payload[2] = make8(Password, 1);
      Payload[3] = make8(Password, 0);
      Payload[4] = make8(Length, 1);
      Payload[5] = make8(Length, 0);
      memcpy(&Payload[6], Tag->EPC, Length);
      Length += 6;
      
      Result = pr9200_put_message(PR9200_MESSAGE_CODE_KILL_TYPE_C_TAG, Payload, Length);
      
      if(Response != NULL)
      {
         if(Result == TRUE)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_KILL_TYPE_C_TAG, Response, &Length);
            
            if(Result)
               Result = Length;
         }
         else
            pr9200_no_tx_error(Response);
      }
      
     #if PR9200_USE_DYNAMIC_MEMORY == TRUE
      free(Payload);
     #endif
   }
   else
   {
      if(Response != NULL)
         pr9200_no_tx_error(Response);
      
      Result = FALSE;
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// LockTypeCTag()
//
// Function used to lock or unlock the Type C Tag memory regions. This controls
// whether the access password is required to access a memory location.  For 
// the access and kill passwords when locked the access password is required 
// for reading and writing the passwords.  For the EPC, TDI and User memory
// regions when locked the access password is required for writing the memory.  
//
// When using this function the access password is required to perform the lock
// or unlock.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of tag to lock.
//
//    Password - Access password.
//
//    LD - 20 bit lock mask and action flags, the least 10 significant bits are
//         the action flags and the 10 most significant bits are the mask.
//
//                               MSB                          LSB
//                               KILL | ACCESS | EPC | TID | USER
//         Mask                   00      00      00    00    00
//         Action (Pwd/perma)     00      00      00    00    00
//
//         When a mask bit is set the corresponding action bit is set on the 
//         tag.  The most significant bit for each section sets the lock/unlock
//         bit, and the least significant bit for each section set the permanent
//         lock/unlock bits.  The permanent mask and action bits should only be
//         used to make the settings permanent.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t LockTypeCTag(TYPE_C_UII *Tag, uint32_t Password, uint32_t LD, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[TYPE_C_MAX_EPC_LENGTH + 9];
  #endif
   uint16_t Length;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(Tag->Length + 9);
   
   if(Payload == NULL)
   {
      if(Response != NULL) 
         pr9200_no_tx_error(Response);
      
      return(0);
   }
  #endif
   
   Length = Tag->Length;

   Payload[0] = make8(Password, 3);
   Payload[1] = make8(Password, 2);
   Payload[2] = make8(Password, 1);
   Payload[3] = make8(Password, 0);
   Payload[4] = make8(Length, 1);
   Payload[5] = make8(Length, 0);
   memcpy(&Payload[6], Tag->EPC, Length);
   Length += 6;
   Payload[Length++] = (make8(LD, 2) & 0x0F);
   Payload[Length++] = make8(LD, 1);
   Payload[Length++] = make8(LD, 0);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_LOCK_TYPE_C_TAG, Payload, Length);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_LOCK_TYPE_C_TAG, Response, &Length);
         
         if(Result)
            Result = Length;
      }
      else
         pr9200_no_tx_error(Response);
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
  
  return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// AntennaCheck()
//
// Function use the check the antenna condition.
//
// Parameters:
//    Level - The reference level, recommended level is 100.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t AntennaCheck(uint8_t Level, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_ANTENNA_CHECK, &Level, 1);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_ANTENNA_CHECK, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetTemperature()
//
// Gets current temperature, returned in °C.
//
// Parameter:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetTemperature(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_TEMP, Response));
}

///////////////////////////////////////////////////////////////////////////////
// GetRSSI()
//
// Gets RSSI level, returned as tenth of dBm * -1.  For example a return value
// of 900 = -90.0 dBm.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetRSSI(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_RSSI, (uint8_t *)Response));
}

///////////////////////////////////////////////////////////////////////////////
// ScanRSSI()
//
// Function to start an RSSI level scan on all channels.
//
// Parameter:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t ScanRSSI(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_SCAN_RSSI, Response));
}

///////////////////////////////////////////////////////////////////////////////
// GetDTCResult()
//
// Function to get the DTC Result
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetDTCResult(PR9200_DTC_RESULT *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_DTC_RESULT, (uint8_t *)Response));
}

///////////////////////////////////////////////////////////////////////////////
// UpdateRegistry()
//
// Function to store data in registry.  When making changes to the RED module,
// Tx power for example, calling the UpdateRegistry() function saves the change
// in the RED5 module.  So when the module is reset or power cycled the setting
// will remain.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t UpdateRegistry(uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[1];
   uint16_t Length;
   
   Payload[0] = 0x01;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_UPDATE_REGISTRY, Payload, 1);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_UPDATE_REGISTRY, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetRegisteryItem()
//
// Get a registry item from PR9200.
//
// Parameters:
//    Item - register item to get, see PR9200_REGISTRY_ITEM type in pr9200.h
//           for different items.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetRegistryItem(PR9200_REGISTRY_ITEM Item, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[2];
   uint16_t Length;
   
   Payload[0] = make8((uint16_t)Item, 1);
   Payload[1] = make8(Item, 0);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_GET_REGISTRY_ITEM, Payload, 2);
    
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_GET_REGISTRY_ITEM, Response=NULL, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// SetOptimumFrequencyHoppingTable()
//
// Set Optimum Frequency Hopping Table.  When the reader's antenna size is not
// large enough, you can't use all channels of your band.  If you read tag's
// information at the channel outside antenna bandwidth, the read range will be 
// quite decreased.  This command help you search good channels within your band
// and set optimized frequency hopping table.  When you execute this command, 
// reader find optimized channel automatically.
// 
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetOptimumFrequencyHoppingTable(uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Length = pr9200_command_no_params(PR9200_MESSAGE_CODE_SET_OPTIMUM_FREQ_HOPPING_TABLE, Response);
   
   if(Response != NULL)
   {
      if(Length > 0)
      {
         if(Response[0] != 0x01)
         {
            Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_OPTIMUM_FREQ_HOPPING_TABLE, Response, &Length);
            
            if(!Result)
               Length = 0;
         }
      }
   }
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// GetFrequencyHoppingMode()
//
// Used to get the frequency hopping mode.  Reader can set two types of 
// frequency hopping tables: normal mode and smart hopping mode.  Reader uses
// all frequency channels of operation band in normal mode.  In smart hopping
// mode, it uses the specified frequency hopping table selected by 
// SetOptimumFrequencyHoppingTable().
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetFrequencyHoppingMode(uint8_t *Response=NULL)
{
   return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_FREQ_HOPPING_MODE, Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetFrequencyHoppingMode()
//
// Used to set the frequency hopping mode.  Reader can set two types of 
// frequency hopping tables: normal mode and smart hopping mode.  Reader uses
// all frequency channels of operation band in normal mode.  In smart hopping
// mode, it uses the specified frequency hopping table selected by 
// SetOptimumFrequencyHoppingTable().
//
// Parameters:
//    Mode - 0x00 normal mode, 0x01 smart hopping mode.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetFrequencyHoppingMode(uint8_t Mode, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_FREQ_HOPPING_MODE, &Mode, 1);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_FREQ_HOPPING_MODE, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// GetTxLeakageRSSILevelForSmartHoppingMode()
//
// Gets tx leakage RSSI level for smart hopping mode, the reference value used
// when doing SetOptimumFrequencyHoppingTable() to determining good channels.
//
// Parameters:
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t GetTxLeakageRSSILevelForSmartHoppingMode(uint8_t *Response=NULL)
{
    return(pr9200_command_no_params(PR9200_MESSAGE_CODE_GET_TX_LEAKAGE_RSSI_LEVEL, Response));
}

///////////////////////////////////////////////////////////////////////////////
// SetTxLeakageRSSILevelForSmartHoppingMode()
//
// Sets tx leakage RSSI level for smart hopping mode, the reference value used
// when doing SetOptimumFrequencyHoppingTable() to determining good channels.
//
// Parameters:
//    Level - the reference level, 1-255.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetTxLeakageRSSILevelForSmartHoppingMode(uint8_t Level, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_SET_TX_LEAKAGE_RSSI_LEVEL, &Level, 1);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_SET_TX_LEAKAGE_RSSI_LEVEL, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// StartReadWithFastLeakageCal()
//
// Start Read with Fast Leakage Cal.
//
// Parameters:
//    Channel - Channel to use.
//
//    IDT - Inductor
//
//    DTC1 - 
//
//    DTC2 - 
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t StartReadWithFastLeakageCal(uint8_t Channel, uint8_t IDT, uint8_t DTC1, uint8_t DTC2, uint8_t *Response=NULL)
{
   int1 Result;
   uint8_t Payload[8];
   uint16_t Length;
   
   Payload[0] = IDT;
   Payload[1] = DTC1;
   Payload[2] = DTC2;
   Payload[3] = Channel;
   memset(&Payload[4], 0x00, 4);
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_START_READ_WITH_FAST_LEAKAGE_CAL, Payload, 8);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_START_READ_WITH_FAST_LEAKAGE_CAL, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// AuthenticateDNATag()
//
// Function used to Authenticate DNA Tag.  This function didn't work for me, it
// returned an unsupported command error code.  I think the way this is suppose
// to work is that it takes what you passed adds some stuff to it including a
// 32 bit random number, encrypts it and returns that encrypted data.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of tag
//
//    Code - Pointer to uint8_t data array containing data to send to Tag to 
//           do authentication with.  
//
//    cLength - Number of code bytes to send to tag.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned. 
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, return value is number of bytes return the in
//           response.
///////////////////////////////////////////////////////////////////////////////
uint16_t AuthenticateDNATag(TYPE_C_UII *Tag, uint8_t *Code, uint16_t cLength, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE];
  #endif
   uint16_t Length;
   uint16_t bLength;
   
   Length = Tag->Length;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(Length + cLength + 7);
   
   if(Payload == NULL)
  #else
   if((Length + cLength + 7) > PR9200_MAX_PAYLOAD_SIZE)
  #endif
   {
     if(Response != NULL) 
         pr9200_no_tx_error(Response);
      
      return(0);
   }
   
   bLength = cLength * 8;     //Message Length in bits
   
   Payload[0] = make8(Length, 1);
   Payload[1] = make8(Length, 0);
   memcpy(&Payload[2], Tag->EPC, Length);
   Length += 2;
   Payload[Length++] = 0x01;  //Send Response
   Payload[Length++] = 0x01;  //Include Length in reply
   Payload[Length++] = 0x00;  //AES crypto suite
   Payload[Length++] = make8(bLength, 1);
   Payload[Length++] = make8(bLength, 0);
   memcpy(&Payload[Length], Code, cLength);
   Length += cLength;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_AUTHENTICATE, Payload, Length);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_AUTHENTICATE, Response, &Length);
         
         if(Result)
            Result = Length;
      }
      else
         pr9200_no_tx_error(Response);
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
  
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// UntraceableDNATagSettings()
//
// Function for setting untraceable settings for DNA tags.  From what I can tell
// this is used to enabled disable whether Access Passwords is required to 
// read none, all or some of the EPC, TID and User memory.  
// 
// For the EPC memory there is two parameters show/hide and length.  The 
// show/hide part controls whether the part of the EPC after the length requires
// the access password, if set to hide (1) then it requires the access password
// to read it and if set to show (0) then it does not require the access 
// password to read it.  The length sets how much of EPC in words is return or 
// needed when calling functions that return or needs the Tags UII.
//
// For the TID memory there is one parameter which sets how much of TID memory
// requires the access passwords.  0x00 access password not required to read TID
// memory, 0x01 access password required to read TID word addresses 2 and above,
// 0x02 access password required to read all of TID memory.
//
// For the User memory there is one parameter which sets if the user memory
// requires access password to read it.  0x00 access password not required to 
// read User memory, 0x01 access password required to read User memory.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of tag to set
//          untraceable setting for.
//
//    Password - Access password.
//
//    Settings - Pointer to UNTRACEABLE_DNA_SETTINGS struct containing settings
//               to set Tag to.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned. 
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, return value is number of bytes return the in
//           response.
///////////////////////////////////////////////////////////////////////////////
uint16_t UntraceableDNATagSettings(TYPE_C_UII *Tag, uint32_t Password, UNTRACEABLE_DNA_SETTINGS *Settings, uint8_t *Response=NULL)
{
   uint16_t Result;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[TYPE_C_MAX_EPC_LENGTH + 9];
  #endif
   uint16_t Length;
   
   Length = Tag->Length;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(Length + 9);
   
   if(Payload == NULL)
   {
      if(Response != NULL) 
         pr9200_no_tx_error(Response);
      
      return(0);
   }
  #endif
   
   Payload[0] = make8(Password, 3);
   Payload[1] = make8(Password, 2);
   Payload[2] = make8(Password, 1);
   Payload[3] = make8(Password, 0);
   Payload[4] = make8(Length, 1);
   Payload[5] = make8(Length, 0);
   memcpy(&Payload[6], Tag->EPC, Length);
   Length += 6;
   Payload[Length++] = Settings->U;
   Payload[Length++] = Settings->EPC;
   Payload[Length++] = Settings->TID;
   Payload[Length++] = Settings->User;
   Payload[Length++] = Settings->Range;
   
   Result = pr9200_put_message(PR9200_MESSAGE_CODE_UNTRACEABLE, Payload, Length);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(PR9200_MESSAGE_CODE_UNTRACEABLE, Response, &Length);
         
         if(Result)
            Result = Length;
      }
      else
         pr9200_no_tx_error(Response);
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
  
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// SetPassword()
//
// Function used to set the Kill and Access passwords on a Tag.  Setting an 
// access password doesn't automatically enable password protection on the tag.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of tag to set password
//          for.
//
//    aPassword - Access password if RFU memory bank was password protected.  
//                Use 0 if not password protected.
//
//    pType - Password to set, use TYPE_C_PASSWORD_KILL to set kill password and
//            TYPE_C_PASSWORD_ACCESS to set access password.
//
//    Password - The password to set, this is an uint32_t value.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t SetPassword(TYPE_C_UII *Tag, uint32_t aPassword, TYPE_C_PASSWORD pType, uint32_t Password, uint8_t *Response=NULL)
{
   uint16_t Length;
   uint16_t Address;
   
   if(pType == TYPE_C_PASSWORD_KILL)
      Address = 0;
   else
      Address = 2;
   
   Password = SwapBytes(Password);
   
   Length = WriteTypeCTagData(Tag, TYPE_C_MEMORY_BANK_RFU, Address, (uint8_t *)&Password, 2, aPassword, Response);
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// ReadPassword()
//
// Function used to get a tag's Kill or Access passwords.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of tag to get password
//          for.
//
//    aPassword - Access password if RFU memory bank was password protected.  
//                Use 0 if not password protected.
//
//    pType - Password to get, use TYPE_C_PASSWORD_KILL to get kill password and
//            TYPE_C_PASSWORD_ACCESS to get access password.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t ReadPassword(TYPE_C_UII *Tag, uint32_t aPassword, uint8_t pType, uint32_t *Response=NULL)
{
   uint16_t Length;
   uint16_t Address;
   
   if(pType == TYPE_C_PASSWORD_KILL)
      Address = 0;
   else
      Address = 2;
   
   Length = ReadTypeCTagData(Tag, TYPE_C_MEMORY_BANK_RFU, Address, 2, aPassword, (uint8_t *)Response);
   
   if((Response != NULL) && (Length > 0))
      *Response = SwapBytes(*Response);
   
   return(Length);
}

///////////////////////////////////////////////////////////////////////////////
// DetectTags()
//
// Function used to detect tags.  This function will continue searching for
// tags until Max tags is reached or time expires.
//
// Parameters:
//    Tags - Pointer to TYPE_C_UII structure array to return detected tag's EPC
//           to.  Array must be large enough to handle the max number of tags
//           that can be detected.
//
//    Max - Max number of tags to detect.
//
//    Time - Time in seconds to search for tags.
//
//    TagsRSSI - Optional pointer to a float32 array to return the detected
//               Tag's RSSI value to.  Array must be large enough to handle the
//               max number of tags that can be detected.
//
// Returns - uint8_t indicating the number of tags detected.
///////////////////////////////////////////////////////////////////////////////
uint8_t DetectTags(TYPE_C_UII *Tags, uint8_t Max, uint8_t Time, float32 *TagsRSSI=NULL)
{
   TYPE_C_UII Tag;
   TYPE_C_UII *pTag;
   uint8_t TagCount = 0;
   PR9200_MESSAGE_TYPE Type;
   PR9200_MESSAGE_CODE Code;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE];
  #endif
   uint16_t Length;
   int1 Done, SeenComplete;
   TAG_RSSI Rssi;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(TYPE_C_MAX_EPC_LENGTH + 6); //2 for PC, 4 for RSSI
   
   if(Payload == NULL)
      return(TagCount);
  #endif
   
   if(TagsRSSI != NULL)
      Length = StartAutoRead_RSSI(TAG_TYPE_C, 0, Time, 0, Payload);
   else
      Length = StartAutoRead(TAG_TYPE_C, 0, Time, 0, Payload);
   
   if(Length > 0)
   {
      Done = FALSE;
      SeenComplete = FALSE;
   
      while(!Done)
      {
         pr9200_rx_task();
         
         if(pr9200_has_rx_data())
         {
            pr9200_get_message(&Type, &Code, Payload, &Length);
         
            switch(Code)
            {
               case PR9200_MESSAGE_CODE_READ_TYPE_C_UII:
               case PR9200_MESSAGE_CODE_START_AUTO_READ2:
                  if(Length == 1)
                  {
                     if(Payload[0] == 0x1F)
                     {
                        Done = TRUE;
                        SeenComplete = TRUE;
                     }
                  }
                  else
                  {
                     memcpy(&Tag, Payload, Length);
                     
                     Tag.PC = SwapBytes(Tag.PC);
                     Tag.Length = Length - 2;
                     
                     if(CheckIfNewTag(&Tag, Tags, TagCount) == TRUE)
                     {
                        pTag = &Tags[TagCount];
                  
                        memcpy(pTag, &Tag, Length);
                        pTag->Length = Tag.Length;
                        
                        if(++TagCount >= Max)
                           Done = TRUE;
                     }
                  }
                  break;
               case PR9200_MESSAGE_CODE_READ_TYPE_C_UII_RSSI:
               case PR9200_MESSAGE_CODE_START_AUTO_READ_RSSI:
                  if(Length == 1)
                  {
                     if(Payload[0] == 0x1F)
                     {
                        Done = TRUE;
                        SeenComplete = TRUE;
                     }
                  }
                  else
                  {
                     memcpy(&Tag, Payload, Length - 4);
               
                     Tag.PC = SwapBytes(Tag.PC);
                     Tag.Length = Length - 6;
                     
                     if(CheckIfNewTag(&Tag, Tags, TagCount) == TRUE)
                     {
                        pTag = &Tags[TagCount];
                        
                        memcpy(pTag, &Tag, Tag.Length + 2);
                        pTag->Length = Tag.Length;
                        
                        memcpy(&Rssi, &Payload[Length - 4], 4);
                        TagsRSSI[TagCount] = CalculateTagRSSI(Rssi);
                        
                        if(++TagCount >= Max)
                           Done = TRUE;
                     }
                  }
                  break;
            }
         }
         
         if(Done && (SeenComplete == FALSE))
            Length = StopAutoRead(Payload);
      }
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
   
   return(TagCount);
}

///////////////////////////////////////////////////////////////////////////////
// DetectTagsAntenna()
//
// Function used to detect tags using the specified antennas.  This function
// will continue searching for tags until Max tags is reached or time expires.
//
// Parameters:
//    Tags - Pointer to TYPE_C_UII structure array to return detected tag's EPC
//           to.  Array must be large enough to handle the max number of tags
//           that can be detected.
//
//    Max - Max number of tags to detect.
//
//    Time - Time in seconds to search for tags.
//
//    Antenna - The antennas to use when detecting tags.  This is a bitwise
//              value setting a bit will cause that antenna to be used, bit 0 -
//              antenna 1, bit 1 - antenna 2, bit 2 - antenna 3, etc.  If more
//              then one bit is set then those antenna are used in sequence.
//
//    TagsAntenna - Pointer to an uint8_t array to return the antenna that the
//                  tag was detected with.  Array must be large enough to
//                  handle the max number of tags that can be detected.
//
//    TagsRSSI - Optional pointer to a float32 array to return the detected
//               Tag's RSSI value to.  Array must be large enough to handle the
//               max number of tags that can be detected.
//
// Returns - uint8_t indicating the number of tags detected.
///////////////////////////////////////////////////////////////////////////////
uint8_t DetectTagsAntenna(TYPE_C_UII *Tags, uint8_t Max, uint8_t Time, uint8_t Antenna, uint8_t *TagsAntenna, float32 *TagsRSSI=NULL)
{
 #if PR9200_USE_EXTERNAL_ANT_SWITCH == FALSE
   TYPE_C_UII Tag;
   TYPE_C_UII *pTag;
   uint8_t TagCount = 0;
   PR9200_MESSAGE_TYPE Type;
   PR9200_MESSAGE_CODE Code;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Payload;
  #else
   uint8_t Payload[PR9200_MAX_PAYLOAD_SIZE];
  #endif
   uint16_t Length;
   int1 Done, SeenComplete;
   int1 HasRssi;
   TAG_RSSI Rssi;
   uint8_t i;
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Payload = malloc(TYPE_C_MAX_EPC_LENGTH + 9); //1-CMD Code, 1-Has RSS, 1-Antenna Port, 2-PC, 4-RSSI
   
   if(Payload == NULL)
      return(TagCount);
  #endif
   
   Length = StartAutoReadAntenna(TagsRSSI != NULL, Antenna, 0, Time, 0, Payload);
   
   if(Length > 0)
   {
      Done = FALSE;
      SeenComplete = FALSE;
   
      while(!Done)
      {
         pr9200_rx_task();
         
         if(pr9200_has_rx_data())
         {
            pr9200_get_message(&Type, &Code, Payload, &Length);
            
            switch(Code)
            {
               case PR9200_MESSAGE_CODE_AUTO_READ_ANTENNA_RESPONSE:
               case PR9200_MESSAGE_CODE_START_AUTO_READ_ANTENNA:
                  if(Length == 1)
                  {
                     if(Payload[0] == 0x1F)
                     {
                        Done = TRUE;
                        SeenComplete = TRUE;
                     }
                  }
                  else
                  {
                     if(Payload[1] == 0x00)
                        HasRssi = FALSE;
                     else
                        HasRssi = TRUE;
                  
                     if(HasRssi)
                     {
                        memcpy(&Tag, &Payload[3], Length - 7);
                        Tag.Length = Length - 9;
                     }
                     else
                     {
                        memcpy(&Tag, &Payload[3], Length - 3);
                        Tag.Length = Length - 5;
                     }
                        
                     Tag.PC = SwapBytes(Tag.PC);
                     
                     if(CheckIfNewTag(&Tag, Tags, TagCount) == TRUE)
                     {
                        pTag = &Tags[TagCount];
                        
                        memcpy(pTag, &Tag, Tag.Length + 2);
                        pTag->Length = Tag.Length;
                        
                        for(i=1;i<9;i++)
                        {
                           if((Payload[2] & ((0x01) << (i - 1))) != 0)
                           {
                              TagsAntenna[TagCount] = i;
                              break;
                           }
                        }
                        
                        if(HasRssi)
                        {
                           memcpy(&Rssi, &Payload[Length - 4], 4);
                           TagsRSSI[TagCount] = CalculateTagRSSI(Rssi);
                        }
                        
                        if(++TagCount >= Max)
                           Done = TRUE;
                     }
                  }
                  break;
            }
         }
         
         if(Done && (SeenComplete == FALSE))
            Length = StopAutoRead(Payload);
      }
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Payload);
  #endif
   
   return(TagCount);
  #else
   uint8_t TagCount;
   uint8_t pTagCount;
   uint8_t aCount = 0;
   uint8_t aTime;
   uint8_t aAntenna;
   uint8_t cAntenna;
   uint8_t i, j;
   uint8_t AndValue;
   int1 Done;
   
   for(i=0;i<8;i++)
   {
      if((Antenna & (0x01 << i)) != 0)
         aCount++;
   }
   
   if(aCount > 0)
   {
      TagCount = 0;
      pTagCount = 0;
      AndValue = 0x01;
      cAntenna = 1;
   
      aTime = Time / aCount;
      
      if(aTime == 0)
         aTime = 1;
      
      for(i=0;i<aCount,TagCount<Max;i++)
      {
         Done = FALSE;
         
         do
         {
            if((Antenna & AndValue) != 0)
            {
               aAntenna = cAntenna;
               Done = TRUE;
            }
            
            AndValue <<= 1;
            cAntenna++;
         } while((Done == FALSE) && (AndValue != 0));
         
         if(Done == FALSE)
            return(TagCount);
      
         select_antenna(aAntenna);
         
         if(TagsRSSI != NULL)
            TagCount += DetectTags(&Tags[TagCount], Max - TagCount, aTime, &TagsRSSI[TagCount]);
         else
            TagCount += DetectTags(&Tags[TagCount], Max - TagCount, aTime);
         
         if(TagCount != pTagCount)
         {
            for(j=pTagCount;j<TagCount;j++)
               TagsAntenna[j] = aAntenna;
            
            pTagCount = TagCount;
         }
      }
   }
   
   return(TagCount);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// LockTagMemoryRegion()
//
// Function used to lock or unlock the tag's memory regions.  This function
// locks or unlocks whether the access password is required to access the
// memory.  For the Access and Kill password memory regions this sets both read
// and write access, for all other memory regions this sets write access.
//
// Parameters:
//    Region - The memory region to lock or unlock, see LOCK_MEMORY_REGION
//             struct in pr9200.h for list of memory regions.
//
//    Tag - Pointer to TYPE_C_UII structure containing EPC of tag to lock or 
//          unlock.
//
//    Password - The access password, the access password is required to lock
//               or unlock the memory region.
//
//    Lock - Whether to lock or unlock memory, TRUE - lock, FALSE - unlock.
//
// Returns - 0x00 if no error occured, otherwise the PR9200_ERROR_CODE.
///////////////////////////////////////////////////////////////////////////////
PR9200_ERROR_CODE LockTagMemoryRegion(LOCK_MEMORY_REGION Region, TYPE_C_UII *Tag, uint32_t Password, int1 Lock)
{
   uint16_t Length;
   uint8_t Payload[PR9200_ERROR_REPSONSE_LENGTH];
   uint32_t LD;
   
   LD = (uint32_t)0b10 << (10 + (Region * 2));
   
   if(Lock)
      LD |= ((uint32_t)0b10 << (Region * 2));
   
   Length = LockTypeCTag(Tag, Password, LD, Payload);
   
   if(Length > 0)
      return(0x00);
   else
      return(Payload[2]);
}

///////////////////////////////////////////////////////////////////////////////
// PermaLockTagMemoryRegion()
//
// Function used to permanently lock or unlock the tag's memory regions.  This 
// function permanently locks or unlocks whether the access password is required
// to access the memory.  For the Access and Kill password memory regions this
// sets both read and write access, for all other memory regions this sets
// write access.  
//
// ** Care should be used when using this function because once the memory
//    region access is permanently locked or unlocked it can't be undone.
//
// Parameters:
//    Region - The memory region to permanently lock or unlock, see
//             LOCK_MEMORY_REGION struct in pr9200.h for list of memory regions.
//
//    Tag - Pointer to TYPE_C_UII structure containing EPC of tag to permanently 
//          lock or unlock.
//
//    Password - The access password, the access password is required to 
//               permanently lock or unlock the memory region.
//
//    Lock - Whether to permanently lock or unlock memory, TRUE - lock, FALSE - 
//           unlock.
//
// Returns - TRUE if successful, FALSE if unsuccessful.
///////////////////////////////////////////////////////////////////////////////
int1 PermaLockTagMemoryRegion(LOCK_MEMORY_REGION Region, TYPE_C_UII *Tag, uint32_t Password, int1 Lock)
{
   uint16_t Length;
   uint8_t Payload[PR9200_ERROR_REPSONSE_LENGTH];
   uint32_t LD;
   
   LD = (uint32_t)1 << (10 + (Region * 2));
   
   if(Lock)
      LD |= ((uint32_t)1 << (Region * 2));
   
   Length= LockTypeCTag(Tag, Password, LD, Payload);
   
   if(Length > 0)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// SwapBytes()
//
// Function used to swap the bytes of an 16 bit value.  The RED5 communication
// protocol requires 16 bit parameters to be sent MSB first.  This a helper
// function that can be called to swap the bytes of a 16 bit value, which are
// stored LSB first, so it can be transmitted MSB first.
//
// Parameters:
//    Value - The 16 bit value to swap the bytes for.
//
// Returns - uint16_t with the bytes swapped.
///////////////////////////////////////////////////////////////////////////////
uint16_t SwapBytes(uint16_t Value)
{
   UINT16_UNION Result;
   
   Result.b[0] = make8(Value, 1);
   Result.b[1] = make8(Value, 0);
   
   return(Result.w);
}

///////////////////////////////////////////////////////////////////////////////
// SwapBytes()
//
// Function used to swap the bytes of a 32 bit value.  The RED5 communication
// protocol requires 32 bit parameters to be sent MSB first.  This a helper
// function that can be called to swap the bytes of a 32 bit value, which are
// stored LSB first, so it can be transmitted MSB first.
//
// Parameters:
//    Value - The 32 bit value to swap the bytes for.
//
// Returns - uint32_t with the bytes swapped.
///////////////////////////////////////////////////////////////////////////////
uint32_t SwapBytes(uint32_t Value)
{
   UINT32_UNION Result;
   
   Result.b[0] = make8(Value, 3);
   Result.b[1] = make8(Value, 2);
   Result.b[2] = make8(Value, 1);
   Result.b[3] = make8(Value, 0);
   
   return(Result.dw);
}

#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// CalculateTagRSSI()
//
// Function used to calculate the RSSI value of a TAG returned from the 
// StartAutoRead_RSSI() function is called to read the TAG UII with RSSI level.
//
// Parameters:
//    Rssi - TAG_RSSI struct containing the tag RSSI parameters.
//
// Returns - float32 value representing the tags RSSI value.
///////////////////////////////////////////////////////////////////////////////
float32 CalculateTagRSSI(TAG_RSSI Rssi)
{
   float64 RFIN_I;
   float64 RFIN_Q;
      
   RFIN_I = 20.0*log10((float64)Rssi.RSSI_I);
   RFIN_I -= (float64)Rssi.Gain_I;
   RFIN_I -= 63.0;
   
   RFIN_Q = 20.0*log10((float64)Rssi.RSSI_Q);
   RFIN_Q -= (float64)Rssi.Gain_Q;
   RFIN_Q -= 63.0;
   
   RFIN_I = pwr(10, RFIN_I / 20.0);
   RFIN_Q = pwr(10, RFIN_Q / 20.0);
   
   RFIN_I = RFIN_I * RFIN_I;
   RFIN_Q = RFIN_Q * RFIN_Q;
   
   RFIN_I = sqrt(RFIN_I + RFIN_Q);
   
   return(20.0*log10(RFIN_I));
}

///////////////////////////////////////////////////////////////////////////////
// CheckIfNewTag()
//
// Function to determine if detected tag is new or a repeated read.
//
// Parameters:
//    Tag - Pointer to TYPE_C_UII struct containing EPC of detected tag.
//
//    Tags - Pointer to TYPE_C_UII_struct array containing EPC of tags to check
//           against.
//
//    Count - Number of detected tags in tags array.
//
// Returns - TRUE if tag is new, FALSE if tag is already in tags array.
///////////////////////////////////////////////////////////////////////////////
int1 CheckIfNewTag(TYPE_C_UII *Tag, TYPE_C_UII *Tags, uint8_t Count)
{
   int1 Result = TRUE;
   uint8_t i;
   uint16_t j;
   uint16_t Length;
   TYPE_C_UII *pTag;
   
   for(i=0;i<Count;i++)
   {
      pTag = &Tags[i];
      Length = pTag->Length;
      
      if(Tag->Length == Length)
      {
         for(j=0;j<Length;j++)
         {
            if(Tag->EPC[j] != pTag->EPC[j])
               break;
         }
         
         if(j >= Length)
         {
            Result = FALSE;
            break;
         }
      }
   }
   
   return(Result);
}

#if PR9200_FORCE_SW_CRC == TRUE
///////////////////////////////////////////////////////////////////////////////
// crc16()
//
// Function for calculating the CRC16 value of the data passed to it.  This
// function is only available when the CRC is calculated in software.
//
// Parameters:
//    Data - Pointer to uint8_t array containing data to calculate CRC for.
//
//    Length - number of data bytes to calculate CRC for.
//
// Returns - uint16_t, the calculated CRC16 value for data.
///////////////////////////////////////////////////////////////////////////////
uint16_t crc16(uint8_t *Data, uint16_t Length)
{
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while(Length--)
    {
        x = crc >> 8 ^ *Data++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
    }
    
    return(crc);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// pr9200_calculate_crc16()
//
// Function used to calculate the CRC16 for the passed message.  Additionally it
// can be used to verify the CRC16 value of the passed message.
//
// Parameters:
//    Message - Pointer to PR9200_MESSAGE_STRUCT containing message to calculate
//              CRC16 for.
//
//    IsCheck - Optional parameter specifying whether to check the passed CRC16
//              to the calculated CRC16.  TRUE - perform check, FALSE - don't
//              perform check.
//
// Returns - TRUE if calculated correctly or calculated CRC16 matches passed
//           CRC16 when IsCheck is TRUE.  FALSE if CRC16 wasn't calculated or
//           calculated CRC16 doesn't match passed CRC16 when IsCheck is TRUE.
///////////////////////////////////////////////////////////////////////////////
int1 pr9200_calculate_crc16(PR9200_MESSAGE_STRUCT *Message, int1 IsCheck=FALSE)
{
   int1 Result;
   uint16_t crcResult;
   uint16_t Length;
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   uint8_t *Buffer;
  #else
   uint8_t Buffer[(PR9200_MAX_PAYLOAD_SIZE + sizeof(PR9200_MESSAGE_HEADER) + 1)];
  #endif
   
   Length = Message->Header.Length;
  
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   Buffer = malloc(sizeof(PR9200_MESSAGE_HEADER) + Length + 1);
  
   if(Buffer == NULL)
      return(FALSE);
  #endif
  
   Buffer[0] = Message->Header.Type;
   Buffer[1] = Message->Header.Code;
   Buffer[2] = make8(Length, 1);
   Buffer[3] = make8(Length, 0);
   memcpy(&Buffer[4], Message->Payload, Length);
   Buffer[4 + Length] = PR9200_MESSAGE_END_MASK;
   
  #if PR9200_FORCE_SW_CRC == FALSE
   crc_init(0xFFFF);
   
   crcResult = crc_calc16(Buffer, 5 + Length, 8);
  #else
   crcResult = crc16(Buffer, 5 + Length);
  #endif
   
   if(IsCheck)
   {
      if(crcResult == Message->Crc)
         Result = TRUE;
      else
         Result = FALSE;
   }
   else
   {
      Message->Crc = crcResult;
      
      Result = TRUE;
   }
   
  #if PR9200_USE_DYNAMIC_MEMORY == TRUE
   free(Buffer);
  #endif
  
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_command_no_params()
//
// Helper function for sending a command and receiving response, if Response
// isn't NULL, when the transmitted command doesn't have any parameters.
//
// Parameters:
//    Code - PR9200_MESSAGE_CODE of command to send and response to receive, if
//           Response isn't NULL.
//
//    Response - Optional pointer to an array to return response to.  If Response
//               is not NULL function will wait for RED5 module to return a
//               response.  The array should be a minimum of 3 bytes incase an
//               Error response is returned.  
//
// Returns - uint16_t returns 0 if an error occurred, 1 or greater if messages 
//           was sent successful.  If Response isn't NULL and return value is 0, 
//           indicating an error occurred, first three bytes of Response is error
//           code, unless they are all 0xFF then an error occurred that caused
//           the request not to be sent to RED5 module.  Otherwise if Response
//           isn't NULL and return value is 1 or greater, indicating messages
//           sent successfully, and return value is number of bytes return the 
//           in response.
///////////////////////////////////////////////////////////////////////////////
uint16_t pr9200_command_no_params(PR9200_MESSAGE_CODE Code, uint8_t *Response=NULL)
{
   int1 Result;
   uint16_t Length;
   
   Result = pr9200_put_message(Code, NULL, 0);
   
   if(Response != NULL)
   {
      if(Result == TRUE)
      {
         Result = pr9200_get_response(Code, Response, &Length);
         
         if(Result)
            return(Length);
      }
      else
         pr9200_no_tx_error(Response);
   }
   
   return((uint16_t)Result);
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_get_response()
//
// Helper function for getting response for the specified command code.  
// Function will also exit if it receive and Error message.
//
// Parameters:
//    Code - PR9200_MESSAGE_CODE of command code to receive response for.
//
//    Response - Pointer to an uint8_t array to return response to.
//
//    Length - Pointer to an uint16_t variable to return number of bytes of 
//             Response to.
//
// Returns - TRUE if response for the command code was received, FALSE if an
//           Error response was received.
///////////////////////////////////////////////////////////////////////////////
int1 pr9200_get_response(PR9200_MESSAGE_CODE Code, uint8_t *Response, uint16_t *Length)
{
   int1 Result;
   int1 Done = FALSE;
   PR9200_MESSAGE_TYPE rType;
   PR9200_MESSAGE_CODE rCode;
   
  #if PR9200_RESTART_WDT == TRUE
   restart_wdt();
  #endif
   
   while(!Done)
   {
      pr9200_rx_task();
      pr9200_tx_task();
      
      if(pr9200_has_rx_data())
      {
         pr9200_get_message(&rType, &rCode, Response, Length);
         
         if(rType == PR9200_MESSAGE_TYPE_RESPONSE)
         {
            if(rCode == Code)
            {
               Result = TRUE;
               Done = TRUE;
            }
            else if(rCode == PR9200_MESSAGE_CODE_COMMAND_FAILURE)
            {
               Result = FALSE;
               Done = TRUE;
            }
         }
      }
   }
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// pr9200_no_tx_error()
//
// Helper function for setting all three bytes of an error response 0xFF.  This
// is a special case to indicate that an error occurred before a command was 
// transmitted to the RED5 module.
//
// Parameters:
//    Response - Pointer to at least a 3 byte uint8_t array to set the error
//               response to 0xFF 0xFF 0xFF.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void pr9200_no_tx_error(uint8_t *Response)
{
   memset(Response, 0xFF, PR9200_ERROR_REPSONSE_LENGTH);
}

#endif


