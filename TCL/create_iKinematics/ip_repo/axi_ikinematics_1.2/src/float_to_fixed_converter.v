`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.02.2019 11:26:24
// Design Name: 
// Module Name: float_to_fixed_converter
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


module float_to_fixed_converter
#(
	parameter C_FXP_WIDTH = 16,
	parameter C_FXP_POINT = 12,
	parameter C_FLP_WIDTH = 32
)(
	input [C_FLP_WIDTH-1:0]			FLP_NUM,
	output signed [C_FXP_WIDTH-1:0]	FXP_NUM,
	output							OUT_RANGE,
	output							FLP_ZERO
    );
//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
localparam C_FLP_EXP_WIDTH = 8;
localparam C_FLP_MANT_WIDTH = 23;
localparam C_LOD_OUT_WIDTH = $clog2(C_FXP_WIDTH);
localparam C_FLP_EXP_BIAS = 2**(C_FLP_EXP_WIDTH-1);
localparam C_FXP_INT_OFFSET = C_FXP_WIDTH - C_FXP_POINT - 2;
localparam C_FXP_INT_WIDTH = C_FXP_WIDTH - C_FXP_POINT;
localparam C_FLP_EXP_LIMIT = 2**(C_FXP_INT_WIDTH-1);

//----------------------------------------------
//-- Signals Definition
//----------------------------------------------
wire [C_FLP_EXP_WIDTH-1:0]		flp_exp;
wire [C_FLP_MANT_WIDTH:0]		flp_mantisa,
								flp_shifted_mantisa;
wire [C_LOD_OUT_WIDTH-1:0]		fxp_leading_one;
wire signed [C_FXP_WIDTH-1:0]	fxp_shifted;
wire signed [C_FXP_WIDTH-1:0]	fxp_signed;

//----------------------------------------------
//-- Behavioral
//----------------------------------------------
// Calculate Shift Amount
assign flp_exp = FLP_NUM[C_FLP_WIDTH-1:C_FLP_WIDTH-C_FLP_EXP_WIDTH-1];
assign flp_mantisa = {1'b1,FLP_NUM[0+:C_FLP_MANT_WIDTH]};
assign fxp_leading_one = C_FLP_EXP_BIAS + (C_FXP_INT_OFFSET - flp_exp);

// Extract Mantisa
assign flp_shifted_mantisa = flp_mantisa >> fxp_leading_one;
assign fxp_shifted = flp_shifted_mantisa[C_FLP_MANT_WIDTH-:C_FXP_WIDTH];

// Operate Sign and Output Signed Fixed-Point
assign fxp_signed = (FLP_NUM[C_FLP_WIDTH-1]) ? -fxp_shifted : fxp_shifted;
assign FXP_NUM = fxp_signed;
assign FLP_ZERO = ~|FLP_NUM;

// Calculate Out of Range
assign OUT_RANGE = (fxp_leading_one > C_FLP_EXP_LIMIT) || (FLP_NUM[C_FLP_WIDTH-1] ^ FXP_NUM[C_FXP_WIDTH-1]);

endmodule
