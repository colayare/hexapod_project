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

def get_ik_out(hexapod):
    hexapod.axi_set_out_mux(0)
    [q1, q2, q3] = hexapod.axi_read_params()
    q1 = hexapod.nc.hfloat2dfloat(q1)
    q2 = hexapod.nc.hfloat2dfloat(q2)
    q3 = hexapod.nc.hfloat2dfloat(q3)
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

#### Numeric Conversions
nc = NUM_CONV()

#### Code
## Initialize Hexapod Class & Memory Map
hexapod = hc()
hexapod.init_axi()

## Import Parameters
hexapod.offsets_file = joint_offsets_filename
hexapod.gait_steps_file = gait_steps_filename
hexapod.init_position_file = init_position_filename
hexapod.import_init_pos()
hexapod.import_offsets()
hexapod.set_default_offsets()
hexapod.set_init_position()

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
    print('(D) Set Delay (ms).')
    print('> '+str(delay))
    print('(R) Set number of gait loops (0:infinite).')
    print('> '+str(gait_loops))
    print('(O) Print outputs. (True/False)')
    print('> '+str(display))
    print('(G) Select Gait. (0-3)')
    print('> '+str(gait_sel))
    print('(S) Start gait')
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
        delay = input('Enter delay (ms) : ')
    elif( usr_opt.upper() == 'R' ):
        gait_loops = int(input('Enter number of loops: '))
    elif( usr_opt.upper() == 'O' ):
        display = raw_input('Enable display : ').upper() == 'TRUE'
    elif( usr_opt.upper() == 'G' ):
        gait_sel = input('Enter gait : ')
        hexapod.read_gait_steps(gait_sel)
    elif( usr_opt.upper() == 'S' ):
        if ( display ):
            print("{:>10s}{:>10s}{:>10s}{:>10s}{:>10s}".format('Step', 'leg', 'Q1', 'Q2', 'Q3'))
        if ( gait_loops == 0 ):
            while(1):
                for s in range (30):
                    for j in range (6):
                        if ( leg_enable[j] ):
                            gait(hexapod, j, s+30*gait_sel)
                            if (display):
                                q1, q2, q3 = get_ik_out(hexapod)
                                print("{:>10.0f}{:>10.0f}{:>10.4f}{:>10.4f}{:>10.4f}".format(s, j, q1, q2, q3))
                    tm.sleep(delay)
        elif ( gait_loops > 0):
            for i in range (gait_loops):
                for s in range (30):
                    for j in range (6):
                        if ( leg_enable[j] ):
                            gait(hexapod, j, s+30*gait_sel)
                            if (display):
                                q1, q2, q3 = get_ik_out(hexapod)
                                print("{:>10.0f}{:>10.0f}{:>10.4f}{:>10.4f}{:>10.4f}".format(s, j, q1, q2, q3))
                    tm.sleep(delay)
    elif( usr_opt.upper() == 'EXIT' ):
        exit()