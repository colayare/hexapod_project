#ifndef __IKINEMATICS_IP__
#deifne __IKINEMATICS_IP__

#include "ikinematics_params.hpp"

// Typedefs 
typedef struct ik_input_t {
    float x_in;
    float y_in;
    float z_in;
} ik_input_t;

typedef struct ik_output_t {
    float q1_out;
    float q2_out;
    float q3_out;
} ik_output_t;

class ikinematics_ip_context : ip_context {
    //==== Properties ==========================================================
    
    //==== Method Prototypes ===================================================
    extern void write_fifo();
    extern void trigger_ikinematics();
    extern void leg_ctrl_config(int32_t config, int32_t leg_select);
    extern void write_ik_input(ik_input_t ik_input);
    extern ik_output_t read_ik_output();
}

void ikinematics_ip_context::write_fifo() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
}

void ikinematics_ip_context::trigger_ikinematics() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
}

void ikinematics_ip_context::write_ik_input(ik_input_t ik_input) {
    this->axi_write(AXI_IK_REG_X_IN, ik_input.x_in;
    this->axi_write(AXI_IK_REG_Y_IN, ik_input.y_in;
    this->axi_write(AXI_IK_REG_Z_IN, ik_input.z_in;
}

ik_output_t ikinematics_ip_context::read_ik_output() {
    ik_output_t temp_out;
    temp_out.q1_out = *(float *) this->axi_read(AXI_IK_REG_Q1_OUT);
    temp_out.q2_out = *(float *) this->axi_read(AXI_IK_REG_Q2_OUT);
    temp_out.q3_out = *(float *) this->axi_read(AXI_IK_REG_Q3_OUT);
    
    return temp_out;
}

#endif //__IKINEMATICS_IP__