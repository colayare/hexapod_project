/***************************** Include Files *******************************/
// #include <stdio.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/mman.h>
// #include <reg_map.h>

#ifndef DEVMEM_H_
#define DEVMEM_H_

#ifndef IKINEMATICS_REG_MAP_H_
#define AXI_IKINEMATICS_BASEADDR 0x40000000
#define AXI_IKINEMATICS_HIGHADDR 0x4000FFFF
#define AXI_IKINEMATICS_RMAPSIZE 40
#endif  

extern int *init_ikinematics();
extern int read_ikinematics(int read_address);
extern void write_ikinematics(int write_address, int value);
extern void fifo_ikinematics();
extern void trigger_ikinematics();

#endif // DEVMEM_H_