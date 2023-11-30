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
	
	// Fixed Point params
	localparam FP_INT_LEN = 12;
	localparam FP_DEC_LEN = 20;
	localparam FP_LEN = FP_INT_LEN + FP_DEC_LEN;
	
	// Motor speed params
//	localparam SPEED_LEN = 11;	
	localparam ENC_CNT_LEN = 32;
	
	// PWM params
	localparam CTR_LEN = 11;  // unsigned range for speed is 0-2047
	localparam MAX_DUTY = 2**CTR_LEN - 1;
	//	localparam CTR_INCR = 2**13;
	
	// Params for calculations (in Q12.20 fixed-point representation)
	localparam FP_CLK_PERIOD_US = 20972; 	// 50 MHz freq = 0.02 us/cycle period
	localparam FP_GEAR_RATIO = 20972; 	// 1:50 ratio, or 0.02
	localparam FP_PPR = 149797;			// 7 pluses per revolution in encoder, or 1/7=1.428...
	localparam FP_MIN_PER_S = 17476;		// 1/60 minutes per s
	localparam FP_TEN_E_NEGSIX = 1; 		// 1/10e6 
	
	
	// ========================== HANDLE PWM DUTY CYCLE ========================
	// Increment a counter to set duty cycle high or low
	
	reg [CTR_LEN-1:0] counter;
	reg [CTR_LEN-1:0] pwm_compare;
	reg pwm_on;
	
	assign pwm_out = {m1, m2};
	
	// Control duty cycle
	always @(*) begin
		// PWM
		if (counter < pwm_compare)
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
	
	reg clockwise_out;
//	reg [SPEED_LEN-1:0] motor_speed_out;
	
	always @(posedge clk_clk) begin
		if (rst_reset) begin
			clockwise_out <= 1; 		// clockwise
			pwm_compare <= 0; 		// stop motor
		end
		else begin
			// Get new speed value from software if available
			if (avalon_slave_write) begin
				// If speed is positive (MSB is 0), set direction to clockwise
				clockwise_out <= avalon_slave_writedata[CTR_LEN] ? 1'b0 : 1'b1;
				
				// Save absolute value of speed
				pwm_compare <= avalon_slave_writedata[CTR_LEN] ? ~avalon_slave_writedata + 1 : avalon_slave_writedata;
			end
			else begin
				pwm_compare <= pwm_compare;
			end
		end
	end
	
	// ======================= CALCULATE DUTY CYCLE ============================
	// Using magnitude of speed read from software, calculate the required duty
	// cycle to achieve that speed
	
	// For now, use the magnitude of the given value as the duty cycle

	
	
	// =========================== WRITE TO MOTOR ==============================
	// Write m1 or m2 to 0 or 1, depending on direction and pwm_out
	
	// Motor output
	reg m1; // GPIO 1 -> clockwise (m1 high, m2 low)
	reg m2; // GPIO 5 -> counterclockwise (m1 low, m2 high)
	
	always @(posedge clk_clk) begin
		if (rst_reset) begin
			m1 <= 0;
			m2 <= 0;
		end
		else begin
			if (pwm_on) begin
				// Duty cycle high - write 1 to m1 or m2, write 0 to the other
				m1 <= clockwise_out ? 1 : 0; // Write 1 to m1 to go clockwise
				m2 <= clockwise_out ? 0 : 1; // Write 1 to m2 to go cc
			end
			else begin
				// Duty cycle low - write to both 0
				m1 <= 0;
				m2 <= 0;
			end
		end
	end
	
	
	// ======================= CALCULATE ACTUAL MOTOR SPEED ====================
	// write actual motor speed to software
//	reg [FP_LEN-1:0] motor_speed_in;
//	assign avalon_slave_readdata = motor_speed_in;
//	assign avalon_slave_readdata = motor_position_in;
	assign avalon_slave_readdata = motor_position_in;

	reg [32-1:0] motor_position_in;
	
//	always @(*) motor_speed_in = motor_position_in;
	
	wire c1 = encoded_in[1];
	wire c2 = encoded_in[0];
	
	always @(posedge c1) begin
		if (c2) begin
			// If c2 is high on rising edge of c1, motor is going cc. increment position
			motor_position_in <= motor_position_in + 1;
		end
		else begin
			// if c2 is low on rising edge of c1, motor is going clockwise. decrement position
			motor_position_in <= motor_position_in - 1;
		end
	end

	
//	reg [FP_INT_LEN-1:0] encoder_period_counter;			// # of cycles should never be an int wider than FP_INT_LEN
//	reg encoder_period_counter_rst;
//	reg [FP_LEN*2-1:0] period_cyc;
	
	// Get period in seconds in fixed-point from period in cycless
//   wire [FP_LEN-1:0] fp_period = ((period_cyc << FP_DEC_LEN) * FP_CLK_PERIOD_US) >> FP_DEC_LEN;
	
	// Get time per revolution in minutes (inverse of RPM)
	// minutes per revolution = encoder period (us) * gear ratio * pulses per rotation * 1/60 (min/s) * 10e-6 (s/us)
//	wire mpr = fp_period * FP_GEAR_RATIO * FP_PPR * FP_MIN_PER_S * FP_TEN_E_NEGSIX;

//	reg clockwise_in;
	
//	// On rising edge of c1, re-calculate the period and find direction
//	always @(posedge c1) begin
//		
//		encoder_period_counter <= encoder_period_counter + 1;
//		// Calculate period
////		period_cyc <= encoder_period_counter;	// Use current counter value to count cycles
////		encoder_period_counter_rst <= 0;			// reset counter
////		
//////		period_cyc <= encoder_period_counter - last_encoder_period_counter;
//////		last_encoder_period_counter <= encoder_period_counter;
//		
//		// Find direction
//		if (c2) begin
//			// If c2 is high on rising edge of c1, motor is going cc
//			clockwise_in <= 0;
//		end
//		else begin
//			// if c2 is low on rising edge of c1, motor is going clockwise
//			clockwise_in <= 1;
//		end
//	end

//	// Every clock cycle, increment or reset counter
//	always @(posedge clk_clk) begin
////		if (encoder_period_counter_rst) begin
////			// Reset counter
////			encoder_period_counter <= 0;
////		end
////		else begin
////			// Increment counter
////			encoder_period_counter <= encoder_period_counter + 1;
////		end
//	end
	
	
	
endmodule
