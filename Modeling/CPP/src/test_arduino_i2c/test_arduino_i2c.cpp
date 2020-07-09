#include <iostream>
#include <string.h>
#include <cstring>
#include "ip_context/i2c_ip_context.h"
#include "globals/linux_timing.h"

#define ARDUINO_I2C_ADDR 0x04

using namespace std;

int main(void) {
  cout << "Testing Zynq7000 - Arduino Nano I2C conectivity" << endl;

  // Declare I2C Generic
  i2c_ip_context arduino("/dev/i2c-0");
  arduino.set_address(ARDUINO_I2C_ADDR);

  // Test
  arduino.i2c_write_byte(0x0A);
  arduino.i2c_write_byte(0x55);

  return 0;
}