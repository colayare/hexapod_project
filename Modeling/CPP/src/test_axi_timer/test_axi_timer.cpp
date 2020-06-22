#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include "globals/global_defines.h"
#include "globals/axi_ip_globals.h"
#include "globals/linux_timing.h"
#include "params/ip_reg_map.h"
#include "hexapod/hexapod_kinematics.h"
#include "hexapod/hexapod_locomotion.h"

#define TMR_10MS  1000000
//#define TMR_10MS  1000
#define TMR_TMOUT   100000000

using  namespace std;

void reset_axi_tmr(ip_context axi_tmr_context) {
  for (int i=0; i < 20; i++) {
    axi_tmr_context.axi_write(i, 0);
  }
}

int main(void) {
  cout << "Testing AXI TIMER 0" << endl;

	//**** Instantiate Hexapod Context ****
  cout << "Initialize iKinematics Context" << endl;
	//hexapod_locomotion hexapod("/dev/mem");
	hexapod_locomotion hexapod(AXI_IK_UIO);
	hexapod.ip_name = "hexapod";

	// Initialize Inverse Kinematics IP
	hexapod.init_axi_mmap_ptr(AXI_IK_RMAPSIZE, AXI_IK_BASEADDR, AXI_IK_WORDSIZE);
	// Initialize Joints Offsets
  cout << "init_joint_offsets" << endl;
	hexapod.init_joint_offsets();
	// Initialize Servo Inversions
  cout << "init_servo_invertion" << endl;
	hexapod.init_servo_invertion();
	// Initialize Joints Positions
  cout << "init_joint_position" << endl;
	hexapod.init_joint_position();

  hexapod.axi_show_regs(0, 43);


	// Initialize Locomotion Parameters
	hexapod.S		= 7.0;
	hexapod.res	= 0.07;
	hexapod.xo	= 12.38;
	hexapod.yo	= 5.0;
	hexapod.zo	= -10.51;
	//********

  //**** Initialize AXI TMR0 Context ****
  cout << "Initialize AXI Timer0 Context" << endl;
	ip_context axi_tmr0(AXI_TMR0_UIO);
  axi_tmr0.ip_name = "AXI Timer 0";
  axi_tmr0.init_axi_mmap_ptr(AXI_TMR0_RMAPSIZE, AXI_TMR0_BASEADDR, AXI_TMR0_WORDSIZE);
	//********
  
	// Test Register Access
  cout << "Test Register Access" << endl;
  axi_tmr0.axi_write(1, 0x55aa);
  uint32_t read_val;
  read_val = axi_tmr0.axi_read(1);

	// Test AXI TMR0 3000ms
  reset_axi_tmr(axi_tmr0);
	cout << "Configure AXI TMR0 : 3000ms" << endl;
	axi_tmr0.axi_write(1, UINT32_MAX - 3 * 100000000);
	axi_tmr0.axi_bit_set(0, 1 << 5);		// Load TMR0
	axi_tmr0.axi_bit_clr(0, 1 << 5);
	axi_tmr0.axi_bit_set(0, 1 << 6);		// Enable TMR0 Interrupt
	axi_tmr0.axi_read(2);								// Check CTR reg
	axi_tmr0.axi_bit_set(0, 1 << 7); 		// Enable TMR0 Start Count
  axi_tmr0.axi_read(2);
  axi_tmr0.axi_read(2);
  axi_tmr0.axi_wait_mask(0, 1 << 8, 1 << 8, 5 * TMR_TMOUT);
	axi_tmr0.axi_bit_set(0, 1 << 8);
  axi_tmr0.axi_read(1);
  axi_tmr0.axi_read(2);
	cout << "Timer Overflow" << endl;

  //**** Test Speed ****
  reset_axi_tmr(axi_tmr0);
  cout << "Configure AXI TMR0 : 10ms" << endl;
  axi_tmr0.axi_bit_set(0, 1 << 1);    // Select Down Counter
  axi_tmr0.axi_write(1, TMR_10MS);
  for (int i=0; i<20; i++) {
    axi_tmr0.axi_bit_set(0, 1 << 5);		// Load TMR0
    axi_tmr0.axi_bit_clr(0, 1 << 5);
    axi_tmr0.axi_bit_set(0, 1 << 6);		// Enable TMR0 Interrupt
    axi_tmr0.axi_bit_set(0, 1 << 7); 		// Enable TMR0 Start Count
    axi_tmr0.axi_wait_mask(0, 1 << 8, 1 << 8, TMR_TMOUT);
    //axi_tmr0.axi_bit_set(0, 1 << 8);
    cout << "Timer Overflow" << endl;
  }


	//**** Test iKinematics - AXI TMR0 Integration ****
  reset_axi_tmr(axi_tmr0);
	cout << "Test iKinematics Trigger AXI TMR0" << endl;
	cout << "Configure AXI TMR0 : 10ms" << endl;
  axi_tmr0.axi_bit_set(0, 1 << 1);    // Select Down Counter
  axi_tmr0.axi_write(1, TMR_10MS);
  axi_tmr0.axi_bit_set(0, 1 << 6);		// Enable TMR0 Interrupt
	// Gaits
	uint32_t walk = 1;
	uint32_t gaits = 10;
	for (uint32_t gait=0; gait<gaits; gait++) {
			cout << RED << "##### GAIT N " << gait << RESET << endl;
			for (uint32_t i=0; i<hexapod.iteration_size(); i++) {
          cout << GREEN << "step : " << i << RESET << endl;
					hexapod.step(i, walk, 0);
          axi_tmr0.axi_bit_set(0, 1 << 5);		// Load TMR0
          axi_tmr0.axi_bit_clr(0, 1 << 5);
          axi_tmr0.axi_bit_set(0, 1 << 7); 		// Enable TMR0 Start Count
          axi_tmr0.axi_wait_mask(0, 1 << 8, 1 << 8, TMR_TMOUT);
          axi_tmr0.axi_bit_set(0, 1 << 8);
			}
	}

	cout << "End of Test." << endl;
  return 0;
}
