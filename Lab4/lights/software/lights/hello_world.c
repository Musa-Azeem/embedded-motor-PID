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

#define MIN_PWM 1150
#define MAX_PWM 2047
#define DELAY_MS 250

float RPMs = -0.01;	// Target RPMs

float kp = 1;
float kd = 1;
float ki = 1;

//clock_t prevT = 0;			// Previous t
float eprev = 0;			// Previous error
float eIntegral = 0;		// Running Integral of error
float target = 0;

int main()
{
  // Calculate target function coefficient from desired RPMs
  float targetIncr = RPMs * GR * PPR * 60e-3 * DELAY_MS;	// increase in target pos each cycle to maintain RPM

  printf("Hello from Nios II!\n");

  while(1) {
	  float deltaT = DELAY_MS*1e-3;	// ms to s

	  // Increment target pos by neessary amount each cycle
	  target += targetIncr;

	  // Read position from motor component
	  int pos = IORD(MOTOR_0_BASE, 0);

	  // Calculate error
	  float e = (float)target - pos;

	  // Calculate Derivate and Integral terms
	  float dedt = (float)(e - eprev) / deltaT;
	  eIntegral += e * deltaT;

	  // Calculate control signal - offset so that 0 is MIN_PWM
	  float u = kp * e + kd * dedt + ki * eIntegral + MIN_PWM;

	  // save previous error
	  eprev = e;

	  // Map magnitude of control signal to be between 1150 and 2047
	  // must be between 1150 and 2047. must be 1350 to start without help
	  // (1150 acts as 0, since 1200 is required for motor to run)
	  int pwm = u;
	  if (fabs(u) < MIN_PWM) {
		pwm = u > 0 ? MIN_PWM : -MIN_PWM;
	  } 
	  else if (fabs(u) > MAX_PWM) {
		pwm = u > 0 ? MAX_PWM : -MAX_PWM;
	  }

	  IOWR(MOTOR_0_BASE, 0, pwm);
	  printf("Target %f, Pos: %d, Error: %f, u: %f, PWM: %d, DeltaT %f\n", target, pos, e, u, pwm, deltaT);
	  usleep(DELAY_MS*1e3);	// ms to us
  }
  return 0;
}
