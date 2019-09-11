#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "reg_map.h"
#include "axi_ikinematics.h"

int main() {
    printf("iKinematics IP Gait Test");
    
    int *map = init_ikinematics();
    
    for (int i=0;i<5;i++)
        printf("idx[%d]\t:0x%x : %f\n", i, *(map+i), * (float *)(map+i));
    
    return 0;
}