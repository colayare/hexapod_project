import os
import sys
sys.path.append('./includes')
from hexapod_class import hexapod_kinematics as hc
from numeric_conversions import numeric_conversions as NUM_CONV

#### Params Files
joint_offsets_filename = "./params/joint_offset.params"
gait_steps_filename = './params/gait_steps.params'

#### Numeric Conversions
nc = NUM_CONV()

#### Code
## Initialize Hexapod Class & Memory Map
hexapod = hc()
hexapod.init_axi()

## Import Parameters
hexapod.offsets_file = joint_offsets_filename
hexapod.gait_steps_file = gait_steps_filename
hexapod.import_offsets()
hexapod.set_default_offsets()
print('Joint offsets:')
print(hexapod.j_offs)

while(1):
    print('---- Set Offsets ----')
    opt_leg = raw_input('Select Leg [0-5]: ')
    if ( not opt_leg.isdigit() ):
        print('Input is NaN.')
        exit()
    else:
        if ( int(opt_leg) > 5 ):
            print('No leg selected.')
    
    print('Setting leg '+opt_leg)
    hexapod.config_leg_ctr("one_leg", int(opt_leg))
    sp_q1 = raw_input('Set Q1 = ')
    sp_q2 = raw_input('Set Q2 = ')
    sp_q3 = raw_input('Set Q3 = ')
    sp_q1_hex = nc.dfloat2hfloat(nc.sec2rad(float(sp_q1)))
    sp_q2_hex = nc.dfloat2hfloat(nc.sec2rad(float(sp_q2)))
    sp_q3_hex = nc.dfloat2hfloat(nc.sec2rad(float(sp_q3)))
    print(sp_q1_hex, sp_q2_hex, sp_q3_hex)
    hexapod.axi_write_params_in(sp_q1_hex, sp_q2_hex, sp_q3_hex)
    hexapod.axi_write_out_direct()
    while(1):
        os.system('clear')
        print('------ Configuring Leg '+opt_leg+' ------')
        print('Set points : Q1 = '+sp_q1+'\tQ2 = '+sp_q2+'\tQ3 = '+sp_q3)
        print(hexapod.j_offs[int(opt_leg)])
        print('-- iKinematics IP values : ')
        hexapod.axi_set_out_mux(int(opt_leg)+1)
        [offset_q1, offset_q2, offset_q3] = hexapod.axi_read_offset(int(opt_leg))
        print('Offset Q1 = '+str(nc.hfloat2dfloat(offset_q1))+'\t| '+str(nc.rad2sec(nc.hfloat2dfloat(offset_q1))))
        print('Offset Q2 = '+str(nc.hfloat2dfloat(offset_q2))+'\t| '+str(nc.rad2sec(nc.hfloat2dfloat(offset_q2))))
        print('Offset Q3 = '+str(nc.hfloat2dfloat(offset_q3))+'\t| '+str(nc.rad2sec(nc.hfloat2dfloat(offset_q3))))
        [q1, q2, q3] = hexapod.axi_read_params()
        print('Q1 = '+str(nc.hfloat2dfloat(q1))+'\t| '+str(nc.rad2sec(nc.hfloat2dfloat(q1)))+'\t| '+q1)
        print('Q2 = '+str(nc.hfloat2dfloat(q2))+'\t| '+str(nc.rad2sec(nc.hfloat2dfloat(q2)))+'\t| '+q2)
        print('Q3 = '+str(nc.hfloat2dfloat(q3))+'\t| '+str(nc.rad2sec(nc.hfloat2dfloat(q3)))+'\t| '+q3)
        
        usr_opt = raw_input('Enter option : ')
        if ( usr_opt == '\n' or usr_opt == '' ):
            break
        #elif
    
    hexapod.axi_write_out_direct()
    usr_opt = raw_input('Enter option : ')
    if ( usr_opt == '\n' or usr_opt == '' ):
        break