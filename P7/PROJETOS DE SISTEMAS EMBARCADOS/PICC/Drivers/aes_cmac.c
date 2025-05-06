///////////////////////////////////////////////////////////////////////////////
////                              aes_cmac.c                               ////
////                                                                       ////
////  Driver for AES-CMAC.                                                 ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////                                                                       ////
////  Preprocessors:                                                       ////
////                                                                       ////
////     AES_USE_GF28MULTIPLY_TABLES - sets whether to use lookup tables   ////
////        to perform GF (2^8) multiplication or a function.  TRUE uses   ////
////        lookup tables or FALSE uses function, uses less program memory ////
////        but takes longer to generate the MAC.  Default is TRUE if not  ////
////        defined before driver is included.                             ////
////                                                                       ////
////  API:                                                                 ////
////                                                                       ////
////     init_keys() - used to initialize the key and subkeys, must be     ////
////        called before any other functions are called or anytime the    ////
////        key is changed.                                                ////
////                                                                       ////
////     generate_mac() - used to generate the MAC for the data being      ////
////        passed to function.                                            ////
////                                                                       ////
////     verify_mac() - used to verify the MAC is correct for data passed  ////
////        to function.                                                   ////
////                                                                       ////
////  * This driver can also be used at the same time as the aes.c driver  ////
////    as long as the aes.c driver is include in your project before this ////
////    driver is included.                                                ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2020 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

#ifndef __AES_CMAC_C__
#define __AES_CMAC_C__

#include <stdint.h>

#ifndef AES_USE_GF28MULTIPLY_TABLES
 #define AES_USE_GF28MULTIPLY_TABLES   TRUE
#endif

#ifndef __AES_C__
 const uint8_t SBox[256] = {0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
                            0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
                            0xB7,0xFD,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
                            0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
                            0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
                            0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
                            0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
                            0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
                            0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
                            0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
                            0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
                            0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
                            0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
                            0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
                            0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
                            0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};


 #if AES_USE_GF28MULTIPLY_TABLES == TRUE
  const uint8_t GF28Multiply2[256] = {0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
                                      0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E,0x30,0x32,0x34,0x36,0x38,0x3A,0x3C,0x3E,
                                      0x40,0x42,0x44,0x46,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56,0x58,0x5A,0x5C,0x5E,
                                      0x60,0x62,0x64,0x66,0x68,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x76,0x78,0x7A,0x7C,0x7E,
                                      0x80,0x82,0x84,0x86,0x88,0x8A,0x8C,0x8E,0x90,0x92,0x94,0x96,0x98,0x9A,0x9C,0x9E,
                                      0xA0,0xA2,0xA4,0xA6,0xA8,0xAA,0xAC,0xAE,0xB0,0xB2,0xB4,0xB6,0xB8,0xBA,0xBC,0xBE,
                                      0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE,0xD0,0xD2,0xD4,0xD6,0xD8,0xDA,0xDC,0xDE,
                                      0xE0,0xE2,0xE4,0xE6,0xE8,0xEA,0xEC,0xEE,0xF0,0xF2,0xF4,0xF6,0xF8,0xFA,0xFC,0xFE,
                                      0x1B,0x19,0x1F,0x1D,0x13,0x11,0x17,0x15,0x0B,0x09,0x0F,0x0D,0x03,0x01,0x07,0x05,
                                      0x3B,0x39,0x3F,0x3D,0x33,0x31,0x37,0x35,0x2B,0x29,0x2F,0x2D,0x23,0x21,0x27,0x25,
                                      0x5B,0x59,0x5F,0x5D,0x53,0x51,0x57,0x55,0x4B,0x49,0x4F,0x4D,0x43,0x41,0x47,0x45,
                                      0x7B,0x79,0x7F,0x7D,0x73,0x71,0x77,0x75,0x6B,0x69,0x6F,0x6D,0x63,0x61,0x67,0x65,
                                      0x9B,0x99,0x9F,0x9D,0x93,0x91,0x97,0x95,0x8B,0x89,0x8F,0x8D,0x83,0x81,0x87,0x85,
                                      0xBB,0xB9,0xBF,0xBD,0xB3,0xB1,0xB7,0xB5,0xAB,0xA9,0xAF,0xAD,0xA3,0xA1,0xA7,0xA5,
                                      0xDB,0xD9,0xDF,0xDD,0xD3,0xD1,0xD7,0xD5,0xCB,0xC9,0xCF,0xCD,0xC3,0xC1,0xC7,0xC5,
                                      0xFB,0xF9,0xFF,0xFD,0xF3,0xF1,0xF7,0xF5,0xEB,0xE9,0xEF,0xED,0xE3,0xE1,0xE7,0xE5};
  
  const uint8_t GF28Multiply3[256] = {0x00,0x03,0x06,0x05,0x0C,0x0F,0x0A,0x09,0x18,0x1B,0x1E,0x1D,0x14,0x17,0x12,0x11,
                                      0x30,0x33,0x36,0x35,0x3C,0x3F,0x3A,0x39,0x28,0x2B,0x2E,0x2D,0x24,0x27,0x22,0x21,
                                      0x60,0x63,0x66,0x65,0x6C,0x6F,0x6A,0x69,0x78,0x7B,0x7E,0x7D,0x74,0x77,0x72,0x71,
                                      0x50,0x53,0x56,0x55,0x5C,0x5F,0x5A,0x59,0x48,0x4B,0x4E,0x4D,0x44,0x47,0x42,0x41,
                                      0xC0,0xC3,0xC6,0xC5,0xCC,0xCF,0xCA,0xC9,0xD8,0xDB,0xDE,0xDD,0xD4,0xD7,0xD2,0xD1,
                                      0xF0,0xF3,0xF6,0xF5,0xFC,0xFF,0xFA,0xF9,0xE8,0xEB,0xEE,0xED,0xE4,0xE7,0xE2,0xE1,
                                      0xA0,0xA3,0xA6,0xA5,0xAC,0xAF,0xAA,0xA9,0xB8,0xBB,0xBE,0xBD,0xB4,0xB7,0xB2,0xB1,
                                      0x90,0x93,0x96,0x95,0x9C,0x9F,0x9A,0x99,0x88,0x8B,0x8E,0x8D,0x84,0x87,0x82,0x81,
                                      0x9B,0x98,0x9D,0x9E,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8F,0x8C,0x89,0x8A,
                                      0xAB,0xA8,0xAD,0xAE,0xA7,0xA4,0xA1,0xA2,0xB3,0xB0,0xB5,0xB6,0xBF,0xBC,0xB9,0xBA,
                                      0xFB,0xF8,0xFD,0xFE,0xF7,0xF4,0xF1,0xF2,0xE3,0xE0,0xE5,0xE6,0xEF,0xEC,0xE9,0xEA,
                                      0xCB,0xC8,0xCD,0xCE,0xC7,0xC4,0xC1,0xC2,0xD3,0xD0,0xD5,0xD6,0xDF,0xDC,0xD9,0xDA,
                                      0x5B,0x58,0x5D,0x5E,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4F,0x4C,0x49,0x4A,
                                      0x6B,0x68,0x6D,0x6E,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7F,0x7C,0x79,0x7A,
                                      0x3B,0x38,0x3D,0x3E,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2F,0x2C,0x29,0x2A,
                                      0x0B,0x08,0x0D,0x0E,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1F,0x1C,0x19,0x1A};
 #endif
 
 const uint8_t rCon[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};

 typedef union
 {
   uint8_t b[4];
   uint32_t dw;
 } uint32_union_t;

 #if AES_USE_GF28MULTIPLY_TABLES == FALSE
  uint8_t MultiplyGF28(uint8_t a, uint8_t b);
 #endif
 void ShiftRows(uint32_union_t *Data, uint8_t Row, uint8_t Bytes, int1 IsInverse=FALSE);
 void MixColumns(uint32_union_t *Data, uint8_t Column);
#endif

uint32_t RoundKeyCMAC[11][4];

uint8_t K1[16];
uint8_t K2[16];

void GenerateSubkeys(void);

///////////////////////////////////////////////////////////////////////////////
// init_keys()
//
// Initializes the key and subkeys used to generate and verify the MAC, must be
// called before any other functions or anytime the key is changed.
//
// Parameters:
//    Key - a pointer pointing to the array contain the key to generate and 
//          verify the MAC.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void init_keys(uint8_t *Key)
{
   uint8_t i, j, k;
   uint32_union_t rValue;
   uint32_union_t sValue;
   uint32_union_t xValue;
   
   RoundKeyCMAC[0][0] = make32(Key[3], Key[2], Key[1], Key[0]);
   RoundKeyCMAC[0][1] = make32(Key[7], Key[6], Key[5], Key[4]);
   RoundKeyCMAC[0][2] = make32(Key[11], Key[10], Key[9], Key[8]);
   RoundKeyCMAC[0][3] = make32(Key[15], Key[14], Key[13], Key[12]);
   
   for(i=1,j=0;i<11;i++,j++)
   {
      rValue.dw = RoundKeyCMAC[i-1][3];
      for(k=0;k<8;k++)
         rotate_right(rValue.b, 4);
      
      for(k=0;k<4;k++)
         sValue.b[k] = SBox[rValue.b[k]];
      
      xValue.dw = sValue.dw;
      xValue.b[0] ^= rCon[j];
      
      RoundKeyCMAC[i][0] = RoundKeyCMAC[j][0] ^ xValue.dw;
      RoundKeyCMAC[i][1] = RoundKeyCMAC[i][0] ^ RoundKeyCMAC[j][1];
      RoundKeyCMAC[i][2] = RoundKeyCMAC[i][1] ^ RoundKeyCMAC[j][2];
      RoundKeyCMAC[i][3] = RoundKeyCMAC[i][2] ^ RoundKeyCMAC[j][3];
   }
   
   GenerateSubkeys();
}

///////////////////////////////////////////////////////////////////////////////
// generate_mac()
//
// Used to generate the MAC from the data being passed to it.  
// 
// Parameters:
//    Data - array containing data to generate the MAC for.
//
//    Count - the number of data bytes to generate the MAC for.
//
//    MAC - array to return the generated MAC value to, size must be at least
//          16 bytes.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void generate_mac(uint8_t *Data, uint16_t Count, uint8_t *MAC)
{
   union
   {
      uint8_t b[16];
      uint32_union_t u[4];
   } eData;
   uint8_t i,j,k;
   uint8_t Loops;
   uint16_t dCount = 0;
   uint8_t Index;
   int1 UseK2 = FALSE;
   uint8_t *kPtr;
   
   Loops = Count / 16;
   
   if((Count == 0) || ((Count % 16) != 0))
   {
      Loops++;
      UseK2 = TRUE;
   }
   
   for(i=0;i<Loops;i++)
   {
      for(j=0;j<4;j++)
      {
         for(k=0;k<4;k++,dCount++)
         {
            if(dCount == Count)
               eData.u[j].b[k] = 0x80;
            else if(dCount > Count)
               eData.u[j].b[k] = 0x00;
            else
               eData.u[j].b[k] = Data[dCount];
         }
      }
      
      if(i != 0)
      {
         for(j=0,Index=0;j<4;j++)
         {
            for(k=0;k<4;k++,Index++)
               eData.u[j].b[k] ^= MAC[Index];
         }
      }
      
      if(i == (Loops - 1))
      {
         if(UseK2)
            kPtr = K2;
         else
            kPtr = K1;
            
         for(j=0;j<4;j++)
         {
            for(k=0;k<4;k++)
               eData.u[j].b[k] ^= kPtr[(j*4) + k];
         }
      }
      
      eData.u[0].dw ^= RoundKeyCMAC[0][0];
      eData.u[1].dw ^= RoundKeyCMAC[0][1];
      eData.u[2].dw ^= RoundKeyCMAC[0][2];
      eData.u[3].dw ^= RoundKeyCMAC[0][3];
      
      for(j=1;j<11;j++)
      {
         //SubBytes
         for(k=0;k<16;k++)
            eData.b[k] = SBox[eData.b[k]];
         
         //ShiftRows
         ShiftRows((uint32_union_t *)&eData, 1, 1);
         ShiftRows((uint32_union_t *)&eData, 2, 2);
         ShiftRows((uint32_union_t *)&eData, 3, 3);
         
         if(j < 10)
         {
            //MixColumns
            for(k=0;k<4;k++)
               MixColumns((uint32_union_t *)&eData, k);
         }
         
         //RoundKey
         eData.u[0].dw ^= RoundKeyCMAC[j][0];
         eData.u[1].dw ^= RoundKeyCMAC[j][1];
         eData.u[2].dw ^= RoundKeyCMAC[j][2];
         eData.u[3].dw ^= RoundKeyCMAC[j][3];
      }
      
      for(j=0;j<16;j++)
         MAC[j] = eData.b[j];
   }
}

///////////////////////////////////////////////////////////////////////////////
// verify_mac()
//
// Used to verify the MAC is correct for the Data.
//
// Parameters:
//    Data - pointer to an array contain the data the MAC is for.
//
//    Count - number of data byes the MAC is for.
//
//    rMAC - pointer to an array contain the MAC that is being verified.
//
//    vBytes - optional specifying the number of bytes of MAC to verify, default
//             is 16 bytes.
//
// Returns:
//    TRUE - MAC was verified.
//    FALSE - MAC was not verified, MAC is not correct for supplied data and 
//            and byte count.
///////////////////////////////////////////////////////////////////////////////
int1 verify_mac(uint8_t *Data, uint16_t Count, uint8_t *rMAC, uint8_t vBytes=16)
{
   uint8_t gMAC[16];
   
   generate_mac(Data, Count, gMAC);
   
   if(memcmp(gMAC, rMAC, vBytes) == 0)
      return(TRUE);
   else
      return(FALSE);
}

/////////////////////////////// Helper Functions //////////////////////////////

void GenerateSubkeys(void)
{
   uint8_t Data[16];
   uint8_t *dPtr;
   int8_t i;
   int1 DoXor;
   int1 Bit;
   
   memset(K1, 0x00, 16);
   memset(Data, 0x00, 16);
   
   generate_mac(Data, 16, Data);
   
   if(bit_test(Data[0], 7))
      DoXor = TRUE;
   else
      DoXor = FALSE;
   
   Bit = 0;
   dPtr = &Data[15];
   
   for(i=15;i>=0;i--,dPtr--)
      Bit = shift_left(dPtr, 1, Bit);
      
   memcpy(K1, Data, 16);
   
   if(DoXor)
      K1[15] = K1[15] ^ 0x87;
   
   if(bit_test(K1[0], 7))
      DoXor = TRUE;
   else
      DoXor = FALSE;
   
   memcpy(K2, K1, 16);
   
   Bit = 0;
   dPtr = &K2[15];
   
   for(i=15;i>=0;i--,dPtr--)
      Bit = shift_left(dPtr, 1, Bit);
   
   if(DoXor)
      K2[15] = K2[15] ^ 0x87;
}

#ifndef __AES_C__
 #if AES_USE_GF28MULTIPLY_TABLES == FALSE
 /* 
   MultiplyGF28() performs a GF(2^8) multiplication of 2 bytes.
 */
 uint8_t MultiplyGF28(uint8_t a, uint8_t b)
 {
   uint8_t i;
   uint8_t p;
   
   for(i=0,p=0;i<7;i++)
   {
      if((a == 0) || (b == 0))
         break;
      
      if(bit_test(b, 0))
         p ^= a;
      
      b >>= 1;
      
      if(shift_left(&a, 1, 0))
         a ^= 0x1B;
   }
   
   if((a != 0) && (b != 0))   //if b isn't 0 then bit 0 has to be a 1 at this point
      p ^= a;
   
   return(p);
 }
 #endif

 /*
   ShiftRows() - used to do the shift row portion of the encryption.
 */
 void ShiftRows(uint32_union_t *Data, uint8_t Row, uint8_t Bytes, int1 IsInverse=FALSE)
 {
   uint32_union_t Temp;
   uint8_t bTemp;
   
   Temp.dw = make32(Data[0].b[Row], Data[1].b[Row], Data[2].b[Row], Data[3].b[Row]);
   
   switch(Bytes)
   {
      case 1: 
         if(IsInverse)
         {
            bTemp = Temp.b[0];
            Temp.b[0] = Temp.b[1];
            Temp.b[1] = Temp.b[2];
            Temp.b[2] = Temp.b[3];
            Temp.b[3] = bTemp;
         }
         else
         {
            bTemp = Temp.b[3];
            Temp.b[3] = Temp.b[2];
            Temp.b[2] = Temp.b[1];
            Temp.b[1] = Temp.b[0];
            Temp.b[0] = bTemp;
         }
         break;
      case 2:
         bTemp = Temp.b[3];
         Temp.b[3] = Temp.b[1];
         Temp.b[1] = bTemp;
         bTemp = Temp.b[2];
         Temp.b[2] = Temp.b[0];
         Temp.b[0] = bTemp;
         break;
      case 3:
         if(IsInverse)
         {
            bTemp = Temp.b[3];
            Temp.b[3] = Temp.b[2];
            Temp.b[2] = Temp.b[1];
            Temp.b[1] = Temp.b[0];
            Temp.b[0] = bTemp;
         }
         else
         {
            bTemp = Temp.b[3];
            Temp.b[3] = Temp.b[0];
            Temp.b[0] = Temp.b[1];
            Temp.b[1] = Temp.b[2];
            Temp.b[2] = bTemp;
         }
         break;
   }
   
   Data[0].b[Row] = Temp.b[3];
   Data[1].b[Row] = Temp.b[2];
   Data[2].b[Row] = Temp.b[1];
   Data[3].b[Row] = Temp.b[0];
 }

 /*
   MixColumns() - used to do the mix column portion of the encryption.
 */
 void MixColumns(uint32_union_t *Data, uint8_t Column)
 {
   uint32_union_t Temp;
   
   Temp.dw = Data[Column].dw;

  #if AES_USE_GF28MULTIPLY_TABLES == TRUE
   Data[Column].b[0] = GF28Multiply2[Temp.b[0]] ^ GF28Multiply3[Temp.b[1]] ^ Temp.b[2] ^ Temp.b[3];
   Data[Column].b[1] = Temp.b[0] ^ GF28Multiply2[Temp.b[1]] ^ GF28Multiply3[Temp.b[2]] ^ Temp.b[3];
   Data[Column].b[2] = Temp.b[0] ^ Temp.b[1] ^ GF28Multiply2[Temp.b[2]] ^ GF28Multiply3[Temp.b[3]];
   Data[Column].b[3] = GF28Multiply3[Temp.b[0]] ^ Temp.b[1] ^ Temp.b[2] ^ GF28Multiply2[Temp.b[3]];
  #else
   Data[Column].b[0] = MultiplyGF28(2, Temp.b[0]) ^ MultiplyGF28(3, Temp.b[1]) ^ Temp.b[2] ^ Temp.b[3];
   Data[Column].b[1] = Temp.b[0] ^ MultiplyGF28(2, Temp.b[1]) ^ MultiplyGF28(3, Temp.b[2]) ^ Temp.b[3];
   Data[Column].b[2] = Temp.b[0] ^ Temp.b[1] ^ MultiplyGF28(2, Temp.b[2]) ^ MultiplyGF28(3, Temp.b[3]);
   Data[Column].b[3] = MultiplyGF28(3, Temp.b[0]) ^ Temp.b[1] ^ Temp.b[2] ^ MultiplyGF28(2, Temp.b[3]);
  #endif
 }
#endif

#endif
