`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 28.10.2019 23:04:59
// Design Name: 
// Module Name: verIP_fxp_adder
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


module verIP_fxp_adder();
//-- Environment Parameters
parameter integer CLK_PERIOD = 100;
//-- IP Parameters
parameter integer C_FXP_LENGTH = 20;
parameter integer C_FXP_POINT = 16;

//--
reg clk;									// System Clock
//-- IP Signals
reg signed [C_FXP_LENGTH-1:0] s_num1;		//
reg signed [C_FXP_LENGTH-1:0] s_num2;		//
reg s_ope;									//
wire signed [C_FXP_LENGTH-1:0] s_result;	//
wire s_of_flag;
 
//-- Device Under Verification
fxp_adder #(
		.C_FXP_LENGTH(C_FXP_LENGTH),
		.C_FXP_POINT(C_FXP_POINT)
	) DUV (
		.S_NUM1(s_num1),
		.S_NUM2(s_num2),
		.S_OPE(s_ope),
		.S_RESULT(s_result),
		.S_OF_FLAG(s_of_flag)
    );

//-- System Clock Generation
always begin
		clk <= 1'b1;
		#(CLK_PERIOD/2);
		clk <= 1'b0;
		#(CLK_PERIOD/2);
		end

initial begin
s_num1 = 0;
s_num2 = 0;
s_ope = 0;
end

endmodule
