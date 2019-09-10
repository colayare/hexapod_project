#!/bin/bash

C_ROOT=$HOME/hexapod/Modeling/C/
C_INC=$C_ROOT/includes/
C_EXE=$C_ROOT/exe/

gcc -o $C_EXE/mem_map_test -I${C_INC} ${C_ROOT}/test_devmem.c