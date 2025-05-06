
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                             dht11.c                               ////
////                                                                   ////
////  Driver for DHT11, humidity sensor with one wire interface.       ////
////                                                                   ////
////  dht11_init()                                                     ////
////     Initializes the sensor for reading.                           ////
////                                                                   ////
////  dht11_read(*humidity, *tempC)                                    ////
////     Reads measurements from DHT11 and saves to humidity and tempC ////
////     pointers.  The DHT11 only updates it's values every 2         ////
////     seconds.  If the DHT11 is faulty or not connected than this   ////
////     routine will deadlock the processor.  Humidity is in          ////
////     percent and tempC in degrees celsius.                         ////
////                                                                   ////
////                            CONNECTIONS                            ////
////                                                                   ////
//// PIN_DHT11_DATA                                                    ////
////     #define this to the PIC GPIO pin connected to the DHT11's     ////
////     data pin.                                                     ////
////     Example:  #define PIN_DHT11_DATA PIN_D0                       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
//// http://www.ccsinfo.com                                            ////
///////////////////////////////////////////////////////////////////////////

/////// begin configuration ///////////////////////////////////////////////

/*
   This is the GPIO pin connected to the DHT11's data pin.
*/
#ifndef PIN_DHT11_DATA
#define PIN_DHT11_DATA  PIN_B5
#endif

/////// end configuration /////////////////////////////////////////////////



/*
   Reads one byte of data from DHT11 sensor.
   for more information about the 1-wire protocol of the DHT11,refer
   to the DHT11 datasheet. Don't call this until after you have
   sent a start signal to the DHT11.
*/
static unsigned int8 _dht11_get_byte(void)
{
   int i=8;
   unsigned int8 ret=0;
   
   while(i--)
   {
      while(!input(PIN_DHT11_DATA));
      
      delay_us(40);
      
      if(input(PIN_DHT11_DATA))
      {
         bit_set(ret,i);
         while(input(PIN_DHT11_DATA));
      }
   }
   
   return(ret);
}

void dht11_init(void)
{
   output_high(PIN_DHT11_DATA);
}

/*
   Read measurements from DHT11 sensor and save to reference parameters 
   'relativeHumidty' and 'tempC'. Sensor module doesn't update faster than
   2 seconds,so there's no point in calling this function faster than every
   2 seconds.
*/
void dht11_read(unsigned int8 *relativeHumidity, unsigned int8 *tempC)
{
   unsigned int8 data[4];
   
   //send start signal
   output_low(PIN_DHT11_DATA);
   delay_ms(30);
   output_high(PIN_DHT11_DATA);
   delay_us(40);
   output_float(PIN_DHT11_DATA);
   
   //wait for start response and beginning of first bit
   while(input(PIN_DHT11_DATA));
   while(!input(PIN_DHT11_DATA));
   while(input(PIN_DHT11_DATA));
   
   //get 4 bytes of data
   for(int i=0;i<4;i++)
      data[i]=_dht11_get_byte();
   
   if (relativeHumidity)
      *relativeHumidity = data[0];
      
   if (tempC)
      *tempC = data[2];
   
   output_high(PIN_DHT11_DATA);
}
