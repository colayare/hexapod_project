#include "i2c_peripherals/i2c_oled.h"
#include "i2c_peripherals/i2c_oled_params.h"

void i2c_oled::send_command(char command) {
    this->i2c_write_byte_data(SSD1306_COMMAND, (uint8_t) command);
}

void i2c_oled::send_data(char data) {
    this->i2c_write_byte_data(SSD1306_DATA, (uint8_t) data);
}

void i2c_oled::set_cursor(uint8_t line, uint8_t cursor) {
    if ((line <= SSD1306_LAST_LINE) && (cursor <= SSD1306_OLED_WIDTH)) {
        this->send_command(SSD1306_SET_COLUMN_ADDR);
        this->send_command(cursor);
        this->send_command(127);
        
        this->send_command(SSD1306_SET_PAGE_ADDR);
        this->send_command(line);
        this->send_command(7);
        
        this->send_command(SSD1306_DATA_CONTINUE);
        
        this->_line = line;
        this->_cursor = cursor;
    }
}

void i2c_oled::goto_next_line() {
    this->_line++;
    this->_line = this->_line & 0x07;
    this->set_cursor(this->_line, 0);
}

void i2c_oled::disp_char(uint8_t character) {
    uint8_t data, i=0;
    
    if (((this->_cursor+C_FONT_SIZE) >= 128) || (character == '\n')) {
        this->goto_next_line();
    }
    
    if ( character != '\n' ) {
        character = character - 0x20;
        
        while(1) {
            data = C_FONT_TABLE[character][i];
            
            this->send_data(data);
            this->_cursor++;
            i++;
            if ( i == C_FONT_SIZE ) {
                this->i2c_write_byte(0x00);
                this->_cursor++;
                break;
            }
        }
    }
    
}

void i2c_oled::disp_str(uint8_t *ptr) {
    while(*ptr) {
        this->disp_char(*ptr++);
    }
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

    this->clear();  /* Clear the complete LCD during init */
}

void i2c_oled::clear() {
    int i;

    this->send_command(SSD1306_SET_COLUMN_ADDR);
    this->send_command(0);
    this->send_command(127);

    this->send_command(SSD1306_SET_PAGE_ADDR);
    this->send_command(0);
    this->send_command(7);

    this->send_command(SSD1306_DATA_CONTINUE);
    
    for (i=0; i<1024; i++)      // Write Zeros to clear the display
    {
        this->send_data(0);
    }

    this->send_command(SSD1306_SET_COLUMN_ADDR);
    this->send_command(0);
    this->send_command(127);

    this->send_command(SSD1306_SET_PAGE_ADDR);
    this->send_command(0);
    this->send_command(7);	  

    this->send_command(SSD1306_DATA_CONTINUE);
}