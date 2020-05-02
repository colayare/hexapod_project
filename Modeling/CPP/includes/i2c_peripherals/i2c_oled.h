#ifndef __I2C_OLED_H__
#define __I2C_OLED_H__

#include "ip_context/i2c_ip_context.h"

class i2c_oled : public i2c_ip_context {
    //==== Members =============================================================
    
    //==== Method Prototypes ===================================================
    public :
        //== Constructor
        i2c_oled(char dev_name[]):i2c_ip_context(dev_name) {}
        //== Send OLED Command
        void send_command(char command);
        //== Initialize OLED
        void init();
        //== Clear OLED
        void clear();
        
};

#endif //__I2C_OLED_H__