//////// Standard Header file for the PIC14000 device ////////////////
#device PIC14000
#nolist
//////// Program memory: 4031x14  Data RAM: 192  Stack: 8
//////// I/O: 20   Analog Pins: 16
//////// C Scratch area: 20   ID Location: 2000
//////// Fuses: HS,INTRC,NOWDT,WDT,NOPUT,PUT,PROTECT,PROTECT_USER
//////// Fuses: PROTECT_CAL,PROTECT_CODE,NOPROTECT
//////// 
////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_B_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT()
// Constants used to identify pins in the above are:

#define PIN_A0  40
#define PIN_A1  41
#define PIN_A2  42
#define PIN_A3  43

#define PIN_C0  56
#define PIN_C1  57
#define PIN_C2  58
#define PIN_C3  59
#define PIN_C4  60
#define PIN_C5  61
#define PIN_C6  62
#define PIN_C7  63

#define PIN_D0  64
#define PIN_D1  65
#define PIN_D2  66
#define PIN_D3  67
#define PIN_D4  68
#define PIN_D5  69
#define PIN_D6  70
#define PIN_D7  71

////////////////////////////////////////////////////////////////// Useful defines
#define FALSE 0
#define TRUE 1

#define BYTE int
#define BOOLEAN short int

#define getc getch
#define fgetc getch
#define getchar getch
#define putc putchar
#define fputc putchar
#define fgets gets
#define fputs puts

////////////////////////////////////////////////////////////////// Control
// Control Functions:  RESET_CPU(), SLEEP(), RESTART_CAUSE()
// Constants returned from RESTART_CAUSE() are:
#define WDT_FROM_SLEEP  0     
#define WDT_TIMEOUT     8     
#define MCLR_FROM_SLEEP 16    
#define NORMAL_POWER_UP 24    


////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC()
// Constants used for SETUP_TIMER0() are:
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
// constants for the 1st param and the following for
// the 2nd param:

////////////////////////////////////////////////////////////////// WDT
// Watch Dog Timer Functions: SETUP_WDT() or SETUP_COUNTERS() (see above)
//                            RESTART_WDT()
//
#define WDT_18MS        8   
#define WDT_36MS        9   
#define WDT_72MS       10   
#define WDT_144MS      11   
#define WDT_288MS      12   
#define WDT_576MS      13   
#define WDT_1152MS     14   
#define WDT_2304MS     15   


////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC()
// Constants used for SETUP_ADC() are:
#define CURRENT_0          0x00  // Current 0
#define CURRENT_2          0x10  // Current 2
#define CURRENT_4          0x20  // Current 4
#define CURRENT_7          0x30  // Current 7
#define CURRENT_9          0x40  // Current 9
#define CURRENT_11         0x50  // Current 11
#define CURRENT_13         0x60  // Current 13
#define CURRENT_16         0x70  // Current 16
#define CURRENT_18         0x80  // Current 18
#define CURRENT_20         0x90  // Current 20
#define CURRENT_22         0xA0  // Current 22
#define CURRENT_25         0xB0  // Current 25
#define CURRENT_27         0xC0  // Current 27
#define CURRENT_29         0xD0  // Current 29
#define CURRENT_31         0xE0  // Current 31
#define CURRENT_34         0xF0  // Current 34

// Constants used in SETUP_ADC_PORTS() are:
#define NO_ANALOGS                     0xf   // None
#define ALL_ANALOG                     0x0   // A0 A1 A2 A3 D4 D5 D6 D7
#define AN0_AN1_AN2_AN4_AN5_AN6_AN7    0x1   // A0 A1 A2 D4 D5 D6 D7
#define AN0_AN1_AN4_AN5_AN6_AN7        0x2   // A0 A1 D4 D5 D6 D7
#define AN4_AN5_AN6_AN7                0x3   // D4 D5 D6 D7
#define AN0_AN1_AN2_AN3_AN4_AN5_AN6    0x4   // A0 A1 A2 A3 D4 D5 D6
#define AN0_AN1_AN2_AN4_AN5_AN6        0x5   // A0 A1 A2 D4 D5 D6
#define AN0_AN1_AN4_AN5_AN6            0x6   // A0 A1 D4 D5 D6
#define AN4_AN5_AN6                    0x7   // D4 D5 D6
#define AN0_AN1_AN2_AN3_AN4_AN5        0x8   // A0 A1 A2 A3 D4 D5
#define AN0_AN1_AN2_AN4_AN5            0x9   // A0 A1 A2 D4 D5
#define AN0_AN1_AN4_AN5                0xa   // A0 A1 D4 D5
#define AN5_AN6                        0xb   // D4 D5
#define AN0_AN1_AN2_AN3                0xc   // A0 A1 A2 A3
#define AN0_AN1_AN2                    0xd   // A0 A1 A2
#define AN0_AN1                        0xe   // A0 A1
#define D_RA0_RA1_RA2_ANALOG           0x1   //!old only provided for compatibility
#define D_RA0_RA1_ANALOG               0x2   //!old only provided for compatibility
#define A_NO_ANALOGS                   0x3   //!old only provided for compatibility
#define A_RD4_RD5_RD6_ANALOG           0x4   //!old only provided for compatibility
#define RA0_RA1_RA2_RD4_RD5_RD6_ANALOG 0x5   //!old only provided for compatibility
#define RA0_RA1_RD4_RD5_RD6_ANALOG     0x6   //!old only provided for compatibility
#define RD4_RD5_RD6_ANALOGS            0x7   //!old only provided for compatibility
#define A_RD4_RD5_ANALOG               0x8   //!old only provided for compatibility
#define RA0_RA1_RA2_RD4_RD5_ANALOG     0x9   //!old only provided for compatibility
#define RA0_RA1_RD4_RD5_ANALOG         0xa   //!old only provided for compatibility
#define RD4_RD5_ANALOG                 0xb   //!old only provided for compatibility
#define D_NO_ANALOGS                   0xC   //!old only provided for compatibility
#define RA0_RA1_RA2_ANALOG             0xd   //!old only provided for compatibility
#define RA0_RA1_ANALOG                 0xe   //!old only provided for compatibility


// Constants used in READ_ADC() are:
#define ADC_START_AND_READ     7   // This is the default if nothing is specified
#define ADC_START_ONLY         1
#define ADC_READ_ONLY          6



////////////////////////////////////////////////////////////////// INT
// Interrupt Functions: ENABLE_INTERRUPTS(), DISABLE_INTERRUPTS(),
//                      EXT_INT_EDGE()
//
// Constants used in EXT_INT_EDGE() are:
#define L_TO_H              0x40
#define H_TO_L                 0
// Constants used in ENABLE/DISABLE_INTERRUPTS() are:
#define GLOBAL                    0x0BC0
#define INT_RTCC                  0x0B20
#define INT_RC                    0x8C04
#define INT_EXT                   0x0B10
#define INT_AD                    0x8C02
#define INT_ADOF                  0x8C01
#define INT_I2C                   0x8C08
#define INT_COMP                  0x8C80
#define INT_BUTTON                0x8C10
#define INT_TIMER0                0x0B20
#define INT_RA                    0x0B08

#list
