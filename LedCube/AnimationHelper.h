/*
* Name: AnimationHelper.h
* Author: Michael Kruger
* Brief:
* 
* This file contains several helper functions to make programming or Animations and Clock Faces easier.
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
#ifndef ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#include "PixelMap.h"

#include "Arduino.h"

typedef enum
{
  PIXEL_SHIFT_UP = 0,
  PIXEL_SHIFT_DOWN
} pixelShiftDir;

#define abs(x) (x<0)?(x*-1):x

#define NUMBER_ORIENTATIONS 26

int AnimationHelper_FindDifferentRandom(int inOldNumber, int inMin, int inMax);

void AnimationHelper_FindRandomPixel(int* outX, int *outY, int inLayer, pixelMapValue inWithState, int inOrientation);

void AnimationHelper_RotatePixel(int inX, int inY, int inDegrees, int *outX, int *outY);

void AnimationHelper_OrientPixel(int inX, int inY, int inZ, int inOrientation, int *outX, int *outY, int *outZ);

pixelMapValue AnimationHelper_GetOrientPixel(int inX, int inY, int inZ, int inOrientation);
void AnimationHelper_SetOrientPixel(int inX, int inY, int inZ, int inOrientation, pixelMapValue inState);

void AnimationHelper_ShiftLayers(int inMinZ, int inMaxZ, int inOrientation, pixelShiftDir inDirection);

int AnimationHelper_WalkAroundHorizontal(int inIndex);
int AnimationHelper_WalkAroundVertical(int inIndex);
int AnimationHelper_WalkAroundVertical2(int inIndex);
int AnimationHelper_WalkAroundSides(int inIndex);
int AnimationHelper_ChangeZAxis(int inIndex);

#endif // ANIMATION_HELPER_H
