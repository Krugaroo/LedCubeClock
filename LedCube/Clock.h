/*
* Name: Clock.h
* Author: Michael Kruger
* Brief:
* 
* This file contains a hardware abstraction to read/set the date and time.
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
#ifndef CLOCK_H
#define CLOCK_H

typedef struct
{
  unsigned char second;
  unsigned char minute;
  unsigned char hour;

  unsigned char day;
  unsigned char date;
  unsigned char month;
  unsigned int  year;
} timeDate;


void Clock_Init(void);

void Clock_FetchTime(void);

timeDate* Clock_GetTime(void); 

void Clock_SetTime(timeDate* inTimeDate); 

#endif // CLOCK_H
