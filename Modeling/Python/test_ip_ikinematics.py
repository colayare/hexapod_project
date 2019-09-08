import sys
sys.path.append('./includes')
import numpy as np
import math as mt
import numeric_conversions as nc
from hexapod_class import hexapod_kinematics as hc
from devmem import axi_ikinematics_ip as iKinematics_IP

#### Params Files
joint_offsets_filename = "./params/joint_offset.params"
gait_steps_filename = './params/gait_steps.params'

#### Code
## Initialize Hexapod Class
hexapod = hc()
## Initialize devmem Class
axi_ip = iKinematics_IP()
axi_ip.base_address = 0x40000000

## Import Parameters
hexapod.offsets_file = joint_offsets_filename
hexapod.gait_steps_file = gait_steps_filename
hexapod.import_offsets()
print('Joint offsets:')
print(hexapod.j_offs)

## Import Gait Seps
hexapod.read_gait_steps()

for i in range ( 30 ):
    [x, y, z] = hexapod.gait_step(i)
    axi_ip.axi_write(2, int(x, 16))
    axi_ip.axi_write(3, int(y, 16))
    axi_ip.axi_write(4, int(z, 16))
    axi_ip.write_fifo()
    axi_ip.trigger_iKinematics()
    q1 = axi_ip.axi_read(2)
    q2 = axi_ip.axi_read(3)
    q3 = axi_ip.axi_read(4)
    print(q1, q2, q3)
