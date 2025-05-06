////////////////////////////////////////////////////////////////////////////
////                                                                    ////
////                        devkit_picrobot.h                           ////
////                                                                    ////
//// Header file containing relevant definitions for the CCS Robot      ////
//// development kit (with 24FJ128GB606).  Will compile the program     ////
//// compatible with the USB bootloader.  Bootloader will be disabled   ////
//// if you use an ICD for debugging.  Will #import the bootloader      ////
//// HEX so you can use an ICD to reprogram a unit to put the           ////
//// bootloader back on the device.                                     ////
////                                                                    ////
//// Bootloader uses USB CDC.  To use bootloader, connect USB           ////
//// cable to USB port.  Then, while holding down the push-button,      ////
//// power cycle the unit.  Open the bootloader COM port in SIOW        ////
//// or a serial terminal program, and you should then see the          ////
//// following string:                                                  ////
////     Bootloader Ready                                               ////
//// Then in SIOW, goto 'Download Software' from the File menu          ////
//// and choose the HEX file you want to program onto the unit.         ////
////                                                                    ////
//// #defining DEVICE_USE_ICD before including this file will add the   ////
//// #device ICD=TRUE line needed to compile with ICD debugging         ////
//// support.                                                           ////
////                                                                    ////
//// #defining __NO_LOADER before including this file will prevent the  ////
//// bootloader from being used in the built application.  Normally     ////
//// you will not want to define this.                                  ////
////                                                                    ////
//// PIN_B0 is connected to ICD ICSP PGD, PIN_B1 is connected to PGC.   ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////                                                                    ////
//// CONFIGURATION OPTIONS                                              ////
////                                                                    ////
//// CONFIG_UART_CONSOLE_FROM_USB_CDC - if this is defined before       ////
//// including this file then it will include the usb_cdc.h drivers     ////
//// to provide USB CDC support, and will map several macros related    ////
//// to console (serial terminal) support to USB CDC:                   ////
////     UARTConsolePutc(c) - Send char c.                              ////
////     UARTConsolePuts(*s) - Sends string s.                          ////
////     UARTConsolePutsROM(*s) - Sends string s.                       ////
////     UARTConsoleInit(baud) - Initialize USB.  'baud' is provided    ////
////        for backwards compatiblity with code using a serial         ////
////        peripheral and is not used by the USB CDC library.          ////
////     UARTConsoleTask() - Handle any background USB tasks, must be   ////
////        called periodically.  If you have a main loop, put it       ////
////        there.                                                      ////
////     UARTConsoleKbhit() - Returns TRUE if there is data to receive  ////
////        from console.                                               ////
////     c = UARTConsoleGetc() - Returns data from console, will wait   ////
////        until data is available.                                    ////
//// If CONFIG_UART_CONSOLE_FROM_USB_CDC is not defined, then these     ////
//// macros will be mapped to empty/harmless macros.                    ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////                                                                    ////
//// APIs AND MACROS                                                    ////
////                                                                    ////
//// The following definitions are provided that can be passed to       ////
//// set_adc_channel() for reading the analog sensors on the board:     ////
////     AN_CHANNEL_LEFT_LINE                                           ////
////     AN_CHANNEL_MIDDLE_LINE                                         ////
////     AN_CHANNEL_RIGHT_LINE                                          ////
////     AN_CHANNEL_LEFT_PROXIMITY                                      ////
////     AN_CHANNEL_RIGHT_PROXIMITY                                     ////
////     AN_CHANNEL_BATTERY                                             ////
////                                                                    ////
//// EZ App Lynx is already configured to be used with an EZ App Lynx   ////
//// board connected to the daughterbaord connector.  To use this, just ////
//// include the following files into your program:                     ////
////     #include <RN4020_MLDP_Bluetooth.h>                             ////
////     #include <bluetooth_serial.c>                                  ////
////     #include <EZApp.c>                                             ////
////                                                                    ////
//// PIN_BUTTON and BUTTON_PRESSED() are available for reading the      ////
//// button status.                                                     ////
////                                                                    ////
//// MAG3110 is already configured for use by this file, just #include  ////
//// <mag3110.c> for use of this device.                                ////
////                                                                    ////
//// WheelMotorInit() - Initializes the wheel motor speed control       ////
////  functionality.  Must be called once.                              ////
////                                                                    ////
//// WheelMotorSetSpeed(pwmIdx, speed) - Sets the speed of 'pwmIdx'     ////
////  DC motor to 'speed'.  If speed is 0, then this motor is stopped.  ////
////  Maximum forward value is 100, maximum reverse direction is -100.  ////
////  Due to nature of DC motor, a speed value that is too low may      ////
////  not actually cause the motor to move.  Set 'pwmIdx' to            ////
////  WHEEL_MOTOR_IDX_RIGHT to control right wheel,                     ////
////  WHEEL_MOTOR_IDX_LEFT to control left wheel.                       ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////                                                                    ////
//// PERIPHERALS USED                                                   ////
////                                                                    ////
//// OC1, OC2 and TIMER2 are used for the PWMs driving the wheel DC     ////
//// motors.  TIMER2 could be reused as long as the period, postcalar   ////
//// and value isn't changed.                                           ////
////                                                                    ////
//// OC3 and TIMER3 are used for the audio/speaker PWM.                 ////
////                                                                    ////
//// UART1 is wired to the EZ App Lynx connector.                       ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2017 Custom Computer Services            ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler.  This source code may only be distributed to other       ////
//// licensed users of the CCS C compiler.  No other use, reproduction  ////
//// or distribution is permitted without written permission.           ////
//// Derivative programs created using this software in object code     ////
//// form are not restricted in any way.                                ////
////                     http://www.ccsinfo.com                         ////
////////////////////////////////////////////////////////////////////////////

#ifndef __DEVKIT_PICROBOT_H__
#define __DEVKIT_PICROBOT_H__

#include <24fj128gb606.h>

#device ICSP=1

#device ADC=10

#if defined(DEVICE_USE_ICD)
   #device ICD=TRUE
#endif

#if defined(__XC16_COMPAT__)
   #device PSV=16
   
   #if defined(__CONST_IS_READONLY__)
      #device CONST=READ_ONLY
   #else
      //this is normally how C30 operates
      #device CONST=ROM
   #endif
   
   #device PASS_STRINGS=IN_RAM
   
   #type signed
   #type short=16 int=16 long=32
   
   #zero_ram
   #zero_local_ram
   
   #if defined(TRUE)
   #undef TRUE
   #endif
   
   #if defined(FALSE)
   #undef FALSE
   #endif
   
   #if defined(BYTE)
   #undef BYTE
   #endif
   
   #if defined(BOOLEAN)
   #undef BOOLEAN
   #endif
#endif

// Configuration bits.  These cannot be changed by the bootloader.
#fuses NOWRTB              //Boot block not write protected
#fuses NOBSS               //No boot segment
#fuses NOBS                //No boot segment
#fuses NOWRT               //Program memory not write protected
#fuses NOPROTECT           //Code not protected from reading
#fuses WPCFG               //Configuration Words page is erase/write-protected
#fuses NOPROTECT_CFG       //No Configuration Segment Code Protection
#fuses NOAIVT              //Disable Alternate Interrupt Vector Table
#fuses SOSC_DIG            //Digital mode, I/O port functionality of SOSCI and SOSCO pins
#fuses NOIOL1WAY           //Allows multiple reconfigurations of peripheral pins
#fuses WPOSTS13            //Watch Dog Timer PostScalar 1:4096
#fuses WDT32               //Watch Dog Timer uses 1:32 Postscale
#fuses WDT_SW              //No Watch Dog Timer, enabled in Software
#fuses WINDIS              //Watch Dog Timer in non-Window mode
#fuses WDTCMX              //WDT clock source is determined by the WDTCLK configuration fuses
#fuses WDTCLK_31KHZ        //WDT uses 31kHz FRC as clock source
#fuses BROWNOUT_SW         //Brownout controlled by configuration bit in special file register
#fuses NOLVR               //Low Voltage Regulator Disabled
#fuses NODNVP              //Downside protection disabled when BOR is inactive
#fuses NOJTAG              //JTAG disabled
#fuses NOBTSWP             //BOOTSWP Instruction Disabled
#fuses NOALTCMPI           //Comparator inputs are on their default pin locations
#fuses NOTMPRPIN           //Tamper Pin Disabled
#fuses SOSC_LOW            //Low-power SOSC circuit is selected
#fuses VREFNORM_CVREFNORM  //VREF and CVREF are mapped to their default pins
#fuses FRC_PLL             //Internal Fast RC oscillator with PLL
#fuses PLL2                //Divide By 2(8MHz oscillator input)
#fuses IESO                //Internal External Switch Over mode enabled
#fuses NOPR                //Pimary oscillaotr disabled
#fuses OSCIO               //OSC2 is general purpose output
#fuses PLL_FROM_PRIMARY    //PLL is fed by the Primary oscillator

#if defined(__NO_LOADER) || getenv("ICD") || defined(_bootloader)
   #use delay(int=8MHz, clock=32MHz, USB_FULL, act=USB)  //This will cause main() to init the clock.
#else
   #use delay(clock=32MHz)    //Bootloader already initialized the clock before app started.
#endif

// macros and mappings for making compiler output compatible with bootloader.
#define PROGRAM_LOADER_SIZE   0x2000
#define PROGRAM_LOADER_START  0
#define PROGRAM_LOADER_END    (PROGRAM_LOADER_SIZE-1)
#define APPLICATION_START     (PROGRAM_LOADER_SIZE+0)
#define APPLICATION_ISR       (PROGRAM_LOADER_SIZE+8)
#define APPLICATION_END       (getenv("PROGRAM_MEMORY")-1)
#if !defined(__NO_LOADER) && !getenv("ICD") && !defined(_bootloader)
   #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR)
   
   #org 0, PROGRAM_LOADER_END {}
   
   // last page is erase protected by bootloader to prevent accidentally 
   // erasing config words.  therefore we must prevent the app from using this
   // space.
   #org 0x15800, getenv("PROGRAM_MEMORY")-1 {}
   
   #ignore_warnings 228
   #import(HEX, file="bootloader_picrobot.hex", RANGE=PROGRAM_LOADER_START:PROGRAM_LOADER_END)
   #ignore_warnings NONE
#endif
#if defined(_bootloader)
   #org APPLICATION_START+0x20,APPLICATION_END {}
   
   #org APPLICATION_START, APPLICATION_START+0x1f
   void _bootloader_blank_application(void)
   {
      for(;;)
      {
      }
   }
   
   #int_default
   void _bootloader_isr_redirect(void)
   {
      jump_to_isr(APPLICATION_ISR);
   }
#endif

// Motor macros and helper functions
#if 0
   // used on maker faire prototypes
   #define PIN_LEFT_MOTOR_ENABLE    PIN_D9   //rp4
   #define PIN_LEFT_MOTOR_REVERSE   PIN_D10  //rp3
   #define PIN_LEFT_MOTOR_FORWARD   PIN_D11  //rp12
   #define PIN_RIGHT_MOTOR_ENABLE   PIN_D8   //rp2
   #define PIN_RIGHT_MOTOR_REVERSE  PIN_E4
   #define PIN_RIGHT_MOTOR_FORWARD  PIN_E5
#else
   // rev 1
   #define PIN_RIGHT_MOTOR_ENABLE    PIN_D10  //rp3
   #define PIN_RIGHT_MOTOR_REVERSE   PIN_E0
   #define PIN_RIGHT_MOTOR_FORWARD   PIN_E1
   #define PIN_LEFT_MOTOR_ENABLE   PIN_D11  //rp12
   #define PIN_LEFT_MOTOR_REVERSE  PIN_E4
   #define PIN_LEFT_MOTOR_FORWARD  PIN_E5
#endif

#ifndef _bootloader
#pin_select OC1=PIN_RIGHT_MOTOR_ENABLE    // should match same order as wheel_motor_idx_t
#pin_select OC2=PIN_LEFT_MOTOR_ENABLE     // should match same order as wheel_motor_idx_t
typedef enum
{
   WHEEL_MOTOR_IDX_RIGHT = 0,
   WHEEL_MOTOR_IDX_LEFT = 1,
   
   WHEEL_MOTOR_NUM
} wheel_motor_idx_t;
signed int8 _g_WheelMotorLastDuty[WHEEL_MOTOR_NUM];
static void _WheelMotorSetupCompare(wheel_motor_idx_t idx, int1 on)
{
   #define _WHEEL_MOTOR_COMPARE_OFF (COMPARE_OFF|COMPARE_TRIG_SYNC_TIMER2)
   #define _WHEEL_MOTOR_COMPARE_ON (COMPARE_PWM_EDGE|COMPARE_TIMER2|COMPARE_TRIG_SYNC_TIMER2)
   
   if (on)
   {
      if (idx == WHEEL_MOTOR_IDX_RIGHT)
      {
         setup_compare(1, _WHEEL_MOTOR_COMPARE_ON);
         //OC1CON1 =  _WHEEL_MOTOR_COMPARE_ON;  //oc1con1
         //OC1CON2 = (_WHEEL_MOTOR_COMPARE_ON >> 16); //oc1con2
      }
      else
      {
         setup_compare(2, _WHEEL_MOTOR_COMPARE_ON);
         //OC2CON1 =  _WHEEL_MOTOR_COMPARE_ON;  //oc2con1
         //OC2CON2 = (_WHEEL_MOTOR_COMPARE_ON >> 16); //oc2con2
      }
   }
   else
   {   
      if (idx == WHEEL_MOTOR_IDX_RIGHT)
      {
         setup_compare(1, _WHEEL_MOTOR_COMPARE_OFF);
         //OC1CON1 =  _WHEEL_MOTOR_COMPARE_OFF;  //oc1con1
         //OC1CON2 = (_WHEEL_MOTOR_COMPARE_OFF >> 16); //oc1con2
      }
      else
      {
         setup_compare(2, _WHEEL_MOTOR_COMPARE_OFF);
         //OC2CON1 =  _WHEEL_MOTOR_COMPARE_OFF;  //oc2con1
         //OC2CON2 = (_WHEEL_MOTOR_COMPARE_OFF >> 16); //oc2con2
      }      
   }
}
void _WheelMotorSetDuty(wheel_motor_idx_t idx, unsigned int16 period)
{
   if (idx == WHEEL_MOTOR_IDX_RIGHT)
   {
      set_pwm_duty(1, period);
      //OC1R = period;
   }
   else
   {
      set_pwm_duty(2, period);
      //OC2R = period;
   }
}
// duty can range from 1:100 for forward direction,
//                    -1:-100 for reverse direction,
//                    or 0 for stop.
void WheelMotorSetSpeed(wheel_motor_idx_t pwmIdx, signed int8 duty)
{
   unsigned int8 absDuty;
   int1 pwmIsOn;
   
   if 
   (
      ((duty > 0) && (_g_WheelMotorLastDuty[pwmIdx] <= 0)) ||
      ((duty < 0) && (_g_WheelMotorLastDuty[pwmIdx] >= 0))
   )
   {
      // reverse the direction
      
      if (pwmIdx == WHEEL_MOTOR_IDX_RIGHT)
      {
         output_low(PIN_RIGHT_MOTOR_REVERSE);
         output_low(PIN_RIGHT_MOTOR_FORWARD);
      }
      else
      {
         output_low(PIN_LEFT_MOTOR_REVERSE);
         output_low(PIN_LEFT_MOTOR_FORWARD);
      }
      
      delay_ms(1);
      
      if (pwmIdx == WHEEL_MOTOR_IDX_RIGHT)
      {
         if (duty > 0)
         {
            output_high(PIN_RIGHT_MOTOR_FORWARD);
         }
         else
         {
            output_high(PIN_RIGHT_MOTOR_REVERSE);
         }
      }
      else
      {
         if (duty > 0)
         {
            output_high(PIN_LEFT_MOTOR_FORWARD);
         }
         else
         {
            output_high(PIN_LEFT_MOTOR_REVERSE);
         }
      }
   }
   
   absDuty = abs(duty);
   
   pwmIsOn = ((_g_WheelMotorLastDuty[pwmIdx] != 0) && (abs(_g_WheelMotorLastDuty[pwmIdx]) != 100));
   
   if (absDuty == 0)
   {
      if (pwmIsOn)
      {
         _WheelMotorSetupCompare(pwmIdx, 0);
      }
      
      if (pwmIdx == WHEEL_MOTOR_IDX_RIGHT)
      {
         output_low(PIN_RIGHT_MOTOR_ENABLE);
      }
      else
      {
         output_low(PIN_LEFT_MOTOR_ENABLE);
      }
   }
   else if (absDuty == 100)
   {
      if (pwmIsOn)
      {
         _WheelMotorSetupCompare(pwmIdx, 0);
      }
      
      if (pwmIdx == WHEEL_MOTOR_IDX_RIGHT)
      {
         output_high(PIN_RIGHT_MOTOR_ENABLE);
      }
      else
      {
         output_high(PIN_LEFT_MOTOR_ENABLE);
      }
   }
   else
   {
      if (!pwmIsOn)
      {
         _WheelMotorSetupCompare(pwmIdx, 1);
      }
      
      _WheelMotorSetDuty(pwmIdx, absDuty);
   }
   
   _g_WheelMotorLastDuty[pwmIdx] = duty;
}
void WheelMotorInit(void)
{
   int i;
   
   for(i=0; i<WHEEL_MOTOR_NUM; i++)
   {
      _g_WheelMotorLastDuty[i] = -1;
      WheelMotorSetSpeed(i, 0);
   }
   
   setup_timer2(TMR_INTERNAL | TMR_DIV_BY_64, 100);
}
#endif   //_bootloader

// Button macros.
#define PIN_BUTTON   PIN_G9
#define BUTTON_PRESSED()  (!input(PIN_BUTTON))

// EZ App Lynx configuration.
#define PIN_BT_MLDP_CONNECTED  PIN_D0
#define PIN_BT_MLDP_CMD_MODE   PIN_D1
#define PIN_BT_SWAKE           PIN_F5  //wake_sw, pin#7
#define PIN_BT_RTS             PIN_F4  //rts, pin#18
#define PIN_BT_CTS             PIN_D2  //cts, pin#14
#ifndef _bootloader
#pin_select U1RX=PIN_G7
#pin_select U1TX=PIN_G8
#endif
#define STREAM_BT_SPP_UART 1
#define STREAM_BT_SPP_BUFFER  32

// USB connection sense macros (read Vbus on RF7).
#bit U1OTGSTAT_SESVD=getenv("BIT:SESVD")
#define USB_CABLE_IS_ATTACHED() (U1OTGSTAT_SESVD)

// Macros for reading the line and proximity analog sensors, these can
// be passed to set_adc_channel().
#define AN_CHANNEL_LEFT_LINE        2     //PIN_B2
#define AN_CHANNEL_MIDDLE_LINE      3     //PIN_B3
#define AN_CHANNEL_RIGHT_LINE       4     //PIN_B4
#define AN_CHANNEL_LEFT_PROXIMITY   6     //PIN_B6
#define AN_CHANNEL_RIGHT_PROXIMITY  5     //PIN_B5
#define AN_CHANNEL_BATTERY          7     //PIN_B7

// Configuration macros for using MAG3110 driver.
#define PIN_MAG3110_SCL PIN_E7
#define PIN_MAG3110_SDA PIN_E6

#ifndef _bootloader
#define PIN_AUDIO PIN_D4
#pin_select OC3=PIN_AUDIO
#use pwm(OC3, timer=3, bits=6, PWM_OFF, stream=STREAM_OC_AUDIO)
// initialize audio DAC (an RC filter out to an amp) by enabling PWM. needs to be called once
#define AUDIO_INIT() pwm_on(STREAM_OC_AUDIO)
// turn off audio DAC by disabling PWM.
#define AUDIO_OFF() pwm_off(STREAM_OC_AUDIO)
// set amplitude of DAC by setting PWM duty, range is 0-255 (due to resolution of DAC there may not be 256 steps, but the API scales 0-255).
#define AUDIO_SET(_val) pwm_set_duty(STREAM_OC_AUDIO, _val/4)
#endif   //_bootloader

// Map UART console functions to USB CDC (virtual COM port).
#if defined(CONFIG_UART_CONSOLE_FROM_USB_CDC)
   #include <usb_cdc.h>
   void UARTConsolePutc(char _c) {if (usb_enumerated() && usb_attached() && usb_cdc_connected()) {usb_cdc_putc(_c);}}
   void UARTConsolePuts(char *s)  {printf(UARTConsolePutc, s);}
   void UARTConsolePutsROM(rom char *s)   {printf(UARTConsolePutc, s);}
   #define UARTConsoleInit(_baud) usb_init_cs()
   #define UARTConsoleTask() usb_task()
   #define UARTConsoleKbhit() usb_cdc_kbhit()
   #define UARTConsoleGetc() usb_cdc_getc()
#else
   void UARTConsolePutc(char _c) {}
   #define UARTConsolePuts(_s)
   #define UARTConsolePutsROM(_s)
   #define UARTConsoleInit(_baud)
   #define UARTConsoleTask()
   #define UARTConsoleKbhit() (0)
   #define UARTConsoleGetc() (0)
#endif

/*
   If USE_RS232_USB_CDC is defined, then load the CCS USB CDC driver to create
   a virtual COM port over USB.  The USB CDC driver is then mapped to the C
   standard IO (printf, getc, kbhit, putc, etc).  If this is not defined then
   you will need to use #use rs232(ICD) or #use rs232(DEBUGGER) to map
   C standard IO over a CCS ICD (requires a CCS ICD tool).  Defining this
   option will be useful if you don't have a CCS ICD, as firmware for the
   robot devkit does support updating using a USB bootloader without the ICD.
   
   If USE_RS232_USB_CDC_WAIT_FOR_CONNECTION is defined, then HW_INIT()
   will wait until USB is connected to a PC and the PC has opened the
   USB CDC COM port.  This is useful if you want to see any messages displayed
   when the PIC powers up.  You may not want this feature if you the firmware
   for the robot isn't displaying any useful data over serial or the firmware
   is meant to run without being connected to a PC.
*/
#ifdef USE_RS232_USB_CDC
   #include <usb_cdc.h>
   void my_usb_cdc_putc(char c)
   {
      static int1 timeout;
      int ms;
      
      if (usb_enumerated() && !usb_cdc_putready() && !timeout)
      {
         // wait 200ms for USB to be ready.
         // if 200ms expires, stop waiting until a character
         // is succesfully sent.
         ms = 0;
         while (!usb_cdc_putready() && (ms++ < 200))
         {
            delay_ms(1);
            usb_task();
         }
         if (!usb_cdc_putready())
            timeout = 1;
      }
      if (usb_enumerated() && usb_cdc_putready())
      {
         timeout = 0;
         usb_cdc_putc(c);
      }
   }
   
   char my_usb_cdc_getc(void)
   {
      while (!usb_enumerated() || !usb_cdc_kbhit())
      {
         usb_task();
      }
      return usb_cdc_getc();
   }
   
   #use rs232(call_putc=my_usb_cdc_putc, call_getc=my_usb_cdc_getc, call_kbhit=usb_cdc_kbhit, stream=STREAM_USB_CDC)
   
   #define USE_RS232_USB_CDC_INIT() usb_init_cs()
   #define USE_RS232_USB_CDC_TASK() usb_task()
   
   #ifdef USE_RS232_USB_CDC_WAIT_FOR_CONNECTION
      void USE_RS232_USB_CDC_WAIT(void)
      {
         while(!usb_enumerated() && !usb_cdc_connected())
         {
            usb_task();
         }
      }
   #else
      #define USE_RS232_USB_CDC_WAIT()
   #endif
#else
   #define USE_RS232_USB_CDC_INIT()
   #define USE_RS232_USB_CDC_TASK()
   #define USE_RS232_USB_CDC_WAIT()
#endif

// Bare minimum functions that should be called on power-up to setup the
// hardware GPIO.
#define HW_INIT() \
   setup_wdt(WDT_OFF); \
   setup_adc_ports(sAN2 | sAN3 | sAN4 | sAN5 | sAN6 | sAN7);  \
   setup_comparator(0, NC_NC);   \
   setup_comparator(1, NC_NC);   \
   setup_comparator(2, NC_NC);   \
   setup_comparator(3, NC_NC);   \
   setup_adc(ADC_CLOCK_INTERNAL);   \
   output_low(PIN_LEFT_MOTOR_ENABLE);  \
   output_low(PIN_RIGHT_MOTOR_ENABLE); \
   output_low(PIN_LEFT_MOTOR_FORWARD); \
   output_low(PIN_RIGHT_MOTOR_FORWARD);   \
   output_low(PIN_LEFT_MOTOR_REVERSE); \
   output_low(PIN_RIGHT_MOTOR_REVERSE);   \
   output_low(PIN_BT_RTS); \
   USE_RS232_USB_CDC_INIT();  \
   USE_RS232_USB_CDC_WAIT()

// Any background tasks needed by the hardware.
#define HW_TASK() USE_RS232_USB_CDC_TASK()

#endif //__DEVKIT_PICROBOT_H__
