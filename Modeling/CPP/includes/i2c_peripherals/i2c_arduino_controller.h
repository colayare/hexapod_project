#ifndef __I2C_ARDUINO_CONTROLLER_H__
#define __I2C_ARDUINO_CONTROLLER_H__

#include "ip_context/i2c_ip_context.h"

enum dSample_t { joystickMin, joystickMid, joystickMax };
enum dJoystick_t { C, N, NE, E, SE, S, SW, W, NW};

class i2c_arduino_controller : public i2c_ip_context {
    //==== Members =============================================================
    private :
        uint8_t     _control_x;
        uint8_t     _control_y;
        dSample_t   _dJoystick_x;
        dSample_t   _dJoystick_y;
        dJoystick_t _dJoystick;
        float       _xAngle;

    public :
        uint8_t     joystick_x() { return this->_control_x; }
        uint8_t     joystick_y() { return this->_control_y; }
        dSample_t   dJoystick_x() { return this->_dJoystick_x; }
        dSample_t   dJoystick_y() { return this->_dJoystick_y; }
        dJoystick_t dJoystick() { return this->_dJoystick; }
        float       xAngle() { return this->_xAngle; }

        //== Constructor
        i2c_arduino_controller(char dev_name[]):i2c_ip_context(dev_name) {}
        //== Update Joystick Position
        void  get_joystick();
};

#endif //__I2C_ARDUINO_CONTROLLER_H__
