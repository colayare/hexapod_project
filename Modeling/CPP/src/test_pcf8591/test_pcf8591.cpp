#include <iostream>
#include <linux/i2c-dev-user.h>
#include "globals/linux_timing.h"
#include "i2c_peripherals/i2c_pcf8591.h"
#include "i2c_peripherals/i2c_pcf8591_params.h"
#include "params/ip_reg_map.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Testing I2C ADC PCF8591 " << endl;
    
    // Declare I2C OLED
    i2c_pcf8591 adc("/dev/i2c-0");
    adc.set_address(PCF8591_ADDR);
    
    // char adc_val;
    // char *adc_val_ptr = &adc_val;
    // adc.read_ad(MODE0, CHNL0, adc_val_ptr);
    // cout << "ADC : " << hex << unsigned(adc_val) << endl;
    
    int32_t read_val;
    read_val = adc.read_ad(MODE0, CHNL0);
    
    cout << "ADC : " << hex << unsigned(read_val) << endl;
    
    // adc.i2c_read_byte((char *) &read_val);
    
    // cout << "ADC : " << hex << unsigned(read_val) << endl;

}