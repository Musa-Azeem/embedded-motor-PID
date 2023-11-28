// motor.v

`timescale 1 ps / 1 ps
module motor (
		input  wire        clk_clk,                //          clk.clk
		input  wire        rst_reset,              //          rst.reset
		input  wire        avalon_slave_address,   // avalon_slave.address
		input  wire [31:0] avalon_slave_writedata, //             .writedata
		output wire [31:0] avalon_slave_readdata,  //             .readdata
		input  wire        avalon_slave_write,     //             .write
		input  wire        avalon_slave_read,      //             .read
		output wire [1:0]  pwm_out,                //  conduit_end.write_speed
		input  wire [1:0]  encoded_in              //             .read_speed
	);
	
	// Motor speed params
	localparam SPEED_LEN = 8;
	
	// PWM params
	localparam CTR_LEN = 11; 
	localparam MAX_DUTY = 2**CTR_LEN - 1;
	//	localparam CTR_INCR = 2**13;

	
	
	// ========================== HANDLE PWM DUTY CYCLE ========================
	// Increment a counter to set duty cycle high or low
	
	reg [CTR_LEN-1:0] counter;
	reg [CTR_LEN-1:0] compare;
	reg pwm_on;
	
	always @(*) compare = MAX_DUTY - 512; // temporary
	
	assign pwm_out = {c1, c2};
	
	// Control duty cycle
	always @(*) begin
		// PWM
		if (counter < compare)
			pwm_on = 1;
		else
			pwm_on = 0;		
	end
	
	// Increment counter on clk
	always @(posedge clk_clk) begin
		if (rst_reset) begin
			counter <= 0;
		end
		else begin
			counter <= counter + 1;
		end
	end
	
	// ========================= MOTOR SPEED OUT ===============================
	// Read speed in RPMs from software - save the sign for direction and get 
	// absolute value for magnitude
	
	reg clockwise;
	reg [SPEED_LEN-1:0] motor_speed_out;
	
	always @(posedge clk_clk) begin
		if (rst_reset) begin
			clockwise <= 1; // clockwise
			motor_speed_out <= 0; // stop motor
		end
		else begin
			// Get new speed value from software if available
			if (avalon_slave_write) begin
				// If speed is positive (MSB is 0), set direction to clockwise
				clockwise <= avalon_slave_writedata[SPEED_LEN-1] ? 1'b0 : 1'b1;
				
				// Save absolute value of speed
				motor_speed_out <= motor_speed_out[SPEED_LEN-1] ? ~motor_speed_out + 1 : motor_speed_out;
			end
		end
	end
	
	// ======================= CALCULATE DUTY CYCLE ============================
	// Using magnitude of speed read from software, calculate the required duty
	// cycle to achieve that speed

	
	
	// =========================== WRITE TO MOTOR ==============================
	// Write c1 or c2 to 0 or 1, depending on direction and pwm_out
	
	// Motor output
	reg c1; // GPIO 1 -> clockwise (m1 high, m2 low)
	reg c2; // GPIO 5 -> counterclockwise (m1 low, m2 high)
	
	always @(posedge clk_clk) begin
		if (rst_reset) begin
			c1 <= 0;
			c2 <= 0;
		end
		else begin
			if (pwm_on) begin
				// Duty cycle high - write 1 to c1 or c2, write 0 to the other
				c1 <= clockwise ? 1 : 0; // Write 1 to c1 to go clockwise
				c2 <= clockwise ? 0 : 1; // Write 1 to c2 to go cc
			end
			else begin
				// Duty cycle low - write to both 0
				c1 <= 0;
				c2 <= 0;
			end
		end
	end
	
	
	// ======================= CALCULATE ACTUAL MOTOR SPEED ====================
	// write actual motor speed to software
	wire motor_speed_in;
	assign avalon_slave_readdata = motor_speed_in;

	
	
//	always @(posedge clk_clk) begin
//		if (rst_reset) begin
//			c1 <= 1'b0;
//			c2 <= 1'b0;
//		end
//		else begin
//			if (pwm_on) begin
//				// always clockwise for now
//				c1 <= 1;
//				c2 <= 0;
//			end
//			else begin
//				c1 <= 0;
//				c2 <= 0;
//			end
//			if (avalon_slave_write) begin
//				
//			end
//			else begin
//			end
//		end
//	end

endmodule
