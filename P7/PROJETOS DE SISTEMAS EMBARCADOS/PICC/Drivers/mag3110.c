////////////////////////////////////////////////////////////////////////////
////                                                                    ////
////                                mag3110.c                           ////
////                                                                    ////
//// Driver for I2C magnetic compass.                                   ////
////                                                                    ////
//// API:                                                               ////
//// -----------------------------------------------------------------  ////
////                                                                    ////
//// Mag3110Init()                                                      ////
////  Initialize device and library.  This needs to be called (once)    ////
////  before any other functions.  Will also put unit into active mode, ////
////  performing ADC conversions.                                       ////
////                                                                    ////
//// Mag3110Stop()                                                      ////
////  Put unit in standby mode.  Needed if you need to change unit's    ////
////  configuration.  Will stop ADC conversions.                        ////
////                                                                    ////
//// Mag3110Start()                                                     ////
////  Put unit into active mode, performing ADC conversions.            ////
////                                                                    ////
//// ready = Mag3110IsReady()                                           ////
////  Returns TRUE if there is axis data in the holding registers on    ////
////  the MAG3110 device.                                               ////
////                                                                    ////
//// error = Mag3110CalibrationSet(*pAxisMin, *pAxisMax)                ////
////  While the MAG3110 provides a calibrated output, the readings have ////
////  an offset that needs to be removed.  Calling this will write the  ////
////  proper offsets to the MAG3110 so future reads have the offset     ////
////  removed.  pAxisMin and pAxisMax point to data that contains       ////
////  minimum and maximum axis readings during a user calibration       ////
////  process.  A good way to do this is to read samples while user     ////
////  rotates (around Z axis, so chip is facing up) and finding the     ////
////  min and max values.  For an example of this process see           ////
////  ex_mag3110.c example.                                             ////
////                                                                    ////
//// error = Mag3110Read(*pAxis)                                        ////
////  Read axis data from the compass.  'pAxis' is a pointer to         ////
////  'mag3110_axis_t' struct.  Returns 0 if OK, non-zero if error.     ////
////                                                                    ////
//// heading = Mag3110CalculateHeading(*pAxisCurr)                      ////
////  Given the current x,y,z readings (saved to pAxisCurr), return     ////
////  heading in degrees (0-360).  Mag3110CalibrationSet() has to be    ////
////  called first to initialize calibration and scaling.               ////
////                                                                    ////
//// error = Mag3110CalibrationRemove()                                 ////
////  Erases zero offset written with Mag3110CalibrationSet().          ////
////                                                                    ////
////                                                                    ////
//// CONFIGURATION:                                                     ////
//// -----------------------------------------------------------------  ////
//// PIN_MAG3110_SCL and PIN_MAG3110_SDA                                ////
////     -or-                                                           ////
//// STREAM_I2C_MAG3110                                                 ////
////     If PIN_MAG3110_SCL and PIN_MAG3110_SDA are defined before      ////
////     including this file, it will use '#use i2c' to create the      ////
////     I2C stream to talk to this device.  If STREAM_I2C_MAG3110 is   ////
////     defined before including this file, it won't use '#use i2c'    ////
////     to createa new stream - instead it will use the existing       ////
////     STREAM_I2C_MAG3110 stream for talking to this device.          ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services            ////
////                                                                    ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler or to users who purchased the rights to use this code.    ////
//// This source code may only be distributed to other licensed users   ////
//// of the CCS C compiler or other users who purchased the rights to   ////
//// this code.  The rights to use this code can only be granted        ////
//// by CCS.  No other use, reproduction or distribution is permitted   ////
//// without written permission.  Derivative programs created using     ////
//// this software in object code form are not restricted in any way.   ////
////                                                                    ////
////                     http://www.ccsinfo.com                         ////
////////////////////////////////////////////////////////////////////////////

#ifndef __MAG3110_C__
#define __MAG3110_C__

//#define _MAG3110_DO_DEBUG

typedef struct
{
   signed int16 x;
   signed int16 y;
   signed int16 z;
} mag3110_axis_t;

#ifndef MAG3110_I2C_ADDRESS
   #define MAG3110_I2C_ADDRESS   0x1C
#endif

#ifndef STREAM_I2C_MAG3110
   #use i2c(MASTER, scl=PIN_MAG3110_SCL, sda=PIN_MAG3110_SDA, fast=400000, stream=_STREAM_I2C_MAG3110)
   #define STREAM_I2C_MAG3110 _STREAM_I2C_MAG3110
#endif

/////////////

struct
{
   float xScale;
   float yScale;
   unsigned int16 ctrl;
} _g_Mag3110;

// return 0 if ok, non-zero if error
unsigned int8 _Mag3110WriteBytes(unsigned int8 addr, unsigned int8 *pVal, int num)
{
   int noAck;
   
   i2c_start(STREAM_I2C_MAG3110);
   
   noAck = i2c_write(STREAM_I2C_MAG3110, MAG3110_I2C_ADDRESS);
   if (noAck) return noAck;
   
   noAck = i2c_write(STREAM_I2C_MAG3110, addr);
   if (noAck) return noAck;
   
   while (num--)
   {
      noAck = i2c_write(STREAM_I2C_MAG3110, *pVal++);
      if (noAck) return noAck;
   }
   
   i2c_stop(STREAM_I2C_MAG3110);
   
   return 0;
}

unsigned int8 _Mag3110WriteByte(unsigned int8 addr, unsigned int8 val)
{
   return _Mag3110WriteBytes(addr, &val, 1);
}

// return 0 if ok, non-zero if error
unsigned int8 _Mag3110ReadBytes(unsigned int8 *pVal, unsigned int8 addr, unsigned int8 n)
{
   int noAck;
   
   i2c_start(STREAM_I2C_MAG3110);
   
   noAck = i2c_write(STREAM_I2C_MAG3110, MAG3110_I2C_ADDRESS);
   if (noAck) return noAck;

   noAck = i2c_write(STREAM_I2C_MAG3110, addr);
   if (noAck) return noAck;  
  
   i2c_start(STREAM_I2C_MAG3110);
   
   noAck = i2c_write(STREAM_I2C_MAG3110, MAG3110_I2C_ADDRESS | 1);
   if (noAck) return noAck;
   
   while(n--)
   {
      *pVal++ = i2c_read(STREAM_I2C_MAG3110, n!=0);
   }
   
   i2c_stop(STREAM_I2C_MAG3110);
   
   return 0;
}

// return 0 if ok, non-zero if error
unsigned int8 _Mag3110ReadByte(unsigned int8 *pVal, unsigned int8 addr)
{
   return(_Mag3110ReadBytes(pVal, addr, 1));
}

// lsb of 'val' is written to CTRL1
// msb of 'val' is written to CTRL2
unsigned int8 Mag3110WriteControl(unsigned int16 val)
{
   int error;
   
   error = _Mag3110WriteByte(0x10, make8(val, 0));
   if (error) return error;
   
   error = _Mag3110WriteByte(0x11, make8(val, 1));
   if (error) return error;
   
   return 0;
}

int1 Mag3110ReadControl(unsigned int16 *pRet)
{
   return(_Mag3110ReadBytes((unsigned int8 *)pRet, 0x10, 2));
}

void Mag3110Stop(void)
{
   unsigned int8 oldCtrl1;
   
   #ifdef _MAG3110_DO_DEBUG
   printf("Mag3110Stop()\r\n");
   #endif
   
   _Mag3110ReadByte(&oldCtrl1, 0x10);
   
   if ((oldCtrl1 & 3) != 0)
   {
      #ifdef _MAG3110_DO_DEBUG
      printf("MAG3110 STOPPING\r\n");
      #endif
   
      _Mag3110WriteByte(0x10, oldCtrl1 & ~(unsigned int8)3);
      
      delay_ms(100);
   }
}

void Mag3110Start(void)
{
   Mag3110Stop();
   
   #ifdef _MAG3110_DO_DEBUG
   printf("Mag3110Start() ctrl=0x%LX\r\n", _g_Mag3110.ctrl);
   #endif

   Mag3110WriteControl(_g_Mag3110.ctrl);
   
   Mag3110WriteControl(_g_Mag3110.ctrl | 1);
}

// returns 1 if there is new x, y and z data to read
int1 Mag3110IsReady(void)
{
   int error;
   unsigned int8 status;
   
   error = _Mag3110ReadByte(&status, 0);
   if (error) return 0;
   
   return((status & 0x07) == 7);
}

// returns 0 if updated, non-zero if error
unsigned int8 Mag3110Read(mag3110_axis_t *pAxis)
{
   unsigned int8 read[6];
   int error;
   
   if (!Mag3110IsReady())
      return -1;
      
   error = _Mag3110ReadBytes(read, 0x01, 6);
   if (error) return error;
   
   pAxis->x = make16(read[0], read[1]);  //swap endian
   pAxis->y = make16(read[2], read[3]);  //swap endian
   pAxis->z = make16(read[4], read[5]);  //swap endian
   
   return 0;
}

//https://github.com/sparkfun/SparkFun_MAG3110_Breakout_Board_Arduino_Library
//http://www.bajdi.com/mag3110-magnetometer-and-arduino/
// returns 0 if updated, non-zero if error
unsigned int8 Mag3110CalibrationSet(mag3110_axis_t *pAxisMin, mag3110_axis_t *pAxisMax)
{
   mag3110_axis_t axis;
   union {
      unsigned int8 b[2];
      unsigned int16 w;
   } v;
   unsigned int8 error;
   
  #ifdef _MAG3110_DO_DEBUG
   printf("MAG3110_CAL MAX x=%ld y=%ld z=%ld\r\n", pAxisMax->x, pAxisMax->y, pAxisMax->z);
   printf("MAG3110_CAL MIN x=%ld y=%ld z=%ld\r\n", pAxisMin->x, pAxisMin->y, pAxisMin->z);
  #endif
   
   memcpy(&axis, pAxisMax, sizeof(mag3110_axis_t));
   axis.x += pAxisMin->x;
   axis.y += pAxisMin->y;
   axis.z += pAxisMin->z;
   
  #ifdef _MAG3110_DO_DEBUG
   printf("MAG3110_CAL RANGE x=%ld y=%ld z=%ld\r\n", axis.x, axis.y, axis.z);
  #endif

   bit_clear(axis.x, 0);
   bit_clear(axis.y, 0);
   bit_clear(axis.z, 0);

   //swap endian
   v.w = axis.x;
   axis.x = make16(v.b[0], v.b[1]);
   v.w = axis.y;
   axis.y = make16(v.b[0], v.b[1]);
   v.w = axis.z;
   axis.z = make16(v.b[0], v.b[1]);
   
   error = _Mag3110WriteBytes(0x09, (unsigned int8*)&axis, sizeof(axis));
   
   axis.x = pAxisMax->x - pAxisMin->x;
   _g_Mag3110.xScale = (float)1.0/((float)axis.x);
   
   axis.y = pAxisMax->y - pAxisMin->y;
   _g_Mag3110.yScale = (float)1.0/((float)axis.y);
   
  #ifdef _MAG3110_DO_DEBUG
   printf("MAG3110_CAL SCALE x=%e y=%e\r\n", _g_Mag3110.xScale, _g_Mag3110.yScale);
  #endif
   
  #if 0
   unsigned int8 *p;
   int i;
   p = &axis;
   printf("MAG3110 CAL_WRITE ");
   for(i=0; i<6; i++)
   {
      printf("%X ", *p++);
   }
   printf("\r\n");
   
   _Mag3110ReadBytes((unsigned int8*)&axis, 0x09, 6);
   p = &axis;
   printf("MAG3110 CAL_READ ");
   for(i=0; i<6; i++)
   {
      printf("%X ", *p++);
   }
   printf("\r\n");
  #endif
   
   return error;
}

#include <math.h>

signed int16 Mag3110CalculateHeading(mag3110_axis_t *pAxisCurr)
{
   return (signed int16)(atan2(-_g_Mag3110.yScale*pAxisCurr->y, _g_Mag3110.xScale*pAxisCurr->x) * (float)((float)180.0/(float)3.14159265358979)) + (signed int16)180;
}

// returns 0 if updated, non-zero if error
unsigned int8 Mag3110CalibrationRemove(void)
{
   int i;
   unsigned int8 error;
   
   #ifdef _MAG3110_DO_DEBUG
   printf("Mag3110CalibrationRemove()\r\n");
   #endif
   
   for (i=0; i<6; i++)
   {
      error = _Mag3110WriteByte(0x09 + i, 0);
      if (error) return error;
   }
   
   _g_Mag3110.yScale = 0;
   _g_Mag3110.xScale = 0;
   
   return 0;
}

void Mag3110Init(void)
{
   #ifdef _MAG3110_DO_DEBUG
   printf("Mag3110Init()\r\n");
   #endif
   
   output_float(PIN_MAG3110_SCL);
   output_float(PIN_MAG3110_SDA);
   
   _g_Mag3110.ctrl = 0; // DR = 0, OS = 0: 80 hz 16:1 os
   //_g_Mag3110.ctrl = 0x0018; // DR = 0, OS = 3: 80 hz 128:1 os
   //_g_Mag3110.ctrl = 0x0028; // DR = 1, OS = 1: 20 hz 32:1 os
   
   Mag3110Stop();
   
   Mag3110Start();
}

#endif   //__MAG3110_C__

