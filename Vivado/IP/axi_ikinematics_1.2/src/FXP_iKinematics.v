`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.01.2019 21:23:02
// Design Name: 
// Module Name: FXP_iKinematics
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module FXP_iKinematics
#(
	//-- Fixed Point Parameters --
	// Fixed Point Data bus width : 16 bit
	parameter integer C_FXP_WIDTH=16,
	// Fixed Point dot ubication from right [INT].[DEC]					
	parameter integer C_FXP_POINT=12,
	//-- Inverse Kinematics Parameters --
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_iK	= 'h09b7,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_iKH	= 'h3c37,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_F		= 'h0ea9,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_C2	= 'hff1e,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_C3	= 'hffaf,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_Ca8	= 'h62cd,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_L1	= 'h0070,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_nL1	= 'hff8f,
	//-- CORDIC Parameters --
	// CORDIC positive iterations					
	parameter integer C_COR_ITER=16,
	// CORDIC non-positive iterations
	parameter integer C_COR_N_ITER=1
)(
	//-- SYSTEM INTERFACE --
    input 								nRST,			// Reset active low
    input 								CLK,			// Clock signal
    //-- CONTROL INTERFACE --
    input								START,			// Architecture Trigger Signal
    output reg							DATA_VALID,		// Output Calculation Valid
    output reg							OUT_DATA_READY,	// 
    output 								BUSY,			// Architecture Busy
    //-- FIFO INTERFACE --
    input								FIFO_EMPTY,
    output								FIFO_READ,		// Read Next Input From FIFO
    //-- Inverse Kinematics In/Out Parameters --
    input signed [C_FXP_WIDTH-1:0]		X_IN,
    input signed [C_FXP_WIDTH-1:0]		Y_IN,
    input signed [C_FXP_WIDTH-1:0]		Z_IN,
    output reg signed [C_FXP_WIDTH-1:0]	Q1_OUT,
    output reg signed [C_FXP_WIDTH-1:0]	Q2_OUT,
    output reg signed [C_FXP_WIDTH-1:0]	Q3_OUT
    );

//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
//-- Fixed Point Parameters --
// Fixed Point Zero
localparam signed [C_FXP_WIDTH-1:0]	C_FXP_ZERO 		= {C_FXP_WIDTH{1'b0}};
// Fixed Point One
localparam signed [C_FXP_WIDTH-1:0]	C_FXP_ONE 		= {{C_FXP_WIDTH-C_FXP_POINT-1{1'b0}},1'b1,{C_FXP_POINT{1'b0}}};
//-- CORDIC Parameters --
// CORDIC Iteration Register Size
localparam 							C_ITER_SIZE		= $clog2(C_COR_ITER);
// CORDIC ATAN LUT Memory Pointer Size
localparam							C_MEM_ASIZE		= $clog2(C_COR_ITER+C_COR_N_ITER);
// Max iteration value
localparam [C_ITER_SIZE-1:0]		C_COR_MAXITER 	= C_COR_ITER - 1;
//-- Pipeline Parameters --
// Pipeline Clock Max Count
localparam 	 						C_PIP_MAX_PCNTR	= C_COR_ITER+2;
// Pipeline Clock Counter Size
localparam							C_PIP_CTR_SIZE	= $clog2(C_PIP_MAX_PCNTR+1);
//-- Finite State Machine States Definition --
localparam							C_FSM_MAX_STATE	= 3;
localparam							C_FSM_BUS_SIZE	= $clog2(C_FSM_MAX_STATE);
localparam [C_FSM_BUS_SIZE:0]		fsm_rset    	= 0,	//Reset
                    				fsm_idle    	= 1,    //Idle
                    				fsm_srun    	= 2,    //Iterating
                    				fsm_dout    	= 3;    //Data Ready

//-- XMP Dual Port ROM Memory Parameters --
// Memory Size : Fixed Point Data Width * Memory Depth(Total Iterations)
localparam 							MEMORY_SIZE		= C_FXP_WIDTH * (C_COR_ITER+C_COR_N_ITER);

//----------------------------------------------
//-- Signals Definition
//----------------------------------------------
//-- FSM Signals --
reg [C_FSM_BUS_SIZE-1:0]		fsm_state,
								fsm_next_state;
//-- Input Registers --
reg signed [C_FXP_WIDTH-1:0]	reg_xin,
								reg_yin,
								reg_zin;
//-- Pipeline Signals --
// Pipeline Activity : 
reg [5:0]						pip_activity;
// Pipeline Clock Counter
reg [C_PIP_CTR_SIZE-1:0]		pip_ctr;
// Pipeline Registers
reg signed [C_FXP_WIDTH-1:0]	stage1_reg0,
								stage1_reg1,
								stage1_reg2,
								stage2_reg0,
								stage2_reg1,
								stage2_reg2,
								stage2_reg3,
								stage3_reg0,
								stage3_reg1,
								stage3_reg2,
								stage3_reg3,
								stage3_reg4,
								stage4_reg0,
								stage4_reg1,
								stage4_reg2,
								stage4_reg3,
								stage5_reg0,
								stage5_reg1,
								stage5_reg2,
								stage5_reg3,
								stage5_reg4,
								stage5_reg5;
reg								stage1_overflow,
								stage2_overflow,
								stage3_overflow,
								stage4_overflow,
								stage5_overflow;
wire							pip_clock;
wire							stage1_c1_overflow,
								stage2_fsm_overflow,
								stage3_c2_overflow,
								stage3_c4_overflow,
								stage3_mult_overflow,
								stage4_c7_overflow,
								stage4_mult_overflow,	// Not connected to the overflow chain
								stage4_c3_overflow,
								stage4_sub_overflow,
								stage5_c8_overflow,
								stage5_add_overflow,
								stage5_mult_overflow,
								stage5_c5_overflow,
								stage6_c9_overflow,
								stage6_c6_overflow,
								stage6_add_overflow;
//-- ATAN LUT Signals --
// CORDIC LUT Pointer Channel A & B
wire [C_ITER_SIZE-1:0]			cor_ptr;
wire [C_MEM_ASIZE-1:0]			lut_ptr_a,
								lut_ptr_b;
wire [C_FXP_WIDTH-1:0]			lut_out_a,
								lut_out_b;
//-- CORDIC Signals --
// CORDIC & Stages Outputs
wire signed [C_FXP_WIDTH-1:0] 	stage1_c1_xout, 		// Stage 1
								stage1_c1_zout, 
								stage2_fsm_out0, 		// Stage 2
								stage2_fsm_out1,
								stage3_c2_xout, 		// Stage 3
								stage3_c4_xout, 
								stage3_mult_out,
								stage4_c7_zout, 		// Stage 4
								stage4_mult_out,
								stage4_c3_zout,
								stage4_sub_out,
								stage5_c8_xout,			// Stage 5
								stage5_add_out,
								stage5_mult_out,
								stage5_c5_yout,
								stage6_c9_zout,			// Stage 6
								stage6_c6_zout,
								stage6_add_out;

//----------------------------------------------
//-- Instantiations
//----------------------------------------------
//-- ATAN LUT ROM Memory Dual Port --
// xpm_memory_dprom: Dual Port ROM
   // Xilinx Parameterized Macro, version 2018.3
   xpm_memory_dprom #(
      .ADDR_WIDTH_A(C_MEM_ASIZE),               // DECIMAL
      .ADDR_WIDTH_B(C_MEM_ASIZE),               // DECIMAL
      .AUTO_SLEEP_TIME(0),            			// DECIMAL
      .CLOCKING_MODE("common_clock"), 			// String
      .ECC_MODE("no_ecc"),            			// String
      .MEMORY_INIT_FILE("atan_lut_mem.mem"),	// String
      .MEMORY_INIT_PARAM("0"),        			// String
      .MEMORY_OPTIMIZATION("true"),   			// String
      .MEMORY_PRIMITIVE("auto"),      			// String
      .MEMORY_SIZE(MEMORY_SIZE),             	// DECIMAL
      .MESSAGE_CONTROL(0),            			// DECIMAL
      .READ_DATA_WIDTH_A(C_FXP_WIDTH),			// DECIMAL
      .READ_DATA_WIDTH_B(C_FXP_WIDTH),			// DECIMAL
      .READ_LATENCY_A(2),             			// DECIMAL
      .READ_LATENCY_B(2),             			// DECIMAL
      .READ_RESET_VALUE_A("0"),       			// String
      .READ_RESET_VALUE_B("0"),       			// String
      .RST_MODE_A("SYNC"),            			// String
      .RST_MODE_B("SYNC"),            			// String
      .USE_MEM_INIT(1),               			// DECIMAL
      .WAKEUP_TIME("disable_sleep")   			// String
   )
   xpm_memory_dprom_inst (
      .dbiterra(),    				// 1-bit output: Leave open.
      .dbiterrb(),    				// 1-bit output: Leave open.
      .douta(lut_out_a),  			// READ_DATA_WIDTH_A-bit output: Data output for port A read operations.
      .doutb(lut_out_b),  			// READ_DATA_WIDTH_B-bit output: Data output for port B read operations.
      .sbiterra(),    				// 1-bit output: Leave open.
      .sbiterrb(),    				// 1-bit output: Leave open.
      .addra(lut_ptr_a),			// ADDR_WIDTH_A-bit input: Address for port A read operations.
      .addrb(lut_ptr_b),			// ADDR_WIDTH_B-bit input: Address for port B read operations.
      .clka(CLK),					// 1-bit input: Clock signal for port A. Also clocks port B when
 									// parameter CLOCKING_MODE is "common_clock".
      .clkb(CLK),					// 1-bit input: Clock signal for port B when parameter CLOCKING_MODE is
									// "independent_clock". Unused when parameter CLOCKING_MODE is
									// "common_clock".
      .ena(1'b1),					// 1-bit input: Memory enable signal for port A. Must be high on clock
									// cycles when read operations are initiated. Pipelined internally.
      .enb(1'b1),					// 1-bit input: Memory enable signal for port B. Must be high on clock
									// cycles when read operations are initiated. Pipelined internally.
      .injectdbiterra(1'b0),		// 1-bit input: Do not change from the provided value.
      .injectdbiterrb(1'b0),		// 1-bit input: Do not change from the provided value.
      .injectsbiterra(1'b0),		// 1-bit input: Do not change from the provided value.
      .injectsbiterrb(1'b0),		// 1-bit input: Do not change from the provided value.
      .regcea(1'b1),				// 1-bit input: Do not change from the provided value.
      .regceb(1'b1),				// 1-bit input: Do not change from the provided value.
      .rsta(!nRST),					// 1-bit input: Reset signal for the final port A output register stage.
									// Synchronously resets output port douta to the value specified by
									// parameter READ_RESET_VALUE_A.
      .rstb(!nRST),					// 1-bit input: Reset signal for the final port B output register stage.
									// Synchronously resets output port doutb to the value specified by
									// parameter READ_RESET_VALUE_B.
      .sleep(1'b0)					// 1-bit input: sleep signal to enable the dynamic power saving feature.
   );
//-- PIPELINE STAGE 1 --
// CORDIC 1 : Circular Vectorial
fxp_cordic_c #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER)
) S1_C1_CV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.MODE(1'b1),
	.X_IN(reg_xin),
	.Y_IN(reg_yin),
	.Z_IN(C_FXP_ZERO),
	.LUT_OUT(lut_out_a),
	.X_OUT(stage1_c1_xout),
	.Y_OUT(),							// DISCONNECTED
	.Z_OUT(stage1_c1_zout),
	.MEM_PTR(cor_ptr),
	.VALID_CO(stage1_c1_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
//-- PIPELINE STAGE 2 --
FSM_iKinematics_FSMv2 #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_IKN_iK(C_IKN_iK),
	.C_IKN_C2(C_IKN_C2),
	.C_IKN_C3(C_IKN_C3),
	.C_IKN_Ca8(C_IKN_Ca8),
	.C_IKN_nL1(C_IKN_nL1)
) S2_FSM (
	.CLK(CLK),
	.nRST(nRST),
	.START(pip_clock),
	.DATA_VALID(stage2_fsm_overflow),
	.STAGE1_REG0(stage1_reg0),
	.STAGE1_REG2(stage1_reg2),
	.STAGE2_REG0(stage2_fsm_out0),
	.STAGE2_REG1(stage2_fsm_out1)
	);
//-- PIPELINE STAGE 3 --
// CORDIC 2: Hyperbolic Vectorial
fxp_cordic_hv #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER-C_COR_N_ITER),
	.C_COR_N_ITER(C_COR_N_ITER)
) S3_C2_HV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.X_IN(C_FXP_ONE),
	.Y_IN(stage2_reg0),
	.X_OUT(stage3_c2_xout),
	.VALID_CO(stage3_c2_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
// CORDIC 4: Circular Vectorial
fxp_cordic_c #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER)
) S3_C4_CV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.MODE(1'b1),
	.X_IN(stage2_reg1),
	.Y_IN(stage2_reg2),
	.Z_IN(C_FXP_ZERO),
	.LUT_OUT(lut_out_a),
	.X_OUT(stage3_c4_xout),
	.Y_OUT(),							//DISCONNECTED
	.Z_OUT(),							//DISCONNECTED
	.MEM_PTR(),							//DISCONNECTED
	.VALID_CO(stage3_c4_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
// Multiplier
fxp_multiplier #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) S3_M1 (
	.S_NUM1(stage3_c4_xout),
	.S_NUM2(C_IKN_iK),
	.S_RESULT(stage3_mult_out),
	.S_OF_FLAG(stage3_mult_overflow)
    );
//-- PIPELINE STAGE 4 --
// CORDIC 7: Linear Vectorial
fxp_cordic_lv #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER)
) S4_C7_LV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.X_IN(stage3_reg2),
	.Y_IN(stage3_reg3),
	.Z_OUT(stage4_c7_zout),
	.VALID_CO(stage4_c7_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
// Multiplier
fxp_multiplier #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) S4_M2 (
	.S_NUM1(stage3_reg1),
	.S_NUM2(C_IKN_iKH),
	.S_RESULT(stage4_mult_out),
	.S_OF_FLAG(stage4_mult_overflow)
    );
// CORDIC 3: Circular Vectorial Expanded
cordic_c_exp #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER-C_COR_N_ITER),
	.C_COR_N_ITER(C_COR_N_ITER)
) S4_C3_CV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.MODE(1'b1),
	.X_IN(stage3_reg0),
	.Y_IN(stage4_mult_out),
	.Z_IN(C_FXP_ZERO),
	.LUT_OUT(lut_out_b),
	.X_OUT(),							//DISCONNECTED
	.Y_OUT(),							//DISCONNECTED
	.Z_OUT(stage4_c3_zout),
	.MEM_PTR(),							//DISCONNECTED
	.VALID_CO(stage4_c3_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
// Substractor
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) S4_SUB2 (
	.S_NUM1(C_FXP_ZERO),
	.S_NUM2(stage4_c3_zout),
	.S_OPE(1'b1),
	.S_RESULT(stage4_sub_out),
	.S_OF_FLAG(stage4_sub_overflow)
    );
//-- PIPELINE STAGE 5 --
// CORDIC 8: Hyperbolic Vectorial
fxp_cordic_hv #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER-C_COR_N_ITER),
	.C_COR_N_ITER(C_COR_N_ITER)
) S5_C8_HV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.X_IN(C_FXP_ONE),
	.Y_IN(stage4_reg0),
	.X_OUT(stage5_c8_xout),
	.VALID_CO(stage5_c8_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
// Multiplier
fxp_multiplier #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) S5_M3 (
	.S_NUM1(stage5_c8_xout),
	.S_NUM2(C_IKN_iKH),
	.S_RESULT(stage5_mult_out),
	.S_OF_FLAG(stage5_mult_overflow)
    );
// Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) S5_ADD1 (
	.S_NUM1(C_IKN_F),
	.S_NUM2(stage4_reg1),
	.S_OPE(1'b0),
	.S_RESULT(stage5_add_out),
	.S_OF_FLAG(stage5_add_overflow)
    );
// CORDIC 5: Circular Rotation
fxp_cordic_c #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER)
) S5_C5_CR (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.MODE(1'b0),
	.X_IN(C_IKN_iK),
	.Y_IN(C_FXP_ZERO),
	.Z_IN(stage4_reg2),
	.LUT_OUT(lut_out_a),
	.X_OUT(),							//DISCONNECTED
	.Y_OUT(stage5_c5_yout),
	.Z_OUT(),							//DISCONNECTED
	.MEM_PTR(),							//DISCONNECTED
	.VALID_CO(stage5_c5_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
//-- PIPELINE STAGE 6 --
// CORDIC 9: Circular Vectorial
fxp_cordic_c #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER)
) S6_C9_CV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.MODE(1'b1),
	.X_IN(stage5_reg0),
	.Y_IN(stage5_reg1),
	.Z_IN(C_FXP_ZERO),
	.LUT_OUT(lut_out_a),
	.X_OUT(),							//DISCONNECTED
	.Y_OUT(),							//DISCONNECTED
	.Z_OUT(stage6_c9_zout),
	.MEM_PTR(),							//DISCONNECTED
	.VALID_CO(stage6_c9_overflow),
	.DATA_RDY()							// DISCONNECTED
    );	
// CORDIC 6: Circular Vectorial
fxp_cordic_c #(
	.C_FXP_WIDTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT),
	.C_COR_ITER(C_COR_ITER)
) S6_C6_CV (
    .nRST(nRST),
	.CLK(CLK),
	.START(pip_clock),
	.MODE(1'b1),
	.X_IN(stage5_reg2),
	.Y_IN(stage5_reg3),
	.Z_IN(C_FXP_ZERO),
	.LUT_OUT(lut_out_a),
	.X_OUT(),							//DISCONNECTED
	.Y_OUT(),							//DISCONNECTED
	.Z_OUT(stage6_c6_zout),
	.MEM_PTR(),							//DISCONNECTED
	.VALID_CO(stage6_c6_overflow),
	.DATA_RDY()							// DISCONNECTED
    );
// Substractor
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) S6_SUB1 (
	.S_NUM1(stage6_c9_zout),
	.S_NUM2(stage6_c6_zout),
	.S_OPE(1'b1),
	.S_RESULT(stage6_add_out),
	.S_OF_FLAG(stage6_add_overflow)
    );

//----------------------------------------------
//-- Behavioral
//----------------------------------------------
//-- FSM --
// FSM State Register
always @(posedge CLK)
	if (nRST)
		fsm_state <= fsm_next_state;
	else
		fsm_state <= {C_FSM_BUS_SIZE{1'b0}};
// FSM Next State Logic
always @(*)
	case (fsm_state)
		fsm_rset: fsm_next_state = fsm_idle;
		fsm_idle: fsm_next_state = (START) ? fsm_srun : fsm_idle;
		fsm_srun: fsm_next_state = (~|pip_activity) ? fsm_dout : fsm_srun;
		fsm_dout: fsm_next_state = fsm_idle;
		default: fsm_next_state = fsm_idle;
	endcase

//-- Pipeline Signals Behavior
//Pipeline Activity Register
always @(posedge CLK)
	if (nRST)
		if (START || pip_clock || OUT_DATA_READY)	// ADD : || DATA_RDY
			pip_activity <= {(START || FIFO_READ),pip_activity[5:1]};
		else
			pip_activity <= pip_activity;
	else
		pip_activity <= 6'h00;

//Pipeline Counter
always @(posedge CLK)
	if (nRST)
		if (fsm_state == fsm_srun)
			if (pip_ctr < C_PIP_MAX_PCNTR)
				pip_ctr <= pip_ctr + 1;
			else
				pip_ctr <= {C_PIP_CTR_SIZE{1'b0}};
		else
			pip_ctr <= {C_PIP_CTR_SIZE{1'b0}};
	else
		pip_ctr <= {C_PIP_CTR_SIZE{1'b0}};

// Pipeline Clock
assign pip_clock = ((pip_ctr == C_PIP_MAX_PCNTR) && |pip_activity[5:1]) || START;	// Verify if START can be asynchronous
// Read FIFO Signal
assign FIFO_READ = pip_clock && ~FIFO_EMPTY;
		
//-- ATAN LUT --
assign lut_ptr_a = cor_ptr + C_COR_N_ITER;
assign lut_ptr_b = cor_ptr;

//-- Input Registers --
always @(posedge CLK)
	if (nRST)
		if (START || FIFO_READ) begin
			reg_xin <= X_IN;
			reg_yin <= Y_IN;
			reg_zin <= Z_IN;
			end
		else begin
			reg_xin <= reg_xin;
			reg_yin <= reg_yin;
			reg_zin <= reg_zin;
		end
	else begin
		reg_xin <= C_FXP_ZERO;
		reg_yin <= C_FXP_ZERO;
		reg_zin <= C_FXP_ZERO;
		end

//-- Pipeline Registers --
always @(posedge CLK)
	if (nRST)
		if (pip_ctr == C_PIP_MAX_PCNTR-1) begin
			stage1_reg0 <= stage1_c1_xout;
			stage1_reg1 <= stage1_c1_zout;
			stage1_reg2 <= reg_zin;
			stage2_reg0 <= stage2_fsm_out0;
			stage2_reg1 <= stage2_fsm_out1;
			stage2_reg2 <= stage1_reg2;
			stage2_reg3 <= stage1_reg1;
			stage3_reg0 <= stage2_reg0;
			stage3_reg1 <= stage3_c2_xout;
			stage3_reg2 <= stage3_mult_out;
			stage3_reg3 <= stage2_reg2;
			stage3_reg4 <= stage2_reg3;
			stage4_reg0 <= stage4_c7_zout;
			stage4_reg1 <= stage3_reg0;
			stage4_reg2 <= stage4_sub_out;
			stage4_reg3 <= stage3_reg4;
			stage5_reg0 <= stage5_mult_out;
			stage5_reg1 <= stage4_reg0;
			stage5_reg2 <= stage5_add_out;
			stage5_reg3 <= stage5_c5_yout;
			stage5_reg4 <= stage4_reg2;
			stage5_reg5 <= stage4_reg3;
			end
		else begin
			stage1_reg0 <= stage1_reg0;
			stage1_reg1 <= stage1_reg1;
			stage1_reg2 <= stage1_reg2;
			stage2_reg0 <= stage2_reg0;
			stage2_reg1 <= stage2_reg1;
			stage2_reg2 <= stage2_reg2;
			stage2_reg3 <= stage2_reg3;
			stage3_reg0 <= stage3_reg0;
			stage3_reg1 <= stage3_reg1;
			stage3_reg2 <= stage3_reg2;
			stage3_reg3 <= stage3_reg3;
			stage3_reg4 <= stage3_reg4;
			stage4_reg0 <= stage4_reg0;
			stage4_reg1 <= stage4_reg1;
			stage4_reg2 <= stage4_reg2;
			stage4_reg3 <= stage4_reg3;
			stage5_reg0 <= stage5_reg0;
			stage5_reg1 <= stage5_reg1;
			stage5_reg2 <= stage5_reg2;
			stage5_reg3 <= stage5_reg3;
			stage5_reg4 <= stage5_reg4;
			stage5_reg5 <= stage5_reg5;
			end
	else begin
		stage1_reg0 <= C_FXP_ZERO;
		stage1_reg1 <= C_FXP_ZERO;
		stage1_reg2 <= C_FXP_ZERO;
		stage2_reg0 <= C_FXP_ZERO;
		stage2_reg1 <= C_FXP_ZERO;
		stage2_reg2 <= C_FXP_ZERO;
		stage2_reg3 <= C_FXP_ZERO;
		stage3_reg0 <= C_FXP_ZERO;
		stage3_reg1 <= C_FXP_ZERO;
		stage3_reg2 <= C_FXP_ZERO;
		stage3_reg3 <= C_FXP_ZERO;
		stage3_reg4 <= C_FXP_ZERO;
		stage4_reg0 <= C_FXP_ZERO;
		stage4_reg1 <= C_FXP_ZERO;
		stage4_reg2 <= C_FXP_ZERO;
		stage4_reg3 <= C_FXP_ZERO;
		stage5_reg0 <= C_FXP_ZERO;
		stage5_reg1 <= C_FXP_ZERO;
		stage5_reg2 <= C_FXP_ZERO;
		stage5_reg3 <= C_FXP_ZERO;
		stage5_reg4 <= C_FXP_ZERO;
		stage5_reg5 <= C_FXP_ZERO;
		end

// Pipeline Overflow
always @(posedge CLK)
	if (nRST)
		if (pip_ctr == C_PIP_MAX_PCNTR-1) begin
			stage1_overflow <= stage1_c1_overflow;
			stage2_overflow <= stage1_overflow || stage2_fsm_overflow;
			stage3_overflow <= stage2_overflow || stage3_c2_overflow || stage3_mult_overflow || stage3_c4_overflow;
			stage4_overflow <= stage3_overflow || stage4_c7_overflow || stage4_c3_overflow || stage4_sub_overflow;
			stage5_overflow <= stage4_overflow || stage5_mult_overflow || stage5_add_overflow || stage5_c5_overflow;
			end
		else begin
			stage1_overflow <= stage1_overflow;
			stage2_overflow <= stage2_overflow;
			stage3_overflow <= stage3_overflow;
			stage4_overflow <= stage4_overflow;
			stage5_overflow <= stage5_overflow;
			end
	else begin
		stage1_overflow <= 1'b0;
		stage2_overflow <= 1'b0;
		stage3_overflow <= 1'b0;
		stage4_overflow <= 1'b0;
		stage5_overflow <= 1'b0;
		end

// Data Ready Signal
always @(posedge CLK)
	if (nRST)
		OUT_DATA_READY <= pip_activity[0] && (pip_ctr == C_PIP_MAX_PCNTR-1);
	else
		OUT_DATA_READY <= 1'b0;
		
//-- Busy Signal --
assign BUSY = ((fsm_state == fsm_srun) || (fsm_state == fsm_dout));
		
//-- Output Registers --
always @(posedge CLK)
	if (nRST)
		if (pip_ctr == C_PIP_MAX_PCNTR-1) begin
			Q1_OUT <= stage5_reg5;
			Q2_OUT <= stage6_add_out;
			Q3_OUT <= stage5_reg4;
			DATA_VALID <= stage5_overflow || stage6_c6_overflow || stage6_c9_overflow || stage6_add_overflow;
			end
		else begin
			Q1_OUT <= Q1_OUT;
			Q2_OUT <= Q2_OUT;
			Q3_OUT <= Q3_OUT;
			DATA_VALID <= DATA_VALID;
		end
	else begin
		Q1_OUT <= C_FXP_ZERO;
		Q2_OUT <= C_FXP_ZERO;
		Q3_OUT <= C_FXP_ZERO;
		DATA_VALID <= 1'b0;
		end

endmodule
