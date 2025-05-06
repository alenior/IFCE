//////////// Standard Header file for the PIC16LF18425 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16LF18425

#nolist
//////// Program memory: 8192x14  Data RAM: 1008  Stack: 16
//////// I/O: 12   Analog Pins: 11
//////// Data EEPROM: 256
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: LP,XT,HS,NOEXTOSC,ECL,ECM,ECH,RSTOSC_HFINTRC_32MHZ
//////// Fuses: RSTOSC_HFINTRC_PLL,RSTOSC_EXT_PLL,RSTOSC_SOSC,RSTOSC_LFINTRC
//////// Fuses: RSTOSC_HFINTRC,RSTOSC_EXT,CLKOUT,NOCLKOUT,CKS,NOCKS,FCMEN
//////// Fuses: NOFCMEN,MCLR,NOMCLR,PUT_1MS,PUT_16MS,PUT_64MS,NOPUT,LPBOR
//////// Fuses: NOLPBOR,NOBROWNOUT,BROWNOUT_SW,BROWNOUT_NOSL,BROWNOUT,BORV27
//////// Fuses: BORV24,ZCDDIS,NOZCDDIS,PPS1WAY,NOPPS1WAY,STVREN,NOSTVREN
//////// Fuses: DEBUG,NODEBUG,WDT64,WDT128,WDT256,WDT512,WDT1024,WDT2048
//////// Fuses: WDT4096,WDT8192,WDT16384,WDT32768,WDT65536,WDT131072
//////// Fuses: WDT262144,WDT524299,WDT1048576,WDT2097152,WDT4194304
//////// Fuses: WDT8388608,WDT32,WDTSW,NOWDT,WDT_SW,WDT_NOSL,WDT,WDTWIN_12%
//////// Fuses: WDTWIN_25%,WDTWIN_37%,WDTWIN_50%,WDTWIN_62%,WDTWIN_75%
//////// Fuses: WDTWIN_100%,WDTWIN_SW,WDTCLK_LFINTRC,WDTCLK_HFINTRC
//////// Fuses: WDTCLK_SOSC,WDTCLK_SW,BBSIZ8K,BBSIZ4K,BBSIZ2K,BBSIZ1K
//////// Fuses: BBSIZ512,BOOTBLOCK,NOBOOTBLOCK,SAF,NOSAF,WRT,NOWRT,WRTB
//////// Fuses: NOWRTB,WRTC,NOWRTC,WRTD,NOWRTD,WRTSAF,NOWRTSAF,LVP,NOLVP
//////// Fuses: PROTECT,NOPROTECT
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
//    PIN_A0,PIN_A1,PIN_A2,PIN_A3,PIN_A4,PIN_A5,PIN_C0,PIN_C1,PIN_C2,PIN_C3,
//    PIN_C4,PIN_C5
// Input Functions: 
//    INT0,T0CK,T1CK,T1G,T3CK,T3G,T5CK,T5G,T2CK,T4CK,T6CK,CCP1,CCP2,CCP3,CCP4,
//    SMT1WIN,SMT1SIG,CWG1IN,CWG2IN,MDCARL,MDCARH,MDSRC,CLCIN0,CLCIN1,CLCIN2,
//    CLCIN3,ADACT,SCK1IN,SCL1IN,SDI1,SDA1IN,SS1IN,SCK2IN,SCL2IN,SDI2,SDA2IN,
//    SS2IN,U1RX,U1CK,T0CKI,T1CKI,T3CKI,T5CKI,T2CKI,T4CKI,T6CKI,CCP1IN,CCP2IN,
//    CCP3IN,CCP4IN,RX1,CK1
// Output Functions: 
//    NULL,CLC1OUT,CLC2OUT,CLC3OUT,CLC4OUT,CWG1OUTA,CWG1OUTB,CWG1OUTC,CWG1OUTD,
//    CCP1OUT,CCP2OUT,CCP3OUT,CCP4OUT,PWM6OUT,PWM7OUT,U1TX,U1DT,C1OUT,C2OUT,
//    SCK1OUT,SCL1OUT,SDO1,SDA1OUT,SCK2OUT,SCL2OUT,SDO2,SDA2OUT,T0OUT,NCO1OUT,
//    CLKROUT,DSMOUT,CWG2OUTA,CWG2OUTB,CWG2OUTC,CWG2OUTD,ADCGRDA,ADCGRDB,PWM6,
//    PWM7,TX1,DT1,SCK1,SCL1,SDA1,SCK2,SCL2,SDA2,TMR0OUT,NCO1
//

////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_x_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT(),
// Discrete I/O Prototypes:
_bif void set_tris_a(int8 value);  
_bif void set_tris_c(int8 value);  
_bif void set_input_level_a(int8 value);  
_bif void set_input_level_c(int8 value);  
_bif void set_open_drain_a(int8 value);  
_bif void set_open_drain_c(int8 value);  
_bif int8 get_tris_a(void);  
_bif int8 get_tris_c(void);  
_bif void output_a(int8 value);  
_bif void output_c(int8 value);  
_bif int8 input_a(void);  
_bif int8 input_c(void);  
_bif int8 input_change_a(void);  
_bif int8 input_change_c(void);  
_bif void port_a_pullups(int8 upmask);   
_bif void port_c_pullups(int8 upmask);   
_bif void set_slow_slew_a(int8 value); 
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

#define PIN_C0  112
#define PIN_C1  113
#define PIN_C2  114
#define PIN_C3  115
#define PIN_C4  116
#define PIN_C5  117

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
_bif void sleep(int8 type); 
// Constants returned from RESTART_CAUSE() are:
#define RESTART_NONE         0x73F //No restart occurred, most common cause for returning this is restart_cause() was called multiple times.  F1840"
#define NORMAL_POWER_UP      0x73C   
#define BROWNOUT_RESTART     0x73E   
#define MCLR_FROM_SLEEP      0x637   
#define WDT_TIMEOUT          0x52F   
#define WDT_FROM_SLEEP       0x43F   
#define INTERRUPT_FROM_SLEEP 0x63F   
#define MCLR_FROM_RUN        0x737   
#define RESET_INSTRUCTION    0x73B   
#define STACK_OVERFLOW       0x7BF   
#define STACK_UNDERFLOW      0x77F   
#define WDT_WINDOW_VIOLATION 0x71F   
#define MEMORY_VIOLATION     0x31F   

// Constants for calls to sleep() are:
#define REG_LOW_POWER            2
#define REG_NORMAL               0  // default

// Constants for calls to sleep() are:
#define SLEEP_FULL       0  // Default
#define SLEEP_IDLE       1  // Clock and peripherals don't stop

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
#define T1_MFINTRC_32KHZ    0x0600 
#define T1_SOSC             0x0703 
#define T1_CLKR             0x0803 
#define T1_TIMER0           0x0903 
#define T1_TIMER3           0x0B03 
#define T1_TIMER5           0x0C03 
#define T1_CLC1             0x0D03 
#define T1_CLC2             0x0E03 
#define T1_CLC3             0x0F03 
#define T1_CLC4             0x1003 

#define T1_SYNC             0x04  

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE             0x8000
#define T1_GATE_INVERTED    0xC000
#define T1_GATE_TOGGLE      0xA000
#define T1_GATE_SINGLE      0x9000
#define T1_GATE_TIMER0      0x018000 
#define T1_GATE_TIMER2      0x038000 
#define T1_GATE_TIMER3      0x048000 
#define T1_GATE_TIMER4      0x058000 
#define T1_GATE_TIMER5      0x068000 
#define T1_GATE_TIMER6      0x078000 
#define T1_GATE_SMT1        0x088000 
#define T1_GATE_CCP1        0x098000 
#define T1_GATE_CCP2        0x0A8000 
#define T1_GATE_CCP3        0x0B8000 
#define T1_GATE_CCP4        0x0C8000 
#define T1_GATE_PWM6        0x0D8000 
#define T1_GATE_PWM7        0x0E8000 
#define T1_GATE_NCO1        0x0F8000 
#define T1_GATE_COMP1       0x108000 
#define T1_GATE_COMP2       0x118000 
#define T1_GATE_ZCD1        0x128000 
#define T1_GATE_CLC1        0x138000 
#define T1_GATE_CLC2        0x148000 
#define T1_GATE_CLC3        0x158000 
#define T1_GATE_CLC4        0x168000 

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
#define T2_CLK_NCO1                                0x0900 
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
#define T2_RESET_FROM_CCP3                         0x03000000 
#define T2_RESET_FROM_CCP4                         0x04000000 
#define T2_RESET_FROM_PWM6                         0x05000000 
#define T2_RESET_FROM_PWM7                         0x06000000 
#define T2_RESET_FROM_C1OUT                        0x07000000 
#define T2_RESET_FROM_C2OUT                        0x08000000 
#define T2_RESET_FROM_ZCD                          0x09000000 
#define T2_RESET_FROM_CLC1                         0x0A000000 
#define T2_RESET_FROM_CLC2                         0x0B000000 
#define T2_RESET_FROM_CLC3                         0x0C000000 
#define T2_RESET_FROM_CLC4                         0x0D000000 

////////////////////////////////////////////////////////////////// Timer 3
// Timer 3 Functions: SETUP_TIMER_3, GET_TIMER3, SET_TIMER3
// Timer 3 Prototypes:
_bif void setup_timer_3(int16 mode); 
_bif int16 get_timer3(void);
_bif void set_timer3(int16 value);
// Constants used for SETUP_TIMER_3() are:
//      (or (via |) together constants from each group)
#define T3_DISABLED         0
#define T3_EXTERNAL         0x0003 
#define T3_INTERNAL         0x0103 
#define T3_FOSC             0x0203 
#define T3_HFINTRC          0x0303 
#define T3_LFINTRC          0x0403 
#define T3_MFINTRC          0x0503 
#define T3_MFINTRC_32KHZ    0x0600 
#define T3_SOSC             0x0703 
#define T3_CLKR             0x0803 
#define T3_TIMER0           0x0903 
#define T3_TIMER1           0x0A03 
#define T3_TIMER5           0x0C03 
#define T3_CLC1             0x0D03 
#define T3_CLC2             0x0E03 
#define T3_CLC3             0x0F03 
#define T3_CLC4             0x1003 

#define T3_SYNC             0x04 

#define T3_DIV_BY_1         0
#define T3_DIV_BY_2         0x10
#define T3_DIV_BY_4         0x20
#define T3_DIV_BY_8         0x30

#define T3_GATE             0x8000
#define T3_GATE_INVERTED    0xC000
#define T3_GATE_TOGGLE      0xA000
#define T3_GATE_SINGLE      0x9000
#define T3_GATE_TIMER0      0x018000 
#define T3_GATE_TIMER1      0x028000 
#define T3_GATE_TIMER2      0x038000 
#define T3_GATE_TIMER4      0x058000 
#define T3_GATE_TIMER5      0x068000 
#define T3_GATE_TIMER6      0x078000 
#define T3_GATE_SMT1        0x088000 
#define T3_GATE_CCP1        0x098000 
#define T3_GATE_CCP2        0x0A8000 
#define T3_GATE_CCP3        0x0B8000 
#define T3_GATE_CCP4        0x0C8000 
#define T3_GATE_PWM6        0x0D8000 
#define T3_GATE_PWM7        0x0E8000 
#define T3_GATE_NCO1        0x0F8000 
#define T3_GATE_COMP1       0x108000 
#define T3_GATE_COMP2       0x118000 
#define T3_GATE_ZCD1        0x128000 
#define T3_GATE_CLC1        0x138000 
#define T3_GATE_CLC2        0x148000 
#define T3_GATE_CLC3        0x158000 
#define T3_GATE_CLC4        0x168000 

////////////////////////////////////////////////////////////////// Timer 4
// Timer 4 Functions: SETUP_TIMER_4, GET_TIMER4, SET_TIMER4
// Timer 4 Prototypes:
_bif void setup_timer_4(int32 mode, int8 period, int8 postscaler);
_bif int8 get_timer4(void);
_bif void set_timer4(int8 value);
//Constants used for SETUP_TIMER_4() are:
#define T4_DISABLED                                0
#define T4_DIV_BY_1                                0x80
#define T4_DIV_BY_2                                0x90
#define T4_DIV_BY_4                                0xA0
#define T4_DIV_BY_8                                0xB0
#define T4_DIV_BY_16                               0xC0
#define T4_DIV_BY_32                               0xD0
#define T4_DIV_BY_64                               0xE0
#define T4_DIV_BY_128                              0xF0
//One of the following may be OR'ed with the above
#define T4_CLK_T4IN                                0  
#define T4_CLK_INTERNAL                            0x0100 //FOSC/4  
#define T4_CLK_FOSC                                0x0200  
#define T4_CLK_HFINTRC                             0x0300  
#define T4_CLK_LFINTRC                             0x0400  
#define T4_CLK_MFINTRC_500KHZ                      0x0500 
#define T4_CLK_MFINTRC_31_25KHZ                    0x0600 
#define T4_CLK_SOSC                                0x0700 
#define T4_CLK_CLKR                                0x0800 
#define T4_CLK_NCO1                                0x0900 
#define T4_CLK_ZCD                                 0x0A00 
#define T4_CLK_CLC1                                0x0B00 
#define T4_CLK_CLC2                                0x0C00 
#define T4_CLK_CLC3                                0x0D00 
#define T4_CLK_CLC4                                0x0E00 

//One of the following may be OR'ed with the above for setting up the Hardware Limit Timer
#define T4_START_IMMEDIATELY                       0
#define T4_START_WHEN_HIGH                         0x010000
#define T4_START_WHEN_LOW                          0x020000
#define T4_RESET_ON_BOTH_EDGES                     0x030000
#define T4_RESET_ON_RE                             0x040000
#define T4_RESET_ON_FE                             0x050000
#define T4_RESET_WHEN_LOW                          0x060000
#define T4_RESET_WHEN_HIGH                         0x070000
#define T4_ONE_SHOT_START_IMMEDIATELY              0x080000
#define T4_ONE_SHOT_START_ON_RE                    0x090000
#define T4_ONE_SHOT_START_ON_FE                    0x0A0000
#define T4_ONE_SHOT_START_ON_BOTH_EDGES            0x0B0000
#define T4_ONE_SHOT_START_ON_RE_RESET_ON_RE        0x0C0000 //starts on first rising edge, resets on subsequent rising edges
#define T4_ONE_SHOT_START_ON_FE_RESET_ON_FE        0x0D0000 //starts on first falling edge, resets on subsequent falling edges
#define T4_ONE_SHOT_START_ON_RE_RESET_WHEN_LOW     0x0E0000
#define T4_ONE_SHOT_START_ON_FE_RESET_WHEN_HIGH    0x0F0000
#define T4_MONO_STABLE_START_ON_RE                 0x110000  
#define T4_MONO_STABLE_START_ON_FE                 0x120000  
#define T4_MONO_STABLE_START_ON_EE                 0x130000  
#define T4_ONE_SHOT_START_WHEN_HIGH_RESET_WHEN_LOW 0x160000  
#define T4_ONE_SHOT_START_WHEN_LOW_RESET_WHEN_HIGH 0x170000  

//Any of the following may be OR'ed with the above 
#define T4_SYNC_ON_WITH_T2CLK                      0x200000
#define T4_INVERTED                                0x400000
#define T4_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T4_RESET_FROM_T4IN                         0
#define T4_RESET_FROM_CCP1                         0x01000000 
#define T4_RESET_FROM_CCP2                         0x02000000 
#define T4_RESET_FROM_CCP3                         0x03000000 
#define T4_RESET_FROM_CCP4                         0x04000000 
#define T4_RESET_FROM_PWM6                         0x05000000 
#define T4_RESET_FROM_PWM7                         0x06000000 
#define T4_RESET_FROM_C1OUT                        0x07000000 
#define T4_RESET_FROM_C2OUT                        0x08000000 
#define T4_RESET_FROM_ZCD                          0x09000000 
#define T4_RESET_FROM_CLC1                         0x0A000000 
#define T4_RESET_FROM_CLC2                         0x0B000000 
#define T4_RESET_FROM_CLC3                         0x0C000000 
#define T4_RESET_FROM_CLC4                         0x0D000000 

////////////////////////////////////////////////////////////////// Timer 5
// Timer 5 Functions: SETUP_TIMER_5, GET_TIMER5, SET_TIMER5
// Timer 5 Prototypes:
_bif void setup_timer_5(int16 mode); 
_bif int16 get_timer5(void);
_bif void set_timer5(int16 value);
// Constants used for SETUP_TIMER_5() are:
//      (or (via |) together constants from each group)
#define T5_DISABLED         0
#define T5_EXTERNAL         0x0003 
#define T5_INTERNAL         0x0103 
#define T5_FOSC             0x0203 
#define T5_HFINTRC          0x0303 
#define T5_LFINTRC          0x0403 
#define T5_MFINTRC          0x0503 
#define T5_MFINTRC_32KHZ    0x0600 
#define T5_SOSC             0x0703 
#define T5_CLKR             0x0803 
#define T5_TIMER0           0x0903 
#define T5_TIMER1           0x0A03 
#define T5_TIMER3           0x0B03 
#define T5_CLC1             0x0D03 
#define T5_CLC2             0x0E03 
#define T5_CLC3             0x0F03 
#define T5_CLC4             0x1003 

#define T5_SYNC             0x04 

#define T5_DIV_BY_1         0
#define T5_DIV_BY_2         0x10
#define T5_DIV_BY_4         0x20
#define T5_DIV_BY_8         0x30

#define T5_GATE             0x8000
#define T5_GATE_INVERTED    0xC000
#define T5_GATE_TOGGLE      0xA000
#define T5_GATE_SINGLE      0x9000
#define T5_GATE_TIMER0      0x018000 
#define T5_GATE_TIMER1      0x028000 
#define T5_GATE_TIMER2      0x038000 
#define T5_GATE_TIMER3      0x048000 
#define T5_GATE_TIMER4      0x058000 
#define T5_GATE_TIMER6      0x078000 
#define T5_GATE_SMT1        0x088000 
#define T5_GATE_CCP1        0x098000 
#define T5_GATE_CCP2        0x0A8000 
#define T5_GATE_CCP3        0x0B8000 
#define T5_GATE_CCP4        0x0C8000 
#define T5_GATE_PWM6        0x0D8000 
#define T5_GATE_PWM7        0x0E8000 
#define T5_GATE_NCO1        0x0F8000 
#define T5_GATE_COMP1       0x108000 
#define T5_GATE_COMP2       0x118000 
#define T5_GATE_ZCD1        0x128000 
#define T5_GATE_CLC1        0x138000 
#define T5_GATE_CLC2        0x148000 
#define T5_GATE_CLC3        0x158000 
#define T5_GATE_CLC4        0x168000 

////////////////////////////////////////////////////////////////// Timer 6
// Timer 6 Functions: SETUP_TIMER_6, GET_TIMER6, SET_TIMER6
// Timer 6 Prototypes:
_bif void setup_timer_6(int32 mode, int8 period, int8 postscaler);
_bif int8 get_timer6(void);
_bif void set_timer6(int8 value);
//Constants used for SETUP_TIMER_6() are:
#define T6_DISABLED                                0
#define T6_DIV_BY_1                                0x80
#define T6_DIV_BY_2                                0x90
#define T6_DIV_BY_4                                0xA0
#define T6_DIV_BY_8                                0xB0
#define T6_DIV_BY_16                               0xC0
#define T6_DIV_BY_32                               0xD0
#define T6_DIV_BY_64                               0xE0
#define T6_DIV_BY_128                              0xF0
//One of the following may be OR'ed with the above
#define T6_CLK_T6IN                                0 
#define T6_CLK_INTERNAL                            0x0100 //FOSC/4 
#define T6_CLK_FOSC                                0x0200 
#define T6_CLK_HFINTRC                             0x0300 
#define T6_CLK_LFINTRC                             0x0400 
#define T6_CLK_MFINTRC_500KHZ                      0x0500 
#define T6_CLK_MFINTRC_31_25KHZ                    0x0600 
#define T6_CLK_SOSC                                0x0700 
#define T6_CLK_CLKR                                0x0800 
#define T6_CLK_NCO1                                0x0900 
#define T6_CLK_ZCD                                 0x0A00 
#define T6_CLK_CLC1                                0x0B00 
#define T6_CLK_CLC2                                0x0C00 
#define T6_CLK_CLC3                                0x0D00 
#define T6_CLK_CLC4                                0x0E00 

//One of the following may be OR'ed with the above for setting up the Hardware Limit Timer
#define T6_START_IMMEDIATELY                       0
#define T6_START_WHEN_HIGH                         0x010000
#define T6_START_WHEN_LOW                          0x020000
#define T6_RESET_ON_BOTH_EDGES                     0x030000
#define T6_RESET_ON_RE                             0x040000
#define T6_RESET_ON_FE                             0x050000
#define T6_RESET_WHEN_LOW                          0x060000
#define T6_RESET_WHEN_HIGH                         0x070000
#define T6_ONE_SHOT_START_IMMEDIATELY              0x080000
#define T6_ONE_SHOT_START_ON_RE                    0x090000
#define T6_ONE_SHOT_START_ON_FE                    0x0A0000
#define T6_ONE_SHOT_START_ON_BOTH_EDGES            0x0B0000
#define T6_ONE_SHOT_START_ON_RE_RESET_ON_RE        0x0C0000 //starts on first rising edge, resets on subsequent rising edges
#define T6_ONE_SHOT_START_ON_FE_RESET_ON_FE        0x0D0000 //starts on first falling edge, resets on subsequent falling edges
#define T6_ONE_SHOT_START_ON_RE_RESET_WHEN_LOW     0x0E0000
#define T6_ONE_SHOT_START_ON_FE_RESET_WHEN_HIGH    0x0F0000
#define T6_MONO_STABLE_START_ON_RE                 0x110000 
#define T6_MONO_STABLE_START_ON_FE                 0x120000 
#define T6_MONO_STABLE_START_ON_EE                 0x130000 
#define T6_ONE_SHOT_START_WHEN_HIGH_RESET_WHEN_LOW 0x160000 
#define T6_ONE_SHOT_START_WHEN_LOW_RESET_WHEN_HIGH 0x170000 
//Any of the following may be OR'ed with the above
#define T6_SYNC_ON_WITH_T2CLK                      0x200000
#define T6_INVERTED                                0x400000
#define T6_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T6_RESET_FROM_T6IN                         0
#define T6_RESET_FROM_CCP1                         0x01000000 
#define T6_RESET_FROM_CCP2                         0x02000000 
#define T6_RESET_FROM_CCP3                         0x03000000 
#define T6_RESET_FROM_CCP4                         0x04000000 
#define T6_RESET_FROM_PWM6                         0x05000000 
#define T6_RESET_FROM_PWM7                         0x06000000 
#define T6_RESET_FROM_C1OUT                        0x07000000 
#define T6_RESET_FROM_C2OUT                        0x08000000 
#define T6_RESET_FROM_ZCD                          0x09000000 
#define T6_RESET_FROM_CLC1                         0x0A000000 
#define T6_RESET_FROM_CLC2                         0x0B000000 
#define T6_RESET_FROM_CLC3                         0x0C000000 
#define T6_RESET_FROM_CLC4                         0x0D000000 

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
_bif void setup_pwm6(int32 mode); 
_bif void set_pwm6_duty(int8 value); 
_bif void set_pwm6_duty(int16 value); 
_bif void setup_pwm7(int32 mode); 
_bif void set_pwm7_duty(int8 value); 
_bif void set_pwm7_duty(int16 value); 
// Constants used for SETUP_PWMx() are:
#define PWM_ENABLED      0x80
#define PWM_DISABLED     0x10000
#define PWM_ACTIVE_LOW   0x10
#define PWM_ACTIVE_HIGH  0
// One of the following may be OR'ed in with the above 
#define PWM_TIMER2       0x0000 
#define PWM_TIMER4       0x0100 
#define PWM_TIMER6       0x0200  

#bit PWM6OUT=getenv("BIT:PWM6OUT") 
#bit PWM7OUT=getenv("BIT:PWM7OUT") 

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
//Optionally OR any of the following with the above
#define CP1_INT_L2H     0x0080
#define CP1_INT_H2L     0x0040
#define CP1_INVERT      0x1000
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
#define DAC_LVP_POS   0x40        
#define DAC_LVP_NEG   0        

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
#define CWG_INPUT_CCP1                 0x1000 
#define CWG_INPUT_CCP2                 0x2000 
#define CWG_INPUT_CCP3                 0x3000 
#define CWG_INPUT_CCP4                 0x4000 
#define CWG_INPUT_PWM6                 0x5000 
#define CWG_INPUT_PWM7                 0x6000 
#define CWG_INPUT_NCO                  0x7000 
#define CWG_INPUT_COMP1                0x8000 
#define CWG_INPUT_COMP2                0x9000 
#define CWG_INPUT_DSM                  0xA000 
#define CWG_INPUT_CLC1                 0xB000 
#define CWG_INPUT_CLC2                 0xC000 
#define CWG_INPUT_CLC3                 0xD000 
#define CWG_INPUT_CLC4                 0xE000 
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
#define CWG_SHUTDOWN_ON_TMR2           0x0002 
#define CWG_SHUTDOWN_ON_TMR4           0x0004 
#define CWG_SHUTDOWN_ON_TMR6           0x0008 
#define CWG_SHUTDOWN_ON_COMP1          0x0010 
#define CWG_SHUTDOWN_ON_COMP2          0x0020 
#define CWG_SHUTDOWN_ON_CLCx           0x0040 //CLC2 - CWG1, CLC3 - CWG2  
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
#define NCO_CLOCK_FOSC              0x0000 
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


#bit N1OUT = getenv("SFR:NCO1CON").5 

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
#define CLC_INPUT_TIMER3                    0x0F 
#define CLC_INPUT_TIMER4                    0x10 
#define CLC_INPUT_TIMER5                    0x11 
#define CLC_INPUT_TIMER6                    0x12 
#define CLC_INPUT_SMT1                      0x13 
#define CLC_INPUT_CCP1                      0x14 
#define CLC_INPUT_CCP2                      0x15 
#define CLC_INPUT_CCP3                      0x16 
#define CLC_INPUT_CCP4                      0x17 
#define CLC_INPUT_PWM6                      0x18 
#define CLC_INPUT_PWM7                      0x19 
#define CLC_INPUT_NCO1                      0x1A 
#define CLC_INPUT_C1OUT                     0x1B 
#define CLC_INPUT_C2OUT                     0x1C 
#define CLC_INPUT_ZCD                       0x1D 
#define CLC_INPUT_IOCIF                     0x1E 
#define CLC_INPUT_DSM                       0x1F 
#define CLC_INPUT_CLC1                      0x20 
#define CLC_INPUT_CLC2                      0x21 
#define CLC_INPUT_CLC3                      0x22 
#define CLC_INPUT_CLC4                      0x23 
#define CLC_INPUT_U1DT                      0x24 
#define CLC_INPUT_U1TX                      0x25 
#define CLC_INPUT_SDO1                      0x26 
#define CLC_INPUT_SCK1                      0x27 
#define CLC_INPUT_SDO2                      0x28 
#define CLC_INPUT_SCK2                      0x29 
#define CLC_INPUT_CWG1A                     0x2A 
#define CLC_INPUT_CWG1B                     0x2B 
#define CLC_INPUT_CWG2A                     0x2C 
#define CLC_INPUT_CWG2B                     0x2D 

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

////////////////////////////////////////////////////////////////// SMT
// SMT Functions: SETUP_SMTx(), SMTx_STATUS(), SMTx_START(),
//                SMTx_STOP(), SMTx_UPDATE(), SMTx_RESET_TIMER(),
//                SMTx_READ(), SMTx_WRITE()
// SMT Prototypes:
_bif void setup_smt1(unsigned int32 mode);
_bif void setup_smt1(unsigned int32 mode, unsigned int32 period);
_bif unsigned int8 smt1_status(void);
_bif void smt1_start(void);
_bif void smt1_stop(void);
_bif void smt1_update(unsigned int8 which);
_bif void smt1_reset_timer(void);
_bif unsigned int32 smt1_read(unsigned int8 which);
_bif void smt1_write(unsigned int8 which, unsigned int32 value);
// Constants used in SETUP_SMTx() are:
#define SMT_ENABLED                         0x80
#define SMT_DISABLED                        0
// One of the following may be OR'ed in with the above using |
#define SMT_TMR_NO_ROLLOVER                 0x20
#define SMT_TMR_ROLLSOVER                   0
// One of the following may be OR'ed in with the above using |
#define SMT_WIN_ACTIVE_LOW                  0x10
#define SMT_WIN_ACTIVE_HIGH                 0
// One of the following may be OR'ed in with the above using |
#define SMT_SIG_ACTIVE_LOW                  0x08
#define SMT_SIG_ACTIVE_HIGH                 0
// One of the following may be OR'ed in with the above using |
#define SMT_TMR_INCREMENTS_ON_FE            0x04
#define SMT_TMR_INCREMENTS_ON_RE            0
// One of the following may be OR'ed in with the above using |
#define SMT_DIV_BY_8                        0x03
#define SMT_DIV_BY_4                        0x02
#define SMT_DIV_BY_2                        0x01
#define SMT_DIV_BY_1                        0
// One of the following may be OR'ed in with the above using |
#define SMT_REPEAT_DATA_ACQ_MODE            0x4000
#define SMT_SINGLE_DATA_ACQ_MODE            0
// One of the following may be OR'ed in with the above using |
#define SMT_MODE_WINDOWED_COUNTER           0x0A00
#define SMT_MODE_GATED_COUNTER              0x0900
#define SMT_MODE_COUNTER                    0x0800
#define SMT_MODE_CAPTURE                    0x0700
#define SMT_MODE_TIME_OF_FLIGHT             0x0600
#define SMT_MODE_GATED_WINDOWED_MEASURE     0x0500
#define SMT_MODE_WINDOWED_MEASURE           0x0400
#define SMT_MODE_HIGH_LOW_TIME_MEASUREMENT  0x0300
#define SMT_MODE_PERIOD_DUTY_CYCLE_ACQ      0x0200
#define SMT_MODE_GATED_TIMER                0x0100
#define SMT_MODE_TIMER                      0
// One of the following may be OR'ed in with the above using |
#define SMT_CLK_CLKR                        0x070000 
#define SMT_CLK_SOSC                        0x060000 
#define SMT_CLK_31250                       0x050000 // MFINTOSC/16 
#define SMT_CLK_500000                      0x040000 // MFINTOSC 
#define SMT_CLK_31000                       0x030000 // LFINTOSC
#define SMT_CLK_HFINTOSC                    0x020000 
#define SMT_CLK_FOSC                        0x010000   
#define SMT_CLK_FOSC_DIV_4                  0 
// One of the following may be OR'ed in with the above using |
#define SMT_WIN_INPUT_NCO                   0x87000000 
#define SMT_WIN_INPUT_CLKR                  0x85000000 
#define SMT_WIN_INPUT_CLC4                  0x84000000 
#define SMT_WIN_INPUT_CLC3                  0x83000000 
#define SMT_WIN_INPUT_CLC2                  0x82000000 
#define SMT_WIN_INPUT_CLC1                  0x81000000 
#define SMT_WIN_INPUT_ZCD                   0x80000000 
#define SMT_WIN_INPUT_COMP2                 0x0F000000 
#define SMT_WIN_INPUT_COMP1                 0x0E000000 
#define SMT_WIN_INPUT_PWM7                  0x0D000000 
#define SMT_WIN_INPUT_PWM6                  0x0C000000 
#define SMT_WIN_INPUT_CCP4                  0x0B000000 
#define SMT_WIN_INPUT_CCP3                  0x0A000000 
#define SMT_WIN_INPUT_CCP2                  0x09000000 
#define SMT_WIN_INPUT_CCP1                  0x08000000 
#define SMT_WIN_INPUT_TMR6                  0x07000000 
#define SMT_WIN_INPUT_TMR4                  0x06000000 
#define SMT_WIN_INPUT_TMR2                  0x05000000 
#define SMT_WIN_INPUT_TMR0                  0x04000000 
#define SMT_WIN_INPUT_SOSC                  0x03000000 
#define SMT_WIN_INPUT_MFINTOSC_DIV_16       0x02000000 
#define SMT_WIN_INPUT_LFINTOSC              0x01000000 
#define SMT_WIN_INPUT_SMTWINx               0
// One of the following may be OR'ed in with the above using |
#define SMT_SIG_INPUT_CLC4                  0x50200000 
#define SMT_SIG_INPUT_CLC3                  0x40200000 
#define SMT_SIG_INPUT_CLC2                  0x30200000 
#define SMT_SIG_INPUT_CLC1                  0x20200000 
#define SMT_SIG_INPUT_ZCD                   0x20100000 
#define SMT_SIG_INPUT_COMP2                 0x20000000 
#define SMT_SIG_INPUT_COMP1                 0x70100000 
#define SMT_SIG_INPUT_NCO                   0x60100000 
#define SMT_SIG_INPUT_PWM7                  0x50100000 
#define SMT_SIG_INPUT_PWM6                  0x40100000 
#define SMT_SIG_INPUT_CCP4                  0x30100000 
#define SMT_SIG_INPUT_CCP3                  0x20100000 
#define SMT_SIG_INPUT_CCP2                  0x10100000 
#define SMT_SIG_INPUT_CCP1                  0x00100000 
#define SMT_SIG_INPUT_TMR6                  0x70000000 
#define SMT_SIG_INPUT_TMR5                  0x60000000 
#define SMT_SIG_INPUT_TMR4                  0x50000000 
#define SMT_SIG_INPUT_TMR3                  0x40000000 
#define SMT_SIG_INPUT_TMR2                  0x30000000 
#define SMT_SIG_INPUT_TMR1                  0x20000000 
#define SMT_SIG_INPUT_TMR0                  0x10000000 
#define SMT_SIG_INPUT_SMTSIGx               0
// The following may be OR'ed in with the above using |
#define SMT_START_NOW                       0x8000

// Constants used in SMTx_UPDATE() and SMTx_READ() are:
#define SMT_CAPTURED_PERIOD_REG             0
#define SMT_CAPTURED_PULSE_WIDTH_REG        1
// Constants used in SMTx_READ() and SMTx_WRITE() are:
#define SMT_TMR_REG                         2
#define SMT_PERIOD_REG                      3

// Constants returned from SMTx_STATUS() are:
#define SMT_TIMER_IS_RUNNING                0x04
#define SMT_WINDOW_IS_OPEN                  0x02
#define SMT_ACQ_IN_PROGRESS                 0x01

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

////////////////////////////////////////////////////////////////// DSM
// Data Signal Modulator Functions: SETUP_DSM()  
// DSM Prototypes:
_bif void setup_dsm(int1 enable);
_bif void setup_dsm(unsigned int16 mode, unsigned int8 source); 
_bif void setup_dsm(unsigned int16 mode, unsigned int8 source, unsigned int16 carrier); 
// Constants used in SETUP_DSM() first parameter are:  
#define DSM_ENABLED                     0x80
#define DSM_DISABLED                    0x00
// Any of the following may be or'ed in with the above using |
#define DSM_OUTPUT_INVERTED             0x10
#define DSM_SYNC_CARRIER_LOW            0x0100 
#define DSM_INVERT_CARRIER_LOW          0x0200 
#define DSM_SYNC_CARRIER_HIGH           0x1000 
#define DSM_INVERT_CARRIER_HIGH         0x2000 
//Constants used as the second parameter are:
#define DSM_SOURCE_MDSCR                0x00 
#define DSM_SOURCE_MANUAL               0x01 
#define DSM_SOURCE_CCP1                 0x02 
#define DSM_SOURCE_CCP2                 0x03 
#define DSM_SOURCE_CCP3                 0x04 
#define DSM_SOURCE_CCP4                 0x05 
#define DSM_SOURCE_PWM6                 0x06 
#define DSM_SOURCE_PWM7                 0x07 
#define DSM_SOURCE_NCO                  0x08 
#define DSM_SOURCE_C1OUT                0x09 
#define DSM_SOURCE_C2OUT                0x0A 
#define DSM_SOURCE_CLC1                 0x0B 
#define DSM_SOURCE_CLC2                 0x0C 
#define DSM_SOURCE_CLC3                 0x0D 
#define DSM_SOURCE_CLC4                 0x0E 
#define DSM_SOURCE_U1TX                 0x0F 
#define DSM_SOURCE_SDO1                 0x10 
#define DSM_SOURCE_SDO2                 0x11 
//Constants used as the third parameter are:
#define DSM_CARRIER_LOW_MDCARL          0x0000 
#define DSM_CARRIER_LOW_SYSTEM_CLOCK    0x0001 
#define DSM_CARRIER_LOW_HFINTOSC        0x0002 
#define DSM_CARRIER_LOW_CLKR            0x0003 
#define DSM_CARRIER_LOW_CCP1            0x0004  
#define DSM_CARRIER_LOW_CCP2            0x0005    
#define DSM_CARRIER_LOW_CCP3            0x0006      
#define DSM_CARRIER_LOW_CCP4            0x0007      
#define DSM_CARRIER_LOW_PWM6            0x0008 
#define DSM_CARRIER_LOW_PWM7            0x0009 
#define DSM_CARRIER_LOW_NCO             0x000A 
#define DSM_CARRIER_LOW_CLC1            0x000B 
#define DSM_CARRIER_LOW_CLC2            0x000C 
#define DSM_CARRIER_LOW_CLC3            0x000D 
#define DSM_CARRIER_LOW_CLC4            0x000E 
#define DSM_CARRIER_HIGH_MDCARH         0x0000 
#define DSM_CARRIER_HIGH_SYSTEM_CLOCK   0x0100 
#define DSM_CARRIER_HIGH_HFINTOSC       0x0200 
#define DSM_CARRIER_HIGH_CLKR           0x0300 
#define DSM_CARRIER_HIGH_CCP1           0x0400  
#define DSM_CARRIER_HIGH_CCP2           0x0500    
#define DSM_CARRIER_HIGH_CCP3           0x0600      
#define DSM_CARRIER_HIGH_CCP4           0x0700       
#define DSM_CARRIER_HIGH_PWM6           0x0800 
#define DSM_CARRIER_HIGH_PWM7           0x0900 
#define DSM_CARRIER_HIGH_NCO            0x0A00 
#define DSM_CARRIER_HIGH_CLC1           0x0B00 
#define DSM_CARRIER_HIGH_CLC2           0x0C00 
#define DSM_CARRIER_HIGH_CLC3           0x0D00 
#define DSM_CARRIER_HIGH_CLC4           0x0E00 

#bit MDBIT = getenv("SFR:MD1CON0").0 
#bit MDOUT = getenv("SFR:MD1CON0").5 


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
// One of the following may be OR'ed in with the above using | 
#define OSC_SOSC_PWR_LOW             0x00000000 
#define OSC_SOSC_PWR_HIGH            0x00400000 
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
//ADC Prototypes:
_bif void setup_adc(int32 mode);
_bif void setup_adc(int32 mode, int8 ADCRS, int8 ADRPT);
_bif void set_adc_trigger(int8 trigger);
_bif int8 read_adc(void);
_bif int8 read_adc(int8 mode);
_bif int16 read_adc(void);
_bif int16 read_adc(int8 mode);
_bif int1 adc_done(void);
_bif int8 adc_read(int8 which);
_bif int16 adc_read(int8 which);
_bif void adc_write(int8 which, int8 value);
_bif void adc_write(int8 which, int16 value);
_bif int8 adc_status(void);
//Constants used for SETUP_ADC() first parameter are:
#define ADC_LEGACY_MODE                                 0
#define ADC_ACCUMULATE_MODE                             0x00010000
#define ADC_AVERAGE_MODE                                0x00020000
#define ADC_BURST_AVERAGE_MODE                          0x00030000
#define ADC_LOW_PASS_FILTER_MODE                        0x00040000
#define ADC_OFF                                         0x80000000
// One of the following may be OR'ed in with the above using |
// If a clock divisor other then one of the provided below is 
// needed the value to be OR'ed in with the above can be calculated
// as follows: value = (divisor / 2) - 1.  For example for a clock
// divisor of 24, (24 / 2) - 1 = 0x0B needs to be OR'ed with the 
// above.
#define ADC_CLOCK_DIV_2                                 0
#define ADC_CLOCK_DIV_4                                 0x00000001
#define ADC_CLOCK_DIV_8                                 0x00000003
#define ADC_CLOCK_DIV_16                                0x00000007
#define ADC_CLOCK_DIV_32                                0x0000000F
#define ADC_CLOCK_DIV_64                                0x0000001F
#define ADC_CLOCK_DIV_128                               0x0000003F
#define ADC_CLOCK_INTERNAL                              0x00000080
// One of the following may be OR'ed in with the above using |
// If an acquisition time other then one of the provided below is
// needed the value to be OR'ed in with the above can be calculated
// as follows: value = (AcqTime xor 0xFF) << 8.  For example for an
// acquisition time of 150, (150 xor 0xFF) << 8 = 0x6900 needs to
// be OR'ed with the above.
#define ADC_TAD_MUL_0                                   0x0000FF00
#define ADC_TAD_MUL_2                                   0x0000FD00
#define ADC_TAD_MUL_4                                   0x0000FB00
#define ADC_TAD_MUL_8                                   0x0000F700
#define ADC_TAD_MUL_16                                  0x0000EF00
#define ADC_TAD_MUL_32                                  0x0000DF00
#define ADC_TAD_MUL_64                                  0x0000BF00
#define ADC_TAD_MUL_128                                 0x00007F00
#define ADC_TAD_MUL_255                                 0
// One of the following may be OR'ed in with the above using |
#define ADC_ADFLTR_TO_ADPREV_AT_START                   0
#define ADC_ADRES_TO_ADPREV_AT_START                    0x00800000
// One of the following may be OR'ed in with the above using |
#define ADC_THRESHOLD_INT_DISABLED                      0
#define ADC_THRESHOLD_INT_ADERR_LT_ADLTH                0x01000000
#define ADC_THRESHOLD_INT_ADERR_GTE_ADLTH               0x02000000
#define ADC_THRESHOLD_INT_ADERR_GT_ADLTH_AND_LT_ADUTH   0x03000000
#define ADC_THRESHOLD_INT_ADERR_LT_ATLTH_OR_GT_ADUTH    0x04000000
#define ADC_THRESHOLD_INT_ADERR_LTE_ADUTH               0x05000000
#define ADC_THRESHOLD_INT_ADERR_GT_ADUTH                0x06000000
#define ADC_THRESHOLD_INT_END_OF_CALCULATION            0x07000000
// One of the following may be OR'ed in with the above using |
#define ADC_ERR_CALC_MODE_ADRES_MINUS_ADPREV            0
#define ADC_ERR_CALC_MODE_ADRES_MINUS_ADSTPT            0x10000000
#define ADC_ERR_CALC_MODE_ADRES_MINUS_ADFLTR            0x20000000
#define ADC_ERR_CALC_MODE_ADPREV_MINUS_ADFLTR           0x40000000
#define ADC_ERR_CALC_MODE_ADFLTR_MINUS_ADSTPT           0x50000000
// The following may be OR'ed in with the above using |
#define ADC_CONTINUOUS_OPERATION_ENABLED                0x00000040
#define ADC_STOP_WHEN_THRESHOLD_CONDITIONS_MET          0x08000000 //If Continuous Operation is enabled

// The second parameter for SETUP_ADC() is a value (0-5). For Accumulate, 
// Average and Burst Average modes it sets how much the accumulated value
// is divided by (2^ADCRS).  For Low-pass Filter mode it sets the cut-off 
// frequency of the filter (see Table 23-4 in device's datasheet).  Not
// used for Legacy mode.

// The third parameter for SETUP_ADC() is a value (0-255).  For Average, Burst
// Average and Low-pass Filter modes it sets the number of samples to be done
// before performing a threshold comparison.  Additionally for Burst Average
// mode it sets the number of times that the ADC module will retrigger its self.
// Not used in Legacy or Accumulate modes.

// Constants used for ADC_WRITE() and ADC_READ() first parameter:
#define ADC_RESULT                      0
#define ADC_PREV_RESULT                 1
#define ADC_ACCUMULATOR                 2
#define ADC_FILTER                      3
#define ADC_ERROR                       4
#define ADC_COUNT                       5
#define ADC_REPEAT                      6
#define ADC_SET_POINT                   7
#define ADC_LOWER_THRESHOLD             8
#define ADC_UPPER_THRESHOLD             9

// Constants used for SET_ADC_TRIGGER() are:
#define ADC_TRIGGER_DISABLED            0 //External trigger disabled, use read_adc() to start conversion
#define ADC_TRIGGER_ADACT_PIN           0x01
#define ADC_TRIGGER_TIMER0              0x02
#define ADC_TRIGGER_TIMER1              0x03
#define ADC_TRIGGER_TIMER2              0x04
#define ADC_TRIGGER_TIMER3              0x05
#define ADC_TRIGGER_TIMER4              0x06
#define ADC_TRIGGER_TIMER5              0x07  
#define ADC_TRIGGER_TIMER6              0x08  
#define ADC_TRIGGER_SMT1                0x09  
#define ADC_TRIGGER_CCP1                0x0A 
#define ADC_TRIGGER_CCP2                0x0B 
#define ADC_TRIGGER_CCP3                0x0C 
#define ADC_TRIGGER_CCP4                0x0D 
#define ADC_TRIGGER_PWM6                0x0E 
#define ADC_TRIGGER_PWM7                0x0F 
#define ADC_TRIGGER_NCO                 0x10 
#define ADC_TRIGGER_COMP1               0x11 
#define ADC_TRIGGER_COMP2               0x12 
#define ADC_TRIGGER_IOC                 0x13 
#define ADC_TRIGGER_CLC1                0x14 
#define ADC_TRIGGER_CLC2                0x15 
#define ADC_TRIGGER_CLC3                0x16 
#define ADC_TRIGGER_CLC4                0x17 
#define ADC_TRIGGER_ADERR               0x1C //Read of ADERR register   
#define ADC_TRIGGER_ADRESH              0x1D //Read of ADRESH register   
#define ADC_TRIGGER_ADPCH               0x1F //Read of ADPCH register   

// Constants returned from ADC_STATUS are:
#define ADC_OVERFLOWED          0x80
#define ADC_GT_UPPER_THRESHOLD  0x40
#define ADC_LT_LOWER_THRESHOLD  0x20
#define ADC_UPDATING            0x10
//Bits 2-0 are multistage status bits, see Register 23-5: ADCSTAT in
//device's datasheet for details (ignore other bits).

//The following variable can be used to clear ADCNT and ADACC registers.
#bit ADACLR = getenv("BIT:ADACLR")

//ADC Prototypes:
_bif void setup_adc_ports(int32 pins);
_bif void setup_adc_ports(int32 pins, int32 reference); 
_bif void set_analog_pins(int8 pin, ...);
_bif void setup_adc_reference(int32 reference);
_bif void set_adc_channel(int8 channel);
// Constants used in SETUP_ADC_PORTS() are:
// First parameter:
// OR together desired pins
#define sAN0          0x01000000  //| A0
#define sAN1          0x02000000  //| A1
#define sAN2          0x04000000  //| A2
#define sAN4          0x10000000  //| A4
#define sAN5          0x20000000  //| A5 
#define sAN16         0x00000100  //| C0 
#define sAN17         0x00000200  //| C1 
#define sAN18         0x00000400  //| C2
#define sAN19         0x00000800  //| C3
#define sAN20         0x00001000  //| C4
#define sAN21         0x00002000  //| C5 
#define NO_ANALOGS             0  // None
#define ALL_ANALOG    0x37003F00  // A0 A1 A2 A4 A5 C0 C1 C2 C3 C4 C5 

// Optional Second parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE: 
#define VSS_VDD       0x00000000  //| Range 0-Vdd
#define VSS_VREF      0x00000002  //| Range 0-VrefH
#define VSS_FVR       0x00000003  //| Range 0-Fixed Voltage Reference
#define VREF_VDD      0x00000010  //| Range VrefL-Vdd 
#define VREF_VREF     0x00000012  //| Range VrefL-VrefH 
#define VREF_FVR      0x00000013  //| Range VrefL-Fixed Voltage Reference 

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
_bif void read_device_info(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count);   
_bif void read_config_info(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count);   
// Constants used in read_device_info() offset parameter:   
#define DINFO_MUI                     0   
#define DINFO_EUI                     10   
#define DINFO_TI_LOW_90C              19   
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
#define INT_EXT_L2H               0x50001601
#define INT_EXT_H2L               0x60001601
#define INT_EXT                   0x301601
#define INT_TIMER0                0x301620
#define INT_AD                    0x301701
#define INT_AD_THRESHOLD            0x301702
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
#define INT_TIMER3                0x301A04
#define INT_TIMER4                0x301A08
#define INT_TIMER5                0x301A10
#define INT_TIMER6                0x301A20
#define INT_TIMER1_GATE            0x301B01
#define INT_TIMER3_GATE            0x301B02
#define INT_TIMER5_GATE            0x301B04
#define INT_CLC                   0x301B10
#define INT_CLC2                  0x301B20
#define INT_CLC3                  0x301B40
#define INT_CLC4                  0x301B80
#define INT_CCP1                  0x301C01
#define INT_CCP2                  0x301C02
#define INT_CCP3                  0x301C04
#define INT_CCP4                  0x301C08
#define INT_CWG                   0x301D01
#define INT_CWG2                  0x301D02
#define INT_NCO                   0x301D10
#define INT_SMT1                  0x301E01
#define INT_SMT1PRA               0x301E02
#define INT_SMT1PWA               0x301E04
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

#list
