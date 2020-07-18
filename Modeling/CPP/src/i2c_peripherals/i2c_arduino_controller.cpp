#include "i2c_peripherals/i2c_arduino_controller.h"
#include "i2c_peripherals/i2c_arduino_controller_params.h"

void i2c_arduino_controller::get_joystick() {
    uint32_t tmpCtrlX;
    uint32_t tmpCtrlY;
    
    tmpCtrlX = this->i2c_read_word_data(ARDUINO_JOYSTICK_X);
    tmpCtrlY = this->i2c_read_word_data(ARDUINO_JOYSTICK_Y);

    this->_control_x = tmpCtrlX;
    this->_control_y = tmpCtrlY;

    if ( tmpCtrlX < MIN_THRESHOLD ) this->_dJoystick_x = joystickMin;
    else if ( tmpCtrlX > MAX_THRESHOLD ) this->_dJoystick_x = joystickMax;
    else this->_dJoystick_x = joystickMid;

    if ( tmpCtrlY < MIN_THRESHOLD ) this->_dJoystick_y = joystickMin;
    else if ( tmpCtrlY > MAX_THRESHOLD ) this->_dJoystick_y = joystickMax;
    else this->_dJoystick_y = joystickMid;

    switch ( this->_dJoystick_x ) {
        case joystickMin:
            this->_xAngle = -90.0f;
            if ( this->_dJoystick_y == joystickMin ) this->_dJoystick = NW;
            else if ( this->_dJoystick_y == joystickMid ) this->_dJoystick = W;
            else this->_dJoystick = SW;
            break;
        case joystickMid:
            this->_xAngle = 0.0f;
            if ( this->_dJoystick_y == joystickMin ) this->_dJoystick = N;
            else if ( this->_dJoystick_y == joystickMid ) this->_dJoystick = C;
            else this->_dJoystick = S;
            break;
        case joystickMax:
            this->_xAngle = 90.0f;
            if ( this->_dJoystick_y == joystickMin ) this->_dJoystick = NE;
            else if ( this->_dJoystick_y == joystickMid ) this->_dJoystick = E;
            else this->_dJoystick = SE;
            break;
        default:
            std::cout << "ERROR: Joystick invalid value" << std::endl;
            exit(1);
    }
}
