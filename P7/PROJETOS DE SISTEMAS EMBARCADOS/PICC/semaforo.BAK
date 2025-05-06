#include <16f874.h>

#use delay(clock=1000000)
#fuses XT,NOWDT,NOPUT

void main() {

   enable_interrupts(GLOBAL | INT_RB);

   while(TRUE){

      // output_d(0b76543210)
      output_d(0b00001100);   // A - verd B - verm
      delay_ms(30000);
      
      output_d(0b00001010);   // A - amar B - verm
      delay_ms(4000);

      output_d(0b00001001);   // A - verm B - verm
      delay_ms(2000);

      output_d(0b00100001);   // A - verm B - verd
      delay_ms(30000);
      
      output_d(0b00010001);   // A - verm B - amar
      delay_ms(4000);
      
      output_d(0b00001001);   // A - verm B - verm
      delay_ms(2000);
  }
}
