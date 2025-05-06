///////////////////////////////////////////////////////////////////////////
////                    ADXL345 ACCELEROMETER Driver                   ////
////                                                                   ////
////  adxl345_init()                                                   ////
////     Initializes the ADXL345.  Must be called once in your         ////
////     application.  Sets ADXL345 for 100Hz, full scale resolution,  ////
////      16g max.                                                     ////
////                                                                   ////
////  adxl345_read(*x, *y, *z)                                         ////
////     Reads the acceleration in the 3 axis and saves the result to  ////
////     the pointers.  x, y and z are signed int16s.  The scale is    ////
////     typically 3.9mg per bit (multiply by 0.0039 to convert to g). ////
////     1g is one gravity, or 9.8m/s^2.  The scale is not calibrated  ////
////     and can range from 3.5mg to 4.3mg.                            ////
////                                                                   ////
////  value = adxl345_read8(Addr)                                      ////
////     Used read 1 byte from an adxl345 register.  Addr is the       ////
////     register address to read.                                     ////
////                                                                   ////
////  value = adxl345_read16(Addr)                                     ////
////     Used to read 2 byes from an adxl345 register.  Addr is the    ////
////     register address to start reading the 2 bytes from.           ////
////                                                                   ////
////                            CONNECTIONS                            ////
////                                                                   ////
//// PIN_ADXL345_SDA                                                   ////
////     #define this to the PIC GPIO pin connected to the ADXL345's   ////
////     SDA pin.                                                      ////
////     Example:  #define PIN_ADXL345_SDA PIN_D0                      ////
////                                                                   ////
//// PIN_ADXL345_SCL                                                   ////
////     #define this to the PIC GPIO pin connected to the ADXL345's   ////
////     SCL pin.                                                      ////
////     Example:  #define PIN_ADXL345_SCL PIN_D1                      ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////  Version History:                                                 ////
////                                                                   ////
////     12/03/20 - Fixed issue with adxl345_read8() function that was ////
////                causing some communication issues when used to     ////
////                read a register.                                   ////
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


//////// begin config options /////////////////////////////////////////////

#ifndef PIN_ADXL345_SDA
   #define PIN_ADXL345_SDA PIN_B4
#endif

#ifndef PIN_ADXL345_SCL
   #define PIN_ADXL345_SCL PIN_B5
#endif

#ifndef ADXL345_I2C_ADDRESS
   #define ADXL345_I2C_ADDRESS 0xA6
#endif

/////// end config options /////////////////////////////////////////////////


#use i2c(sda=PIN_ADXL345_SDA, scl=PIN_ADXL345_SCL)

#define ADXL345_REG_DEVID 0X00
#define ADXL345_REG_BW_RATE 0X2C
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_DATA_FORMAT 0X31
#define ADXL345_REG_DATAX0 0X32
#define ADXL345_REG_DATAY0 0X34
#define ADXL345_REG_DATAZ0 0X36
/*
//some other ADXL345 register locations that you may find useful
#define THRESH_TAP 0X1D 
#define OFSX 0X1E
#define OFSY 0X1F
#define OFSZ 0X20
#define DUR 0X21
#define LATENT 0X22
#define WINDOW 0X23
#define THRESH_ACT 0X24
#define THRESH_INACT 0X25
#define TIME_INACT 0X26
#define ACT_INACT_CTL 0X27
#define THRESH_FF 0X28
#define TIME_FF 0X29
#define TAP_AXES 0X2A
#define ACT_TAP_STATUS 0X2B
#define POWER_CTL 0X2D
#define INT_ENABLE 0X2E
#define INT_MAP 0X2F
#define INT_SOURCE 0X30
#define DATAX1 0X33
#define DATAY1 0X35
#define DATAZ1 0X37
#define FIFO_CTL 0X38
#define FIFO_STATUS 0X39
*/

/*
This method writes through the I2C to an adxl345 register
PARAMS: ADDRESS - register address
PARAMS: DATA - data to be written
RETURNS: none
*/
void adxl345_write(unsigned int8 address, unsigned int8 data)
{
   i2c_start();
   i2c_write(ADXL345_I2C_ADDRESS);
   i2c_write(address);   
   i2c_write(data);
   i2c_stop();
}
/*
This method reads a BYTE from the I2C from an adxl345 register
PARAMS: ADDRESS - register address
RETURNS: RET(The BYTE data)
*/
unsigned int8 adxl345_read8(unsigned int8 address)
{
   unsigned int8 ret = 0; 
   i2c_start();
   i2c_write(ADXL345_I2C_ADDRESS);
   i2c_write(address);
   i2c_start();
   i2c_write(ADXL345_I2C_ADDRESS | 1);
   ret = i2c_read(0);
   i2c_stop();
   return(ret);
}
/*
This method reads 2 BYTES from the I2C from an adxl345 register
PARAMS: ADDRESS - register address
RETURNS: RET.W(The BYTE data)
*/
unsigned int16 adxl345_read16(unsigned int8 address)
{
   union
   {
      unsigned int8 b[2];
      unsigned int16 w;
   } ret;
   
   i2c_start();
   i2c_write(ADXL345_I2C_ADDRESS);
   i2c_write(address);
   i2c_start();
   i2c_write(ADXL345_I2C_ADDRESS | 1);
   ret.b[0] = i2c_read(1);
   ret.b[1] = i2c_read(0);
   i2c_stop();
   return(ret.w);
}

/*
This method initializes the adxl345 to measure data at 100Hz 
and with Full Resolution (16bits) at +/- 16g.
PARAMS: none
RETURNS: none
*/
void adxl345_init(void)
{  
   adxl345_write(ADXL345_REG_POWER_CTL,0x08);
   adxl345_write(ADXL345_REG_BW_RATE,0x0A);
   adxl345_write(ADXL345_REG_DATA_FORMAT,0X0B);
}

/*
This method reads out the X,Y and Z values of the adxl345 and 
returns them in the unit mg which is 3.9mg/LSB
PARAMS: none
RETURNS: none
*/
void adxl345_read(signed int16 *x, signed int16 *y, signed int16 *z)
{
   if (x)
   {
      *x = adxl345_read16(ADXL345_REG_DATAX0);
   }

   if (y)
   {
      *y = adxl345_read16(ADXL345_REG_DATAY0);
   }
   
   if (z)
   {
      *z = adxl345_read16(ADXL345_REG_DATAZ0);
   }
}
