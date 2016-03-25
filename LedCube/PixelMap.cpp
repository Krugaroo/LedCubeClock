/*
* Name: PixelMap.cpp
* Author: Michael Kruger
* Brief:
* 
* This file contains bit array that maintains the state of the LEDs in the LED cube. 
* The animations/clock faces update this state and then the state is written to the LED cube
* using the LEDCubeDriver
* 
* If you have shiftregisters simply change the pin defines below.
* 
* 
* This project is for an LEDCubeClock that shows the time and animations. 
* More information can be found at: https://github.com/Krugaroo/LedCubeClock/
* 
* License: MIT License
*
* Copyright (c) 2016 Michael Kruger, Krugaroo
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include "PixelMap.h"

#include "LEDCubeDriver.h"

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define TOTAL_PIXELS      X_PIXELS*Y_PIXELS*Z_PIXELS
#define PIXELS_PER_LAYER  X_PIXELS*Y_PIXELS
#define PIXELS_PER_COLUMN X_PIXELS

unsigned char s_pixelMap[(TOTAL_PIXELS>>3) + ((TOTAL_PIXELS%8)?1:0)] = {0};

void pixelMap_clearAll(void)
{
  memset(s_pixelMap, 0x00, (TOTAL_PIXELS>>3) + ((TOTAL_PIXELS%8)?1:0));
}

void pixelMap_setAll(void)
{
  memset(s_pixelMap, 0xFF, (TOTAL_PIXELS>>3) + ((TOTAL_PIXELS%8)?1:0));
}

void pixelMap_set3Dpixel(unsigned short inX, unsigned short inY, unsigned short inZ, pixelMapValue inSet)
{
  if(inX>=X_PIXELS) return;
  if(inY>=Y_PIXELS) return;
  if(inZ>=Z_PIXELS) return;
  
  unsigned short lvPixelPos = inX +
                              inY*(PIXELS_PER_COLUMN) +
                              inZ*(PIXELS_PER_LAYER);
                              
  unsigned short lvBytePos = ((lvPixelPos)>>3);
  unsigned short lvBitPos  = ((lvPixelPos)%8);

  if(inSet == PIXEL_SET)
  {
    s_pixelMap[lvBytePos] |= (PIXEL_SET<<lvBitPos);
  }
  else
  {
    s_pixelMap[lvBytePos] &= ~((PIXEL_SET<<lvBitPos));
  }
}

void pixelMap_set2Dpixel(unsigned short inX, unsigned short inY, pixelMapValue inSet)
{
  if(inX>=X_PIXELS) return;
  if(inY>=Y_PIXELS) return;
  
  unsigned short lvPixelPos = inX +
                              inY*(PIXELS_PER_COLUMN);
                              
  unsigned short lvBytePos = ((lvPixelPos)>>3);
  unsigned short lvBitPos  = ((lvPixelPos)%8);

  if(inSet == PIXEL_SET)
  {
    s_pixelMap[lvBytePos] |= (PIXEL_SET<<lvBitPos);
  }
  else
  {
    s_pixelMap[lvBytePos] &= ~((PIXEL_SET<<lvBitPos));
  }
}

pixelMapValue pixelMap_get3Dpixel(unsigned short inX, unsigned short inY, unsigned short inZ)
{
  unsigned short lvPixelPos = inX +
                              inY*(PIXELS_PER_COLUMN) +
                              inZ*(PIXELS_PER_LAYER);
                              
  unsigned short lvBytePos = ((lvPixelPos)>>3);
  unsigned short lvBitPos  = ((lvPixelPos)%8);

  if((s_pixelMap[lvBytePos] & (PIXEL_SET<<lvBitPos)))
  {
    return PIXEL_SET;
  }
  else
  {
    return PIXEL_CLEAR;
  }
}

pixelMapValue pixelMap_get2Dpixel(unsigned short inX, unsigned short inY)
{
  unsigned short lvPixelPos = inX +
                              inY*(PIXELS_PER_COLUMN);
                              
  unsigned short lvBytePos = ((lvPixelPos)>>3);
  unsigned short lvBitPos  = ((lvPixelPos)%8);

  if((s_pixelMap[lvBytePos] & (PIXEL_SET<<lvBitPos)))
  {
    return PIXEL_SET;
  }
  else
  {
   return PIXEL_CLEAR;
  }
}

void pixelMap_update()
{
  /* Loop through each layer and update it */
  for(int i=0;i<Z_PIXELS;i++)
  {
    unsigned short lvBytePos = ((PIXELS_PER_LAYER*i)>>3);
    unsigned short lvBitPos  = ((PIXELS_PER_LAYER*i)%8);

    /* pass the bit array its offset and layer to update */
    LEDCube_setLayer(s_pixelMap+lvBytePos, lvBitPos, i);
  }
}

