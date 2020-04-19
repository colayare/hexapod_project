#ifndef __HEXAPOD_KINEMATICS_H__
#define __HEXAPOD_KINEMATICS_H__

#include "../globals/axi_ip_globals.hpp"
#include "../params/hexapod_ip_params.hpp"
#include "../params/hexapod_params.hpp"

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

class ikinematics_ip_context : public ip_context {
    //==== Properties ==========================================================
    private :
        const float joint_offsets_init [18] = {
            HEXAPOD_JOINT_OFFSET_00,
            HEXAPOD_JOINT_OFFSET_01,
            HEXAPOD_JOINT_OFFSET_02,
            HEXAPOD_JOINT_OFFSET_03,
            HEXAPOD_JOINT_OFFSET_04,
            HEXAPOD_JOINT_OFFSET_05,
            HEXAPOD_JOINT_OFFSET_06,
            HEXAPOD_JOINT_OFFSET_07,
            HEXAPOD_JOINT_OFFSET_08,
            HEXAPOD_JOINT_OFFSET_09,
            HEXAPOD_JOINT_OFFSET_10,
            HEXAPOD_JOINT_OFFSET_11,
            HEXAPOD_JOINT_OFFSET_12,
            HEXAPOD_JOINT_OFFSET_13,
            HEXAPOD_JOINT_OFFSET_14,
            HEXAPOD_JOINT_OFFSET_15,
            HEXAPOD_JOINT_OFFSET_16,
            HEXAPOD_JOINT_OFFSET_17 
            };
    
    public :
        float joint_offsets [18];
    
    //==== Method Prototypes ===================================================
    public :
        void write_fifo();
        void trigger_ikinematics();
        void leg_ctrl_config(int32_t config, int32_t leg_select);
        void set_joint_offsets();
        void write_ik_input(ik_input_t ik_input);
        ik_output_t read_ik_output();
};

void ikinematics_ip_context::write_fifo() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_WRITE_FIFO);
}

void ikinematics_ip_context::trigger_ikinematics() {
    this->axi_bit_set(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
    this->axi_bit_clr(AXI_IK_REG_CONTROL, AXI_IK_REG_CONTROL_TRIGGER);
}

void ikinematics_ip_context::set_joint_offsets() {
    float   offset;
    int     int_offset;
    for (int i=0;i<18;i++) {
        //offset = const_cast <float>(&this->joint_offsets_init[i]);
        offset = this->joint_offsets_init[i];
        int_offset = *(int *) &offset;
        #ifdef __LOG
        cout << "Offset [" << i << "] = " << hex << int_offset << endl;
        #endif
        this->axi_write(i+AXI_IK_REG_OFFSET_BASE, int_offset);
    }
}

void ikinematics_ip_context::write_ik_input(ik_input_t ik_input) {
    this->axi_write(AXI_IK_REG_X_IN, ik_input.x_in);
    this->axi_write(AXI_IK_REG_Y_IN, ik_input.y_in);
    this->axi_write(AXI_IK_REG_Z_IN, ik_input.z_in);
}

ik_output_t ikinematics_ip_context::read_ik_output() {
    ik_output_t temp_out;
    temp_out.q1_out = *(float *) this->axi_read(AXI_IK_REG_Q1_OUT);
    temp_out.q2_out = *(float *) this->axi_read(AXI_IK_REG_Q2_OUT);
    temp_out.q3_out = *(float *) this->axi_read(AXI_IK_REG_Q3_OUT);
    
    return temp_out;
}



#endif //__HEXAPOD_KINEMATICS_H__