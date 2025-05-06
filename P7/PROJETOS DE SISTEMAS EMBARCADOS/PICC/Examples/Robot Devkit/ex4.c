#if 0
   // see documentation in devkit_picrobot.h
   #define USE_RS232_USB_CDC
   #define USE_RS232_USB_CDC_WAIT_FOR_CONNECTION
#endif

#include <devkit_picrobot.h>

#ifndef USE_RS232_USB_CDC
   #use rs232(ICD)
#endif

void main() { 
   HW_INIT();
   
   while(TRUE) {
      HW_TASK();
      
      printf("Hello world!\r\n");
      delay_ms(2000);
   }
}
