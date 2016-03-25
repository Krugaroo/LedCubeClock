/*
* Name: Animations.h
* Author: Michael Kruger
* Brief:
* 
* This file contains declaration of the Animations.
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
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "Arduino.h"


typedef enum{
  ANIMATION_SLICE=0,
  ANIMATION_SPARKLE,
  //ANIMATION_CHASE,
  ANIMATION_SQUARE,
  ANIMATION_RANDOM_FILL,
  ANIMATION_RAIN,
  ANIMATION_ROCKETS,
  ANIMATION_ZIGZAG,
  ANIMATION_FIREWORKS,
  ANIMATION_SNAKE,
  ANIMATION_DROPUPDOWN,
  ANIMATION_DROPALLUPDOWN,
  ANIMATION_DROPMULTIUPDOWN,
  ANIMATION_DROP2STEPUPDOWN,
  ANIMATION_SMOKE,
  ANIMATION_SIDESQUARES,
  ANIMATION_SINE,
  ANIMATION_HINGE,
  ANIMATION_SPIN,
  ANIMATION_ORBIT,
  ANIMATION_FADE,
  //ANIMATION_BLINK,

  /* add animations above */
  ANIMATION_COUNT
} ANIMATION_LIST;

int Animation_TickAnimation(int inID, int firstCall);

ANIMATION_LIST Animation_PickRandomAnimation(int inCurrent);

/* list of animations */

int Animation_Fade(bool inFirstCall);

int Animation_Blink(bool inFirstCall); 

int Animation_Slice(bool inFirstCall); 

int Animation_Sparkle(bool inFirstCall); 

int Animation_Chase(bool inFirstCall); 

int Animation_Square(bool inFirstCall); 

int Animation_RandomFill(bool inFirstCall);

int Animation_Rain(bool inFirstCall); 

int Animation_Rockets(bool inFirstCall); 

int Animation_ZigZag(bool inFirstCall); 

int Animation_Fireworks(bool inFirstCall);

int Animation_Snake(bool inFirstCall); 

int Animation_DropUpDown(bool inFirstCall);

int Animation_DropAllUpDown(bool inFirstCall); 

int Animation_DropMultiUpDown(bool inFirstCall);

int Animation_Smoke(bool inFirstCall);

int Animation_SideSquares(bool inFirstCall); 

int Animation_Sine(bool inFirstCall); 

int Animation_Hinge(bool inFirstCall); 

int Animation_Drop2StepUpDown(bool inFirstCall);

int Animation_Spin(bool inFirstCall);

int Animation_Orbit(bool inFirstCall);

#endif //ANIMATIONS_H
