#include "i2c_peripherals/i2c_pcf8591.h"

int32_t i2c_pcf8591::read_ad(uint8_t mode, uint8_t channel) {
    // char buffer[2];
    // buffer[0] = WD_DADR;
    // buffer[1] = mode|channel;
    // this->i2c_write((char *) &buffer);
    // this->i2c_write_byte(RD_DADR);
    // this->i2c_read_byte(adc);
    this->i2c_write_byte_data(WD_DADR, mode|channel);
    return this->i2c_read_byte_data(RD_DADR);
}