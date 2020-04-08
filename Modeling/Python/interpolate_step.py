# -*- coding: utf-8 -*-
"""
Created on Mon Nov 25 21:41:06 2019

@author: Roy
"""

#### Generic Imports
import os
import sys
import time as tm
import math as mt
import numpy as np
try:
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    #
    PLOT_EN = True
except ImportError:
    PLOT_EN = False
    pass
#### Append includes path           
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/includes')
sys.path.append(abs_path+'/includes')
from hexapod_class import hexapod_kinematics as hexapod_kinematics
from numeric_conversions import numeric_conversions as NUM_CONV

#### Path sourcing
# Params
params_dir  = abs_path+"/params"
# Reference steps
ref_path    = params_dir+"/gait_steps.params"

#### Functions
def read_gait_steps(gait, ref_path):
    if ( not os.path.exists(ref_path) ):
        print('No reference gait steps file detected.')
        exit()
    gait_file = open(ref_path, 'r')
    gait_file_cont = gait_file.read().split('\n')[0:3]
    gait_file.close()
    nc = NUM_CONV()
    gaits   = np.zeros(shape=(30,3))
    for i in range(30):
        gaits[i][0] = nc.hfloat2dfloat(gait_file_cont[0].split(",")[(gait*30)+i])
        gaits[i][1] = nc.hfloat2dfloat(gait_file_cont[1].split(",")[(gait*30)+i])
        gaits[i][2] = nc.hfloat2dfloat(gait_file_cont[2].split(",")[(gait*30)+i])
    return np.transpose(gaits)

def step_interpolate(gait, points, scale):
    lines = np.linspace(0, 30, 30)
    lin_i = np.linspace(0, 30, points)
    gaits   = np.zeros(shape=(3,points))
    for i in range (3):
        inter = np.interp(lin_i, lines, gait[i])
        gaits[i] = inter * scale
    return gaits

def get_ikinematics(hexapod, gaits, points):
    ik   = np.zeros(shape=(3,points))
    for i in range (points):
        [q1, q2, q3] = hexapod.iKinematics( gaits[0][i], gaits[1][i], gaits[2][i] )
        ik[0][i] = hexapod.rad2sec(q1)
        ik[1][i] = hexapod.rad2sec(q2)
        ik[2][i] = hexapod.rad2sec(q3)
    return ik
    
#### Code
hexapod = hexapod_kinematics(invoke_axi_ip=False)
points = 100
scale  = 1.2

for i in range (4):
    gaits = read_gait_steps(i, ref_path)
    inter = step_interpolate(gaits, points, scale)
    
    ik_i = get_ikinematics(hexapod, gaits, 30)
    ik_o = get_ikinematics(hexapod, inter, points)
    
    if ( PLOT_EN ):
        fig = plt.figure(figsize=plt.figaspect(0.5))
        
        ax = fig.add_subplot(2, 2, 1, projection='3d')
        ax.plot(gaits[0], gaits[1], gaits[2], label='Reference', c='r', marker='o')
        ax.legend()
        
        ax = fig.add_subplot(2, 2, 2, projection='3d')
        ax.plot(inter[0], inter[1], inter[2], label='Interpolated', c='b', marker='o')
        ax.legend()
        
        ax = fig.add_subplot(2, 2, 3)
        ax.plot(ik_i[0], c='r', label='Q1')
        ax.plot(ik_i[1], c='g', label='Q2')
        ax.plot(ik_i[2], c='b', label='Q3')
        ax.legend()
        
        ax = fig.add_subplot(2, 2, 4)
        ax.plot(ik_o[0], label='Q1', c='r')
        ax.plot(ik_o[1], label='Q2', c='g')
        ax.plot(ik_o[2], label='Q3', c='b')
        ax.legend()
        
        plt.show()