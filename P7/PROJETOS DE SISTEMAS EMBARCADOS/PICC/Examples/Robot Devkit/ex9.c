#include <devkit_picrobot.h>

unsigned int16 MyAdcRead(unsigned int8 chan) {
   set_adc_channel(chan);
   delay_us(100);
   return(read_adc());
}

void main () {
   HW_INIT();
   
   setup_adc_ports(sAN2 | sAN3 | sAN4 | sAN5 | sAN6 | sAN7);
   setup_adc(ADC_CLOCK_INTERNAL);
   
   WheelMotorInit();
   
   for(;;) {
      if
      (
         (MyAdcRead(AN_CHANNEL_LEFT_PROXIMITY) < 400) 
         && (MyAdcRead(AN_CHANNEL_RIGHT_PROXIMITY) < 400)
      )
      {
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 75);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 75);
      } 
      else if 
      (
         (MyAdcRead(AN_CHANNEL_LEFT_PROXIMITY) > 600) 
         || (MyAdcRead(AN_CHANNEL_RIGHT_PROXIMITY) > 600)
      )
      {
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_RIGHT, 0);
         WheelMotorSetSpeed(WHEEL_MOTOR_IDX_LEFT, 0);
      }
   }
}
