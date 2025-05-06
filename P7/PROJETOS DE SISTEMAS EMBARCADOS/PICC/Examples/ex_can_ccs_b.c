/////////////////////////////////////////////////////////////////////////
////                         EX_CAN_CCS_B.C                          ////
////                                                                 ////
//// Example of CCS's MCP2510 CAN library.  This example was tested  ////
//// using MCP250xxx CCS's CAN Prototype board.                      ////
////                                                                 ////
//// This example provides the firmware for the B node in CCS's      ////
//// CAN prototyping board.  Node B responds to CAN ID 0x202         ////
//// by setting the 3 LEDs to the value transmitted by Node A.       ////
//// Node B also repsonds to requests from CAN ID 0x201 by           ////
//// transmitting an A/D reading.                                    ////
////                                                                 ////
//// Using a serial port, this example also shows all traffic on the ////
//// CAN bus as received by the MCP2510.                             ////
////                                                                 ////
//// For more documentation on the MPC2510 CAN library, see          ////
//// can-mcp2510.c                                                   ////
////                                                                 ////
//// For more documentation on the CCS Can Prototype board see       ////
//// ex_can_ccs_a.c                                                  ////
////                                                                 ////                                                                ////
////  This example will work with the PCM compiler.                  ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <16F876.h>
#fuses HS,NOPROTECT,NOLVP,NOWDT
#use delay(clock=2500000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // Jumpers: 8 to 11, 7 to 12

#define CAN_DO_DEBUG TRUE

#include <can-mcp2510.c>

#define PIN_LED1  PIN_A1
#define PIN_LED2  PIN_A2
#define PIN_LED3  PIN_A3

#define LED1_HIGH output_low(PIN_LED1)
#define LED1_LOW  output_high(PIN_LED1)
#define LED2_HIGH output_low(PIN_LED2)
#define LED2_LOW  output_high(PIN_LED2)
#define LED3_HIGH output_low(PIN_LED3)
#define LED3_LOW  output_high(PIN_LED3)

int16 ms;

#int_timer2
void isr_timer2(void) {
   ms++; //keep a running timer that increments every milli-second
}

#define RESPOND_TO_ID_AD   0x201
#define RESPOND_TO_ID_LED  0x202

void main() {
   struct rx_stat rxstat;
   int32 rx_id;
   int buffer[8];
   int rx_len;


   int i;

   setup_port_a(RA0_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   for(i=0;i<8;i++) {
      buffer[i]=0;
   }

   LED1_HIGH;
   LED2_HIGH;
   LED3_HIGH;
   printf("\r\n\r\nCCS CAN EXAMPLE\r\n");
   delay_ms(1000);
   LED1_LOW;
   LED2_LOW;
   LED3_LOW;

   setup_timer_2(T2_DIV_BY_4,53,3);

   can_init();

   enable_interrupts(INT_TIMER2);   //enable timer2 interrupt
   enable_interrupts(GLOBAL);       //enable all interrupts (else timer2 wont happen)

   printf("\r\nRunning...");

   while(TRUE)
   {

      if ( can_kbhit() )   //if data is waiting in buffer...
      {
         if(can_getd(rx_id, &buffer[0], rx_len, rxstat)) { //...then get data from buffer
            if (rx_id == RESPOND_TO_ID_LED) {
               printf("Chaning LEDs\r\n\r\n");
               if (bit_test(buffer[0],0)) {LED1_HIGH;} else {LED1_LOW;}
               if (bit_test(buffer[0],1)) {LED2_HIGH;} else {LED2_LOW;}
               if (bit_test(buffer[0],2)) {LED3_HIGH;} else {LED3_LOW;}
            }
            if (rx_id == RESPOND_TO_ID_AD) {
               i=read_adc();
               printf("Sending AD reading: %X\r\n\r\n",i);
               can_putd(RESPOND_TO_ID_AD, &i, 1,1,1,0); //put data on transmit buffer
            }
         }
      }
   }
}
