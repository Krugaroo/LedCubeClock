/*
* Name: LEDCubeDriver.cpp
* Author: Michael Kruger
* Brief:
* 
* This file contains the hardware abstraction to allow the pixelMap content to be translated to the LED cube state to turn 
* LEDs on and off. To port to your own cube only one function has to be reimplemented:
* LEDCube_setLayer(unsigned char* inLayerBits, unsigned char inBitStartOffset, unsigned short inLayer)
* 
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
#include "LEDCubeDriver.h"

#include <Arduino.h>

#define LEDS_PER_LAYER LEDCUBE_COLUMN_LEDS*LEDCUBE_ROW_LEDS

#ifndef LEDCUBE_USE_PORTS

#define LED_SHIFT_BIT(bitValue) digitalWrite(LEDCUBE_SHIFT_INPUT, ((bitValue)?HIGH:LOW)); \
                                digitalWrite(LEDCUBE_CLOCK_INPUT, HIGH);                  \
                                /*delayMicroseconds(1);*/                                 \
                                digitalWrite(LEDCUBE_CLOCK_INPUT, LOW);

#define LED_LATCH_OUTPUT()      digitalWrite(LEDCUBE_LATCH_CLOCK_INPUT, HIGH);  \
                                /*delayMicroseconds(1);*/                       \
                                digitalWrite(LEDCUBE_LATCH_CLOCK_INPUT, LOW);  

#define LED_LATCH_OUTPUT()      digitalWrite(LEDCUBE_LATCH_CLOCK_INPUT, HIGH);  \
                                /*delayMicroseconds(1);*/                       \
                                digitalWrite(LEDCUBE_LATCH_CLOCK_INPUT, LOW);  

#define SET_LAYER(layer)       digitalWrite(LEDCUBE_GET_LAYER(layer),HIGH);
#define RESET_LAYER(layer)     digitalWrite(LEDCUBE_GET_LAYER(layer),LOW);

#else

#define LED_SHIFT_BIT(bitValue) if(bitValue){LEDCUBE_SHIFT_INPUT_PORT |= LEDCUBE_SHIFT_INPUT_PIN;}else{LEDCUBE_SHIFT_INPUT_PORT &= ~LEDCUBE_SHIFT_INPUT_PIN;} \
                                LEDCUBE_CLOCK_INPUT_PORT |= LEDCUBE_CLOCK_INPUT_PIN;                  \
                                LEDCUBE_CLOCK_INPUT_PORT &= ~LEDCUBE_CLOCK_INPUT_PIN;

#define LED_LATCH_OUTPUT()      LEDCUBE_LATCH_CLOCK_INPUT_PORT |= LEDCUBE_LATCH_CLOCK_INPUT_PIN;                  \
                                LEDCUBE_LATCH_CLOCK_INPUT_PORT &= ~LEDCUBE_LATCH_CLOCK_INPUT_PIN;

#define SET_LAYER(layer)        SET_LAYER_p(LEDCUBE_GET_LAYER_PORT(layer), LEDCUBE_GET_LAYER_PIN(layer));
#define SET_LAYER_p(port,pin)   port |= pin;
#define RESET_LAYER(layer)      RESET_LAYER_p(LEDCUBE_GET_LAYER_PORT(layer), LEDCUBE_GET_LAYER_PIN(layer));
#define RESET_LAYER_p(port,pin) port &= ~pin;
                               
#endif

void LEDCube_init()
{
  pinMode(LEDCUBE_SHIFT_INPUT, OUTPUT);
  pinMode(LEDCUBE_CLOCK_INPUT, OUTPUT);
  pinMode(LEDCUBE_LATCH_CLOCK_INPUT, OUTPUT);

  for(int i=0;i<LEDCUBE_LAYERS;i++)
  {
     pinMode(LEDCUBE_GET_LAYER(i), OUTPUT);
  }   
}

void LEDCube_set(unsigned short inX, unsigned short inY, unsigned short inZ)
{
  /* get position of LED in layer */
  short lvLEDpos = inX + inY*LEDCUBE_COLUMN_LEDS;

  /* shift the LED per layer */
  for(int i=1;i<=LEDS_PER_LAYER;i++)
  {
    LED_SHIFT_BIT((i==(LEDS_PER_LAYER-lvLEDpos)));
  }

   /* Set the layer */
  for(int i=0;i<LEDCUBE_LAYERS;i++)
  {
      RESET_LAYER(i);
  }
   
  /* Latch output */
  LED_LATCH_OUTPUT();
  
  SET_LAYER(inZ);
}

void LEDCube_setLayer(unsigned char* inLayerBits, unsigned char inBitStartOffset, unsigned short inLayer)
{

  /* shift the LED per layer */
  for(int i=LEDS_PER_LAYER-1;i>=0;i--)
  {
    /* bit position from inLayerBits */
    unsigned short lvBitOffset = inBitStartOffset + i;

    unsigned short lvBytePos = ((lvBitOffset)>>3);
    unsigned short lvBitPos  = ((lvBitOffset)%8);
    
    LED_SHIFT_BIT( ( (*(inLayerBits+lvBytePos))&(1<<lvBitPos) ) );
  }

   /* Set the layer */
  for(int i=0;i<LEDCUBE_LAYERS;i++)
  {
    RESET_LAYER(i);
  }
     
  /* Latch output */
  LED_LATCH_OUTPUT();
  
  SET_LAYER(inLayer);       
}

