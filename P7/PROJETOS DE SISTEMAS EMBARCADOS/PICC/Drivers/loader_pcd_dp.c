///////////////////////////////////////////////////////////////////////////
////                          loader_pcd_dp.c                          ////
////                                                                   ////
//// This driver will take an Intel 8-bit Hex file over RS232 channels ////
//// and modify the flash program memory with the new code.            ////
////                                                                   ////
//// After each good line, the loader sends an ACKLOD character.  The  ////
//// driver uses XON/XOFF flow control.  Also, any buffer on the PC    ////
//// UART must be turned off, or to its lowest setting, otherwise it   ////
//// will miss data.                                                   ////
////                                                                   ////
////  The config bits will be write protected, in software, by the     ////
////  rom_modify() function.  To allow config bits to change then      ////
////  you need to #define ROM_WRITE_CAN_MODIFY_CONFIG_BITS             ////
////                                                                   ////
//// This version has been modified to work with devices that have     ////
//// dual partitions and are being build in dual partition mode.       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#define PROGRAM_MEMORY_FIND_PAGE_START(address)    ((__ADDRESS__)address & ~(((__ADDRESS__)getenv("FLASH_ERASE_SIZE")/2)-(__ADDRESS__)1))
#define PROGRAM_MEMORY_SIZE                        PROGRAM_MEMORY_FIND_PAGE_START(getenv("PROGRAM_MEMORY")) + (getenv("FLASH_ERASE_SIZE")/2)

#define APPLICATION_START     0
#define APPLICATION_END       (PROGRAM_MEMORY_SIZE-1)

#define ROM_WRITE_CAN_MODIFY_CONFIG_BITS

#ifndef debug_rom_write
   #define debug_rom_write(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#endif

#define BUFFER_LEN_LOD 64

#ifdef BOOTLOADER_MODE2X
 #define BUFFER_COUNT   2
#else
 #define BUFFER_COUNT   1
#endif

struct
{
   unsigned int16 idx;
   char buffer[BUFFER_LEN_LOD];
} rBuffer[BUFFER_COUNT];

#define ACKLOD 0x06
#define XON    0x11
#define XOFF   0x13

#ifndef BootLoaderGetc
#ifdef BOOTLOADER_STREAM
 #define BootLoaderGetc()     fgetc(BOOTLOADER_STREAM)
 #define BootLoaderPutc(c)    fputc(c, BOOTLOADER_STREAM)
#else
 #define BootLoaderGetc()     getc()
 #define BootLoaderPutc(c)    putc(c)
#endif
#endif

#include <rom_write_dp.c>

unsigned int8 atoi_b16(char *s);

void load_program(void)
{
   unsigned int1  do_ACKLOD, done=FALSE;
   unsigned int8  checksum, line_type;
   unsigned int16 l_addr,h_addr=0;
   unsigned int32 addr;
   unsigned int8  dataidx;
   unsigned int16 i;
   unsigned int16 count=0;
   unsigned int8  data[32];
   unsigned int8  buffidx;
   
   buffidx = 0;

   while (!done)  // Loop until the entire program is downloaded
   {
      rBuffer[buffidx].idx = 0;  // Read into the buffer until 0x0D ('\r') is received or the buffer is full
      
      do
      {
         rBuffer[buffidx].buffer[rBuffer[buffidx].idx] = BootLoaderGetc();
      }
      while ((rBuffer[buffidx].buffer[rBuffer[buffidx].idx++] != 0x0D) && (rBuffer[buffidx].idx <= BUFFER_LEN_LOD));
     
     #ifdef BOOTLOADER_MODE2X
      if(++buffidx >= BUFFER_COUNT)
      {
     #endif
         BootLoaderPutc(XOFF);  // Suspend sender
   
         do_ACKLOD = TRUE;
         
        #ifdef BOOTLOADER_MODE2X
         if(rBuffer[0].idx != rBuffer[1].idx)
            do_ACKLOD = FALSE;
         else
         {
            for(i=0;i<(rBuffer[0].idx-1);i++)
            {
               if(rBuffer[0].buffer[i] != rBuffer[1].buffer[i])
               {
                  do_ACKLOD = FALSE;
                  break;
               }
            }
            
            if(do_ACKLOD)
            {
        #endif 

               // Only process data blocks that start with ':'
               if (rBuffer[0].buffer[0] == ':')
               {
                  count = atoi_b16 (&rBuffer[0].buffer[1]);  // Get the number of bytes from the buffer
         
                  // Get the lower 16 bits of address
                  l_addr = make16(atoi_b16(&rBuffer[0].buffer[3]),atoi_b16(&rBuffer[0].buffer[5]));
         
                  line_type = atoi_b16 (&rBuffer[0].buffer[7]);
         
                  addr = make32(h_addr,l_addr) / _ROMW_BPA;
         
                  checksum = 0;  // Sum the bytes to find the check sum value
                  for (i=1; i<(rBuffer[0].idx-3); i+=2)
                     checksum += atoi_b16 (&rBuffer[0].buffer[i]);
                  checksum = 0xFF - checksum + 1;
         
                  if (checksum != atoi_b16 (&rBuffer[0].buffer[rBuffer[0].idx-3]))
                  {
                     debug_rom_write(debug_putc, "CS_FAIL\r\n");
                     do_ACKLOD = FALSE;
                  }
                  else
                  {
                     // If the line type is 1, then data is done being sent
                     if (line_type == 1)
                     {
                        done = TRUE;
                     }
                     else if (line_type == 4)
                        h_addr = make16(atoi_b16(&rBuffer[0].buffer[9]), atoi_b16(&rBuffer[0].buffer[11]));
                     else if (line_type == 0)
                     {
                        debug_rom_write(debug_putc, "CHECK_%LX_%u\r\n", addr, count);
                        
                        for (i = 9,dataidx=0; i < rBuffer[0].idx-3; i += 2)
                           data[dataidx++]=atoi_b16(&rBuffer[0].buffer[i]);
         

                        if 
                        (
                           (
                              (addr >= APPLICATION_START)
                              && ((addr + (unsigned int32)count/_ROMW_BPA) <= ((unsigned int32)APPLICATION_END+1))
                           )
                        )
                        {
                           //Add 0x400000 to address to program inactive partition
                           rom_modify(0x400000 + addr, data, count);
                        }
                       #if getenv("EEPROM_ADDRESS")
                        else if 
                        (
                           (addr >= getenv("EEPROM_ADDRESS")) &&
                           (addr < getenv("EEPROM_ADDRESS")+getenv("DATA_EEPROM"))
                        )
                        {
                           unsigned int32 eeAddr;
                           unsigned int8 eeIdx = 0;
                           
                           eeAddr = addr - getenv("EEPROM_ADDRESS");

                           rom_flush();
                           
                           while(count)
                           {
                              write_eeprom(eeAddr, make16(data[eeIdx+1],data[eeIdx]));
                              eeAddr += 2;
                              eeIdx += 4;
                              
                              //incase hex file is bad so it won't get stuck in
                              //an infinite loop
                              if(Count > 4)
                                 count -= 4;
                              else
                                 count = 0;
                           }
                        }
                       #endif
                     }
                  }
               }
               else if(rBuffer[0].buffer[0] != ';')
                  do_ACKLOD = FALSE;
        #ifdef BOOTLOADER_MODE2X
            }
         }
         
         buffidx = 0;
        #endif

         if (do_ACKLOD)
            BootLoaderPutc(ACKLOD);
         
         BootLoaderPutc(XON);
         
     #ifdef BOOTLOADER_MODE2X
      }
     #endif
   }
   
   rom_flush();
   
   BootLoaderPutc(ACKLOD);
   BootLoaderPutc(XON);   
   
   delay_ms(5);

   reset_cpu();
}

unsigned int8 atoi_b16(char *s) {  // Convert two hex characters to a int8
   unsigned int8 result = 0;
   unsigned int8 i;

   for (i=0; i<2; i++,s++)  {
      if (*s >= 'A')
         result = 16*result + (*s) - 'A' + 10;
      else
         result = 16*result + (*s) - '0';
   }

   return(result);
}

