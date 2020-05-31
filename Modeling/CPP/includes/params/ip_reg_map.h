//-- IPs Register Map
#ifndef __IP_REG_MAP__
#define __IP_REG_MAP__

//-- Inverse Kinematics IP
#define AXI_IK_0_DEVICE_ID  0
#define AXI_IK_BASEADDR     0x40000000
#define AXI_IK_HIGHADDR     0x4000FFFF
#define AXI_IK_PAGESIZE     AXI_IK_HIGHADDR - AXI_IK_BASEADDR + 1
#define AXI_IK_RMAPSIZE     40
#define AXI_IK_WORDSIZE     4

//-- Inverse Kinematics IP
#define AXI_TMR_0_DEVICE_ID 0
#define AXI_TMR0_BASEADDR   0x40010000
#define AXI_TMR0_HIGHADDR   0x4001FFFF
#define AXI_TMR0_PAGESIZE   AXI_TMR0_HIGHADDR - AXI_TMR0_BASEADDR + 1
#define AXI_TMR0_RMAPSIZE   40
#define AXI_TMR0_WORDSIZE   4

//-- I2C 
#define AXI_I2C0_DEVICE_ID  0
#define AXI_I2C0_BASEADDR   0xE0004000
#define AXI_I2C0_HIGHADDR   0xE0004028
#define AXI_I2C0_PAGESIZE   0x1000
#define AXI_I2C0_RMAPSIZE   18
#define AXI_I2C0_WORDSIZE   1




#endif //__IP_REG_MAP__
