
`timescale 1 ns / 1 ps

	module axi_ikinematics_v1_2_S00_AXI #
	(
		// Users to add parameters here
		//-- FIFO Depth --
		parameter integer C_FIFO_DEPTH = 16,
		//-- Floating Point Parameters --
		parameter integer C_FLP_WIDTH = 32,
		//-- Fixed Point Parameters --
		// Fixed Point Data bus width : 20 bit
		parameter integer C_FXP_WIDTH = 20,
		// Fixed Point dot ubication from right [INT].[DEC]					
		parameter integer C_FXP_POINT = 16,
		//-- Inverse Kinematics Parameters --
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_iK	= 'h09b74,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_iKH	= 'h3c370,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_F		= 'h0ea90,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_C2	= 'hff1eb,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_C3	= 'hffafd,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_Ca8	= 'h62cd9,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_L1	= 'h0070a,
		parameter signed [C_FXP_WIDTH-1:0]	C_IKN_nL1	= 'hff8f5,
		//-- CORDIC Parameters --
		// CORDIC positive iterations					
		parameter integer C_COR_ITER=16,
		// CORDIC non-positive iterations
		parameter integer C_COR_N_ITER=1,
		//-- PWM Generation Parameters --
		// Offset PI/4
		parameter signed [C_FXP_WIDTH-1:0]	C_PWM_OFFSET	= 'h1921f,
		parameter integer C_PWM_SIZE = 8,
		parameter integer C_PWM_FREQ = 100,
		parameter integer C_PWM_MAX_IN = 200,
		//-- Robot Parameters
		// Number of legs
		parameter integer C_ROB_NLEGS = 6,
		// Number of joints per leg
		parameter integer C_ROB_NJOINTS = 3,
		// User parameters ends
		// Do not modify the parameters beyond this line

		// Width of S_AXI data bus
		parameter integer C_S_AXI_DATA_WIDTH	= 32,
		// Width of S_AXI address bus
		parameter integer C_S_AXI_ADDR_WIDTH	= 8
	)
	(
		// Users to add ports here
		// 
		output wire [C_PWM_SIZE*C_ROB_NLEGS*C_ROB_NJOINTS-1:0] O_JOINT_PWM,
		// User ports ends
		// Do not modify the ports beyond this line

		// Global Clock Signal
		input wire  S_AXI_ACLK,
		// Global Reset Signal. This Signal is Active LOW
		input wire  S_AXI_ARESETN,
		// Write address (issued by master, acceped by Slave)
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] S_AXI_AWADDR,
		// Write channel Protection type. This signal indicates the
    		// privilege and security level of the transaction, and whether
    		// the transaction is a data access or an instruction access.
		input wire [2 : 0] S_AXI_AWPROT,
		// Write address valid. This signal indicates that the master signaling
    		// valid write address and control information.
		input wire  S_AXI_AWVALID,
		// Write address ready. This signal indicates that the slave is ready
    		// to accept an address and associated control signals.
		output wire  S_AXI_AWREADY,
		// Write data (issued by master, acceped by Slave) 
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		// Write strobes. This signal indicates which byte lanes hold
    		// valid data. There is one write strobe bit for each eight
    		// bits of the write data bus.    
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		// Write valid. This signal indicates that valid write
    		// data and strobes are available.
		input wire  S_AXI_WVALID,
		// Write ready. This signal indicates that the slave
    		// can accept the write data.
		output wire  S_AXI_WREADY,
		// Write response. This signal indicates the status
    		// of the write transaction.
		output wire [1 : 0] S_AXI_BRESP,
		// Write response valid. This signal indicates that the channel
    		// is signaling a valid write response.
		output wire  S_AXI_BVALID,
		// Response ready. This signal indicates that the master
    		// can accept a write response.
		input wire  S_AXI_BREADY,
		// Read address (issued by master, acceped by Slave)
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] S_AXI_ARADDR,
		// Protection type. This signal indicates the privilege
    		// and security level of the transaction, and whether the
    		// transaction is a data access or an instruction access.
		input wire [2 : 0] S_AXI_ARPROT,
		// Read address valid. This signal indicates that the channel
    		// is signaling valid read address and control information.
		input wire  S_AXI_ARVALID,
		// Read address ready. This signal indicates that the slave is
    		// ready to accept an address and associated control signals.
		output wire  S_AXI_ARREADY,
		// Read data (issued by slave)
		output wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_RDATA,
		// Read response. This signal indicates the status of the
    		// read transfer.
		output wire [1 : 0] S_AXI_RRESP,
		// Read valid. This signal indicates that the channel is
    		// signaling the required read data.
		output wire  S_AXI_RVALID,
		// Read ready. This signal indicates that the master can
    		// accept the read data and response information.
		input wire  S_AXI_RREADY
	);
	
	//-- User Declarations Begin --
	//----------------------------------------------
	//-- User Local Parameters
	//----------------------------------------------
	//-- FIFO Parameters --
	// FIFO Width
	localparam integer FIFO_WIDTH = C_FXP_WIDTH * 3;
	// FIFO Full Program Threshold
	localparam integer FIFO_PROG_FULL_THRESH = (C_FIFO_DEPTH-3) - 2;
	// Difference between Floating-point and Fixed-point sizes
	localparam integer C_FXP_FLP_DIFF = C_FLP_WIDTH - C_FXP_WIDTH;
	//-- PWM Parameters --
	localparam integer C_FXP_PWM_OFFSET = C_FXP_POINT + 1;
	//-- Joint Counter Size --
	localparam integer C_JOI_CTR_SIZE	= $clog2(C_ROB_NLEGS);
	localparam integer C_JOI_CTR_SIZE_COMPL = 16 - C_JOI_CTR_SIZE;
	//--
	localparam integer C_FXP_DATA_FILL = C_S_AXI_DATA_WIDTH - C_FXP_WIDTH;

	//----------------------------------------------
	//-- User Signals Definitions
	//----------------------------------------------
	//-- Leg counter signals
	wire [C_JOI_CTR_SIZE-1:0]		lgc_leg_sel;
	wire							lgc_invalid;
	reg								lgc_set;
	//-- Floating-point to Fixed-point conversion signals
	wire signed [C_FXP_WIDTH-1:0]	fxp_input_x,
									fxp_input_y,
									fxp_input_z;
	wire							fxp_out_range_x,
									fxp_out_range_y,
									fxp_out_range_z,
									fxp_input_zero_x,
									fxp_input_zero_y,
									fxp_input_zero_z;
	//-- Fixed-point to Floating-point conversion signals
	wire [C_FLP_WIDTH-1:0]			flp_output_q1,
									flp_output_q2,
									flp_output_q3;
	// Pipeline Fixed-point to Floating-point conversion signals			
	reg [C_FLP_WIDTH-1:0]			reg_flp_output_q1,
									reg_flp_output_q2,
									reg_flp_output_q3;
	reg [C_FLP_WIDTH-1:0]			mux_flp_output_q1,
									mux_flp_output_q2,
									mux_flp_output_q3;
	//-- FIFO Signals --
	wire [FIFO_WIDTH-1:0]			fifo_dout;
	reg 							fifo_wr_en;
	wire							fifo_rd_en,
									fifo_empty,
									fifo_full;
	//-- Inverse Kinematics Architecture Signals --
	wire signed [C_FXP_WIDTH-1:0]	ikn_q1,
									ikn_q2,
									ikn_q3;
	reg								ikn_start;
	wire							ikn_data_valid,
									ikn_data_ready,
									ikn_busy;
	//-- Write Output Directly Flag --
	reg 							reg_output_direct;
	//-- Output Configuration Register --
	wire [C_S_AXI_DATA_WIDTH-1:0] 	reg_config_out;
	//-- Inverse Kinematics Output Register + Offset --
	reg [C_FXP_WIDTH*C_ROB_NLEGS*C_ROB_NJOINTS-1:0]	ikn_output_rb;
	//-- PWM Generation Signals --
	// Inverse Kinematics angles offset addition
	reg signed [C_FLP_WIDTH-1:0]	ikn_q1_mux_offset,
									ikn_q2_mux_offset,
									ikn_q3_mux_offset;
	wire signed [C_FXP_WIDTH-1:0]	ikn_q1_p_offset,
									ikn_q2_p_offset,
									ikn_q3_p_offset,
									ikn_q1_offset,
									ikn_q2_offset,
									ikn_q3_offset;
	reg signed [C_FXP_WIDTH-1:0]	reg_ikn_q1_offset,
									reg_ikn_q2_offset,
									reg_ikn_q3_offset;
	// Inverse Kinematics angles trimmed to 8 bit
	wire [7:0]						ikn_q1_pwm,
									ikn_q2_pwm,
									ikn_q3_pwm;
	//-- End User Declarations --

	// AXI4LITE signals
	reg [C_S_AXI_ADDR_WIDTH-1 : 0] 	axi_awaddr;
	reg  	axi_awready;
	reg  	axi_wready;
	reg [1 : 0] 	axi_bresp;
	reg  	axi_bvalid;
	reg [C_S_AXI_ADDR_WIDTH-1 : 0] 	axi_araddr;
	reg  	axi_arready;
	reg [C_S_AXI_DATA_WIDTH-1 : 0] 	axi_rdata;
	reg [1 : 0] 	axi_rresp;
	reg  	axi_rvalid;

	// Example-specific design signals
	// local parameter for addressing 32 bit / 64 bit C_S_AXI_DATA_WIDTH
	// ADDR_LSB is used for addressing 32/64 bit registers/memories
	// ADDR_LSB = 2 for 32 bits (n downto 2)
	// ADDR_LSB = 3 for 64 bits (n downto 3)
	localparam integer ADDR_LSB = (C_S_AXI_DATA_WIDTH/32) + 1;
	localparam integer OPT_MEM_ADDR_BITS = 5;
	//----------------------------------------------
	//-- Signals for user logic register space example
	//------------------------------------------------
	//-- Number of Slave Registers 41
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg0;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg1;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg2;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg3;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg4;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg5;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg6;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg7;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg8;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg9;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg10;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg11;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg12;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg13;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg14;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg15;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg16;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg17;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg18;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg19;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg20;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg21;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg22;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg23;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg24;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg25;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg26;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg27;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg28;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg29;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg30;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg31;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg32;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg33;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg34;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg35;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg36;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg37;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg38;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg39;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg40;
	wire	 slv_reg_rden;
	wire	 slv_reg_wren;
	reg [C_S_AXI_DATA_WIDTH-1:0]	 reg_data_out;
	integer	 byte_index;
	reg	 aw_en;

	// I/O Connections assignments

	assign S_AXI_AWREADY	= axi_awready;
	assign S_AXI_WREADY	= axi_wready;
	assign S_AXI_BRESP	= axi_bresp;
	assign S_AXI_BVALID	= axi_bvalid;
	assign S_AXI_ARREADY	= axi_arready;
	assign S_AXI_RDATA	= axi_rdata;
	assign S_AXI_RRESP	= axi_rresp;
	assign S_AXI_RVALID	= axi_rvalid;
	// Implement axi_awready generation
	// axi_awready is asserted for one S_AXI_ACLK clock cycle when both
	// S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_awready is
	// de-asserted when reset is low.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_awready <= 1'b0;
	      aw_en <= 1'b1;
	    end 
	  else
	    begin    
	      if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	        begin
	          // slave is ready to accept write address when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_awready <= 1'b1;
	          aw_en <= 1'b0;
	        end
	        else if (S_AXI_BREADY && axi_bvalid)
	            begin
	              aw_en <= 1'b1;
	              axi_awready <= 1'b0;
	            end
	      else           
	        begin
	          axi_awready <= 1'b0;
	        end
	    end 
	end       

	// Implement axi_awaddr latching
	// This process is used to latch the address when both 
	// S_AXI_AWVALID and S_AXI_WVALID are valid. 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_awaddr <= 0;
	    end 
	  else
	    begin    
	      if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	        begin
	          // Write Address latching 
	          axi_awaddr <= S_AXI_AWADDR;
	        end
	    end 
	end       

	// Implement axi_wready generation
	// axi_wready is asserted for one S_AXI_ACLK clock cycle when both
	// S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_wready is 
	// de-asserted when reset is low. 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_wready <= 1'b0;
	    end 
	  else
	    begin    
	      if (~axi_wready && S_AXI_WVALID && S_AXI_AWVALID && aw_en )
	        begin
	          // slave is ready to accept write data when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_wready <= 1'b1;
	        end
	      else
	        begin
	          axi_wready <= 1'b0;
	        end
	    end 
	end       

	// Implement memory mapped register select and write logic generation
	// The write data is accepted and written to memory mapped registers when
	// axi_awready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted. Write strobes are used to
	// select byte enables of slave registers while writing.
	// These registers are cleared when reset (active low) is applied.
	// Slave register write enable is asserted when valid address and data are available
	// and the slave is ready to accept the write address and write data.
	assign slv_reg_wren = axi_wready && S_AXI_WVALID && axi_awready && S_AXI_AWVALID;

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      slv_reg0 <= 0;
	      slv_reg1 <= 0;
	      slv_reg2 <= 0;
	      slv_reg3 <= 0;
	      slv_reg4 <= 0;
	      slv_reg5 <= 0;
	      slv_reg6 <= 0;
	      slv_reg7 <= 0;
	      slv_reg8 <= 0;
	      slv_reg9 <= 0;
	      slv_reg10 <= 0;
	      slv_reg11 <= 0;
	      slv_reg12 <= 0;
	      slv_reg13 <= 0;
	      slv_reg14 <= 0;
	      slv_reg15 <= 0;
	      slv_reg16 <= 0;
	      slv_reg17 <= 0;
	      slv_reg18 <= 0;
	      slv_reg19 <= 0;
	      slv_reg20 <= 0;
	      slv_reg21 <= 0;
	      slv_reg22 <= 0;
	      slv_reg23 <= 0;
	      slv_reg24 <= 0;
	      slv_reg25 <= 0;
	      slv_reg26 <= 0;
	      slv_reg27 <= 0;
	      slv_reg28 <= 0;
	      slv_reg29 <= 0;
	      slv_reg30 <= 0;
	      slv_reg31 <= 0;
	      slv_reg32 <= 0;
	      slv_reg33 <= 0;
	      slv_reg34 <= 0;
	      slv_reg35 <= 0;
	      slv_reg36 <= 0;
	      slv_reg37 <= 0;
	      slv_reg38 <= 0;
	      slv_reg39 <= 0;
	      slv_reg40 <= 0;
	    end 
	  else begin
	    if (slv_reg_wren)
	      begin
	        case ( axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
	          6'h00:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 0
	                slv_reg0[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h01:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 1
	                slv_reg1[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h02:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 2
	                slv_reg2[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h03:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 3
	                slv_reg3[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h04:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 4
	                slv_reg4[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h05:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 5
	                slv_reg5[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h06:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 6
	                slv_reg6[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h07:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 7
	                slv_reg7[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h08:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 8
	                slv_reg8[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h09:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 9
	                slv_reg9[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h0A:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 10
	                slv_reg10[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h0B:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 11
	                slv_reg11[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h0C:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 12
	                slv_reg12[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h0D:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 13
	                slv_reg13[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h0E:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 14
	                slv_reg14[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h0F:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 15
	                slv_reg15[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h10:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 16
	                slv_reg16[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h11:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 17
	                slv_reg17[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h12:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 18
	                slv_reg18[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h13:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 19
	                slv_reg19[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h14:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 20
	                slv_reg20[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h15:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 21
	                slv_reg21[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h16:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 22
	                slv_reg22[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h17:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 23
	                slv_reg23[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h18:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 24
	                slv_reg24[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h19:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 25
	                slv_reg25[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h1A:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 26
	                slv_reg26[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h1B:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 27
	                slv_reg27[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h1C:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 28
	                slv_reg28[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h1D:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 29
	                slv_reg29[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h1E:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 30
	                slv_reg30[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h1F:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 31
	                slv_reg31[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h20:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 32
	                slv_reg32[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h21:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 33
	                slv_reg33[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h22:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 34
	                slv_reg34[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h23:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 35
	                slv_reg35[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h24:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 36
	                slv_reg36[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h25:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 37
	                slv_reg37[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h26:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 38
	                slv_reg38[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h27:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 39
	                slv_reg39[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          6'h28:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 40
	                slv_reg40[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          default : begin
	                      slv_reg0 <= slv_reg0;
	                      slv_reg1 <= slv_reg1;
	                      slv_reg2 <= slv_reg2;
	                      slv_reg3 <= slv_reg3;
	                      slv_reg4 <= slv_reg4;
	                      slv_reg5 <= slv_reg5;
	                      slv_reg6 <= slv_reg6;
	                      slv_reg7 <= slv_reg7;
	                      slv_reg8 <= slv_reg8;
	                      slv_reg9 <= slv_reg9;
	                      slv_reg10 <= slv_reg10;
	                      slv_reg11 <= slv_reg11;
	                      slv_reg12 <= slv_reg12;
	                      slv_reg13 <= slv_reg13;
	                      slv_reg14 <= slv_reg14;
	                      slv_reg15 <= slv_reg15;
	                      slv_reg16 <= slv_reg16;
	                      slv_reg17 <= slv_reg17;
	                      slv_reg18 <= slv_reg18;
	                      slv_reg19 <= slv_reg19;
	                      slv_reg20 <= slv_reg20;
	                      slv_reg21 <= slv_reg21;
	                      slv_reg22 <= slv_reg22;
	                      slv_reg23 <= slv_reg23;
	                      slv_reg24 <= slv_reg24;
	                      slv_reg25 <= slv_reg25;
	                      slv_reg26 <= slv_reg26;
	                      slv_reg27 <= slv_reg27;
	                      slv_reg28 <= slv_reg28;
	                      slv_reg29 <= slv_reg29;
	                      slv_reg30 <= slv_reg30;
	                      slv_reg31 <= slv_reg31;
	                      slv_reg32 <= slv_reg32;
	                      slv_reg33 <= slv_reg33;
	                      slv_reg34 <= slv_reg34;
	                      slv_reg35 <= slv_reg35;
	                      slv_reg36 <= slv_reg36;
	                      slv_reg37 <= slv_reg37;
	                      slv_reg38 <= slv_reg38;
	                      slv_reg39 <= slv_reg39;
	                      slv_reg40 <= slv_reg40;
	                    end
	        endcase
	      end
	  end
	end    

	// Implement write response logic generation
	// The write response and response valid signals are asserted by the slave 
	// when axi_wready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted.  
	// This marks the acceptance of address and indicates the status of 
	// write transaction.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_bvalid  <= 0;
	      axi_bresp   <= 2'b0;
	    end 
	  else
	    begin    
	      if (axi_awready && S_AXI_AWVALID && ~axi_bvalid && axi_wready && S_AXI_WVALID)
	        begin
	          // indicates a valid write response is available
	          axi_bvalid <= 1'b1;
	          axi_bresp  <= 2'b0; // 'OKAY' response 
	        end                   // work error responses in future
	      else
	        begin
	          if (S_AXI_BREADY && axi_bvalid) 
	            //check if bready is asserted while bvalid is high) 
	            //(there is a possibility that bready is always asserted high)   
	            begin
	              axi_bvalid <= 1'b0; 
	            end  
	        end
	    end
	end   

	// Implement axi_arready generation
	// axi_arready is asserted for one S_AXI_ACLK clock cycle when
	// S_AXI_ARVALID is asserted. axi_awready is 
	// de-asserted when reset (active low) is asserted. 
	// The read address is also latched when S_AXI_ARVALID is 
	// asserted. axi_araddr is reset to zero on reset assertion.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_arready <= 1'b0;
	      axi_araddr  <= 32'b0;
	    end 
	  else
	    begin    
	      if (~axi_arready && S_AXI_ARVALID)
	        begin
	          // indicates that the slave has acceped the valid read address
	          axi_arready <= 1'b1;
	          // Read address latching
	          axi_araddr  <= S_AXI_ARADDR;
	        end
	      else
	        begin
	          axi_arready <= 1'b0;
	        end
	    end 
	end       

	// Implement axi_arvalid generation
	// axi_rvalid is asserted for one S_AXI_ACLK clock cycle when both 
	// S_AXI_ARVALID and axi_arready are asserted. The slave registers 
	// data are available on the axi_rdata bus at this instance. The 
	// assertion of axi_rvalid marks the validity of read data on the 
	// bus and axi_rresp indicates the status of read transaction.axi_rvalid 
	// is deasserted on reset (active low). axi_rresp and axi_rdata are 
	// cleared to zero on reset (active low).  
	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_rvalid <= 0;
	      axi_rresp  <= 0;
	    end 
	  else
	    begin    
	      if (axi_arready && S_AXI_ARVALID && ~axi_rvalid)
	        begin
	          // Valid read data is available at the read data bus
	          axi_rvalid <= 1'b1;
	          axi_rresp  <= 2'b0; // 'OKAY' response
	        end   
	      else if (axi_rvalid && S_AXI_RREADY)
	        begin
	          // Read data is accepted by the master
	          axi_rvalid <= 1'b0;
	        end                
	    end
	end    

	// Implement memory mapped register select and read logic generation
	// Slave register read enable is asserted when valid address is available
	// and the slave is ready to accept the read address.
	assign slv_reg_rden = axi_arready & S_AXI_ARVALID & ~axi_rvalid;
	always @(*)
	begin
	      // Address decoding for reading registers
	      case ( axi_araddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
	        6'h00   : reg_data_out <= slv_reg0;			// Trigger Register
	        6'h01   : reg_data_out <= reg_config_out;	// Cofiguration : Leg Pointer
	        6'h02   : reg_data_out <= flp_output_q1;	// FLP : iKinematics Q1
	        6'h03   : reg_data_out <= flp_output_q2;	// FLP : iKinematics Q2
	        6'h04   : reg_data_out <= flp_output_q3;	// FLP : iKinematics Q3
	        6'h05   : reg_data_out <= slv_reg5;			// Offset Q1	-- Leg 1
	        6'h06   : reg_data_out <= slv_reg6;			// Offset Q2
	        6'h07   : reg_data_out <= slv_reg7;			// Offset Q3
	        6'h08   : reg_data_out <= slv_reg8;			// Offset Q4	-- Leg 2
	        6'h09   : reg_data_out <= slv_reg9;			// Offset Q5
	        6'h0A   : reg_data_out <= slv_reg10;		// Offset Q6
	        6'h0B   : reg_data_out <= slv_reg11;		// Offset Q7	-- Leg 3
	        6'h0C   : reg_data_out <= slv_reg12;		// Offset Q8	
	        6'h0D   : reg_data_out <= slv_reg13;		// Offset Q9
	        6'h0E   : reg_data_out <= slv_reg14;		// Offset Q10	-- Leg 4
	        6'h0F   : reg_data_out <= slv_reg15;		// Offset Q11
	        6'h10   : reg_data_out <= slv_reg16;		// Offset Q12
	        6'h11   : reg_data_out <= slv_reg17;		// Offset Q13	-- Leg 5
	        6'h12   : reg_data_out <= slv_reg18;		// Offset Q14
	        6'h13   : reg_data_out <= slv_reg19;		// Offset Q15
	        6'h14   : reg_data_out <= slv_reg20;		// Offset Q16	-- Leg 6
	        6'h15   : reg_data_out <= slv_reg21;		// Offset Q17
	        6'h16   : reg_data_out <= slv_reg22;		// Offset Q18
	        6'h17   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[0+:C_FXP_WIDTH]};					// PWM Q1
	        6'h18   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH+:C_FXP_WIDTH]};		// PWM Q2
	        6'h19   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*2+:C_FXP_WIDTH]};		// PWM Q3
	        6'h1A   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*3+:C_FXP_WIDTH]};		// PWM Q4
	        6'h1B   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*4+:C_FXP_WIDTH]};		// PWM Q5
	        6'h1C   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*5+:C_FXP_WIDTH]};		// PWM Q6
	        6'h1D   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*6+:C_FXP_WIDTH]};		// PWM Q7
	        6'h1E   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*7+:C_FXP_WIDTH]};		// PWM Q8
	        6'h1F   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*8+:C_FXP_WIDTH]};		// PWM Q9
	        6'h20   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*9+:C_FXP_WIDTH]};		// PWM Q10
	        6'h21   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*10+:C_FXP_WIDTH]};	// PWM Q11
	        6'h22   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*11+:C_FXP_WIDTH]};	// PWM Q12
	        6'h23   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*12+:C_FXP_WIDTH]};	// PWM Q13
	        6'h24   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*13+:C_FXP_WIDTH]};	// PWM Q14
	        6'h25   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*14+:C_FXP_WIDTH]};	// PWM Q15
	        6'h26   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*15+:C_FXP_WIDTH]};	// PWM Q16
	        6'h27   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*16+:C_FXP_WIDTH]};	// PWM Q17
	        6'h28   : reg_data_out <= {{C_FXP_DATA_FILL{1'b0}}, ikn_output_rb[C_FXP_WIDTH*17+:C_FXP_WIDTH]};	// PWM Q18
//	        6'h29	: reg_data_out <= {{12{1'b0}}, ikn_q1_p_offset};
//	        6'h2A	: reg_data_out <= {{12{1'b0}}, ikn_q2_p_offset};
//	        6'h2B	: reg_data_out <= {{12{1'b0}}, ikn_q3_p_offset};
//	        6'h2C	: reg_data_out <= {{12{1'b0}}, ikn_q1_offset};
//			6'h2D	: reg_data_out <= {{12{1'b0}}, ikn_q2_offset};
//			6'h2E	: reg_data_out <= {{12{1'b0}}, ikn_q3_offset};
//			6'h2F	: reg_data_out <= {{12{1'b0}}, ikn_q1};
//			6'h30	: reg_data_out <= {{12{1'b0}}, ikn_q2};
//			6'h31	: reg_data_out <= {{12{1'b0}}, ikn_q3};
	        default : reg_data_out <= 0;	
	      endcase
	end

	// Output register or memory read data
	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_rdata  <= 0;
	    end 
	  else
	    begin    
	      // When there is a valid read address (S_AXI_ARVALID) with 
	      // acceptance of read address by the slave (axi_arready), 
	      // output the read dada 
	      if (slv_reg_rden)
	        begin
	          axi_rdata <= reg_data_out;     // register read data
	        end   
	    end
	end    

	// Add user logic here
	//----------------------------------------------
	//-- User Instantiations
	//----------------------------------------------
	//-- Floating-point to Fixed-point Converter X
	float_to_fixed_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) flp2fxp_converter_x (
		.FLP_NUM(slv_reg2),
		.FXP_NUM(fxp_input_x),
		.OUT_RANGE(fxp_out_range_x),
		.FLP_ZERO(fxp_input_zero_x)
		);
	//-- Floating-point to Fixed-point Converter Y
	float_to_fixed_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) flp2fxp_converter_y (
		.FLP_NUM(slv_reg3),
		.FXP_NUM(fxp_input_y),
		.OUT_RANGE(fxp_out_range_y),
		.FLP_ZERO(fxp_input_zero_y)
		);
	//-- Floating-point to Fixed-point Converter Z
	float_to_fixed_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) flp2fxp_converter_z (
		.FLP_NUM(slv_reg4),
		.FXP_NUM(fxp_input_z),
		.OUT_RANGE(fxp_out_range_z),
		.FLP_ZERO(fxp_input_zero_z)
		);
	//-- FIFO
	// xpm_fifo_sync: Synchronous FIFO
	// Xilinx Parameterized Macro, version 2018.3
	   xpm_fifo_sync #(
		  .DOUT_RESET_VALUE("0"),    				// String
		  .ECC_MODE("no_ecc"),       				// String
		  .FIFO_MEMORY_TYPE("distributed"), 		// String	: auto
		  .FIFO_READ_LATENCY(0),     				// DECIMAL	: Fall through
		  .FIFO_WRITE_DEPTH(C_FIFO_DEPTH),   			// DECIMAL
		  .FULL_RESET_VALUE(0),      				// DECIMAL
		  .PROG_EMPTY_THRESH(5),    				// DECIMAL
		  .PROG_FULL_THRESH(FIFO_PROG_FULL_THRESH),	// DECIMAL	: (FIFO_DEPTH-3) - 
		  .RD_DATA_COUNT_WIDTH(1),   				// DECIMAL
		  .READ_DATA_WIDTH(FIFO_WIDTH),      		// DECIMAL
		  .READ_MODE("fwft"),         				// String	: First-Word-Fall-Through read mode
		  .USE_ADV_FEATURES("0000"), 				// String
		  .WAKEUP_TIME(0),           				// DECIMAL
		  .WRITE_DATA_WIDTH(FIFO_WIDTH),     		// DECIMAL
		  .WR_DATA_COUNT_WIDTH(1)    				// DECIMAL
	   )
	   xpm_fifo_sync_inst (
		  .almost_empty(),
		  .almost_full(),
		  .data_valid(),
		  .dbiterr(),
		  .dout(fifo_dout), 
		  .empty(fifo_empty), 
		  .full(fifo_full),
		  .overflow(),
		  .prog_empty(),
		  .prog_full(),
		  .rd_data_count(),
		  .rd_rst_busy(),
		  .sbiterr(), 
		  .underflow(),
		  .wr_ack(), 
		  .wr_data_count(),
		  .wr_rst_busy(), 
		  .din({fxp_input_z,fxp_input_y,fxp_input_x}),
		  .injectdbiterr(1'b0),
		  .injectsbiterr(1'b0),
		  .rd_en(ikn_start||fifo_rd_en),
		  .rst(!S_AXI_ARESETN),
		  .sleep(1'b0),
		  .wr_clk(S_AXI_ACLK),
		  .wr_en(fifo_wr_en)
	   );
	//-- Inverse Kinematics Architecture --
	FXP_iKinematics
	#(
		//-- Fixed Point Parameters --
		.C_FXP_WIDTH(C_FXP_WIDTH),				
		.C_FXP_POINT(C_FXP_POINT),
		//-- Inverse Kinematics Parameters --
		.C_IKN_iK(C_IKN_iK),
		.C_IKN_iKH(C_IKN_iKH),
		.C_IKN_F(C_IKN_F),
		.C_IKN_C2(C_IKN_C2),
		.C_IKN_C3(C_IKN_C3),
		.C_IKN_Ca8(C_IKN_Ca8),
		.C_IKN_L1(C_IKN_L1),
		.C_IKN_nL1(C_IKN_nL1),
		//-- CORDIC Parameters --			
		.C_COR_ITER(C_COR_ITER),
		.C_COR_N_ITER(C_COR_N_ITER)
	) InverseKinematics (
		//-- SYSTEM INTERFACE --
		.nRST(S_AXI_ARESETN),
		.CLK(S_AXI_ACLK),
		//-- CONTROL INTERFACE --
		.START(ikn_start),
		.DATA_VALID(ikn_data_valid),	// Lacks output
		.OUT_DATA_READY(ikn_data_ready),
		.BUSY(ikn_busy),
		//-- FIFO INTERFACE --
		.FIFO_EMPTY(fifo_empty),
		.FIFO_READ(fifo_rd_en),
		//-- Inverse Kinematics In/Out Parameters --
		.X_IN(fifo_dout[0+:C_FXP_WIDTH]),
		.Y_IN(fifo_dout[C_FXP_WIDTH+:C_FXP_WIDTH]),
		.Z_IN(fifo_dout[C_FXP_WIDTH*2+:C_FXP_WIDTH]),
		.Q1_OUT(ikn_q1),
		.Q2_OUT(ikn_q2),
		.Q3_OUT(ikn_q3)
		);
	//-- Fixed-point to Floating-point Converter Q1
	fixed_to_float_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) fxp2flp_converter_q1 (
		.FXP_NUM(reg_flp_output_q1),
		.FLP_NUM(flp_output_q1),
		.FLP_ZERO()
		);
	//-- Fixed-point to Floating-point Converter Q2
	fixed_to_float_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) fxp2flp_converter_q2 (
		.FXP_NUM(reg_flp_output_q2),
		.FLP_NUM(flp_output_q2),
		.FLP_ZERO()
		);
	//-- Fixed-point to Floating-point Converter Q3
	fixed_to_float_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) fxp2flp_converter_q3 (
		.FXP_NUM(reg_flp_output_q3),
		.FLP_NUM(flp_output_q3),
		.FLP_ZERO()
		);
	
	//-- 
	//-- Offset obtain& convert
	// Floating-point to Fixed-point Converter Offset Q1
	float_to_fixed_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) flp2fxp_converter_offset_q1 (
		.FLP_NUM(ikn_q1_mux_offset),
		.FXP_NUM(ikn_q1_offset),
		.OUT_RANGE(),
		.FLP_ZERO()
		);
	// Floating-point to Fixed-point Converter Offset Q2
	float_to_fixed_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) flp2fxp_converter_offset_q2 (
		.FLP_NUM(ikn_q2_mux_offset),
		.FXP_NUM(ikn_q2_offset),
		.OUT_RANGE(),
		.FLP_ZERO()
		);
	// Floating-point to Fixed-point Converter Offset Q3
	float_to_fixed_converter
	#(
		.C_FXP_WIDTH(C_FXP_WIDTH),
		.C_FXP_POINT(C_FXP_POINT),
		.C_FLP_WIDTH(C_FLP_WIDTH)
	) flp2fxp_converter_offset_q3 (
		.FLP_NUM(ikn_q3_mux_offset),
		.FXP_NUM(ikn_q3_offset),
		.OUT_RANGE(),
		.FLP_ZERO()
		);
	//-- Fixed point output angle offset + PI/4
	// Q1 + Offset
	fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
	) FXP_Q1_plus_Offset (
		.S_NUM1(ikn_q1),
		.S_NUM2(reg_ikn_q1_offset),
		.S_OPE(1'b0),
		.S_RESULT(ikn_q1_p_offset),
		.S_OF_FLAG()
		);
	// Q2 + Offset
	fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
	) FXP_Q2_plus_Offset (
		.S_NUM1(ikn_q2),
		.S_NUM2(reg_ikn_q2_offset),
		.S_OPE(1'b0),
		.S_RESULT(ikn_q2_p_offset),
		.S_OF_FLAG()
		);
	// Q3 + Offset
	fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
	) FXP_Q3_plus_Offset (
		.S_NUM1(ikn_q3),
		.S_NUM2(reg_ikn_q3_offset),
		.S_OPE(1'b0),
		.S_RESULT(ikn_q3_p_offset),
		.S_OF_FLAG()
		);
	
	// Leg Counter
	leg_counter #(
	.N_LEGS(C_ROB_NLEGS)
	) LEG_COUNTER (
	.CLK(S_AXI_ACLK),
	.nRST(S_AXI_ARESETN),
	.CTR_MODE(slv_reg1[5:4]),
	.SET(lgc_set),
	.TRIGGER(ikn_data_ready),
	.LEG_IN_SELECT(slv_reg1[C_JOI_CTR_SIZE-1:0]),
	.INVALID_SELECT(lgc_invalid),
	.LEG_OUT_SELECT(lgc_leg_sel)
	);
	
	//----------------------------------------------
	//-- User Behavioral
	//----------------------------------------------
	//-- FIFO Write --
	always @( posedge S_AXI_ACLK )
	  if ( S_AXI_ARESETN == 1'b0 )
	  	fifo_wr_en <= 1'b0;
	  else
	  	if (slv_reg_wren && ((axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] == 6'h00)))
	  		fifo_wr_en <= S_AXI_WDATA[0];
	  	else
	  		fifo_wr_en <= 1'b0;
	
	//-- Inverse Kinematics Trigger --
	always @( posedge S_AXI_ACLK )
	  if ( S_AXI_ARESETN == 1'b0 )
	  	ikn_start <= 1'b0;
	  else
	  	if (slv_reg_wren && ((axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] == 6'h00)))
	  		ikn_start <= S_AXI_WDATA[1];
	  	else
	  		ikn_start <= 1'b0;
	
	//-- Write Output Directly
	always @( posedge S_AXI_ACLK )
	  if ( S_AXI_ARESETN == 1'b0 )
	  	reg_output_direct <= 1'b0;
	  else
	  	if (slv_reg_wren && ((axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] == 6'h00)))
	  		reg_output_direct <= S_AXI_WDATA[2];
	  	else
	  		reg_output_direct <= 1'b0;
	  	  
	//-- Joint Counter --
	always @( posedge S_AXI_ACLK )
	  if ( S_AXI_ARESETN == 1'b0 )
	  	lgc_set <= 1'b0;
	  else
	  	if (slv_reg_wren && ((axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] == 6'h01)))
	  		lgc_set <= S_AXI_WDATA[3];
	  	else
	  		lgc_set <= 1'b0;
	  
	//-- Inverse Kinematics Memory  Map Out -- 
	always @( posedge S_AXI_ACLK )
	if ( S_AXI_ARESETN == 1'b0 )
		ikn_output_rb <= {C_FXP_WIDTH*C_ROB_NLEGS*C_ROB_NJOINTS{1'b0}};
	else
		casex ( {ikn_data_ready, reg_output_direct, lgc_leg_sel} )
			5'b1x000	: ikn_output_rb[C_FXP_WIDTH*3*0+:C_FXP_WIDTH*3] <= {ikn_q3_p_offset, ikn_q2_p_offset, ikn_q1_p_offset};
			5'b1x001	: ikn_output_rb[C_FXP_WIDTH*3*1+:C_FXP_WIDTH*3] <= {ikn_q3_p_offset, ikn_q2_p_offset, ikn_q1_p_offset};
			5'b1x010	: ikn_output_rb[C_FXP_WIDTH*3*2+:C_FXP_WIDTH*3] <= {ikn_q3_p_offset, ikn_q2_p_offset, ikn_q1_p_offset};
			5'b1x011	: ikn_output_rb[C_FXP_WIDTH*3*3+:C_FXP_WIDTH*3] <= {ikn_q3_p_offset, ikn_q2_p_offset, ikn_q1_p_offset};
			5'b1x100	: ikn_output_rb[C_FXP_WIDTH*3*4+:C_FXP_WIDTH*3] <= {ikn_q3_p_offset, ikn_q2_p_offset, ikn_q1_p_offset};
			5'b1x101	: ikn_output_rb[C_FXP_WIDTH*3*5+:C_FXP_WIDTH*3] <= {ikn_q3_p_offset, ikn_q2_p_offset, ikn_q1_p_offset};
			5'b01000	: ikn_output_rb[C_FXP_WIDTH*3*0+:C_FXP_WIDTH*3] <= {fxp_input_z, fxp_input_y, fxp_input_x};
			5'b01001	: ikn_output_rb[C_FXP_WIDTH*3*1+:C_FXP_WIDTH*3] <= {fxp_input_z, fxp_input_y, fxp_input_x};
			5'b01010	: ikn_output_rb[C_FXP_WIDTH*3*2+:C_FXP_WIDTH*3] <= {fxp_input_z, fxp_input_y, fxp_input_x};
			5'b01011	: ikn_output_rb[C_FXP_WIDTH*3*3+:C_FXP_WIDTH*3] <= {fxp_input_z, fxp_input_y, fxp_input_x};
			5'b01100	: ikn_output_rb[C_FXP_WIDTH*3*4+:C_FXP_WIDTH*3] <= {fxp_input_z, fxp_input_y, fxp_input_x};
			5'b01101	: ikn_output_rb[C_FXP_WIDTH*3*5+:C_FXP_WIDTH*3] <= {fxp_input_z, fxp_input_y, fxp_input_x};
			default	: ikn_output_rb <= ikn_output_rb;
		endcase
	
	//-- PWM Output --
	genvar pwm_ctr;
	generate
		for (pwm_ctr=0; pwm_ctr<C_ROB_NLEGS*C_ROB_NJOINTS; pwm_ctr=pwm_ctr+1) begin : GENERATE_PWM_OUTPUT_BUS_ASSIGNMENT
			fxp_to_pwm #(
				.FXP_WIDTH(C_FXP_WIDTH),
				.PWM_OFFSET(C_FXP_PWM_OFFSET),
				.PWM_WIDTH(C_PWM_SIZE)
			) FXP2PWM (
				.FXP_IN(ikn_output_rb[C_FXP_WIDTH*pwm_ctr+:C_FXP_WIDTH]),
				.PWM_OUT(O_JOINT_PWM[C_PWM_SIZE*pwm_ctr+:C_PWM_SIZE])
			);
		end
	endgenerate
	  
	//-- Offset Selector --
	always @(*)
		case (lgc_leg_sel)
	  		0	: begin
					ikn_q1_mux_offset = slv_reg5;
					ikn_q2_mux_offset = slv_reg6;
					ikn_q3_mux_offset = slv_reg7;
					end
			1	: begin
					ikn_q1_mux_offset = slv_reg8;
					ikn_q2_mux_offset = slv_reg9;
					ikn_q3_mux_offset = slv_reg10;
					end
			2	: begin
					ikn_q1_mux_offset = slv_reg11;
					ikn_q2_mux_offset = slv_reg12;
					ikn_q3_mux_offset = slv_reg13;
					end
			3	: begin
					ikn_q1_mux_offset = slv_reg14;
					ikn_q2_mux_offset = slv_reg15;
					ikn_q3_mux_offset = slv_reg16;
					end
			4	: begin
					ikn_q1_mux_offset = slv_reg17;
					ikn_q2_mux_offset = slv_reg18;
					ikn_q3_mux_offset = slv_reg19;
					end
			5	: begin
					ikn_q1_mux_offset = slv_reg20;
					ikn_q2_mux_offset = slv_reg21;
					ikn_q3_mux_offset = slv_reg22;
					end
			default : begin
					ikn_q1_mux_offset = 0;
					ikn_q2_mux_offset = 0;
					ikn_q3_mux_offset = 0;
					end
		endcase
	
	//-- Output Read Multiplexer
	always @(*)
		case ( slv_reg1[15:13] )
			0 : begin
				mux_flp_output_q1 = ikn_q1;
				mux_flp_output_q2 = ikn_q2;
				mux_flp_output_q3 = ikn_q3;
				end
			1 : begin
				mux_flp_output_q1 = ikn_output_rb[0+:C_FXP_WIDTH];
				mux_flp_output_q2 = ikn_output_rb[C_FXP_WIDTH+:C_FXP_WIDTH];
				mux_flp_output_q3 = ikn_output_rb[C_FXP_WIDTH*2+:C_FXP_WIDTH];
				end
			2 : begin
				mux_flp_output_q1 = ikn_output_rb[C_FXP_WIDTH*3+:C_FXP_WIDTH];
				mux_flp_output_q2 = ikn_output_rb[C_FXP_WIDTH*4+:C_FXP_WIDTH];
				mux_flp_output_q3 = ikn_output_rb[C_FXP_WIDTH*5+:C_FXP_WIDTH];
				end
			3 : begin
				mux_flp_output_q1 = ikn_output_rb[C_FXP_WIDTH*6+:C_FXP_WIDTH];
				mux_flp_output_q2 = ikn_output_rb[C_FXP_WIDTH*7+:C_FXP_WIDTH];
				mux_flp_output_q3 = ikn_output_rb[C_FXP_WIDTH*8+:C_FXP_WIDTH];
				end
			4 : begin
				mux_flp_output_q1 = ikn_output_rb[C_FXP_WIDTH*9+:C_FXP_WIDTH];
				mux_flp_output_q2 = ikn_output_rb[C_FXP_WIDTH*10+:C_FXP_WIDTH];
				mux_flp_output_q3 = ikn_output_rb[C_FXP_WIDTH*11+:C_FXP_WIDTH];
				end
			5 : begin
				mux_flp_output_q1 = ikn_output_rb[C_FXP_WIDTH*12+:C_FXP_WIDTH];
				mux_flp_output_q2 = ikn_output_rb[C_FXP_WIDTH*13+:C_FXP_WIDTH];
				mux_flp_output_q3 = ikn_output_rb[C_FXP_WIDTH*14+:C_FXP_WIDTH];
				end
			6 : begin
				mux_flp_output_q1 = ikn_output_rb[C_FXP_WIDTH*15+:C_FXP_WIDTH];
				mux_flp_output_q2 = ikn_output_rb[C_FXP_WIDTH*16+:C_FXP_WIDTH];
				mux_flp_output_q3 = ikn_output_rb[C_FXP_WIDTH*17+:C_FXP_WIDTH];
				end
			default : begin
				mux_flp_output_q1 = ikn_q1;
				mux_flp_output_q2 = ikn_q2;
				mux_flp_output_q3 = ikn_q3;
				end
		endcase
		
	
	//-- Configuration register read mode
	assign reg_config_out = {slv_reg1[31:16], slv_reg1[15:13], fifo_full, fifo_empty, ikn_busy, lgc_invalid, lgc_leg_sel, slv_reg1[5:4], slv_reg1[C_JOI_CTR_SIZE], slv_reg1[C_JOI_CTR_SIZE-1:0]};
	
	//-- Pipeline Reg Output
	always @( posedge S_AXI_ACLK )
		begin
		reg_flp_output_q1 <= mux_flp_output_q1;
		reg_flp_output_q2 <= mux_flp_output_q2;
		reg_flp_output_q3 <= mux_flp_output_q3;
		//
		reg_ikn_q1_offset <= ikn_q1_offset;
		reg_ikn_q2_offset <= ikn_q2_offset;
		reg_ikn_q3_offset <= ikn_q3_offset;
		end
	
	// User logic ends

	endmodule
