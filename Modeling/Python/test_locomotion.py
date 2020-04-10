# -*- coding: utf-8 -*-
import os, sys
import numpy as np

###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/includes')
sys.path.append(abs_path+'/includes')
from hexapod_class import hexapod_kinematics as hexapod

###############################################################################
#### Hexapod Object
###############################################################################
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

###############################################################################
#### Locomotion
###############################################################################
## Number of gait iterations
iterations = input('Number of gaits : ')

## Locomotion Parameters
res = 0.1   #iteration resolution
ox  = 5     #x offset
oy  = 5     #y offset 
S   = 10     #y stride length
sx  = 4     #x stride length

p1=np.array([0,0,0],np.float32);
p2=np.array([0,0,0],np.float32);
p3=np.array([0,0,0],np.float32);
p4=np.array([0,0,0],np.float32);
p5=np.array([0,0,0],np.float32);
p6=np.array([0,0,0],np.float32);

## Run Gait
for k in range ( iterations ):
    for j in range(int(S/res)):
        i=res*j;
        if (i<=S/4):
            p1[0]=12.38;    p1[1]=i-oy;     p1[2]=-10.51;
            p2[0]=12.38;    p2[1]=-i-oy;    p2[2]=-(pow(p2[1]+oy,2)*64/(S**2)+6.51);
            p3[0]=12.38;    p3[1]=-i;       p3[2]=-(pow(p3[1],2)*64/(S**2)+6.51);
            p4[0]=12.38;    p4[1]=i;        p4[2]=-10.51;
            p5[0]=12.38;    p5[1]=i+oy;     p5[2]=-10.51;
            p6[0]=12.38;    p6[1]=-i+oy;    p6[2]=-(pow(p6[1]-oy,2)*64/(S**2)+6.51);
            
        if (i>S/4 and i<=3*S/4):
            p1[0]=12.38;    p1[1]=S/2-i-oy; p1[2]=-(pow(p1[1]+oy,2)*64/(S**2)+6.51);
            p2[0]=12.38;    p2[1]=i-S/2-oy; p2[2]=-10.51;
            p3[0]=12.38;    p3[1]=i-S/2;    p3[2]=-10.51;
            p4[0]=12.38;    p4[1]=S/2-i ;   p4[2]=-(pow(p4[1],2)*64/(S**2)+6.51);
            p5[0]=12.38;    p5[1]=S/2-i+oy; p5[2]=-(pow(p5[1]-oy,2)*64/(S**2)+6.51);
            p6[0]=12.38;    p6[1]=i-S/2+oy; p6[2]=-10.51;
     
        if (i>3*S/4 and i<=S):
            p1[0]=12.38;    p1[1]=i-S-oy;   p1[2]=-10.51;
            p2[0]=12.38;    p2[1]=S-i-oy;   p2[2]=-(pow(p2[1]+oy,2)*64/(S**2)+6.51);
            p3[0]=12.38;    p3[1]=S-i;      p3[2]=-(pow(p3[1],2)*64/(S**2)+6.51);
            p4[0]=12.38;    p4[1]=i-S;      p4[2]=-10.51;
            p5[0]=12.38;    p5[1]=i-S+oy;   p5[2]=-10.51; 
            p6[0]=12.38;    p6[1]=S-i+oy;   p6[2]=-(pow(p6[1]-oy,2)*64/(S**2)+6.51);
        
        p1 /=100; p2 /=100; p3 /=100; p4 /=100; p5 /=100; p6 /=100;
        
        hexapod.set_step(0, p1)
        hexapod.set_step(1, p2)
        hexapod.set_step(2, p3)
        hexapod.set_step(3, p4)
        hexapod.set_step(4, p5)
        hexapod.set_step(5, p6)
        hexapod.set_step()
        hexapod.step_delay()

hexapod.set_init_position(print_out=False)