#ifndef __I2C_IP_CONTEXT__
#define __I2C_IP_CONTEXT__

#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c-dev-user.h>
#include "globals/global_defines.h"

using namespace std;

class i2c_ip_context {
    
    //==== Members =============================================================
    public :
        int32_t  i2c_file() { return this->_i2c_file; }
        string   dev_name() { return this->_dev_name; }
        uint32_t i2c_address() { return this->_i2c_address; }
        
    private :
        int32_t  _i2c_file;
        string   _dev_name;
        uint32_t _i2c_address;

    //==== Method Prototypes ===================================================
    public :
        //==  Constructor
        i2c_ip_context(string dev_name);
        //== Set Device Address
        void set_address(uint8_t address);
        //== I2C Access Methods ================================================
        int32_t i2c_read_byte();
        int32_t i2c_write_byte(uint8_t value);
        int32_t i2c_read_byte_data(uint8_t command);
        int32_t i2c_write_byte_data(uint8_t command, uint8_t value);
        int32_t i2c_read_word_data(uint8_t command);
        int32_t i2c_write_word_data(uint8_t command, uint16_t value);
};

#endif //__I2C_IP_CONTEXT__