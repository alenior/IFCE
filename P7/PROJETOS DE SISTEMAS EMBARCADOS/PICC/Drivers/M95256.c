///////////////////////////////////////////////////////////////////////////
////                              M95256.c                             ////
////                                                                   ////
////        Driver for an M95256 256-Kbit serial SPI bus EEPROM.       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// API: - see comments before each function for a more detailed      ////
////        description about function and it's parameters and return  ////
////        value.                                                     ////
////                                                                   ////
////  init_ext_eeprom() - Initialize external eeprom.                  ////
////                                                                   ////
////  read_ext_eeprom() - Read a byte from external eeprom.            ////
////                                                                   ////
////  write_ext_eeprom() - Write a byte to external eeprom.            ////
////                                                                   ////
////  ext_eeprom_read_id() - Read external eeprom identification.      ////
////                                                                   ////
////  ext_eeprom_busy() - Check is external eeprom is busy.            ////
////                                                                   ////
////  ext_eeprom_write_enable() - Enable or Disable external eeprom    ////
////                              writes.                              ////
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

#ifndef __M95256_C__
#define __M95256_C__

#include <stdint.h>

#ifndef EEPROM_SELECT_PIN
 #define EEPROM_SELECT_PIN    PIN_B4
#endif

#ifndef EEPROM_CLK_PIN
 #define EEPROM_CLK_PIN       PIN_C3
#endif

#ifndef EEPROM_SDI_PIN
 #define EEPROM_SDI_PIN       PIN_A4      //MOSI
#endif

#ifndef EEPROM_SDO_PIN
 #define EEPROM_SDO_PIN       PIN_A9      //MISO
#endif

#ifndef EEPROM_SIZE
 #define EEPROM_SIZE          32768       //256 kBit, 32 kByte
#endif

#ifndef EEPROM_SPI_BAUD
 #define EEPROM_SPI_BAUD      10000000    //10 MHz
#endif

#ifndef M95256_STREAM
 #use spi(MASTER, MODE=0, CLK=EEPROM_CLK_PIN, DO=EEPROM_SDI_PIN, DI=EEPROM_SDO_PIN, BITS=8, BAUD=EEPROM_SPI_BAUD, STREAM=M95256_STREAM)
 
 #define M95256_xfer(x)       spi_xfer(M95256_STREAM, x)
#else
 #define M95256_xfer(x)       spi_xfer(M95256_STREAM, x, 8)
#endif


///////////////////////////////////////////////////////////////////////////////

typedef uint16_t EEPROM_ADDRESS;

typedef enum
{
   EEPROM_CMD_WRSR=0x01,
   EEPROM_CMD_WRITE,
   EEPROM_CMD_READ,
   EEPROM_CMD_WRDI,
   EEPROM_CMD_RDSR,
   EEPROM_CMD_WREN,
   EEPROM_CMD_WRID=0x82,
   EEPROM_CMD_RDID,
   EEPROM_CMD_LID=0x82,
   EEPROM_CMD_RDLS
} EEPROM_CMD;

typedef struct
{
   uint8_t ManufactureCode;   //should be 0x20
   uint8_t FamilyCode;        //should be 0x00
   uint8_t DensityCode;       //should be 0x0F (256 Kbit)
} EEPROM_ID;

////////////////////////////////// Prototypes /////////////////////////////////

void init_ext_eeprom(void);
uint8_t read_ext_eeprom(EEPROM_ADDRESS Address);
void write_ext_eeprom(EEPROM_ADDRESS Address, uint8_t Data);
void ext_eeprom_read_id(EEPROM_ID *Id);

int1 ext_eeprom_busy(void);
void ext_eeprom_write_enable(int1 Enable=TRUE);

///////////////////////////////////// API /////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// init_ext_eeprom()
//
// Initializes the external eeprom's chip select, hold and write protect pins.
// Should be called before any other external eeprom functions are called.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void init_ext_eeprom(void)
{
   output_high(EEPROM_SELECT_PIN);
   output_drive(EEPROM_SELECT_PIN);
   
  #ifdef EEPROM_HOLD_PIN
   output_high(EEPROM_HOLD_PIN);
   output_drive(EEPROM_HOLD_PIN);
  #endif
  
  #ifdef EEPROM_WRITE_PROTECT_PIN
   output_high(EEPROM_WRITE_PROTECT_PIN);
   output_drive(EEPROM_WRITE_PROTECT_PIN);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
// read_ext_eeprom()
//
// Reads a byte from the external eeprom from the specified address.
//
// Parameters:
//    Address - the address within the external eeprom to read.
//
// Returns:
//    uint8_t - the byte read from the external eeprom or 0x00 if Address
//              was invalid.
///////////////////////////////////////////////////////////////////////////////
uint8_t read_ext_eeprom(EEPROM_ADDRESS Address)
{
   uint8_t Result;
   
   if(Address < EEPROM_SIZE)
   {
      while(ext_eeprom_busy());
      
      output_low(EEPROM_SELECT_PIN);
      
      M95256_xfer(EEPROM_CMD_READ);
      M95256_xfer(make8(Address, 1));
      M95256_xfer(make8(Address, 0));
      Result = M95256_xfer(0);
      
      output_high(EEPROM_SELECT_PIN);
   }
   else
      Result = 0;
   
   return(Result);
}

///////////////////////////////////////////////////////////////////////////////
// write_ext_eeprom()
//
// Writes a byte to the external eeprom to the specified address.  If an
// invalid address is specified nothing is written.
//
// Parameters:
//    Address - the address within the external eeprom to write.
//
//    Data - the byte to write to the specified address.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void write_ext_eeprom(EEPROM_ADDRESS Address, uint8_t Data)
{
   if(Address < EEPROM_SIZE)
   {
      //Enable writes, ext_eeprom_write_enable() calls ext_eeprom_busy() 
      ext_eeprom_write_enable(TRUE);
      
      output_low(EEPROM_SELECT_PIN);
      
      M95256_xfer(EEPROM_CMD_WRITE);
      M95256_xfer(make8(Address, 1));
      M95256_xfer(make8(Address, 0));
      M95256_xfer(Data);
      
      output_high(EEPROM_SELECT_PIN);
      
      //Disable writes
      ext_eeprom_write_enable(FALSE);
   }
}

///////////////////////////////////////////////////////////////////////////////
// ext_eeprom_read_id()
//
// Function for reading the external eeprom's identification.
//
// Parameters:
//    Id - pointer to EEPROM_ID structure to return read identification to.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void ext_eeprom_read_id(EEPROM_ID *Id)
{
   while(ext_eeprom_busy());
   
   output_low(EEPROM_SELECT_PIN);
   
   M95256_xfer(EEPROM_CMD_RDID);
   M95256_xfer(0);
   M95256_xfer(0);
   Id->ManufactureCode = M95256_xfer(0);
   Id->FamilyCode = M95256_xfer(0);
   Id->DensityCode = M95256_xfer(0);
   
   output_high(EEPROM_SELECT_PIN);
}

///////////////////////////

///////////////////////////////////////////////////////////////////////////////
// ext_eeprom_busy()
//
// Function to read the external eeprom's status register to check the WIP bit
// to determine if the eeprom is busy and unable to accept a new command.
//
// Parameters - None.
//
// Returns:
//    TRUE - external eeprom is busy.
//    FALSE - external eeprom is not busy and can accept a new command.
///////////////////////////////////////////////////////////////////////////////
int1 ext_eeprom_busy(void)
{
   uint8_t Status;
   
   output_low(EEPROM_SELECT_PIN);
   
   M95256_xfer(EEPROM_CMD_RDSR);
   Status = M95256_xfer(0);
   
   output_high(EEPROM_SELECT_PIN);
   
   return(bit_test(Status, 0));
}

///////////////////////////////////////////////////////////////////////////////
// ext_eeprom_write_enable()
//
// Function use enable or disable write functions.
//
// Parameters:
//    Enable - if TRUE writes are enabled, if FALSE writes are disabled.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void ext_eeprom_write_enable(int1 Enable=TRUE)
{
   while(ext_eeprom_busy());
   
   output_low(EEPROM_SELECT_PIN);
   
   if(Enable)
      M95256_xfer(EEPROM_CMD_WREN);
   else
      M95256_xfer(EEPROM_CMD_WRDI);
   
   output_high(EEPROM_SELECT_PIN);
}


#endif
