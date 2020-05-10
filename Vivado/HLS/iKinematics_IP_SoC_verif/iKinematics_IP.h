/*
 * iKinematics_IP.h
 *
 *  Created on: 10 may. 2020
 *      Author: Roy
 */

#ifndef SRC_IKINEMATICS_IP_H_
#define SRC_IKINEMATICS_IP_H_

#include "ip_context.h"
#include "hexapod_params.h"

# define M_PI 3.14159265358979323846  /* pi */
#define SEXA2RAD(X) X*M_PI/180

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
    ik_input_t leg [6];
} ef_pos_t;

class ikinematics_ip_context : public ip_context {
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
    float joint_offsets[18];
    float joint_pos[18];

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
    void set_joint_direct(uint32_t leg, ik_output_t joints);
    // Write Direct Kinematics as IP Input
    void write_ik_input(ik_input_t ik_input);
    // Read Output Joint from Selector
    ik_output_t read_ik_output(uint32_t selector);
    
};

#endif /* SRC_IKINEMATICS_IP_H_ */
