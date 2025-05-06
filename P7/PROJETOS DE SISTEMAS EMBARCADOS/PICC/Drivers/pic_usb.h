/////////////////////////////////////////////////////////////////////////
////                           pic_usb.c                             ////
////                                                                 ////
////  Microchip PIC16C765 Hardware layer for CCS's PIC USB driver.   ////
////                                                                 ////
//// This file is part of CCS's PIC USB driver code, which includes: ////
////   usb_desc.h - an example set of config and device descriptors  ////
////   usb.c - USB token and request handler code                    ////
////   usb.h - definitions, prototypes and global variables          ////
////                                                                 ////
//// ex_usb_hid.c is an example that uses pic_usb.c to act as a USB  ////
//// HID device.  ex_usb_hid.c shows how to use either the pic_usb.c ////
//// hardware layer or usbn960x.c (National USBN960x hardware        ////
//// driver) hardware layer.                                         ////
////                                                                 ////
//// Another example is provided: ex_usb_scope.c, an example written ////
//// specifically for the USBN960x, shows how to use the CCS PIC USB ////
//// driver with the USBN960x.  CCS provides a demo board with the   ////
//// National USBN960x that is compatable with this code.            ////
//// Natoinal's USBN960x is a full speed device that supports bulk   ////
//// and isochronous mode transfers.  The PIC16C765 is a slow speed  ////
//// device that only supports control and interrupt transfers.      ////
////                                                                 ////
////   *************************  NOTE  **************************   ////
////  Need a pullup resistor (1.5K) connecting Vusb (C3) to D- (C4). ////
////  PIC16C765 will not work as a USB device without this pull-up   ////
////  resistor.                                                      ////
////                                                                 ////
////  PIC16C765 must run at 24Mhz.  You can use a 24Mhz              ////
////  crystal / osc, or a 6Mhz crstal / osc if you use a 4x fuse     ////
////  setting.  Either way this PIC has a fixed operation speed of   ////
////  24 MHZ.                                                        ////
////                                                                 ////
////   *************************  NOTE  **************************   ////
////   The PIC16C765 provides for 6 uni-directional endpoints, but   ////
////   only provides enough RX/TX endpoint buffer space for 5        ////
////   un-directional endpoints.  (provided that you use 8bytes      ////
////   per max packet size per endpoint).  Because of this this      ////
////   driver does not enable Endpoint 2 TX (Host -> PIC).           ////
////   If you need to change this, see usb_set_configured().         ////
////                                                                 ////
////   *************************  NOTE  **************************   ////
//// This driver uses INT_USB.  It requires INT_USB to interrupt the ////
//// PIC when an event has happened on the USB Bus.  Therfore        ////
//// this code enables interrupts.  A user modification can be made  ////
//// to poll the USB interrupt flag instead of relying on an         ////
//// interrupt.                                                      ////
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
//// June 20th, 2003: Minor cleanup                                  ////
////                                                                 ////
//// October 28th, 2002: Fixed typos                                 ////
////                                                                 ////
//// October 25th, 2002: Changed IN Endpoints to initialize to DATA1 ////
////                     after device configuration                  ////
////                                                                 ////
//// September 12th, 2002: Fixed a problem with usb_put_packet()     ////
////                       not sending packets or sending packets    ////
////                       with all zeros.                           ////
////                                                                 ////
//// August 28th, 2002: Fixed a problem with data toggle sync when   ////
////                    sending data to PC (host).                   ////
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

#INCLUDE "usb.h"

#DEFINE __PIC__   1

#DEFINE  USB_PUT_DTS TRUE
#DEFINE  USB_GET_DTS TRUE

#define ERROR_COUNTER_LEN 8
int ERROR_COUNTER[ERROR_COUNTER_LEN];

/// user settings
//error_ints are what goes into UEIE
#IFNDEF ERROR_INTS
   #define ERROR_INTS      0xFF
#ENDIF
//standard ints is what goes into UIE, EXCEPT FOR ACTIVITY BIT
#IFNDEF STANDARD_INTS
   #define STANDARD_INTS   0x3B
#ENDIF

//start of input/output buffer.  1b8-1df are reserved for usb, so lets use it
//microchip has 6 endpoints (3 bi-directional endpoints, 1 endpoint per direction) but only enough buffer space for 5
#IFNDEF USB_Buffer
   #define USB_Buffer      0xB8	//actual address 0x1B8
#ENDIF

//number of bi-directional endpoints
#DEFINE  USB_MAX_ENDPOINTS  3

//---pic16c7xx memory locations
#byte UIR     =  0x190
#byte UIE     =  0x191
#byte UEIR    =  0x192
#byte UEIE    =  0x193
#byte USTAT   =  0x194
#byte UCTRL   =  0x195
#byte UADDR   =  0x196
#byte USWSTAT =  0x197
#define  UEP0_LOC 0x198
#byte UEP0    =  0x198
#byte UEP1    =  0x199
#byte UEP2    =  0x19A

#byte BD0OST  =  0x1A0
#byte BD0OBC  =  0x1A1
#byte BD0OAL  =  0x1A2

#byte BD1OST  =  0x1A8
#byte BD1OBC  =  0x1A9
#byte BD1OAL  =  0x1AA

#byte BD2OST  =  0x1B0
#byte BD2OBC  =  0x1B1
#byte BD2OAL  =  0x1B2

#byte BD0IST  =  0x1A4
#byte BD0IBC  =  0x1A5
#byte BD0IAL  =  0x1A6

#byte BD1IST  =  0x1AC
#byte BD1IBC  =  0x1AD
#byte BD1IAL  =  0x1AE

#byte BD2IST  =  0x1B4
#byte BD2IBC  =  0x1B5
#byte BD2IAL  =  0x1B6

#byte INTCON  =  0x0B

//---some important bits
#BIT USBIE = 0x8C.3
#BIT USBIF = 0x0C.3
#BIT UIR_TOK_DNE = 0x190.3
#BIT UIR_USB_RST = 0x190.0

//---interrupt flag possibilites
#define USB_RST    0x01
#define UERR       0x02
#define ACTIVITY   0x04
#define TOK_DNE    0x08
#define UIDLE      0x10
#define STALL      0x20

//See UEPn (0x198-0x19A)
#define ENDPT_DISABLED	0x00   //endpoint not used
#define ENDPT_IN_ONLY	0x02    //endpoint supports IN transactions only
#define ENDPT_OUT_ONLY	0x04    //endpoint supports OUT transactions only
#define ENDPT_CONTROL	0x06    //Supports IN, OUT and CONTROL transactions - Only use with EP0
#define ENDPT_NON_CONTROL 0x0E  //Supports both IN and OUT transactions

//Define the states that the USB interface can be in
//See USWST (0x197)
#define	POWERED_STATE	0x00
#define	DEFAULT_STATE	0x01
#define	ADDRESS_STATE	0x02
#define	CONFIG_STATE	0x03

//global variables that we need.
int8 usb_ustat;             //save the USTAT register because the fifo buffer will munch it
int1 IS_IDLE;

//we can't read individual enpdoint stall status, so we must save it our selves
int8 USB_endpoint_in_stalled[USB_MAX_ENDPOINTS];
int8 USB_endpoint_out_stalled[USB_MAX_ENDPOINTS];

//interrupt handler, specific to PIC16C765 peripheral only
void usb_isr();
void usb_isr_rst();
void usb_isr_uerr();
void usb_isr_activity();
void usb_isr_stall();
void usb_isr_uidle();
void usb_isr_tok_dne();

//following functions standard part of CCS PIC USB driver, and used by usb.c
void usb_init();
int1 usb_put_packet(int endpoint, int * ptr, int len, PID_TOGGLE tgl);
int8 usb_get_packet(int8 endpoint, int8 * ptr, int8 max);
void usb_stall_ep(int8 endpoint, int1 direction);
void usb_unstall_ep(int8 endpoint, int1 direction);
int1 usb_endpoint_stalled(int8 endpoint, int1 direction);
void usb_set_address(int8 address);
void usb_set_configured(int config);
void usb_wrongstate();


//// BEGIN User Functions:

/*******************************************************************************
/* usb_init()
/*
/* Summary: Resets and initalizes USB code and USB peripheral.  You must call this
/*          first before using code.
/*
/*          If you have debug enabled it will enable TBE interrupts on the PIC.
/*
/*          NOTE: this enables interrupts.
/*
/********************************************************************************/
void usb_init() {
   USWSTAT=0;     //default to powered state
	UIE=1;         //mask all interrupts except reset
   UADDR=0;       //we start at address 0
	UCTRL=8;       //device attached

   usb_token_reset();

   enable_interrupts(int_usb);
   enable_interrupts(global);

 #IF USB_DO_DEBUG
   enable_interrupts(INT_TBE);
 #ENDIF
}

/*******************************************************************************
/* usb_put_packet(endpoint,*ptr,len,toggle)
/*
/* Input: endpoint - endpoint to send packet to
/*        ptr - points to data to send
/*        len - amount of data to send
/*        toggle - whether to send data with a DATA0 pid, a DATA1 pid, or toggle from the last DATAx pid.
/*
/* Output: TRUE if data was sent correctly, FALSE if it was not.  The only reason it will
/*         return FALSE is if because the TX buffer is still full from the last time you
/*         tried to send a packet.
/*
/* Summary: Sends one packet out the EP to the host.  Notice that there is a difference
/*          between a packet and a message.  If you wanted to send a 512 byte message you
/*          would accomplish this by sending 8 64-byte packets, followed by a 0 length packet.
/*          If the last (or only packet) being sent is less than the max packet size defined
/*          in your descriptor then you do not need to send a 0 length packet to identify
/*          an end of message.
/*
/*          usb_puts() (provided in usb.c) will send a multi-packet message correctly.
/*
/********************************************************************************/
int1 usb_put_packet(int8 endpoint, int8 * ptr, int8 len, PID_TOGGLE tgl) {
   int8 i;
   int8 * add=0x1A4; //BD0IST - EP0 IN (out of PIC) Status
   int8 * buff_add;

   debug(debug_txb,"\r\nTX %X %X: ",endpoint,len);

   add+=(endpoint*8);   //point to BDxIST

   i = (*add) & 0x80;

   if (i) {return(0);}
   else {
      add+=2;  //point to BDxIAL

      buff_add=(long)0x100 + *add;

      for (i=0;i<len;i++) {
         *buff_add=*ptr;
         debug(debug_txb,"%X ",*buff_add);
         buff_add++;
         ptr++;
      }

      add--;//point back to buffer counter;
      *add=len;  //set buffer count to what we said
      add--;//point at status register;
      if (tgl == TOGGLE) {
//         printf("%X ",i);
         i=*add;//get current status register.  notice that bdstatus is dual nature, the bits have different meaning if
         //you read or write to them.  bits 7 and 6, which we want to modify now, are the same in both though
         i&=0xC0; //clear bits 5-0;
         i^=0x40; //toggle data0/1 bit
    #IF USB_PUT_DTS
         i|=0x88;
    #ELSE
         i|=0x80; //set UOWN bit.  USB is now assigned to BD. do not set dts (data toggle synch)
    #ENDIF
      }
      else if (tgl == DATA1) {
    #IF USB_PUT_DTS
         i=0xC8;
    #ELSE
         i=0xC0;
    #ENDIF
      } //DATA1, UOWN
      else if (tgl == DATA0) {
    #IF USB_PUT_DTS
         i=0x88;
    #ELSE
         i=0x80;
    #ENDIF
      } //DATA0, UOWN

      *add=i;//save changes
//      printf("%LX %X",add,*add);
      debug(debug_txb," %X", i);
    }
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
/* Summary: Gets a packet of data from the USB buffer and puts into local PIC RAM.
/*          You could poll usb_get_packet() until it doesn't return 0, or you could
/*          poll usb_epX_rx_status.rx (where X is endpoint number) until .rx = 1.
/*          usb_kbhit(endpoint) (located in usb.c) will also poll usb_epX_rx_status.rx,
/*          but is easier to read.
/*
/********************************************************************************/
int8 usb_get_packet(int8 endpoint, int8 * ptr, int8 max) {
   int8 * buff = 0x1A1; //BD0OBC - EP0 OUT (in to PIC) byte count
   int8 * al=0x100;
   int8 len;
   int8 i=0;

   buff+=endpoint*8; //goto BDxOL
   len=*buff;
   buff++;           //goto BDxAL
   al+=*buff;

   debug(debug_txb,"\r\nRX %X %X: ", endpoint,len);

   while ((i<len)&&(i<max)) {
       debug("%X ",*al);
       *ptr=*al;
       ptr++;
       al++;
       i++;
   }

   buff--;  //goto bc
   *buff=USB_MAX_EP0_PACKET_LENGTH;
   buff--;  //goto status


   i=*buff;//get current status register.  notice that bdstatus is dual nature, the bits have different meaning if
            //you read or write to them.  bits 7 and 6, which we want to modify now, are the same in both though

   i &= 0xC0; //clear bits 5-0;
   i ^= 0x40; //toggle data0/1 bit
 #IF USB_GET_DTS
   i |= 0x88; //set own and dts bit
 #ELSE
   i |= 0x80; //set own
 #ENDIF

   *buff=i;

   debug(debug_txb," %X",i);

   return(i);
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
   int8 * addy=0x1A0;
   int8 bd_stat;

   if (endpoint < USB_MAX_ENDPOINTS) {     //pic has only 3 endpoints.  starts at 0 (0, 1, 2)
      addy+=endpoint*8;
      if (direction) {addy+=4;}

      bd_stat=*addy;
      bd_stat=(bd_stat & 0xC0) | 0x0C;
      *addy=bd_stat;

      if (direction) {
         USB_endpoint_in_stalled[endpoint]=1;
      }
      else {
         USB_endpoint_out_stalled[endpoint]=1;
      }

         //debug
         if (direction) {endpoint=endpoint | 0x80;}
         debug(debug_txb, " SE%X",endpoint | (direction << 7));;
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
   int8 * addy=0x1A0;
   int8 bd_stat;

   if (endpoint < USB_MAX_ENDPOINTS) {     //pic has only 3 endpoints.  starts at 0 (0, 1, 2)
      addy+=endpoint*8;
      if (direction) {addy+=4;}

      bd_stat=*addy;
      bd_stat=(bd_stat & 0xC0) | 0x08;
      *addy=bd_stat;

      addy=UEP0_LOC + endpoint;
      bit_clear(*addy,0);

      if (direction) {
         USB_endpoint_in_stalled[endpoint]=0;
      }
      else {
         USB_endpoint_out_stalled[endpoint]=0;
      }

         //debug
         if (direction) {endpoint=endpoint | 0x80;}
         debug(debug_txb, " USE%X",endpoint | (direction << 7));
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
   if (direction) {
         return(USB_endpoint_in_stalled[endpoint]);
   }
   else {
         return(USB_endpoint_out_stalled[endpoint]);
   }
}


/*******************************************************************************
/* usb_set_address(address)
/*
/* Input: address - address the host specified that we use
/*
/* Summary: Configures the USB Peripheral for the specified device address.  The host
/*          will now talk to use with the following address.
/*
/********************************************************************************/
void usb_set_address(int8 address) {
   UADDR=address;

   if (address) {
      USWSTAT=(USWSTAT & 0xFC) | 2; //if we got an address other than 0 set the enumeration status to ADDRESSED
   }
   else {
      USWSTAT=(USWSTAT & 0xFC); //unconfigured state
   }

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
      USB_dev_req=SET_CONFIG;
      USWSTAT=USWSTAT & 0xFC; //clear config bits for being set in next line
      if (config==0) {USWSTAT=USWSTAT | ADDRESS_STATE;} //if config=0 then set addressed state
      else {USWSTAT=USWSTAT | CONFIG_STATE;} //else set configed state

      //configure ep1 and ep2 enpoints.  will user want these differently?
/* //endpoints initialized DATA0
 #IF USB_GET_DTS
      BD1OST = 0x88; //set own and dts bit
 #ELSE
      BD1OST = 0x80; //set own
 #ENDIF
      BD1OAL=USB_Buffer+0x10;
      BD1OBC=8;

 #IF USB_PUT_DTS
      BD1IST = 0x08; //set dts bit
 #ELSE
      BD1IST = 0x00;
 #ENDIF
      BD1IAL=USB_Buffer+0x18;

 #IF USB_GET_DTS
      BD2OST = 0x88; //set own and dts bit
 #ELSE
      BD2OST = 0x80; //set own
 #ENDIF
      BD2OAL=USB_Buffer+0x20;
      BD2OBC=8;
*/

//IN endpoints initialized DATA1, OUT endpoints initialized to DATA0
 #IF USB_GET_DTS
      BD1OST = 0x88; //set own and dts bit
 #ELSE
      BD1OST = 0x80; //set own
 #ENDIF
      BD1OAL=USB_Buffer+0x10;
      BD1OBC=8;

 #IF USB_PUT_DTS
      BD1IST = 0x48; //set dts bit
 #ELSE
      BD1IST = 0x40;
 #ENDIF
      BD1IAL=USB_Buffer+0x18;

 #IF USB_GET_DTS
      BD2OST = 0x88; //set own and dts bit
 #ELSE
      BD2OST = 0x80; //set own
 #ENDIF
      BD2OAL=USB_Buffer+0x20;
      BD2OBC=8;

/* //THERE IS NOT ENOUGH BUFFER RAM TO USE THIS ENPDOINT
      BD2IST=0x08;      //clear owns bit (PIC can write)
      BD2IST=USB_Buffer+0x28;
*/

// Set up the Endpoint Control Registers.  The following patterns are defined
// ENDPT_DISABLED - endpoint not used
// ENDPT_IN_ONLY  - endpoint supports IN transactions only
// ENDPT_OUT_ONLY - endpoint supports OUT transactions only
// ENDPT_CONTROL	 - Supports IN, OUT and CONTROL transactions - Only use with EP0
// ENDPT_NON_CONTROL - Supports both IN and OUT transactions
      UEP1=ENDPT_NON_CONTROL;
      UEP2=ENDPT_OUT_ONLY;
}


/*******************************************************************************
/* usb_wrongstate()
/*
/* Summary: Tells the host that they asked us to do something (either illegal or something
/*          we don't support) by stalling EP0.  When the host gets the idea it will
/*          either reset the device or un-stall EP0.
/*
/********************************************************************************/
void usb_wrongstate() {
   bit_set(UEP0,0);
}

/// END Hardware layer functions required by USB.C


/// BEGIN USB Interrupt Service Routine

/*******************************************************************************
/* usb_isr()
/*
/* Summary: Checks the interrupt, and acts upon event.  Processing finished
/*          tokens is the majority of this code, and is handled by usb.c
/*
/********************************************************************************/
#int_usb
void usb_isr() {
   int1 notdone=1;
   int interrupts;
   do {
      interrupts=UIE & UIR;
      debug(debug_txb,"\r\nI %X: ", interrupts);
      if (interrupts & USB_RST) {usb_isr_rst(); bit_clear(UIR,0);}        //usb reset has been detected
      else if (interrupts & UERR) {usb_isr_uerr(); bit_clear(UIR,1);}          //error has been detected
      else if (interrupts & ACTIVITY) {usb_isr_activity(); bit_clear(UIR,2);}  //activity detected.  (only enable after sleep)
      else if (interrupts & UIDLE) {usb_isr_uidle(); bit_clear(UIR,4);}        //idle time, we can go to sleep
      else if (interrupts & TOK_DNE) {usb_isr_tok_dne(); bit_clear(UIR,3);}    //a token has been detected (majority of isrs)
      else if (interrupts & STALL) {usb_isr_stall(); bit_clear(UIR,7);}        //a stall handshake was sent
      else {notdone=0;}
   } while (notdone);
   USBIF=0;
   UIR=0;
}


/*******************************************************************************
/* usb_isr_rst()
/*
/* Summary: The host (computer) sent us a RESET command.  Reset USB device
/*          and token handler code to initial state.
/*
/********************************************************************************/
void usb_isr_rst() {
   int8 * add=0x100;
   int i;

   debug(debug_txb,"R ");

   usb_token_reset();

   IS_IDLE=0;

   for (i=0;i<USB_MAX_ENDPOINTS;i++) {
      USB_endpoint_in_stalled[i]=0;
      USB_endpoint_out_stalled[i]=0;
   }

   UIR_TOK_DNE=0;    //do this 4 times to clear out the ustat fifo
   UIR_TOK_DNE=0;
   UIR_TOK_DNE=0;
   UIR_TOK_DNE=0;


 #IF USB_GET_DTS
   BD0OST = 0x88; //set own and dts bit
 #ELSE
   BD0OST = 0x80; //set own
 #ENDIF
   BD0OAL=USB_Buffer; //Endpoint 0 OUT gets a buffer, set up buffer address
   BD0OBC=USB_MAX_EP0_PACKET_LENGTH;          //set byte count

 #IF USB_PUT_DTS
   BD0IST = 0x08; //set dts bit
 #ELSE
   BD0IST = 0x00;
 #ENDIF

   BD0IAL=USB_Buffer+8;

   add+=USB_Buffer;
   for (i=0;i<40;i++) {
      *add=0;
      add++;
   }

   UADDR=0;          //set USB Address to 0
   UIR_USB_RST=0;    //clear reset flag

   UEP0=ENDPT_CONTROL; //endpoint 0 is a control pipe and requires an ACK
   UEP1=ENDPT_DISABLED; //turn on endpoint 1 is an IN/OUT pipe.
   UEP2=ENDPT_DISABLED;

   UIE=STANDARD_INTS;         //enable all interrupts except activity
   UEIE=ERROR_INTS;        //enable all error interrupts

   USWSTAT=(USWSTAT & 0xFC) | DEFAULT_STATE; //put usb mcu into default state, but keep top 6 bits
}

/*******************************************************************************
/* usb_isr_uerr()
/*
/* Summary: The USB peripheral had an error.  If user specified, error counter
/*          will incerement.  I having problems check the status of these 8 bytes.
/*
/********************************************************************************/
void usb_isr_uerr() {
   int ints;

   debug(debug_txb,"E ");

#IFDEF ERROR_COUNTER_LEN
   ints=UEIR & UEIE; //mask off the flags with the ones that are enabled
   if ( bit_test(ints,0) ) {ERROR_COUNTER[0]++;}   //increment pid_error counter
   if ( bit_test(ints,1) ) {ERROR_COUNTER[1]++;}   //increment crc5 error counter
   if ( bit_test(ints,2) ) {ERROR_COUNTER[2]++;}   //increment crc16 error counter
   if ( bit_test(ints,3) ) {ERROR_COUNTER[3]++;}   //increment dfn8 error counter
   if ( bit_test(ints,4) ) {ERROR_COUNTER[4]++;}   //increment bto error counter
   if ( bit_test(ints,5) ) {ERROR_COUNTER[5]++;}   //increment wrt error counter
   if ( bit_test(ints,6) ) {ERROR_COUNTER[6]++;}   //increment own error counter
   if ( bit_test(ints,7) ) {ERROR_COUNTER[7]++;}   //increment bts error counter
#ENDIF

   UEIR=0;           //clear flags
   bit_clear(UIR,1); //clear UIR.UERR
}

/*******************************************************************************
/* usb_isr_uidle()
/*
/* Summary: USB peripheral detected IDLE.  Put the USB peripheral to sleep.
/*
/********************************************************************************/
void usb_isr_uidle() {
   debug(debug_txb,"I ");

   bit_clear(UIR,4); //uidle
   bit_set(UCTRL,1); //set suspend. we are now suspended
   bit_clear(UIR,2); //clear activity interept flag
   bit_set(UIE,2);   //enable activity interrupt flag. (we are now suspended until we get an activity interrupt. nice)
	IS_IDLE=1;
}


/*******************************************************************************
/* usb_isr_activity()
/*
/* Summary: USB peripheral detected activity on the USB device.  Wake-up the USB
/*          peripheral.
/*
/********************************************************************************/
void usb_isr_activity() {
   debug(debug_txb,"A ");

   bit_clear(UIR,2);  //clear activity flag
   bit_clear(UIE,2);  //clear activity interupt enabling
   bit_clear(UCTRL,1); //turn off low power suspending
   bit_clear(UIR,4);  //clear idle flag
   bit_set(UIE,4);    //turn on idle interrupts
   IS_IDLE=0;
}


/*******************************************************************************
/* usb_isr_stall()
/*
/* Summary: Stall handshake detected.
/*
/********************************************************************************/
void usb_isr_stall() {
   debug(debug_txb,"S ");

   bit_clear(UIR,5); //stall
}

/*******************************************************************************
/* usb_isr_tok_dne()
/*
/* Summary: A Token (IN/OUT/SETUP) has been received by the USB peripheral.
/*          Information about token is received, and sent to usb.c's token handling
/*          code.
/*
/********************************************************************************/
void usb_isr_tok_dne() {
   int8 i,PIDs, BD_count, BD_loc, BD_status, endpoint;
   int8 * uep;
   int8 * bd;

	usb_ustat=USTAT;	// copy USTAT register before..., 0x194

  while (UIR_TOK_DNE) {
   uep=0x1A0;
	UIR_TOK_DNE=0;	   // clearing the token done interrupt., 0x190.3
                     // if next ustat fifo is valid then this will auto be reset to 1

   uep+=usb_ustat;
   BD_status=*uep;   uep++;
   BD_count=*uep;    uep++;
   BD_loc=*uep;

   PIDs=BD_status & 0x3C;
   PIDs=PIDs >> 2;

   bd=BD_loc+0x100;

   endpoint=(usb_ustat & 0x18) >> 3;

   if (PIDs==PID_IN) {usb_isr_tok_in_dne(endpoint); debug(debug_txb,"TX ");}  //pic -> host transfer completed
   else if (PIDs==PID_OUT) {usb_isr_tok_out_dne(endpoint); debug(debug_txb,"RX ");} //host -> pic transfer completed
   else if (PIDs==PID_SETUP) {   //new setup token in the buffer
      debug(debug_txb,"RX S ");
      for(i=0;i<BD_count;i++) {
         debug("%X ",*bd);
         usb_ep0_rx_buffer[i]=*bd;
         bd++;
      }
      #IF USB_GET_DTS
         BD0OST=0x88;   // set UOWNs bit back to SIE
      #ELSE
         BD0OST=0x80;   // set UOWNs bit back to SIE
      #ENDIF
   	BD0OBC=USB_MAX_EP0_PACKET_LENGTH;   // reset the byte count too.
      #IF USB_PUT_DTS
       	BD0IST=0x08;	// return the in buffer to us (dequeue any pending requests)
      #ELSE
         BD0IST=0x00;
      #ENDIF
      usb_isr_tok_setup_dne();
      bit_clear(UCTRL,4);   // UCTRL,PKT_DIS	; Assuming there is nothing to dequeue, clear the packet disable bit
   }
	USB_USTAT=USTAT;	// get next ustat, if there is one
 }
}

/// END USB Interrupt Service Routine

#ENDIF
