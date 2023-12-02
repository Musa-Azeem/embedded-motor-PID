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

#define MIN_PWM 1290
#define MAX_PWM 2047
#define DELAY_MS 10

float RPMs = 60;	// Target RPMs

float kp = 30;
float kd = 30;
float ki = 5;

//clock_t prevT = 0;			// Previous t
float eprev = 0;			// Previous error
float eIntegral = 0;		// Running Integral of error
float target = 0;

int map_pwm(float x);

int main()
{
  // Calculate target function coefficient from desired RPMs
  float targetIncr = RPMs * GR * PPR * DELAY_MS / 60e3;	// increase in target pos each cycle to maintain RPM

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
	  float u = kp * e + kd * dedt + ki * eIntegral;

	  // save previous error
	  eprev = e;

	  // Map magnitude of control signal to be between 1130 and 2047
	  // must be between 1130 and 2047. must be 1350 to start without help
	  // (1130 acts as 0, since 1135 is required for motor to run)
	  int sign = u > 0 ? 1 : -1;
	  int pwm = sign * map_pwm(fabs(u));

	  IOWR(MOTOR_0_BASE, 0, pwm);
	  printf("Target %f, Pos: %d, Error: %f, u: %f, PWM: %d\n", target, pos, e, u, pwm);
	  usleep(DELAY_MS*1e3);	// ms to us
  }
  return 0;
}

int map_pwm(float x) {
	// linear interpolate values to be between 1150 and 2047
	if (x > MAX_PWM) {
		return MAX_PWM;
	}
	return (int)((x*(MAX_PWM-MIN_PWM))/MAX_PWM + MIN_PWM);
}
