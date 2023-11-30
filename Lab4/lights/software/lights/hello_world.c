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
// #include <system.h>
// #include <io.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define PPR 7
#define GR 50

float RPMs = 30;	// Target RPMs

float kp = 1;
float kd = 0;
float ki = 0;

clock_t prevT = 0;			// Previous t
float eprev = 0;			// Previous error
float eIntegral = 0;		// Running Integral of error

int test = 0;

int main()
	// Calculate target function coefficient from desired RPMs
{
  float targetCoeff = RPMs * GR / PPR / 60e3;

  printf("Hello from Nios II!\n");

  while(1) {
	  clock_t currT = clock(); // get time in microseconds
	  float deltaT = (float)(currT - prevT) / 1.0e6; // Get delta t in seconds
	  prevT = currT; // Set previous time to current time

	  // Get target as a function of time in ms
	  int target = targetCoeff * currT;

	  // Read position from motor component
	  int pos = test;
	//   int pos = IORD(MOTOR_0_BASE, 0);

	  // Calculate error
	  float e = (float)target - pos;

	  // Calculate Derivate and Integral terms
	  float dedt = (float)(e - eprev) / deltaT;
	  eIntegral += e * deltaT;

	  // Calculate control signal
	  float u = kp * e + kd * dedt + ki * eIntegral;

	  // save previous error
	  eprev = e;

	  // Map magnitude of control signal to be between 1150 and 2047
	  // must be between 1150 and 2047. must be 1350 to start without help
	  // (1150 acts as 0, since 1200 is required for motor to run)
	  int pwm = u;
	  if (fabs(u) < 1150) {
		pwm = u > 0 ? 1150 : -1150;
	  } 
	  else if (fabs(u) > 2047) {
		pwm = u > 0 ? 2047 : -2047;
	  }

	//   IOWR(MOTOR_0_BASE, 0, pwm);

	  printf("Target %f, Pos: %d, Error: %d, u: %d, PWM: %d, DeltaT %f\n", target, pos, e, u, pwm, deltaT);
	  test += 1*pwm;
  }
  return 0;
}
