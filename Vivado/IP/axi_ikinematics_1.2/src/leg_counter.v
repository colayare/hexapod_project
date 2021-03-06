`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.09.2019 20:32:51
// Design Name: 
// Module Name: leg_counter
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


module leg_counter #(
  parameter integer N_LEGS      = 6,
  parameter integer N_LEGS_SIZE = $clog2(N_LEGS)
) (
  input wire                    CLK,
  input wire                    nRST,
  input wire [2:0]              CTR_MODE,
  input wire                    SET,
  input wire                    TRIGGER,
  input wire [N_LEGS_SIZE-1:0]  LEG_IN_SELECT,
  output reg                    INVALID_SELECT,
  output reg [N_LEGS_SIZE-1:0]  LEG_OUT_SELECT
);
    //--
    reg [N_LEGS_SIZE-1:0]   leg_ctr,
                in_selection;
    //--
    always @(*)
      if ( LEG_IN_SELECT < N_LEGS )begin
        in_selection <= LEG_IN_SELECT;
        INVALID_SELECT <= 1'b0;       
      end else begin
        in_selection <= {N_LEGS_SIZE{1'b0}};
        INVALID_SELECT <= 1'b1;
      end
      
    //--
    always @(posedge CLK)
      if ( ~nRST )
        leg_ctr <= {N_LEGS_SIZE{1'b0}};
      else
        case ( {SET,TRIGGER} )
          0 : leg_ctr <= leg_ctr;
          1 : leg_ctr <= (leg_ctr < N_LEGS - 1) ? leg_ctr + 1 : {N_LEGS_SIZE{1'b0}};
          2 : leg_ctr <= in_selection;
          default : leg_ctr <= in_selection;
        endcase
    
    //--
    always @(*)
      case (CTR_MODE)
        0 : LEG_OUT_SELECT = leg_ctr;
        1 : LEG_OUT_SELECT = in_selection;
        default : LEG_OUT_SELECT = in_selection;
      endcase
endmodule
