#ifndef __HEXAPOD_LOCOMOTION_H__
#define __HEXAPOD_LOCOMOTION_H__

#include "hexapod_kinematics.h"

class hexapod_locomotion : public ikinematics_ip_context {
    //==== Members ==========================================================
    //== Walk Properties
    public :
        float S;
        float res;
        float xo;
        float yo;
        float zo;
    //== Kinematics 
    
    //==== Method Prototypes ===================================================
    public :
        //hexapod_locomotion(char dev_name[]):ikinematics_ip_context(dev_name) {}
        hexapod_locomotion(String ip_name):ikinematics_ip_context(ip_name) {}
        hexapod_locomotion(String ip_name, char dev_name[], uint32_t axi_base_address, uint32_t axi_mmap_size):ikinematics_ip_context(ip_name, dev_name, axi_base_address, axi_mmap_size) {}
        uint32_t    iteration_size();
        uint32_t    get_step(float step_idx, uint32_t walk, float alpha);
        uint32_t    step(float step_idx, uint32_t walk, float alpha);
};

#endif //__HEXAPOD_LOCOMOTION_H__
