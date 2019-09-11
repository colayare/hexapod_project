/***************************** Include Files *******************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <reg_map.h>
#include "axi_ikinematics.h"

/************************** Function Definitions ***************************/
extern int *init_ikinematics() {
    int _fdmem;
    int *map = NULL;
    const char memDevice[] = "/dev/mem";

    _fdmem = open( memDevice, O_RDWR | O_SYNC );
    if ( _fdmem < 0 ) {
        printf("Failed to open /dev/mem !\n");
        return 0;
    } else {
        printf("Open /dev/mem successfully !\n");
    }
    
    map = (int *)(mmap(0, AXI_IKINEMATICS_RMAPSIZE*4, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, AXI_IKINEMATICS_BASEADDR));
    
    return map;
}