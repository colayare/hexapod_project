#ifndef __PCF8591_H__
#define __PCF8591_H__

#include "ip_context/i2c_ip_context.h"
#include "i2c_peripherals/i2c_pcf8591_params.h"

using namespace std;

class i2c_pcf8591 : public i2c_ip_context {
    
    //==== Method Prototypes ===================================================
    public :
        //== Constructor
        i2c_pcf8591(char dev_name[]):i2c_ip_context(dev_name) {}
        //== Set Mode
        int32_t read_ad(uint8_t mode, uint8_t chanel);
    
};

#endif //__PCF8591_H__