#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"

void i2c_oled::send_command(char command) {
    char buffer[2];
    buffer[0] = SSD1306_COMMAND;
    buffer[1] = command;
    this->i2c_write(buffer);
}

void i2c_oled::init() {
    this->send_command(SSD1306_DISPLAY_OFF);
    this->send_command(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
    this->send_command(0x80);
    this->send_command(SSD1306_SET_MULTIPLEX_RATIO);
    this->send_command(0x3F);
    this->send_command(SSD1306_SET_DISPLAY_OFFSET);
    this->send_command(0x0);
    this->send_command(SSD1306_SET_START_LINE | 0x0);
    this->send_command(SSD1306_CHARGE_PUMP);
    this->send_command(0x14);
    this->send_command(SSD1306_MEMORY_ADDR_MODE);
    this->send_command(0x00);
    this->send_command(SSD1306_SET_SEGMENT_REMAP | 0x1);
    this->send_command(SSD1306_COM_SCAN_DIR_DEC);
    this->send_command(SSD1306_SET_COM_PINS);
    this->send_command(0x12);
    this->send_command(SSD1306_SET_CONTRAST_CONTROL);
    this->send_command(0xCF);
    this->send_command(SSD1306_SET_PRECHARGE_PERIOD);
    this->send_command(0xF1);
    this->send_command(SSD1306_SET_VCOM_DESELECT);
    this->send_command(0x40);
    this->send_command(SSD1306_DISPLAY_ALL_ON_RESUME);
    this->send_command(SSD1306_NORMAL_DISPLAY);
    this->send_command(SSD1306_DISPLAY_ON);

    //OLED_Clear();  /* Clear the complete LCD during init */
}

void i2c_oled::clear() {
    int i;

    this->send_command(SSD1306_SET_COLUMN_ADDR);
    this->send_command(0);
    this->send_command(127);

    this->send_command(SSD1306_SET_PAGE_ADDR);
    this->send_command(0);
    this->send_command(7);


    // oledSendStart(SSD1306_ADDRESS<<1);
    // oledSendByte(SSD1306_DATA_CONTINUE);
    this->i2c_write_byte(SSD1306_DATA_CONTINUE);
    
    char buffer[1];
    buffer[0] = 0;

    for (i=0; i<1024; i++)      // Write Zeros to clear the display
    {
        // oledSendByte(0);
        // this->i2c_write(buffer);
        this->i2c_write_byte(0);
    }

    this->send_command(SSD1306_SET_COLUMN_ADDR);
    this->send_command(0);
    this->send_command(127);

    this->send_command(SSD1306_SET_PAGE_ADDR);
    this->send_command(0);
    this->send_command(7);	  

    // oledSendStart(SSD1306_ADDRESS<<1);
    // oledSendByte(SSD1306_DATA_CONTINUE);
    this->i2c_write_byte(SSD1306_DATA_CONTINUE);
}