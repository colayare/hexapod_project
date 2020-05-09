`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 17.02.2019 18:13:42
// Design Name: 
// Module Name: leading_one_detector
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


module leading_one_detector
#(
  parameter C_FXP_WIDTH     = 16,
  parameter C_LOD_OUT_WIDTH = $clog2(C_FXP_WIDTH)
)(
  input [C_FXP_WIDTH-1:0]       IN,
  output [C_LOD_OUT_WIDTH-1:0]  OUT,
  output                        ALL_ZEROS
);
//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
// Nibble Width = 4-bit
localparam C_NIBBLE_WIDTH   = 4;
// NOR Gate Network Repeats
localparam C_NOR_REPEATS    = C_FXP_WIDTH / C_NIBBLE_WIDTH;
// 
localparam C_POS_WIDTH      = $clog2(C_NOR_REPEATS);
//
localparam C_SUB_VAL        = C_NOR_REPEATS - 1;

//----------------------------------------------
//-- Signals Definition
//----------------------------------------------
wire [C_NOR_REPEATS-1:0]  reduction,
                          inv_reduction,
                          first_one,
                          z0_vector,
                          z1_vector,
                          lead_zeros;
wire [C_POS_WIDTH-1:0]    encoder,
                          last_zero;
wire                      z0,
                          z1;

//----------------------------------------------
//-- Behavioral
//----------------------------------------------
//-- 
genvar  gi;
for (gi=0; gi<C_NOR_REPEATS; gi=gi+1) begin : gen_nor_reduction
    assign reduction[C_NOR_REPEATS-gi-1]  = |IN[C_NIBBLE_WIDTH*(gi+1)-1:C_NIBBLE_WIDTH*gi];
    assign first_one[C_NOR_REPEATS-gi-1]  = inv_reduction[gi] & reduction[gi];
    assign z0_vector[gi]                  = ~((IN[C_NIBBLE_WIDTH*gi+1] && ~IN[C_NIBBLE_WIDTH*gi+2]) || IN[C_NIBBLE_WIDTH*gi+3]);
    assign z1_vector[gi]                  = ~(IN[C_NIBBLE_WIDTH*gi+3] || IN[C_NIBBLE_WIDTH*gi+2]);
  end
  
//-- Parametrized DECODER
generate
//  if (C_NOR_REPEATS == 3) begin
  
//  end
  if (C_NOR_REPEATS == 4) begin
    assign encoder[0] = first_one[1] | first_one[3];
    assign encoder[1] = first_one[2] | first_one[3];
  end
  if (C_NOR_REPEATS == 5) begin
    assign encoder[0] = first_one[1] | first_one[3];
    assign encoder[1] = first_one[2] | first_one[3];
    assign encoder[2] = first_one[4];
  end
endgenerate

assign last_zero      = C_SUB_VAL - encoder;
assign inv_reduction  = ~reduction + {{C_NOR_REPEATS-1{1'b0}},1'b1};  
assign ALL_ZEROS      = ~|reduction;
assign z0             = z0_vector >> encoder;
assign z1             = z1_vector >> encoder;
assign lead_zeros     = {last_zero,z1,z0};

//assign OUT = C_FXP_WIDTH - lead_zeros; // Actual value
assign OUT            = lead_zeros; // For conversor shiftin application


endmodule
