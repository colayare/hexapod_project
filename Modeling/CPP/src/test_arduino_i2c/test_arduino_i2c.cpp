#include <iostream>
#include <iomanip>
#include <string.h>
#include <cstring>
#include "ip_context/i2c_ip_context.h"
#include "globals/linux_timing.h"

#define ARDUINO_I2C_ADDR 0x04

using namespace std;

#define print(x, y) cout << x << hex << setw(4) << y << endl;

int main(void) {
  cout << "Testing Zynq7000 - Arduino Nano I2C conectivity" << endl;
  cout.fill('0');

  // Declare I2C Generic
  i2c_ip_context arduino("/dev/i2c-0");
  arduino.set_address(ARDUINO_I2C_ADDR);

  // Test
  arduino.i2c_write_byte(0x0A);
  arduino.i2c_write_byte(0x55);

  uint32_t read_val;
  uint8_t command;
  command = 0x01;

	while(1) {
		read_val = arduino.i2c_read_word_data(0x01);
		print("Val X: ", read_val);
		read_val = arduino.i2c_read_word_data(0x02);
		print("Val Y: ", read_val);
		delay_ms(10);
	}
  

  return 0;
}
