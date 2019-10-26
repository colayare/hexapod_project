import sys
sys.path.append('./includes')
import numpy as np
import math as mt
from numeric_conversions import numeric_conversions as NUM_CONV
from hexapod_class import hexapod_kinematics as hc

#### Params Files
joint_offsets_filename = "./params/joint_offset.params"
gait_steps_filename = './params/gait_steps.params'

#### Numeric Conversions
nc = NUM_CONV()

#### Code
## Initialize Hexapod Class
hexapod = hc()
hexapod.init_axi()

## Error counter
error = 0

## Import Parameters
hexapod.offsets_file = joint_offsets_filename
hexapod.gait_steps_file = gait_steps_filename
hexapod.import_offsets()
print('Joint offsets:')
print(hexapod.j_offs)

## Import Gait Seps
hexapod.read_gait_steps(1)

hexapod.config_leg_ctr("one_leg", 0)

print('---- Test Output Direct Writing ------')
for i in range (6):
    hexapod.config_leg_ctr("one_leg", i)
    [x, y, z] = hexapod.gait_step(i)
    hexapod.axi_write_params_in(x, y, z)
    hexapod.axi_write_out_direct()
    hexapod.axi_set_out_mux(i+1)
    
    [q1, q2, q3] = hexapod.axi_read_params()
    err1 = abs(x - int(q1,16))
    err2 = abs(y - int(q2,16))
    err3 = abs(z - int(q3,16))
    tolerance = 0x1000
    if ( err1 > tolerance ):
        print('Q1['+str(i)+'] read fail.')
    if ( err2 > tolerance ):
        print('Q2['+str(i)+'] read fail.')
    if ( err3 > tolerance ):
        print('Q3['+str(i)+'] read fail.')
    print('Read Q1['+str(i)+'] = '+q1+' | Expected = 0x'+hex(x)[2:].zfill(8).rstrip("L"))
    print('Read Q2['+str(i)+'] = '+q2+' | Expected = 0x'+hex(y)[2:].zfill(8).rstrip("L"))
    print('Read Q3['+str(i)+'] = '+q3+' | Expected = 0x'+hex(z)[2:].zfill(8).rstrip("L"))

print('---- Test Gait Calculation ------')

hexapod.axi_set_out_mux(0)
for i in range ( 10 ):
    [x, y, z] = hexapod.gait_step(i)
    hexapod.axi_write_params_in(x, y, z)
    hexapod.axi_write_fifo()
    hexapod.axi_trigger_ikinematics()
    [q1, q2, q3] = hexapod.axi_read_params()
    leg = hexapod.axi_ip.axi_read(1)
    
#    fq1 = nc.hfloat2dfloat(q1[2:10])
#    fq2 = nc.hfloat2dfloat(q2[2:10])
#    fq3 = nc.hfloat2dfloat(q3[2:10])
    
    print(q1, q2, q3, leg)
#    print(fq1, fq2, fq3, leg)
