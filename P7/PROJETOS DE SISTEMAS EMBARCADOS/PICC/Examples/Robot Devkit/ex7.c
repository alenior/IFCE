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

void main() {
   char switchCase;

   HW_INIT();
   
   WheelMotorInit(); // Initialize wheel motor control
   
   printf("\r\n(F)orward, (B)ackward, (L)eft, (R)ight, (S)top\r\n");
   
   for(;;) {
      HW_TASK();
      
      switchCase = toupper(getc());
      
      switch(switchCase) {
         case 'F': // Move the robot forward
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 100);
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 100);
            break;
         case 'B': // Move the robot backward
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, -100);
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -100);
            break;
         case 'L': // Turn the robot left / counter-clockwise
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 75);
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -75);
            break;
         case 'R': // Turn the robot right / clockwise
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, -75);
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 75);
            break;
         case 'S': // Stop the robot
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0);
            WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
            break;
      }
   }
}
