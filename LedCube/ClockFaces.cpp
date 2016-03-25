/*
* Name: ClockFaces.cpp
* Author: Michael Kruger
* Brief:
* 
* This file contains the code for the various clock faces that can be used on the LedCube.
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
#include "ClockFaces.h"

#include "AnimationHelper.h"
#include "PixelMap.h"

#include "Buzzer.h"

#include "Arduino.h"

int ClockFaces_TickClock(int inID, int firstCall, timeDate* inTime)
{
  int nextUpdate=0;

    /* To test new clock faces uncomment the line below and rename to the function you want to test. */
  //return ClockFaces_ToTest(firstCall, inTime);

  switch(inID)
  {
    case CLOCKFACE_DIGITAL:
      nextUpdate = ClockFaces_Digital(firstCall, inTime);
    break;
    case CLOCKFACE_DOTS:
      nextUpdate = ClockFaces_Dots(firstCall, inTime);
    break;
    case CLOCKFACE_ANALOG:
      nextUpdate = ClockFaces_Analog(firstCall, inTime);
    break;
    case CLOCKFACE_ANALOG_CIRCLE:
      nextUpdate = ClockFaces_AnalogCircle(firstCall, inTime);
    break;
  }

  return nextUpdate;
}

int ClockFaces_Dots(int inFirstCall, timeDate* inTime)
{
  static int rotateCount = 0;
  static int fill = 0;

  #define TOTAL_ORBIT (X_PIXELS+X_PIXELS-1+X_PIXELS-1+X_PIXELS-2)
  #define TOTAL_FILL ((X_PIXELS-2)*(Y_PIXELS-1))

  if(inFirstCall)
  {
    pixelMap_clearAll();
    fill = 0;
  }

  /* update every second */
  if(fill==0)
  {
    pixelMap_clearAll();
    
    for(int i=0;i<(X_PIXELS-2)*(Y_PIXELS-1)*inTime->second/60;i++)
    {
      int offset = (1+i%(X_PIXELS-2)+rotateCount/4);
  
      AnimationHelper_SetOrientPixel(0, offset%(Z_PIXELS-1),i/(X_PIXELS-2)+1, AnimationHelper_WalkAroundHorizontal((offset/(Z_PIXELS-1) + 0)%4), PIXEL_SET);
    }
  
    for(int i=0;i<(X_PIXELS-2)*(Y_PIXELS-1)*inTime->minute/60;i++)
    {
      int offset = (1+i%(X_PIXELS-2)+rotateCount/4);
      
      AnimationHelper_SetOrientPixel(0, offset%(Z_PIXELS-1),i/(X_PIXELS-2)+1, AnimationHelper_WalkAroundHorizontal((offset/(Z_PIXELS-1) + 1)%4), PIXEL_SET);
    }
  
    for(int i=0;i<(X_PIXELS-2)*(Y_PIXELS-1)*(inTime->hour%12)/12;i++)
    {
      int offset = (1+i%(X_PIXELS-2)+rotateCount/4);
      
      AnimationHelper_SetOrientPixel(0, offset%(Z_PIXELS-1),i/(X_PIXELS-2)+1, AnimationHelper_WalkAroundHorizontal((offset/(Z_PIXELS-1) + 2)%4), PIXEL_SET);
    }
  }

  for(int i=0;i<(X_PIXELS-2)*(Y_PIXELS-1);i++)
  {
    int offset = (1+i%(X_PIXELS-2)+rotateCount/4);
    
    AnimationHelper_SetOrientPixel(0, offset%(Z_PIXELS-1),i/(X_PIXELS-2)+1, AnimationHelper_WalkAroundHorizontal((offset/(Z_PIXELS-1) + 3)%4), PIXEL_CLEAR);

    if(i<fill)
    {
      AnimationHelper_SetOrientPixel(0, offset%(Z_PIXELS-1),i/(X_PIXELS-2)+1, AnimationHelper_WalkAroundHorizontal((offset/(Z_PIXELS-1) + 3)%4), PIXEL_SET);
    }
  }

  fill++;
  if(fill>=TOTAL_FILL)
  {
    fill=0;
  }

  return 1000/TOTAL_FILL;
}



int ClockFaces_Analog(int inFirstCall, timeDate* inTime)
{
  static int degrees = 0, degreesSecond=0;
  int x=0;
  int y=0;

  if(inFirstCall)
  {
    degrees = 0;
    degreesSecond=0;
  }

  pixelMap_clearAll();

  degrees=(inTime->hour%12)*360/12 + 180;

  for(int i=0;i<X_PIXELS - X_PIXELS/2;i++)
  {
    AnimationHelper_RotatePixel(X_PIXELS/2+i, Y_PIXELS/2, degrees, &x, &y);

    AnimationHelper_SetOrientPixel(x, y, 0, AnimationHelper_ChangeZAxis(2), PIXEL_SET);
  }

  degrees=(inTime->minute)*360/60 + 180;

  for(int i=0;i<X_PIXELS - X_PIXELS/2;i++)
  {
    AnimationHelper_RotatePixel(X_PIXELS/2+i, Y_PIXELS/2, degrees, &x, &y);

    AnimationHelper_SetOrientPixel(x, y, 1, AnimationHelper_ChangeZAxis(2), PIXEL_SET);
  }

  degrees=(inTime->second)*360/60 + 180;

  for(int i=0;i<X_PIXELS - X_PIXELS/2;i++)
  {
    AnimationHelper_RotatePixel(X_PIXELS/2+i, Y_PIXELS/2, degrees, &x, &y);

    AnimationHelper_SetOrientPixel(x, y, 2, AnimationHelper_ChangeZAxis(2), PIXEL_SET);
  }

  for(int i=0;i<X_PIXELS - X_PIXELS/2;i++)
  {
    AnimationHelper_RotatePixel(X_PIXELS/2+i, Y_PIXELS/2, degreesSecond, &x, &y);

    AnimationHelper_SetOrientPixel(x, y, 3, AnimationHelper_ChangeZAxis(2), PIXEL_SET);
  }

  degreesSecond+=45;
  if(degreesSecond>=360)
  {
    
    degreesSecond=0;
  }

  return 125;
}

int ClockFaces_AnalogCircle(int inFirstCall, timeDate* inTime)
{
  int offset=0;
  int x=0;
  int y=0;

  #define CIRCLE_SIZE ((Y_PIXELS-2)*2+(X_PIXELS-2)*2)

  #define CIRCLE_POS(offset,x,y) if(offset<(Y_PIXELS-2)){y=0,x=1+offset;} \
                            else if(offset<(Y_PIXELS-2)*2){y=1+(offset-(Y_PIXELS-2));x=(X_PIXELS-1);} \
                            else if(offset<(Y_PIXELS-2)*3){y=(X_PIXELS-1);x=(X_PIXELS-2)-(offset-(Y_PIXELS-2)*2);} \
                            else if(offset<(Y_PIXELS-2)*4){y=(X_PIXELS-2)-(offset-(Y_PIXELS-2)*3);x=0;} 
  #define DIRECTION 1

  if(inFirstCall)
  {
    offset=0;
  } 
  
  pixelMap_clearAll();

  offset = (((inTime->second)*CIRCLE_SIZE/60)+ Z_PIXELS/4);
  for(int i=(Z_PIXELS)/4;i<=offset;i++)
  {
    CIRCLE_POS(((i>=CIRCLE_SIZE)?i-CIRCLE_SIZE:i),x,y);
    AnimationHelper_SetOrientPixel(x, y, (Z_PIXELS-5), DIRECTION, PIXEL_SET);
  }

  offset = (((inTime->minute)*CIRCLE_SIZE/60)+ Z_PIXELS/4);
  for(int i=(Z_PIXELS)/4;i<=offset;i++)
  {
     CIRCLE_POS(((i>=CIRCLE_SIZE)?i-CIRCLE_SIZE:i),x,y);
    AnimationHelper_SetOrientPixel(x, y, (Z_PIXELS-3), DIRECTION, PIXEL_SET);
  }

  offset = (((inTime->hour)%12)*CIRCLE_SIZE/12 + Z_PIXELS/4);
  for(int i=(Z_PIXELS)/4;i<=offset;i++)
  {
     CIRCLE_POS(((i>=CIRCLE_SIZE)?i-CIRCLE_SIZE:i),x,y);
    AnimationHelper_SetOrientPixel(x, y, (Z_PIXELS-1), DIRECTION, PIXEL_SET);
  }

  return 250;
}

void ClockFace_DrawNumber(int inNum, int inOffset, int inDepth)
{

  #define OFFSET_CALC(offset,pos) ((offset+pos>TOTAL_ORBIT)? ((pos+offset)-TOTAL_ORBIT) : (offset+(pos)))%(Z_PIXELS-1)
  #define OFFSET_CALC2(offset,pos) AnimationHelper_WalkAroundHorizontal((((offset+pos>TOTAL_ORBIT)? ((pos+offset)-TOTAL_ORBIT) : (offset+(pos)))/(Z_PIXELS-1))) 

  for(int i=0;i<Z_PIXELS;i++)
  {
  switch(inNum)
  {
    case 0:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 1, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 0, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 1:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 3, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 1, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    break;
    case 2:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 1, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 0, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 3:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 0, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 4:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 5:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 0, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 6:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 1, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 0, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 7:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 8:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 1, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 0, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 0, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 9:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 4, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 4, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 4, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,1), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 2, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 1, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 0, OFFSET_CALC2(inOffset,2), PIXEL_SET);
    break;
    case 10: //:
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 1, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    break;
    case 11: // /
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,2), 3, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,1), 2, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    AnimationHelper_SetOrientPixel(inDepth, OFFSET_CALC(inOffset,0), 1, OFFSET_CALC2(inOffset,0), PIXEL_SET);
    break;
  }
  }
}

int ClockFaces_Digital(int inFirstCall, timeDate* inTime)
{
  static int depth = 0, offset=1;
  static int state=0;
  int x=0;
  int y=0;

  #define TOTAL_ORBIT X_PIXELS+X_PIXELS-1+X_PIXELS-1+X_PIXELS-2

  if(inFirstCall)
  {
    depth = 0;
    state = 0;
  }

  pixelMap_clearAll();

  switch(state)
  {
    case 0:
      ClockFace_DrawNumber(inTime->hour/10,offset+8, depth);
    break;
    case 1:
      ClockFace_DrawNumber(inTime->hour%10,offset+8, depth);
    break;
    case 2:
    case 5:
      ClockFace_DrawNumber(10,offset+9, depth);
    break;
    case 3:
      ClockFace_DrawNumber(inTime->minute/10,offset+8, depth);
    break;
    case 4:
    ClockFace_DrawNumber(inTime->minute%10,offset+8, depth);
    break;
    case 6:
      ClockFace_DrawNumber(inTime->second/10,offset+8, depth);
    break;
    case 7:
      ClockFace_DrawNumber(inTime->second%10,offset+8,depth);
    break;
    case 8:
    case 17:
    break;
    case 9:
      ClockFace_DrawNumber(inTime->date/10,offset+8,depth);
    break;
    case 10:
      ClockFace_DrawNumber(inTime->date%10,offset+8,depth);
    break;
    case 11:
    case 14:
      ClockFace_DrawNumber(11,offset+8, depth);
    break;
    case 12:
      ClockFace_DrawNumber(inTime->month/10,offset+8,depth);
    break;
    case 13:
      ClockFace_DrawNumber(inTime->month%10,offset+8,depth);
    break;
    case 15:
      ClockFace_DrawNumber((inTime->year-2000)/10,offset+8,depth);
    break;
    case 16:
      ClockFace_DrawNumber((inTime->year-2000)%10,offset+8,depth);
    break;
    
  }

  depth++;
  if(depth>=Z_PIXELS)
  {
    depth=0;
    
    state++;
    if(state>17)
    {
      state=0;
    }

    return 125;
 }

  return 125;
}

