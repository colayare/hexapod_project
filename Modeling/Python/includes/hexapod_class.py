# -*- coding: utf-8 -*-
import numpy as np
import mmap
from time import sleep as sleep
from cordic_model import CORDIC as CORDIC
from numeric_conversions import numeric_conversions as numeric_conversions
from devmem_map import axi_ip_mmap as axi_ip_mmap
from iKinematics_IP import iKinematics_IP as REGMAP
import math as mt
import os

###############################################################################
#### Hexapod Class : Base AXI IP
###############################################################################
class hexapod_kinematics(axi_ip_mmap, numeric_conversions):
    ###########################################################################
    #### Properties
    ###########################################################################
    ## Kinematics Parameters
    i_pos   = np.zeros(shape=(6,3))         # Initial Joints Positions
    j_offs  = np.zeros(shape=(6,3))         # Joints Offsets
    i_inv_s = np.zeros(18).astype(int).astype(str)
    joints  = np.zeros(shape=(6,3)).astype(np.float32)
    gait    = np.array([]).astype(bytes)                  # Gaits
    bgaits  = np.zeros(shape=(30,3))
    delay   = 0.008
    
    #### Properties
    @property
    def step_size(self):
        return int(self.gait.size/6/3)
    
    #### Parameters
    init_position_file_path     = ""
    offsets_file_path           = ""
    init_servo_inv_file_path    = ""
    gait_steps_file_path        = ""
    axi_ip_log_file_path        = ""
    
    ###########################################################################
    #### Memory Map Parameter
    ###########################################################################
    R1_LEG_IN_SELCT             = 0x7
    R1_SET_LEG_IN               = 0x8
    R1_COUNTER_MODE             = 0x30
    R1_LEG_OUTPUT               = 0xC0
    R1_F2F_READ_MUX             = 0xE00
    R1_PWM_INVERT               = 0xFFFFC0000 #CHECK !!!!! 0x3FFFF000‬
    
    ###########################################################################
    #### Methods
    ###########################################################################
    #### Constructor
    def __init__(self, invoke_axi_ip=True, enable_ip_logs=False):
        if ( invoke_axi_ip ):
            super(hexapod_kinematics, self).__init__(enable_ip_logs=enable_ip_logs)
        return None
    
    #### Import & Export IP Params ############################################
    ## Import parameters
    def import_offsets(self):
        if ( self.offsets_file_path == '' or not os.path.exists(self.offsets_file_path) ):
            print('HEXAPOD CLASS > No joint offsets file detected.\n>'+self.offsets_file_path)
            return False
        off_file = open(self.offsets_file_path, 'r')
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
        print('HEXAPOD CLASS > Import offsets successfuly : '+self.offsets_file_path)
        return True
    
    def import_init_pos(self):
        if ( self.init_position_file_path == '' or not os.path.exists(self.init_position_file_path) ):
            print('HEXAPOD CLASS > No initial positions file detected.\n>'+self.init_position_file_path)
            return False
        file = open(self.init_position_file_path, 'r')
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
        print('HEXAPOD CLASS > Import initial possitions successfuly : '+self.init_position_file_path)
        return True
        
    def import_init_servo_invertion(self):
        if ( self.init_servo_inv_file_path == '' or not os.path.exists(self.init_servo_inv_file_path) ):
            print('HEXAPOD CLASS > No init servo inversion file detected.n>'+self.init_servo_inv_file_path)
            return False
        file        = open(self.init_servo_inv_file_path, 'r')
        file_cont   = file.read().rstrip('\n').split('\n')
        file.close()
        self.i_inv_s = np.array(file_cont)
        for i, val in enumerate(self.i_inv_s):
            self.axi_set_pwm_inv(i, int(val))
        print('HEXAPOD CLASS > Import servo inversions successfuly : '+self.init_servo_inv_file_path)
        return None
    
    ## Save Joints Offsets
    def save_offsets(self):
        if ( self.offsets_file_path == '' or not os.path.exists(self.offsets_file_path) ):
            print('HEXAPOD CLASS > No joint offsets file detected.\n>'+self.offsets_file_path)
            return None
        off_cont = ''
        for i in range ( 6 ):
            for j in range ( 3 ):
                off_cont += str(self.j_offs[i][j])+','
            off_cont = off_cont[:-1]+'\n'
        off_cont = off_cont[:-1]
        off_file = open(self.offsets_file_path, 'w+')
        off_file.write(off_cont)
        off_file.close()
        return True
        
    ## Save Joints Offsets
    def save_init_positions(self):
        if ( self.init_position_file_path == '' or not os.path.exists(self.init_position_file_path) ):
            print('HEXAPOD CLASS > No initial positions file detected.\n>'+self.init_position_file_path)
            return None
        init_cont = ''
        for i in range ( 6 ):
            for j in range ( 3 ):
                init_cont += str(self.i_pos[i][j])+','
            init_cont = init_cont[:-1]+'\n'
        init_cont = init_cont[:-1]
        init_file = open(self.init_position_file_path, 'w+')
        init_file.write(init_cont)
        init_file.close()
        return True
    
    ## Save Servo Inversion
    def save_inversion(self):
        if ( self.init_servo_inv_file_path == '' or not os.path.exists(self.init_servo_inv_file_path) ):
            print('HEXAPOD CLASS > No init servo inversion file detected.n>'+self.init_servo_inv_file_path)
            return False
        inv_cont = '\n'.join(self.i_inv_s)
        inv_file = open(self.init_servo_inv_file_path, 'w+')
        inv_file.write(inv_cont)
        inv_file.close()
        return True
    
    ## Import Gait Steps
    def read_gait_steps(self):
        for leg in range ( 6 ):
            file_path = self.gait_steps_file_path.replace('*', str(leg))
            if ( file_path == '' or not os.path.exists(file_path) ):
                print('HEXAPOD CLASS > No gait steps file detected.\n>'+file_path)
                return None
            f = open(file_path, 'r+b')
            mm = mmap.mmap(f.fileno(), 0)
            for i in range ( int(mm.size()/4) ):
                self.gait = np.append(self.gait, mm.read(4))
            f.close()
            mm = None
        return True
    
    #### Gaits Process ########################################################        
    def run_fast_gait(self):
        for step in range ( self.step_size ):
            self.config_leg_ctr(1, 0)
            for leg in range ( 6 ):
                self.set_ptr(REGMAP.REG_IKIX)
                for axi in range ( 3 ):
                    self.axi_map.write(self.gait[leg*self.step_size*3+step*3+axi])
                self.axi_write_fifo()
            self.axi_trigger_ikinematics()
            sleep(self.delay)
        return True
    
    def set_step(self):
        self.config_leg_ctr(1, 0)
        for leg in self.joints:
            self.set_ptr(REGMAP.REG_IKIX)
            for axis in leg:
                self.axi_map.write( self.to_bytes(int(self.dfloat2hfloat(axis), 16))  )
            self.axi_write_fifo()
        self.axi_trigger_ikinematics()
        return None
    
    def set_step_debug_leg(self, leg_selector=0):
        self.config_leg_ctr(1, 0)
        for i, leg in enumerate(self.joints):
            self.set_ptr(REGMAP.REG_IKIX)
            for axis in leg:
                self.axi_map.write( self.to_bytes(int(self.dfloat2hfloat(axis), 16))  )
            self.axi_write_fifo()
            self.axi_trigger_ikinematics()
            if ( i == leg_selector ):
                self.axi_set_out_mux(0)
                [q1, q2, q3] = self.axi_hread_params()
                print(leg)
                print(self.hfloat2dfloat(q1), self.hfloat2dfloat(q2), self.hfloat2dfloat(q3) )
        return None
    
    def step_delay(self):
        sleep(self.delay)
    
    #### AXI IP Handling ######################################################
    #### Configure Leg Control
    ## Reg 1 : 
    ## > [2:0] RW   = Leg input selector
    ## > [3] T      = Set leg input trigger
    ## Inputs
    ## mode : str
    ## > 'one_leg' : Keep leg pointer value
    ## > 'mux_eg'  : Auto-increasses leg pointer
    ## leg : 32-bit int
    ## > 0-5 : Selects the output leg
    def config_leg_ctr(self, mode, leg):
        if ( self.enable_ip_logs ):
            self.log_file += 'config_leg_ctr: start\n'
        mask    = self.R1_LEG_IN_SELCT + self.R1_SET_LEG_IN + self.R1_COUNTER_MODE
        leg_in  = leg & 0x7
        if ( mode == "one_leg" or mode == 0 ):
            mode_in = 0x10
        elif ( mode == "mux_leg" or mode == 1 ):
            mode_in = 0x0
        else:
            print("Invalid mode, selected leg multiplexion")
            mode_in = 0x0
        self.axi_write_mask(REGMAP.REG_LEGC, leg_in + 0x8 + mode_in, mask)
        if ( self.enable_ip_logs ):
            self.log_file += 'config_leg_ctr: end\n'
        return True
    
    ## Set Leg configuration
    def axi_set_leg_conf(self, conf, leg_select):
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_leg_conf: start\n'
        mask        = self.R1_LEG_IN_SELCT + self.R1_SET_LEG_IN + self.R1_COUNTER_MODE
        reg1        = int(self.axi_read(1), 16)
        set_val     = 8 + (leg_select & 0x7)
        config      = (conf<<4) & 0x30  
        value       = set_val + config + reg1
        self.axi_write_mask(REGMAP.REG_LEGC, value, mask)
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_leg_conf: end\n'
        return True
    
    ## Write iKinematics Parameters on input FIFO
    def axi_write_params_in(self, x_in, y_in, z_in):
        if ( type(x_in) == str or type(y_in) is str or type(z_in) is str ):
            self.axi_hwrite(REGMAP.REG_IKIX, x_in)
            self.axi_hwrite(REGMAP.REG_IKIY, y_in)
            self.axi_hwrite(REGMAP.REG_IKIZ, z_in)
        else:
            self.axi_write(REGMAP.REG_IKIX, x_in)
            self.axi_write(REGMAP.REG_IKIY, y_in)
            self.axi_write(REGMAP.REG_IKIZ, z_in)
        return True
    
    def axi_read_params(self):
        x = self.axi_read(REGMAP.REG_IKIX)
        y = self.axi_read(REGMAP.REG_IKIY)
        z = self.axi_read(REGMAP.REG_IKIZ)
        return [x, y, z]
        
    def axi_hread_params(self):
        x = self.axi_hread(REGMAP.REG_IKIX)
        y = self.axi_hread(REGMAP.REG_IKIY)
        z = self.axi_hread(REGMAP.REG_IKIZ)
        return [x, y, z]
    
    ## Write iKinematics parameters to input FIFO
    def axi_write_fifo(self):
        self.axi_write(REGMAP.REG_TRIG, 1)
        self.axi_write(REGMAP.REG_TRIG, 0)
        return True
    
    ## Trigger iKinematics Calculation
    def axi_trigger_ikinematics(self):
        self.axi_write(REGMAP.REG_TRIG, 2)
        self.axi_write(REGMAP.REG_TRIG, 0)
        return True
    
    ## Write output directly without calculation
    def axi_write_out_direct(self):
        self.axi_write(REGMAP.REG_TRIG, 4)
        self.axi_write(REGMAP.REG_TRIG, 0)
        return True
    
    ## Set PWM Channel Inversion bit
    def axi_set_pwm_inv(self, pwm_idx, val):
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_pwm_inv: start\n'
        idx_mask    = 1 << (pwm_idx+12)
        set_val     = val << (pwm_idx+12)
        self.axi_write_mask(REGMAP.REG_LEGC, set_val, idx_mask)
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_pwm_inv: end\n'
        return True
    
    ## Set output multiplexor
    ## f2f_mux_selector offset
    def axi_set_out_mux(self, selector):
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_out_mux: start\n'
        mask        = self.R1_F2F_READ_MUX
        mux_sel     = (selector & 0x7) << 9
        self.axi_write_mask(REGMAP.REG_LEGC, mux_sel, mask)
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_out_mux: end\n'
        return True
    
    # Set hexapod offsets
    def axi_set_offset(self, leg, off_q1, off_q2, off_q3):
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_offset: start\n'
        self.axi_write(REGMAP.REG_OF01+leg*3, off_q1)
        self.axi_write(REGMAP.REG_OF02+leg*3, off_q2)
        self.axi_write(REGMAP.REG_OF03+leg*3, off_q3)
        if ( self.enable_ip_logs ):
            self.log_file += 'axi_set_offset: end\n'
        return True
    
    # Set default offsets
    def set_default_offsets(self, print_out=False):
        for i in range(6):
            offset_q1 = self.dfloat2hfloat(self.sec2rad(self.j_offs[i][0])).lstrip('x')
            offset_q2 = self.dfloat2hfloat(self.sec2rad(self.j_offs[i][1])).lstrip('x')
            offset_q3 = self.dfloat2hfloat(self.sec2rad(self.j_offs[i][2])).lstrip('x')
            
            if ( print_out ):
                print('Setting leg '+str(i)+' offset')
                print('\tQ1 offset = 0x'+offset_q1+' | '+str(self.sec2rad(self.j_offs[i][0])))
                print('\tQ2 offset = 0x'+offset_q2+' | '+str(self.sec2rad(self.j_offs[i][1])))
                print('\tQ3 offset = 0x'+offset_q3+' | '+str(self.sec2rad(self.j_offs[i][2])))

            self.axi_set_offset(i, int(offset_q1,16), int(offset_q2,16), int(offset_q3,16))
        return True
    
    # Set initial position
    def set_init_position(self, print_out=False):
        if ( self.enable_ip_logs ):
            self.log_file += 'set_init_position: start\n'
        for i in range(6):
            q1 = self.dfloat2hfloat(self.sec2rad(self.i_pos[i][0]+self.j_offs[i][0])).lstrip('x')
            q2 = self.dfloat2hfloat(self.sec2rad(self.i_pos[i][1]+self.j_offs[i][1])).lstrip('x')
            q3 = self.dfloat2hfloat(self.sec2rad(self.i_pos[i][2]+self.j_offs[i][2])).lstrip('x')
            
            self.config_leg_ctr("one_leg", i)
            self.axi_set_out_mux(i+1)
            self.axi_write_params_in(q1, q2, q3)
            self.axi_write_out_direct()
            
            if ( print_out ):
                [pwm1, pwm2, pwm3] = self.axi_get_pwm(int(i))
                
                print('Setting leg '+str(i)+' direct output')
                print('\tQ1 = 0x'+q1+' | '+str(self.hfloat2dfloat(q1))+' | '+str(self.rad2sec(self.hfloat2dfloat(q1)))+' | '+str(pwm1))
                print('\tQ2 = 0x'+q1+' | '+str(self.hfloat2dfloat(q2))+' | '+str(self.rad2sec(self.hfloat2dfloat(q2)))+' | '+str(pwm2))
                print('\tQ3 = 0x'+q1+' | '+str(self.hfloat2dfloat(q3))+' | '+str(self.rad2sec(self.hfloat2dfloat(q3)))+' | '+str(pwm3))
        if ( self.enable_ip_logs ):
            self.log_file += 'set_init_position: end\n'

        return True
    
    # Read offsets
    def axi_read_offset(self, leg):
        off1 = self.axi_read(REGMAP.REG_OF01+leg*3)
        off2 = self.axi_read(REGMAP.REG_OF02+leg*3)
        off3 = self.axi_read(REGMAP.REG_OF03+leg*3)
        return [off1, off2, off3]
    
    # Read offsets
    def axi_hread_offset(self, leg):
        off1 = self.axi_hread(REGMAP.REG_OF01+leg*3)
        off2 = self.axi_hread(REGMAP.REG_OF02+leg*3)
        off3 = self.axi_hread(REGMAP.REG_OF03+leg*3)
        return [off1, off2, off3]
    
    # Get PWM values
    def axi_get_pwm(self, leg):
        pwm1 = (int(self.axi_hread(REGMAP.REG_KO01+leg*3),16) & 0x3FC00) >> 10
        pwm2 = (int(self.axi_hread(REGMAP.REG_KO02+leg*3),16) & 0x3FC00) >> 10
        pwm3 = (int(self.axi_hread(REGMAP.REG_KO03+leg*3),16) & 0x3FC00) >> 10
        return [pwm1, pwm2, pwm3]
