`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.01.2019 13:25:41
// Design Name: 
// Module Name: fxp_cordic_hv
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


module fxp_cordic_hv
#(  
    parameter C_FXP_WIDTH   = 16,						// Fixed Point Data bus width : 16 bit
    parameter C_FXP_POINT   = 12,						// Fixed Point dot ubication from right [INT].[DEC]
    parameter C_COR_ITER    = 16,						// CORDIC positive iterations
    parameter C_COR_N_ITER  = 2,						// CORDIC non-positive iterations
    // ROM Memory ATAN LUT Address Bus Size
    parameter C_ITER_SIZE   = $clog2(C_COR_ITER+C_COR_N_ITER)		
)(	
	// SYSTEM INTERFACE
    input 								              nRST,		  // Reset active low
    input 								              CLK,		  // Clock signal
    // CONTROL INTERFACE
    input 								              START,		// Start active high
    output  							              VALID_CO,	// Valid operation carry-out active high
    output reg 							            DATA_RDY,	// Data ready active high
    // CORDIC INPUTS & OUTPUTS INTERFACE
    input signed [C_FXP_WIDTH-1:0]		  X_IN,			// Fixed Point CORDIC X input
    input signed [C_FXP_WIDTH-1:0] 		  Y_IN,			// Fixed Point CORDIC Y input
    output reg signed [C_FXP_WIDTH-1:0]	X_OUT			// Fixed Point CORDIC X output
    );

//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
// Max iteration value
localparam [C_ITER_SIZE-1:0]	C_COR_MAXITER 	= C_COR_ITER + C_COR_N_ITER - 1;
// Fixed Point Zero
localparam [C_FXP_WIDTH-1:0]	C_FXP_ZERO 		= {C_FXP_WIDTH{1'b0}};
// Fixed Point One
localparam [C_FXP_WIDTH-1:0]	C_FXP_ONE 		= {{C_FXP_WIDTH-C_FXP_POINT-1{1'b0}},1'b1,{C_FXP_POINT{1'b0}}};
// CORDIC FSM States Definition
localparam [2:0]	  rset    = 0, 	// Reset
                    idle    = 1, 	// Idle
                    imem	  = 2, 	// Init Memory
                    iter    = 3, 	// Iterating
                    drdy    = 4; 	// Data Ready

//----------------------------------------------
//-- Signals Definitions
//----------------------------------------------
// Reg Vectors
reg signed [C_FXP_WIDTH-1:0]	reg_xin,        // Register X input
                              reg_yin,        // Register Y input
                              shift_x,			  // X Register Shifting 1
                              shift_y;			  // Y Register Shifting 1
reg [2:0]						          state, 				  // FSM State Register
								              state_next;			// FSM Next State Register
reg [C_ITER_SIZE-1:0]			    iteration,			// Iteration Register
                              nextiter;			  // Next Iteration Register
// Wire Vectors
wire signed [C_FXP_WIDTH-1:0]	mux_xin,			  // Input Mux X
                              mux_yin,			  // Input Mux Y
                              adder2_x_mux,		
                              adder2_y_mux,		
                              mux_xout,			  // Output Mux X
                              mux_yout,			  // Output Mux Y
                              adder1_x_out,	  // Adder X Out
                              adder1_y_out,	  // Adder Y Out
                              adder2_x_out,	  // Adder X Out
                              adder2_y_out;	  // Adder Y Out
// Reg
reg 							            mux_sel, 			  // Input Mux Selectors
                              reg_overflow;		
// Wire
wire							            sign_ope,			  // Addition/Substraction bit select
                              iterSign,			
                              adder1_x_flag,  // Adder X Operation Result Flag
                              adder1_y_flag,	// Adder Y Operation Result Flag
                              adder2_x_flag,	// Adder X Operation Result Flag
                              adder2_y_flag;	// Adder Y Operation Result Flag

//----------------------------------------------
//-- Instantiations
//----------------------------------------------
// Floating Point X1 Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_X1_ADDER (
	.S_NUM1(reg_xin),
	.S_NUM2(reg_yin),
	.S_OPE(~sign_ope),
	.S_RESULT(adder1_x_out),
	.S_OF_FLAG(adder1_x_flag)
);
// Floating Point Y1 Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_Y1_ADDER (
	.S_NUM1(reg_yin),
	.S_NUM2(reg_xin),
	.S_OPE(~sign_ope),
	.S_RESULT(adder1_y_out),
	.S_OF_FLAG(adder1_y_flag)
);
// Floating Point X2 Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_X2_ADDER (
	.S_NUM1(adder2_x_mux),
	.S_NUM2(shift_y),
	.S_OPE(sign_ope^~iterSign),
	.S_RESULT(adder2_x_out),
	.S_OF_FLAG(adder2_x_flag)
);
// Floating Point Y2 Adder
fxp_adder #(
	.C_FXP_LENGTH(C_FXP_WIDTH),
	.C_FXP_POINT(C_FXP_POINT)
) FXP_Y2_ADDER (
	.S_NUM1(adder2_y_mux),
	.S_NUM2(shift_x),
	.S_OPE(sign_ope^~iterSign),
	.S_RESULT(adder2_y_out),
	.S_OF_FLAG(adder2_y_flag)
);

//----------------------------------------------
//-- Architecture
//----------------------------------------------
//-- Input Logic --  
// Input Multiplexer for XYZ-input registers
assign mux_xin = mux_sel ? adder2_x_out : X_IN;
assign mux_yin = mux_sel ? adder2_y_out : Y_IN;

// XYZ-input registers
always @(posedge CLK)
	if (~nRST) begin
		reg_xin       <= {C_FXP_WIDTH{1'b0}};
		reg_yin       <= {C_FXP_WIDTH{1'b0}};
		end
	else begin
		reg_xin       <= mux_xin;
		reg_yin       <= mux_yin;
		end

//-- Iteration Control --
// Iteration Register
always @(posedge CLK) 
	if (~nRST)
		iteration	<= {C_ITER_SIZE{1'b0}};
	else
		iteration <= nextiter;

//-- FSM --
// CORDIC FSM Register
always @(posedge CLK)
	if (~nRST)
		state <= rset;
	else
		state <= state_next;

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
			mux_sel  = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_ITER_SIZE{1'b0}};
			end
		idle : begin
			mux_sel  = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_ITER_SIZE{1'b0}};
			end
		imem : begin
			mux_sel  = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_ITER_SIZE{1'b0}};
			end
		iter : begin
			mux_sel  = 1'b1;
			DATA_RDY = 1'b0;
			nextiter = iteration + 1;
			end
		drdy : begin
			mux_sel  = 1'b1;
			DATA_RDY = 1'b1;
			nextiter = {C_ITER_SIZE{1'b0}};
			end
		default : begin
			mux_sel  = 1'b0;
			DATA_RDY = 1'b0;
			nextiter = {C_ITER_SIZE{1'b0}};
			end
	endcase

//-- CORDIC Operations --
// Mux Mode Selector
assign sign_ope = reg_yin[C_FXP_WIDTH-1]^reg_xin[C_FXP_WIDTH-1];

// Iteration Sign
assign iterSign = iteration <= C_COR_N_ITER;

// Shift Rotations
always @(*)
	if (iterSign) begin
		shift_x = reg_xin >>> (C_COR_N_ITER + 2 - iteration);
		shift_y = reg_yin >>> (C_COR_N_ITER + 2 - iteration);
		end
	else begin
		shift_x = reg_xin >>> (iteration - C_COR_N_ITER);
		shift_y = reg_yin >>> (iteration - C_COR_N_ITER);
		end

// Adder 2 Input Multiplexer
assign adder2_x_mux = (iterSign) ? adder1_x_out : reg_xin;
assign adder2_y_mux = (iterSign) ? adder1_y_out : reg_yin;

//-- Overflow Control --
always @(posedge CLK)
	if (~nRST)
		reg_overflow <= 1'b0;
	else
		if (START)
			reg_overflow <= 1'b0;
		else
			reg_overflow <= reg_overflow || adder1_x_flag || adder2_x_flag || adder1_y_flag || adder2_y_flag;

assign VALID_CO = reg_overflow;

//-- Output Logic --
// Output Mux
assign mux_xout = (iteration == C_COR_MAXITER) ? adder2_x_out : X_OUT;

// Output Registers
always @(posedge CLK)
	if (~nRST)
		X_OUT <= {C_FXP_WIDTH{1'b0}};
	else
		X_OUT <= mux_xout;

endmodule
