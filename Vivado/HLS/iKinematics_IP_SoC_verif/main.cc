/*
 * Empty C++ Application
 */

#include <iostream>
#include "xil_printf.h"
#include "ip_context.h"
#include "iKinematics_IP.h"
#include "iKinematics_IP_params.h"

int main()
{
  ikinematics_ip_context hexapod;

  // Initialize Inverse Kinematics IP
  hexapod.axi_init(AXI_IK_BASEADDR, AXI_IK_PAGESIZE, AXI_IK_WORDSIZE);
  
  // Initialize Joints Offsets
  std::cout << "Initialize Joints Offsets" << std::endl;
  hexapod.init_joint_offsets();
  
  // Initialize Servo Inversions
  std::cout << "Initialize Servo Inversions" << std::endl;
  hexapod.init_servo_invertion();
  
  // Initialize Joints Positions
  std::cout << "Initialize Joints Positions" << std::endl;
  hexapod.init_joint_position();

  hexapod.axi_show_regs(0, 40);
  
  std::cout << "End of test." << std::endl;
  return 0;
}
