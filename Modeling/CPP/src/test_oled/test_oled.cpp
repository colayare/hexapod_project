#include <string.h>
#include <cstring>
#include "globals/linux_timing.h"
#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"
#include "params/ip_reg_map.h"

using  namespace std;

int main(int argc, char* argv[]) {
    cout << "Testing I2C OLED " << endl;
    
    // Declare I2C OLED
    i2c_oled oled("/dev/i2c-0");
    oled.set_address(SSD1306_ADDRESS);
    
    oled.init();
    
    // oled.set_cursor(0,0);
    // oled.send_command(SSD1306_DATA_CONTINUE);
    // for (int i=0; i<1024; i++) {
        // oled.send_data(0xAA);
    // }
    // oled.set_cursor(0,0);
    // oled.send_command(SSD1306_DATA_CONTINUE);
    
    // oled.send_command(SSD1306_INVERT_DISPLAY);
    
    // for (int i=0; i<4; i++) {
        // delay_ms(500);
    // }
    
    // oled.send_command(SSD1306_NORMAL_DISPLAY);
    
    // for (int i=0; i<4; i++) {
        // delay_ms(500);
    // }
    
    // oled.clear();
    
    char str[20];
    
    
    oled.set_cursor(0, 0);
    strncpy(str, "3", sizeof(str));
    oled.disp_str((uint8_t *) &str);
    
    for (int i=0; i<2; i++) delay_ms(500);
    
    oled.set_cursor(0, 0);
    strncpy(str, "2", sizeof(str));
    oled.disp_str((uint8_t *) &str);
    
    for (int i=0; i<2; i++) delay_ms(500);
    
    oled.set_cursor(0, 0);
    strncpy(str, "1", sizeof(str));
    oled.disp_str((uint8_t *) &str);
    
    for (int i=0; i<2; i++) delay_ms(500);
    
    oled.set_cursor(0, 0);
    strncpy(str, "Testing!", sizeof(str));
    oled.disp_str((uint8_t *) &str);
    
    oled.send_command(SSD1306_DATA_CONTINUE);
}