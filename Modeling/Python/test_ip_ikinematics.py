import sys
sys.path.append('./includes')
import numpy as np
import math as mt
from numeric_conversions import numeric_conversions as NUM_CONV
from hexapod_class import hexapod_kinematics as hc

#### Params Files
joint_offsets_filename = "./params/joint_offset.params"
gait_steps_filename = './params/gait_steps.params'

####
nc = NUM_CONV()

#### Code
## Initialize Hexapod Class
hexapod = hc()
#hexapod.init_axi()

## Import Parameters
hexapod.offsets_file = joint_offsets_filename
hexapod.gait_steps_file = gait_steps_filename
hexapod.import_offsets()
print('Joint offsets:')
print(hexapod.j_offs)

## Import Gait Seps
hexapod.read_gait_steps(1)

#hexapod.config_leg_ctr("mux_leg", 3)


#for i in range ( 10 ):
#    [x, y, z] = hexapod.gait_step(i)
#    hexapod.axi_ip.axi_write(2, x)
#    hexapod.axi_ip.axi_write(3, y)
#    hexapod.axi_ip.axi_write(4, z)
#    hexapod.axi_write_fifo()
#    hexapod.axi_trigger_ikinematics()
#    q1 = hex(nc.to_int(hexapod.axi_ip.axi_read(2)))
#    q2 = hex(nc.to_int(hexapod.axi_ip.axi_read(3)))
#    q3 = hex(nc.to_int(hexapod.axi_ip.axi_read(4)))
##    q1 = nc.bytes2dfloat(hexapod.axi_ip.axi_read(2))
##    q2 = nc.bytes2dfloat(hexapod.axi_ip.axi_read(3))
##    q3 = nc.bytes2dfloat(hexapod.axi_ip.axi_read(4))
#    leg = hex(nc.to_int(hexapod.axi_ip.axi_read(1)))
#    print(q1, q2, q3, leg)
