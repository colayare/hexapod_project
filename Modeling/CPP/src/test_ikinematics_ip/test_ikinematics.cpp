#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include "globals/axi_ip_globals.hpp"
#include "hexapod/hexapod_kinematics.hpp"

#define __LOG

using namespace std;

int main() {
    cout << "Test Inverse Kinematics IP" << endl;
    
    ikinematics_ip_context iK_ip;
    
    iK_ip.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
    
    iK_ip.axi_show_regs(0, 0x23);
    
    iK_ip.set_joint_offsets();
    
    iK_ip.axi_show_regs(0, 0x23);

    return 0;
}