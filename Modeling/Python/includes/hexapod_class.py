import numpy as np
from numeric_conversions import numeric_conversions as NUM_CONV
import devmem_map as dm
import math as mt
import os

class hexapod_kinematics:
    #### Kinematics Parameters
    i_pos   = np.zeros(shape=(6,3))
    joints  = np.zeros(shape=(6,3))
    j_offs  = np.zeros(shape=(6,3))
    gaits   = np.zeros(shape=(30,3))
    l1 = 0.0275
    l2 = 0.0963
    l3 = 0.1051
    
    nc = NUM_CONV()
    
    #### Parameters
    init_position_file = ""
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
        else:
            for i in range ( 6 ):
                [off_0, off_1, off_2] = off_cont[i].split(',')
                self.j_offs[i][0] = float(off_0)
                self.j_offs[i][1] = float(off_1)
                self.j_offs[i][2] = float(off_2)
        return True
    
    def import_init_pos(self):
        if ( self.init_position_file == '' or not os.path.exists(self.init_position_file) ):
            print('No joint offsets file detected.')
            return None
        file = open(self.init_position_file, 'r')
        file_cont = file.read().split('\n')
        file.close()
        if ( len(file_cont) < 6 ):
            print('Offsets size missmatch')
        else:
            for i in range ( 6 ):
                [sp_q1, sp_q2, sp_q3] = file_cont[i].split(',')
                self.i_pos[i][0] = float(sp_q1)
                self.i_pos[i][1] = float(sp_q2)
                self.i_pos[i][2] = float(sp_q3)
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
        
    ## Save Joints Offsets
    def save_init_positions(self):
        if ( self.init_position_file == '' or not os.path.exists(self.init_position_file) ):
            print('No initial positions file detected.')
            return None
        
        init_cont = ''
        for i in range ( 6 ):
            for j in range ( 3 ):
                init_cont += str(self.i_pos[i][j])+','
            init_cont = init_cont[:-1]+'\n'
        init_cont = init_cont[:-1]
        init_file = open(self.init_position_file, 'w+')
        init_file.write(init_cont)
        init_file.close()
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
        x, y, z = self.gaits[idx]
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
    def rad2sec(self, rad):
        return rad * 180 / mt.pi
    
    def sec2rad(self, sec):
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
    
    # Write iKinematics Parameters on input FIFO
    def axi_write_params_in(self, x_in, y_in, z_in):
        self.axi_ip.axi_write(2, x_in)
        self.axi_ip.axi_write(3, y_in)
        self.axi_ip.axi_write(4, z_in)
        return True
    
    def axi_read_params(self):
        x = self.axi_ip.axi_read(2)
        y = self.axi_ip.axi_read(3)
        z = self.axi_ip.axi_read(4)
        return [x, y, z]
    
    # Write iKinematics parameters to input FIFO
    def axi_write_fifo(self):
        self.axi_ip.axi_write(0, 1)
        self.axi_ip.axi_write(0, 0)
        return True
    
    # Trigger iKinematics Calculation
    def axi_trigger_ikinematics(self):
        self.axi_ip.axi_write(0, 2)
        self.axi_ip.axi_write(0, 0)
        return True
    
    # Write output directly without calculation
    def axi_write_out_direct(self):
        self.axi_ip.axi_write(0, 4)
        self.axi_ip.axi_write(0, 0)
        return True
    
    # Set Leg configuration
    def axi_set_leg_conf(self, conf, leg_select):
        reg1 = int(self.axi_ip.axi_read(1), 16)
        set_val = 8 + (leg_select & 0x7)
        config = (conf<<4) & 0x30
        value = set_val + config + reg1
        self.axi_ip.axi_write(1, value)
        return True
    
    # Set output multiplexor
    def axi_set_out_mux(self, selector):
        mux_sel = (selector & 0x7) << 13
        reg1 = int(self.axi_ip.axi_read(1), 16)
        
        write_val = (reg1 & ~(0x7 << 13)) + mux_sel
        self.axi_ip.axi_write(1, write_val)
        return True
    
    # Set hexapod offsets
    def axi_set_offset(self, leg, off_q1, off_q2, off_q3):
        self.axi_ip.axi_write(5+leg*3, off_q1)
        self.axi_ip.axi_write(6+leg*3, off_q2)
        self.axi_ip.axi_write(7+leg*3, off_q3)
        return True
    
    # Set read offsets
    def set_default_offsets(self):
        for i in range(6):
            offset_q1 = self.nc.dfloat2hfloat(self.sec2rad(self.j_offs[i][0])).lstrip('x')
            offset_q2 = self.nc.dfloat2hfloat(self.sec2rad(self.j_offs[i][1])).lstrip('x')
            offset_q3 = self.nc.dfloat2hfloat(self.sec2rad(self.j_offs[i][2])).lstrip('x')
            print('Setting leg '+str(i)+' offset')
            print('\tQ1 offset = 0x'+offset_q1+' | '+str(self.sec2rad(self.j_offs[i][0])))
            print('\tQ2 offset = 0x'+offset_q2+' | '+str(self.sec2rad(self.j_offs[i][1])))
            print('\tQ3 offset = 0x'+offset_q3+' | '+str(self.sec2rad(self.j_offs[i][2])))

            self.axi_set_offset(i, int(offset_q1,16), int(offset_q2,16), int(offset_q3,16))
        return True
    
    # Set read offsets
    def set_init_position(self):
        for i in range(6):
            q1 = self.nc.dfloat2hfloat(self.sec2rad(self.i_pos[i][0]+self.j_offs[i][0])).lstrip('x')
            q2 = self.nc.dfloat2hfloat(self.sec2rad(self.i_pos[i][1]+self.j_offs[i][1])).lstrip('x')
            q3 = self.nc.dfloat2hfloat(self.sec2rad(self.i_pos[i][2]+self.j_offs[i][2])).lstrip('x')
            
            self.axi_set_out_mux(i+1)
            self.axi_write_params_in(q1, q2, q3)
            self.axi_write_out_direct()
            
            [pwm1, pwm2, pwm3] = self.axi_get_pwm(int(i))
            
            print('Setting leg '+str(i)+' direct output')
            print('\tQ1 = 0x'+q1+' | '+str(self.nc.hfloat2dfloat(q1))+' | '+str(self.rad2sec(self.nc.hfloat2dfloat(q1))+' | '+str(pwm1)))
            print('\tQ2 = 0x'+q1+' | '+str(self.nc.hfloat2dfloat(q2))+' | '+str(self.rad2sec(self.nc.hfloat2dfloat(q2))+' | '+str(pwm2)))
            print('\tQ3 = 0x'+q1+' | '+str(self.nc.hfloat2dfloat(q3))+' | '+str(self.rad2sec(self.nc.hfloat2dfloat(q3))+' | '+str(pwm3)))

            self.axi_set_offset(i, q1, q2, q3)
        return True
    
    # Read offsets
    def axi_read_offset(self, leg):
        off1 = self.axi_ip.axi_read(5+leg*3)
        off2 = self.axi_ip.axi_read(6+leg*3)
        off3 = self.axi_ip.axi_read(7+leg*3)
        return [off1, off2, off3]
    
    # Get PWM values
    def axi_get_pwm(self, leg):
        pwm1 = (int(self.axi_ip.axi_read(23+leg*3),16) & 0x3FC00) >> 10
        pwm2 = (int(self.axi_ip.axi_read(24+leg*3),16) & 0x3FC00) >> 10
        pwm3 = (int(self.axi_ip.axi_read(25+leg*3),16) & 0x3FC00) >> 10
        return [pwm1, pwm2, pwm3]
