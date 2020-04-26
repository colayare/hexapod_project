#ifndef __I2C_IP_CONTEXT__
#define __I2C_IP_CONTEXT__

#include "globals/axi_ip_globals.h"
#include "params/ip_reg_map.h"
#include "params/i2c_ip_defines.h"

class i2c_ip_context : public ip_context {

    //==== Method Prototypes ===================================================
    public :
        // Register Reset
        uint8_t reg_reset_check(); 

};

#endif //__I2C_IP_CONTEXT__