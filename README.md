# LedCubeClock
An LED cube that tells time ands shows animations. The cube has a buzzer, battery backed up RTC and buttons for input.

## Modes:

The LED cube has 3 modes, these modes can be changed by pressing the buttons

MODE_CLOCK:

The LED cube is used as a clock.
 * The buzzer will beep every quarter of an hour and play random animations for CLOCK_CUCKOO_DURATION minutes
 *    The animations are changed every ANIMATION_DURATION seconds
 *    Use the Left/Right button to change the clock face.
 
MODE_RANDOM_ANIMATION:
 
The LED cube will randomly play animations.
 *    The animations are changed every ANIMATION_DURATION seconds
 
MODE_SELECT_ANIMATION:
 
The left and right buttons can be used to select an animation.
 *    The animation will continue to be played until changed

## Setting the time:

The time can be set in LedCube by uncommenting the Clock_SetTime(&lvDate); and change the date and time. Upload to Arduino, run once and then commment again to reupload.

## Configuration:

Some configuration can be made: the ANIMATION_DURATION (60 seconds) defines the seconds to show an animation before choosing another animation in random mode or clock mode. In clock mode the cube will buzz every 15 minutes and play random animations as a cuckoo clock the duration of the CLOCK_CUCKOO_DURATION (5 min default) can be set.

## Clock faces:

The cube has 4 clock faces:
* CLOCKFACE_DIGITAL (default): The cube shows the time and date by sliding the numbers from front to back. The numbers are intended for a 5x5x5 cube.
* CLOCKFACE_DOTS: For a 5x5x5 cube 12 LEDs on each side is used to represent the hours (1-12), minutes (1 LED per 5 min), seconds (1 LED per 5 min) and (12 LEDs per 1 second) for milliseconds.
* CLOCKFACE_ANALOG: The LEDs rotate like a traditional analog clock there are "dials" for: the hours, minutes, seconds and milliseconds.
* CLOCKFACE_CIRCLE: The hour, minute and seconds fill up as circles.

Adding a new clock face is easy. To do this define a new clock face in ClockFaces.h in the CLOCKFACE_LIST and declare the prototype in the same file. In ClockFaces.cpp add the code for your clock face (use the timeDate pointer for time info) and add it to the 
ClockFaces_TickClock switch-case. This is all that is needed to include the Clock face, the left/right buttons can be used to choose the active clock face. 

## Animations:

The code has a total of 22 animations. Most animations will work regardless of cube size.

Adding new animations is easy. To do this define a new animation in Animations.h in the ANIMATION_LIST and declare the prototype in the same file. In Animations.cpp add the code for your animation and add it to the Animation_TickAnimation switch-case. This is all that is needed to include the animation in the rotation for the clock, random mode and selected animations. 

## HAL Porting:

To port to your own cube the following has to be done:

Define your LED cube size:
* In PixelMap.h change X_PIXELS, Y_PIXELS, Z_PIXELS to your cube size.
* In LEDCubeDriver.h change LEDCUBE_COLUMN_LEDS, LEDCUBE_ROW_LEDS, LEDCUBE_LAYERS to your cube size.

LEDCubeDriver.h:
 *  If you cube uses shift registers change the:
  *  Shift register pins
  *  Layer ground pins
  *  Use ports (fast, difficult) or digitalWrite (slow, easy)
 *  You can use your own electronics:
  * All you need is to draw the PixelMap on your LED cube see LEDCubeDriver below.
  
Input.h:
 *  Declare the button pins to use
 
Buzzer.h:
 * Declare the buzzer pin to use
 Clock.h
 * The clock uses the DS1307 with the default I2C pins. Change this if you have a different setup.

## Code Explanation:
* LedCube:

The main sketch is used to control the modes of the LED Cube (Clock, Random animations, Selected animations). It also run the tick functions for updating the Clock/Animation state, determining button state, non-blocking buzzer playing and fetching the time from the RTC. The main sketch also updates the LED cube in a ASAP style. All initialization is done in the main sketch.

* Animations:

Contains the list of animations and code to draw the animations. Most of these animations are computed. An animation needs to be ticked (updated) regularly. An animation takes an boolean argument that if true means this is the first call to the animation and all previous associated state is to be cleared. The animation functions return a number representing the number of ms after which the function needs to be ticked again. Animations update the PixelMap which represents the state of the LEDs in the cube.

* ClockFaces:

Contains the list of clock faces and code to draw the clock faces. Similar to an animation a clock face needs to be ticked (updated) regularly. An clock face takes an boolean argument that if true means this is the first call to the clock face and all previous associated state is to be cleared. Additionally the second parameter provides a timeDate pointer with the time information. The clock face functions return a number representing the number of ms after which the function needs to be ticked again. Clock faces update the PixelMap which represents the state of the LEDs in the cube.

* AnimationHelper:

Contains useful functions to help with implementing animations and clock faces. This includes finding a random pixel in a layer that is either on or off, rotating a pixel inaround the Z axis, orientating a pixel by channging the origin. And the oritation needed to walk around the cube, and change the direction of the Z axis. Orientations are optional and pixels can simply be set with the orientation as 0, but understanding orientations might be difficult but are really useful in easily making animations work in all directions by simply changing the orientation. 

For example AnimationHelper_SetOrientPixel(1, 1, i, 0, PIXEL_SET); will set the LED at (1,1,i) by increasing i the LED will be set at a higher layer. So you can draw a vertical line, but there are 6 sides to the cube so by changing the orienattion of the Z axis lines can be drawn in all 6 directions easily: AnimationHelper_SetOrientPixel(1, 1, i, AnimationHelper_ChangeZAxis(j), PIXEL_SET); where j is between 0,5. Therefore there is no need to perform any calculations about how the axis need to change. This make animations shorter and easier to implement. As a tip focus on making your animation work in the Z direction (i.e. up or down) and then change the Z axis usign AnimationHelper_ChangeZAxis(j).


* PixelMap:

Contains an bit array of the LEDs in the cube and their state. The abstraction makes it easy to set LEDs given X,Y,Z coordinates. The pixelMap_update function updates the entire LED cube. This array is continously written to the LEDs by the LEDCubeDriver.

* LEDCubeDriver:

The LEDCubeDriver is responsible for physically updating the LEDs in the LED cube, it abstracts the actual LED cube hardware from everything else. To do this the state of the PixelMap is continously as fast as possible written to the shift registers, layer by layer. In order to port to a different hardware implementation. the void LEDCube_setLayer(unsigned char* inLayerBits, unsigned char inBitStartOffset, unsigned short inLayer); function has to be implemented. This function takes a bit array with the state of the Leds on the layer (1 for on 0 for off), and the inBitStartOffset where the first bit for the first LED actually starts and the layer to update. This function is called once after each other for all the layers.  

* Clock:

Contains code to read and set the time and date on the RTC (DS1307).

* Buzzer:

Contains code to buzz the buzzer non-blocking for a fixed amount of time and pitch.

* Input:

Contains code to handle the button input and is responsible for changing the mode and other variables that can be changed by button presses.




