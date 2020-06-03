#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include "globals/global_defines.h"
#include "globals/axi_ip_globals.h"
#include "hexapod/hexapod_kinematics.h"

using namespace std;

int main() {
    cout << "Test Inverse Kinematics IP" << endl;
    
    // Declare Inverse Kinematics AXI IP Context
    ikinematics_ip_context iK_ip("/dev/mem");
    
    // Initialize Inverse Kinematics IP
    iK_ip.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
    
    // Initialize Joints Offsets
    iK_ip.init_joint_offsets();
    
    // Initialize Servo Inversions
    iK_ip.init_servo_invertion();
    
    // Initialize Joints Positions
    iK_ip.init_joint_position();
    
    
    //iK_ip.axi_show_regs(0, 0x23);
  

    return 0;
}
