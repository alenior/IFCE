//////////////////// Driver for MCP45HV51 Digital Pot Converter /////////
////                                                                 ////
//// MCP45HV51_init(command)  Inits the pot                          ////
////                                                                 ////
////                                                                 ////
//// set_MCP45HV51_pot(position)  - sets the pot position 0-255      ////
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


#ifndef MCP45HV51_SCL
   #if defined(__PCD__)
      #define  MCP45HV51_SDA   PIN_D1
      #define  MCP45HV51_SCL  PIN_D2
   #else
      #define  MCP45HV51_SDA   PIN_B1
      #define  MCP45HV51_SCL   PIN_B2
   #endif
#endif

#use i2c(master, SDA=MCP45HV51_SDA, SCL=MCP45HV51_SCL, stream=STREAM_MCP45HV51)

#define MCP45HV51_NORMAL     0xFF
#define MCP45HV51_P0A        0x04
#define MCP45HV51_P0W        0x02
#define MCP45HV51_P0B        0x01
#define MCP45HV51_ON         0x08
#define MCP45HV51_SHUTDOWN   0x00


//////////////////////////////////////////////////////////////////////////////////
// MCP45HV51_init()
// Purpose: To initialize the MCP45HV51.
// Parameters: address - Optional parameter for specifying the command.
// Returns:    Nothing.
//////////////////////////////////////////////////////////////////////////////////
void MCP45HV51_init(unsigned int8 command,int8 address=0)
{
   I2C_START(STREAM_MCP45HV51);
   I2C_WRITE(0X78+address*2);
   I2C_WRITE(0x40);
   I2C_WRITE(command);
   I2C_STOP(STREAM_MCP45HV51);
}

//////////////////////////////////////////////////////////////////////////////////
// set_MCP45HV51_pot()
// Purpose: To set the pot position.
// Parameter: position - 0 to 255 
// Returns:   nothing
//////////////////////////////////////////////////////////////////////////////////
void set_MCP45HV51_pot(unsigned int8 position,int8 address=0)
{
   I2C_START(STREAM_MCP45HV51);
   I2C_WRITE(0X78+address*2);
   I2C_WRITE(0);
   I2C_WRITE(position);
   I2C_STOP(STREAM_MCP45HV51);
}

