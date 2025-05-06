#include "16f874.h"  //biblioteca do pic
#use delay (clock = 1000000)
#fuses XT,NOWDT,PUT
int fluxo_a =0;
int fluxo_b =0;
int verdeA = 0;
int verdeB = 0;
int tempo=0;
INT status = 0;
#int_RB
void trata_rb(){
   if(input(PIN_B4) == 0){
      if(verdeA ==1)
         fluxo_a++;
   }
   if(input(PIN_B5) == 0){
      if(verdeB ==1)
         fluxo_b++;
   } 
}

int compara(int fluxoA, int fluxoB){
   if(fluxoA <= 1.3 * fluxoB){
      return 0;
   }
   if((fluxoA >= 1.3 * fluxoB) && (fluxoA <= 1.6 * fluxoB)){
      return 1;
   }
   if(fluxoA > 1.6 * fluxoB){
      return 2;
   }
   
}


void main(){
   enable_interrupts(GLOBAL | INT_RB);
   
   while(true){
      output_d(0b00001100); //vd_a e vm_b
      verdeA = 1;
      delay_ms(15000);
      verdeA = 0;
      
      output_d(0b00010001); //am_b e vm_a
      delay_ms(4000);
      output_d(0b00001100); //vm_b e vd_a
      delay_ms(30000);
      output_d(0b00001010);// vm_b e am_a
      delay_ms(4000);
   }
}

