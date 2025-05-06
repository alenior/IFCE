//////////// Standard Header file for the PIC16LF1776 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16LF1776

#nolist
//////// Program memory: 8192x14  Data RAM: 1008  Stack: 16
//////// I/O: 25   Analog Pins: 17
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: LP,XT,HS,RC,INTRC_IO,ECL,ECM,ECH,NOWDT,WDT_SW,WDT_NOSL,WDT
//////// Fuses: PUT,NOPUT,MCLR,NOMCLR,PROTECT,NOPROTECT,NOBROWNOUT
//////// Fuses: BROWNOUT_SW,BROWNOUT_NOSL,BROWNOUT,CLKOUT,NOCLKOUT,IESO
//////// Fuses: NOIESO,FCMEN,NOFCMEN,WRT,WRT_1000,WRT_200,NOWRT,PPS1WAY
//////// Fuses: NOPPS1WAY,ZCDDIS,NOZCDDIS,PLLEN,NOPLLEN,STVREN,NOSTVREN
//////// Fuses: BORV25,BORV19,LPBOR,NOLPBOR,DEBUG,NODEBUG,LVP,NOLVP
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
//    PIN_C4,PIN_C5,PIN_C6,PIN_C7,PIN_E3
// Input Functions: 
//    INT0,T0CK,T1CK,T1G,T3CK,T3G,T5CK,T5G,T2CK,T4CK,T6CK,T8CK,CCP1,CCP2,CCP7,
//    COG1IN,COG2IN,COG3IN,MD1CL,MD1CH,MD1MOD,MD2CL,MD2CH,MD2MOD,MD3CL,MD3CH,
//    MD3MOD,PRG1R,PRG1F,PRG2R,PRG2F,PRG3R,PRG3F,CLCIN0,CLCIN1,CLCIN2,CLCIN3,
//    ADCACT,SCK1IN,SCL1IN,SDI1,SDA1IN,SS1IN,U1RX,U1CKIN,T0CKI,T1CKI,T3CKI,T5CKI,
//    T2CKI,T4CKI,T6CKI,T8CKI,CCP1IN,CCP2IN,CCP7IN,RX1
// Output Functions: 
//    NULL,CLC1OUT,CLC2OUT,CLC3OUT,CLC4OUT,COG1A,COG1B,COG1C,COG1D,COG2A,COG2B,
//    COG2C,COG2D,COG3A,COG3B,COG3C,COG3D,CCP1OUT,CCP2OUT,CCP7OUT,PWM3OUT,
//    PWM4OUT,PWM9OUT,PWM5OUT,PWM6OUT,PWM11OUT,SCK1OUT,SCL1OUT,SDA1OUT,SDO1,U1TX,
//    U1DT,C1OUT,C2OUT,C3OUT,C4OUT,C5OUT,C6OUT,MD1OUT,MD2OUT,MD3OUT,PWM3,PWM4,
//    PWM9,PWM5,PWM6,PWM11,SCK1,SCL1,SDA1,TX1,DT1
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
_bif void set_tris_e(int8 value);  
_bif void set_input_level_a(int8 value);  
_bif void set_input_level_b(int8 value);  
_bif void set_input_level_c(int8 value);  
_bif void set_open_drain_a(int8 value);  
_bif void set_open_drain_b(int8 value);  
_bif void set_open_drain_c(int8 value);  
_bif int8 get_tris_a(void);  
_bif int8 get_tris_b(void);  
_bif int8 get_tris_c(void);  
_bif int8 get_tris_e(void);  
_bif void output_a(int8 value);  
_bif void output_b(int8 value);  
_bif void output_c(int8 value);  
_bif void output_e(int8 value);  
_bif int8 input_a(void);  
_bif int8 input_b(void);  
_bif int8 input_c(void);  
_bif int8 input_e(void);  
_bif int8 input_change_a(void);  
_bif int8 input_change_b(void);  
_bif int8 input_change_c(void);  
_bif int8 input_change_e(void);  
_bif void port_a_pullups(int8 upmask);   
_bif void port_b_pullups(int8 upmask);  
_bif void port_c_pullups(int8 upmask);   
_bif void port_e_pullups(int8 upmask);   
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

#define PIN_E3  131

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
#define T2_CLK_T2IN                                0  
#define T2_CLK_INTERNAL                            0x0100 //FOSC/4  
#define T2_CLK_FOSC                                0x0200  
#define T2_CLK_HFINTRC                             0x0300  
#define T2_CLK_LFINTRC                             0x0400  
#define T2_CLK_MFINTRC                             0x0500      
#define T2_CLK_SOSC                                0x0600 
#define T2_CLK_ZCD                                 0x0700 
#define T2_CLK_CLC1                                0x0800 
#define T2_CLK_CLC2                                0x0900 
#define T2_CLK_CLC3                                0x0A00 
#define T2_CLK_CLC4                                0x0B00 

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
#define T2_RESET_FROM_TMR4                         0x02000000 
#define T2_RESET_FROM_TMR6                         0x03000000 
#define T2_RESET_FROM_TMR8                         0x04000000 
#define T2_RESET_FROM_CCP1                         0x05000000 
#define T2_RESET_FROM_CCP2                         0x06000000 
#define T2_RESET_FROM_CCP7                         0x07000000 
#define T2_RESET_FROM_PWM3                         0x09000000 
#define T2_RESET_FROM_PWM4                         0x0A000000 
#define T2_RESET_FROM_PWM9                         0x0B000000 
#define T2_RESET_FROM_PWM5                         0x0D000000 
#define T2_RESET_FROM_PWM6                         0x0E000000 
#define T2_RESET_FROM_PWM11                        0x0F000000 
#define T2_RESET_FROM_C1OUT                        0x11000000 
#define T2_RESET_FROM_C2OUT                        0x12000000 
#define T2_RESET_FROM_C3OUT                        0x13000000 
#define T2_RESET_FROM_C4OUT                        0x14000000 
#define T2_RESET_FROM_C5OUT                        0x15000000 
#define T2_RESET_FROM_C6OUT                        0x16000000 
#define T2_RESET_FROM_ZCD                          0x19000000 
#define T2_RESET_FROM_CLC1                         0x1A000000 
#define T2_RESET_FROM_CLC2                         0x1B000000 
#define T2_RESET_FROM_CLC3                         0x1C000000 
#define T2_RESET_FROM_CLC4                         0x1D000000 

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
#define T3_FOSC             0x47  
#define T3_LFINTRC          0xC7  


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
#define T4_CLK_MFINTRC                             0x0500 
#define T4_CLK_SOSC                                0x0600 
#define T4_CLK_ZCD                                 0x0700 
#define T4_CLK_CLC1                                0x0800 
#define T4_CLK_CLC2                                0x0900 
#define T4_CLK_CLC3                                0x0A00 
#define T4_CLK_CLC4                                0x0B00 

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
#define T4_RESET_FROM_TMR2                         0x01000000 
#define T4_RESET_FROM_TMR6                         0x03000000 
#define T4_RESET_FROM_TMR8                         0x04000000 
#define T4_RESET_FROM_CCP1                         0x05000000 
#define T4_RESET_FROM_CCP2                         0x06000000 
#define T4_RESET_FROM_CCP7                         0x07000000 
#define T4_RESET_FROM_PWM3                         0x09000000 
#define T4_RESET_FROM_PWM4                         0x0A000000 
#define T4_RESET_FROM_PWM9                         0x0B000000 
#define T4_RESET_FROM_PWM5                         0x0D000000 
#define T4_RESET_FROM_PWM6                         0x0E000000 
#define T4_RESET_FROM_PWM11                        0x0F000000 
#define T4_RESET_FROM_C1OUT                        0x11000000 
#define T4_RESET_FROM_C2OUT                        0x12000000 
#define T4_RESET_FROM_C3OUT                        0x13000000 
#define T4_RESET_FROM_C4OUT                        0x14000000 
#define T4_RESET_FROM_C5OUT                        0x15000000 
#define T4_RESET_FROM_C6OUT                        0x16000000 
#define T4_RESET_FROM_ZCD                          0x19000000 
#define T4_RESET_FROM_CLC1                         0x1A000000 
#define T4_RESET_FROM_CLC2                         0x1B000000 
#define T4_RESET_FROM_CLC3                         0x1C000000 
#define T4_RESET_FROM_CLC4                         0x1D000000 

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
#define T5_FOSC             0x47  
#define T5_LFINTRC          0xC7  


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
#define T6_CLK_MFINTRC                             0x0500 
#define T6_CLK_SOSC                                0x0600 
#define T6_CLK_ZCD                                 0x0700 
#define T6_CLK_CLC1                                0x0800 
#define T6_CLK_CLC2                                0x0900 
#define T6_CLK_CLC3                                0x0A00 
#define T6_CLK_CLC4                                0x0B00 

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
#define T6_RESET_FROM_TMR2                         0x01000000 
#define T6_RESET_FROM_TMR4                         0x02000000 
#define T6_RESET_FROM_TMR8                         0x04000000 
#define T6_RESET_FROM_CCP1                         0x05000000 
#define T6_RESET_FROM_CCP2                         0x06000000 
#define T6_RESET_FROM_CCP7                         0x07000000 
#define T6_RESET_FROM_PWM3                         0x09000000 
#define T6_RESET_FROM_PWM4                         0x0A000000 
#define T6_RESET_FROM_PWM9                         0x0B000000 
#define T6_RESET_FROM_PWM5                         0x0D000000 
#define T6_RESET_FROM_PWM6                         0x0E000000 
#define T6_RESET_FROM_PWM11                        0x0F000000 
#define T6_RESET_FROM_C1OUT                        0x11000000 
#define T6_RESET_FROM_C2OUT                        0x12000000 
#define T6_RESET_FROM_C3OUT                        0x13000000 
#define T6_RESET_FROM_C4OUT                        0x14000000 
#define T6_RESET_FROM_C5OUT                        0x15000000 
#define T6_RESET_FROM_C6OUT                        0x16000000 
#define T6_RESET_FROM_ZCD                          0x19000000 
#define T6_RESET_FROM_CLC1                         0x1A000000 
#define T6_RESET_FROM_CLC2                         0x1B000000 
#define T6_RESET_FROM_CLC3                         0x1C000000 
#define T6_RESET_FROM_CLC4                         0x1D000000 

////////////////////////////////////////////////////////////////// Timer 6
// Timer 8 Functions: SETUP_TIMER_8, GET_TIMER8, SET_TIMER8
// Timer 8 Prototypes:
_bif void setup_timer_8(int32 mode, int8 period, int8 postscaler);
_bif int8 get_timer8(void);
_bif void set_timer8(int8 value);
//Constants used for SETUP_TIMER_8() are:
#define T8_DISABLED                                0
#define T8_DIV_BY_1                                0x80
#define T8_DIV_BY_2                                0x90
#define T8_DIV_BY_4                                0xA0
#define T8_DIV_BY_8                                0xB0
#define T8_DIV_BY_16                               0xC0
#define T8_DIV_BY_32                               0xD0
#define T8_DIV_BY_64                               0xE0
#define T8_DIV_BY_128                              0xF0
//One of the following may be OR'ed with the above
#define T8_CLK_T8IN                                0
#define T8_CLK_INTERNAL                            0x0100 //FOSC/4
#define T8_CLK_FOSC                                0x0200
#define T8_CLK_HFINTRC                             0x0300
#define T8_CLK_LFINTRC                             0x0400
#define T8_CLK_MFINTRC                             0x0500
#define T8_CLK_SOSC                                0x0600
#define T8_CLK_ZCD                                 0x0700 
#define T8_CLK_CLC1                                0x0800 
#define T8_CLK_CLC2                                0x0900 
#define T8_CLK_CLC3                                0x0A00 
#define T8_CLK_CLC4                                0x0B00 

//One of the following may be OR'ed with the above for setting up the Hardware Limit Timer
#define T8_START_IMMEDIATELY                       0
#define T8_START_WHEN_HIGH                         0x010000
#define T8_START_WHEN_LOW                          0x020000
#define T8_RESET_ON_BOTH_EDGES                     0x030000
#define T8_RESET_ON_RE                             0x040000
#define T8_RESET_ON_FE                             0x050000
#define T8_RESET_WHEN_LOW                          0x060000
#define T8_RESET_WHEN_HIGH                         0x070000
#define T8_ONE_SHOT_START_IMMEDIATELY              0x080000
#define T8_ONE_SHOT_START_ON_RE                    0x090000
#define T8_ONE_SHOT_START_ON_FE                    0x0A0000
#define T8_ONE_SHOT_START_ON_BOTH_EDGES            0x0B0000
#define T8_ONE_SHOT_START_ON_RE_RESET_ON_RE        0x0C0000 //starts on first rising edge, resets on subsequent rising edges
#define T8_ONE_SHOT_START_ON_FE_RESET_ON_FE        0x0D0000 //starts on first falling edge, resets on subsequent falling edges
#define T8_ONE_SHOT_START_ON_RE_RESET_WHEN_LOW     0x0E0000
#define T8_ONE_SHOT_START_ON_FE_RESET_WHEN_HIGH    0x0F0000
#define T8_MONO_STABLE_START_ON_RE                 0x110000
#define T8_MONO_STABLE_START_ON_FE                 0x120000
#define T8_MONO_STABLE_START_ON_EE                 0x130000
#define T8_ONE_SHOT_START_WHEN_HIGH_RESET_WHEN_LOW 0x160000
#define T8_ONE_SHOT_START_WHEN_LOW_RESET_WHEN_HIGH 0x170000

//Any of the following may be OR'ed with the above
#define T8_SYNC_ON_WITH_T2CLK                      0x200000
#define T8_INVERTED                                0x400000
#define T8_SYNC_PRESCALER_WITH_CLK                 0x800000
//One of the following may be OR'ed with the above to select the Hardware Limit Timer
//External Reset Signal Source
#define T8_RESET_FROM_T8IN                         0
#define T8_RESET_FROM_TMR2                         0x01000000
#define T8_RESET_FROM_TMR4                         0x02000000
#define T8_RESET_FROM_TMR6                         0x03000000
#define T8_RESET_FROM_CCP1                         0x05000000
#define T8_RESET_FROM_CCP2                         0x06000000
#define T8_RESET_FROM_CCP7                         0x07000000 
#define T8_RESET_FROM_PWM3                         0x09000000 
#define T8_RESET_FROM_PWM4                         0x0A000000 
#define T8_RESET_FROM_PWM9                         0x0B000000 
#define T8_RESET_FROM_PWM5                         0x0D000000 
#define T8_RESET_FROM_PWM6                         0x0E000000 
#define T8_RESET_FROM_PWM11                        0x0F000000 
#define T8_RESET_FROM_C1OUT                        0x11000000 
#define T8_RESET_FROM_C2OUT                        0x12000000 
#define T8_RESET_FROM_C3OUT                        0x13000000 
#define T8_RESET_FROM_C4OUT                        0x14000000 
#define T8_RESET_FROM_C5OUT                        0x15000000 
#define T8_RESET_FROM_C6OUT                        0x16000000 
#define T8_RESET_FROM_ZCD                          0x19000000 
#define T8_RESET_FROM_CLC1                         0x1A000000 
#define T8_RESET_FROM_CLC2                         0x1B000000 
#define T8_RESET_FROM_CLC3                         0x1C000000 
#define T8_RESET_FROM_CLC4                         0x1D000000 

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
#define CCP_COMPARE_RESET_TIMER          0x0B            
#define CCP_COMPARE_PULSE_RESET_TIMER    0x0B 
#define CCP_PWM                          0x0C    
// One of the following may be OR'ed in with the above when using Capture mode 
#define CCP_CAPTRUE_INPUT_CCP_PIN        0 
#define CCP_CAPTURE_INPUT_C1OUT          0x010000 
#define CCP_CAPTURE_INPUT_C2OUT          0x020000 
#define CCP_CAPTURE_INPUT_C3OUT          0x030000 
#define CCP_CAPTURE_INPUT_C4OUT          0x040000 
#define CCP_CAPTURE_INPUT_C5OUT          0x050000 
#define CCP_CAPTURE_INPUT_C6OUT          0x060000 
#define CCP_CAPTURE_INPUT_CLC1           0x090000 
#define CCP_CAPTURE_INPUT_CLC2           0x0A0000 
#define CCP_CAPTURE_INPUT_CLC3           0x0B0000 
#define CCP_CAPTURE_INPUT_CLC4           0x0C0000 
#define CCP_CAPTURE_INPUT_IOC_INTERRUPT  0x0D0000 
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

// CCP7 Prototypes:
_bif void setup_ccp7(int32 mode);
_bif void setup_ccp7(int32 mode, int8 pwm);
_bif void set_pwm7_duty(int8 value);
_bif void set_pwm7_duty(int16 value);
#word   CCP_7       =                    getenv("SFR:CCPR7L")
#byte   CCP_7_LOW   =                    getenv("SFR:CCPR7L")
#byte   CCP_7_HIGH  =                    getenv("SFR:CCPR7H")


#define CCP_TIMER2            0x0000  
#define CCP_TIMER4            0x1000  
#define CCP_TIMER6            0x2000  
#define CCP_TIMER8            0x3000  

////////////////////////////////////////////////////////////////// PWM
// PWM Functions: SETUP_PWMx, SET_PWMx_DUTY, SET_PWMx_PERIOD,
//                SET_PWMx_PHASE, SET_PWMx_OFFSET
// PWM Prototypes:
_bif void setup_pwm3(int32 mode);
_bif void set_pwm3_duty(int8 value);
_bif void set_pwm3_duty(int16 value);
_bif void setup_pwm4(int32 mode); 
_bif void set_pwm4_duty(int8 value); 
_bif void set_pwm4_duty(int16 value); 
_bif void setup_pwm5(int16 mode);
_bif void setup_pwm5(int16 mode, int32 mode1);
_bif void set_pwm5_duty(int16 duty);
_bif void set_pwm5_period(int16 period);
_bif void set_pwm5_phase(int16 phase);
_bif void set_pwm5_offset(int16 offset);
_bif void enable_pwm5_interrrupt(int8 interrupt);
_bif void disable_pwm5_interrupt(int8 interrupt);
_bif void clear_pwm5_interrupt(int8 interrupt);
_bif int1 pwm5_interrupt_active(int8 interrupt);
_bif void setup_pwm6(int16 mode); 
_bif void setup_pwm6(int16 mode, int32 mode1); 
_bif void set_pwm6_duty(int16 duty); 
_bif void set_pwm6_period(int16 period); 
_bif void set_pwm6_phase(int16 phase); 
_bif void set_pwm6_offset(int16 offset); 
_bif void enable_pwm6_interrrupt(int8 interrupt); 
_bif void disable_pwm6_interrupt(int8 interrupt); 
_bif void clear_pwm6_interrupt(int8 interrupt); 
_bif int1 pwm6_interrupt_active(int8 interrupt); 
_bif void setup_pwm9(int32 mode); 
_bif void set_pwm9_duty(int8 value); 
_bif void set_pwm9_duty(int16 value); 
_bif void setup_pwm11(int16 mode); 
_bif void setup_pwm11(int16 mode, int32 mode1); 
_bif void set_pwm11_duty(int16 duty); 
_bif void set_pwm11_period(int16 period); 
_bif void set_pwm11_phase(int16 phase); 
_bif void set_pwm11_offset(int16 offset); 
_bif void enable_pwm11_interrrupt(int8 interrupt); 
_bif void disable_pwm11_interrupt(int8 interrupt); 
_bif void clear_pwm11_interrupt(int8 interrupt); 
_bif int1 pwm11_interrupt_active(int8 interrupt); 
// Constants used for SETUP_PWM3(), SETUP_PWM4() and SETUP_PWM9() are:  
#define PWM_ENABLED      0x80
#define PWM_DISABLED     0x10000
// One of the following may be OR'ed in with the above
#define PWM_ACTIVE_LOW   0x10
#define PWM_ACTIVE_HIGH  0
// One of the following may be OR'ed in with the above
#define PWM_TIMER2       0x0000
#define PWM_TIMER4       0x0100
#define PWM_TIMER6       0x0200
#define PWM_TIMER8       0x0300 

//Constants used for SETUP_PWM5(), SETUP_PWM6() and SETUP_PWM11() first parameter are:  
// One of the following may be used
#define PWM_CENTER_ALIGN                   0x8C
#define PWM_TOGGLE_ON_MATCH                0x88
#define PWM_SET_ON_MATCH                   0x84
#define PWM_STANDARD                       0x80
// One of the following may be OR'ed in with the above
#define PWM_CLK_FOSC                       0
#define PWM_CLK_HFINTOSC                   0x0100
#define PWM_CLK_LFINTOSC                   0x0200
// One of the following may be OR'ed in with the above
#define PWM_CLK_DIV_BY_1                   0
#define PWM_CLK_DIV_BY_2                   0x1000
#define PWM_CLK_DIV_BY_4                   0x2000
#define PWM_CLK_DIV_BY_8                   0x3000
#define PWM_CLK_DIV_BY_16                  0x4000
#define PWM_CLK_DIV_BY_32                  0x5000
#define PWM_CLK_DIV_BY_64                  0x6000
#define PWM_CLK_DIV_BY_128                 0x7000
// The following may be OR'ed in with the above
#define PWM_INVERT                         0x10

// Constants used for SETUP_PWM5(), SETUP_PWM6() and SETUP_PWM11() second parameter are:  
#define PWM_OFFSET_INT_ENABLED             0x08
#define PWM_PHASE_INT_ENABLED              0x04
#define PWM_DUTY_INT_ENABLED               0x02
#define PWM_PERIOD_INT_ENABLED             0x01
#define PWM_LOAD_BUFFERS_ON_TRIGGER        0x4000 
// One of the following may be OR'ed in with the above 
#define PWM_LOAD_TRIGGER_LD5               0 
#define PWM_LOAD_TRIGGER_LD6               0x0100 
#define PWM_LOAD_TRIGGER_LD11              0x0200 
// One of the following may be OR'ed in with the above 
#define PWM_OFFSET_MODE_CONTINUOUS         0x600000 
#define PWM_OFFSET_MODE_ONE_SHOT           0x400000 
#define PWM_OFFSET_MODE_INDEPENDENT_SLAVE  0x200000 
#define PWM_OFFSET_MODE_INDEPENDENT        0 
// One of the following may be OR'ed in with the above 
#define PWM_OFFSET_MATCH_INCREMENTING      0x100000 
#define PWM_OFFSET_MATCH_DECREMENTING      0 
// One of the following may be OR'ed in with the above 
#define PWM_OFFSET_TRIGGER_OF11            0x020000 
#define PWM_OFFSET_TRIGGER_OF6             0x010000 
#define PWM_OFFSET_TRIGGER_OF5             0 

//Constants used for ENABLE_PWM5_INTERRUPT(), DISABLE_PWM5_INTERRUPT(), CLEAR_PWM5_INTERRUPT(), PWM5_INTERRUPT_ACTIVE(), 
//ENABLE_PWM6_INTERRUPT(), DISABLE_PWM6_INTERRUPT(), CLEAR_PWM6_INTERRUPT(), PWM6_INTERRUPT_ACTIVE(), ENABLE_PWM11_INTERRUPT(), 
//DISABLE_PWM11_INTERRUPT(), CLEAR_PWM11_INTERRUPT(), and PWM11_INTERRUPT_ACTIVE() are:  
#define PWM_OFFSET_INTERRUPT               0x08
#define PWM_PHASE_INTERRUPT                0x04
#define PWM_DUTY_INTERRUPT                 0x02
#define PWM_PERIOD_INTERRUPT               0x01

#bit PWM3OUT=getenv("BIT:PWM3OUT")
#bit PWM4OUT=getenv("BIT:PWM4OUT") 
#bit PWM5OUT=getenv("BIT:PWM5OUT")
#bit PWM6OUT=getenv("BIT:PWM6OUT") 
#bit PWM9OUT=getenv("BIT:PWM9OUT") 
#bit PWM11OUT=getenv("BIT:PWM11OUT") 

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
// Comparator Variables: C1OUT, C2OUT, C3OUT, C4OUT, C5OUT, C6OUT,
// Comparator Prototypes:
_bif void setup_comparator_1(int16 mode);
_bif void setup_comparator_2(int16 mode);
_bif void setup_comparator_3(int16 mode);
_bif void setup_comparator_4(int16 mode);
_bif void setup_comparator_5(int16 mode);
_bif void setup_comparator_6(int16 mode);
// Constants used in setup_comparator_x() are:
//
#define NC_NC           0x0098

// Constants used in setup_comparator_1() are:
// Pick one of the following
#define CP1_A0_A2       0x8000
#define CP1_A1_A2       0x8001
#define CP1_B3_A2       0x8002
#define CP1_B1_A2       0x8003
#define CP1_C3_A2       0x8004
#define CP1_FVR2_A2     0x8005
#define CP1_PRG1_A2     0x8006
#define CP1_PRG2_A2     0x8007
#define CP1_VSS_A2      0x8008
#define CP1_OPA1_A2     0x8009
#define CP1_OPA2_A2     0x800A
#define CP1_A0_A3       0x8010
#define CP1_A1_A3       0x8011
#define CP1_B3_A3       0x8012
#define CP1_B1_A3       0x8013
#define CP1_C3_A3       0x8014
#define CP1_FVR2_A3     0x8015
#define CP1_PRG1_A3     0x8016
#define CP1_PRG2_A3     0x8017
#define CP1_VSS_A3      0x8018
#define CP1_OPA1_A3     0x8019
#define CP1_OPA2_A3     0x801A
#define CP1_A0_FVR2     0x8020
#define CP1_A1_FVR2     0x8021
#define CP1_B3_FVR2     0x8022
#define CP1_B1_FVR2     0x8023
#define CP1_C3_FVR2     0x8024
#define CP1_FVR2_FVR2   0x8025
#define CP1_PRG1_FVR2   0x8026
#define CP1_PRG2_FVR2   0x8027
#define CP1_VSS_FVR2    0x8028
#define CP1_OPA1_FVR2   0x8029
#define CP1_OPA2_FVR2   0x802A
#define CP1_A0_PRG1     0x8030
#define CP1_A1_PRG1     0x8031
#define CP1_B3_PRG1     0x8032
#define CP1_B1_PRG1     0x8033
#define CP1_C3_PRG1     0x8034
#define CP1_FVR2_PRG1   0x8035
#define CP1_PRG1_PRG1   0x8036
#define CP1_PRG2_PRG1   0x8037
#define CP1_VSS_PRG1    0x8038
#define CP1_OPA1_PRG1   0x8039
#define CP1_OPA2_PRG1   0x803A
#define CP1_A0_PRG2     0x8040
#define CP1_A1_PRG2     0x8041
#define CP1_B3_PRG2     0x8042
#define CP1_B1_PRG2     0x8043
#define CP1_C3_PRG2     0x8044
#define CP1_FVR2_PRG2   0x8045
#define CP1_PRG1_PRG2   0x8046
#define CP1_PRG2_PRG2   0x8047
#define CP1_VSS_PRG2    0x8048
#define CP1_OPA1_PRG2   0x8049
#define CP1_OPA2_PRG2   0x804A
#define CP1_A0_DAC1     0x8050
#define CP1_A1_DAC1     0x8051
#define CP1_B3_DAC1     0x8052
#define CP1_B1_DAC1     0x8053
#define CP1_C3_DAC1     0x8054
#define CP1_FVR2_DAC1   0x8055
#define CP1_PRG1_DAC1   0x8056
#define CP1_PRG2_DAC1   0x8057
#define CP1_VSS_DAC1    0x8058
#define CP1_OPA1_DAC1   0x8059
#define CP1_OPA2_DAC1   0x805A
#define CP1_A0_DAC2     0x8060
#define CP1_A1_DAC2     0x8061
#define CP1_B3_DAC2     0x8062
#define CP1_B1_DAC2     0x8063
#define CP1_C3_DAC2     0x8064
#define CP1_FVR2_DAC2   0x8065
#define CP1_PRG1_DAC2   0x8066
#define CP1_PRG2_DAC2   0x8067
#define CP1_VSS_DAC2    0x8068
#define CP1_OPA1_DAC2   0x8069
#define CP1_OPA2_DAC2   0x806A
#define CP1_A0_DAC3     0x8070
#define CP1_A1_DAC3     0x8071
#define CP1_B3_DAC3     0x8072
#define CP1_B1_DAC3     0x8073
#define CP1_C3_DAC3     0x8074
#define CP1_FVR2_DAC3   0x8075
#define CP1_PRG1_DAC3   0x8076
#define CP1_PRG2_DAC3   0x8077
#define CP1_VSS_DAC3    0x8078
#define CP1_OPA1_DAC3   0x8079
#define CP1_OPA2_DAC3   0x807A
#define CP1_A0_DAC4     0x8080
#define CP1_A1_DAC4     0x8081
#define CP1_B3_DAC4     0x8082
#define CP1_B1_DAC4     0x8083
#define CP1_C3_DAC4     0x8084
#define CP1_FVR2_DAC4   0x8085
#define CP1_PRG1_DAC4   0x8086
#define CP1_PRG2_DAC4   0x8087
#define CP1_VSS_DAC4    0x8088
#define CP1_OPA1_DAC4   0x8089
#define CP1_OPA2_DAC4   0x808A
#define CP1_A0_VSS      0x8090
#define CP1_A1_VSS      0x8091
#define CP1_B3_VSS      0x8092
#define CP1_B1_VSS      0x8093
#define CP1_C3_VSS      0x8094
#define CP1_FVR2_VSS    0x8095
#define CP1_PRG1_VSS    0x8096
#define CP1_PRG2_VSS    0x8097
#define CP1_VSS_VSS     0x8098
#define CP1_OPA1_VSS    0x8099
#define CP1_OPA2_VSS    0x809A
//Optionally OR any of the following with the above
#define CP1_INT_L2H     0x20000
#define CP1_INT_H2L     0x10000
#define CP1_LOW_POWER   0x2000
#define CP1_INVERT      0x1000
#define CP1_FILTER      0x0800
#define CP1_HYST        0x0200
#define CP1_SYNC        0x0100

// Constants used in setup_comparator_2() are:
// Pick one of the following
#define CP2_A0_A2       0x8000
#define CP2_A1_A2       0x8001
#define CP2_B3_A2       0x8002
#define CP2_B1_A2       0x8003
#define CP2_C3_A2       0x8004
#define CP2_FVR2_A2     0x8005
#define CP2_PRG1_A2     0x8006
#define CP2_PRG2_A2     0x8007
#define CP2_VSS_A2      0x8008
#define CP2_OPA1_A2     0x8009
#define CP2_OPA2_A2     0x800A
#define CP2_A0_B0       0x8010
#define CP2_A1_B0       0x8011
#define CP2_B3_B0       0x8012
#define CP2_B1_B0       0x8013
#define CP2_C3_B0       0x8014
#define CP2_FVR2_B0     0x8015
#define CP2_PRG1_B0     0x8016
#define CP2_PRG2_B0     0x8017
#define CP2_VSS_B0      0x8018
#define CP2_OPA1_B0     0x8019
#define CP2_OPA2_B0     0x801A
#define CP2_A0_FVR2     0x8020
#define CP2_A1_FVR2     0x8021
#define CP2_B3_FVR2     0x8022
#define CP2_B1_FVR2     0x8023
#define CP2_C3_FVR2     0x8024
#define CP2_FVR2_FVR2   0x8025
#define CP2_PRG1_FVR2   0x8026
#define CP2_PRG2_FVR2   0x8027
#define CP2_VSS_FVR2    0x8028
#define CP2_OPA1_FVR2   0x8029
#define CP2_OPA2_FVR2   0x802A
#define CP2_A0_PRG1     0x8030
#define CP2_A1_PRG1     0x8031
#define CP2_B3_PRG1     0x8032
#define CP2_B1_PRG1     0x8033
#define CP2_C3_PRG1     0x8034
#define CP2_FVR2_PRG1   0x8035
#define CP2_PRG1_PRG1   0x8036
#define CP2_PRG2_PRG1   0x8037
#define CP2_VSS_PRG1    0x8038
#define CP2_OPA1_PRG1   0x8039
#define CP2_OPA2_PRG1   0x803A
#define CP2_A0_PRG2     0x8040
#define CP2_A1_PRG2     0x8041
#define CP2_B3_PRG2     0x8042
#define CP2_B1_PRG2     0x8043
#define CP2_C3_PRG2     0x8044
#define CP2_FVR2_PRG2   0x8045
#define CP2_PRG1_PRG2   0x8046
#define CP2_PRG2_PRG2   0x8047
#define CP2_VSS_PRG2    0x8048
#define CP2_OPA1_PRG2   0x8049
#define CP2_OPA2_PRG2   0x804A
#define CP2_A0_DAC1     0x8050
#define CP2_A1_DAC1     0x8051
#define CP2_B3_DAC1     0x8052
#define CP2_B1_DAC1     0x8053
#define CP2_C3_DAC1     0x8054
#define CP2_FVR2_DAC1   0x8055
#define CP2_PRG1_DAC1   0x8056
#define CP2_PRG2_DAC1   0x8057
#define CP2_VSS_DAC1    0x8058
#define CP2_OPA1_DAC1   0x8059
#define CP2_OPA2_DAC1   0x805A
#define CP2_A0_DAC2     0x8060
#define CP2_A1_DAC2     0x8061
#define CP2_B3_DAC2     0x8062
#define CP2_B1_DAC2     0x8063
#define CP2_C3_DAC2     0x8064
#define CP2_FVR2_DAC2   0x8065
#define CP2_PRG1_DAC2   0x8066
#define CP2_PRG2_DAC2   0x8067
#define CP2_VSS_DAC2    0x8068
#define CP2_OPA1_DAC2   0x8069
#define CP2_OPA2_DAC2   0x806A
#define CP2_A0_DAC3     0x8070
#define CP2_A1_DAC3     0x8071
#define CP2_B3_DAC3     0x8072
#define CP2_B1_DAC3     0x8073
#define CP2_C3_DAC3     0x8074
#define CP2_FVR2_DAC3   0x8075
#define CP2_PRG1_DAC3   0x8076
#define CP2_PRG2_DAC3   0x8077
#define CP2_VSS_DAC3    0x8078
#define CP2_OPA1_DAC3   0x8079
#define CP2_OPA2_DAC3   0x807A
#define CP2_A0_DAC4     0x8080
#define CP2_A1_DAC4     0x8081
#define CP2_B3_DAC4     0x8082
#define CP2_B1_DAC4     0x8083
#define CP2_C3_DAC4     0x8084
#define CP2_FVR2_DAC4   0x8085
#define CP2_PRG1_DAC4   0x8086
#define CP2_PRG2_DAC4   0x8087
#define CP2_VSS_DAC4    0x8088
#define CP2_OPA1_DAC4   0x8089
#define CP2_OPA2_DAC4   0x808A
#define CP2_A0_VSS      0x8090
#define CP2_A1_VSS      0x8091
#define CP2_B3_VSS      0x8092
#define CP2_B1_VSS      0x8093
#define CP2_C3_VSS      0x8094
#define CP2_FVR2_VSS    0x8095
#define CP2_PRG1_VSS    0x8096
#define CP2_PRG2_VSS    0x8097
#define CP2_VSS_VSS     0x8098
#define CP2_OPA1_VSS    0x8099
#define CP2_OPA2_VSS    0x809A
//Optionally OR any of the following with the above
#define CP2_INT_L2H     0x20000
#define CP2_INT_H2L     0x10000
#define CP2_LOW_POWER   0x2000
#define CP2_INVERT      0x1000
#define CP2_FILTER      0x0800
#define CP2_HYST        0x0200
#define CP2_SYNC        0x0100

// Constants used in setup_comparator_3() are:
// Pick one of the following
#define CP3_A0_A2       0x8000
#define CP3_A1_A2       0x8001
#define CP3_B3_A2       0x8002
#define CP3_B1_A2       0x8003
#define CP3_C3_A2       0x8004
#define CP3_FVR2_A2     0x8005
#define CP3_PRG1_A2     0x8006
#define CP3_PRG2_A2     0x8007
#define CP3_VSS_A2      0x8008
#define CP3_OPA1_A2     0x8009
#define CP3_OPA2_A2     0x800A
#define CP3_A0_B4       0x8010
#define CP3_A1_B4       0x8011
#define CP3_B3_B4       0x8012
#define CP3_B1_B4       0x8013
#define CP3_C3_B4       0x8014
#define CP3_FVR2_B4     0x8015
#define CP3_PRG1_B4     0x8016
#define CP3_PRG2_B4     0x8017
#define CP3_VSS_B4      0x8018
#define CP3_OPA1_B4     0x8019
#define CP3_OPA2_B4     0x801A
#define CP3_A0_FVR2     0x8020
#define CP3_A1_FVR2     0x8021
#define CP3_B3_FVR2     0x8022
#define CP3_B1_FVR2     0x8023
#define CP3_C3_FVR2     0x8024
#define CP3_FVR2_FVR2   0x8025
#define CP3_PRG1_FVR2   0x8026
#define CP3_PRG2_FVR2   0x8027
#define CP3_VSS_FVR2    0x8028
#define CP3_OPA1_FVR2   0x8029
#define CP3_OPA2_FVR2   0x802A
#define CP3_A0_PRG1     0x8030
#define CP3_A1_PRG1     0x8031
#define CP3_B3_PRG1     0x8032
#define CP3_B1_PRG1     0x8033
#define CP3_C3_PRG1     0x8034
#define CP3_FVR2_PRG1   0x8035
#define CP3_PRG1_PRG1   0x8036
#define CP3_PRG2_PRG1   0x8037
#define CP3_VSS_PRG1    0x8038
#define CP3_OPA1_PRG1   0x8039
#define CP3_OPA2_PRG1   0x803A
#define CP3_A0_PRG2     0x8040
#define CP3_A1_PRG2     0x8041
#define CP3_B3_PRG2     0x8042
#define CP3_B1_PRG2     0x8043
#define CP3_C3_PRG2     0x8044
#define CP3_FVR2_PRG2   0x8045
#define CP3_PRG1_PRG2   0x8046
#define CP3_PRG2_PRG2   0x8047
#define CP3_VSS_PRG2    0x8048
#define CP3_OPA1_PRG2   0x8049
#define CP3_OPA2_PRG2   0x804A
#define CP3_A0_DAC1     0x8050
#define CP3_A1_DAC1     0x8051
#define CP3_B3_DAC1     0x8052
#define CP3_B1_DAC1     0x8053
#define CP3_C3_DAC1     0x8054
#define CP3_FVR2_DAC1   0x8055
#define CP3_PRG1_DAC1   0x8056
#define CP3_PRG2_DAC1   0x8057
#define CP3_VSS_DAC1    0x8058
#define CP3_OPA1_DAC1   0x8059
#define CP3_OPA2_DAC1   0x805A
#define CP3_A0_DAC2     0x8060
#define CP3_A1_DAC2     0x8061
#define CP3_B3_DAC2     0x8062
#define CP3_B1_DAC2     0x8063
#define CP3_C3_DAC2     0x8064
#define CP3_FVR2_DAC2   0x8065
#define CP3_PRG1_DAC2   0x8066
#define CP3_PRG2_DAC2   0x8067
#define CP3_VSS_DAC2    0x8068
#define CP3_OPA1_DAC2   0x8069
#define CP3_OPA2_DAC2   0x806A
#define CP3_A0_DAC3     0x8070
#define CP3_A1_DAC3     0x8071
#define CP3_B3_DAC3     0x8072
#define CP3_B1_DAC3     0x8073
#define CP3_C3_DAC3     0x8074
#define CP3_FVR2_DAC3   0x8075
#define CP3_PRG1_DAC3   0x8076
#define CP3_PRG2_DAC3   0x8077
#define CP3_VSS_DAC3    0x8078
#define CP3_OPA1_DAC3   0x8079
#define CP3_OPA2_DAC3   0x807A
#define CP3_A0_DAC4     0x8080
#define CP3_A1_DAC4     0x8081
#define CP3_B3_DAC4     0x8082
#define CP3_B1_DAC4     0x8083
#define CP3_C3_DAC4     0x8084
#define CP3_FVR2_DAC4   0x8085
#define CP3_PRG1_DAC4   0x8086
#define CP3_PRG2_DAC4   0x8087
#define CP3_VSS_DAC4    0x8088
#define CP3_OPA1_DAC4   0x8089
#define CP3_OPA2_DAC4   0x808A
#define CP3_A0_VSS      0x8090
#define CP3_A1_VSS      0x8091
#define CP3_B3_VSS      0x8092
#define CP3_B1_VSS      0x8093
#define CP3_C3_VSS      0x8094
#define CP3_FVR2_VSS    0x8095
#define CP3_PRG1_VSS    0x8096
#define CP3_PRG2_VSS    0x8097
#define CP3_VSS_VSS     0x8098
#define CP3_OPA1_VSS    0x8099
#define CP3_OPA2_VSS    0x809A
//Optionally OR any of the following with the above
#define CP3_INT_L2H     0x20000
#define CP3_INT_H2L     0x10000
#define CP3_LOW_POWER   0x2000
#define CP3_INVERT      0x1000
#define CP3_FILTER      0x0800
#define CP3_HYST        0x0200
#define CP3_SYNC        0x0100

// Constants used in setup_comparator_4() are:
// Pick one of the following
#define CP4_A0_A2       0x8000
#define CP4_A1_A2       0x8001
#define CP4_B5_A2       0x8002
#define CP4_B1_A2       0x8003
#define CP4_C3_A2       0x8004
#define CP4_FVR2_A2     0x8005
#define CP4_PRG1_A2     0x8006
#define CP4_PRG2_A2     0x8007
#define CP4_VSS_A2      0x8008
#define CP4_OPA1_A2     0x8009
#define CP4_OPA2_A2     0x800A
#define CP4_A0_B6       0x8010
#define CP4_A1_B6       0x8011
#define CP4_B5_B6       0x8012
#define CP4_B1_B6       0x8013
#define CP4_C3_B6       0x8014
#define CP4_FVR2_B6     0x8015
#define CP4_PRG1_B6     0x8016
#define CP4_PRG2_B6     0x8017
#define CP4_VSS_B6      0x8018
#define CP4_OPA1_B6     0x8019
#define CP4_OPA2_B6     0x801A
#define CP4_A0_FVR2     0x8020
#define CP4_A1_FVR2     0x8021
#define CP4_B5_FVR2     0x8022
#define CP4_B1_FVR2     0x8023
#define CP4_C3_FVR2     0x8024
#define CP4_FVR2_FVR2   0x8025
#define CP4_PRG1_FVR2   0x8026
#define CP4_PRG2_FVR2   0x8027
#define CP4_VSS_FVR2    0x8028
#define CP4_OPA1_FVR2   0x8029
#define CP4_OPA2_FVR2   0x802A
#define CP4_A0_PRG1     0x8030
#define CP4_A1_PRG1     0x8031
#define CP4_B5_PRG1     0x8032
#define CP4_B1_PRG1     0x8033
#define CP4_C3_PRG1     0x8034
#define CP4_FVR2_PRG1   0x8035
#define CP4_PRG1_PRG1   0x8036
#define CP4_PRG2_PRG1   0x8037
#define CP4_VSS_PRG1    0x8038
#define CP4_OPA1_PRG1   0x8039
#define CP4_OPA2_PRG1   0x803A
#define CP4_A0_PRG2     0x8040
#define CP4_A1_PRG2     0x8041
#define CP4_B5_PRG2     0x8042
#define CP4_B1_PRG2     0x8043
#define CP4_C3_PRG2     0x8044
#define CP4_FVR2_PRG2   0x8045
#define CP4_PRG1_PRG2   0x8046
#define CP4_PRG2_PRG2   0x8047
#define CP4_VSS_PRG2    0x8048
#define CP4_OPA1_PRG2   0x8049
#define CP4_OPA2_PRG2   0x804A
#define CP4_A0_DAC1     0x8050
#define CP4_A1_DAC1     0x8051
#define CP4_B5_DAC1     0x8052
#define CP4_B1_DAC1     0x8053
#define CP4_C3_DAC1     0x8054
#define CP4_FVR2_DAC1   0x8055
#define CP4_PRG1_DAC1   0x8056
#define CP4_PRG2_DAC1   0x8057
#define CP4_VSS_DAC1    0x8058
#define CP4_OPA1_DAC1   0x8059
#define CP4_OPA2_DAC1   0x805A
#define CP4_A0_DAC2     0x8060
#define CP4_A1_DAC2     0x8061
#define CP4_B5_DAC2     0x8062
#define CP4_B1_DAC2     0x8063
#define CP4_C3_DAC2     0x8064
#define CP4_FVR2_DAC2   0x8065
#define CP4_PRG1_DAC2   0x8066
#define CP4_PRG2_DAC2   0x8067
#define CP4_VSS_DAC2    0x8068
#define CP4_OPA1_DAC2   0x8069
#define CP4_OPA2_DAC2   0x806A
#define CP4_A0_DAC3     0x8070
#define CP4_A1_DAC3     0x8071
#define CP4_B5_DAC3     0x8072
#define CP4_B1_DAC3     0x8073
#define CP4_C3_DAC3     0x8074
#define CP4_FVR2_DAC3   0x8075
#define CP4_PRG1_DAC3   0x8076
#define CP4_PRG2_DAC3   0x8077
#define CP4_VSS_DAC3    0x8078
#define CP4_OPA1_DAC3   0x8079
#define CP4_OPA2_DAC3   0x807A
#define CP4_A0_DAC4     0x8080
#define CP4_A1_DAC4     0x8081
#define CP4_B5_DAC4     0x8082
#define CP4_B1_DAC4     0x8083
#define CP4_C3_DAC4     0x8084
#define CP4_FVR2_DAC4   0x8085
#define CP4_PRG1_DAC4   0x8086
#define CP4_PRG2_DAC4   0x8087
#define CP4_VSS_DAC4    0x8088
#define CP4_OPA1_DAC4   0x8089
#define CP4_OPA2_DAC4   0x808A
#define CP4_A0_VSS      0x8090
#define CP4_A1_VSS      0x8091
#define CP4_B5_VSS      0x8092
#define CP4_B1_VSS      0x8093
#define CP4_C3_VSS      0x8094
#define CP4_FVR2_VSS    0x8095
#define CP4_PRG1_VSS    0x8096
#define CP4_PRG2_VSS    0x8097
#define CP4_VSS_VSS     0x8098
#define CP4_OPA1_VSS    0x8099
#define CP4_OPA2_VSS    0x809A
//Optionally OR any of the following with the above
#define CP4_INT_L2H     0x20000
#define CP4_INT_H2L     0x10000
#define CP4_LOW_POWER   0x2000
#define CP4_INVERT      0x1000
#define CP4_FILTER      0x0800
#define CP4_HYST        0x0200
#define CP4_SYNC        0x0100

// Constants used in setup_comparator_5() are:
// Pick one of the following
#define CP5_A0_A2       0x8000
#define CP5_C6_A2       0x8001
#define CP5_C2_A2       0x8002
#define CP5_C4_A2       0x8003
#define CP5_C3_A2       0x8004
#define CP5_FVR2_A2     0x8005
#define CP5_PRG3_A2     0x8006
#define CP5_VSS_A2      0x8008
#define CP5_OPA3_A2     0x8009
#define CP5_A0_B7       0x8010
#define CP5_C6_B7       0x8011
#define CP5_C2_B7       0x8012
#define CP5_C4_B7       0x8013
#define CP5_C3_B7       0x8014
#define CP5_FVR2_B7     0x8015
#define CP5_PRG3_B7     0x8016
#define CP5_VSS_B7      0x8018
#define CP5_OPA3_B7     0x8019
#define CP5_A0_FVR2     0x8020
#define CP5_C6_FVR2     0x8021
#define CP5_C2_FVR2     0x8022
#define CP5_C4_FVR2     0x8023
#define CP5_C3_FVR2     0x8024
#define CP5_FVR2_FVR2   0x8025
#define CP5_PRG3_FVR2   0x8026
#define CP5_VSS_FVR2    0x8028
#define CP5_OPA3_FVR2   0x8029
#define CP5_A0_PRG3     0x8030
#define CP5_C6_PRG3     0x8031
#define CP5_C2_PRG3     0x8032
#define CP5_C4_PRG3     0x8033
#define CP5_C3_PRG3     0x8034
#define CP5_FVR2_PRG3   0x8035
#define CP5_PRG3_PRG3   0x8036
#define CP5_VSS_PRG3    0x8038
#define CP5_OPA3_PRG3   0x8039
#define CP5_A0_DAC5     0x8050
#define CP5_C6_DAC5     0x8051
#define CP5_C2_DAC5     0x8052
#define CP5_C4_DAC5     0x8053
#define CP5_C3_DAC5     0x8054
#define CP5_FVR2_DAC5   0x8055
#define CP5_PRG3_DAC5   0x8056
#define CP5_VSS_DAC5    0x8058
#define CP5_OPA3_DAC5   0x8059
#define CP5_A0_DAC7     0x8070
#define CP5_C6_DAC7     0x8071
#define CP5_C2_DAC7     0x8072
#define CP5_C4_DAC7     0x8073
#define CP5_C3_DAC7     0x8074
#define CP5_FVR2_DAC7   0x8075
#define CP5_PRG3_DAC7   0x8076
#define CP5_VSS_DAC7    0x8078
#define CP5_OPA3_DAC7   0x8079
#define CP5_A0_VSS      0x8090
#define CP5_C6_VSS      0x8091
#define CP5_C2_VSS      0x8092
#define CP5_C4_VSS      0x8093
#define CP5_C3_VSS      0x8094
#define CP5_FVR2_VSS    0x8095
#define CP5_PRG3_VSS    0x8096
#define CP5_VSS_VSS     0x8098
#define CP5_OPA3_VSS    0x8099
//Optionally OR any of the following with the above
#define CP5_INT_L2H     0x20000
#define CP5_INT_H2L     0x10000
#define CP5_LOW_POWER   0x2000
#define CP5_INVERT      0x1000
#define CP5_FILTER      0x0800
#define CP5_HYST        0x0200
#define CP5_SYNC        0x0100

// Constants used in setup_comparator_6() are:
// Pick one of the following
#define CP6_A0_A2       0x8000
#define CP6_C6_A2       0x8001
#define CP6_C2_A2       0x8002
#define CP6_C4_A2       0x8003
#define CP6_C3_A2       0x8004
#define CP6_FVR2_A2     0x8005
#define CP6_PRG3_A2     0x8006
#define CP6_VSS_A2      0x8008
#define CP6_OPA3_A2     0x8009
#define CP6_A0_A6       0x8010
#define CP6_C6_A6       0x8011
#define CP6_C2_A6       0x8012
#define CP6_C4_A6       0x8013
#define CP6_C3_A6       0x8014
#define CP6_FVR2_A6     0x8015
#define CP6_PRG3_A6     0x8016
#define CP6_VSS_A6      0x8018
#define CP6_OPA3_A6     0x8019
#define CP6_A0_FVR2     0x8020
#define CP6_C6_FVR2     0x8021
#define CP6_C2_FVR2     0x8022
#define CP6_C4_FVR2     0x8023
#define CP6_C3_FVR2     0x8024
#define CP6_FVR2_FVR2   0x8025
#define CP6_PRG3_FVR2   0x8026
#define CP6_VSS_FVR2    0x8028
#define CP6_OPA3_FVR2   0x8029
#define CP6_A0_PRG3     0x8030
#define CP6_C6_PRG3     0x8031
#define CP6_C2_PRG3     0x8032
#define CP6_C4_PRG3     0x8033
#define CP6_C3_PRG3     0x8034
#define CP6_FVR2_PRG3   0x8035
#define CP6_PRG3_PRG3   0x8036
#define CP6_VSS_PRG3    0x8038
#define CP6_OPA3_PRG3   0x8039
#define CP6_A0_DAC5     0x8050
#define CP6_C6_DAC5     0x8051
#define CP6_C2_DAC5     0x8052
#define CP6_C4_DAC5     0x8053
#define CP6_C3_DAC5     0x8054
#define CP6_FVR2_DAC5   0x8055
#define CP6_PRG3_DAC5   0x8056
#define CP6_VSS_DAC5    0x8058
#define CP6_OPA3_DAC5   0x8059
#define CP6_A0_DAC7     0x8070
#define CP6_C6_DAC7     0x8071
#define CP6_C2_DAC7     0x8072
#define CP6_C4_DAC7     0x8073
#define CP6_C3_DAC7     0x8074
#define CP6_FVR2_DAC7   0x8075
#define CP6_PRG3_DAC7   0x8076
#define CP6_VSS_DAC7    0x8078
#define CP6_OPA3_DAC7   0x8079
#define CP6_A0_VSS      0x8090
#define CP6_C6_VSS      0x8091
#define CP6_C2_VSS      0x8092
#define CP6_C4_VSS      0x8093
#define CP6_C3_VSS      0x8094
#define CP6_FVR2_VSS    0x8095
#define CP6_PRG3_VSS    0x8096
#define CP6_VSS_VSS     0x8098
#define CP6_OPA3_VSS    0x8099
//Optionally OR any of the following with the above
#define CP6_INT_L2H     0x20000
#define CP6_INT_H2L     0x10000
#define CP6_LOW_POWER   0x2000
#define CP6_INVERT      0x1000
#define CP6_FILTER      0x0800
#define CP6_HYST        0x0200
#define CP6_SYNC        0x0100



#bit C1OUT = getenv("SFR:CM1CON0").6
#bit C2OUT = getenv("SFR:CM2CON0").6
#bit C3OUT = getenv("SFR:CM3CON0").6
#bit C4OUT = getenv("SFR:CM4CON0").6
#bit C5OUT = getenv("SFR:CM5CON0").6
#bit C6OUT = getenv("SFR:CM6CON0").6

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
//                              SETUP_DAC2(), DAC_WRITE2()
//                              SETUP_DAC3(), DAC_WRITE3()
//                              SETUP_DAC4(), DAC_WRITE4()
//                              SETUP_DAC5(), DAC_WRITE5()
//                              SETUP_DAC7(), DAC_WRITE7()
// DAC Prototypes:
_bif void setup_dac(int8 mode);
_bif void setup_dac2(int8 mode);
_bif void setup_dac3(int8 mode);
_bif void setup_dac4(int8 mode);
_bif void setup_dac5(int8 mode);
_bif void setup_dac7(int8 mode);
_bif void dac_write(int16 value);
_bif void dac_write2(int16 value);
_bif void dac_write3(int8 value);
_bif void dac_write4(int8 value);
_bif void dac_write5(int16 value);
_bif void dac_write7(int8 value);
// Constants used in SETUP_DAC() are:
#define DAC_OFF              0
#define DAC_VSS_VDD          0x80
#define DAC_REF0_VDD         0x81
#define DAC_REF1_VDD         0x82
#define DAC_VSS_VREF         0x84
#define DAC_REF0_VREF        0x85
#define DAC_REF1_VREF        0x86
#define DAC_VSS_FVR          0x88
#define DAC_REF0_FVR         0x89
#define DAC_REF1_FVR         0x8A
// The Following may be OR
#define DAC_OUTPUT1          0x20
#define DAC_OUTPUT2          0x10
#define DAC_LEFT_JUSTIFIED  0x40 //10-bit DAC only

////////////////////////////////////////////////////////////////// OPAMP
// OPAMP Functions: SETUP_OPAMPx()
// OPAMP Prototypes:
_bif void setup_opamp1(int32 mode);    
_bif void setup_opamp2(int32 mode);      
_bif void setup_opamp3(int32 mode); 
// Constants used in SETUP_OPAMPx() are: 
#define OPAMP_DISABLED                         0    
#define OPAMP_ENABLED                     0x8000    
// One of the following may be OR'ed in with the above using |    
#define OPAMP_I_TO_OPAIN0                      0 //Inverting input connects to OPAxIN0-      
#define OPAMP_I_TO_OPAIN1                 0x0001 //Inverting input connects to OPAxIN1-      
#define OPAMP_I_TO_DAC1                   0x0002 //Inverting input connects to DAC1 output, OPAMP1 and OPAMP2 only 
#define OPAMP_I_TO_DAC2                   0x0003 //Inverting input connects to DAC2 output, OPAMP1 and OPAMP2 only 
#define OPAMP_I_TO_DAC3                   0x0004 //Inverting input connects to DAC3 output, OPAMP1 and OPAMP2 only 
#define OPAMP_I_TO_DAC4                   0x0005 //Inverting input connects to DAC4 output, OPAMP1 and OPAMP2 only 
#define OPAMP_I_TO_FVR                    0x0006 //Inverting input connects to FVR Buffer 2 output   
#define OPAMP_I_TO_PRG1                   0x0007 //Inverting input connects to PRG1 output, OPAMP1 and OPAMP2 only 
#define OPAMP_I_TO_PRG2                   0x0008 //Inverting input connects to PRG2 output, OPAMP1 and OPAMP2 only 
#define OPAMP_I_TO_DAC5                   0x0002 //Inverting input connects to DAC5 output, OPAMP3 only   
#define OPAMP_I_TO_DAC7                   0x0004 //Inverting input connects to DAC7 output, OPAMP3 only   
#define OPAMP_I_TO_PRG3                   0x0007 //Inverting input connects to PRG3 output, OPAMP3 only   
// One of the following may be OR'ed in with the above using |    
#define OPAMP_NI_TO_OPAIN0                     0 //Non-inverting input connects to OPAxIN0+     
#define OPAMP_NI_TO_OPAIN1                0x0010 //Non-inverting input connects to OPAxIN1+     
#define OPAMP_NI_TO_DAC1                  0x0020 //Non-inverting input connects to DAC1 output, OPAMP1 and OPAMP2 only 
#define OPAMP_NI_TO_DAC2                  0x0030 //Non-inverting input connects to DAC2 output, OPAMP1 and OPAMP2 only 
#define OPAMP_NI_TO_DAC3                  0x0040 //Non-inverting input connects to DAC3 output, OPAMP1 and OPAMP2 only 
#define OPAMP_NI_TO_DAC4                  0x0050 //Non-inverting input connects to DAC4 output, OPAMP1 and OPAMP2 only 
#define OPAMP_NI_TO_FVR                   0x0060 //Non-inverting input connects to FVR Buffer 2 output   
#define OPAMP_NI_TO_PRG1                  0x0070 //Non-inverting input connects to PRG1 output, OPAMP1 and OPAMP2 only 
#define OPAMP_NI_TO_PRG2                  0x0080 //Non-inverting input connects to PRG2 output, OPAMP1 and OPAMP2 only 
#define OPAMP_NI_TO_DAC5                  0x0020 //Non-inverting input connects to DAC5 output, OPAMP3 only   
#define OPAMP_NI_TO_DAC7                  0x0040 //Non-inverting input connects to DAC7 output, OPAMP3 only   
#define OPAMP_NI_TO_PRG3                  0x0070 //Non-inverting input connects to PRG3 output, OPAMP3 only   
// One of the following may be OR'ed in with the above using |    
#define OPAMP_OVERRIDE_MODE_DISABLED           0    
#define OPAMP_OVERRIDE_MODE_TRI_STATED    0x0100    
#define OPAMP_OVERRIDE_MODE_UNITY_GAIN    0x0200    
// One of the following may be OR'ed in with the above using |    
#define OPAMP_OVERRIDE_SOURCE_CCP1             0 
#define OPAMP_OVERRIDE_SOURCE_CCP2      0x010000 
#define OPAMP_OVERRIDE_SOURCE_CCP7      0x020000 
#define OPAMP_OVERRIDE_SOURCE_PWM3      0x040000 
#define OPAMP_OVERRIDE_SOURCE_PWM4      0x050000 
#define OPAMP_OVERRIDE_SOURCE_PWM9      0x060000 
#define OPAMP_OVERRIDE_SOURCE_PWM5      0x080000 
#define OPAMP_OVERRIDE_SOURCE_PWM6      0x090000 
#define OPAMP_OVERRIDE_SOURCE_PWM11     0x0A0000 
#define OPAMP_OVERRIDE_SOURCE_C1OUT     0x0C0000 
#define OPAMP_OVERRIDE_SOURCE_C2OUT     0x0D0000 
#define OPAMP_OVERRIDE_SOURCE_C3OUT     0x0E0000 
#define OPAMP_OVERRIDE_SOURCE_C4OUT     0x0F0000 
#define OPAMP_OVERRIDE_SOURCE_C5OUT     0x100000 
#define OPAMP_OVERRIDE_SOURCE_C6OUT     0x110000 
#define OPAMP_OVERRIDE_SOURCE_CLC1      0x140000 
#define OPAMP_OVERRIDE_SOURCE_CLC2      0x150000 
#define OPAMP_OVERRIDE_SOURCE_CLC3      0x160000 
#define OPAMP_OVERRIDE_SOURCE_CLC4      0x170000 
#define OPAMP_OVERRIDE_SOURCE_COG1A     0x180000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG1B     0x190000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG1C     0x1A0000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG1D     0x1B0000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG2A     0x1C0000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG2B     0x1D0000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG2C     0x1E0000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG2D     0x1F0000 //OPAMP1 and OPAMP2 only 
#define OPAMP_OVERRIDE_SOURCE_COG3A     0x180000 //OPAMP3 only   
#define OPAMP_OVERRIDE_SOURCE_COG3B     0x190000 //OPAMP3 only   
#define OPAMP_OVERRIDE_SOURCE_COG3C     0x1A0000 //OPAMP3 only   
#define OPAMP_OVERRIDE_SOURCE_COG3D     0x1B0000 //OPAMP3 only   
// The following may be OR'ed in with the above using |    
#define OPAMP_OVERRIDE_INVERTED           0x0400    
#define OPAMP_IN_UNITY_GAIN_MODE          0x1000    
#define OPAMP_NI_TO_VSS                            0
#define OPAMP_RESISTOR_LADDER_DISABLED             0
#define OPAMP_RESISTOR_LADDER_ENABLED       0x080000
#define OPAMP_OVERRIDE_MODE_DISABLED               0
#define OPAMP_OVERRIDE_MODE_ENABLED           0x8000

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
#define CLC_INPUT_C3OUT                     0x0A 
#define CLC_INPUT_C4OUT                     0x0B 
#define CLC_INPUT_C5OUT                     0x0C 
#define CLC_INPUT_C6OUT                     0x0D 
#define CLC_INPUT_COG1A                     0x10 
#define CLC_INPUT_COG1B                     0x11 
#define CLC_INPUT_COG2A                     0x12 
#define CLC_INPUT_COG2B                     0x13 
#define CLC_INPUT_COG3A                     0x14 
#define CLC_INPUT_COG3B                     0x15 
#define CLC_INPUT_CCP1                      0x18 
#define CLC_INPUT_CCP2                      0x19 
#define CLC_INPUT_CCP7                      0x1A 
#define CLC_INPUT_PWM3                      0x1C 
#define CLC_INPUT_PWM4                      0x1D 
#define CLC_INPUT_PWM9                      0x1E 
#define CLC_INPUT_PWM5                      0x20 
#define CLC_INPUT_PWM6                      0x21 
#define CLC_INPUT_PWM11                     0x22 
#define CLC_INPUT_SCK1_SCL1                 0x24 
#define CLC_INPUT_SDO1_SDA1                 0x25 
#define CLC_INPUT_ZCD1                      0x26 
#define CLC_INPUT_U1TX                      0x27 
#define CLC_INPUT_U1RX                      0x28 
#define CLC_INPUT_TIMER0                    0x29 
#define CLC_INPUT_TIMER1                    0x2A 
#define CLC_INPUT_TIMER3                    0x2B 
#define CLC_INPUT_TIMER5                    0x2C 
#define CLC_INPUT_TIMER2                    0x2D 
#define CLC_INPUT_TIMER4                    0x2E 
#define CLC_INPUT_TIMER6                    0x2F 
#define CLC_INPUT_TIMER8                    0x30 
#define CLC_INPUT_IOCIF                     0x31 
#define CLC_INPUT_ADCRC                     0x32 
#define CLC_INPUT_LFINTOSC                  0x33 
#define CLC_INPUT_HFINTOSC                  0x34 
#define CLC_INPUT_FOSC                      0x35 
#define CLC_INPUT_MDx                       0x36 

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

////////////////////////////////////////////////////////////////// PRG
// Programmable Ramp Generator (PRG) Functions: SETUP_PRGx(), 
//                                              PRGx_STATUS()
// PRG Prototypes:
_bif void setup_prg1(int16 mode, int8 current, int8 rising_source, int8 falling_source);
_bif void setup_prg2(int16 mode, int8 current, int8 rising_source, int8 falling_source); 
_bif void setup_prg3(int16 mode, int8 current, int8 rising_source, int8 falling_source); 
_bif int8 prg1_status(void);
_bif int8 prg2_status(void); 
_bif int8 prg3_status(void); 
// Constants used in SETUP_PRGx() first parameter are:
#define PRG_ENABLED                             0x8000
#define PRG_DISABLED                            0
// One of the following may be OR'ed in with the above using |
#define PRG_FALLING_INPUT_LEVEL_SENSATIVE       0x2000
#define PRG_FALLING_INPUT_EDGE_SENSATIVE        0
// One of the following may be OR'ed in with the above using |
#define PRG_RISING_INPUT_LEVEL_SENSATIVE        0x1000
#define PRG_RISING_INPUT_EDGE_SENSATIVE         0
// One of the following may be OR'ed in with the above using |
#define PRG_MODE_SLOPE_COMPENSATION             0
#define PRG_MODE_RISING_FALLING_RAMP_GENERATOR  0x0400
#define PRG_MODE_RISING_RAMP_GENERATOR          0x0800 
// One of the following may be OR'ed in with the above using |
#define PRG_ONE_SHOT_ENABLED                    0x0200
#define PRG_ONE_SHOT_DISABLED                   0
// One of the following may be OR'ed in with the above using |
#define PRG_FALLING_INPUT_ACTIVE_LOW            0x0002
#define PRG_FALLING_INPUT_ACTIVE_HIGH           0
// One of the following may be OR'ed in with the above using |
#define PRG_RISING_INPUT_ACTIVE_LOW             0x0001
#define PRG_RISING_INPUT_ACTIVE_HIGH            0
// One of the following may be OR'ed in with the above using |
#define PRG_INPUT_SOURCE_PRGIN0                 0 //PRGxIN0 Pin
#define PRG_INPUT_SOURCE_PRGIN1                 0x010000 //PRGxIN1 Pin 
#define PRG_INPUT_SOURCE_FVR                    0x020000
#define PRG_INPUT_SOURCE_DAC1                   0x030000 //PRG1 and PRG2 only 
#define PRG_INPUT_SOURCE_DAC2                   0x040000 //PRG1 and PRG2 only 
#define PRG_INPUT_SOURCE_DAC3                   0x050000 //PRG1 and PRG2 only 
#define PRG_INPUT_SOURCE_DAC4                   0x060000 //PRG1 and PRG2 only 
#define PRG_INPUT_SOURCE_DAC5                   0x030000 //PRG3 only  
#define PRG_INPUT_SOURCE_DAC7                   0x050000 //PRG3 only  
#define PRG_INPUT_SOURCE_OPA1OUT                0x080000 //PRG1 and PRG2 only 
#define PRG_INPUT_SOURCE_OPA2OUT                0x090000 //PRG1 and PRG2 only 
#define PRG_INPUT_SOURCE_OPA3OUT                0x080000 //PRG3 only  

// Constants used in SETUP_PRGx() third parameter are:
#define PRG_RISING_SOURCE_C1OUT                 0    //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_C2OUT                 0x01 //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_C3OUT                 0x02 //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_C4OUT                 0x03 //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_C5OUT                 0x04 //PRG3 only  
#define PRG_RISING_SOURCE_C6OUT                 0x05 //PRG3 only  
#define PRG_RISING_SOURCE_PRGR                  0x07 //PRGxR Pin 
#define PRG_RISING_SOURCE_PWM3                  0x08 //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_PWM4                  0x09 //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_PWM9                  0x0A //PRG1 and PRG2 only 
#define PRG_RISING_SOURCE_PWM5                  0x0C //PRG3 only  
#define PRG_RISING_SOURCE_PWM6                  0x0D //PRG3 only  
#define PRG_RISING_SOURCE_PWM11                 0x0E //PRG3 only  

// Constants used in SETUP_PRGx() fourth parameter are:
#define PRG_FALLING_SOURCE_C1OUT                0    //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_C2OUT                0x01 //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_C3OUT                0x02 //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_C4OUT                0x03 //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_C5OUT                0x04 //PRG3 only  
#define PRG_FALLING_SOURCE_C6OUT                0x05 //PRG3 only  
#define PRG_FALLING_SOURCE_PRGR                 0x07 //PRGxR Pin 
#define PRG_FALLING_SOURCE_PWM3                 0x08 //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_PWM4                 0x09 //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_PWM9                 0x0A //PRG1 and PRG2 only 
#define PRG_FALLING_SOURCE_PWM5                 0x0C //PRG3 only  
#define PRG_FALLING_SOURCE_PWM6                 0x0D //PRG3 only  
#define PRG_FALLING_SOURCE_PWM11                0x0E //PRG3 only  

// Constants returned from PRGx_STATUS() are:
#define PRG_SLOPE_GENERATOR_READY               0x04
#define PRG_SLOPE_GENERATOR_NOT_READY           0x00

#bit PRG1GO = getenv("SFR:PRG1CON0").0
#bit PRG1OS = getenv("SFR:PRG1CON0").1
#bit PRG2GO = getenv("SFR:PRG2CON0").0 
#bit PRG2OS = getenv("SFR:PRG2CON0").1 
#bit PRG3GO = getenv("SFR:PRG3CON0").0 
#bit PRG3OS = getenv("SFR:PRG3CON0").1 

////////////////////////////////////////////////////////////////// COG
// COG Functions: SETUP_COG(), SET_COG_DEAD_BAND(), SET_COG_PHASE(),
//                SET_COG_BLANKING(), COG_STATUS(), COG_RESTART(), 
//                SETUP_COG2(), SET_COG2_DEAD_BAND(), SET_COG2_PHASE(), 
//                SET_COG2_BLANKING(), COG2_STATUS(), COG2_RESTART(), 
//                SETUP_COG3(), SET_COG3_DEAD_BAND(), SET_COG3_PHASE(), 
// COG Prototypes:
_bif void setup_cog(int32 mode, int16 shutdown, int8 steering);
_bif void setup_cog2(int32 mode, int16 shutdown, int8 steering); 
_bif void setup_cog3(int32 mode, int16 shutdown, int8 steering); 
_bif void set_cog_dead_band(int8 falling_deadband, int8 rising_deadband);
_bif void set_cog2_dead_band(int8 falling_deadband, int8 rising_deadband); 
_bif void set_cog3_dead_band(int8 falling_deadband, int8 rising_deadband); 
_bif void set_cog_phase(int8 falling_phase, int8 rising_phase);
_bif void set_cog2_phase(int8 falling_phase, int8 rising_phase); 
_bif void set_cog3_phase(int8 falling_phase, int8 rising_phase); 
_bif void set_cog_blanking(int8 falling_value, int8 rising_value);
_bif void set_cog2_blanking(int8 falling_value, int8 rising_value); 
_bif void set_cog3_blanking(int8 falling_value, int8 rising_value); 
_bif int8 cog_status(void);
_bif int8 cog2_status(void); 
_bif int8 cog3_status(void); 
_bif void cog_restart(void);
_bif void cog2_restart(void); 
_bif void cog3_restart(void); 
// Constants used in SETUP_COG(), SETUP_COG2() and SETUP_COG3() first parameter are:  
#define COG_ENABLED                              0x80
#define COG_DISABLED                             0
// One of the following may be OR'ed in with the above using |
#define COG_CLOCK_HFINTOSC                       0x10
#define COG_CLOCK_FOSC                           0x08
#define COG_CLOCK_FOSC_DIV_4                     0
// One of the following may be OR'ed in with the above using |
#define COG_PUSH_PULL                            0x05
#define COG_HALF_BRIDGE                          0x04
#define COG_FULL_BRIDGE_REV                      0x03
#define COG_FULL_BRIDGE                          0x02
#define COG_SYNC_PWM                             0x01
#define COG_PWM                                  0
// One of the following may be OR'ed in with the above using |
#define COG_RISING_DEAD_BAND_SOURCE_DELAY_CHAIN  0x8000
#define COG_RISING_DEAD_BAND_SOURCE_COG_CLK      0
// One of the following may be OR'ed in with the above using |
#define COG_FALLING_DEAD_BAND_SOURCE_DELAY_CHAIN 0x4000
#define COG_FALLING_DEAD_BAND_SOURCE_COG_CLK     0
// Any of the following may be OR'ed in with the above using |
#define COG_COGD_ACTIVE_LOW                      0x0800
#define COG_COGC_ACTIVE_LOW                      0x0400
#define COG_COGB_ACTIVE_LOW                      0x0200
#define COG_COGA_ACTIVE_LOW                      0x0100
// One of the following may be OR'ed in with the above using |
#define COG_FALLING_EDGE_SENSITIVE               0x800000  
#define COG_FALLING_LEVEL_SENSITIVE              0
// One of the following may be OR'ed in with the above using |
#define COG_FALLING_SOURCE_COGxIN                0x010000 
#define COG_FALLING_SOURCE_C1OUT                 0x020000 
#define COG_FALLING_SOURCE_C2OUT                 0x030000 
#define COG_FALLING_SOURCE_C3OUT                 0x040000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_C5OUT                 0x040000 //COG3 only   
#define COG_FALLING_SOURCE_C4OUT                 0x050000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_C6OUT                 0x050000 //COG3 only  
#define COG_FALLING_SOURCE_CCP1                  0x060000 //COG1 and COG3 only 
#define COG_FALLING_SOURCE_CCP7                  0x060000 //COG3 only  
#define COG_FALLING_SOURCE_CCP2                  0x070000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_PWM3                  0x080000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_PWM9                  0x080000 //COG3 only  
#define COG_FALLING_SOURCE_PWM4                  0x090000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_PWM5                  0x0A0000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_PWM11                 0x0A0000 //COG3 only  
#define COG_FALLING_SOURCE_PWM6                  0x0B0000 //COG1 and COG2 only 
#define COG_FALLING_SOURCE_MDx                   0x0C0000  
#define COG_FALLING_SOURCE_CLC1                  0x0D0000 
#define COG_FALLING_SOURCE_CLC2                  0x0E0000 
#define COG_FALLING_SOURCE_CLC3                  0x0F0000 
#define COG_FALLING_SOURCE_CLC4                  0x100000 
// One of the following may be OR'ed in with the above using |
#define COG_RISING_EDGE_SENSITIVE                0x80000000  
#define COG_RISING_LEVEL_SENSITIVE               0
// One of the following may be OR'ed in with the above using |
#define COG_RISING_SOURCE_COGxIN                 0x01000000 
#define COG_RISING_SOURCE_C1OUT                  0x02000000 
#define COG_RISING_SOURCE_C2OUT                  0x03000000 
#define COG_RISING_SOURCE_C3OUT                  0x04000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_C5OUT                  0x04000000 //COG3 only   
#define COG_RISING_SOURCE_C4OUT                  0x05000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_C6OUT                  0x05000000 //COG3 only  
#define COG_RISING_SOURCE_CCP1                   0x06000000 //COG1 and COG2 only
#define COG_RISING_SOURCE_CCP7                   0x06000000 //COG3 only  
#define COG_RISING_SOURCE_CCP2                   0x07000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_PWM3                   0x08000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_PWM9                   0x08000000 //COG3 only  
#define COG_RISING_SOURCE_PWM4                   0x09000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_PWM5                   0x0A000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_PWM11                  0x0A000000 //COG3 only  
#define COG_RISING_SOURCE_PWM6                   0x0B000000 //COG1 and COG2 only 
#define COG_RISING_SOURCE_MDx                    0x0C000000  
#define COG_RISING_SOURCE_CLC1                   0x0D000000 
#define COG_RISING_SOURCE_CLC2                   0x0E000000 
#define COG_RISING_SOURCE_CLC3                   0x0F000000 
#define COG_RISING_SOURCE_CLC4                   0x10000000 

// Constants used in SETUP_COG(), SETUP_COG2() and SETUP_COG3() second parameter are:  
#define COG_NO_AUTO_SHUTDOWN                     0
#define COG_SHUTDOWN_ON_COGxIN                   0x0100 
#define COG_SHUTDOWN_ON_C1OUT                    0x0200
#define COG_SHUTDOWN_ON_C2OUT                    0x0400
#define COG_SHUTDOWN_ON_C3OUT                    0x0800 //COG1 and COG2 only 
#define COG_SHUTDOWN_ON_C5OUT                    0x0800 //COG3 only  
#define COG_SHUTDOWN_ON_C4OUT                    0x1000 //COG1 and COG2 only 
#define COG_SHUTDOWN_ON_C6OUT                    0x1000 //COG3 only  
#define COG_SHUTDOWN_ON_CLC2                     0x2000 //COG1 and COG2 only 
#define COG_SHUTDOWN_ON_CLC4                     0x2000 //COG3 only  
#define COG_SHUTDOWN_ON_TIMER2                   0x4000 //COG1 and COG2 only 
#define COG_SHUTDOWN_ON_TIMER6                   0x4000 //COG3 only  
#define COG_SHUTDOWN_ON_TIMER4                   0x8000 //COG1 and COG2 only 
#define COG_SHUTDOWN_ON_TIMER8                   0x8000 //COG3 only  
#define COG_AUTO_RESTART                         0x0040
// One of the following may be OR'ed in with the above using |
#define COG_SHUTDOWN_BD_HIGH                     0x0030 
#define COG_SHUTDOWN_BD_LOW                      0x0020 
#define COG_SHUTDOWN_BD_TRI_STATED               0x0010 
#define COG_SHUTDOWN_BD_INACTIVE_STATE           0 
// One of the following may be OR'ed in with the above using |
#define COG_SHUTDOWN_AC_HIGH                     0x000C 
#define COG_SHUTDOWN_AC_LOW                      0x0008 
#define COG_SHUTDOWN_AC_TRI_STATED               0x0004 
#define COG_SHUTDOWN_AC_INACTIVE_STATE           0 

// Constants used in SETUP_COG(), SETUP_COG2() and SETUP_COG3() third prameter are:  
// One of the following may be used:
#define COG_COGD_STATIC_HIGH                     0x80
#define COG_COGD_STATIC_LOW                      0
// One of the following may be OR'ed in with the above using |
#define COG_COGC_STATIC_HIGH                     0x40
#define COG_COGC_STATIC_LOW                      0
// One of the following may be OR'ed in with the above using |
#define COG_COGB_STATIC_HIGH                     0x20
#define COG_COGB_STATIC_LOW                      0
// One of the following may be OR'ed in with the above using |
#define COG_COGA_STATIC_HIGH                     0x10
#define COG_COGA_STATIC_LOW                      0
// Any of the following may be OR'ed in with the above using |
#define COG_PULSE_STEERING_D                     0x08
#define COG_PULSE_STEERING_C                     0x04
#define COG_PULSE_STEERING_B                     0x02
#define COG_PULSE_STEERING_A                     0x01

// Constants returned from COG_STATUS(), COG2_STATUS() and COG3_STATUS() are:  
// Constants returned from COG_STATUS(), COG2_STATUS(), COG3_STATUS() and COG4_STATUS() are:
#define COG_AUTO_SHUTDOWN                        0x80

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
// Data Signal Modulator Functions: SETUP_DSM(), SETUP_DSM2(), SETUP_DSM3()   
// DSM Prototypes:
_bif void setup_dsm(int1 enable);
_bif void setup_dsm(unsigned int16 mode, unsigned int8 source); 
_bif void setup_dsm(unsigned int16 mode, unsigned int8 source, unsigned int16 carrier); 
_bif void setup_dsm2(int1 enable);  
_bif void setup_dsm2(unsigned int16 mode, unsigned int8 source);  
_bif void setup_dsm2(unsigned int16 mode, unsigned int8 source, unsigned int16 carrier);  
_bif void setup_dsm3(int1 enable); 
_bif void setup_dsm3(unsigned int16 mode, unsigned int8 source); 
_bif void setup_dsm3(unsigned int16 mode, unsigned int8 source, unsigned int16 carrier); 
// Constants used in SETUP_DSM(), SETUP_DSM2() and SETUP_DSM3() first parameter are:  
#define DSM_ENABLED                     0x80
#define DSM_DISABLED                    0x00
// Any of the following may be or'ed in with the above using |
#define DSM_OUTPUT_INVERTED             0x10
#define DSM_SYNC_CARRIER_LOW            0x0100 
#define DSM_INVERT_CARRIER_LOW          0x0200 
#define DSM_SYNC_CARRIER_HIGH           0x1000 
#define DSM_INVERT_CARRIER_HIGH         0x2000 
//Constants used as the second parameter are:
#define DSM_SOURCE_MDMOD                0x00 //MDxMOD pin 
#define DSM_SOURCE_MANUAL               0x01 //MDxBIT 
#define DSM_SOURCE_U1TX                 0x02 
#define DSM_SOURCE_U1DT                 0x03 
#define DSM_SOURCE_SDO1                 0x04 
#define DSM_SOURCE_CCP1                 0x05 
#define DSM_SOURCE_CCP2                 0x06 
#define DSM_SOURCE_CCP7                 0x07 
#define DSM_SOURCE_PWM3                 0x09 
#define DSM_SOURCE_PWM4                 0x0A 
#define DSM_SOURCE_PWM9                 0x0B 
#define DSM_SOURCE_PWM5                 0x0D 
#define DSM_SOURCE_PWM6                 0x0E 
#define DSM_SOURCE_PWM11                0x0F 
#define DSM_SOURCE_CLC1                 0x11 
#define DSM_SOURCE_CLC2                 0x12 
#define DSM_SOURCE_CLC3                 0x13 
#define DSM_SOURCE_CLC4                 0x14 
#define DSM_SOURCE_C1OUT                0x15 
#define DSM_SOURCE_C2OUT                0x16 
#define DSM_SOURCE_C3OUT                0x17 
#define DSM_SOURCE_C4OUT                0x18 
#define DSM_SOURCE_C5OUT                0x19 
#define DSM_SOURCE_C6OUT                0x1A 
//Constants used as the third parameter are:
#define DSM_CARRIER_LOW_MDCL            0x0000 //MDxCL pin 
#define DSM_CARRIER_LOW_FOSC            0x0001 
#define DSM_CARRIER_LOW_HFINTOSC        0x0002 
#define DSM_CARRIER_LOW_CCP1            0x0003 
#define DSM_CARRIER_LOW_CCP2            0x0004 
#define DSM_CARRIER_LOW_CCP7            0x0005 
#define DSM_CARRIER_LOW_PWM3            0x0007 
#define DSM_CARRIER_LOW_PWM4            0x0008 
#define DSM_CARRIER_LOW_PWM9            0x0009 
#define DSM_CARRIER_LOW_PWM5            0x000B 
#define DSM_CARRIER_LOW_PWM6            0x000C 
#define DSM_CARRIER_LOW_PWM11           0x000D 
#define DSM_CARRIER_LOW_CLC1            0x000F 
#define DSM_CARRIER_LOW_CLC2            0x0010 
#define DSM_CARRIER_LOW_CLC3            0x0011 
#define DSM_CARRIER_LOW_CLC4            0x0012 
#define DSM_CARRIER_HIGH_MDCH           0x0000 //MDxCH pin 
#define DSM_CARRIER_HIGH_FOSC           0x0100 
#define DSM_CARRIER_HIGH_HFINTOSC       0x0200 
#define DSM_CARRIER_HIGH_CCP1           0x0300 
#define DSM_CARRIER_HIGH_CCP2           0x0400 
#define DSM_CARRIER_HIGH_CCP7           0x0500 
#define DSM_CARRIER_HIGH_PWM3           0x0700 
#define DSM_CARRIER_HIGH_PWM4           0x0800 
#define DSM_CARRIER_HIGH_PWM9           0x0900 
#define DSM_CARRIER_HIGH_PWM5           0x0B00 
#define DSM_CARRIER_HIGH_PWM6           0x0C00 
#define DSM_CARRIER_HIGH_PWM11          0x0D00 
#define DSM_CARRIER_HIGH_CLC1           0x0F00 
#define DSM_CARRIER_HIGH_CLC2           0x1000 
#define DSM_CARRIER_HIGH_CLC3           0x1100 
#define DSM_CARRIER_HIGH_CLC4           0x1200 

#bit MD1BIT = getenv("SFR:MD1CON0").0 
#bit MD1OUT = getenv("SFR:MD1CON0").5 
#bit MD2BIT = getenv("SFR:MD2CON0").0  
#bit MD2OUT = getenv("SFR:MD2CON0").5  
#bit MD3BIT = getenv("SFR:MD3CON0").0 
#bit MD3OUT = getenv("SFR:MD3CON0").5 


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes:
_bif void setup_oscillator(int8 mode);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_31KHZ   0
#define OSC_31250   (2*8)
#define OSC_62KHZ   (4*8)
#define OSC_125KHZ  (5*8)
#define OSC_250KHZ  (6*8)
#define OSC_500KHZ  (7*8)
#define OSC_1MHZ    (11*8)
#define OSC_2MHZ    (12*8)
#define OSC_4MHZ    (13*8)
#define OSC_8MHZ    (14*8)
#define OSC_16MHZ   (15*8)
// The following may be OR'ed in with the above using |
#define OSC_TIMER1  1 
#define OSC_INTRC   2
#define OSC_NORMAL  0
// The following may be OR'ed in with the above using |
#define OSC_PLL_ON  0x80
#define OSC_PLL_OFF 0


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
#define sAN0          0x00010000  //| A0
#define sAN1          0x00020000  //| A1
#define sAN2          0x00040000  //| A2
#define sAN3          0x00080000  //| A3
#define sAN4          0x00200000  //| A5
#define sAN8          0x00000400  //| B2
#define sAN9          0x00000800  //| B3
#define sAN10         0x00000200  //| B1
#define sAN11         0x00001000  //| B4
#define sAN12         0x00000100  //| B0
#define sAN13         0x00002000  //| B5
#define sAN14         0x00000004  //| C2
#define sAN15         0x00000008  //| C3
#define sAN16         0x00000010  //| C4
#define sAN17         0x00000020  //| C5
#define sAN18         0x00000040  //| C6
#define sAN19         0x00000080  //| C7
#define NO_ANALOGS             0  // None
#define ALL_ANALOG    0x002F3FFC  // A0 A1 A2 A3 A5 B2 B3 B1 B4 B0 B5 C2 C3 C4 C5 C6 C7

// Optional Second parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE:
#define VSS_VDD                0  //| Range 0 to Vdd
#define VSS_VREF      0x02000000  //| Range 0 to Vref+
#define VSS_FVR       0x03000000  //| Range 0 to FVR
#define VREF_VDD      0x04000000  //| Range Vref- to Vdd
#define VREF_VREF     0x06000000  //| Range Vref- to Vref+
#define VREF_FVR      0x07000000  //| Range Vref- to FVR

// Constants used in SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define SWITCHED_AN1_CHANNEL    0x21 
#define SWITCHED_AN10_CHANNEL   0x2A 
#define SWITCHED_AN18_CHANNEL   0x32 
#define DAC7_CHANNEL            0x37 
#define DAC5_CHANNEL            0x39 
#define DAC4_CHANNEL            0x3A 
#define DAC3_CHANNEL            0x3B 
#define DAC2_CHANNEL            0x3C 
#define TEMPERATURE_INDICATOR   0x3D 
#define DAC1_CHANNEL            0x3E 
#define FVR_CHANNEL             0x3F 

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
#define INT_TIMER1                0x309101
#define INT_TIMER2                0x309102
#define INT_CCP1                  0x309104
#define INT_SSP                   0x309108
#define INT_TBE                   0x309110
#define INT_RDA                   0x309120
#define INT_AD                    0x309140
#define INT_TIMER1_GATE            0x309180
#define INT_BUSCOL                0x309208
#define INT_COMP                  0x309220
#define INT_COMP2                 0x309240
#define INT_OSC_FAIL              0x309280
#define INT_CCP2                  0x309201
#define INT_COMP3                 0x309202
#define INT_CLC                   0x309301
#define INT_CLC2                  0x309302
#define INT_CLC3                  0x309304
#define INT_CLC4                  0x309308
#define INT_COMP4                 0x309204
#define INT_ZCD                   0x309310
#define INT_TIMER5                0x309410
#define INT_TIMER5_GATE            0x309420
#define INT_TIMER4                0x309401
#define INT_TIMER6                0x309402
#define INT_TIMER3                0x309404
#define INT_TIMER3_GATE            0x309408
#define INT_COG                   0x309210
#define INT_COG2                  0x309320
#define INT_TIMER8                0x309440
#define INT_COMP5                 0x309501
#define INT_COMP6                 0x309502
#define INT_COG3                  0x309510
#define INT_CCP7                  0x309540
#define INT_PWM5                  0x309601
#define INT_PWM6                  0x309602
#define INT_PWM11                 0x309604
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
#define INT_IOC_A6                0x30400B08
#define INT_IOC_A6_L2H            0x10400B08
#define INT_IOC_A6_H2L            0x20400B08
#define INT_IOC_A7                0x30800B08
#define INT_IOC_A7_L2H            0x10800B08
#define INT_IOC_A7_H2L            0x20800B08
#define INT_IOC_B0                0x31010B08
#define INT_IOC_B0_L2H            0x11010B08
#define INT_IOC_B0_H2L            0x21010B08
#define INT_IOC_B1                0x31020B08
#define INT_IOC_B1_L2H            0x11020B08
#define INT_IOC_B1_H2L            0x21020B08
#define INT_IOC_B2                0x31040B08
#define INT_IOC_B2_L2H            0x11040B08
#define INT_IOC_B2_H2L            0x21040B08
#define INT_IOC_B3                0x31080B08
#define INT_IOC_B3_L2H            0x11080B08
#define INT_IOC_B3_H2L            0x21080B08
#define INT_IOC_B4                0x31100B08
#define INT_IOC_B4_L2H            0x11100B08
#define INT_IOC_B4_H2L            0x21100B08
#define INT_IOC_B5                0x31200B08
#define INT_IOC_B5_L2H            0x11200B08
#define INT_IOC_B5_H2L            0x21200B08
#define INT_IOC_B6                0x31400B08
#define INT_IOC_B6_L2H            0x11400B08
#define INT_IOC_B6_H2L            0x21400B08
#define INT_IOC_B7                0x31800B08
#define INT_IOC_B7_L2H            0x11800B08
#define INT_IOC_B7_H2L            0x21800B08
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
#define INT_IOC_C6                0x32400B08
#define INT_IOC_C6_L2H            0x12400B08
#define INT_IOC_C6_H2L            0x22400B08
#define INT_IOC_C7                0x32800B08
#define INT_IOC_C7_L2H            0x12800B08
#define INT_IOC_C7_H2L            0x22800B08
#define INT_IOC_E3                0x34080B08
#define INT_IOC_E3_L2H            0x14080B08
#define INT_IOC_E3_H2L            0x24080B08

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
#define INT_RA6                   0x30400B08
#define INT_RA6_L2H               0x10400B08
#define INT_RA6_H2L               0x20400B08
#define INT_RA7                   0x30800B08
#define INT_RA7_L2H               0x10800B08
#define INT_RA7_H2L               0x20800B08
#define INT_RB                    0x30FF0B08
#define INT_RB0                   0x31010B08
#define INT_RB0_L2H               0x11010B08
#define INT_RB0_H2L               0x21010B08
#define INT_RB1                   0x31020B08
#define INT_RB1_L2H               0x11020B08
#define INT_RB1_H2L               0x21020B08
#define INT_RB2                   0x31040B08
#define INT_RB2_L2H               0x11040B08
#define INT_RB2_H2L               0x21040B08
#define INT_RB3                   0x31080B08
#define INT_RB3_L2H               0x11080B08
#define INT_RB3_H2L               0x21080B08
#define INT_RB4                   0x31100B08
#define INT_RB4_L2H               0x11100B08
#define INT_RB4_H2L               0x21100B08
#define INT_RB5                   0x31200B08
#define INT_RB5_L2H               0x11200B08
#define INT_RB5_H2L               0x21200B08
#define INT_RB6                   0x31400B08
#define INT_RB6_L2H               0x11400B08
#define INT_RB6_H2L               0x21400B08
#define INT_RB7                   0x31800B08
#define INT_RB7_L2H               0x11800B08
#define INT_RB7_H2L               0x21800B08
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
#define INT_RC6                   0x32400B08
#define INT_RC6_L2H               0x12400B08
#define INT_RC6_H2L               0x22400B08
#define INT_RC7                   0x32800B08
#define INT_RC7_L2H               0x12800B08
#define INT_RC7_H2L               0x22800B08
#define INT_RE                    0x30FF0B08
#define INT_RE3                   0x32080B08
#define INT_RE3_L2H               0x12080B08
#define INT_RE3_H2L               0x22080B08

#list
