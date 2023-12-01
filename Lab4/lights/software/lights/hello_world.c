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
#include <sys/time.h>
#include <unistd.h>

#define PPR 7
#define GR 50

float RPMs = 0.05;	// Target RPMs

float kp = 2;
float kd = 0;
float ki = 0;

//clock_t prevT = 0;			// Previous t
float eprev = 0;			// Previous error
float eIntegral = 0;		// Running Integral of error
float target = 0;

int main()
{
  // Calculate target function coefficient from desired RPMs
  float targetIncr = RPMs * GR * PPR * 60e-3;	// increase in target each cycle to maintain RPM
  printf("%f", targetIncr);

  printf("Hello from Nios II!\n");

  while(1) {
	  float deltaT = 250e-3;	// 250 ms

	  // Get target as a function of time in ms
	  target += targetIncr;
//	  float target = 10000;

	  // Read position from motor component
	  int pos = IORD(MOTOR_0_BASE, 0);

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
	  pwm += 1150;

	  IOWR(MOTOR_0_BASE, 0, pwm);
	  printf("Target %f, Pos: %d, Error: %f, u: %f, PWM: %d, DeltaT %f\n", target, pos, e, u, pwm, deltaT);
	  usleep(250000);
  }
  return 0;
}
