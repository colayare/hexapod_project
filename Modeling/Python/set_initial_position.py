import os
import sys
#### Append includes path           
pathname = os.path.dirname(sys.argv[0])   
abs_path = os.path.abspath(pathname)
sys.path.append(abs_path+'/includes')
from hexapod_class import hexapod_kinematics as hc

#### Params Files
joint_offsets_filename  = abs_path+"/params/joint_offset.params"
gait_steps_filename     = abs_path+'/params/gait_steps.params'
init_position_filename  = abs_path+'/params/init_position.params'

#### Code
## Initialize Hexapod Class & Memory Map
hexapod = hc()

## Import Parameters
hexapod.offsets_file = joint_offsets_filename
hexapod.gait_steps_file = gait_steps_filename
hexapod.init_position_file = init_position_filename
hexapod.import_init_pos()
hexapod.import_offsets()
hexapod.set_default_offsets()
hexapod.set_init_position()