///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// inttypes.h                                                        ////
////                                                                   ////
//// C standard library and API definitions for printf, scanf and      ////
//// intmax_t.                                                         ////
////                                                                   ////
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


#ifndef __INTTYPES_H__
#define __INTTYPES_H__

#include <stdint.h>

#if defined(__PCM__) || defined(__PCH__) || defined(__PCB__)
   #define PRIdMAX "ld"
   #define PRIiMAX "li"
   #define PRIoMAX "lo"
   #define PRIuMAX "lu"
   #define PRIxMAX "lx"
   #define PRIXMAX "lX"
   
   #define PRId8 "d"
   #define PRIi8 "i"
   #define PRIo8 "o"
   #define PRIu8 "u"
   #define PRIx8 "x"
   #define PRIX8 "X"
   
   #define PRId16 "ld"
   #define PRIi16 "li"
   #define PRIo16 "lo"
   #define PRIu16 "lu"
   #define PRIx16 "lx"
   #define PRIX16 "lX"
   
   #define PRId32 "ld"
   #define PRIi32 "li"
   #define PRIo32 "lo"
   #define PRIu32 "lu"
   #define PRIx32 "lx"
   #define PRIX32 "lX"
   
   #define PRIdLEAST8 "d"
   #define PRIiLEAST8 "i"
   #define PRIoLEAST8 "o"
   #define PRIuLEAST8 "u"
   #define PRIxLEAST8 "x"
   #define PRIXLEAST8 "X"
   
   #define PRIdLEAST16 "ld"
   #define PRIiLEAST16 "li"
   #define PRIoLEAST16 "lo"
   #define PRIuLEAST16 "lu"
   #define PRIxLEAST16 "lx"
   #define PRIXLEAST16 "lX"
   
   #define PRIdLEAST32 "ld"
   #define PRIiLEAST32 "li"
   #define PRIoLEAST32 "lo"
   #define PRIuLEAST32 "lu"
   #define PRIxLEAST32 "lx"
   #define PRIXLEAST32 "lX"
   
   #define PRIdFAST8 "d"
   #define PRIiFAST8 "i"
   #define PRIoFAST8 "o"
   #define PRIuFAST8 "u"
   #define PRIxFAST8 "x"
   #define PRIXFAST8 "X"
   
   #define PRIdFAST16 "ld"
   #define PRIiFAST16 "li"
   #define PRIoFAST16 "lo"
   #define PRIuFAST16 "lu"
   #define PRIxFAST16 "lx"
   #define PRIXFAST16 "lX"
   
   #define PRIdFAST32 "ld"
   #define PRIiFAST32 "li"
   #define PRIoFAST32 "lo"
   #define PRIuFAST32 "lu"
   #define PRIxFAST32 "lx"
   #define PRIXFAST32 "lX"
   
   #define PRIdPTR "ld"
   #define PRIiPTR "li"
   #define PRIoPTR "lo"
   #define PRIuPTR "lu"
   #define PRIxPTR "lx"
   #define PRIXPTR "lX"
   
   #define SCNdMAX "ld"
   #define SCNiMAX "li"
   #define SCNoMAX "lo"
   #define SCNuMAX "lu"
   #define SCNxMAX "lx"
   #define SCNXMAX "lX"
   
   #define SCNd8 "d"
   #define SCNi8 "i"
   #define SCNo8 "o"
   #define SCNu8 "u"
   #define SCNx8 "x"
   #define SCNX8 "X"
   
   #define SCNd16 "ld"
   #define SCNi16 "li"
   #define SCNo16 "lo"
   #define SCNu16 "lu"
   #define SCNx16 "lx"
   #define SCNX16 "lX"
   
   #define SCNd32 "ld"
   #define SCNi32 "li"
   #define SCNo32 "lo"
   #define SCNu32 "lu"
   #define SCNx32 "lx"
   #define SCNX32 "lX"
   
   #define SCNdLEAST8 "d"
   #define SCNiLEAST8 "i"
   #define SCNoLEAST8 "o"
   #define SCNuLEAST8 "u"
   #define SCNxLEAST8 "x"
   #define SCNXLEAST8 "X"
   
   #define SCNdLEAST16 "ld"
   #define SCNiLEAST16 "li"
   #define SCNoLEAST16 "lo"
   #define SCNuLEAST16 "lu"
   #define SCNxLEAST16 "lx"
   #define SCNXLEAST16 "lX"
   
   #define SCNdLEAST32 "ld"
   #define SCNiLEAST32 "li"
   #define SCNoLEAST32 "lo"
   #define SCNuLEAST32 "lu"
   #define SCNxLEAST32 "lx"
   #define SCNXLEAST32 "lX"
   
   #define SCNdFAST8 "d"
   #define SCNiFAST8 "i"
   #define SCNoFAST8 "o"
   #define SCNuFAST8 "u"
   #define SCNxFAST8 "x"
   #define SCNXFAST8 "X"
   
   #define SCNdFAST16 "ld"
   #define SCNiFAST16 "li"
   #define SCNoFAST16 "lo"
   #define SCNuFAST16 "lu"
   #define SCNxFAST16 "lx"
   #define SCNXFAST16 "lX"
   
   #define SCNdFAST32 "ld"
   #define SCNiFAST32 "li"
   #define SCNoFAST32 "lo"
   #define SCNuFAST32 "lu"
   #define SCNxFAST32 "lx"
   #define SCNXFAST32 "lX"
   
   #define SCNdPTR "ld"
   #define SCNiPTR "li"
   #define SCNoPTR "lo"
   #define SCNuPTR "lu"
   #define SCNxPTR "lx"
   #define SCNXPTR "lX"
#elif defined(__PCD__)
   #define PRIdMAX "d"
   #define PRIiMAX "i"
   #define PRIoMAX "o"
   #define PRIuMAX "u"
   #define PRIxMAX "x"
   #define PRIXMAX "X"
   
   #define PRId8 "d"
   #define PRIi8 "i"
   #define PRIo8 "o"
   #define PRIu8 "u"
   #define PRIx8 "x"
   #define PRIX8 "X"
   
   #define PRId16 "d"
   #define PRIi16 "i"
   #define PRIo16 "o"
   #define PRIu16 "u"
   #define PRIx16 "x"
   #define PRIX16 "X"
   
   #define PRId32 "d"
   #define PRIi32 "i"
   #define PRIo32 "o"
   #define PRIu32 "u"
   #define PRIx32 "x"
   #define PRIX32 "X"
   
   #define PRId64 "d"
   #define PRIi64 "i"
   #define PRIo64 "o"
   #define PRIu64 "u"
   #define PRIx64 "x"
   #define PRIX64 "X"
   
   #define PRIdLEAST8 "d"
   #define PRIiLEAST8 "i"
   #define PRIoLEAST8 "o"
   #define PRIuLEAST8 "u"
   #define PRIxLEAST8 "x"
   #define PRIXLEAST8 "X"
   
   #define PRIdLEAST16 "d"
   #define PRIiLEAST16 "i"
   #define PRIoLEAST16 "o"
   #define PRIuLEAST16 "u"
   #define PRIxLEAST16 "x"
   #define PRIXLEAST16 "X"
   
   #define PRIdLEAST32 "d"
   #define PRIiLEAST32 "i"
   #define PRIoLEAST32 "o"
   #define PRIuLEAST32 "u"
   #define PRIxLEAST32 "x"
   #define PRIXLEAST32 "X"
   
   #define PRIdLEAST64 "d"
   #define PRIiLEAST64 "i"
   #define PRIoLEAST64 "o"
   #define PRIuLEAST64 "u"
   #define PRIxLEAST64 "x"
   #define PRIXLEAST64 "X"
   
   #define PRIdFAST8 "d"
   #define PRIiFAST8 "i"
   #define PRIoFAST8 "o"
   #define PRIuFAST8 "u"
   #define PRIxFAST8 "x"
   #define PRIXFAST8 "X"
   
   #define PRIdFAST16 "d"
   #define PRIiFAST16 "i"
   #define PRIoFAST16 "o"
   #define PRIuFAST16 "u"
   #define PRIxFAST16 "x"
   #define PRIXFAST16 "X"
   
   #define PRIdFAST32 "d"
   #define PRIiFAST32 "i"
   #define PRIoFAST32 "o"
   #define PRIuFAST32 "u"
   #define PRIxFAST32 "x"
   #define PRIXFAST32 "X"
   
   #define PRIdFAST64 "d"
   #define PRIiFAST64 "i"
   #define PRIoFAST64 "o"
   #define PRIuFAST64 "u"
   #define PRIxFAST64 "x"
   #define PRIXFAST64 "X"
   
   #define PRIdPTR "d"
   #define PRIiPTR "i"
   #define PRIoPTR "o"
   #define PRIuPTR "u"
   #define PRIxPTR "x"
   #define PRIXPTR "X"
   
   #define SCNdMAX "d"
   #define SCNiMAX "i"
   #define SCNoMAX "o"
   #define SCNuMAX "u"
   #define SCNxMAX "x"
   #define SCNXMAX "X"
   
   #define SCNd8 "d"
   #define SCNi8 "i"
   #define SCNo8 "o"
   #define SCNu8 "u"
   #define SCNx8 "x"
   #define SCNX8 "X"
   
   #define SCNd16 "d"
   #define SCNi16 "i"
   #define SCNo16 "o"
   #define SCNu16 "u"
   #define SCNx16 "x"
   #define SCNX16 "X"
   
   #define SCNd32 "d"
   #define SCNi32 "i"
   #define SCNo32 "o"
   #define SCNu32 "u"
   #define SCNx32 "x"
   #define SCNX32 "X"
   
   #define SCNd64 "d"
   #define SCNi64 "i"
   #define SCNo64 "o"
   #define SCNu64 "u"
   #define SCNx64 "x"
   #define SCNX64 "X"
   
   #define SCNdLEAST8 "d"
   #define SCNiLEAST8 "i"
   #define SCNoLEAST8 "o"
   #define SCNuLEAST8 "u"
   #define SCNxLEAST8 "x"
   #define SCNXLEAST8 "X"
   
   #define SCNdLEAST16 "d"
   #define SCNiLEAST16 "i"
   #define SCNoLEAST16 "o"
   #define SCNuLEAST16 "u"
   #define SCNxLEAST16 "x"
   #define SCNXLEAST16 "X"
   
   #define SCNdLEAST32 "d"
   #define SCNiLEAST32 "i"
   #define SCNoLEAST32 "o"
   #define SCNuLEAST32 "u"
   #define SCNxLEAST32 "x"
   #define SCNXLEAST32 "X"
   
   #define SCNdLEAST64 "d"
   #define SCNiLEAST64 "i"
   #define SCNoLEAST64 "o"
   #define SCNuLEAST64 "u"
   #define SCNxLEAST64 "x"
   #define SCNXLEAST64 "X"
   
   #define SCNdFAST8 "d"
   #define SCNiFAST8 "i"
   #define SCNoFAST8 "o"
   #define SCNuFAST8 "u"
   #define SCNxFAST8 "x"
   #define SCNXFAST8 "X"
   
   #define SCNdFAST16 "d"
   #define SCNiFAST16 "i"
   #define SCNoFAST16 "o"
   #define SCNuFAST16 "u"
   #define SCNxFAST16 "x"
   #define SCNXFAST16 "X"
   
   #define SCNdFAST32 "d"
   #define SCNiFAST32 "i"
   #define SCNoFAST32 "o"
   #define SCNuFAST32 "u"
   #define SCNxFAST32 "x"
   #define SCNXFAST32 "X"
   
   #define SCNdFAST64 "d"
   #define SCNiFAST64 "i"
   #define SCNoFAST64 "o"
   #define SCNuFAST64 "u"
   #define SCNxFAST64 "x"
   #define SCNXFAST64 "X"
   
   #define SCNdPTR "d"
   #define SCNiPTR "i"
   #define SCNoPTR "o"
   #define SCNuPTR "u"
   #define SCNxPTR "x"
   #define SCNXPTR "X"
#else
   #error No support for this chip.
#endif

#define imaxabs(_v) abs(_v)

#if defined(__PCD__)
#define imaxdiv(_v1, _v2) ldiv(_v1, _v2)
#define imaxdiv_t ldiv_t
#else
#define imaxdiv(_v1, _v2) div(_v1, _v2)
#define imaxdiv_t div_t
#endif

#define strtoimax(_str, _endPtr, _base) strtol(_str, _endPtr, _base)

#define strtoumax(_str, _endPtr, _base) strtoul(_str, _endPtr, _base)

#define wcstoimax(_str, _endPtr, _base) wcstol(_str, _endPtr, _base)

#define wcstoumax(_str, _endPtr, _base) wcstoul(_str, _endPtr, _base)

#endif //__INTTYPES_H__

