/*
* Name: LedCube.ino
* Author: Michael Kruger
* Brief:
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
#include <Wire.h>

#include "ClockFaces.h"
#include "Animations.h"

#include "PixelMap.h"

#include "LEDCubeDriver.h"
#include "Input.h"
#include "Clock.h"
#include "Buzzer.h"

#define CLOCK_UPDATE_TICK   500L //500ms
#define INPUT_UPDATE_TICK   200L //250ms
#define ANIMATION_UPDATE_TICK   1 //5ms

/* Pin Definition Defines: 
 *  LEDCubeDriver.h:
 *  Shift register pins
 *  Layer ground pins
 *  Use ports or digitalWrite
 *  You can use your own electronics:
 *  All you need is to draw the PixelMap on your LED cube.
 *  The pixel map is an array of bits ordered by layers coordinates 
 * Input.h:
 *  Button pins
 * Buzzer.h:
 *  Buzzer pin
 */

/* User and Mode defines: */
#define ANIMATION_DURATION      60L //60s before choosing another animation in random mode.
#define CLOCK_CUCKOO_DURATION   5 //5min duration of animation when in clock mode (every 15 minutes).


//The middle button can be used to switch mode:
/*  
 *   MODE_CLOCK:
 *    The LED cube is used as a clock.
 *    The buzzer will beep every quarter of an hour and play random animations for CLOCK_CUCKOO_DURATION minutes
 *    The animations are changed every ANIMATION_DURATION seconds
 *    Use the Left/Right button to change the clock face.
 *   MODE_RANDOM_ANIMATION:
 *    The LED cube will randomly play animations.
 *    The animations are changed every ANIMATION_DURATION seconds
 *   MODE_SELECT_ANIMATION:
 *    The left and right buttons can be used to select an animation.
 *    The animation will continue to be played until changed
 */

/* local variables */

static int lvCuckooBusy = false;
static ANIMATION_LIST lvActiveAnimation = (ANIMATION_LIST) random(0,ANIMATION_COUNT);
static long lvNextAnimationTime = 0;

void setup() {
  // put your setup code here, to run once:

  LEDCube_init();

  Clock_Init();
  Clock_FetchTime();

  timeDate* currentTime = Clock_GetTime();
  int timeStamp = currentTime->second + 
                  currentTime->minute + 
                  currentTime->hour + 
                  currentTime->date + 
                  currentTime->month + 
                  currentTime->year;

  /* seed the random number generator using the time */
  randomSeed(timeStamp);

  /*uncomment the lines below and run once to set the time*/
  /*comment the lines and reupload when time is set */
  //timeDate lvDate = {30,46,21,7,20,3,2016};
  //Clock_SetTime(&lvDate);

  Serial.begin(9600);

  #define TOTAL_PIXELS X_PIXELS*Y_PIXELS*Z_PIXELS
  //Serial.println((TOTAL_PIXELS>>3) + ((TOTAL_PIXELS%8)?1:0),DEC);
}


void drawAnimation()
{
  static int nextUpdate = 0;

  switch(Input_GetData()->currentMode)
  {
    case MODE_CLOCK:
    {
      timeDate* currentTime = Clock_GetTime();

      /* every 15 minutes the buzzer should buzz and cuckoo should start */
      if(currentTime->minute%15==0 && !lvCuckooBusy)
      {
        /* buzz for 100 ms */
        Buzzer_Play(100,400);

        lvActiveAnimation = Animation_PickRandomAnimation(lvActiveAnimation);
        lvNextAnimationTime = 0;
        nextUpdate = 0;
        Input_GetData()->selectedFirstCall = true;
        
        lvCuckooBusy = true;
      }
    
      /* reset for next cuckoo time */
      if(currentTime->minute%15>=CLOCK_CUCKOO_DURATION && lvCuckooBusy)
      {
        lvCuckooBusy = false;

        /* buzz for 100 ms */
        Buzzer_Play(100,400);
        
        nextUpdate = 0;
        Input_GetData()->selectedFirstCall = true;
      }

      if(lvCuckooBusy)
      {
        /* if cuckoo is busy show random animations */
        
        if(lvNextAnimationTime>(ANIMATION_DURATION*1000L)/ANIMATION_UPDATE_TICK)
        {
          lvActiveAnimation = Animation_PickRandomAnimation(lvActiveAnimation);
          lvNextAnimationTime = 0;
          Input_GetData()->selectedFirstCall = true;
        }
        lvNextAnimationTime++;

        nextUpdate--;
        if(nextUpdate>0)
        {
          /* Not yet time to update animation*/
         return;
        }

        nextUpdate = Animation_TickAnimation(lvActiveAnimation, Input_GetData()->selectedFirstCall);
        nextUpdate = nextUpdate/ANIMATION_UPDATE_TICK;
        
        Input_GetData()->selectedFirstCall = false;
      }
      else
      {
        /* else cuckoo not busy show clock */
        
        nextUpdate--;
        if(nextUpdate>0)
        {
          /* Not yet time to update animation*/
         return;
        }
        
        nextUpdate = ClockFaces_TickClock(Input_GetData()->selectedClockFace, Input_GetData()->selectedFirstCall, currentTime);
        nextUpdate=nextUpdate/ANIMATION_UPDATE_TICK;
        
        Input_GetData()->selectedFirstCall = false;
      }
    }
    break;
    case MODE_RANDOM_ANIMATION:
    {
      /* choose a new animation after ANIMATION_DURATION seconds */
      if(lvNextAnimationTime>(ANIMATION_DURATION*1000L)/ANIMATION_UPDATE_TICK)
      {
        lvActiveAnimation = Animation_PickRandomAnimation(lvActiveAnimation);
        lvNextAnimationTime=0;
        Input_GetData()->selectedFirstCall = true;
      }
      lvNextAnimationTime++;
    
      /* run the animation to update the state */
      
      nextUpdate--;
      if(nextUpdate>0)
      {
        /* Not yet time to update animation*/
       return;
      }
    
      nextUpdate = Animation_TickAnimation(lvActiveAnimation, Input_GetData()->selectedFirstCall);
    
      nextUpdate=nextUpdate/ANIMATION_UPDATE_TICK;
      Input_GetData()->selectedFirstCall = false; 
    }
    break;
    case MODE_SELECT_ANIMATION:
    {    
      /* run the animation to update the state */
      
      nextUpdate--;
      if(nextUpdate>0)
      {
        /* Not yet time to update animation*/
       return;
      }
    
      nextUpdate = Animation_TickAnimation(Input_GetData()->selectedAnimation, Input_GetData()->selectedFirstCall);
    
      nextUpdate=nextUpdate/ANIMATION_UPDATE_TICK;
      Input_GetData()->selectedFirstCall = false; 
    }
    break;
  }

  
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long timeSinceLastUpdate = 0;
  static unsigned long timeSinceLastClockUpdate = 0;
  static unsigned long timeSinceLastInputUpdate = 0;

  /* Every ANIMATION_UPDATE_TICK ms call the draw function*/
  if(timeSinceLastUpdate >= ANIMATION_UPDATE_TICK*1000)
  {
    drawAnimation();
    
    timeSinceLastUpdate = 0;
  }


  /* Every CLOCK_UPDATE_TICK ms call the clock function */
  if(timeSinceLastClockUpdate >= CLOCK_UPDATE_TICK*1000)
  {
    Clock_FetchTime();
    
    timeSinceLastClockUpdate = 0;
  }

  /* Every INPUT_UPDATE_TICK ms call the clock function */
  if(timeSinceLastInputUpdate >= INPUT_UPDATE_TICK*1000)
  {
    Input_Tick();
    
    timeSinceLastInputUpdate = 0;
  }

  /* update LED cube */
  unsigned long startTime = micros();
  pixelMap_update();
  unsigned long endTime = micros();

  //Serial.println((endTime-startTime),DEC);

  timeSinceLastUpdate+=(endTime-startTime);
  timeSinceLastClockUpdate+=(endTime-startTime);
  timeSinceLastInputUpdate+=(endTime-startTime);

  Buzzer_Tick((endTime-startTime));

}
