/*
 * Copyright 2021 Jason Bakos, Philip Conrad, Charles Daniels
 *
 * Distributed as part of the University of South Carolina CSCE317 course
 * materials. Please do not redistribute without written authorization.
 */

#include <stdio.h>
#include "waves.h"
#define MOSI "mosi"
#define MISO "miso"
#define SCLK "sclk"
#define SS "ss"
#define CPOL "cpol"
#define CPHA "cpha"

bool check_if_posedge(int cpol, int cpha);

int main(int argc, char** argv) {
	/* This macro silences compiler errors about unused variables. */
	UNUSED(argc);
	UNUSED(argv);

	/* Read the input for the test case into a new waves object. */
	waves* w = parse_file(stdin);
	/* We can use the 'log' function like printf() to record information to
	 * standard error, this way it will not interfere with the data sent to
	 * standard out we wish for grade.sh to interpret as our solution to
	 * the testcase. */
	log("read a waves file with %i signals and %i samples\n", w->nsignals, w->nsamples);
	log("input has these signals:\n");
	for (unsigned int i = 0 ; i < w->nsignals ; i++) {
		// index2signal() lets us get the human-readable name of
		// the signal from its index.
		//
		// log(signal_at_idx(w, signal2index("mosi"), 0))
		// w->widths[i] tells us the number of bits in signal i.
		log("\t* %s (%i bits)\n", index2signal(w, i), w->widths[i]);
	}

	// ************************** READ SIGNAL *********************************
	int cpol = 0;
	int cpha = 0;

	// get time of next edge in ss signal
	float next_ss_edge_time = next_edge(w, SS, 0, false, true);

	// while there is next transmission
	while (next_ss_edge_time != INFINITY) {
		// first, get cpol and cpha
		cpol = signal_at(w, CPOL, next_ss_edge_time);
		cpha = signal_at(w, CPHA, next_ss_edge_time);

		// Read Exchange

		bool read_bytes_on_posedge = check_if_posedge(cpol, cpha);
		
		// First Exchange - read 8 bytes
		int mosi_byte = 0;
		int miso_byte = 0;

		float next_read_time = next_ss_edge_time;	// start at beginning of transaction

		// Read first 8 bits (1st exchange) of each signal by getting next edge 8 times
		for (int i = 0; i < 8; i++) {
			// find next edge (pos or neg based on CPHA/CPOL) after the found ss_edge
			next_read_time = next_edge(w, SCLK, next_read_time, read_bytes_on_posedge, !read_bytes_on_posedge);
			
			// shift each byte to the left and read next bit
			miso_byte = (miso_byte << 1) | signal_at(w, MISO, next_read_time);
			mosi_byte = (mosi_byte << 1) | signal_at(w, MOSI, next_read_time);
		}

		// Interpret MOSI signal
		int addr = mosi_byte >> 2;					// bits 7:2 are address to read/write
		int is_write = (mosi_byte >> 1) & 1; 		// bit 1 is read (0) or write (1)
		int is_stream = mosi_byte & 1;				// bit 0 is stream (1) or not (0)

		// Now, read next 8 bytes or read multiple 8 byte chucks for stream
		if (is_stream) {
			// next 8 bytes is length of stream - MOSI
			// then read string of bytes from MOSI (write) or MISO (read)

			// Read next exchange to get N (number of exchanges in this transaction) from MOSI
			mosi_byte = 0;
			for (int i = 0; i < 8; i++) {
				// find next edge (pos or neg based on CPHA/CPOL) after the last edge
				next_read_time = next_edge(w, SCLK, next_read_time, read_bytes_on_posedge, !read_bytes_on_posedge);
				
				// shift byte to the left and read next bit
				mosi_byte = (mosi_byte << 1) | signal_at(w, MOSI, next_read_time);
			}

			int n = mosi_byte;

			// Read N exchanges to read entire stream
			// if write, read from MOSI. if read, read from MISO
			char* value_signal = MISO;
			char* read_or_write = "RD";
			if (is_write) {
				read_or_write = "WR";
				value_signal = MOSI;
			}

			int values[n];
			for (int j = 0; j < n; j++) {
				values[j] = 0;
				for (int i = 0; i < 8; i++) {
					// find next edge (pos or neg based on CPHA/CPOL) after the last edge
					next_read_time = next_edge(w, SCLK, next_read_time, read_bytes_on_posedge, !read_bytes_on_posedge);
					
					// shift each byte to the left and read next bit
					values[j] = (values[j] << 1) | signal_at(w, value_signal, next_read_time);
				}
			}

			printf("%s STREAM %02x", read_or_write, addr);
			if (n <= 32) {	// tmp
				for (int i = 0; i < n; i++) {
					printf(" %02x", values[i]);
				}
			}
			printf("\n");

		}
		else {
			// Read 8 bytes (1 exchange) and stop

			// if write, read from MOSI. if read, read from MISO
			char* value_signal = MISO;
			char* read_or_write = "RD";
			if (is_write) {
				read_or_write = "WR";
				value_signal = MOSI;
			}

			// Read 8 bytes from MOSI or MISO
			int value = 0;
			for (int i = 0; i < 8; i++) {
				// find next edge (pos or neg based on CPHA/CPOL) after the last read time
				next_read_time = next_edge(w, SCLK, next_read_time, read_bytes_on_posedge, !read_bytes_on_posedge);
				
				// shift byte to the left and read next bit
				value = (value << 1) | signal_at(w, value_signal, next_read_time);
			}

			// Print this transaction
			printf("%s %02x %02x\n", read_or_write, addr, value);
		}

		// check for next ss falling edge after last read time
		next_ss_edge_time = next_edge(w, SS, next_read_time, false, true);
	}

	free_waves(w);
	return 0;
}

bool check_if_posedge(int cpol, int cpha) {
	/*
	 * checks cpol and cpha
	 * if cpol is 0 and cpha is 0, first clock edge, a posedge, is checked
	 * if cpol is 0 and cpha is 1, second clock edge, a negedge, is checked
	 * if cpol is 1 and cpha is 0, first clock edge, a negedge, is checked
	 * if cpol is 1 and cpha is 1, second clock edge, a posedge, is checked
	 * 
	 * summary: if cpol == cpha, check on clk posedge
	 *			if cpol != cpha, check on clk negedge
	 * 
	 * Returns: true if we should check posedge,
	 * 			false if we should check negedge
	
	*/

	return cpol == cpha;
}