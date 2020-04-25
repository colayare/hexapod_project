#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include "globals/axi_ip_globals.h"
#include "globals/linux_timing.h"
#include "hexapod/hexapod_kinematics.h"
#include "hexapod/hexapod_locomotion.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Test Inverse Kinematics IP" << endl;
    
    // Variables Declarations
    // Walk Selector
    uint32_t walk = 1;
    // Walk Step Delay in ms
    uint32_t delay = 100;
    // Inverse Kinematics Parameters Struct
    ik_output_t ik_output;
    
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
    
    // Show IP Status
    cout << "IP : " << endl;
    hexapod.axi_show_regs(0x0, 0x28);
    cout << "-------------" << endl;
    
    // Set var
    uint32_t set_S = 1;
    uint32_t set_res = 1;
    uint32_t set_walk = 1;
    uint32_t set_delay = 1;
    
    // Get Argv
     for(int i = 0; i < argc; i++)
    {
        if ( argv[i] == std::string("-s") ) {
            hexapod.S = atof(argv[i+1]);
            set_S = 0;
        } 
        if ( argv[i] == std::string("-res") ) {
            hexapod.res = atof(argv[i+1]);
            set_res = 0;
        }
        if ( argv[i] == std::string("-w") ) {
            walk = atoi(argv[i+1]);
            set_walk = 0;
        } 
        if ( argv[i] == std::string("-delay") ) {
            delay = atoi(argv[i+1]);
            set_delay = 0;
        } 
    }
    if ( set_S ) {
        hexapod.S = 7.0;
    }
    if ( set_res ) {
        hexapod.res = 0.07;
    }
    if ( set_walk ) {
        walk = 1;
    }
    if ( set_delay ) {
        delay = 10;
    }
    
    // Print Settings
    cout << "Set S=" << hexapod.S << endl;
    cout << "Set res=" << hexapod.res << endl;
    cout << "Set walk=" << walk << endl;
    cout << "Set delay(ms)=" << delay << endl;
    
    // Initialize Locomotion Parameters
    hexapod.xo  = 12.38;
    hexapod.yo  = 5.0;
    hexapod.zo  = -10.51;
    
    cout << " --- START --- " << endl;
    
    // 
    for (uint32_t i=0; i<hexapod.iteration_size(); i++) {
        hexapod.step(i, walk, 0);
        
        delay_ms(delay);
        
        for (uint32_t leg=0; leg<6; leg++) {
            ik_output = hexapod.read_ik_output(leg+1);
            cout << "q1 " << ik_output.q1 << endl;
            cout << "q2 " << ik_output.q2 << endl;
            cout << "q3 " << ik_output.q3 << endl;
        }
    }
    
    cout << " --- END --- " << endl;

    return 0;
}