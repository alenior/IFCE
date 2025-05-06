///////////////////////////////////////////////////////////////////////////////
////                      Virtual EEPROM Driver                            ////
////                                                                       ////
////  This driver uses two flash erase pages in program memory as virtual  ////
////  eeprom memory. This driver uses write-balancing to avoid excessive   ////
////  erasing of program memory.  This driver is optimized to use lower    ////
////  eeprom addresses first; however, it is not a requirement to use the  ////
////  available addresses in any particular order. The functions           ////
////  which drive operation of the driver are detailed below:              ////
////                                                                       ////
////  init_virtual_eeprom()       Initializes pointer and global variable  ////
////                              for use with write and read.  Does NOT   ////
////                              clear previous virtual eeprom memory.    ////
////                              Must be called at beginning of program   ////
////                              before write and read are used.          ////
////                                                                       ////
////  data = read_virtual_eeprom(addy)      Reads one byte from virtual    ////
////                                        eeprom address "addy". Default ////
////                                        return is 0xFF if no data is   ////
////                                        found for given address.       ////
////                                                                       ////
////  ok = write_virtual_eeprom(addy,data)  Writes "data" byte to virtual  ////
////                                        eeprom address "addy". Returns ////
////                                        true if write is successful,   ////
////                                        returns false if memory is     ////
////                                        full or an invalid address is  ////
////                                        used.                          ////
////                                        This process can take a lot    ////
////                                        of time, especially if large   ////
////                                        pages are being used.          ////
////                                                                       ////
////  clear_virtual_eeprom()      Clears and reinitializes all virtual     ////
////                              eeprom.                                  ////
////                                                                       ////
////  num = virtual_eeprom_max_entries()  This function is provided for    ////
////                               debugging.  It returns the total number ////
////                               number of entries that can be saved to  ////
////                               the flash memory.  To increase this     ////
////                               value, then increase                    ////
////                               VIRTUAL_EEPROM_NUM_PAGES.               ////
////                                                                       ////
////  VIRTUAL_EEPROM_NUM_PAGES can be defined, when defines the total      ////
////  number of the PIC's flash erase pages used for the virtual EEPROM.   ////
////  The actual value used is 2x this value (two pages are used for       ////
////  writing balancing algorithm).  The total number of entries that      ////
////  can be saved to the EE depends on this setting.                      ////
////                                                                       ////
////  If VIRTUAL_EEPROM_16BIT_ADDY is defined then you can use 16bit       ////
////  addresses for the EEPROM (if not defined 8bit addresses are used).   ////
////  This option is always used on PCD, PCM and PCB chips since these     ////
////  architecture have a multi-byte instruction word (however PCM will    ////
////  limit an address to 14bits and PCB will limit an address to 12bits). ////
////  Using this option reduces the total number of variables that can     ////
////  be stored because now more space is being used to store addresses.   ////
////  VIRTUAL_EEPROM_8BIT_ADDY can be used to force 8bit address on        ////
////  PCB, PCH and PCD chips.                                              ////
////                                                                       ////
////  VIRTUAL_EEPROM_START_ADDR can be defined to an address to start the  ////
////  virtual EEPROM at in program memory.  By default this driver uses    ////
////  the last 2 * VIRTUAL_EEPROM_NUM_PAGES full memory pages for the      ////
////  virtual EEPROM.  This define can be used to change the start         ////
////  address, the address must on a erase page boundary and must be set   ////
////  so that 2 * VIRTUAL_EEPROM_NUM_PAGES does not overrun the program    ////
////  memory.                                                              ////
////                                                                       ////
//// EXAMPLE USAGE                                                         ////
//// --------------------------------------------------------------------  ////
//// The compiler's ex_extee.c example can be quickly modified to work     ////
//// as a demo of this driver.  The ex_extee.c example shows how to use    ////
//// an external EEPROM, but can be adapted to use this library as a       ////
//// virtual EEPROM.                                                       ////
////                                                                       ////
//// To accomplish this, first find this line:                             ////
////     #include "2416.c"                                                 ////
////                                                                       ////
//// Then replace it with these lines:                                     ////
////     #include "virtual_eeprom.c"                                       ////
////     #define init_ext_eeprom() init_virtual_eeprom()                   ////
////     #define read_ext_eeprom(a) read_virtual_eeprom(a)                 ////
////     #define write_ext_eeprom(a, v) write_virtual_eeprom(a, v)         ////
////     #define EEPROM_ADDRESS unsigned int8                              ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////                                                                       ////
//// HISTORY                                                               ////
////                                                                       ////
//// October 4th, 2019 -                                                   ////
////  Added VIRTUAL_EEPROM_START_ADDR which can be defined to an address   ////
////     in program memory to start the virtual EEPROM at.                 ////
////                                                                       ////
//// March 10th, 2017 -                                                    ////
////  Speed optimizations.                                                 ////
////  Added workaround for PIC24FJxxxGx6xx families, which require writing ////
////     8 bytes at once else a double write trap is generated.            ////
////  Added virtual_eeprom_max_entries() function.                         ////
////  Added VIRTUAL_EEPROM_NUM_PAGES.  Changed older                       ////
////     VIRTUAL_EEPROM_DOUBLE_SPACE define to map to this newer setting.  ////
////  Added VIRTUAL_EEPROM_16BIT_ADDY.                                     ////
////  The PAGE0_START, PAGE0_END, PAGE1_START and                          ////
////     PAGE1_END were not being calculated correctly for PCD             ////
////     devices.  That means upgrading to this driver will cause your     ////
////     EEPROM values currently stored in memory to be lost since it      ////
////     will now be reading/writing to the correct space.  The old        ////
////     incorrect values are still there but commented out, in case you   ////
////     need backwards compatibility.                                     ////
////                                                                       ////
//// April 26th, 2016 -                                                    ////
////  Fixed issue causing driver not to compile with PCD compiler.         ////
////                                                                       ////
//// March 31st, 2016 -                                                    ////
////  The PAGE0_START, PAGE0_END, PAGE1_START and                          ////
////     PAGE1_END were not being calculated correctly for PCM and PCH     ////
////     devices.  That means upgrading to this driver will cause your     ////
////     EEPROM values currently stored in memory to be lost since it      ////
////     will now be reading/writing to the correct space.  The old        ////
////     incorrect values are still there but commented out, in case you   ////
////     need backwards compatibility.                                     ////
////  Fixed bugs with PIC16.                                               ////
////  Optimized ROM/RAM memory use with PIC16 and PIC18 devices.           ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////            (C) Copyright 1996,2019 Custom Computer Services           ////
////                                                                       ////
//// This source code may only be used by licensed users of the CCS C      ////
//// compiler or to users who purchased the rights to use this code.  This ////
//// source code may only be distributed to other licensed users of the    ////
//// CCS C compiler or other users who purchased the rights to this code.  ////
//// The rights to use this code can only be granted by CCS.  No other     ////
//// use, reproduction or distribution is permitted without written        ////
//// permission.  Derivative programs created using this software in       ////
//// object code form are not restricted in any way.                       ////
////                                                                       ////
////                         http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////////
#ifndef  VIRTUAL_EEPROM_C
#define  VIRTUAL_EEPROM_C

#ifndef debug_ve_printf
#define debug_ve_printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#endif

#if (getenv("FLASH_ERASE_SIZE")<32)
#error Erase page size on this PIC is too small to use this library effectively
#endif

#if (defined(__PCM__) || defined(__PCB__) || defined(__PCD__)) && !defined(VIRTUAL_EEPROM_8BIT_ADDY)
   #define VIRTUAL_EEPROM_16BIT_ADDY
#endif

#if defined(VIRTUAL_EEPROM_8BIT_ADDY) && defined(VIRTUAL_EEPROM_16BIT_ADDY)
   #error cant use both at the same time
#endif

#if defined(VIRTUAL_EEPROM_16BIT_ADDY)
   typedef unsigned int16 ve_virtual_address_t;
#else
   typedef unsigned int8 ve_virtual_address_t;
#endif

#if !defined(VIRTUAL_EEPROM_16BIT_ADDY)
   #define _VE_INVALID_ADDRESS   -1
#else
   #if defined(__PCM__)
      #define _VE_INVALID_ADDRESS   (unsigned int16)0x3fff
   #elif defined(__PCB__)
      #define _VE_INVALID_ADDRESS   (unsigned int16)0xfff
   #else
      #define _VE_INVALID_ADDRESS   -1
   #endif
#endif

#if defined(__PCH__)
   #define _VE_BPA  1
#else
   #define _VE_BPA  2
#endif

#if defined(VIRTUAL_EEPROM_DOUBLE_SPACE)
   #define VIRTUAL_EEPROM_NUM_PAGES 2
#endif

#ifndef VIRTUAL_EEPROM_NUM_PAGES
   #define VIRTUAL_EEPROM_NUM_PAGES 1
#endif

#define _VE_PAGE_BYTES (getenv("FLASH_ERASE_SIZE")*VIRTUAL_EEPROM_NUM_PAGES)

#ifdef VIRTUAL_EEPROM_START_ADDR
   #if ((VIRTUAL_EEPROM_START_ADDR % (getenv("FLASH_ERASE_SIZE")/_VE_BPA)) != 0)
      #error Virtual EEPROM Start Address must be on a flash erase boundary
   #endif
   
   #if ((VIRTUAL_EEPROM_START_ADDR + (2*(_VE_PAGE_BYTES/_VE_BPA))) > getenv("PROGRAM_MEMORY"))
      #error Virtual EEPROM Start Address to high, memory overruns program memory size
   #endif
   
   #define _VE_FLASH_SIZE_INSTRUCTIONS (VIRTUAL_EEPROM_START_ADDR + (2*(_VE_PAGE_BYTES/_VE_BPA)))
#else
   #if ((getenv("PROGRAM_MEMORY") % (getenv("FLASH_ERASE_SIZE")/_VE_BPA)) != 0)
      //go back one page, because this page is being used for config bits
      #define _VE_FLASH_SIZE_INSTRUCTIONS  (getenv("PROGRAM_MEMORY") - (getenv("PROGRAM_MEMORY") % (getenv("FLASH_ERASE_SIZE")/_VE_BPA)))
   #else
      #define _VE_FLASH_SIZE_INSTRUCTIONS  getenv("PROGRAM_MEMORY")
   #endif
#endif

#define _VE_PAGE0_START (_VE_FLASH_SIZE_INSTRUCTIONS - _VE_PAGE_BYTES/_VE_BPA)
#define _VE_PAGE0_END (_VE_FLASH_SIZE_INSTRUCTIONS)
#define _VE_PAGE1_START (_VE_FLASH_SIZE_INSTRUCTIONS - (2*_VE_PAGE_BYTES/_VE_BPA))
#define _VE_PAGE1_END _VE_PAGE0_START

#org _VE_PAGE0_START,(_VE_PAGE0_END-1) {}
#org _VE_PAGE1_START,(_VE_PAGE1_END-1) {}

typedef unsigned int32 _VE_ADDRESS_;  //an address for the flash memory

/* 
 //old values, are broken
 #define _VE_PAGE0_START (_VE_FLASH_SIZE_INSTRUCTIONS-(2*_VE_PAGE_BYTES/_VE_BPA))
 #define _VE_PAGE0_END (_VE_FLASH_SIZE_INSTRUCTIONS - _VE_PAGE_BYTES)
 #define _VE_PAGE1_START (_VE_FLASH_SIZE_INSTRUCTIONS-(3*_VE_PAGE_BYTES/_VE_BPA))
 #define _VE_PAGE1_END (_VE_FLASH_SIZE_INSTRUCTIONS-(2*_VE_PAGE_BYTES))
*/

struct
{
   _VE_ADDRESS_ next;
   int1 page;
} _g_VE;

#if defined(__PCM__) && (getenv("FLASH_ERASE_SIZE") == getenv("FLASH_WRITE_SIZE"))
   static void _ve_erase_program_eeprom(_VE_ADDRESS_ addy)
   {
      char b[_VE_BPA] = {0xFF, 0xFF};
      unsigned int16 n;
   
      n = _VE_PAGE_BYTES/_VE_BPA;
      
      // change addy to the start of the erase block
      addy &= ~(_VE_ADDRESS_)(_VE_PAGE_BYTES/_VE_BPA-1);
      
      debug_ve_printf(debug_putc, "_ve_erase_program_eeprom(%LX) n=%lu\r\n", addy, n);
      
      while(n-- != 0)
      {
         write_program_memory(addy++, b, sizeof(b));
      }
   }
#elif (VIRTUAL_EEPROM_NUM_PAGES>1)
   void _ve_erase_program_eeprom(_VE_ADDRESS_ addy)
   {
      int i = VIRTUAL_EEPROM_NUM_PAGES;
      
      while(i--)
      {
        #if defined(__PCD__)
         erase_program_memory(addy);
        #else
         erase_program_eeprom(addy);
        #endif
         addy += getenv("FLASH_ERASE_SIZE")/_VE_BPA;
      }
   }
#else
   #if defined(__PCD__)
      #define _ve_erase_program_eeprom(addy) erase_program_memory(addy)
   #else
      #define _ve_erase_program_eeprom(addy) erase_program_eeprom(addy)
   #endif
#endif

static int1 _find_virtual_eeprom(ve_virtual_address_t addy, int8 *pRet);
static void _VE_move_page(void);

#if defined(__PCD__) && (getenv("MIN_FLASH_WRITE") != 4)
   #if (getenv("MIN_FLASH_WRITE") == 8)
      #define VE_PIC_WORKAROUND_RECORD_ALIGNMENT   8  //workaround for PIC24FJxxxGx6xx
   #else
      #error do
   #endif
#endif

typedef union
{
   struct
   {
      union
      {
         ve_virtual_address_t  address;
         
        #if (_VE_BPA==1)
         unsigned int8 addAsInstruction;
        #elif (_VE_BPA==2)
         unsigned int16 addAsInstruction;
        #else
         #error do
        #endif
      };
      union
      {
         unsigned int8 value;
         
        #if (_VE_BPA==1)
         unsigned int8 valAsInstruction;
        #elif (_VE_BPA==2)
         unsigned int16 valAsInstruction;
        #else
         #error do
        #endif
      };
   };
  #if defined(VE_PIC_WORKAROUND_RECORD_ALIGNMENT)
   unsigned int8 alignment[VE_PIC_WORKAROUND_RECORD_ALIGNMENT];
  #endif
} _g_ve_record_t;

#define __VE_RECORD_ISNT_BLANK(rec)   ((rec.address != (ve_virtual_address_t)_VE_INVALID_ADDRESS) || (rec.value != (unsigned int8)-1))

void init_virtual_eeprom(void) 
{
   _g_ve_record_t record;
   _VE_ADDRESS_ end;

   read_program_memory(_VE_PAGE0_START, &record, sizeof(record));
   
   debug_ve_printf(debug_putc, "init_virtual_eeprom() %X %X\r\n", record.address, record.value);
   //debug_ve_printf(debug_putc, "P0 start=%LX end=%LX    P1 start=%LX end=%LX\r\n", _VE_PAGE0_START, _VE_PAGE0_END, _VE_PAGE1_START, _VE_PAGE1_END);
   
   #if 0
   _g_VE.next = _VE_PAGE0_START;
   read_program_memory(_g_VE.next, &record, sizeof(record));
   debug_ve_printf(debug_putc, "P0 read_ram %X %X\r\n", record.address, record.value);
   read_program_memory(_VE_PAGE0_START, &record, sizeof(record));
   debug_ve_printf(debug_putc, "P0 read_rom %X %X\r\n", record.address, record.value);
   #endif
  
   if (__VE_RECORD_ISNT_BLANK(record))
   {
      _g_VE.page = 0;
      _g_VE.next = _VE_PAGE0_START;
      end = _VE_PAGE0_END;
      
      read_program_memory(_VE_PAGE1_START, &record, sizeof(record));
      if (__VE_RECORD_ISNT_BLANK(record))
      {
         // both pages aren't blank - something funny happened.  erase the other one.
         _ve_erase_program_eeprom(_VE_PAGE1_START);
         
         debug_ve_printf(debug_putc, "init_virtual_eeprom() %X %X BOTH_PAGES_ARENT_BLANK\r\n", record.address, record.value);
      }
   }
   else
   {
     #if 0
      read_program_memory(_VE_PAGE1_START, &record, sizeof(record));
      if (!__VE_RECORD_ISNT_BLANK(record))
      {
         debug_ve_printf(debug_putc, "Both pages blank, clear again\r\n");
         clear_virtual_eeprom();
      }
     #endif
     
      _g_VE.page = 1;
      _g_VE.next = _VE_PAGE1_START;
      end = _VE_PAGE1_END;
   }
   
   //debug_ve_printf(debug_putc, "init_virtual_eeprom() using page=%u next=%LX end=%LX\r\n", _g_VE.page, _g_VE.next, end);
   
   while(_g_VE.next < end)
   {
      read_program_memory(_g_VE.next, &record, sizeof(record));
      
      if (!__VE_RECORD_ISNT_BLANK(record))
      {
         break;
      }
      
      //debug_ve_printf(debug_putc, ".");
      //debug_ve_printf(debug_putc, "%LX=%LX=%X ", _g_VE.next, (unsigned int16)record.address, record.value);


      _g_VE.next += sizeof(record)/_VE_BPA;
   }
   
   debug_ve_printf(debug_putc, "init_virtual_eeprom() page=%u next=%LX\r\n", _g_VE.page, _g_VE.next);
}

#if defined(VE_PIC_WORKAROUND_ROM_WRITE_BYTES)
   void _ve_write_program_memory(_VE_ADDRESS_ romStart, unsigned int8 *ramPtr, size_t n)
   {
      char data[VE_PIC_WORKAROUND_ROM_WRITE_BYTES];
      _VE_ADDRESS_ romLoc;
      int frag, i;
      
      romLoc = romStart & ~((_VE_ADDRESS_)(VE_PIC_WORKAROUND_ROM_WRITE_BYTES/_VE_BPA)-1);
      frag = ((_VE_ADDRESS_)romStart - (_VE_ADDRESS_)romLoc) * (_VE_ADDRESS_)_VE_BPA;
      
      debug_ve_printf(debug_putc, "_ve_min(rom=%LX, n=%u) frag=%u", romStart, n, frag);
      
      while(n > 0)
      {
         read_program_memory(romLoc, data, VE_PIC_WORKAROUND_ROM_WRITE_BYTES);
         //memset(&data, 0xFF, sizeof(data));

         i = VE_PIC_WORKAROUND_ROM_WRITE_BYTES - frag;
         if (i > n)
            i = n;

         debug_ve_printf(debug_putc, " rom=%LX,i=%u", romLoc, i);

         memcpy(&data[frag], ramPtr, i);
         
         write_program_memory(romLoc, data, VE_PIC_WORKAROUND_ROM_WRITE_BYTES);
         
         romLoc += VE_PIC_WORKAROUND_ROM_WRITE_BYTES / _VE_BPA;
         
         n -= i;
         ramPtr += i;
         frag = 0;
      }
      
      debug_ve_printf(debug_putc, "\r\n");
   }
#else
   #define _ve_write_program_memory(_romLoc, _ramPtr, _n)   write_program_memory(_romLoc, _ramPtr, _n)
#endif

static void _VE_move_page(void)
{
   _g_ve_record_t record;
   _VE_ADDRESS_ curr, stop;

   curr = _g_VE.next;
   if (_g_VE.page == 0)
   {
      stop = _VE_PAGE0_START;
      _g_VE.next = _VE_PAGE1_START;
   }
   else
   {
      stop = _VE_PAGE1_START;
      _g_VE.next = _VE_PAGE0_START;
   }  
   
   debug_ve_printf(debug_putc, "_VE_move_page() oldPage=%u oldPtr=%LX newPtr=%LX\r\n", _g_VE.page, stop, _g_VE.next);
   
   _g_VE.page = !_g_VE.page;
   
   while(curr > stop)
   {
      curr -= sizeof(record)/_VE_BPA;
      
      read_program_memory(curr, &record, sizeof(record));
               
      if (!_find_virtual_eeprom(record.address, 0))
      {
         //debug_ve_printf(debug_putc, "w_%u@%LX@%LX ", record.value, (unsigned int16)record.address, _g_VE.next);
         _ve_write_program_memory(_g_VE.next, &record, sizeof(record));
         _g_VE.next += sizeof(record)/_VE_BPA;
         //debug_ve_printf(debug_putc, "n%LX\r\n", _g_VE.next);
      }
   }
   
   _ve_erase_program_eeprom(curr);

   debug_ve_printf(debug_putc, "_VE_move_page() DONE newPage=%u stopReadd=%LX nextPtr=%LX\r\n", _g_VE.page, curr, _g_VE.next);
}

unsigned int1 write_virtual_eeprom(ve_virtual_address_t addy, int8 val) {
   _g_ve_record_t record;
   
   debug_ve_printf(debug_putc, "write_virtual_eeprom(%LX,%X) ", addy, val);
   
   if((addy & _VE_INVALID_ADDRESS) == _VE_INVALID_ADDRESS)
   {
      debug_ve_printf(debug_putc, "FAIL_MAX_EEPROM\r\n");
      return(FALSE);
   }
  
   if (_g_VE.page == 0){
      if (_g_VE.next >= _VE_PAGE0_END){
         _VE_move_page();
         if (_g_VE.next >= _VE_PAGE1_END)
         {
            debug_ve_printf(debug_putc, "\r\nFAIL_PAGES_FULL_01\r\n");
            return FALSE;
         }
      }
   }
   else {
      if (_g_VE.next >= _VE_PAGE1_END){
         _VE_move_page();
         if (_g_VE.next >= _VE_PAGE0_END)
         {
            debug_ve_printf(debug_putc, "\r\nFAIL_PAGES_FULL_10\r\n");
            return FALSE;
         }
      }
   }
   
  #if (_VE_BPA != 1)
   record.addAsInstruction = 0;
   record.valAsInstruction = 0;
  #endif

   record.address = addy;
   record.value = val;
   
   //debug_ve_printf(debug_putc, "write_virtual_eeprom() %LX.%LX page=%u ptr=%LX next=%LX\r\n", record.addAsInstruction, record.valAsInstruction, _g_VE.page, _g_VE.next, (_g_VE.next+sizeof(record)/_VE_BPA));
   debug_ve_printf(debug_putc, "write_virtual_eeprom() page=%u ptr=%LX\r\n", _g_VE.page, _g_VE.next);
   
   _ve_write_program_memory(_g_VE.next, &record, sizeof(record));
   
   _g_VE.next += sizeof(record)/_VE_BPA;
   
   return(TRUE);
}

int8 read_virtual_eeprom(ve_virtual_address_t addy) {
   int8 ret = -1;
   _find_virtual_eeprom(addy, &ret);
   return(ret);
}

static int1 _find_virtual_eeprom(ve_virtual_address_t addy, int8 *pRet)
{
   _VE_ADDRESS_ ptr;
   _VE_ADDRESS_ end;
   _VE_ADDRESS_ start;
   
   _g_ve_record_t record;
   
   if((addy & _VE_INVALID_ADDRESS) == _VE_INVALID_ADDRESS)
   {
      debug_ve_printf(debug_putc, "read_virtual_eeprom(%X) FAIL_MAX_EEPROM\r\n", addy);
      return(0);
   }

   if (_g_VE.page == 0)
   {
      end = _VE_PAGE0_END - sizeof(record)/_VE_BPA;
      start = _VE_PAGE0_START;
   }
   else
   {
      end = _VE_PAGE1_END - sizeof(record)/_VE_BPA;
      start = _VE_PAGE1_START;
   }

   for (ptr=end; ptr>=start; ptr=ptr-(sizeof(record)/_VE_BPA))
   {
      read_program_memory(ptr, &record, sizeof(record));
      
      if (record.address==addy)
      {
         debug_ve_printf(debug_putc, "read_virtual_eeprom(%X) ret=%X page=%u ptr=%LX\r\n", addy, record.value, _g_VE.page, ptr);
         if (pRet != 0)
         {
            *pRet = record.value;
         }
         return(1);
      }
   }
   
   debug_ve_printf(debug_putc, "read_virtual_eeprom(%X) FAIL_NO_MATCH\r\n", addy);
   return(0);
}

void clear_virtual_eeprom(void){
   debug_ve_printf(debug_putc, "clear_virtual_eeprom() next=%X\r\n", _VE_PAGE0_START);
   _ve_erase_program_eeprom(_VE_PAGE0_START);
   _ve_erase_program_eeprom(_VE_PAGE1_START);
   _g_VE.next = _VE_PAGE0_START;
   _g_VE.page = 0;
}

unsigned int32 virtual_eeprom_max_entries(void)
{
   unsigned int32 i, j;
   i = _VE_PAGE_BYTES;
   j = sizeof(_g_ve_record_t);
   return((unsigned int32)_VE_PAGE_BYTES/sizeof(_g_ve_record_t));
}

#endif
