`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 22.01.2019 15:10:20
// Design Name: 
// Module Name: fxp_cordic_lh
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


module fxp_cordic_lv
#(  parameter C_FXP_WIDTH=16,						// Fixed Point Data bus width : 16 bit
	parameter C_FXP_POINT=12,						// Fixed Point dot ubication from right [INT].[DEC]
	parameter C_COR_ITER=16,						// CORDIC Iterations	
    parameter C_MEMPTR_SIZE=$clog2(C_COR_ITER)		// ROM Memory ATAN LUT Address Bus Size
)(
	// SYSTEM INTERFACE
    input 								nRST,		// Reset active low
    input 								CLK,		// Clock signal
    // CONTROL INTERFACE
    input 								START,		// Start active high
    output  							VALID_CO,	// Valid operation carry-out active high
    output reg 							DATA_RDY,	// Data ready active high
    // CORDIC INPUTS & OUTPUTS INTERFACE
    input signed [C_FXP_WIDTH-1:0]		X_IN,		// Fixed Point CORDIC X input
    input signed [C_FXP_WIDTH-1:0] 		Y_IN,		// Fixed Point CORDIC Y input
    output reg signed [C_FXP_WIDTH-1:0] Z_OUT		// Fixed Point CORDIC Z output
    );

//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
// Max iteration value
localparam [C_MEMPTR_SIZE-1:0]	C_COR_MAXITER 	= C_COR_ITER - 1;
// Fixed Point Zero
localparam [C_FXP_WIDTH-1:0]	C_FXP_ZERO 		= {C_FXP_WIDTH{1'b0}};
// Fixed Point One
localparam [C_FXP_WIDTH-1:0]	C_FXP_ONE 		= {{C_FXP_WIDTH-C_FXP_POINT-1{1'b0}},1'b1,{C_FXP_POINT{1'b0}}};
// CORDIC FSM States Definition
localparam [2:0]	rset    = 0,    				// Reset
                    idle    = 1,    				// Idle
                    imem	= 2,					// Init Memory
                    iter    = 3,    				// Iterating
                    drdy    = 4;    				// Data Ready

//----------------------------------------------
//-- Signals Definitions
//----------------------------------------------
// Reg Vectors
reg signed [C_FXP_WIDTH-1:0]	reg_xin,        	// Register X input
                				reg_yin,        	// Register Y input
              		  			reg_zin;        	// Register Z input
reg [2:0]						state, 				// FSM State Register
								state_next;			// FSM Next State Register
reg [C_MEMPTR_SIZE-1:0]			iteration,			// Iteration Register
								nextiter;			// Next Iteration Register
// Wire Vectors
wire signed [C_FXP_WIDTH-1:0]	mux_xin,			// Input Mux X
                				mux_yin,			// Input Mux Y
                				mux_zin,			// Input Mux Z
                				mux_zout,			// Output Mux Z
                				yadderout,			// Adder Y Out
                				zadderout,			// Adder Z Out
                				shift_x,			// Y Register Shifting
                				shift_z;			// Z Register Shifting
// Reg
reg 							mux_sel, 			// Input Mux Selectors
								reg_overflow;
// Wire
wire							sign_ope,			// Addition/Substraction bit select
								addery_flag,		// Adder Y Operation Result Flag
								adderz_flag;		// Adder Z Operation Result Flag

//----------------------------------------------
//-- Instantiations
//----------------------------------------------
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
	.S_NUM2(shift_z),
	.S_OPE(~sign_ope),
	.S_RESULT(zadderout),
	.S_OF_FLAG(adderz_flag)
);

//----------------------------------------------
//-- Architecture
//----------------------------------------------
//-- Input Logic --  
// Input Multiplexer for XYZ-input registers
assign mux_xin = mux_sel ? reg_xin 		: X_IN;
assign mux_yin = mux_sel ? yadderout	: Y_IN;
assign mux_zin = mux_sel ? zadderout	: C_FXP_ZERO;

// XYZ-input registers
always @(posedge CLK)
	if (nRST) begin
		reg_xin       <= mux_xin;
		reg_yin       <= mux_yin;
		reg_zin       <= mux_zin;
		end
	else begin
		reg_xin       <= {C_FXP_WIDTH{1'b0}};
		reg_yin       <= {C_FXP_WIDTH{1'b0}};
		reg_zin       <= {C_FXP_WIDTH{1'b0}};
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
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		idle : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		imem : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		iter : begin
			mux_sel = 1'b1;
			DATA_RDY = 1'b0;
			nextiter = iteration + 1;
			end
		drdy : begin
			mux_sel = 1'b1;
			DATA_RDY = 1'b1;
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
		default : begin
			mux_sel = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_MEMPTR_SIZE{1'b0}};
			end
	endcase

//-- CORDIC Operations --
// Shift Rotations
assign shift_x = reg_xin >>> iteration;
assign shift_z = C_FXP_ONE >>> iteration;

// Mux Mode Selector
assign sign_ope = !(reg_yin[C_FXP_WIDTH-1]^reg_xin[C_FXP_WIDTH-1]);

//-- Overflow Control --
always @(posedge CLK)
	if (nRST)
		if (START)
			reg_overflow <= 1'b0;
		else
			reg_overflow <= reg_overflow || addery_flag || adderz_flag;
	else
		reg_overflow <= 1'b0;
		
assign VALID_CO = reg_overflow;

//-- Output Logic --
// Output Mux
assign mux_zout = (iteration == C_COR_MAXITER) ? zadderout : Z_OUT;

// Output Registers
always @(posedge CLK)
	if (nRST)
		Z_OUT <= mux_zout;
	else
		Z_OUT <= {C_FXP_WIDTH{1'b0}};

endmodule
