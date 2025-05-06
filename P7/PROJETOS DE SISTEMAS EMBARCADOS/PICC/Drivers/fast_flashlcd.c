///////////////////////////////////////////////////////////////////////////
////                          fast_flashlcd.c                          ////
////                                                                   ////
////  This driver is designed to be used with the gfx_graphics.c       ////
////  driver.  It adds the define USE_FAST_DRAW and the functions      ////
////  DrawBlock() and WriteFastImage() so the gfx_graphics.c driver    ////
////  can draw images and other graphics onto the LCD faster.  It also ////
////  adds the WriteFastVerticalImage() function if the define         ////
////  USE_FAST_VERTICAL_DRAW is made.  This driver is hardware         ////
////  specific and will only work when using both the S1D131517        ////
////  Display controller, and the S34ML02G104T external Flash Memory.  ////
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

#include <S1D13517.h>
#include <S34ML02G104T.h>

#include <gfx_graphics.h>

#include <stdlibm.h>

#define USE_FAST_DRAW

#inline
void FlashLCDReadWrite(void)
{
   output_low(FLASH_RE);
   output_high(FLASH_RE);
   glcd_TransferData();
}

void DrawBlock(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, uint16_t Color)
{
   uint32_t Pixels;
   uint32_t i;
   uint16_t LineSwitch;
   uint16_t CurrentX, CurrentY, StopX;
   uint16_t *Data = NULL;
   uint16_t iColor;
   uint16_t aWidth, ActualX;
   
   Pixels = (uint32_t)Width * (uint32_t)Height;
   
   if(((StartX % 8) == 0) && ((Width % 8) == 0))
   {
      if((Pixels % 64) == 0)
         LineSwitch = 64;
      else if ((Pixels % 32) == 0)
         LineSwitch = 32;
      else if((Pixels % 16) == 0)
         LineSwitch = 16;
      else if((Pixels % 8) == 0)
         LineSwitch = 8;
      else if((Pixels % 4) == 0)
         LineSwitch = 4;
      else if((Pixels % 2) == 0)
         LineSwitch = 2;
      else
         LineSwitch = 1;
         
      output_low(GLCD_CS);
         
      glcd_WriteRegister(GLCD_WRITE_X_START_POS, StartX / 4, FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_0, (StartY >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_1, (StartY & 0x03), FALSE);
      
      glcd_WriteRegister(GLCD_WRITE_X_END_POS, ((StartX / 4) + (Width / 4) - 2), FALSE);
         
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_0, (((StartY + Height) - 1) >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_1, (((StartY + Height) - 1) & 0x03), FALSE);
      
      GLCDDataOutput();
      
      GLCD_COMMAND();   
      glcd_WritePortRegister(GLCD_MEMORY_DATA_PORT_0);
      
      GLCD_DATA();
      
      GLCDWritePort(Color);
      
      i = 0;
      
      do
      {
         switch(LineSwitch)
         {
            case 64:
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
            case 32:
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
            case 16:
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
            case 8:
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
               glcd_TransferData();
            case 4:
               glcd_TransferData();
               glcd_TransferData();
            case 2:
               glcd_TransferData();
            default:
               glcd_TransferData();
               break;
         }
         
         i += (uint32_t)LineSwitch;
         
      } while(i < Pixels);
      
      output_high(GLCD_CS);
   }
   else
   {
      aWidth = Width + (8 - (Width % 8));
      StopX = StartX + Width;
      
      if((StartX % 8) != 0)
         ActualX = StartX - (StartX % 8);
      else
         ActualX = StartX;
         
      if((ActualX + aWidth) < StopX)
         aWidth += 8;
      
      Data = malloc(aWidth * 2);
      
      if(Data != NULL)
      {
         iColor = Color ^ 0xFFFF;
         
         for(i=0,CurrentX=ActualX;i<aWidth;i++,CurrentX++)
         {
            if((CurrentX >= StartX) && (CurrentX < StopX))
               Data[i] = Color;
            else
               Data[i] = iColor;
         }
         
         output_low(GLCD_CS);
         
         glcd_WriteRegister(GLCD_INPUT_MODE, 0x08, FALSE);
               
         glcd_WriteRegister(GLCD_TRANS_KEY_COLOR_RED, make8(iColor, 1) & 0xF8, FALSE);
         glcd_WriteRegister(GLCD_TRANS_KEY_COLOR_GREEN, (iColor & 0x07E0) >> 3, FALSE);
         glcd_WriteRegister(GLCD_TRANS_KEY_COLOR_BLUE, (make8(iColor, 0) & 0x1F) << 3, FALSE);
         
         output_high(GLCD_CS);
         
         CurrentY = StartY;
         
         for(i=0;i<Height;i++)
         {
            glcd_DrawPixels(ActualX, CurrentY, aWidth, 1, Data);
            CurrentY++;
         }
         
         glcd_WriteRegister(GLCD_INPUT_MODE, 0x00, TRUE);
         
         free(Data);
      }
      else
      {
         CurrentX = StartX;
         CurrentY = StartY;
         StopX = StartX + Width;
         
         i = 0;
         
         while(i < Pixels)
         {
            glcd_DrawPixel(CurrentX, CurrentY, Color);
            
            if(++CurrentX >= StopX)
            {
               CurrentY++;
               CurrentX = StartX;
            }
            
            i++;
         }
      }
   }
}

void WriteFastImage(IMAGE *ImageData, uint16_t StartX, uint16_t StartY)
{
   uint32_t Pixels;
   uint32_t Count;
   uint16_t WriteCount;
   uint16_t aWidth, aHeight;
   FLASH_ADDR Address;
   uint8_t AddressArray[5];
   uint16_t PrevBlock;
   uint16_t *PixelData;
   uint16_t CurrentX, CurrentY, EndX;
   uint16_t Pixel;
   uint16_t i;
   
   if((StartX + ImageData->Header.Width) > GLCD_PIXELS)
      aWidth = GLCD_PIXELS - StartX;
   else
      aWidth = ImageData->Header.Width;
      
   if((StartY + ImageData->Header.Height) > GLCD_LINES)
      aHeight = GLCD_LINES - StartY;
   else
      aHeight = ImageData->Header.Height;
      
   memcpy(&Address, &ImageData->BitmapAddress, sizeof(FLASH_ADDR));

   if(((StartX % MINIMUM_WRITE_WIDTH) == 0) && ((aWidth % MINIMUM_WRITE_WIDTH) == 0) &&
      (aWidth == ImageData->Header.Width))
   {
      Pixels = (uint32_t)aWidth * (uint32_t)aHeight;
      
      if((Pixels % 256) == 0)
         WriteCount = 256;
      else if((Pixels % 128) == 0)
         WriteCount = 128;
      else if((Pixels % 64) == 0)
         WriteCount = 64;
      else if((Pixels % 32) == 0)
         WriteCount = 32;
      else if((Pixels % 16) == 0)
         WriteCount = 16;
      else if((Pixels % 8) == 0)
         WriteCount = 8;
      else if((Pixels % 4) == 0)
         WriteCount = 4;
      else if((Pixels % 2) == 0)
         WriteCount = 2;
      else
         WriteCount = 1;
      
      //Setup Write to GLCD
      output_low(GLCD_CS);
   
      glcd_WriteRegister(GLCD_WRITE_X_START_POS, StartX / 4, FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_0, (StartY >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_1, (StartY & 0x03), FALSE);
      
      glcd_WriteRegister(GLCD_WRITE_X_END_POS, ((StartX / 4) + (aWidth / 4) - 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_0, (((StartY + aHeight) - 1) >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_1, (((StartY + aHeight) - 1) & 0x03), FALSE);
      
      GLCDDataOutput();
      
      GLCD_COMMAND();   
      glcd_WritePortRegister(GLCD_MEMORY_DATA_PORT_0);
   
      GLCD_DATA();
      
      //Setup Read from Flash
      flash_GetAddressArray(Address, AddressArray);
      
      output_low(FLASH_CE);
      
      flash_LatchCommand(FLASH_CMD_READ);
      
      flash_LatchAddress(AddressArray);
      
      flash_LatchCommand(FLASH_CMD_READ_PAGE);
            
      while(!input(FLASH_BUSY));
      
      flash_LatchCommand(FLASH_CMD_READ_CACHE);
      
      while(!input(FLASH_BUSY));
      
      FlashPortInput();
      
      Count = 0;
      PrevBlock = Address.Block;
      
      //Read and Write Data from Flash and to GLCD
      do
      {
         switch(WriteCount)
         {
            case 256:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 128:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 64:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 32:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 16:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 8:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 4:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 2:
               FlashLCDReadWrite();
            default:
               FlashLCDReadWrite();
         }
         
         Count += (uint32_t)WriteCount;
         flash_IncAddressColumn(&Address, WriteCount);
         
         if(Address.Column == 0)
         {
            if(Address.Block != PrevBlock)
            {
               flash_GetAddressArray(Address, AddressArray);
               
               flash_LatchCommand(FLASH_CMD_READ);
         
               flash_LatchAddress(AddressArray);
               
               flash_LatchCommand(FLASH_CMD_READ_PAGE);
               
               while(!input(FLASH_BUSY));
               
               flash_LatchCommand(FLASH_CMD_READ_CACHE);
               
               PrevBlock = Address.Block;
            }
            else
               flash_LatchCommand(FLASH_CMD_READ_CACHE);
            
            while(!input(FLASH_BUSY));
            
            FlashPortInput();
         }
         
      } while(Count < Pixels);
      
      flash_LatchCommand(FLASH_CMD_CATCH_END);
      
      while(!input(FLASH_BUSY));
      
      output_high(FLASH_CE);
      
      output_high(GLCD_CS);
   }
   else
   {
      PixelData = malloc(aWidth * 2);
      
      if(PixelData != NULL)
      {
         for(i=0,CurrentY=StartY;i<aHeight;i++,CurrentY++)
         {
            flash_ReadData(Address, PixelData, aWidth);
            glcd_DrawPixels(StartX, CurrentY, aWidth, 1, PixelData);
            flash_IncAddressColumn(&Address, ImageData->Header.Width);
         }
         
         free(PixelData);
      }
      else
      {
         Pixels = (uint32_t)aWidth * (uint32_t)aHeight;
         CurrentX = StartX;
         CurrentY = StartY;
         EndX = StartX + aWidth;
         
         Count=0;
         while(Count < Pixels)
         {
            flash_ReadData(Address, &Pixel, 1);
            glcd_DrawPixel(CurrentX, CurrentY, Pixel);
            
            flash_IncAddressColumn(&Address);
            Count++;
            
            if(++CurrentX >= EndX)
            {
               CurrentY++;
               CurrentX = StartX;
               
               if(aWidth != ImageData->Header.Width)
                  flash_IncAddressColumn(&Address, (ImageData->Header.Width - aWidth));
            }
         }
      }
   }
}

#ifdef USE_FAST_VERTICAL_DRAW
void WriteFastVerticalImage(IMAGE *ImageData, uint16_t StartX, uint16_t StartY)
{
   uint32_t Pixels;
   uint32_t Count;
   uint16_t WriteCount;
   uint16_t aWidth, aHeight;
   FLASH_ADDR Address;
   uint8_t AddressArray[5];
   uint16_t PrevBlock;
   uint16_t *PixelData;
   uint16_t CurrentX, CurrentY, EndX;
   uint16_t Pixel;
   uint16_t i;
   IMAGE hImageData;
   int16_t hStartX, hStartY;
   
   hImageData.Header.Height = ImageData->Header.Width;
   hImageData.Header.Width = ImageData->Header.Height;
   
   hStartX = StartY;
   hStartY = (int16_t)GLCD_LINES - (int16_t)(StartX + hImageData.Header.Height);
   
   if((hStartX + hImageData.Header.Width) > GLCD_PIXELS)
      aWidth = GLCD_PIXELS - hStartX;
   else
      aWidth = hImageData.Header.Width;
      
   if(hStartY < 0)
   {
      if((hStartY + hImageData.Header.Height) > GLCD_LINES)
         aHeight = GLCD_LINES;
      else
         aHeight = hImageData.Header.Height + hStartY;
   }
   else
   {  
      if((hStartY + hImageData.Header.Height) > GLCD_LINES)
         aHeight = GLCD_LINES - hStartY;
      else
         aHeight = hImageData.Header.Height;
   }
   
   memcpy(&Address, &ImageData->BitmapAddress, sizeof(FLASH_ADDR));
   
   if(hStartY < 0)
   {
      flash_IncAddress(&Address, abs(hStartY) * hImageData.Header.Width);
      hStartY = 0;
   }

   if(((hStartX % MINIMUM_WRITE_WIDTH) == 0) && ((aWidth % MINIMUM_WRITE_WIDTH) == 0) &&
      (aWidth == hImageData.Header.Width) && (aHeight == hImageData.Header.Height))
   {
      Pixels = (uint32_t)aWidth * (uint32_t)aHeight;
      
      if((Pixels % 256) == 0)
         WriteCount = 256;
      else if((Pixels % 128) == 0)
         WriteCount = 128;
      else if((Pixels % 64) == 0)
         WriteCount = 64;
      else if((Pixels % 32) == 0)
         WriteCount = 32;
      else if((Pixels % 16) == 0)
         WriteCount = 16;
      else if((Pixels % 8) == 0)
         WriteCount = 8;
      else if((Pixels % 4) == 0)
         WriteCount = 4;
      else if((Pixels % 2) == 0)
         WriteCount = 2;
      else
         WriteCount = 1;
      
      //Setup Write to GLCD
      output_low(GLCD_CS);
   
      glcd_WriteRegister(GLCD_WRITE_X_START_POS, hStartX / 4, FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_0, (hStartY >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_START_POS_1, (hStartY & 0x03), FALSE);
      
      glcd_WriteRegister(GLCD_WRITE_X_END_POS, ((hStartX / 4) + (aWidth / 4) - 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_0, (((hStartY + aHeight) - 1) >> 2), FALSE);
      glcd_WriteRegister(GLCD_WRITE_Y_END_POS_1, (((hStartY + aHeight) - 1) & 0x03), FALSE);
      
      GLCDDataOutput();
      
      GLCD_COMMAND();   
      glcd_WritePortRegister(GLCD_MEMORY_DATA_PORT_0);
   
      GLCD_DATA();
      
      //Setup Read from Flash
      flash_GetAddressArray(Address, AddressArray);
      
      output_low(FLASH_CE);
      
      flash_LatchCommand(FLASH_CMD_READ);
      
      flash_LatchAddress(AddressArray);
      
      flash_LatchCommand(FLASH_CMD_READ_PAGE);
            
      while(!input(FLASH_BUSY));
      
      flash_LatchCommand(FLASH_CMD_READ_CACHE);
      
      while(!input(FLASH_BUSY));
      
      FlashPortInput();
      
      Count = 0;
      PrevBlock = Address.Block;
      
      //Read and Write Data from Flash and to GLCD
      do
      {
         switch(WriteCount)
         {
            case 256:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 128:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 64:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 32:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 16:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 8:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 4:
               FlashLCDReadWrite();
               FlashLCDReadWrite();
            case 2:
               FlashLCDReadWrite();
            default:
               FlashLCDReadWrite();
         }
         
         Count += (uint32_t)WriteCount;
         flash_IncAddressColumn(&Address, WriteCount);
         
         if(Address.Column == 0)
         {
            if(Address.Block != PrevBlock)
            {
               flash_GetAddressArray(Address, AddressArray);
               
               flash_LatchCommand(FLASH_CMD_READ);
         
               flash_LatchAddress(AddressArray);
               
               flash_LatchCommand(FLASH_CMD_READ_PAGE);
               
               while(!input(FLASH_BUSY));
               
               flash_LatchCommand(FLASH_CMD_READ_CACHE);
               
               PrevBlock = Address.Block;
            }
            else
               flash_LatchCommand(FLASH_CMD_READ_CACHE);
            
            while(!input(FLASH_BUSY));
            
            FlashPortInput();
         }
         
      } while(Count < Pixels);
      
      flash_LatchCommand(FLASH_CMD_CATCH_END);
      
      while(!input(FLASH_BUSY));
      
      output_high(FLASH_CE);
      
      output_high(GLCD_CS);
   }
   else
   {
      PixelData = malloc(aWidth * 2);
      
      if(PixelData != NULL)
      {
         for(i=0,CurrentY=hStartY;i<aHeight;i++,CurrentY++)
         {
            flash_ReadData(Address, PixelData, aWidth);
            glcd_DrawPixels(hStartX, CurrentY, aWidth, 1, PixelData);
            flash_IncAddressColumn(&Address, hImageData.Header.Width);
         }
         
         free(PixelData);
      }
      else
      {
         Pixels = (uint32_t)aWidth * (uint32_t)aHeight;
         CurrentX = hStartX;
         CurrentY = hStartY;
         EndX = hStartX + aWidth;
         
         Count=0;
         while(Count < Pixels)
         {
            flash_ReadData(Address, &Pixel, 1);
            glcd_DrawPixel(CurrentX, CurrentY, Pixel);
            
            flash_IncAddressColumn(&Address);
            Count++;
            
            if(++CurrentX >= EndX)
            {
               CurrentY++;
               CurrentX = hStartX;
               
               if(aWidth != hImageData.Header.Width)
                  flash_IncAddressColumn(&Address, (hImageData.Header.Width - aWidth));
            }
         }
      }
   }
}
#endif
