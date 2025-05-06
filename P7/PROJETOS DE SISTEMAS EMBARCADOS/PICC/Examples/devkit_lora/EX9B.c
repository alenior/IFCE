///////////////////////////////////////////////////////////////////////////
////                               EX9B.c                              ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit that uses CCS's LoRa   ////
////  peer-to-peer (P2P) driver to send message's between devices.     ////
////                                                                   ////
////  The CCS LoRa P2P driver was developed by CCS and doesn't         ////
////  follow any industry standard.  Each device has a unique 8 bit    ////
////  address that is used to determine which device sent and receives ////
////  the message.  The driver uses a predetermined channel hopping    ////
////  table for determine which channels/frequencies are used and the  ////
////  order they are used in.  Additionally the driver allows          ////
////  assigning the device to one of three types, a Base Station, an   ////
////  End Device or a Listening End Devices.                           ////
////                                                                   ////
////  This exercise is the same as EX9A.c except the LORA_DEVICE_ADDR  ////
////  and OTHER_DEVICES_ADDR define are swapped.  See EX9A.c for a     ////
////  description of what the exercise does.                           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <devkit_lora.h>

// Include a tick timer driver and make appropriate defines, the lora.c driver
// requires a tick timer.
#include <tick.c>
#define TickDifference(a,b)      (a-b)

#define _lora_tick_t                TICK
#define _LORA_TICKS_PER_SECOND      TICKS_PER_SECOND
#define _lora_tick_difference(a,b)  TickDifference(a,b)
#define _lora_tick_get()            TickGet()

// Set device type to a listening end-device and set it's device address, each
// device needs to have a unique address.
#define LORA_DEVICE_TYPE      LORA_DEVICE_TYPE_LED
#define LORA_DEVICE_ADDR      0x02

// Set the other device address, the address of the device this device will
// send ADC and Rate messages to.  ACK and NACK messages always get sent back
// to the device that originally sent the message that being ACK'd or NACK'd.
#define OTHER_DEVICES_ADDR    0x01

// Include the LoRa P2P driver's header file, needs to be included before
// rn2903.c so that the required LoRa P2P driver functions in rn2903.c are
// accessible.
#include <lora.h>

// Include the RN2903 module driver and the LoRa P2P driver
#include <rn2903.c>
#include <lora.c>

#define PB1_PIN            PIN_INPUT_BUTTON1
#define PB2_PIN            PIN_INPUT_BUTTON0

#define PB1_IDX            0
#define PB2_IDX            1

// Setup PWM on Big Red LED, CCP2, to output ~1 kHz frequency with 10 bit
// resolution, initialize duty to 0%.
#use pwm(output=PIN_LED_BIG_RED, frequency=1kHz, bits=10, duty=0)

typedef enum
{
   MESSAGE_TYPE_NACK,
   MESSAGE_TYPE_ACK,
   MESSAGE_TYPE_ADC,
   MESSAGE_TYPE_RATE
} message_type_t;

typedef struct
{
   uint8_t Address;
   union
   {
      struct
      {
         message_type_t Type;
         uint8_t Data[2];
      };
      uint8_t Buffer[3];
   };
   uint8_t Length;
} message_struct_t;

typedef enum
{
   FLASH_RATE_OFF,
   FLASH_RATE_1HZ,
   FLASH_RATE_2HZ,
   FLASH_RATE_3HZ,
   FLASH_RATE_4HZ,
   FLASH_RATE_5HZ
} flash_rate_t;

typedef struct
{
   flash_rate_t Rate;
   TICK Ticks;
   TICK Tick;
   int1 LedOn;
} flash_stuct_t;

void InitHW(void)
{
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(AN_CHANNEL_POT0);
}

int1 PutMessage(message_struct_t *Message)
{
   //Load message into TX buffer to send, and return if it was successful or not.
   return(lora_put_message(Message->Address, Message->Buffer, Message->Length + 1));
}

void main(void)
{
   int1 Initialized;
   int1 cLevel[2], pLevel[2];
   uint16_t Reading;
   int1 Sent = FALSE;
   message_struct_t tMessage;
   lora_rx_message_t rMessage;
   message_struct_t *rMessagePtr;
   flash_stuct_t Flash;
   flash_rate_t SendRate = FLASH_RATE_1HZ;
   TICK cTick;
   
   //Initialize the LoRa P2P driver, this always need called before any other
   //LoRa P2P functions.  lora_init() also makes necessary calls to initialize
   //the RN2903 module.
   Initialized = lora_init();
   
   if(Initialized)
   {
      output_high(PIN_LED_GREEN);
      
      InitHW();
      
      pLevel[PB1_IDX] = input(PB1_PIN);
      pLevel[PB2_IDX] = input(PB2_PIN);
      
      rMessagePtr = &rMessage;
      Flash.Rate = FLASH_RATE_OFF;
   }
   else
      output_high(PIN_LED_RED);

   while(TRUE)
   {
      if(Initialized)
      {
         //LoRa P2P task function to maintain state machine for sending and 
         //receiving messages.
         lora_task();
         
         //Check if a new message has been received, since this device is setup
         //as a listening end-device it is always listen for a message.  The
         //only exception to this is after it receive a message meant for it.  
         //In that case it will wait for LORA_RX_TIME milliseconds for a
         //message to send to be loaded into it's TX buffer, during that time
         //it's not listening for messages.
         if(lora_has_message())
         {
            output_low(PIN_LED_YELLOW);
            output_low(PIN_LED_RED);
            
            //Retrieve message from software buffer.
            lora_get_message(&rMessage);
            
            switch(rMessagePtr->Type)
            {
               case MESSAGE_TYPE_NACK:
                  output_high(PIN_LED_RED);
                  
                  if(Sent)
                     Sent = FALSE;
                  break;
               case MESSAGE_TYPE_ACK:
                  if(Sent)
                  {
                     if(tMessage.Type == MESSAGE_TYPE_RATE)
                     {
                        if(++SendRate > FLASH_RATE_5HZ)
                           SendRate = FLASH_RATE_OFF;
                     }
                  
                     Sent = FALSE;
                  }
                  break;
               case MESSAGE_TYPE_ADC:
                  Reading = make16(rMessagePtr->Data[1], rMessagePtr->Data[0]);
                  
                  pwm_set_duty(Reading);
                  
                  tMessage.Address = rMessagePtr->Address;
                  tMessage.Type = MESSAGE_TYPE_ACK;
                  tMessage.Length = 0;
                  
                  //Send acknowledge message back
                  PutMessage(&tMessage);
                  break;
               case MESSAGE_TYPE_RATE:
                  if(rMessagePtr->Data[0] <= FLASH_RATE_5HZ)
                  {
                     Flash.Rate = (flash_rate_t)rMessagePtr->Data[0];
                  
                     switch(Flash.Rate)
                     {
                        case FLASH_RATE_OFF:
                           if(Flash.LedOn == FALSE)
                           {
                              pwm_on();
                              
                              Flash.LedOn = TRUE;
                           }
                           break;
                        case FLASH_RATE_1HZ:
                           Flash.Ticks = (TICKS_PER_SECOND / 2);
                           break;
                        case FLASH_RATE_2HZ:
                           Flash.Ticks = (TICKS_PER_SECOND / 4);
                           break;
                        case FLASH_RATE_3HZ:
                           Flash.Ticks = (TICKS_PER_SECOND / 6);
                           break;
                        case FLASH_RATE_4HZ:
                           Flash.Ticks = (TICKS_PER_SECOND / 8);
                           break;
                        case FLASH_RATE_5HZ:
                           Flash.Ticks = (TICKS_PER_SECOND / 10);
                           break;
                     }
                  
                     tMessage.Type = MESSAGE_TYPE_ACK;
                  }
                  else
                     tMessage.Type = MESSAGE_TYPE_NACK;
                  
                  tMessage.Address = rMessagePtr->Address;
                  tMessage.Length = 0;
                  
                  //Send acknowledge message back
                  PutMessage(&tMessage);
                  break;
            }
         }
      
         if(!Sent)
         {
            //Check if Push Button B4 was pressed
            cLevel[PB1_IDX] = input(PB1_PIN);
            
            if(cLevel[PB1_IDX] != pLevel[PB1_IDX])
            {
               delay_us(50);
               
               if(input(PB1_PIN) != pLevel[PB1_IDX])
               {
                  if(cLevel[PB1_IDX] == BUTTON_PRESSED)
                  {
                     //Make sure it's not busy sending another message. Because
                     //this is a listening end-device it's possible it could
                     //be busy responding to a message received from the other
                     //device.
                     if(lora_busy() == FALSE)
                     {
                        Reading = read_adc();
                        
                        tMessage.Address = OTHER_DEVICES_ADDR;
                        tMessage.Type = MESSAGE_TYPE_ADC;
                        memcpy(tMessage.Data, &Reading, 2);
                        tMessage.Length = 2;
                        
                        //Send ADC reading to other board, this controls
                        //brightness of other board's big red LED.
                        if(PutMessage(&tMessage))
                        {
                           Sent = TRUE;
                           
                           output_high(PIN_LED_YELLOW);
                        }
                        else
                           output_high(PIN_LED_RED);
                     }
                  }
                  
                  pLevel[PB1_IDX] = cLevel[PB1_IDX];
               }
            }
            
            //Check if Push Button D3 was pressed
            cLevel[PB2_IDX] = input(PB2_PIN);
            
            if(cLevel[PB2_IDX] != pLevel[PB2_IDX])
            {
               delay_us(50);
               
               if(input(PB2_PIN) != pLevel[PB2_IDX])
               {
                  if(cLevel[PB2_IDX] == BUTTON_PRESSED)
                  {
                     //Make sure it's not busy sending another message. Because
                     //this is a listening end-device it's possible it could
                     //be busy responding to a message received from the other
                     //device, or if both push buttons were pressed at same
                     //time it could be sending the ADC message.
                     if(lora_busy() == FALSE)
                     {
                        tMessage.Address = OTHER_DEVICES_ADDR;
                        tMessage.Type = MESSAGE_TYPE_RATE;
                        tMessage.Data[0] = SendRate;
                        tMessage.Length = 1;
                        
                        //Send Rate message to board this controls the blink
                        //rate of the other board's big red LED, 0 to 5 Hz.
                        if(PutMessage(&tMessage))
                        {
                           Sent = TRUE;
                           
                           output_high(PIN_LED_YELLOW);
                        }
                        else
                           output_high(PIN_LED_RED);
                     }
                  }
                  
                  pLevel[PB2_IDX] = cLevel[PB2_IDX];
               }
            }
         
         }
         else
         {
            //Check if driver is busy sending/receive a message.  If not busy
            //and response wasn't received try resending.  The most likely cause
            //that this if() will evaluate as TRUE is if the channels got out
            //of sync, one of the prototyping boards were powered cycled for
            //example.
            if(lora_busy() == FALSE)
            {
               //Resend the last message.
               if(PutMessage(&tMessage) == FALSE)
               {
                  Sent = FALSE;
                  
                  output_low(PIN_LED_YELLOW);
                  output_high(PIN_LED_RED);
               }
            }
            
         }
         
         cTick = TickGet();
         
         if(Flash.Rate != FLASH_RATE_OFF)
         {
            if(TickDifference(cTick, Flash.Tick) >= Flash.Ticks)
            {
               if(Flash.LedOn)
                  pwm_off();
               else
                  pwm_on();
               
               Flash.LedOn++;
               Flash.Tick = cTick;
            }
         }
         else
            Flash.Tick = cTick;
      }
   }
}
