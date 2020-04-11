# -*- coding: utf-8 -*-
import os, sys
#from math import floor as floor
#import numpy as np
from math import pi as PI
from math import sin as sin
from math import cos as cos

###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/includes')
sys.path.append(abs_path+'/includes')
from hexapod_class import hexapod_kinematics as hexapod

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

## Locomotion Parameters
res = 0.1           #iteration resolution
# ox  = 5           #x offset
# oy  = 5           #y offset 
S   = 10            #y stride length
# sx  = 4           #x stride length
xo  = 12.38
yo  = 5
zo  = -10.51
alf = 0 * PI/180;

## Selección de caminata
walk = 1

###############################################################################
#### Locomotion
###############################################################################
## Number of gait iterations
n_gaits = input('Number of gaits : ')

## Gait loop
for k in range ( n_gaits ):
    for j in range(int(S/res)):
        i=res*j;
        if(walk==1):
            ## Primera Etapa: 0<i<=S/2
            if i<=S/2:
                x1=xo+i*sin(-alf);          y1=yo+i*cos(-alf);  
                x2=xo+(S/2-i)*sin(alf);     y2=yo+(S/2-i)*cos(alf);       z2=zo;
                x3=xo+(S/4-i)*sin(-alf);    y3=0 +(S/4-i)*cos(-alf);      z3=zo;
                x4=xo+(i-S/4)*sin(-alf);    y4=0 +(i-S/4)*cos(-alf);
                x5=xo+i*sin(-alf);          y5=-yo+i*cos(-alf);
                x6=xo+(S/2-i)*sin(alf);     y6=-yo+(S/2-i)*cos(alf);      z6=zo;
            if S!=0:
                z1=-(8*(i-S/4)**2/S-zo-S/2);
                z4=-(8*(i-S/4)**2/S-zo-S/2);
                z5=-(8*(i-S/4)**2/S-zo-S/2); 
            else:
                z1=zo+S/2;
                z4=zo+S/2;
                z5=zo;
    
            ## Segunda Etapa: S/2<i<=S
            if i>S/2 and i<=S:
                x1=xo+(S-i)*sin(-alf);      y1=yo+(S-i)*cos(-alf);        z1=zo;
                x2=xo+(i-S/2)*sin(alf);     y2=yo+(i-S/2)*cos(alf);       z2=-(8*(i-3*S/4)**2/S-zo-S/2);
                x3=xo+(i-3*S/4)*sin(-alf);  y3=0+(i-3*S/4)*cos(-alf);     z3=-(8*(i-3*S/4)**2/S-zo-S/2);
                x4=xo+(3*S/4-i)*sin(-alf);  y4=0+(3*S/4-i)*cos(-alf);     z4=zo;
                x5=xo-(i-S)*sin(-alf);      y5=-yo-(i-S)*cos(-alf);       z5=zo;
                x6=xo+(i-S/2)*sin(alf);     y6=-yo+(i-S/2)*cos(alf);      z6=-(8*(i-3*S/4)**2/S-zo-S/2);
    
        x1=x1/100; y1=y1/100; z1=z1/100;
        x2=x2/100; y2=y2/100; z2=z2/100;
        x3=x3/100; y3=y3/100; z3=z3/100;
        x4=x4/100; y4=y4/100; z4=z4/100;    
        x5=x5/100; y5=y5/100; z5=z5/100;
        x6=x6/100; y6=y6/100; z6=z6/100;
        
        hexapod.joints[0] = [x1, y1, z1]
        hexapod.joints[1] = [x2, y2, z2]
        hexapod.joints[2] = [x3, y3, z3]
        hexapod.joints[3] = [x4, y4, z4]
        hexapod.joints[4] = [x5, y5, z5]
        hexapod.joints[5] = [x6, y6, z6]
        hexapod.set_step()
        hexapod.step_delay()
