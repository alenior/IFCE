//////////// Standard Header file for the PIC16LF1615 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16LF1615

#nolist
//////// Program memory: 8192x14  Data RAM: 992  Stack: 16
//////// I/O: 12   Analog Pins: 8
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: HS,INTRC_IO,ECL,ECM,ECH,PUT,NOPUT,MCLR,NOMCLR,PROTECT
//////// Fuses: NOPROTECT,NOBROWNOUT,BROWNOUT_SW,BROWNOUT_NOSL,BROWNOUT
//////// Fuses: CLKOUT,NOCLKOUT,IESO,NOIESO,FCMEN,NOFCMEN,WRT,WRT_200,NOWRT
//////// Fuses: PPS1WAY,NOPPS1WAY,ZCDDIS,NOZCDDIS,PLLEN,NOPLLEN,STVREN
//////// Fuses: NOSTVREN,BORV25,BORV19,LPBOR,NOLPBOR,DEBUG,NODEBUG,LVP,NOLVP
//////// Fuses: WDT32,WDT64,WDT128,WDT256,WDT512,WDT1024,WDT2048,WDT4096
//////// Fuses: WDT8192,WDT16384,WDT32768,WDT65536,WDT131072,WDT262144
//////// Fuses: WDT524299,WDT1048576,WDT2097152,WDT4194304,WDT8388608,WDTSW
//////// Fuses: NOWDT,WDT_SW,WDT_NOSL,WDT,WDTWIN_12%,WDTWIN_25%,WDTWIN_37%
//////// Fuses: WDTWIN_50%,WDTWIN_62%,WDTWIN_75%,WDTWIN_100%,WDTWIN_SW
//////// Fuses: WDTCLK_LFINTRC,WDTCLK_HFINTRC,WDTCLK_SW
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
//    INT0,T0CK,T1CK,T1G,CCP1,CCP2,ATIN,CWGIN,T2CK,T3CK,T3G,T4CK,T5CK,T5G,T6CK,
//    ATCC1,SCK1IN,SCL1IN,SDI1,SDA1IN,SS1IN,ATCC2,U1RX,U1CKIN,SMT1SIG,SMT1WIN,
//    CLCIN0,CLCIN1,CLCIN2,CLCIN3,SMT2SIG,SMT2WIN,ATCC3,T0CKI,T1CKI,T2CKI,T2RST,
//    T3CKI,T4CKI,T4RST,T5CKI,T6CKI,T6RST,RX1
// Output Functions: 
//    NULL,C1OUT,C2OUT,ZCD1OUT,CLC1OUT,CLC2OUT,CLC3OUT,CLC4OUT,CWG1OUTA,CWG1OUTB,
//    CWG1OUTC,CWG1OUTD,CCP1OUT,CCP2OUT,PWM3OUT,PWM4OUT,SCK1OUT,SCL1OUT,SDA1OUT,
//    SDO1,U1TX,U1DT,PWM3,PWM4,SCK1,SCL1,SDA1,TX1,DT1
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
_bif int8 restart_cause(void);       
_bif void reset_cpu(void);
_bif void sleep(void);
_bif void sleep(int8 type); 
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

// Constants for calls to sleep() are:
#define REG_LOW_POWER            2
#define REG_NORMAL               0  // default

////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER_0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC()
// Timer 0 Prototypes:
_bif void setup_timer_0(int8 mode);
_bif void set_timer0(int8 value);      
_bif int8 get_timer0(void);            
_bif void setup_counters(int8 mode, int8 prescaler);
_bif void set_rtcc(int8 value);        
_bif int8 get_rtcc(void);              
// Constants used for SETUP_TIMER_0() are:
#define T0_INTERNAL   0   
#define T0_EXT_L_TO_H 32
#define T0_EXT_H_TO_L 48

#define T0_DIV_1      8
#define T0_DIV_2      0
#define T0_DIV_4      1
#define T0_DIV_8      2
#define T0_DIV_16     3
#define T0_DIV_32     4
#define T0_DIV_64     5
#define T0_DIV_128    6
#define T0_DIV_256    7


#define T0_8_BIT      0     

// The following are provided for compatibility
// with older compiler versions
#define RTCC_INTERNAL   0  
#define RTCC_EXT_L_TO_H 32     
#define RTCC_EXT_H_TO_L 48
#define RTCC_DIV_1      8
#define RTCC_DIV_2      0
#define RTCC_DIV_4      1
#define RTCC_DIV_8      2
#define RTCC_DIV_16     3
#define RTCC_DIV_32     4
#define RTCC_DIV_64     5
#define RTCC_DIV_128    6
#define RTCC_DIV_256    7
#define RTCC_8_BIT      0     

// Constants used for SETUP_COUNTERS() are the above
// constants for the 1st parameter and the following for
// the 2nd parameter:

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
#define T1_INTERNAL         0x07     
#define T1_EXTERNAL         0x87     
#define T1_EXTERNAL_SYNC    0x83     
#define T1_FOSC             0x47     

#define T1_ENABLE_SOSC      0x08     

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE             0x8000
#define T1_GATE_INVERTED    0xC000
#define T1_GATE_TOGGLE      0xA000
#define T1_GATE_SINGLE      0x9000
#define T1_GATE_TIMER2      0x8100        
#define T1_GATE_COMP1       0x8200     
#define T1_GATE_COMP2       0x8300      

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
#define T2_CLK_INTERNAL                            0 //FOSC/4  
#define T2_CLK_FOSC                                0x0100  
#define T2_CLK_HFINTRC                             0x0200  
#define T2_CLK_LFINTRC                             0x0300  
#define T2_CLK_ZCD                                 0x0400  
#define T2_CLK_MFINTRC                             0x0500      
#define T2_CLK_EXTERNAL                            0x0600  
#define T2_CLK_CLC2                                0x0800   
#define T2_CLK_CLC3                                0x0900   
#define T2_CLK_CLC4                                0x0A00   
#define T2_CLK_AT1                                 0x0B00   
#define T2_CLK_CLC1                                0x0C00   

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
#define T2_MONO_STABLE_START_ON_RE_RESTART_ON_RE   0x110000   
#define T2_MONO_STABLE_START_ON_FE_RESTART_ON_FE   0x120000   
#define T2_MONO_STABLE_START_ON_EE_RESTART_ON_EE   0x130000   
#define T2_ONE_SHOT_START_ON_HIGH                  0x160000   
#define T2_ONE_SHOT_START_ON_LOW                   0x170000   
//Any of the following may be OR'ed with the above
#define T2_SYNC_ON_WITH_T2CLK                      0x200000
#define T2_INVERTED                                0x400000
#define T2_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T2_RESET_FROM_T2IN                         0
#define T2_RESET_FROM_C1OUT                        0x01000000  
#define T2_RESET_FROM_C2OUT                        0x02000000  
#define T2_RESET_FROM_CCP1                         0x03000000        
#define T2_RESET_FROM_CCP2                         0x04000000        
#define T2_RESET_FROM_TMR4                         0x06000000  
#define T2_RESET_FROM_TMR6                         0x07000000  
#define T2_RESET_FROM_ZCD1                         0x08000000  
#define T2_RESET_FROM_CLC1                         0x09000000   
#define T2_RESET_FROM_CLC2                         0x0A000000   
#define T2_RESET_FROM_CLC3                         0x0B000000   
#define T2_RESET_FROM_CLC4                         0x0C000000   
#define T2_RESET_FROM_PWM3                         0x0D000000   
#define T2_RESET_FROM_PWM4                         0x0E000000   

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

#define T3_ENABLE_SOSC      0x08   

#define T3_DIV_BY_1         0
#define T3_DIV_BY_2         0x10
#define T3_DIV_BY_4         0x20
#define T3_DIV_BY_8         0x30

#define T3_GATE             0x8000
#define T3_GATE_INVERTED    0xC000
#define T3_GATE_TOGGLE      0xA000
#define T3_GATE_SINGLE      0x9000
#define T3_GATE_TIMER4      0x8100    
#define T3_GATE_COMP1       0x8200  
#define T3_GATE_COMP2       0x8300  

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
#define T4_CLK_INTERNAL                            0 //FOSC/4  
#define T4_CLK_FOSC                                0x0100  
#define T4_CLK_HFINTRC                             0x0200  
#define T4_CLK_LFINTRC                             0x0300  
#define T4_CLK_ZCD                                 0x0400  
#define T4_CLK_MFINTRC                             0x0500  
#define T4_CLK_EXTERNAL                            0x0600  
#define T4_CLK_CLC2                                0x0800   
#define T4_CLK_CLC3                                0x0900   
#define T4_CLK_CLC4                                0x0A00   
#define T4_CLK_AT1                                 0x0B00   
#define T4_CLK_CLC1                                0x0C00   

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
#define T4_MONO_STABLE_START_ON_RE_RESTART_ON_RE   0x110000   
#define T4_MONO_STABLE_START_ON_FE_RESTART_ON_FE   0x120000   
#define T4_MONO_STABLE_START_ON_EE_RESTART_ON_EE   0x130000   
#define T4_ONE_SHOT_START_ON_HIGH                  0x160000   
#define T4_ONE_SHOT_START_ON_LOW                   0x170000   

//Any of the following may be OR'ed with the above 
#define T4_SYNC_ON_WITH_T2CLK                      0x200000
#define T4_INVERTED                                0x400000
#define T4_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T4_RESET_FROM_T4IN                         0
#define T4_RESET_FROM_C1OUT                        0x01000000  
#define T4_RESET_FROM_C2OUT                        0x02000000  
#define T4_RESET_FROM_CCP1                         0x03000000       
#define T4_RESET_FROM_CCP2                         0x04000000       
#define T4_RESET_FROM_TMR2                         0x05000000  
#define T4_RESET_FROM_TMR6                         0x07000000  
#define T4_RESET_FROM_ZCD1                         0x08000000  
#define T4_RESET_FROM_CLC1                         0x09000000   
#define T4_RESET_FROM_CLC2                         0x0A000000   
#define T4_RESET_FROM_CLC3                         0x0B000000   
#define T4_RESET_FROM_CLC4                         0x0C000000   
#define T4_RESET_FROM_PWM3                         0x0D000000   
#define T4_RESET_FROM_PWM4                         0x0E000000   

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
#define T5_GATE_TIMER6      0x8100    
#define T5_GATE_COMP1       0x8200  
#define T5_GATE_COMP2       0x8300  

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
#define T6_CLK_INTERNAL                            0 //FOSC/4 
#define T6_CLK_FOSC                                0x0100 
#define T6_CLK_HFINTRC                             0x0200 
#define T6_CLK_LFINTRC                             0x0300 
#define T6_CLK_ZCD                                 0x0400 
#define T6_CLK_MFINTRC                             0x0500 
#define T6_CLK_EXTERNAL                            0x0600 
#define T6_CLK_CLC2                                0x0800  
#define T6_CLK_CLC3                                0x0900  
#define T6_CLK_CLC4                                0x0A00  
#define T6_CLK_AT1                                 0x0B00  
#define T6_CLK_CLC1                                0x0C00  

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
#define T6_MONO_STABLE_START_ON_RE_RESTART_ON_RE   0x110000  
#define T6_MONO_STABLE_START_ON_FE_RESTART_ON_FE   0x120000  
#define T6_MONO_STABLE_START_ON_EE_RESTART_ON_EE   0x130000  
#define T6_ONE_SHOT_START_ON_HIGH                  0x160000  
#define T6_ONE_SHOT_START_ON_LOW                   0x170000  
//Any of the following may be OR'ed with the above
#define T6_SYNC_ON_WITH_T2CLK                      0x200000
#define T6_INVERTED                                0x400000
#define T6_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T6_RESET_FROM_T6IN                         0
#define T6_RESET_FROM_C1OUT                        0x01000000 
#define T6_RESET_FROM_C2OUT                        0x02000000 
#define T6_RESET_FROM_CCP1                         0x03000000 
#define T6_RESET_FROM_CCP2                         0x04000000 
#define T6_RESET_FROM_TMR2                         0x05000000 
#define T6_RESET_FROM_TMR4                         0x06000000 
#define T6_RESET_FROM_ZCD1                         0x08000000 
#define T6_RESET_FROM_CLC1                         0x09000000  
#define T6_RESET_FROM_CLC2                         0x0A000000  
#define T6_RESET_FROM_CLC3                         0x0B000000  
#define T6_RESET_FROM_CLC4                         0x0C000000  
#define T6_RESET_FROM_PWM3                         0x0D000000  
#define T6_RESET_FROM_PWM4                         0x0E000000  

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
#define CCP_OUTPUT_ENABLED               0x40 
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

////////////////////////////////////////////////////////////////// PWM
// PWM Functions: SETUP_PWMx, SET_PWMx_DUTY
// PWM Prototypes:
_bif void setup_pwm3(int32 mode);   
_bif void set_pwm3_duty(int8 value);   
_bif void set_pwm3_duty(int16 value);   
_bif void setup_pwm4(int32 mode);   
_bif void set_pwm4_duty(int8 value);   
_bif void set_pwm4_duty(int16 value);   
// Constants used for SETUP_PWMx() are:
#define PWM_ENABLED      0x80
#define PWM_DISABLED     0x10000
#define PWM_OUTPUT       0x40     
#define PWM_ACTIVE_LOW   0x10
#define PWM_ACTIVE_HIGH  0
// One of the following may be OR'ed in with the above 
#define PWM_TIMER2       0x0000 
#define PWM_TIMER4       0x0100 
#define PWM_TIMER6       0x0200  

#bit PWM3OUT=getenv("BIT:PWM3OUT")   
#bit PWM4OUT=getenv("BIT:PWM4OUT")   

////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IS_IN 
// SPI Prototypes:
_bif void setup_spi(int32 mode);
_bif void spi_write(int8 value);
_bif void spi_write(int1 wait, int8 value);
_bif int8 spi_read(void);
_bif int8 spi_read(int8 value);
_bif int1 spi_data_is_in(void);
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
#define NC_NC_NC_NC      0x00 

//Pick one constant for COMP1
#define CP1_A1_A0        0x8000
#define CP1_C1_A0        0x8001 
#define CP1_C2_A0        0x8002 
#define CP1_C3_A0        0x8003 
#define CP1_VSS_A0       0x8007 
#define CP1_A1_DAC       0x8010
#define CP1_C1_DAC       0x8011 
#define CP1_C2_DAC       0x8012 
#define CP1_C3_DAC       0x8013 
#define CP1_VSS_DAC      0x8017 
#define CP1_A1_FVR       0x8020
#define CP1_C1_FVR       0x8021 
#define CP1_C2_FVR       0x8022 
#define CP1_C3_FVR       0x8023 
#define CP1_VSS_FVR      0x8027 
#define CP1_A1_VSS       0x8030 
#define CP1_C1_VSS       0x8031 
#define CP1_A4_VSS       0x8031 
#define CP1_C2_VSS       0x8032 
#define CP1_C3_VSS       0x8033 
#define CP1_VSS_VSS      0x8037 
//Optionally OR with any of the following
#define CP1_INT_L2H      0x0080
#define CP1_INT_H2L      0x0040
#define CP1_OUT_ON_C1OUT 0x2000  
#define CP1_INVERT       0x1000
#define CP1_FAST         0x0400
#define CP1_HYST         0x0200
#define CP1_SYNC         0x0100

//Pick one constant for COMP2 
#define CP2_A1_C0        0x80000000 
#define CP2_C1_C0        0x80010000 
#define CP2_C2_C0        0x80020000 
#define CP2_C3_C0        0x80030000 
#define CP2_VSS_C0       0x80070000  
#define CP2_A1_DAC       0x80100000 
#define CP2_C1_DAC       0x80110000 
#define CP2_C2_DAC       0x80120000 
#define CP2_C3_DAC       0x80130000 
#define CP2_VSS_DAC      0x80170000  
#define CP2_A1_FVR       0x80200000 
#define CP2_C1_FVR       0x80210000 
#define CP2_C2_FVR       0x80220000 
#define CP2_C3_FVR       0x80230000 
#define CP2_VSS_FVR      0x80270000  
#define CP2_A1_VSS       0x80300000 
#define CP2_C1_VSS       0x80310000 
#define CP2_C2_VSS       0x80320000 
#define CP2_C3_VSS       0x80330000 
#define CP2_VSS_VSS      0x80370000  
//Optionally OR with any of the following 
#define CP2_INT_L2H      0x00800000 
#define CP2_INT_H2L      0x00400000 
#define CP2_OUT_ON_C2OUT 0x20000000  
#define CP2_INVERT       0x10000000 
#define CP2_FAST         0x04000000 
#define CP2_HYST         0x02000000 
#define CP2_SYNC         0x01000000 

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
#define DAC_OFF         0
#define DAC_VDD         0x80
#define DAC_VREF        0x84
#define DAC_FVR         0x88
// The following may be OR'ed in with the above using |
#define DAC_OUTPUT1     0x20

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
#define CWG_INPUT_CWG1IN               0x0000       
#define CWG_INPUT_COMP1                0x1000        
#define CWG_INPUT_COMP2                0x2000         
#define CWG_INPUT_CCP1                 0x3000        
#define CWG_INPUT_CCP2                 0x4000        
#define CWG_INPUT_CLC1                 0x5000          
#define CWG_INPUT_CLC2                 0x6000          
#define CWG_INPUT_PWM3                 0x9000          
#define CWG_INPUT_PWM4                 0xA000          
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
#define CWG_SHUTDOWN_ON_COMP1          0x0002         
#define CWG_SHUTDOWN_ON_COMP2          0x0004          
#define CWG_SHUTDOWN_ON_TMR2           0x0010          
#define CWG_SHUTDOWN_ON_TMR4           0x0020          
#define CWG_SHUTDOWN_ON_TMR6           0x0040          
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
#define CLC_OUTPUT                          0x040          
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
#define CLC_INPUT_CWGA                      0x0A   
#define CLC_INPUT_CWGB                      0x0B   
#define CLC_INPUT_CCP1OUT                   0x0C          
#define CLC_INPUT_CCP2OUT                   0x0D          
#define CLC_INPUT_PWM3                      0x0E           
#define CLC_INPUT_PWM4                      0x0F           
#define CLC_INPUT_AT1_CMP1                  0x10  
#define CLC_INPUT_AT1_CMP2                  0x11  
#define CLC_INPUT_AT1_CMP3                  0x12  
#define CLC_INPUT_SMT1                      0x13  
#define CLC_INPUT_SMT2                      0x14  
#define CLC_INPUT_ZCD                       0x15  
#define CLC_INPUT_TIMER0                    0x16  
#define CLC_INPUT_TIMER1                    0x17  
#define CLC_INPUT_TIMER2                    0x18  
#define CLC_INPUT_TIMER3                    0x19  
#define CLC_INPUT_TIMER4                    0x1A  
#define CLC_INPUT_TIMER5                    0x1B  
#define CLC_INPUT_TIMER6                    0x1C  
#define CLC_INPUT_IOC                       0x1D  
#define CLC_INPUT_ADCRC                     0x1E  
#define CLC_INPUT_LFINTOSC                  0x1F  
#define CLC_INPUT_HFINTOSC                  0x20  
#define CLC_INPUT_FOSC                      0x21  
#define CLC_INPUT_AT1_MISSED_PULSE          0x22  
#define CLC_INPUT_AT1_PERCLK                0x23  
#define CLC_INPUT_AT1_PHSCLK                0x24  
#define CLC_INPUT_U1TX                      0x25  
#define CLC_INPUT_U1RX                      0x26  
#define CLC_INPUT_SCK1                      0x27  
#define CLC_INPUT_SDO1                      0x28  

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

////////////////////////////////////////////////////////////////// PID
// PID Functions: SETUP_PID(), PID_SET_POINT(), PID_GET_RESULT(),
//                PID_READ(), PID_WRITE(), PID_BUSY()
// PID Structure used in PID_GET_RESULT, PID_READ and PID_WRITE functions (pass pointer):
typedef struct
{
   unsigned int32 l;
   unsigned int32 u;
} pid_struct_t;
// PID Prototypes:
_bif void setup_pid(int8 settings);
_bif void setup_pid(int8 settings, int16 K1);
_bif void setup_pid(int8 settings, int16 K1, int16 K2);
_bif void setup_pid(int8 settings, int16 K1, int16 K2, int16 K3);
_bif void pid_set_point(int16 set_point);
_bif void pid_get_result(int16 input, pid_struct_t *result);
_bif void pid_get_result(int8 mode, int16 input);
_bif void pid_get_result(int8 mode, pid_struct_t *result);
_bif void pid_get_result(int8 mode, int16 input, pid_struct_t *result);
_bif void pid_read(int8 which, int16 *result);
_bif void pid_read(int8 which, int32 *result);
_bif void pid_read(int8 which, pid_struct_t *result);
_bif void pid_write(int8 which, int16 *data);
_bif void pid_write(int8 which, int32 *data);
_bif void pid_write(int8 which, pid_struct_t *data);
_bif int1 pid_busy(void);
// Constants used in SETUP_PID() first parameter are:
// Using only one of the following defines will enable or disable the PID module
// without changing any of the module's settings.
#define PID_ENABLED                                        1
#define PID_DISABLED                                       0
// One of the following may be used
#define PID_MODE_PID                                       0x85
#define PID_MODE_SIGNED_ADD_MULTIPLY_WITH_ACCUMULATION     0x83
#define PID_MODE_SIGNED_ADD_MULTIPLY                       0x82
#define PID_MODE_UNSIGNED_ADD_MULTIPLY_WITH_ACCUMULATION   0x81
#define PID_MODE_UNSIGNED_ADD_MULTIPLY                     0x80
// One of the following may be OR'ed in with the above using |
#define PID_OUTPUT_LEFT_JUSTIFIED                          0x08
#define PID_OUTPUT_RIGHT_JUSTIFIED                         0x00

//Constants used in PID_GET_RESULT() first parameter are:
#define PID_START_AND_READ                                 0x03
#define PID_READ_ONLY                                      0x02
#define PID_START_ONLY                                     0x01

//Constants used in PID_READ() and PID_WRITE() first parameter are:
#define PID_ADDR_ACCUMULATOR                               0x00
#define PID_ADDR_OUTPUT                                    0x01
#define PID_ADDR_Z1                                        0x02
#define PID_ADDR_Z2                                        0x03
#define PID_ADDR_SET_POINT                                 0x04
#define PID_ADDR_K1                                        0x05
#define PID_ADDR_K2                                        0x06
#define PID_ADDR_K3                                        0x07

////////////////////////////////////////////////////////////////// AT
// AT Functions: SETUP_AT(), AT_SET_RESOLUTION(), AT_GET_RESOLUTION(),
//               AT_SET_MISSING_PULSE_DELAY(), AT_GET_MISSING_PULSE_DELAY(),
//               AT_GET_PERIOD(), AT_GET_PHASE_COUNTER(), AT_SET_SET_POINT(),
//               AT_GET_SET_POINT(), AT_GET_SET_POINT_ERROR(),
//               AT_ENABLE_INTERRUPTS(), AT_DISABLE_INTERRUPTS(),
//               AT_CLEAR_INTERRUPTS(), AT_INTERRUPT_ACTIVE(), AT_SETUP_CC(),
//               AT_SET_COMPARE_TIME(), AT_GET_CAPTURE(), AT_GET_STATUS()
// AT Prototypes:
_bif void setup_at(int32 settings);
_bif void at_set_resolution(int16 resolution);
_bif int16 at_get_resolution(void);
_bif void at_set_missing_pulse_delay(signed int16 missing_pulse_delay);
_bif signed int16 at_get_missing_pulse_delay(void);
_bif int16 at_get_period(void);
_bif int16 at_get_phase_counter(void);
_bif void at_set_set_point(int16 set_point);
_bif int16 at_get_set_point(void);
_bif int16 at_get_set_point_error(void);
_bif void at_enable_interrupts(int8 interrupts);
_bif void at_disable_interrupts(int8 interrupts);
_bif void at_clear_interrupts(int8 interrupt);
_bif int1 at_interrupt_active(int8 interrupt);
_bif void at_setup_cc(int8 which, int16 settings);
_bif void at_set_compare_time(int8 which, int16 compare_time);
_bif int16 at_get_capture(int8 which);
_bif int8 at_get_status(void);
// Constants used in SETUP_AT() are:
#define AT_ENABLED                           0x80
#define AT_DISABLED                          0
// One of the following may be OR'ed in with the above using |
#define AT_MULTI_PULSE_MODE                  0x01
#define AT_SINGLE_PULSE_MODE                 0
// One of the following may be OR'ed in with the above using |
#define AT_ADAPTIVE_MISSING_PULSE_MODE       0x02
#define AT_FIXED_MISSING_PULSE_MODE          0
// One of the following may be OR'ed in with the above using |
#define AT_FALLING_EDGE_ATSIG                0x08
#define AT_RISING_EDGE_ATSIG                 0
// One of the following may be OR'ed in with the above using |
#define AT_CLOCK_PRESCALAR_DIV_8             0x30
#define AT_CLOCK_PRESCALAR_DIV_4             0x20
#define AT_CLOCK_PRESCALAR_DIV_2             0x10
#define AT_CLOCK_PRESCALAR_DIV_1             0
// One of the following may be OR'ed in with the above using |
#define AT_PRESCALAR_RESET_EVERY_PERIOD      0x40
#define AT_PRESCALAR_NOT_RESET               0
// One of the following may be OR'ed in with the above using |
#define AT_MISSING_PULSE_ACTIVE_LOW          0x0400
#define AT_MISSING_PULSE_ACTIVE_HIGH         0x0000
// One of the following may be OR'ed in with the above using |
#define AT_PERIOD_CLOCK_ACTIVE_LOW           0x1000
#define AT_PERIOD_CLOCK_ACTIVE_HIGH          0
// One of the following may be OR'ed in with the above using |
#define AT_PHASE_CLOCK_ACTIVE_LOW            0x4000
#define AT_PHASE_CLOCK_ACTIVE_HIGH           0
// One of the following may be OR'ed in with the above using |
#define AT_CLOCK_HFINTOSC                    0x010000
#define AT_CLOCK_FOSC                        0
// One of the following may be OR'ed in with the above using |
#define AT_INPUT_CLC4                        0x07000000
#define AT_INPUT_CLC3                        0x06000000
#define AT_INPUT_CLC2                        0x05000000
#define AT_INPUT_CLC1                        0x04000000
#define AT_INPUT_ZCD1                        0x03000000
#define AT_INPUT_CMP2                        0x02000000
#define AT_INPUT_CMP1                        0x01000000
#define AT_INPUT_ATIN                        0

// Constants used in AT_ENABLE_INTERRUPTS(), AT_DISABLE_INTERRUPTS(),
// AT_CLEAR_INTERRUPTS() and AT_INTERRUPT_ACTIVE() are:
#define AT_PHASE_INTERRUPT                   0x0004
#define AT_MISSING_PULSE_INTERRUPT           0x0002
#define AT_PERIOD_INTERRUPT                  0x0001
#define AT_CC3_INTERRUPT                     0x0400
#define AT_CC2_INTERRUPT                     0x0200
#define AT_CC1_INTERRUPT                     0x0100

// Constants used in AT_SETUP_CC() second parameter are:
#define AT_CC_ENABLED                        0x80
#define AT_CC_DISABLED                       0
// One of the following may be OR'ed in with the above using |
#define AT_CC_CAPTURE_MODE                   0x01
#define AT_CC_COMPARE_MODE                   0
// One of the following may be OR'ed in with the above using |
#define AT_CAPTURE_FALLING_EDGE              0x08
#define AT_CAPTURE_RISING_EDGE               0
// One of the following may be OR'ed in with the above using |
#define AT_CC_ACTIVE_LOW                     0x10
#define AT_CC_ACTIVE_HIGH                    0
// One of the following may be OR'ed in with the above using |
#define AT_CAPTURE_INPUT_CWG_INTERRUPT       0x0700
#define AT_CAPTURE_INPUT_CLC4                0x0600
#define AT_CAPTURE_INPUT_CLC3                0x0500
#define AT_CAPTURE_INPUT_CLC2                0x0400
#define AT_CAPTURE_INPUT_CLC1                0x0300
#define AT_CAPTURE_INPUT_CMP2                0x0200
#define AT_CAPTURE_INPUT_CMP1                0x0100
#define AT_CAPTURE_INPUT_ATCAP               0

// Constants returned from AT_GET_STATUS() are:
#define AT_STATUS_PERIOD_AND_PHASE_VALID     0x01
#define AT_STATUS_PERIOD_LESS_THEN_PREVIOUS  0x02

////////////////////////////////////////////////////////////////// CRC
// CRC Functions: setup_crc(), crc_init(), crc_calc16(),
//                crc_calc(), crc_calc8(), crc_write(), crc_read()
// CRC Prototypes:
_bif void setup_crc(unsigned int8 poly_terms, ...);
_bif void crc_init(unsigned int16 value);
_bif unsigned int16 crc_calc16(unsigned int16 data);
_bif unsigned int16 crc_calc16(unsigned int16 data, unsigned int8 data_width);
_bif unsigned int16 crc_calc16(unsigned int8 data, unsigned int8 data_width);
_bif unsigned int16 crc_calc16(unsigned int16 *ptr, unsigned int16 count);
_bif unsigned int16 crc_calc16(unsigned int16 *ptr, unsigned int16 count, unsigned int8 data_width);
_bif unsigned int16 crc_calc16(unsigned int8 *ptr, unsigned int16 count, unsigned int8 data_width);
_bif unsigned int8 crc_calc8(unsigned int8 data);
_bif unsigned int8 crc_calc8(unsigned int8 data, unsigned int8 data_width);
_bif unsigned int8 crc_calc8(unsigned int8 *ptr, unsigned int16 count);
_bif unsigned int8 crc_calc8(unsigned int8 *ptr, unsigned int16 count, unsigned int8 data_width);
_bif unsigned int16 crc_calc(unsigned int16 data);
_bif unsigned int16 crc_calc(unsigned int16 data, unsigned int8 data_width);
_bif unsigned int16 crc_calc(unsigned int8 data, unsigned int8 data_width);
_bif unsigned int16 crc_calc(unsigned int16 *ptr, unsigned int16 count);
_bif unsigned int16 crc_calc(unsigned int16 *ptr, unsigned int16 count, unsigned int8 data_width);
_bif unsigned int16 crc_calc(unsigned int8 *ptr, unsigned int16 count, unsigned int8 data_width);
_bif void crc_write(unsigned int16 data);
_bif void crc_write(unsigned int16 data, unsigned int8 data_width);
_bif void crc_write(unsigned int8 data, unsigned int8 data_width);
_bif unsigned int16 crc_read(void);
// Constants used in setup_crc() are:
#define CRC_LITTLE_ENDIAN    128

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
_bif void setup_smt2(unsigned int32 mode);     
_bif void setup_smt2(unsigned int32 mode, unsigned int32 period);     
_bif unsigned int8 smt2_status(void);     
_bif void smt2_start(void);     
_bif void smt2_stop(void);     
_bif void smt2_update(unsigned int8 which);     
_bif void smt2_reset_timer(void);     
_bif unsigned int32 smt2_read(unsigned int8 which);     
_bif void smt2_write(unsigned int8 which, unsigned int32 value);     
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
#define SMT_CLK_AT1_PERCLK                  0x060000  
#define SMT_CLK_500000                      0x050000 // MFINTOSC  
#define SMT_CLK_31250                       0x040000 // MFINTOSC/16      
#define SMT_CLK_31000                       0x030000 // LFINTOSC
#define SMT_CLK_HFINTOSC                    0x020000 // 16MHz    
#define SMT_CLK_FOSC_DIV_4                  0x010000      
#define SMT_CLK_FOSC                        0      
// One of the following may be OR'ed in with the above using |
#define SMT_WIN_INPUT_MFINTOSC_DIV_16       0x87000000  
#define SMT_WIN_INPUT_AT1_PERCLK            0x86000000  
#define SMT_WIN_INPUT_LFINTOSC              0x85000000  
#define SMT_WIN_INPUT_PWM4                  0x84000000  
#define SMT_WIN_INPUT_PWM3                  0x83000000  
#define SMT_WIN_INPUT_SMT2                  0x82000000  
#define SMT_WIN_INPUT_SMT1                  0x81000000  
#define SMT_WIN_INPUT_TMR0                  0x80000000  
#define SMT_WIN_INPUT_TMR5                  0x0F000000  
#define SMT_WIN_INPUT_TMR3                  0x0E000000  
#define SMT_WIN_INPUT_TMR1                  0x0D000000  
#define SMT_WIN_INPUT_CLC4OUT               0x0C000000  
#define SMT_WIN_INPUT_CLC3OUT               0x0B000000  
#define SMT_WIN_INPUT_CLC2OUT               0x0A000000  
#define SMT_WIN_INPUT_CLC1OUT               0x09000000  
#define SMT_WIN_INPUT_TMR6                  0x08000000      
#define SMT_WIN_INPUT_TMR4                  0x07000000      
#define SMT_WIN_INPUT_TMR2                  0x06000000      
#define SMT_WIN_INPUT_ZCD1                  0x05000000      
#define SMT_WIN_INPUT_CCP2                  0x04000000      
#define SMT_WIN_INPUT_CCP1                  0x03000000      
#define SMT_WIN_INPUT_COMP2                 0x02000000      
#define SMT_WIN_INPUT_COMP1                 0x01000000      
#define SMT_WIN_INPUT_SMTWINx               0
// One of the following may be OR'ed in with the above using |
#define SMT_SIG_INPUT_PWM4                  0x40200000  
#define SMT_SIG_INPUT_PWM3                  0x30200000  
#define SMT_SIG_INPUT_CCP2                  0x20200000  
#define SMT_SIG_INPUT_CCP1                  0x10200000  
#define SMT_SIG_INPUT_TMR0                  0x00200000  
#define SMT_SIG_INPUT_SMT2                  0x70100000  
#define SMT_SIG_INPUT_SMT1                  0x60100000  
#define SMT_SIG_INPUT_TMR5                  0x50100000  
#define SMT_SIG_INPUT_TMR3                  0x40100000  
#define SMT_SIG_INPUT_TMR1                  0x30100000  
#define SMT_SIG_INPUT_CLC4                  0x20100000  
#define SMT_SIG_INPUT_CLC3                  0x10100000  
#define SMT_SIG_INPUT_CLC2                  0x00100000  
#define SMT_SIG_INPUT_CLC1                  0x70000000  
#define SMT_SIG_INPUT_TMR6                  0x60000000      
#define SMT_SIG_INPUT_TMR4                  0x50000000      
#define SMT_SIG_INPUT_TMR2                  0x40000000      
#define SMT_SIG_INPUT_ZCD1                  0x30000000      
#define SMT_SIG_INPUT_COMP2                 0x20000000      
#define SMT_SIG_INPUT_COMP1                 0x10000000      
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


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes
_bif int8 setup_oscillator(int16 mode);
_bif int8 setup_oscillator(int16 mode, signed int8 tune);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_31KHZ                 0
#define OSC_31250_MF              0x10
#define OSC_31250                 0x18
#define OSC_62500                 0x20
#define OSC_125KHZ_MF             0x28
#define OSC_250KHZ_MF             0x30
#define OSC_500KHZ_MF             0x38
#define OSC_125KHZ                0x40
#define OSC_250KHZ                0x48
#define OSC_500KHZ                0x50
#define OSC_1MHZ                  0x58
#define OSC_2MHZ                  0x60
#define OSC_4MHZ                  0x68
#define OSC_8MHZ                  0x70
#define OSC_16MHZ                 0x78
#define OSC_32MHZ                 0xF0
// The following may be OR'ed in with the above using |
#define OSC_INTRC                 0x03
#define OSC_NORMAL                0
// A second optional parameter may be used with this part to fine
// tune the speed (signed int,-32 to 31)
// Result may be (ignore all other bits)
#define OSC_HFINTRC_STABLE        0x01
#define OSC_LFINTRC_READY         0x02
#define OSC_MFINTRC_READY         0x04
#define OSC_HFINTRC_LOCKED        0x08
#define OSC_HFINTRC_READY         0x10
#define OSC_RUNNING_FROM_FUSES    0x20 //Device is running from the clock defined by FOSC<2:0> of the CONFIG1 register 
#define OSC_PLL_READY             0x40


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
#define sAN0            0x00010000    //| A0
#define sAN1            0x00020000    //| A1
#define sAN2            0x00040000    //| A2
#define sAN3            0x00100000    //| A4
#define sAN4            0x00000100    //| C0  
#define sAN5            0x00000200    //| C1  
#define sAN6            0x00000400    //| C2  
#define sAN7            0x00000800    //| C3  
#define NO_ANALOGS             0      // None
#define ALL_ANALOG      0x00170F00    // A0 A1 A2 A4 C0 C1 C2 C3  

// Optional Second parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE:
#define VSS_VDD         0x00000000    //| Range 0-Vdd
#define VSS_VREF        0x02000000    //| Range 0-VrefH
#define VSS_FVR         0x03000000    //| Range 0-FVR  

// Constants used in SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define TEMPERATURE_INDICATOR     0x1D
#define DAC_CHANNEL               0x1E 
#define FVR_CHANNEL               0x1F 

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
#define INT_EXT_L2H               0x50000B10
#define INT_EXT_H2L               0x60000B10
#define INT_EXT                   0x300B10
#define INT_TIMER0                0x300B20
#define INT_TIMER1                0x309001
#define INT_TIMER2                0x309002
#define INT_CCP1                  0x309004
#define INT_SSP                   0x309008
#define INT_TBE                   0x309010
#define INT_RDA                   0x309020
#define INT_AD                    0x309040
#define INT_TIMER1_GATE            0x309080
#define INT_CCP2                  0x309101
#define INT_TIMER4                0x309102
#define INT_TIMER6                0x309104
#define INT_BUSCOL                0x309108
#define INT_COMP                  0x309120
#define INT_COMP2                 0x309140
#define INT_CLC                   0x309201
#define INT_CLC2                  0x309202
#define INT_ZCD                   0x309210
#define INT_CWG                   0x309220
#define INT_SMT1                  0x309301
#define INT_SMT1PRA               0x309302
#define INT_SMT1PWA               0x309304
#define INT_SMT2                  0x309308
#define INT_SMT2PRA               0x309310
#define INT_SMT2PWA               0x309320
#define INT_CRC                   0x309340
#define INT_SCANNER               0x309380
#define INT_PID                   0x309401
#define INT_PIDE                  0x309402
#define INT_AT1                   0x309404
#define INT_TIMER5                0x309410
#define INT_TIMER5_GATE            0x309420
#define INT_TIMER3                0x309440
#define INT_TIMER3_GATE            0x309480
#define INT_OSC_FAIL              0x309180
#define INT_CLC3                  0x309204
#define INT_CLC4                  0x309208
#define INT_IOC                   0x3FFF0B08
#define INT_IOC_A0                0x30010B08
#define INT_IOC_A0_L2H            0x10010B08
#define INT_IOC_A0_H2L            0x20010B08
#define INT_IOC_A1                0x30020B08
#define INT_IOC_A1_L2H            0x10020B08
#define INT_IOC_A1_H2L            0x20020B08
#define INT_IOC_A2                0x30040B08
#define INT_IOC_A2_L2H            0x10040B08
#define INT_IOC_A2_H2L            0x20040B08
#define INT_IOC_A3                0x30080B08
#define INT_IOC_A3_L2H            0x10080B08
#define INT_IOC_A3_H2L            0x20080B08
#define INT_IOC_A4                0x30100B08
#define INT_IOC_A4_L2H            0x10100B08
#define INT_IOC_A4_H2L            0x20100B08
#define INT_IOC_A5                0x30200B08
#define INT_IOC_A5_L2H            0x10200B08
#define INT_IOC_A5_H2L            0x20200B08
#define INT_IOC_C0                0x32010B08
#define INT_IOC_C0_L2H            0x12010B08
#define INT_IOC_C0_H2L            0x22010B08
#define INT_IOC_C1                0x32020B08
#define INT_IOC_C1_L2H            0x12020B08
#define INT_IOC_C1_H2L            0x22020B08
#define INT_IOC_C2                0x32040B08
#define INT_IOC_C2_L2H            0x12040B08
#define INT_IOC_C2_H2L            0x22040B08
#define INT_IOC_C3                0x32080B08
#define INT_IOC_C3_L2H            0x12080B08
#define INT_IOC_C3_H2L            0x22080B08
#define INT_IOC_C4                0x32100B08
#define INT_IOC_C4_L2H            0x12100B08
#define INT_IOC_C4_H2L            0x22100B08
#define INT_IOC_C5                0x32200B08
#define INT_IOC_C5_L2H            0x12200B08
#define INT_IOC_C5_H2L            0x22200B08

//The following defines are provided for compatabilty with older compiler versions
//INT_IOC define should be used to create Interrupt on Change ISR, and the INT_IOC_xx,
//INT_IOC_xx_L2H and INT_IOC_xx_H2L defines should be used in the ENABLE_INTERRUPTS(),
//DISABLE_INTERRUPTS(), CLEAR_INTERRUPT(), INTERRUPT_ACTIVE() and INTERRUPT_ACTIVE()
//function calls.
#define INT_RA                    0x30FF0B08
#define INT_RA0                   0x30010B08
#define INT_RA0_L2H               0x10010B08
#define INT_RA0_H2L               0x20010B08
#define INT_RA1                   0x30020B08
#define INT_RA1_L2H               0x10020B08
#define INT_RA1_H2L               0x20020B08
#define INT_RA2                   0x30040B08
#define INT_RA2_L2H               0x10040B08
#define INT_RA2_H2L               0x20040B08
#define INT_RA3                   0x30080B08
#define INT_RA3_L2H               0x10080B08
#define INT_RA3_H2L               0x20080B08
#define INT_RA4                   0x30100B08
#define INT_RA4_L2H               0x10100B08
#define INT_RA4_H2L               0x20100B08
#define INT_RA5                   0x30200B08
#define INT_RA5_L2H               0x10200B08
#define INT_RA5_H2L               0x20200B08
#define INT_RC                    0x30FF0B08
#define INT_RC0                   0x32010B08
#define INT_RC0_L2H               0x12010B08
#define INT_RC0_H2L               0x22010B08
#define INT_RC1                   0x32020B08
#define INT_RC1_L2H               0x12020B08
#define INT_RC1_H2L               0x22020B08
#define INT_RC2                   0x32040B08
#define INT_RC2_L2H               0x12040B08
#define INT_RC2_H2L               0x22040B08
#define INT_RC3                   0x32080B08
#define INT_RC3_L2H               0x12080B08
#define INT_RC3_H2L               0x22080B08
#define INT_RC4                   0x32100B08
#define INT_RC4_L2H               0x12100B08
#define INT_RC4_H2L               0x22100B08
#define INT_RC5                   0x32200B08
#define INT_RC5_L2H               0x12200B08
#define INT_RC5_H2L               0x22200B08

#list
