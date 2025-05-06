#include <devkit_picrobot.h>

unsigned int16 MyAdcRead(unsigned int8 adcChan) {
   set_adc_channel(adcChan);
   delay_us(100);
   return(read_adc());
}

// return TRUE if over black or dark area.
int1 lt_check(unsigned int8 adcChan) {
   return (MyAdcRead(adcChan) > 700);
}

void main() {
   HW_INIT();
   WheelMotorInit();
   
   while(TRUE) {
      int1 l, r;
      
      l = lt_check(AN_CHANNEL_LEFT_LINE);
      r = lt_check(AN_CHANNEL_RIGHT_LINE);

      if (l && !r) { // go right
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 75);
      } else if (r && !l) {   // go left
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 75);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
      } else { // go straight
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 75);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 75);
      }
   }
}
