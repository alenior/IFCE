#include <devkit_picrobot.h>

void main() {
   HW_INIT();
   WheelMotorInit();    // Initializes motor control
   delay_ms(3000);      // Waits 3 seconds before moving
   
   while(TRUE) {
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 100); // Go forward at max speed
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 100);
      delay_ms(3000);
      
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -100);  // Spin in a tight circle
      delay_ms(2000);
      
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0);    // Stops for one second
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
      delay_ms(1000);
      
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, -25);  // Goes backwards slowly
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, -25);
      delay_ms(5000);
      
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 25);    // Turn in a large circle
      WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 75);  
      delay_ms(4000);
   }
}
