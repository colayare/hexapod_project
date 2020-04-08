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

offxet_x = 0.05
offset_y = 0.05

hexapod.offset_x = offxet_x
hexapod.offset_y = offset_y


for i in range(30):
    x, y, z = hexapod.gaits[i]
    hexapod.set_step(0, [x, y, z])
    hexapod.set_step(1, [x, y, z])
    hexapod.set_step(2, [x, y, z])
    hexapod.set_step(3, [x, y, z])
    hexapod.set_step(4, [x, y, z])
    hexapod.set_step(5, [x, y, z])

hexapod.plot_gait()