/*
* Name: Input.cpp
* Author: Michael Kruger
* Brief:
* 
* This file contains the input handling code for the buttons to allow the mode, clock faces and animations to be changed.
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
#include "Input.h"

#include "Buzzer.h"

inputData userInput = {MODE_CLOCK, true, (ANIMATION_LIST)0, (CLOCKFACE_LIST)0};

typedef enum{
  BUTTON_LEFT=0,
  BUTTON_MIDDLE,
  BUTTON_RIGHT,
  BUTTON_NONE
} InputButton;

InputButton Input_ButtonPressed()
{
  if(digitalRead(INPUT_MIDDLE_BUTTON))
  {
    return BUTTON_MIDDLE;
  }
  else if(digitalRead(INPUT_LEFT_BUTTON))
  {
    return BUTTON_LEFT;
  }
  else if(digitalRead(INPUT_RIGHT_BUTTON))
  {
    return BUTTON_RIGHT;
  }

  return BUTTON_NONE;
}

void Input_Tick(void)
{  
  InputButton pressedButton = Input_ButtonPressed();

  if(pressedButton!=BUTTON_NONE)
  {
    //play a tone to let know that button has been pressed
    Buzzer_Play(20, 200);
  }

  switch(userInput.currentMode)
  {
    case MODE_CLOCK:
    {
      switch(pressedButton)
      {
        case BUTTON_MIDDLE:
        {
          //select next mode
          userInput.currentMode = MODE_RANDOM_ANIMATION;
        }
        break;
        case BUTTON_LEFT:
        {
          //select next clockFace
          if(userInput.selectedClockFace>0)
          {
            userInput.selectedClockFace = (CLOCKFACE_LIST)(userInput.selectedClockFace-1);
          }
          else
          {
            userInput.selectedClockFace = (CLOCKFACE_LIST)(CLOCKFACE_COUNT-1);
          } 

          userInput.selectedFirstCall = true;
        }
        break;
        case BUTTON_RIGHT:
        {
          //select next clockFace
          if(userInput.selectedClockFace<(CLOCKFACE_COUNT-1))
          {
            userInput.selectedClockFace = (CLOCKFACE_LIST)(userInput.selectedClockFace+1);
          }
          else
          {
            userInput.selectedClockFace = (CLOCKFACE_LIST)0;
          }

          userInput.selectedFirstCall = true;
        }
        break;
        default:
        /* other buttons do nothing in this mode */
        break;
      }
    }
    break;
    case MODE_RANDOM_ANIMATION:
    {
      switch(pressedButton)
      {
        case BUTTON_MIDDLE:
        {
          //select next mode
          userInput.currentMode = MODE_SELECT_ANIMATION;
        }
        break;
        default:
        /* other buttons do nothing in this mode */
        break;
      }
    }
    break;
    case MODE_SELECT_ANIMATION:
    {
      switch(pressedButton)
      {
        case BUTTON_MIDDLE:
        {
          //select next mode
          userInput.currentMode = MODE_CLOCK;
        }
        break;
        case BUTTON_LEFT:
        {
          //select next mode
          if(userInput.selectedAnimation>0)
          {
            userInput.selectedAnimation = (ANIMATION_LIST)(userInput.selectedAnimation-1);
          }
          else
          {
            userInput.selectedAnimation = (ANIMATION_LIST)(ANIMATION_COUNT-1);
          } 

          userInput.selectedFirstCall = true;
        }
        break;
        case BUTTON_RIGHT:
        {
          //select next mode
          if(userInput.selectedAnimation<(ANIMATION_COUNT-1))
          {
            userInput.selectedAnimation = (ANIMATION_LIST)(userInput.selectedAnimation+1);
          }
          else
          {
            userInput.selectedAnimation = (ANIMATION_LIST)0;
          }

          userInput.selectedFirstCall = true;
        }
        break;
        default:
        /* other buttons do nothing in this mode */
        break;
      }
    }
    break;
  }
}

inputData* Input_GetData(void)
{
  return &userInput;
}

