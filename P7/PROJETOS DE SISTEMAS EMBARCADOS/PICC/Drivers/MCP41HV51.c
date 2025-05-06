//////////////////// Driver for MCP41HV51 Digital Pot Converter /////////
////                                                                 ////
//// MCP41HV51_init(command)  Inits the pot                          ////
////                                                                 ////
////                                                                 ////
//// set_MCP41HV51_pot(position)  - sets the pot position 0-255      ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 2016 Custom Computer Services              ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////


#ifndef MCP41HV51_SCLK
   #if defined(__PCD__)
      #define  MCP41HV51_SHTDN PIN_D0
      #define  MCP41HV51_CS    PIN_D3
      #define  MCP41HV51_SDI   PIN_D1
      #define  MCP41HV51_SCLK  PIN_D2
   #else
      #define  MCP41HV51_SHTDN PIN_B0
      #define  MCP41HV51_CS    PIN_B4
      #define  MCP41HV51_SDI   PIN_B1
      #define  MCP41HV51_SCLK  PIN_B2
   #endif
#endif

#use spi(master, do=MCP41HV51_SDI, clk=MCP41HV51_SCLK, enable=MCP41HV51_CS, mode=0, baud=1000000, stream=STREAM_MCP41HV51, bits=16)

#define SPI_XFER_MCP41HV51(x)   spi_xfer(STREAM_MCP41HV51, x)

#define MCP41HV51_NORMAL     0xFF
#define MCP41HV51_P0A        0x04
#define MCP41HV51_P0W        0x02
#define MCP41HV51_P0B        0x01
#define MCP41HV51_ON         0x08
#define MCP41HV51_SHUTDOWN   0x00


//////////////////////////////////////////////////////////////////////////////////
// MCP41HV51_init()
// Purpose: To initialize the MCP41HV51.
// Parameters: address - Optional parameter for specifying the command.
// Returns:    Nothing.
//////////////////////////////////////////////////////////////////////////////////
void MCP41HV51_init(unsigned int8 command=MCP41HV51_NORMAL)
{
   SPI_XFER_MCP41HV51(0x4000 | command);
   output_high(MCP41HV51_SHTDN);
}

//////////////////////////////////////////////////////////////////////////////////
// set_MCP41HV51_pot()
// Purpose: To set the pot position.
// Parameter: position - 0 to 255 
// Returns:   nothing
//////////////////////////////////////////////////////////////////////////////////
void set_MCP41HV51_pot(unsigned int8 position)
{
   SPI_XFER_MCP41HV51(position);
}

