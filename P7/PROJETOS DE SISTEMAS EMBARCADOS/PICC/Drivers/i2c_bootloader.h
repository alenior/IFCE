///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          i2c_bootloader.h                         ////
////                                                                   ////
//// Common file used by the I2C bootloader and applications written   ////
//// compatible with the I2C bootloader.  PICmicro acts as a I2C slave ////
//// and receives bootload commands over I2C.  This file will map      ////
//// the program memory, reset and interrupt vectors to be compatible  ////
//// with the bootloader.  This file also provides documentation of    ////
//// how the I2C bootloader works and how to implement it.             ////
////                                                                   ////
//// If you want your application to listen to I2C commands to put     ////
//// it into bootloader mode, then your application needs to call      ////
//// i2c_isr_bootload_check() in the I2C Slave ISR.  See the           ////
//// ex_i2c_ldr_application.c demo for an example.                     ////
////                                                                   ////
////                                                                   ////
//// Configuration                                                     ////
//// ----------------------------------------------------------------  ////
////                                                                   ////
//// STREAM_I2C_BOOTLOADER - A stream as defined by #use i2c().        ////
////     This tells this library which I2C port and settings to use.   ////
////     Example:                                                      ////
////     #use i2c(SLAVE, I2C1, address=0xA0, STREAM=STREAM_I2C_BOOTLOADER) ////
////                                                                   ////
//// INT_I2C_BOOTLOADER - The is the CCS C Compiler syntax INT_xxxx    ////
////     to map to the interrupt vector tied to I2C slave port         ////
////     used by your #use i2c().  For 8bit compilers, this will       ////
////     be INT_SSP or INT_SSP2, etc.  For 16bit compilers, this       ////
////     will be INT_SI2C or INT_SI2C2, etc.  For more information,    ////
////     refer to the CCS C Compiler manual entries for                ////
////     #use_i2c() and INT_SSP.                                       ////
////     Example:                                                      ////
////     #define INT_I2C_BOOTLOADER INT_SSP                            ////
////                                                                   ////
//// IS_BOOTLOADER_FORCED() - This is optional.  If defined, on reset  ////
////     the bootloader will check this macro and if it returns TRUE   ////
////     then it will start the bootloader.  The application does have ////
////     a small I2C slave interrupt handler to check for a GOTO       ////
////     bootloader command; if for some reason the application        ////
////     disabled the interrupt then the bootloader would not work.    ////
////     By using this define you are giving a fail-safe method of     ////
////     forcing the PICmicro to start the bootloader, and would       ////
////     alleviate the potential for accidentally disabling the        ////
////     bootloader in your application.                               ////
////     Example:                                                      ////
////     #define IS_BOOTLOADER_FORCED() (!input(PIN_D0))               ////
////                                                                   ////
////                                                                   ////
//// Other Files                                                       ////
//// ----------------------------------------------------------------  ////
////                                                                   ////
//// ex_i2c_ldr_bootloader.c - The bootloader, that receives the       ////
////     I2C commands and writes to the flash program memory           ////
////     of the PICmicro.                                              ////
////                                                                   ////
//// ex_i2c_ldr_application.c - An example of an application that      ////
////     is compatible with the I2C bootloader.  If you are developing ////
////     an application and you want it compatible with the            ////
////     bootloader, than you can use this as a template.              ////
////     This also shows how to develop/implement an I2C Slave ISR     ////
////     to listen for the GOTO BOOTLOADER command.                    ////
////                                                                   ////
//// ex_bootloader_to_i2c.c - SIOW.EXE and CCSBOOTLOADER.EXE can       ////
////     send a HEX file to a COM port with software flow control.     ////
////     This program takes those serial messages from that            ////
////     software and converts it to the I2C protocol compatible       ////
////     with this bootloader.                                         ////
////                                                                   ////
////                                                                   ////
//// I2C Protocol                                                      ////
//// ----------------------------------------------------------------  ////
////                                                                   ////
//// If a packet is sent with R/~W bit of address clear, host sends    ////
//// a message in this packet format:                                  ////
////                                                                   ////
////          +---+------+------+-------------------------+---+        ////
////          | S | ADDY | CMD  |  Payload                | P |        ////
////          +---+------+------+-------------------------+---+        ////
////    Bytes:      1      1       n                                   ////
////                                                                   ////
////     S is the I2C start command.                                   ////
////                                                                   ////
////     ADDY is the I2C slave address (as defined in the #use i2c())  ////
////     with the R/~W bit clear.                                      ////
////                                                                   ////
////     CMD is the command that you want to execute.  Some commands   ////
////     are for writing to the memory or setting the address, some    ////
////     are for reading.  The commands are documented below.          ////
////                                                                   ////
////     PAYLOAD is any data related to this command.                  ////
////                                                                   ////
////     P is the I2C stop command.                                    ////
////                                                                   ////
//// If a packet is sent with R/~W bit of address set, host receives   ////
//// a message in this packet:                                         ////
////                                                                   ////
////          +---+------+------+-------------------------+---+        ////
////          | S | ADDY | CMD  |  Payload                | P |        ////
////          +---+------+------+-------------------------+---+        ////
////    Bytes:      1      1       n                                   ////
////                                                                   ////
////     ADDY is the I2C slave address (as defined in the #use i2c())  ////
////     with the R/~W bit clear.  This part of the packet is          ////
////     transmitted by the host, the reset of the packet is           ////
////     transmitted by the PICmicro being bootloaded.                 ////
////                                                                   ////
////     CMD is the last CMD that was written to it (see above), and   ////
////     can be used to verify what data is being read.                ////
////                                                                   ////
////     PAYLOAD is any data related to the command.                   ////
////                                                                   ////
//// THE MAXIMUM PAYLOAD SIZE THAT CAN BE RECEIVED OR SENT IS 125      ////
//// BYTES!                                                            ////
////                                                                   ////
//// If the slave is busy processing a byte (for example, writing to   ////
//// the flash) then it will hold the clock lines until ready.         ////
////                                                                   ////
//// These are the CMDs that can be sent:                              ////
////                                                                   ////
////     I2C_LOADER_COMMAND_ROM_ADDRESS = 0x01                         ////
////     Expected payload = 4 bytes (the address, in little endian)    ////
////     The program memory address to write/read.  This sets an       ////
////     internal pointer in the bootloader for future ROM             ////
////     access.  Reading this will read the current pointer.          ////
////                                                                   ////
////     I2C_LOADER_COMMAND_ROM_ACCESS = 0x02                          ////
////     Read/write data to the program memory.  Each byte sent        ////
////     or received increments the ROM access pointer by 1.           ////
////                                                                   ////
////     I2C_LOADER_COMMAND_GOTO_APPLICATION = 0x03                    ////
////     Expected payload = 2 bytes (magic key, in little endian)      ////
////     Reception of this command will cause the bootloader to        ////
////     finish and then goto the application.  It will also finish    ////
////     any pending writes to the program memory flash.               ////
////     This is a write only command, you cannot read this.           ////
////                                                                   ////
////     I2C_LOADER_COMMAND_GOTO_BOOTLOADER = 0x04                     ////
////     Expected payload = 2 bytes (magic key, in little endian)      ////
////     Reception of this command will cause the application to       ////
////     stop running and start the bootloader.  You can verify        ////
////     the bootloader is running by reading this, you should then    ////
////     get the magic key.                                            ////
////                                                                   ////
////     If you wanted to verify that the bootloader is running,       ////
////     read this command and verify that the read value is the       ////
////     magic key.                                                    ////
////                                                                   ////
//// The magic key is 0xAA55.                                          ////
////                                                                   ////
////                                                                   ////
//// I2C Examples                                                      ////
//// ----------------------------------------------------------------  ////
////                                                                   ////
//// All these examples assume a slave address of 0xA0.                ////
////                                                                   ////
//// Start the bootloader, and read the CMD back to verify that        ////
//// it started:                                                       ////
////                                                                   ////
////   +---+------+------+-------------+---+                           ////
////   | S | ADDY | CMD  |  Payload    | P |                           ////
////   +---+------+------+-------------+---+                           ////
////   | S | 0xA0 | 0x04 | 0x55 | 0xAA | P |                           ////
////   +---+------+------+-------------+---+                           ////
////                                                                   ////
////   +---+------+------+-------------+---+                           ////
////   | S | ADDY | CMD  |  Payload    | P |                           ////
////   +---+------+------+-------------+---+                           ////
////   | S | 0xA1 | 0x04 | 0x55 | 0xAA | P |                           ////
////   +---+------+------+-------------+---+                           ////
////                                                                   ////
//// Write 0x00 0x01 0x02 0x03 to address 0x1122:                      ////
////                                                                   ////
////   +---+------+------+---------------------------+---+             ////
////   | S | ADDY | CMD  |  Payload                  | P |             ////
////   +---+------+------+---------------------------+---+             ////
////   | S | 0xA0 | 0x01 | 0x22 | 0x11 | 0x00 | 0x00 | P |             ////
////   +---+------+------+---------------------------+---+             ////
////                                                                   ////
////   +---+------+------+---------------------------+---+             ////
////   | S | ADDY | CMD  |  Payload                  | P |             ////
////   +---+------+------+---------------------------+---+             ////
////   | S | 0xA0 | 0x02 | 0x00 | 0x01 | 0x02 | 0x03 | P |             ////
////   +---+------+------+---------------------------+---+             ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2021 Custom Computer Services           ////
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

#ifndef __I2C_BOOTLOADER_COMMON_H__
#define __I2C_BOOTLOADER_COMMON_H__

#ifndef LOADER_END
 #if defined(__PCM__)
   #define LOADER_END 0x33F
 #elif defined(__PCH__)
  #define FLASH_SIZE getenv("FLASH_ERASE_SIZE")
  #if ((0x500 % FLASH_SIZE) == 0)         //IF 0x500 is even flash boundary
   #define LOADER_END   0x4FF
  #else                                  //ELSE, goto next even boundary
   #define LOADER_END   ((0x500+FLASH_SIZE-(0x500 % FLASH_SIZE))-1)
  #endif
 #else
  #error Bootloader only works with PCM or PCH compiler
 #endif
#endif

/// end configuration

#define PROGRAM_LOADER_SIZE   LOADER_END

#define PROGRAM_APP_START  (PROGRAM_LOADER_SIZE+1)
#if defined(__PCM__)
 #define PROGRAM_BOOTLOADER_START 9
 #define PROGRAM_ISR_START (PROGRAM_APP_START+4)
#else
 #define PROGRAM_BOOTLOADER_START 0x28
 #define PROGRAM_ISR_START (PROGRAM_APP_START+8)
#endif

#define I2C_LOADER_COMMAND_ROM_ADDRESS 0x01
#define I2C_LOADER_COMMAND_ROM_ACCESS  0x02
#define I2C_LOADER_COMMAND_GOTO_APPLICATION 0x03
#define I2C_LOADER_COMMAND_GOTO_BOOTLOADER 0x04
#define I2C_LOADER_MAGIC   0xAA55

#if defined(__bootloader)
   // mappings and code for the bootloader
   
   #org PROGRAM_ISR_START+0x10, (getenv("PROGRAM_MEMORY")-1) {}
   
   #org PROGRAM_APP_START, PROGRAM_APP_START+0xF
   void empty_application(void)
   {
      disable_interrupts(GLOBAL);
      #asm
      GOTO PROGRAM_BOOTLOADER_START
      #endasm
   }   
   
   #if !defined(INT_I2C_BOOTLOADER)
   #error missing definition for your hardware.  see documentation above.
   #endif   
#else    //!__bootloader
   // mappings and code for the application
   
   #build(reset=PROGRAM_APP_START, interrupt=PROGRAM_ISR_START)
   #org 0, PROGRAM_LOADER_SIZE {}
   #IGNORE_WARNINGS 228
   #import(HEX, file="ex_i2c_ldr_bootloader.hex", RANGE=0:PROGRAM_LOADER_SIZE)
   
   // in your application's I2C slave ISR, if the 'state' is less than 0x80
   // then you need to call this function passing the 'state' and 'read'.
   // this function will then monitor incoming data for any messages
   // to start the bootloader.
   // this allows you to re-use the I2C slave ISR for your own functionality,
   // but then call this function to monitor for the bootloader start message.
   // 'state' is the i2c_isr_state() read in your I2C slave ISR.
   // 'read' is the i2c_read() read in your I2C slave ISR.
   // see the ex_i2c_ldr_application.c example for a demo of usage.
   void i2c_isr_bootload_check(unsigned int8 state, unsigned int8 read)
   {
      static unsigned int8 cmd;
      static union
      {
         unsigned int8 b[2];
         unsigned int16 w;
      } val;
   
      if (state == 1)                      //First received byte is address
         cmd = read;
      else if (state == 2)
         val.b[0] = read;
      else if ((state == 3) && (cmd == I2C_LOADER_COMMAND_GOTO_BOOTLOADER))
      {
         val.b[1] = read;
         if (val.w == I2C_LOADER_MAGIC)
         {  
            disable_interrupts(GLOBAL);
            #asm
            GOTO PROGRAM_BOOTLOADER_START
            #endasm
         }
      }
   }
#endif   //__bootloader

#endif
