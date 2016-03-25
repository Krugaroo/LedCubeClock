/*
* Name: ClockFaces.h
* Author: Michael Kruger
* Brief:
* 
* This file contains the declaration for the various clock faces that can be used on the LedCube.
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
#ifndef CLOCKFACES_H
#define CLOCKFACES_H

#include "Arduino.h"

#include "Clock.h"

typedef enum{
  CLOCKFACE_DIGITAL=0,
  CLOCKFACE_DOTS,
  CLOCKFACE_ANALOG,
  CLOCKFACE_ANALOG_CIRCLE,

  /* add clock faces above */
  CLOCKFACE_COUNT
} CLOCKFACE_LIST;

int ClockFaces_TickClock(int inID, int firstCall, timeDate* inTime);

int ClockFaces_Digital(int inFirstCall, timeDate* inTime);

int ClockFaces_Dots(int inFirstCall, timeDate* inTime);

int ClockFaces_Analog(int inFirstCall, timeDate* inTime);

int ClockFaces_AnalogCircle(int inFirstCall, timeDate* inTime);

#endif //CLOCKFACES_H
