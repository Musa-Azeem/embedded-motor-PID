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
		input  wire        avalon_slave_write,     //             .write
		output reg [25:0] leds_new_signal         //         leds.new_signal
	);

	localparam CTR_LEN = 8; 
	localparam CPR = 7'd255;
	// TODO: Auto-generated HDL template

	//	assign leds_new_signal = 26'b00000000000000000000000000;
	reg [CTR_LEN-1:0] counter;
	reg [CTR_LEN-1:0] counter_next;
	wire [CTR_LEN-1:0] compare;
	assign compare = CPR;

	reg [25:0] pwm_bits;
	reg pwm;

	always @(*) begin
		pwm_bits[0] = pwm;
		
		counter_next = counter + 1;
		
		if (counter < compare)
			pwm = 1'b1;
		else
			pwm = 1'b0;
	end
	
	always @(posedge clk_clk or posedge rst_reset) begin
		if (rst_reset == 1) begin
			leds_new_signal <= 26'd0;
			counter <= 1'b0;
		end
		else begin
//			leds_new_signal <= 26'h3FFFFFF;

			counter <= counter_next;
			
			leds_new_signal <= pwm_bits;
		end
	end

endmodule
