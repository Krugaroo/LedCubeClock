/*
* Name: Buzzer.cpp
* Author: Michael Kruger
* Brief:
* 
* This file contains simple functions to play and stop a buzzer in a non-blocking way.
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
#include "Buzzer.h"

static long playDur = 0;

void Buzzer_Play(int inDuration, int inFrequency)
{
    tone(BUZZER_PIN,inFrequency);

    playDur = inDuration*1000L; 
}

void Buzzer_Stop(void)
{
  noTone(BUZZER_PIN);
}

void Buzzer_Tick(long inTimeDiff)
{
  if(playDur>0)
  {
    playDur = playDur - inTimeDiff;
  }
  
  if(playDur<=0)
  {
    Buzzer_Stop();
  }
}

