///////////////////////////////////////////////////////////////////////////////
////                                 DMX.c                                 ////
//// DMX Library for DMX512 protocol.                                      ////
////                                                                       ////
//// This library provides the following functions:                        ////
////  (for more information on these functions see the comment section     ////
////   above each function)                                                ////
////                                                                       ////
//// Defining DMX_TYPE to one of the following before including dmx.c in   ////
//// in your project determines the DMX device your code will be built     ////
//// for:                                                                  ////
////   DMX_CONTROLLER   - DMX driver is built for a DMX Controller.        ////
////   DMX_SLAVE_DEVICE - DMX driver is built for a DMX Slave Device.      ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
//// DMX Controller API:                                                   ////
////                                                                       ////
////   DMXInit() - Initializes the Enable line and clears the DMXData      ////
////               array.                                                  ////
////                                                                       ////
////   DMXSetChannel() - Sets the DMX Channel to the specified value.      ////
////                     This only writes the value to the DMXData array,  ////
////                     use DMXCommit() to send value to devices.         ////
////                                                                       ////
////   DMXGetChannel() - Reads the value that the specified DMX channel is ////
////                     to.  This reads the value that is stored in the   ////
////                     set DMXData array, may not match what the actual  ////
////                     channel is set to if DMXCommit() was done after   ////
////                     making a change with DMXSetChannel().             ////
////                                                                       ////
////   DMXCommit() - Sends channel values set in DMXData array to DMX      ////
////                 devices.                                              ////
////                                                                       ////
////   Use the following defines to set the PIC pins that connect to the   ////
////   RS485 Transceiver DE and DI pins:                                   ////
////     #define DMX_XMIT PIN_xx     //RS485 Transceiver DI pin            ////
////     #define DMX_ENABLE PIN_xx   //RS485 Transceiver DE pin            ////
////                                                                       ////
////   Use the following defines to set the number of DMX unit you are     ////
////   controlling and the number of channels each unit has.               ////
////     #define DMX_UNITS x                                               ////
////     #define DMX_CHANNELS x                                            ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
//// DMX Slave API:                                                        ////
////                                                                       ////
////   DMXInit() - Initializes the RS485 chip and enables the DMX Receive  ////
////               interrupt.                                              ////
////                                                                       ////
////   DMXKbhit() - Determines if new DMX Data has been received.          ////
////                                                                       ////
////   DMXGetd() - Retrieves new DMX Data from Buffer.                     ////
////                                                                       ////
////   Use the following defines to set the RDA interrupt to receive data  ////
////   with and the RS485 RE pin:                                          ////
////     #define DMX_INT    INT_RDA  //RDA interrupt can be INT_RDA,       ////
////                                 //INT_RDA2, INT_RDA3, etc.            ////
////     #define DMX_RCV    PIN_xx   //RS485 Transceiver DO pin, should be ////
////                                 //the PIC's UxRX pin corresponding to ////
////                                 //the RDA interrupt set for DMX_INT.  ////
////     #define DMX_ENABLE PIN_xx   //RS485 Transceiver RE pin, optional  ////
////                                 //RE pin can be tied to GND to enable ////
////                                 //continuous receiving.               ////
////                                                                       ////
////   Use the following defines to set this units channel start, number   ////
////   of channels and number of receive buffers:                          ////
////     #define DMX_START_CHANNEL  x //start channel can be 1 to 512      ////
////     #define DMX_CHANNELS       x //number of channels for this unit,  ////
////                                  //1 to (513 - DMX_START_CHANNEL)     ////
////     #define DMX_BUFFERS        x //number of receive buffer to use    ////
////                                                                       ////
////   The following macros must be defined when using driver to create    ////
////   a slave device:                                                     ////
////     DMX_TICK - variable type to use for DMX_TICK, e.g.                ////
////                #define DMX_TICK unsigned int16.                       ////
////                                                                       ////
////     DMXGetTick() - function to return current tick count.             ////
////                                                                       ////
////     DMXGetTickDifference(x, y) - function to return difference        ////
////                                  between x and y, (x-y).              ////
////                                                                       ////
////     DMX_TICKS_PER_MS - define specifying the number of ticks in one   ////
////                        millisecond.                                   ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services               ////
//// This source code may only be used by licensed users of the CCS C      ////
//// compiler.  This source code may only be distributed to other licensed ////
//// users of the CCS C compiler.  No other use, reproduction or           ////
//// distribution is permitted without written permission.  Derivative     ////
//// programs created using this software in object code form are not      ////
//// restricted in any way.                                                ////
///////////////////////////////////////////////////////////////////////////////

#include <DMX.h>

#if DMX_TYPE == DMX_CONTROLLER
////////////////////////////// DMX Controller API /////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DMXInit()
//  Initializes the RS485 chip and sets all DXM channel values to zero.
//   Parameters: None
//   Returns:    Nothing
///////////////////////////////////////////////////////////////////////////////
void DMXInit(void)
{
   output_low(DMX_ENABLE);

   memset(DMXData,0,sizeof(DMXData));
}

///////////////////////////////////////////////////////////////////////////////
// DMXSetChannel()
//  Writes the specified value to the specified DMX channel
//   Parameters: channel - channel to write the value to, channel is 1 indexed.
//               value - value to set specified channel to.
//   Returns:    Nothing
///////////////////////////////////////////////////////////////////////////////
void DMXSetChannel(uint8_t channel, uint8_t value)
{
   uint8_t unit;
   
   channel -= 1;
   unit = channel / DMX_CHANNELS;
   channel %= DMX_CHANNELS;

   DMXData[unit][channel] = value;
}

///////////////////////////////////////////////////////////////////////////////
// DMXGetChannel()
//  Reads the value of the specified DMX channel.
//   Parameters: channel - channel to read the value from, channel is 1 indexed.
//   Returns:    The value read from the specified channel.
///////////////////////////////////////////////////////////////////////////////
uint8_t DMXGetChannel(uint8_t channel)
{
   uint8_t unit;
  
   channel -= 1;
   unit = channel / DMX_CHANNELS;

   return(DMXData[unit][channel]);
}

///////////////////////////////////////////////////////////////////////////////
// DMXCommit()
//  Sends the DMX channel data to the DMX devices.
//   Parameters: None
//   Returns:    Nothing
///////////////////////////////////////////////////////////////////////////////
void DMXCommit(void)
{
   uint16_t i;
   uint8_t *ptr = DMXData;
   
   output_high(DMX_ENABLE);
   
   output_low(DMX_XMIT);   //Break
   delay_us(100);
   
   output_high(DMX_XMIT);  //MAB
   delay_us(8);
   
   fputc(0x00,DMX_CONTROLLER_STREAM);        //SC
   
   for(i=0;i<(DMX_UNITS*DMX_CHANNELS);i++)
   {
      fputc(*ptr++,DMX_CONTROLLER_STREAM);
   }
   
   output_low(DMX_ENABLE);
}

#else //DMX Slave Device
//////////////////////////////// DMX Slave API ////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DMXInit()
//  Initializes the RS485 chip and enables the DMX Receive interrupt.  Global 
//  interrupt still needs to be enabled in main program.
//   Parameters: None
//   Returns:    Nothing
///////////////////////////////////////////////////////////////////////////////
void DMXInit(void)
{
  #ifdef DMX_ENABLE
   output_low(DMX_ENABLE); //Since DMX slave devices only receive data, pull RE 
  #endif                   //pin of RS485 transceiver low to always enable 
                           //receiving.  Optionally the RE pin can be tied to 
                           //GND to enable continuous receiving.  Driver also
                           //assumes that the DE pin is tied to GND or to the 
                           //RE pin.
   
   dmx_RxMode = DMX_RX_MODE_MARK;
   dmx_NextIn = dmx_NextOut = 0;
   
   enable_interrupts(DMX_INT);
}

///////////////////////////////////////////////////////////////////////////////
// DMXKbhit()
//  Determines if new DMX Data has been received.
//   Parameters: None
//   Returns:    TRUE - if there is new Data to retrieve
//               FALSE - if there is not new Data to retrieve
///////////////////////////////////////////////////////////////////////////////
int1 DMXKbhit(void)
{
   if(dmx_NextIn != dmx_NextOut)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// DMXGetd()
//  Retrieves new DMX Data from the DMX Buffer.
//   Parameters: Data - pointer to an int8 array to return the data to
//               Count - Number of bytes to read
//   Returns:    Nothing
///////////////////////////////////////////////////////////////////////////////
void DMXGetd(uint8_t *Data, uint16_t Count)
{
   uint16_t i;
   
   while(!DMXKbhit());
   
   for(i=0;i<Count,i<DMX_CHANNELS;i++)
      Data[i] = dmx_RxData[dmx_NextOut][i];
   
   if(i<Count)
   {
      for(i=DMX_CHANNELS;i<Count;i++)
         Data[i] = 0x00;
   }
   
   if(++dmx_NextOut >= DMX_BUFFERS)
      dmx_NextOut = 0;
}

///////////////////////////////////////////////////////////////////////////////
// slave_dmx_isr()
//  Interrupt Service Routine for DMX Slave Device.
//   Parameters: None
//   Returns:   Nothing
///////////////////////////////////////////////////////////////////////////////
#if DMX_INT == INT_RDA
#INT_RDA
#elif DMX_INT == INT_RDA2
#INT_RDA2
#elif DMX_INT == INT_RDA3
#INT_RDA3
#elif DMX_INT == INT_RDA4
#INT_RDA4
#else
 #error Must use one of the HW UART peripherals for DMX Slave Device
#endif
void slave_dmx_isr(void)
{
   uint8_t Data;
   static DMX_TICK MarkTick;
   static uint16_t Channel;
   DMX_TICK CurrentTick;
   
   CurrentTick = DMXGetTick();
   Data = fgetc(DMX_SLAVE_STREAM); 
   
   switch(dmx_RxMode)
   {
      case DMX_RX_MODE_MARK:
         if((Data == 0x00) && !input(DMX_RCV))
         {
            MarkTick = CurrentTick;
            dmx_RxMode++;
         }
         break;
      case DMX_RX_MODE_START:
         if(Data == 0x00)
         {
            if(DMXGetTickDifference(CurrentTick, MarkTick) >= ((DMX_TICKS_PER_MS * 88) / 1000))
            {
               dmx_RxMode++;
               Channel = 1;
            }
            else
               MarkTick = CurrentTick;
         }
         else
            dmx_RxMode = DMX_RX_MODE_MARK;
         break;
      case DMX_RX_MODE_DATA:
         if((Channel >= DMX_START_CHANNEL) && (Channel < (DMX_START_CHANNEL + DMX_CHANNELS)))
         {
            dmx_RxData[dmx_NextIn][Channel - DMX_START_CHANNEL] = Data;
            
            if(++Channel >= (DMX_START_CHANNEL + DMX_CHANNELS))
            {
               if(++dmx_NextIn >= DMX_BUFFERS)
                  dmx_NextIn = 0;
            }
         }
         else
            Channel++;
         
         if(Channel >= (DMX_START_CHANNEL + DMX_CHANNELS))
         {
            Channel = 0;
            dmx_RxMode = DMX_RX_MODE_MARK;
         }
         break;
   }
}

#endif
