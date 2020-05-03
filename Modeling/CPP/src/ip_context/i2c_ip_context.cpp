#include <linux/i2c-dev-user.h>
#include "ip_context/i2c_ip_context.h"

i2c_ip_context::i2c_ip_context(string dev_name) {
    int32_t file;
    char char_devname[dev_name.length()];
    strcpy(char_devname, dev_name.c_str());
    if ((file = open(char_devname, O_RDWR)) < 0) {
		cout << "Failed to open the i2c bus" << endl;
        exit(1);
    }
    this->_i2c_file = file;
    this->_dev_name = dev_name;
}

void i2c_ip_context::set_address(uint8_t address) {
    if ( ioctl(this->_i2c_file, I2C_SLAVE, address) < 0) {
        cout << "ERROR HANDLING; you can check errno to see what went wrong " << endl;
        exit(1);
    }
    this->_i2c_address = address;
}

int32_t i2c_ip_context::i2c_read_byte() {
    return i2c_smbus_read_byte(this->_i2c_file);
}

int32_t i2c_ip_context::i2c_write_byte(uint8_t value) {
    return i2c_smbus_write_byte(this->_i2c_file, value);
}

int32_t i2c_ip_context::i2c_read_byte_data(uint8_t command) {
    return i2c_smbus_read_byte_data(this->_i2c_file, command);
}

int32_t i2c_ip_context::i2c_write_byte_data(uint8_t command, uint8_t value) {
    return i2c_smbus_write_byte_data(this->_i2c_file, command, value);
}

int32_t i2c_ip_context::i2c_read_word_data(uint8_t command) {
    return i2c_smbus_read_word_data(this->_i2c_file, command);
}

int32_t i2c_ip_context::i2c_write_word_data(uint8_t command, uint16_t value) {
    return i2c_smbus_write_word_data(this->_i2c_file, command, value);
}