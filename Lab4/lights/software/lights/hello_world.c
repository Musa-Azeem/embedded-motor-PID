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
#include <io.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


float kp = 1;
float kd = 0;
float ki = 0;

clock_t currT = 0;			// Current t
clock_t prevT = 0;			// Previous t
int pos = 0;			// Position of motor
float eprev = 0;		//
float eIntegral = 0;

int main()
{
  printf("Hello from Nios II!\n");

  while(1) {
	  int currT = clock();

	  float deltaT = currT - prevT;

	  int target = 10000;


	  int pwm = -2047;  // must be between 1200 and 2047. must be 1350 to start without help
	  IOWR(MOTOR_0_BASE, 0, pwm);
	  int position = IORD(MOTOR_0_BASE, 0);
	  printf("Position: %d\n", position);
//	  usleep(1000000);
  }
  return 0;
}
