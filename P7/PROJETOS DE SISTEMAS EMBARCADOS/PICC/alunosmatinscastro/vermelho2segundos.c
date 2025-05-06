#include "16F874.h"

#use delay(clock=1000000)
#fuses XT,NOWDT,NOPUT //xt frequencia menor que 4Mhz, nowdt -> nao utilizar watch dog time, noput -> Nao utiliza pequeno delay

void main(){
   while(TRUE){
   
   output_d(0b00100001);
   delay_ms(15000);
   output_d(0b00010001);
   delay_ms(2000);
   output_d(0b00001001);
   delay_ms(2000);
   output_d(0b00001100);
   delay_ms(15000);
   output_d(0b00001010);
   delay_ms(2000);
   output_d(0b00001001);
   delay_ms(2000);
   }
}
