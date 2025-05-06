//////////// Standard Header file for the PIC16F15385 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16F15385

#nolist
//////// Program memory: 8192x14  Data RAM: 1008  Stack: 16
//////// I/O: 44   Analog Pins: 43
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: LP,XT,HS,NOEXTOSC,ECL,ECM,ECH,RSTOSC_HFINTRC_32MHZ
//////// Fuses: RSTOSC_HFINTRC_PLL,RSTOSC_EXT_PLL,RSTOSC_SOSC,RSTOSC_LFINTRC
//////// Fuses: RSTOSC_HFINTRC,RSTOSC_EXT,CLKOUT,NOCLKOUT,CKS,NOCKS,FCMEN
//////// Fuses: NOFCMEN,MCLR,NOMCLR,PUT,NOPUT,LPBOR,NOLPBOR,NOBROWNOUT
//////// Fuses: BROWNOUT_SW,BROWNOUT_NOSL,BROWNOUT,BORV27,BORV24,ZCDDIS
//////// Fuses: NOZCDDIS,PPS1WAY,NOPPS1WAY,STVREN,NOSTVREN,DEBUG,NODEBUG
//////// Fuses: WDT64,WDT128,WDT256,WDT512,WDT1024,WDT2048,WDT4096,WDT8192
//////// Fuses: WDT16384,WDT32768,WDT65536,WDT131072,WDT262144,WDT524299
//////// Fuses: WDT1048576,WDT2097152,WDT4194304,WDT8388608,WDT32,WDTSW
//////// Fuses: NOWDT,WDT_SW,WDT_NOSL,WDT,WDTWIN_12%,WDTWIN_25%,WDTWIN_37%
//////// Fuses: WDTWIN_50%,WDTWIN_62%,WDTWIN_75%,WDTWIN_100%,WDTWIN_SW
//////// Fuses: WDTCLK_LFINTRC,WDTCLK_HFINTRC,WDTCLK_SOSC,WDTCLK_SW,BBSIZ8K
//////// Fuses: BBSIZ4K,BBSIZ2K,BBSIZ1K,BBSIZ512,BOOTBLOCK,NOBOOTBLOCK,SAF
//////// Fuses: NOSAF,WRT,NOWRT,WRTB,NOWRTB,WRTC,NOWRTC,WRTSAF,NOWRTSAF,LVP
//////// Fuses: NOLVP,PROTECT,NOPROTECT
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
//    PIN_A0,PIN_A1,PIN_A2,PIN_A3,PIN_A4,PIN_A5,PIN_A6,PIN_A7,PIN_B0,PIN_B1,
//    PIN_B2,PIN_B3,PIN_B4,PIN_B5,PIN_B6,PIN_B7,PIN_C0,PIN_C1,PIN_C2,PIN_C3,
//    PIN_C4,PIN_C5,PIN_C6,PIN_C7,PIN_D0,PIN_D1,PIN_D2,PIN_D3,PIN_D4,PIN_D5,
//    PIN_D6,PIN_D7,PIN_E0,PIN_E1,PIN_E2,PIN_F0,PIN_F1,PIN_F2,PIN_F3,PIN_F4,
//    PIN_F5,PIN_F6,PIN_F7
// Input Functions: 
//    INT0,T0CK,T1CK,T1G,T2CK,CCP1,CCP2,CWGIN,CLCIN0,CLCIN1,CLCIN2,CLCIN3,ADACT,
//    SCK1IN,SCL1IN,SDI1,SDA1IN,SS1IN,SCK2IN,SCL2IN,SDI2,SDA2IN,SS2IN,U1RX,U1CK,
//    U2RX,U2CK,T0CKI,T1CKI,T2CKI,CCP1IN,CCP2IN,RX1,CK1,RX2,CK2
// Output Functions: 
//    NULL,CLC1OUT,CLC2OUT,CLC3OUT,CLC4OUT,CWG1OUTA,CWG1OUTB,CWG1OUTC,CWG1OUTD,
//    CCP1OUT,CCP2OUT,PWM3OUT,PWM4OUT,PWM5OUT,PWM6OUT,U1TX,U1DT,U2TX,U2DT,C1OUT,
//    C2OUT,SCK1OUT,SCL1OUT,SDO1,SDA1OUT,SCK2OUT,SCL2OUT,SDO2,SDA2OUT,T0OUT,
//    NCO1OUT,CLKROUT,PWM3,PWM4,PWM5,PWM6,TX1,DT1,TX2,DT2,SCK1,SCL1,SDA1,SCK2,
//    SCL2,SDA2,TMR0OUT,NCO1
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
_bif void set_tris_d(int8 value);  
_bif void set_tris_e(int8 value);  
_bif void set_tris_f(int8 value);  
_bif void set_input_level_a(int8 value);  
_bif void set_input_level_b(int8 value);  
_bif void set_input_level_c(int8 value);  
_bif void set_input_level_d(int8 value);  
_bif void set_input_level_e(int8 value);  
_bif void set_input_level_f(int8 value);  
_bif void set_open_drain_a(int8 value);  
_bif void set_open_drain_b(int8 value);  
_bif void set_open_drain_c(int8 value);  
_bif void set_open_drain_d(int8 value);  
_bif void set_open_drain_e(int8 value);  
_bif void set_open_drain_f(int8 value);  
_bif int8 get_tris_a(void);  
_bif int8 get_tris_b(void);  
_bif int8 get_tris_c(void);  
_bif int8 get_tris_d(void);  
_bif int8 get_tris_e(void);  
_bif int8 get_tris_f(void);  
_bif void output_a(int8 value);  
_bif void output_b(int8 value);  
_bif void output_c(int8 value);  
_bif void output_d(int8 value);  
_bif void output_e(int8 value);  
_bif void output_f(int8 value);  
_bif int8 input_a(void);  
_bif int8 input_b(void);  
_bif int8 input_c(void);  
_bif int8 input_d(void);  
_bif int8 input_e(void);  
_bif int8 input_f(void);  
_bif int8 input_change_a(void);  
_bif int8 input_change_b(void);  
_bif int8 input_change_c(void);  
_bif int8 input_change_d(void);  
_bif int8 input_change_e(void);  
_bif int8 input_change_f(void);  
_bif void port_a_pullups(int8 upmask);   
_bif void port_b_pullups(int8 upmask);  
_bif void port_c_pullups(int8 upmask);   
_bif void port_d_pullups(int8 upmask);   
_bif void port_e_pullups(int8 upmask);   
_bif void port_f_pullups(int8 upmask);   
_bif void set_slow_slew_a(int8 value); 
_bif void set_slow_slew_b(int8 value); 
_bif void set_slow_slew_c(int8 value); 
_bif void set_slow_slew_d(int8 value); 
_bif void set_slow_slew_e(int8 value); 
_bif void set_slow_slew_f(int8 value); 
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
#define PIN_A6  102
#define PIN_A7  103

#define PIN_B0  104
#define PIN_B1  105
#define PIN_B2  106
#define PIN_B3  107
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

#define PIN_D0  120
#define PIN_D1  121
#define PIN_D2  122
#define PIN_D3  123
#define PIN_D4  124
#define PIN_D5  125
#define PIN_D6  126
#define PIN_D7  127

#define PIN_E0  128
#define PIN_E1  129
#define PIN_E2  130
#define PIN_E3  131

#define PIN_F0  136
#define PIN_F1  137
#define PIN_F2  138
#define PIN_F3  139
#define PIN_F4  140
#define PIN_F5  141
#define PIN_F6  142
#define PIN_F7  143

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
_bif int16 restart_cause(void); 
_bif void reset_cpu(void);
_bif void sleep(void);
// Constants returned from RESTART_CAUSE() are:
#define RESTART_NONE         0x73F //No restart occurred, most common cause for returning this is restart_cause() was called multiple times. 
#define NORMAL_POWER_UP      0x73C 
#define BROWNOUT_RESTART     0x73E 
#define MCLR_FROM_SLEEP      0x637 
#define WDT_TIMEOUT          0x52F 
#define WDT_FROM_SLEEP       0x43F 
#define INTERRUPT_FROM_SLEEP 0x63F 
#define MCLR_FROM_RUN        0x737 
#define RESET_INSTRUCTION    0x33B 
#define STACK_OVERFLOW       0x7BF 
#define STACK_UNDERFLOW      0x77F 
#define WDT_WINDOW_VIOLATION 0x71F 
#define MEMORY_VIOLATION     0x33F 

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
#define T0_MFINTOSC               0x80A0 //500 kHz 
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
// Watch Dog Timer Functions: SETUP_WDT()
//                            RESTART_WDT()
//                            GET_WDT()
// WDT base is 1ms
// WDT Prototypes:
_bif void setup_wdt(int16 mode);
_bif void restart_wdt(void);
_bif int8 get_wdt(void);
// Constants used for SETUP_WDT() are:
// Only use one of the following to turn WDT On or Off
#define WDT_ON                  0x8000
#define WDT_OFF                 0x0000

// Or use one of the following to enable WDT and set timeout time
#define WDT_1MS                 0x0001
#define WDT_2MS                 0x0003
#define WDT_4MS                 0x0005
#define WDT_8MS                 0x0007
#define WDT_16MS                0x0009
#define WDT_32MS                0x000B
#define WDT_64MS                0x000D
#define WDT_128MS               0x000F
#define WDT_256MS               0x0011
#define WDT_512MS               0x0013
#define WDT_1S                  0x0015
#define WDT_2S                  0x0017
#define WDT_4S                  0x0019  
#define WDT_8S                  0x001B
#define WDT_16S                 0x001D
#define WDT_32S                 0x001F
#define WDT_64S                 0x0021
#define WDT_128S                0x0023
#define WDT_256S                0x0025

// One of the following may be OR'ed in with the above using |
#define WDT_WINDOW_12_PERCENT   0x0800
#define WDT_WINDOW_25_PERCENT   0x0100
#define WDT_WINDOW_37_PERCENT   0x0200
#define WDT_WINDOW_50_PERCENT   0x0300
#define WDT_WINDOW_62_PERCENT   0x0400
#define WDT_WINDOW_75_PERCENT   0x0500
#define WDT_WINDOW_87_PERCENT   0x0600
#define WDT_WINDOW_100_PERCENT  0x0700

// One of the following may be OR'ed in with the above using |
#define WDT_CLK_31000           0x0000
#define WDT_CLK_31250           0x1000

#bit WDTSTATE = getenv("SFR:WDTTMR").2 

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Timer 1 Prototypes:
_bif void setup_timer_1(int16 mode); 
_bif int16 get_timer1(void);
_bif void set_timer1(int16 value);
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_EXTERNAL         0x0003 
#define T1_INTERNAL         0x0103 
#define T1_FOSC             0x0203 
#define T1_HFINTRC          0x0303 
#define T1_LFINTRC          0x0403 
#define T1_MFINTRC          0x0503 
#define T1_MFINTRC_32KHZ    0x0603 
#define T1_SOSC             0x0703 
#define T1_CLKR             0x0803 
#define T1_TIMER0           0x0903 
#define T1_CLC1             0x0A03 
#define T1_CLC2             0x0B03 
#define T1_CLC3             0x0C03 
#define T1_CLC4             0x0D03 

#define T1_SYNC             0x04  

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE             0x8000
#define T1_GATE_INVERTED    0xC000
#define T1_GATE_TOGGLE      0xA000
#define T1_GATE_SINGLE      0x9000

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Timer 2 Prototypes:
_bif void setup_timer_2(int32 mode, int8 period, int8 postscaler);
_bif int8 get_timer2(void);
_bif void set_timer2(int8 value);
//Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED                                0
#define T2_DIV_BY_1                                0x80
#define T2_DIV_BY_2                                0x90
#define T2_DIV_BY_4                                0xA0
#define T2_DIV_BY_8                                0xB0
#define T2_DIV_BY_16                               0xC0
#define T2_DIV_BY_32                               0xD0
#define T2_DIV_BY_64                               0xE0
#define T2_DIV_BY_128                              0xF0
//One of the following may be OR'ed with the above
#define T2_CLK_T2IN                                0  
#define T2_CLK_INTERNAL                            0x0100 //FOSC/4  
#define T2_CLK_FOSC                                0x0200  
#define T2_CLK_HFINTRC                             0x0300  
#define T2_CLK_LFINTRC                             0x0400  
#define T2_CLK_MFINTRC_500KHZ                      0x0500 
#define T2_CLK_MFINTRC_31_25KHZ                    0x0600 
#define T2_CLK_SOSC                                0x0700 
#define T2_CLK_CLKR                                0x0800 
#define T2_CLK_NCO                                 0x0900 
#define T2_CLK_ZCD                                 0x0A00 
#define T2_CLK_CLC1                                0x0B00 
#define T2_CLK_CLC2                                0x0C00 
#define T2_CLK_CLC3                                0x0D00 
#define T2_CLK_CLC4                                0x0E00 

//One of the following may be OR'ed with the above for setting up the Hardware Limit Timer
#define T2_START_IMMEDIATELY                       0
#define T2_START_WHEN_HIGH                         0x010000
#define T2_START_WHEN_LOW                          0x020000
#define T2_RESET_ON_BOTH_EDGES                     0x030000
#define T2_RESET_ON_RE                             0x040000
#define T2_RESET_ON_FE                             0x050000
#define T2_RESET_WHEN_LOW                          0x060000
#define T2_RESET_WHEN_HIGH                         0x070000
#define T2_ONE_SHOT_START_IMMEDIATELY              0x080000
#define T2_ONE_SHOT_START_ON_RE                    0x090000
#define T2_ONE_SHOT_START_ON_FE                    0x0A0000
#define T2_ONE_SHOT_START_ON_BOTH_EDGES            0x0B0000
#define T2_ONE_SHOT_START_ON_RE_RESET_ON_RE        0x0C0000 //starts on first rising edge, resets on subsequent rising edges
#define T2_ONE_SHOT_START_ON_FE_RESET_ON_FE        0x0D0000 //starts on first falling edge, resets on subsequent falling edges
#define T2_ONE_SHOT_START_ON_RE_RESET_WHEN_LOW     0x0E0000
#define T2_ONE_SHOT_START_ON_FE_RESET_WHEN_HIGH    0x0F0000
#define T2_MONO_STABLE_START_ON_RE                 0x110000  
#define T2_MONO_STABLE_START_ON_FE                 0x120000  
#define T2_MONO_STABLE_START_ON_EE                 0x130000  
#define T2_ONE_SHOT_START_WHEN_HIGH_RESET_WHEN_LOW 0x160000  
#define T2_ONE_SHOT_START_WHEN_LOW_RESET_WHEN_HIGH 0x170000  
//Any of the following may be OR'ed with the above
#define T2_SYNC_ON_WITH_T2CLK                      0x200000
#define T2_INVERTED                                0x400000
#define T2_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T2_RESET_FROM_T2IN                         0
#define T2_RESET_FROM_CCP1                         0x01000000 
#define T2_RESET_FROM_CCP2                         0x02000000 
#define T2_RESET_FROM_PWM3                         0x03000000 
#define T2_RESET_FROM_PWM4                         0x04000000 
#define T2_RESET_FROM_PWM5                         0x05000000 
#define T2_RESET_FROM_PWM6                         0x06000000 
#define T2_RESET_FROM_C1OUT                        0x07000000 
#define T2_RESET_FROM_C2OUT                        0x08000000  
#define T2_RESET_FROM_ZCD                          0x09000000 
#define T2_RESET_FROM_CLC1                         0x0A000000 
#define T2_RESET_FROM_CLC2                         0x0B000000 
#define T2_RESET_FROM_CLC3                         0x0C000000 
#define T2_RESET_FROM_CLC4                         0x0D000000 

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
#define CCP_PWM                          0x0C    
// One of the following may be OR'ed in with the above when using Capture mode 
#define CCP_CAPTURE_INPUT_CCP_PIN        0 
#define CCP_CAPTURE_INPUT_C1OUT          0x010000 
#define CCP_CAPTURE_INPUT_C2OUT          0x020000  
#define CCP_CAPTURE_INPUT_IOC_INTERRUPT  0x030000 
#define CCP_CAPTURE_INPUT_CLC1           0x040000 
#define CCP_CAPTURE_INPUT_CLC2           0x050000 
#define CCP_CAPTURE_INPUT_CLC3           0x060000 
#define CCP_CAPTURE_INPUT_CLC4           0x070000 
// The following may be OR'ed in with the above when using PWM mode 
#define CCP_PWM_LEFT_JUSTIFIED_DUTY      0x10 

#word   CCP_1       =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_LOW   =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_HIGH  =                    getenv("SFR:CCPR1H") 



// CCP2 Prototypes:
_bif void setup_ccp2(int32 mode);
_bif void setup_ccp2(int32 mode, int8 pwm); 
_bif void set_pwm2_duty(int8 value); 
_bif void set_pwm2_duty(int16 value); 
#word   CCP_2       =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_LOW   =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_HIGH  =                    getenv("SFR:CCPR2H") 

////////////////////////////////////////////////////////////////// PWM
// PWM Functions: SETUP_PWMx, SET_PWMx_DUTY
// PWM Prototypes:
_bif void setup_pwm3(int32 mode);   
_bif void set_pwm3_duty(int8 value);   
_bif void set_pwm3_duty(int16 value);   
_bif void setup_pwm4(int32 mode);   
_bif void set_pwm4_duty(int8 value);   
_bif void set_pwm4_duty(int16 value);   
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

#bit PWM3OUT=getenv("BIT:PWM3OUT")   
#bit PWM4OUT=getenv("BIT:PWM4OUT")   
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

//Pick one constant for COMP1
#define CP1_A0_A2       0x8000
#define CP1_A1_A2       0x8001
#define CP1_B3_A2       0x8002
#define CP1_B1_A2       0x8003
#define CP1_FVR_A2      0x8006
#define CP1_VSS_A2      0x8007
#define CP1_A0_A3       0x8008
#define CP1_A1_A3       0x8009
#define CP1_B3_A3       0x800A
#define CP1_B1_A3       0x800B
#define CP1_FVR_A3      0x800E
#define CP1_VSS_A3      0x800F
#define CP1_A0_DAC      0x8028
#define CP1_A1_DAC      0x8029
#define CP1_B3_DAC      0x802A
#define CP1_B1_DAC      0x802B
#define CP1_FVR_DAC     0x802E
#define CP1_VSS_DAC     0x802F
#define CP1_A0_FVR      0x8030
#define CP1_A1_FVR      0x8031
#define CP1_B3_FVR      0x8032
#define CP1_B1_FVR      0x8033
#define CP1_FVR_FVR     0x8036
#define CP1_VSS_FVR     0x8037
#define CP1_A0_VSS      0x8038
#define CP1_A1_VSS      0x8039
#define CP1_B3_VSS      0x803A
#define CP1_B1_VSS      0x803B
#define CP1_FVR_VSS     0x803E
#define CP1_VSS_VSS     0x803F
//Optionally OR any of the following with the above
#define CP1_INT_L2H     0x0080
#define CP1_INT_H2L     0x0040
#define CP1_INVERT      0x1000
#define CP1_HYST        0x0200
#define CP1_SYNC        0x0100

//Pick one constant for COMP2
#define CP2_A0_A2       0x80000000
#define CP2_A1_A2       0x80010000
#define CP2_B3_A2       0x80020000
#define CP2_B1_A2       0x80030000
#define CP2_FVR_A2      0x80060000
#define CP2_VSS_A2      0x80070000
#define CP2_A0_B0       0x80080000
#define CP2_A1_B0       0x80090000
#define CP2_B3_B0       0x800A0000
#define CP2_B1_B0       0x800B0000
#define CP2_FVR_B0      0x800E0000
#define CP2_VSS_B0      0x800F0000
#define CP2_A0_DAC      0x80280000
#define CP2_A1_DAC      0x80290000
#define CP2_B3_DAC      0x802A0000
#define CP2_B1_DAC      0x802B0000
#define CP2_FVR_DAC     0x802E0000
#define CP2_VSS_DAC     0x802F0000
#define CP2_A0_FVR      0x80300000
#define CP2_A1_FVR      0x80310000
#define CP2_B3_FVR      0x80320000
#define CP2_B1_FVR      0x80330000
#define CP2_FVR_FVR     0x80360000
#define CP2_VSS_FVR     0x80370000
#define CP2_A0_VSS      0x80380000
#define CP2_A1_VSS      0x80390000
#define CP2_B3_VSS      0x803A0000
#define CP2_B1_VSS      0x803B0000
#define CP2_FVR_VSS     0x803E0000
#define CP2_VSS_VSS     0x803F0000
//Optionally OR with any of the following
#define CP2_INT_L2H     0x00800000
#define CP2_INT_H2L     0x00400000
#define CP2_INVERT      0x10000000
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
#define DAC_OUTPUT2   0x10  
#define DAC_OUTPUT    0x20 

////////////////////////////////////////////////////////////////// CWG
// CWG Functions: SETUP_CWG(), CWG_STATUS(), CWG_RESTART()
// CWG Prototypes:
_bif void setup_cwg(int32 mode, int16 shutdown, int8 dt_rising, int8 dt_falling);
_bif void cwg_restart(void);
_bif int8 cwg_status(void);
// Constants used in SETUP_CWG() first parameter are:      
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
#define CWG_INPUT_CWGCLK               0x0000 
#define CWG_INPUT_CCP1                 0x1000 
#define CWG_INPUT_CCP2                 0x2000 
#define CWG_INPUT_PWM3                 0x3000 
#define CWG_INPUT_PWM4                 0x4000 
#define CWG_INPUT_PWM5                 0x5000 
#define CWG_INPUT_PWM6                 0x6000 
#define CWG_INPUT_NCO                  0x7000 
#define CWG_INPUT_COMP1                0x8000 
#define CWG_INPUT_COMP2                0x9000  
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

// Constants used in SETUP_CWG() second parameter are:      
// Any of the following may be OR'ed together using |
#define CWG_NO_AUTO_SHUTDOWN           0x0000
#define CWG_SHUTDOWN_ON_CWG1IN         0x0001      
#define CWG_SHUTDOWN_ON_TMR2           0x0002 
#define CWG_SHUTDOWN_ON_COMP1          0x0004 
#define CWG_SHUTDOWN_ON_COMP2          0x0008  
#define CWG_SHUTDOWN_ON_CLC2           0x0010 
#define CWG_AUTO_RESTART               0x4000 
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

// Constants returned from CWG_STATUS() are:      
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
#define NCO_CLOCK_FOSC              0 
#define NCO_CLOCK_HFINTOSC          0x0100 
#define NCO_CLOCK_LFINTOSC          0x0200 
#define NCO_CLOCK_MFINTOSC_500KHZ   0x0300 
#define NCO_CLOCK_MFINTOSC          0x0400 
#define NCO_CLOCK_SOSC              0x0500 
#define NCO_CLOCK_CLKR              0x0600 
#define NCO_CLOCK_CLC1              0x0700 
#define NCO_CLOCK_CLC2              0x0800 
#define NCO_CLOCK_CLC3              0x0900 
#define NCO_CLOCK_CLC4              0x0A00 

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
#define CLC_INPUT_CLCIN0                    0x00 
#define CLC_INPUT_CLCIN1                    0x01 
#define CLC_INPUT_CLCIN2                    0x02 
#define CLC_INPUT_CLCIN3                    0x03 
#define CLC_INPUT_FOSC                      0x04 
#define CLC_INPUT_HFINTOSC                  0x05 
#define CLC_INPUT_LFINTOSC                  0x06 
#define CLC_INPUT_MFINTOSC_500KHZ           0x07 
#define CLC_INPUT_MFINTOSC                  0x08 
#define CLC_INPUT_SOSC                      0x09 
#define CLC_INPUT_ADCRC                     0x0A 
#define CLC_INPUT_CLKR                      0x0B 
#define CLC_INPUT_TIMER0                    0x0C 
#define CLC_INPUT_TIMER1                    0x0D 
#define CLC_INPUT_TIMER2                    0x0E 
#define CLC_INPUT_CCP1                      0x0F 
#define CLC_INPUT_CCP2                      0x10 
#define CLC_INPUT_PWM3                      0x11 
#define CLC_INPUT_PWM4                      0x12 
#define CLC_INPUT_PWM5                      0x13 
#define CLC_INPUT_PWM6                      0x14 
#define CLC_INPUT_NCO1                      0x15 
#define CLC_INPUT_C1OUT                     0x16 
#define CLC_INPUT_C2OUT                     0x17  
#define CLC_INPUT_ZCD                       0x18 
#define CLC_INPUT_IOCIF                     0x19 
#define CLC_INPUT_CLC1                      0x1A 
#define CLC_INPUT_CLC2                      0x1B 
#define CLC_INPUT_CLC3                      0x1C 
#define CLC_INPUT_CLC4                      0x1D 
#define CLC_INPUT_U1DT                      0x1E 
#define CLC_INPUT_U1TX                      0x1F 
#define CLC_INPUT_U2DT                      0x20 
#define CLC_INPUT_U2TX                      0x21 
#define CLC_INPUT_SDO1                      0x22 
#define CLC_INPUT_SCK1                      0x23 
#define CLC_INPUT_CWG1A                     0x26 
#define CLC_INPUT_CWG1B                     0x27 

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

////////////////////////////////////////////////////////////////// ZCD
// Zero-Cross Detection (ZCD) Functions: SETUP_ZCD(), ZCD_STATUS()
// ZCD Prototypes:
_bif void setup_zcd(int8 mode);
_bif int8 zcd_status(void);
// Constants used in SETUP_ZCD() are:
#define ZCD_ENABLE          0x80
#define ZCD_DISABLE         0
// Any of the following may be OR'ed in with the above using |
#define ZCD_INVERTED        0x10
#define ZCD_INT_L_TO_H      0x02
#define ZCD_INT_H_TO_L      0x01

// Constants returned from ZCD_STATUS() are:
#define ZCD_IS_SINKING      0x20  //If ZCD_INVERTED is used the return
#define ZCD_IS_SOURCING     0     //value will be reversed.


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes
_bif int16 setup_oscillator(int32 mode);
_bif int16 setup_oscillator(int32 mode, signed int8 tune);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_HFINTRC_1MHZ             0x00000060
#define OSC_HFINTRC_2MHZ             0x00000160
#define OSC_HFINTRC_4MHZ             0x00000260 
#define OSC_HFINTRC_8MHZ             0x00000360 
#define OSC_HFINTRC_12MHZ            0x00000460 
#define OSC_HFINTRC_16MHZ            0x00000560 
#define OSC_HFINTRC_24MHZ            0x00000410  
#define OSC_HFINTRC_32MHZ            0x00000660 
#define OSC_EXTOSC_PLL               0x00000020 
#define OSC_SOSC                     0x00000040 
#define OSC_LFINTRC                  0x00000050 
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
// The following may be OR'ed in with the above using |
#define OSC_CLOCK_SWITCH_HOLD        0x00800000
#define OSC_ADOSC_ENABLED            0x04000000
#define OSC_SOSC_ENABLED             0x08000000
#define OSC_LFINTRC_ENABLED          0x10000000
#define OSC_MFINTRC_ENABLED          0x20000000 
#define OSC_HFINTRC_ENABLED          0x40000000
#define OSC_EXTOSC_ENABLED           0x80000000
// A second optional parameter may be used with this part to fine
// tune the speed (signed int,-32 to 31)
// Result may be (ignore all other bits)
#define OSC_PLL_READY                0x0001
#define OSC_ADCRC_READY              0x0004
#define OSC_SOSC_READY               0x0008
#define OSC_LFINTRC_READY            0x0010
#define OSC_MFINTRC_READY            0x0020 
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
#define sAN0          0x01000000  //| A0
#define sAN1          0x02000000  //| A1
#define sAN2          0x04000000  //| A2
#define sAN3          0x08000000  //| A3 
#define sAN4          0x10000000  //| A4
#define sAN5          0x20000000  //| A5
#define sAN6          0x40000000  //| A6 
#define sAN7          0x80000000  //| A7 
#define sAN8          0x00010000  //| B0 
#define sAN9          0x00020000  //| B1 
#define sAN10         0x00040000  //| B2 
#define sAN11         0x00080000  //| B3 
#define sAN12         0x00100000  //| B4 
#define sAN13         0x00200000  //| B5 
#define sAN14         0x00400000  //| B6 
#define sAN15         0x00800000  //| B7 
#define sAN16         0x00000100  //| C0 
#define sAN17         0x00000200  //| C1 
#define sAN18         0x00000400  //| C2 
#define sAN19         0x00000800  //| C3 
#define sAN20         0x00001000  //| C4 
#define sAN21         0x00002000  //| C5 
#define sAN22         0x00004000  //| C6 
#define sAN23         0x00008000  //| C7 
#define sAN24         0x00000001  //| D0 
#define sAN25         0x00000002  //| D1 
#define sAN26         0x00000004  //| D2 
#define sAN27         0x00000008  //| D3 
#define sAN28         0x00000010  //| D4 
#define sAN29         0x00000020  //| D5 
#define sAN30         0x00000040  //| D6 
#define sAN31         0x00000080  //| D7 
#define NO_ANALOGS             0  // None
#define ALL_ANALOG    0xFFFFFFFF  // A0 A1 A2 A3 A4 A5 A6 A7 B0 B1 B2 B3 B4 B5 B6 B7 C0 C1 C2 C3 C4 C5 C6 C7 D0 D1 D2 D3 D4 D5 D6 D7 
// Second parameter: 
// OR together desired pins 
#define sAN32         0x00000100  //| E0 
#define sAN33         0x00000200  //| E1 
#define sAN34         0x00000400  //| E2 
#define sAN40         0x00000001  //| F0 
#define sAN41         0x00000002  //| F1 
#define sAN42         0x00000004  //| F2 
#define sAN43         0x00000008  //| F3 
#define sAN44         0x00000010  //| F4 
#define sAN45         0x00000020  //| F5 
#define sAN46         0x00000040  //| F6 
#define sAN47         0x00000080  //| F7 
#define NO_ANALOGS_P2          0  // None 
#define ALL_ANALOG_P2 0x000007FF  // E0 E1 E2 F0 F1 F2 F3 F4 F5 F6 F7 

// Optional Third parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE: 
#define VSS_VDD                0  //| Range 0 to Vdd
#define VSS_VREF      0x00000002  //| Range 0 to Vref+
#define VSS_FVR       0x00000003  //| Range 0 to FVR

// Constants used in SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define AVSS_CHANNEL               0x3B
#define TEMPERATURE_INDICATOR      0x3C
#define DAC_CHANNEL                0x3D
#define FVR_BUFFER1_CHANNEL        0x3E
#define FVR_BUFFER2_CHANNEL        0x3F

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
_bif void read_device_info(unsigned int8* dataptr, unsigned int8 count); 
_bif void read_device_info(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 
_bif void read_config_info(unsigned int8* dataptr, unsigned int8 count); 
_bif void read_config_info(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 
// Constants used in read_device_info() offset parameter: 
#define DINFO_MUI                     0 
#define DINFO_EUI                     10 
#define DINFO_TI_LOW_90C              18 
#define DINFO_TI_HIGH_90C             22 
#define DINFO_FVR1_1X                 24 
#define DINFO_FVR1_2X                 25 
#define DINFO_FVR1_4X                 26 
#define DINFO_FVR2_1X                 27 
#define DINFO_FVR2_2X                 28 
#define DINFO_FVR2_4X                 29 
// Constants used in read_device_info() count parameter: 
#define DINFO_MUI_BYTES               18 
#define DINFO_EUI_BYTES               16 
#define DINFO_T1_LOW_90C_BYTES        2 
#define DINFO_TI_HIGH_90C_BYTES       2 
#define DINFO_FVR1_1X_BYTES           2 
#define DINFO_FVR1_2X_BYTES           2 
#define DINFO_FVR1_4X_BYTES           2 
#define DINFO_FVR2_1X_BYTES           2 
#define DINFO_FVR2_2X_BYTES           2 
#define DINFO_FVR2_4X_BYTES           2 
 
// Constants used in read_config_info() offset parameter: 
#define CINFO_ERASE_ROW_SIZE          0 
#define CINFO_WRITE_LATCHES           1 
#define CINFO_ROWS                    2 
#define CINFO_EEPROM_SIZE             3 
#define CINFO_PIN_COUNT               4 
// Constants used in read_config_info() count parameter: 
#define CINFO_ERASE_ROW_SIZE_BYTES    2 
#define CINFO_WRITE_LATCHES_BYTES     2 
#define CINFO_ROWS_BYTES              2 
#define CINFO_EEPROM_SIZE_BYTES       2 
#define CINFO_PIN_COUNT_BYTES         2 

// Program Memory Write Prototypes:
_bif void write_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif void write_program_eeprom(__ADDRESS__ address, unsigned int16 data);
_bif void write_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void write_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 

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
#define INT_EXT_L2H               0x50001601
#define INT_EXT_H2L               0x60001601
#define INT_EXT                   0x301601
#define INT_TIMER0                0x301620
#define INT_AD                    0x301701
#define INT_CSW                   0x301740
#define INT_OSC_FAIL              0x301780
#define INT_COMP                  0x301801
#define INT_COMP2                 0x301802
#define INT_ZCD                   0x301840
#define INT_SSP                   0x301901
#define INT_BUSCOL                0x301902
#define INT_SSP2                  0x301904
#define INT_BUSCOL2               0x301908
#define INT_TBE                   0x301910
#define INT_RDA                   0x301920
#define INT_TIMER1                0x301A01
#define INT_TIMER2                0x301A02
#define INT_TIMER1_GATE            0x301B01
#define INT_CLC                   0x301B10
#define INT_CLC2                  0x301B20
#define INT_CLC3                  0x301B40
#define INT_CLC4                  0x301B80
#define INT_CCP1                  0x301C01
#define INT_CCP2                  0x301C02
#define INT_CWG                   0x301D01
#define INT_NCO                   0x301D10
#define INT_TBE2                  0x301940
#define INT_RDA2                  0x301980
#define INT_NVM                   0x301D20
#define INT_IOC                   0x3FFF1610
#define INT_IOC_A0                0x30011610
#define INT_IOC_A0_L2H            0x10011610
#define INT_IOC_A0_H2L            0x20011610
#define INT_IOC_A1                0x30021610
#define INT_IOC_A1_L2H            0x10021610
#define INT_IOC_A1_H2L            0x20021610
#define INT_IOC_A2                0x30041610
#define INT_IOC_A2_L2H            0x10041610
#define INT_IOC_A2_H2L            0x20041610
#define INT_IOC_A3                0x30081610
#define INT_IOC_A3_L2H            0x10081610
#define INT_IOC_A3_H2L            0x20081610
#define INT_IOC_A4                0x30101610
#define INT_IOC_A4_L2H            0x10101610
#define INT_IOC_A4_H2L            0x20101610
#define INT_IOC_A5                0x30201610
#define INT_IOC_A5_L2H            0x10201610
#define INT_IOC_A5_H2L            0x20201610
#define INT_IOC_A6                0x30401610
#define INT_IOC_A6_L2H            0x10401610
#define INT_IOC_A6_H2L            0x20401610
#define INT_IOC_A7                0x30801610
#define INT_IOC_A7_L2H            0x10801610
#define INT_IOC_A7_H2L            0x20801610
#define INT_IOC_B0                0x31011610
#define INT_IOC_B0_L2H            0x11011610
#define INT_IOC_B0_H2L            0x21011610
#define INT_IOC_B1                0x31021610
#define INT_IOC_B1_L2H            0x11021610
#define INT_IOC_B1_H2L            0x21021610
#define INT_IOC_B2                0x31041610
#define INT_IOC_B2_L2H            0x11041610
#define INT_IOC_B2_H2L            0x21041610
#define INT_IOC_B3                0x31081610
#define INT_IOC_B3_L2H            0x11081610
#define INT_IOC_B3_H2L            0x21081610
#define INT_IOC_B4                0x31101610
#define INT_IOC_B4_L2H            0x11101610
#define INT_IOC_B4_H2L            0x21101610
#define INT_IOC_B5                0x31201610
#define INT_IOC_B5_L2H            0x11201610
#define INT_IOC_B5_H2L            0x21201610
#define INT_IOC_B6                0x31401610
#define INT_IOC_B6_L2H            0x11401610
#define INT_IOC_B6_H2L            0x21401610
#define INT_IOC_B7                0x31801610
#define INT_IOC_B7_L2H            0x11801610
#define INT_IOC_B7_H2L            0x21801610
#define INT_IOC_C0                0x32011610
#define INT_IOC_C0_L2H            0x12011610
#define INT_IOC_C0_H2L            0x22011610
#define INT_IOC_C1                0x32021610
#define INT_IOC_C1_L2H            0x12021610
#define INT_IOC_C1_H2L            0x22021610
#define INT_IOC_C2                0x32041610
#define INT_IOC_C2_L2H            0x12041610
#define INT_IOC_C2_H2L            0x22041610
#define INT_IOC_C3                0x32081610
#define INT_IOC_C3_L2H            0x12081610
#define INT_IOC_C3_H2L            0x22081610
#define INT_IOC_C4                0x32101610
#define INT_IOC_C4_L2H            0x12101610
#define INT_IOC_C4_H2L            0x22101610
#define INT_IOC_C5                0x32201610
#define INT_IOC_C5_L2H            0x12201610
#define INT_IOC_C5_H2L            0x22201610
#define INT_IOC_C6                0x32401610
#define INT_IOC_C6_L2H            0x12401610
#define INT_IOC_C6_H2L            0x22401610
#define INT_IOC_C7                0x32801610
#define INT_IOC_C7_L2H            0x12801610
#define INT_IOC_C7_H2L            0x22801610
#define INT_IOC_E0                0x34011610
#define INT_IOC_E0_L2H            0x14011610
#define INT_IOC_E0_H2L            0x24011610
#define INT_IOC_E1                0x34021610
#define INT_IOC_E1_L2H            0x14021610
#define INT_IOC_E1_H2L            0x24021610
#define INT_IOC_E2                0x34041610
#define INT_IOC_E2_L2H            0x14041610
#define INT_IOC_E2_H2L            0x24041610
#define INT_IOC_E3                0x34081610
#define INT_IOC_E3_L2H            0x14081610
#define INT_IOC_E3_H2L            0x24081610

//The following defines are provided for compatabilty with older compiler versions
//INT_IOC define should be used to create Interrupt on Change ISR, and the INT_IOC_xx,
//INT_IOC_xx_L2H and INT_IOC_xx_H2L defines should be used in the ENABLE_INTERRUPTS(),
//DISABLE_INTERRUPTS(), CLEAR_INTERRUPT(), INTERRUPT_ACTIVE() and INTERRUPT_ACTIVE()
//function calls.
#define INT_RA                    0x30FF1610
#define INT_RA0                   0x30011610
#define INT_RA0_L2H               0x10011610
#define INT_RA0_H2L               0x20011610
#define INT_RA1                   0x30021610
#define INT_RA1_L2H               0x10021610
#define INT_RA1_H2L               0x20021610
#define INT_RA2                   0x30041610
#define INT_RA2_L2H               0x10041610
#define INT_RA2_H2L               0x20041610
#define INT_RA3                   0x30081610
#define INT_RA3_L2H               0x10081610
#define INT_RA3_H2L               0x20081610
#define INT_RA4                   0x30101610
#define INT_RA4_L2H               0x10101610
#define INT_RA4_H2L               0x20101610
#define INT_RA5                   0x30201610
#define INT_RA5_L2H               0x10201610
#define INT_RA5_H2L               0x20201610
#define INT_RA6                   0x30401610
#define INT_RA6_L2H               0x10401610
#define INT_RA6_H2L               0x20401610
#define INT_RA7                   0x30801610
#define INT_RA7_L2H               0x10801610
#define INT_RA7_H2L               0x20801610
#define INT_RB                    0x30FF1610
#define INT_RB0                   0x31011610
#define INT_RB0_L2H               0x11011610
#define INT_RB0_H2L               0x21011610
#define INT_RB1                   0x31021610
#define INT_RB1_L2H               0x11021610
#define INT_RB1_H2L               0x21021610
#define INT_RB2                   0x31041610
#define INT_RB2_L2H               0x11041610
#define INT_RB2_H2L               0x21041610
#define INT_RB3                   0x31081610
#define INT_RB3_L2H               0x11081610
#define INT_RB3_H2L               0x21081610
#define INT_RB4                   0x31101610
#define INT_RB4_L2H               0x11101610
#define INT_RB4_H2L               0x21101610
#define INT_RB5                   0x31201610
#define INT_RB5_L2H               0x11201610
#define INT_RB5_H2L               0x21201610
#define INT_RB6                   0x31401610
#define INT_RB6_L2H               0x11401610
#define INT_RB6_H2L               0x21401610
#define INT_RB7                   0x31801610
#define INT_RB7_L2H               0x11801610
#define INT_RB7_H2L               0x21801610
#define INT_RC                    0x30FF1610
#define INT_RC0                   0x32011610
#define INT_RC0_L2H               0x12011610
#define INT_RC0_H2L               0x22011610
#define INT_RC1                   0x32021610
#define INT_RC1_L2H               0x12021610
#define INT_RC1_H2L               0x22021610
#define INT_RC2                   0x32041610
#define INT_RC2_L2H               0x12041610
#define INT_RC2_H2L               0x22041610
#define INT_RC3                   0x32081610
#define INT_RC3_L2H               0x12081610
#define INT_RC3_H2L               0x22081610
#define INT_RC4                   0x32101610
#define INT_RC4_L2H               0x12101610
#define INT_RC4_H2L               0x22101610
#define INT_RC5                   0x32201610
#define INT_RC5_L2H               0x12201610
#define INT_RC5_H2L               0x22201610
#define INT_RC6                   0x32401610
#define INT_RC6_L2H               0x12401610
#define INT_RC6_H2L               0x22401610
#define INT_RC7                   0x32801610
#define INT_RC7_L2H               0x12801610
#define INT_RC7_H2L               0x22801610
#define INT_RE                    0x30FF1610
#define INT_RE0                   0x32011610
#define INT_RE0_L2H               0x12011610
#define INT_RE0_H2L               0x22011610
#define INT_RE1                   0x32021610
#define INT_RE1_L2H               0x12021610
#define INT_RE1_H2L               0x22021610
#define INT_RE2                   0x32041610
#define INT_RE2_L2H               0x12041610
#define INT_RE2_H2L               0x22041610
#define INT_RE3                   0x32081610
#define INT_RE3_L2H               0x12081610
#define INT_RE3_H2L               0x22081610

#list
