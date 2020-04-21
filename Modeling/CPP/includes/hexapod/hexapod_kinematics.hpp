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
typedef struct ik_input_t {
    float x;
    float y;
    float z;
} ik_input_t;

typedef struct ik_output_t {
    float q1;
    float q2;
    float q3;
} ik_output_t;

typedef struct ef_pos_t {
    ik_input_t leg1;
    ik_input_t leg2;
    ik_input_t leg3;
    ik_input_t leg4;
    ik_input_t leg5;
    ik_input_t leg6;
} ef_pos_t;

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
            
        const uint32_t _servo_inv_init [18] = {
            HEXAPOD_SERVO_INV_00,
            HEXAPOD_SERVO_INV_01,
            HEXAPOD_SERVO_INV_02,
            HEXAPOD_SERVO_INV_03,
            HEXAPOD_SERVO_INV_04,
            HEXAPOD_SERVO_INV_05,
            HEXAPOD_SERVO_INV_06,
            HEXAPOD_SERVO_INV_07,
            HEXAPOD_SERVO_INV_08,
            HEXAPOD_SERVO_INV_09,
            HEXAPOD_SERVO_INV_10,
            HEXAPOD_SERVO_INV_11,
            HEXAPOD_SERVO_INV_12,
            HEXAPOD_SERVO_INV_13,
            HEXAPOD_SERVO_INV_14,
            HEXAPOD_SERVO_INV_15,
            HEXAPOD_SERVO_INV_16,
            HEXAPOD_SERVO_INV_17
        };
        
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
        // Digital Joint Offsets (rad)
        float joint_offsets [18];
        // Joints Positions (rad)
        float joint_pos     [18];
        // Leg End Effectors Positions x,y,z (m)
        ef_pos_t    ef_position;
    
    //==== Method Prototypes ===================================================
    public :
        // Trigger FIFO Write Operation 
        void write_fifo();
        // Trigger Inverse Kinematics Calculation
        void trigger_ikinematics();
        // Configure Leg Counter Module
        void leg_ctr_config(uint32_t config, uint32_t leg_select);
        // Set Output Joint Read Selector MUX
        void leg_ctr_out_mux(uint32_t out_mux_select);
        // Initialize Joint Offsets
        void init_joint_offsets();
        // Initialize Joint Positions
        void init_joint_position();
        // Initialize Servo Inversions
        void init_servo_invertion();
        // Set Leg Joints Value Directly
        void set_joint_direct(uint32_t leg, float joints[3]);
        // Write Direct Kinematics as IP Input
        void write_ik_input(ik_input_t ik_input);
        // Read Output Joint from Selector
        ik_output_t read_ik_output(uint32_t selector);
};

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
    float leg_joints [3];
    for (int leg=0;leg<6;leg++) {
        leg_joints[0] = *(this->_joint_pos_init+leg*3+0);
        leg_joints[1] = *(this->_joint_pos_init+leg*3+1);
        leg_joints[2] = *(this->_joint_pos_init+leg*3+2);
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

void ikinematics_ip_context::set_joint_direct(uint32_t leg, float joints[3]) {
    int int_position [3];
    for (int joint=0;joint<3;joint++) {
        int_position[joint] = *(int *) &joints[joint];
        #if defined(__IK_DEBUG) || defined(__ALL_DEBUG)
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

#endif //__HEXAPOD_KINEMATICS_H__