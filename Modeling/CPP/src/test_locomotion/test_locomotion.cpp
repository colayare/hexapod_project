#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include "globals/axi_ip_globals.hpp"
#include "hexapod/hexapod_kinematics.hpp"
#include "hexapod/hexapod_locomotion.hpp"

using namespace std;

int main() {
    cout << "Test Inverse Kinematics IP" << endl;
    
    // Declare Inverse Kinematics AXI IP Context
    hexapod_locomotion hexapod;
    
    // Initialize Inverse Kinematics IP
    hexapod.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
    // Initialize Joints Offsets
    hexapod.init_joint_offsets();
    // Initialize Servo Inversions
    hexapod.init_servo_invertion();
    // Initialize Joints Positions
    hexapod.init_joint_position();
    
    // Initialize Locomotion Parameters
    hexapod.S   = 7;
    hexapod.res = 0.07;
    hexapod.xo  = 12.38;
    hexapod.yo  = 5.0;
    hexapod.zo  = -10.51;
    
    // Walk Selector
    uint32_t walk = 1;
    
    // 
    for (uint32_t i=0; i<hexapod.iteration_size(); i++) {
        hexapod.step(i, walk, 0);
    }

    return 0;
}