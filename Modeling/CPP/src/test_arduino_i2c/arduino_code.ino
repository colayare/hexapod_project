// THIS CODE GOES TO ARDUINO
#include <Wire.h>

//
#define REG_MAP_SIZE 2
#define ANALOG_SAMPLE_SIZE 2
#define MAX_SENT_BYTES 3

// Global Vars
int analogPinX = A0;
int analogPinY = A1;
byte registerMap[REG_MAP_SIZE];
byte registerMapTemp[REG_MAP_SIZE-1];
byte receivedCommands[MAX_SENT_BYTES];
int control_x, control_y;
int i2c_command, i2c_data;

void set_regMap(int data, int byteOffset, int byteSize) {
  int temp;
  for (int i=0; i<byteSize; i++) {
    temp = data >> (8*i);
    registerMap[i+byteOffset] = *(byte *) &temp;
  }
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
  Serial.print("--- Init --\n\r");

}

void printVal(String info, int val) {
  Serial.print(info);
  Serial.print(val, DEC);
  Serial.print("\n\r");
}

void loop() {
  // put your main code here, to run repeatedly:
  int readByte = 0;
  // Constantly print X&Y
  control_x = analogRead(analogPinX)/4;
  control_y = analogRead(analogPinY)/4;
  //printVal("X : ", control_x);
  //printVal("Y : ", control_y);
  
  if (Serial.available()) {
    readByte = Serial.read();
    Serial.print("Read: ");
    Serial.print(readByte, HEX);
    Serial.print("\n\r");
  }
  //control_x = analogRead(analogPinX);
  //Serial.println(control_x);

  delay(10);
}

void receiveEvent(int howMany) {
  Serial.print("I2C Receive Event.\n\r");
  while(1 < Wire.available()) { // Receive first byte
    int command = Wire.read();
    Serial.print("A\n\r");
    Serial.println(command);
  }
  i2c_command = Wire.read();          // Receive last byte
  printVal("Command : ", i2c_command);
}

void requestEvent() {           // i2c Response
  //Wire.write(0xAA);
  control_x = analogRead(analogPinX)/4;
  control_y = analogRead(analogPinY)/4;

  switch (i2c_command) {
    case 1:
        printVal("Send X: ", control_x);
        set_regMap(control_x, 0, ANALOG_SAMPLE_SIZE);
        break;
    case 2:
        printVal("Send Y: ", control_y);
        set_regMap(control_y, 0, ANALOG_SAMPLE_SIZE);
        break;
    default:
        printVal("Unknown command received : ", i2c_command);
        break;
  }
  
  Wire.write(registerMap, REG_MAP_SIZE);
}
