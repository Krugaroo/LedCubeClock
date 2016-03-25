/*
* Name: Clock.h
* Author: Michael Kruger
* Brief:
* 
* This file contains a hardware abstraction to read/set the date and time on a DS1307.
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

#include "Arduino.h"

#include "Clock.h"

#define CLOCK_DS1307_ADDR 0x68
#define CLOCK_SECONDS_REG 0x00
#define CLOCK_MINUTES_REG 0x01
#define CLOCK_HOURS_REG   0x02
#define CLOCK_DAY_REG     0x03
#define CLOCK_DATE_REG    0x04
#define CLOCK_MONTH_REG   0x05
#define CLOCK_YEAR_REG    0x06
#define CLOCK_CONTROL_REG 0x07

static timeDate timeInfo = {0};

byte Clock_BCDtoDec(byte inVal);  
byte Clock_DectoBCD(byte inVal);  


void Clock_Init(void)
{
  Wire.begin();

  // disable oscillator output
  Wire.beginTransmission(CLOCK_DS1307_ADDR);
  Wire.write(CLOCK_CONTROL_REG);
  Wire.write(0x00);
  Wire.endTransmission();

  // read the second register and write it back with the clock halt cleared
  Wire.beginTransmission(CLOCK_DS1307_ADDR);
  Wire.write(CLOCK_SECONDS_REG);
  Wire.endTransmission(); 
  Wire.requestFrom(CLOCK_DS1307_ADDR, 1);

  //bitwise and with the clock halt to ensure RTC is running
  unsigned char lvSeconds =  Wire.read() & 0b01111111;

  // write the seconds back
  Wire.beginTransmission(CLOCK_DS1307_ADDR);
  Wire.write(CLOCK_SECONDS_REG);
  Wire.write(lvSeconds);
  Wire.endTransmission();
}

void Clock_FetchTime(void)
{
  // change the register to the seconds register
  Wire.beginTransmission(CLOCK_DS1307_ADDR);
  Wire.write(CLOCK_SECONDS_REG);
  Wire.endTransmission();  

  // get the 7 time registers 
  Wire.requestFrom(CLOCK_DS1307_ADDR, 7);

  // do not include the clock halt bit 
  timeInfo.second = Clock_BCDtoDec(Wire.read() & 0b1111111);
  timeInfo.minute = Clock_BCDtoDec(Wire.read());
  // do not include the 12/24 hour notation bit 
  timeInfo.hour = Clock_BCDtoDec(Wire.read() & 0b111111 );
  timeInfo.day = Clock_BCDtoDec(Wire.read());
  timeInfo.date = Clock_BCDtoDec(Wire.read());
  timeInfo.month = Clock_BCDtoDec(Wire.read());
  timeInfo.year = Clock_BCDtoDec(Wire.read()) + 2000;

  // remove to test
  //return;
  
  Serial.print(timeInfo.day,DEC);
  Serial.print(" ");
  Serial.print(timeInfo.year,DEC);
  Serial.print("-");
  Serial.print(timeInfo.month,DEC);
  Serial.print("-");
  Serial.print(timeInfo.date,DEC);
  Serial.print("T");
  Serial.print(timeInfo.hour,DEC);
  Serial.print(":");
  Serial.print(timeInfo.minute,DEC);
  Serial.print(":");
  Serial.println(timeInfo.second,DEC);
   
}

timeDate* Clock_GetTime(void)
{
  return &timeInfo; 
}

void Clock_SetTime(timeDate* inTimeDate)
{
  Wire.beginTransmission(CLOCK_DS1307_ADDR);
  Wire.write(CLOCK_SECONDS_REG);
  Wire.write(Clock_DectoBCD(inTimeDate->second) & 0b1111111);
  Wire.write(Clock_DectoBCD(inTimeDate->minute) & 0b1111111);
  Wire.write(Clock_DectoBCD(inTimeDate->hour) & 0b111111);
  Wire.write(Clock_DectoBCD(inTimeDate->day) & 0b111);
  Wire.write(Clock_DectoBCD(inTimeDate->date) & 0b111111);
  Wire.write(Clock_DectoBCD(inTimeDate->month) & 0b11111);
  Wire.write(Clock_DectoBCD(inTimeDate->year-2000) & 0b11111111);
  Wire.endTransmission();   
}


byte Clock_BCDtoDec(byte inVal)  
{
  // Convert binary coded decimal to normal decimal numbers
  return ( (inVal/16*10) + (inVal%16) );
}

byte Clock_DectoBCD(byte inVal)  
{
  // Convert decimal to binary coded decimal
  return ( ((inVal/10)<<4) + (inVal%10) );
}

