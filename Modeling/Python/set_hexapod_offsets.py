import os
import sys
from math import floor as floor

#### Append includes path          
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path : '+ abs_path+'/includes')
sys.path.append(abs_path+'/includes')
from hexapod_class import hexapod_kinematics as hc
from numeric_conversions import numeric_conversions as NUM_CONV

#### Get Linux prompt params
cols = int(os.popen('tput cols').read()[:-1])
rows = int(os.popen('tput lines').read()[:-1])
dash = cols * '-' 
div  = cols * '#'

def axi_read_fxp_out_params(hexapod, leg):
    fxp_q1 = hexapod.axi_read(23+leg*3)
    fxp_q2 = hexapod.axi_read(24+leg*3)
    fxp_q3 = hexapod.axi_read(25+leg*3)
    return [fxp_q1, fxp_q2, fxp_q3]

def fstr(float_num):
    if ( type(float_num) is str ):
        float_num = float(float_num)
    return '%.4f' % float_num

def print_title(title, char='#'):
    title = ' '+title+' '
    hlen_title = floor(len(title)/2)
    hlen_cols  = floor(cols/2)
    divider = int(hlen_cols - hlen_title)
    rest = cols - (divider + len(title))
    print(divider*char+title+rest*char)
    return True

#### Params Files
joint_offsets_filename      = abs_path+"/params/joint_offset.params"
gait_steps_filename         = abs_path+'/params/gait_steps.params'
init_position_filename      = abs_path+'/params/init_position.params'
servo_inversion_filename    = abs_path+'/params/init_servo_inv.params'
axi_ip_logfile              = abs_path+'/axi.log'

#### Numeric Conversions
nc = NUM_CONV()

#### Code
## Initialize Hexapod Class & Memory Map
hexapod = hc(enable_ip_logs=True)

## Import Parameters
hexapod.offsets_file_path            = joint_offsets_filename
hexapod.gait_steps_file_path         = gait_steps_filename
hexapod.init_position_file_path      = init_position_filename
hexapod.init_servo_inv_file_path     = servo_inversion_filename
hexapod.axi_ip_log_file_path         = axi_ip_logfile
## Initialize Parameters
hexapod.import_init_pos()
hexapod.import_offsets()
hexapod.import_init_servo_invertion()
hexapod.set_default_offsets()
hexapod.set_init_position()

###############################################################################
#### Start Program
###############################################################################
print('Joint offsets:')
print(hexapod.j_offs)
print('Initital Position')
print(hexapod.i_pos)

while(1):
    os.system('clear')
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
    sp_q1 = str(hexapod.i_pos[int(opt_leg)][0])
    sp_q2 = str(hexapod.i_pos[int(opt_leg)][1])
    sp_q3 = str(hexapod.i_pos[int(opt_leg)][2])
    off_q1 = hexapod.j_offs[int(opt_leg)][0]
    off_q2 = hexapod.j_offs[int(opt_leg)][1]
    off_q3 = hexapod.j_offs[int(opt_leg)][2]
    
    print('Q1 = '+sp_q1)
    print('Q2 = '+sp_q2)
    print('Q3 = '+sp_q3)
    print('Offset Q1 = '+str(off_q1))
    print('Offset Q2 = '+str(off_q2))
    print('Offset Q3 = '+str(off_q3))
    
    raw_input('Press enter...')

    while(1):
        sp_q1_p_offset_hex = nc.dfloat2hfloat(nc.sec2rad(float(sp_q1)+off_q1))
        sp_q2_p_offset_hex = nc.dfloat2hfloat(nc.sec2rad(float(sp_q2)+off_q2))
        sp_q3_p_offset_hex = nc.dfloat2hfloat(nc.sec2rad(float(sp_q3)+off_q3))
        
        hexapod.axi_set_out_mux(int(opt_leg)+1)
        hexapod.axi_write_params_in(sp_q1_p_offset_hex, sp_q2_p_offset_hex, sp_q3_p_offset_hex)
        hexapod.axi_write_out_direct()
        
        [offset_q1, offset_q2, offset_q3] = hexapod.axi_read_offset(int(opt_leg))
        [q1, q2, q3] = hexapod.axi_read_params()
        [pwm1, pwm2, pwm3] = hexapod.axi_get_pwm(int(opt_leg))
        [fxp_q1, fxp_q2, fxp_q3] = axi_read_fxp_out_params(hexapod, int(opt_leg))
        
        
        os.system('clear')
        print_title('Configuring Leg '+opt_leg)
        print('{:<12s}{:>10s}{:>12s}{:>12s}{:>10s}'.format('Angle','Rad','Sexa','Float hex','Fixed hex'))
        print('{:<12s}{:>10.4f}{:>12s}{:>12s}{:>10s}'.format('Q1',nc.sec2rad(float(sp_q1)),sp_q1,nc.dfloat2hfloat(nc.sec2rad(float(sp_q1))),nc.dfloat2hfix(nc.sec2rad(float(sp_q1)))))
        print('{:<12s}{:>10.4f}{:>12s}{:>12s}{:>10s}'.format('Q2',nc.sec2rad(float(sp_q2)),sp_q2,nc.dfloat2hfloat(nc.sec2rad(float(sp_q2))),nc.dfloat2hfix(nc.sec2rad(float(sp_q2)))))
        print('{:<12s}{:>10.4f}{:>12s}{:>12s}{:>10s}'.format('Q3',nc.sec2rad(float(sp_q3)),sp_q3,nc.dfloat2hfloat(nc.sec2rad(float(sp_q3))),nc.dfloat2hfix(nc.sec2rad(float(sp_q3)))))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('Q1 offset',nc.sec2rad(off_q1),off_q1,nc.dfloat2hfloat(nc.sec2rad(off_q1)),nc.dfloat2hfix(nc.sec2rad(off_q1))))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('Q2 offset',nc.sec2rad(off_q2),off_q2,nc.dfloat2hfloat(nc.sec2rad(off_q2)),nc.dfloat2hfix(nc.sec2rad(off_q2))))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('Q3 offset',nc.sec2rad(off_q3),off_q3,nc.dfloat2hfloat(nc.sec2rad(off_q3)),nc.dfloat2hfix(nc.sec2rad(off_q3))))
        print(dash)
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('Q1+offset',nc.sec2rad(float(sp_q1)+off_q1),float(sp_q1)+off_q1,sp_q1_p_offset_hex,nc.dfloat2hfix(nc.sec2rad(float(sp_q1)+off_q1))))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('Q2+offset',nc.sec2rad(float(sp_q2)+off_q2),float(sp_q2)+off_q2,sp_q2_p_offset_hex,nc.dfloat2hfix(nc.sec2rad(float(sp_q2)+off_q2))))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('Q1+offset',nc.sec2rad(float(sp_q3)+off_q3),float(sp_q3)+off_q3,sp_q3_p_offset_hex,nc.dfloat2hfix(nc.sec2rad(float(sp_q3)+off_q3))))
        print(dash)
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('IP Q1+Offset',nc.hfloat2dfloat(q1),nc.rad2sec(nc.hfloat2dfloat(q1)),q1[2:],fxp_q1[2:]))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('IP Q2+Offset',nc.hfloat2dfloat(q2),nc.rad2sec(nc.hfloat2dfloat(q2)),q2[2:],fxp_q2[2:]))
        print('{:<12s}{:>10.4f}{:>12.4f}{:>12s}{:>10s}'.format('IP Q3+Offset',nc.hfloat2dfloat(q3),nc.rad2sec(nc.hfloat2dfloat(q3)),q3[2:],fxp_q3[2:]))
        print(dash)
        print('{:<12s}{:>10s}{:>12s}'.format('PWM 1',str(pwm1),hex(pwm1)[2:].zfill(2)))
        print('{:<12s}{:>10s}{:>12s}'.format('PWM 2',str(pwm2),hex(pwm2)[2:].zfill(2)))
        print('{:<12s}{:>10s}{:>12s}'.format('PWM 3',str(pwm3),hex(pwm3)[2:].zfill(2)))        
        print(dash)
        print('{:<12s}{:>10s}'.format('INV 1',str(hexapod.i_inv_s[int(opt_leg) * 3])))
        print('{:<12s}{:>10s}'.format('INV 2',str(hexapod.i_inv_s[int(opt_leg) * 3 + 1])))
        print('{:<12s}{:>10s}'.format('INV 3',str(hexapod.i_inv_s[int(opt_leg) * 3 + 2])))
        
        usr_opt = raw_input('Enter option : ')
        if ( usr_opt == '\n' or usr_opt == '' ):
            break
        elif ( usr_opt.split(',')[0].isdigit() and ',' in usr_opt ):
            off_idx = int(usr_opt.split(',')[0].replace(' ',''))
            if ( off_idx < 3 ):
                off_val = nc.dfloat2hfloat(nc.sec2rad(float(usr_opt.split(',')[1].replace(' ',''))))
                print('Setting Offset['+str(off_idx)+'] = '+off_val)
                hexapod.axi_write(5+int(opt_leg)*3, off_val)
                hexapod.j_offs[int(opt_leg)][off_idx] = float(usr_opt.split(',')[1].replace(' ',''))
                [off_q1, off_q2, off_q3] = hexapod.j_offs[int(opt_leg)]
            else:
                print('Invalid joint index.')
        elif ( 'a' in usr_opt.split(',')[0] and ',' in usr_opt ):
            new_angle = usr_opt.split(',')[1]
            if( usr_opt.split(',')[0][1:] == '0' ):
                hexapod.i_pos[int(opt_leg)][int(usr_opt.split(',')[0][1:])] = float(new_angle)
                sp_q1 = new_angle
            if( usr_opt.split(',')[0][1:] == '1' ):
                hexapod.i_pos[int(opt_leg)][int(usr_opt.split(',')[0][1:])] = float(new_angle)
                sp_q2 = new_angle
            if( usr_opt.split(',')[0][1:] == '2' ):
                hexapod.i_pos[int(opt_leg)][int(usr_opt.split(',')[0][1:])] = float(new_angle)
                sp_q3 = new_angle
        elif ( 'i' in usr_opt.split(',')[0] and ',' in usr_opt ):
            inv_val = int(usr_opt.split(',')[1])
            ser_idx = int(usr_opt.split(',')[0][1:]) + int(opt_leg) * 3
            hexapod.i_inv_s[ser_idx] = str(inv_val)
            hexapod.axi_set_pwm_inv(ser_idx, inv_val)
        elif ( usr_opt.upper() == 'SHOW' ):
            hexapod.show_regs()
        elif ( usr_opt.upper() == 'SAVE OFFSET' ):
            hexapod.save_offsets()
        elif ( usr_opt.upper() == 'SAVE INIT' ):
            hexapod.save_init_positions()
        elif ( usr_opt.upper() == 'SAVE INVERTION' ):
            hexapod.save_inversion()
        elif( usr_opt.upper() == 'SAVE' ):
            hexapod.save_offsets()
            hexapod.save_init_positions()
            hexapod.save_inversion()
        elif( usr_opt.upper() == 'EXIT' ):
            hexapod.export_log()
            exit()
        elif( usr_opt.upper() == 'HELP' ):
            print('Options:\nSHOW : Print all register values\nSAVE OFFSET\nSAVE INIT: Save initial servo positions\nSAVE INVERSION: Save servo inversions\nSAVE: Save all\nEXIT')
            raw_input()
        else:
            print('Unrecognized option. Press enter.')