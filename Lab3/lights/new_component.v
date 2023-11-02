// new_component.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module new_component (
		input  wire        clk_clk,                //          clk.clk
		input  wire        rst_reset,              //          rst.reset
		input  wire        avalon_slave_address,   // avalon_slave.address
		input  wire [31:0] avalon_slave_writedata, //             .writedata
		input  wire        avalon_slave_write,     //             .write,
		output reg [25:0] leds_new_signal         //         leds.new_signal
	);

	localparam CTR_LEN = 8; 
	// TODO: Auto-generated HDL template
	
//	always @(posedge clk_clk) begin
//		if (avalon_slave_write) begin
//			leds_new_signal <= avalon_slave_writedata[25:0];
//		end
//	end

	//	assign leds_new_signal = 26'b00000000000000000000000000;
	reg [CTR_LEN-1:0] counter;
	reg [CTR_LEN-1:0] counter_next;
	reg [CTR_LEN-1:0] compare [25:0];
	reg [CTR_LEN-1:0] compare_next [25:0];
	//reg [25:0] leds_on;
	
	//always @(posedge clk_clk) 
	

	reg [25:0] pwm_bits;
	
	integer k;
	always @(posedge clk_clk) begin
		counter_next = counter + 1;
		for (k=0; k<26; k=k+1) begin
				compare_next[k] = compare[k] - 1;
		end
	end
	
//	genvar i;
//	generate
//		for (i = 0; i < 26; i=i+1) begin: pwm_gen_loop
//			always @(*) begin
//				if (counter < compare[i])
//					pwm_bits[i] = 1'b1;
//				else
//					pwm_bits[i] = 1'b0;
//			end
//		end
//	endgenerate

	always @(*) begin
		if (counter < compare[0])
			pwm_bits[0] = 1'b1;
		else
			pwm_bits[0] = 1'b0;
	end

	always @(posedge clk_clk or posedge rst_reset) begin
		if (rst_reset == 1) begin
			leds_new_signal <= 26'd0;
			counter <= 1'b0;
		end
		else begin
			
//			if (avalon_slave_write && avalon_slave_writedata[1]) begin
//				compare[0] <= 8'd10;
//			end
//			else if (avalon_slave_write && avalon_slave_writedata[0])  begin
//				compare[0] <= 8'd255;
//			end
	
			counter <= counter_next;
			leds_new_signal <= pwm_bits;
			

			if (avalon_slave_write && avalon_slave_writedata[0])
				compare[0] <= 8'd255; // 2^8 - 1
			else if (compare[0] > 1'b0)
				compare[0] <= compare_next[0];
		end
	end
	
//	genvar j;
//	generate
//		for (j = 0; j < 26; j=j+1) begin: compare_loop
//			always @(posedge clk_clk) begin
//				if (rst_reset == 1) begin
//					compare[j] <= 20'b0; 
//				end
//				if (avalon_slave_write && avalon_slave_writedata[j])
//					compare[j] <= 20'd1048576; // 2^20 // 2^8
//				else if (compare[j])
//					compare[j] <= compare_next[j];
//			end
//		end
//	endgenerate

endmodule
