///////////////////////////////////////////////////////////////////////////
////                          rom_write_dp.c                           ////
////                                                                   ////
//// Routines for modifying the program memory.  Has intelligence      ////
//// to properly handle modifying (so it will read the block of        ////
//// program memory before erasing it) and prevent config bits from    ////
//// being changed (if config bits are stored at the end of program    ////
//// memory).  Also all erase/writes are delayed until a page boundary ////
//// is crossed or rom_flush() is called.                              ////
////                                                                   ////
//// If this chip does not have config bits stored in program memory   ////
//// AND this chip's erase block size matches the write block size     ////
//// then no extra intelligence is needed (at which point this driver  ////
//// simply maps to the CCS C Compiler's write_program_memory()).      ////
////                                                                   ////
//// This version has been modified to work with devices that have     ////
//// dual partitions and are being build in dual partition mode.       ////
////                                                                   ////
//// API                                                               ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
//// rom_init()                                                        ////
////     Reinitialize the library.  Usually the global variables       ////
////     used by the library are already initialized, but if for       ////
////     some reason you want to initialize it again then you          ////
////     can call this.  If you call this when there are any           ////
////     pending writes then the pending writes will be lost.          ////
////                                                                   ////
//// rom_flush()                                                       ////
////     Flush any pending writes to the program memory.               ////
////                                                                   ////
//// rom_modify(address, pointer, num)                                 ////
////     Write 'num' bytes from RAM 'pointer' to program memory        ////
////     'address'.  'num' is always in bytes, regardless if the       ////
////     processor architecture stores several bytes per address.      ////
////     Actual writes may be delayed, call rom_flush() when done      ////
////     to insure that any pending writes are completed.              ////
////                                                                   ////
//// CONFIGURATION                                                     ////
//// ----------------------------------------------------------------- ////
////                                                                   ////
//// ROM_WRITE_CAN_MODIFY_CONFIG_BITS                                  ////
////     If this is defined before including this file, then           ////
////     rom_modify() will allow the user to change the config bits.   ////
////     If this isn't defined, the default action is to preserve the  ////
////     existing config bits.                                         ////
////     Care should be taken when using this option.                  ////
////     This configuration is only used if the architecture of the    ////
////     controller has config bits at the end of program memory.      ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services           ////
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

#ifndef __ROMW_C__
#define __ROMW_C__

#ifndef debug_rom_write
   #define debug_rom_write(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#endif

// bytes per address
#if defined(__PCH__)
   #define _ROMW_BPA 1
#else //PCM and PCD
   #define _ROMW_BPA 2
#endif

// size of this controller's write size, in bytes
#define _ROMW_WRITE_SIZE         getenv("FLASH_WRITE_SIZE")

// size of this controller's erase size, in bytes
#define _ROMW_ERASE_SIZE         getenv("FLASH_ERASE_SIZE")

#define _ROMW_FIND_PAGE_START(address) ((unsigned int32)address & ~(((unsigned int32)_ROMW_ERASE_SIZE/_ROMW_BPA)-(unsigned int32)1))

// total size of program memory of this controller, including config bits.  this value is in instructions (NOT BYTES)
#if (_ROMW_FIND_PAGE_START(getenv("PROGRAM_MEMORY")) != getenv("PROGRAM_MEMORY"))
   #define _ROMW_FLASH_SIZE   _ROMW_FIND_PAGE_START(getenv("PROGRAM_MEMORY") + getenv("FLASH_ERASE_SIZE")/_ROMW_BPA)
   
   #define _ROMW_HAS_CONFIG_BITS 1
   
   #if defined(ROM_WRITE_CAN_MODIFY_CONFIG_BITS)
      #define _ROMW_WRITABLE_SIZE   (0x400000 + _ROMW_FLASH_SIZE)
   #else
      #define _ROMW_WRITABLE_SIZE   (0x400000 + _ROMW_FIND_PAGE_START(getenv("PROGRAM_MEMORY")))
   #endif
#else
   #define _ROMW_FLASH_SIZE   getenv("PROGRAM_MEMORY")
   
   #define _ROMW_HAS_CONFIG_BITS 0
   
   #define _ROMW_WRITABLE_SIZE   getenv("PROGRAM_MEMORY")
#endif

// size, in bytes, of the config bits
#define _ROMW_CONFIG_BITS_SIZE   ((_ROMW_FLASH_SIZE - getenv("PROGRAM_MEMORY")) * _ROMW_BPA)

// location of the config bits in program memory
#define _ROMW_CONFIG_BITS_LOC    getenv("PROGRAM_MEMORY")

#if ((_ROMW_WRITE_SIZE == _ROMW_ERASE_SIZE) && (!_ROMW_HAS_CONFIG_BITS || defined(ROM_WRITE_CAN_MODIFY_CONFIG_BITS)))
   #define rom_init()
   
   #define rom_flush()
   
   // don't make location an '__ADDRESS__', because config bits might be a 
   // 32bit address and flash size might be 16bit.
   void rom_modify(unsigned int32 location, unsigned char *src, unsigned int16 size)
   {
      if ((location + (unsigned int32)size/(unsigned int32)_ROMW_BPA) > (unsigned int32)_ROMW_WRITABLE_SIZE)
      {
         if (location > (unsigned int32)_ROMW_WRITABLE_SIZE)
            size = 0;
         else
            size = ((unsigned int32)_ROMW_WRITABLE_SIZE - location) * (unsigned int32)_ROMW_BPA;
      }
      
      if (size > 0)
      {
         write_program_memory(location, src, size);
      }
   }
#else
   #define _ROMW_PAGE_INVALID    0xFFFFFFFF
  
   unsigned int32 _romw_curr_page = _ROMW_PAGE_INVALID;

   unsigned int8 _romw_buffer[_ROMW_ERASE_SIZE];

   void rom_init(void)
   {
      _romw_curr_page = _ROMW_PAGE_INVALID;
   }

   void rom_flush(void)
   {
      if (_romw_curr_page != _ROMW_PAGE_INVALID)
      {
         debug_rom_write(debug_putc, "rom_flush() 0x%LX\r\n", _romw_curr_page);
         
        #if defined(__PCD__)
         erase_program_memory(_romw_curr_page);     //erase entire block
        #else
         erase_program_eeprom(_romw_curr_page);     //erase entire block
        #endif

         write_program_memory(_romw_curr_page, _romw_buffer, sizeof(_romw_buffer));    //write modified block
               
         _romw_curr_page = _ROMW_PAGE_INVALID;
      }
   }

   void rom_modify(unsigned int32 location, unsigned char *src, unsigned int16 size)
   {
      unsigned int32 page_start;
      unsigned int16 instructionOffset, num, max;

      page_start = _ROMW_FIND_PAGE_START(location);

      instructionOffset = location - page_start;
      
      debug_rom_write(debug_putc, "rom_modify(0x%LX,0x%LX,%lu)\r\n", location, src, size);

      while (size)
      {
         if (page_start != _romw_curr_page)
         {
            rom_flush();
            
            _romw_curr_page = page_start;
            
            if (page_start > _ROMW_WRITABLE_SIZE)
            {
               debug_rom_write(debug_putc, "rom_modify(0x%LX,0x%LX,%lu) OVERRUN_SO_EXIT\r\n", location, src, size);
               return;
            }
            
            read_program_memory(page_start, _romw_buffer, sizeof(_romw_buffer));  //read entire block to ram buffer
         }

         max = _ROMW_ERASE_SIZE - (instructionOffset * _ROMW_BPA);

        #if (_ROMW_HAS_CONFIG_BITS && !defined(ROM_WRITE_CAN_MODIFY_CONFIG_BITS))
         // check to see if we need to prevent the change of config bits in _rom_block.
         // this library makes the assumption that the config bits are the last bytes of the page.
         if (page_start == _ROMW_FIND_PAGE_START(_ROMW_CONFIG_BITS_LOC))
         {
            if (max < _ROMW_CONFIG_BITS_SIZE)
               max = 0;
            else
               max -= _ROMW_CONFIG_BITS_SIZE;
            
            if (max == 0)
               size = 0;   //stop
         }
        #endif
         
         
         if (size > max)
         {
            num = max;
         } 
         else 
         {
            num = size;
         }
         
         debug_rom_write(debug_putc, "rom_modify() page=0x%LX io=0x%LX max=%LU num=%LU size=%LU\r\n", page_start, instructionOffset, max, num, size);
         
         memcpy(&_romw_buffer[instructionOffset*_ROMW_BPA], src, num);    //modify ram buffer

         src += num;
         page_start += _ROMW_ERASE_SIZE / _ROMW_BPA;
         instructionOffset = 0;
         size -= num;
      }
   }
#endif   //(_ROMW_WRITE_SIZE != _ROMW_ERASE_SIZE)

#define rom_write_debug_display()   \
   printf(debug_putc, "TOTAL SIZE = 0x%LX\r\n", _ROMW_FLASH_SIZE);   \
   printf(debug_putc, "WITOUT CONFIG = 0x%LX\r\n", getenv("PROGRAM_MEMORY")); \
   printf(debug_putc, "HAS CONFIG = %u\r\n", _ROMW_HAS_CONFIG_BITS); \
   printf(debug_putc, "CONFIG SIZE = %u\r\n", _ROMW_CONFIG_BITS_SIZE);  \
   printf(debug_putc, "CONFIG LOC = 0x%LX\r\n", _ROMW_CONFIG_BITS_LOC); \
   printf(debug_putc, "CONFIG PAGE START = 0x%LX\r\n", _ROMW_FIND_PAGE_START(_ROMW_CONFIG_BITS_LOC)); \
   printf(debug_putc, "WRITE SIZE = 0x%LX\r\n", _ROMW_WRITE_SIZE);   \
   printf(debug_putc, "ERASE SIZE = 0x%LX\r\n", _ROMW_ERASE_SIZE)
   
#endif   //__ROMW_C__


