#include <math.h>
#include "globals/axi_ip_globals.h"
#include "globals/global_defines.h"
#include "hexapod/hexapod_kinematics.h"
#include "params/hexapod_ip_params.h"
#include "params/hexapod_params.h"

void ikinematics_ip_context::write_fifo() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
}

void ikinematics_ip_context::trigger_ikinematics() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
}

void ikinematics_ip_context::leg_ctr_config(uint32_t config, uint32_t leg_select) {
    const uint32_t mask  = AXI_IK_REG_LEG_CTR_LEG_SEL_M + AXI_IK_REG_LEG_CTR_SET_LEG_T + AXI_IK_REG_LEG_CTR_MODE_M;
    uint32_t set_val     = ((leg_select << AXI_IK_REG_LEG_CTR_LEG_SEL_O) & AXI_IK_REG_LEG_CTR_LEG_SEL_M) + AXI_IK_REG_LEG_CTR_SET_LEG_T;
    uint32_t set_config  = (config << AXI_IK_REG_LEG_CTR_MODE_O) & AXI_IK_REG_LEG_CTR_MODE_M;
    uint32_t value       = set_val + set_config;
    this->axi_write_mask(AXI_IK_REG_LEG_CTRL, value, mask);
}

void ikinematics_ip_context::leg_ctr_out_mux(uint32_t out_mux_select) {
    uint32_t mux_sel = (out_mux_select << AXI_IK_REG_LEG_F2F_READ_MUX_O) & AXI_IK_REG_LEG_F2F_READ_MUX_M;
    this->axi_write_mask(AXI_IK_REG_LEG_CTRL, mux_sel, AXI_IK_REG_LEG_F2F_READ_MUX_M);
}

void ikinematics_ip_context::init_joint_offsets() {
    float   offset;
    int     int_offset;
    for (int i=0;i<18;i++) {
        offset      = this->_joint_offsets_init[i];
        int_offset  = *(int *) &offset;
        #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
        cout << "Offset [" << i << "] = " << hex << int_offset << endl;
        #endif
        this->axi_write(i+AXI_IK_REG_OFFSET_BASE, int_offset);
    }
}

void ikinematics_ip_context::init_joint_position() {
    ik_output_t leg_joints;
    for (int leg=0;leg<6;leg++) {
        leg_joints.q1 = *(this->_joint_pos_init+leg*3+0);
        leg_joints.q2 = *(this->_joint_pos_init+leg*3+1);
        leg_joints.q3 = *(this->_joint_pos_init+leg*3+2);
        this->set_joint_direct(leg, leg_joints);
    }
}

void ikinematics_ip_context::init_servo_invertion() {
    uint32_t servo_inv = 0;
    for (int s=0;s<18;s++) {
        servo_inv += this->_servo_inv_init[s] << s;
    }
    #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
    cout << "Inversion values " << hex << servo_inv << endl;
    #endif
    servo_inv = servo_inv << AXI_IK_REG_LEG_PWM_INVERT_O;
    this->axi_write_mask(AXI_IK_REG_LEG_CTRL, servo_inv, AXI_IK_REG_LEG_PWM_INVERT_M);
}

void ikinematics_ip_context::set_joint_direct(uint32_t leg, ik_output_t joints) {
    int int_position [3];
    
    int_position[0] = *(int *) &joints.q1;
    int_position[1] = *(int *) &joints.q2;
    int_position[2] = *(int *) &joints.q3;
    
    this->joint_pos[leg*3+0] = joints.q1;
    this->joint_pos[leg*3+1] = joints.q2;
    this->joint_pos[leg*3+2] = joints.q3;
    
    #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
    cout << "Joint [" << joint+leg*3 << "] = " << hex << int_position[joint] << endl;
    cout << "Joint [" << joint+leg*3 << "] = " << hex << int_position[joint] << endl;
    cout << "Joint [" << joint+leg*3 << "] = " << hex << int_position[joint] << endl;
    #endif
    
    this->leg_ctr_config(AXI_IK_REG_LEG_CTR_ONE_LEG, leg);
    this->leg_ctr_out_mux(leg+1);
    this->axi_write(AXI_IK_REG_Q1_OUT, int_position[0]);
    this->axi_write(AXI_IK_REG_Q2_OUT, int_position[1]);
    this->axi_write(AXI_IK_REG_Q3_OUT, int_position[2]);
    this->axi_write(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_DIRECT_OUT);
}

void ikinematics_ip_context::write_ik_input(ik_input_t ik_input) {
    this->axi_write(AXI_IK_REG_X_IN, *(int *) &ik_input.x);
    this->axi_write(AXI_IK_REG_Y_IN, *(int *) &ik_input.y);
    this->axi_write(AXI_IK_REG_Z_IN, *(int *) &ik_input.z);
}

ik_output_t ikinematics_ip_context::read_ik_output(uint32_t selector) {
    ik_output_t temp_out;
    uint32_t read_values [3];
    this->leg_ctr_out_mux(selector);
    read_values[0] = this->axi_read(AXI_IK_REG_Q1_OUT);
    read_values[1] = this->axi_read(AXI_IK_REG_Q2_OUT);
    read_values[2] = this->axi_read(AXI_IK_REG_Q3_OUT);
    
    temp_out.q1 = *(float *) &read_values[0];
    temp_out.q2 = *(float *) &read_values[1];
    temp_out.q3 = *(float *) &read_values[2];
    
    return temp_out;
}