
#include "sys/alt_stdio.h"
#include <stdio.h>
#include <system.h>
#include <alt_types.h>
#include <io.h>
#include <math.h>
#include <unistd.h>




int main()
{
	float pwm_frequency = 10e3f;
	float dim_period = 2.0f;


	float pwm_period = 1.0f/pwm_frequency;
	float dim_frequency = 1.0f/dim_period;
	alt_u64 pwm_period_in_cycles = (alt_u64)(pwm_period * (float)ALT_CPU_FREQ);
	alt_u64 dim_period_in_cycles = (alt_u64)(dim_period * (float)ALT_CPU_FREQ);


	// Hold counter value
	union {
		struct {
			alt_u32 lo;
			alt_u32 hi;
			} parts;
		alt_u64 raw;
	} cnt;

	// Start Performance Counter
	IOWR(PERFORMANCE_COUNTER_0_BASE,1,0);


	alt_putstr("Hello from Nios II!\n");

	printf("pwm: %f\n", pwm_period_in_cycles);

	/* Event loop never exits. */
	while (1) {
		printf("==============\n");
		// Read upper 32 bits and lower 32 bits of counter
		cnt.parts.hi = IORD(PERFORMANCE_COUNTER_0_BASE,1);
		cnt.parts.lo = IORD(PERFORMANCE_COUNTER_0_BASE,0);
		printf("%d\n", cnt.raw);

		alt_u64 cycle_start = cnt.raw;

		float current_duty_cycle = 0.5;//fabs((cnt.raw % dim_period_in_cycles)*(-2.f/dim_period_in_cycles) + 1.0f);

		int end_on_period = cycle_start + current_duty_cycle * pwm_period_in_cycles;
		int end_pwm_period = cycle_start + pwm_period_in_cycles;

		// First part of pwm cycle (<current_duty_cycle>% of it) - signal high
		while(cnt.raw < end_on_period) {
			// First part of pwm cycle - leds on
			IOWR(LEDS_BASE,0,0x3FFFFFF);
			// Read upper 32 bits and lower 32 bits of counter
			cnt.parts.hi = IORD(PERFORMANCE_COUNTER_0_BASE,1);
			cnt.parts.lo = IORD(PERFORMANCE_COUNTER_0_BASE,0);
		}
		// Second part of pwm cycle (rest of it) - signal low
		while(cnt.raw < end_pwm_period) {
			// First part of pwm cycle - leds on
			IOWR(LEDS_BASE,0,0x0);
			// Read upper 32 bits and lower 32 bits of counter
			cnt.parts.hi = IORD(PERFORMANCE_COUNTER_0_BASE,1);
			cnt.parts.lo = IORD(PERFORMANCE_COUNTER_0_BASE,0);
		}
		printf("%d\n", cnt.raw);


	}

	return 0;
}
