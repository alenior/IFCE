///////////////////////////////////////////////////////////////////////////
////                   BMP180 BAROMETER DRIVER                         ////
////                                                                   ////
//// Driver for Bosch BMP180 digital pressure sensor.                  ////
////                                                                   ////
//// bmp180_init()                                                     ////
////     Initializes driver to communicate with sensor.                ////
////                                                                   ////
//// bmp180_read(mode, *pressure, *temp, *alt)                         ////
////     Reads sensor values and saves them to the *pressure, *temp    ////
////     and *alt pointers.  'mode' is the resolution mode, see        ////
////     bmp180_mode_t for a list of valid values. *temp is read in    ////
////     degrees C, *pressure is read in Pascal (Pa), and *alt is      ////
////     calculated in meters (m).                                     ////
////                                                                   ////
//// BMP180_NO_ALTITUDE                                                ////
////     If #defined, the bmp180_read() function will not do the       ////
////     floating point math to calculate the altitude.  This is       ////
////     useful for devices with small memory footprint that cannot    ////
////     fit the math required.                                        ////
////                                                                   ////
////                              DEFINES                              ////
////                                                                   ////
//// BMP180_STREAM                                                     ////
////     #define this to the #use i2c() stream identifier to use as    ////
////     the I2C Bus to communicate with.  This is useful when         ////
////     there are multiple device on the same I2C Bus so they can     ////
////     share the same #use i2c() code.  If not defined then the      ////
////     following two define can be used to set the SCL and SDA       ////
////     pins used for communication and this driver will add its      ////
////     own #use i2c() statement with the stream identifier set to    ////
////     BMP180_STREAM.                                                ////
////                                                                   ////
//// PIN_BMP180_SDA                                                    ////
////     #define this to the PIC GPIO pin connected to the BMP180's    ////
////     SDA pin.                                                      ////
////     Example:  #define PIN_BMP180_SDA PIN_D0                       ////
////                                                                   ////
//// PIN_BMP180_SCL                                                    ////
////     #define this to the PIC GPIO pin connected to the BMP180's    ////
////     SCL pin.                                                      ////
////     Example:  #define PIN_BMP180_SCL PIN_D1                       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2021 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

//////// begin config options /////////////////////////////////////////////

#ifndef BMP180_STREAM
 #ifndef PIN_BMP180_SDA
  #define PIN_BMP180_SDA      PIN_B4
 #endif

 #ifndef PIN_BMP180_SCL
  #define PIN_BMP180_SCL      PIN_B5
 #endif

 #use i2c(sda=PIN_BMP180_SDA, scl=PIN_BMP180_SCL, FAST, stream=BMP180_STREAM)
#endif

#ifndef BMP180_I2C_ADDRESS
 #define BMP180_I2C_ADDRESS   0xEE
#endif

/////// end config options /////////////////////////////////////////////////

#include <math.h>
#include <stdint.h>

#define BMP180_REG_EE2PROM_STA 0xAA
#define BMP180_REG_MEASURE_CTL 0xF4
#define BMP180_REG_MSB  0xF6
#define BMP180_REG_LSB  0xF7
#define BMP180_REG_XLSB 0xF8

#define BMP180_VAL_TEMP_CTL    0x2e
#define BMP180_VAL_PRES_CTL1   0x34

/*
#define MEASURE_CTL 0xF4
#define TEMP_CTL 0x2E
#define PRES_CTL0 0x34
#define PRES_CTL1 0x74
#define PRES_CTL2 0xB4
#define PRES_CTL3 0xF4
#define EE2PROM_STA 0xAA
#define EE2PROM_STO 0xBF
#define ULP_OSS 0
*/

typedef enum
{
   BMP180_MODE_LOW_POWER = 0,
   BMP180_MODE_STANDARD = 1,
   BMP180_MODE_HIGH_RESOLUTION = 2,
   BMP180_MODE_ULTRA_HIGH_RESOLUTION = 3
} bmp180_mode_t;

struct 
{
   signed int16 AC1;
   signed int16 AC2;
   signed int16 AC3;
   unsigned int16 AC4;
   unsigned int16 AC5;
   unsigned int16 AC6;
   signed int16 B1;
   signed int16 B2;
   signed int16 MB;
   signed int16 MC;
   signed int16 MD;
} _bmp180_cal;
signed int32 x1;
signed int32 x2;
signed int32 x3;
signed int32 b3;
unsigned int32 b4;
signed int32 b5;
signed int32 b6;
unsigned int32 b7;
signed int32 p;
   
/*
This method writes through the I2c to a bmp180 register
PARAMS: ADDRESS - register address
PARAMS: DATA - data to be written
RETURNS: none
*/
void bmp180_write(unsigned int8 address, unsigned int8 data)
{
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS);
   i2c_write(BMP180_STREAM, address);
   i2c_write(BMP180_STREAM, data);
   i2c_stop(BMP180_STREAM);
}

/*
This method reads a BYTE from the I2c from a bmp180 register
PARAMS: ADDRESS - register address
RETURNS: RET(The BYTE data)
*/
unsigned int8 bmp180_read8(unsigned int8 address)
{
   unsigned int8 ret=0;
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS);
   i2c_write(BMP180_STREAM, address);
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS | 1);
   ret = i2c_read(BMP180_STREAM, 0);
   i2c_stop(BMP180_STREAM);
   
   return(ret);
}

/*
This method reads 2 BYTES from the I2c from a bmp180 register
PARAMS: ADDRESS - register address
RETURNS: RET.W(The 2 BYTES data)
*/
unsigned int16 bmp180_read16(unsigned int8 address)
{
   union
   {
      unsigned int8 b[2];
      unsigned int16 w;
   } ret;
   
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS);
   i2c_write(BMP180_STREAM, address);
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS | 1);
   ret.b[1]=i2c_read(BMP180_STREAM, 1);
   ret.b[0]=i2c_read(BMP180_STREAM, 0);
   i2c_stop(BMP180_STREAM);
   
   return(ret.w);
}

/*
This method reads 3 BYTES from the I2c from a bmp180 register
PARAMS: ADDRESS - register address
RETURNS: RET.DW(The 3 BYTES of data)
*/
unsigned int32 bmp180_read24(unsigned int8 address)
{
   union
   {
      unsigned int8 b[4];
      unsigned int32 dw;
   } ret;
   
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS);
   i2c_write(BMP180_STREAM, address);
   i2c_start(BMP180_STREAM);
   i2c_write(BMP180_STREAM, BMP180_I2C_ADDRESS | 1);
   ret.b[3] = 0;
   ret.b[2] = i2c_read(BMP180_STREAM, 1);
   ret.b[1] = i2c_read(BMP180_STREAM, 1);
   ret.b[0] = i2c_read(BMP180_STREAM, 0);
   i2c_stop(BMP180_STREAM);
   
   return(ret.dw);
}

/*
This method calculates the temperature based on data received by the bmp180
PARAMS: rawTemp - temperature data read from I2C
RETURNS: calcTemp
*/
signed int32 bmp180_calculateTemperature(signed int32 rawTemp)
{
   signed int32 calcTemp;
   
   x1=(rawTemp-_bmp180_cal.AC6)*_bmp180_cal.AC5/32768;
   
   x2=_bmp180_cal.MC*2048.0/(x1+_bmp180_cal.MD);
   b5=x1+x2;
   calcTemp=(b5+8)/16;
   calcTemp/=10;
   return(calcTemp);
}

/*
This method calculates the pressure based on data received by the bmp180
PARAMS: rawPres - pressure data read from I2C
PARAMS: OSS - offset setting based on what mode the bmp180 is in
RETURNS: p
*/
signed int32 bmp180_calculatePressure(signed int32 rawPres, unsigned int8 OSS)
{

   b6=b5-4000;
   x1=((((signed int32)_bmp180_cal.B2)*(b6*b6/4096))/2048);
   x2=(signed int32)_bmp180_cal.AC2*b6/2048;
   x3=x1+x2;
   b3=((((signed int32)_bmp180_cal.AC1*4+x3)<<OSS)+2)/4;
   x1=(signed int32)_bmp180_cal.AC3*b6/8192;
   x2=((signed int32)_bmp180_cal.B1*(b6*b6/4096))/65536;
   x3=((x1+x2)+2)/4;
   b4=(signed int32)_bmp180_cal.AC4*(unsigned int32)(x3+32768)/32768;
   b7=((unsigned int32)rawPres-b3)*(50000>>OSS);
   if(b7<0x80000000)
   {
      p=(b7*2)/b4;
      }else{
      p=(b7/b4)*2;
   }
   x1=(p/256)*(p/256);
   x1=(x1*3038)/65536;
   x2=((-7357)*p)/65536;
   p=p+(x1+x2+3791)/16;
   return p;
}

/*
This method calculates the altitude based on calculated pressure
PARAMS: pressure
RETURNS: a
*/
signed int32 bmp180_calculateAltitude(signed int32 pressure)
{
   signed int32 a=0;
   float p0 = 101325.0; //average sea level pressure
   float x;
   float y;
   float z;
   
   x=((float)pressure/p0);
   y=1.0/5.255;
   z=1.0-(pow(x,y));
   z=(44330.0)*(z);
   a=((signed int32)z);
   return a;
}

/*
This method reads in calibration data for calibrating pressure and temperature
data based on the specific bmp180 used
PARAMS: none
RETURNS: none
*/
void bmp180_init()
{
   _bmp180_cal.AC1 = (bmp180_read16(BMP180_REG_EE2PROM_STA));
   
   _bmp180_cal.AC2 = (bmp180_read16(BMP180_REG_EE2PROM_STA+2));
   
   _bmp180_cal.AC3 = (bmp180_read16(BMP180_REG_EE2PROM_STA+4));
   
   _bmp180_cal.AC4 = (bmp180_read16(BMP180_REG_EE2PROM_STA+6));
   
   _bmp180_cal.AC5 = (bmp180_read16(BMP180_REG_EE2PROM_STA+8));
   
   _bmp180_cal.AC6 = (bmp180_read16(BMP180_REG_EE2PROM_STA+10));
   
   _bmp180_cal.B1 = (bmp180_read16(BMP180_REG_EE2PROM_STA+12));
   
   _bmp180_cal.B2 = (bmp180_read16(BMP180_REG_EE2PROM_STA+14));
   
   _bmp180_cal.MB = (bmp180_read16(BMP180_REG_EE2PROM_STA+16));
   
   _bmp180_cal.MC = (bmp180_read16(BMP180_REG_EE2PROM_STA+18));
   
   _bmp180_cal.MD = (bmp180_read16(BMP180_REG_EE2PROM_STA+20));
}

/*
This method reads in data for pressure and temperature from the I2C
PARAMS: mode - mode chosen by user (listed in .h file)
RETURNS: none
*/
void bmp180_read(bmp180_mode_t mode, signed int32 *pPressure, 
                         signed int32 *pTemperature, signed int32 *pAltitude)
{
   union
   {
      unsigned int8 b[4];
      unsigned int16 w[2];
      signed int32 dw;
   } scr32;
   signed int32 rawTemp=0;
   signed int32 pressure=0;
   unsigned int16 ConversionTime;
   
   if (pTemperature)
   {
      //WRITE 0X2E INTO REG 0XF4,WAIT 4.5MS
      bmp180_write(BMP180_REG_MEASURE_CTL, BMP180_VAL_TEMP_CTL);
      delay_us(4500);
      //READ REG 0XF6 AND 0XF7
      rawTemp=bmp180_read16(BMP180_REG_MSB);
      
      *pTemperature = bmp180_calculateTemperature(rawTemp);
   }
   
   if (pPressure || pAltitude)
   {
      //////////////////////////////////////////////////////////////////////
      //read uncompensated pressure value///////////////////////////////////
      //WRITE 0X34+(OSS<<6)INTO REG 0XF4,WAIT DEPENDS ON MODE
      
      switch(mode)
      {
         case BMP180_MODE_LOW_POWER:
            ConversionTime = 4500;
            break;
         case BMP180_MODE_STANDARD:
            ConversionTime = 7500;
            break;
         case BMP180_MODE_HIGH_RESOLUTION:
            ConversionTime = 13500;
            break;
         case BMP180_MODE_ULTRA_HIGH_RESOLUTION:
            ConversionTime = 25500;
            break;
      }
      
      bmp180_write(BMP180_REG_MEASURE_CTL, BMP180_VAL_PRES_CTL1 | (mode << 6));
      
      delay_us(ConversionTime);
      
      //READ REG 0XF6,0XF7 AND 0XF8
      scr32.dw = bmp180_read24(BMP180_REG_MSB);
      scr32.dw = (scr32.dw >> (8 - mode));
      
      pressure = bmp180_calculatePressure(scr32.dw, mode);
      
      if (pPressure)
      {
         *pPressure = pressure;
      }
     
      if (pAltitude)
      {
        #if !defined(BMP180_NO_ALTITUDE)
         *pAltitude = bmp180_calculateAltitude(pressure);
        #else
         *pAltitude = 0;
        #endif
      }
   }
}

