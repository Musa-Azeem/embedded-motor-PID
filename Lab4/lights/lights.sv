module lights (input CLOCK_50,
	input [3:0] KEY,
	output [12:0] DRAM_ADDR,
	output [1:0] DRAM_BA,
	output DRAM_CAS_N,
	output DRAM_CKE,
	output DRAM_CS_N,
	inout [31:0] DRAM_DQ,
	output [3:0] DRAM_DQM,
	output DRAM_RAS_N,
	output DRAM_WE_N,
	output DRAM_CLK,
//	output [17:0] LEDR,
//	output [7:0] LEDG,
//	input [2:0] encoder_in,
//	output [2:0] pwm_out,
	inout [35:0] GPIO
	);

	nios_system u0 (
		.clk_clk                           (CLOCK_50),   //                         clk.clk
		.reset_reset_n                     (KEY[0]),     //                       reset.reset_n
		.new_sdram_controller_0_wire_addr  (DRAM_ADDR),  // new_sdram_controller_0_wire.addr
		.new_sdram_controller_0_wire_ba    (DRAM_BA),    //                            .ba
		.new_sdram_controller_0_wire_cas_n (DRAM_CAS_N), //                            .cas_n
		.new_sdram_controller_0_wire_cke   (DRAM_CKE),   //                            .cke
		.new_sdram_controller_0_wire_cs_n  (DRAM_CS_N),  //                            .cs_n
		.new_sdram_controller_0_wire_dq    (DRAM_DQ),    //                            .dq
		.new_sdram_controller_0_wire_dqm   (DRAM_DQM),   //                            .dqm
		.new_sdram_controller_0_wire_ras_n (DRAM_RAS_N), //                            .ras_n
		.new_sdram_controller_0_wire_we_n  (DRAM_WE_N),  //                            .we_n
		.sdram_clk_clk                     (DRAM_CLK),
		.motor_pwm_out							  ({GPIO[1],GPIO[5]}), 	// read pwm from motor ip
		.motor_encoded_in						  ({GPIO[9],GPIO[11]}), // write encoder to motor ip
		.key_export (KEY)
	);
					
endmodule