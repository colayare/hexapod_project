import numpy as np
from numeric_conversions import numeric_conversions as numeric_conversions
from devmem_map import ikinematics_mmap as ikinematics_mmap
import math as mt
import os

################################################################################
#### Hexapod Class
################################################################################
class hexapod_kinematics(ikinematics_mmap, numeric_conversions):
    ############################################################################
    #### Properties
    ############################################################################
    ## Kinematics Parameters
    i_pos   = np.zeros(shape=(6,3))
    joints  = np.zeros(shape=(6,3))
    j_offs  = np.zeros(shape=(6,3))
    i_inv_s = np.zeros(18).astype(int).astype(str)
    gaits   = np.zeros(shape=(30,3))
    gait    = 0
    steps   = 30
    scale   = 1
    l1      = 0.0275
    l2      = 0.0963
    l3      = 0.1051
    
    #### Parameters
    init_position_file_path     = ""
    offsets_file_path           = ""
    init_servo_inv_file_path    = ""
    gait_steps_file_path        = ""
    axi_ip_log_file_path        = ""
    
    ############################################################################
    #### Defines
    ############################################################################
    R1_LEG_IN_SELCT             = 0x7
    R1_SET_LEG_IN               = 0x8
    R1_COUNTER_MODE             = 0x30
    R1_LEG_OUTPUT               = 0xC0
    R1_F2F_READ_MUX             = 0xE00
    R1_PWM_INVERT               = 0x‭FFFFC0000‬
    
    ############################################################################
    #### Methods
    ############################################################################
    #### Constructor
    def __init__(self, enable_ip_logs=False):
        super(hexapod_kinematics, self).__init__(gen_log_enable=enable_ip_logs)
        return None
    
    #### Import & Export IP Params #############################################
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
        file_cont   = file.read().split('\n')[:-1]
        file.close()
        self.i_inv_s = np.array(file_cont)
        inv_val     = int(''.join(file_cont[::-1]), 2) << 12
        reg1        = int(self.axi_read(1), 16) & (0xFFFFFFFF ^ (0x3FFFF << 12))
        set_val     = reg1 + inv_val
        print('HEXAPOD CLASS > Import servo inversions successfuly : '+self.init_servo_inv_file_path)
        self.axi_write(1, set_val)
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
    def read_gait_steps(self, gait, interp_points=30, scale=1):
        if ( self.gait_steps_file_path == '' or not os.path.exists(self.gait_steps_file_path) ):
            print('HEXAPOD CLASS > No gait steps file detected.\n>'+self.gait_steps_file_path)
            return None
        gait_file = open(self.gait_steps_file_path, 'r')
        gait_file_cont = gait_file.read().split('\n')[0:3]
        gait_file.close()
        gaits   = np.zeros(shape=(30,3))
        for i in range(30):
            gaits[i][0] = self.hfloat2dfloat(gait_file_cont[0].split(",")[(gait*30)+i])
            gaits[i][1] = self.hfloat2dfloat(gait_file_cont[1].split(",")[(gait*30)+i])
            gaits[i][2] = self.hfloat2dfloat(gait_file_cont[2].split(",")[(gait*30)+i])
        
        if ( interp_points > 30 ):
            self.gait  = gait
            self.scale = scale
            self.steps = interp_points
            self.gaits = self.step_interpolate(np.transpose(gaits), interp_points, scale)
        else:
            self.gait  = gait
            self.scale = scale
            self.steps = interp_points
            self.gaits = gaits
        return True
    
	#### Gaits Process 	########################################################
    def gait_step(self, idx):
        x, y, z = self.gaits[idx]
        return self.dfloat2hfloat(x), self.dfloat2hfloat(y), self.dfloat2hfloat(z)
        
    def step_interpolate(self, gait, points, scale):
        lines = np.linspace(0, 30, 30)
        lin_i = np.linspace(0, 30, points)
        gaits   = np.zeros(shape=(3,points))
        for i in range (3):
            inter = np.interp(lin_i, lines, gait[i])
            gaits[i] = inter * scale
        return np.transpose(gaits)
    
	#### Inverse Kinematics Functions ##########################################
    ## Kinematics Functions
    def dKinematics(self, q1, q2, q3):
        x = mt.cos(q1)*(self.l3*mt.cos(q2+q3) + self.l2*mt.cos(q2) + self.l1)
        y = mt.sin(q1)*(self.l3*mt.cos(q2+q3) + self.l2*mt.cos(q2) + self.l1) 
        z = self.l3 * mt.sin(q2+q3) + self.l2*mt.sin(q2)
        return x, y, z
    
    def iKinematics(self, xin, yin, zin):
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
    
    #### AXI IP Handling #######################################################
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
        if ( self.gen_log_enable ):
            self.log_file += 'config_leg_ctr:\n'
        mask    = self.R1_LEG_IN_SELCT + self.R1_SET_LEG_IN + self.R1_COUNTER_MODE
        leg_in  = leg & 0x7
        if ( mode == "one_leg" ):
            mode_in = 0x10
        elif ( mode == "mux_leg" ):
            mode_in = 0x0
        else:
            print("Invalid mode, selected leg multiplexion")
            mode_in = 0x0
        self.axi_write_mask(1, leg_in + 0x8 + mode_in, mask)
        return True
    
    ## Set Leg configuration
    def axi_set_leg_conf(self, conf, leg_select):
        if ( self.gen_log_enable ):
            self.log_file += 'axi_set_leg_conf:\n'
        mask        = self.R1_LEG_IN_SELCT + self.R1_SET_LEG_IN + self.R1_COUNTER_MODE
        reg1        = int(self.axi_read(1), 16)
        set_val     = 8 + (leg_select & 0x7)
        config      = (conf<<4) & 0x30
        value       = set_val + config + reg1
        self.axi_write_mask(1, value, mask)
        return True
    
    ## Write iKinematics Parameters on input FIFO
    def axi_write_params_in(self, x_in, y_in, z_in):
        self.axi_write(2, x_in)
        self.axi_write(3, y_in)
        self.axi_write(4, z_in)
        return True
    
    def axi_read_params(self):
        x = self.axi_read(2)
        y = self.axi_read(3)
        z = self.axi_read(4)
        return [x, y, z]
    
    ## Write iKinematics parameters to input FIFO
    def axi_write_fifo(self):
        self.axi_write(0, 1)
        self.axi_write(0, 0)
        return True
    
    ## Trigger iKinematics Calculation
    def axi_trigger_ikinematics(self):
        self.axi_write(0, 2)
        self.axi_write(0, 0)
        return True
    
    ## Write output directly without calculation
    def axi_write_out_direct(self):
        self.axi_write(0, 4)
        self.axi_write(0, 0)
        return True
    
    ## Set PWM Channel Inversion bit
    def axi_set_pwm_inv(self, pwm_idx, val):
        if ( self.gen_log_enable ):
            self.log_file += 'axi_set_pwm_inv:\n'
        idx_mask    = 1 << (pwm_idx+12)
        set_val     = val << (pwm_idx+12)
        self.axi_write_mask(1, set_val, idx_mask)
        return True
    
    ## Set output multiplexor
    ## f2f_mux_selector offset
    def axi_set_out_mux(self, selector):
        if ( self.gen_log_enable ):
            self.log_file += 'axi_set_out_mux:\n'
        mask        = self.R1_F2F_READ_MUX
        mux_sel     = (selector & 0x7) << 9
        self.axi_write_mask(1, mux_sel, mask)
        return True
    
    # Set hexapod offsets
    def axi_set_offset(self, leg, off_q1, off_q2, off_q3):
        self.axi_write(5+leg*3, off_q1)
        self.axi_write(6+leg*3, off_q2)
        self.axi_write(7+leg*3, off_q3)
        return True
    
    # Set read offsets
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
    
    # Set read offsets
    def set_init_position(self, print_out=False):
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

        return True
    
    # Read offsets
    def axi_read_offset(self, leg):
        off1 = self.axi_read(5+leg*3)
        off2 = self.axi_read(6+leg*3)
        off3 = self.axi_read(7+leg*3)
        return [off1, off2, off3]
    
    # Get PWM values
    def axi_get_pwm(self, leg):
        pwm1 = (int(self.axi_read(23+leg*3),16) & 0x3FC00) >> 10
        pwm2 = (int(self.axi_read(24+leg*3),16) & 0x3FC00) >> 10
        pwm3 = (int(self.axi_read(25+leg*3),16) & 0x3FC00) >> 10
        return [pwm1, pwm2, pwm3]
