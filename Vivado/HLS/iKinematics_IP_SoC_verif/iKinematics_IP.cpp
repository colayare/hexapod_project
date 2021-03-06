/*
 * iKinematics_IP.cpp
 *
 *  Created on: 10 may. 2020
 *      Author: Roy
 */

#include "iKinematics_IP.h"
#include "iKinematics_IP_params.h"

#define __ALL_DEBUG

void ikinematics_ip_context::write_fifo() {
    this->axi_write(REG_TRIG, TRIG_FIFO);
    this->axi_write(REG_TRIG, 0);
}

void ikinematics_ip_context::trigger_ikinematics() {
    this->axi_write(REG_TRIG, TRIG_CALC);
    this->axi_write(REG_TRIG, 0);
}

void ikinematics_ip_context::leg_ctr_config(uint32_t config, uint32_t leg_select) {
    const uint32_t mask  = LEGC_CTR_LEG_SEL_M + LEGC_CTR_SET_LEG_T + LEGC_CTR_MODE_M;
    uint32_t set_val     = ((leg_select << LEGC_INSE_LSB) & LEGC_CTR_LEG_SEL_M) + LEGC_CTR_SET_LEG_T;
    uint32_t set_config  = (config << LEGC_MSEL_LSB) & LEGC_CTR_MODE_M;
    uint32_t value       = set_val + set_config;
    this->axi_write_mask(REG_LEGC, value, mask);
}

void ikinematics_ip_context::leg_ctr_out_mux(uint32_t out_mux_select) {
    uint32_t mux_sel = (out_mux_select << LEGC_FXFC_LSB) & LEGC_F2F_READ_MUX_M;
    this->axi_write_mask(REG_LEGC, mux_sel, LEGC_F2F_READ_MUX_M);
}

void ikinematics_ip_context::init_joint_offsets() {
    float   offset;
    int     int_offset;
    for (int i=0;i<18;i++) {
        offset      = this->_joint_offsets_init[i];
        int_offset  = *(int *) &offset;
        #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
        std::cout << "Offset [" << i << "] = " << std::hex << int_offset << std::endl;
        #endif
        this->axi_write(i+REG_OF01, int_offset);
    }
}

void ikinematics_ip_context::init_joint_position() {
    ik_output_t leg_joints;
    const float *joint_pos_init_ptr;
    const float *joint_offsets_init_ptr;
    joint_pos_init_ptr = this->_joint_pos_init;
    joint_offsets_init_ptr = this->_joint_offsets_init;
    for (int leg=0;leg<6;leg++) {
        leg_joints.q1 = *(joint_pos_init_ptr+leg*3+0) + *(joint_offsets_init_ptr+leg*3+0);
        leg_joints.q2 = *(joint_pos_init_ptr+leg*3+1) + *(joint_offsets_init_ptr+leg*3+1);
        leg_joints.q3 = *(joint_pos_init_ptr+leg*3+2) + *(joint_offsets_init_ptr+leg*3+2);
        this->set_joint_direct(leg, leg_joints);
    }
}

void ikinematics_ip_context::init_servo_invertion() {
    uint32_t servo_inv = 0;
    for (int s=0;s<18;s++) {
        servo_inv += this->_servo_inv_init[s] << s;
    }
    #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
    std::cout << "Inversion values " << std::hex << servo_inv << std::endl;
    #endif
    servo_inv = servo_inv << LEGC_SINV_01;
    this->axi_write_mask(REG_LEGC, servo_inv, LEGC_PWM_INVERT_M);
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
    std::cout << "Joint [" << 0+leg*3 << "] = " << std::hex << int_position[0] << std::endl;
    std::cout << "Joint [" << 1+leg*3 << "] = " << std::hex << int_position[1] << std::endl;
    std::cout << "Joint [" << 2+leg*3 << "] = " << std::hex << int_position[2] << std::endl;
    #endif
    
    this->leg_ctr_config(LEGC_CTR_ONE_LEG, leg);
    this->leg_ctr_out_mux(leg+1);
    this->axi_write(REG_IKQ1, int_position[0]);
    this->axi_write(REG_IKQ2, int_position[1]);
    this->axi_write(REG_IKQ3, int_position[2]);
    this->axi_write(REG_TRIG, TRIG_WRITE_DRCT);
    this->axi_write(REG_TRIG, 0);
}

void ikinematics_ip_context::write_ik_input(ik_input_t ik_input) {
    #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
    std::cout << "Set X = " << ik_input.x << std::endl;
    std::cout << "Set Y = " << ik_input.y << std::endl;
    std::cout << "Set Z = " << ik_input.z << std::endl;
    #endif
    this->axi_write(REG_IKIX, *(int *) &ik_input.x);
    this->axi_write(REG_IKIY, *(int *) &ik_input.y);
    this->axi_write(REG_IKIZ, *(int *) &ik_input.z);
}

ik_output_t ikinematics_ip_context::read_ik_output(uint32_t selector) {
    ik_output_t temp_out;
    uint32_t read_values [3];
    this->leg_ctr_out_mux(selector);
    read_values[0] = this->axi_read(REG_IKQ1);
    read_values[1] = this->axi_read(REG_IKQ2);
    read_values[2] = this->axi_read(REG_IKQ3);
    
    temp_out.q1 = *(float *) &read_values[0];
    temp_out.q2 = *(float *) &read_values[1];
    temp_out.q3 = *(float *) &read_values[2];
    
    return temp_out;
}

