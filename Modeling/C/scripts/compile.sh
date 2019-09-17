#!/bin/bash

C_ROOT=$HOME/hexapod/Modeling/C/
C_INC=$C_ROOT/includes/
C_EXE=$C_ROOT/outputs/

####
#gcc -o $C_EXE/mem_map_test -I${C_INC} ${C_ROOT}/test_devmem.c

####
# Includes
gcc -c -o $C_EXE/axi_ikinematics.o -I$C_INC $C_INC/axi_ikinematics.c
gcc -c -o $C_EXE/hexapod_gaits.o -I$C_INC $C_INC/hexapod_gaits.c

# Source code
gcc -c -o $C_EXE/ikinematics_gait_test.o -I$C_INC $C_ROOT/ikinematics_gait_test.c
gcc -o $C_EXE/ikinematics_gait_test $C_EXE/axi_ikinematics.o $C_EXE/hexapod_gaits.o $C_EXE/ikinematics_gait_test.o

