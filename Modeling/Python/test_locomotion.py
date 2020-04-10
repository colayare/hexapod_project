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

walk = input('Seleccionar caminata : ')

## Run Gait
for k in range ( iterations ):
    for j in range(int(S/res)):
        i=res*j;
        if(walk==1):
            
            if (i<=S/4):
                X1e=12.38;  Y1e=i-oy;           Z1e=-10.51;
                X2e=12.38;  Y2e=-i-oy;          Z2e=-(pow(Y2e+oy,2)*64/(S**2)+6.51);
                X3e=12.38;  Y3e=-i;             Z3e=-(pow(Y3e,2)*64/(S**2)+6.51);
                X4e=12.38;  Y4e=i;              Z4e=-10.51;
                X5e=12.38;  Y5e=i+oy;           Z5e=-10.51;
                X6e=12.38;  Y6e=-i+oy;          Z6e=-(pow(Y6e-oy,2)*64/(S**2)+6.51);
                
            if (i>S/4 and i<=3*S/4):
                X1e=12.38;  Y1e=S/2-i-oy;       Z1e=-(pow(Y1e+oy,2)*64/(S**2)+6.51);
                X2e=12.38;  Y2e=i-S/2-oy;       Z2e=-10.51;
                X3e=12.38;  Y3e=i-S/2;          Z3e=-10.51;
                X4e=12.38;  Y4e=S/2-i ;         Z4e=-(pow(Y4e,2)*64/(S**2)+6.51);
                X5e=12.38;  Y5e=S/2-i+oy;       Z5e=-(pow(Y5e-oy,2)*64/(S**2)+6.51);
                X6e=12.38;  Y6e=i-S/2+oy;       Z6e=-10.51;
         
            if (i>3*S/4 and i<=S):
                X1e=12.38;  Y1e=i-S-oy;         Z1e=-10.51;
                X2e=12.38;  Y2e=S-i-oy;         Z2e=-(pow(Y2e+oy,2)*64/(S**2)+6.51);
                X3e=12.38;  Y3e=S-i;            Z3e=-(pow(Y3e,2)*64/(S**2)+6.51);
                X4e=12.38;  Y4e=i-S;            Z4e=-10.51;
                X5e=12.38;  Y5e=i-S+oy;         Z5e=-10.51; 
                X6e=12.38;  Y6e=S-i+oy;         Z6e=-(pow(Y6e-oy,2)*64/(S**2)+6.51);
        
        elif(walk==2):
            
            if (i<=S/4):
                X1e=12.38;  Y1e=-4*i-oy;        Z1e=-(pow(Y1e+4*S/8+oy,2)*16/(S**2)+6.51); 
                X6e=12.38;  Y6e=-4*i+oy;        Z6e=-(pow(Y6e+4*S/8-oy,2)*16/(S**2)+6.51); 
                X4e=12.38;  Y4e=4*i/3;          Z4e=-10.51;
            if (i>S/4 and i<=S):
                X1e=12.38;  Y1e=4*(i-S)/3-oy;   Z1e=-10.51;
                X6e=12.38;  Y6e=4*(i-S)/3+oy;   Z6e=-10.51;
                
            if (i<=S/2):
                X2e=12.38;  Y2e=4*i/3-oy;       Z2e=-10.51;
                X5e=12.38;  Y5e=4*i/3+oy;       Z5e=-10.51; 
            if (i>S/2 and i<=3*S/4):
                X2e=12.38;  Y2e=4*(2*S/3-i)-oy; Z2e=-(pow(Y2e+4*(5*S/8-2*S/3)+oy,2)*16/(S**2)+6.51); 
                X5e=12.38;  Y5e=4*(2*S/3-i)+oy; Z5e=-(pow(Y5e+4*(5*S/8-2*S/3)-oy,2)*16/(S**2)+6.51); 
            if (i>3*S/4 and i<=S):
                X2e=12.38;  Y2e=4*(i-S)/3-oy;   Z2e=-10.51;
                X5e=12.38;  Y5e=4*(i-S)/3+oy;   Z5e=-10.51; 
                X3e=12.38;  Y3e=4*(S-i);        Z3e=-(pow(Y3e+4*(7*S/8-S),2)*16/(S**2)+6.51);            
                
            if (i<=3*S/4):
                X3e=12.38;  Y3e=4*i/3;          Z3e=-10.51;
                
            if (i>S/4 and i<=S/2):
                X4e=12.38;  Y4e=4*(-i+S/3);     Z4e=-(pow(Y4e+4*(3*S/8-S/3),2)*16/(S**2)+6.51);
            if (i>S/2 and i<=S):
                X4e=12.38;  Y4e=4*(i-S)/3;      Z4e=-10.51;
            
        elif(walk==3):
            
            if (i<=S/3):
                X1e=12.38;  Y1e=6*i/5-oy;       Z1e=-10.51; 
            if (i>S/3 and i<=S/2):
                X1e=12.38;  Y1e=6*(2*S/5-i)-oy; Z1e=-(pow(Y1e+6*(-2*S/5+5*S/12)+oy,2)*16/(S**2)+6.51);
            if (i>S/2 and i<=S):
                X1e=12.38;  Y1e=-6*(S-i)/5-oy;  Z1e=-10.51; 
            
            if (i<=5*S/6):
                X2e=12.38;  Y2e=6*i/5-oy;       Z2e=-10.51; 
            if (i>5*S/6 and i<=S):
                X2e=12.38;  Y2e=6*(S-i)-oy;     Z2e=-(pow(Y2e+6*(-S+11*S/12)+oy,2)*16/(S**2)+6.51);
            
            if (i<=S/6):
                X3e=12.38;  Y3e=6*i/5;          Z3e=-10.51; 
            if (i>S/6 and i<=S/3):
                X3e=12.38;  Y3e=6*(S/5-i);      Z3e=-(pow(Y3e+6*(-S/5+3*S/12),2)*16/(S**2)+6.51);
            if (i>S/3 and i<=S):
                X3e=12.38;  Y3e=-6*(S-i)/5;     Z3e=-10.51; 
        
            if (i<=2*S/3):
                X4e=12.38;  Y4e=6*i/5;          Z4e=-10.51;
            if (i>2*S/3 and i<=5*S/6):
                X4e=12.38;  Y4e=6*(4*S/5-i);    Z4e=-(pow(Y4e+6*(-4*S/5+9*S/12),2)*16/(S**2)+6.51);
            if (i>5*S/6 and i<=S):
                X4e=12.38;  Y4e=-6*(S-i)/5;     Z4e=-10.51;
        
            if (i<=S/6):
                X5e=12.38;  Y5e=-6*i+oy;        Z5e=-(pow(Y5e+6*S/12-oy,2)*16/(S**2)+6.51);
            if (i>S/6 and i<=S):
                X5e=12.38;  Y5e=-6*(S-i)/5+oy;  Z5e=-10.51;
                
            if (i<=S/2):
                X6e=12.38;  Y6e=6*i/5+oy;       Z6e=-10.51; 
            if (i>S/2 and i<=2*S/3):
                X6e=12.38;  Y6e=6*(3*S/5-i)+oy; Z6e=-(pow(Y6e+6*(-3*S/5+7*S/12)-oy,2)*16/(S**2)+6.51);
            if (i>2*S/3 and  i<=S):
                X6e=12.38;  Y6e=-6*(S-i)/5+oy;  Z6e=-10.51;
            
        X1e=X1e/100; Y1e=Y1e/100; Z1e=Z1e/100;
        X2e=X1e/100; Y2e=Y2e/100; Z2e=Z2e/100;
        X3e=X1e/100; Y3e=Y3e/100; Z3e=Z3e/100;
        X4e=X1e/100; Y4e=Y4e/100; Z4e=Z4e/100;
        X5e=X1e/100; Y5e=Y5e/100; Z5e=Z5e/100;
        X6e=X1e/100; Y6e=Y6e/100; Z6e=Z6e/100;
        hexapod.joints[0] = [X1e, Y1e, Z1e]
        hexapod.joints[1] = [X2e, Y2e, Z2e]
        hexapod.joints[2] = [X3e, Y3e, Z3e]
        hexapod.joints[3] = [X4e, Y4e, Z4e]
        hexapod.joints[4] = [X5e, Y5e, Z5e]
        hexapod.joints[5] = [X6e, Y6e, Z6e]
        hexapod.set_step()
        hexapod.step_delay()

hexapod.set_init_position(print_out=False)