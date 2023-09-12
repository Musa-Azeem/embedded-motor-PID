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

bool check_if_posedge(int cpol, int cpha);

int main(int argc, char** argv) {
	/* This macro silences compiler errors about unused variables. */
	UNUSED(argc);
	UNUSED(argv);

	int cpol = 0;
	int cpha = 0;

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

	// get time of next edge in ss signal
	float next_ss_edge_time = next_edge(w, SS, 0, false, true);
	log("\n\n\nNext SS Edge time: %f\n", next_ss_edge_time);
	// while there is next transmission
	while (next_ss_edge_time != INFINITY) {
		// Read Exchange

		// Values to find
		char* read_or_write = "RD";
		int addr = 0;
		int value = 0;

		bool read_bytes_on_posedge = check_if_posedge(cpol, cpha);
		log("read_bytes_on_posedge: %d\n", read_bytes_on_posedge);
		
		// First Transmission - read 8 bytes
		int mosi_byte = 0;
		int miso_byte = 0;
		// int sclk_bits = 0;
		// int ss_bits = 0;

		float next_read_time = next_ss_edge_time;	// start at beginning of transaction

		// Read first 8 bits (1st exchange) of each signal by getting next edge 8 times
		log("\n\nREADING 1st 8\n");
		for (int i = 0; i < 8; i++) {
			// find next edge (pos or neg based on CPHA/CPOL) after the found ss_edge
			next_read_time = next_edge(w, SCLK, next_read_time, read_bytes_on_posedge, !read_bytes_on_posedge);
			log("next_read_time: %f\n", next_read_time);
			
			// shift each byte to the left and read next bit
			miso_byte = (miso_byte << 1) | signal_at(w, MISO, next_read_time);
			mosi_byte = (miso_byte << 1) | signal_at(w, MOSI, next_read_time);
			log("miso_byte: %d, mosi_byte: %d\n\n", miso_byte, mosi_byte);
			// ss_bits = (miso_byte << 1) | signal_at(w, SS, next_read_time);
		}

		// Interpret MOSI signal
		addr = mosi_byte >> 6;				// bits 7:2 are address to read/write
		int is_write = (mosi_byte >> 7) << 1; 	// bit 1 is read (0) or write (1)
		int is_stream = mosi_byte & 254;		// bit 0 is stream (1) or not (0)
		log("addr: %02x, is_write: %d, is_stream: %d\n\n", addr, is_write, is_stream);

		// Now, read next 8 bytes or read multiple 8 byte chucks for stream
		if (is_stream) {
			// next 8 bytes is length of stream - MOSI
			// then read string of bytes from MOSI (write) or MISO (read)
		}
		else {
			// Read 8 bytes (1 exchange) and stop

			// if write, read from MOSI. if read, read from MISO
			char* value_signal = MISO;
			if (is_write) {
				read_or_write = "WR";
				value_signal = MOSI;
			}

			log("\n\nvalue_signal: %s\n", value_signal);

			// Read 8 bytes from MOSI or MISO
			for (int i = 0; i < 8; i++) {
				// find next edge (pos or neg based on CPHA/CPOL) after the last read time
				next_read_time = next_edge(w, SCLK, next_read_time, read_bytes_on_posedge, !read_bytes_on_posedge);
				log("NEXT next_read_time: %f\n", next_read_time);
				
				// shift byte to the left and read next bit
				value = (value << 1) | signal_at(w, value_signal, next_read_time);
				log("NEXT value: %d\n", value);
			}
		}
		
		// Print this transaction
		printf("%s %02x %02x\n", read_or_write, addr, value);


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