`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 19.01.2019 19:43:33
// Design Name: 
// Module Name: fxp_adder
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


module fxp_adder #(
  parameter C_FXP_LENGTH  = 16,
  parameter C_FXP_POINT   = 12
)(
  input signed [C_FXP_LENGTH-1:0]       S_NUM1,
                                        S_NUM2,
  input                                 S_OPE,
  output reg signed [C_FXP_LENGTH-1:0]  S_RESULT,
  output                                S_OF_FLAG
);
    
localparam INT_WIDTH  = C_FXP_LENGTH - C_FXP_POINT;
localparam DEC_WIDTH  = C_FXP_POINT;
localparam [C_FXP_LENGTH-1:0] MAX_POS = {1'b0,{INT_WIDTH-1{1'b1}},{DEC_WIDTH{1'b1}}};
localparam [C_FXP_LENGTH-1:0] MAX_NEG = {1'b1,{C_FXP_LENGTH-1{1'b0}}};
    
wire signed [C_FXP_LENGTH:0] s_add;
wire [1:0] overflow;

assign s_add      = (S_OPE) ? S_NUM1 - S_NUM2 : S_NUM1 + S_NUM2;
assign overflow   = s_add[C_FXP_LENGTH:C_FXP_LENGTH-1];
assign S_OF_FLAG  = s_add[C_FXP_LENGTH] ^ s_add[C_FXP_LENGTH-1];

always @(*) begin
  case (overflow)
    2'b01 : S_RESULT    = MAX_POS;
    2'b10 : S_RESULT    = MAX_NEG;
    default : S_RESULT  = s_add[C_FXP_LENGTH-1:0];
  endcase
  end
    
endmodule
