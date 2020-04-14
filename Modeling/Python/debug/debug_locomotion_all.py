# -*- coding: utf-8 -*-
import os, sys
from math import pi  as pi

###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/../includes')
sys.path.append(abs_path+'/../includes')
from hexapod_debug_class import hexapod_debug as hexapod

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
hexapod.S       = 7.0
hexapod.res     = 0.07
hexapod.xo      = 12.38
hexapod.yo      = 5.0
hexapod.zo      = -10.51

## Step angle
angle_start = -90.0 * pi / 180.0

## Walks set
walks = [1, 2, 3, 4]

###############################################################################
#### Traslational Locomotion
###############################################################################
for i, walk in enumerate(walks):
    print('--- WALK START : '+str(i)+' ---')
    for i in range(18):
        alf = angle_start + ((i*10) * pi / 180.0)
        print('ANGLE:',str(alf))
        for gait in range (2):
            for j in range(int(hexapod.S/hexapod.res)+1):
                hexapod.step(j, walk, alf)
                hexapod.set_step_debug()
                hexapod.step_delay()
        print('--- Kinematics Input START ---')
        print(hexapod.gait_log())
        print('--- Kinematics Input END ---')
        print('--- Kinematics output START ---')
        print(hexapod.joints_log())
        print('--- Kinematics output END ---')
        hexapod.clean_debug()
        print('--- WALK END : '+str(i)+' ---')

hexapod.set_init_position(print_out=False)