///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

/* limits.h  */


#ifndef _LIMITS
#define _LIMITS
#define _C2 1

#define CHAR_BIT  1  /* short */

      /* unsigned char properties*/
      /* treated as unsigned integer*/
#define CHAR_MAX 255 /*unsigned integer maximum*/
#define CHAR_MIN 0   /*unsigned integer minimum*/

      /*signed char properties */
#define SCHAR_MAX 127        /*signed integer maximum*/
#define SCHAR_MIN (-127-_C2) /*signed integer minimum*/

      /* multibyte properties */
      /* multibyte not supported */
#define MB_LEN_MAX 1


      /* signed int properties */
#define INT_MAX 127        /* signed integer minimum*/
#define INT_MIN (-127-_C2) /*signed integer maximum*/
#define UINT_MAX 255       /*unsigned integer maximum*/

      /* signed long properties */
#define LONG_MAX 32767        /* signed long maximum*/
#define LONG_MIN (-32767-_C2) /* signed long minimum*/

      /* signed short properties */
#define SHRT_MAX 1 /* signed short maximum*/
#define SHRT_MIN 0 /* signed short minimum*/

      /* unsigned properties */
#define UCHAR_MAX 255  /*unsigned integer maximum*/
#define ULONG_MAX 65535/*unsigned long integer maximum*/
#define USHRT_MAX 1    /*unsigned short integer maximum*/
#endif
