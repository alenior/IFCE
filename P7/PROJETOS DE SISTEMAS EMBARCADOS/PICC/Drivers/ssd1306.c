/////////////////////////////////////////////////////////////////////////
////                           SSD1306.C                             ////
////                                                                 ////
//// Driver for a SSD1306 Display Controller with an I2C interface.  ////
//// The driver treats the upper left pixel as (0,0).                ////
////                                                                 ////
//// LCD Pin connections:                                            ////
//// (These can be changed as needed in the following defines).      ////
////  * 1: GND is connected to GND                                   ////
////  * 2: VCC is connected to +5V                                   ////
////  * 3: SCL - Clock pin for I2C is connected to C3                ////
////  * 4: SDA - Data pin for I2C is connected to C4                 ////
////  * 5: R/W - Read or Write is connected to B4                    ////
////                                                                 ////
////  oled_init()                                                    ////
////     * Must be called before any other function.                 ////
////                                                                 ////
////  oled_pixel(x,y,color)                                          ////
////     * Sets or clears the given pixel.                           ////
////       - color is TRUE for on and FALSE for off                  ////
////                                                                 ////
////  oled_line(x1,y1,x2,y2,color)                                   ////
////     * Draws a line from the first point to the second point.    ////
////       - color is TRUE for on and FALSE for off                  ////
////                                                                 ////
////  oled_rect(x1,y1,x2,y2,fill,color)                              ////
////     * Draws a rectangle with upper left point (x1,y1) and lower ////
////       right point (x2,y2).                                      ////
////       - fill can be TRUE or FALSE                               ////
////       - color is TRUE for on and FALSE for off                  ////
////                                                                 ////
////  oled_bar(x1,y1,x2,y2,width,color)                              ////
////     * Draws a bar (wide line) from the first point to the       ////
////       second point.                                             ////
////       - width is the number of pixels wide                      ////
////       - color is TRUE for on and FALSE for off                  ////
////                                                                 ////
////  oled_circle(x,y,radius,fill,color)                             ////
////     * Draws a circle with center at (x,y)                       ////
////       - fill can be TRUE or FALSE                               ////
////       - color is TRUE for on and FALSE for off                  ////
////                                                                 ////
////  oled_text57(x,y,textptr,size,color)                            ////
////     * Write the null terminated text pointed to by textptr with ////
////       the upper left coordinate of the first character at (x,y).////
////       Characters are 5 pixels wide and 7 pixels tall.           ////
////       - size is an integer that scales the size of the text     ////
////     * Note - The given text is character wrapped. If this       ////
////       function is used on a different size display, then change ////
////       the OLED_WIDTH define appropriately.                      ////
////       - color is TRUE for on and FALSE for off                  ////
////                                                                 ////
////  oled_fillScreen()                                              ////
////     * Fills the entire OLED.                                    ////
////                                                                 ////
////  oled_flush()                                                   ////
////     * When OLED_FAST_DRAW is defined it draws the pixels to the ////
////       display.  When OLED_FAST_DRAW is not defined the          ////
////       oled_pixel() function draws the pixels to the display.    ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// Defines:                                                        ////
////                                                                 ////
////  OLED_SDA - used to set the I2C SDA pin, defaults to PIN_C4 if  ////
////     not defined before this file is included in project.        ////
////                                                                 ////
////  OLED_SCL - used to set the I2C SCL pin, default to PIN_C3 if   ////
////     not defined before this file is included in project.        ////
////                                                                 ////
////  OLED_SLAVE_ADDRESS - used to set the I2C slave address of the  ////
////     display.  Can be either 0x78 or 0x7A, defaults to 0x78 if   ////
////     not defined before this file is included in project.        ////
////                                                                 ////
////  OLED_WIDTH - used to set the width of the display.  Defaults   ////
////     to 128 if not defined before this file is included in       ////
////     project.                                                    ////
////                                                                 ////
////  OLED_HEIGHT - used to set the height of the display.  Defaults ////
////     to 64 if not defined before this file is included in        ////
////     project.                                                    ////
////                                                                 ////
////  OLED_FAST_DRAW - if defined the oled_pixel() function will     ////
////                   will only update the specified pixel in the   ////
////                   buffer array.  Must used the oled_flush() to  ////
////                   draw the pixels to the screen.                ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                      http://www.ccsinfo.com                     ////
/////////////////////////////////////////////////////////////////////////

#ifndef SSD1306_C
#define SSD1306_C

#ifndef OLED_SDA
 #define OLED_SDA  PIN_C4
#endif

#ifndef OLED_SCL
 #define OLED_SCL  PIN_C3
#endif

#ifndef OLED_SLAVE_ADDRESS
 #define OLED_SLAVE_ADDRESS     0x78
#endif

#use i2c(master, sda=OLED_SDA, scl=OLED_SCL, FAST, stream=STREAM_24XX1025)

#ifndef OLED_WIDTH
 #define OLED_WIDTH     128
#endif

#ifndef OLED_HEIGHT
 #define OLED_HEIGHT    64
#endif

#define DISPLAYOFF         0xAE
#define MUXRATIO           0xA8
#define DISPLAYOFFSET      0xD3
#define DISPLAYSTARTLINE   0x40
#define SEGMENTREMAP       0xA1
#define COMOUTPUTSCAN      0xC8
#define COMHARDWARECONFIG  0xDA
#define CONTRASTCONTROL    0x81
#define DISPLAYALLRESUME   0xA4
#define NORMALDISPLAY      0xA6
#define OSCFREQUENCY       0xD5
#define CHARGEPUMPREG      0x8D
#define ADDRESSINGMODE     0x20
#define DISPLAYON          0xAF
#define COLUMNADDRESS      0x21
#define PAGEADDRESS        0x22

#define OLED_COLUMNS       OLED_WIDTH
#define OLED_PAGES         (OLED_HEIGHT / 8)

#define OLED_LAST_COLUMN   (OLED_COLUMNS - 1)
#define OLED_LAST_PAGE     (OLED_PAGES - 1)


unsigned int8 oled_buffer[(OLED_COLUMNS * OLED_PAGES)];
void oled_clearScreen();
void oled_flush();

const unsigned int8 TEXT[51][5] = {0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
                                   0x00, 0x00, 0x5F, 0x00, 0x00, // !
                                   0x00, 0x03, 0x00, 0x03, 0x00, // "
                                   0x14, 0x3E, 0x14, 0x3E, 0x14, // #
                                   0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
                                   0x43, 0x33, 0x08, 0x66, 0x61, // %
                                   0x36, 0x49, 0x55, 0x22, 0x50, // &
                                   0x00, 0x05, 0x03, 0x00, 0x00, // '
                                   0x00, 0x1C, 0x22, 0x41, 0x00, // (
                                   0x00, 0x41, 0x22, 0x1C, 0x00, // )
                                   0x14, 0x08, 0x3E, 0x08, 0x14, // *
                                   0x08, 0x08, 0x3E, 0x08, 0x08, // +
                                   0x00, 0x50, 0x30, 0x00, 0x00, // ,
                                   0x08, 0x08, 0x08, 0x08, 0x08, // -
                                   0x00, 0x60, 0x60, 0x00, 0x00, // .
                                   0x20, 0x10, 0x08, 0x04, 0x02, // /
                                   0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
                                   0x04, 0x02, 0x7F, 0x00, 0x00, // 1
                                   0x42, 0x61, 0x51, 0x49, 0x46, // 2
                                   0x22, 0x41, 0x49, 0x49, 0x36, // 3
                                   0x18, 0x14, 0x12, 0x7F, 0x10, // 4
                                   0x27, 0x45, 0x45, 0x45, 0x39, // 5
                                   0x3E, 0x49, 0x49, 0x49, 0x32, // 6
                                   0x01, 0x01, 0x71, 0x09, 0x07, // 7
                                   0x36, 0x49, 0x49, 0x49, 0x36, // 8
                                   0x26, 0x49, 0x49, 0x49, 0x3E, // 9
                                   0x00, 0x36, 0x36, 0x00, 0x00, // :
                                   0x00, 0x56, 0x36, 0x00, 0x00, // ;
                                   0x08, 0x14, 0x22, 0x41, 0x00, // <
                                   0x14, 0x14, 0x14, 0x14, 0x14, // =
                                   0x00, 0x41, 0x22, 0x14, 0x08, // >
                                   0x02, 0x01, 0x51, 0x09, 0x06, // ?
                                   0x3E, 0x41, 0x59, 0x55, 0x5E, // @
                                   0x7E, 0x09, 0x09, 0x09, 0x7E, // A
                                   0x7F, 0x49, 0x49, 0x49, 0x36, // B
                                   0x3E, 0x41, 0x41, 0x41, 0x22, // C
                                   0x7F, 0x41, 0x41, 0x41, 0x3E, // D
                                   0x7F, 0x49, 0x49, 0x49, 0x41, // E
                                   0x7F, 0x09, 0x09, 0x09, 0x01, // F
                                   0x3E, 0x41, 0x41, 0x49, 0x3A, // G
                                   0x7F, 0x08, 0x08, 0x08, 0x7F, // H
                                   0x00, 0x41, 0x7F, 0x41, 0x00, // I
                                   0x30, 0x40, 0x40, 0x40, 0x3F, // J
                                   0x7F, 0x08, 0x14, 0x22, 0x41, // K
                                   0x7F, 0x40, 0x40, 0x40, 0x40, // L
                                   0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
                                   0x7F, 0x02, 0x04, 0x08, 0x7F, // N
                                   0x3E, 0x41, 0x41, 0x41, 0x3E, // O
                                   0x7F, 0x09, 0x09, 0x09, 0x06, // P
                                   0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
                                   0x7F, 0x09, 0x09, 0x09, 0x76};// R

const unsigned int8 TEXT2[44][5] = {0x26, 0x49, 0x49, 0x49, 0x32, // S
                                    0x01, 0x01, 0x7F, 0x01, 0x01, // T
                                    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
                                    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
                                    0x7F, 0x20, 0x10, 0x20, 0x7F, // W
                                    0x41, 0x22, 0x1C, 0x22, 0x41, // X
                                    0x07, 0x08, 0x70, 0x08, 0x07, // Y
                                    0x61, 0x51, 0x49, 0x45, 0x43, // Z
                                    0x00, 0x7F, 0x41, 0x00, 0x00, // [
                                    0x02, 0x04, 0x08, 0x10, 0x20, // \
                                    0x00, 0x00, 0x41, 0x7F, 0x00, // ]
                                    0x04, 0x02, 0x01, 0x02, 0x04, // ^
                                    0x40, 0x40, 0x40, 0x40, 0x40, // _
                                    0x00, 0x01, 0x02, 0x04, 0x00, // `
                                    0x20, 0x54, 0x54, 0x54, 0x78, // a
                                    0x7F, 0x44, 0x44, 0x44, 0x38, // b
                                    0x38, 0x44, 0x44, 0x44, 0x44, // c
                                    0x38, 0x44, 0x44, 0x44, 0x7F, // d
                                    0x38, 0x54, 0x54, 0x54, 0x18, // e
                                    0x04, 0x04, 0x7E, 0x05, 0x05, // f
                                    0x08, 0x54, 0x54, 0x54, 0x3C, // g
                                    0x7F, 0x08, 0x04, 0x04, 0x78, // h
                                    0x00, 0x44, 0x7D, 0x40, 0x00, // i
                                    0x20, 0x40, 0x44, 0x3D, 0x00, // j
                                    0x7F, 0x10, 0x28, 0x44, 0x00, // k
                                    0x00, 0x41, 0x7F, 0x40, 0x00, // l
                                    0x7C, 0x04, 0x78, 0x04, 0x78, // m
                                    0x7C, 0x08, 0x04, 0x04, 0x78, // n
                                    0x38, 0x44, 0x44, 0x44, 0x38, // o
                                    0x7C, 0x14, 0x14, 0x14, 0x08, // p
                                    0x08, 0x14, 0x14, 0x14, 0x7C, // q
                                    0x00, 0x7C, 0x08, 0x04, 0x04, // r
                                    0x48, 0x54, 0x54, 0x54, 0x20, // s
                                    0x04, 0x04, 0x3F, 0x44, 0x44, // t
                                    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
                                    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
                                    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
                                    0x44, 0x28, 0x10, 0x28, 0x44, // x
                                    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
                                    0x44, 0x64, 0x54, 0x4C, 0x44, // z
                                    0x00, 0x08, 0x36, 0x41, 0x41, // {
                                    0x00, 0x00, 0x7F, 0x00, 0x00, // |
                                    0x41, 0x41, 0x36, 0x08, 0x00, // }
                                    0x02, 0x01, 0x02, 0x04, 0x02};// ~

// Purpose:       Transfers command to graphic OLED
// Inputs:        command - command to transfer
void oled_command(unsigned int8 command)
{
   unsigned int8 wData[2];
   wData[0] = 0x00;        
   wData[1] = command;     
   i2c_transfer(STREAM_24XX1025, OLED_SLAVE_ADDRESS, wData, 2);
}

// Purpose:       Transfers command to graphic OLED
// Inputs:        command - command to transfer
//                arg - argument to transfer with command
void oled_command(unsigned int8 command, unsigned int8 arg)
{
   unsigned int8 wData[3];
   wData[0] = 0x00;        
   wData[1] = command;     
   wData[2] = arg;         
   i2c_transfer(STREAM_24XX1025, OLED_SLAVE_ADDRESS, wData, 3);
}

// Purpose:       Transfers command to graphic OLED
// Inputs:        command - command to transfer
//                argOne - first argument for the command
//                argTwo - second argument for the command
void oled_command(unsigned int8 command, unsigned int8 argOne, unsigned int8 argTwo)
{
   unsigned int8 wData[4];
   wData[0] = 0x00;        
   wData[1] = command;     
   wData[2] = argOne;   
   wData[3] = argTwo;
   i2c_transfer(STREAM_24XX1025, OLED_SLAVE_ADDRESS, wData, 4);
}

// Purpose:       Transfers data to graphic OLED
//                to be displayed
// Inputs:        data - data to transfer
void oled_data(unsigned int8 data)
{
   unsigned int8 wData[2];
   wData[0] = 0x40;        
   wData[1] = data;
   i2c_transfer(STREAM_24XX1025, OLED_SLAVE_ADDRESS, wData, 2);
}

// Purpose:       Initialize a graphic OLED. This must be called before any
//                other oled function is used.
void oled_init()
{
   oled_command(DISPLAYOFF);                       // Set Display OFF

   oled_command(MUXRATIO,0x3F);                    // Set MUX Ration
   
   oled_command(DISPLAYOFFSET,0x00);               // Set Display Offset
   
   oled_command(DISPLAYSTARTLINE);                 // Set Display Start Line
   
   oled_command(SEGMENTREMAP);                     // Set Segment re-map
   
   oled_command(COMOUTPUTSCAN);                    // Set COM Output Scan Direction
   
   oled_command(COMHARDWARECONFIG,0x12);           // Set COM Pins hardware configuration
   
   oled_command(CONTRASTCONTROL,0x7F);             // Set Contrast Control
   
   oled_command(DISPLAYALLRESUME);                 // Disable Entire Display On
   
   oled_command(NORMALDISPLAY);                    // Set Normal Display
   
   oled_command(OSCFREQUENCY,0x80);                // Set Osc Frequency
   
   oled_command(CHARGEPUMPREG,0x14);               // Enable charge pump regulator
   
   oled_command(ADDRESSINGMODE,0x00);              // Set Horizontal Addressing Mode
   
   oled_command(DISPLAYON);                        // Display On
   
   oled_command(COLUMNADDRESS,0,OLED_LAST_COLUMN); // Set Column Address
   
   oled_command(PAGEADDRESS,0,OLED_LAST_PAGE);     // Set Page Address
}

// Purpose:       Set all pixels on the OLED screen.
void oled_clearScreen()
{
   oled_command(COLUMNADDRESS,0,OLED_LAST_COLUMN);     // Set Column Address
   oled_command(PAGEADDRESS,0,OLED_LAST_PAGE);         // Set Page Address
   
   for(unsigned int16 i = 0; i < (OLED_COLUMNS * OLED_PAGES); i++)
      oled_data(0x00);
   
   memset(oled_buffer, 0x00, sizeof(oled_buffer));
}

// Purpose:       Clear all pixels on the OLED screen.
void oled_fillScreen()
{
   oled_command(COLUMNADDRESS,0,OLED_LAST_COLUMN); // Set Column Address
   oled_command(PAGEADDRESS,0,OLED_LAST_PAGE);     // Set Page Address
   
   for(unsigned int16 i = 0; i < (OLED_COLUMNS * OLED_PAGES); i++)
      oled_data(0xFF);
   
   memset(oled_buffer, 0xFF, sizeof(oled_buffer));
}

// Purpose:       Turn a pixel on a graphic OLED on or off
// Inputs:        x - the x coordinate of the pixel
//                y - the y coordinate of the pixel
//                color - TRUE turn on pixel, FALSE turn off 
// Output:        1 if coordinate out of range, 0 if in range
//
// ** If OLED_FAST_DRAW is defined oled_flush() must be called to draw pixels
//     to display.
unsigned int8 oled_pixel(unsigned int8 x, unsigned int8 y, int1 color)
{
   if((x > (OLED_WIDTH - 1)) || (y > (OLED_HEIGHT - 1))) {
      return 1;
   }
   
   int8 columnY = 1 << (y&7);
   int8 page = y/8;

   int16 index = (x+(unsigned int16)page*OLED_WIDTH);
   if(color) 
      oled_buffer[index] = oled_buffer[index] | columnY;
   else
      oled_buffer[index] = oled_buffer[index] & ~columnY;
   
  #ifndef OLED_FAST_DRAW
   oled_command(COLUMNADDRESS,x,OLED_LAST_COLUMN);
   oled_command(PAGEADDRESS,page,OLED_LAST_PAGE);
   oled_data(oled_buffer[index]);
  #endif
   
   return 0;
}

// Purpose:       Draw a line on a graphic OLED using Bresenham's
//                line drawing algorithm
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                color - TRUE turn on line, FALSE turn off
// Dependencies:  oled_pixel()
//
// ** If OLED_FAST_DRAW is defined oled_flush() must be called to draw pixels
//     to display.
void oled_line(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, int1 color)
{
   signed int8 x, y, addx, addy, dx, dy;
   signed int16 P;
   unsigned int8 i;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   x = x1;
   y = y1;

   if(x1 > x2)
      addx = -1;
   else
      addx = 1;
   if(y1 > y2)
      addy = -1;
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         oled_pixel(x, y, color);

         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         oled_pixel(x, y, color);

         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
      }
   }
}

// Purpose:       Draw a rectangle on a graphic OLED
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                fill  - TRUE or FALSE
//                color - TRUE turn on rect, FALSE turn off
// Dependencies:  oled_pixel(), oled_line()
//
// ** If OLED_FAST_DRAW is defined oled_flush() must be called to draw pixels
//     to display.
void oled_rect(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, int1 fill, int1 color)
{
   if(fill)
   {
      unsigned int8 y, ymax;                          // Find the y min and max
      if(y1 < y2)
      {
         y = y1;
         ymax = y2;
      }
      else
      {
         y = y2;
         ymax = y1;
      }

      for(; y<=ymax; ++y)                    // Draw lines to fill the rectangle
         oled_line(x1, y, x2, y, color);
   }
   else
   {
      oled_line(x1, y1, x2, y1, color);      // Draw the 4 sides
      oled_line(x1, y2, x2, y2, color);
      oled_line(x1, y1, x1, y2, color);
      oled_line(x2, y1, x2, y2, color);
   }
}

// Purpose:       Draw a bar (wide line) on a graphic OLED
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                width  - The number of pixels wide
//                color - TRUE turn on bar, FALSE turn off
// Dependencies:  oled_pixel()
//
// ** If OLED_FAST_DRAW is defined oled_flush() must be called to draw pixels
//     to display.
void oled_bar(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, unsigned int8 width, int1 color)
{
   signed int8  x, y, addx, addy, j;
   signed int16 P, dx, dy, c1, c2;
   unsigned int8 i;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   x = x1;
   y = y1;
   c1 = -dx*x1 - dy*y1;
   c2 = -dx*x2 - dy*y2;

   if(x1 > x2)
   {
      addx = -1;
      c1 = -dx*x2 - dy*y2;
      c2 = -dx*x1 - dy*y1;
   }
   else
      addx = 1;
   if(y1 > y2)
   {
      addy = -1;
      c1 = -dx*x2 - dy*y2;
      c2 = -dx*x1 - dy*y1;
   }
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-(width/2); j<width/2+width%2; ++j)
         {
            if(dx*x+dy*(y+j)+c1 >= 0 && dx*x+dy*(y+j)+c2 <=0)
               oled_pixel(x, y+j, color);
         }
         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
         for(j=-(width/2); j<width/2+width%2; ++j)
         {
            if(dx*x+dy*(y+j)+c1 >= 0 && dx*x+dy*(y+j)+c2 <=0)
               oled_pixel(x+j, y, color);
         }
      }
   }
}

// Purpose:       Draw a circle on a graphic OLED
// Inputs:        (x,y) - the center of the circle
//                radius - the radius of the circle
//                fill - TRUE or FALSE
//                color - TRUE turn on circle, FALSE turn off
// Dependencies:  oled_pixel(), oled_line()
//
// ** If OLED_FAST_DRAW is defined oled_flush() must be called to draw pixels
//     to display.
void oled_circle(unsigned int8 x, unsigned int8 y, unsigned int8 radius, int1 fill, int1 color)
{
   signed int8 a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         oled_line(x-a, y+b, x+a, y+b, color);
         oled_line(x-a, y-b, x+a, y-b, color);
         oled_line(x-b, y+a, x+b, y+a, color);
         oled_line(x-b, y-a, x+b, y-a, color);
      }
      else
      {
         oled_pixel(a+x, b+y, color);
         oled_pixel(b+x, a+y, color);
         oled_pixel(x-a, b+y, color);
         oled_pixel(x-b, a+y, color);
         oled_pixel(b+x, y-a, color);
         oled_pixel(a+x, y-b, color);
         oled_pixel(x-a, y-b, color);
         oled_pixel(x-b, y-a, color);
      }

      if(P < 0)
         P+= 3 + 2*a++;
      else
         P+= 5 + 2*(a++ - b--);
    } while(a <= b);
}

// Purpose:       Write text on a graphic OLED
// Inputs:        (x,y) - The upper left coordinate of the first letter
//                textptr - A pointer to an array of text to display
//                size - The size of the text: 1 = 5x7, 2 = 10x14, ...
//                color - TRUE turn on text, FALSE turn off
// Dependencies:  oled_pixel()
//
// ** If OLED_FAST_DRAW is defined oled_flush() must be called to draw pixels
//     to display.
void oled_text57(unsigned int8 x, unsigned int8 y, char* textptr, unsigned int8 size, int1 color)
{
   unsigned int8 i, j, k, l, m;                     // Loop counters
   unsigned int8 pixelData[5];                     // Stores character data

   for(i=0; textptr[i] != '\0'; ++i, ++x) // Loop through the passed string
   {
      if(textptr[i] < 'S') // Checks if the letter is in the first text array
         memcpy(pixelData, TEXT[textptr[i]-' '], 5);
      else if(textptr[i] <= '~') // Check if the letter is in the second array
         memcpy(pixelData, TEXT2[textptr[i]-'S'], 5);
      else
         memcpy(pixelData, TEXT[0], 5);   // Default to space

      if(x+5*size >= OLED_WIDTH)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += 7*size + 1;                 // Set y at next position down
      }
      for(j=0; j<5; ++j, x+=size)         // Loop through character byte data
      {
         for(k=0; k<7*size; ++k)          // Loop through the vertical pixels
         {
            if(bit_test(pixelData[j], k)) // Check if the pixel should be set
            {
               for(l=0; l<size; ++l)      // The next two loops change the
               {                          // character's size
                  for(m=0; m<size; ++m)
                  {
                     oled_pixel(x+m, y+k*size+l, color); // Draws the pixel
                  }
               }
            }
         }
      }
   }
}

// Purpose:       When OLED_FAST_DRAW is defined it draws the pixels to the 
//                display.
void oled_flush(void)
{
  #ifdef OLED_FAST_DRAW
   oled_command(COLUMNADDRESS,0,OLED_LAST_COLUMN); // Set Column Address
   oled_command(PAGEADDRESS,0,OLED_LAST_PAGE);     // Set Page Address
   
   for(unsigned int16 i = 0; i < (OLED_COLUMNS * OLED_PAGES); i++)
      oled_data(oled_buffer[i]);
  #endif
}

#endif
