	component nios_system is
		port (
			clk_clk                           : in    std_logic                     := 'X';             -- clk
			key_export                        : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			new_sdram_controller_0_wire_addr  : out   std_logic_vector(12 downto 0);                    -- addr
			new_sdram_controller_0_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
			new_sdram_controller_0_wire_cas_n : out   std_logic;                                        -- cas_n
			new_sdram_controller_0_wire_cke   : out   std_logic;                                        -- cke
			new_sdram_controller_0_wire_cs_n  : out   std_logic;                                        -- cs_n
			new_sdram_controller_0_wire_dq    : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			new_sdram_controller_0_wire_dqm   : out   std_logic_vector(3 downto 0);                     -- dqm
			new_sdram_controller_0_wire_ras_n : out   std_logic;                                        -- ras_n
			new_sdram_controller_0_wire_we_n  : out   std_logic;                                        -- we_n
			reset_reset_n                     : in    std_logic                     := 'X';             -- reset_n
			sdram_clk_clk                     : out   std_logic;                                        -- clk
			motor_pwm_out                     : out   std_logic_vector(1 downto 0);                     -- pwm_out
			motor_encoded_in                  : in    std_logic_vector(1 downto 0)  := (others => 'X')  -- encoded_in
		);
	end component nios_system;

	u0 : component nios_system
		port map (
			clk_clk                           => CONNECTED_TO_clk_clk,                           --                         clk.clk
			key_export                        => CONNECTED_TO_key_export,                        --                         key.export
			new_sdram_controller_0_wire_addr  => CONNECTED_TO_new_sdram_controller_0_wire_addr,  -- new_sdram_controller_0_wire.addr
			new_sdram_controller_0_wire_ba    => CONNECTED_TO_new_sdram_controller_0_wire_ba,    --                            .ba
			new_sdram_controller_0_wire_cas_n => CONNECTED_TO_new_sdram_controller_0_wire_cas_n, --                            .cas_n
			new_sdram_controller_0_wire_cke   => CONNECTED_TO_new_sdram_controller_0_wire_cke,   --                            .cke
			new_sdram_controller_0_wire_cs_n  => CONNECTED_TO_new_sdram_controller_0_wire_cs_n,  --                            .cs_n
			new_sdram_controller_0_wire_dq    => CONNECTED_TO_new_sdram_controller_0_wire_dq,    --                            .dq
			new_sdram_controller_0_wire_dqm   => CONNECTED_TO_new_sdram_controller_0_wire_dqm,   --                            .dqm
			new_sdram_controller_0_wire_ras_n => CONNECTED_TO_new_sdram_controller_0_wire_ras_n, --                            .ras_n
			new_sdram_controller_0_wire_we_n  => CONNECTED_TO_new_sdram_controller_0_wire_we_n,  --                            .we_n
			reset_reset_n                     => CONNECTED_TO_reset_reset_n,                     --                       reset.reset_n
			sdram_clk_clk                     => CONNECTED_TO_sdram_clk_clk,                     --                   sdram_clk.clk
			motor_pwm_out                     => CONNECTED_TO_motor_pwm_out,                     --                       motor.pwm_out
			motor_encoded_in                  => CONNECTED_TO_motor_encoded_in                   --                            .encoded_in
		);

