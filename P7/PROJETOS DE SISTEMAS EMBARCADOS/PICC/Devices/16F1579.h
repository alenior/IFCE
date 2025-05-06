//////////// Standard Header file for the PIC16F1579 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16F1579

#nolist
//////// Program memory: 8192x14  Data RAM: 992  Stack: 16
//////// I/O: 18   Analog Pins: 12
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: INTRC_IO,ECL,ECM,ECH,NOWDT,WDT_SW,WDT_NOSL,WDT,PUT,NOPUT
//////// Fuses: MCLR,NOMCLR,PROTECT,NOPROTECT,NOBROWNOUT,BROWNOUT_SW
//////// Fuses: BROWNOUT_NOSL,BROWNOUT,CLKOUT,NOCLKOUT,WRT,WRT_800,WRT_200
//////// Fuses: NOWRT,PPS1WAY,NOPPS1WAY,PLL_SW,PLL,STVREN,NOSTVREN,BORV25
//////// Fuses: BORV19,LPBOR,NOLPBOR,DEBUG,NODEBUG,LVP,NOLVP
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
//    INT0,T0CK,T1CK,T1G,CWGIN,U1RX,U1CKIN,T0CKI,T1CKI,RX1
// Output Functions: 
//    NULL,C1OUT,C2OUT,PWM1OUT,PWM2OUT,PWM3OUT,PWM4OUT,CWG1OUTA,CWG1OUTB,U1TX,
//    U1DT,PWM1,PWM2,PWM3,PWM4,TX1,DT1
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
_bif void setup_timer_2(int8 mode, int8 period, int8 postscale); 
_bif int8 get_timer2(void);
_bif void set_timer2(int8 value);
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6 
#define T2_DIV_BY_64        7 

////////////////////////////////////////////////////////////////// PWM
// PWM Functions: SETUP_PWMx, SET_PWMx_PERIOD, SET_PWMx_DUTY,
//                SET_PWMx_PHASE, SET_PWMx_OFFSET
// PWM Prototypes:
_bif void setup_pwm1(int32 mode);
_bif void setup_pwm1(int32 mode, int32 mode1);
_bif void set_pwm1_duty(int16 duty);
_bif void set_pwm1_period(int16 period);
_bif void set_pwm1_phase(int16 phase);
_bif void set_pwm1_offset(int16 offset);
_bif void enable_pwm1_interrupt(int8 interrupt);
_bif void disable_pwm1_interrupt(int8 interrupt);
_bif void clear_pwm1_interrupt(int8 interrupt);
_bif int1 pwm1_interrupt_active(int8 interrupt);
_bif void setup_pwm2(int32 mode);
_bif void setup_pwm2(int32 mode, int32 mode1);
_bif void set_pwm2_duty(int16 duty);
_bif void set_pwm2_period(int16 period);
_bif void set_pwm2_phase(int16 phase);
_bif void set_pwm2_offset(int16 offset);
_bif void enable_pwm2_interrupt(int8 interrupt);
_bif void disable_pwm2_interrupt(int8 interrupt);
_bif void clear_pwm2_interrupt(int8 interrupt);
_bif int1 pwm2_interrupt_active(int8 interrupt);
_bif void setup_pwm3(int16 mode);
_bif void setup_pwm3(int16 mode, int32 mode1);
_bif void set_pwm3_duty(int16 duty);
_bif void set_pwm3_period(int16 period);
_bif void set_pwm3_phase(int16 phase);
_bif void set_pwm3_offset(int16 offset);
_bif void enable_pwm3_interrupt(int8 interrupt);
_bif void disable_pwm3_interrupt(int8 interrupt);
_bif void clear_pwm3_interrupt(int8 interrupt);
_bif int1 pwm3_interrupt_active(int8 interrupt);
_bif void setup_pwm4(int16 mode);  
_bif void setup_pwm4(int16 mode, int32 mode1);  
_bif void set_pwm4_duty(int16 duty);  
_bif void set_pwm4_period(int16 period);  
_bif void set_pwm4_phase(int16 phase);  
_bif void set_pwm4_offset(int16 offset);  
_bif void enable_pwm4_interrupt(int8 interrupt);  
_bif void disable_pwm4_interrupt(int8 interrupt);  
_bif void clear_pwm4_interrupt(int8 interrupt);  
_bif int1 pwm4_interrupt_active(int8 interrupt);  
// Constants used for SETUP_PWMx() first parameter are:
// One of the following two defines can be used by themselves to enable or disable 
// the PWMx module without changing any of the module's settings.
#define PWM_DISABLE                        0
#define PWM_ENABLE                         1

// The following defines should be used to setup and enable the PWMx module.
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


// The following defines are provided for compatibility
#define PWM_DISABLED                       0
#define PWM_ENABLED                        0x80

// Constants used for SETUP_PWMx() second parameter are:
#define PWM_OFFSET_INT_ENABLED             0x08
#define PWM_PHASE_INT_ENABLED              0x04
#define PWM_DUTY_INT_ENABLED               0x02
#define PWM_PERIOD_INT_ENABLED             0x01
#define PWM_LOAD_BUFFERS_ON_TRIGGER        0x4000
// One of the following may be OR'ed in with the above
#define PWM_LOAD_TRIGGER_LD1               0x0100
#define PWM_LOAD_TRIGGER_LD2               0x0200
#define PWM_LOAD_TRIGGER_LD3               0x0300
// One of the following may be OR'ed in with the above
#define PWM_OFFSET_MODE_CONTINUOUS         0x600000
#define PWM_OFFSET_MODE_ONE_SHOT           0x400000
#define PWM_OFFSET_MODE_INDEPENDENT_SLAVE  0x200000
#define PWM_OFFSET_MODE_INDEPENDENT        0
// One of the following may be OR'ed in with the above
#define PWM_OFFSET_MATCH_DECREMENTING      0x100000
#define PWM_OFFSET_MATCH_INCREMENTING      0
// One of the following may be OR'ed in with the above
#define PWM_OFFSET_TRIGGER_OF4             0x030000  
#define PWM_OFFSET_TRIGGER_OF3             0x020000  
#define PWM_OFFSET_TRIGGER_OF2             0x010000  
#define PWM_OFFSET_TRIGGER_OF1             0x000000  

//Constants used for ENABLE_PWMx_INTERRUPT(), DISABLE_PWMx_INTERRUPT(), CLEAR_PWMx_INTERRUPT() and PWMx_INTERRUPT_ACTIVE() are:
#define PWM_OFFSET_INTERRUPT               0x08
#define PWM_PHASE_INTERRUPT                0x04
#define PWM_DUTY_INTERRUPT                 0x02
#define PWM_PERIOD_INTERRUPT               0x01

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
#define CP1_FVR_A0       0x8006 
#define CP1_VSS_A0       0x8007 
#define CP1_A1_DAC       0x8010
#define CP1_C1_DAC       0x8011 
#define CP1_C2_DAC       0x8012 
#define CP1_C3_DAC       0x8013 
#define CP1_FVR_DAC      0x8016 
#define CP1_VSS_DAC      0x8017 
#define CP1_A1_FVR       0x8020
#define CP1_C1_FVR       0x8021 
#define CP1_C2_FVR       0x8022 
#define CP1_C3_FVR       0x8023 
#define CP2_FVR_FVF      0x8026 
#define CP1_VSS_FVR      0x8027 
#define CP1_A1_VSS       0x8030 
#define CP1_C1_VSS       0x8031 
#define CP1_C2_VSS       0x8032 
#define CP1_C3_VSS       0x8033 
#define CP1_FVR_VSS      0x8036 
#define CP1_VSS_VSS      0x8037 
//Optionally OR with any of the following
#define CP1_INT_L2H      0x0080
#define CP1_INT_H2L      0x0040
#define CP1_INVERT       0x1000
#define CP1_FAST         0x0400
#define CP1_HYST         0x0200
#define CP1_SYNC         0x0100

//Pick one constant for COMP2 
#define CP2_A1_C0        0x80000000 
#define CP2_C1_C0        0x80010000 
#define CP2_C2_C0        0x80020000 
#define CP2_C3_C0        0x80030000 
#define CP2_FVR_C0       0x80060000 
#define CP2_VSS_C0       0x80070000 
#define CP2_A1_DAC       0x80100000 
#define CP2_C1_DAC       0x80110000 
#define CP2_C2_DAC       0x80120000 
#define CP2_C3_DAC       0x80130000 
#define CP2_FVR_DAC      0x80140000 
#define CP2_VSS_DAC      0x80170000 
#define CP2_A1_FVR       0x80200000 
#define CP2_C1_FVR       0x80210000 
#define CP2_C2_FVR       0x80220000 
#define CP2_C3_FVR       0x80230000 
#define CP2_FVR_FVR      0x80240000 
#define CP2_VSS_FVR      0x80270000 
#define CP2_A1_VSS       0x80300000 
#define CP2_C1_VSS       0x80310000 
#define CP2_C2_VSS       0x80320000 
#define CP2_C3_VSS       0x80330000 
#define CP2_FVR_VSS      0x80340000 
#define CP2_VSS_VSS      0x80370000 
//Optionally OR with any of the following 
#define CP2_INT_L2H      0x00800000 
#define CP2_INT_H2L      0x00400000 
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
#define DAC_OFF     0
#define DAC_VDD     0x80
#define DAC_VREF    0x81
#define DAC_FVR     0x82
// The following may be OR'ed in with the above using |
#define DAC_OUTPUT  0x20

////////////////////////////////////////////////////////////////// CWG
// CWG Functions: SETUP_CWG(), CWG_STATUS(), CWG_RESTART()
// CWG Prototypes:
_bif void setup_cwg(int32 mode, int16 shutdown, int8 dt_rising, int8 dt_falling);
_bif void cwg_restart(void);
_bif int8 cwg_status(void);
// Constants used in SETUP_CWG() first parameter are:
#define CWG_ENABLED                    0x0080
#define CWG_DISABLED                   0
#define CWG_A_INVERTED                 0x0008
#define CWG_B_INVERTED                 0x0010
#define CWG_CLOCK_HFINTOSC             0x0001
#define CWG_CLOCK_FOSC                 0x0000  
#define CWG_INPUT_C1OUT                0x0000  
#define CWG_INPUT_C2OUT                0x0100   
#define CWG_INPUT_PWM1                 0x0200  
#define CWG_INPUT_PWM2                 0x0300  
#define CWG_INPUT_PWM3                 0x0400  
#define CWG_INPUT_PWM4                 0x0500  
#define CWG_INPUT_CWGIN                0x0600   


// Constants used in SETUP_CWG() second parameter are:
#define CWG_NO_AUTO_SHUTDOWN           0
#define CWG_SHUTDOWN_ON_CWGIN_HIGH     0x0002   
#define CWG_SHUTDOWN_ON_COMP1          0x0004  
#define CWG_SHUTDOWN_ON_COMP2          0x0008   
#define CWG_AUTO_RESTART               0x0040
// One of the following may be OR'ed in with the above using |
#define CWG_SHUTDOWN_A_INACTIVE_STATE  0
#define CWG_SHUTDOWN_A_TRISTATE        0x1000
#define CWG_SHUTDOWN_A_LOW             0x2000
#define CWG_SHUTDOWN_A_HIGH            0x3000
// One of the following may be OR'ed in with the above using |
#define CWG_SHUTDOWN_B_INACTIVE_STATE  0
#define CWG_SHUTDOWN_B_TRISTATE        0x4000
#define CWG_SHUTDOWN_B_LOW             0x8000
#define CWG_SHUTDOWN_B_HIGH            0xC000

// Constants returned from CWG_STATUS() are:
#define CWG_AUTO_SHUTDOWN              0x80


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
#define sAN8            0x00004000    //| C6  
#define sAN9            0x00008000    //| C7  
#define sAN10           0x00000010    //| B4  
#define sAN11           0x00000020    //| B5  
#define NO_ANALOGS             0      // None
#define ALL_ANALOG      0x0017CF30    // A0 A1 A2 A4 C0 C1 C2 C3 C6 C7 B4 B5  

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
#define INT_TIMER1                0x309101
#define INT_TIMER2                0x309102
#define INT_TBE                   0x309110
#define INT_RDA                   0x309120
#define INT_AD                    0x309140
#define INT_TIMER1_GATE            0x309180
#define INT_COMP                  0x309220
#define INT_COMP2                 0x309240
#define INT_PWM1                  0x309310
#define INT_PWM2                  0x309320
#define INT_PWM3                  0x309340
#define INT_PWM4                  0x309380
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
#define INT_RB                    0x30FF0B08
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

#list
