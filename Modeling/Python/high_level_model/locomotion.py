# -*- coding: utf-8 -*-
import os, sys
from math import floor as floor
import numpy as np
import matplotlib.pyplot as plt

###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/../includes')
sys.path.append(abs_path+'/../includes')
from hexapod_class import hexapod as hexapod
from numeric_conversions import numeric_conversions as NUM_CONV

###############################################################################
#### Hexapod Configuration
###############################################################################
#### Hexapod Object
hexapod = hexapod(invoke_axi_ip=False)

## Import Parameters
hexapod.offsets_file_path            = abs_path+'/../params/joint_offset.params'
hexapod.gait_steps_file_path         = abs_path+'/../params/gait_steps.params'
hexapod.init_position_file_path      = abs_path+'/../params/init_position.params'

## Initialize Parameters
hexapod.import_init_pos()
hexapod.import_offsets()

hexapod.read_gait_steps(0)




for i in range(30):
    x, y, z = hexapod.gaits[i]
    t1, t2, t3 = hexapod.iKinematics_t(x, y, z)
    r1, r2, r3 = hexapod.iKinematics(x, y, z)
    e1 = abs((t1-r1)/t1) * 100
    e2 = abs((t2-r2)/t2) * 100
    e3 = abs((t3-r3)/t3) * 100
    d1 = abs(t1 - r1)
    d2 = abs(t2 - r2)
    d3 = abs(t3 - r3)
#    print(e1, e2, e3)
#    print(d1, d2, d3)