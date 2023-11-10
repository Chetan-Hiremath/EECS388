#include <stdint.h>

#include "eecs388_lib.h"

/*
  Task 2.1: Review eecs388_lib.h and eecs388_lib.c 

  Task 2.2: Implement RGB blinky. 

    - set red, green, and blue leds as output mode, using gpio_mode() function
    - in the main loop, turn on/off a different LED at each iteration. 
      for example, 
         at 1st iteration
            turn on RED led
	    delay 500 ms 
	    turn off RED led
	    delay 300 ms.
	 at 2nd iteration
	    do the same but for GREEN led
	 at 3rd iteration
	    do the same but for BLUE led
	 at 4th iteration
	    back to RED led. 
	    ...
    - use array and modulo (%) operator to change the color. 

  Task 2.3. Implement RGBA (bonus: +1 point)
    - rotate red, green, blue, and white (all LEDs are enabled) colors.
*/

int main()
{
   int gpio[] = {RED_LED , GREEN_LED , BLUE_LED};
   for (int i = 0; i < 3; i++)
   {
      gpio_mode(gpio[i], OUTPUT);
   }  
   int i = 0;
   while(1)
   {
      gpio_write(gpio[0], ON);
      delay(500);
      gpio_write(gpio[0], OFF);
      delay(300);
      gpio_write(gpio[1], ON);
      delay(500);
      gpio_write(gpio[1], OFF);
      delay(300);
      gpio_write(gpio[2], ON);
      delay(500);
      gpio_write(gpio[2], OFF);
      delay(300);
      i = ((i+1) %3);
   }  
}
