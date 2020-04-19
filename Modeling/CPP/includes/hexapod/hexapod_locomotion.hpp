#ifndef __HEXAPOD_LOCOMOTION_H__
#define __HEXAPOD_LOCOMOTION_H__

#include "hexapod_kinematics.hpp"
#include "../params/ikinematics_params.hpp"

class hexapod_locomotion : public hexapod_class {
    //==== Members ==========================================================
    //== Walk Properties
    float S;
    float res;
    float xo;
    float yo;
    float zo;
    //== Kinematics 
    
    //==== Method Prototypes ===================================================
    void    step(float point, float walk, float alpha);
};



#endif //__HEXAPOD_LOCOMOTION_H__