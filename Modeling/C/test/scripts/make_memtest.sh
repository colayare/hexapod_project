#!/bin/bash

C_ROOT=$HOME/hexapod/Modeling/C/
C_INC=$C_ROOT/includes/
C_EXE=$C_ROOT/exe/

####
gcc -o $C_EXE/mem_map_test -I${C_INC} ${C_ROOT}/test_devmem.c

####
gcc -c -o $C_EXE/axi_ikinematics.o -I$C_INC $C_INC/axi_ikinematics.c
gcc -c -o $C_EXE/ikinematics_gait_test.o -I$C_INC $C_ROOT/ikinematics_gait_test.c
gcc -o $C_EXE/ikinematics_gait $C_EXE/axi_ikinematics.o $C_EXE/ikinematics_gait_test.o
