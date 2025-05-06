#include "16F874.h"			//Biblioteca do pic utilizado	
#use delay(clock=1000000)
#fuses XT,NOWDT,NOPUT

int X;
int conta_A = 0;
int conta_B = 0;
int flag_verde_A = 0;
int flag_verde_B = 0;

#INT_RB
void trata_rb(){
if(input(PIN_B4==0)){
    output_high(PIN_C1);
    delay_ms(1000);
    output_low(PIN_C1);
	if(flag_verde_A==1)
			conta_A++;}
if(input(PIN_B5==0)){
	if(flag_verde_B==1)
			conta_B++;}
//input_b();
}


/*
int compara(int A, int B){
	if(A <= 1.3*B)
	    return 0;
	if((A > 1.3*B) && (A<= 1.6*B))
		return 1;
	if(A > 1.6*B)
		return 2;
}*/

void main(){

	enable_interrupts(GLOBAL| INT_RB);		

	
	while(1){                                            //   D5 D4 D3 D2 D1 D0
             //CICLO_1      0b00 1  0 0  0  0  1
		output_d(0b00100001);
	/*	flag_verde_A=1;
		delay_ms(15000);
		X=compara(conta_A,conta_B); 
        flag_verde_A=0;
		if(X==0)		
			delay_ms(15000);
		if(X==1)		
			delay_ms(20000);
		if(X==2)		
			delay_ms(25000);
		conta_B=0;
		output_d(0b00010001);
		delay_ms(4000);
		output_d(0b00001100);
		flag_verde_B=1;
		delay_ms(15000);
		X=compara(conta_B,conta_A); 
        flag_verde_B=0;
		if(X==0)		
			delay_ms(10000);
		if(X==1)		
			delay_ms(10000);
		if(X==2)		
			delay_ms(15000);
		conta_A=0;
		output_d(0b00001010);
		delay_ms(4000);*/
	
	}

}
