
`timescale 1 ns / 1 ps

	module axi_ikinematics_v1_2 #
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


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 8
	)
	(
		// Users to add ports here
		output wire [C_ROB_NLEGS*C_ROB_NJOINTS-1:0] s00_pwm_out,
    output wire s00_ikn_trigger,
		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface S00_AXI
		input wire  s00_axi_aclk,
		input wire  s00_axi_aresetn,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr,
		input wire [2 : 0] s00_axi_awprot,
		input wire  s00_axi_awvalid,
		output wire  s00_axi_awready,
		input wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata,
		input wire [(C_S00_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb,
		input wire  s00_axi_wvalid,
		output wire  s00_axi_wready,
		output wire [1 : 0] s00_axi_bresp,
		output wire  s00_axi_bvalid,
		input wire  s00_axi_bready,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr,
		input wire [2 : 0] s00_axi_arprot,
		input wire  s00_axi_arvalid,
		output wire  s00_axi_arready,
		output wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata,
		output wire [1 : 0] s00_axi_rresp,
		output wire  s00_axi_rvalid,
		input wire  s00_axi_rready
	);
	
	//-- User Declarations Begin --
	//----------------------------------------------
	//-- User Signals Definitions
	//----------------------------------------------
	// 8-bit PWM Value
	wire [C_PWM_SIZE*C_ROB_NLEGS*C_ROB_NJOINTS-1:0]	o_joint_pwm;
	wire [C_ROB_NLEGS*C_ROB_NJOINTS-1:0] o_joint_pwm_complement;
	//-- User Declarations End --
	
// Instantiation of Axi Bus Interface S00_AXI
	axi_ikinematics_v1_2_S00_AXI # ( 
		//-- BEGIN USER PARAMETERs --
		//-- FIFO Parameters --
		.C_FIFO_DEPTH(C_FIFO_DEPTH),
		//-- Floating Point Parameters --
		.C_FLP_WIDTH(C_FLP_WIDTH),
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
		.C_COR_N_ITER(C_COR_N_ITER),
		//-- PWM Parameters --
		.C_PWM_OFFSET(C_PWM_OFFSET),
		//-- Robot Parameters --
		.C_ROB_NLEGS(C_ROB_NLEGS),
		.C_ROB_NJOINTS(C_ROB_NJOINTS),
		//-- END USER PARAMETERS --
		.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
	) axi_ikinematics_v1_2_S00_AXI_inst (
		//-- User ports --
		.O_JOINT_PWM(o_joint_pwm),
		.O_JOINT_PWM_COMPLEMENT_OUT(o_joint_pwm_complement),
    .O_IKN_TRIGGER(s00_ikn_trigger),
		//-- End User Ports --
		.S_AXI_ACLK(s00_axi_aclk),
		.S_AXI_ARESETN(s00_axi_aresetn),
		.S_AXI_AWADDR(s00_axi_awaddr),
		.S_AXI_AWPROT(s00_axi_awprot),
		.S_AXI_AWVALID(s00_axi_awvalid),
		.S_AXI_AWREADY(s00_axi_awready),
		.S_AXI_WDATA(s00_axi_wdata),
		.S_AXI_WSTRB(s00_axi_wstrb),
		.S_AXI_WVALID(s00_axi_wvalid),
		.S_AXI_WREADY(s00_axi_wready),
		.S_AXI_BRESP(s00_axi_bresp),
		.S_AXI_BVALID(s00_axi_bvalid),
		.S_AXI_BREADY(s00_axi_bready),
		.S_AXI_ARADDR(s00_axi_araddr),
		.S_AXI_ARPROT(s00_axi_arprot),
		.S_AXI_ARVALID(s00_axi_arvalid),
		.S_AXI_ARREADY(s00_axi_arready),
		.S_AXI_RDATA(s00_axi_rdata),
		.S_AXI_RRESP(s00_axi_rresp),
		.S_AXI_RVALID(s00_axi_rvalid),
		.S_AXI_RREADY(s00_axi_rready)
	);

	// Add user logic here	
	genvar i;
	generate
		for (i=0; i<C_ROB_NLEGS*C_ROB_NJOINTS; i=i+1) begin : pwm_array
			servo_pwm_generator #(
        .C_PWM_SIZE(C_PWM_SIZE),
        .C_PWM_FREQ(C_PWM_FREQ),
        .C_PWM_MAX_IN(C_PWM_MAX_IN)
			) PWM_Q1 (
        //-- SYSTEM INTERFACE --
        .nRST(s00_axi_aresetn),			// Reset active low
        .CLK(s00_axi_aclk),			// Clock signal
        //-- PWM Control --
        .EN(1'b1),
        .COMPLEMENT(o_joint_pwm_complement[i]),
        .PWM_IN(o_joint_pwm[i*(C_PWM_SIZE)+:C_PWM_SIZE]),
        //-- PWM Output --
        .PWM_OUT(s00_pwm_out[i])
			);
		end
	endgenerate
	// User logic ends

	endmodule
