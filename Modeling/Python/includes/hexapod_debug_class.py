# -*- coding: utf-8 -*-
import os, sys
import numpy as np
###############################################################################
#### Append Paths 
###############################################################################
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/../includes')
sys.path.append(abs_path+'/../includes')
from hexapod_locomotion_class import hexapod_locomotion as hexapod_locomotion
from hl_hexapod_class import hexapod_leg as hexapod_leg

###############################################################################
#### Debug Hexapod Kinematics Class
###############################################################################
class hexapod_debug(hexapod_locomotion):
    ###########################################################################
    #### Properties
    ###########################################################################
    collected_coord   = np.array([
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg()])
    
    collected_joints  = np.array([  
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg(), 
                        hexapod_leg()])

    ###########################################################################
    #### Methods
    ###########################################################################
    #### Constructor
    def __init__(self, invoke_axi_ip=True, gen_log_enable=False):
        super(hexapod_debug, self).__init__(invoke_axi_ip=invoke_axi_ip, enable_ip_logs=gen_log_enable)
        return None
    
    #### Data Manipulation ####################################################
    ## Convert Bytes to decimal Float
    def bytes2dfloat(self, bytes_in):
        return self.hfloat2dfloat(hex(self.to_int(bytes_in)).lstrip('0x').rstrip('L').zfill(8))
    
    ## Convert Bytes to Hexadecimal Float
    def bytes2hfloat(self, bytes_in):
        return hex(self.to_int(bytes_in)).lstrip('0x').rstrip('L').zfill(8)
    
    ##### Debuging Methods ####################################################
    ## Set hexapod step and store results
    def set_step_debug(self):
        self.config_leg_ctr(1, 0)
        self.axi_set_out_mux(0)
        for i, leg in enumerate(self.joints):
            self.set_ptr(2)
            for axis in leg:
                self.axi_map.write(self.to_bytes(int(self.dfloat2hfloat(axis), 16)))
            self.axi_write_fifo()
            self.axi_trigger_ikinematics()
            [q1, q2, q3] = self.axi_hread_params()
            q1 = self.hfloat2dfloat(q1)
            q2 = self.hfloat2dfloat(q2)
            q3 = self.hfloat2dfloat(q3)
            self.collected_joints[i].append([q1, q2, q3])
            self.collected_coord[i].append(leg)
        return True
    
    ##### Reporting Methods ###################################################
    def export_gait_log(self, log_file_path):
        cont = ''
        for i, leg in enumerate(self.collected_coord):
            for step in leg.coordinates:
                cont += str(i)+','+str(step[0])+','+str(step[1])+','+str(step[2])+'\n'
        cont.rstrip('\n')
        file = open(log_file_path, 'w+')
        file.write(cont)
        file.close()
        return cont.rstrip('\n')
    
    def export_joints_log(self, log_file_path):
        cont = ''
        for i, leg in enumerate(self.collected_joints):
            for step in leg.coordinates:
                cont += str(i)+','+str(step[0])+','+str(step[1])+','+str(step[2])+'\n'
        cont.rstrip('\n')
        file = open(log_file_path, 'w+')
        file.write(cont)
        file.close()
        return cont.rstrip('\n')
