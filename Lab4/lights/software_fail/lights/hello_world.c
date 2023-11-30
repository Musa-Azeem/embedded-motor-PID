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

int main()
{
  printf("Hello from Nios II!\n");

  while(1) {

	  int speed = 2048;
//	  printf("%08x\n", speed);
	  IOWR(MOTOR_0_BASE, 0, speed);
	  int actual_speed = IORD(MOTOR_0_BASE, 0);
	  printf("Speed: %016x\n", actual_speed);
	  usleep(1000000);
  }
  return 0;
}
