//////////// Standard Header file for the PIC16F753 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16F753

#nolist
//////// Program memory: 2048x14  Data RAM: 128  Stack: 8
//////// I/O: 12   Analog Pins: 8
//////// C Scratch area: 77   ID Location: 2000
//////// Fuses: INTRC,EC,WDT,NOWDT,PUT,NOPUT,MCLR,NOMCLR,PROTECT,NOPROTECT
//////// Fuses: NOBROWNOUT,BROWNOUT_NOSL,BROWNOUT,WRT,WRT_200,WRT_100,NOWRT
//////// Fuses: CLKOUT,NOCLKOUT,DEBUG,NODEBUG
//////// 
#if (!defined(__PCM__)||defined(__ISNT_CCS__))
#define _bif
#define int8 char
#define int16 long
#define int32 long long
#define float32 float
#define int1 char

#endif
////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_x_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT(),
// Discrete I/O Prototypes:
_bif void set_tris_a(int8 value);  
_bif void set_tris_c(int8 value);  
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

#define PIN_A0  40
#define PIN_A1  41
#define PIN_A2  42
#define PIN_A3  43
#define PIN_A4  44
#define PIN_A5  45

#define PIN_C0  56
#define PIN_C1  57
#define PIN_C2  58
#define PIN_C3  59
#define PIN_C4  60
#define PIN_C5  61

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
#define WDT_FROM_SLEEP    3    
#define WDT_TIMEOUT      11    
#define MCLR_FROM_SLEEP  19    
#define MCLR_FROM_RUN    27    
#define NORMAL_POWER_UP  25    
#define BROWNOUT_RESTART 26    

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
// Watch Dog Timer Functions: SETUP_WDT() or SETUP_COUNTERS() (see above)
//                            RESTART_WDT()
// WDT base is 18ms
// WDT Prototypes:
_bif void setup_wdt(int16 mode);
_bif void restart_wdt(void);
// Constants used for SETUP_WDT() are:

#define WDT_18MS        8   
#define WDT_36MS        9   
#define WDT_72MS       10   
#define WDT_144MS      11   
#define WDT_288MS      12   
#define WDT_576MS      13   
#define WDT_1152MS     14   
#define WDT_2304MS     15   


////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Timer 1 Prototypes:
_bif void setup_timer_1(int16 mode);
_bif int16 get_timer1(void);
_bif void set_timer1(int16 value);
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_WDT_CLK          0xC5 
#define T1_EXTERNAL         0x85
#define T1_EXTERNAL_SYNC    0x81
#define T1_FOSC             0x45
#define T1_INTERNAL         0x05


#define T1_DIV_BY_1         0x00
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE           0x8000
#define T1_GATE_INVERTED  0xC000
#define T1_GATE_TOGGLE    0xA000
#define T1_GATE_SINGLE    0x9000
#define T1_GATE_TIMER0    0x8100 
#define T1_GATE_SYNCC1OUT 0x8200 
#define T1_GATE_SYNCC2OUT 0x8300 

#define T1_GATE_A4          0
#define T1_GATE_A3          0x10000

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

//The following may be OR'ed with the above for setting up the Hardware Limit Timer
#define T4_RESET_FROM_CCP1      0x0000
#define T4_RESET_FROM_C1OUT     0x0400
#define T4_RESET_FROM_C2OUT     0x0800
#define T4_RESET_FROM_COG1FLT   0x0C00
#define T4_RESET_FROM_COG1OUT0  0x1000
#define T4_RESET_FROM_COG1OUT1  0x1400
#define T4_RESET_ON_H2L         0x0200
#define T4_RESET_ON_L2H         0x0100

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

//The following may be OR'ed with the above for setting up the Hardware Limit Timer
#define T6_RESET_FROM_CCP1      0x0000
#define T6_RESET_FROM_C1OUT     0x0400
#define T6_RESET_FROM_C2OUT     0x0800
#define T6_RESET_FROM_COG1FLT   0x0C00
#define T6_RESET_FROM_COG1OUT0  0x1000
#define T6_RESET_FROM_COG1OUT1  0x1400
#define T6_RESET_ON_H2L         0x0200
#define T6_RESET_ON_L2H         0x0100

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
#define CCP_CAPTURE_FE                   0x04
#define CCP_CAPTURE_RE                   0x05
#define CCP_CAPTURE_DIV_4                0x06
#define CCP_CAPTURE_DIV_16               0x07
#define CCP_COMPARE_SET_ON_MATCH         0x08
#define CCP_COMPARE_CLR_ON_MATCH         0x09
#define CCP_COMPARE_INT                  0x0A             
#define CCP_COMPARE_RESET_TIMER          0x0B            
#define CCP_PWM                          0x0C    
#define CCP_PWM_PLUS_1                   0x1c             
#define CCP_PWM_PLUS_2                   0x2c             
#define CCP_PWM_PLUS_3                   0x3c             

#word   CCP_1       =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_LOW   =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_HIGH  =                    getenv("SFR:CCPR1H") 

////////////////////////////////////////////////////////////////// COMP
// Comparator Variables: C1OUT, C2OUT
// Comparator Prototypes:
_bif void setup_comparator(int32 mode);
// Constants used in setup_comparator() are:
//
#define NC_NC_NC_NC     0x00

//Pick one constant for COMP1
#define CP1_A1_A0       0x8000
#define CP1_C1_A0       0x8001 
#define CP1_C2_A0       0x8002 
#define CP1_C3_A0       0x8003 
#define CP1_SCO_A0      0x8004 
#define CP1_A1_DAC      0x8008 
#define CP1_C1_DAC      0x8009 
#define CP1_C2_DAC      0x800A 
#define CP1_C3_DAC      0x800B 
#define CP1_SCO_DAC     0x800C 
#define CP1_A1_FVR      0x8010 
#define CP1_C1_FVR      0x8011 
#define CP1_C2_FVR      0x8012 
#define CP1_C3_FVR      0x8013 
#define CP1_SCO_FVR     0x8014 
#define CP1_A1_SCO      0x8018 
#define CP1_C1_SCO      0x8019 
#define CP1_C2_SCO      0x801A 
#define CP1_C3_SCO      0x801B 
#define CP1_SCO_SCO     0x801C 
#define CP1_A1_VSS      0x8020 
#define CP1_C1_VSS      0x8021 
#define CP1_C2_VSS      0x8022 
#define CP1_C3_VSS      0x8023 
#define CP1_SCO_VSS     0x8024 
//Optionally OR any of the following with the above
#define CP1_INT_L2H     0x0080
#define CP1_INT_H2L     0x0040
#define CP1_OUT_ON_A2   0x2000
#define CP1_INVERT      0x1000
#define CP1_FILTER      0x0800
#define CP1_FAST        0x0400
#define CP1_HYST        0x0200
#define CP1_SYNC        0x0100

//Pick one constant for COMP2
#define CP2_A1_A0       0x80000000
#define CP2_C1_C0       0x80010000 
#define CP2_C2_C0       0x80020000 
#define CP2_C3_C0       0x80030000 
#define CP2_SCO_C0      0x80040000 
#define CP2_A1_DAC      0x80080000 
#define CP2_C1_DAC      0x80090000 
#define CP2_C2_DAC      0x800A0000 
#define CP2_C3_DAC      0x800B0000 
#define CP2_SCO_DAC     0x800C0000 
#define CP2_A1_FVR      0x80100000 
#define CP2_C1_FVR      0x80110000 
#define CP2_C2_FVR      0x80120000 
#define CP2_C3_FVR      0x80130000 
#define CP2_SCO_FVR     0x80140000 
#define CP2_A1_SCO      0x80180000 
#define CP2_C1_SCO      0x80190000 
#define CP2_C2_SCO      0x801A0000 
#define CP2_C3_SCO      0x801B0000 
#define CP2_SCO_SCO     0x801C0000 
#define CP2_A1_VSS      0x80200000 
#define CP2_C1_VSS      0x80210000 
#define CP2_C2_VSS      0x80220000 
#define CP2_C3_VSS      0x80230000 
#define CP2_SCO_VSS     0x80240000 
//Optionally OR any of the following with the above
#define CP2_INT_L2H     0x00800000
#define CP2_INT_H2L     0x00400000
#define CP2_OUT_ON_C4   0x20000000 
#define CP2_INVERT      0x10000000
#define CP2_FILTER      0x08000000
#define CP2_FAST        0x04000000
#define CP2_HYST        0x02000000
#define CP2_SYNC        0x01000000

#bit C1OUT = getenv("SFR:CM1CON0").6
#bit C2OUT = getenv("SFR:CM2CON0").6

////////////////////////////////////////////////////////////////// VREF
// VREF Prototypes:
_bif void setup_vref(int8 mode);
// Constants used in setup_vref() are:
#define VREF_OFF         0
#define VREF_ON          0x80
#define VREF_OUT_A0      0x20
// One of the following can be OR'ed in with the above using |
#define VREF_BUFFER_SRC_FVRIN 0x18 
#define VREF_BUFFER_SRC_OPAMP 0x10 
#define VREF_BUFFER_SRC_DAC   0x08 
#define VREF_BUFFER_SRC_VBG   0
// One of the following can be OR'ed in with the above using | 
#define VREF_BUFFER_ENABLED   0x01 
#define VREF_BUFFER_DISABLED  0 

////////////////////////////////////////////////////////////////// DAC
// Digital to Analog Functions: SETUP_DAC(), DAC_WRITE()
// DAC Prototypes:
_bif void setup_dac(int8 mode);
_bif void dac_write(int16 value); 
// Constants used in SETUP_DAC() are:
#define DAC_OFF              0
#define DAC_VDD              0x80
#define DAC_OPA1OUT          0x84 
#define DAC_VREF             0x88 
#define DAC_FVR              0x8C 
// The following may be OR'ed in with the above using |
#define DAC_OUTPUT           0x20
// One of the following may be OR'ed in with the above using | 
#define DAC_RIGHT_JUSTIFIED  0x40 
#define DAC_LEFT_JUSTIFIED   0 

////////////////////////////////////////////////////////////////// OPAMP
// OPAMP Functions: SETUP_OPAMPx()
// OPAMP Prototypes:
_bif void setup_opamp1(int8 mode);   
// Constants used in SETUP_OPAMPx() are: 
#define OPAMP_ENABLED                       0x80    
#define OPAMP_DISABLED                      0x00    
#define OPAMP_IN_UNITY_GAIN_MODE            0x10 
#define OPAMP_NOT_IN_UNITY_GAIN_MODE        0x00 
#define OPAMP_I_TO_FVR                      0x0C //Inverting input connects to FVR Buffer 1 output 
#define OPAMP_I_TO_DAC                      0x08 //Inverting input connects to DAC output 
#define OPAMP_I_TO_OPAIN                    0x00 //Inverting input connects to OPAxIN- pin 
#define OPAMP_NI_TO_FVR                     0x03 //Non-inverting input connects to FVR Buffer 1 output 
#define OPAMP_NI_TO_DAC                     0x02 //Non-inverting input connects to DAC output 
#define OPAMP_NI_TO_SLOPE                   0x01 //Non-inverting input connects to SLOPE output 
#define OPAMP_NI_TO_OPAIN                   0x00 //Non-inverting input connects to OPAxIN+ pin  
#define OPAMP_NI_TO_VSS                            0
#define OPAMP_RESISTOR_LADDER_DISABLED             0
#define OPAMP_RESISTOR_LADDER_ENABLED       0x080000
#define OPAMP_OVERRIDE_MODE_DISABLED               0
#define OPAMP_OVERRIDE_MODE_ENABLED           0x8000

////////////////////////////////////////////////////////////////// COG
// COG Functions: SETUP_COG(), SET_COG_DEAD_BAND(), SET_COG_PHASE(),
//                SET_COG_BLANKING(), COG_STATUS(), COG_RESTART()
// COG Prototypes:
_bif void setup_cog(int32 mode, int8 shutdown);
_bif void set_cog_dead_band(int8 falling_deadband, int8 rising_deadband);
_bif void set_cog_phase(int8 value);
_bif void set_cog_blanking(int8 falling_value, int8 rising_value);
_bif int8 cog_status(void);
_bif void cog_restart(void);
// Constants used in SETUP_COG() first parameter are:
#define COG_ENABLED                  0x80
#define COG_DISABLED                 0
#define COG_ENABLE_OUT1              0x40
#define COG_ENABLE_OUT0              0x20
#define COG_OUT1_INVERTED            0x10
#define COG_OUT0_INVERTED            0x08
// One of the following may be OR'ed in with the above using | 
#define COG_PUSH_PULL_MODE           0x01 
#define COG_SYNCHRONOUS_MODE         0 
// One of the following may be OR'ed in with the above using |
#define COG_CLOCK_HFINTOSC           0x02
#define COG_CLOCK_FOSC_DIV_4         0x01
#define COG_CLOCK_FOSC               0
// One of the following may be OR'ed in with the above using |
#define COG_FALLING_EDGE_SENSITIVE   0x8000
#define COG_FALLING_LEVEL_SENSITIVE  0
// One of the following may be OR'ed in with the above using |
#define COG_RISING_EDGE_SENSITIVE    0x4000
#define COG_RISING_LEVEL_SENSITIVE   0
// Any of the following may be OR'ed in with the above using | 
#define COG_FALLING_SOURCE_C1OUT     0x010000 
#define COG_FALLING_SOURCE_C2OUT     0x020000 
#define COG_FALLING_SOURCE_CCP1      0x040000 
#define COG_FALLING_SOURCE_FLT       0x080000 
#define COG_FALLING_SOURCE_TIMER2    0x100000 
#define COG_FALLING_SOURCE_HLT1      0x200000 
#define COG_FALLING_SOURCE_HLT2      0x400000 
// Any of the following may be OR'ed in with the above using | 
#define COG_RISING_SOURCE_C1OUT      0x01000000 
#define COG_RISING_SOURCE_C2OUT      0x02000000 
#define COG_RISING_SOURCE_CCP1       0x04000000 
#define COG_RISING_SOURCE_FLT        0x08000000 
#define COG_RISING_SOURCE_TIMER2     0x10000000 
#define COG_RISING_SOURCE_HLT1       0x20000000 
#define COG_RISING_SOURCE_HLT2       0x40000000 


// Constants used in SETUP_COG() second parameter are:
#define COG_NO_AUTO_SHUTDOWN         0
#define COG_SHUTDOWN_ON_FLT          0x0100 
#define COG_SHUTDOWN_ON_C1OUT        0x0200 
#define COG_SHUTDOWN_ON_C2OUT        0x0400 
#define COG_SHUTDOWN_ON_HLT1         0x0800 
#define COG_SHUTDOWN_ON_HLT2         0x1000 
#define COG_AUTO_RESTART             0x40
// One of the following may be OR'ed in with the above using |
#define COG_SHUTDOWN_OUT0_TRI_STATED     0x0C 
#define COG_SHUTDOWN_OUT0_INACTIVE_STATE 0x08 
#define COG_SHUTDOWN_OUT0_HIGH           0x04 
#define COG_SHUTDOWN_OUT0_LOW            0
// One of the following may be OR'ed in with the above using |
#define COG_SHUTDOWN_OUT1_TRI_STATED     0x30 
#define COG_SHUTDOWN_OUT1_INACTIVE_STATE 0x20 
#define COG_SHUTDOWN_OUT1_HIGH           0x10 
#define COG_SHUTDOWN_OUT1_LOW            0

// Constants returned from COG_STATUS() are:
#define COG_AUTO_SHUTDOWN              0x80


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes:
_bif int8 setup_oscillator(int8 mode);
_bif int8 setup_oscillator(int8 mode, signed int8 tune);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_31KHZ                 0
#define OSC_1MHZ                  0x10
#define OSC_4MHZ                  0x20
#define OSC_8MHZ                  0x30
// A second optional parameter may be used with this part to fine
// tune the speed (signed int,-16 to 15)
// Result may be (ignore all other bits)
#define OSC_LFINTRC_STABLE       2
#define OSC_HFINTRC_STABLE       4


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
#define sAN0          0x000100  //| A0
#define sAN1          0x000200  //| A1
#define sAN2          0x000400  //| A2
#define sAN3          0x001000  //| A4
#define sAN4          0x000001  //| C0
#define sAN5          0x000002  //| C1
#define sAN6          0x000004  //| C2
#define sAN7          0x000008  //| C3
#define NO_ANALOGS           0  // None
#define ALL_ANALOG    0x00170F  // A0 A1 A2 A4 C0 C1 C2 C3

// Optional Second parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE:
#define VSS_VDD       0x000000  //| Range 0 to Vdd
#define VSS_VREF      0x010000  //| Range 0 to Vref+

// Constants used in SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define DAC_CHANNEL   0x0E
#define FVR_CHANNEL   0x0F

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

// Program Memory Write Prototypes:
_bif void write_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif void write_program_eeprom(__ADDRESS__ address, unsigned int16 data);

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
#define INT_RTCC                  0x300B20
#define INT_EXT_L2H               0x50000B10
#define INT_EXT_H2L               0x60000B10
#define INT_EXT                   0x300B10
#define INT_AD                    0x308C40
#define INT_TIMER1                0x308C01
#define INT_TIMER2                0x308C02
#define INT_TIMER0                0x300B20
#define INT_RA                    0x30FF0B08
#define INT_TMR1G                 0x308C80
#define INT_TIMER4                0x308C04
#define INT_CCP1                  0x308D01
#define INT_COG1                  0x308D04
#define INT_COMP                  0x308D10
#define INT_COMP2                 0x308D20
#define INT_RC                    0x32FF0B08
#define INT_TIMER6                0x308C08
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
