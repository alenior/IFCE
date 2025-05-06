/******************************************************************************

   timeouts.c
    
   Call a function after a specified duration has expired.
    
   See timeouts.h for documentation
    
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

#include "timeouts.h"

#if 0
   #include <inttypes.h>
   #if defined(DEBUG0) && defined(DEBUG1)
      #define DEBUG_TIMEOUTS0 DEBUG0
      #define DEBUG_TIMEOUTS1 DEBUG1
   #else
      #define DEBUG_TIMEOUTS0(_msg) printf(_msg)
      #define DEBUG_TIMEOUTS1(_msg, _val0) printf(_msg, _val0)
    #endif
#else
   #define DEBUG_TIMEOUTS0(_msg)
   #define DEBUG_TIMEOUTS1(_msg, _val0)
#endif

#include <stddef.h>

#include <critical.h>

#ifndef TIMEOUTS_OVERRIDE_TIMEBASE
    #include <tick.c>
    
    // needs to be an unsigned int type, uint16_t or uint32_t.
    #define timeouts_tick_t uint32_t
    
    // return current tick time.
    #define TimeoutsTickGet() (timeouts_tick_t)TickGet()
    
    // convert '_ms' milliseconds to ticks.
    #define TimeoutsMsToTick(_ms) (timeouts_tick_t)TickFromMs(_ms)
    
    // convert '_tick' ticks to milliseconds.
    #define TimeoutsTickToMs(_tick) TickToMs(_tick)
    
    // return the maximum positive value that can be stored to timeouts_tick_t
    // if timeouts_tick_t was a signed integer.  should be 0x7FFF for 16bit
    // or 0x7FFFFFFF for 32bit tick.
    #define TIMEOUTS_TICK_MAX INT32_MAX
#endif

typedef struct _timeouts_entry_s
{
    timeouts_pf_t pFunc;
    void* pArgs;
    timeouts_tick_t expiration;
    struct _timeouts_entry_s *pNext;
} _timeouts_entry_t;

// the timeout stack; a linked list
static struct _timeouts_entry_s *_p_TimeoutsList;

// stack memory manipulation
#ifdef _TIMEOUTS_STATIC_MEMORY_ENTRIES
    static struct _timeouts_entry_s _g_timeouts_entries[_TIMEOUTS_STATIC_MEMORY_ENTRIES];
    
    void* _timeouts_malloc(void)
    {
        int i;
        
        for(i=0; i<_TIMEOUTS_STATIC_MEMORY_ENTRIES; i++)
        {
            if (_g_timeouts_entries[i].pFunc == NULL)
                return &_g_timeouts_entries[i];
        }
        return NULL;
    }

    void _timeouts_free(void* ptr)
    {
        ((struct _timeouts_entry_s*)ptr)->pFunc = NULL;
    }
#else
    #if defined(__PCD__) || defined(__PCH__) || defined(__PCM__) || defined(__PCB__)
    #include <stdlibm.h>
    #else
    #include <stdlib.h>
    #include <string.h>
    #endif
    #define _timeouts_malloc() malloc(sizeof(_timeouts_entry_t))
    #define _timeouts_free(_ptr) free(_ptr)
#endif

static struct _timeouts_entry_s * _TimeoutsMakeEntry(timeouts_pf_t pFunc, void* pArgs, timeouts_tick_t expiration)
{
   struct _timeouts_entry_s *pToAdd;
   
    pToAdd = (struct _timeouts_entry_s *)_timeouts_malloc();
    
    if (!pToAdd)
        return 0;

    pToAdd->pFunc = pFunc;
    pToAdd->pArgs = pArgs;
    pToAdd->expiration = expiration;
    pToAdd->pNext = NULL;
    
    return pToAdd;
}

// see timeout.h for documentation
bool TimeoutsAdd(timeouts_pf_t pFunc, void* pArgs, uint16_t ms)
{
    struct _timeouts_entry_s *pToAdd, *pNext;
    void **pLast;
    timeouts_tick_t expiration;
    
    expiration = TimeoutsTickGet() + TimeoutsMsToTick(ms);
    
    pToAdd = _TimeoutsMakeEntry(pFunc, pArgs, expiration);

    DEBUG_TIMEOUTS1("TimeoutsAdd(pf=%" PRIxPTR, pFunc);
    DEBUG_TIMEOUTS1(", pArgs=%" PRIxPTR, pArgs);
    DEBUG_TIMEOUTS1(", ms=%" PRIu16, ms);
    DEBUG_TIMEOUTS1(") p=%" PRIxPTR "\r\n", pToAdd);
    
    CRITICAL_SECTION_ENTER();
    
    if (!_p_TimeoutsList)
        _p_TimeoutsList = pToAdd;
    else
    {
        pLast = (void**)&_p_TimeoutsList;
        pNext = _p_TimeoutsList;
        
        while(pNext)
        {
            if (expiration < pNext->expiration)
                break;
            pLast = (void**)&pNext->pNext;
            pNext = pNext->pNext;
        }
        
        *pLast = pToAdd;
        pToAdd->pNext = pNext;
    }
    
    CRITICAL_SECTION_EXIT();

    return 1;
}

// see timeout.h for documentation
bool TimeoutsImmediate(timeouts_pf_t pFunc, void* pArgs)
{
    struct _timeouts_entry_s *pToAdd, *pNext;
    void **pLast;
    
    DEBUG_TIMEOUTS1("TimeoutsImmediate(pf=%" PRIxPTR, pFunc);
    DEBUG_TIMEOUTS1(", pArgs=%" PRIxPTR, pArgs);
    DEBUG_TIMEOUTS0(")\r\n");

    pToAdd = _TimeoutsMakeEntry(pFunc, pArgs, TimeoutsTickGet());
    
    CRITICAL_SECTION_ENTER();
    
    if (!_p_TimeoutsList)
        _p_TimeoutsList = pToAdd;
    else
    {
        pLast = (void**)&_p_TimeoutsList;
        pNext = _p_TimeoutsList;
        
        *pLast = pToAdd;
        pToAdd->pNext = pNext;
    }
    
    CRITICAL_SECTION_EXIT();

    return 1;
}

// see timeout.h for documentation
void TimeoutsRemove(timeouts_pf_t pFunc, void* pArgs)
{
    bool dofree = 0;
    struct _timeouts_entry_s *p, *next;
    void **pLast;

    DEBUG_TIMEOUTS1("TimeoutsRemove(pf=%" PRIxPTR, pFunc);
    DEBUG_TIMEOUTS1(", pArgs=%" PRIxPTR, pArgs);
    DEBUG_TIMEOUTS0(") ");
    
    CRITICAL_SECTION_ENTER();
    
    pLast = (void**)&_p_TimeoutsList;
    p = _p_TimeoutsList;
    
    while(p)
    {
        next = p->pNext;

        if
        (
            (p->pFunc == pFunc)
            && (
                (pArgs == NULL) ||
                (p->pArgs == pArgs)
            )
        )
        {
            *pLast = next;
            dofree = 1;
        }

        pLast = (void**)&p->pNext;
        
        if (dofree)
        {
            DEBUG_TIMEOUTS1(" free=%" PRIxPTR, p);
            dofree = 0;
            _timeouts_free(p);
        }
        
        p = next;
    }
    
    CRITICAL_SECTION_EXIT();

    DEBUG_TIMEOUTS0("\r\n");
}

// see timeout.h for documentation
bool TimeoutsIsAdded(timeouts_pf_t pFunc, void* pArgs)
{
    struct _timeouts_entry_s *p;
    
    CRITICAL_SECTION_ENTER();
    
    p = _p_TimeoutsList;
    
    while(p)
    {
        if
        (
            (p->pFunc == pFunc)
            && (
                (pArgs == NULL) ||
                (pArgs == p->pArgs)
            )
        )
        {
            CRITICAL_SECTION_EXIT();
            
            return 1;
        }
        
        p = p->pNext;
    }
    
    CRITICAL_SECTION_EXIT();
    
    return 0;
}

uint16_t TimeoutsNext(void)
{
   timeouts_tick_t expiration, now;
   uint32_t ret;
   
   CRITICAL_SECTION_ENTER();
   
   if (!_p_TimeoutsList)
   {
      CRITICAL_SECTION_EXIT();
      
      return((uint16_t)0xFFFF);
   }
   
   expiration = _p_TimeoutsList->expiration;
   
   CRITICAL_SECTION_EXIT();
   
   now = TimeoutsTickGet();
   
   expiration = (expiration - now);
   
   if (expiration > (timeouts_tick_t)TIMEOUTS_TICK_MAX)
      return 0;
   
   ret = (uint32_t)TimeoutsTickToMs(expiration);
   
   if (ret > (uint32_t)0xFFFF)
      ret = (uint32_t)0xFFFF;
      
   return ret;
}

// see timeout.h for documentation
void TimeoutsRemoveAll(void)
{
    struct _timeouts_entry_s *p, *next;
    void **pLast;
    
    DEBUG_TIMEOUTS0("TimeoutsRemoveAll()\r\n");

    CRITICAL_SECTION_ENTER();
    
    pLast = (void**)&_p_TimeoutsList;
    p = _p_TimeoutsList;
    
    while(p)
    {
        next = p->pNext;

        *pLast = next;

        pLast = (void**)&p->pNext;

        _timeouts_free(p);
        
        p = next;
    }
    
    CRITICAL_SECTION_EXIT();
}

// see timeout.h for documentation
void TimeoutsTask(void)
{
    if (!_p_TimeoutsList)
        return;
    
    struct _timeouts_entry_s entry;
    
    CRITICAL_SECTION_ENTER();
    
    memcpy(&entry, _p_TimeoutsList, sizeof(entry));
    
    if ((entry.expiration - TimeoutsTickGet()) > (timeouts_tick_t)TIMEOUTS_TICK_MAX)
    {
        _timeouts_free(_p_TimeoutsList);
        
        _p_TimeoutsList = entry.pNext;
        
        CRITICAL_SECTION_EXIT();

        DEBUG_TIMEOUTS1("TimeoutsTask() CALLING pf=%" PRIxPTR, entry.pFunc);
        DEBUG_TIMEOUTS1(", pArgs=%" PRIxPTR, entry.pArgs);
        DEBUG_TIMEOUTS0("\r\n");
        
        entry.pFunc(entry.pArgs);
    }
    else
    {
        CRITICAL_SECTION_EXIT();
    }
}

#if 0
// printf some debugging information about the timeout stack.
void TimeoutsDebug(void)
{
    struct _timeouts_entry_s *p;
    struct _timeouts_entry_s e;
    
    DEBUG_TIMEOUTS1("Timeouts_HEAD=0x%" PRIxPTR "\r\n", _p_TimeoutsList);
    
    p = _p_TimeoutsList;
    
    while(p)
    {
        memcpy(&e, p, sizeof(e));

        DEBUG_TIMEOUTS1("p=0x%" PRIxPTR, p);
        DEBUG_TIMEOUTS1(" pFunc=0x%" PRIxPTR, e.pFunc);
        DEBUG_TIMEOUTS1(" pArgs=0x%" PRIxPTR, e.pArgs);
        DEBUG_TIMEOUTS1(" expy=0x%" PRIuMAX, e.expiration);
        DEBUG_TIMEOUTS1(" pNext=0x%" PRIxPTR "\r\n", e.pNext);
        
        p = p->pNext;
    }
}
#else
void TimeoutsDebug(void) {}
#endif

