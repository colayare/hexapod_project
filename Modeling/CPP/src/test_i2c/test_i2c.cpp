#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include "globals/axi_ip_globals.h"
#include "globals/linux_timing.h"
#include "ip_context/i2c_ip_context.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Testing I2C IP" << endl;
    
    // Declare I2C IP
    i2c_ip_context i2c_ip;
    volatile uint32_t *i2c_ip_ptr;
    
    // Initialize I2C IP
    i2c_ip.init_axi_mmap_ptr(AXI_I2C0_RMAPSIZE, AXI_I2C0_BASEADDR, AXI_I2C0_WORDSIZE);
    
    // IP Context MMAP Pointer
    i2c_ip_ptr = i2c_ip.get_mmap_ptr();
    cout << "I2C MMAP ptr : " << i2c_ip_ptr << endl;
    
    // Check Registers Reset Value
    // uint8_t result;
    // result = i2c_ip.reg_reset_check();
    
    uint32_t imr_reg;
    imr_reg = i2c_ip.axi_read(8);
    
    i2c_ip.axi_show_regs(0, AXI_I2C0_RMAPSIZE);
    
    
    uint32_t value;
    i2c_ip.axi_write(XIICPS_TIME_OUT, 0xA5);
    value = i2c_ip.axi_read(XIICPS_TIME_OUT);
    
    for (int i=0; i<18; i++) {
        i2c_ip.axi_write(i, 0xA5);
        i2c_ip.axi_read(i);
    }

    cout << "imr_reg : " << hex << imr_reg << endl;
    cout << "value : " << hex << value << endl;


}