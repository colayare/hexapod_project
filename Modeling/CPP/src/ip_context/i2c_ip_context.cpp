#include "ip_context/i2c_ip_context.h"

void i2c_ip_context::set_address(uint32_t address) {
    if ( ioctl(this->ip_file(), I2C_SLAVE, address) < 0) {
        cout << "ERROR HANDLING; you can check errno to see what went wrong " << endl;
        exit(1);
    }
    this->_i2c_address = address;
}

void i2c_ip_context::i2c_write(char buffer[]) {
    size_t  size = sizeof(buffer);
    if (write(this->ip_file(), buffer, size) != size) {
        cout << "ERROR HANDLING: i2c transaction failed " << endl;
        exit(1);
    }
}

void i2c_ip_context::i2c_write_byte(uint32_t byte) {
    char buffer[2];
    buffer[0] = this->_i2c_address;
    buffer[1] = (char) byte;
    this->i2c_write(buffer);
}

char i2c_ip_context::i2c_read(size_t read_size) {
    char buffer[read_size];
    if (read(this->ip_file(), buffer, read_size) != read_size) {
        /* ERROR HANDLING: i2c transaction failed */
        cout << "ERROR HANDLING: i2c transaction failed " << endl;
    } else {
        /* buf[0] contains the read byte */
        for (uint32_t i=0; i<read_size; i++) {
            cout << (uint32_t) buffer[i] << hex << endl;
        }
        return buffer[0];
    }
}

uint8_t i2c_ip_context::reg_reset_check() {
    uint8_t result = 1;
    
    #ifdef __IP_LOG
    cout << "i2c_ip_context::reg_reset_check" << endl;
    #endif
    result &= this->axi_read_exp8(XIICPS_CR_L, XIICPS_CR_L_RST);
    result &= this->axi_read_exp8(XIICPS_CR_H, XIICPS_CR_H_RST);
    result &= this->axi_read_exp8(XIICPS_SR_L, XIICPS_SR_L_RST);
    result &= this->axi_read_exp8(XIICPS_SR_H, XIICPS_SR_H_RST);
    result &= this->axi_read_exp8(XIICPS_ADDR_L, XIICPS_ADDR_L_RST);
    result &= this->axi_read_exp8(XIICPS_ADDR_H, XIICPS_ADDR_H_RST);
    result &= this->axi_read_exp8(XIICPS_DATA_L, XIICPS_DATA_L_RST);
    result &= this->axi_read_exp8(XIICPS_DATA_H, XIICPS_DATA_H_RST);
    result &= this->axi_read_exp8(XIICPS_ISR_L, XIICPS_ISR_L_RST);
    result &= this->axi_read_exp8(XIICPS_ISR_H, XIICPS_ISR_H_RST);
    result &= this->axi_read_exp8(XIICPS_TRANS_SIZE, XIICPS_TRANS_SIZE_RST);
    result &= this->axi_read_exp8(XIICPS_SLV_PAUSE, XIICPS_SLV_PAUSE_RST);
    result &= this->axi_read_exp8(XIICPS_TIME_OUT, XIICPS_TIME_OUT_RST);
    result &= this->axi_read_exp8(XIICPS_IMR_L, XIICPS_IMR_L_RST);
    result &= this->axi_read_exp8(XIICPS_IMR_H, XIICPS_IMR_H_RST);
    result &= this->axi_read_exp8(XIICPS_IER_L, XIICPS_IER_L_RST);
    result &= this->axi_read_exp8(XIICPS_IER_H, XIICPS_IER_H_RST);
    result &= this->axi_read_exp8(XIICPS_IDR_L, XIICPS_IDR_L_RST);
    result &= this->axi_read_exp8(XIICPS_IDR_H, XIICPS_IDR_H_RST);
    
    return result;
}