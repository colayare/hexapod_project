# -*- coding: utf-8 -*-
import numpy as np
import mmap
from time import sleep as sleep
from cordic_model import CORDIC as CORDIC
from numeric_conversions import numeric_conversions as numeric_conversions
from devmem_map import axi_ip_mmap as axi_ip_mmap
import math as mt
import os
try:
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    PLOT_EN = True
except ImportError:
    PLOT_EN = False
    pass

from hexapod_class import *

###############################################################################
#### Hexapod Leg Class : Container or locomotion
###############################################################################
class hexapod_leg(numeric_conversions):
    __points    = 0
    coordinates = np.array([0, 0, 0])
    
    @property
    def steps(self):
        return self.coordinates.shape[0]
    
    @property
    def x_traject(self):
        return self.coordinates[:,0]
    
    @property
    def y_traject(self):
        return self.coordinates[:,1]
    
    @property
    def z_traject(self):
        return self.coordinates[:,2]
    
    def append(self, value):
        if ( self.__points > 0 ):
            self.coordinates = np.vstack((self.coordinates, value))
        else:
            self.coordinates = value
        self.__points += 1
        
    def clean(self):
        self.coordinates = np.array([0, 0, 0])
        self.__points = 0

###############################################################################
#### Hexapod Class for High Level Processing :
#### This class object is not intended for run in Petalinux
###############################################################################
class hexapod(hexapod_kinematics):
    ###########################################################################
    #### Properties
    ###########################################################################
    ## Kinematics Parameters
    __coord   = np.zeros(shape=(6,3))
#    __joints  = np.zeros(shape=(6,3))         # Actual Joints Positions
    __locom   = np.array([hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg()])
    
    __fgait   = np.array([hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg()])
    
    __joints  = np.array([hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg(), 
                          hexapod_leg()])
    
    ### Inerse Kinematics Parameters
    l1      = 0.0275
    l2      = 0.0963
    l3      = 0.1051
    ik      = 0.6072529
    ikh     = 3.763427734375
    
    #### Locomotion Parameters
    offset_x = 0
    offset_y = 0
    
    @property
    def Ca(self):
        return 1/(2*self.l2*self.l3)
    
    @property
    def F(self):
        return self.l2/self.l3
    
    @property
    def S(self):
        return self.l1**2 - self.l2**2 - self.l3**2
    
#    @property
#    def joints(self):
#        return self.__joints
    
#    @joints.setter
#    def joints(self, value):
#        self.__joints = value
    
    @property
    def coordinates(self):
        return self.__coord
    
    @property
    def locomotion(self):
        return self.__locom
    
    @property
    def fgait(self):
        return self.__fgait
    
    def get_fstep(self, leg, step):
        x_axi = self.hfloat2dfloat(hex(self.to_int(self.gait[leg*self.step_size*3+step*3+0]))[2:])
        y_axi = self.hfloat2dfloat(hex(self.to_int(self.gait[leg*self.step_size*3+step*3+1]))[2:])
        z_axi = self.hfloat2dfloat(hex(self.to_int(self.gait[leg*self.step_size*3+step*3+2]))[2:])
        
        dstep = [x_axi, y_axi, z_axi]
        return dstep
    
    def get_fgait(self):
        for i, leg in enumerate(self.__fgait):
            for step in range ( int(self.gait.size/(6*3)) ):
                leg.append(self.get_fstep(i, step))
        return True
                
    ###########################################################################
    #### Methods
    ###########################################################################
    #### Constructor
    def __init__(self, invoke_axi_ip=True, enable_ip_logs=False):
        if ( invoke_axi_ip ):
            super(hexapod, self).__init__(invoke_axi_ip=True, gen_log_enable=enable_ip_logs)
        return None
    
    #### Inverse Kinematics Functions #########################################
    ## Kinematics Functions
    def dKinematics(self, q1, q2, q3):
        x = mt.cos(q1)*(self.l3*mt.cos(q2+q3) + self.l2*mt.cos(q2) + self.l1)
        y = mt.sin(q1)*(self.l3*mt.cos(q2+q3) + self.l2*mt.cos(q2) + self.l1) 
        z = self.l3 * mt.sin(q2+q3) + self.l2*mt.sin(q2)
        return x, y, z
    
    #### Theoretical Inverse Kinematics Calculation
    def iKinematics_t(self, xin, yin, zin):
        if ( type(xin) is str ):
            x = self.hfloat2dfloat(xin)
            y = self.hfloat2dfloat(yin)
            z = self.hfloat2dfloat(zin)
        else:
            x = xin
            y = yin
            z = zin
    
        Ca = 1/(2*self.l2*self.l3)
        F = self.l2/self.l3
        S = self.l1**2 - self.l2**2 - self.l3**2    
        r = mt.sqrt(x**2 + y**2)
        A = 2*r*self.l1
        C = r**2 + z**2 + S - A
        D = C * Ca
        B = mt.sqrt((r-self.l1)**2 + z**2)
        G = z/B
        q1 = mt.atan2(y,x)
        q3 = - mt.atan2(mt.sqrt(1 - D**2),D)
        q2 = mt.atan2(G,mt.sqrt(1 - G**2)) - mt.atan2(mt.sin(q3),F+mt.cos(q3))
        return q1, q2, q3
    
    #### CORDIC-based Inverse Kinematics calculation
    def iKinematics(self, coordinates):
        [xin, yin, zin] = coordinates
        C1_CV = CORDIC('circular', 'vectorial', 0, 15)
        C2_HV = CORDIC('hyperbolic', 'vectorial', -1, 14)
#        C3_CV = CORDIC('circular', 'vectorial', 0, 15)
        C3_CV = CORDIC('circular', 'vectorial', -1, 14) #1.7
        C4_CV = CORDIC('circular', 'vectorial', 0, 15)
        C5_CR = CORDIC('circular', 'rotational', 0, 15)
        C6_CV = CORDIC('circular', 'vectorial', 0, 15)
        C7_LV = CORDIC('linear', 'vectorial', 0, 15)
        C8_HV = CORDIC('hyperbolic', 'vectorial', -1, 14)
        C9_CV = CORDIC('circular', 'vectorial', 0, 15)
        #### STAGE 1 ####
        C1_CV.calculate(xin, yin, 0)
        #### STAGE 2 ####
        r = C1_CV.xo * self.ik
        A = r * self.l1 * 2
        D = self.Ca * ( r**2 + zin**2 + self.S - A )
        #### STAGE 3 ####
        C2_HV.calculate(1, D, 0)
        C4_CV.calculate(r - self.l1, zin, 0)
        #### STAGE 4 ####
        C7_LV.calculate(C4_CV.xo * self.ik, zin, 0)
        C3_CV.calculate(D, C2_HV.xo * self.ikh, 0)
        #### STAGE 5 ####
        C8_HV.calculate(1, C7_LV.zo, 0)
        C5_CR.calculate(self.ik, 0, -C3_CV.zo)
        #### STAGE 6 ####
        C9_CV.calculate(C8_HV.xo * self.ikh, C7_LV.zo, 0)
        C6_CV.calculate(D + self.F, C5_CR.yo, 0)
        #### RESULTS ####
        Q1 = C1_CV.zo
        Q2 = C9_CV.zo - C6_CV.zo
        Q3 = -C3_CV.zo
        return Q1, Q2, Q3
    
    #### Locomotion Functions #################################################
    #### Set Leg Step
    def set_step(self, leg, coordinates):
        self.__coord[leg] = np.array(coordinates)
        self.__locom[leg].append(coordinates)
        self.__joints[leg].append(self.iKinematics(coordinates))
#        if ( leg == 2 ):
#            print(coordinates)
#            print(self.iKinematics(coordinates))
#            print()
        return True
    
    #### Clean all Legs gait buffer
    def step_clean(self):
        for leg in self.__locom:
            leg.clean()
        return True
    
    ####
    def plot_gait(self):
#        fig = plt.figure()
        fig = plt.figure(figsize=plt.figaspect(0.5))
        for i in range ( 6 ):
            loc = self.__locom[i]
            ax = fig.add_subplot(3, 2, i+1, projection='3d')
            ax.plot(loc.x_traject, loc.y_traject, loc.z_traject, c='r') # add label
            ax.set_title('Leg '+str(i+1))
            ax.set_xlabel('x (m)')
            ax.set_ylabel('y (m)')
            ax.set_zlabel('z (m)')
#            ax.legend()
        plt.show()
        return True
    
    ####
    def plot_kinematics(self):
        fig = plt.figure(figsize=plt.figaspect(0.5))
        for i in range ( 6 ):
            loc = self.__locom[i]
            ax = fig.add_subplot(3, 2, i+1)
            ax.plot(loc.x_traject, label='x', c='r') # add label
            ax.plot(loc.y_traject, label='y', c='g') # add label
            ax.plot(loc.z_traject, label='z', c='b') # add label
            ax.set_title('Leg '+str(i+1))
            ax.legend()
        plt.show()
        
    def plot_ikinematics(self):
        fig = plt.figure(figsize=plt.figaspect(0.5))
        for i in range ( 6 ):
            loc = self.__joints[i]
            ax = fig.add_subplot(3, 2, i+1)
            ax.plot(loc.x_traject, label='Q1', c='r') # add label
            ax.plot(loc.y_traject, label='Q2', c='g') # add label
            ax.plot(loc.z_traject, label='Q3', c='b') # add label
            ax.set_title('Leg '+str(i+1))
            ax.legend()
        plt.show()