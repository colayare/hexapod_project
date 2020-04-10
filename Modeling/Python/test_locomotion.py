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

## Initialize Parameters
hexapod.import_init_pos()
hexapod.import_offsets()

## Set Delay
hexapod.delay = 0.008

###############################################################################
#### Locomotion
###############################################################################
## Number of gait iterations
iterations = input()

## Locomotion Parameters
res = 0.1   #iteration resolution
ox  = 5     #x offset
oy  = 5     #y offset 
S   = 5     #y stride length
sx  = 4     #x stride length

## Run Gait
for i in range ( iterations ):
    for j in range(int(S/res)):
        i=res*j;
        if (i<=S/4):
            X1e=12.38;
            Y1e=i-oy;
            Z1e=-10.51;
            X2e=12.38;
            Y2e=-i-oy ;
            Z2e=-(pow(Y2e+oy,2)*64/(S**2)+6.51);
            X3e=12.38;
            Y3e=-i;
            Z3e=-(pow(Y3e,2)*64/(S**2)+6.51);
            X4e=12.38;
            Y4e=i;
            Z4e=-10.51;
            X5e=12.38;
            Y5e=i+oy;
            Z5e=-10.51;
            X6e=12.38;
            Y6e=-i+oy; 
            Z6e=-(pow(Y6e-oy,2)*64/(S**2)+6.51);
            
        if (i>S/4 and i<=3*S/4):
            X1e=12.38;
            Y1e=S/2-i-oy;
            Z1e=-(pow(Y1e+oy,2)*64/(S**2)+6.51);
            X2e=12.38;
            Y2e=i-S/2-oy;
            Z2e=-10.51;
            X3e=12.38;
            Y3e=i-S/2;
            Z3e=-10.51;
            X4e=12.38;
            Y4e=S/2-i ;
            Z4e=-(pow(Y4e,2)*64/(S**2)+6.51);
            X5e=12.38;
            Y5e=S/2-i+oy;  
            Z5e=-(pow(Y5e-oy,2)*64/(S**2)+6.51);
            X6e=12.38;
            Y6e=i-S/2+oy;
            Z6e=-10.51;
     
        if (i>3*S/4 and i<=S):
            X1e=12.38;
            Y1e=i-S-oy;
            Z1e=-10.51;
            X2e=12.38;
            Y2e=S-i-oy;
            Z2e=-(pow(Y2e+oy,2)*64/(S**2)+6.51);
            X3e=12.38;
            Y3e=S-i;
            Z3e=-(pow(Y3e,2)*64/(S**2)+6.51);
            X4e=12.38;
            Y4e=i-S;
            Z4e=-10.51
            X5e=12.38;
            Y5e=i-S+oy;
            Z5e=-10.51; 
            X6e=12.38;
            Y6e=S-i+oy; 
            Z6e=-(pow(Y6e-oy,2)*64/(S**2)+6.51);
        
        X1e=X1e/100; Y1e=Y1e/100; Z1e=Z1e/100;
        X2e=X2e/100; Y2e=Y2e/100; Z2e=Z2e/100;
        X3e=X3e/100; Y3e=Y3e/100; Z3e=Z3e/100;
        X4e=X4e/100; Y4e=Y4e/100; Z4e=Z4e/100;
        X5e=X5e/100; Y5e=Y5e/100; Z5e=Z5e/100;
        X6e=X6e/100; Y6e=Y6e/100; Z6e=Z6e/100;
        hexapod.joints[0] = [X1e, Y1e, Z1e]
        hexapod.joints[1] = [X2e, Y2e, Z2e]
        hexapod.joints[2] = [X3e, Y3e, Z3e]
        hexapod.joints[3] = [X4e, Y4e, Z4e]
        hexapod.joints[4] = [X5e, Y5e, Z5e]
        hexapod.joints[5] = [X6e, Y6e, Z6e]
        hexapod.set_step()
        hexapod.step_delay()
