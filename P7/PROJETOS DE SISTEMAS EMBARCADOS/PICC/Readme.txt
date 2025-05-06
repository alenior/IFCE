Changes since the last manual:

PIC18 parts with external ROM can direct the compiler to use the ROM
like this:
        #build(memory=0x20000:0x2FFFF)
For chips with no internal ROM you must use a line like the above.

The reset and interrupt vectors can be moved from the normal location
using #build like this:

         #build(reset=0x200, interrupt=0x208)

    A larger than normal area may be reserved like this:

         #build(reset=0x200:0x207, interrupt=0x208:0x2ff)

The ADC functions have been updated to support the newest chips.  Many
of the constants used for the functions have changed however for a couple
of months the old constants will be included in the .h files.  See your
devices .h file for details.

New PIC18 optimization has been added.  To use the new optimization
add:
           #opt 10
             or
           #opt 11

A new directive has been added to specify the data to be used
to fill unused ROM locations.  For example:
       #fill_rom  0x0063

New directive to prevent WRITE_EEPROM from hanging while
the write takes place:
         #device WRITE_EEPROM=ASYNC
If you use this do not write to the EERPOM from both an ISR
and outside an ISR.

A new function CLEAR_INTERRUPT has been added.  For example:
        clear_interrupt( INT_EXT );

The #ROM directive now creates a segment for the #ROM data.  This means you
will get an error if you #ORG over the same area.  This was not previously an
error.  It also means #ROM data is counted as used program memory space.  These
changes only apply if the #ROM address is inside the program memory space.


A new function has been added:  SETUP_UART that works the same as SET_UART_SPEED.
A new feature of this function is when a FALSE (or 0) is passed it turns off the UART
and when a TRUE (or 1) is passed in it turns the UART on.  Passing in a baud rate also
turns on the UART.  The following constants may also be used on chips with the advanced
UART:
        UART_ADDRESS - UART only accepts data with 9th bit =1
        UART_DATA    - UART accepts all data

New options for #UASE RS232:
   SAMPLE_EARLY A getc() normally samples data in the middle of a bit time.  This option causes the
                sample to be at the start of a bit time.  May not be used with the UART.
   RETURN=pin   For FLOAT_HIGH and MULTI_MASTER this is the pin used to read the signal back.
                The default for FLOAT_HIGH is the XMIT pin and for MULTI_MASTER the RCV pin.
   MULTI_MASTER Uses the RETURN pin to determine if another master on the bus is transmitting at
                the same time.  If a collision is detected bit 6 is set in RS232_ERRORS and all
                future PUTC's are ignored until bit 6 is cleared.  The signal is checked at the
                start and end of a bit time.  May not be used with the UART.
   LONG_DATA    Makes getc() return an int16 and putc accept an int16.  This is for 9 bit data formats.
   DISABLE_INTS Will cause interrupts to be disabled when the routines get or put a character.
                This prevents character distortion for software implemented I/O and prevents interaction
                between I/O in interrupt handlers and the main program when using the UART.

   See RS485.C in the drivers directory for an example RS485 protocol implementation.

A new function SETUP_OSCILLATOR controls and returns the state of the
internal RC oscillator on some parts.  See the devices .h file for
valid options for a particular device.  Note that is INTRC or INTRC_IO
is specified in #fuses and a #USE DELAY is used for a valid speed option,
then the compiler will do this setup automatically at the start of main().
          WARNING:  If the speed is changed at run time the compiler
                    may not generate the correct delays for some built
                    in functions.  The last #USE DELAY encountered in the
                    file is always assumed to be the correct speed.  You
                    can have multiple #USE DELAY lines to control the
                    compilers knowledge about the speed.

To instruct the compiler to use the extermal program memory in a part
add the following line after the devices .h file is included:
         #BUILD( memory=start:end )
Where start and end represent the external address space.

A new built in function OUTPUT_TOGGLE(pin) will toggle the state of
a specified pin.  The parameter is the same as OUTPUT_HIGH.

A new preprocessor directive has been added to make serial numbers easier
to implement.  This new feature only works with the CCS ICD units.  It is
implemented by inserting comments in the hex file that the ICD.EXE program
understands.

        #serialize(id=xxx, file="filename.txt", listfile="filename.txt",
                   prompt="text", log="filename.txt", next="xxx")

           id=xxx   Specify a C CONST identifier, may be int8,int16,int32 or char array

                   Use only one of the next three options:
           file="x"       The file x is used to read the serial number from, and this
                          file is updated by the ICD programmer.  It is asumed this is
                          a one line file with the serial number.  The programmer will
                          increment the serial number.
           listfile="x"   The file x is used to read the serial number from, and this
                          file is updated by the ICD programmer.  It is asumed this is
                          a file one serial number per line.  The programmer will read the
                          first line then delete that line from the file.
           next="x"       The serial number X is used for the first load, then the hex file
                          is updated to increment x by one.

           prompt="text"  This option is optional.  If specified the user will be prompted
                          for a serial number on each load.  If used with one of the above
                          three options then the default value the user may use is picked
                          according to the above rules.

           log="xxx"      A file may optionaly be specified to keep a log of the date, time,
                          hex file name and serial number each time the part is programmed.
                          If no id=xxx is specified then this may be used as a simple log
                          of all loads of the hex file.

#USE I2C now accepts an SMBUS option.

The CCSC program will now accept a +DF command line option to enable the
output of a COFF debug file.  MPLAB 6 may work better with this new file
format.  The MPLAB plug-in will be updated to make this the default.

A new capability has been added to define a memory region.  For example:
         typemod <,,,0x110,0x16f> banktwo;
Will define a memory space named banktwo.  The first three options for
the typemod are for future expansion.  The new type specifier may be used
like this:
         char banktwo buffer[20];

The read_adc() function has an optional parameter to control what read_adc does.
The default is ADC_START_AND_READ however you may also specify ADC_START_ONLY
to start a conversion (read_adc() returns nothing) and ADC_READ_ONLY to read
the last result.  For example:
      read_adc(ADC_START_ONLY);
      sleep();
      value=read_adc(ADC_READ_ONLY);

New capabilities for the 629,630,675,676 chips:
    The PORT_B_PULLUPS function accepts a number like SET_TRIS_B
    to enable specific pins.  For some chips this function is
    named PORT_A_PULLUPS.

    enable/disable_interrupt() functions have constants (see .h file)
    defned to enable/disable change on specific pins.  For example:
              enable_interrupts(INT_RA1);

A new error file format is now available.  This new format is invoked
with the following command line options:
             +EW  Show warning messages
             +EA  Show all error messages and all warnings
             +EA -EW Show all error messages and no warnings
This is invoked in PCW from OPTIONS > FILE FORMATS.  This is not yet
available in MPLAB 6.  CCS has not yet removed warning conditions from
the example programs and include files.

A new pre-processor directive may be used to surpress certain warnings:
              #IGNORE_WARNINGS  ALL
              #IGNORE_WARNINGS  NONE
              #IGNORE_WARNINGS  201
              #IGNORE_WARNINGS  201,205,212

A new function has been added to make program memory writes more effective
for the variety parts now available:
           WRITE_PROGRAM_MEMORY( address, dataptr, count );

           address is a byte address in PIC18 and PIC16, the LSB should be 0
           dataptr points to the data to write
           count is a count of bytes to write

     Notes:
       1. This function is most effective when count is a multiple of FLASH_WRITE_SIZE.
       2. Whenever this function is about to write to a lcoation that is a multiple
          of FLASH_ERASE_SIZE then a erase is performed on the whole block.

The following keywords have been added to the GETENV() function:
    FLASH_WRITE_SIZE    Smallest number of bytes that can be written to FLASH
    FLASH_ERASE_SIZE    Smallest number of bytes that can be erased in FLASH

The SETUP_CCPx and SET_PWMx_DUTY functions now work for up to 5 CCP
units on chips that have them.

The built in function SET_UART_SPEED now accepts a stream identifier
as the second parametter.  For example:
                  SET_UART_SPEED(9600,GPS);

#ORG now accepts a DEFAULT option that will cause it to apply
to all functions including hidden math functions until a #ORG DEFAULT
(with no address) is reached.  See loader.c for an example.


We added some support for PIC18 priority interrupts.  You can define
one interrupt as priority like this:
           #INT_RTCC  FAST
           isr() {
              ...
           }
A fast interrupt can interrupt another interrupt handler.  The compiler
does no save/restore in a fast ISR.  You should do as little as possible
and save any registers that need to be saved on your own.


The READ_PROGRAM_EEPROM and WRITE_PROGRAM_EEPROM functions for PIC18 now
return 16 bits.  The manual indicates it is 8 bits.  New functions are
comming that will will have more options.


Arrays may be defined with [] in many cases.  For example:
               const char id[] = {"Hi There"};    // Same as [9]
               int x[];                           // Same as *x
               int x[] = {1,2,3};                 // Same as [3]

The #USE I2C option NOFORCE_SW is still accepted however the new FORCE_HW
    is what will appear in future documentation.

#INT_???? directives now allow a NOCLEAR option to prevent the compiler
    from clearing the interrupt.  For example:
             #INT_RTCC NOCLEAR
             isr() {
                ...
             }

The compiler now accepts trigraph sequences for keyboards that do not have
the following: # [ ] \ ^ { } | ~
The following three character sequences are translated to the indicated one char:
               ??=    #
               ??(    [
               ??/    \
               ??)    ]
               ??'    ^
               ??<    {
               ??!    |
               ??>    }
               ??-    ~

Some new functions have been added to allow for unorthodox jumps.
        x = label_address(label);   Will return the ROM address of the label
        goto_address(x);            Will jump to the ROM address x

        r = setjmp(env);            Standard C function (requires setjmp.h)
        longjmp(env,val);           Standard C function (requires setjmp.h)

     Use these functions with GREAT caution.  Note as well that setjmp and
     longjmp do not clean up the stack on 12 and 14 bit parts.

The standard functions iscntrl,isgraph,isprint and ispunct are now in ctype.h.

The compiler now supports the offsetof() and offsetofbit() functions.  offsetof()
requires stddef.h be included.   These functions return the offset (in bytes and
bits) of the field within a structure.

The defines __FILE__, __LINE__ and __TIME__ are now supported.

#USE RS232 has been updated to allow a stream identifier for the port.  This
identifier may then be used in fgetc,fgets,fprintf,and fputc to indicate what
port to use for the function.  For example:
      #use rs232(baud=9600,xmit=pin_c6,rcv=pin_c7,stream=HOSTPC)
      #use rs232(baud=1200,xmit=pin_b1,rcv=pin_b0,stream=GPS)
      #use rs232(baud=9600,xmit=pin_b3,stream=DEBUG)
      ...
      while(TRUE) {
         c=fgetc(GPS);
         fputc(c,HOSTPC);
         if(c==13)
           fprintf(DEBUG,"Got a CR\r\n");
      }

The following functions have been added to math.h: sinh, cosh, tanh, fabs,
fmod, atan2, frexp, ldexp, modf

The include file "errno.h" may be included to enable error checking in the
math functions.  Since this checking takes extra ROM the checking is only
done if errno.h has been included before math.h.  The function strerror()
may be used to format an error message.

The standard C header files float.h, limits.h, stddef.h and locale.h are provided
to make these compiler constants available to those who would like to use them.

The assert macro is now supplied in assert.h.  It may be used to test for a condition
and if false will output an error on STDERR.  By defualt STDERR is the first RS232
port defined in a program.  If NODEBUG is #defined then no code is generated for the
assert macro.  Example:
                 assert( number_of_entries < TABLE_SIZE );

The built-in function SPRINTF has been added.  For example:
               char s[10];
               long i;

               sprintf(s,"%lu",i);

The built in function getenv() has been added.  The syntax is
      value = getenv(cstring);
             cstring is a constant string with a recognised keyword
             as follows:
                 FUSE_SET:fffff     Returns 1 if fuse fffff is enabled
                 FUSE_VALID:fffff   Returns 1 if fuse fffff is valid
                 INT:iiiii          Returns 1 if the interrupt iiiii is valid
                 ID                 Returns the device ID (set by #ID)
                 DEVICE             Returns the device name string (like "PIC16C74")
                 CLOCK              Returns the Osc clock value (from a #USE DELAY)
                 VERSION            Returns the compiler version as a float
                 VERSION_STRING     Returns the compiler version as a string
                 PROGRAM_MEMORY     Returns the size of memory for code (in words)
                 STACK              Returns the stack size
                 DATA_EEPROM        Returns the number of bytes of data EERPOM
                 READ_PROGRAM       Returns a 1 if the code memory can be read
                 PIN:pb             Returns a 1 if bit b on port p is on this part
                 ADC_CHANNELS       Returns the number of A/D channels
                 ADC_RESOULTION     Returns the number of bits returned from READ_ADC()
                 ICD                Returns a 1 if this is being compiled for a ICD
                 SPI                Returns a 1 if the device has SPI
                 USB                Returns a 1 if the device has USB
                 CAN                Returns a 1 if the device has CAN
                 I2C_SLAVE          Returns a 1 if the device has I2C slave H/W
                 I2C_MASTER         Returns a 1 if the device has I2C master H/W
                 PSP                Returns a 1 if the device has PSP
                 COMP               Returns a 1 if the device has a comparator
                 VREF               Returns a 1 if the device has a voltage reference
                 LCD                Returns a 1 if the device has direct LCD H/W
                 UART               Returns the number of H/W UARTs
                 CCPx               Returns a 1 if the device has CCP number x
                 TIMERx             Returns a 1 if the device has TIMER number x
       Examples:
             #IF  getenv("VERSION")<3.050
             #ERROR  Compiler version too old for this program
             #ENDIF

             for(i=0;i<getenv("DATA_EEPROM");i++)
               write_eeprom(i,0);

             #IF getenv("FUSE_VALID:BROWNOUT")
             #FUSE BROWNOUT
             #ENDIF

The PCW IDE F4 command has been removed and replaced with a feature that
allows you to move your cursor over a (,),{ or } and the matching item
will highlight.


