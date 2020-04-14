# -*- coding: utf-8 -*-
import os, sys
from math import pi  as pi

###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/includes')
sys.path.append(abs_path+'/includes')
#from hexapod_class import hexapod_kinematics as hexapod
from hexapod_locomotion_class import hexapod_locomotion as hexapod

###############################################################################
#### Hexapod Configuration
###############################################################################
#### Hexapod Object
hexapod = hexapod(invoke_axi_ip=True)

## Import Parameters
hexapod.offsets_file_path            = abs_path+'/params/joint_offset.params'
hexapod.init_position_file_path      = abs_path+'/params/init_position.params'
hexapod.init_servo_inv_file_path     = abs_path+'/params/init_servo_inv.params'

## Initialize Parameters
hexapod.import_init_pos()
hexapod.import_offsets()
hexapod.import_init_servo_invertion()
hexapod.set_default_offsets(print_out=False)
hexapod.set_init_position(print_out=False)

## Set Delay
hexapod.delay = 0.01

## Set Locomotion Parameters
hexapod.S       = float(input('Insert S = '))
hexapod.res     = float(input('Insert res = '))
hexapod.xo      = 12.38
hexapod.yo      = 5.0
hexapod.zo      = -10.51

## Step angle
angle = float(input('Insert Angle(sexa) = '))
alf = angle * pi / 180

## Gait
walk = 1

###############################################################################
#### Traslational Locomotion
###############################################################################
n_gaits = input('Insert number of gaits : ')
for k in range (n_gaits):
    for j in range(int(hexapod.S/hexapod.res)+1):
        hexapod.step(j, walk, alf)
        hexapod.set_step()
        hexapod.step_delay()

hexapod.set_init_position(print_out=False)