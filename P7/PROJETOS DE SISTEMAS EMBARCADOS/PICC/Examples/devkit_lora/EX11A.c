///////////////////////////////////////////////////////////////////////////
////                              EX11A.c                              ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit that uses CCS's        ////
////  LoRaWAN driver to join and communicate on a LoRaWAN network.     ////
////                                                                   ////
////  This exercise is an example of using the Activation By           ////
////  Personalization (ABP) to join the LoRaWAN network.  Once joined  ////
////  the program will periodically send the current Temperature,      ////
////  Light sensor and both POT readings to the network.  To function  ////
////  correctly this example requires a LoRaWAN gateway.               ////
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

/*
   Optional if defined it uses the ICD for the PC serial communication for this
   example.  If this is defined then the program should be programmed with the
   ICD because after program it auto launches the Serial Input/Output Monitor 
   program setup correctly for communicating through the ICD.
*/
//#define  USE_ICD_FOR_SERIAL

#include <devkit_lora.h>

#include <tick.c>
#define TickDifference(a,b)         (a-b)

#include <lorawan.h>

#define LORAWAN_JOIN_TYPE              LORAWAN_JOIN_TYPE_ABP
#define LORAWAN_MINIMUM_CHANNEL        0
#define LORAWAN_MAXIMUM_CHANNEL        7

#define  LORAWAN_DEFAULT_DEVICE_ADDR      0x12345678
const uint8_t LORAWAN_DEFAULT_NWK_SESSION_KEY[LORAWAN_KEY_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                                                                   0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
const uint8_t LORAWAN_DEFAULT_APP_SESSION_KEY[LORAWAN_KEY_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                                                   0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

#define _lorawan_tick_t                   TICK
#define _LORAWAN_TICKS_PER_SECOND         TICKS_PER_SECOND
#define _lorawan_tick_get                 TickGet
#define _lorawan_tick_difference          TickDifference

#include <rn2903.c>
#include <lorawan.c>

#define SENSOR_UPDATE_RATE       60    //in seconds
#define SENSOR_UPDATE_TICKS      (SENSOR_UPDATE_RATE * TICKS_PER_SECOND)

#include <thermistor.c>

typedef struct
{
   int16_t Temperature;    //in tenths of a °F
   uint16_t Light;         //raw 10-bit ADC Reading
   uint16_t Pot0;          //in thousands of a Volt
   uint16_t Pot1;          //in thousands of a Volt
} sensor_readings_t;

typedef enum
{
   SENSOR_CH_POT0,
   SENSOR_CH_POT1,
   SENSOR_CH_LIGHT,
   SENSOR_CH_TERMISTOR
} sensor_ch_t;

void InitHw(void)
{
   setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_20);
   
   //Part Number: NCP21XM472J03RA
   thermistor_config(25, 4700, 3500, FALSE, 1000, 10);
}

uint16_t GetVoltage(uint16_t aReading)
{
   uint16_t Result;
   
   Result = ((uint32_t)aReading * 3300) / 1023;
   
   return(Result);
}

void ReadSensors(sensor_readings_t *Sensor)
{
   sensor_ch_t channel;
   uint16_t Reading;
   
   for(channel=SENSOR_CH_POT0;channel<=SENSOR_CH_TERMISTOR;channel++)
   {
      set_adc_channel(channel);
      delay_us(50);
      
      Reading = read_adc();
      
      switch(channel)
      {
         case SENSOR_CH_POT0:
            Sensor->Pot0 = GetVoltage(Reading);
            break;
         case SENSOR_CH_POT1:
            Sensor->Pot1 = GetVoltage(Reading);
            break;
         case SENSOR_CH_LIGHT:
            Sensor->Light = Reading;
            break;
         case SENSOR_CH_TERMISTOR:
            Sensor->Temperature = thermistor_convert(Reading);
            break;
      }
   }
}

char *PrintJoinState(lorawan_join_state_t State)
{
   static char JoinStr[30];
   
   switch(State)
   {
      case LORAWAN_JOIN_STATE_NOT_JOINED:
         sprintf(JoinStr, "Not Joined");
         break;
      case LORAWAN_JOIN_STATE_JOINING:
         sprintf(JoinStr, "Joining");
         break;
      case LORAWAN_JOIN_STATE_JOINED:
         sprintf(JoinStr, "Joined");
         break;
      case LORAWAN_JOIN_STATE_ERROR:
         sprintf(JoinStr, "Error");
         break;
      case LORAWAN_JOIN_STATE_ERROR_NOT_INIT:
         sprintf(JoinStr, "Join Info Not Initialized");
         break;
      case LORAWAN_JOIN_STATE_DENIED:
         sprintf(JoinStr, "Denied");
         break;
      default:
         sprintf(JoinStr, "Unknown");
         break;
   }
   
   return(JoinStr);
}

char *PrintTxError(lorawan_tx_error_t Error)
{
   static char ErrorStr[10];
   
   switch(Error)
   {
      case LORAWAN_TX_ERROR_OK:
         sprintf(ErrorStr, "None");
         break;
      case LORAWAN_TX_ERROR_PARAM:
         sprintf(ErrorStr, "Parameter");
         break;
      case LORAWAN_TX_ERROR_LENGTH:
         sprintf(ErrorStr, "Length");
         break;
      case LORAWAN_TX_ERROR_ERROR:
         sprintf(ErrorStr, "Error");
         break;
      default:
         sprintf(ErrorStr, "Unknown");
         break;
   }
   
   return(ErrorStr);
}

void main(void)
{
   int1 Initialized;
   TICK cTick, SensorTick;
   sensor_readings_t SensorReading;
   lorawan_rx_message_t rMessage;
   uint8_t i;
   lorawan_join_state_t JoinState, pJoinState = LORAWAN_JOIN_STATE_NOT_JOINED;
   lorawan_tx_error_t TxError, pTxError = LORAWAN_TX_ERROR_OK;
   
   delay_ms(10);
   
   //Initialize the LoRaWAN driver, must be called before LoRaWAN functions
   Initialized = lorawan_init();
   
   user_printf(user_putc, "\r\nEX11A.c (LoRaWAN ABP example) - %s\r\n", getenv("DEVICE"));
   
   if(Initialized)
   {
      output_high(PIN_LED_GREEN);
      
      InitHw();
      
      SensorTick = TickGet();
   }
   else
   {
      output_high(PIN_LED_RED);
      
      user_printf(user_putc, "   Failed to Initialize lorawan.c driver\r\n");
   }

   while(TRUE)
   {
      if(Initialized)
      {
         //Maintain LoRaWAN state machine
         lorawan_task();
         
         //Get current join state
         JoinState = lorawan_get_join_state();
         
         //If join state changed print the current state
         if(JoinState != pJoinState)
         {
            user_printf(user_putc, "LoRaWAN Join State - %s\r\n", PrintJoinState(JoinState));
            
            if(JoinState == LORAWAN_JOIN_STATE_ERROR_NOT_INIT)
            {
               user_printf(user_putc, "LoRaWAN Init Join Info - ");
            
               if(lorawan_init_join_info() == TRUE)
                  user_printf(user_putc, "Successful");
               else
                  user_printf(user_putc, "Failed");
               
               user_printf(user_putc, "\r\n");
            }
            
            pJoinState = JoinState;
         }
         
         //Get Current TX error
         TxError = lorawan_get_tx_error();
         
         //If error state changed pint it out
         if(TxError != pTxError)
         {
            user_printf(user_putc, "LoRaWAN Tx Error - %s\r\n", PrintTxError(TxError));
            
            pTxError = TxError;
         }
         
         //Check if a message was received
         if(lorawan_has_message())
         {
            //Get message
            lorawan_get_message(&rMessage);
            
            //Print message
            user_printf(user_putc, "Received Message - Port: %u, Length: %u", rMessage.Port, rMessage.Length);
            
            if(rMessage.Length > 0)
            {
               user_printf(user_putc, ", Data:");
               for(i=0;i<rMessage.Length;i++)
                  user_printf(user_putc, " %02X", rMessage.Data[i]);
            }
            
            user_printf(user_putc, "\r\n");
         }
         
         cTick = TickGet();
         
         //Check is new message should be transmitted
         if(TickDifference(cTick, SensorTick) >= SENSOR_UPDATE_TICKS)
         {
            //Read sensors
            ReadSensors(&SensorReading);
            
            //Check is joined to a network
            if(lorawan_get_join_state() == LORAWAN_JOIN_STATE_JOINED)
            {
               //Transmit message
               if(lorawan_put_message(1, (uint8_t *)SensorReading, sizeof(sensor_readings_t), TRUE) == FALSE)
               {
                  user_printf("Error occurred sending message\r\n");
               }
            }
            
            user_printf(user_putc, "Temperature: %3.1w F, Light: %LX, Pot0: %5.3w V, Pot1: %5.3w V\r\n", 
                           SensorReading.Temperature,
                           SensorReading.Light,
                           SensorReading.Pot0,
                           SensorReading.Pot1
                       );
            
            SensorTick = cTick;
         }
      }
   }
}
