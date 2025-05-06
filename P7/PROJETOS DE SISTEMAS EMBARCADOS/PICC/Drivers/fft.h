///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                              fft.h                                ////
////                                                                   ////
//// Library for performing FFT and IFFT on a dsPIC.  dsPIC's          ////
//// instruction set are utilized for maximum effeciency.              ////
////                                                                   ////
////                                                                   ////
//// CONFIG                                                            ////
//// ----------------------------------------------------------------- ////
//// FFT_LENGTH                                                        ////
////     Must be #defined before loading this library.  This defines   ////
////     the maximum FFT size (in input samples, or FFT bins           ////
////     multiplied by 2) that can be used by this library.  This      ////
////     must be a power of 2 (2, 4, 8, 16, 32, etc).  Any length      ////
////     sized used by this API must be a power of 2.                  ////
////                                                                   ////
////                                                                   ////
//// API                                                               ////
//// ----------------------------------------------------------------- ////
//// fft_init()                                                        ////
//// fft_init(fft_size)                                                ////
//// fft_init(Complex* tw_y, fft_size)                                 ////
////     If 'fft_size' isn't provided, FFT_LENGTH is used.  Inits      ////
////     the library and generates the FFT twiddle factors for a       ////
////     an FFT size of 'fft_size'.  If 'tw_y' isn't provided then it  ////
////     uses a default internal buffer.  'tw_y' must be located       ////
////     in the Y bank.                                                ////
////                                                                   ////
//// build_twiddle(Complex* tw_y, fft_size)                            ////
////     Builds the FFT twiddle factors for an FFT of size 'fft_size'. ////
////     FFT twiddle factors saved to 'tw_y', which must be in the     ////
////     Y bank.  Twiddle factors only need to be built once.          ////
////     Normally fft_init() will do this for you, but if you are      ////
////     using this library to perform FFT operations on different     ////
////     buffer sizes (instead of sticking with one buffer size for    ////
////     the entirety of the program), then the user needs to generate ////
////     the build_twiddle() for the different sizes being used.       ////
////                                                                   ////
//// unsigned int16 = cplx_magnitude(cplx_magnitude *c)                ////
////     Calculates the magnitude of complex sample c.                 ////
////                                                                   ////
//// Complex* result = fft(cplx_input input)                           ////
//// Complex* result = fft(cplx_input input, fft_size)                 ////
//// Complex* result = fft(cplx_input input, Complex* tw_y, fft_size)  ////
////     Calculates the FFT using 'fft_size' samples from 'input'.     ////
////     Uses 'tw_y' for the twiddle factors, and must be located in   ////
////     the Y bank.  If 'fft_size' is not provided, it will use       ////
////     FFT_LENGTH.  If 'tw_y' is not provided, it will use the       ////
////     default internal buffer used by fft_init() and                ////
////     fft_init(fft_size).  'tw_y' should have been generated with   ////
////     fft_init() or build_twiddle().  Returns a pointer to results. ////
////     'input', 'result' and 'fft_size' cannot be larger than        ////
////     FFT_LENGTH.                                                   ////
////                                                                   ////
//// Complex* result = ifft(cplx_input input)                          ////
//// Complex* result = ifft(cplx_input input, fft_size)                ////
//// Complex* result = ifft(cplx_input input, Complex* tw_y, fft_size) ////
////     Calculates the inverse FFT (frequency domain to time domeain) ////
////     using 'fft_size' samples from 'input'.  Uses 'tw_y' for the   ////
////     twiddle factors, and must be located in the Y bank.  If       ////
////     'fft_size' is not provided, it will use FFT_LENGTH.           ////
////     If 'tw_y' is not provided, it will use the default internal   ////
////     buffer used by fft_init() and fft_init(fft_size).  'tw_y'     ////
////     should have been generated with fft_init() or                 ////
////     build_twiddle().  Returns a pointer to results.  'input',     ////
////     'result' and 'fft_size' cannot be larger than FFT_LENGTH.     ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#ifndef FFT_H
#define FFT_H 1

#ifndef FFT_LENGTH
#error FFT_LENGTH must be defined prior to including this header!
#endif

#if ((FFT_LENGTH != 2) && (FFT_LENGTH != 4) && (FFT_LENGTH != 8) && (FFT_LENGTH != 16) && (FFT_LENGTH != 32) && (FFT_LENGTH != 64) && (FFT_LENGTH != 128) && (FFT_LENGTH != 256) && (FFT_LENGTH != 512))
#error FFT LENGTH must be a power of two!
#endif

#include <math.h>

/* Core configuration registers. */
#word XBREV = 0x0050
#bit BREN = XBREV.15
#word MODCON = 0x0046
#word CORCON = 0x0044
#bit IF_EN = CORCON.0
#bit RND = CORCON.1
#bit ACCSAT = CORCON.4
#bit SATA = CORCON.7
#bit SATB = CORCON.6
#bit US = CORCON.12
#bit US1 = CORCON.13

/* Represent a complex number with both a real and imaginary part. */
#ifndef _complexnum
#define _complexnum
typedef signed int16 fft_int_t;
typedef struct _complex
{
   fft_int_t re;
   fft_int_t im;
} Complex;
#endif

#banky
Complex twiddle[FFT_LENGTH / 2];

// see documentation above
unsigned int16 cplx_magnitude(Complex* c)
{   
   unsigned int32 op;
   unsigned int32 res;
   unsigned int32 one;
   
   signed int16 re;
   signed int16 im;
   
   
   /* Set the core configuration. */
   US = 0;//unsigned
   US1 = 0;
   IF_EN = 1;//integer
   SATB = 1;//enable saturation in the accumulators
   ACCSAT = 1;
   
   re = c->re;
   im = c->im;
   
   
   // op = re^2 + im^2;
   #asm
   MOV re, w4
   MOV im, w6
   MPY w4*w4,A
   MPY w6*w6,B
   ADD A   
   MOV 0x24,w5
   MOV w5, op+2
   MOV 0x22,w5
   MOV w5, op
   #endasm
   
   US = 0;//signed
   IF_EN = 0;//fractional
   RND = 1; //convergent rounding
   
   res = 0;
   one = 1 << 30;
   
   while(one > op)
      one >>= 2;
      
   while(one != 0)
   {
      if( op >= res + one)
      {
         op -= res + one; 
         res = (res >> 1) + one;
      }
      else
      {
         res >>= 1;
      }
      
      one >>= 2;
   }
   
   return (unsigned int16)res;
}

// see documentation above
void build_twiddle(Complex* tw, unsigned int16 fft_size)
{
   unsigned int16 i = 0;
   float32 theta = 0;
   float32 d_theta = 0;

   d_theta = 2 * PI / (fft_size);
   for(i = 0;i < fft_size / 2;i++)
   {
      //e^(-j * theta) = cos(theta) - j * sin (theta)
      tw[i].re = (signed int16) (32767.0 * cos(theta));//scale to full range (-32768 to 32767)
      tw[i].im = (signed int16) (-32767.0 * sin(theta));
      theta += d_theta;//increment to the next theta value
   }
}

// see documentation above
void fft_init(void)
{
   build_twiddle(twiddle, FFT_LENGTH);
}

// see documentation above
void fft_init(unsigned int16 fft_size)
{

   build_twiddle(twiddle, fft_size);
}

// see documentation above
void fft_init(Complex* twid_y, unsigned int16 fft_size)
{
   /* Setup the DSP core for signed fractional operation. */
   US = 0;//signed
   US1 = 0;
   IF_EN = 0;//fractional
   RND = 1; //convergent rounding
   
   build_twiddle(twid_y, fft_size);
}

/* Copy data into a destination buffer in bit reversed order. 
 * The destination buffer must be aligned on x000 address for bit reverse
 * addressing to work correctly, ie: 0x1000 
 * This location should also be located in the X-RAM.
 * (Check your device datasheet for the addresses located within X-RAM)
 *
 * This function could be used to copy in order, sampled data from 
 * DMA RAM into a buffer that will be used by the FFT in X RAM.
 * This function MUST be called before running the FFT on a dataset.
 * The input will not be modified. The output must not overlap the input.
 */
void memcpy_brev(Complex* dest, Complex* source, unsigned int16 fft_size)
{
   unsigned int16 xb;
    
   xb = 0x8000 | (fft_size);

   #asm
   push MODCON                   //save the MODCON register
   push XBREV                    //save the XBREV register

   mov #0x01FF, W4
   mov W4, MODCON
   mov xb, W4
   mov W4, XBREV
   
   mov dest, W1                  //W1 = cplx_output; (bit-reversed pointer) 
   mov source, W0                //W0 = cplx_input; (pointer)
   mov #2, W5                    //W5 = sizeof(signed int16*)

   mov fft_size, W4               //W4 = FFT_LENGTH;
   dec W4, W4                    //W4 = FFT_LENGTH - 1;
   do W4, END
      mov [W0], [W1]             //br[k].re = seq[i].re
      mov [W0+W5], [W1+W5]       //br[k].im = seq[i].im
      add #4, W0                 //i++
   END:mov [W1], [W1++]          //k = k_next
   
   pop XBREV                     //restore XBREV
   pop MODCON                    //restore MODCON
   #endasm
}

/* x_data: fft data array used for the in-place, radix-2, DIT FFT.  A pointer 
 * to this memory array is returned by all FFT and IFFT functions.  This
 * array must be as long as the largest data sample to be transformed (nominally
 * 64 complex values).  This array must be located at 0x1000 for bit reversing
 * up to 2048 points, or at 0x0800 for bit reversing up to 1024 points.
 */
Complex x_data[2*FFT_LENGTH];
//#locate x_data = 0x0800  //old
#locate x_data = 0x1000 //fix for EP


/* FFT function:
 * Transforms a series of time-domain samples into the
 * frequency-domain. This algorithm uses a radix-2, DIT FFT.
 * 
 * -This function requires operates on buffers that are exactly a power of 2. 
 *  16384 is the maximum possible number of samples, however,
 *  this much memory may not be available.
 * -Input samples are expected in signed, Q.15-fractional form; natural order
 * -The maximum value any sample should reach is .5 (0x4000)
 *  or overflow may occur.
 * -Samples are output in natural order into frequency bins of size Fs / (2 * N)
 * -This function returns a pointer to the Fourier Transform of cplx_input.
 *
 * cplx_tw are required twiddle factors as generated by build_twiddle().
 * These factors need be generated only once, but must be located in Y RAM.
 */
static Complex* _fft(Complex* cplx_input, Complex* cplx_tw, unsigned int16 fft_size)
{
   unsigned int16 fft_len_div2 = fft_size / 2;
   
   printf("_fft(%x,%x,%x)\r\n", cplx_input, cplx_tw, fft_size); delay_ms(100);
   
   /* Setup the DSP core for signed fractional operation. */
   US = 0;//signed
   US1 = 0;
   IF_EN = 0;//fractional
   RND = 1; //convergent rounding
   
   /* Bit reverse copy into the correct buffer. */
   memcpy_brev(x_data, cplx_input, fft_size);
   
   #asm   
   /* Initialize. */
   mov fft_len_div2, W2 //W2 = TWI = FFT_LENGTH / 2;
   mov fft_size, W14     //W14 = FFT_LENGTH
   SL W14, #2, W14      //W14 = fft_length * sizeof(complex)
   mov #1, W3           //W3 = k_max = 1
   mov #8, W9           //W9 = offset = 2 * sizeof(complex);
   
   STAGE_LOOP:
         clr W11              //for(k = 0;k < k_max;k++)
         K_LOOP:
      
            /* Get the twiddle factor for this k-group. */
            mov cplx_tw, W10     //W10 = cplx_tw (pointer)
            sl W2, #2, W0        //W0 = sizeof(Complex) * offset
            mul.uu W0, W11, W0   //W0 = k * (sizeof(Complex) * offset)
            add W10, W0, W10     //tw = (tw + 1) (pointer)
         
            /* Reset the input pointer. */
            mov x_data, W8       //W8 = cplx_input (pointer) (reset)
            sl W11, #2, W0       //W0 = 4 * k
            add W8, W0, W8       //W8 = (cplx_input + 2 * k) (pointer)
            asr W9, #1, W13      //W13 = offset to lower leg of buttefly
            
            dec W2, W12          //W12 = W2 - 1
            do W12, END_BFLY     //do (twi - 1 + 1) times
               /* Clear the accumulators, prefetch lower-leg */
               clr B, [W8]+=2, W4                        //B = 0, W4 = a.re W8 = &a[i].im
               asr W4, #1, W0                            //W0 = a.re / 2 (This provides the 1 / N factor in the FFT definition)
               clr B, [W8]-=2, W5                        //B = 0, W5 = a.im W8 = a[i - 1].re
               asr W5, #1, W1                            //W1 = a.im / 2 (This provides the 1 / N factor in the FFT definition)
               
               /* Fetch the Upper Leg & Calculate the (b * Wk) product. */
               add W8, W13, W8                           //cplx_input = (cplx_input + offset) (cplx_input = lowerleg)
               clr A, [W8]+=2, W4, [W10]+=2, W6          //A = 0, W4 = b.re W6 = w.re
               mpy W4*W6, A, [W8]-=2, W5, [W10]-=2, W7   //A = (b.re * w.re), W5 = b.im W7 = w.im
               msc W5*W7, A                              //A = b.re * w.re - b.im * w.im = (B * Wk).re    
               mpy W5*W6, B                              //B = b.im * w.re
               mac W4*W7, B                              //B = b.im * w.re + b.re * w.re = (B * Wk).im, W8 = &a[i + 1].im
               sac.r A, #1, W6                           //W6 = t.re / 2 (This provides the 1 / N factor in the FFT definition)
               sac.r B, #1, W7                           //W7 = t.im / 2 (This provides the 1 / N factor in the FFT definition)
               
               /* Calculate output legs (X = a + (b * Wk), Y = a - (b * Wk)) */
               /* Calculate Y */
               push.s                                    //save a.re & a.im
               sub W0, W6, W0                            //Y.re = .5 * a.re - .5 * t.re
               sub W1, W7, W1                            //Y.im = .5 * a.im - .5 * t.im              
               mov W0, [W8++]                            //*(cplx_input + offset / 2) = Y.re                         
               mov W1, [W8--]                            //*(cplx_input + offset / 2 + 2) = Y.im
               sub W8, W13, W8                           //W8 -> A
               
               /* Calculate X */
               pop.s                                     //restore a.re & a.im
               add W0, W6, W0                            //X.re = .5 * a.re + .5 * t.re
               add W1, W7, W1                            //X.im = .5 * a.im + .5 * t.im
               mov W0, [W8++]                            //*(cplx_input) = X.re               
               mov W1, [W8--]                            //*(cplx_input + 2) = X.im
               
            END_BFLY:  add W8, W9, W8                    //W8 -> next butterfly upper leg
         
      inc W11, W11
      cp W11, W3
      bra n, K_LOOP        //for(k = 0;k < k_max;k++)
      
      sl W3, #1, W3        //k_max *= 2
      sl W9, #1, W9        //offset *= 2
      asr W2, #1, W2       //twi /= 2
       
   cp W9, W14              //while(offset <= FFT_LENGTH) 
   bra LE, STAGE_LOOP
   #endasm
   
   /* Scale up FFT by 2 
    * 
    * This is purely for mathematical accuracy and can be commented out
    * if a relative, rather than absolute, FFT scale is sufficient
    **/
//!   unsigned int16 fft_len_x2 = fft_size * 2;    
//!   #asm
//!   mov x_data, W8       //W8 = cplx_input (pointer) (reset)
//!   mov #2,w1            //multiplicand
//!   mov fft_len_x2, w0   //w0 = 2 * FFT_LENGTH
//!   dec w0,w0            //Decrement w0
//!   do w0, mul2          //Do next 2 instructions 2 * FFT_LENGTH times
//!   mul.us w1,[w8],w2    //w2,w3 = w1*[w8] (2 * x_data[i])
//!   mul2: mov w2,[w8++]  //[w8] = w2, w8++
//!   #endasm   
   
   return x_data;
}

#if defined(__PCD_QUIRK_OVERLOAD__)
// see documentation above
Complex* fft(Complex* cplx_input, Complex* cplx_tw, unsigned int16 fft_size)
{
   return _fft(cplx_input, cplx_tw, fft_size);
}

// see documentation above
Complex* fft(Complex* cplx_input, unsigned int16 fft_size)
{
   return _fft(cplx_input, twiddle, fft_size);
}

// see documentation above
Complex* fft(Complex* cplx_input)
{
   return _fft(cplx_input, twiddle, FFT_LENGTH);
}
#else
#define fft(pComplex,fftSize) _fft(pComplex, twiddle, fftSize)
#endif


/* IFFT function:
 * Transforms a series of complex frequency-domain samples into the
 * time-domain. This algorithm uses a radix-2, DIT IFFT.
 * 
 * -This function requires operates on buffers that are exactly a power of 2. 
 *  16384 is the maximum possible number of samples, however,
 *  this much memory may not be available.
 * -Input samples are expected in signed, Q.15-fractional form; natural order
 * -The maximum value any sample should reach is .5 (0x4000)
 *  or overflow may occur.
 * -Samples are output in natural-time order.
 * -This function returns a pointer to the I-Fourier Transform of cplx_input.
 *
 * cplx_tw are required twiddle factors as generated by build_twiddle().
 * These factors need be generated only once, but must be located in Y RAM.
 */
static Complex* _ifft(Complex* cplx_input, Complex* cplx_tw, unsigned int16 fft_size)
{
   unsigned int16 fft_len_div2 = fft_size / 2;
   
   /* Setup the DSP core for signed fractional operation. */
   US = 0;//signed
   US1 = 0;
   IF_EN = 0;//fractional
   RND = 1; //convergent rounding
   
   /* Bit reverse copy into the correct buffer. */
   memcpy_brev(x_data, cplx_input, fft_size);
   
   #asm   
   /* Initialize. */
   mov fft_len_div2, W2 //W2 = TWI = FFT_LENGTH / 2;
   mov fft_size, W14     //W14 = FFT_LENGTH
   SL W14, #2, W14      //W14 = fft_length * sizeof(complex)
   mov #1, W3           //W3 = k_max = 1
   mov #8, W9           //W9 = offset = 2 * sizeof(complex);
   
   STAGE_LOOP:
         clr W11              //for(k = 0;k < k_max;k++)
         K_LOOP:
      
            /* Get the twiddle factor for this k-group. */
            mov cplx_tw, W10     //W10 = cplx_tw (pointer)
            sl W2, #2, W0        //W0 = sizeof(Complex) * offset
            mul.uu W0, W11, W0   //W0 = k * (sizeof(Complex) * offset)
            add W10, W0, W10     //tw = (tw + 1) (pointer)
         
            /* Reset the input pointer. */
            mov x_data, W8       //W8 = cplx_input (pointer) (reset)
            sl W11, #2, W0       //W0 = 4 * k
            add W8, W0, W8       //W8 = (cplx_input + 2 * k) (pointer)
            asr W9, #1, W13      //W13 = offset to lower leg of buttefly
            
            dec W2, W12          //W12 = W2 - 1
            do W12, END_BFLY     //do (twi - 1 + 1) times
               /* Prefetch lower-leg */               
               mov [W8++],W0                             //W0 = b.re (no scaling)
               mov [W8--],W1                             //W1 = b.im (no scaling)
               
               /* Fetch the Upper Leg & Calculate the (b * Wk) product. */
               add W8, W13, W8                           //cplx_input = (cplx_input + offset) (cplx_input = lowerleg)
               clr A, [W8]+=2, W4, [W10]+=2, W6          //A = 0, W4 = b.re W6 = w.re
               mpy W4*W6, A, [W8]-=2, W5, [W10]-=2, W7   //A = (b.re * w.re), W5 = b.im W7 = w.im
               mac W5*W7, A                              //A = b.re * w.re + b.im * w.im = (B * Wk).re    
               mpy W5*W6, B                              //B = b.im * w.re
               msc W4*W7, B                              //B = b.im * w.re - b.re * w.im = (B * Wk).im, W8 = &a[i + 1].im
               sac.r A, #0, W6                           //W6 = t.re (No Scaling)
               sac.r B, #0, W7                           //W7 = t.im (No Scaling)
               
               /* Calculate output legs (X = a + (b * Wk), Y = a - (b * Wk)) */
               /* Calculate Y */
               push.s                                    //save a.re & a.im
               sub W0, W6, W0                            //Y.re = .5 * a.re - .5 * t.re
               sub W1, W7, W1                            //Y.im = .5 * a.im - .5 * t.im              
               mov W0, [W8++]                            //*(cplx_input + offset / 2) = Y.re                         
               mov W1, [W8--]                            //*(cplx_input + offset / 2 + 2) = Y.im
               sub W8, W13, W8                           //W8 -> A
               
               /* Calculate X */
               pop.s                                     //restore a.re & a.im
               add W0, W6, W0                            //X.re = .5 * a.re + .5 * t.re
               add W1, W7, W1                            //X.im = .5 * a.im + .5 * t.im
               mov W0, [W8++]                            //*(cplx_input) = X.re               
               mov W1, [W8--]                            //*(cplx_input + 2) = X.im
               
            END_BFLY:  add W8, W9, W8                    //W8 -> next butterfly upper leg
         
      inc W11, W11
      cp W11, W3
      bra n, K_LOOP        //for(k = 0;k < k_max;k++)
      
      sl W3, #1, W3        //k_max *= 2
      sl W9, #1, W9        //offset *= 2
      asr W2, #1, W2       //twi /= 2
       
   cp W9, W14              //while(offset <= FFT_LENGTH) 
   bra LE, STAGE_LOOP
   #endasm
   
   /* Scale down IFFT by 2
    * 
    * This is purely presented for mathematical accuracy and can be commented 
    * out if the scale up in the FFT function is also commented out.
    **/
//!   unsigned int16 fft_len_x2 = fft_size * 2;    
//!   #asm
//!   mov x_data, W8       //W8 = cplx_input (pointer) (reset)
//!   clr A, [w8],W6
//!   mov #0x4000,W4
//!   mov fft_len_x2, w0   //w0 = 2 * FFT_LENGTH
//!   dec w0,w0            //Decrement w0
//!   do w0, div2          //Do next 2 instructions 2 * FFT_LENGTH times
//!   mpy w4*w6, A
//!   sac.r A,[w8++]
//!   div2: clr A, [w8],W6
//!   #endasm   
   
   return x_data;
}

Complex* ifft(Complex* cplx_input)
{
   return _ifft(cplx_input, twiddle, FFT_LENGTH);
}

// see documentation above
Complex* ifft(Complex* cplx_input, unsigned int16 fft_size)
{
   return _ifft(cplx_input, twiddle, fft_size);
}

// see documentation above
Complex* ifft(Complex* cplx_input, Complex* cplx_tw, unsigned int16 fft_size)
{
   return _ifft(cplx_input, cplx_tw, fft_size);
}

#endif
