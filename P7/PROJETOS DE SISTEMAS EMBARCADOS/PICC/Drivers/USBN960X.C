/////////////////////////////////////////////////////////////////////////
////                      usbn960x.c                                 ////
////                                                                 ////
////    National USBN960x Hardware layer for CCS's PIC USB driver.   ////
////                                                                 ////
//// This file is part of CCS's PIC USB driver code, which includes: ////
////   usb_desc.h - an example set of config and device descriptors  ////
////   usb.c - USB token and request handler code                    ////
////   usb.h - definitions, prototypes and global variables          ////
////                                                                 ////
//// ex_usb_scope.c, an example written specifically for the         ////
//// USBN960x, shows how to use the CCS PIC USB driver with the      ////
//// USBN960x.                                                       ////
////                                                                 ////
//// usbn960x.c is the hardware layer level driver for the CCS PIC   ////
//// USB driver.  You can replace usbn960x.c with another hardware   ////
//// layer level and still use the other parts of the PIC USB        ////
//// driver.  ex_usb_hid.c shows an example of using the Microchip   ////
//// PIC16C765 (a PIC with a Slow speed USB Peripheral).             ////
////                                                                 ////
//// This example USB peripheral layer was written and tested with   ////
//// CCS's USB Full Speed demo board.  When using your own design,   ////
//// either wire your USB960x the same as our demo board or change   ////
//// the port and pin definitions below.                             ////
////                                                                 ////
////   *************************  NOTE  **************************   ////
//// This driver uses INT_EXT.  It requires INT_EXT to interrupt the ////
//// PIC when an event has happened on the USBN960x.  Because of     ////
//// this code enables interrupts.  A user modification can be made  ////
//// to poll the USBN960x device instead of relying on an interrupt. ////
////                                                                 ////
////    **********************  FUNCTIONS  ***********************   ////
////                                                                 ////
//// usb_init() - Initializes the USB code and USBN960x device.      ////
////              NOTE - this enables interrupts.                    ////
////                                                                 ////
//// usb_isr() - Call this if you choose to poll instead of          ////
////             interrupt.  READ THE COMMENTS AT USB_ISR() FIRST.   ////
////                                                                 ////
//// usb_put_packet() - Sends one packet to the host.                ////
////                    If you need to send a message that spans     ////
////                    more than one packet then see usb_puts() in  ////
////                    usb.c                                        ////
////                                                                 ////
//// usbn_get_version() - Returns the revision number of the 960x    ////
////                                                                 ////
//// For more documentation on these functions read the comments at  ////
//// each function.                                                  ////
////                                                                 ////
//// The other functions defined in this file are for use by the     ////
//// USB code, and is not meant to be used by the user.              ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// Version History:                                                ////
////                                                                 ////
//// October 27th, 2003: RX/TX - IN/OUT Backwards                    ////
////                             in usb_set_configured()             ////
////                                                                 ////
//// June 18th, 2003: Fixed a problem where PIC would miss interrupt ////
////                                                                 ////
//// May 6th, 2003: Fixed a potential read/write to registers        ////
////                                                                 ////
//// August 2nd, 2002: Initial Public Release                        ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2002 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////


#IFNDEF __USB_HARDWARE__
#DEFINE __USB_HARDWARE__

#include <usb.h>

#define __USBN__ 1   //let other parts of the code know that we are running a National USB Part

#define USB_MAX_ENDPOINTS 4 //number of bidrectional endpoints.  960x has 1 control port and 6 individual endpoints = 4 bidrectional endpoints

//CCS USB demo board is connected to the USBN960x in this manner:
#ifndef usbn_out
   #define  usbn_bus_out(x)      output_d(x)
   #define  usbn_bus_in          input_d
   #define  usbn_bus_float()     set_tris_d(0xFF)
   #define  usbn_bus_control()   set_tris_d(0)
#endif

//CCS USB demo board is connected to the USBN960x in this manner:
#ifndef  USBN_CS
   #define  USBN_INT       PIN_B0   //leave at B0 if you want to use external interrupt
   #define  USBN_RS        PIN_C1   //reset
   #define  USBN_CS        PIN_C2
   #define  USBN_RD        PIN_C3
   #define  USBN_WR        PIN_C4
   #define  USBN_A0        PIN_C5
#endif

#if defined(__PCM__)
 #bit int_ext_flag=0x0B.1
#elif defined(__PCH__)
 #bit int_ext_flag=0xFF2.1
#endif

//usbn9603/9604 register set
#define     USBN_MCNTRL    0x00
#define     USBN_CCONF     0x01
//          reserved       0x02
#define     USBN_RID       0x03
#define     USBN_FAR       0x04
#define     USBN_NFSR      0x05
#define     USBN_MAEV      0x06
#define     USBN_MAMSK     0x07
#define     USBN_ALTEV     0x08
#define     USBN_ALTMSK    0x09
#define     USBN_TXEV      0x0A
#define     USBN_TXMSK     0x0B
#define     USBN_RXEV      0x0C
#define     USBN_RXMSK     0x0D
#define     USBN_NAKEV     0x0E
#define     USBN_NAKMSK    0x0F
#define     USBN_FWEV      0x10
#define     USBN_FWMSK     0x11
#define     USBN_FNH       0x12
#define     USBN_FNL       0x13
#define     USBN_DMACNTRL  0x14
#define     USBN_DMAEV     0x15
#define     USBN_DMAMSK    0x16
#define     USBN_MIR       0x17
#define     USBN_DMACNT    0x18
#define     USBN_DMAERR    0x19
//          reserved       0x1A
#define     USBN_WKUP      0x1B
//          reserved       0x1C
//          reserved       0x1D
//          reserved       0x1E
//          reserved       0x1F
#define     USBN_EPC0      0x20
#define     USBN_TXD0      0x21
#define     USBN_TXS0      0x22
#define     USBN_TXC0      0x23
//          reserved       0x24
#define     USBN_RXD0      0x25
#define     USBN_RXS0      0x26
#define     USBN_RXC0      0x27
#define     USBN_EPC1      0x28
#define     USBN_TXD1      0x29
#define     USBN_TXS1      0x2A
#define     USBN_TXC1      0x2B
#define     USBN_EPC2      0x2C
#define     USBN_RXD1      0x2D
#define     USBN_RXS1      0x2E
#define     USBN_RXC1      0x2F
#define     USBN_EPC3      0x30
#define     USBN_TXD2      0x31
#define     USBN_TXS2      0x32
#define     USBN_TXC2      0x33
#define     USBN_EPC4      0x34
#define     USBN_RXD2      0x35
#define     USBN_RXS2      0x36
#define     USBN_RXC2      0x37
#define     USBN_EPC5      0x38
#define     USBN_TXD3      0x39
#define     USBN_TXS3      0x3A
#define     USBN_TXC3      0x3B
#define     USBN_EPC6      0x3C
#define     USBN_RXD3      0x3D
#define     USBN_RXS3      0x3E
#define     USBN_RXC3      0x3F

//max packet size of all endpoints, but not EP0
#define  USB_MAX_PACKET_LENGTH   64

//CCS turns on the following interrupts:
#ifndef USBN_ALTMSK_ENABLES
   #define USBN_ALTMSK_ENABLES   0xC0
#endif
#ifndef USBN_TXMSK_ENABLES
   #define USBN_TXMSK_ENABLES    0xFF  //interrupt on any change on TX interrupt (IN packet or OVR)
#endif
#ifndef USBN_RXMSK_ENABLES
   #define USBN_RXMSK_ENABLES    0xFF  //interrupt on any change on RX interrupt (OUT/SETUP packet or OVR)
#endif
#ifndef USBN_NAKMSK_ENABLES
   #define USBN_NAKMSK_ENABLES   0x10  //interrupt only on NAK on EP0 OUT/SETUP packets.
#endif
#ifndef USBN_FWMSK_ENABLES
   #define USBN_FWMSK_ENABLES    0x00  //dont interrupt at all for frame events
#endif
#ifndef USBN_MAMSK_ENABLES
   #define USBN_MAMSK_ENABLES    0xD6  //interrupt on ALT, NAK, TX and RX events
#endif

int8  usbn_current_alt_mask;  //we will modify what ALT masks depending if we are idle or not

//NFSR STATES
#define     USBN_NFSR_RESET        0
#define     USBN_NFSR_RESUME       1
#define     USBN_NFSR_OPERATIONAL  2
#define     USBN_NFSR_SUSPEND      3

//***************************************************************************************
//* YOU CAN USE THE FOLLOWING 2 CONSTANT ARRAYS TO DEFINE DIFFERENT ENPDOINT MAPPINGS
//*
//* But the way it is setup now is that EPC1 and EPC2 make up USB Endpoint 1,
//* EPC3 and EPC4 make up USB Endpoint 2, and EPC5 and EPC6 make up USB Endpoint 3.
//* EPC0 is always Endpoint 0 (control) and can't be modified.
//* Probably best to leave this way, unless for some reason you need a different
//* endpoint, say you're developing an application that needs to communicate to Endpoint 15.
//****************************************************************************************
//960x has 6 unidrectional endpoint.  this maps bidirectional endpoint[1] to 960x EPC1 and EPC2
//when we see IN for EP1 then we know to look at EPC2 and it's respective FIFO buffer
//directions are respective of HOST
const int8 usb_epc_out_address[USB_MAX_ENDPOINTS]={0,1,2,3};  //epc0=0,epc2=1,epc4=2,epc6=3
const int8 usb_epc_in_address[USB_MAX_ENDPOINTS]={0,1,2,3}; //epc0=0,epc1=1,epc3=2,epc5=3
const int8 epc_to_epadd[]={0,1,1,2,2,3,3};

// *** protoypes of user functions
void usb_init(void);
int8 usbn_get_version(void);

// *** prototypes used by usb.c (token handling)
void usb_stall_ep(int8 endpoint, int1 direction);
void usb_unstall_ep(int8 endpoint, int1 direction);
int1 usb_endpoint_stalled(int8 endpoint, int1 direction);
void usb_set_address(int8 address);
void usb_set_configured(int config);
int8 usb_get_packet(int8 endpoint, int8 * ptr, int8 max);
int1 usb_put_packet(int endpoint, int * ptr, int len, PID_TOGGLE tgl);
#inline
void usb_wrongstate();

// *** prototypes (USB ISR)
void usb_isr(void);
void usb_check_warn(void);
void usb_check_alt(void);
void usb_check_alt_reset(void);
void usb_check_txev(void);
void usb_check_frame(void);
void usb_check_nak(void);
void usb_check_uld(void);
void usb_check_rxev(void);

// *** prototype of USBN960x specific functions used only in this file
void usb_reset(void);
void usb_enable_endpoint(int8 endpoint, int1 direction, int1 iso);
void usb_disable_endpoint(int8 endpoint, int1 direction);
void usbn_write(int8 address, int8 data);
int8 usbn_read(int8 address);
int8 usb_find_epc(int8 endpoint, int1 direction);
void usb_clear_ep0_buffer(void);

//DONT CHANGE THE FOLLOWING 3 CONSTANT ARRAYS
const char USBN_EPCx[]={USBN_EPC0,USBN_EPC1,USBN_EPC2,USBN_EPC3,USBN_EPC4,USBN_EPC5,USBN_EPC6}; //epc_address[x] finds the constant USBN_EPCx
const char USBN_TX_FIFOx[]={USBN_TXD0, USBN_TXD1, 0,         USBN_TXD2, 0,         USBN_TXD3, 0};          //ecp0,ecp1,ecp3,ecp5 have tx_fifos
const char USBN_RX_FIFOx[]={USBN_RXD0, 0,         USBN_RXD1, 0,         USBN_RXD2, 0,         USBN_RXD3};  //ecp0,ecp2,ecp4,ecp6 have rx_fifos


/// BEGIN User Functions

/*******************************************************************************
/* usb_init()
/*
/* Summary: Resets and initalizes USB code and USBN device.  You must call this
/*          first before using code.
/*          If you have debug enabled it will enable TBE interrupts on the PIC.
/*
/*          NOTE: this enables interrupts.
/*
/********************************************************************************/
void usb_init(void) {
   output_high(USBN_CS);
   output_float(USBN_A0);
   output_high(USBN_RD);
   output_high(USBN_WR);
   usbn_bus_float();

   output_high(USBN_RS);
   delay_ms(100);
   output_low(USBN_RS);
   delay_ms(10);
   output_high(USBN_RS);
   delay_ms(10);

   usbn_current_alt_mask=USBN_ALTMSK_ENABLES & 0x7F;  //make sure resume is off

   //turn on masks
   usbn_write(USBN_ALTMSK, usbn_current_alt_mask);
   usbn_write(USBN_TXMSK, USBN_TXMSK_ENABLES);
   usbn_write(USBN_RXMSK, USBN_RXMSK_ENABLES);

   usbn_write(USBN_NAKMSK, USBN_NAKMSK_ENABLES);
   usbn_write(USBN_FWMSK, USBN_FWMSK_ENABLES);
   usbn_write(USBN_MAMSK, USBN_MAMSK_ENABLES);

   usbn_write(USBN_EPC0, 0);
   usbn_write(USBN_FAR, 0x80);

   usbn_write(USBN_WKUP,0x0C);

   usbn_write(0x1F,0x40);             //3.3V regulator workaround

   usb_reset();
   delay_ms(10);

   ext_int_edge(H_TO_L);
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_EXT);

   #IF USB_DO_DEBUG
      enable_interrupts(INT_TBE);
   #ENDIF

   usbn_write(USBN_MCNTRL, 0x04); //vge
   delay_ms(2);
   usbn_write(USBN_MCNTRL, 0xCC); //VGE, NAT, active low
}

/*******************************************************************************
/* usb_put_packet(endpoint,*ptr,len,toggle)
/*
/* Input: endpoint - endpoint to send packet to
/*        ptr - points to data to send
/*        len - amount of data to send
/*        toggle - whether to send data with a DATA0 pid, a DATA1 pid, or toggle from the last DATAx pid.
/*
/* Output: TRUE if data was sent correctly, FALSE if it was not.  It will return FALSE if you gave it
/*         an invalid endpoint, or because TX buffer is still busy sending last packet.  If TX buffer
/*         is still busy sending the last packet then you can keep calling usb_put_packet() until it returns
/*         TRUE.
/*
/* Summary: Sends a packet out the EP to the host.  Notice that there is a difference
/*          between a packet and a message.  If you wanted to send a 512 byte message you
/*          would accomplish this by sending 8 64-byte packets, followed by a 0 length packet.
/*          If the last (or only packet) being sent is less than the max packet size defined
/*          in your descriptor then you do not need to send a 0 length packet to identify an end of message.
/*
/*          usb_puts() (provided in usb.c) will send a multi-packet message correctly.
/*
/********************************************************************************/
int1 usb_put_packet(int endpoint, int * ptr, int len, PID_TOGGLE tgl) {
   int8 epc, address, status, tcount;
   int8 i;

   debug(debug_txb,"\r\nTX %X %X: ",endpoint,len);

   if (endpoint < 16) {
      epc=usb_find_epc(endpoint,1);

      if (epc != 0xFF) {
         address=USBN_TX_FIFOx[epc];  //address = USBN_TXDx
         address+=2; //TXCx
         status=usbn_read(address); //TXCx
         if ((!bit_test(status,1))||(!endpoint)) { //make sure last isnt set


            status |= 0x08;
            usbn_write(address,status); //FLUSH
            i=0;
            do {
               i++;
               status=usbn_read(address);
            } while ((bit_test(status,3))&&(i!=0)); //wait until flush is clear
            address-=2;

            if (i==0) {debug_txb('!');}

            status=usbn_read(address+1); //TXSx
            tcount=status & 0x1F;   //find open space on fifo buffer
            if (tcount==0) {
               debug(debug_txb,"BF ");
               return(0);
            }
            else {
               while (len != 0) {
                  debug(debug_txb,"%X ",*ptr);
                  usbn_write(address, *ptr);
                  ptr++;
                  len--;
               }
               debug_txb(' ');
               if (!endpoint) {  //if endpoint 0 we need to disable rx_en on RXC0
                  status=usbn_read(USBN_RXC0);
                  status &= 0xFE;
                  usbn_write(USBN_RXC0, status);       //disable RC_EN
               }
               status=usbn_read(address+2);  //TXCx

               if (endpoint) {status |= 0x02;}         //set LAST bit
               if (tgl==1) {status ^= 0x04;}   //toggle TOGGLE bit (TOGGLE DATAx)
               else if (tgl==0) {status |= 0x04;}  //set toggle (DATA1)
               else if (tgl==2) {status &= 0xFB;} //clear toggle (DATA0)
               status |= 1;   //set TX_EN
               //debug(debug_txb, "D%U ",(status & 0x04)==0x04);   //show DATA1/DATA0
               debug(debug_txb, "%X ",status);
               usbn_write(address+2,status);
               return(1);
            }
         }
      }
   }
   return(0);
}

int8 currently_opened_epc;
int8 currently_opened_address;

int1 usb_buffer_open(int endpoint, int len) {
   int8 status, tcount;
   int8 i;

   if (endpoint < 16) {
      currently_opened_epc=usb_find_epc(endpoint,1);

      if (currently_opened_epc != 0xFF) {
         currently_opened_address=USBN_TX_FIFOx[currently_opened_epc];  //address = USBN_TXDx
         currently_opened_address+=2; //TXCx
         status=usbn_read(currently_opened_address); //TXCx
         if ((!bit_test(status,1))||(!endpoint)) { //make sure last isnt set


            status |= 0x08;
            usbn_write(currently_opened_address,status); //FLUSH
            i=0;
            do {
               i++;
               status=usbn_read(currently_opened_address);
            } while ((bit_test(status,3))&&(i!=0)); //wait until flush is clear
            currently_opened_address-=2;


            status=usbn_read(currently_opened_address+1); //TXSx
            tcount=status & 0x1F;   //find open space on fifo buffer
            if (tcount!=0) {  //check space in tx buffer
               return(1);  //buffer is full
            }
         }
      }
   }
   return(0);
}


void usb_buffer_write(int8 data) {
                  usbn_write(currently_opened_address, data);
}

void usb_buffer_close(PID_TOGGLE tgl) {
   int8 status;
               if (!currently_opened_epc) {  //if endpoint 0 we need to disable rx_en on RXC0
                  status=usbn_read(USBN_RXC0);
                  status &= 0xFE;
                  usbn_write(USBN_RXC0, status);       //disable RC_EN
               }
               status=usbn_read(currently_opened_address+2);  //TXCx

               if (currently_opened_epc) {status |= 0x02;}         //set LAST bit
               if (tgl==1) {status ^= 0x04;}   //toggle TOGGLE bit (TOGGLE DATAx)
               else if (tgl==0) {status |= 0x04;}  //set toggle (DATA1)
               else if (tgl==2) {status &= 0xFB;} //clear toggle (DATA0)
               status |= 1;   //set TX_EN
               usbn_write(currently_opened_address+2,status);
            }


/*******************************************************************************
/* usbn_get_version()
/*
/* Summary: Stictly for debugging, this function gets the revision number of the
/*          USBN960x you are connected to.  Good to see if you're connection
/*          to the USBN960x is good.
/*
/********************************************************************************/
int8 usbn_get_version(void) {
   return(usbn_read(USBN_RID));
}

/// END User Functions


/// BEGIN Hardware layer functions required by USB.C

/*******************************************************************************
/* usb_get_packet(endpoint, *ptr, max)
/*
/* Input: endpoint - endpoint to get data from
/*        ptr - where to save data to local PIC RAM
/*        max - max amount of data to receive from buffer
/*
/* Output: the amount of data taken from the buffer. If no data is available in the buffer,
/*         or there was an error, will return FALSE.
/*
/* Summary: Gets a packet of data from the USBN buffer and puts into local PIC RAM.
/*          You could poll usb_get_packet() until it doesn't return 0, or you could
/*          poll usb_epX_rx_status.rx (where X is endpoint number) until .rx = 1.
/*          usb_kbhit(endpoint) (located in usb.c) will poll usb_epX_rx_status.rx,
/*          but is easier to read.
/*
/********************************************************************************/
int8 usb_get_packet(int8 endpoint, int8 * ptr, int8 max) {
   int8 epc, address, rx=0, status, len, i;

   if (endpoint < 16) {
      epc=usb_find_epc(endpoint,0);
      if (epc!=0xFF) {
         address=USBN_RX_FIFOx[epc];     //RXDx
         status=usbn_read(address+1);  //RXSx
         if ( ( (!bit_test(status,7)) && (bit_test(status,4) ) ) || (!endpoint) ) { //check make sure RX_ERR==0 and RX_LAST==1, unless endpoint==0
            do {
               status=usbn_read(address+1);  //RXSx
               len=status & 0x0F;
               i=len;
               debug(debug_txb,"\r\nRX %X %X: ", endpoint,len);
               while ( (rx < max) && (i) ) {
                  *ptr=usbn_read(address);
                  debug(debug_txb, "%X ",*ptr);
                  rx++;
                  ptr++;
                  i--;
               }
            } while ( (rx < max) && (len) );
            //debug(debug_txb, " D%U ",(status & 0x20)==0x20);  //display DATA1/DATA0
            debug(debug_txb, " %X ",status);
            if (endpoint) {  //re-enable reception for other endpoints, and flush rx-buffer
               status=usbn_read(address+2);
               status |= 0x09;
               usbn_write(address+2,status);
            }
            return(rx);
         }
      }
   }
   return(0);
}

/*******************************************************************************
/* usb_stall_ep(endpoint,direction)
/*
/* Input: endpoint - endpoint to stall.
/*        direction - direction of endpoint.  0 == OUT or CONTROL, 1 == IN
/*
/* Summary: Stalls specified endpoint.  If endpoint is stalled it will NAK any tokens
/*          destined to that endpoint.
/*
/********************************************************************************/
void usb_stall_ep(int8 endpoint, int1 direction) {
   int8 epc, address, value;
   if (endpoint < 16) {
      epc=usb_find_epc(endpoint,direction);
      if (epc!=0xFF) {
         address=USBN_EPCx[epc];
         value=usbn_read(address);
         value |= 0x80;
         usbn_write(address,value);
         debug(debug_txb, " SE%X",endpoint | (direction << 7));
      }
   }
}

/*******************************************************************************
/* usb_unstall_ep(endpoint, direction)
/*
/* Input: endpoint - endpoint to un-stall.
/*        direction - direction of endpoint.  0 == OUT or CONTROL, 1 == IN
/*
/* Summary: Un-stalls endpoint.
/*
/********************************************************************************/
void usb_unstall_ep(int8 endpoint, int1 direction) {
   int8 epc, address, value;
   if (endpoint < 16) {
      epc=usb_find_epc(endpoint,direction);
      if (epc!=0xFF) {
         address=USBN_EPCx[epc];
         value=usbn_read(address);
         value &= 0x7F;
         usbn_write(address,value);
         debug(debug_txb, " USE%X",endpoint | (direction << 7));
      }
   }
}

/*******************************************************************************
/* usb_endpoint_stalled(endpoint, direction)
/*
/* Input: endpoint - endpoint to check
/*        direction - direction of endpoint.  0 == OUT or CONTROL, 1 == IN
/*
/* Output: returns a TRUE if endpoint is stalled, FALSE if it is not.
/*
/* Summary: Looks to see if an endpoint is stalled, or not.
/*
/********************************************************************************/
int1 usb_endpoint_stalled(int8 endpoint, int1 direction) {
   int8 epc, address, value;

   if (endpoint < 16) {
      epc=usb_find_epc(endpoint,direction);
      if (epc!=0xFF) {
         address=USBN_EPCx[epc];
         value=usbn_read(address);
         return(bit_test(value,7));
      }
   }

   return(1);
}

/*******************************************************************************
/* usb_set_address(address)
/*
/* Input: address - address the host specified that we use
/*
/* Summary: Configures the USBN960x device for the specified device address
/*
/********************************************************************************/
void usb_set_address(int8 address) {
   usbn_write(USBN_EPC0, usbn_read(USBN_EPC0) | 0x40);
   usbn_write(USBN_FAR, address | 0x80);
}


/*******************************************************************************
/* usb_set_configured(config)
/*
/* Input: config - Configuration to use.  0 to uncofigure device.
/*
/* Summary: Configures or unconfigures device.  If configuring device it will
/*          enable all the endpoints.  If un-configuring device it will disable all
/*          endpoints.
/*
/*          NOTE: CCS only provides code to handle 1 configuration.
/*
/********************************************************************************/
void usb_set_configured(int config) {
      USB_Curr_Config=config;

      if (config!=0) { //if config!=0 then enable required endpoints
         //configure IN ep1
         #IF USB_EP1_TX_ENABLE==1
           usb_enable_endpoint(1,1,0);   //endpoint 1, IN
         #ELIF USB_EP1_TX_ENABLE==2
           usb_enable_endpoint(1,1,1);   //endpoint 1, IN,  ISO
         #ENDIF

         //configure OUT ep1
         #IF USB_EP1_RX_ENABLE==1
           usb_enable_endpoint(1,0,0);   //endpoint 1, OUT
         #ELIF USB_EP1_RX_ENABLE==2
           usb_enable_endpoint(1,0,1);   //endpoint 1, OUT, ISO
         #ENDIF

         //configure IN ep2
         #IF USB_EP2_TX_ENABLE==1
           usb_enable_endpoint(2,1,0);   //endpoint 2, IN
         #ELIF USB_EP2_TX_ENABLE==2
           usb_enable_endpoint(2,1,1);   //endpoint 2, IN,  ISO
         #ENDIF

         //configure OUT ep2
         #IF USB_EP2_RX_ENABLE==1
           usb_enable_endpoint(2,0,0);   //endpoint 2, OUT
         #ELIF USB_EP2_RX_ENABLE==2
           usb_enable_endpoint(2,0,1);   //endpoint 2, OUT, ISO
         #ENDIF

         //configure IN ep3
         #IF USB_EP3_TX_ENABLE==1
           usb_enable_endpoint(3,1,0);   //endpoint 3, IN
         #ELIF USB_EP3_TX_ENABLE==2
           usb_enable_endpoint(3,1,1);   //endpoint 3, IN,  ISO
         #ENDIF

         //configure OUT ep3
         #IF USB_EP3_RX_ENABLE==1
           usb_enable_endpoint(3,0,0);   //endpoint 3, OUT
         #ELIF USB_EP3_RX_ENABLE==2
           usb_enable_endpoint(3,0,1);   //endpoint 3, OUT, ISO
         #ENDIF

         //configure IN ep4
         #IF USB_EP4_TX_ENABLE==1
           usb_enable_endpoint(4,1,0);   //endpoint 4, IN
         #ELIF USB_EP4_TX_ENABLE==2
           usb_enable_endpoint(4,1,1);   //endpoint 4, IN,  ISO
         #ENDIF

         //configure OUT ep4
         #IF USB_EP4_RX_ENABLE==1
           usb_enable_endpoint(4,0,0);   //endpoint 4, OUT
         #ELIF USB_EP4_RX_ENABLE==2
           usb_enable_endpoint(4,0,1);   //endpoint 4, OUT, ISO
         #ENDIF

         //configure IN ep5
         #IF USB_EP5_TX_ENABLE==1
           usb_enable_endpoint(5,1,0);   //endpoint 5, IN
         #ELIF USB_EP5_TX_ENABLE==2
           usb_enable_endpoint(5,1,1);   //endpoint 5, IN,  ISO
         #ENDIF

         //configure OUT ep5
         #IF USB_EP5_RX_ENABLE==1
           usb_enable_endpoint(5,0,0);   //endpoint 5, OUT
         #ELIF USB_EP5_RX_ENABLE==2
           usb_enable_endpoint(5,0,1);   //endpoint 5, OUT, ISO
         #ENDIF

         //configure IN ep6
         #IF USB_EP6_TX_ENABLE==1
           usb_enable_endpoint(6,1,0);   //endpoint 6, IN
         #ELIF USB_EP6_TX_ENABLE==2
           usb_enable_endpoint(6,1,1);   //endpoint 6, IN,  ISO
         #ENDIF

         //configure OUT ep6
         #IF USB_EP6_RX_ENABLE==1
           usb_enable_endpoint(6,0,0);   //endpoint 6, OUT
         #ELIF USB_EP6_RX_ENABLE==2
           usb_enable_endpoint(6,0,1);   //endpoint 6, OUT, ISO
         #ENDIF

         //configure IN ep7
         #IF USB_EP7_TX_ENABLE==1
           usb_enable_endpoint(7,1,0);   //endpoint 7, IN
         #ELIF USB_EP7_TX_ENABLE==2
           usb_enable_endpoint(7,1,1);   //endpoint 7, IN,  ISO
         #ENDIF

         //configure OUT ep7
         #IF USB_EP7_RX_ENABLE==1
           usb_enable_endpoint(7,0,0);   //endpoint 7, OUT
         #ELIF USB_EP7_RX_ENABLE==2
           usb_enable_endpoint(7,0,1);   //endpoint 7, OUT, ISO
         #ENDIF

         //configure IN ep8
         #IF USB_EP8_TX_ENABLE==1
           usb_enable_endpoint(8,1,0);   //endpoint 8, IN
         #ELIF USB_EP8_TX_ENABLE==2
           usb_enable_endpoint(8,1,1);   //endpoint 8, IN,  ISO
         #ENDIF

         //configure OUT ep8
         #IF USB_EP8_RX_ENABLE==1
           usb_enable_endpoint(8,0,0);   //endpoint 8, OUT
         #ELIF USB_EP8_RX_ENABLE==2
           usb_enable_endpoint(8,0,1);   //endpoint 8, OUT, ISO
         #ENDIF

         //configure IN ep9
         #IF USB_EP9_TX_ENABLE==1
           usb_enable_endpoint(9,1,0);   //endpoint 9, IN
         #ELIF USB_EP9_TX_ENABLE==2
           usb_enable_endpoint(9,1,1);   //endpoint 9, IN,  ISO
         #ENDIF

         //configure OUT ep9
         #IF USB_EP9_RX_ENABLE==1
           usb_enable_endpoint(9,0,0);   //endpoint 9, OUT
         #ELIF USB_EP9_RX_ENABLE==2
           usb_enable_endpoint(9,0,1);   //endpoint 9, OUT, ISO
         #ENDIF

         //configure IN ep10
         #IF USB_EP10_TX_ENABLE==1
           usb_enable_endpoint(10,1,0);   //endpoint 10, IN
         #ELIF USB_EP10_TX_ENABLE==2
           usb_enable_endpoint(10,1,1);   //endpoint 10, IN,  ISO
         #ENDIF

         //configure OUT ep10
         #IF USB_EP10_RX_ENABLE==1
           usb_enable_endpoint(10,0,0);   //endpoint 10, OUT
         #ELIF USB_EP10_RX_ENABLE==2
           usb_enable_endpoint(10,0,1);   //endpoint 10, OUT, ISO
         #ENDIF


         //configure IN ep11
         #IF USB_EP11_TX_ENABLE==1
           usb_enable_endpoint(11,1,0);   //endpoint 11, IN
         #ELIF USB_EP11_TX_ENABLE==2
           usb_enable_endpoint(11,1,1);   //endpoint 11, IN,  ISO
         #ENDIF

         //configure OUT ep11
         #IF USB_EP11_RX_ENABLE==1
           usb_enable_endpoint(11,0,0);   //endpoint 11, OUT
         #ELIF USB_EP11_RX_ENABLE==2
           usb_enable_endpoint(11,0,1);   //endpoint 11, OUT, ISO
         #ENDIF

         //configure IN ep12
         #IF USB_EP12_TX_ENABLE==1
           usb_enable_endpoint(12,1,0);   //endpoint 12, IN
         #ELIF USB_EP12_TX_ENABLE==2
           usb_enable_endpoint(12,1,1);   //endpoint 12, IN,  ISO
         #ENDIF

         //configure OUT ep12
         #IF USB_EP12_RX_ENABLE==1
           usb_enable_endpoint(12,0,0);   //endpoint 12, OUT
         #ELIF USB_EP12_RX_ENABLE==2
           usb_enable_endpoint(12,0,1);   //endpoint 12, OUT, ISO
         #ENDIF

         //configure IN ep13
         #IF USB_EP13_TX_ENABLE==1
           usb_enable_endpoint(13,1,0);   //endpoint 13, IN
         #ELIF USB_EP13_TX_ENABLE==2
           usb_enable_endpoint(13,1,1);   //endpoint 13, IN,  ISO
         #ENDIF

         //configure OUT ep13
         #IF USB_EP13_RX_ENABLE==1
           usb_enable_endpoint(13,0,0);   //endpoint 13, OUT
         #ELIF USB_EP13_RX_ENABLE==2
           usb_enable_endpoint(13,0,1);   //endpoint 13, OUT, ISO
         #ENDIF

         //configure IN ep14
         #IF USB_EP14_TX_ENABLE==1
           usb_enable_endpoint(14,1,0);   //endpoint 14, IN
         #ELIF USB_EP14_TX_ENABLE==2
           usb_enable_endpoint(14,1,1);   //endpoint 14, IN,  ISO
         #ENDIF

         //configure OUT ep14
         #IF USB_EP14_RX_ENABLE==1
           usb_enable_endpoint(14,0,0);   //endpoint 14, OUT
         #ELIF USB_EP14_RX_ENABLE==2
           usb_enable_endpoint(14,0,1);   //endpoint 14, OUT, ISO
         #ENDIF

         //configure IN ep15
         #IF USB_EP15_TX_ENABLE==1
           usb_enable_endpoint(15,1,0);   //endpoint 15, IN
         #ELIF USB_EP15_TX_ENABLE==2
           usb_enable_endpoint(15,1,1);   //endpoint 15, IN,  ISO
         #ENDIF

         //configure OUT ep15
         #IF USB_EP15_RX_ENABLE==1
           usb_enable_endpoint(15,0,0);   //endpoint 15, OUT
         #ELIF USB_EP15_RX_ENABLE==2
           usb_enable_endpoint(15,0,1);   //endpoint 15, OUT, ISO
         #ENDIF

     }
     //TODO: handle other configurations. Not provided by CCS.
     else {
         //un-configure OUT ep1
         #IF USB_EP1_RX_ENABLE
           usb_disable_endpoint(1, 0);   //endpoint 1, OUT
         #ENDIF
         //un-configure IN ep1
         #IF USB_EP1_TX_ENABLE
           usb_disable_endpoint(1, 1);   //endpoint 1, IN
         #ENDIF

         //un-configure OUT ep2
         #IF USB_EP2_RX_ENABLE
           usb_disable_endpoint(2, 0);   //endpoint 2, OUT
         #ENDIF
         //un-configure IN ep2
         #IF USB_EP2_TX_ENABLE
           usb_disable_endpoint(2, 1);   //endpoint 2, IN
         #ENDIF

         //un-configure OUT ep3
         #IF USB_EP3_RX_ENABLE
           usb_disable_endpoint(3, 0);   //endpoint 3, OUT
         #ENDIF
         //un-configure IN ep3
         #IF USB_EP3_TX_ENABLE
           usb_disable_endpoint(3, 1);   //endpoint 3, IN
         #ENDIF

         //un-configure OUT ep4
         #IF USB_EP4_RX_ENABLE
           usb_disable_endpoint(4, 0);   //endpoint 4, OUT
         #ENDIF
         //un-configure IN ep4
         #IF USB_EP4_TX_ENABLE
           usb_disable_endpoint(4, 1);   //endpoint 4, IN
         #ENDIF

         //un-configure OUT ep5
         #IF USB_EP5_RX_ENABLE
           usb_disable_endpoint(5, 0);   //endpoint 5, OUT
         #ENDIF
         //un-configure IN ep5
         #IF USB_EP5_TX_ENABLE
           usb_disable_endpoint(5, 1);   //endpoint 5, IN
         #ENDIF

         //un-configure OUT ep6
         #IF USB_EP6_RX_ENABLE
           usb_disable_endpoint(6, 0);   //endpoint 6, OUT
         #ENDIF
         //un-configure IN ep6
         #IF USB_EP6_TX_ENABLE
           usb_disable_endpoint(6, 1);   //endpoint 6, IN
         #ENDIF

         //un-configure OUT ep7
         #IF USB_EP7_RX_ENABLE
           usb_disable_endpoint(7, 0);   //endpoint 7, OUT
         #ENDIF
         //un-configure IN ep7
         #IF USB_EP7_TX_ENABLE
           usb_disable_endpoint(7, 1);   //endpoint 7, IN
         #ENDIF

         //un-configure OUT ep8
         #IF USB_EP8_RX_ENABLE
           usb_disable_endpoint(8, 0);   //endpoint 8, OUT
         #ENDIF
         //un-configure IN ep8
         #IF USB_EP8_TX_ENABLE
           usb_disable_endpoint(8, 1);   //endpoint 8, IN
         #ENDIF

         //un-configure OUT ep9
         #IF USB_EP9_RX_ENABLE
           usb_disable_endpoint(9, 0);   //endpoint 9, OUT
         #ENDIF
         //un-configure IN ep9
         #IF USB_EP9_TX_ENABLE
           usb_disable_endpoint(9, 1);   //endpoint 9, IN
         #ENDIF

         //un-configure OUT ep10
         #IF USB_EP10_RX_ENABLE
           usb_disable_endpoint(10, 0);   //endpoint 10, OUT
         #ENDIF
         //un-configure IN ep10
         #IF USB_EP10_TX_ENABLE
           usb_disable_endpoint(10, 1);   //endpoint 10, IN
         #ENDIF

         //un-configure OUT ep11
         #IF USB_EP11_RX_ENABLE
           usb_disable_endpoint(11, 0);   //endpoint 11, OUT
         #ENDIF
         //un-configure IN ep11
         #IF USB_EP11_TX_ENABLE
           usb_disable_endpoint(11, 1);   //endpoint 11, IN
         #ENDIF

         //un-configure OUT ep12
         #IF USB_EP12_RX_ENABLE
           usb_disable_endpoint(12, 0);   //endpoint 12, OUT
         #ENDIF
         //un-configure IN ep2
         #IF USB_EP12_TX_ENABLE
           usb_disable_endpoint(12, 1);   //endpoint 12, IN
         #ENDIF

         //un-configure OUT ep13
         #IF USB_EP13_RX_ENABLE
           usb_disable_endpoint(13, 0);   //endpoint 13, OUT
         #ENDIF
         //un-configure IN ep13
         #IF USB_EP13_TX_ENABLE
           usb_disable_endpoint(13, 1);   //endpoint 13, IN
         #ENDIF

         //un-configure OUT ep14
         #IF USB_EP14_RX_ENABLE
           usb_disable_endpoint(14, 0);   //endpoint 14, OUT
         #ENDIF
         //un-configure IN ep14
         #IF USB_EP14_TX_ENABLE
           usb_disable_endpoint(14, 1);   //endpoint 14, IN
         #ENDIF

         //un-configure OUT ep15
         #IF USB_EP15_RX_ENABLE
           usb_disable_endpoint(15, 0);   //endpoint 15, OUT
         #ENDIF
         //un-configure IN ep15
         #IF USB_EP15_TX_ENABLE
           usb_disable_endpoint(15, 1);   //endpoint 15, IN
         #ENDIF
     }
}

/*******************************************************************************
/* usb_wrongstate()
/*
/* Summary: Tells the host that they asked us to do something (either illegal or something
/*          we don't support) by stalling EP0.  When the host gets the idea it will
/*          either reset the device or un-stall EP0.
/*
/********************************************************************************/
#inline
void usb_wrongstate() {
   usb_stall_ep(0,0);
}

/// END Hardware layer functions required by USB.C


/// BEGIN USB Interrupt Service Routine

/*******************************************************************************
/* usb_isr()
/*
/* Summary: Checks the Main Event interrupt register, and acts upon USB interrupts.
/*          ALT interrupt happens for the misc interrupts (reset, idle, wakeup, etc.)
/*          NAK interrupt happens when the USBN device generated a NAK to an IN or OUT/SETUP token
/*          TX interrupt happens when an IN token has happend (PIC -> HOST)
/*          RX interrupt happens when an OUT/SETUP token has happened. (HOST -> PIC).
/*          Majority of USB token handling is done through the RX event.
/*
/*          If you choose to use a polling method instead of by interrupt then call
/*          this function on any free time.  Use great care if you choose to poll:
/*          some USB requests require a response within a few milli-seconds.
/*
/********************************************************************************/
#int_ext NOCLEAR
void usb_isr(void) {
   int8 maev;
   int_ext_flag=0;   //clear interrupt flag

   do {
      maev=usbn_read(USBN_MAEV) & usbn_read(USBN_MAMSK);
      if (maev) {
         debug(debug_txb,"\r\nI %X: ",maev);
         if (bit_test(maev,6)) {usb_check_rxev();}    //OUT/SETUP tokens to us                        //0x40
         if (bit_test(maev,2)) {usb_check_txev();}    //IN token was processed (set up new data?)     //0x04
         if (bit_test(maev,1)) {usb_check_alt();}     //Resets, Idles, Suspends, etc.                 //0x02
         if (bit_test(maev,4)) {usb_check_nak();}     //PC tried to send us data, but 960x nak'd it   //0x10
         if (bit_test(maev,0)) {usb_check_warn();}    //TODO: CCS does not provide this function      //0x01
         if (bit_test(maev,3)) {usb_check_frame();}   //TODO: CCS does not provide this function      //0x08
         if (bit_test(maev,5)) {usb_check_uld();}     //TODO: CCS does not provide this function      //0x20
      }
   } while (maev != 0);
}

/*******************************************************************************
/* usb_check_warn()
/*
/* Summary: The WARN event happens when a USBN buffer reaches a desired state.
/*          This requires the user to setup the WARN setting for each buffer.
/*          This capability not provided by CCS.
/*
/********************************************************************************/
void usb_check_warn(void) {

   debug_txb('W');

   //TODO: add code to handle when FIFOs get near full / empty
   //This interrupt not used by CCS
}

/*******************************************************************************
/* usb_check_frame()
/*
/* Summary: Handles a FRAME event interrupt.  CCS does not handle this interrupt.
/*          Only relevant to users using isochronous.
/*
/********************************************************************************/
void usb_check_frame(void) {
   debug_txb('F');
   //TODO: not used by CCS
}

/*******************************************************************************
/* usb_check_uld()
/*
/* Summary:  Handles ULD interrupt.  Not provided by CCS.
/*
/********************************************************************************/
void usb_check_uld(void) {
   debug_txb('U');
   //TODO: not used by CCS
}

/*******************************************************************************
/* usb_check_alt()
/*
/* Summary:  This interrupt happens when an RESET, SUSPEND or RESUME event has happened.
/*            RESET - USBN device is reset, USB token handling code is put into initial state
/*            SUSPEND - Resume interrupt is enabled, USBN device put into suspended mode
/*            RESUME - Suspend interrupt is enabled, USBN device put into operational state, EP0 reception enabled
/*
/********************************************************************************/
void usb_check_alt(void) {
   int8 status;
   status=usbn_read(USBN_ALTEV) & usbn_current_alt_mask;

   debug(debug_txb,"A %X",status);
   if (bit_test(status,2)) {} // DMA.  //Not used by CCS
   else if (bit_test(status,6)) {usb_check_alt_reset();} // RESET
   else if ((bit_test(status,5)) || (bit_test(status,4))) { // SD5 or SD3
      usbn_current_alt_mask=(usbn_current_alt_mask & 0xCF) | 0x80; //make sure resume interrupt is on, sd3/sd5 is off
      usbn_write(USBN_ALTMSK, usbn_current_alt_mask);
      usbn_write(USBN_NFSR, USBN_NFSR_SUSPEND);
   }
   else if (bit_test(status,7)) { // RESUME
      usbn_current_alt_mask=usbn_current_alt_mask & 0x7F; //TURN ON ALL EXCEPT RESUME
      usbn_write(USBN_ALTMSK, usbn_current_alt_mask);
      usbn_write(USBN_NFSR, USBN_NFSR_OPERATIONAL);
      usbn_write(USBN_RXC0, usbn_read(USBN_RXC0) | 0x01);  //enable reception
   }
   else if (bit_test(status,3)) {} // EOP   //resume sequence has been acknowledged and completed by host
   else if (bit_test(status,1)) {} // WKUP
}

/*******************************************************************************
/* usb_check_alt_reset()
/*
/* Summary: This Alt event happens when the Host sends a reset signal.  We reset the USBN and
/*          put the USB token handling code back into initial state.
/*
/********************************************************************************/
void usb_check_alt_reset(void) {
   		   usbn_write(USBN_EPC0, usbn_read(USBN_EPC0) & ~0x80); //CLEAR_STALL_EP0
				usbn_write(USBN_NFSR, USBN_NFSR_RESET);

				usbn_write(USBN_FAR, 0x80);   //set default address

				usbn_write(USBN_EPC0, 0x00);         //enable ep0 only

				usb_reset();

				usbn_write(USBN_NFSR, USBN_NFSR_OPERATIONAL);
            usbn_write(USBN_FAR, 0x80);   //default un-addressed address
            usbn_write(USBN_EPC0, 0x40);     //default address
            usbn_write(USBN_RXC0, 0x01);  //enable reception
}

/*******************************************************************************
/* usb_check_nak()
/*
/* Summary: Handles a NAK event.  This interrupt happens when the USBN960x device
/*          generates a NAK.  We only handle the RX NAK, which happens when the Host
/*          tried sending us an OUT/SETUP (host -> pic) packet, but either the buffer
/*          is full or EP0 reception is disabled or EP0 is transmitting.  In all cases
/*          we flush and reset the EP0 buffer.
/*
/*          CCS does not provide code to handle NAKs on the other endpoints.
/*
/********************************************************************************/
void usb_check_nak(void) {
   int8 status;
   status=usbn_read(USBN_NAKEV) & USBN_NAKMSK_ENABLES;
   debug(debug_txb,"N %X ",status);

   if (bit_test(status,0)) {} //TODO: not handled by CCS
   if (bit_test(status,1)) {} //TODO: not handled by CCS
   if (bit_test(status,2)) {} //TODO: not handled by CCS
   if (bit_test(status,3)) {} //TODO: not handled by CCS
   if (bit_test(status,4)) {
         usbn_write(USBN_TXC0, 0x08);  //flush TX FIFO, disable TX
         usbn_write(USBN_RXC0, 0x08);  //flush RX FIFO, disable TX
         usbn_write(USBN_RXC0, 0x01);  //flush RX FIFO, re-enable reception
         USB_dev_req=NONE;       //previous token request state.  init at none
   }
   if (bit_test(status,5)) {} //TODO: not handled by CCS
   if (bit_test(status,6)) {} //TODO: not handled by CCS
   if (bit_test(status,7)) {} //TODO: not handled by CCS
}

/*******************************************************************************
/* usb_check_txev()
/*
/* Summary: The TX interrupt happens whenever an IN token has been processed.  (PIC -> HOST).
/*          Since the IN token has been processed that means the TX buffer is empty.
/*          CCS only handles TX event when EP0 is empty, meaning it's time to send the next packet
/*          of the message if there is one.
/*
/*          CCS does not handle an interrupt on EPC1-EPC3 because we suggest polling, using
/*          repetitive usb_put_packet() calls.  usb_put_packet() will return true if data was put into buffer,
/*          false if the buffer was not ready for data (because it was still putting out the last packet).
/*
/********************************************************************************/
void usb_check_txev(void) {
   int8 status, txs;

   status=usbn_read(USBN_TXEV) & USBN_TXMSK_ENABLES;

   debug(debug_txb,"T %X ",status);

   if (status & 0xF0) {
      //TODO: not used by CCS
   }
   if (bit_test(status,0)) {  //sent data out of EP0
      txs=usbn_read(USBN_TXS0);
      debug(debug_txb,"%X ",txs);
      if (bit_test(txs,6)) {
         if (USB_dev_req != NONE) { //if we are processing a report then send the next part
            usb_isr_tok_in_dne(0);
         }
         else {
            usbn_write(USBN_TXC0, 0x08);  //flush TX FIFO, disable TX_EN
            usbn_write(USBN_RXC0, 0x08);  //flush RX FIFO
            usbn_write(USBN_RXC0, 0x01);  //enable reception
         }
      }
      else {
         usbn_write(USBN_RXC0, 0x01);
      }
      usb_clear_ep0_buffer();
   }
   if (bit_test(status,1)) {
      txs=usbn_read(USBN_TXS1);
      usb_isr_tok_in_dne(usb_epc_in_address[1]);
   }
   if (bit_test(status,2)) {
      txs=usbn_read(USBN_TXS2);
      usb_isr_tok_in_dne(usb_epc_in_address[2]);
   }
   if (bit_test(status,3)) {
      txs=usbn_read(USBN_TXS3);
      usb_isr_tok_in_dne(usb_epc_in_address[3]);
   }
}

/*******************************************************************************
/* usb_check_rxev()
/*
/* Summary: Handles the RX interrupt.  This happens when an OUT/SETUP token has happened (HOST -> PIC)
/*          and there is data in the RX buffers.
/*
/*          If there is data in the EP0 buffer and it is a SETUP token, it is processed by the
/*          SETUP token handler.  The SETUP token handler is the bulk of this code.
/*
/*          If there is data in the EP1-3 buffers then data is placed into RAM on the PIC.
/*          (See usb_isr_tok_out_dne(endpoint)).
/*
/*          TODO: Not provided by CCS - handle non SETUP data on EP0.
/*
/********************************************************************************/
void usb_check_rxev(void) {
   int8 status, rxstatus, len;

   status=usbn_read(USBN_RXEV) & USBN_RXMSK_ENABLES;

   debug(debug_txb,"R %X ",status);

   if (status & 0xF0) {
      debug(debug_txb,"O ");      //TODO: not used by CCS
   }

   if (bit_test(status,0)) {
      rxstatus=usbn_read(USBN_RXS0);
      debug(debug_txb,"%X ",rxstatus);
      len=rxstatus & 0x0F;

      if ( (len==0) && (bit_test(rxstatus,4)) ) {  //skip the zero len packet, get the next one
      //I have noticed that if you wait to long to get a packet from the buffer that the USBN960x device
      //fails and kills the buffer.  The USBN960x crashes the buffer until a reset, and the
      //USBN960x then thinks there is a 0 length packet.  Sometimes
      //it may even think it has a 0 length setup packet (which is not possible).  When this starts
      //happening the only way I found around the problem was to turn off debugging.  Although the
      //debugging was coded in a way that it should not interfere with the USB interrupt, sometimes it still
      //would.  I did not have such a problem with debugging output on the PIC16C7x5 USB part.
      }

      else if (bit_test(rxstatus,6)) {   //setup
         usbn_write(USBN_EPC0, usbn_read(USBN_EPC0) & 0x7F); //clear STALL

         if (len==USB_MAX_EP0_PACKET_LENGTH) {
            usb_get_packet(0, usb_ep0_rx_buffer, len);   //should always be a length of 8, but just in case
            usbn_write(USBN_TXC0, 0x08);  //flush TX FIFO
            usb_isr_tok_setup_dne();
         }
         else {
            usbn_write(USBN_RXC0, 0x01);  //flush RX FIFO
         }
         usb_clear_ep0_buffer();
      }
      else {
            //TODO: handle OUT tokens to EP0.  CCS only handles SETUP tokens.
            usbn_write(USBN_RXC0, 0x08);  //flush RX buffer.
            usbn_write(USBN_RXC0, 0x01);  //re-enable reception
      }
   }
   if (bit_test(status,1)) {usb_isr_tok_out_dne(usb_epc_out_address[1]);}
   if (bit_test(status,2)) {usb_isr_tok_out_dne(usb_epc_out_address[2]);}
   if (bit_test(status,3)) {usb_isr_tok_out_dne(usb_epc_out_address[3]);}
}

/// END USB Interrupt Service Routine


/// BEGIN USBN960x Functions only used in this File

/*******************************************************************************
/* usbn_write(address, data)
/*
/* Input: address - address of USBN960x register to write to
/*        data - data to write to USBN960x
/*
/* Summary: Writes a byte of memory to specified address on USBN960x
/*
/********************************************************************************/
void usbn_write(int8 address, int8 data) {
   output_high(USBN_RD);
   output_high(USBN_WR);

   usbn_bus_out(address);
   output_high(USBN_A0);
   output_low(USBN_CS);
   output_low(USBN_WR);
   output_high(USBN_WR);
   output_high(USBN_CS);
   output_float(USBN_A0);

   usbn_bus_out(data);
   output_low(USBN_A0);
   output_low(USBN_CS);
   output_low(USBN_WR);
   output_high(USBN_WR);
   output_high(USBN_CS);
   output_float(USBN_A0);
   usbn_bus_float();
}

/*******************************************************************************
/* usbn_read(address)
/*
/* Input: address - address of USBN960x register to read
/* Output: data (byte) in specified address.
/*
/* Summary: Reads a byte of memory from specified address on USBN960x
/*
/********************************************************************************/
int8 usbn_read(int8 address) {
   int8 data;

   output_high(USBN_RD);
   output_high(USBN_WR);

   usbn_bus_out(address);
   output_high(USBN_A0);
   output_low(USBN_CS);
   output_low(USBN_WR);
   output_high(USBN_WR);
   output_high(USBN_CS);
   output_float(USBN_A0);
   usbn_bus_float();

   output_low(USBN_A0);
   output_low(USBN_CS);
   output_low(USBN_RD);
   data=usbn_bus_in();
   output_high(USBN_RD);
   output_high(USBN_CS);
   output_float(USBN_A0);
   usbn_bus_float();

   return(data);
}

/*******************************************************************************
/* usb_find_epc()
/*
/* Input: endpoint - endpoint to find USBN channel number
/*        direction - direction of endpoint.  0 == OUT or CONTROL, 1 == IN
/* Output: returns the USBN960x channel number
/*
/* Summary: The USBN960x has 7 channels (EP0, EP1-3 TX, and EP1-3 RX), but
/*          EP1-3 TX/RX can be mapped to any endpoint from 1-15.  This function
/*          looks up what endpoint is mapped to what channel.  See usb_epc_out_address[]
/*          and usb_epc_in_address[] on how to map channels to endpoint.  CCS defaults to
/*          channel 1 <-> endpoint 1, channel 2 <-> endpoint 2, channel 3 <-> endpoint 3.
/*
/********************************************************************************/
int8 usb_find_epc(int8 endpoint, int1 direction) {
      int8 epc;
      int8 found=0xFF;
      for (epc=0;epc<4;epc++) {
         if (direction) {
            if (usb_epc_in_address[epc]==endpoint) {found=(epc*2); if (epc) {found--;} return(found);}   //0,1,3,5
         }
         else {
            if (usb_epc_out_address[epc]==endpoint) {found=epc*2; return(found);}   //0,2,4,6
         }
      }
      return(found);
}

/*******************************************************************************
/* usb_reset()
/*
/* Summary: Resetsthe USB token handler code, resets the USBN device (software reset, not hardware),
/*          clears all the USBN buffers, clears the local EP0 buffer, enables reception
/*          on EP0.
/*
/********************************************************************************/
void usb_reset(void) {
   usb_token_reset();

   usbn_write(USBN_FAR, 0);
   usbn_write(USBN_EPC0,0);
   usbn_write(USBN_NFSR, USBN_NFSR_RESET);

   usbn_write(USBN_TXC0, 0x08);   //flush fifos
   usbn_write(USBN_TXC1, 0x08);
   usbn_write(USBN_TXC2, 0x08);
   usbn_write(USBN_TXC3, 0x08);

   usbn_write(USBN_RXC0, 0x08);
   usbn_write(USBN_RXC1, 0x08);
   usbn_write(USBN_RXC2, 0x08);
   usbn_write(USBN_RXC3, 0x08);

   usb_clear_ep0_buffer();

   usbn_write(USBN_RXC0, 0x01);    //enable rx0
}

/*******************************************************************************
/* usb_enable_endpoint()
/*
/* Input: endpoint - endpoint to enable
/*        direction - direction of endpoint.  0 == OUT or CONTROL, 1 == IN
/*        iso - TRUE if the endpoint is isochronous, FALSE if it is a bulk/interrupt endpoint.
/*
/* Summary: Enables endpoint for receiving / transmitting.
/*
/********************************************************************************/
void usb_enable_endpoint(int8 endpoint, int1 direction, int1 iso) {
   int8 epc, address, value;
      if ((endpoint < 16)&&(endpoint)) {
      epc=usb_find_epc(endpoint,direction);
      if (epc!=0xFF) {
         address=USBN_EPCx[epc];
         value= epc_to_epadd[epc] | 0x10;
         if (iso) {value |= 0x20;}
         usbn_write(address,value);
         if (!direction) {
            address=USBN_RX_FIFOx[epc] + 2;
            value=usbn_read(address);
            value |= 1; //enable reception
            usbn_write(address, value);
           #if USB_EP1_RX_SIZE
            if (endpoint == 1) {
               usb_ep1_rx_status.rx = 0;
               usb_ep1_rx_status.ov = 0;
            }
           #endif
           #if USB_EP2_RX_SIZE
            if (endpoint == 2) {
               usb_ep2_rx_status.rx = 0;
               usb_ep2_rx_status.ov = 0;
            }
           #endif
           #if USB_EP3_RX_SIZE
            if (endpoint == 3) {
               usb_ep3_rx_status.rx = 0;
               usb_ep3_rx_status.ov = 0;
            }
           #endif
           #if USB_EP4_RX_SIZE
            if (endpoint == 4) {
               usb_ep4_rx_status.rx = 0;
               usb_ep4_rx_status.ov = 0;
            }
           #endif
           #if USB_EP5_RX_SIZE
            if (endpoint == 5) {
               usb_ep5_rx_status.rx = 0;
               usb_ep5_rx_status.ov = 0;
            }
           #endif
           #if USB_EP6_RX_SIZE
            if (endpoint == 6) {
               usb_ep6_rx_status.rx = 0;
               usb_ep6_rx_status.ov = 0;
            }
           #endif
           #if USB_EP7_RX_SIZE
            if (endpoint == 7) {
               usb_ep7_rx_status.rx = 0;
               usb_ep7_rx_status.ov = 0;
            }
           #endif
           #if USB_EP8_RX_SIZE
            if (endpoint == 8) {
               usb_ep8_rx_status.rx = 0;
               usb_ep8_rx_status.ov = 0;
            }
           #endif
           #if USB_EP9_RX_SIZE
            if (endpoint == 9) {
               usb_ep9_rx_status.rx = 0;
               usb_ep9_rx_status.ov = 0;
            }
           #endif
           #if USB_EP10_RX_SIZE
            if (endpoint == 10) {
               usb_ep10_rx_status.rx = 0;
               usb_ep10_rx_status.ov = 0;
            }
           #endif
           #if USB_EP11_RX_SIZE
            if (endpoint == 11) {
               usb_ep11_rx_status.rx = 0;
               usb_ep11_rx_status.ov = 0;
            }
           #endif
           #if USB_EP12_RX_SIZE
            if (endpoint == 12) {
               usb_ep12_rx_status.rx = 0;
               usb_ep12_rx_status.ov = 0;
            }
           #endif
           #if USB_EP13_RX_SIZE
            if (endpoint == 13) {
               usb_ep13_rx_status.rx = 0;
               usb_ep13_rx_status.ov = 0;
            }
           #endif
           #if USB_EP14_RX_SIZE
            if (endpoint == 14) {
               usb_ep14_rx_status.rx = 0;
               usb_ep14_rx_status.ov = 0;
            }
           #endif
           #if USB_EP15_RX_SIZE
            if (endpoint == 15) {
               usb_ep15_rx_status.rx = 0;
               usb_ep15_rx_status.ov = 0;
            }
           #endif
         }
      }
   }
}

/*******************************************************************************
/* usb_disable_endpoint()
/*
/* Input: endpoint - endpoint to enable
/*        direction - direction of endpoint.  0 == OUT or CONTROL, 1 == IN
/*
/* Summary: Disables endpoint.  A disabled interrupt does not respond to tokens.
/*
/********************************************************************************/
void usb_disable_endpoint(int8 endpoint, int1 direction) {
   int8 epc, address, value;
   if ((endpoint != 0)&&(endpoint < 16)) {
      epc=usb_find_epc(endpoint,direction);
      if (epc!=0xFF) {
         address=USBN_EPCx[epc];
         value=usbn_read(address);
         value &= 0xEF;
         usbn_write(address,value);

         if (direction) {address=USBN_TX_FIFOx[epc];} else {address=USBN_RX_FIFOx[epc];}
         address+=2;
         value=usbn_read(address);
         value &= 0xFE; //disable TX_EN or RX_EN
         usbn_write(address,value);
      }
   }
}

void usb_clear_ep0_buffer(void) {
   int8 i;

   for (i=0;i<USB_MAX_EP0_PACKET_LENGTH;i++) {
      usb_ep0_rx_buffer[i]=0;
   }
}

/// END USBN960x Functions only used in this File

#ENDIF
