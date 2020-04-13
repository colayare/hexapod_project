# -*- coding: utf-8 -*-
import os, sys
#from math import floor as floor
import numpy as np
import matplotlib.pyplot as plt
from math import sin as sin
from math import cos as cos
from math import pi as PI
###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/../includes')
sys.path.append(abs_path+'/../includes')
from hexapod_class import *
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

## Parametros
S   = 7.0
res = 0.07 
xo  = 12.38
yo  = 5.0
zo  = -10.51

## Angle
angle = 0
alf = angle * PI/180;

## Gait
walk = 3

## Number of Gaits
n_gaits = 2

## Inicialization
x1, y1, z1 = 0.0, 0.0, 0.0
x2, y2, z2 = 0.0, 0.0, 0.0
x3, y3, z3 = 0.0, 0.0, 0.0
x4, y4, z4 = 0.0, 0.0, 0.0
x5, y5, z5 = 0.0, 0.0, 0.0
x6, y6, z6 = 0.0, 0.0, 0.0

for k in range ( n_gaits ):
    for j in range(int(S/res)):
        i=res*j;
        if(walk==1):
            ## Primera Etapa: 0<i<=S/2
            if i<=S/2:
                x1=xo+i*sin(-alf);              y1=yo+i*cos(-alf);  
                x2=xo+(S/2-i)*sin(alf);         y2=yo+(S/2-i)*cos(alf);         z2=zo;
                x3=xo+(S/4-i)*sin(-alf);        y3=0 +(S/4-i)*cos(-alf);        z3=zo;
                x4=xo+(i-S/4)*sin(-alf);        y4=0 +(i-S/4)*cos(-alf);
                x5=xo+i*sin(-alf);              y5=-yo+i*cos(-alf);
                x6=xo+(S/2-i)*sin(alf);         y6=-yo+(S/2-i)*cos(alf);        z6=zo;
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
                x1=xo+(S-i)*sin(-alf);          y1=yo+(S-i)*cos(-alf);          z1=zo;
                x2=xo+(i-S/2)*sin(alf);         y2=yo+(i-S/2)*cos(alf);         z2=-((8*((i-(3*S/4))**2)/S)-zo-(S/2));
                x3=xo+(i-3*S/4)*sin(-alf);      y3=0+(i-3*S/4)*cos(-alf);       z3=-((8*((i-3*S/4)**2)/S)-zo-(S/2));
                x4=xo+(3*S/4-i)*sin(-alf);      y4=0+(3*S/4-i)*cos(-alf);       z4=zo;
                x5=xo-(i-S)*sin(-alf);          y5=-yo-(i-S)*cos(-alf);         z5=zo;
                x6=xo+(i-S/2)*sin(alf);         y6=-yo+(i-S/2)*cos(alf);        z6=-((8*((i-3*S/4)**2)/S)-zo-(S/2));
                
        elif (walk==2):
            ## Primera Etapa: 0<i<=S/3 
            if i<=S/3:
                x1=xo+1.5*i*sin(-alf);          y1=yo+1.5*i*cos(-alf);
                x2=xo-0.75*i*sin(alf);          y2=yo-0.75*i*cos(alf);          z2=zo;
                x3=xo-0.75*i*sin(-alf);         y3=0-0.75*i*cos(-alf);          z3=zo;
                x4=xo+1.5*i*sin(alf);           y4=0+1.5*i*cos(alf);
                x5=xo-0.75*i*sin(-alf);         y5=-yo-0.75*i*cos(-alf);        z5=zo;
                x6=xo-0.75*i*sin(alf);          y6=-yo-0.75*i*cos(alf);         z6=zo;
            if S!=0:
                  z1=-((8*((1.5*i-S/4)**2)/S)-zo-(S/2));
                  z4=-((8*((1.5*i-S/4)**2)/S)-zo-(S/2));
            else:
                  z1=zo+(S/2);
                  z4=zo+(S/2);
           
            ## Segunda Etapa: S/3<i<=2S/3
            if i>S/3 and i<=2*S/3:
                x1=xo+0.75*(S-i)*sin(-alf);     y1=yo+0.75*(S-i)*cos(-alf);     z1=zo;
                x2=xo-0.75*i*sin(alf);          y2=yo-0.75*i*cos(alf);          z2=zo;
                x3=xo+(1.5*i-0.75*S)*sin(-alf); y3=0+(1.5*i-0.75*S)*cos(-alf);  z3=-((8*((1.5*i-0.75*S)**2)/S)-zo-(S/2));
                x4=xo+0.75*(S-i)*sin(alf);      y4=0+0.75*(S-i)*cos(alf);       z4=zo;
                x5=xo-0.75*i*sin(-alf);         y5=-yo-0.75*i*cos(-alf);        z5=zo;
                x6=xo+(1.5*i-0.75*S)*sin(alf);  y6=-yo+(1.5*i-0.75*S)*cos(alf); z6=-((8*((1.5*i-0.75*S)**2)/S)-zo-(S/2));
                
            ## Tercera Etapa: 2S/3<i<=S
            if i>2*S/3 and i<=S:
                x1=xo+0.75*(S-i)*sin(-alf);     y1=yo+0.75*(S-i)*cos(-alf);     z1=zo;
                x2=xo+1.5*(i-S)*sin(alf);       y2=yo+1.5*(i-S)*cos(alf);       z2=-((8*((1.5*(i-S)+(S/4))**2)/S)-zo-(S/2));
                x3=xo+0.75*(S-i)*sin(-alf);     y3=0+0.75*(S-i)*cos(-alf);      z3=zo;
                x4=xo+0.75*(S-i)*sin(alf);      y4=0+0.75*(S-i)*cos(alf);       z4=zo;
                x5=xo+1.5*(i-S)*sin(-alf);      y5=-yo+1.5*(i-S)*cos(-alf);     z5=-((8*((1.5*(i-S)+(S/4))**2)/S)-zo-(S/2));
                x6=xo+0.75*(S-i)*sin(alf);      y6=-yo+0.75*(S-i)*cos(alf);     z6=zo;
    
        elif (walk==3):
    
            ## Primera Etapa: 0<i<=S/2
            if i<=S/4:
                x1=xo+2*i*sin(-alf);            y1=yo+2*i*cos(-alf);
                x3=xo-(2*i/3)*sin(-alf);        y3=0-(2*i/3)*cos(-alf);           z3=zo;
                x4=xo-(2*i/3)*sin(alf);         y4=0-(2*i/3)*cos(alf);            z4=zo;
                x6=xo+2*i*sin(alf);             y6=-yo+2*i*cos(alf);   
            if S!=0:
                z1=-(8*(2*i-S/4)**2/S-zo-S/2);
                z6=-(8*(2*i-S/4)**2/S-zo-S/2);
            else:
                z1=zo+S/2;
                z6=zo+S/2;
            if i<=S/2:
                x2=xo-(2*i/3)*sin(alf);         y2=yo-(2*i/3)*cos(alf);           z2=zo;
                x5=xo-(2*i/3)*sin(-alf);        y5=-yo-(2*i/3)*cos(-alf);         z5=zo;
    
            ## Segunda Etapa: S/4<i<=3S/4
            if i>S/4 and i<=3*S/4:
                x1=xo+(2*(S-i)/3)*sin(-alf);      y1=yo+(2*(S-i)/3)*cos(-alf);      z1=zo;
                x3=xo-(2*i/3)*sin(-alf);          y3=0-(2*i/3)*cos(-alf);           z3=zo;
                x6=xo+(2*(S-i)/3)*sin(alf);       y6=-yo+(2*(S-i)/3)*cos(alf);      z6=zo;
            if i>S/2 and i<=3*S/4:
                x2=xo+(2*i-(4*S/3))*sin(alf);     y2=yo+(2*i-(4*S/3))*cos(alf);     z2=-(8*(2*i-(4*S/3)+S/12)**2/S-zo-S/2);
                x5=xo+(2*i-(4*S/3))*sin(-alf);    y5=-yo+(2*i-(4*S/3))*cos(-alf);   z5=-(8*(2*i-(4*S/3)+S/12)**2/S-zo-S/2);
            if i>S/4 and i<=S/2:
                x4=xo+2*(i-S/3)*sin(alf);         y4=0+2*(i-S/3)*cos(alf);          z4=-(8*(2*(i-S/3)-S/12)**2/S-zo-S/2);       
            
            ## Tercera Etapa: S/2<i<=S
            if i>3*S/4 and i<=S:
                x1=xo+(2*(S-i)/3)*sin(-alf);      y1=yo+(2*(S-i)/3)*cos(-alf);      z1=zo;
                x2=xo+(2*(S-i)/3)*sin(alf);       y2=yo+(2*(S-i)/3)*cos(alf);       z2=zo;
                x3=xo+2*(i-S)*sin(-alf);          y3=0+2*(i-S)*cos(-alf);           z3=-(8*(2*(i-S)+S/4)**2/S-zo-S/2);
                x5=xo+(2*(S-i)/3)*sin(-alf);      y5=-yo+(2*(S-i)/3)*cos(-alf);     z5=zo;
                x6=xo+(2*(S-i)/3)*sin(alf);       y6=-yo+(2*(S-i)/3)*cos(alf);      z6=zo;
            if i>S/2 and i<=S:
                x4=xo+(2*(S-i)/3)*sin(alf);       y4=0+(2*(S-i)/3)*cos(alf);        z4=zo;
        
        elif (walk==4):
            ## Primera Etapa: 0<i<=S/3
            if i<=S/3:
                x1=xo-0.6*i*sin(-alf);          y1=yo-0.6*i*cos(-alf);          z1=zo;
            ## Primera Etapa: 0<i<=5*S/6
            if i<=5*S/6:
                x2=xo-0.6*i*sin(alf);           y2=yo-0.6*i*cos(alf);           z2=zo;
            ## Primera Etapa: 0<i<=S/6
            if i<=S/6:
                x3=xo-0.6*i*sin(-alf);          y3=0-0.6*i*cos(-alf);           z3=zo;
            ## Primera Etapa: 0<i<=2*S/3
            if i<=2*S/3:
                x4=xo-0.6*i*sin(alf);           y4=0-0.6*i*cos(alf);            z4=zo;
            ## Primera Etapa: 0<i<=S/6
            if i<=S/6:
                x5=xo+3*i*sin(-alf);            y5=-yo+3*i*cos(-alf);            
            if S!=0:
                  z5=-(8*(3*i-S/4)**2/S-zo-S/2);
            else:
                  z5=zo+S/2;
            if i<=S/2:
                x6=xo-0.6*i*sin(alf);           y6=-yo-0.6*i*cos(alf);          z6=zo;
            
            ## Segunda Etapa: S/3<i<=S/2
            if i>S/3 and i<=S/2:
                x1=xo+(3*i-1.2*S)*sin(-alf);    y1=yo+(3*i-1.2*S)*cos(-alf);    z1=-(8*((3*i-1.2*S)-S/20)**2/S-zo-S/2);
            ## Segunda Etapa: 5*S/6<i<=S
            if i>5*S/6 and i<=S:
                x2=xo+3*(i-S)*sin(alf);         y2=yo+3*(i-S)*cos(alf);         z2=-(8*(3*(i-S)+S/4)**2/S-zo-S/2);
            ## Segunda Etapa: S/6<i<=S/3
            if i>S/6 and i<=S/3:
                x3=xo+(3*i-0.6*S)*sin(-alf);    y3=0+(3*i-0.6*S)*cos(-alf);     z3=-(8*((3*i-0.6*S)-3*S/20)**2/S-zo-S/2);
            if i>2*S/3 and i<=5*S/6:
                x4=xo+(3*i-2.4*S)*sin(alf);     y4=0+(3*i-2.4*S)*cos(alf);      z4=-(8*((3*i-2.4*S)+3*S/20)**2/S-zo-S/2);
            ## Segunda Etapa: S/6<i<=S
            if i>S/6 and i<=S:
                x5=xo+0.6*(S-i)*sin(-alf);      y5=-yo+0.6*(S-i)*cos(-alf);     z5=zo;
            ## Segunda Etapa: S/2<i<=2S/3
            if i>S/2 and i<=2*S/3:
                x6=xo+(3*i-1.8*S)*sin(alf);     y6=-yo+(3*i-1.8*S)*cos(alf);    z6=-(8*((3*i-1.8*S)+S/20)**2/S-zo-S/2);
    
            ## Tercera Etapa: S/2<i<=S
            if i>S/2 and i<=S:
                x1=xo+0.6*(S-i)*sin(-alf);      y1=yo+0.6*(S-i)*cos(-alf);      z1=zo;
            ## Tercera Etapa: S/3<i<=S
            if i>S/3 and i<=S:
                x3=xo+0.6*(S-i)*sin(-alf);      y3=0+0.6*(S-i)*cos(-alf);       z3=zo;           
            ## Tercera Etapa: 5*S/6<i<=S
            if i>5*S/6 and i<=S:
                x4=xo+0.6*(S-i)*sin(alf);       y4=0+0.6*(S-i)*cos(alf);        z4=zo;
            ## Tercera Etapa: 2S/3<i<=S
            if i>2*S/3 and i<=S:
                x6=xo+0.6*(S-i)*sin(alf);       y6=-yo+0.6*(S-i)*cos(alf);      z6=zo;
        
        x1=x1/100; y1=y1/100; z1=z1/100;
        x2=x2/100; y2=y2/100; z2=z2/100;
        x3=x3/100; y3=y3/100; z3=z3/100;
        x4=x4/100; y4=y4/100; z4=z4/100;    
        x5=x5/100; y5=y5/100; z5=z5/100;
        x6=x6/100; y6=y6/100; z6=z6/100;
        
        hexapod.set_step(0, [x1, y1, z1]);
        hexapod.set_step(1, [x2, y2, z2]);
        hexapod.set_step(2, [x3, y3, z3]);
        hexapod.set_step(3, [x4, y4, z4]);
        hexapod.set_step(4, [x5, y5, z5]);
        hexapod.set_step(5, [x6, y6, z6]);
    

hexapod.plot_kinematics()
hexapod.plot_ikinematics()
hexapod.plot_gait()
