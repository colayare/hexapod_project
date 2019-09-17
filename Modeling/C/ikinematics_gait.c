#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "reg_map.h"
#include "axi_ikinematics.h"
#include "hexapod_gaits.h"

int main() {
    printf("iKinematics IP Gait Test");
    
    int *axi_mmap = init_axi_ikinematics();
    ik_input_t gait_step;
    ik_output_t q_out;
    
    axi_show_all_regs(axi_mmap);
        
    axi_leg_config(axi_mmap, AXI_IK_ONE_LEG, 2);
    
    printf("######### Gait Start\n");
    
    for(int i=0; i<30; i++){
        printf("Gait %d\n", i);
        gait_step = get_gait_step(i);
        axi_ik_write_gait_step(axi_mmap, gait_step);
        axi_write_fifo(axi_mmap);
        axi_trigger_ikinematics(axi_mmap);
        
        q_out = axi_ikinematics_results(axi_mmap);
        
        printf("Q1: %f\nQ2: %f\nQ3: %f\n", *(float *) &q_out.q1_out, *(float *) &q_out.q2_out, *(float *) &q_out.q3_out);
        printf("----\n");
    }
    
    printf("##########################\n");
    axi_show_all_regs(axi_mmap);
    
    return 0;
}