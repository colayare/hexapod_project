`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 19.02.2019 23:04:51
// Design Name: 
// Module Name: FSM_iKinematics_FSMv2
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


module FSM_iKinematics_FSMv2
#(
	//-- Fixed Point Parameters --
	// Fixed Point Data bus width : 16 bit
	parameter integer C_FXP_WIDTH=16,
	// Fixed Point dot ubication from right [INT].[DEC]					
	parameter integer C_FXP_POINT=12,
	//-- Inverse Kinematics Parameters --
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_iK	= 'h09b7,
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_C2	= 'hff1e,	// C2 = - 2L1
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_C3	= 'hffaf,	// C3 = L1^2 - L2^2 - L3^3 
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_Ca8	= 'h62cd, 	// Ca / 8
	parameter signed [C_FXP_WIDTH-1:0]	C_IKN_nL1	= 'hff8f
)(
	//-- SYSTEM INTERFACE --
    input 								nRST,			// Reset active low
    input 								CLK,			// Clock signal
    //-- CONTROL INTERFACE --
    input								START,
    output 								DATA_VALID,
    //-- In/Out Parameters --
    input signed [C_FXP_WIDTH-1:0]		STAGE1_REG0,
    input signed [C_FXP_WIDTH-1:0]		STAGE1_REG2,
    output reg signed [C_FXP_WIDTH-1:0]	STAGE2_REG0,	// Fixed point: D value
    output reg signed [C_FXP_WIDTH-1:0]	STAGE2_REG1		// Fixed point: r value
    );
    
//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
//-- FSM States Definition --
localparam fsm_size = $clog2(12);
localparam C_FXP_ZERO = {C_FXP_WIDTH{1'b0}};
localparam [fsm_size-1:0]	fsm_rset = 0,
							fsm_idle = 1,	// r = C1_CV[0] * iK
							fsm_wtc0 = 2,
							fsm_cal0 = 3,	// r * C2 = r * -2L1
							fsm_wtc1 = 4,
							fsm_cal1 = 5,
							fsm_wtc2 = 6,
							fsm_cal2 = 7,
							fsm_wtc3 = 8,
							fsm_cal3 = 9,
							fsm_wtc4 = 10,
							fsm_cal4 = 11,
							fsm_wtc5 = 12;

//----------------------------------------------
//-- Signals Definitions
//----------------------------------------------
// Reg Vector
reg signed [fsm_size-1:0]		fsm_state,
								fsm_next_state;
reg signed [C_FXP_WIDTH-1:0]	mult_mux_in1,
								mult_mux_in2,
								add_mux_in1,
								add_mux_in2,
								reg_mult_mux_in1,
								reg_mult_mux_in2,
								reg_add_mux_in1,
								reg_add_mux_in2,
								reg_mult,
								reg_add,
								reg_r;
reg								reg_overflow;
// Wire Vector
wire signed [C_FXP_WIDTH-1:0]	mult_out,
								add_out;
wire							mult_overflow,
								add_overflow;

//----------------------------------------------
//-- Instantiations
//----------------------------------------------
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_Adder (
	.S_NUM1(reg_add_mux_in1),
	.S_NUM2(reg_add_mux_in2),
	.S_OPE(1'b0),
	.S_RESULT(add_out),
	.S_OF_FLAG(add_overflow)
    );
fxp_multiplier #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_Multiplier (
	.S_NUM1(reg_mult_mux_in1),
	.S_NUM2(reg_mult_mux_in2),
	.S_RESULT(mult_out),
	.S_OF_FLAG(mult_overflow)
    );

//----------------------------------------------
//-- Behavioral
//----------------------------------------------
// FSM State Register
always @(posedge CLK)
	if (nRST)
		fsm_state <= fsm_next_state;
	else
		fsm_state <= {fsm_size{1'b0}};

// FSM Next State Logic
always @(*)
	case (fsm_state)
		fsm_rset: fsm_next_state = fsm_idle;
		fsm_idle: fsm_next_state = (START) ? fsm_wtc0 : fsm_idle;
		fsm_wtc0: fsm_next_state = fsm_cal0;	// Wait
		fsm_cal0: fsm_next_state = fsm_wtc1;
		fsm_wtc1: fsm_next_state = fsm_cal1;	// Wait
		fsm_cal1: fsm_next_state = fsm_wtc2;
		fsm_wtc2: fsm_next_state = fsm_cal2;	// Wait
		fsm_cal2: fsm_next_state = fsm_wtc3;
		fsm_wtc3: fsm_next_state = fsm_cal3;	// Wait
		fsm_cal3: fsm_next_state = fsm_wtc4;
		fsm_wtc4: fsm_next_state = fsm_cal4;	// Wait
		fsm_cal4: fsm_next_state = fsm_wtc5;
		fsm_wtc5: fsm_next_state = fsm_idle;	// Wait
		default: fsm_next_state = fsm_idle;
	endcase

// FSM Output Logic
always @(*)
	case (fsm_state)
		fsm_rset: begin		// MULT = rk * ik, ADD = 0
			mult_mux_in1 	= STAGE1_REG0;
			mult_mux_in2 	= C_IKN_iK;
			add_mux_in1 	= C_FXP_ZERO;
			add_mux_in2		= C_FXP_ZERO;
			end
		fsm_idle: begin		// MULT = rk * ik, ADD = 0 
			mult_mux_in1 	= STAGE1_REG0;
			mult_mux_in2 	= C_IKN_iK;
			add_mux_in1 	= C_FXP_ZERO;
			add_mux_in2		= C_FXP_ZERO;
			end
		fsm_wtc0: begin		// Wait
			mult_mux_in1 	= STAGE1_REG0;
			mult_mux_in2 	= C_IKN_iK;
			add_mux_in1 	= C_FXP_ZERO;
			add_mux_in2		= C_FXP_ZERO;
			end
		fsm_cal0: begin		// MULT = r*C2, ADD = 0
			mult_mux_in1 	= reg_mult;
			mult_mux_in2 	= C_IKN_C2;
			add_mux_in1 	= C_FXP_ZERO;
			add_mux_in2		= C_FXP_ZERO;
			end
		fsm_wtc1: begin		// MULT = r*C2, ADD = 0
			mult_mux_in1 	= reg_mult;
			mult_mux_in2 	= C_IKN_C2;
			add_mux_in1 	= C_FXP_ZERO;
			add_mux_in2		= C_FXP_ZERO;
			end
		fsm_cal1: begin		// MULT = r**2, ADD = r*C2 + C3
			mult_mux_in1 	= reg_r;
			mult_mux_in2 	= reg_r;
			add_mux_in1 	= reg_mult;
			add_mux_in2		= C_IKN_C3;
			end
		fsm_wtc2: begin		// MULT = r**2, ADD = r*C2 + C3
			mult_mux_in1 	= reg_r;
			mult_mux_in2 	= reg_r;
			add_mux_in1 	= reg_mult;
			add_mux_in2		= C_IKN_C3;
			end
		fsm_cal2: begin		// MULT = z**2, ADD = r**2 + r*C2 + c3
			mult_mux_in1 	= STAGE1_REG2;
			mult_mux_in2 	= STAGE1_REG2;
			add_mux_in1 	= reg_mult;
			add_mux_in2		= reg_add;
			end
		fsm_wtc3: begin		// MULT = z**2, ADD = r**2 + r*C2 + c3
			mult_mux_in1 	= STAGE1_REG2;
			mult_mux_in2 	= STAGE1_REG2;
			add_mux_in1 	= reg_mult;
			add_mux_in2		= reg_add;
			end
		fsm_cal3: begin		// MULT = (r**2 + r*C2 + c3) * Ca/8, ADD = r**2 + r*C2 + c3 + z**2
			mult_mux_in1 	= reg_add;
			mult_mux_in2 	= C_IKN_Ca8;
			add_mux_in1 	= reg_mult;
			add_mux_in2		= reg_add;
			end
		fsm_wtc4: begin		// MULT = (r**2 + r*C2 + c3) * Ca/8, ADD = r**2 + r*C2 + c3 + z**2
			mult_mux_in1 	= reg_add;
			mult_mux_in2 	= C_IKN_Ca8;
			add_mux_in1 	= reg_mult;
			add_mux_in2		= reg_add;
			end
		fsm_cal4: begin		// MULT = (z**2 + r**2 + r*C2 + c3) * Ca/8, ADD = r - L1
			mult_mux_in1 	= reg_add <<< 3;
			mult_mux_in2 	= C_IKN_Ca8;
			add_mux_in1 	= reg_r; 
			add_mux_in2		= C_IKN_nL1;
			end
		fsm_wtc5: begin		// MULT = (z**2 + r**2 + r*C2 + c3) * Ca/8, ADD = r - L1
			mult_mux_in1 	= reg_add <<< 3;
			mult_mux_in2 	= C_IKN_Ca8;
			add_mux_in1 	= reg_r; 
			add_mux_in2		= C_IKN_nL1;
			end
		default: begin		// MULT = rk * ik, ADD = 0
			mult_mux_in1 	= STAGE1_REG0;
			mult_mux_in2 	= C_IKN_iK;
			add_mux_in1 	= C_FXP_ZERO;
			add_mux_in2		= C_FXP_ZERO;
			end
	endcase

always @(posedge CLK)
	if (nRST) begin
		reg_mult_mux_in1	<= mult_mux_in1;
		reg_mult_mux_in2	<= mult_mux_in2;
		reg_add_mux_in1		<= add_mux_in1;
		reg_add_mux_in2		<= add_mux_in2;
		end
	else begin
		reg_mult_mux_in1	<= C_FXP_ZERO;
		reg_mult_mux_in2	<= C_FXP_ZERO;
		reg_add_mux_in1		<= C_FXP_ZERO;
		reg_add_mux_in2		<= C_FXP_ZERO;
		end

// Multiplication Register
always @(posedge CLK)
	if (nRST)
		reg_mult <= mult_out;
	else
		reg_mult <= C_FXP_ZERO;

// Addition Register
always @(posedge CLK)
	if (nRST)
		reg_add <= add_out;
	else
		reg_add <= C_FXP_ZERO;

//// r out Register
always @(posedge CLK)
	if (nRST)
		if (fsm_state == fsm_cal0)	// The first calculation is ik * (k*r)
			reg_r <= mult_out;
		else
			reg_r <= reg_r;
	else
		reg_r <= C_FXP_ZERO;

//-- Overflow Control --
always @(posedge CLK)
	if (nRST)
		if (START)
			reg_overflow <= 1'b0;
		else
			reg_overflow <= add_overflow || mult_overflow;
	else
		reg_overflow <= 1'b0;

assign DATA_VALID = reg_overflow;

// D out Register
always @(posedge CLK)
	if (nRST)
		if (fsm_state == fsm_wtc5) begin
			STAGE2_REG0 <= mult_out;	// <<< 3
			STAGE2_REG1 <= add_out;
			end
		else begin
			STAGE2_REG0 <= STAGE2_REG0;
			STAGE2_REG1 <= STAGE2_REG1;
			end
	else begin
		STAGE2_REG0 <= C_FXP_ZERO;
		STAGE2_REG1 <= C_FXP_ZERO;
		end

endmodule
