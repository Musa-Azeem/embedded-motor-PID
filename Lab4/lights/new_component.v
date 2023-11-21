// new_component.v

`timescale 1 ps / 1 ps
module new_component (
		input  wire        clk_clk,                //          clk.clk
		input  wire        rst_reset,              //          rst.reset
		input  wire        avalon_slave_address,   // avalon_slave.address
		input  wire [31:0] avalon_slave_writedata, //             .writedata
		input  wire        avalon_slave_write,     //             .write,
		output reg [25:0] leds_new_signal         //         leds.new_signal
	);

	localparam CTR_LEN = 26; 
	localparam MAX_DUTY = 2**26 - 1;
	localparam CTR_INCR = 2**13;

	reg [CTR_LEN-1:0] counter;
	reg [CTR_LEN-1:0] counter_next;
	reg [CTR_LEN-1:0] compare [25:0];
	reg [CTR_LEN-1:0] compare_next [25:0];
	reg [25:0] pwm_bits;
	
	genvar i;
	generate
		for (i = 0; i < 26; i=i+1) begin: pwm_gen_loop
			always @(*) begin
				if (counter < compare[i])
					pwm_bits[i] = 1'b1;
				else
					pwm_bits[i] = 1'b0;
			end
			
			always @(posedge clk_clk) begin
				if (rst_reset)
					compare[i] <= 26'b0;
				else if (avalon_slave_write && avalon_slave_writedata[i])
					compare[i] <= MAX_DUTY; 		// 2^26 - 1 ie. 100% duty cycle
				else
					compare[i] <= compare_next[i]; // decrement by one or keep same value
			end
		end
	endgenerate

//	genvar j;
	integer k;
	always @(posedge clk_clk or posedge rst_reset) begin
		if (rst_reset == 1) begin
			leds_new_signal <= 26'd0;
			counter <= 1'b0;
		end
		else begin
		
			// Increment counter_next by CTR_INCR to grow faster (higher freq)
			counter_next <= counter + CTR_INCR;
			
			// Decrement compare_next if compare[k] is not already 0
			for (k=0; k<26; k=k+1) begin
				if (compare[k])
					compare_next[k] <= compare[k] - 1;
				else
					compare_next[k] <= compare[k];
			end
			
			// Update counter and leds output
			counter <= counter_next;
			leds_new_signal <= pwm_bits;
		end
	end
endmodule
