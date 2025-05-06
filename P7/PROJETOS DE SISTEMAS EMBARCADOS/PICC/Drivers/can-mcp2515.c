///////////////////////////////////////////////////////////////////////////////
////                             can-mcp2515.c                             ////
////                                                                       ////
////  Driver for Microchip's MCP2515 external CAN controller with SPI      ////
////  interface.                                                           ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2019 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

#include <can-mcp2515.h>

/////////////////////////// MCP2515 Clock Settings //////////////////////////

#ifndef MCP2515_EXT_CLOCK_SPEED
 #define MCP2515_EXT_CLOCK_SPEED    20000000 //1-40 MHz
#endif

#ifndef MCP2515_CLOCK_OUT_DIVISOR
 #define MCP2515_CLOCK_OUT_DIVISOR        1  //1, 2, 4 or 8
#endif

#if (MCP2515_EXT_CLOCK_SPEED < 1000000) || (MCP2515_EXT_CLOCK_SPEED > 40000000)
 #error The MCP2515 can only be clocked from an external clock from 1 MHz to 40 MHz
#endif

#if (MCP2515_CLOCK_OUT_DIVISOR != 1) && (MCP2515_CLOCK_OUT_DIVISOR != 2) && (MCP2515_CLOCK_OUT_DIVISOR != 4) && (MCP2515_CLOCK_OUT_DIVISOR != 8)
 #error The MCP2515 Clock Out Divisor can only be 1, 2, 4 or 8
#endif

//////////////////////////// MCP2515 SPI Settings ///////////////////////////

#if !definedinc(MCP2515Xfer)
 #ifndef MCP2515_SPI_STREAM
  #ifndef MCP2515_SPI_SCK_PIN
   #if defined(__PCD__)
    #define MCP2515_SPI_SCK_PIN  PIN_E5
   #else
    #define MCP2515_SPI_SCK_PIN  PIN_C3
   #endif
  #endif
 
  #ifndef MCP2515_SPI_SDI_PIN
   #if defined(__PCD__)
    #define MCP2515_SPI_SDI_PIN  PIN_B2   //MOSI
   #else
    #define MCP2515_SPI_SDI_PIN  PIN_C1   //MOSI
   #endif
  #endif
 
  #ifndef MCP2515_SPI_SDO_PIN
   #if defined(__PCD__)
    #define MCP2515_SPI_SDO_PIN  PIN_B4   //MISO
   #else
    #define MCP2515_SPI_SDO_PIN  PIN_C0   //MISO
   #endif
  #endif
 
  #ifndef MCP2515_SPI_BAUD
   #define MCP2515_SPI_BAUD      10000000
  #endif
 
  #use spi(MASTER, CLK=MCP2515_SPI_SCK_PIN, DI=MCP2515_SPI_SDO_PIN, DO=MCP2515_SPI_SDI_PIN, BAUD=MCP2515_SPI_BAUD, BITS=8, MODE=0, STREAM=MCP2515_SPI_STREAM)
 #endif
 
 #define MCP2515Xfer(a)    spi_xfer(MCP2515_SPI_STREAM, a)
#endif

#ifndef MCP2515_SPI_CS_PIN
 #define MCP2515_SPI_CS_PIN            PIN_B1
#endif

#ifndef MCP2515_CLKO_TYPE
 #define MCP2515_CLKO_TYPE             0  //0-Clock on CLKO pin, 1-SOF signal on CLKO pin
#endif

#ifndef MCP2515_CLKO_ENABLED
 #define MCP2515_CLKO_ENABLED          TRUE //TRUE - CLKOUT pin is enabled, FALSE - CLKOUT pin is disabled
#endif

#ifndef MCP2515_RX0BF_PIN
 #ifndef MCP2515_RX0_OUTPUT
  #define MCP2515_RX0_OUTPUT           FALSE //FALSE-pin function disabled, TRUE-pin function enabled
 #endif
 
 #if MCP2515_RX0_OUTPUT == TRUE
  #ifndef MCP2515_RX0_DEFAULT_LEVEL
   #define MCP2515_RX0_DEFAULT_LEVEL   0  //0-drive pin low, 1-drive pin high
  #endif
 #endif
#endif

#ifndef MCP2515_RX1BF_PIN
 #ifndef MCP2515_RX1_OUTPUT
  #define MCP2515_RX1_OUTPUT           FALSE //FALSE-pin function disabled, TRUE-pin function enabled
 #endif
 
 #if MCP2515_RX1_OUTPUT == TRUE
  #ifndef MCP2515_RX1_DEFAULT_LEVEL
   #define MCP2515_RX0_DEFAULT_LEVEL   0  //0-drive pin low, 1-drive pin high
  #endif
 #endif
#endif

///////////////////////////// MCP2515 CAN Settings ////////////////////////////

#define CAN_CLOCK_SPEED       MCP2515_EXT_CLOCK_SPEED

#ifndef CAN_BAUD_RATE
 #define CAN_BAUD_RATE        125000
#endif

#if (CAN_CLOCK_SPEED % CAN_BAUD_RATE) != 0
 #error CAN Clock Speed not evenly divisible by CAN Baud Rate
#endif

#ifndef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT   875   //in tenth of a percent, e.g. 875 = 87.5% of bit time
#endif

#if CAN_DEFAULT_SAMPLE_POINT < 500
 #undef CAN_DEFAULT_SAMPLE_POINT
 #define CAN_DEFAULT_SAMPLE_POINT   500
 #warning CAN Sample point changed to 50% of bit time
#endif

#ifndef CAN_BRG_SAM
 #define CAN_BRG_SAM                0 //sample of the can bus line (def: bus line is sampled 1 times prior to sample point)
#endif

#ifndef CAN_BRG_WAKE_FILTER
 #define CAN_BRG_WAKE_FILTER        FALSE   //selects can bus line filter for wake up bit
#endif

#ifndef CAN_TX_RETRANSMISSION_ATTEMPTS
 #define CAN_TX_RETRANSMISSION_ATTEMPTS   RETRANSMIT_ATTEMPTS_UNLIMITED //see RETRANSMIT_ATTEMPTS type in can-mcp2515.h for options
#endif

#ifndef CAN_USE_RX_DOUBLE_BUFFER
 #define CAN_USE_RX_DOUBLE_BUFFER   TRUE   //TRUE-if buffer 0 overflows massage will be loaded into buffer 1, FALSE-if buffer 0 overflows message will not be loaded into buffer 1
#endif

///////////////////////////// CAN Filter Settings /////////////////////////////

/*
   If no CAN RX Filters are going to be used then nothing needs to be done, by
   default this driver will initialize the CAN controller to use no filters.
   
   If CAN RX Filters are going to be used, the can_init() function can be used
   to initialize them by making the following defines before this driver is
   included:
   
   #define CAN_USE_FILTERS          TRUE
   
   #define CAN_USER_FILT_x          aa    // x being the filter to enable (0-5), and aa being the Id to filter for.
                                          //    Filters 0 and 1 are for RX Buffer 0, and filters 2 to 5 are for RX
                                          //    Buffer 1
   #define CAN_USER_FILT_x_TYPE     bb    // x being the filter to setup (0-5), and bb can be one of the following:
                                          //       CAN_FILTER_TYPE_SID - only match SID messages
                                          //       CAN_FILTER_TYPE_EID - only match EID messages
   
   #define CAN_USER_MASK_x          cc    // x being the mask to setup (0-1), and cc being the ID mask to set.
                                          //    Mask 0 is for RX Buffer 0, and mask 1 is for RX Buffer 1
   #define CAN_USER_MASK_x_TYPE     dd    // x being the mask to setup (0-1), and cc being one of the following:
                                          //       CAN_MASK_TYPE_SID - only write the SID bits of mask, EID bits are set to 0
                                          //       CAN_MASK_TYPE_EID - write both the SID and EID bit of mask
*/

#ifndef CAN_USE_FILTERS
 #define CAN_USE_FILTERS            FALSE
#endif

#if CAN_USE_FILTERS == TRUE
 #ifndef CAN_USER_FILT_0
  #ifdef CAN_USER_FILT_1
   #define CAN_USER_FILT_0          CAN_USER_FILT_1
   
   #ifndef CAN_USER_FILT_0_TYPE
    #ifdef CAN_USER_FILT_1_TYPE
     #define CAN_USER_FILT_0_TYPE   CAN_USER_FILT_1_TYPE
    #endif
   #endif
  #else
   #define CAN_USER_FILT_0          0
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_0_TYPE
  #define CAN_USER_FILT_0_TYPE      CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_1
  #define CAN_USER_FILT_1           CAN_USER_FILT_0
 #endif
 
 #ifndef CAN_USER_FILT_1_TYPE
  #define CAN_USER_FILT_1_TYPE      CAN_USER_FILT_0_TYPE
 #endif
 
 #ifndef CAN_USER_FILT_2
  #ifdef CAN_USER_FILT_3
   #define CAN_USER_FILT_2          CAN_USER_FILT_3
   
   #ifndef CAN_USER_FILT_2_TYPE
    #ifdef CAN_USER_FILT_3_TYPE
     #define CAN_USER_FILT_2_TYPE   CAN_USER_FILT_3_TYPE
    #endif
   #endif
  #elif defined(CAN_USER_FILT_4)
   #define CAN_USER_FILT_2          CAN_USER_FILT_4
   
   #ifndef CAN_USER_FILT_2_TYPE
    #ifdef CAN_USER_FILT_4_TYPE
     #define CAN_USER_FILT_2_TYPE   CAN_USER_FILT_4_TYPE
    #endif
   #endif
  #elif defined(CAN_USER_FILT_5)
   #define CAN_USER_FILT_2          CAN_USER_FILT_5
   
   #ifndef CAN_USER_FILT_2_TYPE
    #ifdef CAN_USER_FILT_5_TYPE
     #define CAN_USER_FILT_2_TYPE   CAN_USER_FILT_5_TYPE
    #endif
   #endif
  #else
   #define CAN_USER_FILT_2          CAN_USER_FILT_0
   
   #ifndef CAN_USER_FILT_2_TYPE
    #define CAN_USER_FILT_2_TYPE    CAN_USER_FILT_0_TYPE
   #endif
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_2_TYPE
  #define CAN_USER_FILT_2_TYPE      CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_3
  #define CAN_USER_FILT_3           CAN_USER_FILT_2
  
  #ifndef CAN_USER_FILT_3_TYPE
   #define CAN_USER_FILT_3_TYPE      CAN_USER_FILT_2_TYPE
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_3_TYPE
  #define CAN_USER_FILT_3_TYPE      CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_4
  #define CAN_USER_FILT_4           CAN_USER_FILT_2
  
  #ifndef CAN_USER_FILT_4_TYPE
   #define CAN_USER_FILT_4_TYPE      CAN_USER_FILT_2_TYPE
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_4_TYPE
  #define CAN_USER_FILT_4_TYPE      CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_FILT_5
  #define CAN_USER_FILT_5           CAN_USER_FILT_2
  
  #ifndef CAN_USER_FILT_5_TYPE
   #define CAN_USER_FILT_5_TYPE      CAN_USER_FILT_2_TYPE
  #endif
 #endif
 
 #ifndef CAN_USER_FILT_5_TYPE
  #define CAN_USER_FILT_5_TYPE      CAN_FILTER_TYPE_EID
 #endif
 
 #ifndef CAN_USER_MASK_0
  #define CAN_USER_MASK_0           0
 #endif
 
 #ifndef CAN_USER_MASK_0_TYPE
  #define CAN_USER_MASK_0_TYPE      CAN_MASK_TYPE_EID
 #endif
 
 #ifndef CAN_USER_MASK_1
  #define CAN_USER_MASK_1           CAN_USER_MASK_0 
 #endif
 
 #ifndef CAN_USER_MASK_1_TYPE
  #define CAN_USER_MASK_1_TYPE     CAN_USER_MASK_0_TYPE
 #endif
#endif

///////////////////////////////////////////////////////////////////////////////

// MCP2515 SPI Commands
#define MCP2515_CMD_RESET           0xC0
#define MCP2515_CMD_READ            0x03
#define MCP2515_CMD_READ_RX_BUFFER  0x90
#define MCP2515_CMD_WRITE           0x02
#define MCP2515_CMD_LOAD_TX_BUFFER  0x40
#define MCP2515_CMD_RTS             0x80
#define MCP2515_CMD_READ_STATUS     0xA0
#define MCP2515_CMD_READ_RX_STATUS  0xB0
#define MCP2515_CMD_BIT_MODIFY      0x05

// MCP2515 Register Addresses
#define MCP2515_ADDR_REG_RXF0       0x00
#define MCP2515_ADDR_REG_RXF1       0x04
#define MCP2515_ADDR_REG_RXF2       0x08
#define MCP2515_ADDR_REG_RXF3       0x10
#define MCP2515_ADDR_REG_RXF4       0x14
#define MCP2515_ADDR_REG_RXF5       0x18
#define MCP2515_ADDR_REG_RXM0       0x20
#define MCP2515_ADDR_REG_RXM1       0x24

#define MCP2515_ADDR_REG_TXB0CTRL   0x30
#define MCP2515_ADDR_REG_TXB0       0x31
#define MCP2515_ADDR_REG_TXB0DLC    0x35
#define MCP2515_ADDR_REG_TXB0D      0x36

#define MCP2515_ADDR_REG_TXB1CTRL   0x40
#define MCP2515_ADDR_REG_TXB1       0x41
#define MCP2515_ADDR_REG_TXB1DLC    0x45
#define MCP2515_ADDR_REG_TXB1D      0x46

#define MCP2515_ADDR_REG_TXB2CTRL   0x50
#define MCP2515_ADDR_REG_TXB2       0x51
#define MCP2515_ADDR_REG_TXB2DLC    0x55
#define MCP2515_ADDR_REG_TXB2D      0x56

#define MCP2515_ADDR_REG_RXB0CTRL   0x60
#define MCP2515_ADDR_REG_RXB0       0x61
#define MCP2515_ADDR_REG_RXB0DLC    0x65
#define MCP2515_ADDR_REG_RXB0D      0x66

#define MCP2515_ADDR_REG_RXB1CTRL   0x70
#define MCP2515_ADDR_REG_RXB1       0x71
#define MCP2515_ADDR_REG_RXB1DLC    0x75
#define MCP2515_ADDR_REG_RXB1D      0x76

#define MCP2515_ADDR_REG_BFPCTRL    0x0C
#define MCP2515_ADDR_REG_TXRTSCTRL  0x0D
#define MCP2515_ADDR_REG_CANSTAT    0x0E
#define MCP2515_ADDR_REG_CANCTRL    0x0F
#define MCP2515_ADDR_REG_TEC        0x1C
#define MCP2515_ADDR_REG_REC        0x1D
#define MCP2515_ADDR_REG_CNF3       0x28
#define MCP2515_ADDR_REG_CNF2       0x29
#define MCP2515_ADDR_REG_CNF1       0x2A
#define MCP2515_ADDR_REG_CANINTE    0x2B
#define MCP2515_ADDR_REG_CANINTF    0x2C
#define MCP2515_ADDR_REG_EFLG       0x2D

////////////////////////////////// CAN Types //////////////////////////////////

typedef uint8_t MCP2515_ADDRESS;

// Transmit Buffer Register Types
typedef struct
{
   uint8_t txp:2;    //Transmit Buffer Priority bits
   uint8_t u2:1;
   uint8_t txreq:1;  //Message Transmit Request bit
   uint8_t trerr:1;  //Transmission Error Detected bit
   uint8_t mloa:1;   //Message Lost Arbitration bit
   uint8_t abtf:1;   //Message Aborted Flag bit
   uint8_t u7:1;
} TXBiCTRL;

typedef struct
{
   uint8_t b0rtsm:1; //TX0RTS Pin mode bit
   uint8_t b1rtsm:1; //TX1RTS Pin mode bit
   uint8_t b2rtsm:1; //TX2RTS Pin mode bit
   uint8_t b0rts:1;  //TX0RTS Pin State bit
   uint8_t b1rts:1;  //TX1RTS Pin State bit
   uint8_t b2rts:1;  //TX2RTS Pin State bit
   uint8_t u6_7:2;
} TXRTSCTRL;

typedef union
{
   struct
   {
      uint8_t sid3_10;     //Standard Identifier bits 3-10
      uint8_t eid16_17:2;  //Extended Identifier bits 16-18
      uint8_t u10:1;
      uint8_t exide:1;     //Extended Identifier Enable bit
      uint8_t u12:1;
      uint8_t sid0_2:3;    //Standard Identifier bits 0-2
      uint8_t eid8_15;     //Extended Identifier bits 8-15
      uint8_t eid0_7;      //Extended Identifier bits 0-7
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} TXBiID;

typedef struct
{
   uint8_t dlc:4;          //Data Length Code bits
   uint8_t u4_5:2;
   uint8_t rtr:1;          //Remote Transmission Request bit
   uint8_t u7:1;
} TXBiDLC;

// Receive Buffer Register Types
typedef enum
{
   CAN_RX_BUFFER_FILT_ENABLED,
   CAN_RX_BUFFER_FILT_DISABLED=3
} CAN_RX_BUFFER_FILT;

typedef struct
{
   uint8_t filhit0:1;      //Filter Hit bit
   uint8_t bukt1:1;        //Copy of BUKT bit
   uint8_t bukt:1;         //Rollover Enable bit
   uint8_t rxrtr:1;        //Received Remote Transfer Request bit
   uint8_t u4:1;
   uint8_t rxm:2;          //Receive Buffer Operating mode bit
   uint8_t u7:1;
} RXB0CTRL;

typedef struct
{
   uint8_t filhit:3;       //Filter Hit bits
   uint8_t rxrtr:1;        //Received Remote Transfer Request bit
   uint8_t rxm:2;          //Receive Buffer Operating mode bit
   uint8_t u7:1;
} RXB1CTRL;

typedef struct
{
   uint8_t b0bfm:1;        //RX0BF Pin Operating mode bit
   uint8_t b1bfm:1;        //RX1BF Pin Operating mode bit
   uint8_t b0bfe:1;        //RX0BF Pin Function Enable bit
   uint8_t b1bfe:1;        //RX1BF Pin Function Enable bit
   uint8_t b0bfs:1;        //RX0BF Pin State bit
   uint8_t b1bfs:1;        //RX1BF Pin State bit
   uint8_t u6_7:2;
} BFPCTRL;

typedef union
{
   struct
   {
      uint8_t sid3_10;     //Standard Identifier bits 3-10
      uint8_t eid16_17:2;  //Extended Identifier bits 16-18
      uint8_t u10:1;
      uint8_t ide:1;       //Extended Identifier Flag bit
      uint8_t srr:1;       //Standard Frame Remote Transmit Request bit
      uint8_t sid0_2:3;    //Standard Identifier bits 0-2
      uint8_t eid8_15;     //Extended Identifier bits 8-15
      uint8_t eid0_7;      //Extended Identifier bits 0-7
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} RXBiID;
   
typedef struct
{
   uint8_t dlc:4;    //Data Length Code bits
   uint8_t rb0:1;    //Reserved Bit 0
   uint8_t rb1:1;    //Reserved Bit 1
   uint8_t rtr:1;    //Extended Frame Remote Transmission Request bit
   uint8_t u7:1;
} RXBiDLC;

// RX Filter/Mask Register Types
typedef union
{
   struct
   {
      uint8_t sid3_10;     //Standard Identifier bits 3-10
      uint8_t eid16_17:2;  //Extended Identifier bits 16-18
      uint8_t u10:1;
      uint8_t exide:1;     //Extended Identifier Enable bit
      uint8_t u12:1;
      uint8_t sid0_2:3;    //Standard Identifier bits 0-2
      uint8_t eid8_15;     //Extended Identifier bits 8-15
      uint8_t eid0_7;      //Extended Identifier bits 0-7
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} RXFiID;

typedef union
{
   struct
   {
      uint8_t sid3_10;     //Standard Identifier bits 3-10
      uint8_t eid16_17:2;  //Extended Identifier bits 16-18
      uint8_t u10_12:3;
      uint8_t sid0_2:3;    //Standard Identifier bits 0-2
      uint8_t eid8_15;     //Extended Identifier bits 8-15
      uint8_t eid0_7;      //Extended Identifier bits 0-7
   };
   uint8_t b[4];
   uint16_t w[2];
   uint32_t dw;
} RXMiID;

// CAN Baud Rate Register Type
typedef struct
{
   uint8_t phseg2:3;       //PS2 Length bits
   uint8_t u3_5:3;
   uint8_t wakfil:1;       //Wake-up Filter bit
   uint8_t sof:1;          //Start-of-Frame signal bit
   uint8_t prseg:3;        //Propagation Segment Length bits
   uint8_t phseg1:3;       //PS1 Length bits
   uint8_t sam:1;          //Sample Point Configuration bit
   uint8_t btlmode:1;      //PS2 Bit Time Length bit;
   uint8_t brp:6;          //Baud Rate Prescaler bits
   uint8_t sjw:2;          //Synchronization Jump Width Length bits
} CNF;

// CAN Error Register Types
typedef uint8_t TEC;    //Transmit Error Count bits
typedef uint8_t REC;    //Receive Error Count bits

typedef struct
{
   uint8_t ewarn:1;     //Error Warning Flag bit
   uint8_t rxwar:1;     //Receive Error Warning Flag bit
   uint8_t txwar:1;     //Transmit Error Warning Flag bit
   uint8_t rxep:1;      //Receive Error-Passive Flag bit
   uint8_t txep:1;      //Transmit Error-Passive Flag bit
   uint8_t txbo:1;      //Bus-Off Error Flag bit
   uint8_t rx0ovr:1;    //Receive Buffer 0 Overflow Flag bit
   uint8_t rx1ovr:1;    //Receive Buffer 1 Overflow flag bit
} EFLG;

// CAN Interrupt Register Types
typedef struct
{
   uint8_t rx0ie:1;     //Receive Buffer 0 Full Interrupt Enable bit
   uint8_t rx1ie:1;     //Receive Buffer 1 Full Interrupt Enable bit
   uint8_t tx0ie:1;     //Transmit Buffer 0 Empty Interrupt Enable bit
   uint8_t tx1ie:1;     //Transmit Buffer 1 Empty Interrupt Enable bit
   uint8_t tx2ie:1;     //Transmit Buffer 2 Empty Interrupt Enable bit
   uint8_t errie:1;     //Error Interrupt Enable bit
   uint8_t wakie:1;     //Wake-up Interrupt Enable bit
   uint8_t merre:1;     //Message Error Interrupt Enable bit
} CANINTE;

typedef struct
{
   uint8_t rx0if:1;     //Receive Buffer 0 Full Interrupt Flag bit
   uint8_t rx1if:1;     //Receive Buffer 1 Full Interrupt Flag bit
   uint8_t tx0if:1;     //Transmit Buffer 0 Empty Interrupt Flag bit
   uint8_t tx1if:1;     //Transmit Buffer 1 Empty Interrupt Flag bit
   uint8_t tx2if:1;     //Transmit Buffer 2 Empty Interrupt Flag bit
   uint8_t errif:1;     //Error Interrupt Flag bit
   uint8_t wakif:1;     //Wake-up Interrupt Flag bit
   uint8_t merrf:1;     //Message Error Interrupt Flag bit
} CANINTF;

// CAN Control/Status Register Types
typedef struct
{
   uint8_t clkpre:2;    //CLKOUT Pin Prescaler bits
   uint8_t clken:1;     //CLKOUT Pin Enable bit
   uint8_t osm:1;       //One-Shot Mode bit
   uint8_t abat:1;      //Abort All Pending Transmissions bit
   uint8_t reqop:3;     //Request Operation Mode bits
} CANCTRL;

typedef struct
{
   uint8_t u0:1;
   uint8_t icod:3;      //Interrupt Flag Code bits
   uint8_t u4:1;
   uint8_t opmod:3;     //Operation Mode bits
} CANSTAT;

typedef struct
{
   uint8_t rx0if:1;
   uint8_t rx1if:1;
   uint8_t tx0req:1;
   uint8_t tx0if:1;
   uint8_t tx1req:1;
   uint8_t tx1if:1;
   uint8_t tx2req:1;
   uint8_t tx2if:1;
} CANSTATUS;

typedef struct
{
   uint8_t Filter:3;    //0-5 - Filter 0-5, 6 - rollover Filter 0, 7 - rollover Filter 1
   uint8_t Type:2;      //0 - Standard data frame, 1 - Standard remote frame, 2 - Extended data frame, 3 - Extended remote frame
   uint8_t u5:1;
   uint8_t Message:2;   //0 - No RX Messages, 1 - Message in Buffer 0, 2 - Message in Buffer 1, 3 - Message in both buffers
} CANRXSTATUS;


/////////////////////////////// Global Variables //////////////////////////////

CAN_OP_MODE g_CANOpmode = CAN_OP_CONFIG;

////////////////////////////// MCP2515 Prototypes /////////////////////////////

// CAN Prototypes
TXBiID can_set_id(uint32_t Id, int1 ext);
void can_set_id(MCP2515_ADDRESS Address, uint32_t Id, int1 ext);
void can_get_header(CAN_RX_HEADER *Header, RXBiID RxId);

// SPI Prototypes

uint8_t mcp2515_read(MCP2515_ADDRESS Address);
void mcp2515_read_bytes(uint8_t *pDest, MCP2515_ADDRESS Address, uint8_t Count);
uint8_t mcp2515_status(void);
void mcp2515_write(MCP2515_ADDRESS Address, uint8_t Data);
void mcp2515_write_bytes(MCP2515_ADDRESS Address, uint8_t *Data, uint8_t Count);
void mcp2515_command(uint8_t Command);
void mcp2515_init(void);
void mcp2515_bit_modify(MCP2515_ADDRESS Address, uint8_t Mask, uint8_t Data);
uint8_t mcp2515_read_status(void);
uint8_t mcp2515_read_rx_status(void);

////////////////////////////////// Functions //////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// can_init()
//
// Initializes the CAN to the default settings that were defined before this
// driver was included or the default defines set by the driver.
//
// Parameters:
//    Mode - optional parameter specifying the operational mode to put the CAN
//           peripheral in before exiting, defaults to CAN_OP_NORMAL mode.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL)
{
   uint8_t Value;

   mcp2515_init();

   can_set_mode(CAN_OP_CONFIG);                    //must be in config mode before params can be set
   can_set_baud(CAN_CLOCK_SPEED, CAN_BAUD_RATE);
   
   Value = mcp2515_read(MCP2515_ADDR_REG_CANCTRL);
   
   ((CANCTRL)Value).clkpre = MCP2515_CLOCK_OUT_DIVISOR;
   ((CANCTRL)Value).clken = MCP2515_CLKO_ENABLED;
   ((CANCTRL)Value).osm = CAN_TX_RETRANSMISSION_ATTEMPTS;
   
   mcp2515_write(MCP2515_ADDR_REG_CANCTRL, Value);
   
   Value = 0;
   
  #if CAN_USE_FILTERS == FALSE
   ((RXB0CTRL)Value).rxm = CAN_RX_BUFFER_FILT_DISABLED;
  #endif
   
   ((RXB0CTRL)Value).bukt = CAN_USE_RX_DOUBLE_BUFFER;
   
   mcp2515_write(MCP2515_ADDR_REG_RXB0CTRL, Value);
   
   Value = 0;
   
  #if CAN_USE_FILTERS == FALSE
   ((RXB1CTRL)Value).rxm = CAN_RX_BUFFER_FILT_DISABLED;
  #endif
  
   mcp2515_write(MCP2515_ADDR_REG_RXB1CTRL, Value);
  
  #if CAN_USE_FILTERS == TRUE
   can_set_id(MCP2515_ADDR_REG_RXF0, CAN_USER_FILT_0, CAN_USER_FILT_0_TYPE);
   can_set_id(MCP2515_ADDR_REG_RXF1, CAN_USER_FILT_1, CAN_USER_FILT_1_TYPE);
   can_set_id(MCP2515_ADDR_REG_RXF2, CAN_USER_FILT_2, CAN_USER_FILT_2_TYPE);
   can_set_id(MCP2515_ADDR_REG_RXF3, CAN_USER_FILT_3, CAN_USER_FILT_3_TYPE);
   can_set_id(MCP2515_ADDR_REG_RXF4, CAN_USER_FILT_4, CAN_USER_FILT_4_TYPE);
   can_set_id(MCP2515_ADDR_REG_RXF5, CAN_USER_FILT_5, CAN_USER_FILT_5_TYPE);
   
   can_set_id(MCP2515_ADDR_REG_RXM0, CAN_USER_MASK_0, CAN_USER_MASK_0_TYPE);
   can_set_id(MCP2515_ADDR_REG_RXM1, CAN_USER_MASK_1, CAN_USER_MASK_1_TYPE);
  #endif
   
   can_set_mode(Mode);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_baud()
//
// Function for setting the bit rate of the CAN Bus.
//
// Parameters:
//    Clock - the clock speed that is presented to the CAN peripheral, the
//            speed of the external crystal/oscillator connected to the 
//            MCP2515.
// 
//    Rate - the bit rate, max speed is 1 Mbps and the Clock must be evenly
//           divisible by the Rate
//
// Return - can_ec_t error code, see can-mcp2515.h for a description of error
//          codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate)
{
   CNF BaudRate;
   uint8_t BRP = 0;
   uint16_t tBits;
   uint16_t tSeg1Bits;
   
   if((Clock % Rate) != 0)
      return(CAN_EC_BAUD_NOT_DIVISIBLE);
   
   do
   {
      BRP++;
      tBits = (Clock / (BRP * 2)) / Rate;
      
      if(tBits < 26)
      {
         if((Clock / (tBits * (BRP * 2))) != Rate)
            continue;
      }
      
      tSeg1Bits = (((uint32_t)tBits * CAN_DEFAULT_SAMPLE_POINT) / 1000) - 1;
   } while((((tBits > 25) || (tBits < 8)) && (BRP < 64)) || ((BRP < 64) && (tSeg1Bits > 16)));
   
   if((tBits > 25) || (tBits < 8))
      return(CAN_EC_BAUD_INVALID);
   
   BaudRate.brp = BRP - 1;
   
   if(tSeg1Bits > 17)
   {
      BaudRate.prseg = 7;
      BaudRate.phseg1 = 7;
      
      tSeg1Bits = 17;
   }
   else
   {
      while((tSeg1Bits < (tBits - tSeg1Bits - 1)) || (tSeg1Bits < 2))
         tSeg1Bits++;
   
      BaudRate.prseg = (tSeg1Bits / 2) - 1;
      BaudRate.phseg1 = (tSeg1Bits - (tSeg1Bits / 2))  - 1;
   }
   
   BaudRate.phseg2 = (tBits - tSeg1Bits) - 2;
   
   BaudRate.sam = CAN_BRG_SAM;
   BaudRate.btlmode = 1;   //Phase Segment 2 Freely programmable
   BaudRate.wakfil = CAN_BRG_WAKE_FILTER;
   BaudRate.sof = MCP2515_CLKO_TYPE;
   
   if(BaudRate.phseg2 == 0)
      BaudRate.sjw = 0;
   else if(BaudRate.phseg2 < 4)
      BaudRate.sjw = BaudRate.phseg2 - 1;
   else
      BaudRate.sjw = 3;
   
   can_set_mode(CAN_OP_CONFIG, FALSE);
   
   mcp2515_write_bytes(MCP2515_ADDR_REG_CNF3, (uint8_t *)&BaudRate, 3);
   
   can_set_mode(g_CANOpmode, FALSE);
   
   return(CAN_EC_OK);
}

///////////////////////////////////////////////////////////////////////////////
// can_set_mode()
//
// Set the operational mode of the CAN peripheral.
//
// Parameters:
//    Mode - the CAN_OP_MODE to change to, can be one of the following:
//             CAN_OP_NORMAL
//             CAN_OP_DISABLE
//             CAN_OP_LOOPBACK
//             CAN_OP_LISTEN
//             CAN_OP_CONFIG
//
//    Update - optional specifies whether to update the global operation mode
//             variable, TRUE update or FALSE don't update.  Default is TRUE if
//             not specified.  This is useful for allowing the option to put it
//             in a different operating mode for a little while and then
//             returning it back to the previous operating mode.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE)
{
   uint8_t Value;
   
   Value = mcp2515_read(MCP2515_ADDR_REG_CANSTAT);
   
   if(((CANSTAT)Value).opmod != Mode)
   {
      Value = mcp2515_read(MCP2515_ADDR_REG_CANCTRL);
      
      ((CANCTRL)Value).reqop = Mode;
   
      mcp2515_write(MCP2515_ADDR_REG_CANCTRL, Value);
   
      do 
      {
         Value = mcp2515_read(MCP2515_ADDR_REG_CANSTAT);
         
      } while (((CANSTAT)Value).opmod != Mode);
   }
   
   if(Update)
      g_CANOpmode = Mode;
}

///////////////////////////////////////////////////////////////////////////////
// can_set_filter_id()
//
// Used to set the specified Filter Id and Type.
//
// Parameters:
//    Filter - the filter to setup.
//
//    Id - the Id to assign to the filter.
//
//    Type - the filter type.  This specifies if the Id is a SID or EID, which
//            controls how the Id is written to the Filter's registers.
//            Additionally it sets the filter to receive only SID or EID
//            messages.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type)
{
   MCP2515_ADDRESS Address;
   
   if(Filter <= CAN_FILTER_5)
   {
      if(Filter < CAN_FILTER_2)
         Address = MCP2515_ADDR_REG_RXB0CTRL;
      else
         Address = MCP2515_ADDR_REG_RXB1CTRL;
      
      //Make sure Buffer is setup to use Filters
      mcp2515_bit_modify(Address, 0x60, 0);
      
      switch(Filter)
      {
         case CAN_FILTER_0:
            Address = MCP2515_ADDR_REG_RXF0;
            break;
         case CAN_FILTER_1:
            Address = MCP2515_ADDR_REG_RXF1;
            break;
         case CAN_FILTER_2:
            Address = MCP2515_ADDR_REG_RXF2;
            break;
         case CAN_FILTER_3:
            Address = MCP2515_ADDR_REG_RXF3;
            break;
         case CAN_FILTER_4:
            Address = MCP2515_ADDR_REG_RXF4;
            break;
         case CAN_FILTER_5:
            Address = MCP2515_ADDR_REG_RXF5;
            break;;
      }
      
      can_set_id(Address, Id, Type);
   }
}

///////////////////////////////////////////////////////////////////////////////
// can_set_mask_id()
//
// Used to set the specified Filter Mask's Id and Type.
//
// Parameters:
//    Mask - the filter mask to setup.
//
//    Id - the Id to assign to the filter mask, setting a bit means the
//         corresponding bit of the received message Id must match corresponding
//         bit of the Filter's Id.
//
//    Type - the filter mask Id type.  This just specifies if the Id is a SID
//           or EID, which controls how the Id is written to the Filter Mask's
//           registers.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_TYPE Type)
{
   MCP2515_ADDRESS Address;
   
   switch(Mask)
   {
      case CAN_FILTER_MASK_0:
         Address = MCP2515_ADDR_REG_RXM0;
         break;
      case CAN_FILTER_MASK_1:
         Address = MCP2515_ADDR_REG_RXM1;
         break;
      default:
         return;
   }
   
   can_set_id(Address, Id, Type);
}

///////////////////////////////////////////////////////////////////////////////
// can_putd()
//
// Function used to load a message into a transmit buffer, after which when the
// CAN Bus becomes available it will be transmitted.
//
// Parameters:
//    Header - pointer to CAN_TX_HEADER variable containing info about the
//             message to be transmitted, e.g. the message ID.  See
//             CAN_TX_HEADER type in can-mcp2515.h for more info.
//
//    Data - pointer to uint8_t array containing data to transmit.
//
// Returns - can_ec_t error code, see can-mcp2515.h for a description of error
//           codes.
////////////////////////////////////////////////////////////////////////////////
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data)
{
   uint8_t Value;
   MCP2515_ADDRESS Address;
   uint8_t Length;
   uint8_t i;
   union
   {
      TXBiID Id;
      uint8_t b[4];
   } TxId;
   uint8_t Buffer;
   
   Length = Header->Length;
   
   Value = mcp2515_read_status();
   
   if(!((CANSTATUS)Value).tx0req)
   {
      Address = MCP2515_ADDR_REG_TXB0CTRL;
      Buffer = 0;
   }
   else if(!((CANSTATUS)Value).tx1req)
   {
      Address = MCP2515_ADDR_REG_TXB1CTRL;
      Buffer = 2;
   }
   else if(!((CANSTATUS)Value).tx2req)
   {
      Address = MCP2515_ADDR_REG_TXB2CTRL;
      Buffer = 4;
   }
   else
      return(CAN_EC_BUFFER_TX_FULL);
   
   //Load TxBuffer
   TxId.Id = can_set_id(Header->Id, Header->ext);
   
   ((TXBiDLC)Value).dlc = Length;
   ((TXBiDLC)Value).rtr = Header->rtr;
   
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_LOAD_TX_BUFFER | Buffer);
   
   for(i=0;i<4;i++)
      MCP2515Xfer(TxId.b[i]);
      
   MCP2515Xfer(Value);
   
   if((Length > 0) && (Header->rtr == FALSE))
   {
      for(i=0;i<Length;i++)
         MCP2515Xfer(Data[i]);
   }
   
   output_high(MCP2515_SPI_CS_PIN);
   
   Value = mcp2515_read(Address);
   
   ((TXBiCTRL)Value).txp = Header->Priority;
   ((TXBiCTRL)Value).txreq = TRUE;
   
   mcp2515_write(Address, Value);
   
   if(Buffer == 0)
      Value = 0x04;
   else if(Buffer == 2)
      Value = 0x08;
   else
      Value = 0x10;
   
   mcp2515_bit_modify(MCP2515_ADDR_REG_CANINTF, Value, 0);
   
   return(CAN_EC_OK);
}

///////////////////////////////////////////////////////////////////////////////
// can_getd()
//
// Function used to get message from a RX Buffer.
//
// Parameters:
//    Header - pointer to CAN_RX_HEADER variable to store info about the
//             retrieved message, e.g. the message ID.  See CAN_RX_HEADER type
//             in can_mcp2515.h for more info.
//
//    Data - pointer to an uint8_t array to return data portion of message to.
//
// Returns - can_ec_t error code, see can-mcp2515.h for a description of error
//           codes.
///////////////////////////////////////////////////////////////////////////////
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data)
{
   union
   {
      uint8_t b[4];
      uint32_t dw;
      RXBiID Id;
   } RxId;
   uint8_t Value;
   uint8_t Buffer;
   uint8_t i;
   
   Value = mcp2515_read_rx_status();
   
   if(((CANRXSTATUS)Value).Message != 0)
   {
      if(bit_test(Value, 6))
         Buffer = 0x00;
      else
         Buffer = 0x04;
   }
   else
      return(CAN_EC_BUFFER_RX_EMPTY);
   
   if(((CANRXSTATUS)Value).Filter < 6)
      Header->Filter = ((CANRXSTATUS)Value).Filter;
   else
      Header->Filter = ((CANRXSTATUS)Value).Filter - 6;
   
   if(bit_test(Value, 4))
      Header->ext = TRUE;
   else
      Header->ext = FALSE;
   
   if(bit_test(Value, 3))
      Header->rtr = TRUE;
   else
      Header->rtr = FALSE;
   
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ_RX_BUFFER | Buffer);
   
   for(i=0;i<4;i++)
      RxId.b[i] = MCP2515Xfer(0);
   
   Value = MCP2515Xfer(0) & 0x0F;
   Header->Length = Value;
   
   for(i=0;i<Value;i++)
      Data[i] = MCP2515Xfer(0);
   
   output_high(MCP2515_SPI_CS_PIN);
   
   can_get_header(Header, RxId.Id);
   
   Value = mcp2515_read(MCP2515_ADDR_REG_EFLG);
   
   if(Buffer == 0x00)
   {
      Header->err_ovfl = ((EFLG)Value).rx0ovr;
      Value = 0x40;
   }
   else
   {
      Header->err_ovfl = ((EFLG)Value).rx1ovr;
      Value = 0x80;
   }
   
   mcp2515_bit_modify(MCP2515_ADDR_REG_EFLG, Value, 0);
   
   return(CAN_EC_OK);
}

///////////////////////////////////////////////////////////////////////////////
// can_kbhit()
//
// Used to determine if there is at least one message to retrieve from the 
// RX buffers.
//
// Parameters - None.
//
// Returns:
//    TRUE - if a buffer has a new message to retrieve.
//    FALSE - if no buffers have a new message to retrieve.
///////////////////////////////////////////////////////////////////////////////
int1 can_kbhit(void)
{
   uint8_t Value;
   
   Value = mcp2515_read_status();
   
   if(((CANSTATUS)Value).rx0if || ((CANSTATUS)Value).rx1if)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_tbe()
//
// Used to determine if at least one TX buffer can accept a new message to 
// transmit.
//
// Parameters - None.
//
// Returns:
//    TRUE - at least one TX buffer can accept a new message.
//    FALSE - all TX buffers are full.
///////////////////////////////////////////////////////////////////////////////
int1 can_tbe(void)
{
   uint8_t Value;
   
   Value = mcp2515_read_status();
   
   if(!((CANSTATUS)Value).tx0req || !((CANSTATUS)Value).tx1req || !((CANSTATUS)Value).tx2req)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_tx_is_empty()
//
// Used to determine if all TX buffers are empty.
//
// Parameters - None.
//
// Returns:
//    TRUE - all TX buffer are empty.
//    FALSE - at least one TX buffer is not empty.
///////////////////////////////////////////////////////////////////////////////
int1 can_tx_is_empty(void)
{
   uint8_t Value;
   
   Value = mcp2515_read_status();
   
   if(!((CANSTATUS)Value).tx0req && !((CANSTATUS)Value).tx1req && !((CANSTATUS)Value).tx2req)
      return(TRUE);
   else
      return(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// can_abort()
//
// Used to abort all pending transmissions.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_abort(void)
{
   uint8_t Value;

   //Set ABAT bit
   mcp2515_bit_modify(MCP2515_ADDR_REG_CANCTRL, 0x10, 0x10);
   
   //Wait for TXREQ bits to be cleared
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ_STATUS);
   
   do
   {
      Value = MCP2515Xfer(0);
   } while(((CANSTATUS)Value).tx0req || ((CANSTATUS)Value).tx1req || ((CANSTATUS)Value).tx2req);
   
   output_high(MCP2515_SPI_CS_PIN);
   
   //Clear ABAT bit
   mcp2515_bit_modify(MCP2515_ADDR_REG_CANCTRL, 0x10, 0);
}

///////////////////////////////////////////////////////////////////////////////
// can_enable_interrupts()
//
//  Used to enable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to enable, can be the following:
//       CAN_INTERRUPT_RX0
//       CAN_INTERRUPT_RX1
//       CAN_INTERRUPT_TX0
//       CAN_INTERRUPT_TX1
//       CAN_INTERRUPT_TX2
//       CAN_INTERRUPT_ERR
//       CAN_INTERRUPT_WAKE
//       CAN_INTERRUPT_INVALID
//
//    You can or these together (via |) to enable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_enable_interrupts(CAN_INTERRUPT Setting)
{
   mcp2515_bit_modify(MCP2515_ADDR_REG_CANINTE, Setting, 0xFF);
}

///////////////////////////////////////////////////////////////////////////////
// can_disable_interrupts()
//
// Used to disable one or more of the CAN Interrupts.
//
// Parameters:
//    Setting - the interrupt or interrupts to disable, can be the following:
//       CAN_INTERRUPT_RX0
//       CAN_INTERRUPT_RX1
//       CAN_INTERRUPT_TX0
//       CAN_INTERRUPT_TX1
//       CAN_INTERRUPT_TX2
//       CAN_INTERRUPT_ERR
//       CAN_INTERRUPT_WAKE
//       CAN_INTERRUPT_INVALID
//
//    You can or these together (via |) to disable more than interrupt.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_disable_interrupts(CAN_INTERRUPT Setting)
{
   mcp2515_bit_modify(MCP2515_ADDR_REG_CANINTE, Setting, 0);
}

///////////////////////////////////////////////////////////////////////////////
// can_interrupts_active()
//
// Used to determined which CAN Interrupts are active.  
//
// Parameters - None.
//
// Returns - CAN_INTERRUPT type.  If an interrupt is active the corresponding
//           bit will be set, set CAN_INTERRUPT type in can-mcp2515.h to
//           determine which bits correspond to which interrupt.
///////////////////////////////////////////////////////////////////////////////
CAN_INTERRUPT can_interrupts_active(void)
{
   return(mcp2515_read(MCP2515_ADDR_REG_CANINTF));
}

///////////////////////////////////////////////////////////////////////////////
// can_interrupt_active()
//
// Used to determine if specified CAN Interrupt is active. 
//
// Parameters:
//    Interrupt - the CAN_INTERRUPT type to test if active.
//
// Returns:
//    TRUE - if active.
//    FALSE - if not active.
///////////////////////////////////////////////////////////////////////////////
int1 can_interrupt_active(CAN_INTERRUPT Interrupt)
{
   return((can_interrupts_active() & Interrupt) != 0);
}

///////////////////////////////////////////////////////////////////////////////
// can_clear_interrupt()
//
// Used to clear specified CAN Interrupt flag.
//
// Parameters:
//    Interrupt - the CAN_INTERRUPT type to clear the flag for.  Multiple 
//                interrupts can be cleared by or'ing the interrupts together.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void can_clear_interrupt(CAN_INTERRUPT Interrupt)
{
   mcp2515_bit_modify(MCP2515_ADDR_REG_CANINTF, Interrupt, 0);
}

///////////////////////////////////////////////////////////////////////////////

TXBiID can_set_id(uint32_t Id, int1 ext)
{
   TXBiID Value;
   
   if(ext)
   {
      Value.sid3_10 = (make16(make8(Id, 3), make8(Id, 2)) >> 5);
      Value.sid0_2 = make8(Id, 2) >> 2;
      
      Value.eid16_17 = make8(Id, 2);
      Value.eid8_15 = make8(Id, 1);
      Value.eid0_7 = make8(Id, 0);
      
      Value.exide = 1;
   }
   else
   {
      Value.dw = 0;
      
      Value.sid3_10 = make16(make8(Id, 1), make8(Id, 0)) >> 3;
      Value.sid0_2 = make8(Id, 0);
   }
   
   return(Value);
}

void can_set_id(MCP2515_ADDRESS Address, uint32_t Id, int1 ext)
{
   TXBiID Value;
   
   Value = can_set_id(Id, ext);

   mcp2515_write_bytes(Address, (uint8_t *)&Value, 4);
}

void can_get_header(CAN_RX_HEADER *Header, RXBiID RxId)
{
   union
   {
      uint8_t b[4];
      uint32_t dw;
   } Id;

   if(RxId.ide)
   {
      Id.b[0] = RxId.eid0_7;
      Id.b[1] = RxId.eid8_15;
      Id.b[2] = RxId.eid16_17 | (RxId.sid0_2 << 2) | (RxId.sid3_10 << 5);
      Id.b[3] = (RxId.sid3_10 >> 3) & 0x1F;
      
      Header->Id = Id.dw;
      Header->ext = TRUE;
   }
   else
   {
      Id.dw = 0;
      
      Id.b[0] = RxId.sid0_2 | (RxId.sid3_10 << 3);
      Id.b[1] = (RxId.sid3_10 >> 5) & 0x07;
      
      Header->Id = Id.dw;
      Header->ext = FALSE;
   }
}

/////////////////////////////////// SPI Code //////////////////////////////////

uint8_t mcp2515_read(MCP2515_ADDRESS Address)
{
   uint8_t  Result;

   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ);
   MCP2515Xfer(Address);
   Result = MCP2515Xfer(0);
   
   output_high(MCP2515_SPI_CS_PIN);

   return(Result);
}

void mcp2515_read_bytes(uint8_t *pDest, MCP2515_ADDRESS Address, uint8_t Count)
{
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ);
   MCP2515Xfer(Address);
   
   while(Count--)
   {
      *pDest = MCP2515Xfer(0);
      pDest++;
   }
   
   output_high(MCP2515_SPI_CS_PIN);
}

uint8_t mcp2515_status(void)
{
   uint8_t Status;
   
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ_STATUS);
   Status = MCP2515Xfer(0);
   MCP2515Xfer(0);
   
   output_high(MCP2515_SPI_CS_PIN);

   return(Status);
}

void mcp2515_write(MCP2515_ADDRESS Address, uint8_t Data)
{
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_WRITE);
   MCP2515Xfer(Address);
   MCP2515Xfer(Data);
   
   output_high(MCP2515_SPI_CS_PIN);
}

void mcp2515_write_bytes(MCP2515_ADDRESS Address, uint8_t *Data, uint8_t Count)
{
   uint8_t i;
   
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_WRITE);
   MCP2515Xfer(Address);
   
   for(i=0;i<Count;i++)
      MCP2515Xfer(Data[i]);
  
   output_high(MCP2515_SPI_CS_PIN);
}

void mcp2515_command(uint8_t Command)
{
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(Command);
   
   output_high(MCP2515_SPI_CS_PIN);
}

void mcp2515_init(void)
{
   uint8_t Value;
   
   output_high(MCP2515_SPI_CS_PIN);
   
  #ifdef MCP2515_INT_PIN
   output_float(MCP2515_INT_PIN);
  #endif
  
  #ifdef MCP2515_RX0BF_PIN
   output_float(MCP2515_RX0BF_PIN);
  #endif
  
  #ifdef MCP2515_RX1BF_PIN
   output_float(MCP2515_RX1BF_PIN);
  #endif

  #ifdef MCP2515_TX0RTS_PIN
   output_high(MCP2515_TX0RTS_PIN);
  #endif
  #ifdef MCP2515_TX1RTS_PIN
   output_high(MCP2515_TX1RTS_PIN);
  #endif
  #ifdef MCP2515_TX2RTS_PIN
   output_high(MCP2515_TX2RTS_PIN);
  #endif

  #ifdef MCP2515_RESET_PIN
   output_high(MCP2515_RESET_PIN);
   output_low(MCP2515_RESET_PIN);
   output_high(MCP2515_RESET_PIN);
  #else
   mcp2515_command(MCP2515_CMD_RESET);
  #endif
   
   delay_ms(5);
  
   Value = 0;
  
  #ifdef MCP2515_TX0RTS_PIN
   ((TXRTSCTRL)Value).b0rtsm = 1;
  #endif
  #ifdef MCP2515_TX1RTS_PIN
   ((TXRTSCTRL)Value).b1rtsm = 1;
  #endif
  #ifdef MCP2515_TX2RTS_PIN
   ((TXRTSCTRL)Value).b2rtsm = 1;
  #endif
   
   mcp2515_write(MCP2515_ADDR_REG_TXRTSCTRL, Value);
   
   Value = 0;
   
 #ifdef MCP2515_RX0BF_PIN
   ((BFPCTRL)Value).b0bfm = 1;
   ((BFPCTRL)Value).b0bfe = TRUE;
 #else
   ((BFPCTRL)Value).b0bfe = MCP2515_RX0_OUTPUT;
  #if MCP2515_RX0_OUTPUT == TRUE
   ((BFPCTRL)Value).b0bfs = MCP2515_RX0_DEFAULT_LEVEL;
  #endif
 #endif
 
 #ifdef MCP2515_RX1BF_PIN
   ((BFPCTRL)Value).b1bfm = 1;
   ((BFPCTRL)Value).b1bfe = TRUE;
 #else
   ((BFPCTRL)Value).b1bfe = MCP2515_RX1_OUTPUT;
  #if MCP2515_RX1_OUTPUT == TRUE
   ((BFPCTRL)Value).b1bfs = MCP2515_RX1_DEFAULT_LEVEL;
  #endif
 #endif
 
   mcp2515_write(MCP2515_ADDR_REG_TXRTSCTRL, Value);
}

void mcp2515_bit_modify(MCP2515_ADDRESS Address, uint8_t Mask, uint8_t Data)
{
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_BIT_MODIFY);
   MCP2515Xfer(Address);
   MCP2515Xfer(Mask);
   MCP2515Xfer(Data);
   
   output_high(MCP2515_SPI_CS_PIN);
}

uint8_t mcp2515_read_status(void)
{
   uint8_t Result;

   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ_STATUS);
   Result = MCP2515Xfer(0);
   
   output_high(MCP2515_SPI_CS_PIN);
   
   return(Result);
}

uint8_t mcp2515_read_rx_status(void)
{
   uint8_t Result;
   
   output_low(MCP2515_SPI_CS_PIN);
   
   MCP2515Xfer(MCP2515_CMD_READ_RX_STATUS);
   Result = MCP2515Xfer(0);
   
   output_high(MCP2515_SPI_CS_PIN);
   
   return(Result);
}
