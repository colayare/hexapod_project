#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "axi_ip_globals.h"
#include "data_types.h"
#include "ikinematics_ip.h"

using namespace std;

int main() {
    cout << "Test Inverse Kinematics IP" << endl;
    
    ikinematics_ip = ikinematics_ip_context();
    
    ikinematics_ip.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
    
    ikinematics_ip.show_regs(0, 0x23);

    return 0;
}