
#include <16f874.h>   //Biblioteca do PIC utilizado  

#use delay(clock=1000000)
#fuses XT,NOWDT,NOPUT

int fluxo_a = 0;
int fluxo_b = 0;
int verdeA = 0;
int verdeB = 0;
int tempo = 0;

#INT_RB
void trata_rb(){
	if(input(PIN_B4) == 0){
       if(verdeA == 1)
       		fluxo_a++;
    }
   	if(input(PIN_B5) == 0){
        if(verdeB == 1)
       		fluxo_b++;
    }
}

int compara(int fluxoA, int fluxoB){
    if(fluxoA <= 1.3*fluxoB){
		return 0;}
    if((fluxoA > 1.3*fluxoB) && (fluxoA <= 1.6*fluxoB)){
		return 1;}
    if(fluxoA > 1.6*fluxoB){
		return 2;}
}
 

void main() {
 	enable_interrupts(GLOBAL | INT_RB);
	while(TRUE){
		output_d(0b00001100); //0 0 verde2 amar2 verm2 verde1 ama1 verm1
        verdeA = 1;
		delay_ms(15000);
        verdeA = 0;
    	tempo = compara(fluxo_a, fluxo_b);
        fluxo_b = 0;
		if(tempo == 0)
			delay_ms(15000);
		if(tempo == 1)
			delay_ms(20000);
		if(tempo == 2)
			delay_ms(25000);
	output_d(0b00001010); // 0 0 verde2 amar2 verm2 verde1 ama1 verm1
        delay_ms(4000);
   }
   
   enable_interrupts(GLOBAL | INT_RB);
	while(TRUE){
		output_d(0b00001100); //0 0 verde2 amar2 verm2 verde1 ama1 verm1
        verdeB = 1;
		delay_ms(15000);
        verdeB = 0;
    	tempo = compara(fluxo_a, fluxo_b);
        fluxo_a = 0;
		if(tempo == 0)
			delay_ms(15000);
		if(tempo == 1)
			delay_ms(20000);
		if(tempo == 2)
			delay_ms(25000);
	output_d(0b00001010); // 0 0 verde2 amar2 verm2 verde1 ama1 verm1
        delay_ms(4000);
   }
}
