# -*- coding: utf-8 -*-
import os
import numpy as np
from math import sin as sin
from math import cos as cos
from math import pi  as pi
from hexapod_class import hexapod_kinematics as hexapod_kinematics

###############################################################################
#### self Class : Base AXI IP
###############################################################################
class hexapod_locomotion(hexapod_kinematics):
    ###########################################################################
    #### Properties
    ###########################################################################
    S       = 0.0
    res     = 0.0
    xo      = 0.0
    yo      = 0.0
    zo      = 0.0
    
    @property
    def valid_params(self):
        return self.S != 0 and self.res != 0 and self.xo != 0 and self.yo != 0 and self.zo != 0
    
    ###########################################################################
    #### Methods
    ###########################################################################
    #### Constructor
    def __init__(self, invoke_axi_ip=True, enable_ip_logs=False):
        if ( invoke_axi_ip ):
            super(hexapod_locomotion, self).__init__(invoke_axi_ip=invoke_axi_ip, enable_ip_logs=enable_ip_logs)
        return None
    
    #### Locomotion Methods ###################################################
    def step(self, point, walk, alf):
        if ( not self.valid_params ):
            print('Invalid Locomotion parameters.')
            return False
        i=self.res*point;
        x1, y1, z1 = 0.0, 0.0, 0.0
        x2, y2, z2 = 0.0, 0.0, 0.0
        x3, y3, z3 = 0.0, 0.0, 0.0
        x4, y4, z4 = 0.0, 0.0, 0.0
        x5, y5, z5 = 0.0, 0.0, 0.0
        x6, y6, z6 = 0.0, 0.0, 0.0
        if(walk==1):
            ## Primera Etapa: 0<i<=self.S/2
            if i<=self.S/2:
                x1=self.xo+i*sin(-alf);              y1=self.yo+i*cos(-alf);  
                x2=self.xo+(self.S/2-i)*sin(alf);         y2=self.yo+(self.S/2-i)*cos(alf);         z2=self.zo;
                x3=self.xo+(self.S/4-i)*sin(-alf);        y3=0 +(self.S/4-i)*cos(-alf);        z3=self.zo;
                x4=self.xo+(i-self.S/4)*sin(-alf);        y4=0 +(i-self.S/4)*cos(-alf);
                x5=self.xo+i*sin(-alf);              y5=-self.yo+i*cos(-alf);
                x6=self.xo+(self.S/2-i)*sin(alf);         y6=-self.yo+(self.S/2-i)*cos(alf);        z6=self.zo;
            if self.S!=0:
                z1=-(8*pow(i-self.S/4,2)/self.S-self.zo-self.S/2);
                z4=-(8*pow(i-self.S/4,2)/self.S-self.zo-self.S/2);
                z5=-(8*pow(i-self.S/4,2)/self.S-self.zo-self.S/2); 
            else:
                z1=self.zo+self.S/2;
                z4=self.zo+self.S/2;
                z5=self.zo;
    
            ## Segunda Etapa: self.S/2<i<=self.S
            if i>self.S/2 and i<=self.S:
                x1=self.xo+(self.S-i)*sin(-alf);          y1=self.yo+(self.S-i)*cos(-alf);          z1=self.zo;
                x2=self.xo+(i-self.S/2)*sin(alf);         y2=self.yo+(i-self.S/2)*cos(alf);         z2=-(8*pow(i-3*self.S/4,2)/self.S-self.zo-self.S/2);
                x3=self.xo+(i-3*self.S/4)*sin(-alf);      y3=0+(i-3*self.S/4)*cos(-alf);       z3=-(8*pow(i-3*self.S/4,2)/self.S-self.zo-self.S/2);
                x4=self.xo+(3*self.S/4-i)*sin(-alf);      y4=0+(3*self.S/4-i)*cos(-alf);       z4=self.zo;
                x5=self.xo-(i-self.S)*sin(-alf);          y5=-self.yo-(i-self.S)*cos(-alf);         z5=self.zo;
                x6=self.xo+(i-self.S/2)*sin(alf);         y6=-self.yo+(i-self.S/2)*cos(alf);        z6=-(8*pow(i-(3*self.S/4),2)/self.S-self.zo-self.S/2);
                
        elif (walk==2):
            ## Primera Etapa: 0<i<=self.S/3 
            if i<=self.S/3:
                x1=self.xo+1.5*i*sin(-alf);          y1=self.yo+1.5*i*cos(-alf);
                x2=self.xo-0.75*i*sin(alf);          y2=self.yo-0.75*i*cos(alf);          z2=self.zo;
                x3=self.xo-0.75*i*sin(-alf);         y3=0-0.75*i*cos(-alf);          z3=self.zo;
                x4=self.xo+1.5*i*sin(alf);           y4=0+1.5*i*cos(alf);
                x5=self.xo-0.75*i*sin(-alf);         y5=-self.yo-0.75*i*cos(-alf);        z5=self.zo;
                x6=self.xo-0.75*i*sin(alf);          y6=-self.yo-0.75*i*cos(alf);         z6=self.zo;
            if self.S!=0:
                  z1=-(8*pow(1.5*i-self.S/4,2)/self.S-self.zo-self.S/2);
                  z4=-(8*pow(1.5*i-self.S/4,2)/self.S-self.zo-self.S/2);
            else:
                  z1=self.zo+self.S/2;
                  z4=self.zo+self.S/2;
           
            ## Segunda Etapa: self.S/3<i<=2S/3
            if i>self.S/3 and i<=2*self.S/3:
                x1=self.xo+0.75*(self.S-i)*sin(-alf);     y1=self.yo+0.75*(self.S-i)*cos(-alf);     z1=self.zo;
                x2=self.xo-0.75*i*sin(alf);          y2=self.yo-0.75*i*cos(alf);          z2=self.zo;
                x3=self.xo+(1.5*i-0.75*self.S)*sin(-alf); y3=0+(1.5*i-0.75*self.S)*cos(-alf);  z3=-(8*pow(1.5*i-0.75*self.S,2)/self.S-self.zo-self.S/2);
                x4=self.xo+0.75*(self.S-i)*sin(alf);      y4=0+0.75*(self.S-i)*cos(alf);       z4=self.zo;
                x5=self.xo-0.75*i*sin(-alf);         y5=-self.yo-0.75*i*cos(-alf);        z5=self.zo;
                x6=self.xo+(1.5*i-0.75*self.S)*sin(alf);  y6=-self.yo+(1.5*i-0.75*self.S)*cos(alf); z6=-(8*pow(1.5*i-0.75*self.S,2)/self.S-self.zo-self.S/2);
                
            ## Tercera Etapa: 2S/3<i<=self.S
            if i>2*self.S/3 and i<=self.S:
                x1=self.xo+0.75*(self.S-i)*sin(-alf);     y1=self.yo+0.75*(self.S-i)*cos(-alf);     z1=self.zo;
                x2=self.xo+1.5*(i-self.S)*sin(alf);       y2=self.yo+1.5*(i-self.S)*cos(alf);       z2=-(8*pow(1.5*(i-self.S)+self.S/4,2)/self.S-self.zo-self.S/2);
                x3=self.xo+0.75*(self.S-i)*sin(-alf);     y3=0+0.75*(self.S-i)*cos(-alf);      z3=self.zo;
                x4=self.xo+0.75*(self.S-i)*sin(alf);      y4=0+0.75*(self.S-i)*cos(alf);       z4=self.zo;
                x5=self.xo+1.5*(i-self.S)*sin(-alf);      y5=-self.yo+1.5*(i-self.S)*cos(-alf);     z5=-(8*pow(1.5*(i-self.S)+self.S/4,2)/self.S-self.zo-self.S/2);
                x6=self.xo+0.75*(self.S-i)*sin(alf);      y6=-self.yo+0.75*(self.S-i)*cos(alf);     z6=self.zo;
    
        elif (walk==3):
    
            ## Primera Etapa: 0<i<=self.S/2
            if i<=self.S/4:
                x1=self.xo+2*i*sin(-alf);            y1=self.yo+2*i*cos(-alf);
                x3=self.xo-2*i/3*sin(-alf);          y3=0-2*i/3*cos(-alf);           z3=self.zo;
                x4=self.xo-2*i/3*sin(alf);           y4=0-2*i/3*cos(alf);            z4=self.zo;
                x6=self.xo+2*i*sin(alf);             y6=-self.yo+2*i*cos(alf);             
            if self.S!=0:
                z1=-(8*pow(2*i-self.S/4,2)/self.S-self.zo-self.S/2);
                z6=-(8*pow(2*i-self.S/4,2)/self.S-self.zo-self.S/2);
            else:
                z1=self.zo+self.S/2;
                z6=self.zo+self.S/2;
            if i<=self.S/2:
                x2=self.xo-2*i/3*sin(alf);           y2=self.yo-2*i/3*cos(alf);           z2=self.zo;
                x5=self.xo-2*i/3*sin(-alf);          y5=-self.yo-2*i/3*cos(-alf);         z5=self.zo;
    
            ## Segunda Etapa: self.S/4<i<=3S/4
            if i>self.S/4 and i<=3*self.S/4:
                x1=self.xo+2*(self.S-i)/3*sin(-alf);      y1=self.yo+2*(self.S-i)/3*cos(-alf);      z1=self.zo;
                x3=self.xo-2*i/3*sin(-alf);          y3=0-2*i/3*cos(-alf);           z3=self.zo;
                x6=self.xo+2*(self.S-i)/3*sin(alf);       y6=-self.yo+2*(self.S-i)/3*cos(alf);      z6=self.zo;
            if i>self.S/2 and i<=3*self.S/4:
                x2=self.xo+(2*i-4*self.S/3)*sin(alf);     y2=self.yo+(2*i-4*self.S/3)*cos(alf);     z2=-(8*pow(2*i-4*self.S/3+self.S/12,2)/self.S-self.zo-self.S/2);
                x5=self.xo+(2*i-4*self.S/3)*sin(-alf);    y5=-self.yo+(2*i-4*self.S/3)*cos(-alf);   z5=-(8*pow(2*i-4*self.S/3+self.S/12,2)/self.S-self.zo-self.S/2);
            if i>self.S/4 and i<=self.S/2:
                x4=self.xo+2*(i-self.S/3)*sin(alf);       y4=0+2*(i-self.S/3)*cos(alf);        z4=-(8*pow(2*(i-self.S/3)-self.S/12,2)/self.S-self.zo-self.S/2);       
            
            ## Tercera Etapa: self.S/2<i<=self.S
            if i>3*self.S/4 and i<=self.S:
                x1=self.xo+2*(self.S-i)/3*sin(-alf);      y1=self.yo+2*(self.S-i)/3*cos(-alf);      z1=self.zo;
                x2=self.xo+2*(self.S-i)/3*sin(alf);       y2=self.yo+2*(self.S-i)/3*cos(alf);       z2=self.zo;
                x3=self.xo+2*(i-self.S)*sin(-alf);        y3=0+2*(i-self.S)*cos(-alf);         z3=-(8*pow(2*(i-self.S)+self.S/4,2)/self.S-self.zo-self.S/2);
                x5=self.xo+2*(self.S-i)/3*sin(-alf);      y5=-self.yo+2*(self.S-i)/3*cos(-alf);     z5=self.zo;
                x6=self.xo+2*(self.S-i)/3*sin(alf);       y6=-self.yo+2*(self.S-i)/3*cos(alf);      z6=self.zo;
            if i>self.S/2 and i<=self.S:
                x4=self.xo+2*(self.S-i)/3*sin(alf);       y4=0+2*(self.S-i)/3*cos(alf);        z4=self.zo;
        
        elif (walk==4):
            ## Primera Etapa: 0<i<=self.S/3
            if i<=self.S/3:
                x1=self.xo-0.6*i*sin(-alf);          y1=self.yo-0.6*i*cos(-alf);          z1=self.zo;
            ## Primera Etapa: 0<i<=5*self.S/6
            if i<=5*self.S/6:
                x2=self.xo-0.6*i*sin(alf);           y2=self.yo-0.6*i*cos(alf);           z2=self.zo;
            ## Primera Etapa: 0<i<=self.S/6
            if i<=self.S/6:
                x3=self.xo-0.6*i*sin(-alf);          y3=0-0.6*i*cos(-alf);           z3=self.zo;
            ## Primera Etapa: 0<i<=2*self.S/3
            if i<=2*self.S/3:
                x4=self.xo-0.6*i*sin(alf);           y4=0-0.6*i*cos(alf);            z4=self.zo;
            ## Primera Etapa: 0<i<=self.S/6
            if i<=self.S/6:
                x5=self.xo+3*i*sin(-alf);            y5=-self.yo+3*i*cos(-alf);            
            if self.S!=0:
                  z5=-(8*pow(3*i-self.S/4,2)/self.S-self.zo-self.S/2);
            else:
                  z5=self.zo+self.S/2;
            if i<=self.S/2:
                x6=self.xo-0.6*i*sin(alf);           y6=-self.yo-0.6*i*cos(alf);          z6=self.zo;
            
            ## Segunda Etapa: self.S/3<i<=self.S/2
            if i>self.S/3 and i<=self.S/2:
                x1=self.xo+(3*i-1.2*self.S)*sin(-alf);    y1=self.yo+(3*i-1.2*self.S)*cos(-alf);    z1=-(8*pow((3*i-1.2*self.S)-self.S/20,2)/self.S-self.zo-self.S/2);
            ## Segunda Etapa: 5*self.S/6<i<=self.S
            if i>5*self.S/6 and i<=self.S:
                x2=self.xo+3*(i-self.S)*sin(alf);         y2=self.yo+3*(i-self.S)*cos(alf);         z2=-(8*pow(3*(i-self.S)+self.S/4,2)/self.S-self.zo-self.S/2);
            ## Segunda Etapa: self.S/6<i<=self.S/3
            if i>self.S/6 and i<=self.S/3:
                x3=self.xo+(3*i-0.6*self.S)*sin(-alf);    y3=0+(3*i-0.6*self.S)*cos(-alf);     z3=-(8*pow((3*i-0.6*self.S)-3*self.S/20,2)/self.S-self.zo-self.S/2);
            if i>2*self.S/3 and i<=5*self.S/6:
                x4=self.xo+(3*i-2.4*self.S)*sin(alf);     y4=0+(3*i-2.4*self.S)*cos(alf);      z4=-(8*pow((3*i-2.4*self.S)+3*self.S/20,2)/self.S-self.zo-self.S/2);
            ## Segunda Etapa: self.S/6<i<=self.S
            if i>self.S/6 and i<=self.S:
                x5=self.xo+0.6*(self.S-i)*sin(-alf);      y5=-self.yo+0.6*(self.S-i)*cos(-alf);     z5=self.zo;
            ## Segunda Etapa: self.S/2<i<=2S/3
            if i>self.S/2 and i<=2*self.S/3:
                x6=self.xo+(3*i-1.8*self.S)*sin(alf);     y6=-self.yo+(3*i-1.8*self.S)*cos(alf);    z6=-(8*pow((3*i-1.8*self.S)+self.S/20,2)/self.S-self.zo-self.S/2);
    
            ## Tercera Etapa: self.S/2<i<=self.S
            if i>self.S/2 and i<=self.S:
                x1=self.xo+0.6*(self.S-i)*sin(-alf);      y1=self.yo+0.6*(self.S-i)*cos(-alf);      z1=self.zo;
            ## Tercera Etapa: self.S/3<i<=self.S
            if i>self.S/3 and i<=self.S:
                x3=self.xo+0.6*(self.S-i)*sin(-alf);      y3=0+0.6*(self.S-i)*cos(-alf);       z3=self.zo;           
            ## Tercera Etapa: 5*self.S/6<i<=self.S
            if i>5*self.S/6 and i<=self.S:
                x4=self.xo+0.6*(self.S-i)*sin(alf);       y4=0+0.6*(self.S-i)*cos(alf);        z4=self.zo;
            ## Tercera Etapa: 2S/3<i<=self.S
            if i>2*self.S/3 and i<=self.S:
                x6=self.xo+0.6*(self.S-i)*sin(alf);       y6=-self.yo+0.6*(self.S-i)*cos(alf);      z6=self.zo;
        
        x1=x1/100; y1=y1/100; z1=z1/100;
        x2=x2/100; y2=y2/100; z2=z2/100;
        x3=x3/100; y3=y3/100; z3=z3/100;
        x4=x4/100; y4=y4/100; z4=z4/100;    
        x5=x5/100; y5=y5/100; z5=z5/100;
        x6=x6/100; y6=y6/100; z6=z6/100;
        
        self.joints[0] = [x1, y1, z1]
        self.joints[1] = [x2, y2, z2]
        self.joints[2] = [x3, y3, z3]
        self.joints[3] = [x4, y4, z4]
        self.joints[4] = [x5, y5, z5]
        self.joints[5] = [x6, y6, z6]
        return True