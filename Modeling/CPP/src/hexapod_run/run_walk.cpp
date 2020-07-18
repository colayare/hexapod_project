#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <cstring>
#include "globals/global_defines.h"
#include "globals/axi_ip_globals.h"
#include "hexapod/hexapod_kinematics.h"
#include "hexapod/hexapod_locomotion.h"
#include "i2c_peripherals/i2c_arduino_controller.h"
#include "i2c_peripherals/i2c_arduino_controller_params.h"
#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"

#define TMR_1MS   100000
#define TMR_10MS  TMR_1MS * 10
#define TMR_1S    TMR_1MS * 1000
#define TMR_TMOUT TMR_1S * 3
#define MAX_STEP  100
//#define USE_OLED

using namespace std;

int main(int argc, char* argv[]) {
    //=================================================================
    // Get Arguments
    //=================================================================
    int32_t   opt;
    uint32_t  walk  = 1;
    uint32_t  delay = 10;
    float     hexapod_S = 7.0; 
    float     hexapod_res = 0.07;
    while ((opt = getopt(argc, argv, "s:hw:hr:hd:hh")) != -1 ) {
      switch(opt) {
          case 's':
            hexapod_S = static_cast<float>(atof(optarg));
            break;
          case 'w':
            if ( atoi(optarg) >= 1 && atoi(optarg) < 5 ) 
              walk = static_cast<uint32_t>(atoi(optarg));
            else {
              cout << "Invalid Walk value : " << optarg << endl;
              exit(1);
            }
            break;
          case 'r':
            hexapod_res = static_cast<float>(atof(optarg));
            break;
          case 'd':
            delay = static_cast<uint32_t>(atoi(optarg));
            break;
          case 'h':
            cout << "Valid options:" << endl;
            cout << "-s :\tSet Hexapod 'S' parameter.\n\tFormat: float." << endl;
            cout << "-r :\tSet gait resolution, number of points (samples) that gait is composed.\n\tFormat: float" << endl;
            cout << "-w :\tSet Gait type." << endl;
            cout << "\tFormat : unsigned integer." << endl;
            cout << "\t\tValues :" << endl;
            cout << "\t\t1 : Tripod Gait." << endl;
            cout << "\t\t2 : Quadruped Gait." << endl;
            cout << "\t\t3 : Quadruped 4+2 Gait." << endl;
            cout << "\t\t4 : Pentapod Gait." << endl;
            cout << "-d :\tSet delay (ms) for gait step.\n\tFormat: unsigned integer." << endl;
            cout << "-g :\tSet number of gaits to execute.\n\tFormat: unsigned integer." << endl;
            exit(1);
            break;
          default:
            cout << "Invalid Option -" << optarg << ". use option -h for help." << endl;
            break;
      }
    }
    cout << "Parameters:" << endl;
    cout << "S=" << hexapod_S << endl;
    cout << "res=" << hexapod_res << endl;
    cout << "walk=" << walk << endl;
    cout << "delay(ms)=" << delay << endl;
    //=================================================================

    //=================================================================
    // AXI TIMER IP CONTEXT 
    //=================================================================
    // Declare AXI TMR IP CONTEXT
    ip_context axi_tmr0("axi_tmr0");
    // Initialize AXI TMR IP 
    axi_tmr0.init_axi_mmap_ptr(AXI_TMR0_UIO, AXI_TMR0_BASEADDR * AXI_TMR_0_DEVICE_ID, AXI_TMR0_RMAPSIZE);
    // Configure AXI TMR
    axi_tmr0.axi_bit_set(0, 1 << 1);          // Select Down Counter
    axi_tmr0.axi_write(1, delay * TMR_1MS);		// Load CTR value to Load Reg
    axi_tmr0.axi_bit_set(0, 1 << 5);		      // Load TMR0
    axi_tmr0.axi_bit_clr(0, 1 << 5);
    axi_tmr0.axi_bit_set(0, 1 << 6);		      // Enable TMR0 Interrupt
    //=================================================================

    //=================================================================
    // ARDUINO I2C CONTROLLER
    //=================================================================
    i2c_arduino_controller joystick("/dev/i2c-0");
    joystick.set_address(ARDUINO_I2C_ADDR);
    joystick.get_joystick();
    //=================================================================

    //=================================================================
    // HEXAPOD IP CONTEXT
    //=================================================================
    // Declare Inverse Kinematics AXI IP Context
    hexapod_locomotion hexapod("ikenamtics_ip");
    // Initialize Inverse Kinematics IP
    hexapod.init_axi_mmap_ptr(AXI_IK_UIO, AXI_IK_MMAPADDR * AXI_IK_0_DEVICE_ID, AXI_IK_RMAPSIZE);
    // Initialize Joints Offsets
    hexapod.init_joint_offsets();
    // Initialize Servo Inversions
    hexapod.init_servo_invertion();
    // Initialize Joints Positions
    hexapod.init_joint_position();
    // Initialize Locomotion Parameters
    hexapod.xo  = 12.38;		//
    hexapod.yo  = 5.0;
    hexapod.zo  = -10.51;
    hexapod.S 	= hexapod_S;
    hexapod.res	= hexapod_res;
    //=================================================================

    //=================================================================
    // RUN
    //=================================================================
    const uint32_t max_step = static_cast<uint32_t>(hexapod_S/hexapod_res);
    uint32_t step = 0;
    while (1) {
        // Get Joystick Position
        joystick.get_joystick();
        // Disable TMR0 Count
        axi_tmr0.axi_bit_clr(0, 1 << 7);		
        // Calculate & Send hexapod walk step
        if ( joystick.dJoystick() ) {
          cout << GREEN << "step : " << step << RESET << "\tdir : " << joystick.dJoystick() << "\tangle : " << joystick.xAngle() << endl;
          hexapod.step(step, walk, joystick.xAngle());
          if ( step < max_step-1 )
              step++;
          else
              step = 0;
        }
        // Load TMR ctr value
        axi_tmr0.axi_bit_set(0, 1 << 5);
        axi_tmr0.axi_bit_clr(0, 1 << 5);
        // Enable TMR0 Start Count
        axi_tmr0.axi_bit_set(0, 1 << 7);
    
        // Poll AXI TMR0 Interrupt
        axi_tmr0.axi_wait_mask(0, 1 << 8, 1 << 8, TMR_TMOUT);
        // Clear TMR0 Interrupt Flag
        axi_tmr0.axi_bit_set(0, 1 << 8);
    }



    return 0;
}
