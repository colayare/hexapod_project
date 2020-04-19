#ifndef __HEXAPOD_KINEMATICS_H__
#define __HEXAPOD_KINEMATICS_H__

#include <math.h>
#include "../globals/axi_ip_globals.hpp"
#include "../globals/global_defines.hpp"
#include "../params/hexapod_ip_params.hpp"
#include "../params/hexapod_params.hpp"

#define _USE_MATH_DEFINES

//==== MACROS ==================================================================
#define SEXA2RAD(X) X*M_PI/180

//==== Typedefs ================================================================
typedef struct ik_param_t {
    float param[3];
} ik_param_t;

//==== Hexapod Inverse Kinematics AXI IP Class Definition ======================
class ikinematics_ip_context : public ip_context {
    //==== Properties ==========================================================
    private :
        const float _joint_offsets_init [18] = {
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_00),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_01),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_02),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_03),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_04),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_05),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_06),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_07),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_08),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_09),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_10),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_11),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_12),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_13),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_14),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_15),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_16),
            SEXA2RAD(HEXAPOD_JOINT_OFFSET_17) 
        };
            
        // const int32_t servo_inv_init [18] = {
            
        // }
        
        const float _joint_pos_init [18] = {
            SEXA2RAD(HEXAPOD_INIT_POS_00),
            SEXA2RAD(HEXAPOD_INIT_POS_01),
            SEXA2RAD(HEXAPOD_INIT_POS_02),
            SEXA2RAD(HEXAPOD_INIT_POS_03),
            SEXA2RAD(HEXAPOD_INIT_POS_04),
            SEXA2RAD(HEXAPOD_INIT_POS_05),
            SEXA2RAD(HEXAPOD_INIT_POS_06),
            SEXA2RAD(HEXAPOD_INIT_POS_07),
            SEXA2RAD(HEXAPOD_INIT_POS_08),
            SEXA2RAD(HEXAPOD_INIT_POS_09),
            SEXA2RAD(HEXAPOD_INIT_POS_10),
            SEXA2RAD(HEXAPOD_INIT_POS_11),
            SEXA2RAD(HEXAPOD_INIT_POS_12),
            SEXA2RAD(HEXAPOD_INIT_POS_13),
            SEXA2RAD(HEXAPOD_INIT_POS_14),
            SEXA2RAD(HEXAPOD_INIT_POS_15),
            SEXA2RAD(HEXAPOD_INIT_POS_16),
            SEXA2RAD(HEXAPOD_INIT_POS_17)
        };
    
    public :
        float joint_offsets [18];
        float joint_pos     [18];
    
    //==== Method Prototypes ===================================================
    public :
        // Trigger FIFO Write Operation 
        void write_fifo();
        // Trigger Inverse Kinematics Calculation
        void trigger_ikinematics();
        // Configure Leg Counter Module
        void leg_ctr_config(int32_t config, int32_t leg_select);
        // Set Output Joint Read Selector MUX
        void leg_ctr_out_mux(int32_t out_mux_select);
        // Initialize Joint Offsets
        void init_joint_offsets();
        // Initialize Joint Positions
        void init_joint_position();
        // Set Leg Joints Value Directly
        void set_joint_direct(int32_t leg, float joints[3]);
        // Write Direct Kinematics as IP Input
        void write_ik_input(float ik_input[3]);
        // Read Output Joint from Selector
        ik_param_t read_ik_output(int32_t selector);
};

void ikinematics_ip_context::write_fifo() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
}

void ikinematics_ip_context::trigger_ikinematics() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
}

void ikinematics_ip_context::leg_ctr_config(int32_t config, int32_t leg_select) {
    const int32_t mask  = AXI_IK_REG_LEG_CTR_LEG_SEL_M + AXI_IK_REG_LEG_CTR_SET_LEG_T + AXI_IK_REG_LEG_CTR_MODE_M;
    int32_t set_val     = ((leg_select << AXI_IK_REG_LEG_CTR_LEG_SEL_O) & AXI_IK_REG_LEG_CTR_LEG_SEL_M) + AXI_IK_REG_LEG_CTR_SET_LEG_T;
    int32_t set_config  = (config << AXI_IK_REG_LEG_CTR_MODE_O) & AXI_IK_REG_LEG_CTR_MODE_M;
    int32_t value          = set_val + set_config;
    this->axi_write_mask(AXI_IK_REG_LEG_CTRL, value, mask);
}

void ikinematics_ip_context::leg_ctr_out_mux(int32_t out_mux_select) {
    int32_t mux_sel = (out_mux_select << AXI_IK_REG_LEG_F2F_READ_MUX_O) & AXI_IK_REG_LEG_F2F_READ_MUX_M;
    this->axi_write_mask(AXI_IK_REG_LEG_CTRL, mux_sel, AXI_IK_REG_LEG_F2F_READ_MUX_M);
}

void ikinematics_ip_context::init_joint_offsets() {
    float   offset;
    int     int_offset;
    for (int i=0;i<18;i++) {
        offset      = this->_joint_offsets_init[i];
        int_offset  = *(int *) &offset;
        #ifdef __LOG
        cout << "Offset [" << i << "] = " << hex << int_offset << endl;
        #endif
        this->axi_write(i+AXI_IK_REG_OFFSET_BASE, int_offset);
    }
}

void ikinematics_ip_context::init_joint_position() {
    float leg_joints [3];
    for (int leg=0;leg<6;leg++) {
        leg_joints[0] = *(this->_joint_pos_init+leg*3+0);
        leg_joints[1] = *(this->_joint_pos_init+leg*3+1);
        leg_joints[2] = *(this->_joint_pos_init+leg*3+2);
        this->set_joint_direct(leg, leg_joints);
    }
}

void ikinematics_ip_context::set_joint_direct(int32_t leg, float joints[3]) {
    int int_position [3];
    for (int joint=0;joint<3;joint++) {
        int_position[joint] = *(int *) &joints[joint];
        #ifdef __LOG
        cout << "Joint [" << joint+leg*3 << "] = " << hex << int_position[joint] << endl;
        #endif
        this->joint_pos[joint+leg*3] = joints[joint];
    }
    this->leg_ctr_config(AXI_IK_REG_LEG_CTR_ONE_LEG, leg);
    this->leg_ctr_out_mux(leg+1);
    this->axi_write(AXI_IK_REG_Q1_OUT, int_position[0]);
    this->axi_write(AXI_IK_REG_Q2_OUT, int_position[1]);
    this->axi_write(AXI_IK_REG_Q3_OUT, int_position[2]);
    this->axi_write(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_DIRECT_OUT);
}

void ikinematics_ip_context::write_ik_input(float ik_input[3]) {
    this->axi_write(AXI_IK_REG_X_IN, *(int *) &ik_input[0]);
    this->axi_write(AXI_IK_REG_Y_IN, *(int *) &ik_input[1]);
    this->axi_write(AXI_IK_REG_Z_IN, *(int *) &ik_input[2]);
}

ik_param_t ikinematics_ip_context::read_ik_output(int32_t selector) {
    ik_param_t temp_out;
    int32_t read_values [3];
    this->leg_ctr_out_mux(selector);
    read_values[0] = this->axi_read(AXI_IK_REG_Q1_OUT);
    read_values[1] = this->axi_read(AXI_IK_REG_Q2_OUT);
    read_values[2] = this->axi_read(AXI_IK_REG_Q3_OUT);
    
    temp_out.param[0] = *(float *) &read_values[0];
    temp_out.param[1] = *(float *) &read_values[1];
    temp_out.param[2] = *(float *) &read_values[2];
    
    return temp_out;
}

#endif //__HEXAPOD_KINEMATICS_H__