#ifndef __I2C_IP_CONTEXT__
#define __I2C_IP_CONTEXT__

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <stdexcept>

#include "globals/axi_ip_globals.h"
#include "globals/global_defines.h"
#include "params/ip_reg_map.h"
#include "params/i2c_ip_defines.h"

class i2c_ip_context : public ip_context {
    
    //==== Members =============================================================
    public :
        uint32_t &i2c_address ( uint32_t );
        
    private :
        uint32_t _i2c_address;

    //==== Method Prototypes ===================================================
    public :
        //==  Constructor
        i2c_ip_context(char dev_name[]):ip_context(dev_name) {}
        //== 
        void set_address(uint32_t address);
        //== I2C Write
        void i2c_write(char buffer[]);
        //== I2C Write Byte
        void i2c_write_byte(uint32_t byte);
        //== I2C Read
        char i2c_read(size_t read_size);
        
        // Register Reset
        uint8_t reg_reset_check(); 

};

#endif //__I2C_IP_CONTEXT__