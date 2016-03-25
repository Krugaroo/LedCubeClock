/*
* Name: Animations.cpp
* Author: Michael Kruger
* Brief:
* 
* This file contains all the Animations and their code.
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
#include "Animations.h"

#include "AnimationHelper.h"
#include "PixelMap.h"

#include "Arduino.h"

int Animation_TickAnimation(int inID, int firstCall)
{
  int nextUpdate = 0;

  /* To test new animations uncomment the line below and rename to the function you want to test. */
  //return Animation_ToTest(firstCall);
  
  switch(inID)
  {
    //case ANIMATION_BLINK:
    //  nextUpdate = Animation_Blink(firstCall);
    //break;
    case ANIMATION_FADE:
      nextUpdate = Animation_Fade(firstCall);
    break;
    case ANIMATION_SLICE:
      nextUpdate = Animation_Slice(firstCall);
    break;
    case ANIMATION_SPARKLE:
      nextUpdate = Animation_Sparkle(firstCall);
    break;
    //case ANIMATION_CHASE:
    //  nextUpdate = Animation_Chase(firstCall);
    //break;
    case ANIMATION_SQUARE:
      nextUpdate = Animation_Square(firstCall);
    break;         
    case ANIMATION_RANDOM_FILL:
      nextUpdate = Animation_RandomFill(firstCall);
    break;
    case ANIMATION_RAIN:
      nextUpdate = Animation_Rain(firstCall);
    break;
    case ANIMATION_ROCKETS:
      nextUpdate = Animation_Rockets(firstCall);
    break;  
    case ANIMATION_SNAKE:
      nextUpdate = Animation_Snake(firstCall);
    break;
    case ANIMATION_FIREWORKS:
      nextUpdate = Animation_Fireworks(firstCall);
    break;
    case ANIMATION_ZIGZAG:
      nextUpdate = Animation_ZigZag(firstCall);
    break;
    case ANIMATION_DROPUPDOWN:
      nextUpdate = Animation_DropUpDown(firstCall);
    break;
    case ANIMATION_DROPALLUPDOWN:
      nextUpdate = Animation_DropAllUpDown(firstCall);
    break;
    case ANIMATION_DROPMULTIUPDOWN:
      nextUpdate = Animation_DropMultiUpDown(firstCall);
    break;
    case ANIMATION_SMOKE:
      nextUpdate = Animation_Smoke(firstCall);
    break;
    case ANIMATION_SIDESQUARES:
      nextUpdate = Animation_SideSquares(firstCall);
    break; 
    case ANIMATION_SINE:
      nextUpdate = Animation_Sine(firstCall);
    break;
    case ANIMATION_HINGE:
      nextUpdate = Animation_Hinge(firstCall);
    break;         
    case ANIMATION_DROP2STEPUPDOWN:
      nextUpdate = Animation_Drop2StepUpDown(firstCall);
    break;     
    case ANIMATION_SPIN:
      nextUpdate = Animation_Spin(firstCall);
    break;  
    case ANIMATION_ORBIT:
      nextUpdate = Animation_Orbit(firstCall);
    break;            
  }

  return nextUpdate;
}


ANIMATION_LIST Animation_PickRandomAnimation(int inCurrent)
{
  int lvRandomNew = (ANIMATION_LIST) random(0,ANIMATION_COUNT);
  
  do
  {
    lvRandomNew = (ANIMATION_LIST) random(0,ANIMATION_COUNT);
  }
  while(inCurrent==lvRandomNew);

  return (ANIMATION_LIST)lvRandomNew;
}

int Animation_Blink(bool inFirstCall)
{
  static int lvOnOff = 0;

  if(lvOnOff)
  {
    /* turn all off */
    pixelMap_clearAll();
    lvOnOff = 0;
  }
  else
  {
    /* turn all on */
    pixelMap_setAll();
    lvOnOff = 1;
  }

  /* update in 200ms */
  return 200;
}

int Animation_Fade(bool inFirstCall)
{
  static int lvDimLevel = 0;
  static int lvDimCount = 0;
  static int lvDimChangeCount = 0;
  static int lvDimChangeDir = 0;

  lvDimChangeCount++;
  
  if(lvDimChangeCount>25)
  {
    lvDimChangeCount=0;

    if(lvDimChangeDir)
    {
      lvDimCount--;
    }
    else
    {
      lvDimCount++;
    }

    if(lvDimChangeDir && lvDimCount<0)
    {
      lvDimCount=0;
      lvDimChangeDir=0;
    }
    else if(!lvDimChangeDir && lvDimCount>15)
    {
      lvDimCount=15;
      lvDimChangeDir=1;
    }
  }

  if(lvDimLevel>lvDimCount)
  {
    /* turn all off */
    pixelMap_clearAll();
  }
  else
  {
    /* turn all on */
    pixelMap_setAll();
  }

  lvDimLevel++;
  if(lvDimLevel>15)
  {
    lvDimLevel=0;
  }

  /* update in 1ms */
  return 1;
}

int Animation_Slice(bool inFirstCall)
{
  static int lvSlice = 0;
  static int lvSliceDir = 0;
  static int lvSliceAxis = 0;

  /* clear all previous state */
  if(inFirstCall)
  {
    lvSlice = 0;
    lvSliceDir = 0;
    lvSliceAxis = 0;
  }

  pixelMap_clearAll();

  /* set all LEDs in the slice */
  for(int i=0;i<Y_PIXELS;i++)
  {
      for(int j=0;j<X_PIXELS;j++)
      {
        AnimationHelper_SetOrientPixel(i,j, lvSlice, AnimationHelper_ChangeZAxis(lvSliceAxis), PIXEL_SET);
      }
  }

  /* move to next slice */
  if(lvSliceDir)
  {
    lvSlice--;
  }
  else
  {
    lvSlice++;
  }

  if(lvSlice>Z_PIXELS-1 && lvSliceDir==0)
  {
   lvSliceDir = 1;
   lvSlice = Z_PIXELS-1;
  }
  else if(lvSlice<0 && lvSliceDir==1)
  {
   lvSliceDir = 0;
   lvSlice = 0;

   //choose a random orientation of the 6 sides to start 
   lvSliceAxis = random(0,6);
  }

  /* update after 75ms */
  return 75;
}

int Animation_Sparkle(bool inFirstCall)
{
  pixelMap_clearAll();

  /* choose a random amount of LEDs to turn on*/
  int lvAmountOnLEDs = random(10, 100);

  for(int i=0;i<lvAmountOnLEDs;i++)
  {
    /* pick a random LED to turn on */
    AnimationHelper_SetOrientPixel(random(0, X_PIXELS), random(0, Y_PIXELS), random(0, Z_PIXELS), 0, PIXEL_SET);  
  }

  /* update after 30ms */
  return 30;
}

int Animation_Chase(bool inFirstCall)
{
  pixelMap_clearAll();
  
  /* pick a random LED to turn on */
  AnimationHelper_SetOrientPixel(random(0, X_PIXELS), random(0, Y_PIXELS), random(0, Z_PIXELS), 0, PIXEL_SET);  

  /* update after 150ms */
  return 150;
}


int Animation_Square(bool inFirstCall)
{
  static int lvSquareSize = 1;
  static int lvSquareDir = 0;

  /* clear all previous state */
  if(inFirstCall)
  {
    lvSquareSize = 1;
    lvSquareDir = 0;
  }
  
  pixelMap_clearAll();

  /* fill the square for the given size (total LEDs is the cube of square size =volume)*/
  for(int i=0;i<lvSquareSize*lvSquareSize*lvSquareSize;i++)
  {
    int x = (X_PIXELS>>1)-(lvSquareSize>>1) + i%lvSquareSize;
    int y = (Y_PIXELS>>1)-(lvSquareSize>>1) + (i/lvSquareSize)%lvSquareSize;
    int z = (Z_PIXELS>>1)-(lvSquareSize>>1) + (i/lvSquareSize/lvSquareSize)%lvSquareSize;
    
    AnimationHelper_SetOrientPixel(x, y, z, 0, PIXEL_SET);  
  }

  if(lvSquareDir!=1)
  {
    lvSquareSize+=2;
  }
  else
  {
    lvSquareSize-=2;
  }

  /* switch direction */
  if(lvSquareSize>X_PIXELS && lvSquareDir==0)
  {
    lvSquareDir = 1;
    lvSquareSize = X_PIXELS-2; 
  }
  else if(lvSquareSize<0 && lvSquareDir==1)
  {
    lvSquareDir = 0;
    lvSquareSize = -1;  
  }
  
  /* update after 100ms */
  return 100;
}

int Animation_RandomFill(bool inFirstCall)
{
  static int lvTotalRuns = 0;

  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
    lvTotalRuns = 0;
  }

  /* do this 2000 times before clearing all */
  if(lvTotalRuns>=2000)
  {
    pixelMap_clearAll();
    lvTotalRuns = 0;
    return 15;
  }
  lvTotalRuns++;
  
  /* pick a random LED to turn on */
  if(lvTotalRuns<1000)
  {
    AnimationHelper_SetOrientPixel(random(0, X_PIXELS), random(0, Y_PIXELS), random(0, Z_PIXELS), 0, PIXEL_SET);  
  }
  else
  {
    AnimationHelper_SetOrientPixel(random(0, X_PIXELS), random(0, Y_PIXELS), random(0, Z_PIXELS), 0, PIXEL_CLEAR);  
  }

  /* update after 5ms */
  return 5;
}

int Animation_Rain(bool inFirstCall)
{
  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
  }

  /* shift all layers up */
  AnimationHelper_ShiftLayers(0, Z_PIXELS-1, 0, PIXEL_SHIFT_DOWN);

  /* choose a random amount of LEDs to turn on*/
  int lvAmountOnLEDs = random(0, 3);

  for(int i=0;i<lvAmountOnLEDs;i++)
  {
    /* pick a random LED to turn on */
    AnimationHelper_SetOrientPixel(random(0, X_PIXELS), random(0, Y_PIXELS), (Z_PIXELS-1), 0, PIXEL_SET);  
  }

  /* update after 100ms */
  return 100;
}

int Animation_Rockets(bool inFirstCall)
{
  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
  }

  /* shift all layers up */
  AnimationHelper_ShiftLayers(0, Z_PIXELS-1, 0, PIXEL_SHIFT_UP);

  /* choose a random amount of LEDs to turn on*/
  int lvAmountOnLEDs = random(0, 3);

  for(int i=0;i<lvAmountOnLEDs;i++)
  {
    /* pick a random LED to turn on */
    AnimationHelper_SetOrientPixel(random(0, X_PIXELS), random(0, Y_PIXELS), 0, 0, PIXEL_SET);  
  }

  /* update after 100ms */
  return 100;
}

int Animation_Snake(bool inFirstCall)
{
  static int curX = 0;
  static int curY = 0;
  static int curZ = 0;
  static int prevX = 0;
  static int prevY = 0;
  static int prevZ = 0;
  static int prev2X = 0;
  static int prev2Y = 0;
  static int prev2Z = 0;
  
  static int destX = 0;
  static int destY = 0;
  static int destZ = 0;
  
  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
    
    curX = 0;
    curY = 0;
    curZ = 0;
    
    destX = 0;
    destY = 0;
    destZ = 0;
  }

  /* reached current destination choose new one */
  if(curX==destX && curY==destY && curZ==destZ)
  {
    /* choose a destination side */
    int side = AnimationHelper_FindDifferentRandom(side,0,6);

   /* pick a random X,Y and change the orientation of the Z axis.*/
    AnimationHelper_OrientPixel(random(0, X_PIXELS),random(0, Y_PIXELS),Z_PIXELS-1, AnimationHelper_ChangeZAxis(side), &destX, &destY, &destZ);
  }

  /* clear oldest pixel */
  AnimationHelper_SetOrientPixel(prev2X,prev2Y, prev2Z, 0, PIXEL_CLEAR);

  prev2X = prevX;
  prev2Y = prevY;
  prev2Z = prevZ;

  prevX = curX;
  prevY = curY;
  prevZ = curZ;

  /* move towards destination */
    
  if(curX<destX) curX++;
  if(curX>destX) curX--;

  if(curY<destY) curY++;
  if(curY>destY) curY--;

  if(curZ<destZ) curZ++;
  if(curZ>destZ) curZ--;

  /* set new pixel */
  AnimationHelper_SetOrientPixel(curX,curY, curZ, 0, PIXEL_SET); 
  
  /* update after 75ms */
  return 75;
}

int Animation_Fireworks(bool inFirstCall)
{
  static int lvExplosionFall = 0;
  static int lvX = 0;
  static int lvY = 0;
  static int lvZ = 0;
  static int lvCurZ = 0;

  /* clear all previous state */
  if(inFirstCall)
  {
    lvExplosionFall = 0;
    lvX = random(0,5);
    lvY = random(0,5);
    lvZ = 4;
  }
  
  /* move rocket up*/
  if(lvCurZ<lvZ)
  {
    pixelMap_clearAll();
         
    /* only if already launched */
    if(lvCurZ>=0)
    {
      AnimationHelper_SetOrientPixel(lvX, lvY, lvCurZ, 0, PIXEL_SET);
    }
    
     lvCurZ++;
  }
  else if(lvExplosionFall<=0)
  {
    for(int i=0;i<5;i++)
    {
      int x=lvX+random(-1,1);
      if(x<0){x=0;}
      int y=lvY+random(-1,1);
      if(y<0){y=0;}
      int z=lvZ+random(-1,1);
      if(z<0){z=0;}
      
      AnimationHelper_SetOrientPixel(x, y, z, 0, PIXEL_SET);
    }
    lvExplosionFall+=1;
  }
  else if(lvExplosionFall<=2)
  {
    for(int i=0;i<5;i++)
    {
      int x=lvX+random(-2,2);
      if(x<0){x=0;}
      int y=lvY+random(-2,2);
      if(y<0){y=0;}
      int z=lvZ+random(-2,2);
      if(z<0){z=0;}
      
      AnimationHelper_SetOrientPixel(x, y, z, 0, PIXEL_SET);
    }
    lvExplosionFall+=1;
  }
  else if(lvExplosionFall<Z_PIXELS*2)
  {
    /* shift all layers down */
    AnimationHelper_ShiftLayers(0, Z_PIXELS-1, 0, PIXEL_SHIFT_DOWN);
    
    lvExplosionFall+=1;
  }
 
  /* finished go to next rocket */
  if(lvExplosionFall>=Z_PIXELS+5)
  {
    lvExplosionFall = 0; 
  
    lvX = random(1,5);
    lvY = random(1,5);
    lvZ = 4;

    /* put rocket on ground - to delay launch*/
    lvCurZ = random(3,5)*-1;
  }

  if(lvExplosionFall>2)
  {
    /* slow down fall */
    return 250;
  }
  else if(lvCurZ==lvZ)
  {
    /* speed up explosions */
    return 75;
  }
  
  /* update after 150ms */
  return 150;
}

int Animation_ZigZag(bool inFirstCall)
{
  static int lvX = 0;
  static int lvXdir = 0;
  static int lvY = 0;
  static int lvYdir = 0;
  static int lvZ=0;
  static int lvZdir=0;
  static int lvSide = 0;
  static pixelMapValue lvPixelState = PIXEL_SET;

  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
    
    lvX = 0;
    lvXdir = 0;
    lvY = 0;
    lvYdir = 0;
    lvZ = 0;
    lvZdir = 0;
    lvSide = random(0,15);
    lvPixelState = PIXEL_SET;
  }

  AnimationHelper_SetOrientPixel(lvX, lvY, lvZ, AnimationHelper_ChangeZAxis(lvSide), lvPixelState);

  if(lvXdir)
  {
    lvX--;
  }
  else
  {
    lvX++;
  }

  /* finished with x inc y */
  if((lvX>(X_PIXELS-1) && lvXdir==0) || (lvX<0 && lvXdir==1))
  {
      if(lvXdir==0)
      {
        lvXdir = 1;
        lvX = (X_PIXELS-1);
      }
      else
      {
        lvXdir = 0;
        lvX = 0;
      }

      if(lvYdir)
      {
        lvY--;
      }
      else
      {
        lvY++;
      }
  }

  /* finished with y increment z */
  if((lvY>(Y_PIXELS-1) && lvYdir==0) || (lvY<0 && lvYdir==1))
  {
      if(lvYdir==0)
      {
        lvYdir = 1;
        lvY = (Y_PIXELS-1);
      }
      else
      {
        lvYdir = 0;
        lvY = 0;
      }

      if(lvZdir)
      {
        lvZ--;
      }
      else
      {
        lvZ++;
      }
  }

  if((lvZ>(Z_PIXELS-1) && lvZdir==0) || (lvZ<0 && lvZdir==1))
  {
    /* finished filling cube start clearing it.*/
    if(lvZdir==0)
    {
      lvZdir = 1;
      lvZ = (Z_PIXELS-1);

      lvPixelState = PIXEL_CLEAR;
    }
    else
    {
      /* finished reset and move to next */
      lvZdir = 0;
      lvZ = 0;

      lvPixelState = PIXEL_SET;

      /* finished choose next side to start at */
      lvSide = AnimationHelper_FindDifferentRandom(lvSide, 0, 6);
    }
  }

  /* update after 30ms */
  return 30;
}

int Animation_DropUpDown(bool inFirstCall)
{
  static int upDown=0;
  static int movedDistance=0;
  static int side=0;
  static int totalMoved=0;

  static int x=0;
  static int y=0;
  
   /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();

    for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
    {
      /* set status of leds randomly across top and bottom layer */
      AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, random(0,2)*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
    }
  }

  if(movedDistance==0)
  {
    /* move a random  LED up/down by 1 */
    int newx = random(0, X_PIXELS);
    while(newx==x)
    {
      newx = random(0, X_PIXELS);
    }
    x = newx;
    y = random(0, Y_PIXELS);
    
    pixelMapValue lvValue = AnimationHelper_GetOrientPixel(x,y, (upDown)?Z_PIXELS-1:0, AnimationHelper_ChangeZAxis(side));
        
    AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-2:1, AnimationHelper_ChangeZAxis(side), lvValue);

    AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-1:0, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);

    movedDistance=1;
 
  }

  if(movedDistance<Z_PIXELS)
  {
    if(upDown==0)
    {
      for(int j=Z_PIXELS-1;j>0;j--)
      {
        /* get status of lower layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(x,y, j-1, AnimationHelper_ChangeZAxis(side));
  
        if(lvValue!=PIXEL_CLEAR)
        {
          AnimationHelper_SetOrientPixel(x,y, j, AnimationHelper_ChangeZAxis(side), lvValue);  
        }
        
        AnimationHelper_SetOrientPixel(x,y, j-1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);  
      }
    }
    else
    {
      for(int j=0;j<Z_PIXELS-1;j++)
      {
        /* get status of upper layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(x,y, j+1, AnimationHelper_ChangeZAxis(side));
  
        if(lvValue!=PIXEL_CLEAR)
        {
          AnimationHelper_SetOrientPixel(x,y, j, AnimationHelper_ChangeZAxis(side), lvValue);  
        }
        
        AnimationHelper_SetOrientPixel(x,y, j+1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);  
      }  
    }
    
    movedDistance++;
  }
  else
  {
    totalMoved++;

    if(totalMoved>100)
    {
      totalMoved=0;
      side = random(0,6);

      pixelMap_clearAll();

      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* set status of leds randomly across top and bottom layer */
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, random(0,2)*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
      }      
    }
    
    if(upDown)
    {
      upDown=0;
    }
    else
    {
      upDown=1;
    }
    
    movedDistance = 0;
  }
 
  /* update after 50ms */
  return 50;
}

int Animation_DropAllUpDown(bool inFirstCall)
{
  static int upDown=0;
  static int totalMoved=0;
  static int side=0;

  static int x=0;
  static int y=0;
  
   /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();

    totalMoved=0;
    upDown=0;
    
    for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
    {
      /* set status of leds randomly across top and bottom layer */
      AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, upDown*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
    }
  }

  if(upDown==0)
  {
    for(int j=Z_PIXELS-1;j>1;j--)
    {
      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* get status of upper layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, AnimationHelper_ChangeZAxis(side));
  
        if(lvValue!=PIXEL_CLEAR)
        {
          AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, AnimationHelper_ChangeZAxis(side), lvValue);  
        }
      
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);
      }  
    }
  }
  else
  {

    for(int j=0;j<Z_PIXELS-2;j++)
    {
      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* get status of upper layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, AnimationHelper_ChangeZAxis(side));
  
        if(lvValue!=PIXEL_CLEAR)
        {
          AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, AnimationHelper_ChangeZAxis(side), lvValue);  
        }
        
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);
      }  
    }  
  }

  /* pick LED to move */
  if(totalMoved<X_PIXELS*Y_PIXELS)
  {
    AnimationHelper_FindRandomPixel(&x, &y, (upDown)?Z_PIXELS-1:0, PIXEL_SET, AnimationHelper_ChangeZAxis(side));
    
    AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-2:1, AnimationHelper_ChangeZAxis(side), PIXEL_SET);
    AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-1:0, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);
  }
    
  totalMoved++;

  /* if all moved then reverse direction */
  if(totalMoved>=(X_PIXELS*Y_PIXELS+Z_PIXELS))
  {        
    if(upDown)
    {
      upDown=0;
      side = random(0,6);

      pixelMap_clearAll();

      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* set status of leds randomly across top and bottom layer */
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, upDown*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
      } 
    }
    else
    {
      upDown=1;
    }

    totalMoved=0;

    return 400;
  }
 
  /* update after 30ms */
  return 30;
}

int Animation_Smoke(bool inFirstCall)
{
 static int curX,destX,curY,destY=0;
 static int lvSide = 0,lvCount=0;;

  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
    
    lvCount=0;
    
    /* pick a side to use */
    lvSide = random(0,6);
  }

  /* shift all Layers up */
  AnimationHelper_ShiftLayers(0, Z_PIXELS-1, AnimationHelper_ChangeZAxis(lvSide), PIXEL_SHIFT_UP);

 /* reached current destination choose new one */
  if(curX==destX && curY==destY)
  {
    destX = random(0, X_PIXELS);
    destY = random(0, Y_PIXELS); 
  }

  /* move towards destination */
  if(curX<destX) curX++;
  if(curX>destX) curX--;

  if(curY<destY) curY++;
  if(curY>destY) curY--;

  /* set new pixel */
  AnimationHelper_SetOrientPixel(curX,curY, 0, AnimationHelper_ChangeZAxis(lvSide), PIXEL_SET); 

  lvCount++;
  if(lvCount>200)
  {
    lvCount=0;
    lvSide=random(0,6);
  }

  /* update after 100ms */
  return 100;
}

int Animation_DropMultiUpDown(bool inFirstCall)
{
  static int upDown=0;
  static int movedDistance=0;
  static int side=0;
  
  /* clear all previous state */
  if(inFirstCall)
  {
    upDown=0;
    movedDistance=0;
    
    pixelMap_clearAll();

    for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
    {
      /* set status of leds randomly across top and bottom layer */
      AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, random(0,2)*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
    }

    return 150;
  }

  if(movedDistance==0)
  {
    /* move random  LEDa up/down by 1 */  
    int x = 0;
    int y = 0;
   
    /* choose a random amount of LEDs to turn on*/
    int lvAmountOnLEDs = random(0,10);
    
    for(int i=0;i<lvAmountOnLEDs;i++)
    {
      x = random(0, X_PIXELS);
      y = random(0, Y_PIXELS);
    
      pixelMapValue lvValue = AnimationHelper_GetOrientPixel(x,y, (upDown)?Z_PIXELS-1:0, AnimationHelper_ChangeZAxis(side));

      if(lvValue==PIXEL_SET)
      {
        AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-1:0, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);  
      }
      
      /* pick a random LED to turn on */
      AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-2:1, AnimationHelper_ChangeZAxis(side), PIXEL_SET);  
    }

    movedDistance=1;
  }
  else if(movedDistance<5)
  {
    if(upDown==0)
    {
      for(int j=Z_PIXELS-1;j>1;j--)
      {
        for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
        {
          /* get status of upper layer */
          pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, AnimationHelper_ChangeZAxis(side));
        
          if(lvValue!=PIXEL_CLEAR)
          {
            AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, AnimationHelper_ChangeZAxis(side), lvValue);  
          }
        
          AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);  
        }  
      }   
    }
    else
    {
      for(int j=0;j<Z_PIXELS-2;j++)
      {
        for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
        {
          /* get status of upper layer */
          pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, AnimationHelper_ChangeZAxis(side));
        
          if(lvValue!=PIXEL_CLEAR)
          {
            AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, AnimationHelper_ChangeZAxis(side), lvValue);  
          }
        
          AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);  
        }  
      }
    }
    
    movedDistance++;
  }
  else
  {
    if(upDown)
    {
      upDown=0;
      side=random(0,6);

      pixelMap_clearAll();

      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* set status of leds randomly across top and bottom layer */
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, random(0,2)*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
      }
    }
    else
    {
      upDown=1;
    }
    
    movedDistance = 0;
  }
 
  /* update after 50ms */
  return 50;
}

int Animation_Drop2StepUpDown(bool inFirstCall)
{
  static int upDown=0;
  static int totalMoved=0;
  static int pauseTotal=0,pauseTime=0;
  static int side=0;

  static int x=0;
  static int y=0;
  
   /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();

    totalMoved=0;
    pauseTime=0;
    pauseTotal=X_PIXELS*3;
    upDown=0;
    
    for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
    {
      /* set status of leds randomly across top and bottom layer */
      AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, upDown*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
    }
  }

  if(upDown==0)
  {
    for(int j=Z_PIXELS-1;j>1;j--)
    {
      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* get status of upper layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, AnimationHelper_ChangeZAxis(side));
  
        if(lvValue!=PIXEL_CLEAR)
        {
          AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, AnimationHelper_ChangeZAxis(side), lvValue);  
        }
      
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);
      }  
    }
  }
  else
  {

    for(int j=0;j<Z_PIXELS-2;j++)
    {
      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* get status of upper layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, AnimationHelper_ChangeZAxis(side));
  
        if(lvValue!=PIXEL_CLEAR)
        {
          AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, AnimationHelper_ChangeZAxis(side), lvValue);  
        }
        
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);
      }  
    }  
  }

  /* pick LED to move */
  if(totalMoved<X_PIXELS*Y_PIXELS && ((pauseTotal)>pauseTime || pauseTime>(pauseTotal)))
  {
    AnimationHelper_FindRandomPixel(&x, &y, (upDown)?Z_PIXELS-1:0, PIXEL_SET, AnimationHelper_ChangeZAxis(side));
    
    AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-2:1, AnimationHelper_ChangeZAxis(side), PIXEL_SET);
    AnimationHelper_SetOrientPixel(x,y, (upDown)?Z_PIXELS-1:0, AnimationHelper_ChangeZAxis(side), PIXEL_CLEAR);
    totalMoved++;
  }
    

  pauseTime++;
  if(pauseTime==pauseTotal)
  {
    return 500;
  }

  /* if all moved then reverse direction */
  if(pauseTime>=(X_PIXELS*Y_PIXELS+Z_PIXELS+Z_PIXELS))
  {        
    if(upDown)
    {
      upDown=0;

      side = random(0,6);

      pixelMap_clearAll();

      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* set status of leds randomly across top and bottom layer */
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, upDown*(Z_PIXELS-1), AnimationHelper_ChangeZAxis(side), PIXEL_SET);
      }
    }
    else
    {
      upDown=1;
    }

    totalMoved=0;
    pauseTime=0;
    pauseTotal=X_PIXELS*3;

    return 10;
  }
 
  /* update after 10ms */
  return 10;
}

int Animation_SideSquares(bool inFirstCall)
{
  static int lvSide=0;
  static int lvDir=0;
  static int lvSize=1;
  static int lvSideDuration=0;

  if(inFirstCall)
  {
    lvSide=random(0,15);
    lvDir=0;
    lvSize=1;
    lvSideDuration = 0;
  }

  pixelMap_clearAll();

  for(int i=0;i<lvSize*lvSize*lvSize;i++)
  {
    AnimationHelper_SetOrientPixel(i%lvSize, (i/lvSize)%lvSize, i/(lvSize*lvSize), (lvSide), PIXEL_SET); 
  }

  if(lvDir==0)
  {
    lvSize++;
  }
  else
  {
    lvSize--;
  }

  if((lvSize>Z_PIXELS && lvDir==0) || (lvSize<0 && lvDir==1))
  {
    
    if(lvDir==0)
    {
      lvSize=Z_PIXELS-1;
      lvDir=1;
    }
    else
    {
      lvSize=1;
      lvDir=0;
    }

    lvSideDuration++;

    if(lvSideDuration>1)
    {
      lvSideDuration=0;
      lvSide = AnimationHelper_FindDifferentRandom(lvSide, 0, 24);
    }
  }

  return 75;
}

int Animation_Sine(bool inFirstCall)
{
  static int lvZ=0;
  static int lvDir=0;
  
  /* clear all previous state */
  if(inFirstCall)
  {
    pixelMap_clearAll();
    
    lvZ=Z_PIXELS-1;
    lvDir=1;
  }

  AnimationHelper_ShiftLayers(0, Z_PIXELS-1, 24, PIXEL_SHIFT_DOWN);
  
  AnimationHelper_SetOrientPixel(lvZ,0,Z_PIXELS-1, 24 , PIXEL_SET);
  AnimationHelper_SetOrientPixel(lvZ,1,Z_PIXELS-1, 24 , PIXEL_SET);
  AnimationHelper_SetOrientPixel(lvZ,2,Z_PIXELS-1, 24 , PIXEL_SET);
  AnimationHelper_SetOrientPixel(lvZ,3,Z_PIXELS-1, 24 , PIXEL_SET);
  AnimationHelper_SetOrientPixel(lvZ,4,Z_PIXELS-1, 24 , PIXEL_SET);
  
  if(lvDir==0)
  {
    lvZ++;
  }
  else
  {
    lvZ--;
  }

  if(lvZ>Z_PIXELS-1 && lvDir==0)
  {
    lvDir=1;
    lvZ=Z_PIXELS-1;
  }
  else if(lvZ<0 && lvDir==1)
  {
    lvDir=0;
    lvZ=0;
  }

  /* update after 75ms */
  return 75;
}

int Animation_Hinge(bool inFirstCall)
{
  static int lvZ=0;
  static int lvSide=0;
  static int lvDir=0;
  static int lvSideDuration=0;
  
  /* clear all previous state */
  if(inFirstCall)
  {   
    lvZ=0;
    lvSide=0;
    lvDir=0;
    lvSideDuration=0;
  }

  pixelMap_clearAll();

  if(lvSideDuration<10)
  {
    for(int i=0;i<Z_PIXELS;i++)
    {
      for(int j=0;j<X_PIXELS;j++)
      { 
       AnimationHelper_SetOrientPixel(j-lvZ*(j*1.0/(X_PIXELS-1)),i,lvZ*(j*1.0/(X_PIXELS-1)), (lvSide), PIXEL_SET); 
      }      
    }
  }

  if(lvDir==0)
  {
    lvZ++;
  }
  else
  {
    lvZ--;
  }

  if(lvZ>(Z_PIXELS-1) && lvDir==0)
  {
    lvDir=1;
    lvZ=(Z_PIXELS-1);
  }
  else if(lvZ<0 && lvDir==1)
  {
    lvDir=0;
    lvZ=0;

    lvSideDuration++;

    if(lvSideDuration>0)
    {
      lvSideDuration = 0;
      lvSide = random(0, NUMBER_ORIENTATIONS);
    }
  }

  /* update after 75ms */
  return 75;
}

int Animation_Spin(bool inFirstCall)
{
  static int countUp=0;
  static int side=0;

  #define TOTAL_ORBIT X_PIXELS+X_PIXELS-1+X_PIXELS-1+X_PIXELS-2
  #define TOTAL_ORBIT_INSIDE (X_PIXELS-2)+(X_PIXELS-2)-1+(X_PIXELS-2)-1+(X_PIXELS-2)-2

  pixelMap_clearAll();


  for(int i=0;i<ceil(Z_PIXELS);i++)
  {
     for(int j=0;j<X_PIXELS;j++)
     {
        int offsetCount = (countUp<(j))? (TOTAL_ORBIT-(j-countUp)) : (countUp-(j)); 

        int x=0;
        int y=0;
        int z=0;

        /* use the orientation to easily walk around the edge */
        AnimationHelper_OrientPixel(0,offsetCount%(Z_PIXELS-1),i*2, AnimationHelper_WalkAroundHorizontal(offsetCount/(Z_PIXELS-1)), &x, &y, &z);

        /* change the Z axis to change orientation */
        AnimationHelper_SetOrientPixel(x,y,z, AnimationHelper_ChangeZAxis(side), PIXEL_SET);
     }
  }

  for(int i=0;i<floor(Z_PIXELS/2);i++)
  {
     for(int j=0;j<X_PIXELS-2;j++)
     {
        int offsetCount = (countUp/2<j)? (TOTAL_ORBIT_INSIDE-(j-countUp/2)) : (countUp/2-j);

        int x=0;
        int y=0;
        int z=0;

        /* use the orientation to easily walk around the edge */
        AnimationHelper_OrientPixel(1,1+offsetCount%2,1+i*2, AnimationHelper_WalkAroundHorizontal(offsetCount/2), &x, &y, &z);

        /* change the Z axis to change orientation */
        AnimationHelper_SetOrientPixel(x,y,z, AnimationHelper_ChangeZAxis(side), PIXEL_SET);
     }
  }

  countUp++;
  if(countUp>=TOTAL_ORBIT)
  {
    countUp=0;
    side=random(0,6);
  }
  
  return 50;
}

int Animation_Orbit(bool inFirstCall)
{
  static int countUp=0;
  static int countDown=0;
  static int side=0;
  static int sideDuration=0;

  #define TOTAL_ORBIT X_PIXELS+X_PIXELS-1+X_PIXELS-1+X_PIXELS-2

  pixelMap_clearAll();


  for(int i=0;i<floor(Z_PIXELS);i++)
  {
     for(int j=0;j<X_PIXELS;j++)
     {
        int offsetCount = (countDown<j)? (TOTAL_ORBIT-(j-countDown)) : (countDown-j);
        
        int x=0;
        int y=0;
        int z=0;

        /* use the orientation to easily walk around the edge */
        AnimationHelper_OrientPixel(0,offsetCount%(Z_PIXELS-1),i, AnimationHelper_WalkAroundHorizontal(offsetCount/(Z_PIXELS-1)), &x, &y, &z);

        /* change the Z axis to change orientation */
        AnimationHelper_SetOrientPixel(x,y,z, AnimationHelper_ChangeZAxis(side), PIXEL_SET);
     }
  }

  countUp++;
  if(countUp>=TOTAL_ORBIT)
  {
    countUp=0;

    sideDuration++;
    if(sideDuration>2)
    {
      sideDuration=0;
      
      side=random(0,6);
    }
  }

  countDown--;
  if(countDown<0)
  {
    countDown=TOTAL_ORBIT;
  }
  
  return 50;
}

