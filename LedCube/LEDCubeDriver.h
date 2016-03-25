/*
* Name: LEDCubeDriver.h
* Author: Michael Kruger
* Brief:
* 
* This file contains the hardware abstraction to allow the pixelMap content to be translated to the LED cube state to turn 
* LEDs on and off. To port to your own cube only one function has to be reimplemented:
* LEDCube_setLayer(unsigned char* inLayerBits, unsigned char inBitStartOffset, unsigned short inLayer)
* 
* If you have shiftregisters simply change the pin defines below.
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
#define LEDCUBE_COLUMN_LEDS 5
#define LEDCUBE_ROW_LEDS    5
#define LEDCUBE_LAYERS      5

/* Total time needed to update the cube */
#define LEDCUBE_TOTAL_UPDATE_TIME LEDCUBE_COLUMN_LEDS*LEDCUBE_ROW_LEDS*LEDCUBE_LAYERS /* shifting time 1us per led = (leds per layer) * 1us * number of layers  */ \
                                  + LEDCUBE_LAYERS /* delay time to latch to output */ \
                                  + LEDCUBE_LAYER_DELAY*LEDCUBE_LAYERS /* delay time per layer * layers */

/* Access the ports directly instead of using the Arduino functions.*/
/* If not defined the code will use the Arduino digitalWrite */
/* This is a lot slower than using the ports directly */
/* But is not portable (changes per microcontroller)*/
#define LEDCUBE_USE_PORTS

/* use the digitalWrite with the Arduino Pin numbers*/
/* Always define these even if using the ports */
#define LEDCUBE_SHIFT_INPUT        4
#define LEDCUBE_CLOCK_INPUT        3
#define LEDCUBE_LATCH_CLOCK_INPUT  2

#define LED_CUBE_LAYER0_ENABLE_PIN  6
#define LED_CUBE_LAYER1_ENABLE_PIN  7
#define LED_CUBE_LAYER2_ENABLE_PIN  8
#define LED_CUBE_LAYER3_ENABLE_PIN  9
#define LED_CUBE_LAYER4_ENABLE_PIN  10

/* add more layers if needed */
#define LEDCUBE_GET_LAYER(layer) ((layer==0)?LED_CUBE_LAYER0_ENABLE_PIN:      \
                                  (layer==1)?LED_CUBE_LAYER1_ENABLE_PIN:      \
                                  (layer==2)?LED_CUBE_LAYER2_ENABLE_PIN:      \
                                  (layer==3)?LED_CUBE_LAYER3_ENABLE_PIN:      \
                                  (layer==4)?LED_CUBE_LAYER4_ENABLE_PIN:      \
                                  0                                           \
                                  )

/* NOT NEEDED : */
/* If LEDCUBE_USE_PORTS is defined use the ports directly */
/* This is a lot quicker! */
#define LEDCUBE_SHIFT_INPUT_PORT   PORTD
#define LEDCUBE_SHIFT_INPUT_PIN    0x10

#define LEDCUBE_CLOCK_INPUT_PORT   PORTD
#define LEDCUBE_CLOCK_INPUT_PIN    0x08

#define LEDCUBE_LATCH_CLOCK_INPUT_PORT   PORTD
#define LEDCUBE_LATCH_CLOCK_INPUT_PIN    0x04

#define LEDCUBE_GET_LAYER_PORT(layer) ((layer==0)?PORTD:      \
                                  (layer==1)?PORTD:           \
                                  (layer==2)?PORTB:           \
                                  (layer==3)?PORTB:           \
                                  (layer==4)?PORTB:           \
                                  PORTB                       \
                                  )

#define LEDCUBE_GET_LAYER_PIN(layer) ((layer==0)? 0x40:       \
                                  (layer==1)?0x80:            \
                                  (layer==2)?0x01:            \
                                  (layer==3)?0x02:            \
                                  (layer==4)?0x04:            \
                                  0                           \
                                  )

void LEDCube_init();

void LEDCube_set(unsigned short inX, unsigned short inY, unsigned short inZ);

void LEDCube_setLayer(unsigned char* inLayerBits, unsigned char inBitStartOffset, unsigned short inLayer);
