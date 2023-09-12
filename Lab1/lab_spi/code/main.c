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
struct DataTuple {
	int byte;
	float last_read_time;
};

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

	// ========================= READ SIGNAL ==================================
	// Constants
	int LEN_EXCH = 8;

	// Hold values of cpol and cpha
	int cpol = 0;
	int cpha = 0;

	// Get time of next edge in ss signal - this is the start of a transaction
	float next_ss_edge_time = next_edge(w, SS, 0, false, true);

	// Keep going until no more transactions in signal
	while (next_ss_edge_time != INFINITY) {

		// First, read cpol and cpha
		cpol = signal_at(w, CPOL, next_ss_edge_time);
		cpha = signal_at(w, CPHA, next_ss_edge_time);

		// Check if we should read data on rising or falling edge
		bool read_bytes_on_posedge = check_if_posedge(cpol, cpha);
		
		// ===================== READ FIRST EXCHANGE ==========================
		// First exchange - only read mosi
		// int mosi_byte = 0;

		// Hold the time of the most recent falling or rising edge in clock
		// Start with the previously found beginning of transaction time
		// float next_read_time = next_ss_edge_time;

		// Read first 8 bits of mosi by reading at next eight edges
		struct DataTuple data = read_exchange(
			w,
			MOSI,
			next_ss_edge_time,
			read_bytes_on_posedge
		);
		int mosi_byte = data.byte;
		float last_read_time = data.last_read_time;
		// for (int i = 0; i < LEN_EXCH; i++) {
		// 	// Find next clock edge (pos or neg based on CPHA/CPOL)
		// 	next_read_time = next_edge(
		// 		w, 
		// 		SCLK, 
		// 		next_read_time, 
		// 		read_bytes_on_posedge, 
		// 		!read_bytes_on_posedge
		// 	);

		// 	// Shift byte to the left and read next bit
		// 	mosi_byte = (mosi_byte << 1) | signal_at(w, MOSI, next_read_time);
		// }

		// Split MOSI signal
		int addr = mosi_byte >> 2;			 // bits 7:2 is address to read/write
		int is_write = (mosi_byte >> 1) & 1; // bit 1 is read (0) or write (1)
		int is_stream = mosi_byte & 1;		 // bit 0 is stream (1) or not (0)

		// If transaction is a write, read the rest of the transaction from 
		// MOSI. If it is a read, read from MISO.
		char* value_signal = MISO;	
		char* read_or_write = "RD";
		if (is_write) {
			read_or_write = "WR";
			value_signal = MOSI;
		}

		// ====================== READ DATA EXCHANGE ==========================
		// Now, read the data being sent either in single exchange or multiple

		// Read stream transaction
		if (is_stream) {
			// First, read the next 8 bits in MOSI to get the N, number of
			// exchanges in the stream. Then, read n exchanges from MISO 
			// (for reads) or MOSI (for writes)

			// Read next exchange to get N from MOSI
			// mosi_byte = 0;
			// for (int i = 0; i < LEN_EXCH; i++) {
			// 	// Find next clock edge (pos or neg based on CPHA/CPOL)
			// 	next_read_time = next_edge(
			// 		w, 
			// 		SCLK, 
			// 		next_read_time, 
			// 		read_bytes_on_posedge, 
			// 		!read_bytes_on_posedge
			// 	);
				
			// 	// Shift byte to the left and read next bit
			// 	mosi_byte = (mosi_byte << 1) | signal_at(
			// 		w, 
			// 		MOSI, 
			// 		next_read_time
			// 	);
			// }
			data = read_exchange(
				w,
				MOSI,
				last_read_time,
				read_bytes_on_posedge
			);

			// Save N
			int n = data.byte;
			last_read_time = data.last_read_time;

			// Hold N
			// int n = mosi_byte;

			// Hold value of each exchange. There will be N values
			int values[n];

			// Read N exchanges to read entire transaction
			for (int j = 0; j < n; j++) {
				// values[j] = 0;

				// Read an exchange from MISO or MOSI
				// for (int i = 0; i < LEN_EXCH; i++) {
				// 	// Find next clock edge (pos or neg based on CPHA/CPOL)
				// 	next_read_time = next_edge(
				// 		w, 
				// 		SCLK, 
				// 		next_read_time, 
				// 		read_bytes_on_posedge, 
				// 		!read_bytes_on_posedge
				// 	);
					
				// 	// Shift byte to the left and read next bit
				// 	values[j] = (values[j] << 1) | signal_at(
				// 		w, 
				// 		value_signal, 
				// 		next_read_time
				// 	);
				// }
				data = read_exchange(
					w,
					value_signal,
					last_read_time,
					read_bytes_on_posedge
				);
				values[j] = data.byte;
				last_read_time = data.last_read_time;
			}

			// Print out transaction
			printf("%s STREAM %02x", read_or_write, addr);
			if (n <= 32) {	// tmp
				for (int i = 0; i < n; i++) {
					printf(" %02x", values[i]);
				}
			}
			printf("\n");

		}

		// Read Single Exchange Transaction
		else {
			// Only read the next exchange (8 bits) to get value of transaction 

			// Read eight bytes from MOSI or MISO
			// int value = 0;
			// for (int i = 0; i < LEN_EXCH; i++) {
			// 	// Find next clock edge (pos or neg based on CPHA/CPOL)
			// 	next_read_time = next_edge(
			// 		w, 
			// 		SCLK, 
			// 		next_read_time, 
			// 		read_bytes_on_posedge, 
			// 		!read_bytes_on_posedge
			// 	);
				
			// 	// Shift byte to the left and read next bit
			// 	value = (value << 1) | signal_at(
			// 		w, 
			// 		value_signal, 
			// 		next_read_time
			// 	);
			// }
			data = read_exchange(
				w,
				value_signal,
				last_read_time,
				read_bytes_on_posedge
			);
			int value = data.byte;
			last_read_time = data.last_read_time;

			// Print this transaction
			printf("%s %02x %02x\n", read_or_write, addr, value);
		}

		// Check for next ss falling edge after last read time to find
		// next transaction
		next_ss_edge_time = next_edge(w, SS, last_read_time, false, true);
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

struct DataTuple read_exchange(
	waves* w, 
	char* signal, 
	float last_read_time, 
	bool read_bytes_on_posedge
) {
	/*
	 * Read an exchange (8 bits) for the given signal from the given wave object
	 * - Starts lookings for clock edges from the given "last_read_time"
	 * - Looks for rising edges if "read_bytes_on_posedge" is true. Otherwise,
	 *   looks for falling edges
	 */
	
	// Length of exchange is always 8 bits
	int LEN_EXCH = 8;
	
	// Time to read next data point and look for next edge from. 
	// Start with provided time
	float next_read_time = last_read_time;

	// Data of exchange - read from given "signal"
	int value;

	for (int i = 0; i < LEN_EXCH; i++) {
		// Find next clock edge (pos or neg based on "read_bytes_on_posedge")
		next_read_time = next_edge(
			w, 
			SCLK, 
			next_read_time, 
			read_bytes_on_posedge,
			!read_bytes_on_posedge
		);
		
		// Shift byte to the left and read next bit
		value = (value << 1) | signal_at(
			w, 
			signal, 
			next_read_time
		);
	}

	// Return value that was read and the last read time
	struct DataTuple ret = { value, next_read_time };
	return ret;
}