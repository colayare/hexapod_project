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

def save_results(hexapod, title, direct, save_path):
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
    
def plot_results(hexapod):
    linewidth=2
    fig = plt.figure()
    for i in range ( 6 ):
        loc = hexapod.leg[i]
        ax = fig.add_subplot(3, 2, i+1)
        ax.plot(loc.component_0, label='x', c='r', linewidth=linewidth) # add label
        ax.plot(loc.component_1, label='y', c='g', linewidth=linewidth) # add label
        ax.plot(loc.component_2, label='z', c='b', linewidth=linewidth) # add label
        ax.set_title('Leg '+str(i+1))
        ax.legend()
    plt.show()

###############################################################################
#### 
###############################################################################
cpp_log_path = 'C:/xilinx_work_area/hexapod_logs/cpp_log.txt'
py_log_path  = 'C:/xilinx_work_area/hexapod_logs/py_log.txt'


cpp_file = open(cpp_log_path, 'r')
cppcont = np.array(cpp_file.read().replace('\n\n','\n').rstrip('\n').split('\n'))
cpp_file.close()

py_file = open(py_log_path, 'r')
pycont = np.array(py_file.read().replace('\n\n','\n').rstrip('\n').split('\n'))
py_file.close()

cpp_ik = leg_container()

cpp_valid = False
py_valid  = False

for line in cppcont:
    if ( line == " --- END --- " ):
        cpp_valid = False
        plot_results(cpp_ik)
    if ( cpp_valid ):
        cpp_ik.leg[int(line[:1])-1].append(get_vals(line[1:]))
    if ( line == " --- START --- " ):
        cpp_valid = True

cpp_ik = ''
py_ik  = leg_container()
py_ik.clean()

for line in pycont:
    if ( line == " --- END --- " ):
        py_valid = False
        plot_results(py_ik)
    if ( py_valid ):
        py_ik.leg[int(line[:1])].append(get_vals(line[1:]))
    if ( line == " --- START --- " ):
        py_valid = True