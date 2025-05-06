#if 0
   // see documentation in devkit_picrobot.h
   #define USE_RS232_USB_CDC
   #define USE_RS232_USB_CDC_WAIT_FOR_CONNECTION
#endif

#include <devkit_picrobot.h>

#ifndef USE_RS232_USB_CDC
   #use rs232(ICD)
#endif

#include <stdlib.h>
#include <input.c>

struct {
   struct motor {
      int8 speed, dir;
   } l, r;
   
   int8 time;
} cfgs[10];

void main() {
   HW_INIT();
   
   WheelMotorInit();
   
   for(;;) {
      int8 i, j, time;
      
      for (i=0; i<10; i++) {
         printf("\r\nPROGRAM POINT %u", i+1);
         do {
            printf("\r\nLeft Speed: ");
            cfgs[i].l.speed = get_int();
         } while (cfgs[i].l.speed > 100 || cfgs[i].l.speed < -100);
         
         do {
            printf("\r\nLeft directions (F, B): ");
            cfgs[i].l.dir = toupper(getc());
            if ((cfgs[i].l.dir == '\r') || (cfgs[i].l.dir == '\n')) cfgs[i].l.dir = 'F';
         } while ((cfgs[i].l.dir != 'F') && (cfgs[i].l.dir != 'B'));
         printf("%c", cfgs[i].l.dir);
         
         do {
            printf("\r\nRight Speed: ");
            cfgs[i].r.speed = get_int();
         } while(cfgs[i].r.speed > 100 || cfgs[i].r.speed < -100);
         
         do {
            printf("\r\nRight direction (F, B): ");
            cfgs[i].r.dir = toupper(getc());
            if ((cfgs[i].r.dir == '\r') || (cfgs[i].r.dir == '\n')) cfgs[i].r.dir = 'F';
         } while ((cfgs[i].r.dir != 'F') && (cfgs[i].r.dir != 'B'));
         printf("%c", cfgs[i].r.dir);
         
         printf("\r\nTime in sec: ");
         cfgs[i].time = get_int();
         
         if (cfgs[i].time == 0)
         {
            break;
         }
         
         printf("\r\n");
      }
      
      printf("\r\n\r\nSET %U POINTS\r\n\r\n", i);
      
      for(i=0; i<10; i++) {
         if (cfgs[i].time == 0)
            break;
         
         printf("RUNNING POINT %u\r\n", i+1);
         
         if (cfgs[i].r.dir == 'F') {
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, cfgs[i].r.speed);
         } else {
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, -cfgs[i].r.speed);
         }
         
         if (cfgs[i].l.dir == 'F') {
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, cfgs[i].l.speed);   
         } else {
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -cfgs[i].l.speed);
         }
         
         for(j=0; j < cfgs[i].time; j++) {
            delay_ms(1000);
         }
      }
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0); // Stop the motors
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
   }
}
