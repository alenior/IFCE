///////////////////////////////////////////////////////////////////////////
////                               EX10.c                              ////
////                                                                   ////
////  Exercise for the CCS LoRa development kit that uses CCS's RN2903 ////
////  driver to update the modules firmware over a serial interface.   ////
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

#include <devkit_lora.h>

#include <rn2903.c>

void InitRN2903Bootloader(void);
int1 RN2903Loader(unsigned char c);
uint16_t RN2903GetCalculatedChecksum(void);

void PrintMenu(void)
{
   user_printf(user_putc, "\r\n\n1) Erase RN2903 Firmware");
   user_printf(user_putc, "\r\n2) Start Bootloading Firmware");
}

void main(void)
{
   char vStr[64];
   int1 LoadingFW = FALSE;
   int1 BootloaderMode = FALSE;
   uint16_t Checksum;
   rn2903_bl_version_t Version;
   char c;
   int1 Success;
   
   delay_ms(10);
   
   RN2903SerialInit(vStr);
   
   user_printf(user_putc, "\r\n\nEX10.c (Bootloading RN2903 Module) - %s", getenv("DEVICE"));
   
   if(vStr[0] != '\0')
   {
      user_printf(user_putc, "\r\n   RN2903 FW Version - %s", vStr);
      
      PrintMenu();
   }
   else
   {
      user_printf(user_putc, "\r\n   RN2903 FW Version not returned RN2903SerialInit() function, try power");
      user_printf(user_putc, "\r\n   cycling board.  If this message is received again RN2903 FW is probably");
      user_printf(user_putc, "\r\n   already erased.  Press '2' to start bootloading RN2903 FW.");
      
      //Flag that the RN2903 module is in bootloader mode, make sure that the 
      //RN2903 isn't attempted to be bootloaded until after the board is
      //powered cycled and it falls into this else a second time.  That is
      //because the RN2903SerialInit() function will only return the version
      //string after the module is power cycled, it doesn't send a command to
      //get the version string.  It's done this way because sending a non-
      //bootloader command to the RN2903 module when it's in bootloader mode
      //will mess up the bootloader's serial baud rate.
      BootloaderMode = TRUE;
   }
   
   while(TRUE)
   {
      //Check if new serial data has been received.
      if(user_kbhit())
      {
         //Get new serial data byte
         c = user_getc();
         
         //Check flag to determine if the data should be passed to bootloader.
         if(LoadingFW)
         {
            //Send serial data byte to bootloader, RN2903Loader() will return
            //TRUE when entire file as been loaded, it received the hex file's
            //end of file line.
            if(RN2903Loader(c))
            {
               //Send the command to get the checksum for the RN2903's
               //Application Memory.
               Checksum = RN2903BLGetCheckSum();
               
               user_printf(user_putc, "\r\nFinished Bootloading RN2903 Module: ");
               
               //Send the command to have the RN2903 BL reset the module, if
               //successfully loaded it will return the Firmware version string
               //causing RN2903BLReset() function to return TRUE.
               Success = RN2903BLReset();
               
               if(Success)
                  user_printf(user_putc, "Success");
               else
                  user_printf(user_putc, "Failed");
               
               user_printf(user_putc, " - Checksum: %04LX, Calculated: %04LX\r\n", Checksum, RN2903GetCalculatedChecksum());
               
               //Clear flag indicating that it no longer bootloading the RN2903
               //module.
               LoadingFW = FALSE;
               
               if(Success)
               {
                  //Was successful so clear flag indicating that RN2903 is no
                  //longer in bootloader mode.
                  BootloaderMode = FALSE;
                  
                  //Get the RN2903 module's firmware version.
                  RN2903Version(vStr);
                  
                  if(vStr[0] != 0)
                  {
                     //Display RN2903 module's firmware version.
                     user_printf(user_putc, "\r\n   RN2903 FW Version - %s", vStr);
                     
                     PrintMenu();
                  }
               }
            }
         }
         else
         {
            switch(c)
            {
               case '1':
                  user_printf(user_putc, "\r\nErase RN2903 (Y/N): ");
                  
                  do
                  {
                     if(user_kbhit())
                        c = toupper(user_getc());
                  } while((c != 'Y') && (c != 'N'));
                  
                  user_putc(c);
                  
                  if((c == 'Y') && (BootloaderMode == FALSE))
                  {
                     //Erase the RN2903 firmware, based on the RN2903 documentation this appears to
                     //only erase the firmware version and reset then RN2903 module into bootloader
                     //mode.  Once in Bootloader Mode the RN2903BLEraseFW() function can be used to
                     //erase the application memory were the firmware actual resides.
                     RN2903EraseFW();
                     
                     user_printf(user_putc, "\r\nRN2903 Firmware was erased, it should be in bootloader mode.");
                     user_printf(user_putc, "\r\nPress '2' to start bootloading module.");
                     
                     //Flag that the RN2903 module is in bootloader mode
                     BootloaderMode = TRUE;
                  }
                  else if(c == 'Y')
                  {
                     user_printf(user_putc, "\r\nRN2903 Firmware was not erased because module is already in");
                     user_printf(user_putc, "\r\nbootloader mode.");
                  }
                  break;
               case '2':
                  if(BootloaderMode == FALSE)
                  {
                     //Erase the RN2903 firmware, based on the RN2903 documentation this appears to
                     //only erase the firmware version and reset then RN2903 module into bootloader
                     //mode.  Once in Bootloader Mode the RN2903BLEraseFW() function can be used to
                     //erase the application memory were the firmware actual resides.
                     RN2903EraseFW();
                     
                     delay_ms(1000);   //make sure RN2903 has reset into bootloader mode
                     
                     //Flag that the RN2903 module is in bootloader mode, this is incase something
                     //goes wrong and the PIC is reset.  If RN2903 module is already in bootloader
                     //mode sending the RN2903EraseFW() command will mess up the RN2903 bootloader's
                     //serial baud rate.
                     BootloaderMode = TRUE;
                  }
                  
                  //Read RN2903 bootloader version.
                  RN2903BLGetVersion(&Version);
                  
                  //Display the RN2903 BL version.
                  user_printf(user_putc, "\r\nRN2903 Bootloader - Version: %04LX, DeviceId: %04LX, Erase Size: %u", Version.Version, Version.DeviceID, Version.EraseSize);
                  user_printf(user_putc, "\r\n                    Write Size: %u, UserID: %08LX", Version.WriteSize, Version.UserId);
                  
                  //Erase the application memory were the RN2903 Firmware is located.
                  if(RN2903BLEraseFW() == TRUE)
                  {
                     //Initialize variables used by this bootloader and flag that it's
                     //bootloading the RN2903 module.
                     InitRN2903Bootloader();
                     LoadingFW = TRUE;
                     
                     user_printf(user_putc, "\r\nLoading RN2903 FW - waiting for file\r\n");
                  }
                  else
                     user_printf(user_putc, "\r\nFailed Erase Not Loading RN2903 FW");
                  
                  break;
               default:
                  PrintMenu();
                  break;
            }
         }
      }
   }
}

#ifndef BUFFER_LEN_LOD
 #define BUFFER_LEN_LOD    64
#endif

#ifndef ACKLOD
#define ACKLOD    0x06
#define XON       0x11
#define XOFF      0x13

#define BootLoaderPutc(c)  user_putc(c)

uint8_t atoi_b16(unsigned char *s);

struct
{
   uint8_t idx;
   unsigned char buffer[BUFFER_LEN_LOD];
} bl_rBuffer;

uint8_t bl_h_addr;
uint16_t bl_checksum;

///////////////////////////////////////////////////////////////////////////////
// InitRN2903Bootloader()
//
// Function to initialize global variables associated with this bootloader.
// 
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void InitRN2903Bootloader(void)
{
   bl_rBuffer.idx = 0;
   bl_h_addr = 0;
   
   bl_checksum = 0;
}

///////////////////////////////////////////////////////////////////////////////
// RN2903Loader()
//
// Function use to actually take received serial hex file data and load it onto
// the RN2903 module.
//
// Parameters:
//    c - the received serial data from hex file.
//
// Returns:
//    TRUE - when entire file as been received, the end of file line from hex
//           was received.
//    FALSE - entire hex file has not been received.
///////////////////////////////////////////////////////////////////////////////
int1 RN2903Loader(unsigned char c)
{
   int1 done = FALSE;
   int1 do_ACKLOD;
   uint8_t checksum, line_type;
   uint16_t l_addr;
   uint32_t addr;
   uint8_t dataidx, i, count;
   uint8_t  data[BUFFER_LEN_LOD / 2];

   bl_rBuffer.buffer[bl_rBuffer.idx++] = c;
   
   //Check if entire line has been received.
   if((c == '\r') || (bl_rBuffer.idx >= BUFFER_LEN_LOD))
   {
      BootLoaderPutc(XOFF);
      
      do_ACKLOD = TRUE;
      
      // Only process data blocks that start with ':'
      if (bl_rBuffer.buffer[0] == ':')
      {
         count = atoi_b16(&bl_rBuffer.buffer[1]);  // Get the number of bytes from the buffer

         // Get the address
         l_addr = make16(atoi_b16(&bl_rBuffer.buffer[3]),atoi_b16(&bl_rBuffer.buffer[5]));
         
         // Get the line type
         line_type = atoi_b16 (&bl_rBuffer.buffer[7]);
         
         // Make the 32 bit address
         addr = make32(bl_h_addr,l_addr);
         
         checksum = 0;  // Sum the bytes to find the check sum value
         for (i=1; i<(bl_rBuffer.idx-3); i+=2)
            checksum += atoi_b16 (&bl_rBuffer.buffer[i]);
         checksum = 0xFF - checksum + 1;

         if (checksum != atoi_b16(&bl_rBuffer.buffer[bl_rBuffer.idx-3]))
            do_ACKLOD = FALSE;
         else
         {
            // If the line type is 1, then data is done being sent
            if (line_type == 1)
               done = TRUE;
            else if (line_type == 4)   // If the line type is 4, then it's the upper 16 bit of the 32 bit address is being set
               bl_h_addr = make16(atoi_b16(&bl_rBuffer.buffer[9]), atoi_b16(&bl_rBuffer.buffer[11]));
            else if (line_type == 0)   // If the line type is 0, this it's data to write
            {
               if((addr >= RN2903_APP_START_ADDR) && ((addr + count) <= (RN2903_APP_LAST_ADDR + 1)))
               {
                  // Loops through all of the data and stores it in data
                  // The last 2 bytes are the check sum, hence buffidx-3
                  for (i = 9,dataidx=0; i < bl_rBuffer.idx-3; i += 2)
                     data[dataidx++]=atoi_b16(&bl_rBuffer.buffer[i]);
                     
                  // Add data to overall checksum calculation
                  for(i=0;i<count;i+=2)
                     bl_checksum += make16(data[i+1], data[i]);
                  
                  // Write data to the RN2903 module
                  do_ACKLOD = RN2903BLWriteFW(addr, data, count);
               }
            }
         }
      }
         
      bl_rBuffer.idx = 0;
      
      if(do_ACKLOD)
         BootLoaderPutc(ACKLOD);
      
      BootLoaderPutc(XON);
   }
   
   return(done);
}

///////////////////////////////////////////////////////////////////////////////
// RN2903GetCalculatedChecksum()
//
// Function to get the current checksum value that's being calculated by the 
// RN2903Loader() function.
//
// Parameters - None.
//
// Returns:
//    uint16_t value representing the current calculated checksum.
///////////////////////////////////////////////////////////////////////////////
uint16_t RN2903GetCalculatedChecksum(void)
{
   return(bl_checksum);
}

///////////////////////////////////////////////////////////////////////////////
// atoi_b16()
//
// Function to convert to hex characters to a uint8_t value.
//
// Parameters:
//    s - pointer to an unsigned char array containing the two hex characters
//        to convert.
//
// Returns:
//    uint8_t value representing the converted value.
///////////////////////////////////////////////////////////////////////////////
uint8_t atoi_b16(unsigned char *s)
{
   uint8_t result = 0;
   uint8_t i;

   for (i=0; i<2; i++,s++)
   {
      if (*s >= 'A')
         result = 16*result + (*s) - 'A' + 10;
      else
         result = 16*result + (*s) - '0';
   }

   return(result);
}
