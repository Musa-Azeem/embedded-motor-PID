// motor.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module motor (
		input  wire        clk_clk,                //          clk.clk
		input  wire        rst_reset,              //          rst.reset
		input  wire        avalon_slave_address,   // avalon_slave.address
		input  wire [31:0] avalon_slave_writedata, //             .writedata
		output wire [31:0] avalon_slave_readdata,  //             .readdata
		input  wire        avalon_slave_write,     //             .write
		input  wire        avalon_slave_read,      //             .read
		output wire [1:0]   pwm_out,                //  conduit_end.write_speed
		input  wire [1:0]  encoded_in              //             .read_speed
	);
	
	localparam SPEED_LEN = 8; // speed in rpms
	
	// PWM stuff
	localparam CTR_LEN = 11; 
	localparam MAX_DUTY = 2**CTR_LEN - 1;
//	localparam CTR_INCR = 2**13;
	reg [CTR_LEN-1:0] counter;
	reg [CTR_LEN-1:0] compare;
	reg pwm_on;
	
	always @(*) compare = MAX_DUTY - 512; // temporary

	// TODO: Auto-generated HDL template

	// write motor speed to output
	wire motor_speed;
	assign avalon_slave_readdata = motor_speed;
	
	// Read speed from software - save sign and get absolute value
//	assign speed = avalon_slave_writedata[SPEED_LEN-1:0];
//	assign speed_sign = speed[SPEED_LEN];
//	assign abs_speed = speed[SPEED_LEN] ? ~speed + 1 : speed;
	
	reg c1; // GPIO 1 -> clockwise (m1 high, m2 low)
	reg c2; // GPIO 5 -> counterclockwise (m1 low, m2 high)
	
	assign pwm_out = {c1, c2};
	
	always @(*) begin
		if (counter < compare)
			pwm_on = 1;
		else
			pwm_on = 0;
	end
	
	always @(posedge clk_clk) begin
		if (rst_reset) begin
			c1 <= 1'b0;
			c2 <= 1'b0;
//			compare <= 0;
			counter <= 0;
		end
		else begin
			counter <= counter + 1;
			if (pwm_on) begin
				// always clockwise for now
				c1 <= 1;
				c2 <= 0;
			end
			else begin
				c1 <= 0;
				c2 <= 0;
			end
			if (avalon_slave_write) begin
				
			end
			else begin
			end
		end
	end
	
//	always @(*) begin
//		pwm_out[0] = c1;
//		pwm_out[1] = c2;
//	end
	
	//always @(posedge clk_clk or posedge rst_reset) begin
		//if (rst_reset == 1) begin
			//c1 <= 1'b0;
			//c2 <= 1'b0;
		//end
		//else begin
			//if (avalon_slave_write && avalon_slave_writedata[0])
			//	c1 <= 1'b1;
			//else
			//	c1 <= 1'b0;
			//c2 <= 1'b0;
		//end
	//end

endmodule
