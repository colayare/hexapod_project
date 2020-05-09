`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.01.2019 23:23:55
// Design Name: 
// Module Name: cordic_c_exp
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


module cordic_c_exp
#(  parameter C_FXP_WIDTH=16,						// Fixed Point Data bus width : 16 bit
	parameter C_FXP_POINT=12,						// Fixed Point dot ubication from right [INT].[DEC]
	parameter C_COR_ITER=16,						// CORDIC Iterations	
	parameter C_COR_N_ITER=2,						// CORDIC non-positive iterations
	// ROM Memory ATAN LUT Address Bus Size
    parameter C_MEMPTR_SIZE=$clog2(C_COR_ITER+C_COR_N_ITER)		
)(	
	// SYSTEM INTERFACE
    input 								nRST,		// Reset active low
    input 								CLK,		// Clock signal
    // CONTROL INTERFACE
    input 								START,		// Start active high
    input 								MODE,		// CORDIC Mode - 0 : Rotation, 1: Vectoring
    output  							VALID_CO,	// Valid operation carry-out active high
    output reg 							DATA_RDY,	// Data ready active high
    // CORDIC INPUTS & OUTPUTS INTERFACE
    input signed [C_FXP_WIDTH-1:0]		X_IN,		// Fixed Point CORDIC X input
    input signed [C_FXP_WIDTH-1:0] 		Y_IN,		// Fixed Point CORDIC Y input
    input signed [C_FXP_WIDTH-1:0] 		Z_IN,		// Fixed Point CORDIC Z input
    output reg signed [C_FXP_WIDTH-1:0]	X_OUT,		// Fixed Point CORDIC X output
    output reg signed [C_FXP_WIDTH-1:0] Y_OUT,		// Fixed Point CORDIC Y output
    output reg signed [C_FXP_WIDTH-1:0] Z_OUT,		// Fixed Point CORDIC Z output
    // MEMORY INTERFACE
    input [C_FXP_WIDTH-1:0] 			LUT_OUT,	// Fixed Point CORDIC ATAN LUT input
    output reg [C_MEMPTR_SIZE-1:0]		MEM_PTR		// Memory pointer 
    );

//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
// Max iteration value
localparam [C_MEMPTR_SIZE-1:0]	C_COR_MAXITER = C_COR_ITER + C_COR_N_ITER - 1;
// Max LUT Access Pointer
localparam [C_MEMPTR_SIZE-1:0] C_LUT_MAX_PTR = C_COR_ITER - 3;
// CORDIC FSM States Definition
localparam [2:0]	rset    = 0,    				// Reset
                    idle    = 1,    				// Idle
                    imem	= 2,					// Init Memory
                    iter    = 3,    				// Iterating
                    drdy    = 4;    				// Data Ready

//----------------------------------------------
//-- LSignals Definitions
//----------------------------------------------
// Reg Vectors
reg signed [C_FXP_WIDTH-1:0]	reg_xin,        	// Register X input
                				reg_yin,        	// Register Y input
              		  			reg_zin,        	// Register Z input
              		  			reg_lutout,     	// Register ATAN LUT input
                				shift_x,			// X Register Shifting
                				shift_y;			// Y Register Shifting
reg [2:0]						state, 				// FSM State Register
								state_next;			// FSM Next State Register
reg [C_MEMPTR_SIZE-1:0]			iteration,			// Iteration Register
								nextiter;			// Next Iteration Register
// Wire Vectors
wire signed [C_FXP_WIDTH-1:0]	mux_xin,			// Input Mux X
                				mux_yin,			// Input Mux Y
                				mux_zin,			// Input Mux Z
                				mux_xout,			// Output Mux X
                				mux_yout,			// Output Mux Y
                				mux_zout,			// Output Mux Z
                				xadderout,			// Adder X Out
                				yadderout,			// Adder Y Out
                				zadderout;			// Adder Z Out
// Reg
reg 							mux_sel, 			// Input Mux Selectors
								reg_overflow;
// Wire
wire							sign_ope,			// Addition/Substraction bit select
								sign_iter,			
								adderx_flag,		// Adder X Operation Result Flag
								addery_flag,		// Adder Y Operation Result Flag
								adderz_flag;		// Adder Z Operation Result Flag

//----------------------------------------------
//-- Instantiations
//----------------------------------------------
// Floating Point X Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_X_ADDER (
	.S_NUM1(reg_xin),
	.S_NUM2(shift_y),
	.S_OPE(~sign_ope),
	.S_RESULT(xadderout),
	.S_OF_FLAG(adderx_flag)
);
// Floating Point Y Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_Y_ADDER (
	.S_NUM1(reg_yin),
	.S_NUM2(shift_x),
	.S_OPE(sign_ope),
	.S_RESULT(yadderout),
	.S_OF_FLAG(addery_flag)
);
// Floating Point Z Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_Z_ADDER (
	.S_NUM1(reg_zin),
	.S_NUM2(reg_lutout),
	.S_OPE(~sign_ope),
	.S_RESULT(zadderout),
	.S_OF_FLAG(adderz_flag)
);

//----------------------------------------------
//-- Architecture
//---------------------------------------------- 
//-- Input Logic --
// Input Multiplexer for XYZ-input registers
assign mux_xin = mux_sel ? xadderout : X_IN;
assign mux_yin = mux_sel ? yadderout : Y_IN;
assign mux_zin = mux_sel ? zadderout : Z_IN;

// XYZ-input registers
always @(posedge CLK)
	if (nRST) begin
		reg_xin       <= mux_xin;
		reg_yin       <= mux_yin;
		reg_zin       <= mux_zin;
		reg_lutout    <= LUT_OUT;
		end
	else begin
		reg_xin       <= {C_FXP_WIDTH{1'b0}};
		reg_yin       <= {C_FXP_WIDTH{1'b0}};
		reg_zin       <= {C_FXP_WIDTH{1'b0}};
		reg_lutout    <= {C_FXP_WIDTH{1'b0}};
		end

//-- Iteration Control --
// Iteration Register
always @(posedge CLK) 
	if (nRST)
		iteration <= nextiter;
	else
		iteration	<= {C_MEMPTR_SIZE{1'b0}};

//-- FSM --
// CORDIC FSM Register
always @(posedge CLK)
	if (nRST)
		state <= state_next;
	else
		state <= rset;

// FSM Next State Logic
always @(*)
	case (state)
		rset : state_next = (nRST) ? idle : rset;
		idle : state_next = (START) ? imem : idle;
		imem : state_next = iter;
		iter : state_next = (iteration == C_COR_MAXITER) ? drdy : iter;
		drdy : state_next = idle;
		default : state_next = idle;
	endcase
	
// FSM Output
always @(*)
	case (state)
		rset : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			MEM_PTR = {C_MEMPTR_SIZE{1'b0}};
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		idle : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			MEM_PTR = (START) ? 1 : 0;
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		imem : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			MEM_PTR = 2;
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		iter : begin
			mux_sel = 1'b1;
			DATA_RDY = 1'b0;
			MEM_PTR = (iteration<C_LUT_MAX_PTR) ? iteration + 3 : {C_MEMPTR_SIZE{1'b0}};
			nextiter = iteration + 1;
			end
		drdy : begin
			mux_sel = 1'b1;
			DATA_RDY = 1'b1;
			MEM_PTR = {C_MEMPTR_SIZE{1'b0}};
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		default : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			MEM_PTR = {C_MEMPTR_SIZE{1'b0}};
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
	endcase

//-- CORDIC Operations --
// Shift Rotations
always @(*)
	if (sign_iter) begin
		shift_x = reg_xin <<< (C_COR_N_ITER - iteration);
		shift_y = reg_yin <<< (C_COR_N_ITER - iteration);
		end
	else begin
		shift_x = reg_xin >>> (iteration - C_COR_N_ITER);
		shift_y = reg_yin >>> (iteration - C_COR_N_ITER);
		end

// Mux Mode Selector
assign sign_ope = MODE ? !reg_yin[C_FXP_WIDTH-1] : reg_zin[C_FXP_WIDTH-1];

// Iteration Sign
assign sign_iter = iteration <= C_COR_N_ITER;

//-- Overflow Control --
always @(posedge CLK)
	if (nRST)
		if (START)
			reg_overflow <= 1'b0;
		else
			reg_overflow <= reg_overflow || adderx_flag || addery_flag || adderz_flag;
	else
		reg_overflow <= 1'b0;

assign VALID_CO = reg_overflow;

//-- Output Logic --
// Output Mux
assign mux_xout = (iteration == C_COR_MAXITER) ? xadderout : X_OUT;
assign mux_yout = (iteration == C_COR_MAXITER) ? yadderout : Y_OUT;
assign mux_zout = (iteration == C_COR_MAXITER) ? zadderout : Z_OUT;

// Output Registers
always @(posedge CLK)
	if (nRST) begin
		X_OUT <= mux_xout;
		Y_OUT <= mux_yout;
		Z_OUT <= mux_zout;
		end
	else begin
		X_OUT <= {C_FXP_WIDTH{1'b0}};
		Y_OUT <= {C_FXP_WIDTH{1'b0}};
		Z_OUT <= {C_FXP_WIDTH{1'b0}};
		end

endmodule
