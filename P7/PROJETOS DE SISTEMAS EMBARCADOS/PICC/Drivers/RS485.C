
// int rs485_get_message( int * data_ptr, int wait )
           // Data will be filled in at pointer as follows:
           //             FROM_ID  DATALEN  DATA...
           // Function returns FALSE if no data want WAIT is FALSE

// int rs485_send_message( int to, int len, int * data )

// void rs485_wait_for_bus()
           // Call before rs485_send_message unless this is a response

// The following is our unit ID:
#ifndef RS485_ID
#define RS485_ID  0x10
#endif

#ifndef RS485_USE_EXT_INT
#define RS485_USE_EXT_INT FALSE
#endif

#if (RS485_USE_EXT_INT==FALSE)
   #define RS485_RX_PIN       PIN_C7
   #define RS485_TX_PIN       PIN_C6
   #define RS485_ENABLE_PIN   PIN_B4   //controls DE pin.  low for RX, high for TX
   #define RS485_RX_ENABLE    PIN_B5   //controls RE pin.  should keep low.

   #use rs232(baud=9600, xmit=RS485_TX_PIN, rcv=RS485_RX_PIN, enable=RS485_ENABLE_PIN, bits=9, long_data, errors, stream=RS485)
   #use rs232(baud=9600, xmit=RS485_TX_PIN, rcv=RS485_RX_PIN, enable=RS485_ENABLE_PIN, bits=9, long_data, force_sw, multi_master, errors, stream=RS485_CD)

   #if getenv("AUART")
      //#define RCV_ON() {setup_uart(UART_ADDRESS);setup_uart(TRUE);}
      #define RCV_OFF() {setup_uart(FALSE);}
   #else
      //#define RCV_ON() {setup_uart(TRUE);}
      #define RCV_OFF() {setup_uart(FALSE);}
   #endif
#else
   #define RS485_RX_PIN       PIN_B0
   #define RS485_TX_PIN       PIN_B3
   #define RS485_ENABLE_PIN   PIN_B4   //controls DE pin.  low for RX, high for TX
   #define RS485_RX_ENABLE    PIN_B5   //controls RE pin.  should keep low.

   #use rs232(baud=9600, xmit=RS485_TX_PIN, rcv=RS485_RX_PIN, enable=RS485_ENABLE_PIN, bits=9, long_data, errors, stream=RS485)
   #use rs232(baud=9600, xmit=RS485_TX_PIN, rcv=RS485_RX_PIN, enable=RS485_ENABLE_PIN, bits=9, long_data, multi_master, errors, stream=RS485_CD)

   #if defined(__PCH__)
    #bit ext_intf=0xFF2.1
   #else
    #bit ext_intf=0x0B.1
   #endif

   //#define RCV_ON() {enable_interrupts(INT_EXT);}
   #define RCV_OFF() {disable_interrupts(INT_EXT);}
#endif



#define RS485_wait_time 20  //Milliseconds

#bit  rs485_collision = rs232_errors.6

#define RS485_RX_BUFFER_SIZE  64

int rs485_state,rs485_ni,rs485_no;
int rs485_buffer[RS485_RX_BUFFER_SIZE];

void RCV_ON(void) {
 #if (RS485_USE_EXT_INT==FALSE)
   while(kbhit(RS485)) {getc();} //clear rx buffer. clear rda interrupt flag. clear overrun error flag.
   #if getenv("AUART")
      setup_uart(UART_ADDRESS);
      setup_uart(TRUE);
   #else
      setup_uart(TRUE);
   #endif
 #else
    ext_intf=0;
    enable_interrupts(INT_EXT);
 #endif
}


void rs485_init() {
   RCV_ON();
   rs485_state=0;
   rs485_ni=0;
   rs485_no=0;
   #if RS485_USE_EXT_INT==FALSE
   enable_interrupts(INT_RDA);
   #else
   ext_int_edge(H_TO_L);
   enable_interrupts(INT_EXT);
   #endif
   enable_interrupts(GLOBAL);
   output_low(RS485_RX_ENABLE);
}


void rs485_ptr_inc(int *index, int inc) {
   int i;
   i=*index;

   i+=inc;
   if (i>=RS485_RX_BUFFER_SIZE) {i-=RS485_RX_BUFFER_SIZE;}

   *index=i;
}

#if (RS485_USE_EXT_INT==FALSE)
#int_rda
#else
#int_ext
#endif
void incomming_rs485() {
   int16 b;
   static int8 cs,state,mlen,len,temp_ni;
   static int16 to,source;

   b=fgetc(RS485);

   switch(state) {
      case 0:  //get from address
         temp_ni=rs485_ni;
         source=b;
         rs485_buffer[temp_ni]=source;
         rs485_ptr_inc(&temp_ni, 1);
         break;

      case 1:  //get to address
         to=b;
         break;

      case 2:  //get len
         len=b;
         mlen=b;
         cs=len^(to&0xFF)^(source&0xFF);
         rs485_buffer[temp_ni]=len;
         rs485_ptr_inc(&temp_ni, 1);
         #if (getenv("AUART")&&(RS485_USE_EXT_INT==FALSE))
           setup_uart(UART_DATA);
         #endif
         break;

      case 255:   //get checksum
         cs^=b;
         if ((!cs)&&(bit_test(to,8))&&(bit_test(source,8))&&((to&0xFF)==RS485_ID)) {  //if cs==0, then checksum is good
            rs485_ni=temp_ni;
         }
         #if (getenv("AUART")&&(RS485_USE_EXT_INT==FALSE))
           setup_uart(UART_ADDRESS);
         #endif

         state=0;
         return;

      default: //get data
         rs485_buffer[temp_ni]=b;
         cs^=b;
         rs485_ptr_inc(&temp_ni, 1);
         len--;
         break;
   }
   if ((state>=3)&&(!len)) {
      state=255;
   }
   else {
      state++;
   }
}



int rs485_send_message( int to, int len, int * data ) {
            // Format:  source | destination | data-length | data | checksum
    int try,i,cs;

    RCV_OFF();
    for(try=1; try<=5; try++) {
       rs485_collision = 0;
       fputc((int16)0x100|rs485_id,RS485_CD);
       fputc((int16)0x100|to,RS485_CD);
       fputc(len,RS485_CD);
       for(i=0,cs=rs485_id^to^len; i<len; i++) {
          cs^=*data;
          fputc(*data,RS485_CD);
          ++data;
       }
       fputc(cs,RS485_CD);
       if(rs485_collision) {
          delay_ms(rs485_id);
          continue;
       }
       RCV_ON();
       return(TRUE);
    }
    RCV_ON();
    return(FALSE);
}



int1 rs485_wait_for_bus(int1 clrwdt) {
    int i;

    RCV_OFF();
    for(i=0;i<=(rs485_wait_time*20);i++) {
       if(!input(RS485_RX_PIN))
         return(TRUE);
       else
         delay_us(50);
       if (clrwdt) {
         restart_wdt();
       }
    }
    return(FALSE);
}


int rs485_get_message( int * data_ptr, int1 wait ) {
           // Data will be filled in at pointer as follows:
           //             FROM_ID  DATALEN  DATA...
           // Function returns FALSE if no data want WAIT is FALSE
    int n;

    if(wait)
       while(rs485_ni==rs485_no) ;
    if(rs485_ni==rs485_no)
       return FALSE;
    else {
       *data_ptr=rs485_buffer[rs485_no];
       rs485_no=(rs485_no+1)%sizeof(rs485_buffer);
       data_ptr++;
       n=rs485_buffer[rs485_no]+1;
       for(;n!=0;--n) {
          *data_ptr=rs485_buffer[rs485_no];
          rs485_no=(rs485_no+1)%sizeof(rs485_buffer);
          data_ptr++;
       }
       return TRUE;
    }
}
