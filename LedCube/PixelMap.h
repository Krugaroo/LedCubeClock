/*
* Name: PixelMap.h
* Author: Michael Kruger
* Brief:
* 
* This file contains prototypes to access the bit array that maintains the state of the LEDs in the LED cube. 
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
#ifndef PIXELMAP_H
#define PIXELMAP_H

#define X_PIXELS 5
#define Y_PIXELS 5
#define Z_PIXELS 5

typedef enum
{
  PIXEL_CLEAR = 0,
  PIXEL_SET = 1
} pixelMapValue;

void pixelMap_clearAll(void);
void pixelMap_setAll(void);

pixelMapValue pixelMap_get3Dpixel(unsigned short inX, unsigned short inY, unsigned short inZ);
pixelMapValue pixelMap_get2Dpixel(unsigned short inX, unsigned short inY);

void pixelMap_set3Dpixel(unsigned short inX, unsigned short inY, unsigned short inZ, pixelMapValue inSet);
void pixelMap_set2Dpixel(unsigned short inX, unsigned short inY, pixelMapValue inSet);

void pixelMap_update();

#endif

