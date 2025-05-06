///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                            stepper.c                              ////
////                                                                   ////
//// Library for controlling a stepper motor.  Works with with         ////
//// 4wire/5wire/6wire stepper motors (MCU directly drives A and B     ////
//// coils of stepper), or 2wire/3wire stepper drivers (MCU clocks a   ////
//// step to a driver, driver then energizes A and B coils of          ////
//// stepper in proper order).                                         ////
////                                                                   ////
//// CCS's tick.c library is required to use this library.  If tick.c  ////
//// is not being used, then this library will include it.             ////
////                                                                   ////
////                                                                   ////
//// CONFIG                                                            ////
//// ---------------------------------------------------------------   ////
////                                                                   ////
//// Some configuration macros are required by this library to control ////
//// the motor or the stepper driver.  The library will determine      ////
//// proper code for 4/5/6 wire or 2/3 wire control based on the       ////
//// macros provided.                                                  ////
////                                                                   ////
//// PIN_STEPPER_A_PLUS                                                ////
//// PIN_STEPPER_A_MINUS                                               ////
//// PIN_STEPPER_B_PLUS                                                ////
//// PIN_STEPPER_B_MINUS                                               ////
////   This is for a 4/5/6 wire stepper motor.                         ////
////   These pins control the 4 windings of the stepper.  All 4 of     ////
////   these pins must be defined.                                     ////
////   Unless PINS_STEPPER_REVERSED is defined, then setting this      ////
////   pin high energizes this coil.                                   ////
////     Example: #define PIN_STEPPER_A_PLUS PIN_D0                    ////
////                                                                   ////
//// STEPPER_ONE_PHASE                                                 ////
//// STEPPER_TWO_PHASE                                                 ////
//// STEPPER_HALF_STEPS                                                ////
////   This is used with 4/5/6 wire stepper motor.                     ////
////   #define one of these before including this library to choose    ////
////   this config option.  If no choice is made then                  ////
////   STEPPER_TWO_PHASE is used.                                      ////
////   STEPPER_ONE_PHASE only drives one coil (A or B) at once,        ////
////   this uses the least amount of power but provides the least      ////
////   amount of torque.  STEPPER_TWO_PHASE drives both coils          ////
////   (A and B) at the same time, this provides the most torque       ////
////   but uses twice as much power as STEPPER_ONE_PHASE.              ////
////   STEPPER_HALF_STEPS increases movement accuracy by adding        ////
////   twice as many steps by alternating between driving both         ////
////   A and B or only driving one A or B.  This uses power            ////
////   inbetween STEPPER_ONE_PHASE and STEPPER_TWO_PHASE and           ////
////   provides torque between STEPPER_ONE_PHASE and                   ////
////   STEPPER_TWO_PHASE, but increases movement accuracy by           ////
////   a factor of two (twice as many steps).                          ////
////                                                                   ////
//// PIN_STEPPER_ENABLE                                                ////
//// PIN_STEPPER_CLOCK                                                 ////
//// PIN_STEPPER_DIR                                                   ////
////   This is for a 2/3 wire stepper driver.                          ////
////   These pins control the stepper driver.  PIN_STEPPER_ENABLE is   ////
////   optional and does not need to be defined.  Sometimes the        ////
////   stepper driver's clock pin is labeled pulse.  Setting           ////
////   enable high enables the stepper driver, this logic can          ////
////   be inverted with PINS_STEPPER_ENABLE_REVERSED.                  ////
////   Pulsing clock pin high causes stepper motor to advance to next  ////
////   step (if PINS_STEPPER_REVERSED is defined then it will invert   ////
////   this logic).                                                    ////
////                                                                   ////
//// PINS_STEPPER_REVERSED                                             ////
////   This is optional.  If #defined, then the logic for controlling  ////
////    the PIN_xxx described above is inverted.                       ////
////                                                                   ////
//// PINS_STEPPER_ENABLE_REVERSED                                      ////
////   Reverse the logic of the PIN_STEPPER_ENABLE pin.                ////
////                                                                   ////
////                                                                   ////
//// API                                                               ////
//// ---------------------------------------------------------------   ////
//// void StepperInit(void);                                           ////
////     Initialize the GPIO and stepper library.                      ////
////     Motor will be inactive (not energized) and not applying       ////
////     force.                                                        ////
////     Sets current position to 0.                                   ////
////                                                                   ////
//// void StepperTask(void);                                           ////
////     Handle any background tasks related to moving the stepper     ////
////     motor.  Must be called often, generally in your main() loop.  ////
////                                                                   ////       
//// void StepperStop(void);                                           ////
////     Stop any movement, motor will remain energized and hold       ////
////     current position.                                             ////
////                                                                   ////
//// void StepperRelease(void);                                        ////
////     Stop any movement and de-energize the motor (no force will    ////
////     be applied by the motor).  Any torque on the motor will       ////
////     cause it to move and position will become corrupt.            ////
////                                                                   ////
//// void StepperSetSpeed(TICK speed);                                 ////
////     Set the speed, this is the TICK rate at which it will move    ////
////     from one step to the next.                                    ////
////                                                                   ////
//// void StepperGoto(stepper_pos_t dest);                             ////
////     Move the stepper motor to this absolute position.             ////
////     MotorInit() sets position to 0.   'dest' can be signed.       ////
////                                                                   ////
//// void StepperMove(stepper_pos_t delta);                            ////
////     Move the stepper motor to this relative position (relative    ////
////     from our current position).  'delta' can be signed.           ////
////                                                                   ////
//// stepper_pos_t StepperGetPos(void);                                ////
////     Get the current position of the stepper motor.                ////
////     StepperInit() sets the position to 0.                         ////
////     This value cound be off by one as the library increments the  ////
////     position when it steps the motor but doesn't wait until the   ////
////     stepper has reached that step (wait until StepperIsMoving()   ////
////     returns FALSE for a valid reading to prevent this).           ////
////                                                                   ////
//// void StepperSetPos(stepper_pos_t pos);                            ////
////     Sets the position of the motor.  This does not move the       ////
////     motor, only sets the position indicator of the library.       ////
////     StepperInit() sets the position to 0.                         ////
////                                                                   ////
//// int1 StepperIsMoving();                                           ////
////     Returns TRUE if the stepper motor is moving.                  ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#ifndef __STEPPER_C__
#define __STEPPER_C__

#include <stdint.h>

#include <tick.c>

#ifndef stepper_pos_t
#define stepper_pos_t int32_t
#endif

#if defined(PINS_STEPPER_REVERSED)
   #define _stepper_output_high(p)      output_low(p)
   #define _stepper_output_low(p)      output_high(p)
#else
   #define _stepper_output_high(p)      output_high(p)
   #define _stepper_output_low(p)      output_low(p)
#endif

#if defined(PIN_STEPPER_ENABLE)
   #if defined(PINS_STEPPER_ENABLE_REVERSED)
      #define _stepper_enable_high()      output_low(PIN_STEPPER_ENABLE)
      #define _stepper_enable_low()       output_high(PIN_STEPPER_ENABLE)
   #else
      #define _stepper_enable_high()      output_high(PIN_STEPPER_ENABLE)
      #define _stepper_enable_low()       output_low(PIN_STEPPER_ENABLE)
   #endif
#else
   #define _stepper_enable_high()
   #define _stepper_enable_low()
#endif

struct
{
   stepper_pos_t pos;
   stepper_pos_t dest;
   #if defined(PIN_STEPPER_A_PLUS)
   uint8_t step;
   #endif
   TICK t;
   TICK speed;
   #if defined(PIN_STEPPER_CLOCK)
   int1 step;
   #endif
   int1 delayed;
} _g_Stepper;

#if defined(PIN_STEPPER_A_PLUS)
   #if !defined(STEPPER_ONE_PHASE) && !defined(STEPPER_TWO_PHASE) && !defined(STEPPER_HALF_STEPS)
      #define STEPPER_TWO_PHASE
   #endif

   static void _StepperStop() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
   }
   
   #if defined(STEPPER_TWO_PHASE)
   // 'two phase full step'
   // more torque, but twice the power (current) of 'one phase full step' mode.
   static void _StepperStep1() 
   {
      _stepper_output_high(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_high(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
   }

   static void _StepperStep2() 
   {
     _stepper_output_low(PIN_STEPPER_A_PLUS);
     _stepper_output_high(PIN_STEPPER_A_MINUS);
     _stepper_output_high(PIN_STEPPER_B_PLUS);
     _stepper_output_low(PIN_STEPPER_B_MINUS);
     
   }

   static void _StepperStep3() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_high(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_high(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep4() 
   {
      _stepper_output_high(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_high(PIN_STEPPER_B_MINUS);
      
   }
   #elif defined(STEPPER_ONE_PHASE)
   // 'one phase full step'
   // less torque, but half the power (current) of 'two phase full step' mode.
   static void _StepperStep1() 
   {
      _stepper_output_high(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep2() 
   {
     _stepper_output_low(PIN_STEPPER_A_PLUS);
     _stepper_output_low(PIN_STEPPER_A_MINUS);
     _stepper_output_high(PIN_STEPPER_B_PLUS);
     _stepper_output_low(PIN_STEPPER_B_MINUS);
     
   }

   static void _StepperStep3() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_high(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep4() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_high(PIN_STEPPER_B_MINUS);
      
   }
   #else //STEPPER_HALF_STEPS
   // 'half step'
   // more torque than 'one phase full step', but less torque than 
   //    'two phase full step'.
   // more power (current) than 'one phase full step', but less power than
   //    'two phase full step'.
   // smoother movement compared to 'one phase full step' or 
   //    'two phase full step'.
   static void _StepperStep1() 
   {
      _stepper_output_high(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep2() 
   {
      _stepper_output_high(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_high(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep3() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_high(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
       
   }

   static void _StepperStep4() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_high(PIN_STEPPER_A_MINUS);
      _stepper_output_high(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep5() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_high(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_low(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep6() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_high(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_high(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep7() 
   {
      _stepper_output_low(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_high(PIN_STEPPER_B_MINUS);
      
   }

   static void _StepperStep8() 
   {
      _stepper_output_high(PIN_STEPPER_A_PLUS);
      _stepper_output_low(PIN_STEPPER_A_MINUS);
      _stepper_output_low(PIN_STEPPER_B_PLUS);
      _stepper_output_high(PIN_STEPPER_B_MINUS);
   }
   #endif

static void _StepperGotoStep(void)
{
   switch(_g_Stepper.step)
   {
      default:
      case 0:   _StepperStep1();   break;
      case 1: _StepperStep2();   break;
      case 2:   _StepperStep3();   break;
      case 3: _StepperStep4();   break;
      #if defined(STEPPER_HALF_STEPS)
      case 4:   _StepperStep5();   break;
      case 5: _StepperStep6();   break;
      case 6:   _StepperStep7();   break;
      case 7: _StepperStep8();   break;
      #endif
   }
   
   _g_Stepper.delayed = TRUE;
   _g_Stepper.t = TickGet();
}

static void _StepperNextStep(void)
{
   _g_Stepper.pos++;
   
   #if defined(STEPPER_HALF_STEPS)
   if (++_g_Stepper.step == 8)
   #else
   if (++_g_Stepper.step == 4)
   #endif
   {
      _g_Stepper.step = 0;
   }
   
   _StepperGotoStep();
}

static void _StepperPrevStep(void)
{
   _g_Stepper.pos--;
   
   if (--_g_Stepper.step == -1)
   {
      #if defined(STEPPER_HALF_STEPS)
      _g_Stepper.step = 7;
      #else
      _g_Stepper.step = 3;
      #endif
   }
   
   _StepperGotoStep();
}
#endif //defined(PIN_STEPPER_A_PLUS)

#if defined(PIN_STEPPER_CLOCK)
static void _StepperStop(void)
{
   _stepper_enable_low();
   
   _stepper_output_low(PIN_STEPPER_CLOCK);
}

static void _StepperWait(void)
{
   _g_Stepper.step = TRUE;
   _g_Stepper.delayed = TRUE;
   _g_Stepper.t = TickGet();
}

static void _StepperNextStep(void)
{
   _stepper_enable_high();
   
   _stepper_output_low(PIN_STEPPER_CLOCK);
   _stepper_output_low(PIN_STEPPER_DIR);
   _stepper_output_high(PIN_STEPPER_CLOCK);
   
   _g_Stepper.pos++;
   
   _StepperWait();
}

static void _StepperPrevStep(void)
{
   _stepper_enable_high();
   
   _stepper_output_low(PIN_STEPPER_CLOCK);
   _stepper_output_high(PIN_STEPPER_DIR);
   _stepper_output_high(PIN_STEPPER_CLOCK);
   
   _g_Stepper.pos--;
   
   _StepperWait();
}
#endif

void StepperMove(stepper_pos_t delta)
{
   _g_Stepper.dest = _g_Stepper.pos + delta;
}

void StepperGoto(stepper_pos_t dest)
{
   _g_Stepper.dest = dest;
}

void StepperStop(void)
{
   _g_Stepper.dest = _g_Stepper.pos;
}

void StepperRelease(void)
{
   StepperStop();
   _StepperStop();
}

stepper_pos_t StepperGetPos(void)
{
   return(_g_Stepper.pos);
}

void StepperSetPos(stepper_pos_t pos)
{
   _g_Stepper.pos = pos;
   _g_Stepper.dest = pos;
}

int1 StepperIsMoving(void)
{
   return((_g_Stepper.pos != _g_Stepper.dest) || _g_Stepper.delayed);
}

void StepperSetSpeed(TICK speed)
{
   _g_Stepper.speed = speed;
}

void StepperInit(void)
{
   memset(&_g_Stepper, 0, sizeof(_g_Stepper));
   _StepperStop();
}

void StepperTask(void)
{
   if (_g_Stepper.delayed)
   {
      if (TickIsExpired(_g_Stepper.t, _g_Stepper.speed))
      {
         _g_Stepper.delayed = FALSE;
        
        #if defined(PIN_STEPPER_CLOCK)
         if (_g_Stepper.step)
         {
            _stepper_output_low(PIN_STEPPER_CLOCK);
            _StepperWait();
            _g_Stepper.step = FALSE;
         }
        #endif
      }
      else
         return;
   }
   
   if (_g_Stepper.pos == _g_Stepper.dest)
      return;
   
   if (_g_Stepper.pos < _g_Stepper.dest)
      _StepperNextStep();
   else
      _StepperPrevStep();
}

#endif
