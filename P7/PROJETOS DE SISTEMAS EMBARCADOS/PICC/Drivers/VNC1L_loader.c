///////////////////////////////////////////////////////////////////////////
////                       VNC1L_loader18.c                            ////
////                                                                   ////
//// This driver will take an Intel 8-bit Hex file over RS232 channels ////
//// and modify the flash program memory with the new code.  A proxy   ////
//// function is required to sit between the real loading function     ////
//// and the main code because the #org preprocessor command could     ////
//// possibly change the order of functions within a defined block.    ////
//// This code will read a hex file from the USB flash drive and use   ////
//// use it to bootload the PIC micro. The PIC reads the new hex file  ////
//// from the flsh disk through the VNC1L interface.                   ////
////                                                                   ////
//// After each good line, the loader sends an ACKLOD character.  The  ////
//// driver uses XON/XOFF flow control.  Also, any buffer on the PC    ////
//// UART must be turned off, or to its lowest setting, otherwise it   ////
//// will miss data.                                                   ////
////                                                                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////       (C) Copyright 1996, 2010 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#DEFINE BUFFER_LEN_LOD 64

char buffer[BUFFER_LEN_LOD+3];
int8 datastream_BL[DATA_STREAM_BUF_SIZE+3];

#DEFINE ACKLOD 0x06
#DEFINE XON    0x11
#DEFINE XOFF   0x13

#DEFINE SHORT_RDF_COMMAND  0x0B

#SEPARATE
unsigned int atoi_b16(char *s);
#SEPARATE
void USBMasterOpenFile_BL(char * filename);
#SEPARATE
void USBMasterReadFile_BL(void);
#SEPARATE
void usb_putc_BL(char c);
#SEPARATE
void USBMasterCallback_BL(void);
#SEPARATE
int1 DecodeAndWrite(int8 buffidx);

#ORG LOADER_ADDR+10, LOADER_END auto=0 default

#use rs232(baud=9600, UART1, stream=USBLDR, ERRORS)
void real_load_program(void)
{
   int8 i;
   int1  do_ACKLOD,done=FALSE;
   char filename[STRING_SIZE];
   #if getenv("FLASH_ERASE_SIZE")>2
      int32 next_addr;
   #endif
   static int8  dataidx, buffid, count;
   
   buffid = 0;
   for(i=0;i<DATA_STREAM_BUF_SIZE+3;i++)
   {  buffer[i]=0;
      datastream_BL[i]=0;
   }

   buffid = 0;  // Read into the buffer until 0x0D('\r') is received or the buffer is full
   done = FALSE;
   while(!done)  // Loop until the entire program is downloaded
   {      
      USBMasterReadFile_BL(); // Send command to read 64 bytes
      do {
         USBMasterCallback_BL();         
      } while(start_read);  // Do a callback till reading complete
      
      for(i=0;i<DATA_STREAM_BUF_SIZE;i++)
      {  
         if(done)
            break;
         if(datastream_BL[i]== ':') // 
         {        
            for(buffid=0;buffid<DATA_STREAM_BUF_SIZE; buffid++)
              buffer[buffid]=0; // clear buffer
            for(buffid=0;buffid<DATA_STREAM_BUF_SIZE; buffid++)
            {
               if(datastream_BL[i]==0x0D)                
                  break; // go out of for loop
               if(datastream_BL[i]==0x3E) // Wait for next line
                  goto ClearDataStream;
               buffer[buffid]=datastream_BL[i];
               i++;
            }
            
            done = DecodeAndWrite(buffid);
            // go to next line, skip LF(0x0A)  
            for(buffid=0;buffid<DATA_STREAM_BUF_SIZE;buffid++)
               buffer[buffid]=0; // clear buffer after write is complete
            buffid=0; // reset index
          }
          else
          {
             if(datastream_BL[i]==0x0D)
             {
                done = DecodeAndWrite(buffid);
             }
             else
             {
                buffer[buffid]= datastream_BL[i];             
                buffid++;
             }
          }
      }
      ClearDataStream:
      for(i=0;i<DATA_STREAM_BUF_SIZE+2;i++)
      datastream_BL[i]=0;
    }      

   #ifndef _bootloader
   reset_cpu();
   #endif
}

int1 DecodeAndWrite(int8 buffidx)
{
   #if getenv("FLASH_ERASE_SIZE")>2
      int32 next_addr;
   #endif
   int8  dataidx, i,k, count;
   int1  do_ACKLOD,done=FALSE;
   int8  checksum, line_type;
   static int16 l_addr,h_addr=0;
   int32 addr;
   int8  data[32];
   
   do_ACKLOD = TRUE;

   // Only process data blocks that start with ':'
   if(buffer[0] == ':'){
            
      count = atoi_b16(&buffer[1]);  // Get the number of bytes from the buffer

      // Get the lower 16 bits of address
      l_addr = make16(atoi_b16(&buffer[3]),atoi_b16(&buffer[5]));
      line_type = atoi_b16(&buffer[7]);

      

      #if defined(__PCM__)  // PIC16 uses word addresses
         addr /= 2;
      #endif
      done = FALSE;
         // If the line type is 1, then data is done being sent
      if(line_type == 4)
         h_addr = make16(atoi_b16(&buffer[9]), atoi_b16(&buffer[11]));  
         
      addr = make32(h_addr,l_addr);
      
      if(line_type == 1){
         done = TRUE;
      #if defined(__PCM__)
      } else if((addr < LOADER_ADDR){
      #elif defined(__PCH__)
      } else if(addr < LOADER_ADDR) 
      {
      #endif
         checksum = 0;  // Sum the bytes to find the check sum value
         for(i=1; i<(buffidx-3); i+=2)
            checksum += atoi_b16(&buffer[i]);
         checksum = 0xFF - checksum + 1;

         if(checksum != atoi_b16(&buffer[buffidx-2]))
            do_ACKLOD = FALSE;
         else   {
            if(line_type == 0){
               // Loops through all of the data and stores it in data
               // The last 2 bytes are the check sum, hence buffidx-3
               for(i = 9,dataidx=0; i < buffidx-2; i += 2)
                  data[dataidx++]=atoi_b16(&buffer[i]);
                #if getenv("FLASH_ERASE_SIZE") > getenv("FLASH_WRITE_SIZE")
                  #if defined(__PCM__)
                     if((addr!=next_addr)&&(addr&(getenv("FLASH_ERASE_SIZE")-1)!=0))
                  #else
                     if((addr!=next_addr)&&(addr&(getenv("FLASH_ERASE_SIZE")/2-1)!=0))
                  #endif
                        erase_program_eeprom(addr);
                  next_addr = addr + 1;
               #endif
               write_program_memory(addr, data, count);
            }            
         }
      }
   }

      return done;
    
}

unsigned int atoi_b16(char *s){  // Convert two hex characters to a int8
   unsigned int result = 0;
   int i;

   for(i=0; i<2; i++,s++)  {
      if(*s >= 'A')
         result = 16*result +(*s) - 'A' + 10;
      else
         result = 16*result +(*s) - '0';
   }

   return(result);
}


void usb_putc_BL(char c)
{
   output_low(USB_CTS);
   while(input(USB_RTS));
   fputc(c, USBLDR);
   output_high(USB_CTS);   
}

void USBMasterOpenFile_BL(char * filename)
{
   int8 i;
   
   for(i=0;i<DATA_STREAM_BUF_SIZE; i++)
   datastream_BL[i]= 0; // clear the data      
   
   usb_putc_BL(0x0E);  // Send the read command   
   usb_putc_BL(0x20);  
   do
   {
      usb_putc_BL(*filename++);
   }while(*filename!=0);  
   
   usb_putc_BL("\r");
}

void USBMasterReadFile_BL(void)
{
   start_read=1;
   end_read = 1;
   usb_putc_BL(SHORT_RDF_COMMAND);  // Short Read command  0x0B
   usb_putc_BL(0x20); // Send space
   usb_putc_BL(0x0);
   usb_putc_BL(0x0);
   usb_putc_BL(0x6);
   usb_putc_BL(0x4);  // Byte to read = 64
   usb_putc_BL(0x0D);    // Carriage return after short command
}

void USBMasterCallback_BL(void)
{
   int8 i;
   char c;
   if(!input(USB_RTS))
      output_low(USB_CTS);
      
   if(kbhit(USBLDR))
   {
      c=fgetc(USBLDR);
         
     #ifdef USE_READ_BUFFER
     if(end_read)
     {
       i=0; // reset buffer index if read has ended
     }
     if(start_read)
     {
        end_read = 0; // reset the flag
        datastream_BL[i]= c;
        if(i++==DATA_STREAM_BUF_SIZE+2)
           i=0; // Reset index
        if(i == BUFFER_LEN_LOD+2) // Read end @ 64 bytes max
        {
           start_read = 0;
           end_read = 1;
        }
     }
     #endif
   }            

}

#ORG default

#ORG LOADER_ADDR, LOADER_ADDR+9
void load_program(void)
{
   real_load_program();
}
