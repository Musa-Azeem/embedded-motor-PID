/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <system.h>
#include <alt_types.h>
#include <io.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main()
{
  printf("Hello from Nios II!\n");
  while (1){
//	  IOWR(LED_PWM_0_BASE, 0, 0x3FFFF00);
	  IOWR(LED_PWM_0_BASE, 0, 1);
	  usleep(4000000);
	  int key = 1;
	  key = IORD(0x0, 0);
	  int wait_time;
	  switch (key) {
	      case 13: // key 1
	    	  wait_time = 90000;
	    	  break;
	      case 11: // key 2
	    	  wait_time = 70000;
	    	  break;
	      case 7: // key 3
	    	  wait_time = 50000;
	    	  break;
	      default:
	    	  wait_time = 90000;
	  }
	  printf("%i - %i\n", key, wait_time);


//	  usleep(90000);
//	  /* random int between 0 and 19 */
//	  int r = rand() % 26;
//	  int leds = 0 | (1<<r);
//	  printf("%.8x\n", leds);
  }




  return 0;
}
