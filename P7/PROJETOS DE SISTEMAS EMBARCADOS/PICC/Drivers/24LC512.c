///////////////////////////////////////////////////////////////////////////
////   Library for a 24LC512 serial EEPROM                             ////
////                                                                   ////
////   init_ext_eeprom();    Call before the other functions are used  ////
////                                                                   ////
////   write_ext_eeprom(a, d);  Write the byte d to the address a      ////
////                                                                   ////
////   write_ext_eeprom_bytes(ee, p, n) - Write 'n' bytes from 'p' to  ////
////     EEPROM address 'ee'.  Properly handles page boundries.        ////
////                                                                   ////
////   d = read_ext_eeprom(a);   Read the byte d from the address a    ////
////                                                                   ////
////   read_ext_eeprom_bytes(p, ee, n) - Read 'n' bytes from EEROM     ////
////     address 'ee' and saves to RAM pointer 'p'.                    ////
////                                                                   ////
////   The main program may define eeprom_sda                          ////
////   and eeprom_scl to override the defaults below.                  ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
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

#ifndef __24LC512_C__
#define __24LC512_C__

#ifndef SHARED_STREAM_I2C_EEPROM
   #ifndef EEPROM_SDA
      #define EEPROM_SDA  PIN_B1
      #define EEPROM_SCL  PIN_B0
   #endif

   #use i2c(master, sda=EEPROM_SDA, scl=EEPROM_SCL, stream=STREAM_I2C_EEPROM, NOINIT)
   #define SHARED_STREAM_I2C_EEPROM STREAM_I2C_EEPROM
   #define __24LC512_ISNT_SHARED
#endif

#ifndef EEPROM_SLAVE_ADDRESS
   #define EEPROM_SLAVE_ADDRESS  0xA0
#endif

#ifndef EEPROM_ADDRESS
#define EEPROM_ADDRESS     unsigned int16 //proper setting for 24LC512
#endif

#ifndef EEPROM_SIZE
#define EEPROM_SIZE        (64*1024)   //proper setting for 24LC512
#endif

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE   128      //proper setting for 24LC512
#endif

#include <stddef.h>

void init_ext_eeprom()
{
  #if defined(__24LC512_ISNT_SHARED)
   output_float(EEPROM_SCL);
   output_float(EEPROM_SDA);
   i2c_init(SHARED_STREAM_I2C_EEPROM);
  #endif
}

void write_ext_eeprom_bytes(EEPROM_ADDRESS locEE, unsigned int8 *pRAM, size_t n)
{
   EEPROM_ADDRESS block;
   unsigned int8 status;
   size_t i;
   
   if (!n)
      return;
   
   block = locEE & ~((EEPROM_ADDRESS)EEPROM_PAGE_SIZE - 1);
   i = EEPROM_PAGE_SIZE - (locEE - block);
      
   while (n)
   {
      if (i > n)
         i = n;
      
      n -= i;
      
      i2c_start(SHARED_STREAM_I2C_EEPROM);
      i2c_write(SHARED_STREAM_I2C_EEPROM, EEPROM_SLAVE_ADDRESS);
      i2c_write(SHARED_STREAM_I2C_EEPROM, make8(locEE, 1));
      i2c_write(SHARED_STREAM_I2C_EEPROM, make8(locEE, 0));
      
      locEE += i;
      
      while(i--)
      {
         i2c_write(SHARED_STREAM_I2C_EEPROM, *pRAM++);
      }
      i2c_stop(SHARED_STREAM_I2C_EEPROM);

      do
      {
         i2c_start(SHARED_STREAM_I2C_EEPROM);
         status = i2c_write(SHARED_STREAM_I2C_EEPROM, EEPROM_SLAVE_ADDRESS);
      } while(status==1);     
     
      i = EEPROM_PAGE_SIZE;
   }
}

void write_ext_eeprom(EEPROM_ADDRESS locEE, unsigned int8 data)
{
   unsigned int8 status;
   
   i2c_start(SHARED_STREAM_I2C_EEPROM);
   i2c_write(SHARED_STREAM_I2C_EEPROM, EEPROM_SLAVE_ADDRESS);
   i2c_write(SHARED_STREAM_I2C_EEPROM, make8(locEE, 1));
   i2c_write(SHARED_STREAM_I2C_EEPROM, make8(locEE, 0));
   i2c_write(SHARED_STREAM_I2C_EEPROM, data);
   i2c_stop(SHARED_STREAM_I2C_EEPROM);

   do
   {
      i2c_start(SHARED_STREAM_I2C_EEPROM);
      status = i2c_write(SHARED_STREAM_I2C_EEPROM, EEPROM_SLAVE_ADDRESS);
   } while(status==1);
   i2c_stop(SHARED_STREAM_I2C_EEPROM);
}


void read_ext_eeprom_bytes(unsigned int8 *pRAM, EEPROM_ADDRESS locEE, size_t n)
{
   i2c_start(SHARED_STREAM_I2C_EEPROM);
   i2c_write(SHARED_STREAM_I2C_EEPROM, EEPROM_SLAVE_ADDRESS);
   i2c_write(SHARED_STREAM_I2C_EEPROM, make8(locEE, 1));
   i2c_write(SHARED_STREAM_I2C_EEPROM, make8(locEE, 0));
   i2c_start(SHARED_STREAM_I2C_EEPROM);
   i2c_write(SHARED_STREAM_I2C_EEPROM, EEPROM_SLAVE_ADDRESS|1);
   while(n--)
   {
      *pRAM++ = i2c_read(SHARED_STREAM_I2C_EEPROM, (n!=0));
   }
   i2c_stop(SHARED_STREAM_I2C_EEPROM);
}

unsigned int8 read_ext_eeprom(EEPROM_ADDRESS address) {
   unsigned int8 ret;
   read_ext_eeprom_bytes(&ret, address, 1);
   return(ret);
}

#endif
