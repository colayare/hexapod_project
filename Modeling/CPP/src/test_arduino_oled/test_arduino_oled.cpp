#include <iostream>
#include <iomanip>
#include <string.h>
#include <cstring>
#include "ip_context/i2c_ip_context.h"
#include "globals/linux_timing.h"
#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"
#include "params/ip_reg_map.h"

#define ARDUINO_I2C_ADDR 0x04
#define print(x, y) cout << x << hex << setw(4) << y << endl;
#define MIN_TH 64
#define MAX_TH 192

using namespace std;

void disp_dot(i2c_oled oled, uint8_t line, uint8_t cursor) {
  char str[20];
  strncpy(str, "O", sizeof(str));
  oled.set_cursor(line, cursor);
  oled.disp_str((uint8_t *) &str);
}

void clear_line(i2c_oled oled, uint8_t line, uint8_t cursor) {
  oled.set_cursor(line, cursor);
  
  for (int i=0; i<C_FONT_SIZE; i++) {
    oled.send_data(0x00);
    oled.set_cursor(line, ++cursor);
  }
}

int main(void) {
  cout << "Testing Zynq7000 - Arduino Nano I2C conectivity" << endl;
  cout.fill('0');

  // Declare I2C Generic for Arduino
  i2c_ip_context arduino("/dev/i2c-0");
  arduino.set_address(ARDUINO_I2C_ADDR);

	// Declare I2C OLED
	i2c_oled oled("/dev/i2c-0");
	oled.set_address(SSD1306_ADDRESS);
    
	oled.init();
	char str[20];
	/*
	oled.set_cursor(0, 0);
	strncpy(str, "Testing!", sizeof(str));
	oled.disp_str((uint8_t *) &str);
	*/
	disp_dot(oled, 0, 120);

  uint32_t control_x;
  uint32_t control_y;
  uint8_t line;
  uint8_t cursor;
	uint8_t last_line;
	uint8_t last_cursor;

	while(1) {
		control_x = arduino.i2c_read_word_data(0x01);
		print("Val X: ", control_x);
		control_y = arduino.i2c_read_word_data(0x02);
		print("Val Y: ", control_y);
		
		//oled.clear();
		if ( control_x <= MIN_TH && control_y <= MIN_TH ) {
			line =  0;
			cursor = 0;
		} else if ( control_x > MIN_TH && control_x < MAX_TH && control_y <= MIN_TH ) {
			line =  0;
			cursor = 60;
		} else if ( control_x >= MAX_TH && control_y <= MIN_TH ) {
			line =  0;
			cursor = 120;
		} else if ( control_x <= MIN_TH && control_y > MIN_TH && control_y < MAX_TH ) {
      line = 4;
      cursor = 0;
    } else if ( control_x > MAX_TH && control_y > MIN_TH && control_y < MAX_TH) {
      line = 4;
      cursor = 120;
    } else if ( control_x < MIN_TH && control_y > MAX_TH ) {
      line = 7;
      cursor = 0;
    } else if ( control_x > MIN_TH && control_x < MAX_TH && control_y > MAX_TH ) {
      line = 7;
      cursor = 60;
    } else if ( control_x > MAX_TH && control_y > MAX_TH ) {
      line = 7;
      cursor = 120;
    } else {
      line = 4;
      cursor = 60;
    }
    if ( last_line != line || last_cursor != cursor )
      clear_line(oled, last_line, last_cursor);
		disp_dot(oled, line, cursor);
    last_line = line;
    last_cursor = cursor;

		delay_ms(10);
	}
  

  return 0;
}

