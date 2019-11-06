/***************************** Include Files *******************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "reg_map.h"
#include "axi_ikinematics.h"

/************************** Function Definitions ***************************/
//-- Init Inverse Kinematics AXI IP Memory Map
// Pointer axi_mmap points to the iKinematics IP base address, and is 32-bit sized word.
int *init_axi_ikinematics() {
    int _fdmem;
    int *axi_mmap = NULL;
    const char memDevice[] = "/dev/mem";

    _fdmem = open( memDevice, O_RDWR | O_SYNC );
    if ( _fdmem < 0 ) {
        printf("Failed to open /dev/mem !\n");
        return 0;
    } else {
        printf("Open /dev/mem successfully !\n");
    }
    
    axi_mmap = (int *)(mmap(0, AXI_IK_RMAPSIZE*4, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, AXI_IK_BASEADDR));
    
    return axi_mmap;
}

//-- Inverse Kinematics AXI IP memory map read data
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
// read_address : Relative address to read data (No consider base address)
// RETURNS : 32-bit read register
int axi_read(int *axi_mmap, int read_address) {
    return *(axi_mmap+read_address);
}

//-- Inverse Kinematics AXI IP memory map write data
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
// write_address : Relative address to write data (No consider base address)
void axi_write(int *axi_mmap, int write_address, int value) {
    *(axi_mmap+write_address) = value;
}

//-- Inverse Kinematics AXI IP Write FIFO 
// Writes Reg 2, 3, 4 into internal FIFO
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
void axi_write_fifo(int *axi_mmap) {
    *(axi_mmap+AXI_IK_REG_CONTROL) = AXI_IK_WRITE_FIFO;
    *(axi_mmap+AXI_IK_REG_CONTROL) = AXI_IK_CLEAR_REG;
}

//-- Inverse Kinematics AXI IP Trigger Calculations
// Triggers the inverse kinematics calculations
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
void axi_trigger_ikinematics(int *axi_mmap) {
    *(axi_mmap+AXI_IK_REG_CONTROL) = AXI_IK_WRITE_FIFO;
    *(axi_mmap+AXI_IK_REG_CONTROL) = AXI_IK_TRIGGER;
}

//-- Configure R1 : Leg counter
void axi_leg_config(int *axi_mmap, int conf, int leg_select) {
	int set_val = 8 + (leg_select & 0x7);
	int config = (conf<<4) & 0x30;
	int value = set_val + config;
    *(axi_mmap+AXI_IK_REG_LEG_CONTROL) = value;
}

//--
void axi_show_all_regs(int *axi_mmap) {
    for (int i=0; i<AXI_IK_RMAPSIZE; i++) {
        printf("REG[%d]\t:0x%x\n", i, *(axi_mmap+i));
    }
}

void axi_ik_write_gait_step(int *axi_mmap, ik_input_t gait_step) {
    axi_write(axi_mmap, AXI_IK_REG_X_IN, gait_step.x_in);
    axi_write(axi_mmap, AXI_IK_REG_Y_IN, gait_step.y_in);
    axi_write(axi_mmap, AXI_IK_REG_Z_IN, gait_step.z_in);
}

ik_output_t axi_ikinematics_results(int *axi_mmap) {
    ik_output_t temp_out;
    temp_out.q1_out = axi_read(axi_mmap, AXI_IK_REG_Q1_OUT);
    temp_out.q2_out = axi_read(axi_mmap, AXI_IK_REG_Q2_OUT);
    temp_out.q3_out = axi_read(axi_mmap, AXI_IK_REG_Q3_OUT);
    
    return temp_out;
}