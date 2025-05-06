
#define _bootloader

#include <bootloader.h>
#include <loader.c>


void main(void) {

   if(!input(PIN_B4))
      real_load_program();
   
   application();
}

#if defined(__PCM__)
   #int_global 
   void isr(void) {
     #asm
       goto LOADER_END+5
     #endasm
   }
#elif defined(__PCH__)
   #int_global 
   void isr(void) {
     #asm
       goto LOADER_END+9
       goto LOADER_END+9
       goto LOADER_END+9
       goto LOADER_END+9
       goto LOADER_END+0x19
     #endasm
   }
#endif


#org LOADER_END+1,LOADER_END+10
void application(void) {
  while(TRUE);
}

