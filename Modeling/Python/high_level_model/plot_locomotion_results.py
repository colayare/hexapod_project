# -*- coding: utf-8 -*-
import sys, os
import numpy as np
from math import pi as pi
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/../includes')
sys.path.append(abs_path+'/../includes')
from hl_hexapod_class import *
from numeric_conversions import numeric_conversions as NUM_CONV

###############################################################################
#### 
###############################################################################
class container(object):
    __points    = 0
    __coordinates = np.array([0, 0, 0])
    
    @property
    def coordinates(self):
        return self.__coordinates
    
    @property
    def steps(self):
        return self.__coordinates.shape[0]
    
    @property
    def component_0(self):
        return self.__coordinates[:,0]
    
    @property
    def component_1(self):
        return self.__coordinates[:,1]
    
    @property
    def component_2(self):
        return self.__coordinates[:,2]
    
    def append(self, value):
        if ( self.__points > 0 ):
            self.__coordinates = np.vstack((self.__coordinates, value))
        else:
            self.__coordinates = value
        self.__points += 1
        
    def clean(self):
        self.__coordinates = np.array([0, 0, 0])
        self.__points = 0
        
class leg_container(object):
    leg = [
            container(), 
            container(), 
            container(), 
            container(), 
            container(), 
            container()
            ]
    
    def clean(self):
        for idx in self.leg:
            idx.clean()
    

def get_vals(line):
    [x, y, z] = line.split(',')[1:]
    x = np.float32(x)
    y = np.float32(y)
    z = np.float32(z)
    return [x, y, z]

def plot_results(hexapod, title, direct, save_path):
    linewidth=2
    fig = plt.figure(figsize=(16, 12), dpi=150)
    if ( direct ):
        label_0, label_1, label_2 = 'x', 'y', 'z'
        at = 'K_'
    else:
        label_0, label_1, label_2 = 'q1', 'q2', 'q3'
        at = 'IK_'
    plt.title(title)
    for i in range ( 6 ):
        loc = hexapod.leg[i]
        ax = fig.add_subplot(3, 2, i+1)
        ax.plot(loc.component_0, label=label_0, c='r', linewidth=linewidth) # add label
        ax.plot(loc.component_1, label=label_1, c='g', linewidth=linewidth) # add label
        ax.plot(loc.component_2, label=label_2, c='b', linewidth=linewidth) # add label
        ax.set_title('Leg '+str(i+1))
        ax.legend()
    plt.savefig(save_path+at+title.replace('=','_').replace(',','__')+'.png')

###############################################################################
#### 
###############################################################################
log_path = 'C:/xilinx_work_area/hexapod_logs/putty.txt'
figs_path = 'C:/xilinx_work_area/hexapod_logs/'

file = open(log_path, 'r')
cont = np.array(file.read().replace('\n\n','\n').rstrip('\n').split('\n'))
file.close()

step_ctr    = 0
walk_ctr    = 0
angle_ctr   = 0
valid_line  = False
ik_in       = False
ik_out      = False
angle       = 0

hexapod_kinematics = leg_container()
hexapod_ikinematics = leg_container()

#for i in range ( 2418 ):
#    line = cont[i]
for line in cont:
    if ( 'ANGLE' in line ):
        print(line)
        angle = float(line.split(':')[1]) * 180 / pi
        angle = str(round(angle, 2))
        angle_ctr += 1
        step_ctr = 0
    if ( 'Kinematics Input END' in line ):
        title = 'Walk='+str(walk_ctr)+',Angle='+angle
        plot_results(hexapod_kinematics, title, direct=True, save_path=figs_path)
        hexapod_kinematics.clean()
        print(line)
        ik_in = False
    if ( 'Kinematics output END' in line ):
        plot_results(hexapod_ikinematics, title, direct=False, save_path=figs_path)
        hexapod_ikinematics.clean()
        print(line)
        ik_out = False
    if ( 'WALK END' in line ):
        print(line)
        valid_line = False
        walk_ctr += 1
        angle_ctr = 0
    if ( valid_line and ik_in ):
        hexapod_kinematics.leg[int(line[:1])].append(get_vals(line[1:]))
    if ( valid_line and ik_out ):
        hexapod_ikinematics.leg[int(line[:1])].append(get_vals(line[1:]))
        step_ctr += 1
    if ( 'WALK START' in line ):
        print(line)
        valid_line = True
    if ( 'Kinematics Input START' in line ):
        print(line)
        ik_in = True
    if ( 'Kinematics output START' in line ):
        print(line)
        ik_out = True

