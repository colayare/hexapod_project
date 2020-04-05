import os
import sys
import time as tm
from math import floor as floor
import numpy as np

#### Append includes path           
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
print('Appending path'+ abs_path+'/includes')
sys.path.append(abs_path+'/includes')
from hexapod_class import hexapod_kinematics as hc
from numeric_conversions import numeric_conversions as NUM_CONV

#### Get Linux prompt params
cols = int(os.popen('tput cols').read()[:-1])
rows = int(os.popen('tput lines').read()[:-1])
dash = cols * '-' 
div  = cols * '#'

#### Functions
def gait(hexapod, leg, step):
    hexapod.config_leg_ctr("one_leg", leg)
    [x, y, z] = hexapod.gait_step(step)
    hexapod.axi_write_params_in(x, y, z)
    hexapod.axi_write_fifo()
    hexapod.axi_trigger_ikinematics()
    return None
    
def get_ik_out(hexapod, sexa=True):
    hexapod.axi_set_out_mux(0)
    [q1, q2, q3] = hexapod.axi_hread_params()
    q1 = hexapod.hfloat2dfloat(q1)
    q2 = hexapod.hfloat2dfloat(q2)
    q3 = hexapod.hfloat2dfloat(q3)
    if ( sexa ):
        q1 = hexapod.rad2sec(q1)
        q2 = hexapod.rad2sec(q2)
        q3 = hexapod.rad2sec(q3)
    return q1, q2, q3

def print_title(title, char='#'):
    title = ' '+title+' '
    hlen_title = floor(len(title)/2)
    hlen_cols  = floor(cols/2)
    divider = int(hlen_cols - hlen_title)
    rest = cols - (divider + len(title))
    print(divider*char+title+rest*char)
    return True

#### Params Files
joint_offsets_filename  = abs_path+"/params/joint_offset.params"
gait_steps_filename     = abs_path+'/params/gait_steps.params'
init_position_filename  = abs_path+'/params/init_position.params'
servo_inversion_filename = abs_path+'/params/init_servo_inv.params'
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
hexapod.ip_logfile_path              = axi_ip_logfile
hexapod.import_init_pos()
hexapod.import_offsets()
hexapod.import_init_servo_invertion()
hexapod.set_default_offsets(print_out=True)
hexapod.set_init_position(print_out=True)

## Execution Variables
display     = True      # True / False
delay       = 0.01      # Delay in ms
leg_enable  = np.array([True, False, False, False, False, False])
gait_loops  = 1
gait_sel    = 0         # Gait selector ( 0 - 3 )

hexapod.read_gait_steps(gait_sel)

while(1):
    os.system('clear')
    print_title('Test iKinematics Gait')
    print('(E) Enable leg.')
    print('> '+str(leg_enable))
    print('(D) Set Delay (s).')
    print('> '+str(hexapod.delay))
    print('(R) Set number of gait loops (0:infinite).')
    print('> '+str(gait_loops))
    print('(O) Print outputs. (True/False)')
    print('> '+str(display))
    print('(G) Select Gait. (0-3)')
    print('> '+str(gait_sel))
    print('(I) Set initial position')
    print('(S) Start gait')
    print('(F) Start gait in Fast Mode')
    print('(A) Show all Registers')
    print('(L) Enable/Disable AXI IP Log')
    print('> '+str(hexapod.gen_log_enable))
    print('(EXIT) Exit')
    print(dash)
    print('Servo Inversion:')
    print(hexapod.i_inv_s)
    print(dash)
    usr_opt = raw_input('Enter option : ')
    
    if( usr_opt.upper() == 'E' ):
        opt = raw_input('Enter leg,value: ')
        if ( opt.upper() == 'A,TRUE' ):
            leg_enable = np.array([True, True, True, True, True, True])
        elif ( opt.upper() == 'A,FALSE' ):
            leg_enable = np.array([False, False, False, False, False, False])
        else:
            leg_enable[int(opt.split(',')[0])] = opt.split(',')[1].upper() == 'TRUE'
    elif( usr_opt.upper() == 'D' ):
        hexapod.delay = input('Enter delay (s) : ')
    elif( usr_opt.upper() == 'R' ):
        gait_loops = int(input('Enter number of loops: '))
    elif( usr_opt.upper() == 'O' ):
        display = raw_input('Enable display : ').upper() == 'TRUE'
    elif ( usr_opt.upper() == 'A' ):
            hexapod.show_regs(end_address=0x28, autoskip=False)
    elif( usr_opt.upper() == 'G' ):
        print('Enter gait, interpolation dots, scale : ')
        usr_in = raw_input('[0-3],[30-300],[1.0-2.0]\n')
        if ( ',' not in usr_in ):
            gait_sel = int(usr_in)
            hexapod.read_gait_steps(int(gait_sel), hexapod.steps, hexapod.scale)
        else:
            if ( usr_in.count(",") == 2 ):
                gait_sel        = int(usr_in.split(",")[0])
                gait_interp     = int(usr_in.split(",")[1])
                gait_scale      = float(usr_in.split(",")[2])
                hexapod.read_gait_steps(gait_sel, gait_interp, gait_scale)
    elif( usr_opt.upper() == 'I' ):
        hexapod.set_init_position()
    elif( usr_opt.upper() == 'L' ):
        hexapod.gen_log_enable = not hexapod.gen_log_enable
    elif( usr_opt.upper() == 'F' ):
        for i in range ( gait_loops ):
            hexapod.run_fast_gait()
    elif( usr_opt.upper() == 'S' ):
        if ( display ):
            print("{:>10s}{:>10s}{:>10s}{:>10s}{:>10s}{:>10s}{:>10s}{:>10s}".format('Step', 'leg', 'Q1', 'PWM1', 'Q2', 'PWM2', 'Q3', 'PWM3'))
        if ( gait_loops > 0):
            for i in range (gait_loops):
                for step in range (hexapod.steps):
                    for j in range (6):
                        if ( leg_enable[j] ):
                            gait(hexapod, j, step+hexapod.steps*hexapod.gait)
                            if (display):
                                q1, q2, q3 = get_ik_out(hexapod)
                                [pwm1, pwm2, pwm3] = hexapod.axi_get_pwm(j)
                                print("{:>10.0f}{:>10.0f}{:>10.4f}{:>10.0f}{:>10.4f}{:>10.0f}{:>10.4f}{:>10.0f}".format(step, j, q1, pwm1, q2, pwm2, q3, pwm3))
                    tm.sleep(hexapod.delay)
    elif( usr_opt.upper() == 'EXIT' ):
        hexapod.export_log()
        exit()