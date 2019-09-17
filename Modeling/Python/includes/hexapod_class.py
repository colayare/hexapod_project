import numpy as np
import numeric_conversions as nc
import devmem_map as dm
import math as mt
import os

class hexapod_kinematics:
    #### Kinematics Parameters
    ee_pos = np.zeros(shape=(6,3))
    joints = np.zeros(shape=(6,3))
    j_offs = np.zeros(shape=(6,3))
    gaits   = np.zeros(shape=(30,3))
    l1 = 0.0275
    l2 = 0.0963
    l3 = 0.1051
    
    #### Parameters
    offsets_file = ""
    gait_steps_file = ""
    
    #### Constructor
    def __init__(self):
        return None
    
    def init_axi(self):
        self.axi_ip = dm.ikinematics_mmap()
        return True
    
    #### Functions
    ## Import parameters
    def import_offsets(self):
        if ( self.offsets_file == '' or not os.path.exists(self.offsets_file) ):
            print('No joint offsets file detected.')
            return None
        off_file = open(self.offsets_file, 'r')
        off_cont = off_file.read().split('\n')
        off_file.close()
        if ( len(off_cont) < 6 ):
            print('Offsets size missmatch')
        for i in range ( 6 ):
            [off_0, off_1, off_2] = off_cont[i].split(',')
            self.j_offs[i][0] = float(off_0)
            self.j_offs[i][1] = float(off_1)
            self.j_offs[i][2] = float(off_2)
        return True
    
    ## Save Joints Offsets
    def save_offsets(self):
        if ( self.offsets_file == '' or not os.path.exists(self.offsets_file) ):
            print('No joint offsets file detected.')
            return None
        
        off_cont = ''
        for i in range ( 6 ):
            for j in range ( 3 ):
                off_cont += str(self.j_offs[i][j])+','
            off_cont = off_cont[:-1]+'\n'
        off_cont = off_cont[:-1]
        off_file = open(self.offsets_file, 'w+')
        off_file.write(off_cont)
        off_file.close()
        return True
    
    ## Import Gait Steps
    def read_gait_steps(self, gait):
        if ( self.gait_steps_file == '' or not os.path.exists(self.gait_steps_file) ):
            print('No gait steps file detected.')
            return None
        gait_file = open(self.gait_steps_file, 'r')
        gait_file_cont = gait_file.read().split('\n')[0:3]
        gait_file.close()
        for i in range(30):
            self.gaits[i][0] = int(gait_file_cont[0].split(",")[(gait*30)+i], 16)
            self.gaits[i][1] = int(gait_file_cont[1].split(",")[(gait*30)+i], 16)
            self.gaits[i][2] = int(gait_file_cont[2].split(",")[(gait*30)+i], 16)
        return True
    
    def gait_step(self, idx):
        x, y, z = self.gaits[0]
        return x, y, z
    
    ## Kinematics Functions
    def dKinematics(self, q1, q2, q3):
        x = mt.cos(q1)*(self.l3*mt.cos(q2+q3) + self.l2*mt.cos(q2) + self.l1)
        y = mt.sin(q1)*(self.l3*mt.cos(q2+q3) + self.l2*mt.cos(q2) + self.l1) 
        z = self.l3 * mt.sin(q2+q3) + self.l2*mt.sin(q2)
        return x, y, z
    
    def iKinematics(self, xin, yin, zin):
        if ( type(xin) is str ):
            x = nc.hfloat2dfloat(xin)
            y = nc.hfloat2dfloat(yin)
            z = nc.hfloat2dfloat(zin)
        elif ( type(xin) is float ):
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
    
    ## Numeric functions
    def rad2sec(rad):
        return rad * 180 / mt.pi
    
    def sec2rad(sec):
        return sec * mt.pi / 180
    
    #### Execution Functions
    def config_leg_ctr(self, mode, leg):
        leg_in = leg & 0x7
        if ( mode == "one_leg" ):
            mode_in = 0x10
        elif ( mode == "mux_leg" ):
            mode_in = 0x0
        else:
            print("Invalid mode, selected leg multiplexion")
            mode_in = 0x0
        self.axi_ip.axi_write(1, leg_in + 0x8 + mode_in)
        return True
    
    def axi_write_fifo(self):
        self.axi_ip.axi_write(0, 1)
        self.axi_ip.axi_write(0, 0)
        return True
    
    def axi_trigger_ikinematics(self):
        self.axi_ip.axi_write(0, 2)
        self.axi_ip.axi_write(0, 0)
        return True
