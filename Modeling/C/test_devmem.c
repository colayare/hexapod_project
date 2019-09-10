#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
//--
#include <reg_map.h>

//-- Testing access to iKinematics IP register map
int main() {
        //-
        printf("Testing /dev/mem/ iKinematics Access.\n");
        printf("AXI_IKINEMATICS_BASEADDR=0x%x\n", AXI_IKINEMATICS_BASEADDR);
        printf("AXI_IKINEMATICS_HIGHADDR=0x%x\n", AXI_IKINEMATICS_HIGHADDR);
        printf("AXI_IKINEMATICS_RMAPSIZE=0x%x\n", AXI_IKINEMATICS_RMAPSIZE);

        int _fdmem;
        int *map = NULL;
        const char memDevice[] = "/dev/mem";

        // Open /dev/mem and error checking
        _fdmem = open( memDevice, O_RDWR | O_SYNC );
        if ( _fdmem < 0 ) {
                printf("Failed to open /dev/mem !\n");
                return 0;
        } else {
                printf("Open /dev/mem successfully !\n");
        }

        // mmap() the opened /dev/mem
        map = (int *)(mmap(0, AXI_IKINEMATICS_RMAPSIZE*4, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, AXI_IKINEMATICS_BASEADDR));

        // map ptr to access mapped area
        for (int i=0;i<5;i++)
                printf("idx[%d]\t:0x%x : %f\n", i, *(map+i), * (float *)&*(map+i));

        return 0;
}
