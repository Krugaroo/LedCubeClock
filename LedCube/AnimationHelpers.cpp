/*
* Name: AnimationHelper.cpp
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
#include "AnimationHelper.h"

int AnimationHelper_FindDifferentRandom(int inOldNumber, int inMin, int inMax)
{
  int lvNextNumber = random(inMin,inMax);
  
  while(lvNextNumber==inOldNumber)
  {
    lvNextNumber = random(inMin,inMax);
  }

  return lvNextNumber;
}

/*
 * This function can be used to find a random pixel with the state inWithState
 * on the given inLayer with the orientation inOrientation.
 * The result is stored in the location given by the outX and outY pointers 
 */
void AnimationHelper_FindRandomPixel(int* outX, int *outY, int inLayer, pixelMapValue inWithState, int inOrientation)
{
  //pick a random place to start checking
  int lvStart=random(0,X_PIXELS) + random(0,Y_PIXELS)*Y_PIXELS;
  
  int lvChecked=0;                   
  pixelMapValue lvValue = inWithState;                                                
  
  do                                                                            
  {                                                                             
    lvStart++;                                                                    
    if(lvStart>=X_PIXELS*Y_PIXELS)                                                
    {
      //wrap around to start in layer                                                                           
      lvStart=0;                                                                  
    }                                                                           
    lvValue = AnimationHelper_GetOrientPixel(lvStart%X_PIXELS, lvStart/X_PIXELS, inLayer, inOrientation);

    lvChecked++;
    if(lvChecked>X_PIXELS*Y_PIXELS)
    {
      // No LED with the requested state found in the layer
      *outX=-1;                                                             
      *outY=-1;
      return; 
    }
  }                                                                             
  while(lvValue!=inWithState);                                                        
  
  *outX=lvStart%X_PIXELS;                                                             
  *outY=lvStart/Y_PIXELS;                                                             
}

/* this function can be used to rotate a pixel in the X/Y plane */
void AnimationHelper_RotatePixel(int inX, int inY, int inDegrees, int *outX, int *outY)
{
  int xfromCenter = X_PIXELS/2 - inX;
  int yfromCenter = Y_PIXELS/2 - inY;
  int length = (sqrt((xfromCenter*xfromCenter + yfromCenter*yfromCenter)));

  *outX= round(cos(inDegrees*PI/180)*length)  + X_PIXELS/2;
  *outY = round(sin(inDegrees*PI/180)*length)  + Y_PIXELS/2;  
}

/*
 * These helper functions can be used to easily change the orientation of Pixels
 * These functions basically change the origin of the cube.
 * The advantage is that an animation can be written for one direction and easily
 * animated in all directions by changing the orientation.
 * The orientations are ordered:
 * i.e. 0-3 moves the animation horizontally around the cube using the bottom left corner as the orientation
 * i.e. 3-6 moves the animation vertically around the cube using the bottom left corner as the orientation
 */

void AnimationHelper_OrientPixel(int inX, int inY, int inZ, int inOrientation, int *outX, int *outY, int *outZ)
{
  int lvX=0;
  int lvY=0;
  int lvZ=0;

  int orientX=0;
  int orientY=0;
  int orientZ=0;

  switch(inOrientation)
  {
    case 0:
    orientX=0;orientY=2; orientZ=4;
    break;
    case 1:
    orientX=3;orientY=0;orientZ=4;
    break;
    case 2:
    orientX=1;orientY=3;orientZ=4;
    break;
    case 3:
    orientX=2;orientY=1;orientZ=4;
    break;
    case 4:
    orientX=0;orientY=3;orientZ=4;
    break;
    case 5:
    orientX=3;orientY=1;orientZ=4;
    break;
    case 6:
    orientX=1;orientY=2;orientZ=4;
    break;
    case 7:
    orientX=2;orientY=0;orientZ=4;
    break;
    case 8:
    orientX=0;orientY=2;orientZ=5;
    break;
    case 9:
    orientX=3;orientY=0;orientZ=5;
    break;
    case 10:
    orientX=1;orientY=3;orientZ=5;
    break;
    case 11:
    orientX=2;orientY=1;orientZ=5;
    break;
    case 12:
    orientX=0;orientY=3;orientZ=5;
    break;
    case 13:
    orientX=3;orientY=1;orientZ=5;
    break;
    case 14:
    orientX=1;orientY=2;orientZ=5;
    break;
    case 15:
    orientX=2;orientY=0;orientZ=5;
    break;
    case 16:
    orientX=5;orientY=2;orientZ=1;
    break;
    case 17:
    orientX=5;orientY=3;orientZ=1;
    break;
    case 18:
    orientX=5;orientY=2;orientZ=0;
    break;
    case 19:
    orientX=5;orientY=3;orientZ=0;
    break;
    case 20:
    orientX=4;orientY=2;orientZ=1;
    break;
    case 21:
    orientX=4;orientY=3;orientZ=1;
    break;
    case 22:
    orientX=4;orientY=2;orientZ=0;
    break;
    case 23:
    orientX=4;orientY=3;orientZ=0;
    break;
    case 24:
    orientX=5;orientY=0;orientZ=3;
    break;
    case 25:
    orientX=4;orientY=0;orientZ=2;
    break;
  }

  /* move X axis  */
  switch(orientX)
  {
    case 0:
      lvX=inX;
    break;
    case 1:
      lvX=X_PIXELS-1-inX;
    break;
    case 2:
      lvY=inX;
    break;
    case 3:
      lvY=X_PIXELS-1-inX;
    break;
    case 4:
      lvZ=inX;
    break;
    case 5:
      lvZ=X_PIXELS-1-inX;
    break;
  }

  /* move Y axis  */
  switch(orientY)
  {
    case 0:
      lvX=inY;
    break;
    case 1:
      lvX=Y_PIXELS-1-inY;
    break;
    case 2:
      lvY=inY;
    break;
    case 3:
      lvY=Y_PIXELS-1-inY;
    break;
    case 4:
      lvZ=inY;
    break;
    case 5:
      lvZ=Y_PIXELS-1-inY;
    break;
  }
  
  /* move Z axis  */
  switch(orientZ)
  {
    case 0:
      lvX=inZ;
    break;
    case 1:
      lvX=Z_PIXELS-1-inZ;
    break;
    case 2:
      lvY=inZ;
    break;
    case 3:
      lvY=Z_PIXELS-1-inZ;
    break;
    case 4:
      lvZ=inZ;
    break;
    case 5:
      lvZ=Z_PIXELS-1-inZ;
    break;
  }

  *outX = lvX;
  *outY = lvY;
  *outZ = lvZ;
}

pixelMapValue AnimationHelper_GetOrientPixel(int inX, int inY, int inZ, int inOrientation)
{
  int lvX=0;
  int lvY=0;
  int lvZ=0;

  AnimationHelper_OrientPixel(inX, inY, inZ, inOrientation, &lvX, &lvY, &lvZ);

  return pixelMap_get3Dpixel(lvX, lvY, lvZ);
}

void AnimationHelper_SetOrientPixel(int inX, int inY, int inZ, int inOrientation, pixelMapValue inState)
{
  int lvX=0;
  int lvY=0;
  int lvZ=0;

  AnimationHelper_OrientPixel(inX, inY, inZ, inOrientation, &lvX, &lvY, &lvZ);

  pixelMap_set3Dpixel(lvX, lvY, lvZ, inState);
}

void AnimationHelper_ShiftLayers(int inMinZ, int inMaxZ, int inOrientation, pixelShiftDir inDirection)
{
  if(inDirection == PIXEL_SHIFT_DOWN)
  {
    // shift layers down
    for(int j=inMinZ;j<inMaxZ;j++)
    {
      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* get status of upper layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, inOrientation);
        
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, inOrientation, lvValue);  
  
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j+1, inOrientation, PIXEL_CLEAR);  
      }
    }
  }
  else
  {
    //shift layers up
    for(int j=inMaxZ;j>inMinZ;j--)
    {
      for(int i=0;i<X_PIXELS*Y_PIXELS;i++)
      {
        /* get status of lower layer */
        pixelMapValue lvValue = AnimationHelper_GetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, inOrientation);
        
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j, inOrientation, lvValue);  
  
        AnimationHelper_SetOrientPixel(i%X_PIXELS,i/X_PIXELS, j-1, inOrientation, PIXEL_CLEAR);  
      }
    }
  }
}

int AnimationHelper_WalkAroundHorizontal(int inIndex)
{
  int lvHorizontalSides[4] = {0,1,2,3};

  if(inIndex>3){return 0;}

  return lvHorizontalSides[inIndex];
}

int AnimationHelper_WalkAroundVertical(int inIndex)
{
  int lvVerticalSides[4] = {0,18, 14, 20};

  if(inIndex>3){return 0;}

  return lvVerticalSides[inIndex];
}

int AnimationHelper_WalkAroundVertical2(int inIndex)
{
  int lvVerticalSides[4] = {1, 24, 15, 25};

  if(inIndex>3){return 0;}

  return lvVerticalSides[inIndex];
}

int AnimationHelper_WalkAroundSides(int inIndex)
{
  int lvSides[6] = {0, 1, 2, 3, 18, 20};

  if(inIndex>5){return 0;}

  return lvSides[inIndex];
}

int AnimationHelper_ChangeZAxis(int inIndex)
{
  int lvSides[6] = {0, 8, 16, 18, 24, 25};

  if(inIndex>5){return 0;}

  return lvSides[inIndex];
}


