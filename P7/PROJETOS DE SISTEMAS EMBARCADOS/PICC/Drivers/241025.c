///////////////////////////////////////////////////////////////////////////
////   Library for a 24XX1025 serial EEPROM                            ////
////                                                                   ////
////   init_ext_eeprom();    Call before the other functions are used  ////
////                                                                   ////
////   write_ext_eeprom(a, d, c);  Write the byte d to the address a   ////
////                               for device address c                ////
////                                                                   ////
////   d = read_ext_eeprom(a, c);   Read the byte d from the address a ////
////                                for device address c               ////
////                                                                   ////
////   The main program may define eeprom_sda                          ////
////   and eeprom_scl to override the defaults below.                  ////
////                                                                   ////
////                            Pin Layout                             ////
////   -----------------------------------------------------------     ////
////   |                                                          |    ////
////   | 1: A0   Address Input 0| 8: VCC   +5V                    |    ////
////   |                        |                                 |    ////
////   | 2: A1   Address Input 1| 7: WP    Write Protect          |    ////
////   |                        |                                 |    ////
////   | 3: A2   Connect to VCC | 6: SCL   EEPROM_SCL and Pull-Up |    ////
////   |                        |                                 |    ////
////   | 4: VSS  GND            | 5: SDA   EEPROM_SDA and Pull-Up |    ////
////   -----------------------------------------------------------     ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2019 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                       http://www.ccsinfo.com                      ////
///////////////////////////////////////////////////////////////////////////                                                                                                                                                         

#ifndef EEPROM_SDA
 #define EEPROM_SDA  PIN_C4
#endif

#ifndef EEPROM_SCL
 #define EEPROM_SCL  PIN_C3
#endif

#ifndef DEFAULT_EEPROM_ADDRESS
 #define DEFAULT_EEPROM_ADDRESS     0
#endif

typedef unsigned int32 EEPROM_ADDRESS;

#define EEPROM_SIZE     0x1FFFF

#use i2c(master, sda=EEPROM_SDA, scl=EEPROM_SCL, FAST, stream=STREAM_24XX1025)

#define EEPROM_I2C_ADDRESS

/////////////////////////////
////                     ////
//// Function Prototypes ////
////                     ////
/////////////////////////////

/*
void init_ext_eeprom()
This function needs to be called before other functions are used
Param: none
Returns: none
*/
void init_ext_eeprom();
/*
void write_ext_eeprom(int32 address, BYTE data);
This function writes a byte of data to the specified address in the EEPROM
Param: EEPROM_ADDRESS : The address where data is to be written (32 bit)
       data: The byte of data to be written
       DeviceAddress: Option parameter to specify the chip address of the EEPROM
                      to write to.  If not specified defaults to DEFAULT_EEPROM_ADDRESS.
Returns: none
*/

void write_ext_eeprom(EEPROM_ADDRESS address, unsigned int8 data, unsigned int8 DeviceAddress = DEFAULT_EEPROM_ADDRESS);
/*
BYTE read_ext_eeprom(EEPROM_ADDRESS address)
This will read a byte of data from the EEPROM
Param: EEPROM_ADDRESS : The read address of the EEPROM
       DeviceAddress: Option parameter to specify the chip address of the EEPROM
                      to read from.  If not specified defaults to DEFAULT_EEPROM_ADDRESS.
Returns: Data byte
*/
unsigned int8 read_ext_eeprom(EEPROM_ADDRESS address, unsigned int8 DeviceAddress = DEFAULT_EEPROM_ADDRESS);


//////////////////////////////////////////////
////////IMPLEMENTATION////////////////////////
//////////////////////////////////////////////

void init_ext_eeprom()
{ 
}


void write_ext_eeprom(EEPROM_ADDRESS address, unsigned int8 data, unsigned int8 DeviceAddress = DEFAULT_EEPROM_ADDRESS)
{
   int1 status;
   unsigned int8 command;
   unsigned int8 wData[3];

   if (address>65535)
      command = 0xA8; // Check if address is located in Block 0/1
   else
      command = 0xA0;
   
   command |= ((DeviceAddress & 0x03) << 1);
   
   wData[0] = make8(address, 1);
   wData[1] = make8(address, 0);
   wData[2] = data;
   
   i2c_transfer(STREAM_24XX1025, command, wData, 3);

   do
   {
      status = i2c_transfer(STREAM_24XX1025, command, 0, 0);
   } while(status == 1);
}

unsigned int8 read_ext_eeprom(EEPROM_ADDRESS address, unsigned int8 DeviceAddress = DEFAULT_EEPROM_ADDRESS)
{
   unsigned int8 rData[1];
   unsigned int8 wData[2];
   unsigned int8 command;

   if (address>65535)
      command = 0xA8;
   else
      command = 0xA0;
   
   command |= ((DeviceAddress & 0x03) << 1);
   
   wData[0] = make8(address, 1);
   wData[1] = make8(address, 0);
   
   i2c_transfer(STREAM_24XX1025, command, wData, 2, rData, 1);

   return(rData[0]);
}
