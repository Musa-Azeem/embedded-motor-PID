	nios_system u0 (
		.clk_clk                           (<connected-to-clk_clk>),                           //                         clk.clk
		.leds_new_signal                   (<connected-to-leds_new_signal>),                   //                        leds.new_signal
		.new_sdram_controller_0_wire_addr  (<connected-to-new_sdram_controller_0_wire_addr>),  // new_sdram_controller_0_wire.addr
		.new_sdram_controller_0_wire_ba    (<connected-to-new_sdram_controller_0_wire_ba>),    //                            .ba
		.new_sdram_controller_0_wire_cas_n (<connected-to-new_sdram_controller_0_wire_cas_n>), //                            .cas_n
		.new_sdram_controller_0_wire_cke   (<connected-to-new_sdram_controller_0_wire_cke>),   //                            .cke
		.new_sdram_controller_0_wire_cs_n  (<connected-to-new_sdram_controller_0_wire_cs_n>),  //                            .cs_n
		.new_sdram_controller_0_wire_dq    (<connected-to-new_sdram_controller_0_wire_dq>),    //                            .dq
		.new_sdram_controller_0_wire_dqm   (<connected-to-new_sdram_controller_0_wire_dqm>),   //                            .dqm
		.new_sdram_controller_0_wire_ras_n (<connected-to-new_sdram_controller_0_wire_ras_n>), //                            .ras_n
		.new_sdram_controller_0_wire_we_n  (<connected-to-new_sdram_controller_0_wire_we_n>),  //                            .we_n
		.reset_reset_n                     (<connected-to-reset_reset_n>),                     //                       reset.reset_n
		.sdram_clk_clk                     (<connected-to-sdram_clk_clk>),                     //                   sdram_clk.clk
		.key_export                        (<connected-to-key_export>)                         //                         key.export
	);

