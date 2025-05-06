/******************************************************************************

    timeouts.h
    
    Call a function after a specified duration has expired.
    
    See the comments above the prototypes for API documentation.
    
    Functions can be called from inside ISRs, which is useful if you are using
    the library to post an event from an ISR to be handled in the main loop.
    
    This library does require a timebase.  If none is provided, the tick.c
    driver provided by CCS will be used.  To override this and provide your
    own timebase, then you need to #define TIMEOUTS_OVERRIDE_TIMEBASE before
    including timeouts.c.  See TIMEOUTS_OVERRIDE_TIMEBASE in timeouts.c
    for a list of functions and datatypes that you must provide.
    
*****************************************************************************/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2019 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef __TIMEOUTS_H__
#define __TIMEOUTS_H__

#include <stdbool.h>
#include <stdint.h>

// All functions pushed onto the timeouts stack must follow this prototype.
typedef void (* timeouts_pf_t)(void* pArgs);

// 'ms' milliseconds from now, call 'pFunc' and pass 'pArgs' to it.
bool TimeoutsAdd(timeouts_pf_t pFunc, void* pArgs, uint16_t ms);

// Push 'pFunc' to top of the stack so it is called next.
// This differs from caling TimeoutsAdd() with ms=0 as TimeoutsAdd() may
// push the new event until after events that have already expired.
bool TimeoutsImmediate(timeouts_pf_t pFunc, void* pArgs);

// Returns TRUE if this timeout has been added to the stack and will be
// called once it's duration has expired.
// if the 'pArgs' passed to this function is NULL, then it will match
// any 'pFunc' that is found.
// Once the timeout's expiration has expired and has been called, it will
// be removed from the stack and future calls to TimeoutsIsAdd() will be 
// FALSE.
bool TimeoutsIsAdded(timeouts_pf_t pFunc, void* pArgs);

// Returns how many milliseconds until next posted event.
// If your cooprative multitasking scheme is built around the timeouts library,
// this function could be used to put the PIC to sleep for this amount of 
// time because the PIC wouldn't have anything to do until this time has
// expired.
// returns 0xFFFF if there are no timeouts or if the value doesn't fit
// within 16bits.
uint16_t TimeoutsNext(void);

// Remove any previously added 'pFunc' with 'pArgs'.
// if 'pArgs' is NULL, then remove all that match 'pFunc' regardless of
// what their args are.
void TimeoutsRemove(timeouts_pf_t pFunc, void* pArgs);

// Clear the existing stack of all posted timeouts.
void TimeoutsRemoveAll(void);

// If a timeout has expired, it gets called.
// This needs to be called in your main loop.
void TimeoutsTask(void);

#endif //__TIMEOUTS_H__


