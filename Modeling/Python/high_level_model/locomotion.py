# -*- coding: utf-8 -*-
import os, sys
#from math import floor as floor
import numpy as np
import matplotlib.pyplot as plt
from math import sin as sin
from math import cos as cos
from math import pi as pi
###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/../includes')
sys.path.append(abs_path+'/../includes')
from hl_hexapod_class import *
from numeric_conversions import numeric_conversions as NUM_CONV

###############################################################################
#### Hexapod Configuration
###############################################################################
#### Hexapod Object
hexapod = hexapod(invoke_axi_ip=False)

## Import Parameters
hexapod.offsets_file_path            = abs_path+'/params/joint_offset.params'
hexapod.init_position_file_path      = abs_path+'/params/init_position.params'

## Initialize Parameters
hexapod.import_init_pos()
hexapod.import_offsets()

## Set Locomotion Parameters
hexapod.S       = 7
hexapod.res     = 0.07
hexapod.xo      = 12.38
hexapod.yo      = 5.0
hexapod.zo      = -10.51

## Step angle
angle = 0
alf = angle * pi / 180

## Gait
walk = 3

## Number of Gaits
n_gaits = 2

for k in range ( n_gaits ):
    for j in range(int(hexapod.S/hexapod.res)):
        i=hexapod.res*j;
        hexapod.step(j, walk, alf)
        hexapod.get_step()

hexapod.plot_kinematics()
hexapod.plot_ikinematics()
hexapod.plot_gait()
