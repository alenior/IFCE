/*
   Tick.c
   
   Uses a PIC's timer peripheral to create a tick system that increments at
   a rate of TICKS_PER_SECOND.
   
   See Tick.h for documentation.
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __TICK_C__
#define __TICK_C__

#include <tick.h>

// see tick.h for documentation
void TickInit(void)
{
   // nothing to do, this is done by the #use timer
}

// see tick.h for documentation
TICK TickGet(void)
{
   return(get_ticks(STREAM_TIMER_TICK));
}

// see tick.h for documentation
void TickDelay(TICK duration)
{
   TICK t;
   
   t = TickGet();
   
   while(!TickIsExpired(t, duration)) { }
}

// see tick.h for documentation
void TickDelayWDT(TICK duration)
{
   TICK t;
   
   t = TickGet();
   
   while(!TickIsExpired(t, duration))   
   {
      restart_wdt();
   }
}

// see tick.h for documentation
TICK TickFromMs(unsigned int16 ms)
{
   unsigned int32 scr32;
   
   scr32 = ms;
   scr32 *= TICKS_PER_SECOND;
   scr32 /= 1000;
   
   return(scr32);
}

// see tick.h for documentation
unsigned int16 TickToMs(TICK t)
{
   unsigned int32 scr32;
   
   scr32 = t;
   scr32 *= 1000;
   scr32 /= TICKS_PER_SECOND;
   
   if (scr32 > (unsigned int32)0xFFFF)
   {
      scr32 = (unsigned int32)0xFFFF;
   }
   
   return(scr32);
}

// see tick.h for documentation
void TickDelayMs(unsigned int16 duration)
{
   TickDelay(TickFromMs(duration));
}

// see tick.h for documentation
void TickDelayMsWDT(unsigned int16 duration)
{
   TickDelayWDT(TickFromMs(duration));
}

// add functions to be time.h compatible
#if defined(__TIME_H__) && !defined(__TIME_C__)

#ifndef TickDiffernce
 #define TickDiffernce(a,b)   (a-b)
#endif

#ifndef NULL
 #define NULL  0
#endif

#define CLOCKS_PER_SECOND     TICKS_PER_SECOND

#include <time.c>

/* Globals & Resources */
static time_t calendar_time; //seconds since Jan 1, 1970 00:00:00 or January 1 2010 00:00:00 if TIME_T_USES_2010 was defined
TICK TimeTick;

/* Returns processor time used by program (clock ticks) */
clock_t clock(void)
{
   return(TickGet());
}

/* Returns the current calendar time. */
time_t time(time_t * timer)
{
   if(timer != NULL)
      *timer = calendar_time;
      
   return(calendar_time);
}

/* Sets the current calendar time (in seconds)from the given time. */
void SetTime(struct_tm * nTime)
{ 
   calendar_time = mktime(nTime);
}

/* Sets the calendar time to this time. */
void SetTimeSec(time_t sTime)
{
   calendar_time = sTime;
}

/* Returns the current time as a time struct. */
void GetTime(struct_tm *pRetTm)
{
   struct_tm *Time;
   
   Time = localtime(&calendar_time);
   
   memcpy(pRetTm, Time, sizeof(struct_tm));
}

/* Initializes */
void TimeInit(void)
{
   calendar_time = 0;
   TimeTick = TickGet();
}

/* Task function for increamenting calendar time. 
   - This need to be called inorder to increament time_t calandar_time variable.
*/
void TimeTask(void)
{
   TICK cTick;
   
   cTick = TickGet();
   
   while(TickDiffernce(cTick, TimeTick) >= (CLOCKS_PER_SECOND))
   {
      calendar_time++;
      TimeTick += CLOCKS_PER_SECOND;
   }
}

#endif   //__TIME_H__
#endif   //ifndef __TICK_H__
