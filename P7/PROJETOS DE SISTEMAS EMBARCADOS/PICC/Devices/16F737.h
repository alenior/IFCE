//////// Standard Header file for the PIC16F737 device ////////////////
#device PIC16F737
#nolist
//////// Program memory: 4096x14  Data RAM: 367  Stack: 8
//////// I/O: 25   Analog Pins: 11
//////// C Scratch area: 77   ID Location: 2000
//////// Fuses: LP,XT,HS,EC_IO,NOWDT,WDT,NOPUT,PUT,PROTECT,NOPROTECT
//////// Fuses: NOBROWNOUT,BROWNOUT,INTRC_IO,INTRC,RC_IO,RC,BORV45,BORV42
//////// Fuses: BORV27,BORV20,MCLR,NOMCLR,CCP2C1,CCP2B3,DEBUG,NODEBUG,FCMEN
//////// Fuses: NOFCMEN,IESO,NOIESO,BORSEN,NOBORSEN
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
#define PIN_A4  44
#define PIN_A5  45
#define PIN_A6  46
#define PIN_A7  47

#define PIN_B0  48
#define PIN_B1  49
#define PIN_B2  50
#define PIN_B3  51
#define PIN_B4  52
#define PIN_B5  53
#define PIN_B6  54
#define PIN_B7  55

#define PIN_C0  56
#define PIN_C1  57
#define PIN_C2  58
#define PIN_C3  59
#define PIN_C4  60
#define PIN_C5  61
#define PIN_C6  62
#define PIN_C7  63

#define PIN_E3  75

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

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_INTERNAL         0x85
#define T1_EXTERNAL         0x87
#define T1_EXTERNAL_SYNC    0x83

#define T1_CLK_OUT          8

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6

////////////////////////////////////////////////////////////////// CCP
// CCP Functions: SETUP_CCPx, SET_PWMx_DUTY
// CCP Variables: CCP_x, CCP_x_LOW, CCP_x_HIGH
// Constants used for SETUP_CCPx() are:
#define CCP_OFF                         0
#define CCP_CAPTURE_FE                  4
#define CCP_CAPTURE_RE                  5
#define CCP_CAPTURE_DIV_4               6
#define CCP_CAPTURE_DIV_16              7
#define CCP_COMPARE_SET_ON_MATCH        8
#define CCP_COMPARE_CLR_ON_MATCH        9
#define CCP_COMPARE_INT                 0xA
#define CCP_COMPARE_RESET_TIMER         0xB
#define CCP_PWM                         0xC
#define CCP_PWM_PLUS_1                  0x1c
#define CCP_PWM_PLUS_2                  0x2c
#define CCP_PWM_PLUS_3                  0x3c
long CCP_1;
#byte   CCP_1    =                      0x15        
#byte   CCP_1_LOW=                      0x15        
#byte   CCP_1_HIGH=                     0x16        
long CCP_2;
#byte   CCP_2    =                      0x1B        
#byte   CCP_2_LOW=                      0x1B        
#byte   CCP_2_HIGH=                     0x1C        
long CCP_3;
#byte   CCP_3    =                      0x95        
#byte   CCP_3_LOW=                      0x95        
#byte   CCP_3_HIGH=                     0x96        
////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IN
// Constants used in SETUP_SSP() are:
#define SPI_MASTER       0x20
#define SPI_SLAVE        0x24
#define SPI_L_TO_H       0
#define SPI_H_TO_L       0x10
#define SPI_CLK_DIV_4    0
#define SPI_CLK_DIV_16   1
#define SPI_CLK_DIV_64   2
#define SPI_CLK_T2       3
#define SPI_SS_DISABLED  1

#define SPI_SAMPLE_AT_END 0x8000
#define SPI_XMIT_L_TO_H  0x4000

////////////////////////////////////////////////////////////////// UART
// Constants used in setup_uart() are:
// FALSE - Turn UART off
// TRUE  - Turn UART on
#define UART_ADDRESS 2
#define UART_DATA    4
// TRUE  - Turn UART on
////////////////////////////////////////////////////////////////// COMP
// Comparator Variables: C1OUT, C2OUT
// Constants used in setup_comparators() are:
#define A0_A3_A1_A3  0xfff04
#define A0_A3_A1_A2_OUT_ON_A4_A5  0xfcf03
#define A0_A3_A1_A3_OUT_ON_A4_A5  0xbcf05
#define NC_NC_NC_NC  0x0ff07
#define A0_A3_A1_A2  0xfff02
#define A0_A3_NC_NC_OUT_ON_A4  0x9ef01
#define A0_VR_A1_VR 0x3ff06
#define A3_VR_A2_VR 0xcff0e

#bit C1OUT = 0x9c.6      
#bit C2OUT = 0x9c.7      

////////////////////////////////////////////////////////////////// VREF
// Constants used in setup_vref() are:
//
#define VREF_LOW  0xa0
#define VREF_HIGH 0x80
// Or (with |) the above with a number 0-15
#define VREF_A2   0x40

////////////////////////////////////////////////////////////////// INTERNAL RC
// Constants used in setup_oscillator() are:
#define OSC_31KHZ   0
#define OSC_125KHZ  0x10
#define OSC_250KHZ  0x20
#define OSC_500KHZ  0x30
#define OSC_1MHZ    0x40
#define OSC_2MHZ    0x50
#define OSC_4MHZ    0x60
#define OSC_8MHZ    0x70
// The following may be OR'ed in with the above using |
#define OSC_TIMER1  1
#define OSC_INTRC   2
#define OSC_NORMAL  0
// A second optional parametter may be used with this part to fine
// tune the speed (signed int)
// Result may be (ignore all other bits)
#define OSC_STATE_STABLE 4


////////////////////////////////////////////////////////////////// EXT MEM
// External Memory Functions: SETUP_EXTERNAL_MEMORY(), READ_EXTERNAL_MEMORY(),
//                            WRITE_EXTERNAL_MEMORY()
//
// Constants used in SETUP_EXTERNAL_MEMORY() are:
#define EXTMEM_BYTE_WRITE  0x80
#define EXTMEM_BYTE_SELECT 0x81
#define EXTMEM_WORD_WRITE  0x82
#define EXTMEM_DISABLE     0
// Use one of the above and optionaly OR in one of:
#define EXTMEM_WAIT_0 0x30
#define EXTMEM_WAIT_1 0x20
#define EXTMEM_WAIT_2 0x10
#define EXTMEM_WAIT_3 0x00  // Default


////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC()
// Constants used for SETUP_ADC() are:
#define ADC_OFF                 0              // ADC Off
#define ADC_CLOCK_DIV_2 0x1000000
#define ADC_CLOCK_DIV_4    0x4000
#define ADC_CLOCK_DIV_8    0x0040
#define ADC_CLOCK_DIV_16   0x4040
#define ADC_CLOCK_DIV_32   0x0080
#define ADC_CLOCK_DIV_64   0x4080
#define ADC_CLOCK_INTERNAL 0x00c0              // Internal 2-6us
// The following may be OR'ed in with the above using |
#define ADC_TAD_MUL_0      0x000000
#define ADC_TAD_MUL_2      0x080000
#define ADC_TAD_MUL_4      0x100000
#define ADC_TAD_MUL_6      0x180000
#define ADC_TAD_MUL_8      0x200000
#define ADC_TAD_MUL_12     0x280000
#define ADC_TAD_MUL_16     0x300000
#define ADC_TAD_MUL_20     0x380000

// Constants used in SETUP_ADC_PORTS() are:
#define NO_ANALOGS   0x0F   // None
#define ALL_ANALOG   0x00   // A0 A1 A2 A3 A5 B2 B3 B1 B4 B0 B5           
#define AN0_TO_AN12  0x02   // A0 A1 A2 A3 A5 B2 B3 B1 B4 B0              
#define AN0_TO_AN11  0x03   // A0 A1 A2 A3 A5 B2 B3 B1 B4                
#define AN0_TO_AN10  0x04   // A0 A1 A2 A3 A5 B2 B3 B1                   
#define AN0_TO_AN9   0x05   // A0 A1 A2 A3 A5 B2 B3                      
#define AN0_TO_AN8   0x06   // A0 A1 A2 A3 A5 B2                         
#define AN0_TO_AN4   0x0A   // A0 A1 A2 A3 A5
#define AN0_TO_AN3   0x0B   // A0 A1 A2 A3
#define AN0_TO_AN2   0x0C   // A0 A1 A2
#define AN0_TO_AN1   0x0D   // A0 A1
#define AN0          0x0E   // A0
#define AN0_TO_AN12_ANALOG     0x02   //!old only provided for compatibility    
#define AN0_TO_AN11_ANALOG     0x03   //!old only provided for compatibility
#define AN0_TO_AN10_ANALOG     0x04   //!old only provided for compatibility
#define AN0_TO_AN9_ANALOG      0x05   //!old only provided for compatibility
#define AN0_TO_AN8_ANALOG      0x06   //!old only provided for compatibility
#define AN0_TO_AN4_ANALOG      0x0A   //!old only provided for compatibility
#define AN0_TO_AN3_ANALOG      0x0B   //!old only provided for compatibility
#define AN0_TO_AN2_ANALOG      0x0C   //!old only provided for compatibility
#define AN0_TO_AN1_ANALOG      0x0D   //!old only provided for compatibility
#define AN0_ANALOG             0x0E   //!old only provided for compatibility

// The following may be OR'ed in with the above using |
#define VSS_VDD               0x00              // Range 0-Vdd
#define VREF_VREF             0x30              // Range VrefL-VrefH
#define VREF_VDD              0x20              // Range VrefL-Vdd
#define VSS_VREF              0x10              // Range 0-VrefH


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
#define INT_RB                    0x0B08
#define INT_EXT                   0x0B10
#define INT_AD                    0x8C40
#define INT_TBE                   0x8C10
#define INT_RDA                   0x8C20
#define INT_TIMER1                0x8C01
#define INT_TIMER2                0x8C02
#define INT_CCP1                  0x8C04
#define INT_CCP2                  0x8D01
#define INT_SSP                   0x8C08
#define INT_BUSCOL                0x8D08
#define INT_TIMER0                0x0B20
#define INT_COMP                  0x8D40
#define INT_OSC_FAIL              0x8D80
#define INT_LOWVOLT               0x8D20
#define INT_CCP3                  0x8D02

#list
