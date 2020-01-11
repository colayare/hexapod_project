`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10.03.2019 22:13:07
// Design Name: 
// Module Name: servo_pwm_generator
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


module servo_pwm_generator
#(
	// PWM bit-size
	parameter integer C_PWM_SIZE  = 8,
	// Clock Frequency in MHz
	parameter integer C_PWM_FREQ  = 100,
	// PWM Input Max Value
	parameter integer C_PWM_MAX_IN = 200
)(
	//-- SYSTEM INTERFACE --
    input 					nRST,			// Reset active low
    input 					CLK,			// Clock signal
    //-- PWM Control --
    input					EN,
    input					COMPLEMENT,
    input [C_PWM_SIZE-1:0]	PWM_IN,
    //-- PWM Output --
    output reg				PWM_OUT
	);
	
//----------------------------------------------
//-- Local Parameters Definitions
//----------------------------------------------
//-- Finite State Machine states definition --
localparam [2:0]	fsm_reset	= 0,
					fsm_idle 	= 1,
					fsm_init 	= 2,
					fsm_pwm 	= 3,
					fsm_tail 	= 4;
//-- Init Max Counter --
localparam integer C_PWM_INIT_MAX = C_PWM_FREQ / 2 * 1000;
localparam integer C_PWM_TAIL_MAX = 17500 * C_PWM_FREQ;
localparam integer C_PWM_STEP_MAX  = 2000 * C_PWM_FREQ / C_PWM_MAX_IN;

//----------------------------------------------
//-- Signals
//----------------------------------------------
//-- FSM Signals
reg [2:0]	fsm_state,
			fsm_next_state;

//-- Counter
reg [$clog2(C_PWM_TAIL_MAX)-1:0]	counter,
									counter_next;
reg [$clog2(C_PWM_STEP_MAX)-1:0]	pwm_step_counter,
									pwm_step_counter_next;
reg [C_PWM_SIZE-1:0]				pwm_counter,
									pwm_counter_next;
//-- PWM Input Register
reg [7:0]							pwm_in;
reg									s_pwm_out;

//----------------------------------------------
//-- Behavioral
//----------------------------------------------
//-- FSM --
always @(posedge CLK)
	if (nRST)
		fsm_state <= fsm_next_state;
	else
		fsm_state <= 0;

//-- FSM Next State Logic --
always @(*)
	case (fsm_state)
		fsm_reset	: fsm_next_state = fsm_idle;
		fsm_idle	: fsm_next_state = (EN) ? fsm_init : fsm_idle;
		fsm_init	: fsm_next_state = (counter==C_PWM_INIT_MAX-1) ? fsm_pwm : fsm_init;
		fsm_pwm		: fsm_next_state = (pwm_counter==C_PWM_MAX_IN-1) ? fsm_tail : fsm_pwm;
		fsm_tail	: begin
						if (counter==C_PWM_TAIL_MAX-1)
							if (EN)
								fsm_next_state = fsm_init;
							else
								fsm_next_state = fsm_idle;
						else
							fsm_next_state = fsm_tail;
						end
		default		: fsm_next_state = fsm_idle;
	endcase

//-- FSM Output Logic --
always @(*)
	case (fsm_state)
		fsm_reset	: begin
						counter_next			= 0;
						pwm_step_counter_next	= 0;
						pwm_counter_next		= 0;
						s_pwm_out				= 0;
					end
		fsm_idle	: begin
						counter_next			= 0;
						pwm_step_counter_next	= 0;
						pwm_counter_next		= 0;
						s_pwm_out				= 0;
					end
		fsm_init	: begin
						if (counter == C_PWM_INIT_MAX - 1)
							counter_next = 0;
						else
							counter_next = counter + 1;
						pwm_step_counter_next	= 0;
						pwm_counter_next		= 0;
						s_pwm_out				= 1;
					end
		fsm_pwm		: begin
						counter_next			= 0;
						if (pwm_step_counter == C_PWM_STEP_MAX - 1)
							pwm_step_counter_next = 0;
						else
							pwm_step_counter_next = pwm_step_counter + 1;
						if (pwm_counter == C_PWM_MAX_IN - 1)
							pwm_counter_next		= 0;
						else
							if (pwm_step_counter == C_PWM_STEP_MAX - 1)
								pwm_counter_next = pwm_counter + 1;
							else
								pwm_counter_next = pwm_counter;
						if (pwm_counter < pwm_in)
							s_pwm_out = 1;
						else
							s_pwm_out = 0;
					end
		fsm_tail	: begin
						if (counter == C_PWM_TAIL_MAX - 1)
							counter_next = 0;
						else
							counter_next = counter + 1;
						pwm_step_counter_next	= 0;
						pwm_counter_next		= 0;
						s_pwm_out				= 0;
					end
		default	: begin
						counter_next			= 0;
						pwm_step_counter_next	= 0;
						pwm_counter_next		= 0;
						s_pwm_out				= 0;
					end
	endcase

// Counters
always @(posedge CLK)
	if (nRST) begin
		pwm_in				<= (COMPLEMENT) ? C_PWM_MAX_IN - PWM_IN : PWM_IN;
		counter				<= counter_next;
		pwm_step_counter	<= pwm_step_counter_next;
		pwm_counter			<= pwm_counter_next;
		PWM_OUT				<= s_pwm_out;
		end
	else begin
		pwm_in				<= 0;
		counter				<= 0;
		pwm_step_counter	<= 0;
		pwm_counter			<= 0;
		PWM_OUT				<= 0;
		end

endmodule
