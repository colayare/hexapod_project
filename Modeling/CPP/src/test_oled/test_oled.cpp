#include "globals/linux_timing.h"
#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"
#include "params/ip_reg_map.h"

using  namespace std;

int main(int argc, char* argv[]) {
    cout << "Testing I2C OLED " << endl;
    
    // Declare I2C OLED
    i2c_oled oled("/dev/i2c-0");
    oled.init_axi_mmap_ptr(AXI_I2C0_RMAPSIZE, AXI_I2C0_BASEADDR, 1);
    
    oled.set_address(SSD1306_ADDRESS);
    
    // oled.i2c_read(4);
    
    // oled.send_command(SSD1306_DISPLAYON);
    // oled.i2c_read(4);
    // oled.send_command(SSD1306_INVERTDISPLAY);
    // oled.i2c_read(4);
    
    // for (int i=0; i<4; i++) {
        // delay_ms(500);
    // }
    
    // oled.send_command(SSD1306_DISPLAYOFF);
    // oled.i2c_read(4);
    
    cout << "-----------------------" << endl;
    
    oled.i2c_read(4);
    oled.init();
    oled.i2c_read(4);
    oled.clear();
    oled.i2c_read(4);
    
    oled.send_command(0xA6);
    
    for (int i=0; i<4; i++) {
        delay_ms(500);
    }
    
    oled.send_command(0xA7);
    
    for (int i=0; i<4; i++) {
        delay_ms(500);
    }
}