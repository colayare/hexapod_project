#ifndef __I2C_OLED_H__
#define __I2C_OLED_H__

#include "ip_context/i2c_ip_context.h"

class i2c_oled : public i2c_ip_context {
    //==== Members =============================================================
    private :
        uint8_t _line;
        uint8_t _cursor;
    
    public :
        uint8_t line() { return this->_line; }
        uint8_t cursor() { return this->_cursor; }
    
    //==== Method Prototypes ===================================================
    public :
        //== Constructor
        i2c_oled(char dev_name[]):i2c_ip_context(dev_name) {}
        //== Send OLED Command
        void send_command(char command);
        //== Send OLED Data
        void send_data(char data);
        //== Initialize OLED
        void init();
        //== Clear OLED
        void clear();
        //== Set Cursor
        void set_cursor(uint8_t line, uint8_t cursor);
        //== Go to the next line
        void goto_next_line();
        //== Display Char
        void disp_char(uint8_t character);
        //== Display String
        void disp_str(uint8_t *ptr);
};

#endif //__I2C_OLED_H__