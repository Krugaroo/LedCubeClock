/*
* Name: Input.h
* Author: Michael Kruger
* Brief:
* 
* This file contains the input handling prototypes for the buttons functions to allow the mode, clock faces and animations to be changed.
* It also stores the state of the variables.
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
#ifndef INPUT_H
#define INPUT_H

#include "ClockFaces.h"
#include "Animations.h"

#include "Arduino.h"

#define INPUT_LEFT_BUTTON         A0
#define INPUT_MIDDLE_BUTTON       A1
#define INPUT_RIGHT_BUTTON        A2

typedef enum{
  MODE_CLOCK = 0,
  MODE_RANDOM_ANIMATION,
  MODE_SELECT_ANIMATION,
  MODE_MAX
} LEDcube_Mode;



typedef struct
{
  LEDcube_Mode currentMode;
  bool selectedFirstCall;
  
  // MODE_SELECT_ANIMATION INPUT 
  ANIMATION_LIST selectedAnimation;

  //MODE_CLOCK
  CLOCKFACE_LIST selectedClockFace;

} inputData;



void Input_Tick(void);

inputData* Input_GetData(void); 

#endif // INPUT_H
