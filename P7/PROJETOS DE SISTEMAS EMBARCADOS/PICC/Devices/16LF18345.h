//////////// Standard Header file for the PIC16LF18345 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16LF18345

#nolist
//////// Program memory: 8192x14  Data RAM: 1008  Stack: 16
//////// I/O: 18   Analog Pins: 17
//////// Data EEPROM: 256
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: LP,XT,HS,NOEXTOSC,ECL,ECM,ECH,RSTOSC_HFINTRC_PLL
//////// Fuses: RSTOSC_EXT_PLL,RSTOSC_SOSC,RSTOSC_LFINTRC,RSTOSC_HFINTRC
//////// Fuses: RSTOSC_EXT,CLKOUT,NOCLKOUT,CKS,NOCKS,FCMEN,NOFCMEN,MCLR
//////// Fuses: NOMCLR,PUT,NOPUT,NOWDT,WDT_SW,WDT_NOSL,WDT,LPBOR,NOLPBOR
//////// Fuses: NOBROWNOUT,BROWNOUT_SW,BROWNOUT_NOSL,BROWNOUT,BORV27,BORV24
//////// Fuses: PPS1WAY,NOPPS1WAY,STVREN,NOSTVREN,DEBUG,NODEBUG,WRT,WRT_400
//////// Fuses: WRT_200,NOWRT,LVP,NOLVP,PROTECT,NOPROTECT,CPD,NOCPD
//////// 
#if (!defined(__PCM__)||defined(__ISNT_CCS__))
#define _bif
#define int8 char
#define int16 long
#define int32 long long
#define float32 float
#define int1 char

#endif
////////////////////////////////////////////////////////////////// PIN_SELECT
// #pin_select function=pin
// Valid Pins: 
//    PIN_A0,PIN_A1,PIN_A2,PIN_A3,PIN_A4,PIN_A5,PIN_B4,PIN_B5,PIN_B6,PIN_B7,
//    PIN_C0,PIN_C1,PIN_C2,PIN_C3,PIN_C4,PIN_C5,PIN_C6,PIN_C7
// Input Functions: 
//    INT0,T0CK,T1CK,T1G,CCP1,CCP2,CCP3,CCP4,CWG1IN,CWG2IN,MDCIN1,MDCIN2,MDMIN,
//    SCK2IN,SCL2IN,SDI2,SDA2IN,SS2IN,SCK1IN,SCL1IN,SDI1,SDA1IN,SS1IN,U1RX,
//    U1CKIN,CLCIN0,CLCIN1,CLCIN2,CLCIN3,T3CK,T3G,T5CK,T5G,T0CKI,T1CKI,CCP1IN,
//    CCP2IN,CCP3IN,CCP4IN,T3CKI,T5CKI,RX1
// Output Functions: 
//    NULL,PWM5OUT,PWM6OUT,CLC1OUT,CLC2OUT,CLC3OUT,CLC4OUT,CWG1OUTA,CWG1OUTB,
//    CWG1OUTC,CWG1OUTD,CCP1OUT,CCP2OUT,CCP3OUT,CCP4OUT,CWG2OUTA,CWG2OUTB,
//    CWG2OUTC,CWG3OUTD,U1TX,U1DT,C1OUT,C2OUT,SCK1OUT,SCL1OUT,SDO1,SDA1OUT,
//    SCK2OUT,SCL2OUT,SDO2,SDA2OUT,T0OUT,NCO1OUT,CLKROUT,DSMOUT,PWM5,PWM6,TX1,
//    DT1,SCK1,SCL1,SDA1,SCK2,SCL2,SDA2,NCO1
//

////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_x_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT(),
// Discrete I/O Prototypes:
_bif void set_tris_a(int8 value);  
_bif void set_tris_b(int8 value);  
_bif void set_tris_c(int8 value);  
_bif void set_input_level_a(int8 value);  
_bif void set_input_level_b(int8 value);  
_bif void set_input_level_c(int8 value);  
_bif void set_open_drain_a(int8 value);  
_bif void set_open_drain_b(int8 value);  
_bif void set_open_drain_c(int8 value);  
_bif int8 get_tris_a(void);  
_bif int8 get_tris_b(void);  
_bif int8 get_tris_c(void);  
_bif void output_a(int8 value);  
_bif void output_b(int8 value);  
_bif void output_c(int8 value);  
_bif int8 input_a(void);  
_bif int8 input_b(void);  
_bif int8 input_c(void);  
_bif int8 input_change_a(void);  
_bif int8 input_change_b(void);  
_bif int8 input_change_c(void);  
_bif void port_a_pullups(int8 upmask);   
_bif void port_b_pullups(int8 upmask);  
_bif void port_c_pullups(int8 upmask);   
_bif void set_slow_slew_a(int8 value); 
_bif void set_slow_slew_b(int8 value); 
_bif void set_slow_slew_c(int8 value); 
_bif int1 input(int16 pin); 
_bif int1 input_state(int16 pin); 
_bif void output_low(int16 pin); 
_bif void output_high(int16 pin); 
_bif void output_toggle(int16 pin); 
_bif void output_bit(int16 pin, int1 level); 
_bif void output_float(int16 pin); 
_bif void output_drive(int16 pin); 

// Constants used to identify pins in the above are:

#define PIN_A0  96
#define PIN_A1  97
#define PIN_A2  98
#define PIN_A3  99
#define PIN_A4  100
#define PIN_A5  101

#define PIN_B4  108
#define PIN_B5  109
#define PIN_B6  110
#define PIN_B7  111

#define PIN_C0  112
#define PIN_C1  113
#define PIN_C2  114
#define PIN_C3  115
#define PIN_C4  116
#define PIN_C5  117
#define PIN_C6  118
#define PIN_C7  119

////////////////////////////////////////////////////////////////// Useful defines
#define FALSE 0
#define TRUE 1

#define BYTE int8
#define BOOLEAN int1

#define getc getch
#define fgetc getch
#define getchar getch
#define putc putchar
#define fputc putchar
#define fgets gets
#define fputs puts

////////////////////////////////////////////////////////////////// Control
// Control Functions:  RESET_CPU(), SLEEP(), RESTART_CAUSE()
// Prototypes:
_bif int8 restart_cause(void);       
_bif void reset_cpu(void);
_bif void sleep(void);
// Constants returned from RESTART_CAUSE() are:
#define NORMAL_POWER_UP      0x3C       
#define MCLR_FROM_POWER_UP   0x34       
#define BROWNOUT_RESTART     0x3E       
#define MCLR_FROM_SLEEP      0x27       
#define WDT_TIMEOUT          0x1F       
#define WDT_FROM_SLEEP       0x0F       
#define INTERRUPT_FROM_SLEEP 0x2F       
#define MCLR_FROM_RUN        0x37       
#define RESET_INSTRUCTION    0x3B       
#define STACK_OVERFLOW       0xBF       
#define STACK_UNDERFLOW      0x7F       

////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER_0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC(),
//                              SET_TIMER0_PERIOD()
// Timer 0 Prototypes:
_bif void setup_timer_0(int16 mode);
_bif void setup_timer_0(int16 mode, int8 postscale);
_bif void setup_timer_0(int16 mode, int8 period, int8 postscale); //8-bit mode only
_bif void set_timer0(int8 value);
_bif void set_timer0(int16 value);
_bif int8 get_timer0(void);
_bif int16 get_timer0(void);
_bif void setup_counters(int8 mode, int8 prescaler);
_bif void set_rtcc(int8 value);
_bif void set_rtcc(int16 value);
_bif int8 get_rtcc(void);
_bif int16 get_rtcc(void);
// Constants used for SETUP_TIMER_0() first parameter are:
#define T0_INTERNAL               0x8040
#define T0_HFINTOSC               0x8060
#define T0_LFINTOSC               0x8080
#define T0_SOSC                   0x80C0  
#define T0_CLC1                   0x80E0  
#define T0_EXT_L_TO_H             0x8000
#define T0_EXT_H_TO_L             0x8020
#define T0_OFF                    0
// One of the following may be OR'ed in with the above using |
#define T0_INPUT_SYNCRONIZED      0x0000
#define T0_INPUT_NOT_SYNCRONIZED  0x0010
// One of the following may be OR'ed in with the above using |
#define T0_DIV_1                  0x0000
#define T0_DIV_2                  0x0001
#define T0_DIV_4                  0x0002
#define T0_DIV_8                  0x0003
#define T0_DIV_16                 0x0004
#define T0_DIV_32                 0x0005
#define T0_DIV_64                 0x0006
#define T0_DIV_128                0x0007
#define T0_DIV_256                0x0008
#define T0_DIV_512                0x0009
#define T0_DIV_1024               0x000A
#define T0_DIV_2048               0x000B
#define T0_DIV_4096               0x000C
#define T0_DIV_8192               0x000D
#define T0_DIV_16384              0x000E
#define T0_DIV_32768              0x000F
// One of the following may be OR'ed in with the above using |
#define T0_8_BIT                  0x1000
#define T0_16_BIT                 0x0000

// The following are provided for compatibility
// with older compiler versions
// Constants used for SETUP_COUNTERS() first parameter are:
#define RTCC_INTERNAL             0x8040 
#define RTCC_EXT_L_TO_H           0x8000
#define RTCC_EXT_H_TO_L           0x8020
#define RTCC_OFF                  0x0000
#define RTCC_8_BIT                0x1000
// Constants used for SETUP_COUNTERS() second parameter are:
#define RTCC_DIV_1                0x0000
#define RTCC_DIV_2                0x0001
#define RTCC_DIV_4                0x0002
#define RTCC_DIV_8                0x0003
#define RTCC_DIV_16               0x0004
#define RTCC_DIV_32               0x0005
#define RTCC_DIV_64               0x0006
#define RTCC_DIV_128              0x0007
#define RTCC_DIV_256              0x0008

////////////////////////////////////////////////////////////////// WDT
// Watch Dog Timer Functions: SETUP_WDT(), RESTART_WDT()
// WDT base is 1ms
// WDT Prototypes:
_bif void setup_wdt(int16 mode);
_bif void restart_wdt(void);
// Constants used for SETUP_WDT() are:
#define WDT_ON      0x1000
#define WDT_OFF     0x2000

#define WDT_1MS     0x100
#define WDT_2MS     0x101
#define WDT_4MS     0x102
#define WDT_8MS     0x103
#define WDT_16MS    0x104
#define WDT_32MS    0x105
#define WDT_64MS    0x106
#define WDT_128MS   0x107
#define WDT_256MS   0x108
#define WDT_512MS   0x109
#define WDT_1S      0x10A
#define WDT_2S      0x10B
#define WDT_4S      0x10C
#define WDT_8S      0x10D
#define WDT_16S     0x10E
#define WDT_32S     0x10F
#define WDT_64S     0x110
#define WDT_128S    0x111
#define WDT_256S    0x112

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Timer 1 Prototypes:
_bif void setup_timer_1(int16 mode); 
_bif int16 get_timer1(void);
_bif void set_timer1(int16 value);
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_INTERNAL         0x07     
#define T1_EXTERNAL         0x87     
#define T1_EXTERNAL_SYNC    0x83     
#define T1_FOSC             0x47     
#define T1_LFINTRC          0xC7  

#define T1_ENABLE_SOSC      0x08     

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE             0x8000
#define T1_GATE_INVERTED    0xC000
#define T1_GATE_TOGGLE      0xA000
#define T1_GATE_SINGLE      0x9000
#define T1_GATE_TIMER0      0x8100  
#define T1_GATE_COMP1       0x8200     
#define T1_GATE_COMP2       0x8300      

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Timer 2 Prototypes:
_bif void setup_timer_2(int8 mode, int8 period, int8 postscale); 
_bif int8 get_timer2(void);
_bif void set_timer2(int8 value);
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6 
#define T2_DIV_BY_64        7 

////////////////////////////////////////////////////////////////// Timer 3
// Timer 3 Functions: SETUP_TIMER_3, GET_TIMER3, SET_TIMER3
// Timer 3 Prototypes:
_bif void setup_timer_3(int16 mode); 
_bif int16 get_timer3(void);
_bif void set_timer3(int16 value);
// Constants used for SETUP_TIMER_3() are:
//      (or (via |) together constants from each group)
#define T3_DISABLED         0
#define T3_INTERNAL         0x07  
#define T3_EXTERNAL         0x87   
#define T3_EXTERNAL_SYNC    0x83   
#define T3_FOSC             0x47  
#define T3_LFINTRC          0xC7  

#define T3_ENABLE_SOSC      0x08   

#define T3_DIV_BY_1         0
#define T3_DIV_BY_2         0x10
#define T3_DIV_BY_4         0x20
#define T3_DIV_BY_8         0x30

#define T3_GATE             0x8000
#define T3_GATE_INVERTED    0xC000
#define T3_GATE_TOGGLE      0xA000
#define T3_GATE_SINGLE      0x9000
#define T3_GATE_TIMER0      0x8100 
#define T3_GATE_COMP1       0x8200  
#define T3_GATE_COMP2       0x8300  

////////////////////////////////////////////////////////////////// Timer 4
// Timer 4 Functions: SETUP_TIMER_4, GET_TIMER4, SET_TIMER4
// Timer 4 Prototypes:
_bif void setup_timer_4(int16 mode,int8 period, int8 postscaler);
_bif int8 get_timer4(void);
_bif void set_timer4(int8 value);
// Constants used for SETUP_TIMER_4() are:
#define T4_DISABLED         0
#define T4_DIV_BY_1         4
#define T4_DIV_BY_4         5
#define T4_DIV_BY_16        6
#define T4_DIV_BY_64        7 

////////////////////////////////////////////////////////////////// Timer 5
// Timer 5 Functions: SETUP_TIMER_5, GET_TIMER5, SET_TIMER5
// Timer 5 Prototypes:
_bif void setup_timer_5(int16 mode); 
_bif int16 get_timer5(void);
_bif void set_timer5(int16 value);
// Constants used for SETUP_TIMER_5() are:
//      (or (via |) together constants from each group)
#define T5_DISABLED         0
#define T5_INTERNAL         0x07  
#define T5_EXTERNAL         0x87   
#define T5_EXTERNAL_SYNC    0x83   
#define T5_FOSC             0x47  
#define T5_LFINTRC          0xC7  

#define T5_ENABLE_SOSC      0x08   

#define T5_DIV_BY_1         0
#define T5_DIV_BY_2         0x10
#define T5_DIV_BY_4         0x20
#define T5_DIV_BY_8         0x30

#define T5_GATE             0x8000
#define T5_GATE_INVERTED    0xC000
#define T5_GATE_TOGGLE      0xA000
#define T5_GATE_SINGLE      0x9000
#define T5_GATE_TIMER0      0x8100 
#define T5_GATE_COMP1       0x8200  
#define T5_GATE_COMP2       0x8300  

////////////////////////////////////////////////////////////////// Timer 6
// Timer 6 Functions: SETUP_TIMER_6, GET_TIMER5, SET_TIMER5
// Timer 6 Prototypes:
_bif void setup_timer_6(int16 mode, int8 period, int8 postscaler);
_bif int8 get_timer6(void);
_bif void set_timer6(int8 value);
// Constants used for SETUP_TIMER_6() are:
//      (or (via |) together constants from each group)
#define T6_DISABLED         0
#define T6_DIV_BY_1         4
#define T6_DIV_BY_4         5
#define T6_DIV_BY_16        6
#define T6_DIV_BY_64        7 

////////////////////////////////////////////////////////////////// CCP
// CCP Functions: SETUP_CCPx, SET_PWMx_DUTY 
// CCP Variables: CCP_x, CCP_x_LOW, CCP_x_HIGH
// CCP1 Prototypes:
_bif void setup_ccp1(int32 mode);
_bif void setup_ccp1(int32 mode, int8 pwm); 
_bif void set_pwm1_duty(int8 value); 
_bif void set_pwm1_duty(int16 value); 
// Constants used for SETUP_CCPx() are:
#define CCP_OFF                          0
#define CCP_CAPTURE_EE                   0x03 
#define CCP_CAPTURE_FE                   0x04
#define CCP_CAPTURE_RE                   0x05
#define CCP_CAPTURE_DIV_4                0x06
#define CCP_CAPTURE_DIV_16               0x07
#define CCP_COMPARE_TOGGLE_RESET_TIMER   0x01 
#define CCP_COMPARE_TOGGLE               0x02 
#define CCP_COMPARE_SET_ON_MATCH         0x08
#define CCP_COMPARE_CLR_ON_MATCH         0x09
#define CCP_COMPARE_PULSE                0x0A 
#define CCP_COMPARE_PULSE_RESET_TIMER    0x0B 
#define CCP_PWM                          0x0F 
// One of the following may be OR'ed in with the above when using Capture mode 
#define CCP_CAPTURE_INPUT_CCP_PIN        0 
#define CCP_CAPTURE_INPUT_C1OUT          0x010000 
#define CCP_CAPTURE_INPUT_C2OUT          0x020000 
#define CCP_CAPTURE_INPUT_NCO1OUT        0x030000 
#define CCP_CAPTURE_INPUT_IOC_INTERRUPT  0x040000 
#define CCP_CAPTURE_INPUT_CLC1           0x050000 
#define CCP_CAPTURE_INPUT_CLC2           0x060000 
#define CCP_CAPTURE_INPUT_CLC3           0x070000  
#define CCP_CAPTURE_INPUT_CLC4           0x080000  
// The following may be OR'ed in with the above when using PWM mode 
#define CCP_PWM_LEFT_JUSTIFIED_DUTY      0x10 

#word   CCP_1       =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_LOW   =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_HIGH  =                    getenv("SFR:CCPR1H") 


#define CCP_TIMER2            0x0000  
#define CCP_TIMER4            0x1000  
#define CCP_TIMER6            0x2000  

// CCP2 Prototypes:
_bif void setup_ccp2(int32 mode);
_bif void setup_ccp2(int32 mode, int8 pwm); 
_bif void set_pwm2_duty(int8 value); 
_bif void set_pwm2_duty(int16 value); 
#word   CCP_2       =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_LOW   =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_HIGH  =                    getenv("SFR:CCPR2H") 

// CCP3 Prototypes:
_bif void setup_ccp3(int32 mode);
_bif void setup_ccp3(int32 mode, int8 pwm);
_bif void set_pwm3_duty(int8 value);
_bif void set_pwm3_duty(int16 value);
#word   CCP_3       =                    getenv("SFR:CCPR3L")
#byte   CCP_3_LOW   =                    getenv("SFR:CCPR3L")
#byte   CCP_3_HIGH  =                    getenv("SFR:CCPR3H")

// CCP4 Prototypes:
_bif void setup_ccp4(int32 mode);
_bif void setup_ccp4(int32 mode, int8 pwm);
_bif void set_pwm4_duty(int8 value);
_bif void set_pwm4_duty(int16 value);
#word   CCP_4       =                    getenv("SFR:CCPR4L")
#byte   CCP_4_LOW   =                    getenv("SFR:CCPR4L")
#byte   CCP_4_HIGH  =                    getenv("SFR:CCPR4H")

////////////////////////////////////////////////////////////////// PWM
// PWM Functions: SETUP_PWMx, SET_PWMx_DUTY
// PWM Prototypes:
_bif void setup_pwm5(int32 mode); 
_bif void set_pwm5_duty(int8 value); 
_bif void set_pwm5_duty(int16 value); 
_bif void setup_pwm6(int32 mode); 
_bif void set_pwm6_duty(int8 value); 
_bif void set_pwm6_duty(int16 value); 
// Constants used for SETUP_PWMx() are:
#define PWM_ENABLED      0x80
#define PWM_DISABLED     0x10000
#define PWM_ACTIVE_LOW   0x10
#define PWM_ACTIVE_HIGH  0
// One of the following may be OR'ed in with the above 
#define PWM_TIMER2       0x0000 
#define PWM_TIMER4       0x0100 
#define PWM_TIMER6       0x0200  

#bit PWM5OUT=getenv("BIT:PWM5OUT") 
#bit PWM6OUT=getenv("BIT:PWM6OUT") 

////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IS_IN 
// SPI Prototypes:
_bif void setup_spi(int32 mode);
_bif void spi_write(int8 value);
_bif void spi_write(int1 wait, int8 value);
_bif int8 spi_read(void);
_bif int8 spi_read(int8 value);
_bif int1 spi_data_is_in(void);
_bif void setup_spi2(int32 mode);  
_bif void spi_write2(int8 value);  
_bif int8 spi_read2(void);  
_bif int8 spi_read2(int8 value);  
_bif int1 spi_data_is_in2(void);  
// Constants used in SETUP_SPI() are: 
#define SPI_DISABLED             0x00 
#define SPI_MASTER               0x20 
#define SPI_SLAVE                0x24 
#define SPI_SCK_IDLE_HIGH        0x10 
#define SPI_SCK_IDLE_LOW         0x00 
#define SPI_CLK_DIV_4            0x00 
#define SPI_CLK_DIV_16           0x01 
#define SPI_CLK_DIV_64           0x02 
#define SPI_CLK_T2               0x03 
#define SPI_SS_DISABLED          0x01 

#define SPI_XMIT_L_TO_H          0x4000
#define SPI_XMIT_H_TO_L          0x0000

#define SPI_SAMPLE_AT_MIDDLE     0x0000
#define SPI_SAMPLE_AT_END        0x8000

//The following are provided for compatibility
#define SPI_L_TO_H       SPI_SCK_IDLE_LOW
#define SPI_H_TO_L       SPI_SCK_IDLE_HIGH

////////////////////////////////////////////////////////////////// UART
// UART Prototypes:
_bif void setup_uart(int32 baud);
_bif void setup_uart(int32 baud, int8 stream);
_bif void setup_uart(int32 baud, int8 stream, int32 clock);
_bif void set_uart_speed(int32 baud);
_bif void set_uart_speed(int32 baud, int8 stream);
_bif void set_uart_speed(int32 baud, int8 stream, int32 clock);
// Constants used in setup_uart() are:
// FALSE - Turn UART off
// TRUE  - Turn UART on
#define UART_ADDRESS           2
#define UART_DATA              4
#define UART_AUTODETECT        8
#define UART_AUTODETECT_NOWAIT 9
#define UART_WAKEUP_ON_RDA     10
#define UART_SEND_BREAK        13

////////////////////////////////////////////////////////////////// COMP
// Comparator Variables: C1OUT, C2OUT 
// Comparator Prototypes:
_bif void setup_comparator(int32 mode);
// Constants used in setup_comparator() are:
//
#define NC_NC_NC_NC     0x003F003F 
#define NC_NC           0x003F

//Pick one constant for COMP1
#define CP1_A1_A0       0x8000
#define CP1_C1_A0       0x8001 
#define CP1_C2_A0       0x8002 
#define CP1_C3_A0       0x8003 
#define CP1_FVR_A0      0x8006
#define CP1_VSS_A0      0x8007
#define CP1_A1_DAC      0x8028
#define CP1_C1_DAC      0x8029 
#define CP1_C2_DAC      0x802A 
#define CP1_C3_DAC      0x802B 
#define CP1_FVR_DAC     0x802E
#define CP1_VSS_DAC     0x802F
#define CP1_A1_FVR      0x8030
#define CP1_C1_FVR      0x8031 
#define CP1_C2_FVR      0x8032 
#define CP1_C3_FVR      0x8033 
#define CP1_FVR_FVR     0x8036
#define CP1_VSS_FVR     0x8037
#define CP1_A1_VSS      0x8038
#define CP1_C1_VSS      0x8039 
#define CP1_C2_VSS      0x803A 
#define CP1_C3_VSS      0x803B 
#define CP1_FVR_VSS     0x803E
#define CP1_VSS_VSS     0x803F
//Optionally OR with any of the following
#define CP1_INT_L2H     0x0080
#define CP1_INT_H2L     0x0040
#define CP1_INVERT      0x1000
#define CP1_FAST        0x0400
#define CP1_HYST        0x0200
#define CP1_SYNC        0x0100

//Pick one constant for COMP2 
#define CP2_A1_C0       0x80000000 
#define CP2_C1_C0       0x80010000 
#define CP2_C2_C0       0x80020000 
#define CP2_C3_C0       0x80030000 
#define CP2_FVR_C0      0x80060000 
#define CP2_VSS_C0      0x80070000 
#define CP2_A1_DAC      0x80280000 
#define CP2_C1_DAC      0x80290000 
#define CP2_C2_DAC      0x802A0000 
#define CP2_C3_DAC      0x802B0000 
#define CP2_FVR_DAC     0x802E0000 
#define CP2_VSS_DAC     0x802F0000 
#define CP2_A1_FVR      0x80300000 
#define CP2_C1_FVR      0x80310000 
#define CP2_C2_FVR      0x80320000 
#define CP2_C3_FVR      0x80330000 
#define CP2_FVR_FVR     0x80360000 
#define CP2_VSS_FVR     0x80370000 
#define CP2_A1_VSS      0x80380000 
#define CP2_C1_VSS      0x80390000 
#define CP2_C2_VSS      0x803A0000 
#define CP2_C3_VSS      0x803B0000 
#define CP2_FVR_VSS     0x803E0000 
#define CP2_VSS_VSS     0x803F0000 
//Optionally OR with any of the following 
#define CP2_INT_L2H     0x00800000 
#define CP2_INT_H2L     0x00400000 
#define CP2_INVERT      0x10000000 
#define CP2_FAST        0x04000000 
#define CP2_HYST        0x02000000 
#define CP2_SYNC        0x01000000 

#bit C1OUT = getenv("SFR:CM1CON0").6
#bit C2OUT = getenv("SFR:CM2CON0").6 

////////////////////////////////////////////////////////////////// VREF
// VREF Prototypes:
_bif void setup_vref(int8 mode);
// Constants used in setup_vref() are:
#define VREF_OFF                        0
#define VREF_ON                         0x80
// The following may be OR'ed in with the above using |
#define VREF_ADC_OFF                    0
#define VREF_ADC_1v024                  0x81
#define VREF_ADC_2v048                  0x82
#define VREF_ADC_4v096                  0x83  
// The following may be OR'ed in with the above using | 
#define VREF_COMP_DAC_OFF               0 
#define VREF_COMP_DAC_1v024             0x84 
#define VREF_COMP_DAC_2v048             0x88 
#define VREF_COMP_DAC_4v096             0x8C 
// The following may be OR'ed in with the above using |
#define TEMPERATURE_INDICATOR_DISABLED  0
#define TEMPERATURE_INDICATOR_ENABLED   0x20
#define TEMPERATURE_RANGE_LOW           0
#define TEMPERATURE_RANGE_HIGH          0x10

////////////////////////////////////////////////////////////////// DAC
// Digital to Analog Functions: SETUP_DAC(), DAC_WRITE()  
// DAC Prototypes:
_bif void setup_dac(int8 mode);
_bif void dac_write(int8 value);
// Constants used in SETUP_DAC() are:
#define DAC_OFF  0
#define DAC_VSS_VDD   0x80
#define DAC_VREF_VDD  0x81  
#define DAC_VSS_VREF  0x84
#define DAC_VREF_VREF 0x85  
#define DAC_VSS_FVR   0x88
#define DAC_VREF_FVR  0x89  
// The following may be OR'ed in with the above using | 
#define DAC_OUTPUT    0x20 

////////////////////////////////////////////////////////////////// CWG
// CWG Functions: SETUP_CWG(), CWG_STATUS(), CWG_RESTART()
// CWG Prototypes:
_bif void setup_cwg(int32 mode, int16 shutdown, int8 dt_rising, int8 dt_falling);
_bif void setup_cwg2(int32 mode, int16 shutdown, int8 dt_rising, int8 dt_falling);        
_bif void cwg_restart(void);
_bif void cwg2_restart(void);        
_bif int8 cwg_status(void);
_bif void cwg2_status(void);        
// Constants used in SETUP_CWG() and SETUP_CWG2() first parameter are:           
#define CWG_ENABLED                    0x80
#define CWG_DISABLED                   0x00
// One of the following may be OR'ed in with the above using |
#define CWG_MODE_STEERING              0x00
#define CWG_MODE_SYNCRONOUS_STEERING   0x01
#define CWG_MODE_FULL_BRIDGE           0x02 
#define CWG_MODE_FULL_BRIDGE_REV       0x03 
#define CWG_MODE_HALF_BRIDGE           0x04
#define CWG_MODE_PUSH_PULL             0x05
// One of the following may be OR'ed in with the above using |
#define CWG_CLOCK_HFINTOSC             0x0100
#define CWG_CLOCK_FOSC                 0x0000
// One of the following may be OR'ed in with the above using |
#define CWG_INPUT_CWGxIN               0x0000 
#define CWG_INPUT_COMP1                0x1000        
#define CWG_INPUT_COMP2                0x2000         
#define CWG_INPUT_CCP1                 0x3000        
#define CWG_INPUT_CCP2                 0x4000        
#define CWG_INPUT_CCP3                 0x5000  
#define CWG_INPUT_CCP4                 0x6000  
#define CWG_INPUT_PWM5                 0x7000 
#define CWG_INPUT_PWM6                 0x8000 
#define CWG_INPUT_NCO1                 0x9000 
#define CWG_INPUT_CLC1                 0xA000 
#define CWG_INPUT_CLC2                 0xB000 
#define CWG_INPUT_CLC3                 0xC000  
#define CWG_INPUT_CLC4                 0xD000  
// Any of the following may be OR'ed in with the above using |
#define CWG_A_INVERTED                 0x010000
#define CWG_B_INVERTED                 0x020000
#define CWG_C_INVERTED                 0x040000 
#define CWG_D_INVERTED                 0x080000 
// One of the following may be OR'ed in with the above using |
#define CWG_A_FORCE_HIGH               0x10000000 //Only available in Steering modes
#define CWG_A_FORCE_LOW                0x00000000 //Only available in Steering modes
#define CWG_A_OUTPUT_PWM               0x01000000 //Only available in Steering modes
// One of the following may be OR'ed in with the above using |
#define CWG_B_FORCE_HIGH               0x20000000 //Only available in Steering modes
#define CWG_B_FORCE_LOW                0x00000000 //Only available in Steering modes
#define CWG_B_OUTPUT_PWM               0x02000000 //Only available in Steering modes
// One of the following may be OR'ed in with the above using | 
#define CWG_C_FORCE_HIGH               0x40000000 //Only available in Steering modes 
#define CWG_C_FORCE_LOW                0x00000000 //Only available in Steering modes 
#define CWG_C_OUTPUT_PWM               0x04000000 //Only available in Steering modes 
// One of the following may be OR'ed in with the above using | 
#define CWG_D_FORCE_HIGH               0x80000000 //Only available in Steering modes 
#define CWG_D_FORCE_LOW                0x00000000 //Only available in Steering modes 
#define CWG_D_OUTPUT_PWM               0x08000000 //Only available in Steering modes 

// Constants used in SETUP_CWG() and SETUP_CWG2() second parameter are:           
// Any of the following may be OR'ed together using |
#define CWG_NO_AUTO_SHUTDOWN           0x0000
#define CWG_SHUTDOWN_ON_CWGxIN         0x0001 
#define CWG_SHUTDOWN_ON_COMP1          0x0002         
#define CWG_SHUTDOWN_ON_COMP2          0x0004          
#define CWG_SHUTDOWN_ON_CLC2           0x0008 
#define CWG_SHUTDOWN_ON_CLC4           0x0010  
// One of the following may be OR'ed in with the above using |
#define CWG_SHUTDOWN_AC_INACTIVE_STATE 0x0000
#define CWG_SHUTDOWN_AC_TRISTATE       0x0400
#define CWG_SHUTDOWN_AC_LOW            0x0800
#define CWG_SHUTDOWN_AC_HIGH           0x0C00
// One of the following may be OR'ed in with the above using |
#define CWG_SHUTDOWN_BD_INACTIVE_STATE 0
#define CWG_SHUTDOWN_BD_TRISTATE       0x1000
#define CWG_SHUTDOWN_BD_LOW            0x2000
#define CWG_SHUTDOWN_BD_HIGH           0x3000

// Constants returned from CWG_STATUS() and CWG2_STATUS() are:           
#define CWG_AUTO_SHUTDOWN              0x80

////////////////////////////////////////////////////////////////// NCO
// NCO Functions: SETUP_NCO(), SET_NCO_ACCUMULATOR(), 
//                GET_NCO_ACCUMULATOR(), SET_NCO_INC_VALUE(),
//                GET_NCO_INC_VALUE()
// NCO Prototypes:
_bif void setup_nco(int32 mode, int32 inc_value);  
_bif void set_nco_inc_value(int32 value);  
_bif void set_nco_accumulator(int32 value);
_bif int32 get_nco_accumulator(void);
_bif int32 get_nco_inc_value(void);  
// Constants used in SETUP_NCO() first parameter are: 
#define NCO_ENABLED                 0x0080
#define NCO_DISABLED                0
// One of the following may be OR'ed in with the above using |
#define NCO_ACTIVE_HIGH             0x0010
#define NCO_ACTIVE_LOW              0
// One of the following may be OR'ed in with the above using |
#define NCO_PULSE_FREQ_MODE         0x0001
#define NCO_FIXED_DUTY_MODE         0
// One of the following may be OR'ed in with the above using |
#define NCO_CLOCK_LC1OUT            0x0200       
#define NCO_CLOCK_FOSC              0x0100      
#define NCO_CLOCK_HFINOSC           0       

// One of the following may be OR'ed in with the above using |
#define NCO_PULSE_WIDTH_1           0
#define NCO_PULSE_WIDTH_2           0x2000
#define NCO_PULSE_WIDTH_4           0x4000
#define NCO_PULSE_WIDTH_8           0x6000
#define NCO_PULSE_WIDTH_16          0x8000
#define NCO_PULSE_WIDTH_32          0xA000
#define NCO_PULSE_WIDTH_64          0xC000
#define NCO_PULSE_WIDTH_128         0xE000


#bit N1OUT = getenv("BIT:N1OUT")    

////////////////////////////////////////////////////////////////// CLC
// CLC Functions: SETUP_CLCx(), CLCx_SETUP_INPUT(), 
//                CLCx_SETUP_GATE()
// CLC Prototypes:
_bif void setup_clc1(int32 mode);
_bif void setup_clc2(int32 mode);  
_bif void setup_clc3(int32 mode);  
_bif void setup_clc4(int32 mode);  
_bif void clc1_setup_input(int8 input, int8 selection);
_bif void clc2_setup_input(int8 input, int8 selection);  
_bif void clc3_setup_input(int8 input, int8 selection);  
_bif void clc4_setup_input(int8 input, int8 selection);  
_bif void clc1_setup_gate(int8 gate, int16 mode);
_bif void clc2_setup_gate(int8 gate, int16 mode);  
_bif void clc3_setup_gate(int8 gate, int16 mode);  
_bif void clc4_setup_gate(int8 gate, int16 mode);  
// Constants used in SETUP_CLCx() are:
#define CLC_ENABLED                         0x080
#define CLC_DISABLED                        0
// Any of the following may be OR'ed in with the above uing |
#define CLC_INT_L_TO_H                      0x010
#define CLC_INT_H_TO_L                      0x008
#define CLC_OUTPUT_INVERTED                 0x100
// One of the following may be OR'ed in with the above using |
#define CLC_MODE_AND_OR                     0x000
#define CLC_MODE_OR_XOR                     0x001
#define CLC_MODE_AND                        0x002
#define CLC_MODE_SR_LATCH                   0x003
#define CLC_MODE_1_INPUT_D_FLIP_FLOP        0x004
#define CLC_MODE_2_INPUT_D_FLIP_FLOP        0x005
#define CLC_MODE_JK_FLIP_FLOP               0x006
#define CLC_MODE_1_INPUT_TRANSPARENT_LATCH  0x007

// Constants used in CLCx_SETUP_INPUT() second parameter are: (first parameter is input 1-4)
#define CLC_INPUT_CLCIN0                    0   
#define CLC_INPUT_CLCIN1                    0x01   
#define CLC_INPUT_CLCIN2                    0x02   
#define CLC_INPUT_CLCIN3                    0x03   
#define CLC_INPUT_CLC1OUT                   0x04        
#define CLC_INPUT_CLC2OUT                   0x05        
#define CLC_INPUT_CLC3OUT                   0x06         
#define CLC_INPUT_CLC4OUT                   0x07         
#define CLC_INPUT_C1OUT                     0x08         
#define CLC_INPUT_C2OUT                     0x09          
#define CLC_INPUT_DSMOUT                    0x0A 
#define CLC_INPUT_CLKR                      0x0B 
#define CLC_INPUT_CCP1OUT                   0x0C          
#define CLC_INPUT_CCP2OUT                   0x0D          
#define CLC_INPUT_CCP3                      0x0E  
#define CLC_INPUT_CCP4                      0x0F  
#define CLC_INPUT_PWM5                      0x10 
#define CLC_INPUT_PWM6                      0x11 
#define CLC_INPUT_SCL1                      0x12 
#define CLC_INPUT_SDA1                      0x13 
#define CLC_INPUT_SCL2                      0x14  
#define CLC_INPUT_SDA2                      0x15  
#define CLC_INPUT_U1TX                      0x16 
#define CLC_INPUT_U1DT                      0x17 
#define CLC_INPUT_TIMER0                    0x18 
#define CLC_INPUT_TIMER1                    0x19 
#define CLC_INPUT_TIMER2                    0x1A 
#define CLC_INPUT_IOCIF                     0x1B 
#define CLC_INPUT_ADCRC                     0x1C 
#define CLC_INPUT_LFINTOSC                  0x1D 
#define CLC_INPUT_HFINTOSC                  0x1E 
#define CLC_INPUT_FOSC                      0x1F 
#define CLC_INPUT_TIMER3                    0x20  
#define CLC_INPUT_TIMER4                    0x21  
#define CLC_INPUT_TIMER5                    0x22  
#define CLC_INPUT_TIMER6                    0x23  

// Constants used in CLCx_SETUP_GATE() second parameter are: (first parameter is gate 1-4) 
// One of the following defines can be used to set the logic that will be performed on 
// all four inputs to the specified gate.
#define CLC_GATE_AND                        0x155
#define CLC_GATE_NAND                       0x055
#define CLC_GATE_NOR                        0x1AA
#define CLC_GATE_OR                         0x0AA
#define CLC_GATE_CLEAR                      0
#define CLC_GATE_SET                        0x100

// As an alternative to the above defines any of the following can be OR'ed together
// to enable the individual inputs to the specified gate.
#define CLC_GATE_INVERTED_INPUT_1           0x001
#define CLC_GATE_NON_INVERTED_INPUT_1       0x002
#define CLC_GATE_INVERTED_INPUT_2           0x004
#define CLC_GATE_NON_INVERTED_INPUT_2       0x008
#define CLC_GATE_INVERTED_INPUT_3           0x010
#define CLC_GATE_NON_INVERTED_INPUT_3       0x020
#define CLC_GATE_INVERTED_INPUT_4           0x040
#define CLC_GATE_NON_INVERTED_INPUT_4       0x080
#define CLC_GATE_OUTPUT_INVERTED            0x100

#bit LC1OUT = getenv("SFR:CLC1CON").5 
#bit LC2OUT = getenv("SFR:CLC2CON").5  
#bit LC3OUT = getenv("SFR:CLC3CON").5  
#bit LC4OUT = getenv("SFR:CLC4CON").5  

////////////////////////////////////////////////////////////////// DSM
// Data Signal Modulator Functions: SETUP_DSM()  
// DSM Prototypes:
_bif void setup_dsm(int1 enable);
_bif void setup_dsm(unsigned int8 mode, unsigned int8 source);      
_bif void setup_dsm(unsigned int8 mode, unsigned int8 source, unsigned int16 carrier);      
// Constants used in SETUP_DSM() first parameter are:  
#define DSM_ENABLED                     0x80
#define DSM_DISABLED                    0x00
// Any of the following may be or'ed in with the above using |
#define DSM_OUTPUT_INVERTED             0x10
//Constants used as the second parameter are:
#define DSM_SOURCE_MANUAL               0x00       
#define DSM_SOURCE_MDMIN                0x01       
#define DSM_SOURCE_CCP1                 0x02 
#define DSM_SOURCE_CCP2                 0x03 
#define DSM_SOURCE_PWM5                 0x04 
#define DSM_SOURCE_PWM6                 0x05 
#define DSM_SOURCE_C1OUT                0x06 
#define DSM_SOURCE_C2OUT                0x07 
#define DSM_SOURCE_SPI1                 0x08 
#define DSM_SOURCE_SPI2                 0x09  
#define DSM_SOURCE_U1TX                 0x0A 
#define DSM_SOURCE_NCO1                 0x0B 
#define DSM_SOURCE_CLC1                 0x0C 
#define DSM_SOURCE_CLC2                 0x0D 
#define DSM_SOURCE_CLC3                 0x0E  
#define DSM_SOURCE_CLC4                 0x0F  
//Constants used as the third parameter are:
#define DSM_CARRIER_LOW_VSS             0x0000      
#define DSM_CARRIER_LOW_MDCIN1          0x0001      
#define DSM_CARRIER_LOW_MDCIN2          0x0002        
#define DSM_CARRIER_LOW_REFO            0x0003 
#define DSM_CARRIER_LOW_CCP1            0x0004  
#define DSM_CARRIER_LOW_CCP2            0x0005    
#define DSM_CARRIER_LOW_PWM5            0x0006 
#define DSM_CARRIER_LOW_PWM6            0x0007 
#define DSM_CARRIER_LOW_NCO1            0x0008 
#define DSM_CARRIER_LOW_SYSTEM_CLOCK    0x000A 
#define DSM_CARRIER_LOW_HFINTOSC        0x000B 
#define DSM_CARRIER_LOW_CLC1            0x000C 
#define DSM_CARRIER_LOW_CLC2            0x000D 
#define DSM_CARRIER_LOW_CLC3            0x000E  
#define DSM_CARRIER_LOW_CLC4            0x000F  
#define DSM_SYNC_CARRIER_LOW            0x0020      
#define DSM_INVERT_CARRIER_LOW          0x0040      
#define DSM_CARRIER_HIGH_VSS            0x0000      
#define DSM_CARRIER_HIGH_MDCIN1         0x0100     
#define DSM_CARRIER_HIGH_MDCIN2         0x0200     
#define DSM_CARRIER_HIGH_REFO           0x0300 
#define DSM_CARRIER_HIGH_CCP1           0x0400  
#define DSM_CARRIER_HIGH_CCP2           0x0500    
#define DSM_CARRIER_HIGH_PWM5           0x0600 
#define DSM_CARRIER_HIGH_PWM6           0x0700 
#define DSM_CARRIER_HIGH_NCO1           0x0800 
#define DSM_CARRIER_HIGH_SYSTEM_CLOCK   0x0A00 
#define DSM_CARRIER_HIGH_HFINTOSC       0x0B00 
#define DSM_CARRIER_HIGH_CLC1           0x0C00 
#define DSM_CARRIER_HIGH_CLC2           0x0D00 
#define DSM_CARRIER_HIGH_CLC3           0x0E00  
#define DSM_CARRIER_HIGH_CLC4           0x0F00  
#define DSM_SYNC_CARRIER_HIGH           0x2000      
#define DSM_INVERT_CARRIER_HIGH         0x4000      

#bit MDBIT = getenv("SFR:MDCON").0      
#bit MDOUT = getenv("SFR:MDCON").3      


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes
_bif int16 setup_oscillator(int32 mode);
_bif int16 setup_oscillator(int32 mode, signed int8 tune);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_HFINTRC_1MHZ             0x00000060
#define OSC_HFINTRC_2MHZ             0x00000160
#define OSC_HFINTRC_4MHZ             0x00000360 
#define OSC_HFINTRC_8MHZ             0x00000460 
#define OSC_HFINTRC_12MHZ            0x00000560 
#define OSC_HFINTRC_16MHZ            0x00000660 
#define OSC_HFINTRC_24MHZ            0x00000500 
#define OSC_HFINTRC_32MHZ            0x00000600 
#define OSC_EXTOSC_PLL               0x00000010 
#define OSC_SOSC                     0x00000030 
#define OSC_LFINTRC                  0x00000040 
#define OSC_EXTOSC                   0x00000070
// One of the following may be OR'ed in with the above using |
#define OSC_CLK_DIV_BY_1             0x00000000
#define OSC_CLK_DIV_BY_2             0x00000001
#define OSC_CLK_DIV_BY_4             0x00000002
#define OSC_CLK_DIV_BY_8             0x00000003
#define OSC_CLK_DIV_BY_16            0x00000004
#define OSC_CLK_DIV_BY_32            0x00000005
#define OSC_CLK_DIV_BY_64            0x00000006
#define OSC_CLK_DIV_BY_128           0x00000007
#define OSC_CLK_DIV_BY_256           0x00000008
#define OSC_CLK_DIV_BY_512           0x00000009
// One of the following may be OR'ed in with the above using | 
#define OSC_SOSC_PWR_LOW             0x00000000 
#define OSC_SOSC_PWR_HIGH            0x00400000 
#define OSC_SOSC_DIGITAL             0x00200000 
// The following may be OR'ed in with the above using |
#define OSC_CLOCK_SWITCH_HOLD        0x00800000
#define OSC_ADOSC_ENABLED            0x04000000
#define OSC_SOSC_ENABLED             0x08000000
#define OSC_LFINTRC_ENABLED          0x10000000
#define OSC_HFINTRC_ENABLED          0x40000000
#define OSC_EXTOSC_ENABLED           0x80000000
// A second optional parameter may be used with this part to fine
// tune the speed (signed int,-32 to 31)
// Result may be (ignore all other bits)
#define OSC_PLL_READY                0x0001
#define OSC_ADCRC_READY              0x0004
#define OSC_SOSC_READY               0x0008
#define OSC_LFINTRC_READY            0x0010
#define OSC_HFINTRC_READY            0x0040
#define OSC_EXTOSC_READY             0x0080
#define OSC_CLOCK_SWITCH_IN_PROGRESS 0x0800
#define OSC_CLOCK_SWITCH_COMPLETE    0x1000


////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC(), ADC_DONE()
// ADC Prototypes:
_bif void setup_adc(int16 mode);
_bif int8 read_adc(void);
_bif int8 read_adc(int8 mode);
_bif int16 read_adc(void);
_bif int16 read_adc(int8 mode);
_bif int1 adc_done(void);
// Constants used for SETUP_ADC() are: 
#define ADC_OFF                0              // ADC Off
#define ADC_CLOCK_DIV_2    0x100  
#define ADC_CLOCK_DIV_4     0x40  
#define ADC_CLOCK_DIV_8     0x10
#define ADC_CLOCK_DIV_32    0x20
#define ADC_CLOCK_DIV_16    0x50
#define ADC_CLOCK_DIV_64    0x60
#define ADC_CLOCK_INTERNAL  0x30              // Internal 2-6us

//ADC Prototypes:
_bif void setup_adc_ports(int32 pins);
_bif void setup_adc_ports(int32 pins, int32 reference);
_bif void set_analog_pins(int8 pin, ...);
_bif void setup_adc_reference(int32 reference);
_bif void set_adc_channel(int8 channel);
// Constants used in SETUP_ADC_PORTS() are:
// First argument:
// OR together desired pins
#define sAN0          0x00010000    //| A0
#define sAN1          0x00020000    //| A1
#define sAN2          0x00040000    //| A2
#define sAN4          0x00100000    //| A4
#define sAN5          0x00200000    //| A5
#define sAN12         0x00001000    //| B4 
#define sAN13         0x00002000    //| B5 
#define sAN14         0x00004000    //| B6 
#define sAN15         0x00008000    //| B7 
#define sAN16         0x00000001    //| C0 
#define sAN17         0x00000002    //| C1 
#define sAN18         0x00000004    //| C2 
#define sAN19         0x00000008    //| C3 
#define sAN20         0x00000010    //| C4 
#define sAN21         0x00000020    //| C5 
#define sAN22         0x00000040    //| C6 
#define sAN23         0x00000080    //| C7 
#define NO_ANALOGS             0    // None
#define ALL_ANALOG    0x0037F0FF    // A0 A1 A2 A4 A5 B4 B5 B6 B7 C0 C1 C2 C3 C4 C5 C6 C7 

// Optional Second parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE:
#define VSS_VDD       0x00000000    //| Range 0-Vdd
#define VSS_VREF      0x02000000    //| Range 0-VrefH
#define VSS_FVR       0x03000000    //| Range 0-Fixed Voltage Reference
#define VREF_VDD      0x04000000    //| Range VrefL-Vdd
#define VREF_VREF     0x06000000    //| Range VrefL-VrefH
#define VREF_FVR      0x07000000    //| Range VrefL-Fixed Voltage Reference

// Constants used in SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define AVSS_CHANNEL              0x3C
#define TEMPERATURE_INDICATOR     0x3D
#define DAC_CHANNEL               0x3E
#define FVR_CHANNEL               0x3F

// Constants used in READ_ADC() are:
#define ADC_START_AND_READ     7   // This is the default if nothing is specified
#define ADC_START_ONLY         1
#define ADC_READ_ONLY          6

////////////////////////////////////////////////////////////////// BIF
// Built In Functions Prototypes
//
// Math Prototypes:
_bif signed int8 abs(signed int8 x);
_bif signed int16 abs(signed int16 x);
_bif signed int32 abs(signed int32 x);
_bif float32 abs(float32 x);
_bif unsigned int16 _mul(unsigned int8, unsigned int8);
_bif signed int16 _mul(signed int8, signed int8);
_bif unsigned int32 _mul(unsigned int16, unsigned int16);
_bif signed int32 _mul(signed int16, signed int16);

// Memory Manipulation Prototypes:
_bif int8 read_bank(int8 bank, int8 offset);
_bif void write_bank(int8 bank, int8 offset, int8 value);
_bif void strcpy(char* dest, char* src);
_bif void memset(unsigned int8* destination, unsigned int8 value, unsigned int16 num);
_bif void memcpy(unsigned int8* destination, unsigned int8* source, unsigned int16 num);

// String Prototypes:
_bif char toupper(char cvalue);
_bif char tolower(char cvalue);
_bif void sprintf(char* string, char* cstring, ...);

// Data Manipulators Prototypes:
_bif int1 shift_left(unsigned int8* address, unsigned int8 bytes, int1 value);
_bif int1 shift_right(unsigned int8* address, unsigned int8 bytes, int1 value);
_bif void rotate_left(unsigned int8* address, unsigned int8 bytes);
_bif void rotate_right(unsigned int8* address, unsigned int8 bytes);
_bif void swap(unsigned int8 value);
_bif unsigned int8 make8(unsigned int16 var, unsigned int8 offset);
_bif unsigned int8 make8(unsigned int32 var, unsigned int8 offset);
_bif unsigned int16 make16(unsigned int8 varhigh, unsigned int8 varlow);
_bif unsigned int32 make32(unsigned int16 var1);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int16 var2);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int8 var2);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int8 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int8 var3, unsigned int8 var4);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int16 var2);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int16 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int16 var3);
_bif void bit_set(unsigned int8 var, unsigned int8 bit);
_bif void bit_set(unsigned int16 var, unsigned int8 bit);
_bif void bit_set(unsigned int32 var, unsigned int8 bit);
_bif void bit_clear(unsigned int8 var, unsigned int8 bit);
_bif void bit_clear(unsigned int16 var, unsigned int8 bit);
_bif void bit_clear(unsigned int32 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int8 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int16 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int32 var, unsigned int8 bit);

// #use delay() Prototypes:
_bif void delay_cycles(unsigned int8 count);
_bif void delay_ms(unsigned int16 time);
_bif void delay_us(unsigned int16 time);

// #use rs232() Prototypes:
_bif void putchar(char cdata);
_bif void putchar(char cdata, unsigned int8 stream);
_bif void puts(char* string);
_bif void puts(char* string, unsigned int8 stream);
_bif char getch(void);
_bif char getch(unsigned int8 stream);
_bif void gets(char* string);
_bif void gets(char* string, unsigned int8 stream);
_bif int1 kbhit(void);
_bif int1 kbhit(unsigned int8 stream);
_bif void printf(char* string, ...);
_bif void fprintf(unsigned int8 stream, char* string, ...);
_bif void putc_send(void);
_bif void fputc_send(unsigned int8 stream);
_bif int1 rcv_buffer_full(void);
_bif int1 rcv_buffer_full(unsigned int8 stream);
_bif unsigned int16 rcv_buffer_bytes(void);
_bif unsigned int16 rcv_buffer_bytes(unsigned int8 stream);
_bif int1 tx_buffer_full(void);
_bif int1 tx_buffer_full(unsigned int8 stream);
_bif unsigned int16 tx_buffer_bytes(void);
_bif unsigned int16 tx_buffer_bytes(unsigned int8 stream);
_bif unsigned int8 linbus_header(unsigned int8 identifier);
_bif unsigned int8 linbus_header(unsigned int8 stream, unsigned int8 identifier);
_bif int1 linbus_header_hit(void);
_bif int1 linbus_header_hit(unsigned int8 stream);
_bif unsigned int8 linbus_header_get(void);
_bif unsigned int8 linbus_header_get(unsigned int8 stream);
_bif unsigned int8 linbus_rx_response(unsigned int8 *DataPtr, unsigned int8 Count);
_bif unsigned int8 linbus_rx_response(unsigned int8 stream, unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_tx_response(unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_tx_response(unsigned int8 stream, unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_checksum_type(unsigned int8 type);
_bif void linbus_checksum_type(unsigned int8 stream, unsigned int8 type);

// #use i2c() Prototypes:
_bif unsigned int8 i2c_read(void);
_bif unsigned int8 i2c_read(unsigned int8 ack);
_bif unsigned int8 i2c_read(unsigned int8 stream, unsigned int8 ack);
_bif int1 i2c_write(unsigned int8 data);
_bif int1 i2c_write(unsigned int8 stream, unsigned int8 data);
_bif void i2c_start(void);
_bif void i2c_start(unsigned int8 stream);
_bif void i2c_start(unsigned int8 stream, unsigned int8 restart);
_bif void i2c_stop(void);
_bif void i2c_stop(unsigned int8 stream);
_bif int8 i2c_isr_state(void);
_bif void i2c_slaveaddr(unsigned int8 addr);
_bif void i2c_slaveaddr(unsigned int8 stream, unsigned int8 addr);
_bif int1 i2c_poll(void);
_bif int1 i2c_poll(unsigned int8 stream);
_bif void i2c_init(unsigned int32 baud);
_bif void i2c_init(unsigned int8 stream, unsigned int32 baud);
_bif int8 i2c_transfer(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount, unsigned int8 *rData, unsigned int8 *rCount);
_bif int8 i2c_transfer(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount, unsigned int8 *rData, unsigned int8 *rCount);
_bif int8 i2c_transfer_out(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer_out(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer_in(unsigned int8 Address, unsigned int8 *rData, unsigned int8 rCount);
_bif int8 i2c_transfer_in(unsigned int8 stream, unsigned int8 Address, unsigned int8 *rData, unsigned int8 rCount);

// #use spi() Prototypes:
_bif unsigned int8 spi_xfer(void);
_bif unsigned int16 spi_xfer(void);
_bif unsigned int32 spi_xfer(void);
_bif unsigned int8 spi_xfer(unsigned int8 data);
_bif unsigned int16 spi_xfer(unsigned int16 data);
_bif unsigned int32 spi_xfer(unsigned int32 data);
_bif unsigned int8 spi_xfer(unsigned int8 stream, unsigned int8 data);
_bif unsigned int16 spi_xfer(unsigned int8 stream, unsigned int16 data);
_bif unsigned int32 spi_xfer(unsigned int8 stream, unsigned int32 data);
_bif unsigned int8 spi_xfer(unsigned int8 stream, unsigned int8 data, unsigned int8 bits);
_bif unsigned int16 spi_xfer(unsigned int8 stream, unsigned int16 data, unsigned int8 bits);
_bif unsigned int32 spi_xfer(unsigned int8 stream, unsigned int32 data, unsigned int8 bits);
_bif void spi_init(unsigned int32 baud);
_bif void spi_init(unsigned int8 stream, unsigned int32 baud);
_bif void spi_speed(unsigned int32 baud);
_bif void spi_speed(unsigned int8 stream, unsigned int32 baud);
_bif void spi_speed(unsigned int8 stream, unsigned int32 baud, unsigned int32 clock);
_bif void spi_prewrite(unsigned int8 data);
_bif void spi_prewrite(unsigned int16 data);
_bif void spi_prewrite(unsigned int32 data);
_bif void spi_prewrite(unsigned int8, unsigned int8 data);
_bif void spi_prewrite(unsigned int8, unsigned int16 data);
_bif void spi_prewrite(unsigned int8, unsigned int32 data);
_bif unsigned int8 spi_xfer_in(void);
_bif unsigned int16 spi_xfer_in(void);
_bif unsigned int32 spi_xfer_in(void);
_bif unsigned int8 spi_xfer_in(unsigned int8 bits);
_bif unsigned int16 spi_xfer_in(unsigned int8 bits);
_bif unsigned int32 spi_xfer_in(unsigned int8 bits);
_bif unsigned int8 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif unsigned int16 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif unsigned int32 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif void spi_transfer(unsigned int8 *wData, unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer(unsigned int8 stream, unsigned int8 *wData, unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer_write(unsigned int8 *wData, unsigned int16 Count);
_bif void spi_transfer_write(unsigned int8 stream, unsigned int8 *wData, unsigned int16 Count);
_bif void spi_transfer_read(unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer_read(unsigned int8 stream, unsigned int8 *rData, unsigned int16 Count);

// #use rtos() Prototypes:
_bif void rtos_run(void);
_bif void rtos_yield(void);
_bif void rtos_enable(unsigned int8 task);
_bif void rtos_disable(unsigned int8 task);
_bif void rtos_terminate(void);
_bif void rtos_await(int1 flag);
_bif void rtos_wait(unsigned int8 sem);
_bif void rtos_signal(unsigned int8 sem);
_bif void rtos_msg_send(unsigned int8 task, unsigned int8 msg);
_bif unsigned int8 rtos_msg_read(void);
_bif unsigned int8 rtos_msg_poll(void);
_bif int1 rtos_overrun(unsigned int8 task);
_bif void rtos_stats(unsigned int8 task, unsigned int8* stat);

// #use timer() Prototypes:
_bif unsigned int8 get_ticks(void);
_bif unsigned int16 get_ticks(void);
_bif unsigned int32 get_ticks(void);
_bif unsigned int8 get_ticks(unsigned int8 stream);
_bif unsigned int16 get_ticks(unsigned int8 stream);
_bif unsigned int32 get_ticks(unsigned int8 stream);
_bif void set_ticks(unsigned int8 value);
_bif void set_ticks(unsigned int16 value);
_bif void set_ticks(unsigned int32 value);
_bif void set_ticks(unsigned int8 stream, unsigned int8 value);
_bif void set_ticks(unsigned int8 stream, unsigned int16 value);
_bif void set_ticks(unsigned int8 stream, unsigned int32 value);

// #use pwm() Prototypes:
_bif void pwm_on(void);
_bif void pwm_on(unsigned int8 stream);
_bif void pwm_off(void);
_bif void pwm_off(unsigned int8 stream);
_bif void pwm_set_duty(unsigned int16 duty);
_bif void pwm_set_duty(unsigned int8 stream, unsigned int16 duty);
_bif void pwm_set_duty_percent(unsigned int16 percent);
_bif void pwm_set_duty_percent(unsigned int8 stream, unsigned int16 percent);
_bif void pwm_set_frequency(unsigned int32 frequency);
_bif void pwm_set_frequency(unsigned int8 stream, unsigned int32 frequency);
_bif unsigned int16 pwm_get_duty_period(void);
_bif unsigned int16 pwm_get_duty_period(unsigned int8 stream);

// #use capture() Prototypes:
_bif unsigned int16 get_capture_time(void);
_bif unsigned int16 get_capture_time(unsigned int8 stream);
_bif int1 get_capture_event(void);
_bif int1 get_capture_event(unsigned int8 stream);

// Enviroment Prototypes:
//_bif unsigned int8 getenv(char* cstring);

// Address Prototypes:
#ifndef __ADDRESS__
 #if defined(__PCM__) || defined(__PCB__)
  #define __ADDRESS__ unsigned int16
 #else
  #define __ADDRESS__ unsigned int32
 #endif
#endif
_bif void goto_address(__ADDRESS__ address);
_bif __ADDRESS__ label_address(__ADDRESS__ label);

// Program Memory Read Prototypes:
_bif void read_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif unsigned int16 read_program_eeprom(__ADDRESS__ address);
_bif void read_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void read_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 
_bif void read_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void read_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 

// Program Memory Write Prototypes:
_bif void write_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif void write_program_eeprom(__ADDRESS__ address, unsigned int16 data);
_bif void write_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void write_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 
_bif void write_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void write_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 

// EEPROM Prototypes:
#ifndef __EEADDRESS__
 #if getenv("DATA_EEPROM")>256
  #define __EEADDRESS__ unsigned int16
 #else
  #define __EEADDRESS__ unsigned int8
 #endif
#endif
_bif unsigned int8 read_eeprom(__EEADDRESS__ address);
_bif void write_eeprom(__EEADDRESS__ address, unsigned int8 value);

////////////////////////////////////////////////////////////////// INT
// Interrupt Functions: ENABLE_INTERRUPTS(), DISABLE_INTERRUPTS(),
//                      CLEAR_INTERRUPT(), INTERRUPT_ACTIVE(),  
//                      EXT_INT_EDGE()  
// INT Prototypes:
_bif void enable_interrupts(int32 interrupt);
_bif void disable_interrupts(int32 interrupt);
_bif void clear_interrupt(int32 interrupt);
_bif int1 interrupt_active(int32 interrupt);
_bif int1 interrupt_enabled(int32 interrupt);
_bif void ext_int_edge(int8 source, int8 edge);  
_bif void jump_to_isr(int16 address);
// Constants used in EXT_INT_EDGE() are:  
#define L_TO_H              0x40  
#define H_TO_L                 0  
// Constants used in ENABLE/DISABLE_INTERRUPTS() are:
#define GLOBAL                    0x0BC0
#define PERIPH                    0x0B40
#define INT_TIMER1                0x309101
#define INT_TIMER2                0x309102
#define INT_SSP                   0x309108
#define INT_TBE                   0x309110
#define INT_RDA                   0x309120
#define INT_AD                    0x309140
#define INT_TIMER1_GATE            0x309180
#define INT_EEPROM                0x309210
#define INT_COMP                  0x309220
#define INT_COMP2                 0x309240
#define INT_CLC                   0x309301
#define INT_CLC2                  0x309302
#define INT_CLC3                  0x309304
#define INT_CLC4                  0x309308
#define INT_TIMER4                0x309202
#define INT_TIMER5                0x309410
#define INT_TIMER5_GATE            0x309420
#define INT_EXT_L2H               0x50009001
#define INT_EXT_H2L               0x60009001
#define INT_EXT                   0x309001
#define INT_TIMER0                0x309020
#define INT_BUSCOL                0x309104
#define INT_NCO                   0x309201
#define INT_BUSCOL2               0x309204
#define INT_SSP2                  0x309208
#define INT_TIMER6                0x309280
#define INT_TIMER3                0x309310
#define INT_TIMER3_GATE            0x309320
#define INT_CSW                   0x309340
#define INT_OSC_FAIL              0x309380
#define INT_CCP1                  0x309401
#define INT_CCP2                  0x309402
#define INT_CCP3                  0x309404
#define INT_CCP4                  0x309408
#define INT_CWG                   0x309440
#define INT_CWG2                  0x309480
#define INT_IOC                   0x3FFF9010
#define INT_IOC_A0                0x30019010
#define INT_IOC_A0_L2H            0x10019010
#define INT_IOC_A0_H2L            0x20019010
#define INT_IOC_A1                0x30029010
#define INT_IOC_A1_L2H            0x10029010
#define INT_IOC_A1_H2L            0x20029010
#define INT_IOC_A2                0x30049010
#define INT_IOC_A2_L2H            0x10049010
#define INT_IOC_A2_H2L            0x20049010
#define INT_IOC_A3                0x30089010
#define INT_IOC_A3_L2H            0x10089010
#define INT_IOC_A3_H2L            0x20089010
#define INT_IOC_A4                0x30109010
#define INT_IOC_A4_L2H            0x10109010
#define INT_IOC_A4_H2L            0x20109010
#define INT_IOC_A5                0x30209010
#define INT_IOC_A5_L2H            0x10209010
#define INT_IOC_A5_H2L            0x20209010
#define INT_IOC_B4                0x31109010
#define INT_IOC_B4_L2H            0x11109010
#define INT_IOC_B4_H2L            0x21109010
#define INT_IOC_B5                0x31209010
#define INT_IOC_B5_L2H            0x11209010
#define INT_IOC_B5_H2L            0x21209010
#define INT_IOC_B6                0x31409010
#define INT_IOC_B6_L2H            0x11409010
#define INT_IOC_B6_H2L            0x21409010
#define INT_IOC_B7                0x31809010
#define INT_IOC_B7_L2H            0x11809010
#define INT_IOC_B7_H2L            0x21809010
#define INT_IOC_C0                0x32019010
#define INT_IOC_C0_L2H            0x12019010
#define INT_IOC_C0_H2L            0x22019010
#define INT_IOC_C1                0x32029010
#define INT_IOC_C1_L2H            0x12029010
#define INT_IOC_C1_H2L            0x22029010
#define INT_IOC_C2                0x32049010
#define INT_IOC_C2_L2H            0x12049010
#define INT_IOC_C2_H2L            0x22049010
#define INT_IOC_C3                0x32089010
#define INT_IOC_C3_L2H            0x12089010
#define INT_IOC_C3_H2L            0x22089010
#define INT_IOC_C4                0x32109010
#define INT_IOC_C4_L2H            0x12109010
#define INT_IOC_C4_H2L            0x22109010
#define INT_IOC_C5                0x32209010
#define INT_IOC_C5_L2H            0x12209010
#define INT_IOC_C5_H2L            0x22209010
#define INT_IOC_C6                0x32409010
#define INT_IOC_C6_L2H            0x12409010
#define INT_IOC_C6_H2L            0x22409010
#define INT_IOC_C7                0x32809010
#define INT_IOC_C7_L2H            0x12809010
#define INT_IOC_C7_H2L            0x22809010

//The following defines are provided for compatabilty with older compiler versions
//INT_IOC define should be used to create Interrupt on Change ISR, and the INT_IOC_xx,
//INT_IOC_xx_L2H and INT_IOC_xx_H2L defines should be used in the ENABLE_INTERRUPTS(),
//DISABLE_INTERRUPTS(), CLEAR_INTERRUPT(), INTERRUPT_ACTIVE() and INTERRUPT_ACTIVE()
//function calls.
#define INT_RA                    0x30FF9010
#define INT_RA0                   0x30019010
#define INT_RA0_L2H               0x10019010
#define INT_RA0_H2L               0x20019010
#define INT_RA1                   0x30029010
#define INT_RA1_L2H               0x10029010
#define INT_RA1_H2L               0x20029010
#define INT_RA2                   0x30049010
#define INT_RA2_L2H               0x10049010
#define INT_RA2_H2L               0x20049010
#define INT_RA3                   0x30089010
#define INT_RA3_L2H               0x10089010
#define INT_RA3_H2L               0x20089010
#define INT_RA4                   0x30109010
#define INT_RA4_L2H               0x10109010
#define INT_RA4_H2L               0x20109010
#define INT_RA5                   0x30209010
#define INT_RA5_L2H               0x10209010
#define INT_RA5_H2L               0x20209010
#define INT_RB                    0x30FF9010
#define INT_RB4                   0x31109010
#define INT_RB4_L2H               0x11109010
#define INT_RB4_H2L               0x21109010
#define INT_RB5                   0x31209010
#define INT_RB5_L2H               0x11209010
#define INT_RB5_H2L               0x21209010
#define INT_RB6                   0x31409010
#define INT_RB6_L2H               0x11409010
#define INT_RB6_H2L               0x21409010
#define INT_RB7                   0x31809010
#define INT_RB7_L2H               0x11809010
#define INT_RB7_H2L               0x21809010
#define INT_RC                    0x30FF9010
#define INT_RC0                   0x32019010
#define INT_RC0_L2H               0x12019010
#define INT_RC0_H2L               0x22019010
#define INT_RC1                   0x32029010
#define INT_RC1_L2H               0x12029010
#define INT_RC1_H2L               0x22029010
#define INT_RC2                   0x32049010
#define INT_RC2_L2H               0x12049010
#define INT_RC2_H2L               0x22049010
#define INT_RC3                   0x32089010
#define INT_RC3_L2H               0x12089010
#define INT_RC3_H2L               0x22089010
#define INT_RC4                   0x32109010
#define INT_RC4_L2H               0x12109010
#define INT_RC4_H2L               0x22109010
#define INT_RC5                   0x32209010
#define INT_RC5_L2H               0x12209010
#define INT_RC5_H2L               0x22209010
#define INT_RC6                   0x32409010
#define INT_RC6_L2H               0x12409010
#define INT_RC6_H2L               0x22409010
#define INT_RC7                   0x32809010
#define INT_RC7_L2H               0x12809010
#define INT_RC7_H2L               0x22809010

#list
