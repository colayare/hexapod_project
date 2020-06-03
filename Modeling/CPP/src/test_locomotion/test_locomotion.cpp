#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include "globals/global_defines.h"
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
    uint32_t delay = 10;
    // Number of gaits
    uint32_t gaits = 10;
    
    // Declare Inverse Kinematics AXI IP Context
    hexapod_locomotion hexapod("/dev/mem");
    
    // Assign AXI IP Name
    hexapod.ip_name = "IKinematics";
    
    // Initialize Inverse Kinematics IP
    hexapod.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
    cout << hexapod.ip_name << " IP ptr : " << hexapod.get_mmap_ptr() << endl;
    
    // Initialize Joints Offsets
    hexapod.init_joint_offsets();
    // Initialize Servo Inversions
    hexapod.init_servo_invertion();
    // Initialize Joints Positions
    hexapod.init_joint_position();
    
    // Set var
    uint32_t set_S = 1;
    uint32_t set_res = 1;
    
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
        } 
        if ( argv[i] == std::string("-delay") ) {
            delay = atoi(argv[i+1]);
        }
        if ( argv[i] == std::string("-gaits") ) {
            gaits = atoi(argv[i+1]);
        }
    }
    if ( set_S ) {
        hexapod.S = 7.0;
    }
    if ( set_res ) {
        hexapod.res = 0.07;
    }
    // Print Settings
    cout << "Set S=" << hexapod.S << endl;
    cout << "Set res=" << hexapod.res << endl;
    cout << "Set walk=" << walk << endl;
    cout << "Set delay(ms)=" << delay << endl;
    cout << "Gaits=" << gaits << endl;
    
    // Initialize Locomotion Parameters
    hexapod.xo  = 12.38;
    hexapod.yo  = 5.0;
    hexapod.zo  = -10.51;
    
    // Gaits
    for (uint32_t gait=0; gait<gaits; gait++) {
			  cout << RED << "##### GAIT N " << gait << RESET << endl;
        for (uint32_t i=0; i<hexapod.iteration_size(); i++) {
            cout << GREEN << "step : " << i << RESET << endl;
            hexapod.step(i, walk, 0);
            
            delay_ms(delay);
        }
    }
    
    return 0;
}
