`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 20.10.2019 18:38:16
// Design Name: 
// Module Name: fxp_to_pwm
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


module fxp_to_pwm #(
	parameter integer FXP_WIDTH = 20,
	parameter integer PWM_OFFSET = 8,
	parameter integer PWM_WIDTH = 8
	) (
	input wire [FXP_WIDTH-1:0]	FXP_IN,
	output wire [PWM_WIDTH-1:0]	PWM_OUT
    );
    
    assign PWM_OUT = FXP_IN[PWM_OFFSET-:PWM_WIDTH];
    
endmodule