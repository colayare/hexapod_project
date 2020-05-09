`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 17.02.2019 16:23:31
// Design Name: 
// Module Name: fixed_to_float_converter
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


module fixed_to_float_converter
#(
  parameter C_FXP_WIDTH = 16,
  parameter C_FXP_POINT = 12,
  parameter C_FLP_WIDTH = 32
)(
  input signed [C_FXP_WIDTH-1:0]  FXP_NUM,
  input                           CLK,
  input                           nRST,
  input                           TRIGGER,
  output [C_FLP_WIDTH-1:0]        FLP_NUM,
  output                          FLP_ZERO,
  output reg                      VALID
);
//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
localparam C_FLP_EXP_WIDTH    = 8;
localparam C_FLP_MANT_WIDTH   = 23;
localparam C_LOD_OUT_WIDTH    = $clog2(C_FXP_WIDTH);
localparam C_FLP_MANT_OFFSET  = C_FLP_MANT_WIDTH - C_FXP_WIDTH;
localparam C_FLP_EXP_BIAS     = 2**(C_FLP_EXP_WIDTH-1);
localparam C_FXP_INT_OFFSET   = C_FXP_WIDTH - C_FXP_POINT - 2;

//----------------------------------------------
//-- Signals Definition
//----------------------------------------------
wire signed [C_FXP_WIDTH-1:0]     fxp_abs_val;
wire signed [C_FLP_EXP_WIDTH-1:0] flp_exp;
reg [C_FLP_MANT_WIDTH-1:0]        flp_mantisa;
wire [C_LOD_OUT_WIDTH-1:0]        fxp_leading_one;
wire [C_FXP_WIDTH-1:0]            fxp_shifted;
wire                              flp_sign,
                                  fxp_all_zeros;

//----------------------------------------------
//-- Instantiations
//----------------------------------------------
leading_one_detector
#(
  .C_FXP_WIDTH(C_FXP_WIDTH)
) LOD (
  .IN(fxp_abs_val),
  .OUT(fxp_leading_one),
  .ALL_ZEROS() //fxp_all_zeros - NC
);

//----------------------------------------------
//-- Behavioral
//----------------------------------------------
// Sign
assign flp_sign = FXP_NUM[C_FXP_WIDTH-1];

// Calculate Exponent
assign flp_exp = C_FLP_EXP_BIAS + (C_FXP_INT_OFFSET - fxp_leading_one);

// Calculate Mantisa
assign fxp_abs_val = (FXP_NUM[C_FXP_WIDTH-1]) ? -FXP_NUM : FXP_NUM;
assign fxp_shifted = fxp_abs_val << (fxp_leading_one + 1);
//assign flp_mantisa = {fxp_shifted,{C_FLP_MANT_OFFSET{1'b0}}};

always @(posedge CLK)
  if ( ~nRST )
    flp_mantisa <= {C_FLP_MANT_WIDTH{1'b0}};
  else
    flp_mantisa <= {fxp_shifted,{C_FLP_MANT_OFFSET{1'b0}}};

// Calculate Data Valid
always @(posedge CLK)
  if ( ~nRST )
    VALID <= 1'b0;
  else
    VALID <= TRIGGER;

// Output
assign FLP_NUM  = (|FXP_NUM) ? {flp_sign, flp_exp, flp_mantisa} : {C_FLP_WIDTH{1'b0}};
assign FLP_ZERO = |FXP_NUM;

endmodule
