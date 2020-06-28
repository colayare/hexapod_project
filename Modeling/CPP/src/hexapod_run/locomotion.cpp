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
#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"

#define TMR_10MS  100000
#define TMR_TMOUT TMR_10MS * 2
//#define USE_OLED

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Test Inverse Kinematics IP" << endl;
    
		//=================================================================
		// AXI TIMER IP CONTEXT 
		//=================================================================
		// Declare AXI TMR IP CONTEXT
		ip_context axi_tmr0(AXI_TMR0_UIO);
		// Assign AXI IP Name
		axi_tmr0.ip_name = "axi_tmr0";
		// Initialize AXI TMR IP 
		axi_tmr0.init_axi_mmap_ptr(AXI_TMR0_RMAPSIZE, AXI_TMR0_BASEADDR * AXI_TMR_0_DEVICE_ID, AXI_TMR0_WORDSIZE);
		// Configure AXI TMR
		axi_tmr0.axi_bit_set(0, 1 << 1);    // Select Down Counter
		axi_tmr0.axi_write(1, TMR_10MS);		// Load CTR value to Load Reg
    axi_tmr0.axi_bit_set(0, 1 << 5);		// Load TMR0
    axi_tmr0.axi_bit_clr(0, 1 << 5);
		axi_tmr0.axi_bit_set(0, 1 << 6);		// Enable TMR0 Interrupt

		//=================================================================

		//=================================================================
		// HEXAPOD IP CONTEXT
		//=================================================================
    // Declare Inverse Kinematics AXI IP Context
    hexapod_locomotion hexapod(AXI_IK_UIO);
    // Assign AXI IP Name
    hexapod.ip_name = "hexapod";
    // Initialize Inverse Kinematics IP
    hexapod.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_MMAPADDR * AXI_IK_0_DEVICE_ID, AXI_IK_WORDSIZE);
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
		hexapod.S 	= 7.0;
		hexapod.res	= 0.07;
		//=================================================================

		//=================================================================
		// I2C OLED CONTEXT
		//=================================================================
    #ifdef USE_OLED
    // Declare I2C OLED Context
    i2c_oled oled("/dev/i2c-0");
    // Set I2C OLED Address
    oled.set_address(SSD1306_ADDRESS);
    // Initialize I2C OLED
    oled.init();
    // Declare String variable
    char str[20];
    strncpy(str, "Crixus 2.0", sizeof(str));
    oled.set_cursor(0,0);
    oled.disp_str((uint8_t *) &str);
    strncpy(str, "Step:", sizeof(str));
    oled.set_cursor(1,0);
    oled.disp_str((uint8_t *) &str);
    strncpy(str, "x:", sizeof(str));
    oled.set_cursor(2,0);
    oled.disp_str((uint8_t *) &str);
    strncpy(str, "y:", sizeof(str));
    oled.set_cursor(3,0);
    oled.disp_str((uint8_t *) &str);
    strncpy(str, "z:", sizeof(str));
    oled.set_cursor(4,0);
    oled.disp_str((uint8_t *) &str);
    #endif

    
		//=================================================================
		// Get Arguments
		//=================================================================
    // Variables Declarations
    // Walk Selector
		// 0 : Forbiden 
		// 1 : Tripod
		// 2 : Quadruped
		// 3 : Quadruped 4+2
		// 4 : Pentapod
    uint32_t walk = 1;
    // Walk Step Delay in ms
    uint32_t delay = 10;
    // Number of gaits
    uint32_t gaits = 2;
    // Get Argv
    for(int i = 0; i < argc; i++) {
        if ( argv[i] == std::string("-S") ) {
            hexapod.S = atof(argv[i+1]);
        } 
        if ( argv[i] == std::string("-res") ) {
            hexapod.res = atof(argv[i+1]);
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
		//=================================================================
    
		//=================================================================
		// RUN
		//=================================================================
    for (uint32_t gait=0; gait<gaits; gait++) {
			  cout << RED << "##### GAIT N " << gait << RESET << endl;
        for (uint32_t i=0; i<hexapod.iteration_size(); i++) {
            cout << GREEN << "step : " << i << RESET << endl;
						// Disable TMR0 Count
						axi_tmr0.axi_bit_clr(0, 1 << 7);		
						// Calculate & Send hexapod walk step
            hexapod.step(i, walk, 0);
            // Load TMR ctr value
            axi_tmr0.axi_bit_set(0, 1 << 5);
            axi_tmr0.axi_bit_clr(0, 1 << 5);
						// Enable TMR0 Start Count
            axi_tmr0.axi_bit_set(0, 1 << 7);
            // During TMR0 Count display status
            #ifdef USE_OLED
            oled.set_cursor(1, C_FONT_SIZE * 6);
            sprintf(str, "%d", gait);
            oled.disp_str((uint8_t *) &str);
            //oled.set_cursor(2, C_FONT_SIZE * 3);
            //sprintf(str, "%.3f", hexapod.ef_position.leg[0].x);
            //oled.disp_str((uint8_t *) &str);
            //oled.set_cursor(3, C_FONT_SIZE * 3);
            //sprintf(str, "%.3f", hexapod.ef_position.leg[0].y);
            //oled.disp_str((uint8_t *) &str);
            //oled.set_cursor(4, C_FONT_SIZE * 3);
            //sprintf(str, "%.3f", hexapod.ef_position.leg[0].z);
            //oled.disp_str((uint8_t *) &str);
            #endif
						// Poll AXI TMR0 Interrupt
            axi_tmr0.axi_wait_mask(0, 1 << 8, 1 << 8, TMR_TMOUT);
						// Clear TMR0 Interrupt Flag
            axi_tmr0.axi_bit_set(0, 1 << 8);
        }
    }
		//=================================================================

    // Return Joints to initial position
    hexapod.init_joint_position();
    
    return 0;
}

