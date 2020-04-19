#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include "globals/axi_ip_globals.hpp"
#include "hexapod/hexapod_kinematics.hpp"

using namespace std;

int main() {
    cout << "Test Inverse Kinematics IP" << endl;
    
    // Declare Inverse Kinematics AXI IP Context
    ikinematics_ip_context iK_ip;
    
    // Initialize Inverse Kinematics IP
    iK_ip.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
    
    // Initialize Joints Offsets
    iK_ip.init_joint_offsets();
    
    // Initialize Joints Positions
    iK_ip.init_joint_position();
    
    iK_ip.axi_show_regs(0, 0x23);
    
    ik_param_t ik_out;
    
    ik_out = iK_ip.read_ik_output(1);
    
    cout << ik_out.param[0] << endl;
    cout << ik_out.param[1] << endl;
    cout << ik_out.param[2] << endl;

    return 0;
}