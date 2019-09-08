//`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////////
//// Company: 
//// Engineer: 
//// 
//// Create Date: 21.01.2019 23:47:55
//// Design Name: 
//// Module Name: fxp_multiplier
//// Project Name: 
//// Target Devices: 
//// Tool Versions: 
//// Description: 
//// 
//// Dependencies: 
//// 
//// Revision:
//// Revision 0.01 - File Created
//// Additional Comments:
//// 
////////////////////////////////////////////////////////////////////////////////////


//module fxp_multiplier #(
//	parameter C_FXP_LENGTH=16,
//	parameter C_FXP_POINT=12
//	)(
//	input signed [C_FXP_LENGTH-1:0]			S_NUM1,
//											S_NUM2,
//	output reg signed [C_FXP_LENGTH-1:0]	S_RESULT,
//	output 									S_OF_FLAG
//    );
    
//localparam INT_WIDTH 	= C_FXP_LENGTH - C_FXP_POINT;
//localparam DEC_WIDTH 	= C_FXP_POINT;
//localparam HALF_WIDTH	= C_FXP_LENGTH / 2;
//localparam C_FXP_MULT_OFFSET = DEC_WIDTH;
//localparam [C_FXP_LENGTH-1:0] MAX_POS	= {1'b0,{INT_WIDTH-1{1'b1}},{DEC_WIDTH{1'b1}}};
//localparam [C_FXP_LENGTH-1:0] MAX_NEG	= {1'b1,{C_FXP_LENGTH-1{1'b0}}};

//wire signed [C_FXP_LENGTH-1:0] 	s_mult;
//wire signed [HALF_WIDTH-1:0]	s_half_num1,
//								s_half_num2;
//wire [1:0] overflow;

//assign s_half_num1 = S_NUM1[C_FXP_LENGTH-1-:HALF_WIDTH];
//assign s_half_num2 = S_NUM2[C_FXP_LENGTH-1-:HALF_WIDTH];

//assign s_mult		= s_half_num1 * s_half_num2;
//assign overflow		= s_mult[C_FXP_MULT_OFFSET:C_FXP_MULT_OFFSET-1];
//assign S_OF_FLAG	= s_mult[C_FXP_MULT_OFFSET] ^ s_mult[C_FXP_MULT_OFFSET-1];

//always @(*) begin
//	case (overflow)
//		2'b01	: S_RESULT = MAX_POS;
//		2'b10	: S_RESULT = MAX_NEG;
//		default	: S_RESULT = s_mult;	//[C_FXP_MULT_OFFSET-1-:C_FXP_LENGTH]
//	endcase
//	end

//endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 21.01.2019 23:47:55
// Design Name: 
// Module Name: fxp_multiplier
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


module fxp_multiplier #(
	parameter C_FXP_LENGTH=16,
	parameter C_FXP_POINT=12
	)(
	input signed [C_FXP_LENGTH-1:0]			S_NUM1,
											S_NUM2,
	output reg signed [C_FXP_LENGTH-1:0]	S_RESULT,
	output 									S_OF_FLAG
    );
    
localparam INT_WIDTH 	= C_FXP_LENGTH - C_FXP_POINT;
localparam DEC_WIDTH 	= C_FXP_POINT;
localparam C_FXP_MULT_OFFSET = DEC_WIDTH*2 + INT_WIDTH;
localparam [C_FXP_LENGTH-1:0] MAX_POS	= {1'b0,{INT_WIDTH-1{1'b1}},{DEC_WIDTH{1'b1}}};
localparam [C_FXP_LENGTH-1:0] MAX_NEG	= {1'b1,{C_FXP_LENGTH-1{1'b0}}};

wire signed [C_FXP_LENGTH*2-1:0] s_mult;
wire [1:0] overflow;

assign s_mult		= S_NUM1 * S_NUM2;
assign overflow		= s_mult[C_FXP_MULT_OFFSET:C_FXP_MULT_OFFSET-1];
assign S_OF_FLAG	= s_mult[C_FXP_MULT_OFFSET] ^ s_mult[C_FXP_MULT_OFFSET-1];

always @(*) begin
	case (overflow)
		2'b01	: S_RESULT = MAX_POS;
		2'b10	: S_RESULT = MAX_NEG;
		default	: S_RESULT = s_mult[C_FXP_MULT_OFFSET-1-:C_FXP_LENGTH];
	endcase
	end

endmodule

