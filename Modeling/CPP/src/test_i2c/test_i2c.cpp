// #include <stdio.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/mman.h>
// #include <stdint.h>
// #include <string.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

#include "globals/axi_ip_globals.h"
#include "globals/linux_timing.h"
#include "ip_context/i2c_ip_context.h"
#include "params/ip_reg_map.h"

#define OLED_ADDR 0x3C

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Testing I2C IP" << endl;
    
    // Declare I2C IP
    i2c_ip_context i2c_ip("/dev/i2c-0");
    i2c_ip.ip_name = "i2c-0";
    volatile uint32_t *i2c_ip_ptr;
    int32_t i2c_file;
    
    // Initialize I2C IP
    // i2c_ip.init_axi_mmap_ptr(AXI_I2C0_RMAPSIZE, AXI_I2C0_BASEADDR, AXI_I2C0_WORDSIZE);
    i2c_ip.init_axi_mmap_ptr(AXI_I2C0_RMAPSIZE, AXI_I2C0_BASEADDR, 4);
    
    // IP Context MMAP Pointer
    i2c_ip_ptr = i2c_ip.get_mmap_ptr();
    cout << "I2C MMAP ptr : " << i2c_ip_ptr << endl;
    
    // Get I2C file
    //i2c_file = i2c_ip.ip_file;
    
    i2c_ip.set_address(OLED_ADDR);
    
    char buffer[2];
    buffer[0] = 0x00;
    buffer[1] = 0xAF;
    i2c_ip.i2c_write(buffer);
    
    buffer[0] = 0x00;
    buffer[1] = 0xA7;
    i2c_ip.i2c_write(buffer);
    
    i2c_ip.i2c_read(4);
    
    // Check Registers Reset Value
    // uint8_t result;
    // result = i2c_ip.reg_reset_check();
    
    // uint32_t imr_reg;
    // imr_reg = i2c_ip.axi_read(8);
    
    // i2c_ip.axi_show_regs(0, AXI_I2C0_RMAPSIZE);
    
    
    // uint32_t value;
    // i2c_ip.axi_write(XIICPS_TIME_OUT, 0xA5);
    // value = i2c_ip.axi_read(XIICPS_TIME_OUT);
    
    // for (int i=0; i<18; i++) {
        // i2c_ip.axi_write(i, 0xA5);
        // i2c_ip.axi_read(i);
    // }

    // cout << "imr_reg : " << hex << imr_reg << endl;
    // cout << "value : " << hex << value << endl;


}